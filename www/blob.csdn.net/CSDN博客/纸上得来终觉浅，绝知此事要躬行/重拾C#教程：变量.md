# 重拾C#教程：变量 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月10日 12:48:27[boonya](https://me.csdn.net/boonya)阅读数：37








原文地址：[http://www.runoob.com/csharp/csharp-variables.html](http://www.runoob.com/csharp/csharp-variables.html)




一个变量只不过是一个供程序操作的存储区的名字。在 C# 中，每个变量都有一个特定的类型，类型决定了变量的内存大小和布局。范围内的值可以存储在内存中，可以对变量进行一系列操作。

我们已经讨论了各种数据类型。C# 中提供的基本的值类型大致可以分为以下几类：
|类型|举例|
|----|----|
|整数类型|sbyte、byte、short、ushort、int、uint、long、ulong 和 char|
|浮点型|float 和 double|
|十进制类型|decimal|
|布尔类型|true 或 false 值，指定的值|
|空类型|可为空值的数据类型|
C# 允许定义其他值类型的变量，比如 **enum**，也允许定义引用类型变量，比如 **class**。这些我们将在以后的章节中进行讨论。在本章节中，我们只研究基本变量类型。

## C# 中的变量定义

C# 中变量定义的语法：
<data_type><variable_list>;
在这里，data_type 必须是一个有效的 C# 数据类型，可以是 char、int、float、double 或其他用户自定义的数据类型。variable_list 可以由一个或多个用逗号分隔的标识符名称组成。

一些有效的变量定义如下所示：
int i, j, k;char c, ch;float f, salary;double d;
您可以在变量定义时进行初始化：
int i =100;
## C# 中的变量初始化

变量通过在等号后跟一个常量表达式进行初始化（赋值）。初始化的一般形式为：
variable_name = value;
变量可以在声明时被初始化（指定一个初始值）。初始化由一个等号后跟一个常量表达式组成，如下所示：
<data_type><variable_name> = value;
一些实例：
int d =3, f =5;/* 初始化 d 和 f. */byte z =22;/* 初始化 z. */double pi =3.14159;/* 声明 pi 的近似值 */char x ='x';/* 变量 x 的值为 'x' */
正确地初始化变量是一个良好的编程习惯，否则有时程序会产生意想不到的结果。

请看下面的实例，使用了各种类型的变量：
namespaceVariableDefinition{classProgram{staticvoidMain(string[] args){short a;int b ;double c;/* 实际初始化 */
            a =10;
            b =20;
            c = a + b;Console.WriteLine("a = {0}, b = {1}, c = {2}", a, b, c);Console.ReadLine();}}}
当上面的代码被编译和执行时，它会产生下列结果：
a =10, b =20, c =30
## 接受来自用户的值
System 命名空间中的 Console 类提供了一个函数 ReadLine()，用于接收来自用户的输入，并把它存储到一个变量中。


例如：
int num;
num =Convert.ToInt32(Console.ReadLine());
函数 **Convert.ToInt32()** 把用户输入的数据转换为 int 数据类型，因为 **Console.ReadLine()** 只接受字符串格式的数据。

## C# 中的 Lvalues 和 Rvalues

C# 中的两种表达式：
- 
**lvalue**：lvalue 表达式可以出现在赋值语句的左边或右边。

- 
**rvalue**：rvalue 表达式可以出现在赋值语句的右边，不能出现在赋值语句的左边。


变量是 lvalue 的，所以可以出现在赋值语句的左边。数值是 rvalue 的，因此不能被赋值，不能出现在赋值语句的左边。下面是一个有效的语句：
int g =20;
下面是一个无效的语句，会产生编译时错误：
10=20;
不同类型变量进行运算的问题：
double a =42.29;int b =4229;int c = a + b;Console.WriteLine("c = {0}",c);Console.ReadKey();
上面这种编程方法是错误的，会出现错误提示：
"无法将类型'double'隐式转换为'int'。"
举例说明，当一个精度高的数据类型与一个精度低的数据类型进行运算时，定义运算结果的变量类型必须与精度最高的变量类型相同。这是为了防止在运算过程中造成数据丢失。

下面是正确代码：
double a =42.29;int b =4229;double c = a + b;Console.WriteLine("c = {0}",c);Console.ReadKey();
能输出运算结果：
c =4271.29



