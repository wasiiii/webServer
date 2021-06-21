#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>  // epoll_ctl()
#include <fcntl.h>      // fcntl()
#include <unistd.h>     // close()
#include <assert.h>     // close()
#include <vector>
#include <errno.h>

class Epoller {
public:
    explicit Epoller(int maxEvent = 1024);

    ~Epoller();

    bool addFd(int fd, uint32_t events);

    bool modFd(int fd, uint32_t events);

    bool delFd(int fd);

    // 默认-1，永远阻塞，直到有事件发生
    int wait(int timeoutMs = -1);

    int getEventFd(size_t index) const;

    uint32_t getEvents(size_t index) const;

private:
    int epollFd_;

    std::vector<struct epoll_event> events_;
};

#endif