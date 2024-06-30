
#include <unordered_map>
#include <functional>
#include <sys/epoll.h>
#include <thread>

typedef void* (*reactorFunc)(int fd);

class Reactor {
public:
    Reactor();
    ~Reactor();

    // Starts the reactor and returns a pointer to it
    static void* startReactor();

    // Adds fd to Reactor (for reading); returns 0 on success
    int addFdToReactor(int fd, reactorFunc func);

    // Removes fd from Reactor; returns 0 on success
    int removeFdFromReactor(int fd);

    // Stops the reactor; returns 0 on success
    int stopReactor();

private:
    // Reactor event loop
    void run();

    // Epoll file descriptor
    int epollFd;
    
    // Flag to control the reactor loop
    bool running;

    // Map to store the file descriptors and their associated functions
    std::unordered_map<int, reactorFunc> callbacks;

    // Thread for running the reactor
    std::thread reactorThread;
};
