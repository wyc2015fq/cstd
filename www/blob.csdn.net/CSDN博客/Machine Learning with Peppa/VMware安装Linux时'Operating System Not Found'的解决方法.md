# VMware安装Linux时'Operating System Not Found'的解决方法 - Machine Learning with Peppa - CSDN博客





2018年03月06日 23:30:02[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：28237标签：[Linux																[系统																[编程																[计算机																[虚拟机](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)
个人分类：[***系统与网络***																[系统：Linux与Shell](https://blog.csdn.net/qq_39521554/article/category/7479173)](https://blog.csdn.net/qq_39521554/article/category/7361345)

所属专栏：[Linux私房菜](https://blog.csdn.net/column/details/20153.html)





今天在WIN10上安装CENTOS 7，VM出现了几个问题，找了很多方法终于解决并且成功安装。其中最麻烦的就是这个operating system not found。下面给出解决方法：

第一步：单击这个地方

![](https://img-blog.csdn.net/2018030623352099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





第二步：选择**Use ISO image file**

这一步的原因是VM默认的自动选择，但很多时候目录都是错的。还有一点，**ISO的路径里不能出现中文**

![](https://img-blog.csdn.net/2018030623323060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





第三步，点OK

![](https://img-blog.csdn.net/20180306233359450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





最后断电重启，从BOOT进去就可以开始设置安装向导了。](https://so.csdn.net/so/search/s.do?q=计算机&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=系统&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)




