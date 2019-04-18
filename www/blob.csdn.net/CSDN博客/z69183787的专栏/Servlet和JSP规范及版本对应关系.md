# Servlet和JSP规范及版本对应关系 - z69183787的专栏 - CSDN博客
2014年08月01日 10:38:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2650
JSR 53: JavaTM Servlet 2.3 and JavaServer PagesTM 1.2
JSR 154: JavaTM Servlet 2.4
JSR 154: JavaTM Servlet 2.5(Maintenance Release 2)
JSR 315: JavaTM Servlet 3.0（This JSR will be an update to the existing Servlet 2.5 specification.）
JSR 152: JavaServer PagesTM 2.0
JSR 245: JavaServerTM Pages 2.1
JSR 127: JavaServerTM Faces
JSR 252: JavaServerTM Faces 1.2
JSR 314: JavaServerTM Faces 2.0
[admin10000.com](http://www.admin10000.com/) 整理
**Servlet和JSP规范版本对应关系：**
|Servlet规范版本|JSP版本|JSF版本|JAVA EE版本|
|----|----|----|----|
|Servlet2.3|JSP1.2、JSP1.1||J2EE1.3|
|Servlet2.4|JSP2.0|JSF1.1|J2EE1.4|
|Servlet2.5|JSP2.1|JSF1.2、JSF2.0|Java EE5|
|Servlet3.0|JSP2.2||Java EE6|
**Tomcat所对应的Servlet/JSP规范和JDK版本：**
|Servlet/JSP Spec|Apache Tomcat version|Actual release revision|Minimum Java Version|
|----|----|----|----|
|3.0/2.2|7.0.x|7.0.12|1.6|
|2.5/2.1|6.0.x|6.0.32|1.5|
|2.4/2.0|5.5.x|5.5.33|1.4|
|2.3/1.2|4.1.x (archived)|4.1.40 (archived)|1.3|
|2.2/1.1|3.3.x (archived)|3.3.2 (archived)|1.1|
Apache官方对各版本的解释：[http://tomcat.apache.org/whichversion.html](http://tomcat.apache.org/whichversion.html)
|Tomcat版本|Servlet版本|Jsp版本|Jstl版本|
|----|----|----|----|
|6.0.x|2.5|2.1|1.2、1.1.x|
|5.5.x|2.4|2.0|1.1.x|
|5.0.x|2.4|2.0|1.1|
|4.1.x|2.3|1.2|1.0|
|3.3.x|2.2|1.1||
jstl对应的jar包说明：
（1）JSTL1.0（包含2个jar文件：jstl.jar和standard.jar。这两个jar文件都存在于Standard1.0.x的发布版本中。）
下载地址：http://archive.apache.org/dist/jakarta/taglibs/standard-1.0/binaries/
（2）JSTL1.1（也包含2个jar文件：jstl.jar和standard.jar。这两个jar文件都存在于Standard1.1.x的发布版本中。）
下载地址：http://archive.apache.org/dist/jakarta/taglibs/standard-1.0/binaries/
（3）JSTL1.2（也包含2个jar文件：jstl-api-1.2.jar和jstl-impl-1.2.jar）
下载地址：http://jstl.java.net/
