/*
描述
给定N个结点，M条边的无向图，你需要统计图的连通块数量。
输入
第一行包含两个整数N，M，(1 <= N, M <= 100,000) 分别为图的结点数和连边数。接下来M行，每行包含两个整数u，v (1 <= u, v <= N)，代表图中有一条无向边(u,v)。
输出
输出图中连通块的数量
*/
#include <iostream>
using namespace std;

const int V = 1000005;

struct Edge {
    int to;
    Edge *next;
};

Edge *head[V];

void addedge(int u, int v) {
    Edge *e = new Edge();
    e->next = head[u];
    e->to = v;
    head[u] = e;
}

int vis[V];

void dfs(int u) {
    vis[u] = 1;
    for (Edge *e = head[u]; e != NULL; e = e->next) {
        if (vis[e->to] == 0) {
            dfs(e->to);
        }
    }
}

int main() {
    int m, n;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        addedge(u, v);
        addedge(v, u);
    }

    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        if (vis[i] == 0) {
            dfs(i);
            cnt++;
        }
    }

    cout << cnt << endl;
    return 0;
}
