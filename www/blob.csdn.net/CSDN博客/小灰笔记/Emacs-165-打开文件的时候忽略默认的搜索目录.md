# Emacs-165-打开文件的时候忽略默认的搜索目录 - 小灰笔记 - CSDN博客





2018年03月18日 11:18:04[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：62
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









            C-x C-f执行，去查找并打开文件的时候会有一个默认的目录。这个目录通常来说是当前buffer对应的文件所在的目录。比如：

![](https://img-blog.csdn.net/20180318111741814)

            当我正在用emacs打开我的emacs配置文件的时候，此时查找并打开文件提示的目录就是我配置文件所在的目录。如果想要忽略这个目录，直接输入其他的目录，那么只需要再加一个/即可。在Linux下面，可以从根目录或者home目录开始查找，Windows下直接直接切换到盘符。当然，如果设置了HOME环境变量的话也可以从home目录开始。

            以访问e盘目录下的一个文件为例：

![](https://img-blog.csdn.net/20180318111750135)

![](https://img-blog.csdn.net/20180318111757290)

            由于磁盘上没有这个文件，emacs自动创建了一个这样的文件。




