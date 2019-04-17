# Java —— 语法基础 - Alex_McAvoy的博客 - CSDN博客





2018年10月01日 10:33:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：63
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【基本要点】
- 源文件以 .java 结尾
- 程序执行：javac.exe 将 .java 编译成多个字节码文件 .class，java.exe 再将字节码文件执行
- 类中有主方法（main()方法），格式是固定的：public static void main(String[] args){}
- 主方法是程序的入口，方法内是程序的执行部分
- 源文件可以有多个 class 声明的类，但一个源文件中只能有一个声明为 public 的类，同时要求此类名与源文件名一致
- 定义 long 型变量时，需要在值的末尾加 L 或 l，例：long a=123456L
- 定义 float 型变量时，需要在值的末尾加 F 或 f，例：float a=12.34F

# 【文档注释】

```java
/**
    @author 指定java程序的作者

    @version 指定源文件的版本

    @param 方法的参数说明信息
*/
```

注释内容可被JDK提供的工具 javadoc.exe 解析，生成一套以网页文件形式体现的该程序的说明文档

# 【命名规范】

包名：多单词组成时所有字母小写，xxxyyyzzz

类、接口名：多单词组成时所有单词首字母大写，XxxYyyZzz

变量、方法名：多单词组成时，第一个单词首字母小写，第二个单词开始每个单词首字母大写，xxxYyyZzz

常量名：所有字母均大写，多单词时每个单词用下划线连接，XXX_YYY_ZZZ

# 【变量间的运算】

不考虑 boolean 型

## 1.自动类型转换

当容量小的数据类型与容量大的数据类型做运算时，容量小的会自动转换为容量大的数据类型

char、byte、short ===> int ===> long ===> float ===> double

当 char/byte/short 之间做运算时，默认结果为 int 型

## 2.强制类型转换

容量大的转换成容量小的，需要加强制类型转换符 ()

强制类型转换的问题：精度的损失

```java
short a=10;
a=a+3;            //error，默认结果为int型
a=(short)(a+3);   //true
a+=3;             //true,+=运算符既可以实现运算，又不会更改本身的数据类型
```

## 3.字符串与基本数据类型的运算

只能为连接运算 +，得到的结果仍为一个字符串

```java
String str=“abc”;
int a=123
String str1=str+a;//abc123
```

# 【逻辑运算符】

## 1.真值表
|a|b|a&b|a|b|!a|a^b|a&&b|a||b|
|----|----|----|----|----|----|----|----|
|true|true|true|true|false|false|true|true|
|true|false|false|true|false|true|false|true|
|false|true|false|true|true|true|false|true|
|false|false|false|false|true|false|false|false|

&：逻辑与，|：逻辑或，!：逻辑非，^：逻辑异或，&&：短路与，||：短路或

## 2.逻辑与短路的区别

& 与 &&：逻辑与 & 不管左边是 true 还是 false，右端都会进行判断；短路与 && 只有当左端为 false 时，右端不再进行运算。因此建议使用 &&

| 与 ||：逻辑或 | 左端为 true 时，右端仍进行运算；短路或 || 当左端为 true 时，右端不再进行运算。因此建议使用 ||

# 【数组元素的默认初始化值】
- 对于 byte、short、int、long 而言，创建数组后，默认值为 0
- 对于 float、double 而言，创建数组后，默认值为 0.0
- 对于 char 而言，创建数组后，默认值为空格
- 对于 boolean 而言，创建数组后，默认为 false
- 对于引用类型(String、class等)的变量而言，创建数组后，默认为 null



