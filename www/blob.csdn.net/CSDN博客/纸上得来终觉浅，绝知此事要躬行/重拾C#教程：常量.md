# 重拾C#教程：常量 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月10日 12:50:18[boonya](https://me.csdn.net/boonya)阅读数：45









原文地址：[http://www.runoob.com/csharp/csharp-constants.html](http://www.runoob.com/csharp/csharp-constants.html)

常量是固定值，程序执行期间不会改变。常量可以是任何基本数据类型，比如整数常量、浮点常量、字符常量或者字符串常量，还有枚举常量。

常量可以被当作常规的变量，只是它们的值在定义后不能被修改。


## 整数常量

整数常量可以是十进制、八进制或十六进制的常量。前缀指定基数：0x 或 0X 表示十六进制，0 表示八进制，没有前缀则表示十进制。

整数常量也可以有后缀，可以是 U 和 L 的组合，其中，U 和 L 分别表示 unsigned 和 long。后缀可以是大写或者小写，多个后缀以任意顺序进行组合。

这里有一些整数常量的实例：
212/* 合法 */215u/* 合法 */0xFeeL/* 合法 */078/* 非法：8 不是一个八进制数字 */032UU/* 非法：不能重复后缀 */
以下是各种类型的整数常量的实例：
85/* 十进制 */0213/* 八进制 */0x4b/* 十六进制 */30/* int */30u/* 无符号 int */30l/* long */30ul/* 无符号 long */
## 浮点常量

一个浮点常量是由整数部分、小数点、小数部分和指数部分组成。您可以使用小数形式或者指数形式来表示浮点常量。

这里有一些浮点常量的实例：
3.14159/* 合法 */314159E-5L/* 合法 */510E/* 非法：不完全指数 */210f/* 非法：没有小数或指数 */.e55          /* 非法：缺少整数或小数 */
使用小数形式表示时，必须包含小数点、指数或同时包含两者。使用指数形式表示时，必须包含整数部分、小数部分或同时包含两者。有符号的指数是用 e 或 E 表示的。

## 字符常量

字符常量是括在单引号里，例如，'x'，且可存储在一个简单的字符类型变量中。一个字符常量可以是一个普通字符（例如 'x'）、一个转义序列（例如 '\t'）或者一个通用字符（例如 '\u02C0'）。

在 C# 中有一些特定的字符，当它们的前面带有反斜杠时有特殊的意义，可用于表示换行符（\n）或制表符 tab（\t）。在这里，列出一些转义序列码：
|转义序列|含义|
|----|----|
|\\|\ 字符|
|\'|' 字符|
|\"|" 字符|
|\?|? 字符|
|\a|Alert 或 bell|
|\b|退格键（Backspace）|
|\f|换页符（Form feed）|
|\n|换行符（Newline）|
|\r|回车|
|\t|水平制表符 tab|
|\v|垂直制表符 tab|
|\ooo|一到三位的八进制数|
|\xhh . . .|一个或多个数字的十六进制数|
以下是一些转义序列字符的实例：namespaceEscapeChar{classProgram{staticvoidMain(string[] args){Console.WriteLine("Hello\tWorld\n\n");Console.ReadLine();}}}
当上面的代码被编译和执行时，它会产生下列结果：
HelloWorld
## 字符串常量

字符常量是括在双引号 "" 里，或者是括在 @"" 里。字符串常量包含的字符与字符常量相似，可以是：普通字符、转义序列和通用字符

使用字符串常量时，可以把一个很长的行拆成多个行，可以使用空格分隔各个部分。

这里是一些字符串常量的实例。下面所列的各种形式表示相同的字符串。
string a ="hello, world";// hello, worldstring b =@"hello, world";// hello, worldstring c ="hello \t world";// hello     worldstring d =@"hello \t world";// hello \t worldstring e ="Joe said \"Hello\" to me";// Joe said "Hello" to mestring f =@"Joe said ""Hello"" to me";// Joe said "Hello" to mestring g ="\\\\server\\share\\file.txt";// \\server\share\file.txtstring h =@"\\server\share\file.txt";// \\server\share\file.txtstring i ="one\r\ntwo\r\nthree";string j =@"one
two
three";
## 定义常量

常量是使用 **const** 关键字来定义的 。定义一个常量的语法如下：
const<data_type><constant_name>= value;
下面的代码演示了如何在程序中定义和使用常量：
usingSystem;namespaceDeclaringConstants{classProgram{staticvoidMain(string[] args){constdouble pi =3.14159;// 常量声明double r;Console.WriteLine("Enter Radius: ");
            r =Convert.ToDouble(Console.ReadLine());double areaCircle = pi * r * r;Console.WriteLine("Radius: {0}, Area: {1}", r, areaCircle);Console.ReadLine();}}}


当上面的代码被编译和执行时，它会产生下列结果：
EnterRadius:3Radius:3,Area:28.27431
**1、Convert.ToDouble 与 Double.Parse 的区别**

实际上 Convert.ToDouble 与 Double.Parse 较为类似，实际上 Convert.ToDouble内部调用了 Double.Parse：

(1)对于参数为null的时候：
-  Convert.ToDouble参数为 null 时，返回 0.0；
-  Double.Parse 参数为 null 时，抛出异常。

(2)对于参数为""的时候：
-  Convert.ToDouble参数为 "" 时，抛出异常；
-  Double.Parse 参数为 "" 时，抛出异常。

(3)其它区别：
-  Convert.ToDouble可以转换的类型较多；
-  Double.Parse 只能转换数字类型的字符串。
-  Double.TryParse 与 Double.Parse 又较为类似，但它不会产生异常，转换成功返回 true，转换失败返回 false。最后一个参数为输出值，如果转换失败，输出值为 0.0。

附测试代码：
usingSystem;usingSystem.Collections.Generic;usingSystem.Linq;usingSystem.Text;namespaceConsoleApplication1{classProgram{staticvoidMain(string[] args){try{//string a = "0.2";//string a = null;string a ="";try{double d1 =Double.Parse(a);}catch(Exception err){Console.WriteLine("d1转换出错:"+ err.Message);}try{double d2 =Convert.ToDouble(a);}catch(Exception err){Console.WriteLine("d2转换出错:"+ err.Message);}try{double d3;Double.TryParse(a,out d3);}catch(Exception err){Console.WriteLine("d3转换出错:"+ err.Message);}}finally{Console.ReadKey();}}}}
**2.Convert.ToInt32() 与 int.Parse() 的区别**

没搞清楚Convert.ToInt32和int.Parse()的细细微区别时千万别乱用，否则可能会产生无法预料的结果，举例来说：假如从url中取一个参数page的值，我们知道这个值是一个int，所以即可以用Convert.ToInt32(Request.QueryString["page"])，也可以用,int.Parse(Request.QueryString["page"])，但是如果page这个参数在url中不存在，那么前者将返回0，0可能是一个有效的值，所以你不知道url中原来根本就没有这个参数而继续进行下一下的处理，这就可能产生意想不到的效果，而用后一种办法的话没有page这个参数会抛出异常，我们可以捕获异常然后再做相应的处理，比如提示用户缺少参数，而不是把参数值当做0来处理。

(1)这两个方法的最大不同是它们对null值的处理方法：

Convert.ToInt32(null)会返回0而不会产生任何异常，但int.Parse(null)则会产生异常。

(2)对数据进行四舍五入时候的区别

a. Convert.ToInt32(double value)

如果 value 为两个整数中间的数字，则返回二者中的偶数；即 3.5转换为4，4.5 转换为 4，而 5.5 转换为 6。 不过4.6可以转换为5，4.4转换为4

b. int.Parse("4.5")

直接报错:"输入字符串的格式不正确".

c. int(4.6) = 4

Int转化其他数值类型为Int时没有四舍五入，强制转换

(3)对被转换类型的区别

int.Parse 是转换 String为int

Convert.ToInt32是转换继承自Object的对象为int的(可以有很多其它类型的数据).

你得到一个object对象,你想把它转换为int,用int.Parse就不可以,要用Convert.ToInt32.










