#include <bits/stdc++.h>
#include <stdexcept>
#include <sstream>
#include <queue>
#include <unordered_set>

using namespace std;

bool isUsed(int debug[], int n, int v);

class GFG {
public:
    // dfs Function to reach destination
    bool dfs(int curr, int des, vector<vector<int>>& adjMatrix, vector<int>& vis) {
        // If curr node is destination return true
        if (curr == des) {
            return true;
        }
        vis[curr] = 1;
        for (size_t i = 0; i < adjMatrix[curr].size(); ++i) {
            if (adjMatrix[curr][i] == 1 && !vis[i]) {
                if (dfs(i, des, adjMatrix, vis)) {
                    return true;
                }
            }
        }
        return false;
    }

    // To tell whether there is path from source to destination
    bool isPath(int src, int des, vector<vector<int>>& adjMatrix) {
        vector<int> vis(adjMatrix.size(), 0);
        return dfs(src, des, adjMatrix, vis);
    }

    // Function to return all the strongly connected components of a graph.
    vector<vector<int>> findSCC(int n, vector<vector<int>>& adjMatrix) {
        // Stores all the strongly connected components.
        vector<vector<int>> ans;

        // Mark vis array to keep track of visited nodes during DFS
        vector<int> vis(n, 0);

        vector<int> inStack(n, 0);

        vector<int> disc(n, -1);
        vector<int> low(n, -1);

        int time = 0;
        vector<vector<int>> sccs;
        vector<int> s;
        // Depth-first search
        function<void(int)> dfs = [&](int u) {
            disc[u] = low[u] = time;
            time++;
            vis[u] = 1;
            inStack[u] = 1;
            s.push_back(u);

            for (int v = 0; v < n; v++) {
                if (adjMatrix[u][v] == 1) {
                    if (disc[v] == -1) {
                        dfs(v);
                        low[u] = min(low[u], low[v]);
                    } else if (inStack[v] == 1) {
                        low[u] = min(low[u], disc[v]);
                    }
                }
            }

            if (low[u] == disc[u]) {
                vector<int> scc;
                int v;
                do {
                    v = s.back();
                    s.pop_back();
                    inStack[v] = 0;
                    scc.push_back(v);
                } while (v != u);
                sccs.push_back(scc);
            }
        };

        for (int i = 0; i < n; i++) {
            if (disc[i] == -1) {
                dfs(i);
            }
        }

        return sccs;
    }
};

int main() {
    GFG obj;
    size_t m, n;
    cout << "Enter number of vertices" << endl;
    cin >> n;

    // Initialize adjacency matrix with zeros
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));

    cout << "Enter number of edges" << endl;
    cin >> m;

    int* debug = new int[n]; // array of zeros
    memset(debug, 0, n * sizeof(int));

    cout << "Enter edges (source destination) value" << endl;
    for (size_t i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (isUsed(debug, n, u) && isUsed(debug, n, v)) {
            adjMatrix[u][v] = 1;
        } else {
            std::ostringstream oss;
            oss << "You have " << n << " vertices but entered a vertex out of range.";
            throw std::runtime_error(oss.str());
        }
    }

    vector<vector<int>> ans = obj.findSCC(n, adjMatrix);
    cout << "Strongly Connected Components are:\n";
    for (auto x : ans) {
        for (auto y : x) {
            cout << y << " ";
        }
        cout << "\n";
    }

    delete[] debug;

    return 0;
}

bool isUsed(int debug[], int n, int v) {
    for (int i = 0; i < n; i++) {
        if (debug[i] == v) {
            // Vertex already used
            return true;
        }
    }
    for (int i = 0; i < n; i++) {
        if (debug[i] == 0) {
            // Found an empty slot, mark this vertex as used
            debug[i] = v;
            return true;
        }
    }
    // No empty slot found and vertex is not already used
    return false;
}
