# 通过创建MapFile来定位程序崩溃地址 - gauss的专栏 - CSDN博客
2013年01月04日 00:21:49[gauss](https://me.csdn.net/mathlmx)阅读数：408
个人分类：[开发工具/调试技巧](https://blog.csdn.net/mathlmx/article/category/1317877)
想必大家对于程序莫名其妙的程序崩溃感到苦恼了，但更苦恼的却是没有一个好的方法去解决它。
近日，看了[这篇的文章](http://www.codeproject.com/debug/mapfile.asp)，甚有大的收获。现将心得记录下来，以供大家分享。
我就直接列出步骤了：
1、在图一的Debug Info中，可以选中Program Database for Edit and Continue或按原文中的说法，选择Line
 Numbers Only，选择此项的好处是会使得应用程序变小，但你在Debug模式下却无法查看变量值。(但是对于Release模块下，不能选Program
 Database for Edit and Continue，你只好选Line Numbers Only了，具体选项意义请参考MSDN。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ytfrdfiw/p1.JPG)
图一
2、在图二中，选中Generate mapfile，并在Project Options中输入/MAPINFO:LINES
 /MAPINFO:EXPORTS，到此，你可以在你的Debug目录下找到一个以 “.map”结尾的文件名，用UE打开它。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ytfrdfiw/p2.jpg)
图二
MAP文件的大致情况如图三：(注意红圈)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ytfrdfiw/3.jpg)
图三
好了，我们看似做了很多的无用工作，还没有进入主题，让我们来看一下，如何利用这个map文件来定位我们的程序崩溃位置。
我们在我们的程序main函数中输入下面的代码：
char *p = NULL;
*p = ‘x’;                             //这行代码有问题了。
让我们运行这个程序，则会弹出以下对话框，图四。
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ytfrdfiw/4.jpg)
图四
然后我们点“请单击此处”，图五
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ytfrdfiw/5.jpg)
图五
现一次点“请单击此处”，看你出不出来J.
好了，这次终于出来了。（图六）看，我们红线圈住的地方，就是我们程序的崩溃地址，但这对我们要了解程序出错地还是没有什么用，怎么办呢？
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ytfrdfiw/6.jpg)
图六
3、紧跟在图三所示后的map文件是这样的（图七）
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ytfrdfiw/7.jpg)
图七
请各位看官注意红圈，左边第一个，表示我们的函数名，当然它有一些修饰符，大概看懂就行了。看不懂，可以查资料J。
左边第二个红圈，表示这个函数在这个进程中的起始位置。噢，你是不是知道了一些什么。对了，让我们看看图六中的崩溃地址，它是0x00401080。
最后一次红圈是指这个函数在哪个obj文件中。
好了，接下来，到我们整个故事的关键了（图八）
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ytfrdfiw/8.jpg)
图八
我们可以看到有形如这样的东东“12 0001:00000080”，这个有什么作用呢？
我们能过这个公式，就可以算出我们程序崩溃源代码的位置了。
**程序崩溃源代码的位置 = 图六的Address值 –图三的Preferred load address – 0x1000**
这样我们先套用一下公式，得到我们的值是0x00000080，但究竟源代码出错在哪一行，因为有可能有多个形如12 0001：00000080这样的东东。我们要记住，我们在图七中的内容，因为崩溃地址是对应的obj是hello.obj，所以我们对应的cpp文件是hello.cpp，所以到此我们可以定位出我们的程序出错地址在源代码“hello.cpp”的12行。
结束了，一切都结束了。
注：在我们的公式中，为什么要减去0x1000，这是由于PE头的大小是0x1000。代码真正开始的位置是在0x1000以后的，这个0x1000是个相对位移，不是绝对位移J。
DLL崩溃，用同样的方法，也可以得到源代码出错所在行。
