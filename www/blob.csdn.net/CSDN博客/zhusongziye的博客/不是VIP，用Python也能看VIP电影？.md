# 不是VIP，用Python也能看VIP电影？ - zhusongziye的博客 - CSDN博客





2018年03月22日 20:43:34[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3327








运行平台： Windows

Python版本： Python3.x

IDE： Sublime text3

![](https://img-blog.csdn.net/20180322204141937?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

PS：本软件只用来交流学习，请勿用于商业用途。如涉及版权侵权等问题，请联系我，我会删除文章。我可是守法的好公民….

选择一部自己喜欢的电影，浏览器地址栏的内容即为视频连接。

![](https://img-blog.csdn.net/20180322204157207?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 代码编写

![](https://img-blog.csdn.net/20180322204218409?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

继续分析我们会发现在发送POST请求之前，有一个GET请求，如果我们向这个Request URL的地址发送GET请求，你会惊奇的发现，返回的信息里包含我们需要的url信息。

![](https://img-blog.csdn.net/20180322204232414?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

它是这个样子的：

![](https://img-blog.csdn.net/20180322204248380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码如下：

![](https://img-blog.csdn.net/20180322204306638?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

运行结果如下图所示：

![](https://img-blog.csdn.net/2018032220431873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 总结

1、需要对传入的视频链接进行处理。找到规律，做相应替换即可。

2、单纯获取url不行了，需要获取time和key，然后发送post请求。如果顺利，服务器返回Success为1，直接登录，过几秒链接就会失效。

![](https://img-blog.csdn.net/20180322204109918?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




