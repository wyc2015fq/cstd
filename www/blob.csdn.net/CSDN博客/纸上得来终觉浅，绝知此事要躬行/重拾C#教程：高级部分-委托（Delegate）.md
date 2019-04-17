# 重拾C#教程：高级部分-委托（Delegate） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年04月13日 13:49:03[boonya](https://me.csdn.net/boonya)阅读数：123








原文地址：[http://www.runoob.com/csharp/csharp-delegate.html](http://www.runoob.com/csharp/csharp-delegate.html)




C# 中的委托（Delegate）类似于 C 或 C++ 中函数的指针。**委托（Delegate）** 是存有对某个方法的引用的一种引用类型变量。引用可在运行时被改变。

委托（Delegate）特别用于实现事件和回调方法。所有的委托（Delegate）都派生自 **System.Delegate** 类。


## 声明委托（Delegate）

委托声明决定了可由该委托引用的方法。委托可指向一个与其具有相同标签的方法。

例如，假设有一个委托：
publicdelegateintMyDelegate(string s);
上面的委托可被用于引用任何一个带有一个单一的 *string* 参数的方法，并返回一个 *int* 类型变量。

声明委托的语法如下：
delegate<return type><delegate-name><parameter list>
## 实例化委托（Delegate）

一旦声明了委托类型，委托对象必须使用 **new** 关键字来创建，且与一个特定的方法有关。当创建委托时，传递到 **new** 语句的参数就像方法调用一样书写，但是不带有参数。例如：
publicdelegatevoid printString(string s);...
printString ps1 =new printString(WriteToScreen);
printString ps2 =new printString(WriteToFile);
下面的实例演示了委托的声明、实例化和使用，该委托可用于引用带有一个整型参数的方法，并返回一个整型值。
usingSystem;delegateintNumberChanger(int n);namespaceDelegateAppl{classTestDelegate{staticint num =10;publicstaticintAddNum(int p){
         num += p;return num;}publicstaticintMultNum(int q){
         num *= q;return num;}publicstaticint getNum(){return num;}staticvoidMain(string[] args){// 创建委托实例NumberChanger nc1 =newNumberChanger(AddNum);NumberChanger nc2 =newNumberChanger(MultNum);// 使用委托对象调用方法
         nc1(25);Console.WriteLine("Value of Num: {0}", getNum());
         nc2(5);Console.WriteLine("Value of Num: {0}", getNum());Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
Value of Num:35Value of Num:175
## 委托的多播（Multicasting of a Delegate）

委托对象可使用 "+" 运算符进行合并。一个合并委托调用它所合并的两个委托。只有相同类型的委托可被合并。"-" 运算符可用于从合并的委托中移除组件委托。

使用委托的这个有用的特点，您可以创建一个委托被调用时要调用的方法的调用列表。这被称为委托的 **多播（multicasting）**，也叫组播。下面的程序演示了委托的多播：
usingSystem;delegateintNumberChanger(int n);namespaceDelegateAppl{classTestDelegate{staticint num =10;publicstaticintAddNum(int p){
         num += p;return num;}publicstaticintMultNum(int q){
         num *= q;return num;}publicstaticint getNum(){return num;}staticvoidMain(string[] args){// 创建委托实例NumberChanger nc;NumberChanger nc1 =newNumberChanger(AddNum);NumberChanger nc2 =newNumberChanger(MultNum);
         nc = nc1;
         nc += nc2;// 调用多播
         nc(5);Console.WriteLine("Value of Num: {0}", getNum());Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
Value of Num:75
## 委托（Delegate）的用途

下面的实例演示了委托的用法。委托 *printString* 可用于引用带有一个字符串作为输入的方法，并不返回任何东西。

我们使用这个委托来调用两个方法，第一个把字符串打印到控制台，第二个把字符串打印到文件：
usingSystem;usingSystem.IO;namespaceDelegateAppl{classPrintString{staticFileStream fs;staticStreamWriter sw;// 委托声明publicdelegatevoid printString(string s);// 该方法打印到控制台publicstaticvoidWriteToScreen(string str){Console.WriteLine("The String is: {0}", str);}// 该方法打印到文件publicstaticvoidWriteToFile(string s){
         fs =newFileStream("c:\\message.txt",FileMode.Append,FileAccess.Write);
         sw =newStreamWriter(fs);
         sw.WriteLine(s);
         sw.Flush();
         sw.Close();
         fs.Close();}// 该方法把委托作为参数，并使用它调用方法publicstaticvoid sendString(printString ps){
         ps("Hello World");}staticvoidMain(string[] args){
         printString ps1 =new printString(WriteToScreen);
         printString ps2 =new printString(WriteToFile);
         sendString(ps1);
         sendString(ps2);Console.ReadKey();}}}
当上面的代码被编译和执行时，它会产生下列结果：
TheStringis:HelloWorld



