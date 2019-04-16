# HDU 6186 CS Course - 紫芝的博客 - CSDN博客





2018年05月12日 10:06:07[紫芝](https://me.csdn.net/qq_40507857)阅读数：39













# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=6186)

# CS Course
**Time Limit: 4000/2000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 1699    Accepted Submission(s): 726**



## Problem Description


Little A has come to college and majored in Computer and Science.

Today he has learned bit-operations in Algorithm Lessons, and he got a problem as homework.

Here is the problem:

You are giving n non-negative integers 12, and some queries.

A query only contains a positive integer p, which means you 
are asked to answer the result of bit-operations (and, or, xor) of all the integers except .






## Input


There are no more than 15 test cases. 

Each test case begins with two positive integers n and p
in a line, indicate the number of positive integers and the number of queries.

2105

Then n non-negative integers 12 follows in a line, 0109 for each i in range[1,n].

After that there are q positive integers 12in q lines, 1 for each i in range[1,q].





## Output


For each query p, output three non-negative integers indicates the result of bit-operations(and, or, xor) of all non-negative integers except  in a line.





Sample Input


3 31 1 1123




Sample Output


1 1 01 1 01 1 0





Source

[2017ACM/ICPC广西邀请赛-重现赛（感谢广西大学）](http://acm.hdu.edu.cn/search.php?field=problem&key=2017ACM%2FICPC%B9%E3%CE%F7%D1%FB%C7%EB%C8%FC-%D6%D8%CF%D6%C8%FC%A3%A8%B8%D0%D0%BB%B9%E3%CE%F7%B4%F3%D1%A7%A3%A9&source=1&searchmode=source)




Recommend

liuyiding   |   We have carefully selected several similar problems for you:  [6275](http://acm.hdu.edu.cn/showproblem.php?pid=6275)[6274](http://acm.hdu.edu.cn/showproblem.php?pid=6274)[6273](http://acm.hdu.edu.cn/showproblem.php?pid=6273)[6272](http://acm.hdu.edu.cn/showproblem.php?pid=6272)[6271](http://acm.hdu.edu.cn/showproblem.php?pid=6271)



## 题意：

给定n个数，然后是q次询问，每次询问为去掉第i个数，输出剩下n-1个数与、或、异或的结果

## 解法1：

前缀数组和后缀数组

用p1[i]表示 [1,i] 所有数与的结果，b1[i]表示 [i,n] 所有数与的结果




```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+5;
int a[maxn];
int p1[maxn],b1[maxn];//and
int p2[maxn],b2[maxn];//or
int p3[maxn],b3[maxn];//xor
int main()
{
	ios::sync_with_stdio(0);
	int n,q,p;
	while(cin>>n>>q){
	    cin>>a[1];
	    p1[1]=p2[1]=p3[1]=a[1];
        for(int i=2;i<=n;i++)
        {
            cin>>a[i];

            p1[i]=p1[i-1]&a[i];
            p2[i]=p2[i-1]|a[i];
            p3[i]=p3[i-1]^a[i];
        }
        b1[n]=b2[n]=b3[n]=a[n];
        for(int i=n-1;i>=1;i--)
        {

            b1[i]=b1[i+1]&a[i];
            b2[i]=b2[i+1]|a[i];
            b3[i]=b3[i+1]^a[i];
        }
        while(q--){
            cin>>p;
            if(p==1)
                printf("%d %d %d\n",b1[p+1],b2[p+1],b3[p+1]);
            else if(p==n)
                printf("%d %d %d\n",p1[p-1],p2[p-1],p3[p-1]);
            else
                printf("%d %d %d\n",p1[p-1]&b1[p+1],p2[p-1]|b2[p+1],p3[p-1]^b3[p+1]);
        }
	}
	return 0;
}
```

## 解法2：

用num数组记录n个数转为二进制后，每一位上1的个数之和，

当去掉一个数后，更新num数组，然后遍历num数组

如果num[i]==n-1，那么这一位与的结果为1，否则为0（与运算：全1为1，有0为0）

如果num[i]>=1，那么这一位或的结果为1，否则为0，（或运算：全0为0，有1为1）


对于异或运算：

先让n个数异或的结果为XOR，然后XOR和去掉的数字异或，结果就是n-1个数字异或的结果（异或运算的逆运算）




```cpp
#include<bits/stdc++.h>
#include<bitset>
using namespace std;
const int maxn=1e5+5;
int a[maxn];
int num[32];

int main()
{
	ios::sync_with_stdio(0);
	int n,q,p;

	while(cin>>n>>q){
            int XOR=0;
            fill(num,num+32,0);

        for(int i=1;i<=n;i++)
        {
            cin>>a[i];
            XOR^=a[i];
            bitset<32>bit(a[i]);
            for(int j=0;j<32;j++)
                if(bit[j]==1)
                num[j]++;
        }

        while(q--){
            cin>>p;
            bitset<32>bit(a[p]);
            bitset<32>AND;//n-1个数字与的结果
            bitset<32>OR;//n-1个数字或的结果
           for(int j=0;j<32;j++)
           {
               if(num[j]-bit[j]==(n-1))//这一位有n-1个1，与的结果为1
                    AND.set(j);//把第j位置1
                else
                    AND.reset(j);//把第j位置0

                if(num[j]-bit[j]>=1)//这一位上有1，或的结果为1
                    OR.set(j);
                else
                    OR.reset(j);
           }
           printf("%d %d %d\n",AND,OR,XOR^a[p]);
        }
	}
	return 0;
}
```




