#include "Reactor.hpp"
#include <unistd.h>
#include <iostream>

Reactor::Reactor() : running(false)
{
    epollFd = epoll_create1(0);
    if (epollFd == -1)
    {
        perror("epoll_create1");
        throw std::runtime_error("Failed to create epoll file descriptor");
    }
}

Reactor::~Reactor()
{
    close(epollFd);
}

void *Reactor::startReactor()
{
    Reactor *reactor = new Reactor();
    reactor->running = true;
    reactor->reactorThread = std::thread(&Reactor::run, reactor);
    return static_cast<void *>(reactor);
}

int Reactor::addFdToReactor(int fd, reactorFunc func)
{
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event) == -1)
    {
        perror("epoll_ctl: add");
        return -1;
    }
    callbacks[fd] = func;
    return 0;
}

int Reactor::removeFdFromReactor(int fd)
{
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr) == -1)
    {
        perror("epoll_ctl: remove");
        return -1;
    }
    callbacks.erase(fd);
    return 0;
}

int Reactor::stopReactor()
{
    running = false;
    if (reactorThread.joinable())
    {
        reactorThread.join();
    }
    return 0;
}

void Reactor::run()
{
    constexpr int MAX_EVENTS = 10;
    struct epoll_event events[MAX_EVENTS];

    while (running)
    {
        int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            break;
        }

        for (int n = 0; n < nfds; ++n)
        {
            int fd = events[n].data.fd;
            if (callbacks.find(fd) != callbacks.end())
            {
                callbacks[fd](fd);
            }
        }
    }
}
