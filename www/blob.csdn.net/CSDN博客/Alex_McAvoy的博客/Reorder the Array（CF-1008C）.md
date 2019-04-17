# Reorder the Array（CF-1008C） - Alex_McAvoy的博客 - CSDN博客





2018年08月23日 20:02:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：53
个人分类：[CodeForces																[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

You are given an array of integers. Vasya can permute (change order) its integers. He wants to do it so that as many as possible integers will become on a place where a smaller integer used to stand. Help Vasya find the maximal number of such integers.

For instance, if we are given an array [10,20,30,40], we can permute it so that it becomes [20,40,10,30]. Then on the first and the second positions the integers became larger (20>10, 40>20) and did not on the third and the fourth, so for this permutation, the number that Vasya wants to maximize equals 2. Read the note for the first example, there is one more demonstrative test case.

Help Vasya to permute integers in such way that the number of positions in a new array, where integers are greater than in the original one, is maximal.

# Input

The first line contains a single integer n (1≤n≤105) — the length of the array.

The second line contains nn integers a1,a2,…,an (1≤ai≤109) — the elements of the array.

# Output

Print a single integer — the maximal number of the array's elements which after a permutation will stand on the position where a smaller element stood in the initial array.

# Examples

**Input**

7

10 1 1 1 5 5 3

**Output**

4

**Input**

5

1 1 1 1 1

**Output**

0


题意：给出 n 个数，要求重新排序后的当前位置上的数要大于排序前的这个位置上的数，求最多有多少个这样的数

思路：贪心，用优先队列不断判断统计即可

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
#define N 1001
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
priority_queue <int,vector<int>,greater<int>> q;
LL a[100010];
int main()
{
    int n,num=1,ans=0;
    cin>>n;
    for(int i=1;i<=n;i++)
    {
        cin>>a[i];
        q.push(a[i]);
    }

    sort(a+1,a+1+n);

    for(int i=1;i<=n;i++)
        if(a[i]==a[1])
            q.pop();
    
    if(q.empty())
    {
        cout<<0<<endl;
        return 0;
    }

    for(int i=1;i<=n&&q.size()!=0;i++)
    {
        if(q.top()>a[i])
        {
            ans++;
            q.pop();
        }
        else 
        {
            q.pop();
            i--;
        }
    }
    cout<<ans<<endl;
    return 0;
}
```






