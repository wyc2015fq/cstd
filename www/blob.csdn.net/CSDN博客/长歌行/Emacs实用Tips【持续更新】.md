# Emacs实用Tips【持续更新】 - 长歌行 - CSDN博客





2017年12月24日 17:30:34[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：337








### Tips No.1:

不要纠结Vim好还是Emacs好。我认为Vim好！但是我的使用情景和你不同，不适用与你的情况啊。

我主要还是使用Vim的。当一个编辑器来使用Vim的。并不用来调试代码，打游戏。。。Vim有不少优点。 

- **形成机械记忆时间短**

虽然Vim的学习曲线一开始比较陡峭，但是学了一段时间，手指形成机械记忆之后，做各种编辑工作很方便。 

- **各种软件都有高质量的Vim插件**

我现在在VS，Sublime上都安装了Vim插件。写代码，编辑文字效率很高。 

- **精准编辑**

远程连其他电脑，写代码，通过对行号，字符数偏移的精确计算，也避免了很多鼠标交互操作，降低了网络传输要求。 

- **快捷键简单**

基本都是按数字，按字母。组合键很少。（当时使用青轴键盘来作为输入使用Emacs时的痛苦，我记忆犹新）（注：在Emacs中使用了Evil包之后，能兼容Vim的快捷键，所以这一条严格意义上不成立。但是对于新手来说，直接使用Vim仍然不失为一个比较好的选择，至少减少了环境配置的时间。）
对于一般情况来说，Vim是一个比Emacs更好的选择。（毕竟windows的Emacs用起来不方便，还要九十多兆）但是，对于程序员来说，必须要不断的尝试更多的可能啊。（我是四年前使用Vim一直到现在，之前尝试Emacs，但是一年之后就放弃了，主要还是我个人用起来效率低）不作的程序员不是好程序员。对了，我实际用Emacs不到半年，但是我觉得有必要写一些Tips，有能力通过以后不断的实践来完善这些Tips。用Vim时间虽然长，但是觉得没必要写任何Tips。直接去用就行了。

### Tips No.2:

下载完之后快速上手，不需要上网看各种教程，指南和Tips。基本上使用工具栏的各种指令和鼠标各种操作就能够正常的使用Emacs了。（不会出现不知道如何退出的情况）Emacs用它的首页来放如此详细的首页链接，Vim却用来帮助乌干达的儿童。

对于一般人来说，也不要太作了。不要一开始就想着用大师的配置来增强或者美化自己的emacs。工具是用来用的，而不是装饰品。我之前也用了几个月的时间，尝试了各种配置。但是发现，对于我的工作（C++， C#， Python的三维地图软件开发），为什么我不用VS2017（PyCharm）呢？

真要用配置，但是以前没接触过Emacs，可以在c盘根目录或者~根目录放个.emacs文件，写点配置，举个例子。

```
(global-set-key "\C-ca" 'org-agenda)
(set-default-font "-outline-Microsoft Yahei-normal-normal-normal-sans-16-*-*-*-p-*-iso8859-1")
(load-theme 'wombat t)
```

Emacs的warning会提示你下一步应该怎么做的。

我的建议是，对待任何事物，都不要想着速成，对于emacs，也不要抱着过于功利的想法。但是快速上手却很重要，之后的知识点可以在快速上手，判断emacs是否适合你之后，再进行进一步的学习，如果想进一步的简单的了解emacs的配置文件，可以看看我的这篇文章：[简易Emacs配置](http://blog.csdn.net/uninterrupted/article/details/79120402)。

### 试试org-mode:

不多说怎么用了，主要是用来做任务清单的。可以给任务设置状态，设置开始时间和截止时间。我列举一些快捷键，可以大致看看功能。

```
S+M+RET 创建新条目
C+c C+t 修改TODO的状态
M+左右 修改TODO层级
M+上下 移动列表
C+c C+c 增加tag
C+c C+c 修改checkbox
S+TAB 折叠展开
S+→推进状态
对于短时间的任务，可以计时
C+c C+x C+i 开始计时
C+c C+x C+o 停止计时
```

至于界面，是这样的： 
![这里写图片描述](https://img-blog.csdn.net/20171224172650823?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
写完之后，导出为各种html等格式的报告也很容易。可以用于汇报。（需要自己写格式过滤条件，样式等） 
![这里写图片描述](https://img-blog.csdn.net/20171224173000502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 未完待续






