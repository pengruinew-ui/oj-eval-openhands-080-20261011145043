#include <iostream>
#include <vector>
#include <queue>

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
                // Component has odd cycles
                // A vertex can be invited if it has degree >= 2
                for (int vertex : component_vertices) {
                    if (adj[vertex].size() < 2) {
                        answer++; // Cannot be in any cycle
                    }
                }
            }
        }
    }
    
    cout << answer << endl;
    
    return 0;
}