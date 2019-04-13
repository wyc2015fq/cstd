
# Redis发布、订阅 - 追求卓越,做到专业 - CSDN博客


2017年04月14日 11:44:09[Waldenz](https://me.csdn.net/enter89)阅读数：308标签：[redis																](https://so.csdn.net/so/search/s.do?q=redis&t=blog)[发布订阅																](https://so.csdn.net/so/search/s.do?q=发布订阅&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=redis&t=blog)个人分类：[Redis																](https://blog.csdn.net/enter89/article/category/6537491)



## Redis发布订阅
发布订阅模式中发布消息的为publisher即发布者，接收消息的为subscriber即订阅者。在Redis中，所有的消息通过channel即频道进行发布，一个发布者可以向多个channel发布消息，一个订阅者也可以订阅多个channel。Redis不对消息进行持久化，如果消息发布时订阅者还没有进行订阅，则不会再收到此消息。
![](http://caiguoqing.org/upload/2017021101.jpeg)

## 发布订阅命令
|命令|格式|说明|
|PUBLISH|PUBLISH channel message|发布message到指定的channel|
|SUBSCRIBE|SUBSCRIBE channel [channel ...]|订阅1个或多个指定的channel|
|UNSUBSCRIBE|UNSUBSCRIBE [channel [channel ...]]|取消订阅1个或多个指定的channel，如果不指定channel退订所有通过SUBSCRIBE订阅的channel|
|PSUBSCRIBE|PSUBSCRIBE pattern [pattern ...]|根据匹配模式订阅channel|
|PUNSUBSCRIBE|PUNSUBSCRIBE [pattern [pattern ...]]|根据匹配模式取消订阅channel，如果不指定匹配模式退订所有通过PSUBSCRIBE订阅的channel|
subscribe不能重复订阅同一个channel，而psubscribe按照匹配模式订阅时有可能会多次订阅同一个channel。如果psubscribe多次订阅了同一个channel，发布者使用publish发布消息到此channel后，订阅者会多次收到此消息。
unsubscribe只能退订subscribe订阅的channel，punsubscribe只能退订psubscribe订阅的channel。

订阅方法

```python
// <summary>
        /// 订阅
        /// </summary>
        public static void Subscription()
        {
            using (ServiceStack.Redis.RedisClient consumer = new RedisClient("192.168.210.36", 6379))
            {
                //创建订阅
                ServiceStack.Redis.IRedisSubscription subscription = consumer.CreateSubscription();
                //接收消息处理Action
                subscription.OnMessage = (channel, msg) =>
                {
                    Console.WriteLine("频道【" + channel + "】订阅客户端接收消息：" + ":" + msg + "         [" +
                                      DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss") + "]");
                    Console.WriteLine("订阅数：" + subscription.SubscriptionCount);
                    Console.WriteLine("___________________________________________________________________");
                };
                //订阅事件处理
                subscription.OnSubscribe = (channel) =>
                {
                    Console.WriteLine("订阅客户端：开始订阅" + channel);
                };
                //取消订阅事件处理
                subscription.OnUnSubscribe = (a) => { Console.WriteLine("订阅客户端：取消订阅"); };
                //订阅频道
                subscription.SubscribeToChannels("channel-1");
            }
        }
```
发布程序
```python
static void Main(string[] args)
        {
            Console.Title = "Redis发布、订阅服务";
            RedisClient client = new RedisClient("192.168.210.36", 6379);
            while (true)
            {
                string input = Console.ReadLine();
                client.PublishMessage("channel-1", input);
            }
        }
```



