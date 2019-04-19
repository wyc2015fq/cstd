# EA使用初步 - Big Smile - CSDN博客
2016年03月04日 20:47:13[王啸tr1912](https://me.csdn.net/tr1912)阅读数：752
众所周知的是，EA是一个强大的软件，通过它我们可以做很多的事情，在软件工程当中它可是帮了我们不少的忙，下面我们就来认识一下它：
# 一、简介：
        EA（Enterprise Architect）——生命周期软件设计方案，是以目标为导向的软件系统。它覆盖了系统开发的整个周期，除了开发类模型之外，还包括事务进程分析，使用案例需求，动态模型，组件和布局，系统管理，非功能需求，用户界面设计，测试和维护等。它内置了UML建模工具，可以让我们很方便的使用。目前，该软件已经出到了12.0 这个版本，需要的同学可以下载：[http://download.csdn.net/download/xlicwb/7457227](http://download.csdn.net/download/xlicwb/7457227)
# 二、基本设置
        首先来看EA的基本设置，在工具->选项中可以找到最基本的设置，包括设置作者，默认语言等等：
![](https://img-blog.csdn.net/20160304194406361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160304194443649?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
基本上所有的设置都可以在这里找到，还记得最开始的时候，画图的时候还是在一个一个的选择使用语言的，现在这么一弄，就可以不用管它了，字体颜色了等都可以随意变换，这里还是很人性化的。
# 三、图的结构
 用EA画图的时候，要先理清其中的层次关系，模型节点，包，图，元素都是构成这个图(模型)文件的关键，详细的层次名称如下：
![](https://img-blog.csdn.net/20160304195944452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        其中最基本的就是图和元素了，他们是组成图的根本，是模型的实际表现，然后就是包了，其中分为图包和视图集，他们下面如果只有元素而没有图的话是没有办法显示的，而包就是要把元素分类用的，例如七层中的七个层是一样的，视图集就是把每一层的图给集合起来，形成了类图的集合，用例图也是同理。模型集就好像是一个大的项目，这个项目可能有很多的部分，每个部分有很多的零件，分别对应视图集和图包。
# 四、例子
        拿用例图举例，uml中的图中其中的一个叫用例图，所以我就给他分了一个视图集（当然整个模型叫uml图），然后用例图分为用例和参与者，这里又分为了两个包，一个叫参与者，一个叫用例。
![](https://img-blog.csdn.net/20160304203707383?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后就是参与者图，其中每个参与者都是这个图的元素：
![](https://img-blog.csdn.net/20160304204047121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20160304204133685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用例图：
![](https://img-blog.csdn.net/20160304204148419?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意看上图中的参与者写的是（fromActor），这是从参与者的元素中拖过来的，看上图的列表，用例图中并没有参与者这个元素。在EA画的图中还有很多这样的地方，如时序图等。
# 五、总结
        EA中的知识还有很多，他可以画的图也有很多，需要我们不断的总结。
