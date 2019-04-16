# Netty构建游戏服务器(三)--netty spring简单整合 - 菜鸟路上的小白 - CSDN博客





2016年10月26日 20:07:55[somnus_小凯](https://me.csdn.net/u012486840)阅读数：10382








# 一，基本方法

上节实现了netty的基本连接，这节加入spring来管理netty，由spring来开启netty服务。

在netty服务器中，我们建立了三个类：HelloServer（程序主入口） ， HelloServerInitializer（传输通道初始化），HelloServerHandler（业务控制器）

这三个类中HelloServer中new了一个HelloServerInitializer，在HelloServerInitializer最后又new了一个HelloServerHandler。其中需要new的地方，就是spring要管理的地方。

# 二，实现源码

## 1，相关准备

导入需要的JAR包和相关依赖，主要是spring，log4j，netty等，后面的项目源码中包含了所有jar包；

另外你可以使用maven来构建项目，添加各种jar包。

## 2，主程序TestSrping.java

主程序很简单，就是加载spring配置文件就可以了。

```java
```java
package com.wayhb;

import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class TestSpring {
	
	private static Logger log = Logger.getLogger(TestSpring.class); 

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//加载spirng配置文件
		ApplicationContext context= new ClassPathXmlApplicationContext("server.xml");
		
		
	}

}
```
```

## 3，spring的XML配置单server.xml和LOG4J配置文件

server.xml

```java
```xml
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:aop="http://www.springframework.org/schema/aop"
	xmlns:tx="http://www.springframework.org/schema/tx" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:lang="http://www.springframework.org/schema/lang" xmlns:util="http://www.springframework.org/schema/util"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd http://www.springframework.org/schema/lang http://www.springframework.org/schema/lang/spring-lang-3.0.xsd http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util-3.0.xsd"
	default-lazy-init="false" default-autowire="byName">

<!--  <bean id="user" class="com.wayhb.User">
<property name="userid" value="20"></property>
<property name="username" value="wayhb"></property>
</bean>-->
<!--开启注解方式，扫描com.wayhb,com.netty两个包路径-->
<context:annotation-config/> 
<context:component-scan base-package="com.wayhb,com.netty" /> 

<!-- 传统方法配置BEAN
<bean id="helloServer" class="com.netty.HelloServer" init-method="serverStart">
<property name="helloServerInitializer" ref="helloServerInitializer"></property>
</bean>

<bean id="helloServerInitializer" class="com.netty.HelloServerInitializer">
<property name="helloServerHandler" ref="helloServerHandler"></property>
</bean>

<bean id="helloServerHandler" class="com.netty.HelloServerHandler" scope="prototype"></bean>
 -->

</beans>
```
```

log4j.properties

```
### 设置###
#log4j.rootLogger = debug,stdout,D,E
log4j.rootLogger = debug,stdout
### 输出信息到控制抬 ###
log4j.appender.stdout = org.apache.log4j.ConsoleAppender
log4j.appender.stdout.Target = System.out
log4j.appender.stdout.layout = org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern = [%-5p] %d{yyyy-MM-dd HH:mm:ss,SSS} method:%l%n%m%n

### 输出DEBUG 级别以上的日志到=E://logs/error.log ###
#log4j.appender.D = org.apache.log4j.DailyRollingFileAppender
#log4j.appender.D.File = C://logs/log.log
#log4j.appender.D.Append = true
#log4j.appender.D.Threshold = DEBUG 
#log4j.appender.D.layout = org.apache.log4j.PatternLayout
#log4j.appender.D.layout.ConversionPattern = %-d{yyyy-MM-dd HH:mm:ss}  [ %t:%r ] - [ %p ]  %m%n

### 输出ERROR 级别以上的日志到=E://logs/error.log ###
#log4j.appender.E = org.apache.log4j.DailyRollingFileAppender
#log4j.appender.E.File =C://logs/error.log 
#log4j.appender.E.Append = true
#log4j.appender.E.Threshold = ERROR 
#log4j.appender.E.layout = org.apache.log4j.PatternLayout
#log4j.appender.E.layout.ConversionPattern = %-d{yyyy-MM-dd HH:mm:ss}  [ %t:%r ] - [ %p ]  %m%n
```

其中输出功能是关闭的，需要的话去掉#就可以了

## 4，HelloServer

```java
package com.netty;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.ChannelFuture;
import io.netty.channel.EventLoopGroup;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.nio.NioServerSocketChannel;

import javax.annotation.PostConstruct;

import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
//注解方式注入bean，名字是helloServer
@Service("helloServer")
public class HelloServer {
	private static Logger log = Logger.getLogger(HelloServer.class); 
    /**
     * 服务端监听的端口地址
     */
    private static final int portNumber = 7878;
    
    //自动装备变量，spring会根据名字或者类型来装备这个变量，注解方式不需要set get方法了
    @Autowired
    private HelloServerInitializer helloServerInitializer;

    //程序初始方法入口注解，提示spring这个程序先执行这里
	@PostConstruct
    public void serverStart() throws InterruptedException{
    	 EventLoopGroup bossGroup = new NioEventLoopGroup();
         EventLoopGroup workerGroup = new NioEventLoopGroup();
         try {
             ServerBootstrap b = new ServerBootstrap();
             b.group(bossGroup, workerGroup);
             b.channel(NioServerSocketChannel.class);
             b.childHandler(helloServerInitializer);

             // 服务器绑定端口监听
             ChannelFuture f = b.bind(portNumber).sync();
             // 监听服务器关闭监听
             f.channel().closeFuture().sync();

             log.info("###########################################");
             // 可以简写为
             /* b.bind(portNumber).sync().channel().closeFuture().sync(); */
         } finally {
             bossGroup.shutdownGracefully();
             workerGroup.shutdownGracefully();
         }
    }
    
}
```

注解方式更加简便，配置内容少了很多

## 5，HelloServerInitializer

```java
package com.netty;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import io.netty.channel.ChannelInitializer;
import io.netty.channel.ChannelPipeline;
import io.netty.channel.socket.SocketChannel;
import io.netty.handler.codec.DelimiterBasedFrameDecoder;
import io.netty.handler.codec.Delimiters;
import io.netty.handler.codec.string.StringDecoder;
import io.netty.handler.codec.string.StringEncoder;

@Service("helloServerInitializer")
public class HelloServerInitializer extends ChannelInitializer<SocketChannel> {

	@Autowired
	private HelloServerHandler helloServerHandler;
	
	@Override
    protected void initChannel(SocketChannel ch) throws Exception {
        ChannelPipeline pipeline = ch.pipeline();

        // 以("\n")为结尾分割的 解码器
        pipeline.addLast("framer", new DelimiterBasedFrameDecoder(8192, Delimiters.lineDelimiter()));

        // 字符串解码 和 编码
        pipeline.addLast("decoder", new StringDecoder()); 
        pipeline.addLast("encoder", new StringEncoder());

        // 自己的逻辑Handler
        pipeline.addLast("handler", helloServerHandler);
    }
}
```

## 6，HelloServerHandler

```java
package com.netty;

import java.net.InetAddress;

import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Service;

import io.netty.channel.Channel;
import io.netty.channel.ChannelHandlerContext;
import io.netty.channel.SimpleChannelInboundHandler;
import io.netty.channel.group.ChannelGroup;
import io.netty.channel.group.DefaultChannelGroup;
import io.netty.util.concurrent.GlobalEventExecutor;
import io.netty.channel.ChannelHandler.Sharable;

@Service("helloServerHandler")
@Scope("prototype")
//特别注意这个注解@Sharable，默认的4版本不能自动导入匹配的包，需要手动加入
//地址是import io.netty.channel.ChannelHandler.Sharable;
@Sharable
public class HelloServerHandler extends SimpleChannelInboundHandler<String> {
	
	
	public static ChannelGroup channels = new DefaultChannelGroup(GlobalEventExecutor.INSTANCE);

    @Override
    public void handlerAdded(ChannelHandlerContext ctx) throws Exception {  // (2)
        Channel incoming = ctx.channel();
        for (Channel channel : channels) {
            channel.writeAndFlush("[SERVER] - " + incoming.remoteAddress() + " 加入\n");
        }
        channels.add(ctx.channel());
    } 
    
    @Override
    public void handlerRemoved(ChannelHandlerContext ctx) throws Exception {  // (3)
        Channel incoming = ctx.channel();
        for (Channel channel : channels) {
            channel.writeAndFlush("[SERVER] - " + incoming.remoteAddress() + " 离开\n");
        }
        channels.remove(ctx.channel());
    }
    
    @Override
    protected void channelRead0(ChannelHandlerContext ctx, String msg) throws Exception {
        // 收到消息直接打印输出
        System.out.println(ctx.channel().remoteAddress() + " Say : " + msg);
        
        // 返回客户端消息 - 我已经接收到了你的消息
        ctx.writeAndFlush("Received your message !\n");
    }
    
    /*
     * 
     * 覆盖 channelActive 方法 在channel被启用的时候触发 (在建立连接的时候)
     * 
     * channelActive 和 channelInActive 在后面的内容中讲述，这里先不做详细的描述
     * */
    @Override
    public void channelActive(ChannelHandlerContext ctx) throws Exception {
        
        System.out.println("RamoteAddress : " + ctx.channel().remoteAddress() + " active !");
        
        ctx.writeAndFlush( "Welcome to " + InetAddress.getLocalHost().getHostName() + " service!\n");
        
        super.channelActive(ctx);
    }
}
```

特别注意这个注解@Sharable，默认的4版本不能自动导入匹配的包，需要手动加入

地址是import io.netty.channel.ChannelHandler.Sharable;

# 三，运行测试

1，打开TestSpring.java，右键运行，正常情况下服务器会开启；

2，打开上节写的项目，打开HelloClient.java，右键运行；

3，HelloClient.java上再运行一次，出现两个客户端，如果都连接成功，说明项目没有问题。

注意事项：netty4需要@Sharable才可以开启多个handler，所以需要多个连接的handler类上要加入注解@Sharable，该注解自动导入有问题，手动加入包import io.netty.channel.ChannelHandler.Sharable;



