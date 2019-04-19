# linux下su 与su -的区别-老男孩之永不止步-51CTO博客
Linux下su与su -命令的区别
在启动服务器ntpd服务时遇到一个问题
![linux下su 与su -的区别](https://s1.51cto.com/images/blog/201904/11/264ecc7517d7026dfb3764ecb43fdd87.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
使用 su root 切换到root用户后，不可以使用service命令；
使用 su - 后，就可以使用service命令了。
原因：
![linux下su 与su -的区别](https://s1.51cto.com/images/blog/201904/11/eb535fa5f9cc6eafb5fb224874493788.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
su命令和su -命令区别就是：
su只是切换了root身份，但Shell环境仍然是普通用户的Shell；而su -连用户和Shell环境一起切换成root身份了。只有切换了Shell环境才不会出现PATH环境变量错误，报command not found的错误。
su切换成root用户以后，pwd一下，发现工作目录仍然是普通用户的工作目录；而用su -命令切换以后，工作目录变成root的工作目录了。
用echo $PATH命令看一下su和su - 后的环境变量已经变了。
![linux下su 与su -的区别](https://s1.51cto.com/images/blog/201904/11/6aedc993e76f574f72eea403969bc90f.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
以此类推，要从当前用户切换到其它用户也一样，应该使用su -命令。
