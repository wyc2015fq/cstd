# codeforces 337A - Puzzles - westbrook1998的博客 - CSDN博客





2018年04月20日 23:02:14[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：53








简单的题目，不过觉得还是有一点价值，记录一下 

题目：

> 
A. Puzzles 

  time limit per test1 second 

  memory limit per test256 megabytes 

  inputstandard input 

  outputstandard output 

  The end of the school year is near and Ms. Manana, the teacher, will soon have to say goodbye to a yet another class. She decided to prepare a goodbye present for her n students and give each of them a jigsaw puzzle (which, as wikipedia states, is a tiling puzzle that requires the assembly of numerous small, often oddly shaped, interlocking and tessellating pieces).
The shop assistant told the teacher that there are m puzzles in the shop, but they might differ in difficulty and size. Specifically, the first jigsaw puzzle consists of f1 pieces, the second one consists of f2 pieces and so on.

Ms. Manana doesn’t want to upset the children, so she decided that the difference between the numbers of pieces in her presents must be as small as possible. Let A be the number of pieces in the largest puzzle that the teacher buys and B be the number of pieces in the smallest such puzzle. She wants to choose such n puzzles that A - B is minimum possible. Help the teacher and find the least possible value of A - B.

Input 

  The first line contains space-separated integers n and m (2 ≤ n ≤ m ≤ 50). The second line contains m space-separated integers f1, f2, …, fm (4 ≤ fi ≤ 1000) — the quantities of pieces in the puzzles sold in the shop.

Output 

  Print a single integer — the least possible difference the teacher can obtain.

Examples 

  inputCopy 

  4 6 

  10 12 10 7 5 22 

  output 

  5
题意： 

给一个整数数组m个数，从中挑n个，使得最大值减最小值的差值最小 

先将数组排序，然后依次比较相邻n-2的元素的差值大小，找出最小值即可
代码：

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int main(void){
    int n,m;
    while(scanf("%d %d",&n,&m)!=EOF){
        int a[m];
        for(int i=0;i<m;i++){
            scanf("%d",a+i);
        }
        sort(a,a+m);
        int min=a[n-1]-a[0];
        for(int i=1;i<m-n+1;i++){
            int t=a[i+n-1]-a[i];
            if(t<min){
                min=t;
            }
        }
        printf("%d\n",min);
    }
    return 0;
}
```







