# codeforces-754A-Lesha and array splitting（简单分类处理） - HJ - CSDN博客
2017年01月14日 20:52:46[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：633
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Lesha and array splitting
One spring day on his way to university Lesha found an array A. Lesha likes to split arrays into several parts. This time Lesha decided to split the array A into several, possibly one, new arrays so that the sum of elements in each of the new arrays is not zero. One more condition is that if we place the new arrays one after another they will form the old array A.
Lesha is tired now so he asked you to split the array. Help Lesha! 
Input
The first line contains single integer n (1 ≤ n ≤ 100) — the number of elements in the array A.
The next line contains n integers a1, a2, …, an ( - 103 ≤ ai ≤ 103) — the elements of the array A. 
Output
If it is not possible to split the array A and satisfy all the constraints, print single line containing “NO” (without quotes).
Otherwise in the first line print “YES” (without quotes). In the next line print single integer k — the number of new arrays. In each of the next k lines print two integers li and ri which denote the subarray A[li… ri] of the initial array A being the i-th new array. Integers li, ri should satisfy the following conditions:
```
l1 = 1
rk = n
ri + 1 = li + 1 for each 1 ≤ i < k.
```
If there are multiple answers, print any of them. 
Examples 
Input
3 
1 2 -3
Output
YES 
2 
1 2 
3 3
Input
8 
9 -12 3 4 -4 -10 7 3
Output
YES 
2 
1 2 
3 8
Input
1 
0
Output
NO
Input
4 
1 2 3 -5
Output
YES 
4 
1 1 
2 2 
3 3 
4 4
题目大意： 
    给出一串数，要求把这些数分成若干组，要求每一组上下相连，并且每一组内的各个数之和都不为0
解法： 
    把n个数首先分成n个，然后将数字为0的合并到前面一个数组中，第一个数字为0的话另外考虑
```cpp
#include<iostream>
using namespace std;
int s[100005];
int main()
{
    int n,m;
    while(cin>>n)
    {
        int cnt=0;
        for(int i=0;i<n;i++)
        {
            cin>>s[i];
            if(s[i]==0)
              cnt++;
        }
        if(cnt==n)
          cout<<"NO"<<endl;
        else
        {
            cout<<"YES"<<endl;
            cout<<n-cnt<<endl;
            int i=0,sum=1,blag=0;
            if(s[0]==0)
            {
                for(i=0;i<n;i++)
                  if(s[i]!=0)
                  {
//                    if(n-cnt!=1)
//                      cout<<1<<" "<<i+1<<endl;
                      break;
                  }
                i++;
                for(;i<n;i++)
                {
                    if(s[i]!=0)
                    {
                        cout<<sum<<" "<<i<<endl;
                        sum=i+1;
                    }
                }
                cout<<sum<<" "<<n<<endl;
            }
            else
            {
                for(i=0;i<n;i++)
                {
                    if(s[i]!=0&&i!=0)
                    {
                        cout<<sum<<" "<<i<<endl;
                        sum=i+1;
                    }
                }
                cout<<sum<<" "<<n<<endl;
            }
        }
    }
    return 0;
}
```
