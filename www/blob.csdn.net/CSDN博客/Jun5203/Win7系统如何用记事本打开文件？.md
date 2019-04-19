# Win7系统如何用记事本打开文件？ - Jun5203 - CSDN博客
2018年10月25日 13:28:40[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：350
所属专栏：[Linux](https://blog.csdn.net/column/details/25476.html)
现在大多数人们的电脑都是Win10系统，各别装Win7的真是少之又少。但是在虚拟机中Win7却是一个非常棒的选择，相比较而言Win7系统的功能肯定不如Win10完善，而且在Win7系统中很多文件的打开方式也比较麻烦。我们是否更简单的用记事本打开某个文件呢？下面就跟着我来学习一下吧！
1、“Win+R”打开“运行”；
![在这里插入图片描述](https://img-blog.csdn.net/2018102513230939?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、输入命令“regedit”，打开“注册表编辑器”窗口；
3、在注册表编辑器窗口的左侧依次打开路径选项为：“HKEY_CLASSES_ROOT*\shell”；
4、在“shell”选项下新建项“用记事本打开”；
![在这里插入图片描述](https://img-blog.csdn.net/2018102513240897?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
6、选中“用记事本打开”选项，在注册表编辑器右侧右键空白处“新建——字符串值”，数值名称为“command”，数值数据为“c:\windows\notepad.exe %1”；
![在这里插入图片描述](https://img-blog.csdn.net/20181025132439762?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
7、关闭窗口，在系统中右键点击文件就可以看到“用记事本打开”的选项。
![在这里插入图片描述](https://img-blog.csdn.net/20181025132600415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0VsbGVuNTIwMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
学无止境，只希望在同样的时间里做更多的事情，不仅要提高效率，还要保证质量，You Can！
