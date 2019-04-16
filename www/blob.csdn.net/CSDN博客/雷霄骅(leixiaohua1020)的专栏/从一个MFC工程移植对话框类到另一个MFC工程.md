# 从一个MFC工程移植对话框类到另一个MFC工程 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月22日 01:17:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：29
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)












今天碰到模块移植问题了,自己做的对话框类(继承于CDialog)要移植到目标工程中.在网上搜了几个帖子发现都说的很生涩.问了我们的技术总监之后总算解决这个问题了.

问题:把一个MFC SDI工程中的一个对话框资源和类拷贝到另一个MFC SDI中.

1.在VC6中打开老的工程,在工作区Resource标签页中选中Dialog文件夹下的资源文件(就是双击后会出现编辑窗口界面的那个东西),按Ctrl+C(Edit->Copy也一样).

2.关闭当前工程,注意不要关闭VC6,打开目标工程(File->OpenWorkspace),然后选中工作区的Resource标签页中的Dialog文件夹,然后按Ctrl+V(Edit->Paste也一样).好了,对话框拷贝过来了.

3.将对话框类的cpp h文件拷贝到目标工程中(不详细说了,往当前工程添加文件)

4.打开复制过来的类cpp和h文件,删除没用的头文件(老工程相关的),并且在.h文件中添加文件包含

- #include"Resource.h"


第四步很关键,老工程中根本就没有这个文件包含,可是移植之后必须手动添加头文件包含.因为这个问题卡了10几分钟...



然后在新工程中就可以使用这个类了,主要就是这个类的对话框中的资源复制头一次遇到.不敢独享,拿出来跟新手们一起学习.



原文地址：[http://blog.csdn.net/wowoandlyn/article/details/2937785](http://blog.csdn.net/wowoandlyn/article/details/2937785)




