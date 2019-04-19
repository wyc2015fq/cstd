# hdu 2175  汉诺塔IX（找规律） - HJ - CSDN博客
2017年03月06日 23:38:08[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：297
1,2,…,n表示n个盘子．数字大盘子就大．n个盘子放在第１根柱子上．大盘不能放在小盘上． 
在第１根柱子上的盘子是a1,a2,…,an. a1=n,a2=n-1,…,an=1.即a1 
    是最下 
    面的盘子．把n个盘子移动到第３根柱子．每次只能移动１个盘子，且大盘不能放在小盘上． 
    问第m次移动的是那一个盘子.  
Input 
    每行2个整数n (1 ≤ n ≤ 63) ,m≤ 2^n-1.n=m=0退出 
Output 
    输出第m次移动的盘子的号数． 
Sample Input
```
63 1
63 2
0 0
```
Sample Output
```
1
2
```
先来看看盘子数比较小的时候情况：
当n==1时
```
1
```
当n==2时
```
1 2 1
```
当n==3时
```
1 2 1 3 1 2 1
```
当n==4时
```
1 2 1 3 1 2 1 4 1 2 1 3 1 2 1
```
由此可见，可以根据“从大到小”取的规则，判断是否整除即可。
```cpp
#include<iostream>
using namespace std;
int main()
{
    long long n,m;
    while(cin>>n>>m)
    {
        if(n==0&&m==0)
          break;
        long long ans=1,cnt=1;
        for(int i=0;i<n-1;i++)
          ans*=2;
        for(int i=n;i>=1;i--)
        {
            if(m%ans==0)
            {
                cnt=i;
                break;
            }
            ans/=2;
        }
        cout<<cnt<<endl;
    }
    return 0;
}
```
