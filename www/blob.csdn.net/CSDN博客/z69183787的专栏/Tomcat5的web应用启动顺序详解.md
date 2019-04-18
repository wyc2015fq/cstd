# Tomcat5的web应用启动顺序详解 - z69183787的专栏 - CSDN博客
2014年03月06日 13:49:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2023
﻿﻿
　应用Tomcat对于我们来讲实在是司空见惯了,但是对于每个使用者来讲,应该了解其运转的机制也是必不可少的,本人在维护"apache开源项目"论坛时遇到此问题，并略作研究，望与大家共讨、分享。
　　一、配置自动部署时的web应用加载顺序：
　　当tomcat的server.xml中对虚拟主机（Host）配置中autoDeploy=true和unpackWARs=true时，如：
　　server.xml
　　...<Host name="localhost" debug="0" appBase="webapps"
　　unpackWARs="true" autoDeploy="true"
　　xmlValidation="false" xmlNamespaceAware="false">...
　　下面的部署顺序在Tomcat启动时发生：
　　1。任何具有上下文描述符（Context Descriptors）的web应用首先被部署，tomcat在$CATALINA_HOME/conf/[enginename]/[hostname]/目录中罗列出所有的以xml为结尾的文件，将其作为web应用的上下文描述符，并按照文件名排序逐一部署。
　　注意作为上下文描述符的文件名可以不为web应用名，因为tomcat会读取其中的内容来判断，但是改变上下文描述符的文件名会使部署的顺序发生变化。
　　如：
　　$CATALINA_HOME\conf\Catalina\localhost\devoffer.xml
　　<?xml version='1.0' encoding='utf-8'?><Context docBase="E:/eclipse3.1RC3/workspace/devOfferProject/web" path="/devoffer" useNaming="false" workDir="work\Catalina\localhost\devoffer"></Context>
　　上面的上下文描述符说明了devoff这个web应用的docBase和部署的path以及其工作目录。
　　2。另外，位于$CATALINA_HOME/webapps/[webappname]/META-INF/目录中的context.xml也作为上下文描述符使用,在处理了上文所说的位于$CATALINA_HOME/conf/[enginename]/[hostname]/目录中的上下文描述符后,tomcat将部署这些在web应用的META-INF目录中的context.xml。加载顺序按照应用名的字母顺序。
　　3。没有上下文描述符的已经被展开的web应用将按照其应用名顺序逐个被部署，如果其中的一个web应用关联着一个在appBase（一般为"$CATALINA_HOME/webapps"目录）中的WAR文件，则当WAR文件比相对应的被展开的web应用新时，那个被展开的web应用将被删除，tomcat将WAR文件展开并部署作为替换旧的web应用。
　　4。在执行了1-3步后，tomcat将部署在appBase中的WAR文件。
　　请注意：在每个应用被部署后，tomcat为没有上下文描述符的web应用建立上下文描述符。
　　二、非自动部署配置下的应用加载顺序：
　　此时完全按照在tomcat manager中人工部署顺序。
　　三、参考资源：
　　http://tomcat.apache.org/tomcat-5.5-doc/deployer-howto.html
