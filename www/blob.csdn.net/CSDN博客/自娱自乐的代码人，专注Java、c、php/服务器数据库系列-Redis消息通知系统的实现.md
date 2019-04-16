# 服务器数据库系列 - Redis消息通知系统的实现 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月27日 17:56:14[initphp](https://me.csdn.net/initphp)阅读数：1272








作者：老王

链接：http://huoding.com/2012/02/29/146







最近忙着用Redis实现一个消息通知系统，今天大概总结了一下技术细节，其中演示代码如果没有特殊说明，使用的都是[PhpRedis](https://github.com/nicolasff/phpredis)扩展来实现的。



## 内存


比如要推送一条全局消息，如果真的给所有用户都推送一遍的话，那么会占用很大的内存，实际上不管粘性有多高的产品，活跃用户同全部用户比起来，都会小很多，所以如果只处理登录用户的话，那么至少在内存消耗上是相当划算的，至于未登录用户，可以推迟到用户下次登录时再处理，如果用户一直不登录，就一了百了了。

## 队列


当大量用户同时登录的时候，如果全部都即时处理，那么很容易就崩溃了，此时可以使用一个队列来保存待处理的登录用户，如此一来顶多是反应慢点，但不会崩溃。


Redis的[LIST](http://www.redis.io/commands/#list)数据类型可以很自然的创建一个队列，代码如下：
<?php

$redis = new Redis;
$redis->connect('/tmp/redis.sock');

$redis->lPush('usr', <USRID>);

while ($usr = $redis->rPop('usr')) {
    var_dump($usr);
}

?>

出于类似的原因，我们还需要一个队列来保存待处理的消息。当然也可以使用LIST来实现，但LIST只能按照插入的先后顺序实现类似FIFO或LIFO形式的队列，然而消息实际上是有优先级的：比如说个人消息优先级高，全局消息优先级低。此时可以使用[ZSET](http://www.redis.io/commands/#sorted_set)来实现，它里面分数的概念很自然的实现了优先级。


不过ZSET没有原生的POP操作，所以我们需要模拟实现，代码如下：
<?php

class RedisClient extends Redis
{
    const POSITION_FIRST = 0;
    const POSITION_LAST = -1;

    public function zPop($zset)
    {
        return $this->zsetPop($zset, self::POSITION_FIRST);
    }

    public function zRevPop($zset)
    {
        return $this->zsetPop($zset, self::POSITION_LAST);
    }

    private function zsetPop($zset, $position)
    {
        $this->watch($zset);

        $element = $this->zRange($zset, $position, $position);

        if (!isset($element[0])) {
            return false;
        }

        if ($this->multi()->zRem($zset, $element[0])->exec()) {
            return $element[0];
        }

        return $this->zsetPop($zset, $position);
    }
}

?>

模拟实现了POP操作后，我们就可以使用ZSET实现队列了，代码如下：
<?php

$redis = new RedisClient;
$redis->connect('/tmp/redis.sock');

$redis->zAdd('msg', <PRIORITY>, <MSGID>);

while ($msg = $redis->zRevPop('msg')) {
    var_dump($msg);
}

?>
## 推拉


以前微博架构中推拉选择的问题已经被大家讨论过很多次了。实际上消息通知系统和微博差不多，也存在推拉选择的问题，同样答案也是类似的，那就是应该推拉结合。具体点说：在登陆用户获取消息的时候，就是一个拉消息的过程；在把消息发送给登陆用户的时候，就是一个推消息的过程。

## 速度


假设要推送一百万条消息的话，那么最直白的实现就是不断的插入，代码如下：
<?php

for ($msgid = 1; $msgid <= 1000000; $msgid++) {
    $redis->sAdd('usr:<USRID>:msg', $msgid);
}

?>

Redis的速度是很快的，但是借助[PIPELINE](http://redis.io/topics/pipelining)，会更快，代码如下：
<?php

for ($i = 1; $i <= 100; $i++) {
    $redis->multi(Redis::PIPELINE);
    for ($j = 1; $j <= 10000; $j++) {
        $msgid = ($i - 1) * 10000 + $j;
        $redis->sAdd('usr:<USRID>:msg', $msgid);
    }
    $redis->exec();
}

?>

说明：所谓PIPELINE，就是省略了无谓的折返跑，把命令打包给服务端统一处理。


前后两段代码在我的测试里，使用PIPELINE的速度大概是不使用PIPELINE的十倍。

## 查询


我们用Redis命令行来演示一下用户是如何查询消息的。


先插入三条消息，其<MSGID>分别是1，2，3：
redis> HMSET msg:1 title title1 content content1
redis> HMSET msg:2 title title2 content content2
redis> HMSET msg:3 title title3 content content3

再把这三条消息发送给某个用户，其<USRID>是123：
redis> SADD usr:123:msg 1
redis> SADD usr:123:msg 2
redis> SADD usr:123:msg 3

此时如果简单查询用户有哪些消息的话，无疑只能查到一些<MSGID>：
redis> SMEMBERS usr:123:msg
1) "1"
2) "2"
3) "3"

如果还需要用程序根据<MSGID>再来一次查询无疑有点低效，好在Redis内置的[SORT](http://www.redis.io/commands/sort)命令可以达到事半功倍的效果，实际上它类似于SQL中的JOIN：
redis> SORT usr:123:msg GET msg:*->title
1) "title1"
2) "title2"
3) "title3"
redis> SORT usr:123:msg GET msg:*->content
1) "content1"
2) "content2"
3) "content3"

SORT的缺点是它只能GET出字符串类型的数据，如果你想要多个数据，就要多次GET：
redis> SORT usr:123:msg GET msg:*->title GET msg:*->content
1) "title1"
2) "content1"
3) "title2"
4) "content2"
5) "title3"
6) "content3"

很多情况下这显得不够灵活，好在我们可以采用其他一些方法平衡一下利弊，比如说新加一个字段，冗余保存完整消息的序列化，接着只GET这个字段就OK了。


实际暴露查询接口的时候，不会使用PHP等程序来封装，因为那会成倍降低RPS，推荐使用[Webdis](http://webd.is/)，它是一个Redis的Web代理，效率没得说。


…


最近[Tumblr](https://www.tumblr.com/)发表了一篇类似的文章：[Staircar:
 Redis-powered notifications](http://engineering.tumblr.com/post/7819252942/staircar-redis-powered-notifications)，介绍了他们使用Redis实现消息通知系统的一些情况，有兴趣的不妨一起看看。




