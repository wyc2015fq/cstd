# STL 的基本使用 —— pair - Alex_McAvoy的博客 - CSDN博客





2019年01月12日 21:44:04[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60








# 【概述】

pair 是定义在 <utility> 头文件中的一个简单的模板类，常用于表示一个二元组或元素对，且其提供了按照字典序对元素对进行大小比较的比较运算符模版函数。

定义：** pair<T1,T2> pairName**

参数：T1 是第一个值的数据类型，T2 是第二个值的数据类型。

# 【构造与访问】
- pair<T1,T2> p：构造一个空 pair，其中无任何元素
- pair<T1,T2> p(elem1,elem2)：构造一个元素值分别为 elem1、elem2 的 pair
- pair<T1,T2> p1(p2)：产生另一个同型 pair 的副本，所有元素都被复制

pair 的一对值可以具有不同的数据类型（T1 和 T2），两个值可以分别通过其公有函数 first、second 来进行访问与赋值。

```cpp
pair<int, string> p1; //默认构造函数
pair<string, double> p2("abc’", 100); //重载构造函数
pair<string, double> p3(p2);  //复制构造函数

//访问
cout << p2.first << " " << p2.second << endl;
cout << p3.first << " " << p3.second << endl;
```

使用技巧：由于 pair 类型比较繁琐，若要定义多个 pair 类型时，可以使用 typedef 简化声明

```cpp
typedef pair<string, string> name;
name name1("May", "Lily");
name name2("James", "Joyce");
```

# 【赋值】

1.利用 make_pair() 函数来赋值

```cpp
pair<int, double> p1;
p1 = make_pair(1, 1.2);
```

2.变量间赋值

```cpp
pair<int, double> p1(1, 1.2);
pair<int, double> p2 = p1;
```

# 【运算符的重载】

在 <utility> 中，已经定义了 pair 上的六个比较运算符：<、>、<=、>=、==、!=，其基本规则是先比较 first，当 first 相等时再比较 second，若想重定义自己的比较逻辑，可以通过重载运算符来重新指定比较逻辑。

```cpp
pair<int, double> p1(1, 2.5);
pair<int, double> p2(2, 2.5);
pair<int, double> p3(2, 3.5);

//结果：p1<p2
if(p1>=p2)
    cout<<"p1>=p2"<<endl;
else
    cout<<"p1<p2"<<endl;

//结果：p2<p3
if(p2<p3)
    cout<<"p2<p3"<<endl;
else
    cout<<"p2>=p3"<<endl;
```

# 【应用】

pair 主要的作用是将两个数据组合成一个数据，两个数据可以是同一类型或者不同类型。

其实质上是一个结构体，主要的两个成员变量是 first 和 second，这两个变量可以直接使用。

最常用的例子就是在构建二维坐标系的点时，可以使用 pair 来构造






