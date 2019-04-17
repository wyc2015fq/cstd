# Python爬虫快手视频 - zhusongziye的博客 - CSDN博客





2018年03月04日 09:26:02[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3860








准备

 环境： python 2.7 + win10
工具：fiddler postman 安卓模拟器
首先，打开fiddler，fiddler作为httptps 抓包神器，这里就不多介绍。

配置允许https 




![](https://img-blog.csdn.net/20180304092114580?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

配置允许远程连接 也就是打开http代理

![](https://img-blog.csdn.net/20180304092152210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



电脑ip: 192.168.1.110

然后 确保手机和电脑是在一个局域网下，可以通信。由于我这边没有安卓手机，就用了安卓模拟器代替，效果一样的。

打开手机浏览器，输入192.168.1.110:8888   也就是设置的代理地址，安装证书之后才能抓包

![](https://img-blog.csdn.net/20180304092219352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装证书之后，在WiFi设置 修改网络 手动指定http代理

![](https://img-blog.csdn.net/20180304092254353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


保存后就可以了，fiddler就可以抓到app的数据了，打开快手 刷新，可以 看到有很多http请求进来，一般接口地址之类的很明显的，可以看到 是json类型的

![](https://img-blog.csdn.net/20180304092335775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 http post请求，返回数据是json ，展开后发现一共是20条视频信息，先确保是否正确，找一个视频链接看下。



![](https://img-blog.csdn.net/20180304092358481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

ok 是可以播放的 很干净也没有水印。

 那就打开postman 来测试，form-data 方式提交则报错

![](https://img-blog.csdn.net/20180304092431832?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


那换raw 这种

![](https://img-blog.csdn.net/20180304092451520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

报错信息不一样了，试试加上headers 

![](https://img-blog.csdn.net/20180304092527738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

nice   成功返回数据，我又多试几次，发现每次返回结果不一样，都是20个视频，刚才其中post参数中 有个page=1 这样一直都是第一页 就像一直在手机上不往下翻了 就开始一直刷新那样，反正 也无所谓，只要返回数据 不重复就好。
代码

![](https://img-blog.csdn.net/20180304091834712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



