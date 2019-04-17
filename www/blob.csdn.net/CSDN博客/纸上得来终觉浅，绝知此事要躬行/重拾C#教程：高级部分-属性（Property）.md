# 重拾C#教程：高级部分-属性（Property） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 13:42:20[boonya](https://me.csdn.net/boonya)阅读数：125








原文地址：[http://www.runoob.com/csharp/csharp-property.html](http://www.runoob.com/csharp/csharp-property.html)




**属性（Property）** 是类（class）、结构（structure）和接口（interface）的命名（named）成员。类或结构中的成员变量或方法称为 **域（Field）**。属性（Property）是域（Field）的扩展，且可使用相同的语法来访问。它们使用 **访问器（accessors）** 让私有域的值可被读写或操作。

属性（Property）不会确定存储位置。相反，它们具有可读写或计算它们值的 **访问器（accessors）**。

例如，有一个名为 Student 的类，带有 age、name 和 code 的私有域。我们不能在类的范围以外直接访问这些域，但是我们可以拥有访问这些私有域的属性。


## 访问器（Accessors）

属性（Property）的**访问器（accessor）**包含有助于获取（读取或计算）或设置（写入）属性的可执行语句。访问器（accessor）声明可包含一个 get 访问器、一个 set 访问器，或者同时包含二者。例如：
// 声明类型为 string 的 Code 属性publicstringCode{get{return code;}set{
      code = value;}}// 声明类型为 string 的 Name 属性publicstringName{get{return name;}set{
     name = value;}}// 声明类型为 int 的 Age 属性publicintAge{get{return age;}set{
      age = value;}}
## 实例

下面的实例演示了属性（Property）的用法：
usingSystem;namespace tutorialspoint
{classStudent{privatestring code ="N.A";privatestring name ="not known";privateint age =0;// 声明类型为 string 的 Code 属性publicstringCode{get{return code;}set{
            code = value;}}// 声明类型为 string 的 Name 属性publicstringName{get{return name;}set{
            name = value;}}// 声明类型为 int 的 Age 属性publicintAge{get{return age;}set{
            age = value;}}publicoverridestringToString(){return"Code = "+Code+", Name = "+Name+", Age = "+Age;}}classExampleDemo{publicstaticvoidMain(){// 创建一个新的 Student 对象Student s =newStudent();// 设置 student 的 code、name 和 age
         s.Code="001";
         s.Name="Zara";
         s.Age=9;Console.WriteLine("Student Info: {0}", s);// 增加年龄
         s.Age+=1;Console.WriteLine("Student Info: {0}", s);Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
StudentInfo:Code=001,Name=Zara,Age=9StudentInfo:Code=001,Name=Zara,Age=10
## 抽象属性（Abstract Properties）

抽象类可拥有抽象属性，这些属性应在派生类中被实现。下面的程序说明了这点：
usingSystem;namespace tutorialspoint
{publicabstractclassPerson{publicabstractstringName{get;set;}publicabstractintAge{get;set;}}classStudent:Person{privatestring code ="N.A";privatestring name ="N.A";privateint age =0;// 声明类型为 string 的 Code 属性publicstringCode{get{return code;}set{
            code = value;}}// 声明类型为 string 的 Name 属性publicoverridestringName{get{return name;}set{
            name = value;}}// 声明类型为 int 的 Age 属性publicoverrideintAge{get{return age;}set{
            age = value;}}publicoverridestringToString(){return"Code = "+Code+", Name = "+Name+", Age = "+Age;}}classExampleDemo{publicstaticvoidMain(){// 创建一个新的 Student 对象Student s =newStudent();// 设置 student 的 code、name 和 age
         s.Code="001";
         s.Name="Zara";
         s.Age=9;Console.WriteLine("Student Info:- {0}", s);// 增加年龄
         s.Age+=1;Console.WriteLine("Student Info:- {0}", s);Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
StudentInfo:Code=001,Name=Zara,Age=9StudentInfo:Code=001,Name=Zara,Age=10



