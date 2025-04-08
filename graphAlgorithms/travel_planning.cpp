/*
描述
有N个城市，爱旅游的小A搜集到了这些城市中的M个单向航班，小A希望从城市S开始出发，并希望访问尽可能多不同的城市，由于小A很有钱，他并不在意重复搭乘航班或者访问重复的城市。
请告诉小A他最多能够访问多少个不同的城市。
输入
第一行包含两个整数N，M (1 <= N, M <= 1,000,000)，分别为城市的数量和航班的数量。第二行包含一个整数S (1 <= S <= N)， 为小A出发的城市。
接下来M行，每行包含两个整数u，v (1 <= u, v <= N)，代表有一个从城市u到城市v的单向航班。
输出
输出一个整数，为小A最多能够访问到的不同的城市数量。
*/
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<stack>
using namespace std;

//第一次DFS，用于计算postorder
void dfs(int u, vector<vector<int>>& adj, vector<bool>& visited, stack<int>& stk){
    visited[u] = true;
    for(int v: adj[u]){
        if(!visited[v]){
            dfs(v, adj, visited, stk);
        }
    }
    stk.push(u);
}

//第二次DFS，用于计算强连通分量
void dfs2(int u, vector<vector<int>>& adj, vector<bool>& visited, vector<int>& comp){
    visited[u] = true;
    comp.push_back(u);
    for(int v: adj[u]){
        if(!visited[v]){
            dfs2(v, adj, visited, comp);
        }
    }
}

//Kosaraju算法计算强连通分量
vector<vector<int>> kosaraju(int n, vector<vector<int>>& adj, vector<vector<int>>& adjRev){
    vector<bool> visited(n + 1, false);
    stack<int> stk;

    // 第一步 DFS 得到节点出栈顺序
    for(int i = 1; i <= n; i++){
        if(!visited[i]){
            dfs(i, adj, visited, stk);
        }
    }

    // 第二步 DFS 在反图上按照出栈顺序找出 SCC
    fill(visited.begin(), visited.end(), false);
    vector<vector<int>> comps;
    while(!stk.empty()){
        int u = stk.top();
        stk.pop();
        if(!visited[u]){
            vector<int> comp;
            dfs2(u, adjRev, visited, comp);
            comps.push_back(comp);
        }
    }
    return comps;
}

// 拓扑排序，用于在 DAG（缩点图）上做 DP
vector<int> topologicalSort(const vector<vector<int>>& condensedGraph){
    int n = condensedGraph.size();
    vector<int> inDegree(n, 0);
    for(int u = 0; u < n; u++){
        for(int v: condensedGraph[u]){
            inDegree[v]++;
        }
    }

    queue<int> q;
    for(int u = 0; u < n; u++){
        if(inDegree[u] == 0){
            q.push(u);
        }
    }

    vector<int> topoOrder;
    while(!q.empty()){
        int u = q.front();
        q.pop();
        topoOrder.push_back(u);
        for(int v: condensedGraph[u]){
            inDegree[v]--;
            if(inDegree[v] == 0){
                q.push(v);
            }
        }
    }
    return topoOrder;
}

int main(){
    int n, m;
    cin >> n >> m;  // n个城市，m条航班

    int s;
    cin >> s;   // 起点城市编号

    vector<vector<int>> adj(n + 1);     // 原图
    vector<vector<int>> adjRev(n + 1);  // 反图
    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);    
        adjRev[v].push_back(u);
    }

    // 获取所有强连通分量
    vector<vector<int>> comps = kosaraju(n, adj, adjRev);
    
    // 创建缩点图
    vector<vector<int>> condensedGraph(comps.size());
    vector<int> componentID(n + 1);
    for(int i = 0; i < comps.size(); i++){
        for(int u: comps[i]){
            componentID[u] = i;
        }
    }

    for(int u = 1; u <= n; u++){
        for(int v: adj[u]){
            if(componentID[u] != componentID[v]){
                condensedGraph[componentID[u]].push_back(componentID[v]);
            }
        }
    }

    int startComponent = componentID[s];
    vector<int> topoOrder = topologicalSort(condensedGraph);
    
    vector<int> dp(comps.size(), 0);
    dp[startComponent] = comps[startComponent].size();
    for(int u: topoOrder){
        if(dp[u] == 0) continue;
        for(int v: condensedGraph[u]){
            dp[v] = max(dp[v], dp[u] + static_cast<int>(comps[v].size()));
        }
    }

    int maxCities = 0;
    for(int i = 0; i < comps.size(); i++){
        maxCities = max(maxCities, dp[i]);
    }
    cout << maxCities << endl;
    return 0;
}