# Dagger 2 项目配置错误: error: the import javax.annotation.Generated cannot be resolved - 定义域 - CSDN博客
2016年03月02日 10:56:43[定义域](https://me.csdn.net/u010278882)阅读数：2182
Dagger 2 项目配置错误 `error: the import javax.annotation.Generated cannot be resolved`
**处理**
报错为缺少`javax.annotation.Generated`注解（注解从JDK6中开始才被添加），经过[查询](https://github.com/google/dagger/issues/95)，发现是由于 Android SDK 中没有改注解，该注解在编译阶段会使用。可以添加 Oracle 对注解 API 的官方实现：[javax.annotation-api-1.2.jar](http://search.maven.org/remotecontent?filepath=javax/annotation/javax.annotation-api/1.2/javax.annotation-api-1.2.jar)
> 
报错信息
![](https://img-blog.csdn.net/20160302105513874)
> 
添加注解包
![](https://img-blog.csdn.net/20160302105546343)
![](https://img-blog.csdn.net/20160302105603906)
