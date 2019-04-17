# 重拾C#教程：枚举 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月10日 12:57:51[boonya](https://me.csdn.net/boonya)阅读数：45








原文地址：[http://www.runoob.com/csharp/csharp-enum.html](http://www.runoob.com/csharp/csharp-enum.html)




枚举是一组命名整型常量。枚举类型是使用 **enum** 关键字声明的。

C# 枚举是值类型。换句话说，枚举包含自己的值，且不能继承或传递继承。


## 声明 *enum* 变量

声明枚举的一般语法：
enum<enum_name>{ 
    enumeration list 
};
其中，
- *enum_name* 指定枚举的类型名称。
- *enumeration list* 是一个用逗号分隔的标识符列表。

枚举列表中的每个符号代表一个整数值，一个比它前面的符号大的整数值。默认情况下，第一个枚举符号的值是 0.例如：
enumDays{Sun,Mon, tue,Wed, thu,Fri,Sat};
## 实例

下面的实例演示了枚举变量的用法：
usingSystem;namespaceEnumApplication{classEnumProgram{enumDays{Sun,Mon, tue,Wed, thu,Fri,Sat};staticvoidMain(string[] args){intWeekdayStart=(int)Days.Mon;intWeekdayEnd=(int)Days.Fri;Console.WriteLine("Monday: {0}",WeekdayStart);Console.WriteLine("Friday: {0}",WeekdayEnd);Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
Monday:1Friday:5



