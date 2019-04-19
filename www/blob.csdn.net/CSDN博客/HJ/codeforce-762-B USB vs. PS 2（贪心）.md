# codeforce-762-B USB vs. PS/2（贪心） - HJ - CSDN博客
2017年01月26日 00:56:34[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：475
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Due to the increase in the number of students of Berland State University it was decided to equip a new computer room. You were given the task of buying mouses, and you have to spend as little as possible. After all, the country is in crisis!
The computers bought for the room were different. Some of them had only USB ports, some — only PS/2 ports, and some had both options.
You have found a price list of a certain computer shop. In it, for m mouses it is specified the cost and the type of the port that is required to plug the mouse in (USB or PS/2). Each mouse from the list can be bought at most once.
You want to buy some set of mouses from the given price list in such a way so that you maximize the number of computers equipped with mouses (it is not guaranteed that you will be able to equip all of the computers), and in case of equality of this value you want to minimize the total cost of mouses you will buy. 
Input
The first line contains three integers a, b and c (0 ≤ a, b, c ≤ 105)  — the number of computers that only have USB ports, the number of computers, that only have PS/2 ports, and the number of computers, that have both options, respectively.
The next line contains one integer m (0 ≤ m ≤ 3·105)  — the number of mouses in the price list.
The next m lines each describe another mouse. The i-th line contains first integer vali (1 ≤ vali ≤ 109)  — the cost of the i-th mouse, then the type of port (USB or PS/2) that is required to plug the mouse in. 
Output
Output two integers separated by space — the number of equipped computers and the total cost of the mouses you will buy. 
Example 
Input
2 1 1 
4 
5 USB 
6 PS/2 
3 PS/2 
7 PS/2
Output
3 14
Note
In the first example you can buy the first three mouses. This way you will equip one of the computers that has only a USB port with a USB mouse, and the two PS/2 mouses you will plug into the computer with PS/2 port and the computer with both ports.
题目大意： 
  电脑有三种插口（一种“USB”,一种“PS/2”，一种两者通用），但是只有两种型号的鼠标（“USB”,“PS/2”。给出数据，求出最多能买的鼠标个数以及所花费的最少的钱。
解题思路： 
  主要是贪心，分成三个部分，每个部分排一次序，从头往后循环处理即可。
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
long long m[1000005],n[1000005],w[1000005];
char s[10];
int main()
{
    long long a,b,c;
    while(cin>>a>>b>>c)
    {
        long long y,i,j,p=0,q=0,r=0,l=0,x,sum=0;
        cin>>y;
        for(i=0;i<y;i++)
        {
            cin>>x>>s;
            if(s[0]=='U')
              m[p++]=x;      //m存"USB" 
            else
              n[q++]=x;     //n存"PS/2" 
        }
        sort(m,m+p);
        sort(n,n+q);       //价格排序 
        for(i=0;i<a;i++)    //给a类电脑配上鼠标 
        {
            if(i==p)
              break;
            sum+=m[i];
            r++;
        }
        for(j=0;j<b;j++)    //给b类电脑配上鼠标 
        {
            if(j==q)
              break;
            sum+=n[j];
            r++;
        }
        while(m[i])
          w[l++]=m[i++];
        while(n[j])
          w[l++]=n[j++];
        sort(w,w+l);      //最后剩下的给c类电脑配上鼠标 
        for(i=0;i<c;i++)
        {
            if(i==l)
              break;
            sum+=w[i];
            r++;
        }
        cout<<r<<" "<<sum<<endl;
    }
    return 0;
}
```
