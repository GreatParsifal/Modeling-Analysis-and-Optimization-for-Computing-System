/*
描述
阅读E中Simplex算法的实现并补全缺失的pivot，之后调用Simplex算法，实现一个用来求解
maximize c^T x
   s.t.         Ax <= b
                  x>=0
的算法。
输入
第一行包含两个整数M，N (1 <= N, M <= 20),分别为约束个数和变量个数。
第二行包含N个整数，其中第i个数为ci.接下来M行，每行描述一个约束，包含N+1个整数，前N个数为Aij, 第N+1个数为bi (|ci|, |aij| <= 100, 0 <= bi <= 100).
输出
如果无解，输出"Infeasible"，如果解没有上界，输出"Unbounded"。
否则输出两行，第一行包含一个浮点数，为最优的目标函数值。第二行包含N个浮点数，第i个浮点数为最优解中xi的值。
你输出的目标函数值与最优答案的相对误差或绝对误差不超过1e-3即视为正确。
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
    int n, m;
    cin >> m >> n;

    Simplex S(n + m, m);
    for(int i = 0; i < n; i++){
        cin >> S.c[i];
    }
    for(int i = n; i < n + m; i++){
        S.c[i] = 0;
    }
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            cin >> S.a[i][j];
        }
        cin >> S.a[i][n + m];
        
        for(int j = n; j < n + m; j++){
            if(j - n == i) S.a[i][j] = 1;
            else S.a[i][j] = 0;
        }
    }

    vector<int> basic(m);
    for(int i = 0; i < m; i++){
        basic[i] = i + n;
    }
    S.init(basic);
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

    cout << fixed << setprecision(6) << obj << "\n";
    for (int i = 0; i < n; ++i)
        cout << fixed << setprecision(6) << sol[i] << " ";
    cout << "\n";

    return 0;
}
