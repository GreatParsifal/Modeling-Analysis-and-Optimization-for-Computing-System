/*
描述
Every time it rains on Farmer John's fields, a pond forms over Bessie's favorite clover patch. 
This means that the clover is covered by water for awhile and takes quite a long time to regrow. 
Thus, Farmer John has built a set of drainage ditches so that Bessie's clover patch is never covered in water. Instead, the water is drained to a nearby stream. 
Being an ace engineer, Farmer John has also installed regulators at the beginning of each ditch, so he can control at what rate water flows into that ditch.
Farmer John knows not only how many gallons of water each ditch can transport per minute but also the exact layout of the ditches, which feed out of the pond and into each other and stream in a potentially complex network.
Given all this information, determine the maximum rate at which water can be transported out of the pond and into the stream. 
For any given ditch, water flows in only one direction, but there might be a way that water can flow in a circle.
输入
The input includes several cases. For each case, the first line contains two space-separated integers, N (0 <= N <= 200) and M (2 <= M <= 200). 
N is the number of ditches that Farmer John has dug. M is the number of intersections points for those ditches. 
Intersection 1 is the pond. Intersection point M is the stream. 
Each of the following N lines contains three integers, Si, Ei, and Ci. Si and Ei (1 <= Si, Ei <= M) designate the intersections between which this ditch flows. 
Water will flow through this ditch from Si to Ei. Ci (0 <= Ci <= 10,000,000) is the maximum rate at which water will flow through the ditch.
输出
For each case, output a single integer, the maximum rate at which water may emptied from the pond.
*/
#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<climits>
using namespace std;

const int INF = INT_MAX;

struct Edge{
    int u, v, c, f;
    Edge(int u, int v, int c) : u(u), v(v), c(c), f(0){}
};

int n, m, s, t;
vector<Edge> edges;
vector<vector<int>> adj;
vector<int> level, ptr;

// BFS构建分层网络
bool bfs() {
    level.assign(m + 1, -1);
    queue<int> q;
    q.push(s);
    level[s] = 0;
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int idx : adj[u]) {
            int v = edges[idx].v;
            if (level[v] == -1 && edges[idx].c > edges[idx].f) {
                level[v] = level[u] + 1;
                q.push(v);
            }
        }
    }
    return level[t] != -1;  
}

// 在分层图上执行DFS查找阻塞流
int dfs(int u, int pushed){
    if(pushed == 0) return 0;
    if(u == t) return pushed;
    for(;ptr[u] < adj[u].size(); ptr[u]++){
        int idx = adj[u][ptr[u]];
        int v = edges[idx].v;
        if(level[v] != level[u] + 1 || edges[idx].c - edges[idx].f <= 0) continue;

        // 更新正向边和反向边的流量
        int tr = dfs(v, min(edges[idx].c - edges[idx].f, pushed));
        if(tr == 0) continue;
        edges[idx].f += tr;
        edges[idx ^ 1].f -= tr; // ^1 得到反向边编号
        return tr;
    }
    return 0;
}

int main(){
    while(cin >> n >> m){
        s = 1; t = m;
        edges.clear();
        adj.assign(m + 1, vector<int>());

        for(int i = 0; i < n; i++){
            int u, v, c;
            cin >> u >> v >> c;
            edges.emplace_back(u, v, c);
            adj[u].push_back(edges.size() - 1);
            edges.emplace_back(v, u, 0); 
            adj[v].push_back(edges.size() - 1);
        }

        int total_flow = 0;
        while(bfs()){
            ptr.assign(m + 1, 0);
            int pushed;
            while((pushed = dfs(s, INF)) > 0){
                total_flow += pushed;
            }    
        }

        cout << total_flow << endl;
    }
    return 0;
}