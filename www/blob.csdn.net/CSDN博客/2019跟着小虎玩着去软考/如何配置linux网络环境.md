# 如何配置linux网络环境 - 2019跟着小虎玩着去软考 - CSDN博客
2011年08月03日 17:32:33[littletigerat](https://me.csdn.net/littletigerat)阅读数：2277标签：[网络																[linux																[服务器																[vim](https://so.csdn.net/so/search/s.do?q=vim&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
个人分类：[Android																[Linux																[Java																[手机开发](https://blog.csdn.net/littletigerat/article/category/649495)](https://blog.csdn.net/littletigerat/article/category/781909)](https://blog.csdn.net/littletigerat/article/category/613914)](https://blog.csdn.net/littletigerat/article/category/613910)
**如何配置linux网络环境**
# 第一步：查看网络是否可以联通
**ping www.g.cn**
# 第二步：配置服务器信息
**vim /etc/resolv.conf **
**添加如下信息：**
**nameserver 211.166.66.88**
# 第三步：查看网络IP信息
**ifconfig**
# 第四步：配置网卡IP
**ifconfig eth0 192.168.13.21/15**
# 第五步：添加缺省网关
**route add default gw 192.168.0.1**
# 第六步：验证
**查看网络是否可以联通**
**ping  www.g.cn**
