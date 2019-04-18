# Emacs-066-dired mode中返回上一层 - 小灰笔记 - CSDN博客





2018年01月20日 13:13:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：236








Emacs-066-dired mode中返回上一层

            这里的返回上一层不仅仅是在目录上的返回，同时还有从打开文件退回到上个界面的意思。最初我自己期待的一个功能仅仅是返回上一层目录，后来发现还附赠了一个额外的小糖果，VeryNice！

            我的Emacs配置的组合键是C-xC-j，接下来演示下效果：

![](https://img-blog.csdn.net/20180120131142288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


            如果打开了一个文件，C-xC-j，进入到diredmode，进入的当前目录是编辑文件所在的目录。进入C-xC-j，可以切换到上一层目录：

![](https://img-blog.csdn.net/20180120131201173?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


            如果此时打开一个新的文件，再次C-xC-j操作就又回到了前面的第一个操作了。这算是一个小糖果，可以利用这个功能特性打开一个文件然后在diredmode中直接查看管理当前的目录。



