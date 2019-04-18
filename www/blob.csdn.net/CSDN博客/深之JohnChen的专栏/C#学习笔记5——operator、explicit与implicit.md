# C#学习笔记5——operator、explicit与implicit - 深之JohnChen的专栏 - CSDN博客

2011年08月31日 00:39:37[byxdaz](https://me.csdn.net/byxdaz)阅读数：1576标签：[c#																[float																[编译器																[struct																[system																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)
个人分类：[.Net & C#](https://blog.csdn.net/byxdaz/article/category/628880)


# C# 参考之转换关键字：operator、explicit与implicit

#### **operator**

*operator* 关键字用于在类或结构声明中声明运算符。运算符声明可以采用下列四种形式之一：
·                        public static result-type operator unary-operator ( op-type operand )·                        public static result-type operator binary-operator (·                            op-type operand,·                            op-type2 operand2·                            )·                        public static implicit operator conv-type-out ( conv-type-in operand )·                        public static explicit operator conv-type-out ( conv-type-in operand )
参数：

·                result-type 运算符的结果类型。 

·                unary-operator 下列运算符之一：+ - ! ~ ++ — true false 

·                op-type 第一个（或唯一一个）参数的类型。 

·                operand 第一个（或唯一一个）参数的名称。 

·                binary-operator 其中一个：+ - * / % & | ^<< >> == != > < >= <= 

·                op-type2 第二个参数的类型。 

·                operand2 第二个参数的名称。 

·                conv-type-out 类型转换运算符的目标类型。 

·                conv-type-in 类型转换运算符的输入类型。 

注意：

·                前两种形式声明了用户定义的重载内置运算符的运算符。并非所有内置运算符都可以被重载（请参见可重载的运算符）。op-type和 op-type2 中至少有一个必须是*封闭类型*（即运算符所属的类型，或理解为*自定义的类型*）。例如，这将防止重定义整数加法运算符。 

·                后两种形式声明了转换运算符。conv-type-in 和conv-type-out 中正好有一个必须是封闭类型（即，转换运算符只能从它的封闭类型转换为其他某个类型，或从其他某个类型转换为它的封闭类型）。 

·                运算符只能采用值参数，不能采用 ref 或 out 参数。 

·                C# 要求成对重载比较运算符。如果重载了==，则也必须重载!=，否则产生编译错误。同时，比较运算符必须返回bool类型的值，这是与其他算术运算符的根本区别。 

·                C# 不允许重载=运算符，但如果重载例如+运算符，编译器会自动使用+运算符的重载来执行+=运算符的操作。 

·                运算符重载的其实就是函数重载。首先通过指定的运算表达式调用对应的运算符函数，然后再将运算对象转化为运算符函数的实参，接着根据实参的类型来确定需要调用的函数的重载，这个过程是由编译器完成。 

·                任何运算符声明的前面都可以有一个可选的属性（C# 编程指南）列表。 

#### **explicit**

*explicit* 关键字用于声明必须使用强制转换来调用的用户定义的类型转换运算符。
static explicit operator target_type { source_type identifier }
参数：

·                target_type 目标类型 

·                source_type 源类型。 

·                identifier Something。 

注意：

·                转换运算符将源类型转换为目标类型。源类型提供转换运算符。与隐式转换不同，必须通过强制转换的方式来调用显式转换运算符。如果转换操作可能导致异常或丢失信息，则应将其标记为 explicit。这可以防止编译器无提示地调用可能产生无法预见后果的转换操作。 

#### **implicit**

*implicit *关键字用于声明隐式的用户定义类型转换运算符。
static implicit operator target_type { source_type identifier }注意：
·                隐式转换可以通过消除不必要的类型转换来提高源代码的可读性。但是，因为可以在程序员未指定的情况下发生隐式转换，因此必须注意防止令人不愉快的后果。一般情况下，隐式转换运算符应当从不引发异常并且从不丢失信息，以便可以在程序员不知晓的情况下安全使用它们。如果转换运算符不能满足那些条件，则应将其标记为 explicit。 

示例：

以下是一个综合示例，简要展示用法。如要更具体细节的了解，请参阅MSDNLibrary。

//keywords_operator.cs
                              // keywords_operator.cs

 using System;

 namespace Hunts.Keywords
 {

// 定义一个人民币结构。数据类型转换的语法对于结构和类是一样的
public struct RMB
 {
// 注意:这些数的范围可能不能满足实际中的使用
public uint Yuan;
public uint Jiao;
public uint Fen;

public RMB(uint yuan, uint jiao, uint fen)
 {
if (fen > 9)
 {
jiao += fen / 10;
fen = fen % 10;
}
if (jiao > 9)
 {
yuan += jiao / 10;
jiao = jiao % 10;
}
this.Yuan = yuan;
this.Jiao = jiao;
this.Fen = fen;
}

public override string ToString()
 {
return string.Format("￥{0}元{1}角{2}分",Yuan, Jiao, Fen);
}

// 一些操作
public static RMB operator +(RMB rmb1, RMB rmb2)
 {
return new RMB(rmb1.Yuan + rmb2.Yuan, rmb1.Jiao +rmb2.Jiao, rmb1.Fen + rmb2.Fen);
}

public static implicit operator float(RMB rmb)
 {
return rmb.Yuan + (rmb.Jiao/10.0f) + (rmb.Fen/100.00f);
}

public static explicit operator RMB(float f)
 {
uint yuan = (uint)f;
uint jiao = (uint)((f - yuan) * 10);
uint fen = (uint)(((f - yuan) * 100) % 10);
return new RMB(yuan, jiao, fen);
}

// more 
}
class App
 {
static void Main()
 {
RMB r1, r2, r3, r4;

// 记得小学时的某次捐款，我把口袋里藏好的一块钱加6张一毛钱以及13个一分钱的硬币都贡献出去了：（
r1 = new RMB(1, 6, 13);
// 其实当时其他人都已经交过了，他们总共交了：
r2 = new RMB(46, 9, 3);
// 那么加上我的就是：
r3 = r1 + r2;
Console.WriteLine("r3 = {0}", r3.ToString());

// 隐式转换
float f = r3;
Console.WriteLine("float f= {0}", f);

// 显式转换
r4 = (RMB)f;
Console.WriteLine("r4 = {0}", r4.ToString());
//如果不进行显示转换,将出现错误 CS0266: 无法将类型“float”隐式转换为“Hunts.Keywords.RMB”。存在一个显式转换(是否缺少强制转换?)

Console.Read();
}
}
}

/*
控制台输出：
r3 = ￥48元6角6分
float f = 48.66
r4 = ￥48元6角5分
*/

我们会发现r4结果少了一分钱！这是因为在：

uint fen = (uint)(((f- yuan) * 100) % 10);

这句中，在将float转换为uint时发生了圆整错误（这与计算机以二进制存储有关）。解决这个错误，我们可以使用System.Convert类中用于处理数字的静态方法：

uint fen = Convert.ToUInt32(((f - yuan) * 100) % 10);

不过使用System.Convert处理会有些性能的损失。


