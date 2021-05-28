#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>

bool BFS(std::vector <std::unordered_map <int, int>>& graph, int start, int end, std::vector<int>& parents) {
    // finding a way from start to end using BFS
    parents.clear();
    parents.resize(graph.size(), -1);
    std::vector<bool> visited(graph.size());
    std::queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int cur_vertex = q.front();
        q.pop();
        for (auto edge : graph[cur_vertex]) {
            int new_vertex = edge.first;
            int new_capacity = edge.second;
            if (!visited[new_vertex] && new_capacity) {
                // edge between current and new vertex exists and new vertex was not visited yet
                visited[new_vertex] = true;
                q.push(new_vertex);
                parents[new_vertex] = cur_vertex;
                if (new_vertex == end) {
                    // we've found a way between start and end
                    return true;
                }
            }
        }
    }
    return false;
}

long long MaxFlow (std::vector <std::unordered_map <int, int>>& graph, int source, int sink) {
    long long resFlow = 0;
    std::vector<int> parents;
    while (BFS(graph, source, sink, parents)) {
        // while there is a path from source to sink...
        int flow = 1000000001;
        // finding the lowest capacity in path found with BFS
        for (int i = sink; i != source; i = parents[i]) {
            if (i == -1) {
                throw std::runtime_error("Can't find a parent for not source node.");
            }
            if (graph[parents[i]][i] < flow) {
                flow = graph[parents[i]][i];
            }
        }
        // we can increase result by the value of flow found in current path
        resFlow += flow;

        // updating capacities
        for (int i = sink; i != source; i = parents[i]) {
            // remaining capacity of edge parent->child is decreased
            graph[parents[i]][i] -= flow;
            // we've increased flow -> creating a reverse-path edge
            graph[i][parents[i]] += flow;
        }
    }
    return resFlow;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector <std::unordered_map <int, int>> graph(n); // adjacency list
    // graph[i][j] = remaining capacity of edge i -> j
    for (int i = 0; i < m; ++i) {
        int from, to, capacity;
        std::cin >> from >> to >> capacity;
        --from;
        --to;
        graph[from][to] = capacity;
    }

    std::cout << MaxFlow(graph, 0, n - 1) << "\n";
}
