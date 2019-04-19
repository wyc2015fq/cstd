# hdu  1501  Zipper（dfs） - HJ - CSDN博客
2017年06月29日 18:19:54[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：206
个人分类：[====ACM====																[-----------搜索-----------																[DFS																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6317663)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Problem Description 
Given three strings, you are to determine whether the third string can be formed by combining the characters in the first two strings. The first two strings can be mixed arbitrarily, but each must stay in its original order.
For example, consider forming “tcraete” from “cat” and “tree”:
String A: cat 
String B: tree 
String C: tcraete
As you can see, we can form the third string by alternating characters from the two strings. As a second example, consider forming “catrtee” from “cat” and “tree”:
String A: cat 
String B: tree 
String C: catrtee
Finally, notice that it is impossible to form “cttaree” from “cat” and “tree”.
Input 
The first line of input contains a single positive integer from 1 through 1000. It represents the number of data sets to follow. The processing for each data set is identical. The data sets appear on the following lines, one data set per line.
For each data set, the line of input consists of three strings, separated by a single space. All strings are composed of upper and lower case letters only. The length of the third string is always the sum of the lengths of the first two strings. The first two strings will have lengths between 1 and 200 characters, inclusive.
Output 
For each data set, print:
Data set n: yes
if the third string can be formed from the first two, or
Data set n: no
if it cannot. Of course n should be replaced by the data set number. See the sample output below for an example.
Sample Input
3 
cat tree tcraete 
cat tree catrtee 
cat tree cttaree
Sample Output
Data set 1: yes 
Data set 2: yes 
Data set 3: no
题目大意：给出三个串，其中c串是a串和b串的串长之和，现在由你来判断，在不改变a串和b串每个字符前后顺序的前提下，能不能组成c串？
解题思路：抓住一个关键点，最后c串是a串和b串的串长之和，可以用dfs，最后跳出的条件就是c串直到最后的一个字符要么是a串的最后一个字符，要么是b串的最后一个字符。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring> 
using namespace std;
char a[1005],b[1005],c[2010];
int aa,bb,cc;
int vis[1005][1005];
int blag;
void dfs(int x,int y,int z)    //x,y,z分别表示a、b、c串的第x,y,z个位置
{
    if(blag||vis[x][y])
      return ;
    if(z==cc)        
    {
        blag=1;
        return ;
    }
    vis[x][y]=1;
    if(x<aa&&a[x]==c[z])     //a串的第x个字符跟c串的第z个字符相等 
      dfs(x+1,y,z+1);
    if(y<bb&&b[y]==c[z])      //b串的第y个字符跟c串的第z个字符相等 
      dfs(x,y+1,z+1);
}
int main()
{
    int t,tt=1;
    cin>>t;
    while(t--)
    {
        cin>>a>>b>>c;
        aa=strlen(a),bb=strlen(b),cc=strlen(c);
        memset(vis,0,sizeof(vis));
        blag=0;
        dfs(0,0,0);
        printf("Data set %d: ",tt++);
        if(blag)
          printf("yes\n");
        else
          printf("no\n");
    }
    return 0;
}
```
