# eclipse C开发添加自己的头文件搜索路径 - DoubleLi - 博客园






eclipse编译C程序时提示：

..\src\main.c:8:21: fatal error: my_type.h: No such file or directory

如图：

![](http://img.blog.csdn.net/20140922143433493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2hiX2ZlaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](http://blog.csdn.net/whb_fei/article/details/39475763)

需要添加自己的头文件的路径：



选中工程再右键点击，选择右键菜单的Propertise：

C/C++ Build -> Settings -> Tool Settings -> Includes 点击添加的符号，会出现选择头文件的对话框，此时可以选择头文件或头文件所在目录，我选择的是

当前工程下的inc目录，出现下图所示：

![](http://img.blog.csdn.net/20140922143308187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2hiX2ZlaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](http://blog.csdn.net/whb_fei/article/details/39475763)

在这一步可以点击确定，这个工程就可以找到当前工程目录下的头文件及当前工程目录下inc目录下的头文件。也可也复制路径：

“${workspace_loc:/${ProjName}/inc}”

添加到上图C/C++ Build -> Environment选项卡中C_INCLUDE_PATH的值得最后，这样以后新建C工程时也可以引用当前工程目录下的inc目录下的头文件，

需要注意的是新建的工程中也要用inc做头文件的目录，如果不是，则需要按以上步奏重新添加。









