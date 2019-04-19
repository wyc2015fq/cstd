# cygwin,cdt,eclipse搭建c,cpp环境 - xqhrs232的专栏 - CSDN博客
2011年06月18日 22:40:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：848标签：[eclipse																[c																[application																[properties																[google																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://yuanzhifei89.iteye.com/blog/1062721](http://yuanzhifei89.iteye.com/blog/1062721)
今天闲着无聊，突然想到上次eclipse装了cdt插件，于是准备在eclipse下把c,cpp的开发环境搭建一下，c在大一的时候学过，后来就一直用java，c基本忘得也差不多了，不过不妨碍搭建开发环境。
主要参考了以下的一些教程： 
cygwin的安装：[http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300292.aspx](http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300292.aspx)
环境的搭建：[http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300638.aspx](http://blog.csdn.net/jianping_shen/archive/2011/04/03/6300638.aspx)
launch failed binary not found错误的解决：[http://blog.sina.com.cn/s/blog_58844e710100irt2.html](http://blog.sina.com.cn/s/blog_58844e710100irt2.html)
java开发环境的搭建就不再这边介绍了，google一下，网上很多
1：首先是安装cdt插件
![](http://dl.iteye.com/upload/attachment/489233/229200b5-52bf-39cf-98f7-deab01d37330.png)
2：安装完后，按照上面提供的cygwin安装教程，把cygwin安装好。然后才能在这边配置cygwin
![](http://dl.iteye.com/upload/attachment/489239/1c267bc3-220c-3ab2-b6ee-2668f37de785.png)
3：配置完后，基本就可以创建一个cpp工程来测试一下了
![](http://dl.iteye.com/upload/attachment/489243/68436e3b-d095-3d2d-808d-6aadbb5f6ad8.png)
4：工程创建好后，我们还需要在工程上 -> properties来配置一些内容
![](http://dl.iteye.com/upload/attachment/489237/d765194f-ac0e-3066-b7c5-33863e0758eb.png)
![](http://dl.iteye.com/upload/attachment/489235/76e000ba-274d-3810-92f8-c8acd576c9c1.png)
5：创建完成后，首先你需要编译整个项目
![](http://dl.iteye.com/upload/attachment/489241/c4900051-da80-3825-8509-3ff34a1cf663.png)
6：编译成功会在控制台显示下面的内容
![](http://dl.iteye.com/upload/attachment/489245/dedebe8d-e409-386f-b625-92dfb592c501.png)
7：然后，run as -> local c/c++ application，首先会让我们选择调试器，选择后就会在控制台显示hello world
![](http://dl.iteye.com/upload/attachment/489247/9ec36874-1502-36d0-9b8e-87b57a011d0d.png)
![](http://dl.iteye.com/upload/attachment/489251/c8c6bfe0-cd59-3ed2-898a-8f27b7745ad7.png)
8：如果没有编译就直接run as -> local c/c++ application就会出现一下错误
![](http://dl.iteye.com/upload/attachment/489249/6a535926-3129-3f40-84ee-736be8e037cc.png)

