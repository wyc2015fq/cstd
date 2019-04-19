# Ubuntu设置代理的方法 - =朝晖= - 博客园
# [Ubuntu设置代理的方法](https://www.cnblogs.com/dhcn/p/7105432.html)
用过Linux的都知道，众多的PROXY配置，让人应接不暇，本文列出常见的一些PROXY的配置
1.apt-get proxy 的配置
sudo gedit /etc/apt/apt.conf 
NOTE：系统里有可能没有这个文件，直接建立一个就行
输入下面的proxy的配置信息就行
Acquire::http::Proxy "http://username:password@address:port";
也可以增加ftp和https的proxy
Acquire::ftp::Proxy "http://username:password@address:port";
Acquire::https::Proxy "http://username:password@address:port";
其中：
username 用户名称
password 用户密码
address: 代理服务器地址
port:    代理端口号
下面的表述都是这样的，不再重复
2.terminal proxy 配置
gedit ～/.bashrc
输入下面的proxy的配置信息就行
export http_proxy=http://username:password@address:port
也可以增加ftp和https的proxy
export ftp_proxy=http://username:password@address:port
export https_proxy=http://username:password@address:port
3. UI的proxy设置
点击 System -> Preferences -> Network Proxy
4. 设置Synaptic Package Manager的proxy
System -> Administration -> Synaptic Package Manager -> Settings -> Preferences
5. 设置浏览器的proxy
这个由各个浏览器的特性来决定，比较简单

