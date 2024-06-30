#include <bits/stdc++.h>
#include <stdexcept>
#include <sstream>
#include <list>
#include <unordered_set>

using namespace std;

bool isUsed(int debug[], int n, int v);

class GFG {
public:
    // dfs Function to reach destination
    bool dfs(int curr, int des, list<list<int>>& adj, vector<int>& vis) {
        // If curr node is destination return true
        if (curr == des) {
            return true;
        }
        vis[curr] = 1;
        auto it = next(adj.begin(), curr);
        for (auto x : *it) {
            if (!vis[x]) {
                if (dfs(x, des, adj, vis)) {
                    return true;
                }
            }
        }
        return false;
    }

    // To tell whether there is path from source to destination
    bool isPath(int src, int des, list<list<int>>& adj) {
        vector<int> vis(adj.size(), 0);
        return dfs(src, des, adj, vis);
    }

    // Function to return all the strongly connected components of a graph.
    list<list<int>> findSCC(int n, list<pair<int, int>>& edges) {
        // Stores all the strongly connected components.
        list<list<int>> ans;

        // Stores whether a vertex is a part of any Strongly Connected Component
        vector<int> is_scc(n + 1, 0);

        list<list<int>> adj(n + 1);

        for (auto& edge : edges) {
            auto it = next(adj.begin(), edge.first);
            it->push_back(edge.second);
        }

        // Traversing all the vertices
        for (int i = 1; i <= n; i++) {
            if (!is_scc[i]) {
                // If a vertex i is not a part of any SCC
                // insert it into a new SCC list and check for other vertices whether they can be part of this list.
                list<int> scc;
                scc.push_back(i);

                for (int j = i + 1; j <= n; j++) {
                    // If there is a path from vertex i to vertex j and vice versa put vertex j into the current SCC list.
                    if (!is_scc[j] && isPath(i, j, adj) && isPath(j, i, adj)) {
                        is_scc[j] = 1;
                        scc.push_back(j);
                    }
                }

                // Insert the SCC containing vertex i into the final list.
                ans.push_back(scc);
            }
        }
        return ans;
    }
};

int main() {
    GFG obj;
    size_t m, n;
    cout << "Enter number of edges" << endl;
    cin >> m;
    cout << "Enter number of vertices" << endl;
    cin >> n;

    if (n > m * 2) {
        std::ostringstream oss;
        oss << "Too many vertices";
        throw std::runtime_error(oss.str());
    }
    
    int* debug = new int[n](); // array of zeros

    list<pair<int, int>> edges;
    unordered_set<int> uniqueVertices;
    
    cout << "Enter vertices value" << endl;
    for (size_t i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        uniqueVertices.insert(u);
        uniqueVertices.insert(v);
        if (isUsed(debug, n, u) && isUsed(debug, n, v)) {
            edges.push_back({u, v});
        } else {
            std::ostringstream oss;
            oss << "You have " << n << " vertices but entered " << n + 1;
            throw std::runtime_error(oss.str());
        }
    }

    if (uniqueVertices.size() < n) {
        std::ostringstream oss;
        oss << "Fewer vertices entered than expected. Expected: " << n << ", Entered: " << uniqueVertices.size();
        throw std::runtime_error(oss.str());
    }

    for (auto& edge : edges) {
        cout << edge.first << "," << edge.second << endl;
    }

    list<list<int>> ans = obj.findSCC(n, edges);
    cout << "Strongly Connected Components are:\n";
    for (auto& scc : ans) {
        for (auto& vertex : scc) {
            cout << vertex << " ";
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
