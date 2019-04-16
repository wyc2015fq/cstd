# Excel ActiveX教程（一） - lyx的专栏 - CSDN博客





2016年05月20日 17:38:23[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：4596








        VBA真的是一个很强大的东西，当人家手工黏贴的时候，你只要编好代码，然后一劳永逸。不过最近发现ActiveX用起来好像更加方便的样子，特别是对于要做简单界面，人性化的应用场景。

       excel中什么是ActiveX？不用思考那么多，反正就是一种集成在Excel中的控件，也就是说，你可以在excel表格旁边加一个按钮，别人打开excel表格之后，填几个数字，点一下按钮就可以实现某些功能，这就是本质。而单纯的VBA其实还是适合后台，当然啦，你也可以做界面，但是，那么low的界面也好意思分离出来么，而且没有activeX方便。

       好了，不说了，上基础教程（一）

1.打开你的excel，点开发工具选项卡，如果没有的话 自行百度吧。如果有过VB界面或者Qt界面开发经验的，应该很熟悉插入控件这样的事情。


![](https://img-blog.csdn.net/20160520172909453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

点击插入之后，有下箭头，然后就可以选择了，建议大家选下面两行里面的，应为这是activeX支持控件，而上面两行则是以前的。稍后的用法也有点点不一样哦。

        我在我的表格中插入了下面这些控件。大家可以一个个自己插入一下看看，熟悉熟悉，具体的是什么功能，想必每一个控件的功能只要看到你就会知道了。

        教程里我们只说最常用的。一般为了方便查询最常用的控件当然是下拉列表，叫做Comobox。还有就是button、textbox、lable。

2.大家插入之后是不是发现有种无力选中的感觉呢。点击插入旁边的 “设计模式”， 然后“设计模式”图标就会被按下去的感觉，说明在这样的情形下，你是可以编辑控件的，而要退出，就再点击一下 设计模式 然后就是普通的模式了。


![](https://img-blog.csdn.net/20160520173234564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




