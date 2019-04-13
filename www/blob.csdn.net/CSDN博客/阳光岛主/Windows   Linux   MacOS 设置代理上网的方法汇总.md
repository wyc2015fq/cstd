
# Windows - Linux - MacOS 设置代理上网的方法汇总 - 阳光岛主 - CSDN博客

2018年12月12日 23:20:46[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1020个人分类：[NetWork																](https://blog.csdn.net/sunboy_2050/article/category/698436)



本文汇总了 Windows / Linux / MacOS 设置代理上网的各种方法，总结如下：
1、设置系统代理（Windows、Linux、MacOS）
2、设置代理插件（Chrome、Chromium、Firefox、Opera、QQ等浏览器）
3、第三方软件设置代理，如[Proxifier](https://blog.mimvp.com/article/28488.html)、[花刺代理](https://blog.mimvp.com/article/29584.html)、[代理猎手](https://blog.mimvp.com/article/29586.html)、[QQ设置代理](https://blog.mimvp.com/article/29704.html)
4、编程使用代理，请见：[代码示例](https://proxy.mimvp.com/demo.php)

**一、设置代理上网的方法汇总**
**1、Windows 设置代理**
1）[Windows 系统代理设置](https://blog.mimvp.com/article/25794.html#menu-windows)（局部代理）
2）[Windows 7 / 10 设置全局和授权代理上网的图文教程](https://blog.mimvp.com/article/28460.html)（全局代理）
3）[浏览器设置代理插件上网的图文教程](https://blog.mimvp.com/article/21108.html)（代理插件）
**2、Linux（Ubuntu / CentOS）设置代理**
1）[Ubuntu 系统代理设置](https://blog.mimvp.com/article/25794.html#menu-ubuntu)（局部代理）
2）[Linux / MacOS 设置全局和授权代理上网的图文教程](https://blog.mimvp.com/article/29268.html#menu-linux)（全局代理）
3）[浏览器设置代理插件上网的图文教程](https://blog.mimvp.com/article/21108.html)（代理插件）
**3、MacOS 设置代理**
1）[MacOS 系统代理设置](https://blog.mimvp.com/article/25794.html#menu-macos)（局部代理）
2）[Linux / MacOS 设置全局和授权代理上网的图文教程](https://blog.mimvp.com/article/29268.html#menu-macos)（全局代理）
3）[浏览器设置代理插件上网的图文教程](https://blog.mimvp.com/article/21108.html)（代理插件）
**4、Android / iPhone / iPad 设置代理**
[Android / iPhone / iPad 设置全局和授权代理上网的图文教程](https://blog.mimvp.com/article/29274.html)

**二、浏览器支持的代理插件**
1、[SwitchyOmega](https://blog.mimvp.com/article/29144.html)（强烈推荐）
2、[Proxy Switcher and Manager](https://blog.mimvp.com/article/29266.html)（推荐）
3、[Proxy Switcher](https://blog.mimvp.com/article/29264.html)（推荐）
4、[SimpleProxy](https://blog.mimvp.com/article/29262.html)（推荐）
本文使用米扑代理的独享代理做的测试，100%高匿，99%可用，可免费试用5小时
米扑代理：[https://proxy.mimvp.com](https://proxy.mimvp.com/)
![](https://cdn-blog.mimvp.com/wp-content/uploads/2017/01/liu-lan-qi-she-zhi-dai-li-cha-jian-shang-wang-de-tu-wen-jiao-cheng-09.png)
**浏览器支持的操作系统**
![](https://cdn-blog.mimvp.com/wp-content/uploads/2017/01/liu-lan-qi-she-zhi-xi-tong-dai-li-shang-wang-de-tu-wen-jiao-cheng-01.png)
有些浏览器支持 Windows、Ubuntu、MacOS 等操作系统，如 Chrome、Chromium、Firefox、Opera
有些浏览器支持 Windows、MacOS 操作系统，如 QQ浏览器、傲游浏览器，不过都做了阉割简化
有些浏览器支持 Windows，如大部分国产浏览器 360、UC、搜狗、百度等浏览器
米扑博客进行了大量的调研，分享成果如下，有遗漏或错误之处，恳盼指正。
![](https://cdn-blog.mimvp.com/wp-content/uploads/2017/01/liu-lan-qi-she-zhi-xi-tong-dai-li-shang-wang-de-tu-wen-jiao-cheng-02.png)
Chrome 和 Chromium 之间的关系，请见米扑博客：[Chrome 和 Chromium 区别](https://blog.mimvp.com/article/28645.html)

**三、第三方设置代理的软件**
**1、****[Proxifier](https://blog.mimvp.com/article/28488.html)**（推荐）
支持 Windows、MacOS 系统的全局代理软件
支持 http、https、socks4、socks5 等多种协议
支持 密码授权和无密码授权，支持白名单ip授权
**2、****[花刺代理](https://blog.mimvp.com/article/29584.html)**（ProxyThorn）
只支持 Windows
支持 http、https 等多种协议
花刺代理可直接设置代理ip，通过系统代理设置
**3、****[代理猎手](https://blog.mimvp.com/article/29586.html)**（ProxyHunter）
只支持 Windows
支持 http、socks5 等多种协议
代理猎手不可以直接设置代理，需要借助系统代理设置 127.0.0.1
支持扫码代理，但效率比较低，扫描量太大，且只扫描无授权的代理
**4、****[QQ设置代理](https://blog.mimvp.com/article/29704.html)**
QQ设置代理只支持Windows QQ客户端，不支持MacOS、Android、iOS等客户端
QQ设置代理，支持 http 和 socks5，支持代理有授权和无授权

**四、米扑代理的代码示例**
[米扑代理](https://proxy.mimvp.com/)，是一家专业、高可用、高可靠的代理提供商，团队来自BAT技术研发，米扑代理的代码示例，包含**Python、Java、PHP、C\#、Go、Perl、Ruby、Shell**等十多种编程语言，已开源Github：[mimvp-proxy-demo](https://github.com/mimvp/mimvp-proxy-demo)
米扑代码示例：[https://proxy.mimvp.com/demo.php](https://proxy.mimvp.com/demo.php#demo-main-div)
![](https://cdn-blog.mimvp.com/wp-content/uploads/2017/01/mimvp-dai-li-windows-linux-macos-she-zhi-dai-li-shang-wang-de-fang-fa-hui-zong-02.png)
编程使用代理，必须 IP + Port + http/https(socks4/5)协议 三个字段，一起配合使用（很多用户反馈代理无法使用原因在此），如下图：
![](https://cdn-blog.mimvp.com/wp-content/uploads/2017/01/mimvp-dai-li-windows-linux-macos-she-zhi-dai-li-shang-wang-de-fang-fa-hui-zong-01.png)
验证代理ip是否可用，米扑推荐验证步骤如下：
1、注册[米扑代理](https://proxy.mimvp.com/)，免费试用5小时的超级代理，推荐提取[独享代理](https://proxy.mimvp.com/fetchsole.php)测试，可用率100%
2、使用浏览器插件快速验证，强烈推荐使用代理插件[SwitchyOmega](https://blog.mimvp.com/article/29144.html)
3、若是 linux / unix，推荐使用命令行验证，更多命令请见：[米扑代理](https://proxy.mimvp.com/)—>[代码示例](https://proxy.mimvp.com/demo.php)—>[Shell curl wget](https://proxy.mimvp.com/demo.php#language-shell)
|1
|2
|3
|4
|5
|6
|7
|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\# proxy no auth（代理无用户名密码验证）\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
|curl -m 30 --retry 3 -x http:|//120|.77.176.179:8888 http:|//proxy|.mimvp.com|/test_proxy2|.php|\# http_proxy
|curl -m 30 --retry 3 -x http:|//46|.105.214.133:3128 -k https:|//proxy|.mimvp.com|/test_proxy2|.php|\# https_proxy
|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\# proxy auth（代理需要用户名和密码验证） \#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
|curl -m 30 --retry 3 -x http:|//username|:password@210.159.166.225:5718 http:|//proxy|.mimvp.com|/test_proxy2|.php|\# http
|curl -m 30 --retry 3 -x http:|//username|:password@210.159.166.225:5718 https:|//proxy|.mimvp.com|/test_proxy2|.php|\# https
|
说明：上面的代理ip是临时的，需换成你提取的最新的[米扑代理](https://proxy.mimvp.com/fetchsole.php)，有些人直接复制粘贴说不可用，我也是服了醉了...
4、使用第三方代理软件[Proxifier](https://blog.mimvp.com/article/28488.html)，支持 Windows、MacOS 系统，支持 http/https/socks4/socks5 代理，支持用户名密码授权
若还是解决不了或不明白的，先看米扑代理的[官网文档](https://proxy.mimvp.com/question.php)，非常专业、全面
欢迎联系米扑代理的技术客服，QQ：463103470，QQ群：783604156

**五、****设置代理的图文教程**
[Windows / Linux / MacOS 设置代理上网的方法汇总](https://blog.mimvp.com/article/28495.html)
[Android / iPhone / iPad 设置全局和授权代理上网的图文教程](https://blog.mimvp.com/article/29274.html)
[Windows 7 / 10 设置全局和授权代理上网的图文教程](https://blog.mimvp.com/article/28460.html)
[Linux / MacOS 设置全局和授权代理上网的图文教程](https://blog.mimvp.com/article/29268.html)
[浏览器设置代理插件上网的图文教程](https://blog.mimvp.com/article/21108.html)（代理插件）
[浏览器设置系统代理上网的图文教程](https://blog.mimvp.com/article/25794.html)（系统代理）
[浏览器代理插件：SwitchyOmega](https://blog.mimvp.com/article/29144.html)
[浏览器代理插件：Proxy Switcher and Manager](https://blog.mimvp.com/article/29266.html)
[浏览器代理插件：Proxy Switcher](https://blog.mimvp.com/article/29264.html)
[浏览器代理插件：SimpleProxy](https://blog.mimvp.com/article/29262.html)
[Proxifier 代理软件介绍和使用教程](https://blog.mimvp.com/article/28488.html)
[ProxyThorn 花刺代理软件介绍和使用教程](https://blog.mimvp.com/article/29584.html)
[ProxyHunter 代理猎手软件介绍和使用教程](https://blog.mimvp.com/article/29586.html)
[QQ 设置代理上网的详细教程](https://blog.mimvp.com/article/29704.html)
[编程使用代理的代码示例](https://proxy.mimvp.com/demo.php)（含十多种编程语言示例）

**致谢**
本文测试的代理，全部来自米扑代理：[https://proxy.mimvp.com](https://proxy.mimvp.com/)
米扑代理采购了大量阿里云、腾讯云等服务器自建代理，支持http、https、socks4、socks5等多种协议
推荐使用米扑代理的独享代理做测试，其有效期有一个月，100%高匿，99.9%可用，可免费试用5小时
感谢米扑代理的鼎力支持，谢谢！

