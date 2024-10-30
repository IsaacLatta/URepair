#include "ThreadPool.h"

ThreadPool::ThreadPool(const size_t n_threads)
{
    _stop = false;
    addThreads(n_threads);
}

ThreadPool::ThreadPool()
{
    _stop = false;
    addThreads(std::thread::hardware_concurrency());
}

bool ThreadPool::busy()
{
    std::lock_guard<std::mutex> lock(_tasks_lock);
    return !_tasks.empty();
}

size_t ThreadPool::queueSize()
{
    std::lock_guard<std::mutex> lock(_tasks_lock);
    return _tasks.size();
}

void ThreadPool::addThreads(const size_t n_threads)
{
    std::lock_guard<std::mutex> lock(_threads_lock);
    for(int i =0; i < n_threads; i++)
    {
        _threads.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard <std::mutex> lock(_tasks_lock);
        _stop = true;
    }
    _tasks_cv.notify_all();

    std::lock_guard<std::mutex> lock(_threads_lock);
    for(auto& thread : _threads)
    {
        thread.join();
    }
}

void ThreadPool::worker()
{
    while(!_stop)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(_tasks_lock);
            _tasks_cv.wait(lock, [this] {return !_tasks.empty() || _stop; });

            if(_stop && _tasks.empty()) return;

            task = std::move(_tasks.front());
            _tasks.pop();
        }
        task();
    }
}

