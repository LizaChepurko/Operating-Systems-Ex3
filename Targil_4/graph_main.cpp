#include "Graph.hpp"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Invalid command" << endl;
        return 1;
    }

    Graph &graph = Graph::getInstance();
    string action;
    int u, v;

    // Combine all arguments into a single command string
    string cmd;
    for (int i = 1; i < argc; ++i) {
        cmd += string(argv[i]) + " ";
    }

    istringstream iss(cmd);
    iss >> action;
    if (action == "new") {
        string sub_action;
        iss >> sub_action;

        if (sub_action == "graph") {
            iss >> u >> v;
            graph.Newgraph(u, v);
            cout << "Graph created." << endl;
        } else {
            cout << "Invalid sub-command for 'new'." << endl;
        }
    } else if (action == "add") {
        iss >> u >> v;
        cout << "Edge added.\n" << graph.Newedge(u, v) << endl;
    } else if (action == "remove") {
        iss >> u >> v;
        graph.Removeedge(u, v);
        cout << "Edge removed." << endl;
    } else if (action == "Kosaraju") {
        try {
            cout << "Kosaraju done.\n" << graph.Kosaraju() << endl;
        } catch (const std::bad_alloc &e) {
            cerr << "Kosaraju memory allocation error: " << e.what() << endl;
            cout << "Kosaraju failed: memory allocation error." << endl;
        } catch (const std::exception &e) {
            cerr << "Kosaraju error: " << e.what() << endl;
            cout << "Kosaraju failed: unknown error." << endl;
        }
    } else {
        cout << "Invalid command." << endl;
    }

    return 0;
}
