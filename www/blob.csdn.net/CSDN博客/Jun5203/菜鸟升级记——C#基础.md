# 菜鸟升级记——C#基础 - Jun5203 - CSDN博客
2018年11月18日 21:29:21[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：1800
所属专栏：[C#](https://blog.csdn.net/column/details/29824.html)
### 导入
- **.NET 框架**
.NET Framework是Windows、Windows Phone、Windows Server和WindowsAzure生成应用程序的开发平台。它包括公共语言运行时（CLR）[和.NET](http://xn--0tr.NET) Framework类库，其中包含类、接口和支持多种技术的值的类型。.NET Framework提供托管执行环境、简化的开发和部署以及与各种编程语言（包括Visual Basic和Visual C#）的集成。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181118104650457.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
.NET 4.5完善了LINQ、WCF、WF和WCF技术。
  （1）LINQ技术将传统查询语言（如SQL）和传统开发语言（如C#）集成到一起，使得对内存数据的查询更加高效、语法更简单。
  （2）WPF是创建华丽用户界面的基本技术，让用户的Windows界面想Windows7系统一样。WPF能开发什么东西呢？
WPF应用程序：独立可执行应用程序，类似exe应用程序。
WPF浏览器应用程序：基于XAML的浏览器的应用程序（好似B/S应用程序）。
WPF用户控件库：用于创建可重用的、组件化的用户控件库。
WPF自定义控件库：用于开发WPF自定义控件的控件库项目。
  （3）WCF（Windows Communication Foundation）简化了SOA（Service Oriented Architecture，面向服务）面向服务开发技术，并且支持跨区域、跨服务器的各种分布式开发，也支持未来的开发模式。
  （4）WF就是工作流。
- **C#的类库框架**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181118111142849.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- **C#的特点**- 简洁流畅的语法
- 精心的面向对象设计
- 与Web结合紧密
- 严格的安全性与强大的错误处理能力
- 优秀的版本处理技术
### 语法知识
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119125858160.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
- 常量、变量和表达式
常量：程序中不变的数据。
变量：经常发生变化的数据。
```
常量
   public const string CHINA="中国";
```
上述代码中，const是定义常量的标识，如果没有此标识，即使为CHINA指定固定的值，也不算是常量。string用来定义常量的类型。注意常量在命名时，通常使用大写字母。
```
变量
    public int count=10;
```
其中，int表示变量的类型，count表示变量名，变量也可以不需要初始值。
|类型名称|类型的意义|类型使用语法|
|----|----|----|
|int|整型，表示简单的整数|int count=9；|
|double|双精度浮点数，表示小数|double pi=2.14;|
|float|单精度浮点数|float m=2.1F;|
|short|短整型，范围-32768~32768|short count=999;|
|long|长整型，比短整型的范围更大|longcount1=9999999999;|
|byte|字节，数据的存储形式，范围0~255|byte=105;|
- 类型转换
隐式转换：C#会自动对一些类型进行转换。
显式转换：类型之间不可以自动转换。
```
隐式转换
   int count=9;              //定义整型
   float fcount=count;   //隐式转换为浮点型
```
```
显式转换
   int count=10;
   string Scoumt=count.ToString(count);  //数值到字符串的显式转换
```
- 变量的作用域
作用域是指变量在某一范围内有效，其相对于定义状态，即在代码中调用时的状态，如方法中的变量，只能在此方法中被调用。
- 全局变量：全局变量的作用域覆盖整个应用程序，使用public关键字定义变量。
- 局部变量：一般在方法窗体中定义，其作用域就是所在的窗体，使用private关键字定义变量。
- 静态变量：静态变量可以在方法中定义，也可以在窗体内定义，其作用域等同于全局变量，使用static定义静态变量。
```
全局变量
      public string ApplicationName="财务系统";
```
```
局部变量
      private string FormName="人员管理";
```
```
静态变量
      private static string UserName="张三";
```
- 变量的生存周期
生命周期则是相对于运行状态，即程序运行此方法时，方法中的变量有效。当成程序运行此方法后，方法中的变量也就消失了。
- 运算符
算术运算符：常用的计算符号，如“+”、“-”、“*”、“/”等。
赋值运算符：为某个变量指定值
？运算符：通常被称为三目运算符，因为有三个变量擦于其中。
```
b = ( a > 0 ) ? a : a ++ ;
```
上述表达式中有两个关键符号“？”和“：”。其中，前边通常是一个关系运算，？后面紧跟两个变量。？运算符的意思是判断前边的表达式。如果表达式结果为true，则选择？后面的第一个值，如果表达式结果为false,则选择？后面的第二个值。两个值之间以“：”间隔。
|逻辑运算符|说明|
|----|----|
|&&|逻辑与，只有两个表达式都为true，结果才为true|
|II|逻辑或，只要有1个表达式为true，结果就为true|
|!|逻辑非，取表达式的反值|
|关系运算符|说明|运算符|说明|
|----|----|----|----|
|>|大于|<=|小于等于|
|>=|大于等于|！=|不等于|
|<|小于|=|等于|
- 运算符优先级
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181118212903804.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==,size_16,color_FFFFFF,t_70)
### 小结
初学C#，领悟不深，如有错误，请多多指教！
