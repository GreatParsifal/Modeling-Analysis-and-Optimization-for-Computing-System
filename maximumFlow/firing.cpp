/*
描述
You’ve finally got mad at “the world’s most stupid” employees of yours and decided to do some firings. 
You’re now simply too mad to give response to questions like “Don’t you think it is an even more stupid decision to have signed them?”, yet calm enough to consider the potential profit and loss from firing a good portion of them. 
While getting rid of an employee will save your wage and bonus expenditure on him, termination of a contract before expiration costs you funds for compensation. 
If you fire an employee, you also fire all his underlings and the underlings of his underlings and those underlings’ underlings’ underlings… 
An employee may serve in several departments and his (direct or indirect) underlings in one department may be his boss in another department. Is your firing plan ready now?
输入
The input starts with two integers n (0 < n ≤ 5000) and m (0 ≤ m ≤ 60000) on the same line. 
Next follows n + m lines. The first n lines of these give the net profit/loss from firing the i-th employee individually bi (|bi| ≤ 107, 1 ≤ i ≤ n). 
The remaining m lines each contain two integers i and j (1 ≤ i, j ≤ n) meaning the i-th employee has the j-th employee as his direct underling.
输出
Output two integers separated by a single space: the minimum number of employees to fire to achieve the maximum profit, and the maximum profit.
*/
#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<climits>
#include<set>
using namespace std;

const int INF = INT_MAX;

struct Edge{
    int u, v;
    long long c, f; //本题中流量和容量需要用long long
    Edge(int u, int v, long long c) : u(u), v(v), c(c), f(0){}
};

int n, m, s, t; //s：超级源点，t：超级汇点
vector<Edge> edges;
vector<vector<int>> adj;
vector<int> level, ptr;
bool flag;  // 是否启用 cutSet 收集
set<int> cutSet;    // 最小割中被访问到的点集合

void addEdge(int u, int v, long long c){
    edges.emplace_back(u, v, c);
    adj[u].push_back(edges.size() - 1);
    edges.emplace_back(v, u, 0);
    adj[v].push_back(edges.size() - 1);
}

// BFS构建分层网络
bool bfs() {
    level.assign(n + 2, -1);
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

                if(flag) cutSet.insert(v);
            }
        }
    }
    return level[t] != -1;  
}

// 在分层图上执行DFS查找阻塞流
long long dfs(int u, long long pushed){
    if(pushed == 0) return 0;
    if(u == t) return pushed;
    for(;ptr[u] < adj[u].size(); ptr[u]++){
        int idx = adj[u][ptr[u]];
        int v = edges[idx].v;
        if(level[v] != level[u] + 1 || edges[idx].c - edges[idx].f <= 0) continue;

        // 更新正向边和反向边的流量
        long long tr = dfs(v, min(edges[idx].c - edges[idx].f, pushed));
        if(tr == 0) continue;
        edges[idx].f += tr;
        edges[idx ^ 1].f -= tr; // ^1 得到反向边编号
        return tr;
    }
    return 0;
}

// Dinic 算法主流程
long long Dinic(){
    long long total_flow = 0;
    while(bfs()){
        ptr.assign(n + 2, 0);
        long long pushed;
        while((pushed = dfs(s, INF)) > 0){
            total_flow += pushed;
        }
    }
    return total_flow;
}

int main(){
    cin >> n >> m;
    s = 0; t = n + 1;   // 超级源点和汇点
    adj.assign(n + 2, vector<int>());
    
    long long sum = 0;  // 所有正利润的总和
    for(int i = 1; i <= n; i++){
        int b;
        cin >> b;
        if(b > 0){
            // 如果解雇他是盈利的，连边从源点到该员工，容量为b
            addEdge(s, i, b);
            sum +=b;
        }
        else{
            // 如果解雇他会亏损，连边从该员工到汇点，容量为 -b
            addEdge(i, t, -b);
        }
    }
    // 解雇u必须连带解雇v，相当于 u -> v 连一条容量为INF的边
    for(int i = 1; i <= m; i++){
        int u, v;
        cin >> u >> v;
        addEdge(u, v, INF);
    }

    flag = false;
    long long minCut = Dinic();
    flag = true;
    bfs();  // 再跑一次 bfs 收集从源点可达的点

    cout << cutSet.size() << " " << sum - minCut << endl;
    return 0;
}