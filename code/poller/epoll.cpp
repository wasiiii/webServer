#include "epoll.h"

// epoll_create返回值是内核事件表fd，参数无意义
Epoller::Epoller(int maxEvent):
    epollFd_(epoll_create(0)),
    events_(maxEvent) {
    assert(epollFd_ >= 0 && events_.size() > 0);
}

Epoller::~Epoller() {
    close(epollFd_);
}

bool Epoller::addFd(int fd, uint32_t events) {
    if (fd < 0) return false;

    epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;

    return epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev) == 0;
}

bool Epoller::modFd(int fd, uint32_t events) {
    if (fd < 0) return false;

    epoll_event ev;
    ev.data.fd = fd;
    ev.events = events;

    return epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev) == 0;
}

bool Epoller::delFd(int fd) {
    if (fd < 0) return false;

    epoll_event ev;
    
    return epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &ev) == 0;
}

int Epoller::wait(int timeoutMs) {
    return epoll_wait(epollFd_, &events_[0], static_cast<int>(events_.size()), timeoutMs);
}

int Epoller::getEventFd(size_t index) const {
    assert(index < events_.size() && index >= 0);

    return events_[index].data.fd;
}

uint32_t Epoller::getEvents(size_t index) const {
    assert(index < events_.size() && index >= 0);

    return events_[index].events;
}