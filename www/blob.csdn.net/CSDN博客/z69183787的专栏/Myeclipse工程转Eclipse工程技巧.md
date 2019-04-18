# Myeclipse工程转Eclipse工程技巧 - z69183787的专栏 - CSDN博客
2014年02月28日 16:48:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2550
在eclipse右击工程properties->Project Facets 出现如下图： 
![](http://dl.iteye.com/upload/attachment/316747/8af0d25c-7ace-3099-b13d-1d70f4595591.jpg)
  按照上图打钩，不过设置好以后还是不能添加工程到tomcat6.0运行,因为Dynamic Web Module3.0需要tomcat7的支持,而我又不想用tomcat7，所以接下来还需要手动修改当前工程下的属性文件.settings下面的org.eclipse.wst.common.project.facet.core.xml 
<?xml version="1.0" encoding="UTF-8"?> 
<faceted-project> 
  <fixed facet="wst.jsdt.web"/> 
  <installed facet="java" version="1.6"/> 
<installed facet="jst.web" version="2.5"/>
  <installed facet="wst.jsdt.web" version="1.0"/> 
</faceted-project> 
修改了以后刷新工程，然后添加到tomcat webapp目录。ok！ 
部署以后运行工程居然找不到地址，我日，到webapp下面看，发现工程没有全部加载进去，看来还需要再修改下属性文件。 
于是继续修改org.eclipse.wst.common.component文件。添加 <wb-resource deploy-path="/" source-path="/WebRoot"/>到<wb-module deploy-name="infosys"><//wb-module>中，文中加粗的字体需要根据实际情况自行修改。 
  改完刷新工程，重新启动tomcat工程,居然成功了,我晕！ 
