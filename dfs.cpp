#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector< vector<int> > adj;  // ?? Add space between >

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected graph
    }

    void parallelDFSUtil(int v, vector<bool>& visited) {
        visited[v] = true;
        cout << "Visited (DFS): " << v << endl;

        #pragma omp parallel for
        for (int i = 0; i < adj[v].size(); i++) {
            int u = adj[v][i];
            bool doVisit = false;

            #pragma omp critical
            {
                if (!visited[u]) {
                    visited[u] = true;
                    doVisit = true;
                }
            }

            if (doVisit) {
                parallelDFSUtil(u, visited);
            }
        }
    }

    void parallelDFS(int start) {
        vector<bool> visited(V, false);
        parallelDFSUtil(start, visited);
    }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);

    cout << "\n----- Parallel DFS Traversal -----\n";
    g.parallelDFS(0);

    cin.get();  // Keeps the terminal open
    return 0;
}
