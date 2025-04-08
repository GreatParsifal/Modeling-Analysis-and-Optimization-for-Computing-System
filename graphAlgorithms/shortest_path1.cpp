/*
描述
给定一个带权有向图G，边权的范围在 [-1000, 1000]之间，保证图中没有权重小于等于0的环。给定两个节点s，t，你需要求出s到t的最短路径数量对998244353取模的结果。
输入
第一行包含两个整数N，M (1 <= N, M <= 1,000) ，分别为图的节点数和连边数。第二行包含两个整数s，t (1 <= s, t <= N)。
接下来M行，每行包含三个整数u，v，w (1 <= u, v <= N, -1000 <= w <= 1000) 代表图中有一条从u到v的有向边，边权为w。
输出
输出一个整数，为s到t的最短路径数量对998244353取模的结果。
*/
#include<iostream>
#include<queue>
#include<vector>
#include<climits>
using namespace std;

const int INF = INT_MAX;
const int MOD = 998244353;

struct Edge{
    int u, v, w;
};

int main(){
    int n, m;
    cin >> n >> m;

    int s, t;
    cin >> s >> t;

    vector<Edge> edges(m + 1);
    for(int i = 1; i <= m; i++){
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    vector<int> dist(n + 1, INF);
    vector<long long> cnt(n + 1, 0);                                     // cnt数组用于存储从起点到每个节点的最短路径数量，初始化为0
    vector<vector<long long>> last(n + 1, vector<long long>(n + 1, 0));  // last数组用于记录通过某条边(u, v)到达v的路径数量，避免重复计算
    dist[s] = 0;
    cnt[s] = 1;

    // Bellman-Ford算法的主循环：最多进行n-1次松弛操作
    for(int i = 1; i <= n - 1; i++){
        for(int j = 1; j <= m; j++){
            const Edge& e = edges[j];
            if(dist[e.u] != INF && dist[e.v] > dist[e.u] + e.w){
                dist[e.v] = dist[e.u] + e.w;
                cnt[e.v] = cnt[e.u];
                last[e.u][e.v] = cnt[e.u];
                for(int k = 1; k <= n; k++){
                    if(k != e.u) last[k][e.v] = 0;
                }
            }
            else if(dist[e.u] != INF && dist[e.v] == dist[e.u] + e.w){
                cnt[e.v] = (cnt[e.v] + cnt[e.u] - last[e.u][e.v]) % MOD;
                last[e.u][e.v] = cnt[e.u];
            }
        }
    }

    cout << cnt[t] <<endl;
    return 0;
}