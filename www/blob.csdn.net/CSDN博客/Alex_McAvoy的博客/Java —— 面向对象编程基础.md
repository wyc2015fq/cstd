# Java —— 面向对象编程基础 - Alex_McAvoy的博客 - CSDN博客





2018年10月02日 09:58:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：159
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【包】

## 1.概念及作用

1）是类的组织方式，相当于系统中的文件夹

2）不同包内可存在同名类，在一定程度上可以避免命名冲突

3）功能相近的类可放在一个包中，便于查找与使用

4）某些访问权限以包为单位

## 2.创建

```java
package 包名
```

当包中还有包时，可通过 包1.包2. … .包n 来指定，包1为最外层的包，包n为最内存的包

## 3.使用包中的类

1）长名引用包中的类：在每个类名前加上完整的包名

```java
com.lzw.Round round=new com.lzw.Round();
```

2）import 语句引入包中的类

```java
import java.util.Date;    //引入util包的Date类
import java.io.*;         //引入io包的所有类
```

## 4.Java类库中的常用包

```java
java.util：实用类
java.io：输入输出类
java.sql：进行JDBC数据库编程的类
java.lang：核心类
javax.swing：抽象窗口工具的几种图形、文本、窗口GUI类
```

# 【类与成员方法】

## 1.类的声明

```java
[public/abstract/final] class <类名> [extends 父类名] [implements 接口列表] {
    类体
}
```

## 2.成员变量与局部变量

1）成员变量：类体中所声明的变量，用来表示类的状态与属性，作用域是整个类，对其操作只能放在方法中

```java
[public/protected/private] [static] [final] <变量类型> <变量名>;
```

2.局部变量：方法体中所声明的变量，作用域是整个方法

```java
[final] <变量类型> <变量名>;
```

## 3.成员方法

1）作用：使用各种语句对成员变量和局部变量进行操作，从而实现类的行为

2）格式

```java
[public/protected/private] [static] [final] <方法返回值类型> <方法名> ([参数列表]){
     方法体
}
```

3）重载：一个类中，多个方法名相同，但参数个数与参数类型不同的方法（方法返回值类型不能作为区分重载标志）

4）可变个数的形参方法：允许为方法指定数量不确定的形参，若在定义方法时，在最后一个形参的类型后增加三点 ...，则表明该形参可以接受多个参数值，多个参数值被当成数组传入，其本质就是一个数组参数，要注意的是，数组形式的参数可以处于形参列表的任意位置，但是个数可变的形参处于形参列表的最后，也就是说，一个方法中最多只能有一个个数可变的形参

```java
public class Main{
	//======================1.定义形参个数可变的方法======================
	public static void testVarargs(int a, String... books) {
		// book当成数组处理
		for (String book : books) {
			System.out.println(book);
		}
		// 输出整数变量a的值
		System.out.println(a);
	}
	// ======================2.采用数组形参来定义方法======================
	public static void testArray(int a, String[] books) {
		// book当成数组处理
		for (String book : books) {
			System.out.println(book);
		}
		// 输出整数变量a的值
		System.out.println(a);
	}
 
	public static void main(String[] args) {
		// 采用可变个数形参的方法调用
		testVarargs(5, "A", "B", "C");
		// 采用数组形参的方法调用
		testArray(2, new String[]{"A","B","C"});
	}
}
```

# 【参数传值】

1.基本类型的参数传值：将实际参数的副本传递到方法内，参数本身不受任何影响

2.引用类型的参数传值：传递的值不受变量所引用的实体，而是变量中存放的引用（地址），因此当一个参数传值给一个函数时，两个变量指向的是同一个对象的引用地址，这两个变量操作的是同一个对象。

# 【初始化块】

初始化块又被称为代码块，是在类中由 {} 括起的语句，根据 static 是否修饰分为静态初始化块、非静态初始化块

## 1.非静态初始化块

1）每创建一个类的对象，非静态初始化块就加载一次，且其执行早于构造器。

2）一个类中可以有多个非静态初始化块，多个块之间按顺序结构执行。

3）非静态初始化块中允许有输出语句。

4）非静态初始化块中可以对类的属性进行初始化操作。

```java
class A{
    private int a;
    private String b;
    {
        a=123;
        b="abc";
        System.out.println("这是一个非静态代码块");
    } 
}
```

## 2.静态初始化块

1）静态初始化块随类的加载而加载，其执行要早于非静态初始化块，且只执行一次。

2）一个类中可以有多个静态初始化块，多个块之间按顺序结构执行。

3）静态初始化块中允许有输出语句。

4）静态初始化块中只可执行对静态结构的操作，即对静态属性、静态方法的操作

```java
class A{
    static int a;
    static String b;
    static{
        a=123;
        b="abc";
        System.out.println("这是一个静态代码块");
    } 
}
```

# 【对象属性赋值的先后顺序】

1.属性的默认初始化

2.属性的显性初始化

3.初始化块中属性的赋值

3.构造器(构造函数)对属性的初始化

4.对象.方法 对属性的赋值

# 【类的多态】

1.多态性指的是同一事物的多种表现形态，具体体现为方法的重载与重写、子类对象的多态性，使用前提是要有类的继承以及子类对父类的重写

2.对于多态性来说，编译时，“看左边”，即将此引用变量理解为父类的类型；运行时，“看右边”，关注于真正对象的实体，即子类的对象，因此执行的方法就是子类重写的方法

3.类的属性无多态性，即：子类对象的多态性，不适用于类的属性

4.类型转换

1）向上转型：子类转父类，即多态的使用

2）向下转型：父类转子类，需要使用强转符 ()，为避免出现异常，一般需要使用 instanceof 运算符判断

```java
class Person{
    ...
}
class Student extends Person{
    ...
}
class Main{
    public static void main(String[] args){
        //向上转型
        Person p=new Student();
        //向下转型
        if(p instanceof Student)
            Student s=(Student)p;
    }
}
```

instanceof 运算符是用来在运行时指出对象是否是特定类或其子类的一个实例，若是，返回 true，若不是，返回 false

5.接口与实现类间的多态：不同的类在实现同一接口时，具有不同的表现方式。



