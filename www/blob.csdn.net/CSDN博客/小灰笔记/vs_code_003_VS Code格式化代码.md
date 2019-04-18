# vs_code_003_VS Code格式化代码 - 小灰笔记 - CSDN博客





2018年10月08日 22:01:42[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：587








         今天简单使用了一下最新的VS Code，感觉在体验上似乎又有新的提升。也许是之前没有找到相应的功能，不过这种简单程度让我感到有点吃惊。今天接触到的一个功能是代码的格式化，比如缩进已经对齐等功能。

         在此之前，我在emacs中掌握了这个功能。使用的体验非常好，这样我逐渐产生了对emacs的依赖。现在看来，在其他的编辑器中或许也有类似的功能可以发掘。

以下是我随便写的一个不规范的代码：

![](https://img-blog.csdn.net/20181008220107934?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         对齐的功能有两种：按照文件、安全选定的区域。这个跟我用的emacs的功能也是一致的。
- 选定的代码格式化

![](https://img-blog.csdn.net/2018100822010875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         操作很简单，选中之后鼠标右键选择格式化选定的代码。这个功能也提供了一个组合键来实现这个功能。
- 格式化整个文件

两种方式，第一种是选择全部的文件内容，这其实是我一直在emacs中的操作方式，已经形成了肌肉记忆。另一种方式，右键选择格式化整个文件，也有相应的组合键。


代码的处理效果：

![](https://img-blog.csdn.net/2018100822010893?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

         总体的功能还是不错的，但是也发现了一点点小问题。其实，这也可以说是VS Code做的好的一点吧！那就是，如果你的代码文件先前保持了一种格式化模式，那么VS Code会按照你原来的方式格式化代码。比如，你代码中大多数的代码都是按照2个空格来进行缩进，那么格式化的效果是全部按照2个空格的代码进行缩进。这种方式不错，但是有时候会在一个工程中出现几种不同的编码风格。相比之下，我现在倒是觉得emacs的处理方式要好一些。当然，这里说的是spacemacs吧！



