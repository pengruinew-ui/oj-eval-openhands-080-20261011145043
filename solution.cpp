#include <iostream>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    cin >> n >> m;
    
    // Build adjacency list for the graph of safe pairs
    vector<vector<int>> adj(n + 1);
    
    // Read the m edges (pairs that have NOT seen each other slacking - safe pairs)
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    
    vector<bool> visited(n + 1, false);
    int answer = 0;
    
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            queue<int> q;
            q.push(i);
            visited[i] = true;
            
            vector<int> component_vertices;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                component_vertices.push_back(u);
                
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
            
            // Check if this component has an odd cycle
            bool has_odd_cycle = false;
            
            // Use BFS to check if component is bipartite
            vector<int> color(n + 1, -1);
            
            for (int start : component_vertices) {
                if (color[start] == -1) {
                    queue<int> bfs_q;
                    bfs_q.push(start);
                    color[start] = 0;
                    
                    while (!bfs_q.empty() && !has_odd_cycle) {
                        int u = bfs_q.front();
                        bfs_q.pop();
                        
                        for (int v : adj[u]) {
                            if (color[v] == -1) {
                                color[v] = 1 - color[u];
                                bfs_q.push(v);
                            } else if (color[v] == color[u]) {
                                has_odd_cycle = true;
                                break;
                            }
                        }
                    }
                    
                    if (has_odd_cycle) break;
                }
            }
            
            if (!has_odd_cycle) {
                // Component is bipartite, no odd cycles
                answer += component_vertices.size();
            } else {
                // Component has odd cycles, check individual vertices
                for (int vertex : component_vertices) {
                    // A vertex can be part of an odd cycle if:
                    // 1. It has degree >= 2 (to be in a cycle)
                    // 2. It's in a component with odd cycles
                    
                    if (adj[vertex].size() < 2) {
                        answer++; // Cannot be in any cycle
                    } else {
                        // Check if this vertex is actually part of some cycle
                        // Use DFS to find a cycle containing this vertex
                        
                        vector<int> parent(n + 1, -1);
                        vector<bool> in_path(n + 1, false);
                        bool found_cycle = false;
                        
                        function<void(int)> dfs_cycle = [&](int u) {
                            in_path[u] = true;
                            
                            for (int v : adj[u]) {
                                if (found_cycle) return;
                                
                                if (!in_path[v]) {
                                    parent[v] = u;
                                    dfs_cycle(v);
                                } else if (parent[u] != v) {
                                    // Found a cycle
                                    found_cycle = true;
                                    
                                    // Check if vertex is in this cycle
                                    int curr = u;
                                    while (curr != -1 && curr != v) {
                                        if (curr == vertex) {
                                            return;
                                        }
                                        curr = parent[curr];
                                    }
                                    if (v == vertex) {
                                        return;
                                    }
                                    
                                    // If we get here, vertex is not in this cycle
                                    found_cycle = false;
                                }
                            }
                            
                            in_path[u] = false;
                        };
                        
                        dfs_cycle(vertex);
                        
                        if (!found_cycle) {
                            answer++; // Vertex is not in any cycle
                        }
                    }
                }
            }
        }
    }
    
    cout << answer << endl;
    
    return 0;
}