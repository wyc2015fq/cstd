# Emacs-153-隐藏启动界面 - 小灰笔记 - CSDN博客





2018年03月16日 23:42:10[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：137








![](https://img-blog.csdn.net/20180316234137250)

            上面文档中描述了隐藏启动界面的相关内容。

            如果对Emacs稍微熟悉一点之后，启动界面的帮助等信息不需要每次都看，那时候可以隐藏这个启动界面。方法就是设置变量inhibit-startup-screen为t。

            之后的效果：

![](https://img-blog.csdn.net/20180316234153940)


            这是启动时候的效果。如果启动的时候还把需要编辑的文件的名称作为参数传递给了emacs，emacs将会只显示被编辑的文件信息。

            效果：

![](https://img-blog.csdn.net/20180316234203961)




