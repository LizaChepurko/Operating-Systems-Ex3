#include "libactor.hpp"
#include "Graph.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sstream>
#include <mutex>
#include <thread>

const int PORT = 9034;
Proactor proactor;
#define MAXDATASIZE 256

Graph &graph = Graph::getInstance();
std::mutex graph_mutex;

void* handle_client(int client_socket) {
    char buf[MAXDATASIZE];

    while (true) {
        int nbytes = recv(client_socket, buf, sizeof(buf) - 1, 0);

        if (nbytes <= 0) {
            if (nbytes == 0) {
                std::cout << "Thread " << std::this_thread::get_id() << ": Server: socket " << client_socket << " hung up" << std::endl;
            } else {
                perror("recv");
            }
            close(client_socket);
            return nullptr;
        }

        buf[nbytes] = '\0'; // Null-terminate the string
        std::string cmd(buf);

        std::cout << "Thread " << std::this_thread::get_id() << ": Received command: " << cmd << std::endl;

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

        send(client_socket, response.str().c_str(), response.str().length(), 0);
    }

    return nullptr;
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (client_socket < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Start a new thread to handle the client
        std::cout << "Proactor is starting for client socket " << client_socket << " on main thread " << std::this_thread::get_id() << std::endl;

        std::thread([](int sock) {
            handle_client(sock);
            std::cout << "Thread " << std::this_thread::get_id() << ": Finished handling client socket " << sock << std::endl;
        }, client_socket).detach();
    }

    close(server_fd);
    return 0;
}
