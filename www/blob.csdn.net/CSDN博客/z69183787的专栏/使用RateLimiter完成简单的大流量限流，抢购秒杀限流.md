# 使用RateLimiter完成简单的大流量限流，抢购秒杀限流 - z69183787的专栏 - CSDN博客
2018年04月27日 15:31:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：333
[https://www.cnblogs.com/yeyinfu/p/7316972.html](https://www.cnblogs.com/yeyinfu/p/7316972.html)
RateLimiter是guava提供的基于令牌桶算法的实现类，可以非常简单的完成限流特技，并且根据系统的实际情况来调整生成token的速率。
通常可应用于抢购限流防止冲垮系统；限制某接口、服务单位时间内的访问量，譬如一些第三方服务会对用户访问量进行限制；限制网速，单位时间内只允许上传下载多少字节等。
下面来看一些简单的实践，需要先引入guava的maven依赖。
## 一 有很多任务，但希望每秒不超过N个
**[java]**[view
 plain](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[copy](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[print](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[?](https://www.cnblogs.com/yeyinfu/p/7316972.html#)
- import com.google.common.util.concurrent.RateLimiter;  
- 
- import java.util.ArrayList;  
- import java.util.List;  
- import java.util.concurrent.ExecutorService;  
- import java.util.concurrent.Executors;  
- 
- /**
-  * Created by wuwf on 17/7/11.
-  * 有很多个任务，但希望每秒不超过X个，可用此类
-  */
- publicclass Demo1 {  
- 
- publicstaticvoid main(String[] args) {  
- //0.5代表一秒最多多少个
-         RateLimiter rateLimiter = RateLimiter.create(0.5);  
-         List<Runnable> tasks = new ArrayList<Runnable>();  
- for (int i = 0; i < 10; i++) {  
-             tasks.add(new UserRequest(i));  
-         }  
-         ExecutorService threadPool = Executors.newCachedThreadPool();  
- for (Runnable runnable : tasks) {  
-             System.out.println("等待时间：" + rateLimiter.acquire());  
-             threadPool.execute(runnable);  
-         }  
-     }  
- 
- privatestaticclass UserRequest implements Runnable {  
- privateint id;  
- 
- public UserRequest(int id) {  
- this.id = id;  
-         }  
- 
- publicvoid run() {  
-             System.out.println(id);  
-         }  
-     }  
- 
- }  
该例子是多个线程依次执行，限制每2秒最多执行一个。运行看结果
![](https://img-blog.csdn.net/20170711160619785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们限制了2秒放行一个，可以看到第一个是直接执行了，后面的每2秒会放行一个。
rateLimiter.acquire()该方法会阻塞线程，直到令牌桶中能取到令牌为止才继续向下执行，并返回等待的时间。
## 二 抢购场景限流
譬如我们预估数据库能承受并发10，超过了可能会造成故障，我们就可以对该请求接口进行限流。
**[java]**[view
 plain](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[copy](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[print](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[?](https://www.cnblogs.com/yeyinfu/p/7316972.html#)
- package com.tianyalei.controller;  
- 
- import com.google.common.util.concurrent.RateLimiter;  
- import com.tianyalei.model.GoodInfo;  
- import com.tianyalei.service.GoodInfoService;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- import org.springframework.web.bind.annotation.RestController;  
- 
- import javax.annotation.Resource;  
- 
- /**
-  * Created by wuwf on 17/7/11.
-  */
- @RestController
- publicclass IndexController {  
- @Resource(name = "db")  
- private GoodInfoService goodInfoService;  
- 
-     RateLimiter rateLimiter = RateLimiter.create(10);  
- 
- @RequestMapping("/miaosha")  
- public Object miaosha(int count, String code) {  
-         System.out.println("等待时间" + rateLimiter.acquire());  
- if (goodInfoService.update(code, count) > 0) {  
- return"购买成功";  
-         }  
- return"购买失败";  
-     }  
- 
- 
- 
- @RequestMapping("/add")  
- public Object add() {  
- for (int i = 0; i < 100; i++) {  
-             GoodInfo goodInfo = new GoodInfo();  
-             goodInfo.setCode("iphone" + i);  
-             goodInfo.setAmount(100);  
-             goodInfoService.add(goodInfo);  
-         }  
- 
- return"添加成功";  
-     }  
- }  
这个是接着之前的文章（秒杀系统db，http://blog.csdn.net/tianyaleixiaowu/article/details/74389273）加了个Controller
代码很简单，就是请求过来时，调用RateLimiter.acquire，如果每秒超过了10个请求，就阻塞等待。我们使用jmeter进行模拟100个并发。
创建一个线程数为100，启动间隔时间为0的线程组，代表100个并发请求。
![](https://img-blog.csdn.net/20170711163232865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
启动jmeter请求，看控制台结果
![](https://img-blog.csdn.net/20170711163418564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20170711163640976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
初始化10个的容量，所以前10个请求无需等待直接成功，后面的开始被1秒10次限流了，基本上每0.1秒放行一个。
## 三 抢购场景降级
上面的例子虽然限制了单位时间内对DB的操作，但是对用户是不友好的，因为他需要等待，不能迅速的得到响应。当你有1万个并发请求，一秒只能处理10个，那剩余的用户都会陷入漫长的等待。所以我们需要对应用降级，一旦判断出某些请求是得不到令牌的，就迅速返回失败，避免无谓的等待。
由于RateLimiter是属于单位时间内生成多少个令牌的方式，譬如0.1秒生成1个，那抢购就要看运气了，你刚好是在刚生成1个时进来了，那么你就能抢到，在这0.1秒内其他的请求就算白瞎了，只能寄希望于下一个0.1秒，而从用户体验上来说，不能让他在那一直阻塞等待，所以就需要迅速判断，该用户在某段时间内，还有没有机会得到令牌，这里就需要使用tryAcquire(long
 timeout, TimeUnit unit)方法，指定一个超时时间，一旦判断出在timeout时间内还无法取得令牌，就返回false。注意，这里并不是真正的等待了timeout时间，而是被判断为即便过了timeout时间，也无法取得令牌。这个是不需要等待的。
看实现：
**[java]**[view
 plain](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[copy](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[print](https://www.cnblogs.com/yeyinfu/p/7316972.html#)[?](https://www.cnblogs.com/yeyinfu/p/7316972.html#)
- /**
-      * tryAcquire(long timeout, TimeUnit unit)
-      * 从RateLimiter 获取许可如果该许可可以在不超过timeout的时间内获取得到的话，
-      * 或者如果无法在timeout 过期之前获取得到许可的话，那么立即返回false（无需等待）
-      */
- @RequestMapping("/buy")  
- public Object miao(int count, String code) {  
- //判断能否在1秒内得到令牌，如果不能则立即返回false，不会阻塞程序
- if (!rateLimiter.tryAcquire(1000, TimeUnit.MILLISECONDS)) {  
-             System.out.println("短期无法获取令牌，真不幸，排队也瞎排");  
- return"失败";  
-         }  
- if (goodInfoService.update(code, count) > 0) {  
-             System.out.println("购买成功");  
- return"成功";  
-         }  
-         System.out.println("数据不足，失败");  
- return"失败";  
-     }  
在不看执行结果的情况下，我们可以先分析一下，一秒出10个令牌，0.1秒出一个，100个请求进来，假如100个是同时到达，那么最终只能成交10个，90个都会因为超时而失败。事实上，并不会完全同时到达，必然会出现在0.1秒后到达的，就会被归入下一个周期。这是一个挺复杂的数学问题，每一个请求都会被计算未来可能获取到令牌的概率。
还好，RateLimiter有自己的方法去做判断。
我们运行看结果
![](https://img-blog.csdn.net/20170711172739878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
多执行几次，发现每次这个顺序都不太一样。
经过我多次试验，当设置线程组的间隔时间为0时，最终购买成功的数量总是22.其他的78个都是失败。但基本都是开始和结束时连续成功，中间的大段失败。
我修改一下jmeter线程组这100个请求的产生时间为1秒时，结果如下
![](https://img-blog.csdn.net/20170711173111949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
除了前面几个和最后几个请求连续成功，中间的就比较稳定了，都是隔8个9个就会成功一次。
当我修改为2秒内产生100个请求时，结果就更平均了
![](https://img-blog.csdn.net/20170711173425277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbnlhbGVpeGlhb3d1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
基本上就是前10个成功，后面的就开始按照固定的速率而成功了。
这种场景更符合实际的应用场景，按照固定的单位时间进行分割，每个单位时间产生一个令牌，可供购买。
看到这里是不是有点明白抢小米的情况了，很多时候并不是你网速快，手速快就能抢到，你需要看后台系统的分配情况。所以你能否抢到，最好是开很多个账号，而不是一直用一个账号在猛点，因为你点也白点，后台已经把你的资格排除在外了。
当然了，真正的抢购不是这么简单，瞬间的流量洪峰会冲垮服务器的负载，当100万人抢1万个小米时，连接口都请求不进来，更别提接口里的令牌分配了。
此时就需要做上一层的限流，我们可以选择在上一层做分布式，开多个服务，先做一次限流，淘汰掉绝大多数运气不好的用户，甚至可以随机丢弃某些规则的用户，迅速拦截90%的请求，让你去网页看单机排队动画，还剩10万。10万也太大，足以冲垮数据层，那就进队列MQ，用MQ削峰后，然后才放进业务逻辑里，再进行RateLimiter的限流，此时又能拦截掉90%的不幸者，还剩1万，1万去交给业务逻辑和数据层，用redis和DB来处理库存。恭喜，你就是那个漏网之鱼。
重点在于迅速拦截掉99%的不幸者，避免让他们去接触到数据层。而且不能等待时间太长，最好是请求的瞬间就能确定你是永远看单机动画最好。
/***************************************************************************************************/
补充：
只在本地时效果不怎么明显，我把这个小工程部署到线上服务器压测了一下。
首先试了一下去掉了RateLimiter，只用db的Service处理数据的情况，发现mysql的服务占CPU约20%，总体请求失败率较高。多是Tomcat超时。
使用RateLimiter阻塞后，数据库CPU基本没动静，压力几乎没有，Tomcat超时还有一些，因为还是并发数大，处理不了。
使用RateLimiter非阻塞，超时和请求失败极少，总体QPS上升了不少。
测试不太正规，就大概跑了跑。
