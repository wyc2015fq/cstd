# 更改Eclipse下Tomcat的默认自动部署目录 - z69183787的专栏 - CSDN博客
2014年03月27日 20:58:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12020
使用eclipse开发是因为机器不够用myeclipse，eclipse也比myeclipse清爽很多，启动速度也快。这里的搭建开发环境使用： Jdk1.6+Tomcat6+Eclipse JEE， 工作目录如下环境目录如下：
       安装路径：
       C:\Java\Jdk1.6.0
       C:\Java\Jre1.6.0
       D:\Tomcat 6.0
       D:\workSpace
       D:\Eclipse
      配置eclipse的开发环境，配置jdk的安装路径和tomcat安装路径。在eclipse下建立Dynamic Web Project工程zhgy，在使用eclipse中new一个tomcat，通过启动该tomcat来发布Dynamic Web Project的时候，其实并未将工程发布到tomcat安装目录所在的webapps下。这点可以去上述的tomcat安装目录的webapps目录下查看。从启动时候的控制台输出来看项目是被发布到了如下的目录：
信息: Set web app root system property: 'webapp.root' = [D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp0\wtpwebapps\zhgy\]
打开该目录可以很清楚的看到存在zhgy这样一个文件夹，这就是我们现在可以访问的项目目录。
再打开D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp0\   这个目录，可以看到这个目录下的结构和D:\Tomcat
 6.0的目录结构是一模一样的，只是多了个wtpwebapps目录。其实D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp0\ 这个目录就是eclipse的对D:\Tomcat
 6.0 目录的一个克隆，从而使D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp0\ 也能够具备源服务器的功能。
      如果再new几个服务器，就会在D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\目录下依次出现temp0、temp1、temp2等多个克隆服务器，但是这里每次只能启动上面一个克隆服务器，因为他们都使用的是相同的启动端口（当然还有相同的关闭端口等）。
      这样会给我们带来很多的不方便。举个例子：就上述工程而言，当我们在进行开发的时候，项目需要将上传的图片放入到工程的同级目录的upload文件夹的时候，会发现图片是上传到了所在的目录D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp0\wtpwebapps\upload\，但是无法在浏览器中访问到上传的图片。这时候我们可以手动将该upload目录整个复制到D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp0\webapps\，这时在浏览器中的确就可以访问了。造成这种现象的原因是tomcat服务器默认webapps为工程目录，而不是wtpwebapps目录。之所能够通过浏览器访问D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp0\wtpwebapps\zhgy\下的zhgy这个项目，是由于eclipse通过tomcat发布项目的时候在D:\workSpace-jx\.metadata\.plugins\org.eclipse.wst.server.core\tmp3\conf目录的server.xml文件中有如下的设置：
Xml代码  ![收藏代码](http://kingxss.iteye.com/images/icon_star.png)
- <ContextdocBase="D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp3\wtpwebapps\zhgy"path="/zhgy"reloadable="true"source="org.eclipse.jst.jee.server:zhgy"/>
这一点我们可以通过在查看eclipse中新建的tomcat属性来了解，如下图中Server Locations中所示：
![](http://dl.iteye.com/upload/picture/pic/121483/74c9a9ba-26d8-3d5e-8e1c-6973835e4240.jpg)
手动拷贝虽然可以解决问题，但是确实不方便。但是上图中的Server Locations是灰色的，不能更改。其实eclipse新建tomcat克隆的时候是可以更改的，只是在new这个tomcat的时候不要添加任何工程进去，再在tomcat上点击右键open：
![](http://dl.iteye.com/upload/picture/pic/121485/9d1bbfe2-77e5-37cb-80bc-479adfdf195e.jpg)
就可以看到Server Locations选项不再是灰色，是可以编辑的了：
![](http://dl.iteye.com/upload/picture/pic/121487/33a1f69a-41b2-3307-962f-7e4edb72dfa7.jpg)
这里有三种可以选择：
1.使用eclipse的工作空间，以上述为例就是D:\workSpace\.metadata\.plugins\org.eclipse.wst.server.core\tmp0\
2.使用tomcat的安装目录，以上述为例就是D:\tomcat\
3.自定义路径，这里就是自己选择目录了
还可以通过修改Deploy path来定义工程到底部署到容器的哪个目录下。例如下图中，我们就可以选着Use Tomcat Installtion 这种方式，并且设置Deploy
 path为 webapps:
![](http://dl.iteye.com/upload/picture/pic/121489/c3c04b50-ef30-394a-8d01-3a6411b60122.jpg)
其实，最简单的方法是修改VM arguments的-Dwtp.deploy属性。该属性位置为：eclipse菜单栏Run -> Run Configurations -> Arguments -> VM arguments.
