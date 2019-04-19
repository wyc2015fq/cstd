# vmware虚拟机NAT模式下的端口转发 - xqhrs232的专栏 - CSDN博客
2017年07月18日 16:01:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1587
原文地址::[http://jingyan.baidu.com/article/03b2f78c6cd4cd5ea337ae11.html](http://jingyan.baidu.com/article/03b2f78c6cd4cd5ea337ae11.html)
相关文章
1、[VMware虚拟机NAT模式的具体配置](http://blog.csdn.net/whatday/article/details/7939406)----[http://blog.csdn.net/whatday/article/details/7939406](http://blog.csdn.net/whatday/article/details/7939406)
2、利用端口转发工具实现socket测试----[http://blog.sina.com.cn/s/blog_1508344760102wju2.html](http://blog.sina.com.cn/s/blog_1508344760102wju2.html)
虚拟机网络选择NAT方式，启用端口转发，可以实现宿主机器和虚拟机的端口映射，如果我们把映射虚拟机的端口设为22，就可以通过宿主机器ip加端口直接远程虚拟机了。本文以vmware为例：
## 工具/原料
- 
普通pc
- 
vmware虚拟机
## 方法/步骤
- 
点“编辑” “虚拟网络编辑器”
![vmware虚拟机NAT模式下的端口转发](http://a.hiphotos.baidu.com/exp/w=500/sign=a1004273ad014c08193b28a53a7a025b/0b46f21fbe096b63be00918a08338744eaf8accb.jpg)
- 
选择“Nat设置”
![vmware虚拟机NAT模式下的端口转发](http://b.hiphotos.baidu.com/exp/w=500/sign=3a909b3af303918fd7d13dca613f264b/2934349b033b5bb5905dec2432d3d539b700bc66.jpg)
- 
选择“添加”端口转发
![vmware虚拟机NAT模式下的端口转发](http://e.hiphotos.baidu.com/exp/w=500/sign=cf628095ce8065387beaa413a7dca115/cf1b9d16fdfaaf51afaf8464885494eef11f7a98.jpg)
- 
主机端口：宿主机器的端口，我这里设置的5555
虚拟机端口：实现远程，我们这里选22
描述：随便描述下
![vmware虚拟机NAT模式下的端口转发](http://h.hiphotos.baidu.com/exp/w=500/sign=dc670118b6119313c743ffb055390c10/91ef76c6a7efce1b12320710ab51f3deb58f65cb.jpg)
- 
测试：在浏览器里输入宿主机器的ip和端口号，出现下图所示，说明设置成功，
![vmware虚拟机NAT模式下的端口转发](http://c.hiphotos.baidu.com/exp/w=500/sign=5fdaac11b6fb43161a1f7a7a10a64642/3bf33a87e950352aa4ebe9d25743fbf2b3118b66.jpg)
- 
ssh远程:此时就能在别的机器通过宿主机器的ip和端口号上进行ssh远程了。
![vmware虚拟机NAT模式下的端口转发](http://f.hiphotos.baidu.com/exp/w=500/sign=cb235a8b083387449cc52f7c610ed937/b3b7d0a20cf431adf6e847e24f36acaf2edd9836.jpg)步骤阅读
END
## 注意事项
- 
注意：宿主机器要是开防火墙的话，要把主机端口（5555）添加例外！
