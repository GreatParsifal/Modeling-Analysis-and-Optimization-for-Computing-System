/*
描述
In modern society, each person has his own friends. Since all the people are very busy, they communicate with each other only by phone. 
You can assume that people A can keep in touch with people B, only if
1. A knows B's phone number, or
2. A knows people C's phone number and C can keep in touch with B.
It's assured that if people A knows people B's number, B will also know A's number.
Sometimes, someone may meet something bad which makes him lose touch with all the others. 
For example, he may lose his phone number book and change his phone number at the same time.
In this problem, you will know the relations between every two among N people.
To make it easy, we number these N people by 1,2,...,N. Given two special people with the number S and T, when some people meet bad things, S may lose touch with T. 
Your job is to compute the minimal number of people that can make this situation happen. It is supposed that bad thing will never happen on S or T.
输入
The first line of the input contains three integers N (2<=N<=200), S and T ( 1 <= S, T <= N , and S is not equal to T).
Each of the following N lines contains N integers. If i knows j's number, then the j-th number in the (i+1)-th line will be 1, otherwise the number will be 0.
You can assume that the number of 1s will not exceed 5000 in the input.
输出
If there is no way to make A lose touch with B, print "NO ANSWER!" in a single line.
Otherwise, the first line contains a single number t, which is the minimal number you have got, 
and if t is not zero, the second line is needed, which contains t integers in ascending order that indicate the number of people who meet bad things.
The integers are separated by a single space.
If there is more than one solution, we give every solution a score, and output the solution with the minimal score. 
We can compute the score of a solution in the following way: assume a solution is A1, A2, ..., At (1 <= A1 < A2 <...< At <=N ), the score will be (A1-1)*N^t+(A2-1)*N^(t-1)+...+(At-1)*N. 
The input will assure that there won't be two solutions with the minimal score.
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

int n;
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
    level.assign(2 * n + 1, -1);
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
        ptr.assign(2 * n + 1, 0);
        int pushed;
        while((pushed = dfs(s, INF, t)) > 0){
            total_flow += pushed;
        }
    }
    return total_flow;
}

int main(){
    int s, t;
    cin >> n >> s >> t; 

    adj.resize(2 * n + 1);  // 邻接表的大小为2*n+1，因为每个节点有两个部分：原始节点和映射后的节点
    for(int i = 1; i <= n; i++){
        addEdge(i, i + n, 1);   // 添加每个人和其对应的映射节点之间的边，容量为1
    }

    bool flag = false;
    for(int i = 1; i <= n; i++){
        for(int j = 1, x; j <= n; j++){
            cin >> x;
            if(x == 1){
                if(i == s && j == t) flag = true;
                addEdge(i + n, j, INF);  // 如果i和j有联系，添加边，容量为INF
            }
        }
    }

    if(flag){
        cout << "NO ANSWER!" << endl;
        return 0;
    }

    int max_flow = maxFlow(s + n, t);
    vector<int> remove_nodes; 

    for(int i = 1; i <= n; i++){
        if(max_flow == 0) break;
        if(i == s || i == t) continue;
        
        edges[adj[i][0]].c = 0;  // 暂时删除节点i
        for (Edge &e : edges) {
            e.f = 0;
        }
        
        if (maxFlow(s + n, t) < max_flow) { // 如果删除节点i后，最大流变小，说明i是割点
            max_flow--;
            remove_nodes.push_back(i);
        } 
        else {
            edges[adj[i][0]].c = 1;
        }
    }

    cout << remove_nodes.size() << endl;
    for(size_t i = 0; i < remove_nodes.size(); i++){
        cout << remove_nodes[i] << (i + 1 == remove_nodes.size() ? "\n" : " ");
    }
    return 0;
}