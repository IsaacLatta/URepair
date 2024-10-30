#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <future>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool
{
public:
    ThreadPool();
    ThreadPool(const size_t thread_count);
    ~ThreadPool();

    void addThreads(const size_t thread_count);

    bool busy();

    size_t queueSize();

    template<typename Func, typename... Args>
    auto addTask_callback(std::function<void()>&& callback, Func&& func, Args&&... args)
    {
        using return_type = decltype(func(std::forward<Args>(args)...)); // Get the return type of the function
        auto local_callback = std::make_shared<std::function<void()>>(std::move(callback));
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        std::future<return_type> result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(_tasks_lock);
            _tasks.emplace([task, local_callback]() { (*task)(); (*local_callback)(); }); // Wrap task in lambda
        }
        _tasks_cv.notify_one();
        return result;
    }

    template<typename Func, typename... Args>
    auto addTask(Func&& func, Args&&... args)
    {
        using return_type = decltype(func(std::forward<Args>(args)...)); // Get the return type of the function
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        std::future<return_type> result = task->get_future();
        {
            std::lock_guard<std::mutex> lock(_tasks_lock);
            _tasks.emplace([task]() { (*task)(); }); // Wrap task in lambda
        }
        _tasks_cv.notify_one();
        return result;
    }

private:
    bool _stop;
    std::mutex _tasks_lock, _threads_lock;
    std::condition_variable _tasks_cv;
    std::queue<std::function<void()>> _tasks;
    std::vector<std::thread> _threads;

    void worker();
};

#endif