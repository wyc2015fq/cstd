# C++网络IP算法 - fanyun的博客 - CSDN博客
2018年09月28日 20:24:02[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：131
1、网络IP算法要求
     一个网络地址A和一个子网掩码可以描述一个子网，子网是一个数包含32个二进制位，前n个位为1，后32－n个位为0，如：255.255.255.48(11111111|11111111|11111111|11111000) 表示某个ip地址如果和A的前n位相等则说明其属于这个子网。 
现给定n个网络地址组成一子网，求该子网的最小范围的首地址和子网掩码。
2、思路
求出ip地址在哪一位最开始存在不同即可解决，通过位运算的异或操作再取反即可得出。 
易错地方： 
所有ip异或一轮之后，得出的结果可能有类似 111110100 的情况出现，不符合掩码的要求； 
位运算的两个小技巧，纪录于此： 
1. a & (a - 1) 将a的二进制位上最后一个1转为0 
2. a | (a - 1) 将a的二进制位上末尾连续的0全部转为1
3、代码实现
```cpp
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <queue>
#include <string.h>
#include <set>
#include <stack>
#include <stdlib.h>
#include <time.h>
using namespace std;
unsigned int num[1009] = {};
int main()
{
    int n;
    while(cin>>n)
    {
        memset(num, 0, sizeof(num));
        for(int i=0;i<n;i++)
        {
            char t[40];
            cin>>t;
            int c = 0;
            for(int j=0;j<strlen(t);j++)
            {
                if(t[j] == '.')
                {
                    num[i] = (num[i]<<8) + c;
                    c = 0;
                }
                else
                    c = c*10 + t[j]-'0';
            }
            num[i] = (num[i]<<8) + c;
        }
        unsigned int ans = ~0, tt;
        for(int i=1;i<n;i++)
        {
            unsigned int q = num[i] ^ num[0];
            while(q)
            {
                ans &= ~(q | (q-1));
                q = q&(q-1);
            }
        }
        tt = ans & num[0];
        int sans[4]={};
        int pos = 0;
        while(tt != 0)
        {
            sans[pos++] = tt%(1<<8);
            tt >>= 8;
        }
        for(int i=3;i>=0;i--)
        {
            cout<<sans[i];
            if(i)
                cout<<".";
        }
        cout<<endl;
        pos = 0;
        memset(sans, 0, sizeof(sans));
        while(ans != 0)
        {
            sans[pos++] = ans%(1<<8);
            ans >>= 8;
        }
        for(int i=3;i>=0;i--)
        {
            cout<<sans[i];
            if(i)
                cout<<".";
        }
        cout<<endl;
    }
    return 0;
}
```
