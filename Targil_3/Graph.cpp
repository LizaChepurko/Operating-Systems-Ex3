#include "Graph.hpp"

bool isUsed(int debug[], int n, int v);

/**
 * Initializes a new graph with a specified number of vertices (n) and edges (m) based on user input. 
 * It constructs a graph by taking input for edge connections between vertices and 
 * validates the input to ensure it forms a valid graph structure.
*/
vector<vector<int>> Graph::Newgraph(int n, int m)
{
    this->n = n;
    this->m = m;
    int v;
    if (n > m * 2)
    {
        std::ostringstream oss;
        oss << "Too many vertices";
        throw std::runtime_error(oss.str());
    }

    int *debug = new int[n]; // array of zeros

    vector<vector<int>> edges(m, vector<int>(2));
    unordered_set<int> uniqueVertices;

    cout << "Enter vertices value" << endl;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            cin >> v;
            uniqueVertices.insert(v);
            if (isUsed(debug, n, v))
            {
                edges[i][j] = v;
            }
            else
            {
                std::ostringstream oss;
                oss << "You have " << n << " vertices but entered " << n + 1;
                throw std::runtime_error(oss.str());
            }
        }
    }

    if (uniqueVertices.size() < (size_t)n)
    {
        std::ostringstream oss;
        oss << "Fewer vertices entered than expected. Expected: " << n << ", Entered: " << uniqueVertices.size();
        throw std::runtime_error(oss.str());
    }

    cout << "The edges of the graph: " << endl;
    for (int i = 0; i < m; i++)
    {
        cout << edges[i][0] << "," << edges[i][1] << endl;
    }

    delete[] debug;
    this->graph = edges;
    return edges;
}
//Checks if a vertex was already used
bool isUsed(int debug[], int n, int v)
{
    for (int i = 0; i < n; i++)
    {
        if (debug[i] == v)
        {
            // Vertex already used
            return true;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (debug[i] == 0)
        {
            // Found an empty slot, mark this vertex as used
            debug[i] = v;
            return true;
        }
    }
    // No empty slot found and vertex is not already used
    return false;
}

/**
 * Calculates Koasaraju algorithm.
*/
void Graph::Kosaraju()
{
    Graph::GFG obj;
    vector<vector<int>> ans = obj.findSCC(this->n, this->graph);
    cout << "Strongly Connected Components are:\n";
    for (auto x : ans)
    {
        for (auto y : x)
        {
            cout << y << " ";
        }
        cout << "\n";
    }
}

/**
 * Adding and removing edges
 * assumming correct input
*/

void Graph::Newedge(int i, int j)
{
    this->m++;
    this->graph.push_back({i, j});

    cout << "The edges after adding: " << endl;
    for (int i = 0; i < m; i++)
    {
        cout << graph[i][0] << "," << graph[i][1] << endl;
    }
}

void Graph::Removeedge(int i, int j)
{
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

int main(){


    int n,m;
    cin >> n >> m;
    Graph g;
    g.Newgraph(n,m);
    g.Kosaraju();

    return 0;
}