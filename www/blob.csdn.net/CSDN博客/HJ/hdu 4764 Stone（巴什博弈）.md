# hdu  4764  Stone（巴什博弈） - HJ - CSDN博客
2016年08月12日 21:03:52[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：408
Problem Description 
Tang and Jiang are good friends. To decide whose treat it is for dinner, they are playing a game. Specifically, Tang and Jiang will alternatively write numbers (integers) on a white board. Tang writes first, then Jiang, then again Tang, etc… Moreover, assuming that the number written in the previous round is X, the next person who plays should write a number Y such that 1 <= Y - X <= k. The person who writes a number no smaller than N first will lose the game. Note that in the first round, Tang can write a number only within range [1, k] (both inclusive). You can assume that Tang and Jiang will always be playing optimally, as they are both very smart students.
Input 
There are multiple test cases. For each test case, there will be one line of input having two integers N (0 < N <= 10^8) and k (0 < k <= 100). Input terminates when both N and k are zero.
Output 
For each case, print the winner’s name in a single line.
Sample Input 
1 1 
30 3 
10 2 
0 0
Sample Output 
Jiang 
Tang 
Jiang
简单博弈！ 
分析题目，数目总共n个，两个人都只能拿1~k个，那么可以保证无论先手第一次拿多少，后手拿了一次之后第一轮总共拿了k+1个，只要n-1是k+1的倍数，那么先手无论如何都可以赢，反之就会输，代码如下：
```cpp
#include<iostream>
using namespace std;
int main()
{
    int n,k;
    while(cin>>n>>k)
    {
        if(n==0&&k==0)
          break;
        n--;          
        k++;       //每次先手在1~k之内取任意数，后手都能保持这一轮取的数字为k+1 
        if(n%k==0)         //先手取到n-1则先手败 
          cout<<"Jiang"<<endl;
        else              //否则先手赢 
          cout<<"Tang"<<endl;
    }
    return 0;
}
```
