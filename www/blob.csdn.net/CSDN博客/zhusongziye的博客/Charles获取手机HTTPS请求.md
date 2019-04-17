# Charles获取手机HTTPS请求 - zhusongziye的博客 - CSDN博客





2018年09月01日 12:41:44[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：498








 在接口测试中，特别是对移动产品的测试，经常要争取APP端的请求来和开发沟通，目前基本都是使用的是HTTPS（超文本传输安全协议）。这里详细的讲下使用Charles工具抓取手机端的HTTPS的请求配置。

       启动Charles工具后，在菜单栏点击Proxy，然后点击Proxy Settings，在弹出的框中配置端口，端口填写8888，然后点击OK按钮，见如图：

![](https://img-blog.csdn.net/2018090112381659?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

让手机和电脑连接同一个网络，查询电脑的IP地址（ipconfig），查询出来后，在手机端配置代理，见如下：

![](https://img-blog.csdn.net/2018090112383124?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

手机端设置代理成功后，Charles就会弹出一个框，直接点击允许。然后在Charles菜单栏中点击Help，然后点击SSL Prosying，在弹出框中点击如下图部分：

![](https://img-blog.csdn.net/20180901123847103?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

见点击后的截图：

![](https://img-blog.csdn.net/20180901123859193?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

       打开手机的浏览器，输入http://charlesproxy/getssl按下回车键后，跳转到一个新的地址，提示是否允许，点击允许，见如下所示：

![](https://img-blog.csdn.net/20180901123917850?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击允许后，跳转到一个安装描述文件，见如图：

![](https://img-blog.csdn.net/20180901123932890?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击安装按钮，会提示警告，忽略警告，再次点击安装按钮，见如图所示：

![](https://img-blog.csdn.net/20180901123948880?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在下面弹出的框中，直接点击安装，见图：

![](https://img-blog.csdn.net/20180901124001595?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装成功后，点击完成按钮，见图：

![](https://img-blog.csdn.net/20180901124014479?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下来设置信任证书，在苹果手机中，点击设置-->通用-->关于手机-->证书信任设置，点击后，见图：

![](https://img-blog.csdn.net/20180901124026852?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击信任证书，会弹出框，见图：

![](https://img-blog.csdn.net/20180901124044743?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



点击继续按钮，证书设置信任完成。

      下来在Charles菜单栏点击Proxy，然后点击SSL  Proxying Settings，配置弹出框显示的信息：

![](https://img-blog.csdn.net/20180901124104660?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后点击OK。至此，抓取手机上HTTPS的请求配置完成。

       在网易云课堂，搜索无涯 接口自动化测试，在手机上操作，在Charles就可以抓取到搜索的请求，见图：

![](https://img-blog.csdn.net/20180901124122829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

再见服务端返回的响应数据：

![](https://img-blog.csdn.net/20180901124134986?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



