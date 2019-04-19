# https的证书 - 左直拳的马桶_日用桶 - CSDN博客
2017年05月24日 06:16:46[左直拳](https://me.csdn.net/leftfist)阅读数：834
将一个网站设置成支持https协议很简单，以IIS为例：
![这里写图片描述](https://img-blog.csdn.net/20170524045825259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170524045840012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170524045858902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一定要绑定一个证书，否则会无法访问。
另外，https这里绑定的端口是443（默认），那么地址 
[https://testweb/webapi/api/test/test1/1](https://testweb/webapi/api/test/test1/1) 相当于 
[https://testweb:443/webapi/api/test/test1/1](https://testweb:443/webapi/api/test/test1/1)。 
我有一次因为创建了一个本地网站，端口是8000，然后支持https，思维定式地认为访问地址是：[https://testweb:8000/](https://testweb:8000/)，结果提示404。https可不是单纯地在http后面加个s而已，是两种不同的协议，分别有自己对应的端口。
以上可见，https需要证书。证书，亦即数字证书，在https这里，分为
> 
服务器端证书 
  客户端证书
数字证书的作用是身份确认。服务器端证书证明你就是那个网站；客户端证书证明你就是那个用户。 
有关https的基础知识，可看拙作： 
[http://blog.csdn.net/leftfist/article/details/67613837](http://blog.csdn.net/leftfist/article/details/67613837)
最重要的就是服务器端证书。https网站必须绑定一个证书，否则无法访问。至于客户端证书，非必须。
证书在正式部署的时候，可以购买或申请。但在开发阶段，我们可以自己生成。
1、自建根证书
`makecert -r -pe -n "CN=WebSSLTestRoot" -b 12/22/2013 -e 12/23/2024 -ss root -sr localmachine -len 2048`
makecert -r -pe -n “CN=WebSSLTestRoot” -b 12/22/2013 -e 12/23/2024 -ss root -sr localmachine -len 2048
这个根证书会安装到本机“受信任的根证书颁发机构”里去。下面的网站证书和客户端证书都由它来颁发。
[证书创建工具 (Makecert.exe)使用说明](https://docs.microsoft.com/zh-cn/previous-versions/dotnet/netframework-2.0/bfsktky3%28v=vs.80%29)
查看本机证书情况，可以通过运行“mmc”，点“文件”，选择“添加管理单元”- “证书”来查阅。可参考拙作： 
[http://blog.csdn.net/leftfist/article/details/53381335](http://blog.csdn.net/leftfist/article/details/53381335)
2、网站证书
`makecert -pe -n "CN=localhost" -b 12/22/2013 -e 12/23/2024 -eku 1.3.6.1.5.5.7.3.1 -is root -ir localmachine -in WebSSLTestRoot -len 2048 -ss my -sr localmachine`
makecert -pe -n “CN=localhost” -b 12/22/2013 -e 12/23/2024 -eku 1.3.6.1.5.5.7.3.1 -is root -ir localmachine -in WebSSLTestRoot -len 2048 -ss my -sr localmachine
cn是网站对应的域名。这个域名很重要，不能随便起，否则跟网站对应不上，浏览器会认为不可信。
网站证书部署是将网站和该证书绑定。 
![这里写图片描述](https://img-blog.csdn.net/20170524045858902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2017.8.16更新 
以上又是创建根证书，又是创建网站证书，在本机测试和局域网是可以的，但在互联网上好像就不行。 
如果像以下那样，不用根证书，单独发布一个就可以：
`makecert -r -pe -n "CN=服务器IP地址" -b 01/01/2017 -e 01/01/2110 -sky exchange -ss my  -sr localmachine`
但我不知道究竟是不用根证书起了作用，还是因为用IP地址作为名称起了作用。
3、客户端证书
`makecert -pe -n "CN=leftfist" -eku 1.3.6.1.5.5.7.3.2 -is root -ir localmachine -in WebSSLTestRoot -ss my -sr currentuser -len 2048`
makecert -pe -n “CN=leftfist” -eku 1.3.6.1.5.5.7.3.2 -is root -ir localmachine -in WebSSLTestRoot -ss my -sr currentuser -len 2048
cn可以是机器名，账户名。这个可以随便起，方便辨认。
以上这个语句，已将客户端证书生成到当前账户下的个人证书里，无须再做什么部署。在首次访问网站的时候，浏览器会弹出对话框，让确认用哪张证书来访问，选择我们自己生成的这张，即可正常访问矣。
说是这么说，我在一台机器上可以，但在家里这台机，浏览器并没有弹出对话框，也就无法访问，跟没有客户端证书一样。原因未明，可能是我浏览器的问题。
不过，客户端证书不是必须的，就看我们在服务器配置SSL的时候选了哪个选项。我想，只有在类似网银这种很严格的场合才要吧。
makercert.exe是系统自带的。如果没有，从WIN7机器中拷贝一个到服务器（比如win2008)也可以。 或者到我[这里下载](http://download.csdn.net/detail/leftfist/9695991)
参考文章： 
[iis https 客户端证书](http://www.cnblogs.com/czcz1024/p/5695842.html)
[IIS SSL客户端证书（忽略/接受/必须）之三——思考验证（3）](http://mlxia.blog.51cto.com/972988/1631157)
