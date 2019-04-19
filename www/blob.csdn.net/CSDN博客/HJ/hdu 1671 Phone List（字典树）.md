# hdu  1671  Phone List（字典树） - HJ - CSDN博客
2017年01月25日 17:00:55[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：222
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------字符串---------																[字典树](https://blog.csdn.net/feizaoSYUACM/article/category/7238010)](https://blog.csdn.net/feizaoSYUACM/article/category/7238090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
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
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
using namespace std;
const int maxn=10;
int blag;
typedef struct node{
    int cnt;
    struct node *child[maxn]; 
}Trie;
void insert(Trie *root,char *ss)
{
    Trie *p=root;
    while(*ss!='\0')
    {
        if(p->child[*ss-'0']==NULL)
        {
            Trie *temp=(Trie *)malloc(sizeof(Trie));
            temp->cnt=0;
            for(int i=0;i<maxn;i++) 
              temp->child[i]=NULL;
            p->child[*ss-'0']=temp;
        }
        if(p->cnt)
          blag=1;
        p=p->child[*ss-'0'];
        ss++;
    }
    p->cnt=1;
    for(int i=0;i<maxn;i++)
    {
        if(p->child[i]!=NULL)
        {
            blag=1;
            break;
        }
    }
}
void del(Trie *root)
{
    for(int i=0;i<maxn;i++)
      if(root->child[i]!=NULL)
        del(root->child[i]);
    free(root); 
}
int main()
{
    int t,n;
    cin>>t;
    while(t--)
    {
        cin>>n;
        char s[11];
        Trie *root=(Trie *)malloc(sizeof(node));
        root->cnt=0;
        blag=0;
        for(int i=0;i<maxn;i++)
          root->child[i]=NULL;
        for(int i=0;i<n;i++)
        {
            cin>>s;
            if(!blag)
              insert(root,s);
        } 
        if(!blag)
          cout<<"YES"<<endl;
        else
          cout<<"NO"<<endl;
        del(root);
    }
    return 0;
}
```
