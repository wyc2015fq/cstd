# javaweb+tomcat实现通过ip访问项目 - xqhrs232的专栏 - CSDN博客
2018年11月30日 15:07:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：98
原文地址::[https://blog.csdn.net/lost_wen/article/details/54584779](https://blog.csdn.net/lost_wen/article/details/54584779)
相关文章
1、javaweb项目只输入ip就可访问无需输入端口号和项目名的实现方法----[https://blog.csdn.net/qq_27843945/article/details/75105786](https://blog.csdn.net/qq_27843945/article/details/75105786)
2、java web项目如何限制访问的ip访问地址----[https://blog.csdn.net/junehappylove/article/details/80566987](https://blog.csdn.net/junehappylove/article/details/80566987)
3、怎样发布java web项目，直接输入IP就可以访问网页，而不需要在IP后加项目名。----[https://zhidao.baidu.com/question/550474011.html](https://zhidao.baidu.com/question/550474011.html)
4、配置Tomcat 输入ip可以直接访问项目的方法----[https://blog.csdn.net/wangl2014/article/details/79082076/](https://blog.csdn.net/wangl2014/article/details/79082076/)
url="10.43.147.180:8080"
1）设置1、Open /webapp/ROOT/WEB-INF/web.xml and remove servlet mapping with path /index.jsp and save.
2、Open your /webapp/ROOT/index.jsp and add this line:
response.sendRedirect("../NutchManage/"); 
url="10.43.147.180"
2）配置虚拟目录：
1、将tomcat连接端口号设置为80，因为TCP/IP协议默认80端口访问
2、修改host节点<Host name="localhost"  appBase="/usr/local/dawen/web" unpackWARs="true" autoDeploy="true">
将war包置于appBase目录下后将自动解压，而不是再放在默认的tomcat的webapps下
3、在</Host>之前添加<Context path="" docBase="/usr/local/dawen/web/NutchManage" debug="0" reloadable="true" />
path为空表示直接访问ip+port可访问到docBase所指定的项目，
例如地址栏输入“10.43.147.180”即可访问项目“NutchManage”（端口号80可以不写）
*此时目录“/usr/local/dawen/web”即为虚拟目录*
4、重启tomcat，若已开启先关闭，即时查看tomcat日志，若报端口占用错误，查明占用端口是否可以杀掉，之后再重启，即可通过ip访问了
--------------------- 
作者：lost_wen 
来源：CSDN 
原文：https://blog.csdn.net/lost_wen/article/details/54584779 
版权声明：本文为博主原创文章，转载请附上博文链接！
