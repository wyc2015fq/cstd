# 怎样理解 REST、RESTful - Nicole的博客 - CSDN博客
2018年04月11日 18:02:21[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：528
本文虽然是参考别人的内容，但是是作者花很多心思整理出来，整理本文章的目的是为了自己可以好理解REST和方便其他人理解，如果要转载，请征求作者意见！
**一、什么是API？**
    为了解释清楚REST，必须理解API。API（Application Programming Interface,应用程序编程接口），通俗来讲就是像一个公司比如腾讯、阿里之类，他们可以提供一个API，然后我们或者其他的小公司可以编一个软件去跟这个接口（API）进行相连或者交互。举个例子，比如你可以用手机的其他软件分享内容到微信朋友群或者新浪微微博，这些软件就是与微信和微博的API进行交互。
## 二、什么是WEB？
      为了解释清楚什么是REST架构风格，必须提到WEB，因为REST是以Web为平台的。Web是分布式信息系统为超文本文件和其他对象（资源）提供访问入口。其中资源是Web架构的关键点，需要以下3个操作，识别（identity）、表示（represent）、交互（interact with），通过这三个操作，有引出三个概念URI（统一资源标识符包括URL和URN）是用来识别资源，representation（例如html、xml、图片、视频等等）是用来表示资源，通过协议（包括http、ftp等等）来与资源进行交互。
## 三、什么是URI和URL？
 （1） 统一资源标志符URI
         就是在某一规则下能把一个资源独一无二地标识出来。拿人做例子，假设这个世界上所有人的名字都不能重复，那么名字就是URI的一个实例，通过名字这个字符串就可以标识出唯一的一个人。现实当中名字当然是会重复的，所以身份证号才是URI，通过身份证号能让我们能且仅能确定一个人。
（2）统一资源定位符URL
       也拿人做例子然后跟HTTP的URL做类比，就可以有：动物住址协议://地球/中国/浙江省/杭州市/西湖区/某大学/14号宿舍楼/525号寝/张三.人可以看到，这个字符串同样标识出了唯一的一个人，起到了URI的作用，所以URL是URI的子集。URL是以描述人的位置来唯一确定一个人的。
         在上文我们用身份证号也可以唯一确定一个人。对于这个在杭州的张三，我们也可以用：身份证号：[123456789](tel:123456789)来标识他。所以不论是用定位的方式还是用编号的方式，我们都可以唯一确定一个人，都是URl的一种实现，而URL就是用定位的方式实现的URI。
        回到Web上，假设所有的Html文档都有唯一的编号，记作html:xxxxx，xxxxx是一串数字，即Html文档的身份证号码，这个能唯一标识一个Html文档，那么这个号码就是一个URI。
        而URL则通过描述是哪个主机上哪个路径上的文件来唯一确定一个资源，也就是定位的方式来实现的URI。对于现在网址我更倾向于叫它URL，毕竟它提供了资源的位置信息，如果有一天网址通过号码来标识变成了[http://741236985.html](https://link.zhihu.com/?target=http%3A//741236985.html)，那感觉叫成URI更为合适，不过这样子的话还得想办法找到这个资源咯
## 四、什么是REST？
REST全称是Resource REpresentational State Transfer，中文翻译是表现层状态转移，理解起来很困难。这里引用@Ivony老师的一句话概括很精辟：**URL定位资源，用HTTP动词（GET,POST,DELETE,DETC）描述操作。**REST是一种架构风格，腾讯公司或者其他公司建立API是都要遵守的一种规则/风格，当然也有其他的 规则可以使用。  下面是REST全称中每个单词的含义：  
        （1）Transfer：通俗来讲就是：资源在网络中以某种表现形式进行状态转移。分解开来：
        （2）Resource：资源，即数据（前面说过网络的核心）。比如 newsfeed，friends等；
        （3）Representational：某种表现形式，比如用JSON，XML，JPEG等；
        （4） State Transfer：状态变化。通过HTTP动词实现。
       所以REST就是通过使用http协议和URI，利用client/server model对资源进行CRUD（Create、Read、Updata、Delete）增加改查操作。
## 五、RESTful API
       REST描述的是在网络中client和server的一种交互形式；REST本身不实用，实用的是如何设计好的 RESTful API（REST风格的网络接口）。
为什么要使用RESTful结构呢？
       大家都知道"古代"网页是前端后端融在一起的，比如之前的PHP，JSP等。在之前的桌面时代问题不大，但是近年来移动互联网的发展，各种类型的Client层出不穷，RESTful可以通过一套统一的接口为 Web，iOS和Android提供服务。另外对于广大平台来说，比如Facebook platform，微博开放平台，微信公共平台等，它们不需要有显式的前端，只需要一套提供服务的接口，于是RESTful更是它们最好的选择。在RESTful架构下：
![](https://img-blog.csdn.net/20180411172914263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zOTU0MTU1OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 六、为什么要使用REST风格？
    我们使用REST风格肯定是有其的优点，要介绍它的优点又要提到它的留个限制，这里把它的限制和优点都列出来：
### 1.客户--服务器（Client--Server）
限制：客户端和服务器分离
优点：（1）提高用户界面的便携性（操作简单）
（2）通过简化服务器提高可伸缩性（高性能，低成本）
（3）允许组件分别优化（可以让服务器和客户端分别进行改进和优化）
### 2.无状态（Stateless）
限制：从客户端的每个请求要包含服务器所需要的所有信息
优点：（1）提高可见性（可以单独考虑每个请求）
（2）提高了可靠性（更容易从局部故障中修复）
（3）提高可扩展性（降低了服务器资源使用）
## 3.缓存（Cachable）
限制：服务器返回信息必须被标记是否可以缓存，如果缓存，客户端可能会重用之前的信息发送请求。
优点：（1）减少交互次数
（2）减少交互的平均延迟
### 4.分层系统（Layered System）--约束
限制：系统组件不需要知道与他交流组件之外的事情。封装服务，引入中间层。
优点：（1）限制了系统的复杂性
（2）提高可扩展性
### 5.统一接口（Uniform Interface）--约束
优点：（1）提高交互的可见性
（2）鼓励单独改善组件
### 6.支持按需代码（Code-On-Demand可选）---约束
优点：（1）提高可扩展性
## **七、理解REST的几点注意事项**
第一、REST不是英文单词“rest”,它们两者之间没有任何联系！！！！REST是 REpresentational State Transfer这几个单词的简称。
第二、Server提供的RESTful API中，URL中只使用名词来指定资源，原则上不使用动词。“资源”是REST架构或者说整个网络处理的核心。比如：
[http://api.qc.com/v1/newsfeed](https://link.zhihu.com/?target=http%3A//api.qc.com/v1/newsfeed): 获取某人的新鲜; 
[http://api.qc.com/v1/friends](https://link.zhihu.com/?target=http%3A//api.qc.com/v1/friends): 获取某人的好友列表;
[http://api.qc.com/v1/profile](https://link.zhihu.com/?target=http%3A//api.qc.com/v1/profile): 获取某人的详细信息;3. 用HTTP协议里的动词来实现资源的添加，修改，删除等操作。即通过HTTP动词来实现资源的状态扭转：
第三、用HTTP协议里的动词来实现资源的添加，修改，删除等操作。即通过HTTP动词来实现资源的状态扭转，HTTP标准的方法有如下：
（1）GET    用来获取资源，例如：http://example.com/api/orders，用来检索order list。
例如：http://example.com/api/orders/123用来获取某个特定资源的信息。
（2）POST  用来新建资源（也可以用于更新资源），例如：[http://api.qc.com/v1/](https://link.zhihu.com/?target=http%3A//api.qc.com/v1/friends)friends: 添加好友
（3）PUT    用来更新资源，例如：http://example.com/api/orders/123
（4）DELETE  用来删除资源。比如：[http://api.qc.com/v1/](https://link.zhihu.com/?target=http%3A//api.qc.com/v1/friends)friends: 删除某人的好友 （在http parameter指定好友id）
（5）UPDATE [http://api.qc.com/v1/profile](https://link.zhihu.com/?target=http%3A//api.qc.com/v1/profile): 更新个人资料
下面是API的图例：
![](https://img-blog.csdn.net/20180411171523105?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zOTU0MTU1OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
第四、 Server和Client之间传递某资源的一个表现形式，比如用JSON，XML传输文本，或者用JPG，WebP传输图片等。当然还可以压缩HTTP传输时的数据（on-wire data compression）。
第五、用 HTTP Status Code传递Server的状态信息。比如最常用的 200 表示成功，500 表示Server内部错误等。
本章内容参考知乎的问答：https://www.zhihu.com/question/28557115下的覃超和[houhaichao830](https://www.zhihu.com/people/houhaichao830)的回答。
链接：https://www.zhihu.com/question/21950864/answer/154309494
