# ERROR: Res2Exe: Res2Res: ERROR: Line: 267 File: ...../checksum.cpp错误 - xqhrs232的专栏 - CSDN博客
2013年01月30日 10:46:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：968
原文地址::[http://blog.csdn.net/zmq5411/article/details/7679756](http://blog.csdn.net/zmq5411/article/details/7679756)
相关网帖
1.
### WinCE开发常遇到的问题和错误  ----[http://kuangguofang.blog.163.com/blog/static/5171065020112163712863/](http://kuangguofang.blog.163.com/blog/static/5171065020112163712863/)
今天编译wince5.0 出的错误！看了好多网友建议关闭防火墙，哈哈果然如此
wince6.0定制过程中，sysgen时出现错误 Error:res2res
原文地址::[http://blog.sina.com.cn/s/blog_5aea09bb0100k8y2.html](http://blog.sina.com.cn/s/blog_5aea09bb0100k8y2.html)
使用vs2005定制wince6.0的操作系统时，刚开始sysgen的时候0个错误，0个跳过，只有一些warning，感觉还挺成功的。但是由于功能的需要，修改了一些驱动，增加了一些和蓝牙相关的组件。重新sysgen编译项目，这次编译不到五分钟时间就错误的终止了，错误提示内容如下：
ERROR: Res2Res: Res2Res:  ERROR: Line: 267  …………\checksum.cpp
ERROR: Res2Res: Res2Res:  ERROR: Line: 267  …………\checksum.cpp
Res2Res: Error adding resources (-1)
这种错误看着不像是组件配置过程中出现的不匹配之类的错误，但是错误原因也无法判断。于是我就将先前增加的蓝牙驱动等组件去掉，还原为起初的BSP，重新sysgen编译，这次却也没有编译成功，和刚才的错误提示一样，也是不到五分钟时间就错误的终止了编译。这真是奇怪了，同样的东西，前后相差不过一个小时，电脑连重启都没有过，同样的vs2005居然先前编译通过了，现在却又不行了，真是匪夷所思。没办法啊，这个问题不解决工作就没法进行了，于是就上网查资料吧。查啊查，查啊查……居然是这个原因！
都是杀毒软件惹的祸！只要将杀毒软件关闭了，编译就可畅通无阻。出错不是因为我添加了蓝牙相关的驱动和组件。我重新对起初的BSP进行编译，成功通过没有问题；我增加了蓝牙驱动和组件，重新sysgen编译，也成功编译完成，没有再出现上述问题。
只是有一点尚不明白，刚开始的时候，我编译最初定制的系统，杀毒软件也是开着的，为什么就编译成功了？只是后来几次的编译杀毒软件才开始干扰？这个问题看来一时半会儿是找不到根本原因了。所幸的是关闭杀毒软件能正常工作了！
//========================================
备注::
1、今天试了一下还真的如此！！！害我还把WINCE60重新装了一遍。悲剧！
