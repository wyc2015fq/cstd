# myeclipse中的转为Web项目 - z69183787的专栏 - CSDN博客
2014年02月28日 16:35:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6845
一直以来都用的是myeclipse，昨天心血来潮，将myeclipse中生成的web项目导入eclipse,发现原来web左上角的小球不见了，变成了J标志，即，javaweb项目标示，变成了Java项目标示，网上找资料发现需要以下操作。
1，首先看是否有红色叹号，如果有红色叹号说明引入jar包路径错误，不匹配。先把所有的jar包匹配正确。
2，到.peject文件修改<natures>...</natures>代码段。在其中添加
<nature>org.eclipse.wst.common.project.facet.core.nature</nature> 
<nature>org.eclipse.wst.common.modulecore.ModuleCoreNature</nature> 
<nature>org.eclipse.jem.workbench.JavaEMFNature</nature>
这样基本上就ok了。
3，在eclipse的项目上点右键，刷新项目。
4，右键点击项目名进入properties---project facets,选中java和Dynamic Web model,apply
至此此项目将恢复web项目图标
不过我添加到web容器并部署的时候有出现了新问题：提示
Tomcat version 6.0 only supports J2EE 1.2, 1.3, 1.4, and Java EE 5 Web modul并拒绝添加和部署
在.setting文件夹下查看配置信息，发现
org.eclipse.wst.common.project.facet.core.xml文件中配置有错误
<?xml
 version="1.0" encoding="UTF-8"?>
<faceted-project>
  <installed facet="java" version="1.6"/>
  <installed facet="jst.web" version="2.5"/>
  <installed facet="wst.jsdt.web" version="1.0"/>
</faceted-project>
原来myeclipse中建web项目用的是3.0，但是我eclipse中配的是tomcat6.0只支持2.5，两个方式解决，换tomcat7.0，或者修改为2.5.呵呵，偷懒，直接换2.5吧
