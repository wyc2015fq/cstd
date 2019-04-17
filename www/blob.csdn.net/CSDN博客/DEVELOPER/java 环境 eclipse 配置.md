# java 环境 eclipse 配置 - DEVELOPER - CSDN博客





2014年04月25日 17:39:57[学术袁](https://me.csdn.net/u012827205)阅读数：878








电脑系统重复的安装，导致开发环境也要不断的进行配置。但是，配置它都是会忘记怎么弄的步骤。不说了，看完下面的配置方式你都会瞬间明白——原来这么简单！




首先，安装步骤：**jdk**安装+环境配置——>**eclips**e安装+**ADT**配置+**SD**K配置
   ==**android**开发




***步骤一：*jdk**安装+环境配置：

安装jdk：![](https://img-blog.csdn.net/20140425173906671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

配置环境：电脑图标/属性/高级系统变量/环境变量/系统变量/新建

在系统环境变量中配置，新建一个文件夹**CLASS_PATH**，在这里放你安装**jdk的lib文件夹的据对路径**（如："D:\AndroidDP\AndroidDP\JDK\lib"）

然后在系统变量的Path文件夹中添加**jdk的bin文件夹的绝对路**径并注意标点的使用。（如：";D:\AndroidDP\AndroidDP\JDK\bin;"）




***步骤二*：eclips**e安装+**ADT**配置+**SD**K配置：

安装eclipse：

然后安装ADT,并在eclipse中做一下步骤：help/Install New Software.../add/ADT.rar<ADT的压缩包>

在Name栏全选，Details栏下面的复选框除了最后一个不勾选。

![](https://img-blog.csdn.net/20140425172229031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




除了上图那个不勾选其他全选，然后Finish就行了。如果出现问题，在下图中

![](https://img-blog.csdn.net/20140425172517328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

的第二个复选框取消勾选，然后再Finish就好了。但是对于开发者的我们来说，尽量使都要装上去，有益无害嘛！




然后再跟着eclipse的提示走<这个是有前提的，要在你第一次安装时候>，完成后会有一个提示框，点进去安装SDK,把SDK的压缩包所解压的文件Browse进去就好了。但是也有异常情况发生，当上面的操作没有出现的时候。那只有手动操作了，操作步骤：Windows/Preferences/...然后就把SDK的压缩包已经解压的文件Browse进去就OK了。

![](https://img-blog.csdn.net/20140425173327296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







