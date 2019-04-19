# jmeter4.0 执行jmeter_server.bat报错 - _天枢 - 博客园
## [jmeter4.0 执行jmeter_server.bat报错](https://www.cnblogs.com/yhleng/p/9083329.html)
2018-05-24 15:57 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9083329)
**Jmeter分布式执行**
1.
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
Jmeter4.0  执行jmeter_server.bat   报错，是由于4.0要手工生成密钥
bin目录下有一个create-rmi-keystore.bat执行这个按提示生成密钥。
会在bin目录下生成一个rmi_keystore.jks文件。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
2.
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
jmeter.properties
server.rmi.ssl.disable ＝ true    否则远程执行，会报一个ssl错误。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
3.
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
Agent机：jmeter.properties设置本机ip:1099端口
controler机：jmeter.properties 设置Agent ip:1099
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
配置完成
