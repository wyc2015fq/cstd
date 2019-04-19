# Jenkins自动化构建(二)众多问题 - _天枢 - 博客园
## [Jenkins自动化构建(二)众多问题](https://www.cnblogs.com/yhleng/p/7594892.html)
2017-09-26 08:59 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7594892)
**1、反向代理设置有误**
**系统管理页面提示，反向代理设置有误，其实这是由于URL设置有问题；**
**打开系统管理-->系统设置-->URL**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170926085630760-1149022895.png)
Jenkins URL设置有问题，改一下，去掉jenkins；http://127.0.0.1:8080
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170926085709839-1152746525.png)
去掉jenkins问题解决。
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170926085821932-219648402.png)
