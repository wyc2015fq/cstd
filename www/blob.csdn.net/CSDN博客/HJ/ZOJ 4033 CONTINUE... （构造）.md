# ZOJ  4033 CONTINUE...?（构造） - HJ - CSDN博客
2018年08月11日 19:18:45[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：91
[http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=4033](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=4033)
题目大意： 
有男女共n个人，给出一个字符串表示这n个人的站队，’0’表示女，’1’表示男，下标从1开始。其中给这些人分G1、G2、G3、G4四个组，其中G1和G2都是女生，G3和G4都是男生。要求G1和G3组内的所有人的下标之和要等于G2和G4组内的所有人的下标之和。如果不存在，则输出-1，否则输出每个人所在的组名。
解题思路： 
首先判断是否能够分组：当且仅当n取余4的余数为0或者3的时候，才能有解
考虑n的奇偶性，进行构造。
如果n为奇数，取前三个数，把a[1]和a[2]分别分到G1或者G3中，a[3]分到G2或者G4中，剩下的n-3两两分组，让其之和均为n+4即可；
如果n为偶数，则可以直接分组，让其之和均为n+1即可；
```cpp
#include<iostream>
using namespace std;
const int maxn=1e5+10;
char s[maxn],a[maxn];
int main()
{
    int t,n;
    cin>>t;
    while(t--)
    {
        cin>>n>>s;
        if(n%4==1||n%4==2)
          cout<<-1<<endl;
        else
        {
            if(n%2)
            {
                a[0]=s[0]=='1'?'3':'1';
                a[1]=s[1]=='1'?'3':'1';
                a[2]=s[2]=='1'?'4':'2';
                for(int i=3,j=n-1;i<(n+3)/2;i++,j--)
                {
                    if(i%2)
                    {
                        a[i]=s[i]=='1'?'3':'1';
                        a[j]=s[j]=='1'?'3':'1';
                    }
                    else
                    {
                        a[i]=s[i]=='1'?'4':'2';
                        a[j]=s[j]=='1'?'4':'2';
                    }
                }
            }
            else
            {
                for(int i=0,j=n-1;i<n/2;i++,j--)
                {
                    if(i%2)
                    {
                        a[i]=s[i]=='1'?'3':'1';
                        a[j]=s[j]=='1'?'3':'1';
                    }
                    else
                    {
                        a[i]=s[i]=='1'?'4':'2';
                        a[j]=s[j]=='1'?'4':'2';
                    }
                }
            }
            for(int i=0;i<n;i++)
              cout<<a[i];
            cout<<endl;
        }
    }
    return 0;
}
```
