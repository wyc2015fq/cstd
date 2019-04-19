# hdu  2846  Repository（字典树） - HJ - CSDN博客
2017年01月25日 17:06:24[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：289
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------字符串---------																[字典树](https://blog.csdn.net/feizaoSYUACM/article/category/7238010)](https://blog.csdn.net/feizaoSYUACM/article/category/7238090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
When you go shopping, you can search in repository for avalible merchandises by the computers and internet. First you give the search system a name about something, then the system responds with the results. Now you are given a lot merchandise names in repository and some queries, and required to simulate the process.
Input 
There is only one case. First there is an integer P (1<=P<=10000)representing the number of the merchanidse names in the repository. The next P lines each contain a string (it’s length isn’t beyond 20,and all the letters are lowercase).Then there is an integer Q(1<=Q<=100000) representing the number of the queries. The next Q lines each contains a string(the same limitation as foregoing descriptions) as the searching condition.
Output 
For each query, you just output the number of the merchandises, whose names contain the search string as their substrings.
Sample Input
20 
ad 
ae 
af 
ag 
ah 
ai 
aj 
ak 
al 
ads 
add 
ade 
adf 
adg 
adh 
adi 
adj 
adk 
adl 
aes 
5 
b 
a 
d 
ad 
s
Sample Output
0 
20 
11 
11 
2
Source 
2009 Multi-University Training Contest 4 - Host by HDU 
题目大意： 
给出一些模式串，再给出几个询问，询问给出的字符串在多少个模式串中出现
比如字符串abc所含的字串有a,ab,abc,b,bc,c
字典树处理前缀出现的次数，所以可将模式串分解，依次插入
需要注意的是对于同一个模式串的不同子串可能有相同的前缀，为了避免多次计算，可以添加字典树节点的信息，添加num记录最后插入的字符串是第num个模式串的子串
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cstring>
using namespace std;
struct tree{
    int id,num;
    tree *child[26];
    tree()
    {
        memset(child,0,sizeof(child));
        id=num=0;
    }
};
tree *root=new tree,*p;
char s[25];
void build(char *str,int k)
{
    p=root;
    for(int i=0;i<strlen(str);i++)
    {
        int m=str[i]-'a';
        if(p->child[m]==NULL)
          p->child[m]=new tree(); 
        p=p->child[m];
        if(p->id!=k)     //如果当前结点的商品ID不等于要插入商品的ID，则计数器num++，并且重新置ID的值 
        {          
            p->num++;
            p->id=k;
        }
    }
}
int find(char *str)
{
    p=root;
    for(int i=0;i<strlen(str);i++)
    {
        int m=str[i]-'a';
        if(!p->child[m])
          return 0;
        p=p->child[m];
    }
    return p->num;
}
int main()
{
    int n,m;
    cin>>n;
    for(int i=0;i<n;i++)
    {
        cin>>s;
        for(int j=0;j<strlen(s);j++)     //将字符串X=X1X2...Xn的分别以X1,X2...Xn开头的后缀字符串插入到Tree树中 
          build(s+j,i+1);
    }
    cin>>m;
    while(m--)
    {
        cin>>s;
        printf("%d\n",find(s));
    }
    return 0;
}
```
