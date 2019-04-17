# decltype详解之指针类型的推断 - DumpDoctorWang的博客 - CSDN博客





2017年09月07日 19:51:12[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：334








原标题:  C++11 decltype将uchar*推断为uchar的方法

## 一、站在巨人的肩膀上
- decltype简介 [auto和decltype的用法总结](http://www.cnblogs.com/XiangfeiAi/p/4451904.html)

## 二、uchar数组和uchar*的推断结果
- 提示：在VS中，鼠标悬停在ElemType上即可查看其类型。

```
/*
decltype(a)推断结果为: uchar [](对数组的推断结果还是数组)
*/
typedef decltype(a) Type1;
/*
decltype(a[0])推断结果为:uchar
*/
typedef decltype(a[0]) Type2;
/*
decltype(b)推断结果为: uchar *(对指针的推断结果还是指针)
*/
typedef decltype(b) Type4;
/*
decltype(*b)推断结果为:uchar &(对指针的解引用的推断结果还是引用)
*/
typedef decltype(*b) Type5;
/*
decltype(*b++)和上例的结果一样，因为++的优先级高于*，注意和下例的对比
*/
typedef decltype(*b++) Type6;
/*
decltype((*b)++)的推断结果为: uchar(解引用后，进行自增操作，
这时候decltype括号里面已经是一个值，不是解引用，推断结果为该值的类型)
*/
typedef decltype((*b)++) Type7;
/*
decltype((*b)+0)的推断结果为: int(除++操作符和--操作符以外，
其他的运算都会把uchar的类型进行提升，所以推断的结果不是uchar)
*/
typedef decltype((*b)+0) Type8;
```

## 三、从指针推断出变量的类型

```
uchar a[100];
    uchar *b = a;
	// 使用typedef定义类型别名Type
    typedef std::remove_reference<decltype(*b)>::type Type;
	// 定义变量
    Type c='a';
	// 输出
    cout << c << endl;
```

decltype(*b)的推断结果是`uchar &`，`std::remove_reference<uchar &>::type`就可以得到`uchar`类型。`std::remove_reference<T &>::type`的结果就是类型`T`。



