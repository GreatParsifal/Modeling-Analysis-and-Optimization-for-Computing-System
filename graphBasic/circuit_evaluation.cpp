/*
描述
我们可以用一个有向图来表示一个组合电路，用结点表示逻辑运算（本题所有的逻辑运算都是8-bit的逐位运算，例如 01001010 and 00111010 = 00001010），用边表示扇入扇出关系。

我们用如下方式来描述这个电路：

每个结点有5种类型，分别为input, output, and, or, xor，其中后三类为运算结点。
通过边 (u, v) 表示结点u为结点v的扇入，保证input结点没有扇入，output结点扇入为1。
对于运算结点v来说，假如他的运算类型为op，他的扇入分别为u1, u2, ..., uk，那么运算结点v的结果f(v) = f(u1) op (fu2) op ... op f(uk) （当只有一个扇入时，f(v)=f(u1)）

现在给定一个有向图表示的组合电路，以及每个input结点的值，你需要输出每个output结点的结果。

输入
第一行包含两个整数N，M，分别代表结点和边的数量（1<= N, M <= 1,000,000)。接下来N行，每行包含一个字符串，第i行为描述结点i的类型。接下来M行，每行包含两个整数u，v（1<=u,v<=N)，表示图中包含u到v的有向边 （保证边没有重复）。若图中包含I个input结点，则接下来有I行，按编号从小到大的顺序指定每个input结点的值（在0~255之间）。
输出
若电路中有O个output结点，则需要输出O行，按照编号从小到大的顺序输出每个output结点的结果（在0~255之间）
*/
#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<unordered_map>
using namespace std;

enum nodetype{
    input,
    output,
    and_,
    or_,
    xor_
};

//节点类型匹配
nodetype parsenodetype(const string& type) {
    if (type == "input") return input;
    if (type == "output") return output;
    if (type == "and") return and_;
    if (type == "or") return or_;
    if (type == "xor") return xor_;
    return input; 
}

//逐位运算函数
int bitwiseop(int a, int b, nodetype op){
    switch(op){
        case and_: return a & b;
        case or_: return a | b;
        case xor_: return a ^ b;
        default: return a;
    }
}

int main(){
    int n, m;
    cin >> n >> m;

    vector<nodetype> nodetypes(n + 1); //节点类型
    vector<int> values(n + 1, -1); //节点值
    vector<vector<int>> adjlist(n + 1); //邻接矩阵
    vector<int> indegree(n + 1, 0); //入度数组

    for(int i = 1; i <= n; i++){
        string type;
        cin >> type;
        nodetypes[i] = parsenodetype(type);
    }

    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        adjlist[u].push_back(v);
        indegree[v]++;
    }

    //读取input结点的值
    for(int i = 1; i <= n; i++){
        if(nodetypes[i] == input){
            cin >> values[i];
        }
    }

    //拓扑排序
    queue<int> q;
    for(int i = 1; i <= n; i++){
        if(indegree[i] == 0){
            q.push(i);
        }
    }

    while(!q.empty()){
        int u = q.front();
        q.pop();

        for(int v : adjlist[u]){
            indegree[v]--;
            if(nodetypes[v] == input && nodetypes[v] == output){
                values[v] = values[u];
            }
            else{
                if (values[v] == -1) {
                    values[v] = values[u];
                } 
                else {
                    values[v] = bitwiseop(values[v], values[u], nodetypes[v]);
                }
            }

            if(indegree[v] == 0) {
                q.push(v);
            }
        }
    }

    for(int i = 1; i <= n; i++){
        if(nodetypes[i] == output){
            cout << values[i] << endl;
        }
    }

    return 0;
}