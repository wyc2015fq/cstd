# 桌面系统集成WEB认证系统方案 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月05日 12:03:43[左直拳](https://me.csdn.net/leftfist)阅读数：2069
个人分类：[node.js																[JavaScript																[Web																[.NET																[WPF](https://blog.csdn.net/leftfist/article/category/2441177)](https://blog.csdn.net/leftfist/article/category/94497)](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94500)](https://blog.csdn.net/leftfist/article/category/2626561)
最近做的一个项目，有WEB版、WPF版、手机版。领导想集成集团的一个现成的认证系统，姑且称这个认证系统名为 W4认证系统。
W4认证系统有如下特点：
1、现成的
2、是个单点登录系统
3、不支持oAuth2
4、是个WEB版的认证系统，严重依赖于cookie
WEB版还好，没有什么大的问题，但WPF版这些桌面程序呢，如何与WEB版的认证系统集成？
如果看百度、微信等的oAuth2接口，发现其中有个参数，名曰display，支持多种访问WEB、桌面等多种方式。因为没有研究过，网上也没有找到什么介绍资料，不知道具体是怎么样的。
我在项目中，设想是构建一个认证中介服务，接管客户端（包括WEB端、桌面端等）与W4认证系统之间的工作。对于客户端而言，并不知道有所谓W4系统。同样，对W4而言，亦不知那些客户端为何物。
桌面端具体登录过程设计如下：
0、桌面端是WPF版，认证中介是WEB版
1、桌面端先与认证中介服务建立连接，通过socket.io
2、建立连接后，触发事件，桌面端打开一个新窗口，里面是一个webbrowser，打开认证中介的页面
3、认证中介转向W4。转向前做相关准备
4、在W4中登录，成功后，认证中介向W4读取账户资料（W4有相关接口）
5、认证中介相关处理后，将账户资料及access_token通过连接发回到桌面端
6、桌面端收到资料，关闭连接，登录流程结束
