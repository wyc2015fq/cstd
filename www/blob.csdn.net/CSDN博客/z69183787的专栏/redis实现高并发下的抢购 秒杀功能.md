# redis实现高并发下的抢购/秒杀功能 - z69183787的专栏 - CSDN博客
2018年03月29日 19:18:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1107
[https://www.cnblogs.com/phpper/p/7085663.html](https://www.cnblogs.com/phpper/p/7085663.html)
之前写过一篇文章，[高并发的解决思路(点此进入查看)](http://www.cnblogs.com/phpper/p/6716248.html),今天再次抽空整理下实际场景中的具体代码逻辑实现吧：
抢购/秒杀是如今很常见的一个应用场景，那么高并发竞争下如何解决超抢(或超卖库存不足为负数的问题)呢？
常规写法：
查询出对应商品的库存，看是否大于0，然后执行生成订单等操作，但是在判断库存是否大于0处，如果在高并发下就会有问题，导致库存量出现负数
这里我就只谈redis的解决方案吧...
我们先来看以下代码（这里我以laravel为例吧）是否能正确解决超抢/卖的问题：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
<?php
 
 $num = 10;   //系统库存量
 $user_id =  \Session::get('user_id');//当前抢购用户id
 $len = \Redis::llen('order:1');  //检查库存，order:1 定义为健名
 if($len >= $num)
 　　return '已经抢光了哦';
$result = \Redis::lpush('order:1',$user_id);  //把抢到的用户存入到列表中
if($result)
　　return '恭喜您!抢到了哦';
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
如果代码正常运行，按照预期理解的是列表order:1中最多只能存储10个用户的id，因为库存只有10个。
然而，但是,在使用jmeter工具模拟多用户并发请求时，最后发现order:1中总是超过5个用户，也就是出现了“超抢/超卖”。
分析问题就出在这一段代码：
```
$len = \Redis::llen('order:1');  //检查库存，order:1 定义为健名
 if($len >= $num)
 　　return '已经抢光了哦';
```
在抢购进行到一定程度，假如现在已经有9个人抢购成功，又来了3个用户同时抢购，这时if条件将会被绕过(条件同时被满足了)，这三个用户都能抢购成功。而实际上只剩下一件库存可以抢了。
在高并发下，很多看似不大可能是问题的，都成了实际产生的问题了。要解决“超抢/超卖”的问题，核心在于保证检查库存时的操作是依次执行的，再形象的说就是把“多线程”转成“单线程”。即使有很多用户同时到达，也是一个个检查并给与抢购资格，一旦库存抢尽，后面的用户就无法继续了。
我们需要使用redis的原子操作来实现这个“单线程”。首先我们把库存存在goods_store:1这个列表中，假设有10件库存，就往列表中push10个数，这个数没有实际意义，仅仅只是代表一件库存。抢购开始后，每到来一个用户，就从goods_store:1中pop一个数，表示用户抢购成功。当列表为空时，表示已经被抢光了。因为列表的pop操作是原子的，即使有很多用户同时到达，也是依次执行的。抢购的示例代码如下：
比如这里我先把库存(可用库存,这里我强调下哈,一般都是商品详情页抢购,后来者进来看到的库存可能不再是后台系统配置的10个库存数了)放入redis队列：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
$num=10; //库存
 $len=\Redis::llen('goods_store:1'); //检查库存,goods_store:1 定义为健名
 $count = $num-$len; //实际库存-被抢购的库存 = 剩余可用库存
 for($i=0;$i<$count;$i++)
 　　\Redis::lpush('goods_store:1',1);//往goods_store列表中,未抢购之前这里应该是默认滴push10个库存数了
　//echo \Redis::llen('goods_store:1');//未抢购之前这里就是10了
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
好吧，抢购时间到了：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
/* 模拟抢购操作,抢购前判断redis队列库存量 */
 $count=\Redis::lpop('goods_store:1');//lpop是移除并返回列表的第一个元素。
 if(!$count)
    return '已经抢光了哦';
```
` /* 下面处理抢购成功流程 */``\DB::table('goods')->decrement('num', 1);//减少num库存字段`
![复制代码](https://common.cnblogs.com/images/copycode.gif)
用户抢购成功后，上面的我们也可以稍微优化下，比如我们可用将用户ID存入了order:1列表中。接下来我们可以引导这些用户去完成订单的其他步骤，到这里才涉及到与数据库的交互。最终只有很少的人走到这一步吧，也就解决的数据库的压力问题。
我们再改下上面的代码：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
$user_id =  \Session::get('user_id');//当前抢购用户id
/* 模拟抢购操作,抢购前判断redis队列库存量 */
$count=\Redis::lpop('goods_store:1');
if(!$count)
　　return '已经抢光了哦';
$result = \Redis::lpush('order:1',$user_id);
if($result)
　　return '恭喜您!抢到了哦';
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
为了检测实际效果，我使用jmeter工具模拟100、200、1000个用户并发进行抢购，经过大量的测试，最终抢购成功的用户始终为10，没有出现“超抢/超卖”。
上面只是简单模拟高并发下的抢购思路，真实场景要比这复杂很多，比如双11活动远远比这更复杂多啦，很多注意的地方如抢购活动页面做成静态的，通过ajax调用接口
再如上面的会导致一个用户抢多个，思路：
需要一个排队队列(比如：queue:1,以user_id为值的列表)和抢购结果队列(比如：order:1,以user_id为值的列表)及库存队列(比如上面的goods_store:1)。高并发情况，先将用户进入排队队列，用一个线程循环处理从排队队列取出一个用户，判断用户是否已在抢购结果队列，如果在则已抢购，否则未抢购，接着执行库存减1，写入数据库，将此user_id用户同时也进入结果队列。
