# WebSocket的使用（基于VUE与SpringBoot） - weixin_33985507的博客 - CSDN博客
2018年12月05日 10:53:40[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
WebSocket 是 HTML5 开始提供的一种在单个 TCP 连接上进行全双工通讯的协议。
WebSocket 使得客户端和服务器之间的数据交换变得更加简单，允许服务端主动向客户端推送数据。在 WebSocket API 中，浏览器和服务器只需要完成一次握手，两者之间就直接可以创建持久性的连接，并进行双向数据传输。
在 WebSocket API 中，浏览器和服务器只需要做一个握手的动作，然后，浏览器和服务器之间就形成了一条快速通道。两者之间就直接可以数据互相传送。
现在，很多网站为了实现推送技术，所用的技术都是 Ajax 轮询。轮询是在特定的的时间间隔（如每1秒），由浏览器对服务器发出HTTP请求，然后由服务器返回最新的数据给客户端的浏览器。这种传统的模式带来很明显的缺点，即浏览器需要不断的向服务器发出请求，然而HTTP请求可能包含较长的头部，其中真正有效的数据可能只是很小的一部分，显然这样会浪费很多的带宽等资源。
HTML5 定义的 WebSocket 协议，能更好的节省服务器资源和带宽，并且能够更实时地进行通讯。
详细的WebSocket介绍请参考[菜鸟教程WebSocket](http://www.runoob.com/html/html5-websocket.html)
因为近期所使用的技术栈为VUE和SpringBoot，因此此文章所用技术环境也为VUE以及SpringBoot下。
建议先在后端（SpringBoot）配置好WebSocket。
**maven依赖**（因为我的SpringBoot项目为2.0以上，会自动选择最优版本，因此此处没有带上版本号）：
```
<dependency>
   <groupId>org.springframework.boot</groupId>
   <artifactId>spring-boot-starter-websocket</artifactId>
</dependency>
```
**WebSocket配置类**：
```
@Configuration
public class WebSocketConfig {
     /**
     * 注入ServerEndpointExporter，
     * 这个bean会自动注册使用了@ServerEndpoint注解声明的Websocket endpoint
     */
    @Bean
    public ServerEndpointExporter serverEndpointExporter() {
        return new ServerEndpointExporter();
    }
    
}
```
**WebSocket操作类**：
`WebSocket的向用户推送可以为向所有用户推送以及单点推送`
```
@Component
@ServerEndpoint("/websocket/{shopId}")
//此注解相当于设置访问URL
public class WebSocket {
    
    private Session session;
    
    private static CopyOnWriteArraySet<WebSocket> webSockets =new CopyOnWriteArraySet<>();
    private static Map<String,Session> sessionPool = new HashMap<String,Session>();
    
    @OnOpen
    public void onOpen(Session session, @PathParam(value="shopId")String shopId) {
        this.session = session;
        webSockets.add(this);
        sessionPool.put(shopId, session);
        System.out.println("【websocket消息】有新的连接，总数为:"+webSockets.size());
    }
    
    @OnClose
    public void onClose() {
        webSockets.remove(this);
        System.out.println("【websocket消息】连接断开，总数为:"+webSockets.size());
    }
    
    @OnMessage
    public void onMessage(String message) {
        System.out.println("【websocket消息】收到客户端消息:"+message);
    }
    
    // 此为广播消息
    public void sendAllMessage(String message) {
        for(WebSocket webSocket : webSockets) {
            System.out.println("【websocket消息】广播消息:"+message);
            try {
                webSocket.session.getAsyncRemote().sendText(message);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
    // 此为单点消息
    public void sendOneMessage(String shopId, String message) {
        Session session = sessionPool.get(shopId);
        if (session != null) {
            try {
                session.getAsyncRemote().sendText(message);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    
}
```
**在Controller中使用**
```
@RestController
@RequestMapping("api")
public class TestController {
    @Autowired
    private WebSocket webSocket;
    @RequestMapping("/sendAllWebSocket")
    public String test() {    
        webSocket.sendAllMessage("清晨起来打开窗，心情美美哒~");
        return "websocket群体发送！";        
    }
    
    @RequestMapping("/sendOneWebSocket")
    public String sendOneWebSocket() {
        webSocket.sendOneMessage("DPS007", "只要你乖给你买条gai！");
        return "websocket单人发送";
    }
}
```
**在前端中（VUE）使用WebSocket**
```
当然不在vue中使用也是一样的，只不过要注意的是WebSocket在普通js中如何创建以及销毁
```
```
<script>
    export default {
        data() {
            return {
                shopId：''
            }
        },
        created() { // 页面创建生命周期函数
              this.initWebSocket()
        },
        destroyed: function () { // 离开页面生命周期函数
              this.websocketclose();
        },
        methods: {
            collapse: function(){
                this.isCollapse = !this.isCollapse;
                if (this.isCollapse) {
                    this.iconClass = "cebianlanzhankai";
                } else{
                    this.iconClass = "cebianlanshouhui";
                }
            },
            initWebSocket: function () {
                // WebSocket与普通的请求所用协议有所不同，ws等同于http，wss等同于https
                this.websock = new WebSocket("ws://localhost:8046/websocket/DPS007");
                this.websock.onopen = this.websocketonopen;
                this.websock.onerror = this.websocketonerror;
                   this.websock.onmessage = this.websocketonmessage;
                this.websock.onclose = this.websocketclose;
              },
              websocketonopen: function () {
                console.log("WebSocket连接成功");
              },
              websocketonerror: function (e) {
                console.log("WebSocket连接发生错误");
              },
              websocketonmessage: function (e) {
                var da = JSON.parse(e.data);
                console.log(da);
                this.message = da;
              },
              websocketclose: function (e) {
                console.log("connection closed (" + e.code + ")");
              }
        }
    }
</script>
```
