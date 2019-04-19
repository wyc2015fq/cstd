# 从病毒到AU3编程语言 - Big Smile - CSDN博客
2016年05月27日 18:05:10[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2537
        不知道大家有没有碰到过这种病毒，就是开机提示你Googleupdate未能成功启动，等等之云，然后再C盘的目录下可以看到有个Google文件夹，里面有一些文件，其中有Googleupdate.a3u这个文件，文件图标是这个样子：![](https://img-blog.csdn.net/20160527171318053?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，这个就是一种恶意软件病毒，Autoit3病毒，他的清除方法见网址：[百度文库-Autoit3病毒清除](http://wenku.baidu.com/link?url=tqhjyiar6v20GPNNj8tQUWXpIl07qOPF16cylo1urogYLbcRKVgP8eysnhAoEf0Am5BSahrR2EyG7Av49Wxh5XgoC4VlbytzTPlWCl0lktO)
# 一、从病毒到AU3
        其实发现AU3这个编程语言也是一个偶然，在IT天空上面转的时候就发现了AU3爱好者区域，于是充满好奇的我就点进去看了看，究竟AU3是什么，其实AU3就是AutoIt3 Windows自动安装脚本语言的缩写，基于windows的脚本语言就和vbs，bat脚本差不多的东西，以Windows为环境才能够发挥作用。
        这么一看这个病毒的前称和AU3语言的前称非常的像，由此可知这个病毒就是由AU3来编写的，那么AU3语言究竟是什么，竟然如此强大，可以以一个脚本语言的身份编写出来病毒程序呢？
# 二、AU3编程语言简介
        Au3脚本是AutoIt3 Windows自动安装脚本语言，AutoIt 是一种自动控制工具。它可以被用来自动完成任何基于 Windows 或 DOS 的简单任务。它最初被设计用来自动完成安装那些其它方法不能自动安装的软件。
        由于它可以完成基于Windows或者Dos的简单任务，因此在Windows系统中它具有强大的功能，成功的例子有很多，像万能驱动助理，一键软件安装器等等安装工具都是用AU3脚本进行编写的。不仅如此，它还可以调用offic的VBA、宏等命令进行操作，有些office的辅助软件也是用AU3来制作的，可见他的强大之处。
        但是，编程语言虽然强大，编出的软件却要因编程者而异，AU3可以模拟鼠标、键盘各种事件，还提供了对大多数win32 API的封装，所以一些不法分子就可以用这么强大而又方便的编程语言进行病毒的编写了，这几年用AU3编写的病毒也不在少数，所以用AU3软件编写敏感程序的时候可能会遭到杀毒软件的误杀，这点要注意。
一个很不错的推荐帖子：[http://www.xuebuyuan.com/1106293.html](http://www.xuebuyuan.com/1106293.html)
# 三、对于我们的用处
        由于AU3对于Windows的各种操作和模拟比较强大，所以可以用作基于Windows的软件的自动测试，包括cs的程序和web的程序。由于AU3的设计可以捕获窗口对象，所以在测试的时候可以随时捕捉报错还有进行输入输出，对于高效的测试很有帮助。当然如果我们有兴趣也可以写一些可以帮助自己的小程序AU3提供可界面编程，所以可以很轻松的做出窗体界面。
# 四、总结
        这样的一款强大而使用的编程语音，对于他的认识竟然是基于病毒的，可见一样的工具会做成不同的结果，真的是一把双刃剑，所以对于自己编程能力的的应用也要慎重，我们的能力越高，责任就越大，所以我们以后一定要当一名正义的程序设计师，不要当写木马的黑客。
想要了解AU3的同学可以进入中文官方论坛进行了解：[http://www.autoitx.com/](http://www.autoitx.com/)
