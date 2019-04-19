# CentOS6.5配置代理上网 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月17日 11:48:11[左直拳](https://me.csdn.net/leftfist)阅读数：8227
虚拟机vmWare下装了个CentOS，第一件事是配置代理上网，因为工作环境需要使用代理上网，但发觉与uBuntu不同，根本没有什么/etc/environment。
在网上疯狂搜索，都说要配置/etc/profile，如果只是当前用户使用，则只须配置 ~/.dash_profile。方法如下：
http_proxy="http://user:password@server:port/"
export http_proxy #这个是设为环境变量
保存退出后，要让它生效：
source /etc/profile
扯淡！根本无效
后来发现，在图形界面，顶端有个 **系统 - 首选项 - 网络代理**，在这里设置就行了。但内容显然不是保存在/etc/profile，不知道保存在哪里，先存疑。
YUM也要设置，YUM就相当于uBuntu的apt-get
方法是编辑 /etc/yum.conf
输入：
#proxy
proxy=http://user:password@server:port/
保存退出即可。
注意：没有双引号。名称也就一个：proxy。

