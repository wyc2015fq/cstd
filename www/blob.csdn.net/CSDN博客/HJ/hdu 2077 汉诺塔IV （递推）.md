# hdu  2077 汉诺塔IV  （递推） - HJ - CSDN博客
2017年04月28日 13:03:46[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：469
Problem Description 
还记得汉诺塔III吗？他的规则是这样的：不允许直接从最左(右)边移到最右(左)边(每次移动一定是移到中间杆或从中间移出)，也不允许大盘放到小盘的上面。xhd在想如果我们允许最大的盘子放到最上面会怎么样呢？（只允许最大的放在最上面）当然最后需要的结果是盘子从小到大排在最右边。
Input 
输入数据的第一行是一个数据T，表示有T组数据。 
每组数据有一个正整数n(1 <= n <= 20)，表示有n个盘子。
Output 
对于每组输入数据，最少需要的摆放次数。
Sample Input
2 
1 
10
Sample Output
2 
19684
解题思路： 
因为最大的盘子可以放在最上面，因此可以分成如下几步：
（1）将n-1个盘子移动到B杆
（2）将第n个盘子先移动到B杆，然后再移动到C杆
（3）将n-1个盘子移动到C杆
因此可以得到公式：
```
D（n） = 2*F(n-1) + 2
```
但是由于前面n-1个盘子不允许大盘子放在小盘子上面，因此仍然需要按照递推公式
```
F（n） = 3*F（n-1） +2
```
```cpp
#include<iostream>
using namespace std;
long long a[25];
int main()
{
    a[1]=1;
    for(int i=2;i<=20;i++)
      a[i]=3*a[i-1]+1;
    int n,t;
    cin>>t;
    while(t--)
    {
        cin>>n;
        cout<<2*a[n-1]+2<<endl;
    }
    return 0;
}
```
