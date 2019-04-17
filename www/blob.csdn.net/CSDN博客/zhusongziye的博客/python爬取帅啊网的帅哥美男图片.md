# python爬取帅啊网的帅哥美男图片 - zhusongziye的博客 - CSDN博客





2018年03月07日 22:21:49[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：648








 预备知识 
![](https://img-blog.csdn.net/20180307221334201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 爬取《帅啊》网的帅哥图片！
 URL : http://www.shuaia.net/index.html   先看一眼网站的样子：

![](https://img-blog.csdn.net/20180307221433992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

requests安装
在cmd中，使用如下指令安装第三方库requests：
pip3 install requests
或者：
easy_install requests

爬取单页目标链接
通过审查元素，我们不难发现，目标的地址存储在class属性为”item-img”的<a>标签的href属性中。这时候，有人可能会问为啥不用下面的<img>标签的src属性？因为这个图片是首页的浏览图片，根据这个地址保存下来的图片，太小了，并且不清清楚。秉承着热爱“高清无码”的精神，这种图片可不是我想要的。因此，先获取目标的地址，也就是我们点击图片之后，进入的网页地址，然后根据下一个网页，找到图片的地址。

![](https://img-blog.csdn.net/20180307221537136?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码 

![](https://img-blog.csdn.net/20180307221615185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们将爬取的信息保存到list中，图片名字和图片地址使用”=”连接，运行结果：
![](https://img-blog.csdn.net/20180307221637495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
爬取多页链接 
翻到第二页的时候，很容易就发现地址变了:www.shuaia.net/index_2.html。第三页、第四页、第五页依此类推。
代码+运行结果 
![](https://img-blog.csdn.net/201803072216579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 单张图片下载 
进入目标地址，审查元素。可以看到，图片地址保存在了class属性为
”wr-single-content-list “的div->div->img的src属性中。
![](https://img-blog.csdn.net/20180307221732769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 代码 
![](https://img-blog.csdn.net/20180307221758776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 我们将图片保存在程序文件所在目录的imgase目录下：
![](https://img-blog.csdn.net/20180307221814658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

整体代码 
已经获取到了每张图片的连接，我们就可以下载了。整合下代码，先少下载一点，下载前2页的图片。
![](https://img-blog.csdn.net/20180307221830238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 运行结果
![](https://img-blog.csdn.net/20180307222037837?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

作者：Jack-Cui
源自：http://blog.csdn.net/c406495762/article/details/72597755
声明：文章著作权归作者所有，如有侵权，请联系小编删除




