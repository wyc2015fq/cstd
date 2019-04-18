# Weblogic11g（server10.3.3）部署项目,遇到的问题解决记录。 - z69183787的专栏 - CSDN博客
2013年02月21日 15:44:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8440
情况：项目平时用tomcat6调试，生产机是AIX，上面安装的是Weblogic11g（server10.3.3）。趁着项目正在开发，赶快上去部署测试一下，免得最后上线的时候才发现一大堆的问题。
处理情况：下载了一个版本相同的WebLogic，在本机测试，测好了再上去测试。
                参考了2个帖子：【http://xly1981.javaeye.com/blog/563896】            【 http://ons.javaeye.com/blog/484251】
问题1： Hibernate3  部署问题 
Hibernate3中hibernate.query.factory_class的默认值为org.hibernate.hql.ast.ASTQueryTranslatorFactory，在WebLogic下系统运行时会抛出org.hibernate.QueryException: ClassNotFoundException: org.hibernate.hql.ast.HqlToken异常。
这个问题网上说的很多，解决方法也各式各样，其实很简单，Weblogic系统默认加载EJB-QL parser，存在重名类，所以使用时会出现ClassNotFoundException。一般网上的修改方式都是修改startWebLogic运行的脚本，将antlr-2.7.5H3.jar文件优先加载。但这样的方法会带来一些其他问题，所以不推荐使用。
解决办法：在WEB-INF目录下建一个weblogic.xml文件，文件中写入如下代码： 
<?xml version="1.0" encoding="UTF-8"?> 
<!DOCTYPE weblogic-web-app PUBLIC "-//BEA Systems, Inc.//DTD Web Application 8.1//EN" "http://www.bea.com/servers/wls810/dtd/weblogic810-web-jar.dtd"> 
<weblogic-web-app> 
  <container-descriptor> 
    <prefer-web-inf-classes>true</prefer-web-inf-classes> 
  </container-descriptor> 
</weblogic-web-app> 
说明：prefer-web-inf-classes=true是WebLogic's classloader在有重名类时，优先加载Web应用中WEB-INF内的类。 
问题2： JSTL解析失败
index.jsp:1:1: validator 类 "org.apache.taglibs.standard.tlv.JstlCoreTLV" 失败，出现如下异常: "java.lang.ClassCastException: weblogic.xml.jaxp.RegistrySAXParserFactory"。
<%@page contentType="text/html;charset=utf-8" language="java"%>
解决办法：参考问题1，在WEB-INF目录下建一个weblogic.xml文件，最后是把prefer-web-inf-classes设置为false就可以了。
问题1和问题2都碰到了怎么办？一个要true，一个要false。 我cao，Weblogic的确够恶心的！
第一步： 修改启动脚本 D:/oracle/Middleware/user_projects/domains/base_domain/bin/startWebLogic.cmd
找到set CLASSPATH=%SAVE_CLASSPATH%，在后面加入
@REM 老胡 add开始
set PRE_CLASSPATH=%WL_HOME%/server/lib/antlr-2.7.6.jar;
set CLASSPATH=%PRE_CLASSPATH%;%CLASSPATH% 
@REM 老胡 add结束
第二步：antlr-2.7.6.jar包 需要拷贝到D:/oracle/Middleware/wlserver_10.3/server/lib下。
第三步：删除原来你在web-inf目录下增加的 WebLogic.xml  （非常重要！！！ 我cao，昨天试到晚上1点多，都没解决问题，今天同事一说才明白过来，晕死）
shit！ AIX上的写法和windows平台有很大区别。搞了半天才发现。
# 老胡add start
PRE_CLASSPATH="${WL_HOME}/server/lib/antlr-2.7.6.jar:"
CLASSPATH="${PRE_CLASSPATH}${SAVE_CLASSPATH}"
# 老胡 add end
注意，AIX是【冒号】分隔。。。。。  搞了好几个小时，才被一个同事看出不一样。
------------------------------华丽的分割线--------------------------------------------------
问题3： 如何在AIX上加大JVM内存？
解决办法：google了一下，有好多办法，这里只采用最简单的。
编辑 /weblogic/bea/user_projects/domains/base_domain/bin/startWebLogic.cmd
增加以下内容：
# 老胡 增大内存start
MEM_ARGS="-Xms1024m -Xmx1024m -XX:PermSize=256m -XX:MaxPermSize=512m"
export MEM_ARGS
# 老胡 增大内存end
问题：IE中会直接打开WORD、excel、pdf等文件，导致乱码。 tomcat不会有问题
如果在一个WEB APPLICATION 中要访问WORD，EXCEL等类型的文件为了让能在IE浏览器中自动打开的设置：需要在WEB.XML中进行如下的设置：在WEB.XML中添加,其中:
如能在IE中自动打开DOC，XLS，PDF文件的配置如下: 
<?xml version="1.0" ?> 
<!DOCTYPE web-app PUBLIC "-//Sun Microsystems, Inc.//DTD Web Application 1.2//EN" "http://java.sun.com/j2ee/dtds/web-app_2_2.dtd"> 
<web-app>
  <mime-mapping> 
  <extension>doc</extension> 
  <mime-type>application/msword</mime-type> 
  </mime-mapping> 
  <mime-mapping> 
  <extension>xls</extension> 
  <mime-type>application/msexcel</mime-type> 
  </mime-mapping> 
  <mime-mapping> 
  <extension>pdf</extension> 
  <mime-type>application/pdf</mime-type> 
  </mime-mapping> 
</web-app>
本文摘自：http://blog.csdn.net/hdfyq/article/details/5752912
