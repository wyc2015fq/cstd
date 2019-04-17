# 重拾C#教程：高级部分-匿名方法（Anonymous methods） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 13:56:05[boonya](https://me.csdn.net/boonya)阅读数：80








原文地址：[http://www.runoob.com/csharp/csharp-anonymous-methods.html](http://www.runoob.com/csharp/csharp-anonymous-methods.html)




我们已经提到过，委托是用于引用与其具有相同标签的方法。换句话说，您可以使用委托对象调用可由委托引用的方法。

**匿名方法（Anonymous methods）** 提供了一种传递代码块作为委托参数的技术。匿名方法是没有名称只有主体的方法。

在匿名方法中您不需要指定返回类型，它是从方法主体内的 return 语句推断的。


## 编写匿名方法的语法

匿名方法是通过使用 **delegate** 关键字创建委托实例来声明的。例如：
delegatevoidNumberChanger(int n);...NumberChanger nc =delegate(int x){Console.WriteLine("Anonymous Method: {0}", x);};
代码块 *Console.WriteLine("Anonymous Method: {0}", x);* 是匿名方法的主体。

委托可以通过匿名方法调用，也可以通过命名方法调用，即，通过向委托对象传递方法参数。

例如：
nc(10);
## 实例

下面的实例演示了匿名方法的概念：
usingSystem;delegatevoidNumberChanger(int n);namespaceDelegateAppl{classTestDelegate{staticint num =10;publicstaticvoidAddNum(int p){
            num += p;Console.WriteLine("Named Method: {0}", num);}publicstaticvoidMultNum(int q){
            num *= q;Console.WriteLine("Named Method: {0}", num);}staticvoidMain(string[] args){// 使用匿名方法创建委托实例NumberChanger nc =delegate(int x){Console.WriteLine("Anonymous Method: {0}", x);};// 使用匿名方法调用委托
            nc(10);// 使用命名方法实例化委托
            nc =newNumberChanger(AddNum);// 使用命名方法调用委托
            nc(5);// 使用另一个命名方法实例化委托
            nc =newNumberChanger(MultNum);// 使用命名方法调用委托
            nc(2);Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
AnonymousMethod:10NamedMethod:15NamedMethod:30



