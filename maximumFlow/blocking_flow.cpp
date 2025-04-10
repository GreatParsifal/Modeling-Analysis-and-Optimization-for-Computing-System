/*
描述
给定一个剩余网络(residual network) 以及源点(source)、汇点(sink)，你需要
1）求出剩余网络对应的分层网络 (level graph)
2）求出分层网络上的任意一个阻塞流 (blocking flow)
输入
第一行包含两个整数N，M （1 <= N <= 1000, 1 <= M <= 20,000)，为剩余网络的节点数和连边数。
第二行包含两个整数s，t（1 <= s，t <= N)，对应源点和汇点。
接下来N行，每行包含三个整数u，v，c，(1 <= u, v <= N, 1 <= c <= 100000) 表示剩余网络中包含一条从u到v，容量为c的边（两个点之间可能有多条边，u可能等于v）。
输出
先输出一行，为阻塞流的大小。接下来输出M行，每行包含一个整数f，第i行的数为输入中第i条边上的流量，如果没有流量或者这条边不在分层网络上就输出0。
*/
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<climits>
using namespace std;

const int INF = INT_MAX;

struct Edge{
    int u, v, c, f; 
    Edge(int u, int v, int c): u(u), v(v), c(c), f(0){}
};

int n, m, s, t;
vector<Edge> edges; // 存所有边，正向边和反向边都存
vector<vector<int>> adj; // 邻接表，存每个节点对应的边编号
vector<int> level, ptr;  // 分别是层次图的层数和当前弧优化指针

// 构建层次图（Level Graph）
bool bfs(){
    level.assign(n + 1, -1);
    queue<int> q;
    q.push(s);
    level[s] = 0;

    while(!q.empty()){
        int u = q.front();
        q.pop();
        for(int idx : adj[u]){
            int v = edges[idx].v;
            if(level[v] == -1 && edges[idx].c > edges[idx].f){
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
    cin >> n >> m >> s >> t;
    
    adj.resize(n + 1);
    vector<pair<int, int>> input_edges;

    // 读入边，正向边+反向边配对插入
    for(int i = 0; i < m; i++){
        int u, v, c;
        cin >> u >> v >> c;
        input_edges.emplace_back(edges.size(), edges.size() + 1);
        edges.emplace_back(u, v, c);
        adj[u].push_back(edges.size() - 1);
        edges.emplace_back(v, u, 0); 
        adj[v].push_back(edges.size() - 1);
    }

    int total_flow = 0;
    if(bfs()){
        ptr.assign(n + 1, 0);
        int pushed;
        while((pushed = dfs(s, INF)) > 0){
            total_flow += pushed;
        }    
    }

    cout << total_flow << endl;
    for(auto [idx, _] : input_edges){
        cout << edges[idx].f << endl;
    }
    return 0;
}
