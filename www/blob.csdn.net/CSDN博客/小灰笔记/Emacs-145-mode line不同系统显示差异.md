# Emacs-145-mode line不同系统显示差异 - 小灰笔记 - CSDN博客





2018年03月14日 23:50:14[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：71








![](https://img-blog.csdn.net/20180314234940612)

            上面的modeline是从Windows版本的emaca中截取的。可以看到左边的c\---的符号。

![](https://img-blog.csdn.net/20180314234946770)

            而上面的界面则截取自Windows10的Linux子系统。

            鉴于比较麻烦，暂时先不看Mac上的效果了。

            两个显示差异，一个是---另一个则是UUU或者百分号等。其实主要的差异是因为系统定义的换行符不同导致。而我也在Windows发现能够显示Unix的字样，一直弄不明白。后来才想到或许是因为文件在不同的系统上编辑过。而现在的编辑器足够智能，能够自动分辨并调整格式显示。

![](https://img-blog.csdn.net/2018031423500715)

            比如，上面的文件其实是我在Linux上面编辑过的，只是在Windows下面的Emacs中打开。但是，modeline的最左边显示的字符是Unix。

            这样，也就是说这个显示的信息其实是与操作系统关系不大，而是与编码方式或者格式有很大的关系。




