#include "threadpool.h"

ThreadPool::ThreadPool(size_t threadNum):
    isClosed_(false),
    threadNum_(threadNum) {
    assert(threadNum_ > 0);
}

ThreadPool::~ThreadPool() {
    isClosed_ = true;
    cv_.notify_all();
    for (int i = 0; i < threadNum_; i++) {
        workers[i].join();
    }
}

void ThreadPool::start() {
    isClosed_ = false;
    for (int i = 0; i < threadNum_; i++) {
        workers.push_back(std::thread(&ThreadPool::workFunc, this));
    }
}

void ThreadPool::addTask(Task&& task) {
    std::unique_lock<std::mutex> ul(mtx_);
    tasks_.emplace(task);
    cv_.notify_one();
}

void ThreadPool::workFunc() {
    while(!isClosed_) {
        Task tmp = takeTask();
        tmp();
    }
}

ThreadPool::Task ThreadPool::takeTask() {
    std::unique_lock<std::mutex> ul(mtx_);
    cv_.wait(ul, [this]() {
        return !tasks_.empty() || isClosed_;
    });
    if (isClosed_) return nullptr;
    Task ret((std::move(tasks_.front())));
    tasks_.pop();
    return std::move(ret);
}