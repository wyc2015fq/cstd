# Decoda编译方法 - Ms.Fox的专栏 - CSDN博客
2018年07月16日 11:02:44[lulu-lu](https://me.csdn.net/smbluesky)阅读数：398
最近看到 有个很牛逼的lua IDE, decoda,  我就到官网下载来用了
http://unknownworlds.com/decoda/download/
可是安装后打开, 晕,还要注册  !!!
![](https://blog.csdn.net/fuzhufang/article/details/48000437)
这不是开源的吗? 
我觉得把代码下载下来,自己编译个, 该不用注册了吧 !? 
首先在开源中国里找到
http://www.oschina.net/p/decoda
git 地址是
https://github.com/unknownworlds/decoda
clone 下来后. 就准备编译 了. 
处于懒惰的思想,  先从网上找找有没有编译的教程, 结果看到网上也有很多人问 Decoda 编译方法, 暂还没有找到成功编译的文章. 
决定还是自己试试吧.  其实还是挺简单的
首先要下载 编译工具 Premake
http://premake.github.io/download.html#v4
解压后 放到 你刚才 clone decoda 目录下.
我这里是 F:\git\decoda
然后打开命令行 cmd
进入 decoda 目录
在cmd 输入
F:\git\decoda>premake4.exe vs2010 vcbuild decoda.sln --os=windows --platform=x64
生成vs工程项
Building configurations...
Running action 'vs2010'...
Generating build/Decoda.sln...
Generating build/Frontend.vcxproj...
Generating build/Frontend.vcxproj.user...
Generating build/Frontend.vcxproj.filters...
Generating build/LuaInject.vcxproj...
Generating build/LuaInject.vcxproj.user...
Generating build/LuaInject.vcxproj.filters...
Generating build/Shared.vcxproj...
Generating build/Shared.vcxproj.user...
Generating build/Shared.vcxproj.filters...
Done.
![](https://blog.csdn.net/fuzhufang/article/details/48000437)
我当时直接生成 vs2010 工程了, 
可是 git 下来的依赖库都是在 vs2013 下编译的,   如果不用vs2013 编译, 那么其他的依赖库, 你也要重新用你当前的vs重新编译, 太麻烦了! 
![](https://blog.csdn.net/fuzhufang/article/details/48000437)
没关系, vs2010 的工程可以直接用vs2013 工程打开, 打开时候提示某些工程文件要升级, 就升级好 了. 
![](https://blog.csdn.net/fuzhufang/article/details/48000437)
用vs2013 编译完毕后, (注意,必须编译release 版本哦)
![](https://blog.csdn.net/fuzhufang/article/details/48000437)
就看到 decoda 了. 
打开decoda.exe,  OK, 不用注册了! 
