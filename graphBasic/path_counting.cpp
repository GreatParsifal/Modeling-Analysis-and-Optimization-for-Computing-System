/*
描述
给定一个有向图和正整数K，对于图中的任意两个结点u，v （u，v可以相同），你需要求出有多少条从u到v的路径包含恰好K条边（不要求为简单路径），由于结果可能会很大，你只需要输出结果对998244353取模的值即可。
输入
第一行包含三个整数N，M，K（1<=N<=100, 1<=M<=N(N-1)/2, 1<=K<=10^9），分别为图中结点数、边数以及描述中要求的路径长度。接下来包含M行，每行包含两个整数u, v，代表图中有一条u到v的有向边。
输出
输出N行，每行包含N个数，其中第i行的第j个数为结点i到结点j的路径数量对998244353取模的结果
*/
#include<iostream>
#include<vector>
using namespace std;

const int MOD = 998244353;
const int MAXN = 105;

typedef vector<vector<int>> Matrix;

int n, m;
long long k;

//矩阵乘法：C = A * B
Matrix multiply(const Matrix &A, const Matrix &B){
    Matrix C(n, vector<int>(n, 0));
    for(int i = 0; i < n; i++){
        for(int k = 0; k < n; k++){
            if(A[i][k] == 0) continue;
            for(int j = 0; j < n; j++){
                C[i][j] = (C[i][j] + 1LL * A[i][k] * B[k][j]) % MOD;
            }
        }
    }
    return C;
}

//矩阵快速幂：C = A^k
Matrix power(Matrix A, long long L){
    if(L == 1) return A;
    Matrix X = power(A, L / 2);
    if(L % 2 == 0) return multiply(X, X);
    else return multiply(A, multiply(X, X));
}

int main(){
    cin >> n >> m >> k;
    Matrix adj(n, vector<int>(n, 0));

    for(int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        u--; v--;
        adj[u][v]++;
    }

    Matrix res = power(adj, k);
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << res[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}