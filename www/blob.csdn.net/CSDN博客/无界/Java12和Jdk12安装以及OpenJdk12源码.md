
# Java12和Jdk12安装以及OpenJdk12源码 - 无界 - CSDN博客

置顶2019年03月23日 14:12:39[21aspnet](https://me.csdn.net/21aspnet)阅读数：2301个人分类：[Java																](https://blog.csdn.net/21aspnet/article/category/1877583)



**文档：**
JDK 12文档:[https://docs.oracle.com/en/java/javase/12/](https://docs.oracle.com/en/java/javase/12/)
**下载：**
OracleJDK12下载：[https://www.oracle.com/technetwork/java/javase/downloads/jdk12-downloads-5295953.html](https://www.oracle.com/technetwork/java/javase/downloads/jdk12-downloads-5295953.html)
csdn上我下好的，速度较快：[https://download.csdn.net/download/21aspnet/11052067](https://download.csdn.net/download/21aspnet/11052067)
OpenJDK12：[https://jdk.java.net/java-se-ri/12](https://jdk.java.net/java-se-ri/12)
源码：[https://download.java.net/openjdk/jdk12/ri/openjdk-12+32_src.zip](https://download.java.net/openjdk/jdk12/ri/openjdk-12+32_src.zip)
说明：在Windows上作为开发和研究下载OracleJDK12就可以了，这个和OpenJDK差距不大。
OracleJDK12个人学习研究是免费的，而如果你安装在公司服务器用于商业服务是需要付费，
但是你也可以使用OpenJDK12用于线上，没有问题！
**安装：**
新版的jdk安装很快，不像老版的1.8需要分为jdk和jre已经合并一个目录了。
![](https://img-blog.csdnimg.cn/20190323140714247.PNG)
很快就安装好了，很快很简单。
**配置环境变量**：
之前的JAVA_HOME和CLASSPATH已经都不要了。只要配置jdk的bin到**Path**里：
## C:\Program Files\Java\jdk-12\bin
win10的：
![](https://img-blog.csdnimg.cn/20190323140924805.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
下面是win7的：
![](https://img-blog.csdnimg.cn/20190325121930857.PNG)
**验证版本**：
Win10下 ：
![](https://img-blog.csdnimg.cn/20190323141012177.PNG)
win7下：
![](https://img-blog.csdnimg.cn/20190325125051268.PNG)
**修改IDEA配置：**
jdk版本，语言特性和maven都要改为对应的jdk也就12.
![](https://img-blog.csdnimg.cn/20190323141044899.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190323141109349.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190323141057668.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
**运行测试：**
![](https://img-blog.csdnimg.cn/2019032314120482.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
一切很简单！

