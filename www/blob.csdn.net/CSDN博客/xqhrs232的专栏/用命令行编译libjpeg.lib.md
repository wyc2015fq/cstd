# 用命令行编译libjpeg.lib - xqhrs232的专栏 - CSDN博客
2013年07月08日 16:16:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：881
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/shuixin536/article/details/5706820](http://blog.csdn.net/shuixin536/article/details/5706820)
今天在折腾libjpeg，这是一个开源的JPG文件的编码函数库。因为DirectX中提供了完善的图像文件格式支持，而OpenGL在这方面却显得非常简陋，只好自己来写JPG文件读取函数。而且libjpeg只提供了从文件读取的函数，无法处理内存中的数据，也就是说在处理打包的资源时会比较麻烦。幸好已经有高手写出了修改libjpeg代码的方法，需要重新编译libjpeg.lib。在修改之前先尝试了用命令行（Visual C++ 2008 Express Edition）来编译代码，遇到了一些问题，还好都不是大问题，都解决了，呵呵，赶紧记下来免得忘了。
1、从[ftp://ftp.simtel.net/pub/simtelnet/msdos/graphics/jpegsr6.zip](ftp://ftp.simtel.net/pub/simtelnet/msdos/graphics/jpegsr6.zip)下载libjpeg的源代码
2、解压出来一堆文件，打开命令行窗口（CMD），进入代码所放的目录下
3、运行命令：nmake /f makefile.vc nodebug=1 开始编译，然后问题就来了
4、“nmake 不是内部或外部命令”
    　　这个执行文件在VC的安装目录中，解决方法：输入命令 path C:/Program Files/Microsoft Visual Studio 9.0/VC/bin 回车
5、makefile.vc<11>:fatal error U1052: file 'win32.mak' not found
    　　提示找不到win32.mak文件，这个文件定义了一些必须的宏，这个文件在Windows SDK的安装目录中，打开makefile.vc文件修改第12行为：
    !include <C:/Program Files/Microsoft SDKs/Windows/v6.0A/Include/win32.mak>
    　　保存，再执行编译命令又有新的错误。
6、NMAKE:fatal error U1073: don't know how to make 'jconfig.h'
    　　这是个奇怪的问题，在网上搜了半天没有结果，最后才发现原来目录中没有这个文件。其实只要把文件jconfig.vc更名为jconig.h即可。再执行编译命令又有新的错误。
7、NMAKE: fatal error U1077: '.../cl.EXE' : return code '0xc0000135'
    　　CL命令未能执行，我们直接在命令行提示后面输入cl回车，会有个提示“没有找到 mspdb80.dll ”。解决方法：输入 vcvars32 回车，这是一个用来设置VC路径环境的批处理。再次执行编译命令，出现了一行行字符快速的刷新，哈哈，已经开始编译了。最后又出现一个错误，这个就不用管它了，因为我们需要的libjpeg.lib文件已经出现在了当前目录中。
[ ![](http://www.mcore.com.cn/img2/reply.gif) 说两句？好啊，点这里哦 ]
|**0**|**einmus**2009-06-12 21:19|把jconfig.vc更名为jconfig.h，用VC提供的命令行，运行你的第3步就不会有其他的麻烦了。|
|----|----|----|
|**1**|**johnny孙**2009-06-17 18:41|谢谢楼上的朋友，你说的操作在第6步有做，很多问题都是出现在第一次使用命令行编译时。|
|**2**|**xluo**2009-09-07 19:42|你真是人才,这都被你找到了.|
|**3**|**einmus**2009-09-15 20:57|不知道你解决这个问题没有，我说的“VC提供的命令行”是说，Visual Studio提供的命令行，在开始菜单里面可以找到，而不是，你在第二步里面直接运行的cmd。应该是环境变量一类的区别吧。在那个命令行里面输入之后的命令就不会有问题了。|
|**4**|**Johnny孙**2009-09-16 14:30|哦，非常感谢楼上的朋友，我还不知道VC有个命令行，呵呵，试试看。|
//==================================================================================
备注：：
1》要在WINDOWS环境下编译，需要去下载WINDOWS版本的源代码
2》针对VC++6.0环境其win32.mak文件所处的路径为----C:\Program Files\Microsoft Visual Studio\VC98\Include
