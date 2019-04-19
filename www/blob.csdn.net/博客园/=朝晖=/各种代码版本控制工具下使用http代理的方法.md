# 各种代码版本控制工具下使用http代理的方法 - =朝晖= - 博客园
# [各种代码版本控制工具下使用http代理的方法](https://www.cnblogs.com/dhcn/p/7105402.html)
原文：各种SCM工具下使用http代理下载源码：[http://www.linuxeden.com/html/develop/20090723/66951.html](http://www.linuxeden.com/html/develop/20090723/66951.html)
SCM是软件配置管理的简称，常见的SCM系统有CVS，SVN，HG（Mercurial）和[Git](http://lib.csdn.net/base/git)等。
经常有使用SCM下载源码的必要，尤其是软件开发人员。但偶们的网络环境经常逼得我们不得不使用代理，或者是因为速度，或者是因为直连根本就不通。而且源码通常是零零碎碎的几千几万个小文件，不像一般的软件是单个的文件，一个下载工具即可搞定。
1，CVS
CVS使用代理非常麻烦，似乎不能用http代理（如果偶错了请提醒），而socks代理相当难找。好在这个东东已经慢慢变成了昨日黄花，我们不讨论它。
2，SVN
SVN比较方便，编辑~/.subversion/servers文件即可：
[global]
http-proxy-host = <代理的ip或域名>
http-proxy-port = <代理端口>
http-proxy-username = <用户名，如果需要的话>
http-proxy-password = <密码，如果需要的话>
3，HG
HG更简单，因为它使用的是http协议，一个命令解决：
export http_proxy=http://<用户名>:<密码>@<ip>:<端口>
4，[git](http://lib.csdn.net/base/git)
Git可以使用两种协议，git协议和http，如果是后者，那么办法跟HG一样，如果是git，比较麻烦，但也不是不可解决：
先去下载一个协议转换小工具：http://www.meadowy.org/~gotoh/ssh/connect.c
编译之：gcc -o connect connect.c
将生成的connect放到可执行文件路径中，如/usr/bin
然后创建一个使用代理的脚本，假设是http-proxy-gw，注意加上可执行属性，内容如下：
#!/bin/sh
# Filename: ~/bin/http-proxy-gw
# This ｓｃｒｉｐｔ connects to an HTTP proxy using connect.c
connect -H http://yourproxyaddress:proxyport $@
最后，配置git：
git config --global core.gitproxy "http-proxy-gw for xxx.yyy"
后面的xxx.yyy是域名后缀，比如，下载[Linux](http://lib.csdn.net/base/linux)源码就可以是设置为kernel.org

