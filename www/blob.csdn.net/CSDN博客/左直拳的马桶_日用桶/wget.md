# wget - 左直拳的马桶_日用桶 - CSDN博客
2014年10月22日 09:06:50[左直拳](https://me.csdn.net/leftfist)阅读数：1392
个人分类：[Linux](https://blog.csdn.net/leftfist/article/category/2605933)
有一台Linux虚拟机，多人共用。每个人用自己的账号登录进去操作。需要设置网络代理才能上网。网络代理需要包含代理服务器的地址、端口，个人账号及密码。
众所周知，每个账户可以设置自己的配置文件：~/.bash_profile。但是，wget命令似乎不能使用这个配置文件。wget使用的是/etc/wgetrc。
于是问题就来了，难道真要将某个人的账号密码写到这个共用的/etc/wgetrc里面去吗？这样的话那个人牺牲有点大。学习Linux技术到底哪家强？
可以这样子：
在/etc/wgetrc里写入代理服务器及端口：
http_proxy=http://proxyserver:port/
ftp_proxy=ftp://proxyserver:port/
然后在使用wget命令的时候，账号和密码写到参数里面：
wget --proxy-user=账号 --proxy-password=密码 url
