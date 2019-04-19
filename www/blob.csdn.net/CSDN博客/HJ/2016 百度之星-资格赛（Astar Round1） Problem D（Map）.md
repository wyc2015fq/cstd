# 2016"百度之星" - 资格赛（Astar Round1）  Problem D（Map） - HJ - CSDN博客
2017年08月03日 08:54:16[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：284
个人分类：[====ACM====																[---------OJ---------																[HDU																[--------数据结构--------																[STL](https://blog.csdn.net/feizaoSYUACM/article/category/6308005)](https://blog.csdn.net/feizaoSYUACM/article/category/6130307)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
度熊所居住的 D 国，是一个完全尊重人权的国度。以至于这个国家的所有人命名自己的名字都非常奇怪。一个人的名字由若干个字符组成，同样的，这些字符的全排列的结果中的每一个字符串，也都是这个人的名字。例如，如果一个人名字是 ACM，那么 AMC， CAM， MAC， MCA， 等也都是这个人的名字。在这个国家中，没有两个名字相同的人。
度熊想统计这个国家的人口数量，请帮助度熊设计一个程序，用来统计每一个人在之前被统计过多少次。
Input 
这里包括一组测试数据，第一行包含一个正整数N，接下来的N 行代表了 N 个名字。N 不会超过100,000，他们的名字不会超过40位.
Output 
对于每输入的一个人名，输出一个整数，代表这个人之前被统计了多少次。
Sample Input
5 
ACM 
MAC 
BBA 
ACM 
BAB
Sample Output
0 
1 
0 
2 
1
每个字符串排序之后用map就行了，之前还想着用字典树，没想到超时了。
诶。还是太菜了~
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<map>
using namespace std;
map<string,int> M;
char s[505];
int main()
{
    int n;
    while(cin>>n)
    {
        while(n--)
        {
            cin>>s;
            sort(s,s+strlen(s));
            cout<<M[s]++<<endl;
        }
    }
    return 0;
}
```
