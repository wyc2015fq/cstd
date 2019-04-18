# VS2015 MATLAB混合编程之COM组件 - fighting！！！ - CSDN博客
2017年11月04日 22:31:20[dujiahei](https://me.csdn.net/dujiahei)阅读数：1859
原地址：VS2015 MATLAB混合编程之COM组件_梦中萤火虫_新浪博客  http://blog.sina.com.cn/s/blog_8d5bae080102x792.html
概述：利用m文件生成dll再由vs2015的c#程序调用。
1：matlab编译器设置
在Command Window窗口中输入mex-setup和mbuild –setup查看编译器，如果没有，需将编辑器设置为相应的vs，下图为matlab2015b调用vs2015的正确截图。
![VS2015 <wbr>MATLAB混合编程之COM组件](http://s15.sinaimg.cn/bmiddle/002AuYoUzy7bI0H3jhc8e&690)
点评：这一步花了太久太久的时间，因为电脑上最开始装的是matlab2012b ,早版本的matlab都有自带的编译器，输入mex-setup出来的是自带的编译器。最开始按照网上的教程“matlab要找windows sdk的mt.exe，找不到报错.因为vs2013在安装时，windows sdk的mt.exe位置变了，而matlab还是按照老的路径找，肯定找不到.解决：网上找到相应文件，将mbuildopts和mexopts两个文件夹复制到 (matlabroot)\bin\win64 下，(matlabroot)是matlab的安装目录”。按照各种博客一步步改了mbuildopts和mexopts两个文件夹里面的文件内容，仍然行不通，这个方法有待考察。
忙了一天所以最后重装了matlab2015b, 就可以连接上编译器了。
2：安装MCRinstaller.exe 
位置在安装目录下的toolbox\compiler\deploy\win64文件夹中。
3：注册dll 
对mwcomutil.dll进行注册。这个dll文件位于安装目录下的\bin\win64文件夹中（右键点击开始，命令提示符（管理员），cd+’空格’+路径，regsvr32 mwcomutil.dll）
4：生成.m文件 
这是我所使用的函数，名称为add
function [c]=add(a,b)
c=a+b;
end
5：生成dll文件 
在在Command Window窗口中输入deploytool,并选择Library Complier,按照下图进行配置,选择Generic COM Component类型；然后把add.m文件添加到project里面，其中Class name 为在c#中所调用的类的名字。最后点上面菜单条上的package.
![VS2015 <wbr>MATLAB混合编程之COM组件](http://s16.sinaimg.cn/bmiddle/002AuYoUzy7bI1skhzh4f&690)
然后生成，那个add.dll就是c#将要调用的dll文件.
![VS2015 <wbr>MATLAB混合编程之COM组件](http://s9.sinaimg.cn/bmiddle/002AuYoUzy7bI1F60tid8&690)
6、开始写c# 
引用添加之前生成的那个dll文件。
![VS2015 <wbr>MATLAB混合编程之COM组件](http://s16.sinaimg.cn/bmiddle/002AuYoUzy7bI1MsrqD5f&690)
最后就可以运行查看结果了，运行比较慢=。=
![VS2015 <wbr>MATLAB混合编程之COM组件](http://s2.sinaimg.cn/bmiddle/002AuYoUzy7bI1TmQGR51&690)
总而言之一个小的case就算跑通了，走下来主要卡在了第一步连接编译器，各种版本的软件之间的兼容性啊适用性啊都很玄乎，网上的教程也都是五花八门。如果不是重装的2015b这么强大，我估计还要折腾好久。
参考：http://blog.sina.com.cn/s/blog_6f7265cf0101nqpi.html
http://blog.csdn.net/qq_22033759/article/details/48122383
=============分割线：后续碰到的问题=====================
1：返回值的个数
a.add(1, ref result, 5,6);//1表示返回的结果数量，要小于等于matlab对应函数实际的返回值数量。
2：返回的类型
c#中返回的结果是object类型，那么当返回的是数组，C#中如何进行类型的转化。这个查了很久的文献
http://bbs.csdn.net/topics/330032260 以及http://blog.csdn.net/zzhays/article/details/13623151
诸如上面两篇试过很多种方法，最后想到一种土方法：(double[,])result_x;这样就能直接用了，值得注意的是matlab中的1维数组到c#中会变成2维的。
3：一个dll中多个方法
修改类名，每一个类名对应一个方法。

