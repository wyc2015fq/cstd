# 浅谈学习JS和JQuery中的几点收获 - Big Smile - CSDN博客
2016年06月19日 21:18:04[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2940
        刚学习完了JQurey的视频，觉得这段时间下来，收获甚是少，想总结一下，由此引来了这篇博客。
# 一、学习中的那点事
        估计每个人都会遇到不想学习的时候，我看完了JS，看css+div的时候真的是不想去看了，觉得学习到的东西是重复的，所以看着看着就睡着了，估计也会有人和我是一样的感受，JS里面涉及到的东西是在是太多了让人无法一下子就消化透彻，所以对于后面的学习是很有影响的。
        于是这个时候看完了CSS+Div的视频(视频就跟念演讲稿是的，非常有意思但是学不到什么东西)，书有480+的页数，怎么也不想看了，所以就想自己实践一把CSS和JS的理论，看看自己掌握的怎么样。于是乎就有了自己虐待自己的经历。
         第一步，我建立了一个HTML的文件，又添加了一个css的文件，把框架什么的html代码都敲好了，然后就开始敲一个一个节点和控件，突然发现img这个控件不会用啊，于是就各种翻例子，翻到了牛腩的例子之后看了一会才知道对关键的设置图片路径的属性项原来是src，用了src，加载不出来图片怎么办呢，又开始查，结果是alt这个属性定义的是没有图片的默认显示，加载好了图片之后又犯难了，如何才能居中呢？又开始查，结果就是align这个的属性设置为center就好了。这样的例子还有很多，下面给大家罗列一下：
        加载css文件：
        <link rel="stylesheet" type="text/css" href="css/tag.css" />    一定要注意是link
        加载js文件：
        <script type="text/javascript" src="boke.js"></script>
       开始的时候模仿css的写：<script type="text/javascript" src="boke.js" /> 没有成功，这里要注意。
       连接的鼠标手势是；cursor：pointer   而不是网上说的：cursor：hand
       我用的第三方鼠标，需要鼠标的图形文件cur：
       cursor:url(control/CursorLinkSelect.cur),pointer;          （CursorLinkSelect.cur是从网上下的鼠标文件）
        从敲完了这个例子了之后，感觉学习css+div的劲头也就足了，因为里面有很多的共鸣，有些东西一看就知道我用过，我的这个项目是一个自己做的非常基本的项目基本没有什么技术可言，但是用的基础很多，所以一下子我就看得进去CSS+DIV的书了，所以我感觉自己敲一敲这些学过的东西还是很有用的。
# 二、JQuery的学习
        JQurey的学习确实是JS学习中的很重要的一部分，它主要学习的就是如何利用好Jqurey的JS库，这个库和JS中的老师写的那个库是差不多的，所以学习起来应该很好学的。但是其中最主要的还是思想和API文档（手册）的使用，这个JQuery的学习，手册是非常重要的，其中的功能都有讲解和收录，查询也很方便。
         但是你们下载下来的手册有可能打开之后就是一片白，什么都看不到，这个时候怎么办呢？其实就是系统的安全策略导致的，只需要在属性里面点击接触锁定就可以了。
![](https://img-blog.csdn.net/20160619211159938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20160619211359101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20160619211537180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这也是上网找才发现的，这样API文档就可以看了。JQurey主要也是靠实战，同学们也可以试着用它来实现一个小例子。
