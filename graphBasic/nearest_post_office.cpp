/*
描述
城市P被划分成了N*M个大小相同的街区，其中有些街区有邮局。我们可以用坐标(i, j) (1 <= i <= N, 1 <= j <= M)来表示一个街区。坐标为(i, j)的街区能够去到坐标为(i-1, j), (i+1, j), (i, j-1), (i, j+1)这4个相邻的街区 (处于边界的街区不能够越界)，且距离都是1。现在你需要求出每个街区到最近的邮局的距离。
输入
第一行包含两个整数N，M (1 <= N, M <= 500) 。接下来N行，每行包含M个整数0或1，0表示这个街区中没有邮局，1表示这个这个街区中有邮局。
输出
输出N行，每行包含M个整数，第i行的第j个整数为坐标(i,j)的街区到最近的邮局的距离（如果该街区内有邮局，则输出0）。
*/
#include<iostream>
#include<queue>
#include<climits>
using namespace std;

const int MAXN = 505;
const int dx[] = {-1, 1, 0, 0};
const int dy[] = {0, 0, -1, 1};

void bfs(int n, int m, vector<vector<int>>& grid, vector<vector<int>>& dist){
    queue<pair<int,int>> q;

    for(int i = 1; i <= n; ++i){
        for(int j = 1; j <= m; ++j){
            if(grid[i][j] == 1){
                dist[i][j] = 0;
                q.push({i, j});
            }
        }
    }

    while(!q.empty()){
        auto [x, y] = q.front();
        q.pop();

        for(int i = 0; i < 4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(nx > 0 && nx <= n && ny > 0 && ny <= m){
                if(dist[nx][ny] > dist[x][y] + 1){
                    dist[nx][ny] = dist[x][y] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }
}

int main(){
    int n, m;
    cin >> n >> m;

    vector<vector<int>> grid(n + 1, vector<int>(m + 1));
    vector<vector<int>> dist(n + 1, vector<int>(m + 1, INT_MAX));

    for(int i = 1; i <= n; ++i){
        for(int j = 1; j <= m; ++j){
            cin >> grid[i][j];
        }
    }

    bfs(n, m, grid, dist);

    for(int i = 1; i <= n; ++i){
        for(int j = 1; j <= m; ++j){
            cout << dist[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}