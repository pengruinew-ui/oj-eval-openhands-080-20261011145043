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
                // All vertices in bipartite components cannot be part of odd cycles
                answer += component_vertices.size();
            } else {
                // In non-bipartite components, check if vertices have degree >= 2
                // Vertices with degree < 2 cannot be part of cycles
                for (int vertex : component_vertices) {
                    if (adj[vertex].size() < 2) {
                        answer++;
                    }
                }
            }
        }
    }
    
    cout << answer << endl;
    
    return 0;
}