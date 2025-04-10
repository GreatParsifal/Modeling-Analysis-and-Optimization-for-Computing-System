/*
描述
给定一个无向图，判断这个图是否为二分图(bipartite graph)。
输入
第一行包含两个整数N，M (1 <= N, M <= 1,000,000)。分别为图中点的数量和边的数量，其中点的标号在1~N之间。接下来M行，每行包含两个整数u, v (1 <= u, v <= N)，表示图中存在一条连接结点u和结点v的无向边。
输出
根据给定的图是否为二分图，输出"YES"或"NO"
*/
#include <iostream>
#include <cstring>
using namespace std;

const int V = 1000005;

struct Edge {
    int to;
    Edge *next;
};

Edge *head[V];
void addedge(int u, int v) {
    Edge *e = new Edge();
    e->next = head[u], e->to = v;
    head[u] = e;
}

int color[V]; //-1表示未访问，0表示黑色，1表示白色

void dfs(int u, int c) {
    color[u] = c;
    for (Edge *e = head[u]; e != NULL; e = e->next){
        if(color[e->to] == -1)
            dfs(e->to, c ^ 1);
        if(color[e->to] != -1 && color[e->to] == c) {
            cout << "NO" << endl;
            exit(0);
        }
    }
}

int main(){
    int M, N;
    cin >> N >> M;
    for (int i = 1; i <= M; i++) {
        int a, b;
        cin >> a >> b;
        addedge(a, b);
        addedge(b, a);
    }
    
    memset(color, -1, sizeof(color));
    for(int i = 1; i <= N; i++){
        if(color[i] == -1){
            dfs(i, 0);
        }
    }

    cout << "YES" << endl;
    return 0;
}
