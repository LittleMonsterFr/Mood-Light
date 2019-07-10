#ifndef MOOD_LIGHT_RING_BUFFER_H
#define MOOD_LIGHT_RING_BUFFER_H

#include <cstdlib>
#include <iostream>
#include <vector>

#include "mySemaphore.h"

class RingBuffer
{

public:
    RingBuffer(size_t bufferSize, size_t periodSize)
            : buffer_(bufferSize), periodSize_(periodSize),
              writePos_(0), readPos_(0), sem_()
    {
        overwriteList_ = std::vector<uint32_t>(bufferSize / periodSize);
        MY_LOGGER_LOG("RingBuffer :\n");
        MY_LOGGER_LOG("- Buffer size of : %d\n", bufferSize);
        MY_LOGGER_LOG("- Period size of : %d\n", periodSize);
        MY_LOGGER_LOG("- Overwrite list capacity : %d\n\n", overwriteList_.size());
    };

    void put(std::vector<int16_t> &data);

    void get(std::vector<int16_t> &res);

    size_t getPeriodSize() const;

    void dumpOverwriteList();

private:
    std::vector<int16_t> buffer_;
    size_t periodSize_;

    std::vector<uint32_t> overwriteList_;

    uint32_t writePos_;
    uint32_t readPos_;

    mySemaphore sem_;
};

#endif /* !MOOD_LIGHT_RING_BUFFER_H */
