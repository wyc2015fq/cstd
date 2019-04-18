# 使用LAV Filter要注意的问题 - 爱写代码的自由人 - CSDN博客





2018年02月24日 15:42:59[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：7651








前面的博文介绍了 LAV Filter 是一款很不错的开源的DirectShow视频分离和解码插件。但是使用时有一些要注意的地方。

第一，是怎么注册。LAV Filter的目录下有3个Bat文件，分别运行这3个BAT文件，注册成功会有提示。但是有些用户注册的时候提示报错，是什么原因呢？这个一般是因为你执行命令的权限不够，需要提高到管理员权限运行，方法是：在系统的开始菜单中找到“命令提示符”，然后按右键，在右键菜单中选择“以管理员权限运行”，然后在弹出的CMD窗口中切换到Filter所在的目录，接着输入注册的三个命令：


regsvr32  LAVSplitter.ax


regsvr32  LAVVideo.ax


regsvr32  LAVAudio.ax


这样就提示注册成功。

**（注意：目前LAVFilter有32位和64位的版本，如果你的Directshow程序是32位的，应该安装32位的版本。）**




第二，怎么在GraphEdit中查看这几个Filter。成功安装到系统之后，我们打开GraphEdit，点击工具栏的“insert a filter to graph"按钮，出现的Filter列表对话框中选择"Directshow Filters"分支，截图如下：

![](https://img-blog.csdn.net/20180224152306595)


但是很多用户一点开分支会发现GraphEdit就崩溃退出。作者对这个问题的回答是：这是GraphEdit的一个Bug引起的，如果安装了LAV Video Decoder插件就会有这个问题（只安装其他两个不会），建议大家不要用GraphEdit来添加和查看 LAV的Filter![尴尬](http://static.blog.csdn.net/xheditor/xheditor_emot/default/awkward.gif)。不过幸好还有其他替代的工具，大家可以试一下**GraphEditPlus**，这个工具很好用，能够正常查看“Directshow Filters”分支下的Filter，显示的Filter信息也比较丰富，下面是这个工具的界面截图：

![](https://img-blog.csdn.net/2018022415345964)





第三，是怎么配置LAV Filter的属性。其实LAV 每个Filter都有属性页，大家可以通过前面的GraphEditPlus工具查看到，下面是LAV Video Decoder的属性页：

![](https://img-blog.csdn.net/2018022415350911)


  另外我们还可以在代码中调用Filter提供的接口来设置Filter的参数属性，LAV提供了几个Filter的接口头文件，路径在\\developer_info子目录下面。

   如果读者在使用和开发过程中有遇到其他问题，欢迎留言交流。






