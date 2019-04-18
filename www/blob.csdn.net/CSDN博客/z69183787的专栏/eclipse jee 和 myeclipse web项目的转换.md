# eclipse jee 和 myeclipse web项目的转换 - z69183787的专栏 - CSDN博客
2014年02月28日 16:33:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1144
个人分类：[IDE-Eclipse](https://blog.csdn.net/z69183787/article/category/2176833)

今天受到一个同事的启发，他一直在javaee的开发中，使用纯eclipse，虽然麻烦，但是也有好处，小巧免费。
于是，自己下了一个eclipse jee来部署一下开发环境，下面记录了eclipse jee 和 myeclipse web工程项目的转换：
eclipse版本：eclipse-jee-kepler-SR1-win32.zip  大小：247MB
myeclipse版本：MyEclipse_Blue_6.5M1.exe  大小：497MB，如果是myeclipse 10的话，估计得900多MB了。
JDK是1.6的，tomcat是apache-tomcat-6.0.30
首先：eclipse jee是免费的软件，不用担心注册号，版权的问题。而且轻巧，占内存少。只是很多功能不如myeclipse完整。需要很多地方手工配置。
myeclipse是收费的软件，不过很多人都用破解的。myeclipse很庞大，功能全，集成的东西也多。只是很耗电脑的资源。
eclipse jee 和 myeclipse 都可以用来做javaee的开发，而且工程可以在这两个IDE中，相互切换部署都可以，至于要用哪个，可以看个人的实际情况而定。再复杂的IDE，用熟了，都不复杂了。
1、首先是myeclipse下面的工程，转换部署到eclipse下面去：
主要是修改.project文件：把这三句话加在.project文件的<natures></natures>节点中
**[plain]**[view
 plain](http://blog.csdn.net/zyujie/article/details/16845587#)[copy](http://blog.csdn.net/zyujie/article/details/16845587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/74726/fork)
- <nature>org.eclipse.wst.common.project.facet.core.nature</nature>   
- <nature>org.eclipse.wst.common.modulecore.ModuleCoreNature</nature>   
- <nature>org.eclipse.jem.workbench.JavaEMFNature</nature>  
把下面这段加入到.project文件的<buildSpec></buildSpec>中
**[plain]**[view
 plain](http://blog.csdn.net/zyujie/article/details/16845587#)[copy](http://blog.csdn.net/zyujie/article/details/16845587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/74726/fork)
- <buildCommand>   
-     <name>org.eclipse.wst.common.project.facet.core.builder</name>   
-     <arguments>   
-     </arguments>   
- </buildCommand>   
- <buildCommand>   
-     <name>org.eclipse.wst.validation.validationbuilder</name>   
-     <arguments>   
-     </arguments>   
- </buildCommand>  
接下来import文件到eclipse jee中，右键工程----Properties----Project Facets----把Dynamic Web Module、Java、JavaScript给勾上--右边Runtimes选择tomcat----ok
![](https://img-blog.csdn.net/20131120163219687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenl1amll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样就OK了，启动tomcat，环境就部署好了！
关于eclipse jee配置tomcat问题：
eclipse配置了tomcat，其实就是在workspace里面建一个tomcat发布器，默认会把工程发布在workspace的：.metadata\.plugins\org.eclipse.wst.server.core\tmp0下面。把tomcat通过虚拟路径指到该目录下。感觉不太方便。所以我们可以通过修改workspace里的tomcat发布器，把工程发布到tomcat的物理目录webapps下面，就和myeclipse一样了。修改如下：在eclipse jee的server下面，双击Tomcat Server，在Server
 Locations里面修改。
![](https://img-blog.csdn.net/20131120163230750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenl1amll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
再把eclipse里面的Servers的server.xml打开，修改，把不必要的去掉，不然tomcat启动会有警告
**[html]**[view
 plain](http://blog.csdn.net/zyujie/article/details/16845587#)[copy](http://blog.csdn.net/zyujie/article/details/16845587#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/74726/fork)
- <ContextdocBase="test"path="/test"reloadable="true"/>
这样就完成了eclipse jee 部署 myeclipse的工程了。
还有一点，因为myeclipse默认的是WebRoot，而eclipse jee默认的是WebContent，我们需要把页面的发布路径配置成WebRoot，右键----Properties----Deployment Assembly，把WebContent删除了，配置了一个WebRoot，如下图：
![](https://img-blog.csdn.net/20131121154153234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenl1amll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、下面是eclipse jee的工程转换部署到myeclipse下面去：
这个就很简单了，找一个myeclipse的工程下面的文件：.myeclipse文件夹，.classpath文件，.mymetadata文件，.project文件，全部拷贝到要转换的工程下面去。
然后把这些文件里面的，工程名字，全部改成要转换的工程名就可以了。
