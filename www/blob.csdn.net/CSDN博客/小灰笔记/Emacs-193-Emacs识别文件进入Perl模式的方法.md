# Emacs-193-Emacs识别文件进入Perl模式的方法 - 小灰笔记 - CSDN博客





2018年03月24日 21:18:18[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：89
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









       一般来说，进入一个major-mode的方式基本上是通过识别文件的扩展名即可实现。但是也不绝对。比如，在Perl中的识别方式不仅如此。

       先看看通过文件扩展名识别的情况。直接创建一个新的pl文件，具体如下：

![](https://img-blog.csdn.net/20180324211753242)

       创建了一个以pl为扩展名的新文件，即使没有任何内容也直接进入到了Perl mode。

       再看一下Emacs通过识别文件信息实现的major mode的识别。创建另一个新的不带扩展名的文件如下：

![](https://img-blog.csdn.net/20180324211800932)

       起初并不是Perl mode，接下来在文本中加入如下信息，保存重新打开：

![](https://img-blog.csdn.net/20180324211808939)

       从上面的截图可以看得出，Emacs根据内容判断出了相应的major mode。






