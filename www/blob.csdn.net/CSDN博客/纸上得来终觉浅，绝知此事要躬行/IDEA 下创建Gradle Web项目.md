# IDEA 下创建Gradle Web项目 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月14日 11:52:21[boonya](https://me.csdn.net/boonya)阅读数：275








Gradle和Maven很相似，但比Maven更简洁。

##### IDEA 安装Gradle

去[https://gradle.org/releases](https://gradle.org/releases)下载Gradle，然后解压到本地e。

配置环境变量：

![](https://img-blog.csdn.net/20180614114811485?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

添加到Path：

![](https://img-blog.csdn.net/20180614114841790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


配置IDEAGradle：


![](https://img-blog.csdn.net/20180614114612867?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### IDEA创建Gradle Web项目
以下流程参考：[https://segmentfault.com/a/1190000008712955](https://segmentfault.com/a/1190000008712955)


创建项目：
![图片描述](https://segmentfault.com/img/bVKIL4?w=491&h=532)

使用`Gradle`生成一个web项目
![图片描述](https://segmentfault.com/img/bVKIM8?w=818&h=769)

配置ID
![图片描述](https://segmentfault.com/img/bVKI4y?w=819&h=770)

这里选择项目的文件目录结构和使用`Gradle`的方式
![图片描述](https://segmentfault.com/img/bVKI3W?w=819&h=774)

项目名称和位置
![图片描述](https://segmentfault.com/img/bVKI3V?w=821&h=777)

完成后目录结构如下图,点击右上角编辑，配置Tomcat
![图片描述](https://segmentfault.com/img/bVKI5J?w=1436&h=437)

配置Tomcat服务器界面如下
1.服务器
2.服务器名称
3.Tomcat路径，如果这里没有可以点击config配置，找到你按照的文件夹即可
4.启动的本地域名和默认浏览器设置
5.JRE配置
![图片描述](https://segmentfault.com/img/bVKI7q?w=1088&h=736)

再次配置`Gradle`,在上一个界面中点击`Deployment`
![图片描述](https://segmentfault.com/img/bVKJi6?w=1158&h=736)
![图片描述](https://segmentfault.com/img/bVKJjb?w=489&h=518)

点击完成后，现在我们启动`Tomcat`
![图片描述](https://segmentfault.com/img/bVKJj4?w=1433&h=847)

启动成功，这里显示的是`index.jsp`的内容


![图片描述](https://segmentfault.com/img/bVKJka?w=367&h=193)


##### Gradle示例下载
![](https://img-blog.csdn.net/20180614135855906?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


Gradle 依赖配置build.gradle

```
group 'com.boonya.gradle.test'
version '1.0-SNAPSHOT'

apply plugin: 'java'
apply plugin: 'war'

sourceCompatibility = 1.8

repositories {
    mavenCentral()
}

dependencies {
    testCompile group: 'junit', name: 'junit', version: '4.11'
    testCompile group: 'junit', name: 'junit', version: '4.12'
}
```

下载地址：[https://download.csdn.net/download/boonya/10478741](https://download.csdn.net/download/boonya/10478741)












