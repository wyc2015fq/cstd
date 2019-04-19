# eclipse中将web项目部署到tomcat - 左直拳的马桶_日用桶 - CSDN博客
2016年10月25日 19:56:36[左直拳](https://me.csdn.net/leftfist)阅读数：39491
eclipse中将web项目部署到tomcat.
myeclipse部署WEB项目到tomcat比较方便，但eclipse貌似默认是不会替你将web自动部署到tomcat下的。你Run as该web项目，选了tomcat作为服务器，浏览器中明明可以访问，但你跑到tomcat\webapps\下，却看不到项目的影子。它好像是被放在了某一个临时文件夹下了。
我run这个WEB项目，主要目的在于获得它的发布包。总不能拷贝源代码部署吧。所以如果能自动部署到tomcat，那么我们就能拿到这个部署包了。我是一个java小白，也不知道这样子理解对不对，先记下来。
那么，如何自动部署到tomcat？
**一、 在 Eclipse 中添加 Tomcat 服务器**
如果这一步已经做了，可忽略。 
![这里写图片描述](https://img-blog.csdn.net/20161106114445425)
注意要先停掉tomcat服务，否则run不起来。
**二、将 web 工程发布至 tomcat**
选择刚安装的Tomcat， 
![这里写图片描述](https://img-blog.csdn.net/20161106114608783)
顺利的话，应该能看到运行结果。通过浏览器访问也没问题。
**三、在 Servers 视图，Remove 刚刚发布的项目**
![这里写图片描述](https://img-blog.csdn.net/20161106114753976)
刚发布就剔除，有病？我的理解，主要是为了修改默认设置吧。
**四、在 Servers 视图，打开 Tomcat 服务器配置项，然后修改**
![这里写图片描述](https://img-blog.csdn.net/20161106120246963)
![这里写图片描述](https://img-blog.csdn.net/20161106120319994)
deploy path: webapps 表示将当前工程部署至 tomcat/webapps 目录下。
问题是，如何才能看到这个Servers视图？天下文章一大抄，我在网上寻寻觅觅，所有的参考资料都类似，且都没说如何切换到Servers视图。
经摸索，方法如下： 
点击eclipse菜单window - Show View，选Other 
![这里写图片描述](https://img-blog.csdn.net/20161106120634308)
![这里写图片描述](https://img-blog.csdn.net/20161106120806059)
如此，即可在eclipse的底部看到该视图矣： 
![这里写图片描述](https://img-blog.csdn.net/20161106120910763)
另外！ 
修改配置的时候，务必使 Tomcat 保持启动状态，否则 Server Locations 一栏会变灰色，导致不可更改。
但有时即使Tomcat保持启动，Server Locations 一栏也是灰的。这时候，要将tomcat\webapps\下，我们自己新加的所有项目（即非系统默认的项目）删掉，然后点击在SERVER视图，选中服务器，clean一下 
![这里写图片描述](https://img-blog.csdn.net/20161106121810469)
再打开配置进行修改，即可enable矣。
参考资料中还有 
配置 lib 输出（非必须） 
修改工程下的.classpath文件（非必须） 
但这个在我这个WEB项目里不是必须的，所以略过不提
**五、修改发布的目录名称**
我这个WEB项目，名叫“cas.server.web”，然后eclipse发布之后，自动将其目录命名为“cas.server.xxx”，这很奇怪，也不知道它为何要这样命名。所以访问的时候，路径类似这样：
`http://localhost:8080/cas.server.xxx/login`
但我其实希望是这样：
`http://localhost:8080/cas/login`
咋弄呢？我探索得到结果如下： 
1、在SERVER视图，Remove该项目（见步骤三）
2、在SERVER视图，选中服务器，clean一下 
![这里写图片描述](https://img-blog.csdn.net/20161106121810469)
3、修改项目下.settings\org.eclipse.wst.common.component文件
![这里写图片描述](https://img-blog.csdn.net/20161106122048161)
4、然后再run一下
参考资料： 
[配置 Eclipse 中 web 项目部署至 Tomcat 步骤](http://www.liuqianfei.com/article/2d8db6b67621428cb3a8691ccbce33b6)
妈的，好麻烦啊。
