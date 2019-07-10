#include <cassert>
#include <thread>
#include <csignal>
#include <cstring>
#include <unistd.h>

#include "everloop.h"
#include "helpers.h"
#include "myLogger.h"
#include "microphone_array.h"
#include "microphone_core.h"
#include "patternHandler.h"
#include "ringBuffer.h"

#include "patterns/errorPattern.h"
#include "patterns/decodeFailedPattern.h"
#include "patterns/transitionPattern.h"

#include "chirp_connect.h"

// This file contains the Chirp key, secret and config from the Chirp account.
#include "credentials.h"

// Sample rate used by the microphones
#define SAMPLE_RATE 44100

// Default sleep time of 1 second expressed in micro seconds
#define DEFAULT_SLEEP_TIME 1000000

// Default LED intensity : maximum intensity
#define DEFAULT_INTENSITY 1.0f

// ID of the error pattern
#define ERROR_PATTERN_ID    (-1)

// ID of the decode failed pattern
#define DECODE_FAILED_PATTERN_ID    (-2)

// ID of the transition pattern
#define TRANSITION_PATTERN_ID    (-3)

// Length of the stop byte sequence
#define STOP_BYTES_LENGTH   8

// Usage string of the program
#define USAGE "Usage : mood-light <json_pattern_file> [key secret config]."

namespace matrix = matrix_hal;

static bool stop_program = false;
static bool stop_rpi = false;
static chirp_connect_t *connect = nullptr;

// Byte sequence to stop the program
static const uint8_t stop_bytes[STOP_BYTES_LENGTH] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
static PatternHandler *patternHandler = nullptr;

void on_received_callback(void *ptr, uint8_t *data, size_t length, uint8_t channel)
{
    if (data == nullptr || length == 0)
    {
        MY_LOGGER_LOG("Decode failed\n");
        patternHandler->decodeFailed();
        return;
    }

    /*
     * Stop the rpi if we've received the stop byte sequence
     */
    if (length == STOP_BYTES_LENGTH && memcmp(stop_bytes, data, STOP_BYTES_LENGTH) == 0)
    {
        stop_program = true;
        stop_rpi = true;
        return;
    }

    MY_LOGGER_LOG("Length : %d\n", (int) length);
    MY_LOGGER_LOG("Bytes : (");
    for (uint8_t i = 0; i < length - 1; i++)
    {
        MY_LOGGER_PRINT("%d, ",data[i]);
    }
    MY_LOGGER_PRINT("%d)\n\n", data[length - 1]);

    /*
     * A length of 1 means we select the pattern using the latest configuration
     * used for this pattern.
     */
    if (length == 1)
        patternHandler->select(data[0]);
    else if (length == 3)
        patternHandler->update(data[0], data[1], data[2], nullptr, 0);
    else if (length > 3)
        patternHandler->update(data[0], data[1], data[2], (void *)&data[3], length - 3);
    else
        MY_LOGGER_LOG("Invalid data length\n");
}

void reader_thread(matrix::MicrophoneArray& mics, RingBuffer& RingBuffer)
{
    assert(mics.NumberOfSamples() == RingBuffer.getPeriodSize());

    std::vector<int16_t> buf(RingBuffer.getPeriodSize());

    while (!stop_program)
    {
        mics.Read();

        for (uint32_t s = 0; s < mics.NumberOfSamples(); s++)
        {
            buf[s] = mics.Beam((int16_t) s);
        }

        RingBuffer.put(buf);
    }

    MY_LOGGER_LOG("Reader exiting\n");
}

void writer_thread(RingBuffer& RingBuffer)
{
    std::vector<int16_t> buf(RingBuffer.getPeriodSize());
    std::vector<float> float_buf(RingBuffer.getPeriodSize());

    if (chirp_connect_get_input_sample_rate(connect) != SAMPLE_RATE)
    {
        stop_program = true;
        MY_LOGGER_LOG("Microphone sample rate doesn't match connect sample rate.\n");
    }

    chirp_connect_error_code_t error_code;

    while (!stop_program)
    {
        RingBuffer.get(buf);
        float_buf.clear();

        for (int16_t val : buf)
        {
            float_buf.push_back(val / 32767.0f);
        }

        error_code = chirp_connect_process_input(connect, &float_buf[0], RingBuffer.getPeriodSize());
        if (error_code != CHIRP_CONNECT_OK)
        {
            MY_LOGGER_LOG("Error when processing input : %s\n", chirp_connect_error_code_to_string(error_code));
        }
    }

    MY_LOGGER_LOG("Writer exiting\n");
}

void everloop_thread(matrix::Everloop &everloop, matrix::EverloopImage &everloopImage)
{
    Pattern *currentPattern = nullptr;

    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t w = 0;
    float intensity;

    while (!stop_program)
    {
        currentPattern = patternHandler->getCurrentPattern();
        intensity = currentPattern->getIntensity();

        for (matrix::LedValue& led : everloopImage.leds)
        {
            currentPattern->process(&r, &g, &b, &w);

            /*
             * Applying the intensity here allows to create a global control of
             * the intensity. A per LED and / or per RGB value can be selected
             * when processing the pattern.
             *
             * For a full control of the intensity, one can set "intensity" to
             * 1 and process the intensity in the process function.
             */
            led.red = clamp_255(r * intensity);
            led.green = clamp_255(g * intensity);
            led.blue = clamp_255(b * intensity);
            led.white = clamp_255(w * intensity);
        }
        everloop.Write(&everloopImage);

        if (usleep(currentPattern->getSleep()) != 0)
            MY_LOGGER_LOG("Usleep failed\n");
    }

    /*
     * When terminating the program, ensure all the LED are off.
     */
    for (matrix::LedValue& led : everloopImage.leds)
    {
        led.red = 0;
        led.green = 0;
        led.blue = 0;
        led.white = 0;
    }
    everloop.Write(&everloopImage);

    MY_LOGGER_LOG("Everloop exiting\n");
}

void stop(int dummy)
{
    (void)dummy;
    stop_program = true;
}

int main(int argc, char **argv)
{
    MY_LOGGER_LOG("%s\n", std::string(80, '-').c_str());
    MY_LOGGER_LOG("Mood light :\n\n");

    matrix::MatrixIOBus bus;
    if (!bus.Init())
    {
        MY_LOGGER_LOG("Error when initialising MatrixIOBus\n");
        return EXIT_FAILURE;
    }

    if (!bus.IsDirectBus())
        MY_LOGGER_LOG("Using kernel modules\n");

    matrix::MicrophoneArray mics;
    mics.Setup(&bus);
    mics.SetSamplingRate(SAMPLE_RATE);

    matrix::MicrophoneCore mic_core(mics);
    mic_core.Setup(&bus);

    /*
     * We compute the buffer size and the period size instead of using defines
     * as they depends on the matrix implementation
     */
    size_t buffer_size = matrix::kMicarrayBufferSize;
    size_t period_size = matrix::kMicarrayBufferSize / matrix::kMicrophoneChannels;
    RingBuffer RingBuffer(buffer_size, period_size);

    matrix::Everloop everloop;
    everloop.Setup(&bus);
    matrix::EverloopImage everloopImage(bus.MatrixLeds());

    char *pattern_file = nullptr;
    char *key = (char *) CHIRP_APP_KEY;
    char *secret = (char *) CHIRP_APP_SECRET;
    char *config = (char *) CHIRP_APP_CONFIG;

    if (argc <= 1 || argc >= 6)
    {
        MY_LOGGER_LOG(USAGE " Exiting.\n");
        return EXIT_FAILURE;
    }

    if (argc == 2)
    {
        pattern_file = argv[1];
    }
    else if (argc == 5)
    {
        pattern_file = argv[1];
        key = argv[2];
        secret = argv[3];
        config = argv[4];
    }
    else
    {
        MY_LOGGER_LOG(USAGE " Exiting.\n");
        return EXIT_FAILURE;
    }

    connect = new_chirp_connect(key, secret);
    if (connect == nullptr)
    {
        MY_LOGGER_LOG("Connect creation failed\n");
        return EXIT_FAILURE;
    }

    chirp_connect_error_code_t error_code = chirp_connect_set_config(connect, config);
    if (error_code != CHIRP_CONNECT_OK)
    {
        del_chirp_connect(&connect);
        MY_LOGGER_LOG("Error when setting the connect config : %s\n", chirp_connect_error_code_to_string(error_code));
        return EXIT_FAILURE;
    }

    char *info = chirp_connect_get_info(connect);
    MY_LOGGER_LOG("%s - %s\n", info, chirp_connect_get_version());
    chirp_connect_free(info);

    error_code = chirp_connect_set_input_sample_rate(connect, SAMPLE_RATE);
    if (error_code != CHIRP_CONNECT_OK)
    {
        del_chirp_connect(&connect);
        MY_LOGGER_LOG("Error when setting the connect sample rate : %s\n", chirp_connect_error_code_to_string(error_code));
        return EXIT_FAILURE;
    }

    chirp_connect_callback_set_t callback_set;
    callback_set.on_state_changed = nullptr;
    callback_set.on_sending = nullptr;
    callback_set.on_sent = nullptr;
    callback_set.on_receiving = nullptr;
    callback_set.on_received = on_received_callback;

    error_code = chirp_connect_set_callbacks(connect, callback_set);
    if (error_code != CHIRP_CONNECT_OK)
    {
        del_chirp_connect(&connect);
        MY_LOGGER_LOG("Error when setting the connect callbacks : %s\n", chirp_connect_error_code_to_string(error_code));
        return EXIT_FAILURE;
    }

    error_code = chirp_connect_start(connect);
    if (error_code != CHIRP_CONNECT_OK)
    {
        del_chirp_connect(&connect);
        MY_LOGGER_LOG("Error when starting connect : %s\n", chirp_connect_error_code_to_string(error_code));
        return EXIT_FAILURE;
    }

    ErrorPattern error(ERROR_PATTERN_ID, DEFAULT_SLEEP_TIME, DEFAULT_INTENSITY, nullptr);
    DecodeFailedPattern decodeFailed(DECODE_FAILED_PATTERN_ID, DEFAULT_SLEEP_TIME / 2, DEFAULT_INTENSITY, nullptr);
    TransitionPattern transitionPattern(TRANSITION_PATTERN_ID, DEFAULT_SLEEP_TIME / 5, 1.0f, nullptr);

    patternHandler = new PatternHandler(&error, &decodeFailed, &transitionPattern);
    if (!patternHandler->addFromFile(pattern_file))
    {
        MY_LOGGER_LOG("Pattern adding from file failed. Quitting\n");
        delete patternHandler;
        del_chirp_connect(&connect);
        return EXIT_FAILURE;
    }

    patternHandler->select(10);

    signal(SIGINT, stop);

    std::thread writer(writer_thread, std::ref(RingBuffer));
    std::thread reader(reader_thread, std::ref(mics), std::ref(RingBuffer));
    std::thread light(everloop_thread, std::ref(everloop), std::ref(everloopImage));

    writer.join();
    reader.join();
    light.join();

    delete patternHandler;

    error_code = chirp_connect_stop(connect);
    if (error_code != CHIRP_CONNECT_OK)
    {
        del_chirp_connect(&connect);
        MY_LOGGER_LOG("Error when stopping connect : %s\n", chirp_connect_error_code_to_string(error_code));
        return EXIT_FAILURE;
    }
    del_chirp_connect(&connect);

    /*
     * For debugging and statistics purpose.
     */
    RingBuffer.dumpOverwriteList();

    MY_LOGGER_LOG("Main exiting\n");

    if (stop_rpi)
    {
        MY_LOGGER_LOG("Powering off the RPI\n");
        system("sudo poweroff");
    }
}


