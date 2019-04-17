# 科学网—[转载]WIN7如何设置WIFI上网 - 王伟华的博文




# [转载]WIN7如何设置WIFI上网                           

已有 2973 次阅读2014-8-28 22:08|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|文章来源:转载



1、以管理员身份[运行命令](http://www.so.com/s?q=%E8%BF%90%E8%A1%8C%E5%91%BD%E4%BB%A4&ie=utf-8&src=wenda_link)提示符:快捷键win+R→输入cmd→回车

2、启用并设定虚拟WiFi网卡:运行命令:netsh wlan set hostednetwork mode=allow ssid=mywifi key=12345678此命令有三个参数，mode:是否启用虚拟WiFi网卡，改为[disallow](http://www.so.com/s?q=disallow&ie=utf-8&src=wenda_link)则为禁用。 ssid:无线网名称，最好用英文(以mywifi为例)。 key:无线网密码，八个以上字符(以12345678为例)。以上三个参数可以单独使用，例如只使用mode=disallow可以直接禁用虚拟Wifi网卡。

3，开启成功后，网络连接中会多出一个网卡为“Microsoft Virtual WiFi Miniport Adapter”的无线连接。若没有，只需更新[无线网卡](http://www.so.com/s?q=%E6%97%A0%E7%BA%BF%E7%BD%91%E5%8D%A1&ie=utf-8&src=wenda_link)驱动就OK了。 

4、设置Internet连接共享:在“网络连接”窗口中，右键单击已连接到Internet的网络连接，选择“属性”→“共享”，勾上“允许其他······连接(N)”并选择“虚拟WiFi”。确定之后，提供共享的网卡图标旁会出现“共享的”字样，表示“[宽带连接](http://www.so.com/s?q=%E5%AE%BD%E5%B8%A6%E8%BF%9E%E6%8E%A5&ie=utf-8&src=wenda_link)”已共享至“虚拟WiFi”。

5、开启无线网络:继续在[命令提示符](http://www.so.com/s?q=%E5%91%BD%E4%BB%A4%E6%8F%90%E7%A4%BA%E7%AC%A6&ie=utf-8&src=wenda_link)中运行:netsh wlan start hostednetwork

(将start改为stop即可关闭该无线网，以后开机后要启用该无线网只需再次运行此命令即可)至此，虚拟WiFi的红叉叉消失，WiFi[基站](http://www.so.com/s?q=%E5%9F%BA%E7%AB%99&ie=utf-8&src=wenda_link)已组建好。笔记本、带[WiFi模块](http://www.so.com/s?q=WiFi%E6%A8%A1%E5%9D%97&ie=utf-8&src=wenda_link)的手机等子机搜索到无线网络mywifi，输入密码12345678，就能共享上网啦!







win7怎么设置wifi

[http://jingyan.baidu.com/article/546ae1857d38eb1148f28c4c.html](http://jingyan.baidu.com/article/546ae1857d38eb1148f28c4c.html)




转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-816123.html](http://blog.sciencenet.cn/blog-81613-816123.html)

上一篇：[[转载]数据显示死于空难的概率是470万分之一](blog-81613-814866.html)
下一篇：[英文简历常用词汇](blog-81613-823760.html)


