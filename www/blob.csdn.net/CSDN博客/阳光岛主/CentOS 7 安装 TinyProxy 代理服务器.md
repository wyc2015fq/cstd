
# CentOS 7 安装 TinyProxy 代理服务器 - 阳光岛主 - CSDN博客

2017年10月12日 21:57:53[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：16697标签：[代理IP																](https://so.csdn.net/so/search/s.do?q=代理IP&t=blog)[米扑代理																](https://so.csdn.net/so/search/s.do?q=米扑代理&t=blog)[TinyProxy																](https://so.csdn.net/so/search/s.do?q=TinyProxy&t=blog)[Proxy																](https://so.csdn.net/so/search/s.do?q=Proxy&t=blog)[代理																](https://so.csdn.net/so/search/s.do?q=代理&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Proxy&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=TinyProxy&t=blog)个人分类：[NetWork																](https://blog.csdn.net/sunboy_2050/article/category/698436)
[
																								](https://so.csdn.net/so/search/s.do?q=TinyProxy&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=米扑代理&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=米扑代理&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=代理IP&t=blog)

米扑科技，是一家专注互联网金融和大数据挖掘的初创互联网公司，正式注册成立于2016年9月，总部位于北京市海淀区中关村核心功能区。
米扑团队，成员来自百度、小米、阿里、创新工场等一线互联网公司，核心技术研发，实战经验丰富，具有浓厚的极客精神和精益的工匠品质。
**米扑代理**，是米扑科技旗下的一款用于网页抓取、数据采集的代理产品，作为全球代理服务的领导品牌，产品优势如下：
1. 五年来，已经积累超过2000多万的海量代理库，每天可用代理5000个左右
2. 覆盖全球120多个国家，中国34个省市，支持国家批量筛选
3. 支持http、https、socks4、socks5等协议，总能满足您的需求
4. 拥有完全自主研发的代理检测核心引擎，技术研发实力雄厚
5. 拥有几十台云服务器，完全自主搭建的代理服务器，分布于全球十多个国家
6. 作为全球代理的企业级领导品牌，服务于全球5000多家企业客户，日均代理提取数超过10亿个
**米扑代理官网**：[https://proxy.mimvp.com](https://proxy.mimvp.com/)
**米扑代理案例**
杨小米，最近在公司做爬虫工作，采集股票金融数据，写程序抓取数据的过程并不像平常我们用浏览器打开网页那么简单！
大多数的金融数据网站为了自己站点的性能和数据安全都设置了各种反爬策略。最常见的反爬虫策略有输入验证码、需要用户登陆、单个IP设置次数和频率、访问网页轨迹等等！所以广大的爬虫工程师，在抓取数据之前一般都会先调研目标网站是否设置了反爬策略。当不能正常访问网站时，可以在程序中设置代理IP来判断网站是不是封禁了我们的IP，或者使用海量的代理IP来爬取网页采集数据，推荐使用米扑代理：[https://proxy.mimvp.com](https://proxy.mimvp.com/)
**CentOS 7 搭建 TinyProxy 代理**
TinyProxy 和 Squid 都是比较优秀的代理软件
TinyProxy比较小众，虽然没有Squid的功能丰富，但是小巧简单，也能满足普通用户的需求。
Squid 是一款优秀的代理软件，有很丰富的ACL管理功能，虽然squid很强大，但配置比较繁琐。
**1. 安装 TinyProxy**
yum -y install tinyproxy
**2. 配置 TinyProxy**
vim /etc/tinyproxy/tinyproxy.conf
修改 Port 端口，默认为 8888
Port 8888
注释掉 Allow，表示允许所有人访问代理
\#Allow 127.0.0.1
隐藏掉Via请求头部，去掉下面的注释
DisableViaHeader Yes
更多配置项，下面是列举一些配置文件默认的，不需要配置：
PidFile "/var/run/tinyproxy/tinyproxy.pid"
LogFile "/var/log/tinyproxy/tinyproxy.log"
LogLevel Info
MaxClients 100
MinSpareServers 5
MaxSpareServers 20
StartServers 10
**3. 启动 TinyProxy**
systemctl start tinyproxy.service
更多命令如下：
systemctl restart tinyproxy.service
systemctl stop tinyproxy.service
systemctl status tinyproxy.service
systemctl enable tinyproxy.service
**4. 关闭防火墙或开放端口访问**
方式1：关闭防火墙
/etc/init.d/iptables stop
方式2：开放端口访问
iptables -A INPUT -p tcp --dport 8888 -j ACCEPT
重启防火墙
/etc/init.d/iptables restart
注意：若采用的阿里云、腾讯云、AWS等云服务器，记得要在安全策略里开放端口号
**5. 验证生效**
1）命令行测试验证
curl 和 wget 测试使用代理，请参考[米扑代理的使用示例](https://proxy.mimvp.com/demo2.php)，总结的非常全面！强烈推荐！
这里，直接使用米扑代理的最简单示例：
curl -m 30 --retry 3 -x http://58.87.90.149:8888 http://proxy.mimvp.com/exist.php| grep 'color="red"'
<font color="red">58.87.90.149</font>
curl -m 30 --retry 3 -x http://58.87.90.149:8888 -k https://proxy.mimvp.com/exist.php| grep 'color="red"'
<font color="red">58.87.90.149</font>
tinyproxy 支持 http 和 https 两种协议，经米扑代理测试，成功！
2）Firefox 插件测试
首先，安装 Proxy Switcher
其次，配置代理，如下图：
![centos-7-an-zhuang-tinyproxy-dai-li-fu-wu-qi-01](https://blog.mimvp.com/wp-content/uploads/2016/08/centos-7-an-zhuang-tinyproxy-dai-li-fu-wu-qi-01.png)
然后，访问米扑代理网站，验证代理
[https://proxy.mimvp.com/exist.php](https://proxy.mimvp.com/exist.php)
![centos-7-an-zhuang-tinyproxy-dai-li-fu-wu-qi-02](https://blog.mimvp.com/wp-content/uploads/2016/08/centos-7-an-zhuang-tinyproxy-dai-li-fu-wu-qi-02.png)
上图里，http_via 带有参数，要隐藏需要修改配置文件
vim /etc/tinyproxy/tinyproxy.conf
隐藏掉Via请求头部，去掉下面的注释
DisableViaHeader Yes
重启 tinyproxy
systemctl restart tinyproxy.service

**网友评价**：
非常棒的总结，按照米扑代理的配置步骤，都配置成功了！分享米扑代理的测试方法：
curl 和 wget 测试使用代理，请参考米扑代理的使用示例，总结的非常全面！强烈推荐！
米扑代理的使用示例：[https://proxy.mimvp.com/demo2.php](https://proxy.mimvp.com/demo2.php)
这里，直接使用米扑代理的最简单示例：
curl -m 30 --retry 3 -x http://58.87.90.149:8888 http://proxy.mimvp.com/exist.php| grep 'color="red"'
<font color="red">58.87.90.149</font>
curl -m 30 --retry 3 -x http://58.87.90.149:8888 -k https://proxy.mimvp.com/exist.php| grep 'color="red"'
<font color="red">58.87.90.149</font>


