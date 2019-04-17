# Cantor表（洛谷-P1014） - Alex_McAvoy的博客 - CSDN博客





2018年03月03日 14:35:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：186








## 题目描述

现代数学的著名证明之一是Georg Cantor证明了有理数是可枚举的。他是用下面这一张表来证明这一命题的：

1/1 1/2 1/3 1/4 1/5 …

2/1 2/2 2/3 2/4 …

3/1 3/2 3/3 …

4/1 4/2 …

5/1 …

… 我们以Z字形给上表的每一项编号。第一项是1/1，然后是1/2，2/1，3/1，2/2，…

## 输入输出格式

**输入格式：**

整数N（1≤N≤10000000）

**输出格式：**

表中的第N项

## 输入输出样例

**输入样例#1：**
`7`
**输出样例#1：**
`1/4`
## 源代码

```cpp
#include<iostream>
**using** **namespace** std;
**int** main()
{
    **int** n;
    **int** i,sum;
    **int** odd,even;
    
    cin>>n;
    
    **for**(i=0,sum=0;sum<n;i++)//累加前n项和
        sum+=i;
        
    
    odd=sum-n+1;//奇数和
    even=n+i-sum-1;//偶数和
    
    **if**(i%2)
        cout<<even<<"/"<<odd<<endl;
    **else**
        cout<<odd<<"/"<<even<<endl;
        
    **return** 0;
}
```



