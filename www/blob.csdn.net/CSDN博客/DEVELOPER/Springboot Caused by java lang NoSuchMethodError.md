# Springboot:Caused by: java.lang.NoSuchMethodError... - DEVELOPER - CSDN博客





2018年08月07日 15:33:53[学术袁](https://me.csdn.net/u012827205)阅读数：563
所属专栏：[后端开发](https://blog.csdn.net/column/details/25905.html)









当前问题，我从移动前端协助后台开发跑项目遇到的第一个问题。本来吧，功力就不是很深厚。遇到这个问题，把身边自诩已经工作**3年5年**的后台”牛牛”请来帮忙解决下问题。结果，也是一个劲儿的网上Google、百度！可最终和我前期结果是一样的无能为力。我想算了吧，哥们自己来！然后这里贴出自己的解决思路就是为了让更多朋友不要像我一样浪费一天半时间就为解决这个不值当的小问题上。网上确实也有很多的这种问题出现并贴出在自己的博客上，但是我觉得会有很多向我一样刚接触这种后台开发的**看不懂**，就是**看不懂**。而我的目的，只为让你瞬间解决问题，不再浪费时间，也不用去麻烦那些**忙碌的大牛**。

# 问题展示：

```
Caused by: java.lang.NoSuchMethodError: javax.servlet.ServletContext.getVirtualServerName()Ljava/lang/String;
    at org.apache.catalina.authenticator.AuthenticatorBase.startInternal(AuthenticatorBase.java:1122) ~[tomcat-embed-core-8.5.5.jar:8.5.5]
    at org.apache.catalina.util.LifecycleBase.start(LifecycleBase.java:150) [tomcat-embed-core-8.5.5.jar:8.5.5]
    ... 10 common frames omitted
```

## 图片3d效果展示：

![这里写图片描述](https://img-blog.csdn.net/20180807143741778?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

***其实真正出现问题的提示是这个：***
![这里写图片描述](https://img-blog.csdn.net/20180807143751508?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
***具体出现问题的项目结构请看：***
![这里写图片描述](https://img-blog.csdn.net/20180807143902728?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
***使用maven引入的jar包结构列表：***
![这里写图片描述](https://img-blog.csdn.net/20180807144204413?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180807144117763?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 终极解决方案

根据最后解决问题的方式以及思路，其实在别人的博客中还是能够得出很多启发的。比如这里的[她的博客](https://blog.csdn.net/u012012240/article/details/79498031)

其中有几句很重要的信息，基本已经和我的问题已经很接近了。而，我的**问题就是出现在servlet-api.jar冲突导致的**。其实这种问题就是这个原因。怎么解决才是今天的主题？！简单说就是，在安装jdk的目录，`C:\Program Files\Java\jre1.8.0_141\lib\ext` 下的如果有**servlet-api.jar**就删掉，并且保证你启动的**tomcat版本**和**Springboot项目**的**tomcat版本**与项目的**有关tomcat的jar包版本**基本一致。*因为使用Springboot时候，项目本身就自动下载有servlet-api.jar功能的api。* 所以最终就与`...lib\ext`下的jar导致了错误的结果。

## 解决问题的思路

正确时候，项目对应开发版本
|spring-boot-starter-parente版本|Eclipse版本|Maven版本|Tomcat版本|
|----|----|----|----|
|1.4.1.RELEASE|Release 4.7.0 (Oxygen)|apache-maven-3.5.4|apache-tomcat-8.5.32|

### 网上海量搜索

这个一般都会有的，希望能够顺利些！但是即使不能完全的解决你的问题，但是能够让你明白你的问题出在什么地方了，这是很重要的努力结果！

### 检查你的配置环境

#### Tomcat环境是否正确配置

windows环境，下面是正确了。具体配置可Google。 
![这里写图片描述](https://img-blog.csdn.net/20180807150427713?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Eclipse环境，不太清楚流程也可Google。完了之后，在Eclipse上Server下使用start命令启动一下就能判断是否配置好了。
#### Maven环境是否正确配置

windows环境，下面是正确了。具体配置可Google。有两个地方需注意要正确配置： 

1，用户配置上，这里我自定义了本地仓库。 
![这里写图片描述](https://img-blog.csdn.net/20180807151327617?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2，编译环境上，maven编译使用的是jdk不是jre。 
![这里写图片描述](https://img-blog.csdn.net/2018080715142865?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

完了之后，使用maven命令test一下就能判断是否配置好了。
### 解决问题

当然，我错误的配置时候，使用的是`Tomcat7.0.8`，然后又在官网下载了`Tomcat8.5.32`进行配置。 

而在版本`Tomcat7.0.8`出错的原因是，之前我把**Tomcat**中**lib包**下的`servlet-api.jar`拷贝到了`C:\Program Files\Java\jre1.8.0_141\lib\ext`目录下，而使用**Springboot**时候，**maven**又会下载Springboot配置下的jar包。由此这两处的jar包就理所当然的冲突了！ 

我的解决方式，是下载了一个和项目中的**Springboot**下载版本相同的Tomcat并布置到项目中，然后删除了`..lib\ext`目录下的`servlet-api.jar`。再次执行，就成功了。 

来一起看下成功运行的结果： 
![这里写图片描述](https://img-blog.csdn.net/20180807153044148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180807153049811?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







