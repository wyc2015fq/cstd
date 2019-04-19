# NBUT  1219    Time - HJ - CSDN博客
2016年08月19日 18:58:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：240
Description 
Digital clock use 4 digits to express time, each digit is described by 3*3 characters (including”|”,”_”and” “).now given the current time, please tell us how can it be expressed by the digital clock.
Input 
There are several test cases. 
Each case contains 4 integers in a line, separated by space. 
Proceed to the end of file.
Output 
For each test case, output the time expressed by the digital clock such as Sample Output.
Sample Input
1 2 5 6 
2 3 4 2
Sample Output
```
_  _  _ 
| _||_ |_ 
||_  _||_|
 _  _     _ 
 _| _||_| _|
|_  _|  ||_
```
Hint
The digits showed by the digital clock are as follows:
```
_  _     _  _  _  _  _  _ 
| _| _||_||_ |_   ||_||_|| |
||_  _|  | _||_|  ||_| _||_|
```
解析：本题考查对字符串处理能力，值得一做！
代码如下：
```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int main()
{
    int a[][10]={{' ','_',' ','|',' ','|','|','_','|'},{' ',' ',' ',' ',' ','|',' ',' ','|'},{' ','_',' ',' ','_','|','|','_',' '},{' ','_',' ',' ','_','|',' ','_','|'},{' ',' ',' ','|','_','|',' ',' ','|'},{' ','_',' ','|','_',' ',' ','_','|'},{' ','_',' ','|','_',' ','|','_','|'},{' ','_',' ',' ',' ','|',' ',' ','|'},{' ','_',' ','|','_','|','|','_','|'},{' ','_',' ','|','_','|',' ','_','|'}};                  //按照题中所给，敲出模板
    int aa,bb,cc,dd;
    while(cin>>aa>>bb>>cc>>dd)
    {
       int pp[4];        //注意数字符号有三层，而每层都有四个数据同时输入，为了循环方便，开一个数组保存这四个数
       pp[0]=aa;
       pp[1]=bb;
       pp[2]=cc;
       pp[3]=dd;
       for(int i=0;i<4;i++)    //数字符号有三层，因此循环开三个，每个两重，注意输出的格式
        for(int j=0;j<3;j++)
          printf("%c",a[pp[i]][j]);
       printf("\n");
       for(int i=0;i<4;i++)
        for(int j=3;j<6;j++)
          printf("%c",a[pp[i]][j]);
       printf("\n");
       for(int i=0;i<4;i++)
        for(int j=6;j<9;j++)
          printf("%c",a[pp[i]][j]);
       printf("\n");
    }
    return 0;
}
```
