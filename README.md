#include <iostream>
#include <omp.h>

using namespace std;

int graph[10][10];
int visited[10];
int vertices;

// Parallel DFS Function
void parallelDFS(int node)
{
    // Mark node as visited
    visited[node] = 1;

    // Print node
    cout << node << " ";

    // Traverse adjacent nodes in parallel
    #pragma omp parallel for
    for (int i = 0; i < vertices; i++)
    {
        if (graph[node][i] == 1 && visited[i] == 0)
        {
            #pragma omp critical
            {
                if (visited[i] == 0)
                {
                    parallelDFS(i);
                }
            }
        }
    }
}

int main()
{
    int edges;

    omp_set_num_threads(4);

    cout << "Enter number of vertices: ";
    cin >> vertices;

    cout << "Enter number of edges: ";
    cin >> edges;

    // Initialize graph and visited array
    for (int i = 0; i < vertices; i++)
    {
        visited[i] = 0;

        for (int j = 0; j < vertices; j++)
        {
            graph[i][j] = 0;
        }
    }

    cout << "Enter edges (source destination):\n";

    // Input edges
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        cin >> u >> v;

        graph[u][v] = 1;
    }

    cout << "\nDFS Traversal: ";

    double start = omp_get_wtime();

    // DFS for disconnected graph
    for (int i = 0; i < vertices; i++)
    {
        if (visited[i] == 0)
        {
            parallelDFS(i);
        }
    }

    double end = omp_get_wtime();

    cout << "\nTime Taken: "
         << end - start << " seconds\n";

    return 0;
}
