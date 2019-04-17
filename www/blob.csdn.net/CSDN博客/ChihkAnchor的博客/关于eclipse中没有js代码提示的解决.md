# 关于eclipse中没有js代码提示的解决 - ChihkAnchor的博客 - CSDN博客





2018年12月25日 16:36:26[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：56








自学js，发现eclipse中不管js文件、html文件、jsp文件没有都没js代码的提示，对于js代码也不报错，有时候就因为单词敲错却查了很久没查出来，很烦很难受。

在网上找了很多方法，都没有解决，特别是有个在javascript中editor中content assit里改一个地方为zjs的方法，试了几次也没成功，不知道各位老铁有没有成功？

再后来，我下了个插件，问题就解决了。

打开eclipse点击window旁边的Help选项--->

Eclipse Marketplace--->

在find中查找AngularJS Eclipse--->

![](https://img-blog.csdn.net/20160813171517177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

--->点击install（我下的是1.2.0）

---->安装好以后，重启eclipse

---->选择你的项目，右键你的项目选择Configure

---->convert to AngularJS project

---->在弹出的Modules中选择根据自己的需要选择（我选的是AngularJS），点OK

![](https://img-blog.csdn.net/20170121181313944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTk9LSUFfTEM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后，万事大吉了

![](https://img-blog.csdn.net/20160813171925651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

安装插件后

![](https://img-blog.csdn.net/20160813172307499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

--------------------- 

作者：Lewy95 

来源：CSDN 

原文：https://blog.csdn.net/LewyPhoenix/article/details/78483344 

版权声明：本文为博主原创文章，转载请附上博文链接！



