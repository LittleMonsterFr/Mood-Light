#include <cstdlib>
#include <iostream>

#include "myLogger.h"
#include "ringBuffer.h"

void RingBuffer::put(std::vector<int16_t> &data)
{
    if (data.size() != periodSize_)
    {
        MY_LOGGER_LOG("Vector size must be the same as the period sze\n");
        MY_LOGGER_LOG("Vector size is %d\n", data.size());
        MY_LOGGER_LOG("Period size is %d\n", periodSize_);
        sem_.post();
        return;
    }

    bool overwrite = false;
    uint32_t count = 0;
    sem_.getValue(&count);

    if (count >= overwriteList_.size())
    {
        MY_LOGGER_LOG("The reader is in advance of %d periods\n", count);
        MY_LOGGER_LOG("The current read values will be discarded\n");
        overwrite = true;
    }

    if (overwrite)
        return;

    for (size_t i = 0; i < periodSize_; i++)
    {
        buffer_[(writePos_ + i) % buffer_.size()] = data[i];
    }

    writePos_ += periodSize_;

    sem_.post();
}

void RingBuffer::get(std::vector<int16_t> &res)
{
    if (res.size() != periodSize_)
    {
        MY_LOGGER_LOG("Vector size must be the same as the period size\n");
        MY_LOGGER_LOG("Vector size is %d\n", res.size());
        MY_LOGGER_LOG("Period size is %d\n", periodSize_);
        return;
    }

    sem_.wait();

    for (size_t i = 0; i < periodSize_; i++)
    {
        res[i] = buffer_[(readPos_ + i) % buffer_.size()];
    }

    readPos_ += periodSize_;

    uint32_t count = 0;
    sem_.getValue(&count);

    if (count >= overwriteList_.capacity())
    {
        MY_LOGGER_LOG("Writer overwritten : %d\n", count);
    }
    else
    {
        overwriteList_[count]++;
    }
}

size_t RingBuffer::getPeriodSize() const
{
    return periodSize_;
}

void RingBuffer::dumpOverwriteList()
{
    for (uint32_t val : overwriteList_)
    {
        MY_LOGGER_LOG("%u\n", val);
    }
}
