/*
描述
补全代码
*/
#include <iostream>
#include <iomanip>
using namespace std;
// 在此处补充你的代码
template <typename T>
T myMax(T a, T b){
    return a > b ? a : b;
}

int main() {
    int a, b;
    cin >> a >> b;
    cout << myMax(a, b) << endl;

    float fa, fb;
    cin >> fa >> fb;
    cout.setf(ios::fixed);
    cout << setprecision(3) << myMax(fa, fb) << endl;
}