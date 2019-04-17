# C++ 格式化输入函数scanf() - Alex_McAvoy的博客 - CSDN博客





2018年02月06日 15:05:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：1512








## 【功能】



格式化输入任意数据列表



## 【语句格式】



scanf("格式控制符",地址列表);

①格式控制符：由%与格式符构成，将要输入的字符按指定格式输入

②地址列表：要输入的各变量地址



## 【基本格式符】




- d：输入十进制整数
- u：输入无符号整数
- o：输入八进制整数
- x：输入十六进制整数
- c：输入单个字符
- s：输入字符串（输入时以非空格开始，空格结束，自动填补成以'\0'为结尾）
- f：输入实数
- e：输入实数





## 【附加格式符】


- l：可用于整型、实型（例：%ld、%lf）
- h：可用于整型（例：%hd）
- *：对应输入量不赋给一变量
- 域宽（一个整数）：指定要输入所占的列宽（例：%4d）



## 【实例】


-  *的使用
	
```cpp
#include<cstdio>
int main()
{
    int a,b;
    scanf( "%d %*d %d" , &a , &b );//输入1 2 3
    printf( "a = %d , b = %d \n" , a , b );//输出a=1，b=3
    return 0;
}
```

- 域宽格式符的使用
	
```cpp
#include<cstdio>
int main()
{
    int a,b;
    scanf( "%4d %4d" , &a , &b );//输入1234567
    printf( "a = %d , b = %d \n" , a , b );//输出a=1234，b=567
    return 0;
}
```

- %s的使用
	
```cpp
#include<cstdio>
int main()
{
    char st[40];
    scanf( "%s" , st );//输入Hello World！
    printf( "%s\n" , st );//输出Hello
    return 0;
}
```

- 非格式符的使用
	
```cpp
#include<cstdio>
int main()
{
    int a,b,c;
    scanf( "%d , %d , %d ", &a , &b ,&c );//因有间隔符“,”，故应输入1,2,3，而非此前的1 2 3
    printf( "a = %d , b = %d , c = %d\n" , a , b , c );
    return 0;
}
```

- 空格的应用
	
```cpp
#include<cstdio>
int main()
{
    /*
        输入要求为： A   B   C   D   E时
        采用格式化输入中的空格，可以代表换行、Tab、空格
    */
    char str[5];
    for(int i=0;i<10;i++)
        scanf( "%ch " , &str[i] );
    scanf( "%s\n" , str );
    return 0;
}
```






