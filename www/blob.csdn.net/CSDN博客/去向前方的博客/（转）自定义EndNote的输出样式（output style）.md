# （转）自定义EndNote的输出样式（output style） - 去向前方的博客 - CSDN博客





2018年10月28日 20:33:31[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：402








EndNote是一种常用文献管理软件，由于其对Word、Page以及中文的良好支持，已经成为论文写作的必备工具之一。

这几天帮人修改毕业论文的格式，发现EndNote自带的输出样式都不能满足要求，于是上网查了一些资料，整理了一份EndNote output style的自定义指南，和大家共享一下。

1、打开EndNote软件，笔者这里的是EndNote X4（Windows版）和EndNote X5（Mac版），其实两者没有本质的区别。

2、点击Edit->Output Styles，会弹出如下列表。如果要直接编辑某些已经存在的Output Style，可点击Open Style Manager。然后选择要编辑的条目进行编辑。否则的话选择New Style

3 点击New Style后，会弹出如下内容。

其中，左侧的就是需要编辑的各种条目，右侧的为具体的编辑内容。

4 编辑EndNote条目

左侧列表中，最为重要的是Citation和Bibliography两项，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028203243368.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_27,color_FFFFFF,t_70)

Citation：表示的正文中的引注的样式

Bibliography：表示的是文后参考文献的样式
下面主要说明Temlates的一些内容，因为这是修改的重点：

1、Templates：表示的是引用或者参考文献的样式模板。这是整个修改最重要的，基本上所有的修改都在此处。

在Citataion中，我们一般会使用"[1]"这种样式的引用，因此，可以点击右侧的“Insert Field”按钮，从中选择Bibliography Number，并在其左右两侧加上方括号，这样在文档中插入文献的时候，正文中就会显示[1]了。但是如何变成上标模式呢？别着急，全选需要变成上标的部分，然后点击上方的样式按钮（Windows版本）或者Edit->Style->Superscript，而在Mac中则要点击Edit->Font->Superscript。

如果要显示别的样式，也很容易，在Insert Field中选择不同的域的组合即可，应该没有什么难以理解的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028203255775.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_27,color_FFFFFF,t_70)
在Bibliography中，要针对不同的文献类型（Book、Conference Proceedings、Journey Article等）设置不同类型的输出样式。插入的方式也和上面很相像。首先，通过左上角的Reference Types选择一个样式，如下图所示，在本文中选择的是Book类型

然后通过右边的“Insert Field”按钮来插入不同的域，并以不同的顺序显示，如果需要加入其他的同一的文字，一般直接输入即可。此外，还有一个值得注意的小问题就是，如果某个关键字缺失肿么办？其实EndNote也为我们做好了相应的工作，那就是“|”（也可以使用Insert Field->Forced Separated），有了这个就可以很好的解决信息不全的问题了。

下面举一个小例子:

Author. Title [M]. City|:Publisher|,Year|:Pages|.

那么生成的参考文献的格式就会是这样滴：

包尚联. 脑功能成像物理学[M].郑州：郑州大学出版社,2006.

不知道大家注意到没有，在上条参考文献之前是没有[1]这个符号的，如果要添加这个符号，就要点击Bibliography的Layout项目，在右侧上半部分，即“Start each reference with”中，向前面一样，添加[Bibliography Number]

这样，一个最基本的output style就做好了还是比较简单的吧？呵呵~

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181028203308468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_27,color_FFFFFF,t_70)

转载此文，请务必标出本文地址：[http://www.cnblogs.com/supakito/archive/2012/09/23/EndNote_OutputStyles.html](http://www.cnblogs.com/supakito/archive/2012/09/23/EndNote_OutputStyles.html)





