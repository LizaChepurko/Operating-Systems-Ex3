#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>
#include <sys/wait.h>
#include "Graph.hpp"

#define PORT "9034"
#define MAXDATASIZE 256

Graph &graph = Graph::getInstance();


// Get sockaddr, IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void)
{
    int listener;
    int yes = 1;
    int rv;

    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0)
    {
        std::cerr << "pollserver: " << gai_strerror(rv) << std::endl;
        return -1;
    }

    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
        {
            continue;
        }

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai);

    if (p == NULL)
    {
        return -1;
    }

    if (listen(listener, 10) == -1)
    {
        return -1;
    }

    return listener;
}

// Add a new file descriptor to the set
void add_to_pfds(struct pollfd *pfds[], int newfd, int *fd_count, int *fd_size)
{
    if (*fd_count == *fd_size)
    {
        *fd_size *= 2;
        *pfds = (struct pollfd *)realloc(*pfds, sizeof(**pfds) * (*fd_size));
    }

    (*pfds)[*fd_count].fd = newfd;
    (*pfds)[*fd_count].events = POLLIN;

    (*fd_count)++;
}

// Remove an index from the set
void del_from_pfds(struct pollfd pfds[], int i, int *fd_count)
{
    pfds[i] = pfds[*fd_count - 1];
    (*fd_count)--;
}

// Client command handler
void handle_client_command(int client_fd) {
    char buf[MAXDATASIZE];
    int nbytes = recv(client_fd, buf, sizeof(buf) - 1, 0);

    if (nbytes <= 0) {
        if (nbytes == 0) {
            std::cout << "server: socket " << client_fd << " hung up" << std::endl;
        } else {
            perror("recv");
        }
        close(client_fd);
        return;
    }

    buf[nbytes] = '\0'; // Null-terminate the string
    std::string cmd(buf);

    std::cout << "Received command: " << cmd << std::endl; // Debug statement

    std::istringstream iss(cmd);
    std::string action;
    int u, v;
    std::ostringstream response;

    iss >> action;
    if (action == "new") {
        std::string sub_action;
        iss >> sub_action;

        if (sub_action == "graph") {
            iss >> u >> v;
            graph.Newgraph(u, v);
            response << "Graph created.\n";
        } else {
            response << "Invalid sub-command for 'new'.\n";
        }
    } else if (action == "add") {
        iss >> u >> v;
        response << "Edge added.\n" << graph.Newedge(u, v) << "\n";
    } else if (action == "remove") {
        iss >> u >> v;
        graph.Removeedge(u, v);
        response << "Edge removed.\n";
    } else if (action == "Kosaraju") {
        try {
            response << "Kosaraju done.\n" << graph.Kosaraju() << "\n";
        } catch (const std::exception &e) {
            response << "Kosaraju failed: " << e.what() << "\n";
        }
    } else {
        response << "Invalid command.\n";
    }

    send(client_fd, response.str().c_str(), response.str().length(), 0);
}

int main(void)
{
    int listener;
    int newfd;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;
    char remoteIP[INET6_ADDRSTRLEN];
    int fd_count = 0;
    int fd_size = 5;
    struct pollfd *pfds = (struct pollfd *)malloc(sizeof(*pfds) * fd_size);

    listener = get_listener_socket();

    if (listener == -1)
    {
        std::cerr << "error getting listening socket" << std::endl;
        return 1;
    }

    pfds[0].fd = listener;
    pfds[0].events = POLLIN;

    fd_count = 1;

    for (;;)
    {
        int poll_count = poll(pfds, fd_count, -1);

        if (poll_count == -1)
        {
            perror("poll");
            exit(1);
        }

        for (int i = 0; i < fd_count; i++)
        {
            if (pfds[i].revents & POLLIN)
            {
                if (pfds[i].fd == listener)
                {
                    addrlen = sizeof(remoteaddr);
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

                    if (newfd == -1)
                    {
                        perror("accept");
                    }
                    else
                    {
                        add_to_pfds(&pfds, newfd, &fd_count, &fd_size);

                        std::cout << "pollserver: new connection from "
                                  << inet_ntop(remoteaddr.ss_family,
                                               get_in_addr((struct sockaddr *)&remoteaddr),
                                               remoteIP, INET6_ADDRSTRLEN)
                                  << " on socket " << newfd << std::endl;
                    }
                }
                else
                {
                    handle_client_command(pfds[i].fd);
                }
            }
        }
    }

    free(pfds);
    return 0;
}
