# 解决GVIM中C-A组合键不起作用的问题 - 小灰笔记 - CSDN博客





2017年10月16日 20:14:26[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：160
个人分类：[VIM](https://blog.csdn.net/grey_csdn/article/category/6888413)









            VIM中有对数字操作的命令，分别是对数字增加或者减少。具体的命令如下：

                        增加：C-a

                        减少：C-x

            在GVIM中进行联系的时候，我发现减少是有作用的，但是C-a会被识别成全选操作。印象中，这应该是GVIM的新特性。很久之前看过一眼的一篇文章似乎提到了，说这种组合键以及鼠标操作算是一种Windows环境中的功能扩展。

            查看了一下GVIM中的几个配置文件，看到了这个组合键的定义。发现这个组合键的定义，屏蔽之后再次测试发现这个命令已经有效。

            屏蔽的脚本如下：

![](https://img-blog.csdn.net/20171016201416343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


            关于数字的操作，也有很多简单的技巧值得总结。比如说：增加或者减少任意数目的变化量，可以使用数字加组合键的方式等。这么看下来，VIM确实是一个很有学习点的编辑器。不知道花多少时间才能够让我对此使用的得心应手？这种状态只能是不断努力中等待了。



