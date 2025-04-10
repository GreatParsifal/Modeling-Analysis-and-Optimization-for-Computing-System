/*
描述
Farmer John completed his new barn just last week, complete with all the latest milking technology. 
Unfortunately, due to engineering problems, all the stalls in the new barn are different. 
For the first week, Farmer John randomly assigned cows to stalls, but it quickly became clear that any given cow was only willing to produce milk in certain stalls. 
For the last week, Farmer John has been collecting data on which cows are willing to produce milk in which stalls. 
A stall may be only assigned to one cow, and, of course, a cow may be only assigned to one stall.
Given the preferences of the cows, compute the maximum number of milk-producing assignments of cows to stalls that is possible.
输入
The input includes several cases. For each case, the first line contains two integers, N (0 <= N <= 200) and M (0 <= M <= 200). 
N is the number of cows that Farmer John has and M is the number of stalls in the new barn. 
Each of the following N lines corresponds to a single cow. The first integer (Si) on the line is the number of stalls that the cow is willing to produce milk in (0 <= Si <= M). 
The subsequent Si integers on that line are the stalls in which that cow is willing to produce milk. 
The stall numbers will be integers in the range (1..M), and no stall will be listed twice for a given cow.
输出
For each case, output a single line with a single integer, the maximum number of milk-producing stall assignments that can be made.
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

int n, m;
vector<Edge> edges;
vector<vector<int>> adj;
vector<int> level, ptr;

void addEdge(int u, int v, int c){
    edges.emplace_back(u, v, c);
    adj[u].push_back(edges.size() - 1);
    edges.emplace_back(v, u, 0);
    adj[v].push_back(edges.size() - 1);
}


// BFS构建分层网络
bool bfs(int s, int t) {
    level.assign(t + 1, -1);
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
int dfs(int u, int pushed, int t){
    if(pushed == 0) return 0;
    if(u == t) return pushed;
    for(;ptr[u] < adj[u].size(); ptr[u]++){
        int idx = adj[u][ptr[u]];
        int v = edges[idx].v;
        if(level[v] != level[u] + 1 || edges[idx].c - edges[idx].f <= 0) continue;

        // 更新正向边和反向边的流量
        int tr = dfs(v, min(edges[idx].c - edges[idx].f, pushed), t);
        if(tr == 0) continue;
        edges[idx].f += tr;
        edges[idx ^ 1].f -= tr; // ^1 得到反向边编号
        return tr;
    }
    return 0;
}

// Dinic算法主流程，计算最大流
int maxFlow(int s, int t){
    int total_flow = 0;
    while(bfs(s, t)){
        ptr.assign(t + 1, 0);
        int pushed;
        while((pushed = dfs(s, INF, t)) > 0){
            total_flow += pushed;
        }
    }
    return total_flow;
}

int main(){
    while(cin >> n >> m){
        int s = 0, t = n + m + 1;   // 源点编号为0，汇点编号为n+m+1

        edges.clear();
        adj.assign(t + 1, vector<int>());
        // 添加从源点到每个牛的边，容量为1
        for(int i = 1; i <= n; i++){
            addEdge(s, i, 1);
        }
        // 读取每只牛愿意去的牛棚编号，建立牛->牛棚的边
        for(int i = 1; i <= n; i++){
            int k;
            cin >> k;
            for(int j = 0; j < k; j++){
                int stall;
                cin >> stall;
                addEdge(i, n + stall, 1);
            }
        }
        // 添加从每个牛棚到汇点的边，容量为1
        for(int i = 1; i <= m; i++){
            addEdge(n + i, t, 1);
        }

        cout << maxFlow(s, t) << endl;
    }
    return 0;
}