# Vasya and Books（CF-1073B） - Alex_McAvoy的博客 - CSDN博客





2018年10月31日 20:39:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：215








> 
# Problem Description

Vasya has got nn books, numbered from 1 to n, arranged in a stack. The topmost book has number a1, the next one — a2, and so on. The book at the bottom of the stack has number anan. All numbers are distinct.

Vasya wants to move all the books to his backpack in nn steps. During i-th step he wants to move the book number bibi into his backpack. If the book with number bibi is in the stack, he takes this book and all the books above the book bibi, and puts them into the backpack; otherwise he does nothing and begins the next step. For example, if books are arranged in the order [1,2,3] (book 1 is the topmost), and Vasya moves the books in the order [2,1,3], then during the first step he will move two books (1 and 2), during the second step he will do nothing (since book 1 is already in the backpack), and during the third step — one book (the book number 33). Note that b1,b2,…,bn are distinct.

Help Vasya! Tell him the number of books he will put into his backpack during each step.

# Input

The first line contains one integer n (1≤n≤2⋅105)— the number of books in the stack.

The second line contains nn integers a1,a2,…,an (1≤ai≤n) denoting the stack of books.

The third line contains nn integers b1,b2,…,bn (1≤bi≤n) denoting the steps Vasya is going to perform.

All numbers a1…an are distinct, the same goes for b1…bn.

# Output

Print n integers. The i-th of them should be equal to the number of books Vasya moves to his backpack during the i-th step.

# Examples

**Input**

3

1 2 3

2 1 3

**Output**

2 0 1 

**Input**

5

3 1 4 2 5

4 5 1 3 2

**Output**

3 2 0 0 0 

**Input**

6

6 5 4 3 2 1

6 5 3 4 2 1

**Output**

1 1 2 0 1 1 


题意：给出 n 本书，以及这些书从上到下的编号，每次只能从上到下依次取书存入书包，假设要取编号为 i 的书，那么其上的所有书都算存入书包，现在要按顺序取这些书，求取每本书花费的次数，如果书以及在书包中，则输出 0

思路：栈的基本使用，先将数据倒序存入栈中，首先判断书是否已在书包内，若在直接输出 0，然后开始下一轮取书，否则依次取栈顶元素并进行标记，如果栈顶元素与要取的书相同，则输出计数器 cnt 并终止取书，继续下一轮取书

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 200001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
int n;
int a[N];
int vis[N];
stack<int> S;
int main(){
    cin>>n;
    for(int i=1;i<=n;i++)
        cin>>a[i];
    for(int i=n;i>=1;i--)//倒序存入栈中，以保证栈中元素顺序与所给顺序相同
        S.push(a[i]);

    for(int i=1;i<=n;i++){//取书n轮
        int cnt=0;
        int x;
        cin>>x;

        if(vis[x]==1){//如果书已经在包中
            cout<<"0 ";
            continue;
        }

        while(true){//循环取书，直到取到为止
            if(S.empty()){
                cout<<"0 ";
                break;
            }

            int temp=S.top();//取栈顶元素
            S.pop();
            cnt++;
            vis[temp]=1;//存入书包，进行标记

            if(temp==x){//如果相同，输出次数
                cout<<cnt<<" ";
                break;
            }
        }
    }
    return 0;
}
```





