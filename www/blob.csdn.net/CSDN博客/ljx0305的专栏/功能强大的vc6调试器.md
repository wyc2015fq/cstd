# 功能强大的vc6调试器 - ljx0305的专栏 - CSDN博客
2010年01月18日 09:20:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：587标签：[borland																[debugging																[windows																[api																[平台																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=debugging&t=blog)](https://so.csdn.net/so/search/s.do?q=borland&t=blog)
个人分类：[VC](https://blog.csdn.net/ljx0305/article/category/401790)
**功能强大的vc6调试器**
作者：[yy2better](mailto:yy2better@126.com)
  要成为一位优秀的软件工程师，调试能力必不可缺。本文将较详细介绍VC6调试器的主要用法。 
  windows平台的调试器主要分为两大类： 
  1 用户模式(user-mode)调试器：它们都基于win32 Debugging API，有使用方便的界面，主要用于调试用户模式下的应用程序。这类调试器包括Visual C++调试器、WinDBG、BoundChecker、Borland C++ Builder调试器、NTSD等。 
  2 内核模式(kernel-mode)调试器：内核调试器位于CPU和操作系统之间，一旦启动，操作系统也会中止运行，主要用于调试驱动程序或用户模式调试器不易调试的程序。这类调试器包括WDEB386、WinDBG和softice等。其中WinDBG和softice也可以调试用户模式代码。 
  国外一位调试高手曾说，他70％调试时间是在用VC++，其余时间是使用WinDBG和softice。毕竟，调试用户模式代码，VC6调试器的效率是非常高的。因此，我将首先在本篇介绍VC6调试器的主要用法，其他调试器的用法及一些调试技能在后续文章中阐述。 
**一 位置断点（Location Breakpoint）**
  大家最常用的断点是普通的位置断点，在源程序的某一行按F9就设置了一个位置断点。但对于很多问题，这种朴素的断点作用有限。譬如下面这段代码： 
void CForDebugDlg::OnOK() 	
{
	for (int i = 0; i < 1000; i++)	//A
	{
		int k = i * 10 - 2;	//B
		SendTo(k);		//C
		int tmp = DoSome(i);	//D
		int j = i / tmp;	//E
	}
}      
      
  执行此函数，程序崩溃于E行，发现此时tmp为0，假设tmp本不应该为0，怎么这个时候为0呢？所以最好能够跟踪此次循环时DoSome函数是如何运行的，但由于是在循环体内，如果在E行设置断点，可能需要按F5（GO）许多次。这样手要不停的按，很痛苦。使用VC6断点修饰条件就可以轻易解决此问题。步骤如下。 
  1 Ctrl+B打开断点设置框，如下图： 
![](http://writeblog.csdn.net/document/journal/vckbase36/images/debug1.jpg)
**Figure 1**设置高级位置断点
  2 然后选择D行所在的断点，然后点击condition按钮，在弹出对话框的最下面一个编辑框中输入一个很大数目，具体视应用而定，这里1000就够了。 
  3 按F5重新运行程序，程序中断。Ctrl+B打开断点框，发现此断点后跟随一串说明：...487 times remaining。意思是还剩下487次没有执行，那就是说执行到513（1000－487）次时候出错的。因此，我们按步骤2所讲，更改此断点的skip次数,将1000改为513。 
  4 再次重新运行程序，程序执行了513次循环，然后自动停在断点处。这时，我们就可以仔细查看DoSome是如何返回0的。这样，你就避免了手指的痛苦，节省了时间。 
  再看位置断点其他修饰条件。如**Figure 1**所示，在“Enter the expression to be evaluated:”下面，可以输入一些条件，当这些条件满足时，断点才启动。譬如，刚才的程序，我们需要i为100时程序停下来，我们就可以输入在编辑框中输入“i==100”。 
  另外，如果在此编辑框中如果只输入变量名称，则变量发生改变时，断点才会启动。这对检测一个变量何时被修改很方便，特别对一些大程序。 
  用好位置断点的修饰条件，可以大大方便解决某些问题。 
**二 数据断点（Data Breakpoint）**
  软件调试过程中，有时会发现一些数据会莫名其妙的被修改掉（如一些数组的越界写导致覆盖了另外的变量），找出何处代码导致这块内存被更改是一件棘手的事情（如果没有调试器的帮助）。恰当运用数据断点可以快速帮你定位何时何处这个数据被修改。譬如下面一段程序： 
#include "stdafx.h"
#include 
int main(int argc, char* argv[])
{
	char szName1[10];
	char szName2[4];
	strcpy(szName1,"shenzhen");		
	printf("%s/n", szName1);		//A
	strcpy(szName2, "vckbase");		//B
	printf("%s/n", szName1);
	printf("%s/n", szName2);
	return 0;
}
      
  这段程序的输出是 
      	szName1: shenzhen
	szName1: ase
	szName2: vckbase
     
szName1何时被修改呢？因为没有明显的修改szName1代码。我们可以首先在A行设置普通断点，F5运行程序，程序停在A行。然后我们再设置一个数据断点。如下图： 
![](http://writeblog.csdn.net/document/journal/vckbase36/images/debug2.jpg)
**Figure 2** 数据断点
  F5继续运行，程序停在B行，说明B处代码修改了szName1。B处明明没有修改szName1呀？但调试器指明是这一行，一般不会错，所以还是静下心来看看程序，哦，你发现了：szName2只有4个字节，而strcpy了7个字节，所以覆写了szName1。 
  数据断点不只是对变量改变有效，还可以设置变量是否等于某个值。譬如，你可以将Figure 2中红圈处改为条件”szName2[0]==''''y''''“,那么当szName2第一个字符为y时断点就会启动。 
  可以看出，数据断点相对位置断点一个很大的区别是不用明确指明在哪一行代码设置断点。 
**三 其他**
  1 在call stack窗口中设置断点，选择某个函数，按F9设置一个断点。这样可以从深层次的函数调用中迅速返回到需要的函数。 
  2 Set Next StateMent命令（debug过程中，右键菜单中的命令） 
  此命令的作用是将程序的指令指针（EIP）指向不同的代码行。譬如，你正在调试上面那段代码，运行在A行，但你不愿意运行B行和C行代码，这时，你就可以在D行，右键，然后“Set Next StateMent”。调试器就不会执行B、C行。只要在同一函数内，此指令就可以随意跳前或跳后执行。灵活使用此功能可以大量节省调试时间。 
  3 watch窗口 
  watch窗口支持丰富的数据格式化功能。如输入0x65,u，则在右栏显示101。 
  实时显示windows API调用的错误：在左栏输入@err,hr。 
  在watch窗口中调用函数。提醒一下，调用完函数后马上在watch窗口中清除它，否则，单步调试时每一步调试器都会调用此函数。 
  4 messages断点不怎么实用。基本上可以用前面讲述的断点代替。 
**总结**
  调试最重要的还是你要思考，要猜测你的程序可能出错的地方，然后运用你的调试器来证实你的猜测。而熟练使用上面这些技巧无疑会加快这个过程。最后，大家如果有关于调试方面的问题，我乐意参与探讨。
引用:http://www.vckbase.com/document/viewdoc/?id=1262
