# RabbitMQ 开发常识初见 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年12月29日 19:31:31[boonya](https://me.csdn.net/boonya)阅读数：996标签：[windows																[maven																[java																[mac																[server](https://so.csdn.net/so/search/s.do?q=server&t=blog)
个人分类：[RabbitMQ](https://blog.csdn.net/boonya/article/category/6804176)





居然忘了自己粗略的研究过RabbitMQ ，但是记忆已被尘封，居然又想研究它了，之前只是蜻蜓点水地看了一下效果，对其特性还没深入研究过，慢慢学习。

![RabbitMQ](http://www.rabbitmq.com/img/rabbitmq_logo_strap.png)


### 1.RabbitMQ Server 服务端

#### 1.1系统支持

windows/linux/unix/mac OS

#### 1.2Server下载

[http://www.rabbitmq.com/releases/rabbitmq-server/v3.4.2/rabbitmq-server-windows-3.4.2.zip](http://www.rabbitmq.com/releases/rabbitmq-server/v3.4.2/rabbitmq-server-windows-3.4.2.zip)


#### 1.3历史版本

[http://www.rabbitmq.com/releases/rabbitmq-server/](http://www.rabbitmq.com/releases/rabbitmq-server/)


#### 1.4资源下载

如果你还没有RabbitMQ，那就访问这里吧：[http://www.rabbitmq.com/download.html](http://www.rabbitmq.com/download.html)，在这里你可以更好的把我其特性。

#### 1.5启动Server

命令规则：rabbitmq-server [-detached]

安装配置完环境变量，创建rabbitmq-server.bat，在文件中输入如下脚本：

`rabbitmq-server — detached`执行bat文件 服务器后台进程中运行。

注：如果环境变量已经配好了这些命令都可以在cmd窗口执行。

#### 1.6建立服务

命令规则：rabbitmq-service.bat [command]

在rabbitmq-service.bat文件中输入操作命令：


`rabbitmq-service install`
启动服务命令：

`rabbitmq-service start`


服务可以操作的命令有：

help
Display usage information.
install
Install the service. The service will not be started. Subsequent invocations will update the service parameters if relevant environment variables were modified.
remove
Remove the service. If the service is running then it will automatically be stopped before being removed. No files will be deleted as a consequence and rabbitmq-server will
 remain operable.
start
Start the service. The service must have been correctly installed beforehand.
stop
Stop the service. The service must be running for this command to have any effect.
disable
Disable the service. This is the equivalent of setting the startup type to Disabled using
 the service control panel.
enable
Enable the service. This is the equivalent of setting the startup type to Automatic using
 the service control panel.
在执行中bat作为服务可以省掉不写。




### 2.RabbitMQ 客户端开发

#### 2.1支持客户端语言

Java 、C#/.NET

#### 2.2Maven配置



```
<dependency>
  <groupId>com.rabbitmq</groupId>
  <artifactId>amqp-client</artifactId>
  <version>3.4.2</version>
</dependency>
```



#### 2.3Java Client下载



[http://www.rabbitmq.com/java-client.html](http://www.rabbitmq.com/java-client.html)


如果还不知道怎么去安装使用，我这里还有篇文章：[http://blog.csdn.net/boonya/article/details/37879739](http://blog.csdn.net/boonya/article/details/37879739)以后有时间再深入研究。](https://so.csdn.net/so/search/s.do?q=mac&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=maven&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)




