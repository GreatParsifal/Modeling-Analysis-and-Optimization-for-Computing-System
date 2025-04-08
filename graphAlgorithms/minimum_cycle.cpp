/*
描述
给定一个带权有向图G，找出G中边权之和最小的环（保证图中存在环），你只要输出环的边权之和即可。
输入
第一行包含两个整数N，M（1 <= N <= 300, 1 <= M <= N*(N-1)）。
接下来M行，每行包含三个整数u，v，w，（1 <= u，v <= N，1 <= w <= 1000），表示图中有一条u到v的有向边，权值为w。
输出
输出一个整数，为边权最小的环的边权之和。
*/
#include<iostream>
#include<vector>
#include<climits>
using namespace std;

const int INF = INT_MAX / 2;

int main(){
    int n, m;
    cin >> n >> m;

    vector<vector<int>> dist(n + 1, vector<int>(n + 1, INF));
    for(int i = 1; i <= n; i++){
        dist[i][i] = 0;
    }

    for(int i = 0; i < m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        dist[u][v] = min(dist[u][v], w);
    }

    int minCycle = INF;

    for(int k = 1; k <= n; k++){
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= n; j++){
                if(i != j && dist[i][k] != INF && dist[k][j] != INF){
                    minCycle = min(minCycle, dist[i][k] + dist[k][j] + dist[j][i]);
                }
            }
        }

        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= n; j++){
                if(dist[i][k] != INF && dist[k][j] != INF){
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    cout << minCycle << endl;
    return 0;
}