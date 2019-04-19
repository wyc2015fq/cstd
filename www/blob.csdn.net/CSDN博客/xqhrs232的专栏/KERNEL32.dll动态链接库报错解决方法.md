# KERNEL32.dll动态链接库报错解决方法 - xqhrs232的专栏 - CSDN博客
2015年12月08日 23:52:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：7147
个人分类：[PC/Windows/Office技术](https://blog.csdn.net/xqhrs232/article/category/906929)
原文地址::[http://jingyan.baidu.com/article/a3aad71ada1efeb1fb0096ec.html](http://jingyan.baidu.com/article/a3aad71ada1efeb1fb0096ec.html)
　在Windows　XP安装游戏或一些程序时，如百度云管理等软件面向Windows　7高版本的系统，对Windows　XP兼容性不那么注重了，会出现错误提示：“定位程序输入点GetlogicalprocessorInformation于动态链接库KERNEL32.dll上”，这不是.net没有安装导致，而是Windows　XP系统本身环境导致，如遇到这样的错误提示，请参考下面的经验步骤，或许能帮到你。
## 工具/原料
- 
SP3补丁包
- 
SP2环境
## 方法/步骤
- 
1
　　这类现象在安装游戏或一些新版本软件会出现，这里就以安装百度云管家为例，下载完软件程序之后，双击安装，如图安装的时候没有出错误提示，但只是抽取了程序包中的文件到安装目录文件夹中。
![KERNEL32.dll动态链接库报错解决方法](http://d.hiphotos.baidu.com/exp/w=500/sign=10ab621b3a12b31bc76ccd29b6193674/908fa0ec08fa513d45b192823e6d55fbb2fbd957.jpg)
- 
2
　　桌面不会有正常安装完之后的软件图标，找到安装目录文件夹下的可执行的软件EXE，双击打开，出现上面介绍的错误提示。经过验证，是由于Windows　XP版本是SP2的，而不是去下载KERNEL32.dll文件能解决的。
![KERNEL32.dll动态链接库报错解决方法](http://c.hiphotos.baidu.com/exp/w=500/sign=f9937b91ff039245a1b5e10fb795a4a8/4afbfbedab64034f42ad4f5aacc379310a551d0d.jpg)
- 
3
　　进入百度搜索SP3补丁包，选择可靠一点的软件网站进行下载
![KERNEL32.dll动态链接库报错解决方法](http://g.hiphotos.baidu.com/exp/w=500/sign=1dff7fe38f5494ee87220f191df5e0e1/d0c8a786c9177f3e8928025173cf3bc79f3d564a.jpg)
- 
4
　　等待文件下载完成，双击执行补丁包安装更新Windows　XP系统。
![KERNEL32.dll动态链接库报错解决方法](http://d.hiphotos.baidu.com/exp/w=500/sign=c908e699e3fe9925cb0c695004a85ee4/c83d70cf3bc79f3d0bfd1a6cb9a1cd11728b294a.jpg)
- 
5
　　补丁包会将文件提取到临时文件夹中，等待提取完之后，软件会自动执行安装。
![KERNEL32.dll动态链接库报错解决方法](http://d.hiphotos.baidu.com/exp/w=500/sign=5271fe46c55c1038247ecec28211931c/d4628535e5dde7112bcebdaaa4efce1b9d16614a.jpg)
- 
6
　　注册更新补丁包的时候关掉所有打开的软件或程序窗口。点击：“下一步”。
![KERNEL32.dll动态链接库报错解决方法](http://e.hiphotos.baidu.com/exp/w=500/sign=6329727ecbfcc3ceb4c0c933a244d6b7/83025aafa40f4bfbdfd46008004f78f0f7361861.jpg)
- 
7
　　进入更新安装向导，按照提示一步一步安装完成，然后重新启动电脑，Windows系统会进行更新配置。
![KERNEL32.dll动态链接库报错解决方法](http://b.hiphotos.baidu.com/exp/w=500/sign=6b471b7a0a24ab18e016e13705fae69a/4b90f603738da97700337877b351f8198618e34a.jpg)
- 
8
　　配置完成之后，进入系统，再找到百度云管家安装程序，双击之后就不会出现第一步当中图示那样显示抽取文件，而如下图中出现程序LOGO显示安装中。。。
![KERNEL32.dll动态链接库报错解决方法](http://a.hiphotos.baidu.com/exp/w=500/sign=59aa503c4cc2d562f208d0edd71090f3/810a19d8bc3eb135d36849e7a51ea8d3fd1f443b.jpg)
END
## 注意事项
- 
请点【投票】以资鼓励；觉得帮了小忙，可在评论中给个【好评】；还可以点击右边的【双箭头】来分享；怕下次出问题之后忘记找不到，可点击【☆】来收藏。若有不足，请给出您的方法。点击”关注”关注我，点击“sanpitt”进入我的主页查看相关经验，收获更多。
