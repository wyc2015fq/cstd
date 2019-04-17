# Sequence Two（HDU-2611） - Alex_McAvoy的博客 - CSDN博客





2018年03月19日 19:50:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：95








# Problem Description

    Search is important in the acm algorithm. When you want to solve a problem by using the search method, try to cut is very important.

Now give you a number sequence, include n (<=100) integers, each integer not bigger than 2^31, you want to find the first P subsequences that is not decrease (if total subsequence W is smaller than P, than just give the first W subsequences). The order of subsequences is that: first order the length of the subsequence. Second order the subsequence by lexicographical. For example initial sequence 1 3 2 the total legal subsequences is 5. According to order is {1}; {2}; {3}; {1,2}; {1,3}. If you also can not understand , please see the sample carefully. 

# Input

    The input contains multiple test cases.

    Each test case include, first two integers n, P. (1<n<=100, 1<p<=100000). 


# Output

    For each test case output the sequences according to the problem description. And at the end of each case follow a empty line.

# Sample Input

**3 5**

**1 3 23 61 3 24 1001 2 3 2**

# Sample Output

**1    2    3    1 2    1 3    1    2    3    1 2    1 3    1    2    3    1 2    1 3    2 2    2 3    1 2 2    1 2 3**


————————————————————————————————————————————

思路：

    首先按照长度输出，然后当长度相同时，按照字典序最小输出（用sort对序列排序），此时，输出的子序列元素位置对应于原序列的位置下标要保证是递增的，所以用结构体来存储值和下标。

    本题还需要注意的问题是，在判重时不能在排好序的数组中判重，应该在原数组中遍历，看是否已经出现过当前数字，因为在排好序的数组中判重，会删去一些合法答案。

# Source Program

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;

int n,p;
int result[101];
int sum;
int step;

struct node
{
    int num;//序列元素
    int postion;//序列元素位置
}order[101];

bool cmp(node a,node b)  //排序
{
    if(a.num==b.num)
        return a.postion<b.postion;
    return a.num<b.num;
}

int dfs(int deep,int postion,int re_postion)
{
    bool flag;
    int temp;
    int i;

    if(deep==step)//当前深度与搜索深度相同时
    {
        for(i=0;i<deep-1;i++)//输出元素
            printf("%d ",result[i]);
        printf("%d\n",result[deep-1]);

        sum++;//输出的序列数+1

        if(sum==p)//记录的序列数与要求序列数相同时
            return 1;
        else
            return 0;
    }

    flag=false;
    for(i=postion;i<=n;i++)
    {
        if(order[i].postion>re_postion)
        {
           if(flag==false)//判重
           {
               flag=true;
               temp=order[i].num;
           }
           else//判重
           {
               if(order[i].num==temp)//若相等，说明有重复
                   continue;
           }

           temp=order[i].num;//若不相等保留当前位置的值
           result[deep]=order[i].num;

           if(dfs(deep+1,i+1,order[i].postion))
               return 1;
        }
    }
    return 0;
}

int main()
{
    int i;

    memset(order,0,sizeof(order));
    memset(result,0,sizeof(result));

    while(scanf("%d%d",&n,&p)!=EOF)
    {
        for(i=1;i<=n;i++)
        {
            scanf("%d",&order[i].num);//输入序列
            order[i].postion=i;//记录序列位置
        }

        sort(order+1,order+n+1,cmp);//对序列进行排序
        sum=0;
        for(i=1;i<n;i++)
        {
            step=i;//记录要输出序列长
            if(dfs(0,1,0))
                break;
        }
        printf("\n");
    }
    return 0;
}
```





