# 使用meshlabserver批量处理三维模型(附代码) - 快来学习鸭～～～ - CSDN博客





2016年12月25日 13:18:54[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：3623








[http://blog.csdn.net/sxf1061926959](http://blog.csdn.net/sxf1061926959)
[欢迎访问我的博客](http:%2a%2a重点内容%2a%2a//blog.csdn.net/sxf1061926959)

Meshlab的.mlx脚本，可以保存对原数据的操作，然后通过meshlabserver进行批量处理。 

本人最近要对三维模型进行批量降采样，如果只是一个三维模型的话，我们可以使用meshlab图形界面中的filters进行操作。但是如果想要批量处理大量的三维模型，用图形界面去操作就太慢了。那么我们就得用到meshlabserver了。这个是meshlab的一个小工具。安装meshlab的时候已经一起安装好了。它和meshlab.exe在同一个文件夹下。 
![这里写图片描述](https://img-blog.csdn.net/20161225125733033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***怎么用呢？***

Meshlab是可以创建.mlx文件形式的XML脚本的，这些XML过滤器脚本能通过Meshlab图形界面或者使用meshlabserver使用命令行来执行。
## **1、怎么创建Meshlab MLX 过滤器脚本**

将你的模型用meshlab打开， 
![这里写图片描述](https://img-blog.csdn.net/20161225125820034?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后菜单栏有个Filters，你会发现下面有好多操作。 
![这里写图片描述](https://img-blog.csdn.net/20161225125854798?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果你现在的模型是有处理脚本的，那么点击show current filter script是会显示出来的，没有的话，那就是空的。 
***下面我举个简单的栗子***
## 减面操作（我做的就是这个的批处理）

![这里写图片描述](https://img-blog.csdn.net/20161225125933286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如上图所示，在Filters下找到Remeshing,Simplification and Reconstruction选项,再进一步找到它下面的Quadric Edge Collapse Decimation。然后会出来一个下图所示框框。 
![这里写图片描述](https://img-blog.csdn.net/20161225130020892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

里面有好多参数，想怎么改怎么改，改完点Apply确认一下，Meshlab就自动帮你处理了。 

下面是我处理的前后两张图，把面降到了2500. 
![这里写图片描述](https://img-blog.csdn.net/20161225130219814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 保存MLX脚本

如果你这些参数是要记住的或者进一步用来批处理，那么就要保存啦。 

点击FIlters菜单，选择Show current filter script。打开后会显示刚才的脚本。 
![这里写图片描述](https://img-blog.csdn.net/20161225130305368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

直接点击save Script就OK了。 
![这里写图片描述](https://img-blog.csdn.net/20161225130327190?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上图这个界面同样可以加载外部写好的脚本，点击那个Open Script，然后再点Apply Script就可以了处理模型了。
## 批处理脚本

保存好的脚本是.mlx文件，里面是XML形式的内容，记录了处理了哪些操作。 
![这里写图片描述](https://img-blog.csdn.net/20161225130458964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

进行批处理就要使用meshlabserver了。其实使用meshlabserver很简单，直接在命令行下就能用，那就直接可以用批命令处理了。 

如图是linux下写的shell进行批处理的代码： 
![这里写图片描述](https://img-blog.csdn.net/20161225130544941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hmMTA2MTkyNjk1OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我从网上搜集并修改了两份代码，**一份是windows下的，一份是linux下的**。都是批处理文件，直接用记事本打开就根据自己的要求修改参数，针对不同的处理问题只要调用不同的.mlx文件即可。代码可以到我的[github](https://github.com/Shicoder/Meshlab-MLXScriptBatchProcessing)上下载，里面有详细的说明。 
[**github下载地址链接**](https://github.com/Shicoder/Meshlab-MLXScriptBatchProcessing)
[https://github.com/Shicoder/Meshlab-MLXScriptBatchProcessing](https://github.com/Shicoder/Meshlab-MLXScriptBatchProcessing)
## 参考资料：

[http://blog.csdn.net/linear_luo/article/details/52396439](http://blog.csdn.net/linear_luo/article/details/52396439)
[http://www.andrewhazelden.com/blog/2012/04/automate-your-meshlab-workflow-with-mlx-filter-scripts/](http://www.andrewhazelden.com/blog/2012/04/automate-your-meshlab-workflow-with-mlx-filter-scripts/)
[https://github.com/tum-vision/csd_lmnn/](https://github.com/tum-vision/csd_lmnn/)








