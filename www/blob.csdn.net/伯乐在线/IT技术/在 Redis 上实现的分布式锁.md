# 在 Redis 上实现的分布式锁 - 文章 - 伯乐在线
原文出处： [Kelly](http://www.cnblogs.com/it-cen/p/4440745.html)
由于近排很忙，忙各种事情，还有工作上的项目，已经超过一个月没写博客了，确实有点惭愧啊，没能每天或者至少每周坚持写一篇博客。这一个月里面接触到很多新知识，同时也遇到很多技术上的难点，在这我将对每一个有用的技术点做一个小小的分析理解和总结。每天去学会总结，才会有进步。
本次对我在工作上的项目中用到的技术，在Redis上实现分布式锁，进行一个分析和总结。
**先了解下什么是分布式锁，在百科上是这么定义的：**
> 
分布式锁是控制分布式系统之间同步访问共享资源的一种方式。在分布式系统中，常常需要协调他们的动作。如果不同的系统或是同一个系统的不同主机之间共享了一个或一组资源，那么访问这些资源的时候，往往需要互斥来防止彼此干扰来保证一致性，在这种情况下，便需要使用到分布式锁。
简单的理解就是：分布式锁是一个在很多环境中非常有用的原语，它是不同的系统或是同一个系统的不同主机之间互斥操作共享资源的有效方法。
**背景：**
在很多互联网产品应用中，有些场景需要加锁处理，比如：秒杀，全局递增ID，楼层生成等等。大部分是解决方案基于DB实现的，Redis为单进程单线程模式，采用队列模式将并发访问变成串行访问，且多客户端对Redis的连接并不存在竞争关系。
**我们的项目：**
我们现在的项目中，任务队列用到分布式锁的情况比较多，在将业务逻辑中可以异步处理的操作放入队列，在其他线程中处理后出队，此时队列中使用了分布式锁，保证入队和出队的一致性。关于redis队列这块的逻辑分析，我将在下一次对其进行总结，此处先略过。
**接下来对redis实现的分布式锁的逻辑代码进行详细的分析和理解：**
1、为避免特殊原因导致锁无法释放, 在加锁成功后, 锁会被赋予一个生存时间(通过 lock 方法的参数设置或者使用默认值), 超出生存时间锁将被自动释放.
2、锁的生存时间默认比较短(秒级, 具体见 lock 方法), 因此若需要长时间加锁, 可以通过 expire 方法延长锁的生存时间为适当的时间. 比如在循环内调用 expire
3、系统级的锁当进程无论因为任何原因出现crash，操作系统会自己回收锁，所以不会出现资源丢失。
4、但分布式锁不同。若一次性设置很长的时间，一旦由于各种原因进程 crash 或其他异常导致 unlock 未被调用，则该锁在剩下的时间就变成了垃圾锁，导致其他进程或进程重启后无法进入加锁区域。

PHP
```
<?php
require_once 'RedisFactory.php';
/**
* 在 Redis 上实现的分布式锁
*/
class RedisLock {
    //单例模式
    private static $_instance = null;
    public static function instance() {
        if(self::$_instance == null) {
            self::$_instance = new RedisLock();
        }
        return self::$_instance;
    }
    //redis对象变量
    private $redis;
    //存放被锁的标志名的数组
    private $lockedNames = array();
    public function __construct() {
        //获取一个 RedisString 实例
        $this->redis = RedisFactory::instance()->getString();
    }
    /** 
    * 加锁
    *
    * @param string 锁的标识名
    * @param int 获取锁失败时的等待超时时间(秒), 在此时间之内会一直尝试获取锁直到超时. 为 0 表示失败后直接返回不等待
    * @param int 当前锁的最大生存时间(秒), 必须大于 0 . 如果超过生存时间后锁仍未被释放, 则系统会自动将其强制释放
    * @param int 获取锁失败后挂起再试的时间间隔(微秒)
    */
    public function lock($name, $timeout = 0, $expire = 15, $waitIntervalUs = 100000) {
        if(empty($name)) return false;
        $timeout = (int)$timeout;
        $expire = max((int)$expire, 5);
        $now = microtime(true);
        $timeoutAt = $now + $timeout;
        $expireAt = $now + $expire;
        $redisKey = "Lock:$name";
        while(true) {
            $result = $this->redis->setnx($redisKey, (string)$expireAt);
            if($result !== false) {
                //对$redisKey设置生存时间
                $this->redis->expire($redisKey, $expire);
                //将最大生存时刻记录在一个数组里面
                $this->lockedNames[$name] = $expireAt;
                return true;
            }
            //以秒为单位，返回$redisKey 的剩余生存时间
            $ttl = $this->redis->ttl($redisKey);
            // TTL 小于 0 表示 key 上没有设置生存时间(key 不会不存在, 因为前面 setnx 会自动创建)
            // 如果出现这种情况, 那就是进程在某个实例 setnx 成功后 crash 导致紧跟着的 expire 没有被调用. 这时可以直接设置 expire 并把锁纳为己用
            if($ttl < 0) {
                $this->redis->set($redisKey, (string)$expireAt, $expire);
                $this->lockedNames[$name] = $expireAt;
                return true;
            }
            // 设置了不等待或者已超时
            if($timeout <= 0 || microtime(true) > $timeoutAt) break;
            // 挂起一段时间再试
            usleep($waitIntervalUs);
        }
        return false;
    }
    /**
    * 给当前锁增加指定的生存时间(秒), 必须大于 0
    *
    * @param string 锁的标识名
    * @param int 生存时间(秒), 必须大于 0
    */
    public function expire($name, $expire) {
        if($this->isLocking($name)) {
            if($this->redis->expire("Lock:$name", max($expire, 1))) {
                return true;
            }
        }
        return false;
    }
    /**
    * 判断当前是否拥有指定名称的锁
    *
    * @param mixed $name
    */
    public function isLocking($name) {
        if(isset($this->lockedNames[$name])) {
            return (string)$this->lockedNames[$name] == (string)$this->redis->get("Lock:$name");
        }
        return false;
    }
    /**
    * 释放锁
    *
    * @param string 锁的标识名
    */
    public function unlock($name) {
        if($this->isLocking($name)) {
            if($this->redis->deleteKey("Lock:$name")) {
                unset($this->lockedNames[$name]);
                return true;
            }
        }
        return false;
    }
    /** 释放当前已经获取到的所有锁 */
    public function unlockAll() {
        $allSuccess = true;
        foreach($this->lockedNames as $name => $item) {
            if(false === $this->unlock($name)) {
                $allSuccess = false;
            }
        }
        return $allSuccess;
    }
}
```
此类很多代码都写上了注释，只要认真理解下，就很容易懂得如何在redis实现分布式锁了。
另外，我在网上找到另一篇关于redis实现分布式锁的文章：《[使用 Redis 实现分布式锁](http://blog.jobbole.com/95211/)》。我感觉挺不错的，推荐给大家。结合我所总结的和我推荐的文章做对比，基本上能理解清楚是如何在redis实现分布式锁的了。
如果此博文中有哪里讲得让人难以理解，欢迎留言交流，若有讲解错的地方欢迎指出。
