# 局域网内PC通过笔记本共享上网 - 左直拳的马桶_日用桶 - CSDN博客
2015年07月29日 19:06:40[左直拳](https://me.csdn.net/leftfist)阅读数：3447标签：[局域网																[共享上网](https://so.csdn.net/so/search/s.do?q=共享上网&t=blog)](https://so.csdn.net/so/search/s.do?q=局域网&t=blog)
个人分类：[应用](https://blog.csdn.net/leftfist/article/category/2778939)
现实：PC、笔记本都通过网线接在局域网内，局域网无法上网；笔记本有无线网卡，可连WIFI上网。
现在想让PC通过笔记本来共享上网。
步骤： 
1、笔记本开启DHCP。方法是开启”服务“里的dhcp client。
2、打开笔记本的网络和共享中心-更改适配器配置，右击无线网络，属性，打开共享页签，勾上允许其他网络用户通过此计算机的internet连接来连接
3、这时系统会提示笔记本的IP变为多少
4、在PC中，将网关设为笔记本的IP，并设为自动获取IP，也许需要重启
如此大事可成，爽歪歪。
