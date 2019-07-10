/**
 * Code based on https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads
 */

#ifndef MOOD_LIGHT_MY_SEMAPHORE_H
#define MOOD_LIGHT_MY_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class mySemaphore
{
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    uint32_t count_ = 0;

public:
    void post();

    void wait();

    void getValue(uint32_t *value);
};

#endif /* !MOOD_LIGHT_MY_SEMAPHORE_H */
