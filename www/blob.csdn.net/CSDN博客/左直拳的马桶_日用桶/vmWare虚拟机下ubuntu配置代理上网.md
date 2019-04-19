# vmWare虚拟机下ubuntu配置代理上网 - 左直拳的马桶_日用桶 - CSDN博客
2014年09月29日 22:57:48[左直拳](https://me.csdn.net/leftfist)阅读数：13241
终于用上了linux，windows下用vmWare安装虚拟机，挂ubuntu 14.04,，觉得很不习惯，很难侍候。
并且遇到的客观困难还不少，这两天着实费了不少功夫。主要是上班这里上网要配置代理。windows下，在IE的连接配置一下，其他浏览器都可以用了。而在ubuntu下，就没有这么简单。当然，事后想起来，好像也不算很复杂，就是不习惯。
1、首先要保证两个虚拟机服务要启动：VMware NAT Service、VMware DHCP Service。我因为有个系统优化软件将它们关闭了，结果搞到ubuntu无法联网，查来查去，焦头烂额，半天就过去了
2、进入ubuntu的系统设置，NetWork，然后设置代理，应用（一定要按下应用）
3、如果代理需要指定用户名和密码，那么本步骤就是必须。并且一定要先完成2，然后再做这一步。
打开终端，输入 sudo gedit /etc/environment
系统提示输入超级管理员密码。注意Linux下，密码输入没有显示，但确确实实是有输入。
打开这个环境配置文件，将代理及账号、密码输入。如
http_proxy="http://account:password@proxyhost:port"
https_proxy="https://account:password@proxyhost:port"
……
因为步骤2完成以后，系统会将代理信息覆盖到这里，但并无账号信息，只有代理服务器信息。如果先做步骤3，再做步骤2，那我们的工作成果会被覆盖掉。
4、set up the proxies for apt-get and Update Manager
终端下，输入sudo gedit /etc/apt/apt.conf
然后输入：
Acquire::http::proxy "http://account:password@proxyhost:port";
Acquire::https::proxy "https://account:password@proxyhost:port";
Acquire::ftp::proxy "ftp://account:password@proxyhost:port"";
Acquire::socks::proxy "socks://account:password@proxyhost:port";
如此，大功告成矣，软件中心，安装、更新软件都能顺利联网。保证网络畅通非常重要，否则安装、更新失败，各种错误提示简直让人发疯。
不过，这些设定似乎并不对浏览器完全起作用，浏览器每次打开都要询问密码。
Linux下，firefox可以设置自己的代理；但谷歌的chromium却不行。它能使用系统的代理，但不经代理的设置却没有在chromium起效果。解决的途径是在chromium中安装扩展程序：SwitchySharp。方法是打开chromium的设置，扩展程序，输入 SwitchySharp，找到它，安装它，设置它，搞定它。
