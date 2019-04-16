# java基础学习总结——方法的重载(overload) - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [java基础学习总结——方法的重载(overload)](https://www.cnblogs.com/xdp-gacl/p/3637321.html)



## 一、方法的重载

![](https://images0.cnblogs.com/i/289233/201403/312342436729620.png)

　　方法名一样，但参数不一样，这就是重载(overload)。

　　所谓的参数不一样，主要有两点：第一是参数的个数不一样，第二是参数的类型不一样。只要这两方面有其中的一方面不一样就可以构成方法的重载了。

```
1 package cn.galc.test;
 2 
 3 public class TestOverLoad {
 4 
 5     void max(int a, int b) {
 6         System.out.println(a > b ? a : b);
 7     }
 8 
 9     /*
10      * int max(int a, int b) { 
11      *         return a > b ? a : b; 
12      * }
13      */
14 
15     void max(float a, float b) {
16         System.out.println(a > b ? a : b);
17     }
18 }
```

　　这里有void修饰符的两个max方法名字相同，但它们的参数类型不一样，所以可以构成重载。而int  max(int a , int b)方法与void  max(int a, int b)方法是不构成重载的，现在它们是重名的两个方法，在一个类中声明两个重名的方法是不允许的，编译会出错。方法名一样，参数类型一样，只有返回值不一样，这个不构成重载，因为方法可以这么调用，调用一个方法时可以不使用的它返回值，所以当调用这两个方法时，把整型的数传进去，由于两个方法都是一样的名字，一样的参数类型，所以编译器无法区分到底要调用的是哪一个方法。构成重载深层次的原因：只要这两个方法编译器能区分开来，在调用的时候知道要调用的是哪一个，不会产生混淆，这两个方法就构成重载。

再看下面这两个方法：

```
1 int max(int a, int b) {
2     System.out.println("调用的int max(int a, int b)方法");
3     return a > b ? a : b;
4 }
5      
6 int max(short a, short b) {
7     System.out.println("调用的int max(short a, short b)方法");
8     return a > b ? a : b;
9 }
```

　　这两个方法也是可以构成重载的，因为编译器一看到整数，就会把它当作int类型。所以当把整数传进来的时候，编译器首先调用的是max(int a , int b)这个方法。而要想调用max(short a ,short b)这个方法，那么就得在main方法里面这样写：

```
1 public static void main(String[] args) {
2     TestOverLoad t= new TestOverLoad();
3     t.max(3,4);  //这里调用的是max(int a , int b)这个方法
4     short a = 3;
5     short b = 4;
6     t.max(a, b);  //这里调用的是max(short a , short b)这个方法。
7 }
```

## 二、构造方法的重载

与普通方法一样，构造方法也可以重载

```
1 package cn.galc.test;
 2 
 3 public class Person {
 4 
 5     int id;
 6     int age;
 7     
 8     /**
 9      * 构造方法
10      */
11     public Person() {
12         id=0;
13         age=20;
14     }
15     
16     /**
17      * 构造方法重载一
18      * @param i
19      */
20     public Person(int i) {
21         id=i;
22         age=20;
23     }
24     
25     /**
26      * 构造方法重载二
27      * @param i
28      * @param j
29      */
30     public Person(int i,int j) {
31         id=i;
32         age=j;
33     }
34 }
```









