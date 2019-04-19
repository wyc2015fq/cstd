# VB.NET小总 - Big Smile - CSDN博客
2015年12月05日 10:48:42[王啸tr1912](https://me.csdn.net/tr1912)阅读数：570
        完成了设计模式的学习后，马上开始了VB.NET的学习，学习的时候没有什么太多的感受，知识觉得微软的官方讲师讲的太深入了，完全没有让我看下去的欲望，恰好又赶上了评教导数据的活，所以就草草的看完了，现在把自己所看到的和在其他地方学习到的知识分享一下。
# 一、界面的不同
        首先我们来看一下VB和VS的界面确实有很多的不同：
![](https://img-blog.csdn.net/20151213173249076?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
窗体代码之间的差别：
![](https://img-blog.csdn.net/20151213173602688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        很明显在vs中的VB.NET中有了class的字符标识，说明这个窗体是一个窗体类，从这里就看出来VB.NET 更加的面向对象，并且在结构上也有很多的差别，比如按钮的click事件下多了很多的参数等等。控件的摆放位置还是一样的，只不过这次的控件是通用控件，并且多了很多的没有见过的控件，让我们有了新鲜的感觉。另外在观察整个菜单栏下面的专用按钮少了很多。但是数据类型要求的更加严格了，大小写在某些方面也有要求，访问数据库的方法上也有些许差别，用到了ADO.NET。
# 二、相似的地方
        声明变量的语句是一样的，定义变量的方法也都是差不多的，循环，判断，数组，调用自定义函数和写自定义函数的方法都是差不多的。从控件的属性来看也是差不多的，只是vs多了一个属性的分类，没有了找属性的困难度（虽然我不习惯）但是他也可以改变属性的排序方法。
# 三、使用策略
        在vs中你会发现没有了格式这个菜单，控件的大小和对齐就成了问题，但是vs这么强大的一个IDE是不会这样的，调整格式的菜单需要单独的掉出来，具体如下：
![](https://img-blog.csdn.net/20151213175112617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击视图菜单->工具栏->布局，就会出现这样的工具栏：
![](https://img-blog.csdn.net/20151213175310321?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中左面的六个是各种对齐按钮，右面的四个是调整大小、宽度的按钮。这样就和vb中的差不多了。
        下一个是对属性和事件的修改预览问题，vs中把属性和事件都放在了一起，在右下角的窗格里面，可以分别查看：
属性分类![](https://img-blog.csdn.net/20151213180013829?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)事件分类：![](https://img-blog.csdn.net/20151213180104230?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
他们也可以按字母排序，方便查找：
![](https://img-blog.csdn.net/20151213180155420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 四、解决方案和项目
        在使用vs的时候肯定会遇到解决方案这个新的名次，那么这个解决方案和项目又有什么区别呢？
通俗的来说解决方案就相当于是一个整体的策划，包含了配置和目录等信息，就好比是VB6中的一个总工程文件，而项目就是解决方案的具体实行策略，项目是有模板的，而解决方案是没有模板的，项目就好像是vb6中的子工程。
![](https://img-blog.csdn.net/20151213183433215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)vb6![](https://img-blog.csdn.net/20151213183507582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)vs
以下是微软的官方解释：[点我](https://msdn.microsoft.com/zh-cn/library/b142f8e7.aspx)
# 总结：
        说实话，vb.net的视频的确是没有看进去多少，但是对vs和VB.NET程序的写法和使用却又了解了很多，希望以后的项目中能够熟练的应用这个语言。
