#include "threading.hpp"
#include <future>
#include <iostream>
#include <mutex>

void thread_pool::run() noexcept
{
    thread_local std::packaged_task<void()> job;
    while (true) {
        {
            std::unique_lock<std::mutex> lk(m_mtx);
            m_cv.wait(lk, [&]() -> bool {
                return !m_tasks.empty() || !m_is_active;
            });
            if (!m_is_active) {
                break;
            }
            job.swap(m_tasks.front());
            m_tasks.pop_front();
        }

        job();
    }
}

void thread_pool::post(std::packaged_task<void()> job)
{
    std::unique_lock<std::mutex> lk(m_mtx);
    m_tasks.emplace_back(std::move(job));
}
