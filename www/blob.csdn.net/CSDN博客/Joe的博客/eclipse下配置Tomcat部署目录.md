# eclipse下配置Tomcat部署目录 - Joe的博客 - CSDN博客





2016年11月22日 08:28:31[Joe-Han](https://me.csdn.net/u010089444)阅读数：15834








# **1. Tomcat目录配置**

使用eclipse时，默认情况下Tocmat把我们部署的项目放在了workspaces下面，而不是像Myeclipse默认的那样放在tomcat安装目录所在的 webapps下。通过以下方式可查看项目的部署目录：右键点击tomcat服务器，依次点击Open → open launch configuration → Arguments，在VM arguments里面可看到项目被发布到了G:\workspace\J2EE.metadata.plugins\org.eclipse.wst.server.core\tmp0目录下


![这里写图片描述](https://img-blog.csdn.net/20161122091000313)


其实 G:\workspace\J2EE.metadata.plugins\org.eclipse.wst.server.core\tmp0这个目录就是eclipse对 Tomcat 安装目录的一个克隆，从而使 G:\workspace\J2EE.metadata.plugins\org.eclipse.wst.server.core\tmp0也能够具备源服务器的功能。如果再新建几个服务器，就会在 G:\workspace\J2EE.metadata.plugins\org.eclipse.wst.server.core\ 目录下依次出现temp0、temp1、temp2等多个克隆服务器，但是这里每次只能启动上面一个克隆服务器，因为他们都使用的是相同的启动端口。

这样会给我们带来很多的不方便。举个例子：就上述工程而言，当我们在进行开发的时候，项目需要将上传的图片放入到工程的同级目录的upload文件夹的时候，会发现图片是上传到了所在的目录 G:\workspace\J2EE.metadata.plugins\org.eclipse.wst.server.core\tmp0\wtpwebapps\upload\ ，但是无法在浏览器中访问到上传的图片。这时候我们可以手动将该upload目录整个复制到 G:\workspace\J2EE.metadata.plugins\org.eclipse.wst.server.core\tmp0\webapps\ ，这时在浏览器中的确就可以访问了。造成这种现象的原因是tomcat服务器默认webapps为工程目录，而不是 wtpwebapps 目录。

**可通过以下方式：**

双击服务器，修改Server Locations选项为**Use Tomcat installation(takes control of Tomcat installation) **，然后将Deploy path改为**webapps**
![这里写图片描述](https://img-blog.csdn.net/20161122091935145)


点击open launch configuration → Arguments，将Working directory修改为tomcat下的bin目录。 

![这里写图片描述](https://img-blog.csdn.net/20161122093328369)


# **2. 解决配置选项不能修改**

双击tomca进入设置界面，如果看到此时Service Locations下各个选项都是处于灰色的不可编辑的状态，则可以通过以下方式恢复为可编辑状态： 

![这里写图片描述](https://img-blog.csdn.net/20161122132435196)

第一种最直接的办法就是删除原有的服务器，重新新建一个，并且先不要添加任何工程进去。 

第二种是先把tomcat下的所有项目删除，然后右击，选择clean。再次双击tomcat进入设置界面，此时可看到Service Locations下各个选项已处于可编辑状态了。 
![这里写图片描述](https://img-blog.csdn.net/20161122132741856)


# **3. 参考资料**
- [http://kingxss.iteye.com/blog/1741438/](http://kingxss.iteye.com/blog/1741438/)
- [http://blog.sina.com.cn/s/blog_804c1dce0102wxyd.html](http://blog.sina.com.cn/s/blog_804c1dce0102wxyd.html)




