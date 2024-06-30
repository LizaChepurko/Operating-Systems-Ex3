#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <sys/types.h>
#include <mutex>
#include <sys/socket.h>
#include <netdb.h>
#include "Reactor.hpp"
#include "Graph.hpp"

#define PORT "9034"
#define MAXDATASIZE 256

// The singleton instance of the Graph
Graph &graph = Graph::getInstance();
std::mutex graph_mutex;

// Utility function for getting the sockaddr (both IPv4 and IPv6)
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

// Utility function for getting a listening socket
int get_listener_socket(void) {
    int listener;
    int yes = 1;
    int rv;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        std::cerr << "server: " << gai_strerror(rv) << std::endl;
        return -1;
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai);

    if (p == NULL) {
        return -1;
    }

    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}

// Client command handler
void *handle_client_command(int client_fd) {
    char buf[MAXDATASIZE];
    int nbytes = recv(client_fd, buf, sizeof(buf) - 1, 0);

    if (nbytes <= 0) {
        if (nbytes == 0) {
            std::cout << "server: socket " << client_fd << " hung up" << std::endl;
        } else {
            perror("recv");
        }
        close(client_fd);
        return nullptr;
    }

    buf[nbytes] = '\0'; // Null-terminate the string
    std::string cmd(buf);

    std::cout << "Received command: " << cmd << std::endl; // Debug statement

    std::lock_guard<std::mutex> lock(graph_mutex);

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

    return nullptr;
}

// Connection accept handler
void* accept_client(int listener) {
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen = sizeof(remoteaddr);
    int newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

    if (newfd == -1) {
        perror("accept");
        
    }

    char remoteIP[INET6_ADDRSTRLEN];
    std::cout << "server: new connection from "
              << inet_ntop(remoteaddr.ss_family,
                           get_in_addr((struct sockaddr *)&remoteaddr),
                           remoteIP, INET6_ADDRSTRLEN)
              << " on socket " << newfd << std::endl;

    Reactor *reactor = static_cast<Reactor *>(Reactor::startReactor());
    reactor->addFdToReactor(newfd, handle_client_command);
    return nullptr;
}

// Main function
int main(void) {
    int listener = get_listener_socket();

    if (listener == -1) {
        std::cerr << "error getting listening socket" << std::endl;
        return 1;
    }
    std::cout << "Reactor is starting for client socket " << listener << std::endl;
    Reactor *reactor = static_cast<Reactor *>(Reactor::startReactor());
    reactor->addFdToReactor(listener, accept_client);

    while (true) {
        sleep(1);
    }

    reactor->stopReactor();
    return 0;
}
