/*
描述
给定一个容量网络，对于每一条边，有容量限制c以及单位代价p，意味着这条边上的流量f不能超过c，而且会产生f*p的代价。
现在你需要求出一个从s到t的可行流，流量大小至少为K，总代价最小。
输入
第一行包含两个整数N，M (1 <= N <= 200, 1<= M <= 500)，分别为网络中节点和边的数量，其中源点s的标号为1，汇点的标号为N。
第二行包含一个整数K (1 <= K <= 100)，为要求的流量下限。
接下来M行，每行包含4个整数u,v,c,p，(1 <= u, v <= N, 1 <= c <= 100, |p| <= 1000) 代表从u到v有一条容量为c，单位代价为p的边。
输出
如果找不到容量至少为K的流，则输出Infeasible。否则输出一个浮点数，为最小的总代价，保留小数点后3位。
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include<iomanip>
using namespace std;

const double eps = 1e-8;

int sign(double f) {
    if (fabs(f) < eps)
        return 0;
    return f > 0 ? 1 : -1;
}

struct Edge {
    int u, v;
    double c, p;
    Edge(int u, int v, int c, int p) : u(u), v(v), c(c), p(p){};
};
vector<Edge> edges;

struct Simplex {
    int n, m;
    vector<double> c;
    vector<vector<double>> a;
    vector<int> basic;
    int status;

    Simplex(int _n, int _m) {
        n = _n;
        m = _m;
        c.clear();
        a.clear();
        c.resize(n + 1, 0.0);
        a.resize(m, vector<double>(n + 1, 0.0));
        basic.resize(m, -1);
        status = -1;
    }

    int select_column() {
        for (int i = 0; i < n; ++i)
            if (sign(c[i]) > 0)
                return i;
        return -1;
    }

    int select_row(int col) {
        double min_bound = 1e30;
        int row = -1;
        for (int i = 0; i < m; ++i) {
            if (sign(a[i][col]) <= 0)
                continue;

            double bound = a[i][n] / a[i][col];
            if (bound < min_bound || (sign(bound - min_bound) == 0 && basic[i] < basic[row])) {
                min_bound = bound;
                row = i;
            }
        }
        return row;
    }

    void pivot(int col, int row) {
        double div = a[row][col];
        for (int j = 0; j <= n; ++j)
            a[row][j] /= div;

        for (int i = 0; i < m; ++i) {
            if (i != row && sign(a[i][col]) != 0) {
                double factor = a[i][col];
                for (int j = 0; j <= n; ++j)
                    a[i][j] -= factor * a[row][j];
            }
        }

        double factor = c[col];
        for (int j = 0; j <= n; ++j)
            c[j] -= factor * a[row][j];

        basic[row] = col;
    }

    // 对给定的基变量进行初始化，init_basic[i]为初始的B(i)，init_basic[i]=-1表示第i行没有基本变量。
    int init(vector<int> init_basic) {
        vector<bool> visited(n, false);
        for (int i = 0; i < m; ++i) {
            if (init_basic[i] == -1)
                continue;

            if (init_basic[i] >= n)
                return 1;
            if (visited[init_basic[i]])
                return 1;

            visited[init_basic[i]] = true;
        }

        for (int i = 0; i < m; ++i) {
            if (init_basic[i] != -1) {
                int j = i;
                for (; j < m; ++j)
                    if (sign(a[j][init_basic[i]]) != 0)
                        break;
                if (j != i)
                    swap(a[j], a[i]);

                pivot(init_basic[i], i);
            }
            basic[i] = init_basic[i];
        }

        for (int i = 0; i < m; ++i)
            assert(sign(a[i][n]) >= 0);

        return 0;
    }

    double get_objective() {
        assert(status == 0);
        return -c[n];
    }

    vector<double> get_solution() {
        vector<double> sol(n, 0.0);
        for (int i = 0; i < m; ++i)
            if (basic[i] != -1)
                sol[basic[i]] = a[i][n];
        return sol;
    }

    int solve() {
        while (true) {
            int col = select_column();
            if (col == -1) {
                // col = -1 说明是当前已经是最优解了
                status = 0;
                break;
            }

            int row = select_row(col);
            if (row == -1) {
                // row = -1 说明x_col没有上界，LP的解无界。
                status = 1;
                break;
            }

            pivot(col, row);
        }

        return status;
    }
};

int main() {
    int n, m, k;
    cin >> n >> m >> k;

    //2m + 2个变量：0 ~ m - 1原变量f_i，m ~ 2m - 1辅助变量s_i，2m 为总流量变量F，
    //2m + 1为总流量辅助变量S，2m + 2 ~ 2m + 2 + n - 1为流量守恒补充变量w_i(基变量)，
    //n + m + 1个约束条件：0 ~ n - 1为流量守恒，n ~ n + m - 1为流量最大限制，n + m为总流量最小限制
    Simplex S(2 + 2 * m, n + m + 1);
    for(int i = 0; i < m; i++){
        int u, v, c, p;
        cin >> u >> v >> c >> p;
        edges.push_back(Edge(u, v, c, p));
    }
    for(int i = 0; i < m; i++){
        Edge e = edges[i];
        if(e.u == 1){
            S.a[0][i] = -1.0;
            S.a[e.v - 1][i] = 1.0;
            S.a[0][2 * m] = 1.0; //总流量在第2m列
        }else if(e.v == n){
            S.a[n - 1][i] = 1.0;
            S.a[e.u - 1][i] = -1.0;
            S.a[n - 1][2 * m] = -1.0; 
        }
        else{
            S.a[e.v - 1][i] = 1.0;
            S.a[e.u - 1][i] = -1.0;
        }
        S.a[n + i][i] = 1.0;
        S.a[n + i][m + i] = 1.0; //s_i
        S.a[n + i][2 * m + 2] = e.c;
    }
    S.a[n + m][2 * m] = 1.0;
    S.a[n + m][2 * m + 1] = -1.0;
    S.a[n + m][2 * m + 2] = k;

    for(int i = 0; i < m; i++){
        S.c[i] = -edges[i].p;
    }
    
    //辅助问题
    Simplex aux(2 * m + 2 + n + m + 1, n + m + 1);
    for(int i = 0; i < n + m + 1; i++){
        for(int j = 0; j < 2 * m + 2; j++){
          aux.a[i][j] = S.a[i][j];
        }
        aux.a[i][2 * m + 2 + n + m + 1] = S.a[i][2 * m + 2];
    }
    vector<int> aux_basic;
    aux_basic.resize(n + m + 1, -1);
    for(int i = 0; i < n + m + 1; i++){
        aux.a[i][2 * m + 2 + i] = 1.0;
        aux_basic[i] = 2 * m + 2 + i;
    }
    
    for(int i = 0; i < n + m + 1; i++){
        aux.c[2 * m + 2 + i] = -1.0;
    }

    aux.status = -1;
    aux.init(aux_basic);
    aux.solve();

    double aux_obj = aux.get_objective();
    if(sign(aux_obj) != 0) {
        cout << "Infeasible\n";
        return 0;
    }
    for(int i = 0; i < n + m + 1; i++){
        for(int j = 0; j <= 2 * m + 1; j++){
          S.a[i][j] = aux.a[i][j];
        }
        S.a[i][2 * m + 2] = aux.a[i][2 * m + 2 + n + m + 1];
        if(aux.basic[i] < 2 * m + 2){
         S.basic[i] = aux.basic[i];
        }
        else{
            S.basic[i] = -1;
        }
    }

    if(S.basic[n + m] == -1){
        S.pivot(2 * m, n + m);
    }

    S.status = -1;
    S.init(S.basic);
    
    int stat = S.solve();
    if (stat == 1) {
        cout << "Unbounded\n";
        return 0;
    }
    if (stat != 0) {
        cout << "Infeasible\n";
        return 0;
    }

    double obj = S.get_objective();
    vector<double> sol = S.get_solution();

    cout << fixed << setprecision(3) << -obj << "\n";

    return 0;
}
