# Eclipse报错：Setting property 'source' to 'org.eclipse.jst.jee.server:test1' did no - z69183787的专栏 - CSDN博客
2014年02月25日 14:17:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：24954
最近把Eclipse的maven插件从m2eclipse更新到m2e后出了一些莫名其妙的的问题。今天又出了一个，就是Eclipse新建的Maven Web project在tomcat里启动后报错，具体报错信息如下：
Shell代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- 警告: [SetPropertiesRule]{Server/Service/Engine/Host/Context} Setting property 'source' to 'org.eclipse.jst.jee.server:test1' did not find a matching property.  
对于这个问题，网上很多人给出的原因和解决方案如下：
写道
出现SetPropertiesRule警告的原因是因为Tomcat在server.xml的Context节点中不支持source属性：<Context docBase="…" path="/…" source="org.eclipse.jst.j2ee.server:…"/>
解决方法是在Servers视图里双击创建的server，然后在其server的配置界面中选中"Publish module contexts to separate XML files"选项。
不过我按这种方式操作后发现还是会报这个问题，除了这种方式之外网上也没有什么参考资料了，这个问题纠结了我几天。仔细检查工程结构后发现我的工程根目录下生成了一个WebContent文件夹。具体截图如下：
![](http://dl.iteye.com/upload/picture/pic/124389/6e7ec95f-9b5f-34f6-b2fc-e804539950ff.jpg)
一般来说正常的maven项目的web资源文件都是默认位于src/main/webapp下，WebContent只是普通的Dynamic Web project使用的默认Web文件目录。而我这个工程竟然同时生成了这两个目录，而且两个目录下都有WEB-INF文件夹。这个是不是问题的根源呢？
为了排查，我又重新新建了一个maven web project，工程名为test2。新建完后发现并没有生成WebContent文件夹。我们都知道，Eclipse中建Maven Web工程需要添加Project Facets。具体步骤如下图所示：
1.右键--Properties
![](http://dl.iteye.com/upload/picture/pic/124381/94ec9e3c-1a2e-3a03-9475-0e3f02e60356.jpg)
2.选择Project Facets，然后点击右边的Convert to faceted from...
![](http://dl.iteye.com/upload/picture/pic/124383/37b1c6fc-4cf3-3577-9b83-97f187e87046.jpg)
3.在弹出的窗口中选择Dynamic Web Module
![](http://dl.iteye.com/upload/picture/pic/124385/d31d9327-13da-35a0-ad2a-4ab2698ccbd3.jpg)
4.如上图所示在窗口下方还有一个Further configuration available...，点击后弹窗如下，打开完后终于找到了原因所在，在这一步需要设置工程的content directory，如果不设置，目录名称默认就是WebContent。
![](http://dl.iteye.com/upload/picture/pic/124387/e8ca63aa-f3f6-307c-95f9-3eb234014297.jpg)
前一步我是没有设置，所以才会生成默认的WebContent目录。
添加完Facets后，把test2工程部署在tomcat启动，发现之前的错误已经不存在了。
解决完问题后，心里还是有点不踏实，问题的根源到底是什么呢。逐一比对了两个工程的文件内容后终于发现了区别所在。区别在于工程.settings文件夹下。.settings目录下的文件附图如下：
![](http://dl.iteye.com/upload/picture/pic/124391/459209e6-ce8d-3b6a-847b-87a8cb3b3055.jpg)
上图高亮的部分的两个文件就是区别所在。
其中.jsdtscope中有一行定义：
Xml代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- <classpathentrykind="src"path="WebContent"/>
 而test2工程的定义如下：
Xml代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- <classpathentrykind="src"path="src/main/webapp"/>
另外org.eclipse.wst.common.component文件中定义了工程的wb-resource
Xml代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- <wb-resourcedeploy-path="/"source-path="/WebContent"tag="defaultRootSource"/>
test2工程的定义如下：
Xml代码  ![收藏代码](http://chenzhou123520.iteye.com/images/icon_star.png)
- <wb-resourcedeploy-path="/"source-path="/src/main/webapp"tag="defaultRootSource"/>
综上所述，推测tomcat在部署工程时会去查找.settings中定义的配置。如果配置有问题，则会报错。上面的问题，只要把test1工程.settings目录下对应的两个文件的配置修改后重新部署就能正常启动了。
