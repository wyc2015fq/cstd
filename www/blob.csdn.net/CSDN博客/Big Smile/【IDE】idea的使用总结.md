# 【IDE】idea的使用总结 - Big Smile - CSDN博客
2017年07月30日 20:22:25[王啸tr1912](https://me.csdn.net/tr1912)阅读数：502
        小编最近使用的IDE是Idea，有些使用的新的，和大家分享一下。
**一、Idea的基本配置**
        对于idea来说，他和别的IDE最大的不同（我认为）其实就是配置和服务器的设置有些不同，这个IDE的好处和有点我就不说了，直接来看一下他的整体和一些配置。
![](https://img-blog.csdn.net/20170730192127637?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        上图为整个Idea的整体图，我们可以看出来，和eclipse还有和netBeans都差不多，都是有一个代码区，一个项目目录区，下面是日志，错误，以及各种信息的区域，右面是一些辅助面板，上面是一些常用的配置菜单和功能按钮，整体来说还是比较熟悉的。
        首先来说他的整个配置，也就是settings，是分两种的，一种是全局的，也就是这个IDE的，还有一个种是针对于正在打开的项目的，我们点击file->Settings就可以看到最常见的一些设置，例如maven的设置，编辑器的设置，还有插件的设置等等都在里面：
![](https://img-blog.csdn.net/20170730194741339?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170730194747802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        注意这个里面我们进入的是项目的配置，如果你当前打开项目，那好，你配置完成之后，整个项目的设置是可以生效的，但是如果一旦你换了项目，那么你的设置又要从新在来了，包括服务器，maven，字体等等。
      但是，这个IDE还有一个默认设置这个选项，可以让你直接设置整个IDE的通用设置，在如下的这个地方，设置页面和项目的设置类似。
![](https://img-blog.csdn.net/20170730201708102?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20170730201728119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        在这个里面设置了之后，我们就可以在打开任何项目的时候都用着一套的配置内容了，不用再次进行配置了，减少麻烦。
# 二、代码的规范使用
        在敲.net的代码在Vs2012这个IDE上面的时候，我们通常都会把写好的方法，用#region  和  #endregion加上注释，然后折叠起来，这样看着就方便多了，然而到了java的IDE上，还有项目中，我并没有看见谁的代码里面用过这个，于是就觉得奇怪，所以去网上查询了一下，发现这个代码的折叠是与IDE 有关的，不同的IDE折叠的方法都是不同的，对于eclipse需要安装插件，具体安装哪个插件，和怎么使用，这里就不多说了，下面说一下在netBeans里面和idea里面是如何折叠的，因为这两个IDE自带这样的解析。
        首先是NetBeans中：
```java
// <editor-fold desc="Description">    
Your code goes here...    
// </editor-fold>
```
然后是一个在ideal中仿照VS中的方式：
```java
//region Description    
Your code goes here...    
//endregion
```
        需要注意的是，这两种在Idea中是都可以用的，因为Ideal同时支持解析这两种东西。
下面是折叠之后的效果：
![](https://img-blog.csdn.net/20170730201645124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 三、总结
        在开始用Idea之前，一直以为这是一个很不好学习的IDE，但是用过之后才发现，他比较eclipse来说，要好用很多，并且插件库也非常的丰富，他的一些操作还有一些习惯和主流的IDE 都保持一样，所以是一个很好的IDE，我们在接触这样的新东西是往往要借助于旧的事物去认识他们。
