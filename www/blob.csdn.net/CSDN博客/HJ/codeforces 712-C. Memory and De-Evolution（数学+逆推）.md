# codeforces 712-C. Memory and De-Evolution（数学+逆推） - HJ - CSDN博客
2017年07月12日 11:25:17[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：353
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
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
In the first sample test, Memory starts with an equilateral triangle of side length 6 and wants one of side length 3. Denote a triangle with sides a, b, and c as (a, b, c). Then, Memory can do ![这里写图片描述](http://codeforces.com/predownloaded/78/23/7823c31c27139a382b61aee582fd451f8c07905b.png).
In the second sample test, Memory can do ![这里写图片描述](http://codeforces.com/predownloaded/22/b2/22b2088d2e869848a78af18576dbfab0233bdbc2.png).
In the third sample test, Memory can do:![这里写图片描述](http://codeforces.com/predownloaded/59/f9/59f9f1033e37dcff494a7de4ca5155e95e34da43.png)![](http://codeforces.com/predownloaded/7b/1a/7b1a1db6a7ff1635c9d24429cfe7208048f6d0f2.png)
题目大意： 
 有边长为x的等边三角形,Memory想要将其变成边长为y的等边三角形。现规定Memory每秒能够改变一条边的大小,但要保证改变后的三条边仍能构成一个三角形。
请问最少需要多少秒才能变为边长为y的等边三角形？ 
解题思路： 
逆推法：如何从边长为b用尽量少的次数变到边长为a呢？（假设a足够大）
第一次三边长：  2*b-1  b  b 
第二次三边长：  2*b-1  3*b-2  b 
第三次三边长：  2*b-1  3*b-2  5*b-4 
第四次三边长：  8*b-7  3*b-2  5*b-4
通过观察可以发现一串数字： 2，3，5，8，….（Fibonacci）.
因此，先打好表，然后每次循环确定好范围就行了。
```cpp
#include<iostream>
using namespace std;
int d[30];
int main()
{
    d[0]=1,d[1]=1;
    for(int i=2;i<30;i++)
      d[i]=d[i-1]+d[i-2];
    int a,b;
    while(cin>>a>>b)
    {
        int p=2;
        while(b*d[p]-(d[p]-1)<a)
          p++;
        cout<<p+1<<endl;
    }
    return 0;
}
```
