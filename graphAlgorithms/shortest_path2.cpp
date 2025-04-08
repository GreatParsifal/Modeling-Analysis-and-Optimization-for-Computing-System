/*
描述
给定一个带权有向图G，边权为正整数，给定两个节点s，t，你需要求出s到t的最短路径数量对998244353取模的结果。
输入
第一行包含两个整数N，M (1 <= N, M <= 100,000) ，分别为图的节点数和连边数。
第二行包含两个整数s，t (1 <= s, t <= N)。接下来M行，每行包含三个整数u，v，w (1 <= u, v <= N, 1 <= w <= 1000) 代表图中有一条从u到v的有向边，边权为w。
输出
输出一个整数，为s到t的最短路径数量对998244353取模的结果。
*/
#include<iostream>
#include<vector>
#include<queue>
#include<climits>
using namespace std;

const int INF = INT_MAX;
const int MOD = 998244353;

struct Edge {
    int v, w;
};

// 节点结构体，用于表示Dijkstra优先队列中的节点
struct Node {
    int u, dist;
    bool operator<(const Node& other) const{
        return dist > other.dist;
    }
};

int main(){
    int n, m;
    cin >> n >> m;

    int s, t;
    cin >> s >> t;

    vector<vector<Edge>> adj(n + 1);
    for(int i = 0; i < m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    vector<int> dist(n + 1, INF);
    vector<int> cnt(n + 1, 0);  // cnt数组记录从起点s到每个节点的最短路径数量，初始化为0
    dist[s] = 0;
    cnt[s] = 1;

     // 使用优先队列（最小堆）来实现Dijkstra算法
    priority_queue<Node> pq;
    pq.push({s, 0});
    while(!pq.empty()){
        Node node = pq.top();
        pq.pop();
        int u = node.u;
        int d = node.dist;

        if(d > dist[u]) continue;

        for(const Edge& e : adj[u]){
            int v = e.v;
            int w = e.w;

            if(dist[v] > dist[u] + w){
                dist[v] = dist[u] + w;
                cnt[v] = cnt[u];
                pq.push({v, dist[v]});
            }
            else if(dist[v] == dist[u] + w){
                cnt[v] = (cnt[v] + cnt[u]) % MOD;
            }
        }
    }

    cout << cnt[t] << endl;
    return 0;
}