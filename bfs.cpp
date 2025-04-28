#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Graph {
    int V;
    vector< vector<int> > adj; // ?? space between >>

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int size = q.size();
            vector<int> nextLevel;

            #pragma omp parallel for shared(q, visited, nextLevel)
            for (int i = 0; i < size; i++) {
                int u;
                #pragma omp critical
                {
                    if (!q.empty()) {
                        u = q.front();
                        q.pop();
                        cout << "Visited (BFS): " << u << endl;
                    }
                }

                #pragma omp critical
                {
                    for (int j = 0; j < adj[u].size(); j++) {
                        int v = adj[u][j];
                        if (!visited[v]) {
                            visited[v] = true;
                            nextLevel.push_back(v);
                        }
                    }
                }
            }

            for (int i = 0; i < nextLevel.size(); i++) {
                q.push(nextLevel[i]);
            }
        }
    }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);

    cout << "\n----- Parallel BFS Traversal -----\n";
    g.parallelBFS(0);

    cin.get();
    return 0;
}
