//
// Created by fpeterek on 22.01.22.
//

#ifndef VSBPGCONTEST21_BLOCKING_QUEUE_HPP
#define VSBPGCONTEST21_BLOCKING_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <atomic>


template<typename T>
class BlockingQueue {

    std::queue<T> queue;
    std::condition_variable cv;
    std::mutex mutex;
    std::atomic<std::size_t> atomicSize { 0 };

public:

    bool empty() {
        return not atomicSize.load();
    }

    bool nonEmpty() {
        return atomicSize.load();
    }

private:

    std::function<bool(void)> nonEmptyLambda = [this]() -> bool { return nonEmpty(); };

public:

    size_t size() {
        return atomicSize.load();
    }

    void push(T && t) {
        {
            // std::cout << "Obtaining lock" << std::endl;
            std::lock_guard lock(mutex);
            // std::cout << "Pushing" << std::endl;
            queue.push(std::move(t));
            ++atomicSize;
        }
        cv.notify_all();
    }

    T pop() {
        if constexpr (false) {
            std::lock_guard lock(mutex);
            if (nonEmpty()) {
                T t = std::move(queue.front());
                std::cout << "Popping immediately" << std::endl;
                queue.pop();
                --atomicSize;
                return t;
            }
        }
        std::unique_lock lock(mutex);
        // std::cout << "Waiting" << std::endl;
        cv.wait(lock, nonEmptyLambda);
        // std::cout << "Finally popping" << std::endl;
        T t = std::move(queue.front());
        queue.pop();
        --atomicSize;
        return t;
    }

};

#endif //VSBPGCONTEST21_BLOCKING_QUEUE_HPP
