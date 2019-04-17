# Function（洛谷-P1464） - Alex_McAvoy的博客 - CSDN博客





2018年03月03日 14:34:24[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：187
个人分类：[洛谷																[基础算法——递归](https://blog.csdn.net/u011815404/article/category/8119752)](https://blog.csdn.net/u011815404/article/category/7429966)








## 题目描述

对于一个递归函数w(a,b,c)

如果a<=0 or b<=0 or c<=0就返回值1.

如果a>20 or b>20 or c>20就返回w(20,20,20)

如果a<b并且b<c 就返回w(a,b,c-1)+w(a,b-1,c-1)-w(a,b-1,c)

其它别的情况就返回w(a-1,b,c)+w(a-1,b-1,c)+w(a-1,b,c-1)-w(a-1,b-1,c-1)

这是个简单的递归函数，但实现起来可能会有些问题。当a,b,c均为15时，调用的次数将非常的多。你要想个办法才行.

## 输入输出格式

**输入格式：**

会有若干行，并以-1，-1，-1结束，保证输入的数在-9223372036854775808~9223372036854775807之间，并且是整数

**输出格式：**

输出若干行

格式:[b]w(a,_b,_c)_=_你的输出(_代表空格)[/b]

## 输入输出样例

**输入样例#1：**

```
1 1 1
2 2 2
-1 -1 -1
```

**输出样例#1：**

```
w(1, 1, 1) = 2
w(2, 2, 2) = 4
```

## 源代码  

```cpp
#include<iostream>
**using** **namespace** std;
**long** **int** w(**long** **int** a,**long** **int** b,**long** **int** c);
**long** **int** result[30][30][30]={0};

**int** main()
{
    **long** **int** num[100][3];
    **long** **int** a,b,c;
    **int** i;
    
    **for**(i=0;;i++)
    {
        cin>>num[i][0]>>num[i][1]>>num[i][2];//输入函数
        **if**( num[i][0]==-1 && num[i][1]==-1 && num[i][2]==-1 )
            **break**;
    }
        
    **for**(i=0;;i++)
    {
        a=num[i][0]; b=num[i][1]; c=num[i][2];
        **if**(a>20)	a=21;//大于20时，返回20
        **if**(b>20)	b=21;//大于20时，返回20
        **if**(c>20)	c=21;//大于20时，返回20
        **if**( a==-1 && b==-1 && c==-1 )//均为-1时
            **break**;//终止循环
        **else**//按要求输出
            cout<<"w("<<num[i][0]<<", "<<num[i][1]<<", "<<num[i][2]<<") = "<<w(a,b,c)<<endl;
    }
    **return** 0;
}

**long** **int** w(**long** **int** a,**long** **int** b,**long** **int** c)
{
    **if**( a<=0 || b<=0 || c<=0 )//小于0时
        **return** 1;//返回1
    **else** **if**(result[a][b][c]!=0)//不等于0时
        **return** result[a][b][c];//返回当前值
    **else** **if**( a>20 || b>20 || c>20 )//任意一参数大于20时
        **return** w(20,20,20);//返回20
    **else** **if**( a<b && b<c )//a<b并且b<c时
        {
            result[a][b][c]=w(a,b,c-1)+w(a,b-1,c-1)-w(a,b-1,c);
            **return** result[a][b][c];
        }
    **else**//其他情况
        {
            result[a][b][c]=w(a-1,b,c)+w(a-1,b-1,c)+w(a-1,b,c-1)-w(a-1,b-1,c-1);
            **return** result[a][b][c];
        }
}
```




