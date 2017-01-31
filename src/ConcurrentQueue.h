/*
 * Copyright (c) 2016 Dariusz Stojaczyk. All Rights Reserved.
 * The following source code is released under an MIT-style license,
 * that can be found in the LICENSE file.
 */

#ifndef VEE_CONCURRENTQUEUE_H
#define VEE_CONCURRENTQUEUE_H

#include <cstddef>
#include <atomic>
#include <mutex>
#include <condition_variable>

/**
 * Semi-lockless single-producer single-consumer queue.
 * push(T&) will always be successfull. This means, that
 * if during a push request a queue is full, the request will wait.
 * 
 * This queue uses preallocated array for storing elements.
 * 
 * @tparam T type of kept elements 
 * @tparam SIZE max size of the queue
 */
template<typename T, size_t SIZE>
class ConcurrentQueue {
public:
    /**
     * The constructor.
     */
    ConcurrentQueue()
        : m_head(0),
          m_tail(0) {
    }

    /**
     * Add given element to the queue.
     * If queue is full this request will wait.
     * @param element element to be added
     */
    void push(T element) {
        auto currentTail = m_tail.load();
        auto newTail = next(currentTail);
        m_full = newTail == m_head.load();

        std::unique_lock<std::mutex> lk(m_pushMutex);
        m_fullCondVar.wait(lk, [this] { return !m_full; });

        m_data[currentTail] = std::move(element);
        m_tail.store(newTail);
    }

    /**
     * Get the element from the queue.
     * If queue is empty, it will throw std::underflow_error
     * @return element from the queue
     * @throws std::underflow_error when queue is empty
     */
    T pop() {
        auto currentHead = m_head.load();
        if (currentHead == m_tail.load()) {
            throw std::underflow_error("Trying to pop from empty queue");
        }

        T ret = std::move(m_data[currentHead]);
        m_head.store(next(currentHead));

        if (m_full) {
            m_full = false;
            m_fullCondVar.notify_one();
        }

        return ret;
    }

    /**
     * Whether or not the queue is empty
     * @return whether or not the queue is empty
     */
    bool empty() const {
        return m_head.load() == m_tail.load();
    }

private:
    size_t next(size_t num) {
        return ++num % SIZE;
    }

private:
    std::atomic<size_t> m_head, m_tail;
    T m_data[SIZE];

    std::mutex m_pushMutex;
    std::condition_variable m_fullCondVar;
    bool m_full = false;
};

#endif //VEE_CONCURRENTQUEUE_H
