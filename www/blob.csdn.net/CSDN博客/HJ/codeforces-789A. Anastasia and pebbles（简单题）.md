# codeforces-789A. Anastasia and pebbles（简单题） - HJ - CSDN博客
2017年03月30日 13:35:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：1053
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Anastasia loves going for a walk in Central Uzhlyandian Park. But she became uninterested in simple walking, so she began to collect Uzhlyandian pebbles. At first, she decided to collect all the pebbles she could find in the park.
She has only two pockets. She can put at most k pebbles in each pocket at the same time. There are n different pebble types in the park, and there are wi pebbles of the i-th type. Anastasia is very responsible, so she never mixes pebbles of different types in same pocket. However, she can put different kinds of pebbles in different pockets at the same time. Unfortunately, she can’t spend all her time collecting pebbles, so she can collect pebbles from the park only once a day.
Help her to find the minimum number of days needed to collect all the pebbles of Uzhlyandian Central Park, taking into consideration that Anastasia can’t place pebbles of different types in same pocket. 
Input
The first line contains two integers n and k (1 ≤ n ≤ 105, 1 ≤ k ≤ 109) — the number of different pebble types and number of pebbles Anastasia can place in one pocket.
The second line contains n integers w1, w2, …, wn (1 ≤ wi ≤ 104) — number of pebbles of each type. 
Output
The only line of output contains one integer — the minimum number of days Anastasia needs to collect all the pebbles. 
Examples 
Input
3 2 
2 3 4
Output
3
Input
5 4 
3 1 8 9 7
Output
5
Note
In the first sample case, Anastasia can collect all pebbles of the first type on the first day, of second type — on the second day, and of third type — on the third day.
Optimal sequence of actions in the second sample case:
```
In the first day Anastasia collects 8 pebbles of the third type.
In the second day she collects 8 pebbles of the fourth type.
In the third day she collects 3 pebbles of the first type and 1 pebble of the fourth type.
In the fourth day she collects 7 pebbles of the fifth type.
In the fifth day she collects 1 pebble of the second type.
```
题目大意： 
      有n堆种类各不相同的东西需要移动位置，现在一件衣服上面有两个口袋，每个口袋最多放k件东西，并且要求一个口袋里面的东西必须是种类相同的。问最少需要的移动次数是多少？
解题思路： 
    贪心一下，如果一个口袋不够放，就移到后面去，先把能放满的此时累加。
```cpp
#include<iostream>
using namespace std;
int main()
{
    long long n,k;
    while(cin>>n>>k)
    {
        long long sum=0,x;
        for(int i=0;i<n;i++)
        {
            cin>>x;
            sum+=(x+k-1)/k;    //如果一个口袋不够放就需要再加一个口袋，结果向上取整 
        }
        cout<<(sum+1)/2<<endl;    //如果不能整除2就需要多走一趟 
    }
    return 0;
}
```
