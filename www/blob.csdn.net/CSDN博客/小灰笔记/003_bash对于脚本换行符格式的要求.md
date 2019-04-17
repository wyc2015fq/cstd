# 003_bash对于脚本换行符格式的要求 - 小灰笔记 - CSDN博客





2018年10月31日 23:02:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：144








    入门的bash脚本反复调试出问题，具体提示如下：
![](https://img-blog.csdnimg.cn/20181031230134174.png)

    排查了好久没有找出有什么不对的地方，后来发现使用vim编辑完的文件执行没有问题。而vim是我win10子系统中的自带软件。在此之前，我使用的编辑器是emacs，而且是Windows版本。一下子联想到了两个系统中换行符的差异。

    返回到之前错误的文件修改之后：
![](https://img-blog.csdnimg.cn/20181031230140463.png)

    没有完全修复，但是出现的新提示中的\r更加让我确信是这个问题。继续修改，其实即使是Windows版本的emacs最初的时候也给了unix文本编辑的模式。
![](https://img-blog.csdnimg.cn/20181031230149511.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

    最后修改成功：

![](https://img-blog.csdnimg.cn/20181031230158501.png)



