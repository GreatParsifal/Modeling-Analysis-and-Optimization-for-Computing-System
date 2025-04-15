/*
描述
在C++中，template可以用于class（类），设计模板类。请阅读题中的代码片段，设计模板类并补全代码的缺失片段。
*/
#include <iostream>
using namespace std;
// 在此处补充你的代码
template <int N>
class FixedArray {
    private: 
        int arr[N];
    public:
        FixedArray(){
            for(int i = 0; i < N; i++){
                arr[i] = 0;
            }
        }

        void set(int index, int value){
            if(index >= 0 && index < N){
                arr[index] = value;
            }
            else{
                cout << "Error set: Invalid index!" << endl;
            }
        }

        int access(int index){
            if(index >= 0 && index < N){
                return arr[index];
            }
            else{
                cout << "Error access: Invalid index!" << endl;
                return -1;
            }
        }
};

int main() {
    FixedArray<10> A; // An array with fixed length 10, index must ranges between 0 ... 10 - 1
    int x;

    A.set(-1, 1); // Report an error and do nothing
    A.set(0, 1);  // set A[0]=1;
    x = A.access(0); // return A[0]
    cout << x << endl;
    x = A.access(10); // Report an error and return -1;
    cout << x << endl;

    FixedArray<20> B; // An array with fixed length 20, index must ranges between 0 ... 20 - 1
    for (int i = 0; i < 20; ++i)
    B.set(i, i);
    x = B.access(19);
    cout << x << endl;
    x = B.access(20); // Report an error and return -1;
    cout << x << endl;
    return 0;
}