# Spring Boot 1.X和2.X优雅重启实战 - 纯洁的微笑 - CSDN博客
2018年05月31日 07:36:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：3810
项目在重新发布的过程中，如果有的请求时间比较长，还没执行完成，此时重启的话就会导致请求中断，影响业务功能，优雅重启可以保证在停止的时候，不接收外部的新的请求，等待未完成的请求执行完成，这样可以保证数据的完整性。
# Spring Boot 1.X
```
import java.util.concurrent.Executor;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import org.apache.catalina.connector.Connector;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.context.embedded.ConfigurableEmbeddedServletContainer;
import org.springframework.boot.context.embedded.EmbeddedServletContainerCustomizer;
import org.springframework.boot.context.embedded.tomcat.TomcatConnectorCustomizer;
import org.springframework.boot.context.embedded.tomcat.TomcatEmbeddedServletContainerFactory;
import org.springframework.context.ApplicationListener;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.event.ContextClosedEvent;
/**
 * Spring Boot1.X Tomcat容器优雅停机
 * @author yinjihuan
 *
 */
@Configuration
public class ShutdownConfig {
    /**
     * 用于接受shutdown事件
     * @return
     */
    @Bean
    public GracefulShutdown gracefulShutdown() {
        return new GracefulShutdown();
    }
    /**
     * 用于注入 connector
     * @return
     */
    @Bean
    public EmbeddedServletContainerCustomizer tomcatCustomizer() {
        return new EmbeddedServletContainerCustomizer() {
            @Override
            public void customize(ConfigurableEmbeddedServletContainer container) {
                if (container instanceof TomcatEmbeddedServletContainerFactory) {
                    ((TomcatEmbeddedServletContainerFactory) container).addConnectorCustomizers(gracefulShutdown());
                }
            }
        };
    }
    private static class GracefulShutdown implements TomcatConnectorCustomizer, ApplicationListener<ContextClosedEvent> {
        private static final Logger log = LoggerFactory.getLogger(GracefulShutdown.class);
        private volatile Connector connector;
        private final int waitTime = 120;
        @Override
        public void customize(Connector connector) {
            this.connector = connector;
        }
        @Override
        public void onApplicationEvent(ContextClosedEvent event) {
            this.connector.pause();
            Executor executor = this.connector.getProtocolHandler().getExecutor();
            if (executor instanceof ThreadPoolExecutor) {
                try {
                    ThreadPoolExecutor threadPoolExecutor = (ThreadPoolExecutor) executor;
                    log.info("shutdown start");
                    threadPoolExecutor.shutdown();
                    log.info("shutdown end");
                    if (!threadPoolExecutor.awaitTermination(waitTime, TimeUnit.SECONDS)) {
                        log.info("Tomcat 进程在" + waitTime + "秒内无法结束，尝试强制结束");
                    }
                    log.info("shutdown success");
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }
}
```
# Spring Boot 2.X
```
import java.util.concurrent.Executor;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import org.apache.catalina.connector.Connector;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.web.embedded.tomcat.TomcatConnectorCustomizer;
import org.springframework.boot.web.embedded.tomcat.TomcatServletWebServerFactory;
import org.springframework.boot.web.servlet.server.ServletWebServerFactory;
import org.springframework.context.ApplicationListener;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.event.ContextClosedEvent;
/**
 * Spring Boot2.X Tomcat容器优雅停机
 * @author yinjihuan
 *
 */
@Configuration
public class ShutdownConfig {
    /**
     * 用于接受shutdown事件
     * @return
     */
    @Bean
    public GracefulShutdown gracefulShutdown() {
        return new GracefulShutdown();
    }
    @Bean
    public ServletWebServerFactory servletContainer() {
      TomcatServletWebServerFactory tomcat = new TomcatServletWebServerFactory();
      tomcat.addConnectorCustomizers(gracefulShutdown());
      return tomcat;
    }
    private static class GracefulShutdown implements TomcatConnectorCustomizer, ApplicationListener<ContextClosedEvent> {
        private static final Logger log = LoggerFactory.getLogger(GracefulShutdown.class);
        private volatile Connector connector;
        private final int waitTime = 120;
        @Override
        public void customize(Connector connector) {
            this.connector = connector;
        }
        @Override
        public void onApplicationEvent(ContextClosedEvent event) {
            this.connector.pause();
            Executor executor = this.connector.getProtocolHandler().getExecutor();
            if (executor instanceof ThreadPoolExecutor) {
                try {
                    ThreadPoolExecutor threadPoolExecutor = (ThreadPoolExecutor) executor;
                    log.info("shutdown start");
                    threadPoolExecutor.shutdown();
                    log.info("shutdown end");
                    if (!threadPoolExecutor.awaitTermination(waitTime, TimeUnit.SECONDS)) {
                        log.info("Tomcat 进程在" + waitTime + "秒内无法结束，尝试强制结束");
                    }
                    log.info("shutdown success");
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }
}
```
重启服务脚本：
```
NG="zh_CN.UTF-8"
pid=`ps ax | grep fangjia-youfang-web | grep java | head -1 | awk '{print $1}'`
echo $pid
#kill $pid
curl -X POST http://127.0.0.1:8086/shutdown?token=认证信息
while [[ $pid != "" ]]; do
    echo '服务停止中...'
    sleep 1
    pid=`ps ax | grep fangjia-youfang-web | grep java | head -1 | awk '{print $1}'`
done
echo '服务停止成功,开始重启服务...'
java -jar xxx.jar
```
在重启之前首先发送重启命令到endpoint，或者用kill 进程ID的方式，千万不要用kill -9。
然后循环检测进程是否存在，如果服务正常停止了，进程也就不存在了，如果进程还在，证明还有未处理完的请求，停止1秒，继续检测。
关于重启服务，建议用kill方式，这样就不用依赖spring-boot-starter-actuator，如果用endpoint方式，则需要控制好权限，不然随时都有可能被人重启了，可以用security来控制权限，我这边是自己用过滤器来控制的。
如果用actuator方式重启的话需要配置启用重启功能:
1.x配置如下：
```
endpoints.shutdown.enabled=true
```
2.x配置就比较多了，默认只暴露了几个常用的，而且访问地址也有变化，比如health, 以前是直接访问/health,现在需要 /actuator/health才能访问。我们可以通过配置来兼容之前的访问地址。
shutdown默认是不暴露的，可以通过配置暴露并开始，配置如下：
```
#访问路径，配置后就和1.x版本路径一样
management.endpoints.web.base-path=/
# 暴露所有，也可以暴露单个或多个
management.endpoints.web.exposure.include=*
# 开启shutdown
management.endpoint.shutdown.enabled=true
```
文档请参考：https://docs.spring.io/spring-boot/docs/2.0.2.RELEASE/reference/htmlsingle/#production-ready
# 如何测试
测试的话我们可以写一个简单的接口，在接口中等待，然后执行脚本停止项目，如果正常的话会输出服务停止中，等到你的接口执行完成，进程才会消失掉，但是如果超过了你配置的等待时间就会强行退出。
```
@GetMapping("/hello")
public String hello() {
    System.out.println("req.........");
    try {
        Thread.sleep(1000 * 60 * 3);
    } catch (InterruptedException e) {
        e.printStackTrace();
    }
    return "hello";
}
```
# 需要注意的问题
如果你的项目中有用到其他的线程池，比如Spring的ThreadPoolTaskExecutor，不熟悉的同学可以参考我的这篇文章《Spring Boot Async异步执行》
在发送停止命令后如果ThreadPoolTaskExecutor有线程还没处理完的话，这个时候进程是不会自动关闭的。这个时候我们需要对线程池进行关闭处理，增加代码如下：
```
AsyncTaskExecutePool asyncTaskExecutePool = event.getApplicationContext().getBean(AsyncTaskExecutePool.class);
Executor executors = asyncTaskExecutePool.getAsyncExecutor();
try {
      if (executors instanceof ThreadPoolTaskExecutor) {
           ThreadPoolTaskExecutor threadPoolExecutor = (ThreadPoolTaskExecutor) executors;
           log.info("Async shutdown start");
           threadPoolExecutor.setWaitForTasksToCompleteOnShutdown(true);
           threadPoolExecutor.setAwaitTerminationSeconds(waitTime);
           threadPoolExecutor.shutdown();
      }
} catch (Exception ex) {
     Thread.currentThread().interrupt();
}
```
ThreadPoolTaskExecutor只有shutdown方法，没有awaitTermination方法，通过查看源码，在shutdown之前设置setWaitForTasksToCompleteOnShutdown和setAwaitTerminationSeconds同样能实现awaitTermination。
源码如下：
```
public void shutdown() {
        if (logger.isInfoEnabled()) {
            logger.info("Shutting down ExecutorService" + (this.beanName != null ? " '" + this.beanName + "'" : ""));
        }
        if (this.executor != null) {
            if (this.waitForTasksToCompleteOnShutdown) {
                this.executor.shutdown();
            }
            else {
                for (Runnable remainingTask : this.executor.shutdownNow()) {
                    cancelRemainingTask(remainingTask);
                }
            }
            awaitTerminationIfNecessary(this.executor);
        }
    }
```
当waitForTasksToCompleteOnShutdown为true的时候就直接调用executor.shutdown();，最后执行awaitTerminationIfNecessary方法。
```
private void awaitTerminationIfNecessary(ExecutorService executor) {
        if (this.awaitTerminationSeconds > 0) {
            try {
                if (!executor.awaitTermination(this.awaitTerminationSeconds, TimeUnit.SECONDS)) {
                    if (logger.isWarnEnabled()) {
                        logger.warn("Timed out while waiting for executor" +
                                (this.beanName != null ? " '" + this.beanName + "'" : "") + " to terminate");
                    }
                }
            }
            catch (InterruptedException ex) {
                if (logger.isWarnEnabled()) {
                    logger.warn("Interrupted while waiting for executor" +
                            (this.beanName != null ? " '" + this.beanName + "'" : "") + " to terminate");
                }
                Thread.currentThread().interrupt();
            }
        }
    }
```
awaitTerminationIfNecessary中会判断属性awaitTerminationSeconds 如果与值的话就执行关闭等待检测逻辑，跟我们处理tomcat关闭的代码是一样的。
发现这样做之后好像没什么效果，于是我换了一种写法，直接通过获取ThreadPoolTaskExecutor中的ThreadPoolExecutor来执行关闭逻辑：
```
AsyncTaskExecutePool asyncTaskExecutePool = event.getApplicationContext().getBean(AsyncTaskExecutePool.class);
Executor executors = asyncTaskExecutePool.getAsyncExecutor();
try {
      if (executors instanceof ThreadPoolTaskExecutor) {
            ThreadPoolTaskExecutor threadPoolExecutor = (ThreadPoolTaskExecutor) executors;
            log.info("Async shutdown start");
            threadPoolExecutor.getThreadPoolExecutor().shutdown();
            log.info("Async shutdown end"+threadPoolExecutor.getThreadPoolExecutor().isTerminated());
            if (!threadPoolExecutor.getThreadPoolExecutor().awaitTermination(waitTime, TimeUnit.SECONDS)) {
                log.info("Tomcat 进程在" + waitTime + "秒内无法结束，尝试强制结束");
            }
            log.info("Async shutdown success");
       }
} catch (Exception ex) {
      Thread.currentThread().interrupt(); 
}
```
这是方式也没用达到我想要的效果，当我发出kill命令之后，直接就退出了，其实我有一个后台线程在ThreadPoolTaskExecutor中运行，通过输出的日志看到，只要调用了shutdown，isTerminated方法返回的就是true,说已经关闭了，这块还没找到原因，有研究出来的小伙伴还请分享出来。
END
更多精彩内容，尽在阅读原文
