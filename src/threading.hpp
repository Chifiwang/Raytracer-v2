#ifndef THREADING_HPP
#define THREADING_HPP

#include <atomic>
#include <condition_variable>
#include <deque>
#include <future>
#include <mutex>
#include <thread>
#define MAX_THREADS 512

class thread_pool {
    std::atomic_bool m_is_active { true };
    std::vector<std::thread> m_pool;
    std::deque<std::packaged_task<void()>> m_tasks;

    std::mutex m_mtx{};
    std::condition_variable m_cv{};

    void run() noexcept;

public:
    explicit thread_pool(int thread_count = MAX_THREADS)
    {
        for (int i = 0; i < std::max(thread_count, MAX_THREADS); ++i) {
            m_pool.emplace_back(&thread_pool::run, this);
            m_pool.back().detach();
        }
    }
    ~thread_pool()
    {
        m_is_active = false;
        m_cv.notify_all();
        // for (auto& th : m_pool) {
        //     th.join();
        // }
    }
    void post(std::packaged_task<void()> job);
};

#endif // !THREADING_HPP
