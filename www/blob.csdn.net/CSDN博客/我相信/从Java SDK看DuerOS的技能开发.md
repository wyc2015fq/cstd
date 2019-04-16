# 从Java SDK看DuerOS的技能开发 - 我相信...... - CSDN博客





2019年03月22日 17:30:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：15标签：[人工智能																[AI																[DuerOS																[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)
个人分类：[AI](https://blog.csdn.net/wireless_com/article/category/7470805)






DuerOS是对话式AI系统，既然都已经是人工智能了，为什么还要在DuerOS上开发技能服务呢？![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrlBoejv0OiahgibM9HAssWcFVRvhibGuciaxxH6ourb8nb4nXtw97zZrCtG8lTl3AWf0JFFibhQQ6EicGQ/640?wx_fmt=jpeg)

温故知新，我们还是要重新审视一下AI，具体可以参见《[老码农眼中的简明A](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658971818&idx=1&sn=aea14e255fc35689b19f5d0fbc281877&chksm=80d32d8cb7a4a49a6b509aab10c49440616436ffbd28ba1d9450e0552f49520c4d98a5b15d21&scene=21#wechat_redirect)I》。从编程的角度看，面向人工智能的应用大约由三部分组成：领域知识、数学算法和计算方式。数学算法和计算已经由类似DuerOS这样的AI系统提供了，但是领域知识涉及到具体的行业知识，业务逻辑，专业术语等等，尤其是业务逻辑，AI系统也难以做到面面俱到。做一个不太恰当的类比，互联网拥有了web技术，但是具体的Web应用服务还是需求开发的，面向对话式AI系统的技能服务也是类似。

## 面向人的交互

《[面向协议的DuerOS技能开发](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974063&idx=1&sn=8d15d4d764f6237a359556e6714e7943&chksm=80d32449b7a4ad5f43da9ce81a684d01795c2a8e40d27807dc036f661a9d7dadd2432e2cffd5&scene=21#wechat_redirect)》一文中谈到，技能开发基本上可以理解为Web服务开发，只是用户交互的方式发生了改变。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrlBoejv0OiahgibM9HAssWcFX9c1F4Ykq11XU4RSUxdAIUHUcOmFnq4jjfZwIJcSIUHoKQmmunWwhA/640?wx_fmt=jpeg)

电脑/手机等原来使用键盘/鼠标/触摸屏完成输入，用显示屏幕完成输出，现在基于DuerOS的小度系列产品使用语音对话完成输入，使用扬声器完成音频输出，当然，有屏设备同时支持了原有的功能。

简单地说，传统的交互方式是面向机器的，DuerOS的交互方式是面向人的。

## DBP的SDK

为了方便开发者高效地完成技能服务的开发，DuerOS Bot Platform （DBP）提供了多种语言的SDK：Java，JavaScript，PHP，Go以及Python。对程序员而言，实现一个基于HTTP的协议，并不是一个很轻松的事，而使用DBP的SDK，则可以极大的提升开发效率。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrlBoejv0OiahgibM9HAssWcFicicRhZAal5ShuNY7y4Ivez9byqXUuvKEfQd5Hy6ib3eSXId0mwaW4jNg/640?wx_fmt=jpeg)

我们可以在https://github.com/dueros看到关于DuerOS的各种语言SDK，而Java SDK 就成为了Java程序员的福音。

DBP的Bot-SDK-Java提供了一下主要功能：
- 
封装了DuerOS的request和response

- 
提供了session的简化接口

- 
提供了nlu简化接口

- 
提供了多轮对话开发接口

- 
提供了事件监听接口


需要注意的是，DBP的Java版SDK 需要在Java 8 及以上版本运行，采用Maven作为工程管理工具，同时DBP Java SDK的升级、维护也都通过Maven进行发布，在pom.xml中添加最新版本依赖的示例如下：

```
<dependency>
    <groupId>com.baidu.dueros</groupId>
    <artifactId>bot-sdk</artifactId>
    <version>1.1.8</version>
</dependency>
```

## DBP Java SDK 浅析

从DBP Java SDK 的POM 文件中可以看到其中的依赖：
- 
JUnit 4.11

- 
Javax.serverlet 3.0.1

- 
Jackson 2.9.7

- 
commons-codec 1.6

- 
commons-io 2.4

- 
commons-digester 2.1

- 
百度的APM SDK 1.1.0


这与一般的Java 工程差别不大，唯一有点特殊的，可能算是APM的SDK了。 对于微服务而言，APM SDK 提供了类似sidecar之类的能力。

DBP Java SDK 工程结构主体包括：
- 
bot：技能服务的基类

- 
certificate：资源证书的实现

- 
data：DBP协议的实体封装

- 
model：对请求和响应的封装

- 
nlu：对槽位和意图的实现

- 
samples： 示例代码


从命名来看，除了data稍显歧义，其他都基本上可以见字知意。

### DBP协议的主体——请求和响应

Model目录的request.java 和 response.java 实现了DBP协议中请求和响应的封装。

从面向对象的角度看Request的组成大体如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczrlBoejv0OiahgibM9HAssWcF9qQM6mGibpT4U2NaKXoibEDPKec2ibL7052IYuicL5c2kS0Ljb9QJtlU5w/640?wx_fmt=png)

而Response的响应大体是这样的：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczrlBoejv0OiahgibM9HAssWcFXWcaZeets43cG1IoIQFpibwvpFUST2ywxibCo6tsAFFVbV1NyE1iajC3w/640?wx_fmt=png)

各成员变量的意义可以参见《在面向协议的DuerOS技能开发》。DBP协议中的实体分别位于Data目录中的Request 和 Response Package。下面逐一看一下代码中实现的协议实体。

### DBP协议Request中的实体

Request中的数据实体可以用户相关，设备相关，对话相关以及消息事件。

#### 用户相关

Account类承载了百度账号的信息，也就是说整个DBP平台的用户体系是基于百度账号的，技能平台可以基于百度账号进行用户的绑定以及用户系统的贯通。

CoordinatesInfo类表达了位置的经纬度，而GeoLocation类承载了不同坐标系的用户位置信息，是LocationInfo 的成员变量。

#### 应用服务相关

AccessToken类用于实现DuerOS设备与DuerOS技能之间的鉴权。

Application类表明了技能服务也就是bot的标识，开发者通过DBP管理平台生成的bot-id就是这里的ApplicationID，而 Appinfo类描述了终端伴侣app的软件信息，成员变量包括包括技能名称，应用包名，版本号及版本名称。

Skillinfo 用于在有屏终端和App 伴侣上的技能呈现，包括了图标Icon类和技能的名称。

#### 终端设备的属性特征

Device类和DeviceInfo类描述了基于DuerOS的终端终端信息，成员变量包括终端的ID以及终端的硬件能力即supportedInterface：

```
public class SupportedInterfaces {

    private TextInput textInput;
    private VoiceInput voiceInput;
    private VoiceOutput voiceOutput;
    private PlayController playController;
    private AudioPlayer audioPlayer;
    private Alerts alerts;
    private com.baidu.dueros.data.request.supportedInterfaces.Screen screen;
    private SpeakerController speakerController;
    private com.baidu.dueros.data.request.supportedInterfaces.System system;
    private ScreenExtendedCard screenExtendedCard;
    private VideoPlayer videoPlayer;
    private Display display;
    ....
```

Context是一个比较重要的类，成员变量包括：
- 
System类：系统信息包括Application，User，Device以及apiAccessToken和apiEndPoint 

- 
音/视频播放器状态 

- 
显示屏幕的信息。


#### 对话请求相关

RequestBody类封装了http post中的body，是个基础类，继承关系如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczrlBoejv0OiahgibM9HAssWcFapPyswtGn8Nu0ib6WfzxwqywQoRThmvPiaqygq0am9Dh50SZxslvPEvg/640?wx_fmt=png)

LaunchRequest是技能服务生命周期的开始，SessionEndedRequest是技能服务生命周期的终止，IntentRequest 则对应着技能服务处理的消息循环。

IntentRequest 中的成员Query是DuerOS设备语音识别后的结果，成员DialogState代表对话的状态，对应的意图以列表方式表达。

人机交互的对话是Dialog，而会话是session。这里的Session是另一个重要的类，Session ID 是本次多轮对话的唯一标识，更重要的是Session类中属性列表：
`private Map<String, String> attributes = new HashMap<String, String>();`
该属性列表实现了类似浏览器cookie的功能。

#### 事件

事件是DBP协议中从DuerOS设备端发往技能服务的消息，目前包括显示，音/视频播放器，支付和用户授权事件。显示相关的继承关系如下：

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczrlBoejv0OiahgibM9HAssWcFvpVJibjx5gBicjCurTgkR6DqkGJDHaVzcQJp0Iq9G80WonbDXrbia5OUA/640?wx_fmt=png)

LinkAccountSucceededEvent类对应与协议中的Connections.Response事件，当用户授权完成之后，技能服务会收到此事件。

ButtonClickedEvent和 RadioButtonClickedEvent了实现了Form.ButtonClicked和Form.RadioButtonClicked事件，当用户在有屏设备的表单上点击按钮/单选按钮，技能服务会收到此事件。

ElementSelectedEvent类实现了Display.ElementSelected事件，当用户在有屏设备的列表模版上选择了某一项时，技能服务会收到此事件。

LinkClickedEvent类实现了Screen.LinkClicked事件，如果在卡片或者卡片列表配置了URL地址，当用户点击卡片或者卡片列表时，技能服务会收到此事件。

在Audioplayer 的事件中，AudioPlayerEvent继承自httpbody，而其他事件类均继承自AudioPlayerEvent。
|事件名|对应的类|
|----|----|
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
AudioPlayer.PlaybackFailedPlaybackFailedEventAudioPlayer.PlaybackFinishedPlaybackFinishedEventAudioPlayer.PlaybackNearlyFinishedPlaybackNearlyFinishedEventAudioPlayer.PlaybackPausedPlaybackPausedEventAudioPlayer.PlaybackResumedPlaybackResumedEventAudioPlayer.PlaybackStartedPlaybackStartedEventAudioPlayer.PlaybackStoppedPlaybackStoppedEventAudioPlayer.PlaybackStutterFinishedPlaybackStutterFinishedEventAudioPlayer.PlaybackStutterStartedPlaybackStutterStartedEventAudioPlayer.ProgressReportDelayElapsedProgressReportDelayElapsedEventAudioPlayer.ProgressReportIntervalElapsedProgressReportIntervalElapsedEvent

类似的，在videoplayer 的事件中，VideoPlayerEvent继承自httpbody，而其他事件类均继承自VideoPlayerEvent。
|事件名|对应的类|
|----|----|
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
| | |
VideoPlayer.PlaybackFinishedPlaybackFinishedEventVideoPlayer.PlaybackNearlyFinishedPlaybackNearlyFinishedEventVideoPlayer.PlaybackPausedPlaybackPausedEventVideoPlayer.PlaybackQueueClearedPlaybackQueueClearedEventVideoPlayer.PlaybackResumedPlaybackResumedEventVideoPlayer.PlaybackStartedPlaybackStartedEventVideoPlayer.PlaybackStoppedPlaybackStoppedEventVideoPlayer.PlaybackStutterFinishedPlaybackStutterFinishedEventVideoPlayer.PlaybackStutterStartedPlaybackStutterStartedEventVedioPlayer.ProgressReportDelayElapsedProgressReportDelayElapsedEventVideoPlayer.ProgressReportIntervalElapsedProgressReportIntervalElapsedEvent
支付及应用内付费涉及到具体的商业模式，ChargeEvent.java 是主要的支付事件，继承自RequestBody类，包括了支付指令的token和详细的支付信息。

凡是涉及到用户隐私的数据或者操作均需用户授权，与用户授权相关的事件有：

```
PermissionGrantFailedEvent.java 
PermissionGrantedEvent.java 
PermissionRejectedEvent.java    
PermissionRequiredEvent.java
```

### DBP协议Response中的实体

ResponseBody类是响应的核心：

```
public class ResponseBody {

    // 如果DuerOS仍然会选用当前Bot结果，应该再次下发请求，并将request.determined字段设置为true
    private boolean needDetermine;
    // 表示本次返回的结果是否为兜底结果
    private boolean fallBack;
    // 表示本次返回结果中需要播报的语音信息
    private OutputSpeech outputSpeech;
    // 在需要用户输入时，如果用户没有输入或用户输入内容系统不理解，则播报reprompt内容
    private Reprompt reprompt;
    // Bot输出的Resource内容
    private Resource resource;
    // Bot输出的Card内容，用于在有屏场景下展示
    private Card card;
    // Bot输出的指令，分为： 对DuerOS指令主要是对话指令;其他都是对端的指令
    private List<Directive> directives = new ArrayList<>();
    // 是否需要结束本次会话，DuerOS用于判断是否需要关闭某个打开的Bot，端用于关闭麦克风
    private boolean shouldEndSession;
    // 麦克风是否开启
    private boolean expectSpeech;
    ......
```

#### 输出相关

Card类主要应用于有屏设备的展示，是个抽象类，派生出多种卡片展示：
- 
ImageCard.java

- 
LinkAccountCard.java

- 
ListCard.java

- 
StandardCard.java

- 
TextCard.java


语音的输出主要是通过OutputSpeech类来实现，Reprompt的应用场景不同，但在实现上封装了OutputSpeech类。

对于具体的播放资源实体，通过Resource类实现，可以使用各种Entity的各种数据对象，尤其要注意Entity中的token，错误的token设置可能会导致资源无法播放。

#### 会话相关

Session类与Request package中的Session类类似，主要是HashMap的session attribute 列表。

#### 技能服务的指令

和Event 对应， 技能服务返回一系列指令（directive）使DuerOS的设备端完成对应的操作。Directive 是一个抽象类，其他的各种directive大都继承于此。

音视频播放器都保护play.java和stop.java 指令，但是视频播放还有一个ClearQueue.java 的指令。同时，二者都有对媒体内容的封装，例如Stream，AudioItem，VedioItem等。

与对话相关的指令都继承自DialogDirective类，指令对应的类文件如下所示：
|事件名|对应的类|
|----|----|
| | |
| | |
| | |
| | |
Dialog.ElicitSlotElicitSlot.javaDialog.DelegateDelegate.javaDialog.ConfirmIntentConfirmIntent.javaDialog.ConfirmSlotConfirmSlot
需要注意的是，Delegate代理的是处理逻辑，话术及意图需要通过DBP 平台进行配置。

用户授权的指令包括permission类和AskForPermissionsConsent类。 目前，DBP平台提高了4种类型的用户授权：

```
public static final String READ_USER_PROFILE = "READ::USER:PROFILE";
    public static final String READ_DEVICE_LOCATION = "READ::DEVICE:LOCATION";
    public static final String WRITE_SMARTHOME_PRINTER = "WRITE::SMARTHOME:PRINTER";
    public static final String RECORD_SPEECH = "RECORD::SPEECH";
```

当然，这些授权可以组合传递。

DBP 提供了轻量级的支付体系，类ChargeBaiduPay实现了主要的支付功能，SellerOrderAttributes 是订单的信息。也就是说， DBP 是通过百度的聚合收银台实现了支付特性，用户可以通过支付宝、微信和百度钱包完成支付。

对DuerOS的有屏设备如小度在家而言，DBP的SDK还有完成触摸屏上的人机交互，主要指令只有一个Hint。但是，为了使开发更为高效，在Display.templates中提供了大量的模版可以使用。

![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqZB5Wo1ciaWTgia4Idd0ZesPA3cuxibRCiaicYCq6y3ScErd8LHf7lJZpZ5RGMRQVgAAicvkrPKVdgicyrw/640?wx_fmt=png)

### NLU相关

NLU的模块主要提供了Intent类和Slot类， 用于描述意图和槽位。

```
public class Intent {

    // 意图名称
    private final String name;
    // 意图确认状态
    private final ConfirmationStatus confirmationStatus;
    // 意图中的槽位
    private final Map<String, Slot> slots;
    // 意图置信度
    private final int score;
    ......
```

关于意图和槽位的具体含义，可以参见《[感知自然语言理解（NLU）](http://mp.weixin.qq.com/s?__biz=MzAwOTcyNzA0OQ==&mid=2658974048&idx=1&sn=8890276ba789c43d295333f4d926f282&chksm=80d32446b7a4ad502d94271f67c96c55371db52df85963367c0b65ec5dbfa77477ff56f30b81&scene=21#wechat_redirect)》一文。

### 技能服务的载体——Bot

Bot 是技能服务的载体，BaseBot是所有Bot的基类，使用Bot-SDK开发的Bot都需要继承这个类。

```
public class BaseBot {

    // Base子类构造的Response
    protected Response response;
    // Bot收到的Request请求
    private final Request request;
    // 会话信息
    private Session session = new Session();
    // 是否需要结束本次会话
    private boolean shouldEndSession = false;
    // 如果DuerOS仍然会选用当前Bot结果，应该再次下发请求，并将request.determined字段设置为true
    private boolean needDetermine = false;
    // 麦克风开关是否打开
    private boolean expectSpeech = false;
    // 返回的指令
    private List<Directive> directives = new ArrayList<Directive>();
    // 通过NLU解析出来的Intent
    private Intent intent;

    // 是否打开参数验证，默认为false
    private boolean enableCertificate = false;
    // afterSearchScore
    private float afterSearchScore = 1.0f;
    private List<ExpectResponse> expectResponses = new ArrayList<>();
    // 认证签名
    private Certificate certificate;
    // 缓存认证相关信息
    private static ConcurrentHashMap<String, PublicKey> cache = new ConcurrentHashMap<>();
    // 数据统计信息
    public BotMonitor botMonitor;
    ......
```

BaseBot通过一系列方法完成了对Request/Response的读取和设置，可以通过Serverlet的request来构造BaseBot：

```
protected BaseBot(HttpServletRequest request) throws IOException, JsonMappingException {
        certificate = new Certificate(request);
        String message = certificate.getMessage();
        ObjectMapper mapper = new ObjectMapper();
        this.request = mapper.readValue(message, Request.class);
        this.botMonitor = new BotMonitor(message);
        this.session.getAttributes().putAll(this.request.getSession().getAttributes());
    }
```

BaseBot 还完成了事件的分发，证书验证等功能，是主要的逻辑处理引擎。

AudioPlayer和VideoPlayer继承自BaseBot，主要对DuerOS设备端上传的事件进行具体处理。

## DBP Java SDK 的应用

建议采用Maven 来构建工程，这样可以避免库版本的不一致性带来的困扰。

通过DBP Java SDK 构建一个DuerOS技能和开发一个web服务没什么区别，只需要实现对应的Action 和自己的技能服务逻辑即可。

以sample中的个税计算器为例，只有TaxAction.java和TaxBot.java两个文件，但可以看到实现的要点。

### Action的实现

TaxAction 继承自HttpServlet，但只实现了http中的3个方法：Head，GET和POST。

在POST方法中实现了技能服务的入口：

```
protected void doPost(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        // 根据request创建Bot
        TaxBot bot;
        try {
            bot = new TaxBot(request);
            // 线下调试时，可以关闭签名验证
            // bot.enableVerify();
            bot.disableVerify();

            // 调用bot的run方法
            String responseJson = bot.run();
            // 设置response的编码UTF-8
            response.setCharacterEncoding("UTF-8");
            // 返回response
            response.getWriter().append(responseJson);

            // 打开签名验证
            // bot.enableVerify();

        } catch (Exception e) {
            e.printStackTrace();
            response.getWriter().append(e.toString());
        }
    }
```

其中，在测试环境中，可以关闭签名的验证。

### TaxBot的实现

TaxBot继承自BaseBot，onLaunch()方法是启动对话的入口：

```
protected Response onLaunch(LaunchRequest launchRequest) {
        // 新建文本卡片
        TextCard textCard = new TextCard("所得税为您服务");
        // 设置链接地址
        textCard.setUrl("www:....");
        // 设置链接内容
        textCard.setAnchorText("setAnchorText");
        // 添加引导话术
        textCard.addCueWord("欢迎进入");

        // 新建返回的语音内容
        OutputSpeech outputSpeech = new OutputSpeech(SpeechType.PlainText, "所得税为您服务");

        // 构造返回的Response
        Response response = new Response(outputSpeech, textCard);
        return response;
    }
```

onInent()方法实现了DuerOS处理后的语音意图：

```
@Override
protected Response onInent(IntentRequest intentRequest) {

    // 判断NLU解析的意图名称是否匹配 inquiry
    if ("inquiry".equals(intentRequest.getIntentName())) {
        // 判断NLU解析解析后是否存在这个槽位
        if (getSlot("monthlysalary") == null) {
            // 询问月薪槽位monthlysalary
            ask("monthlysalary");
            return askSalary();
        } else if (getSlot("location") == null) {
            // 询问城市槽位location
            ask("location");
            return askLocation();
        } else if (getSlot("compute_type") == null) {
            // 询问查询种类槽位compute_type
            ask("compute_type");
            return askComputeType();
        } else {
            // 具体计算方法
            return compute();
        }
    }
    return null;
}
```

onSessionEnded()方法用于结束会话：

```
@Override
    protected Response onSessionEnded(SessionEndedRequest sessionEndedRequest) {

        // 构造TextCard
        TextCard textCard = new TextCard("感谢使用所得税服务");
        textCard.setAnchorText("setAnchorText");
        textCard.addCueWord("欢迎再次使用");

        // 构造OutputSpeech
        OutputSpeech outputSpeech = new OutputSpeech(SpeechType.PlainText, "欢迎再次使用所得税服务");

        ListCard listCard = new ListCard();
        StandardCardInfo item1 = new StandardCardInfo("title1", "content1");
        StandardCardInfo item2 = new StandardCardInfo("title2", "content2");
        listCard.addStandardCardInfo(item1);
        listCard.addStandardCardInfo(item2);

        // 构造Response
        Response response = new Response(outputSpeech, textCard);

        return response;
    }
```

具体的，askLocation()和askComputeType()完成槽位的填充，compute（）完成具体的个税计算。

当然，开发技能服务需要从开发者注册开始，还需要在DBP的管理平台对意图技能配置单很简单，从注册到配置技能在10分钟内可以完成，详情参考官网：dueros.baidu.com/dbp

## 小结

为了方便DBP协议实现，可以使用DBP SDK 提高开发的效率，理解SDK的实现方式和原理对于SDK的使用有着很大的帮助，从而，开发者可以方便且高效地发布自己的技能服务。对于尝鲜的用户而言，可以尝试小技能的开发。无需编程，几分钟就可能实现一个自己的小技能。

当然，各种编程语言的DBP SDK还在持续演进中，值得期待。

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrlBoejv0OiahgibM9HAssWcFL2LicrpblGQWjTpskymUnOEUkqq94Dj4pZaRWaxIUUAh7oXsS6b7kKg/640?wx_fmt=jpeg)

##### 参考
- 
https://dueros.baidu.com/dbp

- 
https://github.com/dueros/bot-sdk-java](https://so.csdn.net/so/search/s.do?q=DuerOS&t=blog)](https://so.csdn.net/so/search/s.do?q=AI&t=blog)](https://so.csdn.net/so/search/s.do?q=人工智能&t=blog)




