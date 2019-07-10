#include "mySemaphore.h"

void mySemaphore::post()
{
    std::unique_lock<std::mutex> lock(mutex_);
    ++count_;
    condition_.notify_one();
}

void mySemaphore::wait()
{
    std::unique_lock<std::mutex> lock(mutex_);
    while(!count_) // Handle spurious wake-ups.
        condition_.wait(lock);
    --count_;
}

void mySemaphore::getValue(uint32_t *value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    *value = count_;
}


