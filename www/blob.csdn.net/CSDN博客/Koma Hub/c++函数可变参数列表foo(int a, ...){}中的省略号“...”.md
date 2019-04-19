# c++函数可变参数列表foo(int a, ...){}中的省略号“...” - Koma Hub - CSDN博客
2018年06月08日 15:35:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：154
```cpp
int add(int num,...)  
/*求和函数*/
{
    va_list args;
    int sum=0;
    va_start(args,num);
    for(int i=0;i<num;i++)
        sum += va_arg(args,int);      
    va_end(args);
    return sum;
}
//调用
add(3,1,2,3,4,5);//求前三位的和1+2+3
```
