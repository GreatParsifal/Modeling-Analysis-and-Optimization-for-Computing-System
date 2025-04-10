/*
描述
给定一个容量网络以及每条边上的流量（源点为1，汇点为N），求出对应的剩余网络（residual network）。
输入
第一行包含两个整数N，M，（1 <= N <= 100, 1 <= M <= N(N-1))，分别为容量网络的节点数和边数。
接下来M行，每行包含4个整数u，v，c，f (1 <= u, v <= N, 1 <= f <= c <= 1000)，表示图中有一条从u到v且容量为c的边，这条边上的流量为f，保证任意两条边的起点和终点不会都相同。
输出
按顺序输出剩余网络中的所有边，每行3个整数u，v，c，代表剩余网络中有一条从u到v，容量为c的边，如果容量为0，则不用输出。
输出顺序为：先输出u较小的，如果u相等，先输出v较小的。对于任意起点u和终点v，最多输出一条从u到v的边。
*/
#include<iostream>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;

struct Edge{
    int u, v, c;
    Edge(int u, int v, int c): u(u), v(v), c(c){};
};

int main(){
    int n, m;
    cin >> n >> m;

    // 使用 map 存储边，自动去重并聚合容量
    map<pair<int, int>, int> residual;

    for(int i = 0; i < m; i++){
        int u, v, c, f;
        cin >> u >> v >> c >> f;

        // 正向剩余边：如果还有剩余容量
        if(c - f > 0){
            residual[{u, v}] += c - f;
        }

        // 反向剩余边：可以反悔当前流量
        if(f > 0){
            residual[{v, u}] += f;
        }
    }

    // 将 map 转换为 vector 以排序输出
    vector<Edge> result;
    for(const auto& entry : residual){
        int u = entry.first.first;
        int v = entry.first.second;
        int c = entry.second;
        if(c > 0){
            result.emplace_back(u, v, c);
        }
    }

    sort(result.begin(), result.end(), [](const Edge& a, const Edge& b){
        return a.u == b.u ? a.v < b.v : a.u < b.u;
    });
    for(const auto& edge: result){
        cout << edge.u << " " << edge.v << " " << edge.c << endl;
    }

    return 0;
}