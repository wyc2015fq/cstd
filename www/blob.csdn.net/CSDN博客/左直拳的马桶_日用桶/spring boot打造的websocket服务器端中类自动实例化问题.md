# spring boot打造的websocket服务器端中类自动实例化问题 - 左直拳的马桶_日用桶 - CSDN博客
2019年04月11日 19:11:03[左直拳](https://me.csdn.net/leftfist)阅读数：31
唉，真拗口啊这标题，也不知道说的对不对，好像很不专业的说法？大致是这么个意思吧。
总所周知，spring boot最喜欢最擅长干的事情就是标注。多复杂的事情，只要加个标注就好了。1个不够，就加2个。没有标注解决不了的问题。
比如说，类的实例化，只需加个 `@Autowired` 就好了：
```java
@Autowired
SocketService skService;
```
但是！在websocket 服务器端代码中，这种实例会失效，根本没有构造实例，skService为null。
为什么会这样呢？
原因据说是spring 为单例模式，而websocket是多对象，每个会话会创建一个websocket对象。导致除了第一个websocket，其他的都不能注入实例。
*本质原因：spring管理的都是单例（singleton），和 websocket （多对象）相冲突。详细解释：项目启动时初始化，会初始化 websocket （非用户连接的），spring 同时会为其注入 service，该对象的 service 不是 null，被成功注入。但是，由于 spring 默认管理的是单例，所以只会注入一次 service。当新用户进入聊天时，系统又会创建一个新的 websocket 对象，这时矛盾出现了：spring 管理的都是单例，不会给第二个 websocket 对象注入 service，所以导致只要是用户连接创建的 websocket 对象，都不能再注入了。*
原文：[https://blog.csdn.net/m0_37202351/article/details/86255132](https://blog.csdn.net/m0_37202351/article/details/86255132)
我寻寻觅觅，找到的答案都说是要在每个websocket里自己构造实例。试验之后，果真如此，代码如下：
websocket服务端
```java
import javax.websocket.*;
import javax.websocket.server.ServerEndpoint;
import java.io.IOException;
import java.util.concurrent.CopyOnWriteArraySet;
@ServerEndpoint("/ws")
@Component
public class CaptureSocket {
    private final static Logger LOGGER = LoggerFactory.getLogger(CaptureSocket.class);
    private static CopyOnWriteArraySet<CaptureSocket> set = new CopyOnWriteArraySet<>();
    private Session session;
	
    //关键代码，设置一个静态上下文属性appcontext
	private static ApplicationContext appcontext;
    public static void setAppcontext(ApplicationContext appcontext) {
        CaptureSocket.appcontext = appcontext;
    }
	public static ApplicationContext getAppcontext() {
        return appcontext;
    }
    @OnMessage
    public void onMessage(String message,Session session){
        LOGGER.info("来自客户端消息：" + message);
		//手动构造实例
		//这个构造实例，要放在事件里进行，如果放在代码开头，极有可能失败，可能是还没来得及拿到这个上下文对象？
		SocketService skService = appcontext.getBean(SocketServiceImpl.class);
    }
	@OnOpen
    public void onOpen(Session session) throws IOException,InterruptedException {
        this.session=session;
        set.add(this);
        LOGGER.info("新连接来自：" + session.getRequestURI().getHost());
    }
    @OnClose
    public void onClose(){
        set.remove(this);
        LOGGER.info("连接关闭：" + this.session.getRequestURI().getHost());
    }
    @OnError
    public void onError(Session session,Throwable err){
        LOGGER.info(err.getMessage());
        err.printStackTrace();
    }
}
```
在主程序入口里，将上下文对象保存下来，保存到websocket代码设置的静态属性appcontext里。如此，即可用该上下文对象来构造实例矣。
```java
@SpringBootApplication
@EnableAsync
public class OptcFfmpegApplication {
    
    public static void main(String[] args) {
        ConfigurableApplicationContext context = SpringApplication.run(OptcFfmpegApplication.class, args);
        CaptureSocket.setAppcontext(context);
    }
}
```
但是要注意，这个构造实例，要放在事件里进行，如果放在代码开头，极有可能失败，可能是还没来得及拿到这个上下文对象？
