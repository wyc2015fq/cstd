# 关于Resin SSL支持的两个问题 - =朝晖= - 博客园
# [关于Resin SSL支持的两个问题](https://www.cnblogs.com/dhcn/p/7105464.html)
1、Resin的OpenSLL支持功能只有收费Pro版才支持，这一点，只有你做好配置，[测试](http://lib.csdn.net/base/softwaretest)的时候才会在提示中发现，文档里没有说明。
2、它的官方文档这部分有问题，第一个问题就是上面第一条没有说，第二个大问题是官方文档中提供的Win32的OpenSSL Lib下载地址有问题，那个地址上提供的OpenSSL下载的东西实际安装后其实不可用，实际可用的到GNUWin上去下就成了，GNUWin这个东西我在XP下给Nginx做CSR的时候报错，后来在[Linux](http://lib.csdn.net/base/linux)虚拟机上做的key和crt(中间有一步需要做csr)。
