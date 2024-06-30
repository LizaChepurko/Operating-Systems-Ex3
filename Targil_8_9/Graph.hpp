#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <mutex>

using namespace std;

class Graph {
private:
    vector<vector<int>> graph;
    int n, m;
    static Graph* instance;

    // Private constructor to prevent instantiation
    Graph() : n(0), m(0) {}

public:
    static Graph &getInstance() {
        if (instance == nullptr) {
            instance = new Graph();
        }
        return *instance;
    }

    vector<vector<int>> Newgraph(int n, int m);
    string Kosaraju();
    string Newedge(int i, int j);
    void Removeedge(int i, int j);

    class GFG {
    public:
        bool dfs(int curr, int des, vector<vector<int>> &adj, vector<int> &vis) {
            if (curr == des) {
                return true;
            }
            vis[curr] = 1;
            for (auto x : adj[curr]) {
                if (!vis[x]) {
                    if (dfs(x, des, adj, vis)) {
                        return true;
                    }
                }
            }
            return false;
        }

        bool isPath(int src, int des, vector<vector<int>> &adj) {
            vector<int> vis(adj.size(), 0);
            return dfs(src, des, adj, vis);
        }

        vector<vector<int>> findSCC(int n, vector<vector<int>> &a) {
            vector<vector<int>> ans;
            vector<int> is_scc(n + 1, 0);
            vector<vector<int>> adj(n + 1);

            for (size_t i = 0; i < a.size(); i++) {
                adj[a[i][0]].push_back(a[i][1]);
            }

            for (int i = 1; i <= n; i++) {
                if (!is_scc[i]) {
                    vector<int> scc;
                    scc.push_back(i);

                    for (int j = i + 1; j <= n; j++) {
                        if (!is_scc[j] && isPath(i, j, adj) && isPath(j, i, adj)) {
                            is_scc[j] = 1;
                            scc.push_back(j);
                        }
                    }
                    ans.push_back(scc);
                }
            }
            return ans;
        }
    };
};

#endif // GRAPH_HPP
