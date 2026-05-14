#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Graph {
public:
    map<int, bool> visited;
    map<int, list<int>> adj;

    void addEdge(int v, int w);
    void DFS(int v);
};

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);   // undirected graph
    adj[w].push_back(v);
}

void Graph::DFS(int v) {

    #pragma omp parallel
    {
        #pragma omp single
        {
            if (!visited[v]) {
                visited[v] = true;
                cout << v << " ";
            }

            for (int u : adj[v]) {
                if (!visited[u]) {
                    DFS(u);
                }
            }
        }
    }
}

int main() {
    omp_set_num_threads(4);

    Graph g;
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(3, 5);
    g.addEdge(2, 6);

    int start;
    cout << "Enter the vertex to start DFS traversal: ";
    cin >> start;

    cout << "\nDepth First Traversal:\n";
    g.DFS(start);
    cout << endl;

    return 0;
}
