/*
描述
给定一个带权无向图G，现在有Q个询问，每个询问包含两个数 (x, c)，你需要回答从节点x出发，只经过权值不超过c的边，能够达到多少节点（包括节点x）。
输入
第一行包含三个数N，M，Q (1 <= N, M, Q <= 500,000)，分别为图的节点数，连边数和询问个数。
接下来M行，每行包含三个数u，v，w (1 <= u, v <= N, 0 <= w <= 1,000,000)，代表图中有一条连接u，v的边，边权值为w。接下来Q行，每行两个整数(x,c)，为一个询问。
输出
输出N行，第i行输出第i个询问的结果。
*/
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

struct Edge{
    int u, v, w;
    bool operator<(const Edge& other) const{
        return w < other.w;
    }
};

struct Query{
    int x, c, idx;
    bool operator<(const Query& other) const{
        return c < other.c;
    }
};

// 并查集
vector<int> parent, set_size;

// 查找代表元 + 路径压缩
int find(int x){
    if(parent[x] != x){
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

// 合并两个集合 + 按秩合并（将小集合合到大集合）
void unite(int x, int y){
    x = find(x);
    y = find(y);
    if(x != y){
        if(set_size[x] < set_size[y]) swap(x, y);
        parent[y] = x;
        set_size[x] += set_size[y];
    }
}

int main(){
    int n, m, q;
    cin >> n >> m >> q;

    vector<Edge> edges(m);
    for(int i = 0; i < m; i++){
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    vector<Query> queries(q);
    for(int i = 0; i < q; i++){
        cin >> queries[i].x >> queries[i].c;
        queries[i].idx = i;
    }

    // 将边和查询都按照权值升序排序
    sort(edges.begin(), edges.end());
    sort(queries.begin(), queries.end());

    parent.resize(n + 1);
    set_size.resize(n + 1, 1);
    for(int i = 1; i <= n; i++){
        parent[i] = i;
    }

    vector<int> result(q);
    int edge_index = 0;

    // 离线处理所有查询
    for(int i = 0; i < q; i++){
        int x = queries[i].x;
        int c = queries[i].c;

        while(edge_index < m && edges[edge_index].w <= c){
            unite(edges[edge_index].u, edges[edge_index].v);
            edge_index++;
        }

        result[queries[i].idx] = set_size[find(x)];
    }

    // 按照输入顺序输出每个查询的答案
    for(int i = 0; i < q; i++){
        cout << result[i] << endl;
    }
    return 0;
}