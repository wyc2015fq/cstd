# myeclipse中maven3的使用，部署web项目到tomcat7 - z69183787的专栏 - CSDN博客
2014年03月26日 15:06:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5891
myeclipse中需要集成maven3,具体操作可以参看这篇文章
[http://www.cnblogs.com/fancyzero/archive/2012/06/09/maven3.html](http://www.cnblogs.com/fancyzero/archive/2012/06/09/maven3.html)
这里就不多加描述
在创建web工程前，咱们先创建一个普通的maven工程，后面的web工程将会使用到这个工程的jar包
1.普通maven工程创建过程如下，右键>>new Maven Project>>next
![](http://static.oschina.net/uploads/space/2013/0731/093749_aoU2_815996.png)
输入对应的groupId,artifactId及包名
![](http://static.oschina.net/uploads/space/2013/0731/095636_TkTr_815996.png)
生成的文件目录结构如下
![](http://static.oschina.net/uploads/space/2013/0731/095703_1Sqy_815996.png)
修改App.java
|`01`|`package``com.johnmy.study;`|
|`02`||
|`03`|`/**`|
|`04`|` ``*`|
|`05`|` ````* @author JohnmyWork```|
|`06`|` ````* @date 2013-7-30```|
|`07`|` ``*/`|
|`08`|`public``class````App {```|
|`09`|`    ``public``static````String handleString(String str) {```|
|`10`|```````return``"handled:"````+ str;```|
|`11`|`    ``}`|
|`12`|`}`|
保存，执行install
![](http://static.oschina.net/uploads/space/2013/0731/100021_20MD_815996.png)
此时会在本地库中安装mvnapp的资源内容，下面的web工程可以直接从maven库中读取到资源
![](http://static.oschina.net/uploads/space/2013/0731/100245_H91H_815996.png)
其实，还有一种更高效的方法，可以在dos窗口中使用mvn命令快速创建普通工程，再导入到myeclipse中编辑
![](http://static.oschina.net/uploads/space/2013/0731/100732_7dO6_815996.png)
-----------------------------------------------------------------------------------------------------------------
现在开始创建web工程
同样new maven project >> next
![](http://static.oschina.net/uploads/space/2013/0731/100912_Mluz_815996.png)
输入相应信息
![](http://static.oschina.net/uploads/space/2013/0731/101101_plgg_815996.png)
生成的目录结构如下
![](http://static.oschina.net/uploads/space/2013/0731/101217_GJpo_815996.png)
修改目录结构
![](http://static.oschina.net/uploads/space/2013/0731/101318_VH6J_815996.png)
![](http://static.oschina.net/uploads/space/2013/0731/101333_NH6R_815996.png)
修改之后如下
![](http://static.oschina.net/uploads/space/2013/0731/101354_UAvH_815996.png)
向工程中添加资源包
![](http://static.oschina.net/uploads/space/2013/0731/101644_zGwj_815996.png)
![](http://static.oschina.net/uploads/space/2013/0731/101735_2RZm_815996.png)
也可以直接修改pom文件,这里引入了之前创建的普通工程
|`01`|`    ``<``dependencies``>`|
|`02`|```````<``dependency``>`|
|`03`|```````<``groupId``>junit</``groupId``>`|
|`04`|```````<``artifactId``>junit</``artifactId``>`|
|`05`|```````<``version``>3.8.1</``version``>`|
|`06`|```````<``scope``>test</``scope``>`|
|`07`|```````</``dependency``>`|
|`08`||
|`09`||
|`10`|```````<``dependency``>`|
|`11`|```````<``groupId``>javax.servlet</``groupId``>`|
|`12`|```````<``artifactId``>servlet-api</``artifactId``>`|
|`13`|```````<``version``>2.4</``version``>`|
|`14`|```````</``dependency``>`|
|`15`||
|`16`|```````<``dependency``>`|
|`17`|```````<``groupId``>com.johnmy.study</``groupId``>`|
|`18`|```````<``artifactId``>mvnapp</``artifactId``>`|
|`19`|```````<``version``>1.0-SNAPSHOT</``version``>`|
|`20`|```````</``dependency``>`|
|`21`||
|`22`|`    ``</``dependencies``>`|
新建Myservlet.java并放到study包中
![](http://static.oschina.net/uploads/space/2013/0731/102500_6Jdt_815996.png)
Myservlet.java
|`01`|`package``study;`|
|`02`||
|`03`|`import``java.io.IOException;`|
|`04`|`import``java.io.Writer;`|
|`05`||
|`06`|`import``javax.servlet.ServletException;`|
|`07`|`import``javax.servlet.http.HttpServlet;`|
|`08`|`import``javax.servlet.http.HttpServletRequest;`|
|`09`|`import``javax.servlet.http.HttpServletResponse;`|
|`10`||
|`11`|`import``com.johnmy.study.App;`|
|`12`||
|`13`|`/**`|
|`14`|` ``*`|
|`15`|` ````* @author JohnmyWork```|
|`16`|` ````* @date 2013-7-31```|
|`17`|` ``*/`|
|`18`|`public``class``Myservlet ``extends````HttpServlet {```|
|`19`||
|`20`|`    ``@Override`|
|`21`|`    ``protected``void````doPost(HttpServletRequest req, HttpServletResponse resp)````throws````ServletException, IOException {```|
|`22`|`        ``super````.doGet(req, resp);```|
|`23`|`    ``}`|
|`24`||
|`25`|`    ``@Override`|
|`26`|`    ``protected``void````doGet(HttpServletRequest req, HttpServletResponse resp)````throws````ServletException, IOException {```|
|`27`|`        ````String input = req.getParameter(````"name"``);`|
|`28`|`        ````Writer out = resp.getWriter();```|
|`29`|`        ``out.append(App.handleString(input));`|
|`30`|`    ``}`|
|`31`||
|`32`|`    ``private``static``final``long````serialVersionUID = 1L;```|
|`33`||
|`34`|`}`|
修改web.xml,及index.jsp
![](http://static.oschina.net/uploads/space/2013/0731/103009_oICm_815996.png)
web.xml
|`01`|```<!DOCTYPE web-app PUBLIC```|
|`02`|` ````"-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"```|
|`03`|` ````"http://java.sun.com/dtd/web-app_2_3.dtd" >```|
|`04`||
|`05`|`<``web-app``>`|
|`06`|`    ``<``display-name````>Archetype Created Web Application</````display-name``>`|
|`07`|`    ``<``welcome-file-list``>`|
|`08`|`        ``<``welcome-file``>index.jsp</``welcome-file``>`|
|`09`|`    ``</``welcome-file-list``>`|
|`10`||
|`11`|`    ``<``servlet``>`|
|`12`|`        ``<``servlet-name``>Myservlet</``servlet-name``>`|
|`13`|`        ``<``servlet-class``>study.Myservlet</``servlet-class``>`|
|`14`|`    ``</``servlet``>`|
|`15`|`    ``<``servlet-mapping``>`|
|`16`|`        ``<``servlet-name``>Myservlet</``servlet-name``>`|
|`17`|`        ``<``url-pattern``>/myservlet</``url-pattern``>`|
|`18`|`    ``</``servlet-mapping``>`|
|`19`|`</``web-app``>`|
index.jsp
|`01`|```<!doctype html>```|
|`02`|`<``html``lang``=``"en"``>`|
|`03`|`<``head``>`|
|`04`|`<``meta``charset``=``"UTF-8"``/>`|
|`05`|`<``title````>hello maven!</````title``>`|
|`06`|`</``head``>`|
|`07`|`<``body``>`|
|`08`|`    ``<``form``action``=``"myservlet"``method``=``"get"``>`|
|`09`|`        ``name:<``input``name``=``"name"````> <````input``type``=``"submit"``>`|
|`10`|`    ``</``form``>`|
|`11`|`</``body``>`|
|`12`|`</``html``>`|
差不多了，执行下maven test，maven install看下有木有问题
 BUILD SUCCESS ！！一切正常
下面就把项目部署到tomcat7中
部署之前先修改下tomcat的配置文件
D:\WebContainer\3-tomcat7\apache-tomcat-7.0.39\conf\tomcat-users.xml
|`1`|`<?``xml``version``=``'1.0'``encoding``=``'utf-8'``?>`|
|`2`|`<``tomcat-users``>`|
|`3`|` ``<``role``rolename``=``"manager-gui"``/>`|
|`4`|` ``<``role``rolename``=``"manager"``/>`|
|`5`|` ``<``role``rolename``=``"manager-script"``/>`|
|`6`|` ``<``user``username``=``"tomcat"``password``=``"tomcat"``roles``=``"manager-gui,manager,manager-script"``/>`|
|`7`|`</``tomcat-users``>`|
还有修改下maven3的配置文件
D:\Program Files\MavenServer\maven3\conf\settings.xml
找到servers修改之
|`1`|`<``servers``>`|
|`2`|`   ``<``server``>`|
|`3`|`     ``<``id``>mytomcat7</``id``>`|
|`4`|`     ``<``username``>tomcat</``username``>`|
|`5`|`     ``<``password``>tomcat</``password``>`|
|`6`|`   ``</``server``>`|
|`7`|` ``</``servers``>`|
外部配置好了，现在配置下web工程中的pom，添加tomcat7-maven-plugin
|`01`|`    ``<``build``>`|
|`02`|```````<``finalName``>mvnwebapp</``finalName``>`|
|`03`|```````<``plugins``>`|
|`04`|```````<``plugin``>`|
|`05`|```````<``groupId``>org.apache.tomcat.maven</``groupId``>`|
|`06`|```````<``artifactId``>tomcat7-maven-plugin</``artifactId``>`|
|`07`|```````<``version``>2.2-SNAPSHOT</``version``>`|
|`08`|```````<``configuration``>`|
|`09`|```````<``path``>/${project.build.finalName}</``path``>`|
|`10`|```````<``server``>mytomcat7</``server``>`|
|`11`|`                    ````<!-- 这里是本地tomcat，如果是远程服务器可以改成对应的地址，实现自动部署-->```|
|`12`|```````<``url``>http://localhost:8080/manager/text</``url``>`|
|`13`|```````</``configuration``>`|
|`14`|```````</``plugin``>`|
|`15`|```````</``plugins``>`|
|`16`|`    ``</``build``>`|
添加插件仓库配置，下面这两个任选其一或者两个都用
|`01`|`<``repositories``>`|
|`02`|`    ``<``repository``>`|
|`03`|`        ``<``id``>apache.snapshots</``id``>`|
|`04`|`        ``<``url``>https://repository.apache.org/content/repositories/snapshots</``url``>`|
|`05`|`        ``<``releases``>`|
|`06`|`            ``<``enabled``>false</``enabled``>`|
|`07`|`        ``</``releases``>`|
|`08`|`        ``<``snapshots``>`|
|`09`|`            ``<``enabled``>true</``enabled``>`|
|`10`|`        ``</``snapshots``>`|
|`11`|`    ``</``repository``>`|
|`12`|`</``repositories``>`|
|`01`|`<``pluginRepositories``>`|
|`02`|`        ``<``pluginRepository``>`|
|`03`|`            ``<``id``>apache.snapshots</``id``>`|
|`04`|`            ``<``name````>Apache Snapshots</````name``>`|
|`05`|`            ``<``url``>https://repository.apache.org/content/repositories/snapshots</``url``>`|
|`06`|`            ``<``releases``>`|
|`07`|`                ``<``enabled``>false</``enabled``>`|
|`08`|`            ``</``releases``>`|
|`09`|`            ``<``snapshots``>`|
|`10`|`                ``<``enabled``>true</``enabled``>`|
|`11`|`            ``</``snapshots``>`|
|`12`|`        ``</``pluginRepository``>`|
|`13`|`    ``</``pluginRepositories``>`|
启动tomcat7,执行maven build
![](http://static.oschina.net/uploads/space/2013/0731/111117_2gjB_815996.png)
配置goals
![](http://static.oschina.net/uploads/space/2013/0731/111157_RBKn_815996.png)
run
![](http://static.oschina.net/uploads/space/2013/0731/111509_yMyD_815996.png)
访问[http://localhost:8080/mvnwebapp/](http://localhost:8080/mvnwebapp/)
![](http://static.oschina.net/uploads/space/2013/0731/113609_kJnF_815996.png)
提交后
![](http://static.oschina.net/uploads/space/2013/0731/113626_JRfP_815996.png)
成功！！！！！！
调用了普通工程的jar中的handleString()方法
如果普通工程的jar有更新并重新发布到tomcat中
只需要简单的maven install，maven bulid
修改handleString方法
|`1`|`public``static````String handleString(String str) {```|
|`2`|`        ``return````"nice!!! you handled it:"``````+ str+````"\n"``+``new``Date().toString();`|
|`3`|`    ``}`|
在mvnapp中执行maven install
在mvnwebapp中执行maven bulid
