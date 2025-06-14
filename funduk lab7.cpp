#include <iostream>
#include <fstream>
#include <climits>
#include <iomanip>
using namespace std;

class Graph {
private:
    int numVertices;
    int** adjMatrix;

public:
    Graph(int vertices) : numVertices(vertices) {
        adjMatrix = new int* [numVertices];
        for (int i = 0; i < numVertices; ++i) {
            adjMatrix[i] = new int[numVertices];
            for (int j = 0; j < numVertices; ++j) {
                adjMatrix[i][j] = 0;
            }
        }
    }

    ~Graph() {
        for (int i = 0; i < numVertices; ++i) {
            delete[] adjMatrix[i];
        }
        delete[] adjMatrix;
    }

    void newEdge(int from, int to, int weight) {
        if (from >= 0 && from < numVertices && to >= 0 && to < numVertices) {
            adjMatrix[from][to] = weight;
        }
    }

    void print() {
        cout << "Adjacency Matrix:\n";
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                cout << setw(3) << adjMatrix[i][j] << " ";
            }
            cout << "\n";
        }
    }

    int* BFS(int startVertex, int& outSize) {
        bool* visited = new bool[numVertices];
        for (int i = 0; i < numVertices; ++i) visited[i] = false;

        int* queue = new int[numVertices];
        int front = 0, rear = 0;

        int* traversalOrder = new int[numVertices];
        int count = 0;

        queue[rear++] = startVertex;
        visited[startVertex] = true;

        while (front < rear) {
            int current = queue[front++];
            traversalOrder[count++] = current;

            for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
                if (adjMatrix[current][neighbor] > 0 && !visited[neighbor]) {
                    visited[neighbor] = true;
                    queue[rear++] = neighbor;
                }
            }
        }

        delete[] visited;
        delete[] queue;
        outSize = count;
        return traversalOrder;
    }

    int* DeykstraAlg(int startVertex, int& outSize) {
        int* distances = new int[numVertices];
        bool* visited = new bool[numVertices];

        for (int i = 0; i < numVertices; ++i) {
            distances[i] = INT_MAX;
            visited[i] = false;
        }

        distances[startVertex] = 0;

        for (int i = 0; i < numVertices; ++i) {
            int u = -1;
            int minDist = INT_MAX;
            for (int j = 0; j < numVertices; ++j) {
                if (!visited[j] && distances[j] < minDist) {
                    minDist = distances[j];
                    u = j;
                }
            }

            if (u == -1) break;
            visited[u] = true;

            for (int v = 0; v < numVertices; ++v) {
                int weight = adjMatrix[u][v];
                if (weight > 0 && distances[u] != INT_MAX && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                }
            }
        }

        delete[] visited;
        outSize = numVertices;
        return distances;
    }

    int getNumVertices() const {
        return numVertices;
    }
};

Graph readGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file\n";
        exit(1);
    }

    int numVertices;
    file >> numVertices;

    Graph graph(numVertices);

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < numVertices; ++j) {
            int weight;
            file >> weight;
            if (weight > 0) {
                graph.newEdge(i, j, weight);
            }
        }
    }

    file.close();
    return graph;
}

int main() {
    string filename = "input.txt";
    Graph graph = readGraphFromFile(filename);

    cout << "Graph representation:\n";
    graph.print();

    int startVertex;
    cout << "\nEnter start vertex (0-" << graph.getNumVertices() - 1 << "): ";
    cin >> startVertex;

    int bfsSize = 0;
    int* bfsOrder = graph.BFS(startVertex, bfsSize);
    cout << "BFS traversal order: ";
    for (int i = 0; i < bfsSize; ++i) {
        cout << bfsOrder[i] << " ";
    }
    cout << "\n";
    delete[] bfsOrder;

    int distSize = 0;
    int* shortestDistances = graph.DeykstraAlg(startVertex, distSize);
    cout << "Shortest distances from vertex " << startVertex << ":\n";
    for (int i = 0; i < distSize; ++i) {
        if (shortestDistances[i] == INT_MAX) {
            cout << "Vertex " << i << ": INF\n";
        }
        else {
            cout << "Vertex " << i << ": " << shortestDistances[i] << "\n";
        }
    }
    delete[] shortestDistances;

    return 0;
}