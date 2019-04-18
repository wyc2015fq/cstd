# Office 2016 Excel实现下拉栏 - 小灰笔记 - CSDN博客





2017年08月13日 14:52:21[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：997








       我对Office并不精通，最多就是拿来码码字，Excel以及PPT用的很少。在这几个工具中，我觉得最有技术含量的就是Excel。特别是在数据处理、各种总结、规划等等方面有着很广泛的使用。

       之前有个小功能我还是掌握了的，那就是实现下拉栏的功能。掌握这个功能的时候我使用的Office版本还是2007。这个功能用了多年，一直用着不错。今天用了一下2016版本的Excel，发现这个功能找不到了。以前的这个功能在数据有效性中实现，现在的版本中却找不到这个功能了。

       摸索半天，总算又找到了这个功能。其实，这个功能的实现跟旧版本差不多，只是换了一个名字。接下来通过截图方式实现一下这个功能：

![](https://img-blog.csdn.net/20170813145134485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       基于上面的信息，实现一下星期几的下拉栏填入。实现方式跟之前类似，首先选中即将实现下拉栏的列，然后切换到数据分栏选择数据验证。

![](https://img-blog.csdn.net/20170813145145346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       如上图，在弹出窗口的允许中选择序列，接着通过来源选择待选数据。这样，下拉栏的功能就已经实现：

![](https://img-blog.csdn.net/20170813145154945?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






