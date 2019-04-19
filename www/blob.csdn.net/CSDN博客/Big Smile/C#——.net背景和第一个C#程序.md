# C#——.net背景和第一个C#程序 - Big Smile - CSDN博客
2015年10月04日 19:46:08[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1641标签：[C#																[.net](https://so.csdn.net/so/search/s.do?q=.net&t=blog)](https://so.csdn.net/so/search/s.do?q=C#&t=blog)
个人分类：[C#																[编码学习](https://blog.csdn.net/tr1912/article/category/5787481)](https://blog.csdn.net/tr1912/article/category/5855387)
所属专栏：[C#学习基础](https://blog.csdn.net/column/details/csharp-wx.html)
# 一、.net平台
微软的.net平台是为了解决以前的类库和API编程的缺点而设计出来的一个统一的开发平台，他的优点就是统一，兼容性好。类库（dll）和API编程在以前的程序使用中带来了不少的困惑，比如老的软件安装之后，新的软件就各种报错，装不同版本的软件需要装很多的类库等等问题。
.net平台的一大优点就是整合了各种的微软编程语言所编出来的程序的互用性。只要在.net的平台下面编程，生成的类也好，对象也好，都是通过同样套路的一个编译器所编译出来的，因而可以随意的调用，做到不同人员的汇集。
![](https://img-blog.csdn.net/20151004195603519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 二、C#语言
C#语言是基于C++和C语言，添加了许多纯粹的面向对象的方法之后衍生出来的一种编程语言，和Java有很多相似的地方，C#的优点在于有很强的.net类库和.net平台做支持，而Java的优点在于是开源的，可以在各种环境下面运行。C#语言可以和其他的.net语言无缝进行操作使得.net平台的性能突出，加上有了微软的.net类库和vsIDE环境的支持，使得面向对象的编程就好像是在搭积木一般（虽然没有感受到），这就是C#的优点。
# 三、第一个小程序
`虽然只是一个最基本的小程序，但是却包含了C#和VS中的许多的规范：`
```
Using system
Namespace  BasicCsharp
{
	Class Program
	{
		Static void main(string[] args)
		{
			Console.Write("这是一个最基本的C#应用程序")
		}
	}
}
```
以上程序的功能是在一个小黑框框内输出一个“这是一个最基本的C#应用程序”。
`Using system  //导入系统命名空间``Namespace  BasicCsharp   //生命命名空间<span style="font-family: Arial, Helvetica, sans-serif;">BasicCsharp</span>``Class Program     //生明Program类``<pre name="code" class="csharp">Static void main(string[] args)  //程序运行的入口，每个程序都要有``Console.Write("这是一个最基本的C#应用程序")     //调用控制台的输出方法，输出字符串`
console类的方法是由.net控制台程序提供的方法，这就是为什么开始的时候要创建一个控制台的项目了。
控制台还有其他的方法：
read：char x = Console.Read()   //为获取字符串并放到变量X中
WriteLine： Console.WriteLine((char)x)     //把变量X中的内容输出到屏幕上
Write：     Console.Write(char(X))   //与上面一条的区别是不带最后的换行符
整体的语法和C语言的Printf差不多，只不过加变量的时候是用{0}代替%d的。
# 总结：
现阶段还没有到面向对象的阶段，还只是过程编程的阶段，在这个阶段的语法，语句的判断，循环，数组等都和C语言的差不多，所以不细谈了，以后就是要写面向对象的知识了。
