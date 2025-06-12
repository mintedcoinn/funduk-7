#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>
#include <iomanip>
using namespace std;

class Graph {
private:
    int numVertices;
    vector<vector<int>> adjMatrix;

public:
    Graph(int vertices) : numVertices(vertices), adjMatrix(vertices, vector<int>(vertices, 0)) {}

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

    vector<int> BFS(int startVertex) {
        vector<bool> visited(numVertices, false);
        queue<int> q;
        vector<int> traversalOrder;

        q.push(startVertex);
        visited[startVertex] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            traversalOrder.push_back(current);

            for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
                if (adjMatrix[current][neighbor] > 0 && !visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        return traversalOrder;
    }

    vector<int> DeykstraAlg(int startVertex) {
        vector<int> distances(numVertices, INT_MAX);
        vector<bool> visited(numVertices, false);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        distances[startVertex] = 0;
        pq.push({ 0, startVertex });

        while (!pq.empty()) {
            int current = pq.top().second;
            pq.pop();

            if (visited[current]) continue;
            visited[current] = true;

            for (int neighbor = 0; neighbor < numVertices; ++neighbor) {
                int weight = adjMatrix[current][neighbor];
                if (weight > 0 && distances[current] + weight < distances[neighbor]) {
                    distances[neighbor] = distances[current] + weight;
                    pq.push({ distances[neighbor], neighbor });
                }
            }
        }

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

    vector<int> bfsOrder = graph.BFS(startVertex);
    cout << "BFS traversal order: ";
    for (int v : bfsOrder) {
        cout << v << " ";
    }
    cout << "\n";

    vector<int> shortestDistances = graph.DeykstraAlg(startVertex);
    cout << "Shortest distances from vertex " << startVertex << ":\n";
    for (int i = 0; i < shortestDistances.size(); ++i) {
        if (shortestDistances[i] == INT_MAX) {
            cout << "Vertex " << i << ": INF\n";
        }
        else {
            cout << "Vertex " << i << ": " << shortestDistances[i] << "\n";
        }
    }

    return 0;
}