# 使用pycharm调试django项目 - _天枢 - 博客园
## [使用pycharm调试django项目](https://www.cnblogs.com/yhleng/p/7837891.html)
2017-11-15 11:06 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7837891)
**要使用pycharm调试django 打断点调试后台代码，首先要进行一下配置：**
**1、debug 配置**
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171115105238156-1742072356.png)
**打开debug界面**
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171115105337749-830104911.png)
2、选择python点+加号，然后选择python
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171115105454187-793253379.png)
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171115105555343-840527623.png)
**3、名字debug，这个看你喜欢**
**脚本调manage.py  命令runserver**
**从这个可以看出，我们可以用同样的方法，将一些执行命令的，都做成一个debug**
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171115105638234-829616477.png)
4、上步保存后，我们回到pychram开发界面，点run,看到里边有我们刚刚创建的debug。名字上一步设置的。
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171115105925296-989369914.png)
** 5、直接点击，我们设置好的Debug '你设置的名字'**
从下图看出，执行了，我们设置的manage.py runserver命令。启动了web服务
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171115110040406-768104552.png)
**6、web服务起来之后，直接在后台代码上打断点吧，然后在web访问，直接到断点会停下。**
![](https://images2017.cnblogs.com/blog/1149221/201711/1149221-20171115110222077-729787540.png)
**远程调试Django项目配置如下:**
**1>runserver ip:端口号:**
**　　runserver 10.195.227.197:800**
**2>Script :**
**       选择你映射到本地的路径**
**3>Python interpreter:**
**       选择远程python**
![](https://images2017.cnblogs.com/blog/1149221/201802/1149221-20180207172047607-1307817538.png)
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,
