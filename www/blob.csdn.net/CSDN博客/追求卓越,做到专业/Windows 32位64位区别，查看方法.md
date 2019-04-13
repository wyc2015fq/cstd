
# Windows 32位64位区别，查看方法 - 追求卓越,做到专业 - CSDN博客


2014年08月07日 12:46:53[Waldenz](https://me.csdn.net/enter89)阅读数：857


术语“32 位”和“64 位”是指计算机的处理器（也称为“CPU”）处理信息的方式。64 位版本的 Windows 可处理大量的随机存取内存 (RAM)，其效率远远高于 32 位的系统。

**查看我的计算机运行的是32位还是64位的Windows?**
1.对于Win 7：单击【开始】按钮，右键单击“计算机”，单击属性，在系统下的“系统类型”显示32位操作系统或者64位操作系统。
![](https://img-blog.csdn.net/20140807113037626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZW50ZXI4OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. 还有一种方法，是对Win7, Vista 或者Win XP都可以的。
在【开始】中找到【运行】，输入cmd, 然后输入systeminfo, 回车，会显示出系统的详细信息。
![](https://img-blog.csdn.net/20140807113512483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZW50ZXI4OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在系统类型中，显示“x86-based PC”则为32位操作系统，显示“x64-based PC”则为64位操作系统，上图中我的计算机则为64位的Win7。

**安装哪个版本的 Windows 7：32 位版本还是 64 位版本？**
要安装 64 位版本的 Windows 7，电脑的 CPU 需要能够运行 64 位版本的 Windows，目前绝大多数的双核处理器都是64位的。当电脑上安装有大量的随机存取内存 (RAM)（通常为 4 GB 的 RAM 或更多）时，使用 64 位操作系统的优势最为显著。在这种情况下，因为 64 位操作系统较 32 位操作系统而言能够更加高效地处理大容量的内存，所以当有多个程序同时运行且需要频繁切换时，64
 位系统的响应速度更快。
所以你的机子如果是双核的CPU，而且内存为4G或者更多，那就推荐安装64位的操作系统。
本文引用文献：[http://windows.microsoft.com/zh-cn/windows/32-bit-and-64-bit-windows\#1TC=windows-7](http://windows.microsoft.com/zh-cn/windows/32-bit-and-64-bit-windows#1TC=windows-7)

