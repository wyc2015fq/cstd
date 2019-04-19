# Java web项目打包成war包的方法 - xqhrs232的专栏 - CSDN博客
2018年09月29日 14:29:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：98
原文地址::[https://blog.csdn.net/Julycaka/article/details/78211548](https://blog.csdn.net/Julycaka/article/details/78211548)
当开发人员在自己的开发机器上调试所有代码并通过后，为了交给测试人员测试和未来进行产品发布，都需要将开发人员的源码打包成War包进行发布。javaWeb项目（maven项目）打包的方式有以下几种：
**1.直接使用Ecilpse打包（有三种）**
- 在ecilpse中右键项目，选择Export——>web目录下的WAR file——>Destination选择自己需要保存的路径——>Finish
- 
右键项目的pom.xml文件，选择Debug as或Run as 都行。再选择Maven install  打包。执行成功后，日志会打印出位置（看自己配置是否日志输出如下图）。如果没有输出，在默认的C盘.m2文件夹中。
![](https://img-blog.csdn.net/20171016152858840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSnVseWNha2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 右键项目。选择Debug as 或 Run as 。再选择 Debug configurations，Browse Workspace  选择需要打包的项目（默认是当前项目）在Goals中输入clean install  命令  Debug执行后，会在target目录下生成打的war包
![](https://img-blog.csdn.net/20171016152735692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSnVseWNha2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.命令打包**
环境：eclipse、apache-maven-3.3.3、apache-tomcat-7.0.70
利用maven的maven-war-plugin
` `- 
`<build> `
- 
`<plugins> `
- 
`...... `
- 
`<plugin> `
- 
`<groupId>org.apache.maven.plugins</groupId> `
- 
`<artifactId>maven-war-plugin</artifactId> `
- 
`<version>2.0.2</version> `
- 
`<configuration> `
- 
`       <webXml>/webapp/WEB-INF/web.xml</webXml>`
- 
`    </configuration> `
- 
`</plugin> `
- 
`</plugins> `
- 
`</build> `
开始-运行-cmd，打开cmd命令窗口，执行命令切换到工程所在目录
执行命令“mvn clean package”，此时会将当前工程执行打包操作，打包成功后会生成*.war形式的war包。
（但是我执行了这个命令，没有忽略单元测试，会报错，导致打包失败，如下图）
![](https://img-blog.csdn.net/20171016153003863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSnVseWNha2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决：打包忽略单元测试，直接打包，执行命令“mvn clean install -DskipTests”或者"mvn clean install -Dmaven.Test.skip=true"都可以忽略单元测试。打包成功显示success。
![](https://img-blog.csdn.net/20171016153033920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSnVseWNha2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
到工程下的target文件夹下找到war包。此war包即为工程war包。
--------------------- 本文来自 Julycaka 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/Julycaka/article/details/78211548?utm_source=copy
