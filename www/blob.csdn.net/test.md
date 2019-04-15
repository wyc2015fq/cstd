# 手机抓包操作文档 - qq_21826709的博客 - CSDN博客












2017年01月18日 14:26:41[Guan-Javid](https://me.csdn.net/qq_21826709)阅读数：225








手机抓包操作文档 

前言 

（必看）下列无论哪种方法都是捕获网络接口所传输的数据包，并不能捕获针对某一个软件或者APP所发出数据。 

所以在捕获之前尽量减少其他不相关软件或者APP的影响，在捕获时尽量不要太多多余的繁复的操作。要尽量有针对性，目的性的去捕获。
Android抓包 

方法一、使用tcpdump方法抓包（可以抓WIFI、4G、3G） 

前提： 

?手机需要root权限。 

?电脑已经安装相应的环境。JDK的安装，SDK的安装。 

详细方法可以点击图标： 
![这里写图片描述](https://img-blog.csdn.net/20170118141652197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20170118141729082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

简易操作说明： 
![这里写图片描述](https://img-blog.csdn.net/20170118141933598?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
方法二、使用wireshark软件抓包（只可以抓电脑显示的网络接口） 

前提： 

?用第三方软件创建免费WIFI。 

?电脑安装wireshark软件。 

简易操作说明： 

1.用360创建一个免费WIFI网络，手机连接360发出来的WIFI。 
![这里写图片描述](https://img-blog.csdn.net/20170118141957724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20170118142012208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2.打开网络和共享中心，进入更改适配器设置，查看360免费WIFI网络接口名字。 
![这里写图片描述](https://img-blog.csdn.net/20170118142031983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.打开wireshark软件，双击相应的网络接口进行捕获。 
![这里写图片描述](https://img-blog.csdn.net/20170118142057484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

IOS抓包 

说明： 

iOS 5后，apple引入了RVI remote virtual interface的特性，它只需要将iOS设备使用USB数据线连接到mac上，然后使用rvictl工具以iOS设备的UDID为参数在Mac中建立一个虚拟网络接口rvi，就可以在mac设备上使用wireshark 、tcpdump等工具对创建的接口进行抓包分析了。
方法一、在Mac电脑使用wireshark抓包。（可以抓WIFI、4G、3G） 

1.使用USB数据线将iOS设备连接到MAC上 

2.点击ituns，查看复制UDID（重点是获取手机UDID，用什么工具获取不重要） 
![这里写图片描述](https://img-blog.csdn.net/20170118142129063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3.使用MAC命令行输入rvictl –s （注意命令里面的空格） 
![这里写图片描述](https://img-blog.csdn.net/20170118142158267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170118142217678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170118142248616?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170118142317597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170118142341738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170118142400676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170118142414333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170118142427695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4.打开wireshark开始，点击相应网口，开始捕获。 
![这里写图片描述](https://img-blog.csdn.net/20170118142528663?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE4MjY3MDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

终极秘籍： 

1.有事靠度娘。 

自行百度相关字词。解决问题。（优先百度标题中的关键字） 

2.问周围懂得的人。 

总有人可以解决的，万一解决不了或者情况紧急，就问一下比你高级的人，他们都有自己的一套解决方法。








