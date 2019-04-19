# windows SSH Tunnel实施日记 - =朝晖= - 博客园
# [windows SSH Tunnel实施日记](https://www.cnblogs.com/dhcn/p/8029800.html)
      1、准备条件：SSH跳板服务器一个、软件：Putty，CCProxy
      2、putty建立SSH Tunnel:先在session那儿把服务器地址填好，到Tunnel界面上，选Dynamics和Auto，填好本地port号，然后点Add添加，然后点击open，登录以后SSH Tunnel即成功
      3、利用CCProxy把SSH Tunnel的Socks代理转成各种协议，这个地方的要点是二级代理的地址要填127.0.0.1，如果填实际地址可能不行。
      4、浏览器里面设置代理
参考http://www.jb51.net/softjc/34833.html 

