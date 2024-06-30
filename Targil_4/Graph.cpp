#include "Graph.hpp"

Graph* Graph::instance = nullptr;

vector<vector<int>> Graph::Newgraph(int n, int m) {
    this->n = n;
    this->m = 0;  // Initialize edge count to 0
    if (n > m * 2) {
        throw runtime_error("Too many vertices");
    }

    // Initialize the graph with empty vectors
    this->graph = vector<vector<int>>();

    return this->graph;
}

std::string Graph::Kosaraju() {
    try {
        GFG obj;
        std::vector<std::vector<int>> ans = obj.findSCC(this->n, this->graph);
        std::ostringstream oss;
        oss << "Strongly Connected Components are:\n";
        for (const auto &x : ans) {
            for (const auto &y : x) {
                oss << y << " ";
            }
            oss << "\n";
        }
        return oss.str();
    } catch (const std::exception &e) {
        std::ostringstream oss;
        oss << "Kosaraju error: " << e.what();
        throw std::runtime_error(oss.str());
    }
}

string Graph::Newedge(int i, int j) {
    std::ostringstream oss;

    // Find an empty slot or add a new edge if all slots are filled
    bool edgeAdded = false;
    for (auto &edge : this->graph) {
        if (edge.empty()) {
            edge = {i, j};
            edgeAdded = true;
            break;
        }
    }
    if (!edgeAdded) {
        this->graph.push_back({i, j});
    }

    this->m++; // Increment edge count

    oss << "The edges after adding:\n";
    for (const auto &edge : this->graph) {
        if (!edge.empty()) {
            oss << edge[0] << "," << edge[1] << " ";
        }
    }

    return oss.str();
}

void Graph::Removeedge(int i, int j) {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        if ((*it)[0] == i && (*it)[1] == j) {
            graph.erase(it);
            break;
        }
    }
    this->m--;
    cout << "The edges after removing: " << endl;
    for (int i = 0; i < m; i++)
    {
        cout << graph[i][0] << "," << graph[i][1] << endl;
    }
}
