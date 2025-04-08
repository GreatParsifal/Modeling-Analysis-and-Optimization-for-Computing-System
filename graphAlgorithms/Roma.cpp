/*
描述
条条大道通罗马！
现在你负责规划罗马共和国的交通，境内共有N个城市，罗马的编号为1，你已经调查好了在一些城市中间修路的花费。
你需要在一些城市之间修路使得每个城市都能够通过这些公路到达罗马，你想知道最小的花费是多少，以便向元老院申请经费。
输入
第一行包含两个整数N，M（1 <= N, M <= 100,000)，分别代表城市个数和你调查好了的修路方案数量。
接下来M行，每行包含三个整数u，v，w，(1 <= u, v <= N, 1 <= w <= 10000) 代表在城市u，v之间修路的花费为w。输入保证存在可行的修路方案。
输出
输出一个整数,为最小花费。
*/
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

vector<int> parent;// 用并查集来管理每个城市的父节点

int find(int x){
    if(parent[x] != x){
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

// 合并操作，将x和y合并到同一个集合中
void unite(int x, int y){
    x = find(x);
    y = find(y);
    if(x != y){
        parent[y] = x;
    }
}

int main(){
    int n, m;
    cin >> n >> m;

    vector<Edge> edges(m);
    for(int i = 0; i < m; i++){
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    sort(edges.begin(), edges.end());

    parent.resize(n + 1);
    for(int i = 1; i <= n; i++){
        parent[i] = i;
    }
    
    // Kruskal's algorithm to find the minimum spanning tree
    int totalCost = 0;
    int edgesCount = 0;
    for(const auto& edge: edges){
        if(find(edge.u) != find(edge.v)){
            unite(edge.u, edge.v);
            totalCost += edge.w;
            edgesCount++;
            if(edgesCount == n - 1){
                break;
            }
        }
    }

    cout << totalCost << endl;
    return 0;
}