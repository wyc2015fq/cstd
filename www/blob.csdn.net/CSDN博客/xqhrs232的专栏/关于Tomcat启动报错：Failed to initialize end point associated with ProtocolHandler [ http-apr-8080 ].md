# 关于Tomcat启动报错：Failed to initialize end point associated with ProtocolHandler ["http-apr-8080"] - xqhrs232的专栏 - CSDN博客
2018年09月29日 15:23:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：119
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/jacy_lee_ldf/article/details/53045815](https://blog.csdn.net/jacy_lee_ldf/article/details/53045815)
相关文章
1、tomcat后台启动成功，但是前台服务器页面无法访问的问题----[https://blog.csdn.net/neolulu1987/article/details/78316343](https://blog.csdn.net/neolulu1987/article/details/78316343)
这几天Tomcat启动老是遇见这个错 ： Failed to initialize end point associated with ProtocolHandler ["http-apr-8080"]，看来是非要我记住这个教训不可呀。
一般是 Tomcat 的默认端口被占用，或者 Tomcat 进程没有关掉。（这个可以通过任务管理器查看一下，如果发现 Tomcat 进程开着，那就结束它再启动 Tomcat 就可以了。）
解决办法：
1.先查看占用此端口的玩意儿的PID
运行 cmd ，命令行输入：netstat -ano|findstr 8080（这里我的Tomcat 的端口是使用的默认的 8080），会发现有个占用了 8080 的 PID
2. 然后运行：taskkill /f /pid PID（就是上一条命令查出来的PID），回车，当提示终止成功，说明可以了，现在再重新启动 Tomcat 应该就没有问题了。
//===============================================
本人备注::
1>直接去tomcat bin目录下单起，如果可以起起来，那就看tomcat服务配置到后台服务时哪里出错了。
