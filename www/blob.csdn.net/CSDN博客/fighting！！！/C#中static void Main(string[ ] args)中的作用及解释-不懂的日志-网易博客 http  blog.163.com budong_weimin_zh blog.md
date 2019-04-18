# C#中static void Main(string[ ] args)中的作用及解释 - 不懂的日志 - 网易博客  http://blog.163.com/budong_weimin_zh/blog - fighting！！！ - CSDN博客
2017年10月29日 21:16:11[dujiahei](https://me.csdn.net/dujiahei)阅读数：288
C#中static void Main(string[ ] args)中的作用及解释 - 不懂的日志 - 网易博客  http://blog.163.com/budong_weimin_zh/blog/static/129198524201112445319102/
C#中static void Main(string[ ] args)中的作用及解释
static 表示方法是静态的就是说方法在程序被编译的时候就被分配了内存，使用的时候不用生成某个类型的对象，知道程序退出才释放。
void 表示方法没有返回值，就是方法没有renturn 关键字。
main 是方法名，当然这个方法是特殊的方法，是Main（）就是说是主函数，就是整个程序的入口，程序被编译后从这里开始执行，这也是main方法要是static的原因，因为在啥都没做的时候这个函数就得存在。
string[] args 表示命令行参数是字符串数组，就是说，你在命令行（黑框框）里输入的参数可以使多个字符串（在某种意义上意味着什么都可以是命令行参数）。
运行时会弹出命令窗口，你可以在那里输入一些参数，string[] args 指的就是你在命令窗口输入的参数。args是用来处理命令行参数的。命令行参数，就是你运行这个程序的时候给它传的参数。它是可选项，不是必须的。
如何向C# Main函数传递参数呢？我们看到C# Main函数带有一个string[] args参数，那么你知道它有什么实际的功能呢？下面我们来具体谈谈：
C＃控制台应用程序下，我们常常看到Main函数带有一个string[] args参数，那么你知道它有什么用处？
步骤1：建立一个控制台应用程序，暂且命名为Main.cs
步骤2：贴上下面这段代码。
using System;
class Program{
      static void Main(string[] args){
      int argsLength = args.Length;
      Console.WriteLine("Main函数参数args的长度：" + argsLength.ToString());
      for (int i = 0; i < argsLength; i++)
      {
           Console.Write("第" + i.ToString() + "位是：");
           Console.WriteLine(args[i].ToString());
      }
      }
}
步骤3：编译并运行Main.cs，生成Main.exe文件
步骤4：在命令行模式下输入c:\>Main a b c ,敲回车看结果
输出结果为：
Main函数参数args的长度：3
第0位是：a
第1位是：b
第2位是：c
由于参数个数是未知的，所以允许输入任意多个，也可以不输入。
