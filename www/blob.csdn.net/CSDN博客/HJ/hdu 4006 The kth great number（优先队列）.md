# hdu  4006 The kth great number（优先队列） - HJ - CSDN博客
2017年03月31日 22:26:55[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：352
Problem Description 
Xiao Ming and Xiao Bao are playing a simple Numbers game. In a round Xiao Ming can choose to write down a number, or ask Xiao Bao what the kth great number is. Because the number written by Xiao Ming is too much, Xiao Bao is feeling giddy. Now, try to help Xiao Bao.
Input 
There are several test cases. For each test case, the first line of input contains two positive integer n, k. Then n lines follow. If Xiao Ming choose to write down a number, there will be an ” I” followed by a number that Xiao Ming will write down. If Xiao Ming choose to ask Xiao Bao, there will be a “Q”, then you need to output the kth great number.
Output 
The output consists of one integer representing the largest number of islands that all lie on one line.
Sample Input
8 3 
I 1 
I 2 
I 3 
Q 
I 5 
Q 
I 4 
Q
Sample Output
1 
2 
3
Hint 
Xiao  Ming  won’t  ask  Xiao  Bao  the  kth  great  number  when  the  number  of  the  written number is smaller than k. (1=< k<=n<=1000000).
```cpp
#include<iostream> 
#include<queue>
using namespace std;
int main()
{
    int n,k,y,x;
    char c;
    while(cin>>n>>k)
    {
        priority_queue<int,vector<int>,greater<int> >q;  //按数的值从大到小排序 
        while(n--)
        {
            cin>>c;
            if(c=='I')
            {
                cin>>x;
                if(q.size()==k)    //如果队列里面已经有k个数 
                {
                    y=q.top();     //取队头，也就是第k大的数 
                    if(x>y)            //如果输入的数x比第k个数大 
                    {
                        q.pop();     //队头元素出队 
                        q.push(x);     //将输入的数x元素压入队中 
                    }
                }
                else
                  q.push(x);     //如果不够k个数，就一直压入队中 
            }
            else
              cout<<q.top()<<endl;      //否则只要每次输出队头元素即可 
        }
    }
    return 0;
}
```
