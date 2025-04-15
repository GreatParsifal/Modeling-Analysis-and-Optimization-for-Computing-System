/*描述
给定N个长方形的长和宽，你需要将这N个长方形按照面积从小到大的顺序输出每个长方形的长和宽，当面积相同时，先输出长较小的，当面积和长都相同时，先输出宽较小的。
输入
第一行包含一个整数N (1 <= N <= 100,000)，为长方形的个数。接下来N行，每行包含两个正整数 (大小不超过1000），为长方形的长和宽。
输出
输出N行，每行输出两个整数，分别为长方形的长和宽。
*/
#include <iostream>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

struct Rectangle {
    int length, width;
    Rectangle(int l, int w): length(l), width(w){};
};

bool cmp(const Rectangle& a, const Rectangle& b){
    int areaA = a.length * a.width;
    int areaB = b.length * b.width;

    if(areaA != areaB){
        return areaA < areaB;
    }
    else if(a.length != b.length){
        return a.length < b.length;
    }
    else {
        return a.width < b.width;
    }
}

// priority_queue实现：比较器：priority_queue 默认是大顶堆，我们需要小顶堆，所以反过来写
struct Compare {
    bool operator()(const Rectangle& a, const Rectangle& b) {
        int areaA = a.length * a.width;
        int areaB = b.length * b.width;

        if (areaA != areaB)
            return areaA > areaB; // 面积小的优先
        else if (a.length != b.length)
            return a.length > b.length; // 长度小的优先
        else
            return a.width > b.width; // 宽度小的优先
    }
};

int main(){
    int n;
    cin >> n;

    vector<Rectangle> rects;
    //priority_queue<Rectangle, vector<Rectangle>, Compare> pq; priority_queue实现

    for(int i = 1; i <= n; i++){
        int l, w;
        cin >> l >> w;
        rects.emplace_back(l, w);
        //pq.push(Rectangle(l, w));
    }

    /*
    while (!pq.empty()) {
        Rectangle rect = pq.top();
        pq.pop();
        cout << rect.length << " " << rect.width << endl;
    }
    */

    sort(rects.begin(), rects.end(), cmp);

    for(const auto& rect : rects){
        cout << rect.length << " " << rect.width << endl;
    }
    return 0;
}