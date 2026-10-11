#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
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
    vector<int> color(n + 1, -1); // -1: uncolored, 0/1: colored
    int answer = 0;
    
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            queue<int> q;
            q.push(i);
            visited[i] = true;
            color[i] = 0;
            
            vector<int> component_vertices;
            bool is_bipartite = true;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                component_vertices.push_back(u);
                
                // Check all neighbors in the safe graph
                for (int v : adj[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        is_bipartite = false;
                    }
                }
            }
            
            if (is_bipartite) {
                answer += component_vertices.size();
            } else {
                // For non-bipartite components, we need to check more carefully
                // A vertex can be part of an odd cycle if it's in a non-bipartite biconnected component
                
                // Find articulation points and biconnected components
                vector<int> disc(n + 1, 0), low(n + 1, 0), parent(n + 1, -1);
                vector<bool> is_articulation(n + 1, false);
                int time = 0;
                
                // DFS to find articulation points
                function<void(int, int)> dfs_articulation = [&](int u, int p) {
                    disc[u] = low[u] = ++time;
                    parent[u] = p;
                    int children = 0;
                    
                    for (int v : adj[u]) {
                        if (v == p) continue;
                        
                        if (disc[v] == 0) {
                            children++;
                            dfs_articulation(v, u);
                            low[u] = min(low[u], low[v]);
                            
                            if (parent[u] == -1 && children > 1) {
                                is_articulation[u] = true;
                            }
                            if (parent[u] != -1 && low[v] >= disc[u]) {
                                is_articulation[u] = true;
                            }
                        } else {
                            low[u] = min(low[u], disc[v]);
                        }
                    }
                };
                
                // Run DFS from each unvisited vertex in this component
                for (int vertex : component_vertices) {
                    if (disc[vertex] == 0) {
                        dfs_articulation(vertex, -1);
                    }
                }
                
                // Check each vertex in the component
                for (int vertex : component_vertices) {
                    if (is_articulation[vertex]) {
                        // Articulation points might not be part of odd cycles
                        // Need to check if removing this vertex separates odd cycles
                        
                        // Temporarily remove this vertex and check if remaining graph is bipartite
                        vector<bool> temp_visited(n + 1, false);
                        vector<int> temp_color(n + 1, -1);
                        bool all_bipartite = true;
                        
                        for (int other : component_vertices) {
                            if (other != vertex && !temp_visited[other]) {
                                queue<int> temp_q;
                                temp_q.push(other);
                                temp_visited[other] = true;
                                temp_color[other] = 0;
                                bool temp_is_bipartite = true;
                                
                                while (!temp_q.empty() && temp_is_bipartite) {
                                    int u = temp_q.front();
                                    temp_q.pop();
                                    
                                    for (int v : adj[u]) {
                                        if (v == vertex) continue;
                                        
                                        if (!temp_visited[v]) {
                                            temp_visited[v] = true;
                                            temp_color[v] = 1 - temp_color[u];
                                            temp_q.push(v);
                                        } else if (temp_color[v] == temp_color[u]) {
                                            temp_is_bipartite = false;
                                            break;
                                        }
                                    }
                                }
                                
                                if (!temp_is_bipartite) {
                                    all_bipartite = false;
                                    break;
                                }
                            }
                        }
                        
                        if (all_bipartite) {
                            answer++; // This vertex cannot be part of any odd cycle
                        }
                    }
                }
            }
        }
    }
    
    cout << answer << endl;
    
    return 0;
}