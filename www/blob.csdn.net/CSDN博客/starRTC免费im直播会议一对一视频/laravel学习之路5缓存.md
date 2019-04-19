# laravel学习之路5缓存 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月18日 12:35:26[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：192
redis需要先安装
需要通过 Composer 安装 predis/predis 扩展包
 (~1.0) 或者使用 PECL 安装 PhpRedis PHP 拓展。
composer
require predis/predis
redis的配置在cache.php和database.php里面都有
Redis 多连接的配置：
[https://laravel-china.org/topics/2466](https://laravel-china.org/topics/2466)
**集群配置**
使用 clusters 键
PhpRedis
If you have the PhpRedis PHP extension installed via PECL, you will need to rename the Redis alias
 in your config/app.php configuration
 file.
Redis facade可以调用任何的 Redis 命令，如set, lrange
注意不是Cache facade
Redis::*set*('key2',
'value2');//work
管道化命令pipeline可以运行多个
 命令
发布与订阅
[https://redis.io/commands/publish](https://redis.io/commands/publish)
php artisan redis:subscribe
your message is:{"foo1":"bar1"}
使用Cache facade
$value= Cache::get('key','default');刚开始取不到数据，是什么原因？
//'prefix' => 'laravel',
'prefix'
=>
'',

如果不存在，就返回数据库里面的值
$value= Cache::get('key',function(){return DB::table(...)->get();});
也可以使用全局函数cache('key');

从缓存中取出所有用户，或者当用户不存在时，从数据库中将这些用户取出并放入缓存中，你可以使用 Cache::remember 方法实现：
$value= Cache::remember('users',$minutes,function(){return DB::table('users')->get();});
获取一个缓存项然后删除它，可以使用 pull 方法
手动在命令行里面设置的key,为何报unserialize
"s:11:\"elesos1.com\";" 这个是程序存储的值elesos1.com
直接用
Redis::set('elesos3', "elesos33");
return Redis::get('elesos3');
不用序列号
或cache存，cache取也不用（cache内部会自动序列化与反序列化）
Cache::*store*('redis')->put('elesos4',
'elesos44',
10);
**return**Cache::*get*('elesos4',
'no this key:1');
但cache存储的在命令行中看是序列化后的数据： "s:8:\"elesos44\";"
只有你redis存 cache取  或者反过来才需要序列化或反序列化
如
Redis::*set*('elesos6',
serialize("elesos66"));
**return**Cache::*get*('elesos6',
'no this key:1');//会自动反序列化
Cache::*store*('redis')->put('elesos7',
'elesos77',
60);
 //60单位是分钟
**return**Redis::*get*('elesos7');
// s:8:"elesos77";
**return**unserialize(Redis::*get*('elesos7'));

过期
$expiresAt=Carbon::now()->addMinutes(10);Cache::put('key','value',$expiresAt);
cache(['key'=>'value'],$minutes);cache(['key'=>'value'],
 Carbon::now()->addSeconds(10));
**Store If Not Present：add**
**永久写入**forever必须通过 forget 方法手动删除
flush 方法清空所有缓存
还可以监听缓存事件
'Illuminate\Cache\Events\CacheHit'=>['App\Listeners\LogCacheHit',],
