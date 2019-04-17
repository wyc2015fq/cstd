# Emacs_231_org_mode设计单元格 - 小灰笔记 - CSDN博客





2018年07月24日 21:01:38[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：47
个人分类：[Emacs](https://blog.csdn.net/grey_csdn/article/category/7131378)









org-mode可以十分方便地设计表格，具体的效果如下：

![](https://img-blog.csdn.net/20180724210123845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



使用||可以设置单元格，光标在两个竖线之间按TAB键可以实现跳转到下一个单元格或者创建新的一等功能。跳转的时候同时会实现一个宽度的自动调节功能。

       如果想要给表格的行之间加上一条分界线，可以在竖线的后面紧跟着输入一个-，然后按TAB按键即可出现一条分界线。

       第一行尖括号是对齐方式，三个分别代表左中右对齐。此外，还可以指定表格的宽度，使用尖括号中加一个数字n实现。用于说明相应列的宽度是n个字符宽度。如果内容超出了宽度，可以自动折叠。查看折叠的内容只需要把鼠标移动到折叠位置即可。

       导出的网页版本的效果：

![](https://img-blog.csdn.net/20180724210123970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       不是很美观，但是已经有了很多我们需要的功能。后续继续进行功能扩充吧！



