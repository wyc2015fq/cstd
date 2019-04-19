# codeforces 625-D. Finals in arithmetic（构造+模拟） - HJ - CSDN博客
2017年07月16日 19:03:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：210
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Vitya is studying in the third grade. During the last math lesson all the pupils wrote on arithmetic quiz. Vitya is a clever boy, so he managed to finish all the tasks pretty fast and Oksana Fillipovna gave him a new one, that is much harder.
Let’s denote a flip operation of an integer as follows: number is considered in decimal notation and then reverted. If there are any leading zeroes afterwards, they are thrown away. For example, if we flip 123 the result is the integer 321, but flipping 130 we obtain 31, and by flipping 31 we come to 13.
Oksana Fillipovna picked some number a without leading zeroes, and flipped it to get number ar. Then she summed a and ar, and told Vitya the resulting value n. His goal is to find any valid a.
As Oksana Fillipovna picked some small integers as a and ar, Vitya managed to find the answer pretty fast and became interested in finding some general algorithm to deal with this problem. Now, he wants you to write the program that for given n finds any a without leading zeroes, such that a + ar = n or determine that such a doesn’t exist. 
Input
The first line of the input contains a single integer n (1 ≤ n ≤ 10^100 000). 
Output
If there is no such positive integer a without leading zeroes that a + ar = n then print 0. Otherwise, print any valid a. If there are many possible answers, you are allowed to pick any. 
Examples 
Input
4
Output
2
Input
11
Output
10
Input
5
Output
0
Input
33
Output
21
Note
In the first sample 4 = 2 + 2, a = 2 is the only possibility.
In the second sample 11 = 10 + 1, a = 10 — the only valid solution. Note, that a = 01 is incorrect, because a can’t have leading zeroes.
It’s easy to check that there is no suitable a in the third sample.
In the fourth sample 33 = 30 + 3 = 12 + 21, so there are three possibilities for a: a = 30, a = 12, a = 21. Any of these is considered to be correct answer.
题目大意： 
 定义ar为a的翻转数，例如a=123,那么ar=321。现在给出一个数n,要你求任意一个数a，满足a+ar=n。
解题思路： 
先把字符改成int，设置两个指针l=0,r=len-1。一个从前面走，一个从后面走。当两个数相等的时候自由分配即可。  
当两个数不相等的时候有两种处理方法：
```
1）向后借一位；
2）向后推一位；
```
如果处理之后两个数仍不相等，则无解 
然后如果l=r的时候，必须要判断这个数字是不是偶数，否则无解。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define maxn 100005
char a[maxn];
int s[maxn],l,r;
void minchange1(int x)          //处理一位数 
{
    s[l]=(l==0)?1:0;
    s[r]=(l==0)?x-1:x;
}
void minchange2(int x)        //处理两位数 
{
    s[l]=x-9;
    s[r]=9; 
}
int main()
{
    while(cin>>a)
    {
        int len=strlen(a),k=0,blag=0;
        l=0,r=len-1;
        for(int i=0;i<len;i++)
          s[i]=a[i]-'0';
        if(s[0]!=s[len-1])         //判断第一个数字时候不相等的时候 
        {
            s[0]--; 
            s[1]+=10;
            l=s[0]==0?1:0;               //是否可以借位 
        }
        while(l<=r)
        {
            if(s[l]!=s[r])         //如果两个数不相等 
            {
                if((s[l]-s[r]>=10)&&(s[r]<10))    //考虑借位 即判断两位数是否相等 
                {
                    s[r-1]--;
                    s[r]+=10;
                }
                if(s[l]-s[r]==1)                  //如果此时相差为1 则往后借出10个数（也就是一位） 
                {
                    s[l]--;
                    s[l+1]+=10;
                }
            }   
            if(s[l]!=s[r])            //如果经过如上处理之后两个数仍不相等 则无解 
              break;
            if(l!=r)                    //如果l!=r                         
              s[l]<10?minchange1(s[l]):minchange2(s[l]);           //判断是一位数还是两位数 然后分别处理即可 
            else                   //如果l==r   
              s[l]%2?blag=1:s[l]/=2;          //必须要保证s[l]为偶数  否则无解 
            if(s[l]<0||s[l]>9||s[r]>9||s[r]<0||blag)            //如果处理之后两个数字不在0~9的范围内 则无解 
              break;
            l++;
            r--;
        }
        if(l<=r)
        {
            cout<<0<<endl;
            continue;
        }
        while(s[k]==0)
          k++;
        while(k<len)
          cout<<s[k++];
        cout<<endl;
        a[0]='\0';
    }
    return 0;
}
```
