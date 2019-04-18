# 编译器在处理const变量跟一般变量时的区别 - TaigaComplex求职中 - 博客园







# [编译器在处理const变量跟一般变量时的区别](https://www.cnblogs.com/TaigaCon/archive/2012/12/19/2824355.html)





const变量，也就是常量，当某个变量加上const属性的时候，表明这个变量不可修改。



const变量是不可写的，那么我们对于const变量的处理只剩下读操作了。



当我们读一般的变量的时候，会先获取该变量的地址，然后到该地址去取数据，那么对于const变量的读操作是怎么样的呢。

下面代码有助于更好理解const变量：



```
#include<iostream>
using namespace std;

int main(){
    const int a=10;
    const int * p=&a;                    //让指针p指向常量a所在的内存
    int * p_var=const_cast<int *>(p);            //强制转换const指针为非const
    *p_var=20;                //往该指针所指向的内存写20，也就是把10给覆盖了
    
    cout<<a<<endl;
    cout<<(&a)<<endl;
    
    cout<<*p<<endl;
    cout<<(p)<<endl;
    
    cout<<*p_var<<endl;
    cout<<(p_var)<<endl;
    
    return 0;
}
```





然后看看输出结果：

10　　　　　　　　//a             
0x28ac54　　　　//&a            
20　　　　　　　　//*p           
0x28ac54　　　　//p              
20　　　　　　　　//*p_var     
0x28ac54　　　　//p_var        

输出结果显示&a,p,p_var 相同，不过a =10  ， *p=*p_var=20，

一个内存地址怎么可能得到两个不同的值？



看看反汇编：

```
4011d6:    c7 44 24 04 0a 00 00     movl   $0xa,0x4(%esp)
```

上面的就是读取const a的值的汇编代码，它不是从内存取数据出来，而是直接得到常数0xa，也就是把const 变量当作真正的常量处理。



因此可以得到的结论就是：

对于const变量，编译器会在编译之前做处理，在所有要读取该const变量的地方，把该const变量替换成他一开始就制定的常量，如

const int a = 10;

int b= a ;

会在编译之前替换const变量，变成：

const int a = 10;

int b= 10;














