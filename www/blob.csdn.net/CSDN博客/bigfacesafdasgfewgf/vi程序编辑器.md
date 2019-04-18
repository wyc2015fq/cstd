# vi程序编辑器 - bigfacesafdasgfewgf - CSDN博客





2015年05月04日 16:09:36[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：461标签：[编辑器																[vi](https://so.csdn.net/so/search/s.do?q=vi&t=blog)](https://so.csdn.net/so/search/s.do?q=编辑器&t=blog)
个人分类：[Linux](https://blog.csdn.net/puqutogether/article/category/3147399)











    Linux下常用的一个编辑器是vi。




    vi基本上有3种模式可用：一般模式、编辑模式、命令行模式。




![](https://img-blog.csdn.net/20150504165336968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





# 1）一般模式

     vi打开一个文件就直接进入了一般模式，默认为一般模式。

     一般模式中，我们可以使用上下左右键来移动光标，实现查看；也可以删除字符或者删除整行；我们还可以复制粘贴文件数据。在一般模式下，按":wq"保存后离开。




# 2）编辑模式

    vi打开文件后，按i, o, a, r（大小写均可），进入编辑模式。此时界面左下方会出现insert或者replace，可以编辑。按下esc就会退出编辑模式，回到一般模式。




# 3）命令行模式

    vi进如一般模式后，按下： / ?这3种字符中任何一个即进入命令行模式，光标移动到最下面一行。在此模式中，我们可以查找数据，读取，保存，替换字符。按下esc就会退出编辑模式，回到一般模式。




注意：编辑模式和命令行模式之间是不可以切换的。






















