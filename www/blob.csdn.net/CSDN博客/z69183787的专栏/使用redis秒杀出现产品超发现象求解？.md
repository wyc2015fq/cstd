# 使用redis秒杀出现产品超发现象求解？ - z69183787的专栏 - CSDN博客
2018年02月05日 18:54:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1351
[https://segmentfault.com/q/1010000007165163](https://segmentfault.com/q/1010000007165163)
经典的[Check-then-Act](http://brevitaz.com/race-condition-java-concurrency-2/)错误
最近在做一个秒杀活动，处于性能和响应速度的考虑，使用了redis。写的时候就特别注意了杜绝超发现象，基于redis理论的cas（check and set）乐观锁，想着应该能够杜绝该问题，但是还是出现了，很疑惑求大神帮助，具体的代码大致如下：
```
**<?php**  
header("content-type:text/html;charset=utf-8");  
$redis = **new** redis();  
$result = $redis->connect('10.10.10.119', 6379);  
$mywatchkey = $redis->get("mywatchkey");  
$rob_total = 100;   //抢购数量  
**if**($mywatchkey<$rob_total){  
    $redis->watch("mywatchkey");  
    $redis->multi();  
      
    //设置延迟，方便测试效果。  
    sleep(5);  
    //插入抢购数据  
    $redis->hSet("mywatchlist","user_id_".mt_rand(1, 9999),time());  
    $redis->set("mywatchkey",$mywatchkey+1);  
    $rob_result = $redis->exec();  
    **if**($rob_result){  
        $mywatchlist = $redis->hGetAll("mywatchlist");  
        **echo** "抢购成功！<br/>";  
        **echo** "剩余数量：".($rob_total-$mywatchkey-1)."<br/>";  
        **echo** "用户列表：<pre>";  
        var_dump($mywatchlist);  
    }**else**{  
        **echo** "手气不好，再抢购！";**exit**;  
    }  
}  
**?>**
```
**重点：上面是错误的。。。。。。。。。。。。。。。。。。。。。。。。。。。**
我觉得这个代码在高并发的情况，仍然会出现超卖现象。假如：只剩 1个奖品的时候，有三个人同时执行 `$redis->watch("mywatchkey")` 拿到的数据是 99 ，那么就出现超卖现象了。
**如果在A、B都得到99以后，A先执行完了，B才开始watch呢**
由于`redis`是单线程读取，那么就用最简单的队列实现吧。
- 
在抽奖前先把奖品数量，写入`redis`队列`award:100` // 长度为100的 list ，值只是作为是否中奖
- 
并发抽奖
```
$award = $redis->lpop('award:100'); // 由于队列只有100个值，可以确保只有100个人中奖
**if**(!$award){
    echo "手气不好，再抢购！";**exit**;  
}
// 剩下就是中奖操作的事情了
```
```php
亲测，用ab 压测并发500 请求4000 无超卖！
<?php
header("content-type:text/html;charset=utf-8");
$redis = new redis();
$result = $redis->connect('127.0.0.1', 7379);
$redis->watch("mywatchlist");
$len = $redis->hlen("mywatchlist");
$rob_total = 100; //抢购数量
if ($len < $rob_total) {
$redis->multi();
$redis->hSet("mywatchlist", "user_id_" . mt_rand(1, 999999), time());
$rob_result = $redis->exec();
file_put_contents("log.txt", $len . PHP_EOL, FILE_APPEND);
if ($rob_result) {
    $mywatchlist = $redis->hGetAll("mywatchlist");
    echo "抢购成功！<br/>";
    echo "剩余数量：" . ($rob_total - $len - 1) . "<br/>";
    echo "用户列表：<pre>";
    var_dump($mywatchlist);
} else {
    echo "手气不好，再抢购！";
    exit;
}
} else {
echo "已卖光！";
exit;
}
?>
```
Optimistic locking using check-and-set。你似乎调用反了，应该是先watch，再get。而且乐观锁似乎只控制当你修改的时候是否和你原来获得的一样，也就是说在高并发情况下很容易出现一堆错误，至于超选不是它控制的。
[http://brevitaz.com/race-condition-java-concurrency-2/](http://brevitaz.com/race-condition-java-concurrency-2/)
