# widow下svn上传项目时的文件可执行权限问题 - DoubleLi - 博客园






还是项目上发现的问题，要上传[Android](http://lib.csdn.net/base/15)的源码项目。这里客户端是windows的机器，

 测试后发现俩个问题。

**1. 文件后缀是.so的文件默认上传不了。**

**   2. 文件后缀是.sh的文件，上传后，再从服务器上下下来，文件的可执行权限就会丢失。说明在上传的时候文件的可执行权限没有上传。**



    问题1，解决方法：右击-->TortoiseSVN-->Setting   进入如下界面。

![](http://img.blog.csdn.net/20140412164541828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWFpbHlpcmFuMjAwMTAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



    在上图中

   在Global ignore pattern中设置即可。

    原因分析：svn服务器要设置上传过滤；很多文件，会被过滤掉，不能正常上传。把选项中的*.so去掉，然后保存。



    问题2，解决方法：

  上网查了一圈，发现确实是window版本的默认问题，也看了不少解决方法，要不在ubuntu下操作上传，要不在windows下只能对单个文件进行操作。对于文件很少的情况还好处理，但是比如像这个android的源码项目的上传就比较麻烦了。如果网络可以，当然建议直接在ubuntu上上传。不过我这里的情况是暂时只能从windows下上传。

   后来经过实验，也解决了这个问题。

   还是打开上图：点击按钮 Edit;



![](http://img.blog.csdn.net/20140412164604515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbWFpbHlpcmFuMjAwMTAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

      弹出一个名为：config的svn默认配置文件：

找到以下两条

#enable-auto-props= yes

# *.sh =svn:eol-style=native;svn:executable

修改如下：

enable-auto-props = yes

*.sh = svn:eol-style=native;svn:executable=*

保存后退出，退回到上图，点确定退出，好了问题解决，赶快试一下把。









