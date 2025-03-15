#include "dijkstras.h"

vector<int> dijkstra_shortest_path(const Graph& G, int source, vector<int>& previous) {
    int n = G.numVertices;
    vector<int> distances(n, INF);
    previous.assign(n, -1);
    vector<bool> visited(n, false);
    
    // Priority queue to store nodes with their distances
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    
    distances[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        
        if (visited[u]) continue;
        visited[u] = true;
        
        // Explore all neighbors of current vertex
        for (const Edge& edge : G[u]) {
            int v = edge.dst;
            int weight = edge.weight;
            
            if (!visited[v] && distances[u] != INF && 
                distances[u] + weight < distances[v]) {
                distances[v] = distances[u] + weight;
                previous[v] = u;
                pq.push({distances[v], v});
            }
        }
    }
    return distances;
}

vector<int> extract_shortest_path(const vector<int>& distances, 
                                const vector<int>& previous, 
                                int destination) {
    vector<int> path;
    int current = destination;
    
    // Build path by backtracking from destination to source
    while (current != -1) {
        path.push_back(current);
        current = previous[current];
    }
    reverse(path.begin(), path.end());
    return path;
}

void print_path(const vector<int>& path, int total) {
    // Print path and total cost
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) cout << " ";
    }
    cout << "\nTotal cost is " << total << "\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    
    Graph G;
    try {
        file_to_graph(argv[1], G);
    } catch (const runtime_error& e) {
        cerr << e.what() << "\n";
        return 1;
    }
    
    vector<int> previous;
    vector<int> distances = dijkstra_shortest_path(G, 0, previous);
    
    // Print shortest path to each vertex from source (0)
    for (int i = 0; i < G.numVertices; ++i) {
        if (distances[i] != INF) {
            vector<int> path = extract_shortest_path(distances, previous, i);
            print_path(path, distances[i]);
        }
    }
    
    return 0;
}