# A*B Problem（洛谷-P1303） - Alex_McAvoy的博客 - CSDN博客





2018年04月12日 22:44:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：81
个人分类：[洛谷																[基础算法——高精度](https://blog.csdn.net/u011815404/article/category/7586330)](https://blog.csdn.net/u011815404/article/category/7429966)








## 题目描述

求两数的积。

## 输入输出格式

**输入格式：**

两行，两个数。

**输出格式：**

积

## 输入输出样例

**输入样例#1：**

```
1
2
```

**输出样例#1：**

```
2
```

———————————————————————————————————————————————

思路：高精度乘法模板题，高精度乘法详解：[点击这里](https://blog.csdn.net/u011815404/article/details/79927278)

## 源代码

```cpp
#include<iostream>
#include<cstring>
#include<string>
using namespace std;
char str1[2560],str2[2560];
int a[2560],b[2560],c[2560];
int main()
{

    int lena,lenb,lenc;
    int x;
    int i,j;

    memset(a,0,sizeof(a));
    memset(b,0,sizeof(b));
    memset(c,0,sizeof(c));

    cin>>str1;//输入乘数str1
    cin>>str2;//输入乘数str2

    lena=strlen(str1);
    lenb=strlen(str2);
    for(i=0;i<=lena-1;i++)//乘数str1存入数组a
        a[lena-i]=str1[i]-'0';
    for(i=0;i<=lenb-1;i++)//乘数str2存入数组b
        b[lenb-i]=str2[i]-'0';

    for(i=1;i<=lenb;i++)
    {
        x=0;//用于存放进位
        for(j=1;j<=lena;j++)//对乘数每一位进行处理
        {
            c[i+j-1]=a[j]*b[i]+x+c[i+j-1];//当前乘积+上次乘积进位+原数
            x=c[i+j-1]/10;
            c[i+j-1]%=10;
        }
        c[i+lena]=x;//进位
    }
    lenc=lena+lenb;
    while((c[lenc]==0)&&(lenc>1))//删除前导0
        lenc--;
    for(i=lenc;i>=1;i--)//倒序输出
        cout<<c[i];
    cout<<endl;
    return 0;
}
```






