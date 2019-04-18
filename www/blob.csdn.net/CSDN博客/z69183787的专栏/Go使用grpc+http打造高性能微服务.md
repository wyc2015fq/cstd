# Go使用grpc+http打造高性能微服务 - z69183787的专栏 - CSDN博客
2018年09月09日 20:57:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：248
[https://blog.csdn.net/RA681t58CJxsgCkJ31/article/details/78601747](https://blog.csdn.net/RA681t58CJxsgCkJ31/article/details/78601747)
大家可以发现，Go越来越流行，其一是目前云计算领域基本是使用Go作为底层开发语言；其二是随着区块链的火爆，引申出了其背后很多开源项目，很多都是使用Go语言进行开发；其三就是在微服务方面，Go也展示很大的优势性。那么微服务是什么，如何将其与Go做一个整合，来提升整体开发生产效率，下面会进行详细讲解。
**什么是微服务**
微服务的概念最早由 Martin Fowler 提出，在他的理论当中，微服务是一整套小的服务，其中每一个服务互相之间都是独立的，它们之间通过 lightweight 机制进行交互。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzRJCfVibKw2PzSk8mdHL5yEpT5wDKmQE8gwqnJbbHbH50nqqDJHsaP7Q/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
图 1
图 1 所示是最传统的一个应用软件架构，最顶层是 load balancer ，下面布局很多cuisine，这些cuisine共享一个数据库。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzEeMtjM5jNp5NvpX5sUw7okHEgSI6cfSTicoj6NWm2LsgG0kvhcCd8vg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
图 2
图 2 是微服务的架构图，由图中我们可以看出整体架构分布的改变，一个服务被分成A、B、C三种服务，每个服务有自己的数据库，前面都会有一个 API Gateway将其三者链接起来。显而易见，微服务的难点也就在服务的划分上，通俗点讲就是如何服务其中的微服务。
在之前我与一些公司朋友做线下交流时曾提到他们对微服务划分的经验，有些部门是按照模块进行划分的，简单的比如会员认证模块、评论模块、点赞模块等，有些则分的比较细的，会将一个很小的功能（函数级别）分成一个模块；在国外盛行一种“披萨分法”，比如一个团队所有的开发、部署、运维人员能够将一个披萨吃完，那么整个团队就被称之为一个微服务团队（基本三到四人能够完成整套工作）。
这里大概介绍了微服务的基本概念，但是微服务并非良药，很多架构并非刚开始就会采用微服务的方式，后续会对这方面做更加详细的介绍。
**GRPC介绍**
gRPC是由Google主导开发的RPC框架，使用HTTP/2协议并用Protobuf作为序列化工具。与REST不同的是，REST是基于HTTP1.1 JOSN格式的一种轻量级服务模式，那么从这两方面来对比gRPC与REST就比较容易了。
首先可以从JOSN与Protobuf之间的差别入手进行对比，Protobuf很难读，它是面向机器的文字格式，而JOSN则是面向人的；Protobuf相对于JOSN而言编解码速度都非常快；最后就是兼容性，现在基本所有浏览器都支持JOSN格式，而Protobuf目前仅部分语言支持。
接下来可以对比一下http1.1与http2。http1.1是文本方式，因此http的请求我们都可以很清楚的读懂；http1.1中每一个请求会有一个新的connection，但是http2 是一个持续的http connection；http1是持续repetitive，http2是compressed；http1.1所有浏览器都默认支持，而http2只是部分支持。综上，就是gRPC和REST在支持协议上的不同。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFz8NZnXZhTPg1eEOaF4wFgOWOVdpNsicu2yicl69ia2Ttle4U7mBKU1rUVQ/0?wx_fmt=png)
图 3
图 3 所示是一个 REST API 的代码示例，其中validate是一个校验函数，可以校验input的内容是否合法，如果校验失败输出false，反之则输出”200“ 那么同样功能的一个API，图 5 展示了gRPC API 当中的内容。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzY7iaasAnU8DBfKXpuYdDcZL8EHumTBGL441c15lgpztdz7tia2DoQ7gw/0?wx_fmt=png)
图 4
因为先前有提到，使用gRPC时，传递的是 Protobuf 那么首先我们就需要去定义一个Protobuf文件，，如图 4。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzsT2R0jxSMpNyuAun2GoKwUNRWn8LUjbQ5smZqO42DVZfnuno0o2rug/0?wx_fmt=png)
图 5
图 5 当中定义的参数如 InfoRequest 等在图 4 描述的定义文件中都有详细的参数说明。那么通过gRPC与REST的接口内容对比我们可以看出，gRPC的代码更加简洁。那么这是接口代码的对比，性能上它们有什么差异性呢？
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFz6ia7ozc6NO4aRGXoiaiat8qOeyOK3qZS2AuJsBCn1icuxBCVdbl4ic2pM7w/0?wx_fmt=png)
图 6
如图 6 所示，是gRPC与REST之间性能方面的对比，我们可以看出来性能差异还是比较大的，在请求相同内容时，差异性也是比较大的，gRPC会跑1000000次，每次时长大致为2063ns/op，但是REST在同等情况下只会跑3000次，每次耗时为40493772ns/op。
以上是两者的对比，从前面的对比我们也可以看出，两者之间的差异性，那么问题很明显，两者之间的转换就背负着很大的历史包袱，所以，今天会着重去介绍如何使用gRPC+HTTP去打造一个微服务。
**如何使用 GRPC+HTTP 打造微服务**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzia8IudeX8bzsHTbqmYK8yePRz8YEq4B3ibhEYdfqdLxqI4x4rXT31rrQ/0?wx_fmt=png)图7   
图 7 所示是现在常见的微服务架构方式，由图中我们可以看出，最前面试load balance，后面紧接着是API gateway，主要做一些聚合的工作。对外采用REST API方式相互调用，对内则采用gGPC API的方式相互进行调用。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzTmiajuGcia2rD6QNsjX2p9IXkLSfhEsj6bxztngMr0PibyrzQBr6oJhQQ/0?wx_fmt=png)
图 8
图 8 所示举了一个例子，比如我拥有一个www的网站，这个网站页面使用的是react，现在不论是那种页面框架都会遵循以上的方式去调用API，那么调用的API是基于JOSN+HTTP，后面紧跟四个gRPC的service，参照图 8 所示的流程，每个调用都是循序渐进的。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzbwxWRqBmrEjtMOPQGAB9AnPfmWkicH0Oq1u6l6A0kaWm0KZxkYx7J0Q/0?wx_fmt=png)
图 9
图 9 所示是以上流程图所提供的五个服务的代码架构，其中包含API服务，认证（auth）服务，geo服务（根据当天的位置反馈定位经纬度），profile服务（获取经纬度周边信息）以及rate服务（获取酒店排名信息）。由图中所示，可以看出对外统一是采取REST的方式，而对内则都使用Protobuf，只需要定义一些文件的内容即关于service是什么即可。最后可以在API当中注册service，那么之后API在被调用的时候，去调用相应服务即可。
接下来会介绍我们在内部做整个微服务框架时的一些想法和实践。微服务看似简单，如上述例子，在调用一个函数的时候，可以完成五个函数的工作，但是一旦进行拆分就会变成五个服务，复杂度也会升高，所以我一直在思考微服务框架到底解决了哪些问题，因此我在beego 中也产生了很多思考。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFziabXn7CqzfaPvT0ODVrkXks4NdwicZHrl6yghiaiaht7ibaFDCuF0rJb9HQ/0?wx_fmt=png)
图 10
首先是测试，测试包括两个方面：单元测试和集成测试。所谓的单元测试（图 10）在Go中是比较被推崇的一种方式，在单元测试当中要生成一个stract就必须要声明其中的输入和输出，对于需要测试的函数只需要增加一个for循环，那么每当增加一行代码需要进行测试时，只需要增加一行要测试的input和outout即可。
但是今天着重会介绍集成测试，集成测试相对会比较麻烦一点，因为需要去模拟http或者是gRPC请求，例如测试文件array.request中的内容，其中test-fixtures（包含3000多个测试服务）这里面我只需要完成array.request的内容，response的内容可以自动生成。
其次是Tracing，当微服务被引入后，每个服务的运行时长会变得很难追踪，当调试一个问题时，因为微服务很多（尤其是上千个时）此时去找问题就会很麻烦，那么通过Tracing的方式就可以很容易的找到调用的东西从而大大降低了寻找问题的时间成本。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzUjnNMlGeLv2GYK7WG9kwaYCUd0PoMpaqD9OfvRncyGKXwUOegibC9Kg/0?wx_fmt=png)
图 11
图 11 所示是Opentracing，它是一个分布式Tracing标准（Kubernetes成立了一个云计算基金会CNCF，这个就是其中的一个项目）。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzEwDX8Sr315CJ6dcD9bEdqqicHwaqrznkRt6OBLworiac05Y5JrqSKypg/0?wx_fmt=png)
图 12
图 12 所示是chrome中进行追踪的一个工具（chrome://tracing/），它最主要的是为了调多线程、多进程、多goroutine所存在的。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzMiaZuAgASJVbhwenqZLCSxcRuJ6SF7852ib2jpWWrjw73uBapZUzsnOg/0?wx_fmt=png)
图 13
在讲完Tracing之后，大家会比较关注Monitoring（监控系统）这里我主要会举prometheus（图 13）的例子进行说明，在prometheus中有三种类型：基础型、上下标准差型以及时间类型。这里我采用的是标准差类型，因为需要判断当前服务器时间与NTP时间的差值。prometheus中有很多定义好的函数（参数） 当然我这边定义了三个函数（参数）比如Collectors、GoCollector（监控了Go中所有的信息）刚才所说的 Monitoring 的项目，可以关注https://prometheus.io/ 以及 https://grafana.com 这两个网址的内容进行了解。
最后讲一下Debug，其实应该改成logging。在微服务当中一直在讲三个东西，一是Tracing，其次是prometheus需要收集的东西，最后就要介绍一下logging。现在写log经常会分不同的level，比如infor、running、debug等，实际上其实我们去查log时并不会太过于关注这些东西，Dave之前写了这篇blog：https://dave.cheney.net/2015/11/05/lets-talk-about-logging 有兴趣的朋友可以去看一下，里面就提到两种真正要的level。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/5WXEuGYZIibDkyqTkqTHrWibBqibRloniaFzLAic00ahkhR9fKib7kcZdspvlBUA7KStvDHA4e6T84um0ztAS3m7nofA/0?wx_fmt=png)
图 14
图 14 所示是微服务架构的核心。那么讲了这么多，微服务架构到底是不是最完美的架构，答案当然是否定的，如果一个公司的团队要上微服务架构前提是需要考虑你们团队的整体实力，如果有能力，那么我建议大家上微服务，如果没有，我不建议大家去迁微服务的架构。
**为什么 Go 适合微服务**
首先是考虑到语言的复杂程度，一些静态语言效率的确很高，可是非常复杂，比如C++，复杂性太高，大家都喜欢简单的东西，所以复杂的都被排除在外。其次考虑到性能问题。最后考虑到并发编程环境，一般来说并发的编程环境很难处理，因为要考虑线程、锁等问题。所以，一般提到这些，都会提及速度、简单、可读这三个关键指标去评判一项语言，那么Go是不是可以三者兼并呢？大家在编程时都希望越简单越好，简单就意味着”小“，说到小，微服务的概念似乎更小。我个人认为，Go之所以适合做微服务，是因为它本身就很小，这个小蕴含两层意思：其一，编译出来的东西小；其二，关键字小导致核心小，学起来也比较简单。
总而言之，Go相对其他所有语言来说，它是简单易读的，Go是工程性的语言，它们都是从工程上的东西出发，完全按照工程师的思路，从工程师思路出发，去辅助你完成工作，这样在接触Go的时候就会有很大的亲切感。
我觉得，相对于其他语言来说，Go最大的优点：其一是高并发，但是其实Go的并发并没有C好，但是在多层情况下，C需要多核使用，对程序员要求很高；其二是Team协作，Go由将近一万人在共同协作，代码当然也可以从一千万行协作；其三是高性能，因为它的性能以及简单性，让其在PHP与C之间找到了平衡点，PHP可以快速开发，但是性能说到底比C还是差一点，但是C的学习成本又很高，所以Go刚好在两者之间找到了平衡点；四是对运行非常友好，所有写的东西编译之后就是二进制的文件，部署时直接就可以使用。
综上，也就是我对于Go为何适用于微服务的一些看法，当然任何技术也在不断迭代更新，我们也仍需要去不断更新自己的知识库
