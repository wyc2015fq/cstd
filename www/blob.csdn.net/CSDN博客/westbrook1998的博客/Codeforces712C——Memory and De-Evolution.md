# Codeforces712C——Memory and De-Evolution - westbrook1998的博客 - CSDN博客





2018年09月19日 19:23:00[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27标签：[贪心](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Memory is now interested in the de-evolution of objects, specifically triangles. He starts with an equilateral triangle of side length x, and he wishes to perform operations to obtain an equilateral triangle of side length y.

In a single second, he can modify the length of a single side of the current triangle such that it remains a non-degenerate triangle (triangle of positive area). At any moment of time, the length of each side should be integer.

What is the minimum number of seconds required for Memory to obtain the equilateral triangle of side length y?

Input

The first and only line contains two integers x and y (3 ≤ y < x ≤ 100 000) — the starting and ending equilateral triangle side lengths respectively.

Output

Print a single integer — the minimum number of seconds required for Memory to obtain the equilateral triangle of side length y if he starts with the equilateral triangle of side length x.

Examples

Input

6 3

Output

4

Input

8 5

Output

3

Input

22 4

Output

6

Note

In the first sample test, Memory starts with an equilateral triangle of side length 6 and wants one of side length 3. Denote a triangle with sides a, b, and c as (a, b, c). Then, Memory can do .

In the second sample test, Memory can do .

In the third sample test, Memory can do:
给定x和y求边长为x的等边三角形要经过多少次变换（边次只能变换一条边长度），可以得到边长为y的等边三角形

贪心，一开始是从x变到y正向的，第三个样例没过，想了一下发现要从y逆着，每次排序找最小边，变成最大边且不超过其他两边和，这样循环即可

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
int x,y;
int a[3];
bool cmp(int a,int b){
    return a>b;
}
int main(void){
    scanf("%d%d",&x,&y);
    a[0]=a[1]=a[2]=y;
    int c=0;
    while(!(a[0]==x && a[1]==x && a[2]==x)){
        sort(a,a+3);
        //printf("%d %d %d\n",a[0],a[1],a[2]);
        a[0]=a[1]+a[2]-1 > x ? x :a[1]+a[2]-1;
        c++;
    }
    printf("%d\n",c);
    return 0;
}
```





