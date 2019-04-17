# 新手如何正确使用CLion之输出hello world - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





以前只使用过vc6.0，在用过jetbrain的pycharm后就考虑换个c++的编译器了，第一还是考虑了vs2017但用惯了色彩鲜艳的jb产品后竟然有点不习惯vs，最后还是果断选择了jb的CLion。

鉴于本人也是个小白，连各种编译器的名字都不认识，安装CLion的时候走了很多坑，现在把这些坑记录填起来，造福后人。。

1.在官网上下载正版的CLion

2.注册账号使用（其实我是淘宝上买的账号，等我有钱了一定买正版）

3.在刚刚下载完的Clion上是没有编译器的，需要下载MinGW

![](https://images2017.cnblogs.com/blog/1196023/201710/1196023-20171012235546215-1766575748.png)

也就是这个东西，这个东西超级难装，动不动报错，我装了2天都没装好。终于在一位老哥的帖子上看到一个简单的方法。

http://blog.csdn.net/yanqueen2011/article/details/69808482

也就是利用dev c++中装好的编译器

![](https://images2017.cnblogs.com/blog/1196023/201710/1196023-20171012235850137-211952611.png)

确定文件的位置

4.

![](https://images2017.cnblogs.com/blog/1196023/201710/1196023-20171013000042527-716931654.png)

进入settings

![](https://images2017.cnblogs.com/blog/1196023/201710/1196023-20171013000134246-1835637185.png)

将路径设置为Dev c++中MinGW64的位置

点击ok

然后它会build一段时间，弹出来的窗口，点击右上角的绿箭头就ok了。

![](https://images2017.cnblogs.com/blog/1196023/201710/1196023-20171013000511184-1014239147.png)



 当你不小心把什么cmakelist不小心删掉了的话，重新new project一个就好了！

![](https://images2017.cnblogs.com/blog/1196023/201710/1196023-20171013000459184-776070843.png)



我已经迫不及待的用jetbrain的产品写题啦！













