/*
描述
中位数的定义：一组数据按照从小到大排序后，如果数据个数为奇数，那么中位数是中间那个，如果为偶数，那么中位数是中间两个数种较小的那个数。
现在你需要维护一组数据，这组数据初始时为空，你需要处理两种类型的操作：
Add num -- 加入数据num
Query -- 查询这组数据的中位数
你需要对于每个Query操作输出正确的结果。
输入
第一行包含一个整数M (1 <= M <= 100,000) ，为操作的数量。之后M行，每行包含一个操作。每个加入的数num满足 1 <= num <= 100,000。
输出
对于每个Query操作，输出数据中的中位数。
提示
使用priority_queue实现堆，需要时间复杂度为 O(Mlog M)
*/
#include <iostream>
#include <queue>
#include <string>
using namespace std;

int main() {
    int m;
    cin >> m;

    priority_queue<int> maxHeap; // 大顶堆，保存较小的一半   
    priority_queue<int, vector<int>, greater<int>> minHeap; // 小顶堆，保存较大的一半

    while(m--) {
        string op;
        cin >> op;

        if(op == "Add") {
            int num;
            cin >> num;

            // 插入逻辑：先放到 maxHeap
            if(maxHeap.empty() || num <= maxHeap.top()) {
                maxHeap.push(num);
            }
            else {
                minHeap.push(num);
            }

            // 平衡两个堆
            if(maxHeap.size() > minHeap.size() + 1) {
                minHeap.push(maxHeap.top());
                maxHeap.pop();
            }
            else if(minHeap.size() > maxHeap.size()) {
                maxHeap.push(minHeap.top());
                minHeap.pop();
            }
        }
        else if(op == "Query") {
            if(!maxHeap.empty()) {
                cout << maxHeap.top() << endl;
            }
        }
    }

    return 0;
}