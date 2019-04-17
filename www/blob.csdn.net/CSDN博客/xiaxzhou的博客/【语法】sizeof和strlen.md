# 【语法】sizeof和strlen - xiaxzhou的博客 - CSDN博客





2017年08月31日 10:50:18[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：224
个人分类：[面试题																[面试题																[数据结构](https://blog.csdn.net/xiaxzhou/article/category/7119623)




[http://blog.csdn.net/niushuai666/article/details/7677210](http://blog.csdn.net/niushuai666/article/details/7677210)

一、sizeof

sizeof(…)是运算符，而不是一个函数。

其值在编译时即计算好了。 

由于在编译时计算，因此sizeof不能用来返回动态分配的内存空间的大小。

具体而言，当参数分别如下时，sizeof返回的值表示的含义如下： 

    数组——编译时分配的数组空间大小； 

    指针——存储该指针所用的空间大小（存储该指针的地址的长度，是长整型，应该为4）； 

    类型——该类型所占的空间大小； 

    对象——对象的实际占用空间大小； 

    函数——函数的返回类型所占的空间大小。函数的返回类型不能是void。
二、strlen

strlen(…)是函数，要在运行时才能计算。

参数必须是字符型指针（char*）, 且必须是以’\0’结尾的。 

当数组名作为参数传入时，实际上数组就退化成指针了。

```cpp
char arr[10] = "Hello";
int len_one = strlen(arr);
int len_two = sizeof(arr); 
cout << len_one << " and " << len_two << endl;
```

输出结果为：5 and 10

sizeof返回定义arr数组时，编译器为其分配的数组空间大小，**不关心数组里面的数据**。 

strlen只关心存储的**数据内容**，不关心空间的大小和类型。](https://blog.csdn.net/xiaxzhou/article/category/7119468)](https://blog.csdn.net/xiaxzhou/article/category/6896019)





