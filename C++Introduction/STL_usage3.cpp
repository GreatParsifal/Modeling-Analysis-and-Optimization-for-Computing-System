/*
描述
给定数组A[1...N]，对于每个下标i，求出最大的下标 f(i) 使得 1<= f(i) < i 且 A[f(i)] > A[i].
输入
第一行包含一个整数N (1 <= N <= 1,000,000)，接下来一行包含N个整数，为输入数组（1 <= A[i] <= 1,000,000)。
输出
输出一行N个数，为每个下标i对应的f(i)，当f(i)不存在时，输出0
*/
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

int main(){
    int n;
    cin >> n;

    vector<int> A(n + 1);
    vector<int> result(n + 1, 0);

    for(int i = 1; i <= n; i++){
        cin >> A[i];
    }

    stack<int> s; // 用来保存下标的栈

    for(int i = 1; i <= n; i++){
        // 弹出栈中不满足条件的元素
        while(!s.empty() && A[s.top()] <= A[i]){
            s.pop();
        }

        // 如果栈非空，栈顶元素即为满足条件的f(i)
        if(!s.empty()){
            result[i] = s.top();
        }

        // 将当前下标压入栈
        s.push(i);
    }

    for(int i = 1; i <= n; i++){
        cout << result[i] << " ";
    }

    return 0;
}