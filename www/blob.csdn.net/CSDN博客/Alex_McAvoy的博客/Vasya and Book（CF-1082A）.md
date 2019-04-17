# Vasya and Book（CF-1082A） - Alex_McAvoy的博客 - CSDN博客





2018年12月05日 21:52:17[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：76








# Problem Description

Vasya is reading a e-book. The file of the book consists of nn pages, numbered from 1 to n. The screen is currently displaying the contents of page x, and Vasya wants to read the page y. There are two buttons on the book which allow Vasya to scroll dd pages forwards or backwards (but he cannot scroll outside the book). For example, if the book consists of 10 pages, and d=3, then from the first page Vasya can scroll to the first or to the fourth page by pressing one of the buttons; from the second page — to the first or to the fifth; from the sixth page — to the third or to the ninth; from the eighth — to the fifth or to the tenth.

Help Vasya to calculate the minimum number of times he needs to press a button to move to page y.

# Input

The first line contains one integer tt (1≤t≤1031≤t≤103) — the number of testcases.

Each testcase is denoted by a line containing four integers nn, xx, yy, dd (1≤n,d≤1091≤n,d≤109, 1≤x,y≤n1≤x,y≤n) — the number of pages, the starting page, the desired page, and the number of pages scrolled by pressing one button, respectively.

# Output

Print one line for each test.

If Vasya can move from page xx to page yy, print the minimum number of times he needs to press a button to do it. Otherwise print −1−1.

# Examples

**Input**

3

10 4 5 2

5 1 3 4

20 4 19 3

**Output**

4

-1

5

————————————————————————————————————————————

题意：给出一 n 页的笔记本，要从 x 翻到 y 页，每次翻页必须间隔 d 页，最多翻到第一页或最后一页，求从 x 翻到 y 需要的最小次数

思路：首先判断能否从 x 直接翻到 y，再判断能否从第 1 页翻到 y，最后判断能否从第 n 页翻到 y 页，比较三次，记录最小值即可，要注意的是，当 x=y 时，直接输出 0

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int n,x,y,d;
        scanf("%d%d%d%d",&n,&x,&y,&d);

        int step=INF;
        if(x==y){
            step=0;
        }
        else{
            if(abs((y-x))%d==0){
                step=min(step,abs(y-x)/d);
            }
            if((y-1)%d==0){
                int temp;
                if(x%d==0)
                    temp=0;
                else
                    temp=1;
                temp=x/d+(y-1)/d+temp;
                step=min(step,temp);
            }
            if((n-y)%d==0){
                int temp;
                if((n-x)%d==0)
                    temp=0;
                else
                    temp=1;
                temp=(n-x)/d+(n-y)/d+temp;
                step=min(step,temp);
            }

            if(step==INF)
                step=-1;
        }

        printf("%d\n",step);
    }
    return 0;
}
```





