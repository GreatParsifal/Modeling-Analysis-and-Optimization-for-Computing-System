/*
描述
有一个工厂准备开工了，现在市面上有K种原材料，每种原材料最多可购买ti吨。
工厂共有N种产品，每制造1吨第i种产品，需要第j种原材料pij吨，可以净赚wi元。
你需要求出一个生产方案（每种产品的数量不需要是整数），使得利润最大化。
输入
第一行包含两个整数N，K (1 <= N, K <= 200)，为产品和原材料种类数量。第二行包含K个整数，第i个数为ti。第三行包含N个整数，第i个数为wi。
接下来N行，每行表描述一种产品所需的原料数量，第i行第j列为pij。(0 <= ti, wi, pij <= 1000)
输出
输出一个浮点数，为最大的利润，保留小数点后三位数字。
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
    cin >> n >> m;

    Simplex S(n + m, m);
    for(int i = 0; i < m; i++){
        cin >> S.a[i][m + n];
    }
    for(int i = 0; i < n; i++){
        cin >> S.c[i];
    }
    for(int i = n; i < n + m; i++){
        S.c[i] = 0;
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> S.a[j][i];
        }
    }
    for(int i = 0; i < m; i++){
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

    cout << fixed << setprecision(3) << obj << "\n";

    return 0;
}
