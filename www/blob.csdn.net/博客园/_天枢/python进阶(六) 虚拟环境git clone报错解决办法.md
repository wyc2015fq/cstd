# python进阶(六) 虚拟环境git clone报错解决办法 - _天枢 - 博客园
## [python进阶(六) 虚拟环境git clone报错解决办法](https://www.cnblogs.com/yhleng/p/8479186.html)
2018-02-27 15:37 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8479186)
**在虚拟环境目录，进行git clone xxxxx.git  项目，报如下图错误**
![](https://images2018.cnblogs.com/blog/1149221/201802/1149221-20180227153624070-975548795.png)
**解决办法：env GIT_SSL_NO_VERIFY=true git clone xxxx.git**
![](https://images2018.cnblogs.com/blog/1149221/201802/1149221-20180227153509110-1663473245.png)
