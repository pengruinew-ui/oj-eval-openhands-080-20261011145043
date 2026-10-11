#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int n;
vector<int> disc, low;
vector<vector<int>> adj;
vector<bool> in_stack;
vector<int> stack_vec;
int timer;
vector<bool> has_odd_cycle;

void dfs(int u, int parent) {
    disc[u] = low[u] = ++timer;
    stack_vec.push_back(u);
    in_stack[u] = true;
    
    for (int v : adj[u]) {
        if (v == parent) continue;
        
        if (disc[v] == 0) {
            dfs(v, u);
            low[u] = min(low[u], low[v]);
        } else if (in_stack[v]) {
            low[u] = min(low[u], disc[v]);
        }
    }
    
    // If u is the root of a BCC
    if (low[u] == disc[u]) {
        vector<int> bcc;
        int w;
        do {
            w = stack_vec.back();
            stack_vec.pop_back();
            in_stack[w] = false;
            bcc.push_back(w);
        } while (w != u);
        
        // Check if this BCC has an odd cycle
        if (bcc.size() >= 3) {
            // Build subgraph for this BCC
            vector<vector<int>> sub_adj(bcc.size());
            vector<int> idx_map(n + 1, -1);
            
            for (int i = 0; i < bcc.size(); i++) {
                idx_map[bcc[i]] = i;
            }
            
            for (int i = 0; i < bcc.size(); i++) {
                int u = bcc[i];
                for (int v : adj[u]) {
                    if (idx_map[v] != -1) {
                        sub_adj[i].push_back(idx_map[v]);
                    }
                }
            }
            
            // Check if subgraph is bipartite
            vector<int> color(bcc.size(), -1);
            bool is_bipartite = true;
            
            for (int i = 0; i < bcc.size() && is_bipartite; i++) {
                if (color[i] == -1) {
                    queue<int> q;
                    q.push(i);
                    color[i] = 0;
                    
                    while (!q.empty() && is_bipartite) {
                        int curr = q.front();
                        q.pop();
                        
                        for (int next : sub_adj[curr]) {
                            if (color[next] == -1) {
                                color[next] = 1 - color[curr];
                                q.push(next);
                            } else if (color[next] == color[curr]) {
                                is_bipartite = false;
                                break;
                            }
                        }
                    }
                }
            }
            
            if (!is_bipartite) {
                // This BCC has an odd cycle
                for (int vertex : bcc) {
                    has_odd_cycle[vertex] = true;
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int m;
    cin >> n >> m;
    
    adj.resize(n + 1);
    disc.resize(n + 1, 0);
    low.resize(n + 1, 0);
    in_stack.resize(n + 1, false);
    has_odd_cycle.resize(n + 1, false);
    
    // Read the m edges (pairs that have NOT seen each other slacking - safe pairs)
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    
    timer = 0;
    
    // Find all Biconnected Components
    for (int i = 1; i <= n; i++) {
        if (disc[i] == 0) {
            dfs(i, -1);
        }
    }
    
    // Count vertices that cannot be part of any odd cycle
    int answer = 0;
    for (int i = 1; i <= n; i++) {
        if (!has_odd_cycle[i]) {
            answer++;
        }
    }
    
    cout << answer << endl;
    
    return 0;
}