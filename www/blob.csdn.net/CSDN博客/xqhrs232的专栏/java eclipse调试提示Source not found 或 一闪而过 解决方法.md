# java eclipse调试提示Source not found 或 一闪而过 解决方法 - xqhrs232的专栏 - CSDN博客
2018年11月28日 15:36:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：51
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/kun_zai/article/details/78873241](https://blog.csdn.net/kun_zai/article/details/78873241)
相关文章
1、在Eclipse中关联源代码----[https://blog.csdn.net/longyuhome/article/details/8497505](https://blog.csdn.net/longyuhome/article/details/8497505)
菜鸟今天按照书上的代码在eclipse上敲了一个程序，运行没有问题。然后想调试一下看看运行过程，接着设置一个断点，然后就是F5了，发现每次运行到一行代码的时候就提示Source not found下面还有一个按钮“Edit Source Lookup Path”。现象就是下面这样的
![](https://img-blog.csdn.net/20140610211216171)
当然这个时候如果我继续按F5的话呢，程序又会接着正常运行了。到这里那就是说程序本身是没有问题，肯定是哪个地方没弄好的原因，然后菜鸟就开始到网上去google了，找了一些资料，最后终于知道原因了，原来就是我用的那行代码是系统库里的，但是呢每次运行到那一行的时候总是找不到源代码，所以就提示Source not found。
既然知道问题了，那就好办了。没有源码那就关联源码呗，关联源码之后问题完美解决。以后每次调试到这一行的时候就会跳到这个源码中，继续程序的调试过程。
**关联源码程序的几种方式：**
**1.直接点击下面的小按钮”Edit Source Lookup Path“，得到如下的界面**
![](https://img-blog.csdn.net/20140610212618265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140610212625671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140610214343312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140610214424671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
添加完了之后，点击OK按钮，以后调试就不会弹出那些烦人的Source not found了。
**2.第二种方法就是看看你的函数在哪个jar文件里，然后向相应的jar文件关联源码。**
首先，按住Ctrl键用鼠标点击哪个类，那么在左边的Package Explorer中看到你的类，当然那个双向箭头要按下去。
![](https://img-blog.csdn.net/20140610213214593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里我要找的是Scanner类，那么我在左侧就找到了这个类在rt.jar这个文件中，如下所示
![](https://img-blog.csdn.net/20140610213355593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
那么我们就可以对这个jar文件进行关联源码了，
开始我们如下图所示，右击项目，
![](https://img-blog.csdn.net/20140610213749656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140610213842093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击Edit按钮，然后弹出的界面设置如下所示。
![](https://img-blog.csdn.net/20140610213903390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
添加完了以后的效果图如下所示：
![](https://img-blog.csdn.net/20140610213917546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmlzaGVyd2Fu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
到这里我们就结束了，OK了。
参考资料：[http://blog.csdn.net/longyuhome/article/details/8497505](http://blog.csdn.net/longyuhome/article/details/8497505)
