# jms基础概念和应用场景 - z69183787的专栏 - CSDN博客
2015年06月09日 22:44:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：647
个人分类：[Mq-Jms](https://blog.csdn.net/z69183787/article/category/2281423)
kimmking：介绍下jms和ActiveMQ。在讲JMS之前，我们聊聊相关的背景。谁知道JMS是什么意思？
kimmking：对，是message service。那，什么事message呢？嗯，对了，就是信息。
kimmking：：所有的信息，我们都可以说是消息。
kimmking：前提是，它必须是动态的，可流动的，或者说是可传递的。这个过程，就是messaging
大家：请问jms的消息传递跟常用的通信协议有什么区别？
答：通信协议一般是针对一种具体的通信业务场景，制定了一个规定。是多方都需要遵守的。而jms消息机制，在更加抽象和通用的层度上定义了一个消息传递的规约。适用场景不一样。
大家：继续。
kimmking：ok，如何描述消息呢，如何传递消息呢？考虑这些问题，JMS规范就出场了。
kimmking：jcp组织02-03年定义了jsr914规范，[http://jcp.org/en/jsr/detail?id=914](http://jcp.org/en/jsr/detail?id=914)
kimmking：jsr914就是jms规范。
kimmking：主要干了两件事，定义通用的消息格式，和消息传递的模式。
kimmking：一个通用的消息格式，应该是什么样子的呢？
大家：啥样～？
kimmking：消息头 消息体
大家：头 主体 这样子吗 ？和soap类似？
kimmking：对，和soap类似，主要分header，properties，body
大家：那和soap有什么区别呢
kimmking：就是ppt的第8页。
kimmking：soap描述的是xml格式的数据格式。关注于rpc，与wsdl组成一个体系。
kimmking：jms描述更通用的消息。
kimmking：或者可以说是，soap专注于远程服务调用，jms专注于信息交换。
kimmking：soap是两个点的连接系统，jms是3个点。
kimmking：header和properties都是键值对，jms消息头预定义了一些消息的元数据。peoperties一般用来保存一些附加信息。body用来保存数据内容。
kimmking：一般来说，ws是同步的，jms是异步或同步的。
大家：题外话，那rmi跟soap里面的rpc是啥关系？
kimmking：rpc是remote process call
kimmking：rmi是一种rpc，rpc本身没有规范，
kimmking：只要是实现了远程调用的，都是rpc
kimmking：比如rmi .net-remoting ws/soap/rest hessian xmlrpc thrift potocolbuffer 等等。
kimmking：rpc有一个通用的结构，就是serialization/deserialization+stub+skeleton
林木森：这么说rpc是一种远程调用设计思维，强调的是透明的调用远程的方法，看起来就跟本地调用一样是伐？
大家：哦，那rpc强调的是透明的远程方法调用，而ms强调的是通用的信息交流。
kimmking：正解。有兴趣的可以自己了解下，ps：dwr也算一种标准的rpc
kimmking：回到jms。
kimmking：jms定义了消息格式以后，我们就可以考虑消息如何交互了。
kimmking：消息不是凭空出现的，所以，一定有传出消息的消息生产者。
大家：请问header，properties，body各自主要存哪一类型的信息？
kimmking[：](mailto:kimmking@163.com%3E%EF%BC%9A)
header和properties都是键值对。
body是具体要传递的内容。
header一般是要消息的元数据。前面说过了。
properties可以放一些自定义的附加信息。
kimmking：消息需要由生产者产生。同样的道理，消息也需要传递到需要它的消息消费者手中。
kimmking：除了消息以外，我们又得到两个概念，生产者和消费者。
大家：恩。
kimmking：其实还有一个隐含的东西，那就是MQ
kimmking：或者叫jms server
kimmking：生产者产生的消息，一般先发到jms server，然后由server传递给消费者处理。
大家：之前说的jms的3点结构就是指生产者，消费者和server嘛？
kimmking：对。
kimmking：消息可以看做是一个实物的抽象，也可以是抽象的概念。
kimmking：server应该说是用来暂存或者中转消息的吧
kimmking：假设存在多个消费者，如果一条消息代表一个实物，或是不可分的数据，那么将只有一个消息者可以获取到每条消息。
kimmking：我们称这种模式叫point to point，也就是ptp。
林木森：为什么只有一个消息者可以获取到每条消息？
kimmking：就是一条数据不能重复发给两个消费者。
kimmking：比如我有一个苹果，你们三个人要，我只能给一个人。再来一个苹果，我还是只能给一个人。
kimmking：这个人和前面那个人，可能不是同一个人。
大家：取了就没了嘛。相当于每条消息只有一个实例。取了就没有了。
kimmking：对。
kimmking：但是，如果消息是一个可以复制的，或者是抽象的东西，
kimmking：情况就不同了。
kimmking：比如消息是一句话：6木同学是泡妞高手。
6木：。。。。
kimmking：你们都想问我6木同学的情况，我就可以分别告诉你们这个消息。
大家：那相当于多播。。微薄那种。。
kimmking：我们称这个模式为发布订阅模式。
kimmking：publish/subscribe。就是每一个发布出阿里的消息，都可以被多个人共享。
kimmking：这个消息的发送者叫发布者，拿到各个消息的人，叫订阅者。
kimmking：消息发出来以后，需要放到server上的一个“地方”，这个地方我们叫目的地destination
kimmking：ptp时的destination我们叫queue
kimmking：pub/sub时的destination我们叫topic
kimmking：注意一点：jms规范和jdbc规范很类似，都是单纯的客户端视角，没有涉及任何server段的概念和描述。
kimmking：jms 和 jms server的关系 等价于
jdbc和数据库的关系
kimmking：从这个意义上，ActiveMQ相当于mysql
大家：这么说activeMQ就是支持jms规范的一个server！
kimmking：jms规范本身没对server做任何详细规范～
kimmking：所以，它主要是实现了一个MQ，其中的很小一部分，是实现jms规范，就是怎么去用mq，相当于mysql java驱动。
kimmking：”ActiveMQ是jms的一个实现“，这句话其实是很不科学的。
kimmking：jms还定义了一个selector，类似js框架的selector
kimmking：可以定义条件过滤消息，其语法是sql92的一个子集。
kimmking：ActiveMQ加强了一下，可以使用XPath。
大家：xpath不是xml的东西么。。消息的发送难道不是xml格式么？ 异构的系统 不用xml用什么？
kimmking：消息是抽象概念，可以是任何格式。注意jms三个字母等于 java message service。一般都不是xml格式。
kimmking：xpath不仅仅可以用在xml.xpath本质上是一个针对树状层次数据的一个选择器。抛开语法怪异以外，xpath和 各种el（mvel\ognl\。。。）差不多
大家：选择的功能很强大。
kimmking：一般jms消息不是一个xml，而是一个java对象，很明显的是jms没考虑异构系统。
kimmking：它压根儿没考虑非java的东西。但是，MQ都必须自己去解决这个问题。
kimmking：JMS不是为了解决异构系统而定义的，好在它发展成熟以后，基本上所以的jms server的实现（各种MQ）都解决了异构问题。
kimmking：跟数据库的各个平台下实现各自的驱动一个道理.
kimmking：jms默认情况下，发送和接收消息都是同步的。
kimmking：或者说，是主动的。
kimmking：需要我们显式的 send或receive
kimmking：所以，规范里定义了一个MessageListener，可以在有消息到达时（异步）被动的处理消息。
kimmking：其实消息传递还有一个常见场景
kimmking：就是request/response模式
kimmking：不光光是发出去一个消息，我还要等一个回来的响应消息。
kimmking：是靠什么机制知道当前发出去的消息已经回来了呢？
kimmking：JMS不是为了解决异构系统而定义的，好在它发展成熟以后，基本上所以的jms server的实现（各种MQ）都解决了异构问题。
大家：如果发送出去的消息 回来响应的时候失败会做些什么处理？
kimmking：呵呵，你提出一个很有意义的问题。
kimmking：其实也是jms考虑的所有概念和功能之上的最重要的问题。
kimmking：就是我们不光要传递消息，还要保障消息的可靠性。
kimmking：再进一步，我们要保证消息传递的高效和高可靠性。
kimmking：所以，JMS引入了两个概念，确认和事务。
大家：像数据库的啊～
kimmking：Kimm不是一直拿DB做比方嘛 更好理解
kimmking：确认的对象是消息，可以在收取一个消息时自动确认。也可以在一次会话过程，可以像jdbc那样，设置一个手动事务，处理完成后，提交，确认所有的消息。
kimmking：遗憾的是，发送不支持事务。
kimmking：淘宝内部的MQ框架，实现了发送的事务性。
kimmking：确认就是说，消费者获取一个消息，如何没有确认，MQ还会重试再次发送给你。
kimmking：你可以看做是你没有签收消息。所以，它要给你补发一份。
kimmking：你的意思是消费者在接受到信息后，发一个确认给jms server
kimmking：jms server没收到这个确认就在一定的时间内一直发
大家：那么生产者如何知道他的消息发送成功了呢？或者是接收者已经成功接收了呢？
kimmking：生产者不知道。生产者和消费者是透明的。它们不知道彼此的存在。
大家：那生产者需不需要jms server发给他确认消息？这样才知道他的信息已经确实的发出去了啊.不然中间丢了咋办？
kimmking：其实也有确认的。相当于邮局，但是如何邮局给你搞丢了信件，你还是木有办法。\其中隐含的一个假设就是，消息一旦发出，我们就完全的信任邮局。
kimmking：我们相信mq是可靠的。
kimmking：MQ告诉我们，我们发送成功了，MQ内部的东西，生产者是看不见的，我们怎么能相信消费者一定能接收到呢？
kimmking: 我们知道，接收我们信件的人不签收，消息会重新发送，就是可能会在消费者处重复。
kimmking: 但是，生产者不知道这个过程，发送这头没有这个可靠性的处理，怎么办呢？
kimmking: 还有就是如果MQ不负责任，把消息放在内存，MQ崩溃后，消息都丢失了怎么办？
kimmking: 所以，jms规范定义了消息的持久化。
kimmking: 把消息分为两种，一种是严格的不能丢失的消息，一种是可以偶尔丢失的消息。
kimmking: 前者性能明显会比后者慢，因为后者可以完全在内存处理，系统崩溃时内存中的消息会丢失。
kimmking: 前者需要MQ做持久化，内存放一份，持久化的介质中放一份。
kimmking: 然后MQ告诉消息生产者，我接收到了你发送的消息，你放心吧。
kimmking: 这比不持久化的消息，明显会慢。
kimmking: 消息被消费者消费掉了以后，MQ再从介质中删除掉消息。
kimmking: 系统崩溃时也不要紧，重启后从介质中恢复消息。
kimmking: queue默认是持久化的。
kimmking: topic，因为要发送给多个人，默认是非持久化的。
kimmking: 就是说topic里的消息放在内存里，只发给在线的订阅者
kimmking: 如果想要收到topic里所有的消息怎么办呢（像LDAP那样，把上次同步数据后的数据都给我）
kimmking: 这时，又有一个新的概念了，持久订阅。
kimmking: 你可以想象成，你在线期间，你接收了第1、2、3条数据。你掉线了，不在线期间，系统里多了第4、5条数据，发送给所有的订阅者后，删除了。
你又上线了。。。你想要你上次获取的最后一条数据后的第4、5条数据
kimmking: 这个是MQ里比较麻烦的地方。
kimmking: 原理就是同时持久化订阅者和topic的消息。
ticmy：我持久订阅了后长期不上线，咋整？我几年都不上线.那得保留多少消息....总不能我不上线耗费了服务器的所有存储空间吧
kimmking: 有可能。这个就是持久订阅不常用的原因之一吧。
kimmking: 持久订阅还有一个问题是，你断线以后，再连上jms server，server怎么知道你是以前的那个你呢？
大家：按kk上面的说法，这个发送直接还存在一个会话状态才能收信息哦～
kimmking: 跟http的sessionid一个道理。你每次来，都用一个相同的clientid就成了。
kimmking: mq把持久订阅者的信息存到本地，在其不在线期间，也把它当做一个订阅者，把该发给它的消息持久化到存储。等这条消息被所有的持久订阅者确认后删除。
kimmking: 所以，MQ还需要持久化确认的数据。因为对有持久订阅的TOPIC来讲，可能有些持久订阅者上线拿到数据并确认了，有些 木有确认。
kimmking: 可靠的原则是宁可重复也不少发，存入持久化保证安全，数据没用以后才能丢弃。
kimmking: 可靠天生就和高可用是冲突的。
kimmking: 所以，优化JMS的几个要点就是
kimmking: 所以，优化JMS性能的几个要点就是根据使用的场景，在可靠性和高性能之间做出权衡：
1、优化存储
2、优化确认
3、尽量快速的消费掉消息
4、尽量异步处理消息
kimmking: 今天的JMS就到这里吧，1个半小时。。。。
kimmking: ActiveMQ找个时间再讲吧。
提问时间：
问题1：activeMQ是一款开源产品么？那个公司出的？目前在国内的使用情况如何？
kimmking: apache的，[http://activemq.apache.org/](http://activemq.apache.org/)
kimmking: 目前用的最多的MQ是 Websphere MQ（商业的） ActiveMQ（开源的）
kimmking: ActiveMQ从3.X以后非常稳定。很多大公司都在用。
kimmking: 最新正式版本 5.5.0 activemq6 正在开发，不是java的了，使用的是scala
kimmking: 快钱用的是sonicMQ
kimmking: 现在好像稍大点的公司都有自己的MQ实现
kimmking: 淘宝现在主要用的是自己实现的MQ
kimmking: 某些项目里还在用ActiveMQ
kimmking: 去哪儿网，用了很多年的ActiveMQ了。一直很稳定。
问题2：使用mq的典型场景，能举个现实中的例子么
kimmking: 系统间集成传递数据，同步转异步。甚至可以用来做通知。
问：系统间集成传递数据，用ws也可以把
kimmking: ws一般不是用来传递数据的。
kimmking: 前面以及说过了ws的关注点是service。是调用一个服务。可以看ppt里面有两个图很形象的说明了问题。
kimmking: 其实可以把MQ看做是BlockingQueue 的一个自然的延伸。
kimmking: 1、是在一个独立的jvm里了。2、概念和内容丰富了。
2、概念和内容丰富了。
kimmking: MQ最大的作用是 （解耦系统间的依赖关系）（缺2张图）
kimmking: jms还可以看做是一个退化的ESB。。。。
问题3：MQ存在单点问题么？
kimmking: 一个很明显的缺陷就是，引入了一个单点。如果中间的MQ挂了，那么所有的系统都不能交互了。
kimmking: 但是mq一般都可以集群，或者做master-slave
kimmking: 集群的主要目的是负载均衡还是灾难恢复抑或两者皆有
kimmking: 有多种集群方式。针对各种场景。master-slave，是针对 disaster recovery的。
问题3：spring有mq相关的实现嘛？
kimmking: spring好像没有mq
kimmking: spring下有个spring-jms的子项目。
kimmking: 跟spring的jdbcsupport的功能差不多。
kimmking: 封装了一些jms的常用操作。
kimmking: 这个是spring的强项。spring很少做轮子。一般是集成
kimmking: spring是到处做 别人的东西到spring的适配。
kimmking: 比如现在的spring-data项目。
kimmking: 意图包括所有场景类型的 nosql
kimmking: 愿景很远大。
kimmking: 今天讲座到此结束。
kimmking: 明天我会发布activemq-store-mongodb的第一个可用版本。支持queue的消息持久化到mongodb。还不支持持久订阅。
kimmking: 感兴趣的人请关注：[http://code.google.com/p/activemq-store-mongodb/](http://code.google.com/p/activemq-store-mongodb/)
