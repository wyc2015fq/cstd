# Python脚本简介 - fanyun的博客 - CSDN博客
2017年04月04日 19:29:06[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：297标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[脚本语言](https://blog.csdn.net/fanyun_01/article/category/6838309)
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
[Python](http://doc.codesky.net/art/200509/3940.htm)作为一种功能强大的脚本语言，语法简洁，但功能强大，可以 跨平台使用，在Linux、Windows和Mac上都有很好支持。她的设计很出色。
[Python](http://doc.codesky.net/art/200509/3940.htm)脚本语言通常把想要操作的程序分为几个模块，首先要创建一个模块，把先关的Python脚本放入一个文件，这个文件名就是模块名。import语句创建一个新的名字空间，该空间包含模块中所有定义对象的名称。要访问这个名字空间，把模块名作为一个前缀来使用这个模块内的对象，就像上边例子中那样:div.divide()
[Python](http://developer.51cto.com/art/200509/3940.htm)脚本和模块都是一个以.py结束的文件，那程序是如何判断一个.py文件是作为脚本还是模块呢？关键是一个名为__name__的变量，如果它的值是__main__，则是作为脚本直接运行，否则是做为模块运行的。
init__ 函数是每次生成类的时候都会执行的,self 指向类对象自身记住, 类函数(或者叫做方法) 它的第一个参数 "self" 不要忘记写了。 
1） 命名规范类的首字母大写 
2） 函数和变量尽量全小写, 单词间下划线 
3） 类里面 "单下划线" 开始的成员变量叫做保护变量,意思是只有类对象和子类对象自己能访问到这些变量; 
4） 而 "双下划线" 开始的是私有成员, 意思是只有类对象自己能访问,连子类对象也不能访问到这个数据。
        Python脚本的创始人为Guido van Rossum。1989年圣诞节期间，在阿姆斯特丹，Guido为了打发圣诞节的无趣，决心开发一个新的脚本解释程序。做为 ABC 语言的一种继承。之所以选中Python（大蟒蛇的意思）作为程序的名字。
是因为他是一个Monty Python的飞行马戏团的爱好者。ABC是由Guido参加设计的一种教学语言。就Guido本人看来，ABC 这种语言非常优美和强大，是专门为非专业程序员设计的。
        但是ABC语言并没有成功，究其原因，Guido认为是非开放造成的。Guido 决心在 Python 中避免这一错误（的确如此，Python 与其它的语言如C、C++和Java结合的非常好）。同时，他还想实现在 ABC 中闪现过但未曾实现的东西。
就这样，Python在Guido手中诞生了。实际上，第一个实现是在Mac机上。可以说，Python是从ABC发展起来，主要受到了Modula-3（另一种相当优美且强大的语言，为小型团体所设计的）的影响。并且结合了Unix shell和C的习惯。
       Python在编程语言中的定位很多大规模软件开发计划例如 Zope， Mnet 及 BitTorrent. Google都在广泛地使用它。通常认为，Python是一种解释性的语言，但是这种说法是不正确的，实际上，Python脚本在执行时，首先会将.py文件中的源代码编译成Python的byte code（字节码）。
       然后再由Python VirtualMachine来执行这些编译好的byte code。这种机制的基本思想跟Java，.NET是一致的。然而，Python Virtual Machine与Java或.NET的VirtualMachine不同的是，Python的Virtual Machine是一种更高级的Virtual Machine。
       这里的高级并不是通常意义上的高级，不是说Python的Virtual Machine比Java或.NET的功能更强大，而是说和Java 或.NET相比，Python的Virtual Machine距离真实机器的距离更远。或者可以这么说，Python的Virtual Machine是一种抽象层次更高的Virtual Machine。
