# 浅析消息队列 RabbitMQ - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [oliver_lv](http://www.jobbole.com/members/lvxiang639) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 什么是rabbitMQ
rabbitMQ是一款基于AMQP协议的消息中间件，它能够在应用之间提供可靠的消息传输。在易用性，扩展性，高可用性上表现优秀。而且使用消息中间件利于应用之间的解耦，生产者（客户端）无需知道消费者（服务端）的存在。而且两端可以使用不同的语言编写，大大提供了灵活性。
## rabbitMQ工作原理
首先我们得先理解rabbitMQ里的一些基本定义，主要如下：
exchange： producer只能将消息发送给exchange。而exchange负责将消息发送到queues。Exchange必须准确的知道怎么处理它接受到的消息，是被发送到一个特定的queue还是许多quenes,还是被抛弃，这些规则则是通过exchange type来定义。主要的type有direct,topic,headers,fanout。具体针对不同的场景使用不同的type。
queue: 消息队列，消息的载体。接收来自exchange的消息，然后再由consumer取出。exchange和queue是可以一对多的，它们通过routingKey来绑定。
Producer:生产者，消息的来源,消息必须发送给exchange。而不是直接给queue
Consumer:消费者，直接从queue中获取消息进行消费，而不是从exchange。
从以上可以看出Rabbitmq工作原理大致就是producer把一条消息发送给exchange。rabbitMQ根据routingKey负责将消息从exchange发送到对应绑定的queue中去，这是由rabbitMQ负责做的。而consumer只需从queue获取消息即可。
大致流程如下：
![](http://ww4.sinaimg.cn/mw690/63918611gw1f9cg2c5hpmj213007kjs5.jpg)
## rabbitMQ工作模型
下面通过几个列子来详细说明一下如何使用rabbitmq
### 简单发送模型
在rabbit MQ里消息永远不能被直接发送到queue。这里我们通过提供一个空字符串来使用默认的exchange。这个exchange是特殊的，它可以根据routingKey把消息发送给指定的queue。所以我们的设计看起来如下所示
![](http://ww2.sinaimg.cn/mw690/63918611gw1f9cg2d7awjj20ka05imxk.jpg)
代码如下
send.py
![](http://ww4.sinaimg.cn/mw690/63918611gw1f9cg2e5ah2j20ns0agq4m.jpg)
receive.py
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9cg2ex9plj20m80a6abx.jpg)
### 工作队列模型
这种模式常常用来处理耗资源耗时间的任务在多个workers中，主要是为了避免立即去处理一个耗时的任务而等待它的完成。代替的做法是一个稍后去处理这个任务，让一个worker process 在后台处理这个任务。当有许多workers的时候，消息将会以轮询的方式被workers获取。模型如下
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9cg2gcd5yj20cj04xt8z.jpg)
这里就会有一个问题，如果consumer在执行任务时需要花费一些时间，这个时候如果突然挂了，消息还没有被完成，消息岂不是丢失了，为了不让消息丢失，rabbitmq提供了消息确认机制，consumer在接收到，执行完消息后会发送一个ack给rabbitmq告诉它可以从queue中移除消息了。如果没收到ack。Rabbitmq会重新发送此条消息，如果有其他的consumer在线，将会接收并消费这条消息。消息确认机制是默认打开的。如果想关闭它只需要设置no_ack=true。在此处我们不需要设置。默认如下就行


```
channel.basic_consume(callback,  queue='hello')
```
除了consumer之外我们还得确保rabbitMQ挂了之后消息不被丢失。这里我们就需要确保队列queue和消息messages都得是持久化的。
队列的持久话需要设置durable属性


```
channel.queue_declare(queue= task_queue, durable=True)
```
消息的持久话则是通过delivery_mode属性，设置值为2即可


```
channel.basic_publish(exchange='',
                      routing_key="task_queue",
                      body=message,
                      properties=pika.BasicProperties(
                         delivery_mode = 2, # make message persistent
                      ))
```
还有一个属性相对比较重要，它可以保证consumer确认消费完一条消息之后再去获取下一条消息。如果consumer正在忙碌的状态，消息将会被分发到下一个不是很忙的consumer。设置如下


```
channel.basic_qos(prefetch_count=1)
```
下面贴出部分代码
producer.py
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9cg2gspp7j20nk08475t.jpg)
consumer.py
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9cg2hiknlj20ko08u408.jpg)
### 广播模型
在前面2个示例我们都适用默认的exchange。这里我们将自己定义一个exchange。并设置type为fanout。它可以将消息广播给绑定的每一个queue。而不再是某一个queue。我们在此创建一个叫logs的exchange，就像下面这样


```
channel.exchange_declare(exchange='logs', type='fanout')
```
所以发布消息就变成了下面这样


```
channel.basic_publish(exchange='logs',routing_key='', body=message)
```
在这里我们需要将消息发送给所有的queues。而不需要指定某些队列。所以我们这里就用临时队列代替。并设置在失去连接后删除队列。当然我们也可以不这么做。
设置临时队列，让rabbitmq给我们一个随机的队列名字，设置exclusive为true确保失去连接的时候队列也被删除了。因为我们这里不需要持久话它。


```
result = channel.queue_declare(exclusive=True)
```
下面就是要绑定queues和exchange


```
channel.queue_bind(exchange='logs', queue=result.method.queue)
```
综上所述我们的设计就像下面这样
![](http://ww4.sinaimg.cn/mw690/63918611gw1f9cg2i9940j20au04m0su.jpg)
部分代码如下
producer.py
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9cg2j63ulj20mm04m755.jpg)
consumer.py
![](http://ww4.sinaimg.cn/mw690/63918611gw1f9cg2joczmj20lc08ijt0.jpg)
### direct模型
在上个模型中，消息被发送给所有的消费者，而在这一部分我们将通过路由的方式使exchange通过定义的路由方式将消息发送给队列。所以我们需要在绑定exchange和queue的时候指定routing_key字段，注意这里的routing_key不是basic_publish中的routing_key。见如下


```
channel.queue_bind(exchange=exchange_name,queue=queue_name,routing_key='black')
```
这里我们将使用type为direct的exchange。这种路由方式exchange将消息通过绑定的routing_key发送到指定的队列。而且exchange可以通过多个routing_key把消息发送给同一个queue
通过下面这张图我们来分析一下
![](http://ww4.sinaimg.cn/mw690/63918611gw1f9cg2kw791j20cv04qjro.jpg)
在上面的图中，我们可以看出type为direct的exchange X 绑定了2个队列。队列Q1与关联路由orange。队列Q2关联路由black和green。所以一个带有路由健orange消息将被exchange发送给队列Q1。而带有路由健black或者green的消息将被发送给队列Q2。
我们还是通过修改前面的日志系统，来展示direct类型的exchange如何工作，如下图
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9cg2l9p33j20du04oq3b.jpg)
部分代码如下
producer.py
![](http://ww2.sinaimg.cn/mw690/63918611gw1f9cg2mgfsfj20n806ymz1.jpg)
consumer.py
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9cg2ncfr7j20mu0aamzg.jpg)
让我们运行一下看看结果是什么，我们启动了3个consumer,routing_key分别指定为warning, error，第三个同时指定这2个。然后在运行producer时带上路由信息routing_key。运行后可以看出指定了warning的不会收到error的消息。同时指定warning 和error的consumer则会都收到消息。
发送消息
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9cg2odvj2j20ag01xt8w.jpg)
只收到warning的消息
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9cg2qer8nj20an01ldg1.jpg)
只收到error的消息
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9cg2soqchj20aw01k0so.jpg)
error和waring的都能收到
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9cg2tgi5vj20do01x0sw.jpg)
### Topic模型
这种模型是最灵活的，相比较于direct的完全匹配和fanout的广播。Topic可以用类似正则的手法更好的匹配来满足我们的应用。下面我们首先了解一下topic类型的exchange
topic类型的routing_key不可以是随意的单词，它必须是一系列的单词组合，中间以逗号隔开，譬如“quick.orange.rabbit”这个样子。发送消息的routing_key必须匹配上绑定到队列的routing_key。消息才会被发送。此外还有个重要的地方要说明，在如下代码处绑定的routing_key种可以有*和#2种字符


```
channel.queue_bind(exchange='topic_logs',queue=queue_name,
                    routing_key=binding_key)
```
它们代表的意义如下
*(星号) 可以匹配任意一个单词
#(井号) 可以匹配0到多个单词
我们通过下图来解释一下
![](http://ww2.sinaimg.cn/mw690/63918611gw1f9cg2u8pncj20cl04e3yt.jpg)
Q1匹配3个单词中间为orange的routing_key ,而Q2可以匹配3个单词最后一个单词为rabbit和第一个单词为lazy后面可以有多个单词的routing_key
下面贴上部分示例
producer.py
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9cg2v0tfoj20o404qdh2.jpg)
consumer.py
![](http://ww3.sinaimg.cn/mw690/63918611gw1f9cg2w597ij20li08k0ui.jpg)
### RPC应用模型
当我们需要在远程服务器上执行一个方法并等待它的结果的时候，我们将这种模式称为RPC。下面我们用rabbitMQ建立一个RPC系统
在rabbit MQ中为了能让client收到server端的response message。需要定义一个callback queue ,就像下面这样
![](http://ww4.sinaimg.cn/mw690/63918611gw1f9cg2x04wmj20oe05kgmo.jpg)
不过现在有一个问题，就是每次请求都会创建一个callback queue .这样的效率是极其低下的。幸运的是我们可以通过correlation_id为每一个client创建一个单独的callback queue。通过指定correlation_id我们可以知道callback queue中的消息属于哪个client。要做到这样只需client每次发送请求时带上这唯一的correlation_id。然后当我们从callback queue中收到消息时，我们能基于 correlation_id 匹配上我们的消息。匹配不上的消息将被丢弃，看上去就像下图这样
![](http://ww4.sinaimg.cn/mw690/63918611gw1f9cg2xqzfoj20gl05ht90.jpg)
总结一下流程如下
- client发起请求，请求中带有2个参数reply_to和correlation_id
- 请求发往rpc_queue
- server获取到rpc_queue中的消息，处理完毕后，将结果发往reply_to指定的callback queue
- client 获取到callback queue中的消息，匹配correlation_id,如果匹配就获取，不匹配就丢弃.
示列代码参考[http://www.rabbitmq.com/tutorials/tutorial-six-python.html](http://www.rabbitmq.com/tutorials/tutorial-six-python.html)
从上面的6个示例我们大致了解了如何运用rabbitMQ解决我们的实际需求，下面我们再来看看如何管理和监控rabbitMQ的实际运行情况
## rabbitMQ管理和监控
### rabbitmq management插件
rabbitMQ提供了一个管理插件，通过这个插件我们可以查看当前rabbitMQ服务的运行情况。在解压缩官网提供的rabbitMQ安装包之后，在sbin目录可以看见rabbitmq-plugins文件，我们只需运行一下命令


```
rabbitmq-plugins enable rabbitmq_management
```
然后再游览器中输入http://*server-name*:15672/ 就可以查看当前rabbitMQ的一些运行状况。如下所示
![](http://ww1.sinaimg.cn/mw690/63918611gw1f9cg2yrokcj20od0iemzf.jpg)
在这个管理控制台我可以做很多事情，譬如
- 查看运行的exchanges,queues,users,virtual hosts还有权限
- 添加exchanges,queue,users,virtual host，以及给用户赋予权限
- 监控消息长度，通道，消息速度。连接数
- 发送接收消息
- 关闭连接，清除队列
### rabbitmqctl使用
在rabbitMQ中，rabbitctl是一个被广泛使用的命令。对用户的增加，删除，列出列表，创建权限，都是通过rabbitmqctl完成的。下面举几个例子来熟悉一下如何使用
创建一个用户名和密码都为test的新用户


```
./rabbitmqctl  add_user test test
```
删除的话使用一下命令


```
./rabbitmqctl  delete_user test
```
列出所有用户


```
./rabbitmqctl  list_users
```
同样也可以用此命令为用户赋予权限
譬如我们想为用户test在vhost rabbitmq赋予全部访问权限，只许执行如下命令


```
./rabbitmqctl set_permissions –p rabbitmq test “.*” “.*” “.*”
```
列出权限


```
./rabbitmqctl set_permissions –p rabbitmq test “.*” “.*” “.*”
```
删除权限


```
./rabbitmqctl clear_permissions –p rabbitmq
```
同样的rabbitmqctl也可以用来查看rabbitmq的运行状况，如下
列出队列和消息数目


```
./rabbitmqctl  list_queues –p rabbitmq
```
如果想要了解更多的队列消息，譬如名字，消息数目，消费者数目，内存使用情况，以及其他属性 。则可以发送一下命令


```
./rabbitmqctl list_queues name messages consumers memory durable auto_delete
```
列出exchanges相关信息


```
./rabbitmqctl list_exchanges  name  type  durable  auto_delete
```
rabbitmqctl还有很多功能，这里不一一例举了。有兴趣的可以去官方网站查看。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/11/7459d45a2640544b522a167777ff4f82.jpeg)![](http://jbcdn2.b0.upaiyun.com/2016/11/079836b69e33cb91e10665bb3726b6d8.jpeg)
