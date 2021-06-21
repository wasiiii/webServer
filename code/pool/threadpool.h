#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>
#include <assert.h>

class ThreadPool {
public:
	typedef std::function<void()> Task;

    explicit ThreadPool(size_t threadNum = 4);

    ~ThreadPool();

    void start();

    void addTask(Task&& task);

private:
    std::mutex mtx_;

    std::condition_variable cv_;
    
    bool isClosed_;

    std::queue<Task> tasks_;

    size_t threadNum_;

    std::vector<std::thread> workers;

    void workFunc();

    Task takeTask();
};

#endif 