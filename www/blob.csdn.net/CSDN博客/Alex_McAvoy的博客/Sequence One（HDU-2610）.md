# Sequence One（HDU-2610） - Alex_McAvoy的博客 - CSDN博客





2018年03月19日 19:35:51[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：92








# Problem Description

    Search is important in the acm algorithm. When you want to solve a problem by using the search method, try to cut is very important.

    Now give you a number sequence, include n (<=1000) integers, each integer not bigger than 2^31, you want to find the first P subsequences that is not decrease (if total subsequence W is smaller than P, than just give the first W subsequences). The order of subsequences is that: first order the length of the subsequence. Second order the sequence of each integer’s position in the initial sequence. 

    For example initial sequence 1 3 2 the total legal subsequences is 5. According to order is {1}; {3}; {2}; {1,3}; {1,2}. {1,3} is first than {1,2} because the sequence of each integer’s position in the initial sequence are {1,2} and {1,3}. {1,2} is smaller than {1,3}. If you also can not understand , please see the sample carefully. 

# Input

    The input contains multiple test cases.

    Each test case include, first two integers n, P. (1<n<=1000, 1<p<=10000). 

# Output

    For each test case output the sequences according to the problem description. And at the end of each case follow a empty line.

# Sample Input

**3 5**

**    1 3 2    3 6    1 3 2    4 100    1 2 3 2**

# Sample Output

**1    3    2    1 3    1 2    1    3    2    1 3    1 2    1    2    3    1 2    1 3    2 3    2 2    1 2 3    1 2 2**

————————————————————————————————————————————

思路：枚举出所有从1~n中选出的非递减的按照字典序的前P个数，需要判重

# Source Program

```cpp
#include<iostream>
using namespace std;

struct number
{
    int num;
    int postion;
}vis[1001];

int n,p;
int a[1001];
bool flag;
int work,length;
bool t;

void dfs(int now_length,int now_postion)
{
    int i,j;

    if(work>=p)//搜索时的子序列长度大于所要求的长度,终止搜索
        return;
    if(now_length==length)//当前子序列长度与搜索要求的长度相等时
    {
        work++;//搜索的子序列长度+1
        flag=true;
        for(i=0;i<now_length-1;i++)//输出当前子序列长度的元素
            cout<<vis[i].num<<" ";
        cout<<vis[now_length-1].num<<endl;
        return;
    }

    for(i=now_postion;i<n;i++)
    {
        t=false;
        if((now_length!=0&&vis[now_length-1].num<=a[i])||now_length==0)
        {
            /*比较元素值*/
            if(now_length!=0)
            {
                for(j=vis[now_length-1].postion+1;j<i;j++)
                    if(a[j]==a[i])
                    {
                        t=true;
                        continue;
                    }
                if(t)
                    continue;
            }
            else
            {
                for(j=0;j<i;j++)
                    if(a[j]==a[i])
                    {
                        t=true;
                        continue;
                    }
                if(t)
                    continue;
            }

            /*符合条件时，记录*/
            vis[now_length].num=a[i];//记录元素
            vis[now_length].postion=i;//记录位置

            dfs(now_length+1,i+1);//向下扩展继续搜索
        }
    }
}


int main()
{
    int i;

    while(cin>>n>>p)
    {
        for(i=0;i<n;i++)
            cin>>a[i];
        work=0;//work用于记录搜索时的子序列长度
        for(i=1;i<n;i++)
        {
            flag=false;
            length=i;
            dfs(0,0);
            if(work>=p||(!flag))//当搜索时的子序列长度大于所要求的长度时，终止
                break;
        }
        cout<<endl;
    }
    return 0;
}
```





