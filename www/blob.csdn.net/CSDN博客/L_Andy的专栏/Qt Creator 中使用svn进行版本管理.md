# Qt Creator 中使用svn进行版本管理 - L_Andy的专栏 - CSDN博客

2017年02月06日 11:51:59[卡哥](https://me.csdn.net/L_Andy)阅读数：862



最近做的一个项目是用Qt Creator做的。开发环境在windows7 下。公司有台linux的服务器，上面装了SVN。昨天项目经理要求我把自己的代码上传到那台linux服务器上。没办法，去网上找了好多资料。都没有看到有这方面的文章。那就自己动手研究吧！

        刚开始的时候。一点头绪都没有，不知道如何才能将自己电脑里的代码上传到服务器上去。后面想了一个偷懒的办法。先将代码用FileZilla上传到那台服务器上去。在通过putty在服务器上得svn中创建了一个仓库。（具体的配置参见：http://www.cnblogs.com/peixiang/archive/2010/03/11/1683620.html 讲的很详细。）后面发现，这样做很麻烦，没办法。还得自己想办法解决。。。

下面就开始自己的Qt creator中使用SVN的摸索。

        1.在配置服务器上的SVN时有一条检出命名：svn co svn://127.0.0.1 启发了我。觉得，只要在windows中通过IP地址是不是也能访问linux服务器？也能够检出项目呢？

        于是去Qt Creator的 Tools->OPtions中去看了看。subversion中有相关的设置。![](http://hi.csdn.net/attachment/201201/18/0_13268648028Ok8.gif)

        svn.exe   这个东西很是不解。莫非在windows下也得要装一个svn客户端？

        于是去网上搜了一大堆资料。终于找到了：VisualSVN   下载地址：http://www.visualsvn.com/downloads/

        安装上了VisualSVN后提示没有TortoiseSVN  不能正常的运行，没办法，只有去下一个。下载地址：http://www.visualsvn.com/visualsvn/download/tortoisesvn/

        装上了以后就找到VisualSVN安装目录选中svn.exe

        如下图所示：

![](http://hi.csdn.net/attachment/201201/18/0_13268653304nuc.gif)

        点确定就OK了。这就暂时不用管了。

        2.在打开的Qt Creator中右击左边的项目框![](http://hi.csdn.net/attachment/201201/18/0_1326865479dRKh.gif)  选择新建项目

        3.如图所示：不在啰嗦

![](http://hi.csdn.net/attachment/201201/18/0_1326865581q4cc.gif)

        4. 在仓库中输入如下的URL： svn://192.168.0.10  (具体的IP，自己配置。)

![](http://hi.csdn.net/attachment/201201/18/0_1326865681jZl7.gif)

![](http://hi.csdn.net/attachment/201201/18/0_1326865829JmiF.gif)

        5. 下一步就OK了。

        6.以后你修改了代码需要提交，键入检出等等操作。直接去  Tools -> subversion 中操作即可。

![](http://hi.csdn.net/attachment/201201/18/0_1326865909G1mv.gif)

        总结：其实遇到这类问题的问题。真真去动手解决，过程可能会痛苦一点。结论，还真是蛮简单的。希望能够帮助到需要的人。

