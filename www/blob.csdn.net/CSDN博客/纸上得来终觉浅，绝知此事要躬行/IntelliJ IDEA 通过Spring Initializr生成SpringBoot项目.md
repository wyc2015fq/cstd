# IntelliJ IDEA 通过Spring Initializr生成SpringBoot项目 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月17日 15:28:24[boonya](https://me.csdn.net/boonya)阅读数：2012








IntellJ IDEA 通过Spring Initializr创建SpringBoot项目会遇到 [IntelliJ “Initialization failed for 'https://start.spring.io' Please check URL, network and proxy settings”](https://stackoverflow.com/questions/38653809/intellij-initialization-failed-for-https-start-spring-io-please-check-url) 问题。这是IDE中遇到的问题，也可以 参考[https://github.com/spring-io/initializr/issues/267](https://github.com/spring-io/initializr/issues/267)进行了解。

##### 解决Spring Initializr连接错误



进入到IDEA的setting ，搜索 HTTP Proxy ，选择Auto-detect proxy settings
![](https://img-blog.csdn.net/20180617151438831?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


点击最下面的Check connection弹出如下的输入框，输入地址https://start.spring.io点击ok

![](https://img-blog.csdn.net/20180617151527767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


如果successful证明连接成功，重新生成一次，完美生成springboot项目
![](https://img-blog.csdn.net/20180617151540707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 创建Maven SpringBoot项目
1、打开Spring Initialzr工具创建项目

![](https://img-blog.csdn.net/20180617151049985?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、填写项目信息

![](https://img-blog.csdn.net/20180617151818749?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3、勾选项目依赖


![](https://img-blog.csdn.net/20180617151755733?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4、修改项目路径

![](https://img-blog.csdn.net/20180617152243138?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5、项目创建完成效果

![](https://img-blog.csdn.net/20180617152305971?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 创建Gradle SpringBoot项目
1、打开Spring Initialzr工具创建项目
![](https://img-blog.csdn.net/20180617151049985?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


2、填写项目信息

![](https://img-blog.csdn.net/20180617151921872?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


3、勾选项目依赖


![](https://img-blog.csdn.net/20180617151943480?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


4、设置项目文件路径

![](https://img-blog.csdn.net/20180617152027212?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5、选择Gradle依赖环境

![](https://img-blog.csdn.net/20180617152055491?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

6、项目生成完成效果

![](https://img-blog.csdn.net/20180617152122180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


参考文章地址：

[W3cSchool Gradle教程](https://www.w3cschool.cn/gradle/)


[IDEA 访问Spring Initialzr异常处理](https://blog.csdn.net/u014417033/article/details/58721184)

[Spring Boot + gradle环境搭建](https://blog.csdn.net/u013005791/article/details/73325685)


[springboot+maven快速构建项目](https://www.cnblogs.com/suncj/p/4065589.html)




