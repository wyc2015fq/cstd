# 【编程小工具】通过depends工具查看dll - 三少GG - CSDN博客
2012年02月27日 13:17:46[三少GG](https://me.csdn.net/scut1135)阅读数：1530
[http://hi.baidu.com/chenchangxin/blog/item/32fef72ee3f9cc434ec22660.html](http://hi.baidu.com/chenchangxin/blog/item/32fef72ee3f9cc434ec22660.html)
Depends用来显示与一个可执行文件（exe或者dll）相关的依赖项（dll），以及该exe或dll引用了这些dll中的哪些接口。
同时也可以看到每个被依赖的dll中的所有导出的函数接口。
===============================================
VC6.0自带的Depends软件，该软件在VC6安装目录下的tools文件夹里面   D:\Microsoft Visual Studio\Common\Tools，直接
双击执行，然后打开exe文件即可，它主要有下面几个注意事项：
1）所编的软件所需的Dll文件，可以得到相应的Dll路径，版本，属性等。
2）窗口分四部分：
左上角是Dll信息窗口，显示你程序所需的Dll模块，
右边第一个窗口是所选的Dll模块所使用的函数，
右边第二个窗口是所选Dll模块的所有的导出函数，
下面窗口是所有需要的Dll模块的属性
3）右边两个窗口出现四个标签：序号，提示，函数，入口点
如果利用导出是函数，那么出现函数名，如果导出的是序号，那么函数项就是N/A（无法显示）
出现红色提示表示不正常，一般为没有该导出函数
4）该工具得到的是你软件中隐式链接的Dll库，也就是用lib关联的Dll模块，
无法显示显式链接的Dll模块，也就是用LoadLibrary函数导入的Dll函数。（切记切记！）
5）无法提供Borland C++ Builder所提供的Dll文件，无法提供vxd软件的调用
下面是一个myDll：
![](http://hiphotos.baidu.com/chenchangxin/pic/item/c890b481fad44689bd3e1e3b.jpg)
下面是一个调用myDll的test.exe
![](http://hiphotos.baidu.com/chenchangxin/pic/item/9a7ed7b3b9d91df6d8335a1a.jpg)‍
