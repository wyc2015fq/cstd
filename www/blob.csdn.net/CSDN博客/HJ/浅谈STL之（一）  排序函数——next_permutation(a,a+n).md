# 浅谈STL之（一）   排序函数——next_permutation(a,a+n) - HJ - CSDN博客
2016年08月02日 18:48:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：468
首先来看一个题：
POJ   1731  Orders
Description 
The stores manager has sorted all kinds of goods in an alphabetical order of their labels. All the kinds having labels starting with the same letter are stored in the same warehouse (i.e. in the same building) labelled with this letter. During the day the stores manager receives and books the orders of goods which are to be delivered from the store. Each order requires only one kind of goods. The stores manager processes the requests in the order of their booking. 
You know in advance all the orders which will have to be processed by the stores manager today, but you do not know their booking order. Compute all possible ways of the visits of warehouses for the stores manager to settle all the demands piece after piece during the day. 
Input 
Input contains a single line with all labels of the requested goods (in random order). Each kind of goods is represented by the starting letter of its label. Only small letters of the English alphabet are used. The number of orders doesn’t exceed 200. 
Output 
Output will contain all possible orderings in which the stores manager may visit his warehouses. Every warehouse is represented by a single small letter of the English alphabet – the starting letter of the label of the goods. Each ordering of warehouses is written in the output file only once on a separate line and all the lines containing orderings have to be sorted in an alphabetical order (see the example). No output will exceed 2 megabytes. 
Sample Input 
bbjd
Sample Output 
bbdj 
bbjd 
bdbj 
bdjb 
bjbd 
bjdb 
dbbj 
dbjb 
djbb 
jbbd 
jbdb 
jdbb
解析：这个题的主要意思其实可以从输入输出的结果来看，其实就是给你一个长度不超过10的字符串，要求你输出按ASCII码从小到大依次排列的总的个数每一个情况输出占一行。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
int main()
{
    char s[210];
    int len;
    cin>>s;
    len=strlen(s);
    sort(s,s+len);
    do{
        cout<<s<<endl;
    }while(next_permutation(s,s+len));    //把所有排列情况表示出来并且输出
    return 0;
}
```
通过以上应该对next_permutation函数有所了解了吧，下面这道题，虽然表面上看不太出来，但实际上如果用到next_permutation函数，枚举暴力解决起来要轻松的多。
POJ   3187   Backward Digit Sums
Description 
FJ and his cows enjoy playing a mental game. They write down the numbers from 1 to N (1 <= N <= 10) in a certain order and then sum adjacent numbers to produce a new list with one fewer number. They repeat this until only a single number is left. For example, one instance of the game (when N=4) might go like this: 
```
3   1   2   4
  4   3   6
    7   9
     16
```
Behind FJ’s back, the cows have started playing a more difficult game, in which they try to determine the starting sequence from only the final total and the number N. Unfortunately, the game is a bit above FJ’s mental arithmetic capabilities. 
Write a program to help FJ play the game and keep up with the cows.
Input 
Line 1: Two space-separated integers: N and the final sum.
Output 
Line 1: An ordering of the integers 1..N that leads to the given sum. If there are multiple solutions, choose the one that is lexicographically least, i.e., that puts smaller numbers first.
Sample Input 
4 16
Sample Output 
3 1 2 4
Hint 
Explanation of the sample:  
There are other possible sequences, such as 3 2 1 4, but 3 1 2 4 is the lexicographically smallest.
解析：在暴力过程中需要全排列，但是如何把所有排列表示出来呢，这个时候就需要next_permutation函数出马了。
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
int main()
{
    int t,n;
    while(cin>>t>>n)
    {
        int a[11],b[11];
        for(int i=0;i<t;i++)
          a[i]=i+1;
        do{
            for(int i=0;i<t;i++)
              b[i]=a[i];
            for(int i=0;i<t;i++)
              for(int j=0;j<t-i-1;j++)
                b[j]=b[j]+b[j+1];
            if(b[0]==n)  break;
        }while(next_permutation(a,a+t));            //枚举出每一个排列的情况       
        for(int i=0;i<t-1;i++)
          cout<<a[i]<<" ";
        cout<<a[t-1]<<endl;
    }
    return 0;
```
