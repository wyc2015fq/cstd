# hdu   1671 Phone List（排序） - HJ - CSDN博客
2017年01月25日 17:02:51[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：231
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Given a list of phone numbers, determine if it is consistent in the sense that no number is the prefix of another. Let’s say the phone catalogue listed these numbers: 
1. Emergency 911 
2. Alice 97 625 999 
3. Bob 91 12 54 26 
In this case, it’s not possible to call Bob, because the central would direct your call to the emergency line as soon as you had dialled the first three digits of Bob’s phone number. So this list would not be consistent.
Input 
The first line of input gives a single integer, 1 <= t <= 40, the number of test cases. Each test case starts with n, the number of phone numbers, on a separate line, 1 <= n <= 10000. Then follows n lines with one unique phone number on each line. A phone number is a sequence of at most ten digits.
Output 
For each test case, output “YES” if the list is consistent, or “NO” otherwise.
Sample Input
2 
3 
911 
97625999 
91125426 
5 
113 
12340 
123440 
12345 
98346
Sample Output
NO 
YES
Source 
2008 “Insigma International Cup” Zhejiang Collegiate Programming Contest - Warm Up（3） 
题目大意： 
 判断输入的电话号码中是否有号码是其他号码的前缀
解题思路： 
先对电话号码序列进行排序，然后判断相邻的两个号码是否符合条件即可
以下多种排序
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<cstring>
using namespace std;
//struct node{
//  char m[105];
//}s1[100005];
char s1[100005][100];
//char *s1[100005];
int cmp(const void* a,const void* b)
{
   return strcmp((char*)a,(char*)b);    
}
int main() 
{
    int t;
    cin>>t;
    while(t--)
    {
        int n,blag=0;
        cin>>n;
        for(int i=0;i<n;i++)
        {
            cin>>s1[i];
//          s1[i]=s[i];
        }
//      sort(s1,s1+n,cmp);
        qsort(s1,n,sizeof(s1[0]),cmp);
        for(int i=0;i<n-1;i++)
        {
            if(strncmp(s1[i],s1[i+1],strlen(s1[i]))==0)
            {
                blag=1;
                break;
            }
        } 
        if(blag)
          cout<<"NO"<<endl;
        else
          cout<<"YES"<<endl;
    } 
    return 0;
}
```
