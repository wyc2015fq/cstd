# A+B Problem（高精）（洛谷-P1601） - Alex_McAvoy的博客 - CSDN博客





2018年04月12日 22:40:59[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：127








## 题目描述

高精度加法,x相当于a+b problem，不用考虑负数。

## 输入输出格式

**输入格式：**

分两行输入a,b<=10^500

**输出格式：**

输出只有一行，代表A+B的值

## 输入输出样例

**输入样例#1：**

```
1
1
```

**输出样例#1：**
`2`
———————————————————————————————————————————————

思路：高精度加法详解：[点击这里](https://blog.csdn.net/u011815404/article/details/79920270)

## 源代码

```cpp
#include<iostream>
#include<cstring>
#include<string>
using namespace std;
int main()
{
    string str1,str2;
    int a[100],b[100],c[100];
    int len1,len2;
    int i;

    cin>>str1;
    cin>>str2;
    len1=str1.length();
    len2=str2.length();

    /*补0工作，保证两个字符串相同*/
    if(len1<len2)
        for(i=1;i<=len2-len1;i++)
           str1="0"+str1;
    else
        for(i=1;i<=len1-len2;i++)
           str2="0"+str2;

    len1=str1.length();
    len2=str2.length();

    for(i=0;i<=len1-1;i++)//加数str1存入数组a
        a[len1-i]=str1[i]-'0';
    for(i=0;i<=len2-1;i++)//加数str2存入数组b
        b[len2-i]=str2[i]-'0';

    int x=0;
    int lenc=1;
    while(  (lenc<=len1)  ||  (lenc<=len2)  )
    {
        c[lenc]=a[lenc]+b[lenc]+x;//第i位相加并加上次的进位
        x=c[lenc]/10;//向高位进位
        c[lenc]%=10;//存储第i位的值
        lenc++;//位置下标
    }

    /*处理最高进位*/
    c[lenc]=x;
    if(c[lenc]==0)
        lenc--;

    for(i=lenc;i>0;i--)//倒序输出
        cout<<c[i];
    cout<<endl;

    return 0;
}
```



