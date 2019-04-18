# redis分布式锁-自动超时锁（在用） - z69183787的专栏 - CSDN博客
2018年08月02日 17:29:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1500
[http://www.cnblogs.com/jiangtao1218/p/9349788.html](http://www.cnblogs.com/jiangtao1218/p/9349788.html)
1、加锁代码结构
2、解锁代码结构
3、java实例
4、测试类
5、测试日志
加锁代码结构
![复制代码](http://common.cnblogs.com/images/copycode.gif)
def acquire_lock_with_timeout（conn,lockname,acquire_timeout,lock_timeout)
    identifer=uuid.uuid4
    lockname='lock:'+lockname
    repeat_end_time=current_time()+acquire_timeout
    
    while current_time<repeat_end_time
        if conn.setnx(lockname,identifer)
            conn.expire(lockname,lock_timeout)
            return identifer
        elif not conn.ttl(lockname)
            conn.expire(lockname,lock_timeout)
        time.sleep(0.001)
    return false
![复制代码](http://common.cnblogs.com/images/copycode.gif)
解锁代码结构
![复制代码](http://common.cnblogs.com/images/copycode.gif)
def release_loc(conn,lockname,identifer)
    pipe=conn.pipeline(true)
    lockname='lock:'+lockname
    while True
        try:
            pipe.watch(lockname)
            if pipe.get(lockname) == identifer   // 检查进程是否仍然是有锁，若未持有锁，则返回false
                pipe.multi()
                pipe.delete(lockname)
                pipe.execute
                return true
            pipe.unwatch()
            break
        except redis.exceptions.WatchError
            pass    // 有其他客户端修改了锁，重试
    return False
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 java实例
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import redis.clients.jedis.Jedis;
import redis.clients.jedis.JedisPool;
import redis.clients.jedis.Transaction;
import redis.clients.jedis.exceptions.JedisException;
import java.util.List;
import java.util.UUID;
public class Distributed_Lock {
    public static String lock_withTimeout(String lockName, long acquire_timeout, long timeout) {
        Jedis redis = null;
        String key_identifier = null;
        try {
            String identifier = Thread.currentThread().getName() + ":" + UUID.randomUUID().toString(); // Thread
                                                                                                        // just
                                                                                                        // for
                                                                                                        // debug
            String lockKey = "lock:" + lockName;
            int lockExpire = (int) (timeout / 1000);
            long end = System.currentTimeMillis() + acquire_timeout;
            while (System.currentTimeMillis() < end) {
                redis = JedisUtil.getJedis();
                if (redis == null) {
                    System.out.println(Thread.currentThread().getName()
                            + "  warning!!! lock_withTimeout can not get redis conn ,program repeat for get the conn");
                    Thread.sleep(100);
                    continue;
                }
                if (redis.setnx(lockKey, identifier) == 1) {// execute
                                                            // successfully will
                                                            // return "1"
                    redis.expire(lockKey, lockExpire);
                    key_identifier = identifier;
                    System.out.println(Thread.currentThread().getName() + "  获取锁:"+key_identifier);
                    return key_identifier;
                }
                if (redis.ttl(lockKey) == -1) {
                    redis.expire(lockKey, lockExpire);
                }
                long lockKey_ttl = redis.ttl(lockKey);
                try {
                    System.out.println(Thread.currentThread().getName()
                            + "  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：" + lockKey_ttl);
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
                if (redis != null) {
                    redis.close();
                }
            }
            System.out.println(Thread.currentThread().getName() + "  获取redis连接失败，放弃获取锁");
        } catch (Exception e) {
            System.out.println(Thread.currentThread().getName() + "  获取锁发生异常");
            e.printStackTrace();
        } finally {
            if (redis != null) {
                redis.close();
            }
        }
        return key_identifier;
    }
    public static boolean lock_release(String lockName, String identifier) {
        Jedis redis = null;
        String lockKey = "lock:" + lockName;
        boolean retFlag = false;
        String _temp_identifier_from_redis = "";
        try {
            redis = JedisUtil.getJedis();
            while (true) {
                if (redis == null) {
                    System.out.println(Thread.currentThread().getName()
                            + "  warning!!! lock_release can not get redis function,program repeat for get the conn");
                    try {
                        Thread.sleep(10);
                        redis = JedisUtil.getJedis();
                    } catch (InterruptedException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                    }
                    continue;
                }
                redis.watch(lockKey);
                _temp_identifier_from_redis = redis.get(lockKey);
                if (_temp_identifier_from_redis == null || "".equals(_temp_identifier_from_redis)) {
                    System.out.println(Thread.currentThread().getName() + "  锁已过期失效失效");
                } else if (identifier.equals(_temp_identifier_from_redis)) {
                    long del_result = redis.del(lockKey);
                    if (del_result == 1) {
                        System.out.println(Thread.currentThread().getName() + "  完成任务，释放锁");
                        retFlag = true;
                    } else {
                        System.out.println(Thread.currentThread().getName() + "  释放锁失败，锁已提前释放");
                        //continue;
                    }
                } else {
                    System.out.println(Thread.currentThread().getName() + "  锁已过期失效，被污染");
                }
                redis.unwatch();
                break;
            }
        } catch (JedisException e) {
            e.printStackTrace();
        } finally {
            if (redis != null) {
                redis.close();
            }
        }
        return retFlag;
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
测试类
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import redis.clients.jedis.Jedis;
public class TestDistributed_Lock {
    public static void main(String[] args) {
        System.out.println( " main start");
        for (int i = 0; i < 100; i++) {
            Test_Thread_lock threadA = new Test_Thread_lock();
            threadA.start();
            try {
                Thread.sleep(1);
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }
}
class Test_Thread_lock extends Thread {
    public void run() {
        System.out.println(Thread.currentThread().getName() + " 启动运行");
        String indentifier = Distributed_Lock.lock_withTimeout("resource", 500000, 100000);
        if (indentifier == null) {
            System.out.println(Thread.currentThread().getName() + " 获取锁失败，取消任务");
        }else{
            System.out.println(Thread.currentThread().getName() + "  执行任务");
            Distributed_Lock.lock_release("resource", indentifier);
        }
        System.out.println(Thread.currentThread().getName() + " 完成推出");
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
测试日志
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 main start
Thread-0 启动运行
Thread-1 启动运行
Thread-2 启动运行
Thread-3 启动运行
Thread-4 启动运行
Thread-5 启动运行
Thread-6 启动运行
Thread-7 启动运行
Thread-8 启动运行
Thread-9 启动运行
Thread-10 启动运行
Thread-11 启动运行
Thread-12 启动运行
Thread-13 启动运行
Thread-14 启动运行
Thread-15 启动运行
Thread-16 启动运行
Thread-17 启动运行
Thread-18 启动运行
Thread-19 启动运行
Thread-20 启动运行
Thread-21 启动运行
Thread-22 启动运行
Thread-23 启动运行
Thread-24 启动运行
Thread-25 启动运行
Thread-26 启动运行
Thread-27 启动运行
Thread-28 启动运行
Thread-29 启动运行
Thread-30 启动运行
Thread-31 启动运行
Thread-32 启动运行
Thread-33 启动运行
Thread-34 启动运行
Thread-35 启动运行
Thread-36 启动运行
Thread-37 启动运行
Thread-38 启动运行
Thread-39 启动运行
Thread-40 启动运行
Thread-41 启动运行
Thread-42 启动运行
Thread-43 启动运行
Thread-44 启动运行
Thread-45 启动运行
Thread-46 启动运行
Thread-47 启动运行
Thread-48 启动运行
Thread-49 启动运行
Thread-50 启动运行
Thread-51 启动运行
Thread-52 启动运行
Thread-53 启动运行
Thread-54 启动运行
Thread-55 启动运行
Thread-56 启动运行
Thread-57 启动运行
Thread-58 启动运行
Thread-59 启动运行
Thread-60 启动运行
Thread-61 启动运行
Thread-62 启动运行
Thread-63 启动运行
Thread-64 启动运行
Thread-65 启动运行
Thread-66 启动运行
Thread-67 启动运行
Thread-68 启动运行
Thread-69 启动运行
Thread-70 启动运行
Thread-71 启动运行
Thread-72 启动运行
Thread-73 启动运行
Thread-74 启动运行
Thread-75 启动运行
Thread-76 启动运行
Thread-77 启动运行
Thread-78 启动运行
Thread-79 启动运行
Thread-80 启动运行
Thread-81 启动运行
Thread-82 启动运行
Thread-83 启动运行
Thread-84 启动运行
Thread-85 启动运行
Thread-86 启动运行
Thread-87 启动运行
Thread-88 启动运行
Thread-89 启动运行
Thread-90 启动运行
Thread-91 启动运行
Thread-92 启动运行
Thread-93 启动运行
Thread-94 启动运行
Thread-95 启动运行
Thread-96 启动运行
Thread-97 启动运行
Thread-98 启动运行
Thread-99 启动运行
Thread-98  获取锁:Thread-98:2c75baea-0d98-48bb-8736-3d96be3b9e12
Thread-98  执行任务
Thread-97  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-16  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-15  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-14  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-95  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-99  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-13  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-12  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-11  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-9  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-10  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-8  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-7  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-4  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-6  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-1  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-0  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-5  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-3  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-17  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-18  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-19  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-20  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-21  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-22  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-23  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-24  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-25  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-26  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-27  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-28  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-29  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-30  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-31  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-32  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-33  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-34  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-35  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-36  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-37  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-38  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-39  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-40  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-41  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-42  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-43  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-44  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-45  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-46  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-47  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-48  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-49  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-50  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-51  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-52  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-53  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-54  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-55  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-56  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-57  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-58  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-59  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-60  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-61  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-62  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-63  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-64  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-96  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-65  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-66  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-67  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-68  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-69  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-70  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-71  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-72  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-73  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-74  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-75  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-76  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-78  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-77  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-79  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-80  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-2  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-94  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-93  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-81  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-92  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-83  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-82  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-84  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-85  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-91  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-90  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-86  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-87  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-89  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-88  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-98  完成任务，释放锁
Thread-98 完成推出
Thread-16  获取锁:Thread-16:4172d60b-795c-42b6-ab79-6bd35ab3d393
Thread-16  执行任务
Thread-95  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-99  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-16  完成任务，释放锁
Thread-16 完成推出
Thread-12  获取锁:Thread-12:09d449e3-c290-46f3-99d0-5975ed3e2b74
Thread-97  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-12  执行任务
Thread-13  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-9  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-8  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-12  完成任务，释放锁
Thread-20  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-21  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-12 完成推出
Thread-24  获取锁:Thread-24:4ec8404c-b5e6-4e52-94eb-e960afff5592
Thread-24  执行任务
Thread-5  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-0  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-17  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-52  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-51  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-48  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-44  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-49  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-45  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-1  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-59  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-60  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-4  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-63  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-64  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-96  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-61  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-65  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-70  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-66  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-73  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-75  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-76  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-80  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-58  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-74  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-92  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-83  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-84  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-91  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-77  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-87  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-78  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-88  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-57  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-2  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-93  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-94  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-81  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-82  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-85  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-90  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-86  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-89  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-54  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-53  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-50  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-55  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-56  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-47  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-24  完成任务，释放锁
Thread-24 完成推出
Thread-40  获取锁:Thread-40:6074038b-6b5d-466e-95f4-26ccd8881e03
Thread-40  执行任务
Thread-40  完成任务，释放锁
Thread-40 完成推出
Thread-43  获取锁:Thread-43:c84d78c8-21fe-4896-91b0-9902d4b8c1ea
Thread-43  执行任务
Thread-46  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-42  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-41  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-38  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-37  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-34  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-33  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-30  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-29  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-26  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-23  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-22  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-19  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-18  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-11  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-3  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-15  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-14  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-39  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-35  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-10  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-7  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-6  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-36  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-28  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-32  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-31  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-27  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-25  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-43  完成任务，释放锁
Thread-43 完成推出
Thread-79  获取锁:Thread-79:6fdce7a5-19a1-453a-bc71-7d9925e78cdc
Thread-79  执行任务
Thread-69  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-71  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-72  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-68  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-67  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-62  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-79  完成任务，释放锁
Thread-79 完成推出
Thread-99  获取锁:Thread-99:9bdf89e4-712b-4ae1-8ef5-58769d8b6027
Thread-99  执行任务
Thread-95  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-99  完成任务，释放锁
Thread-99 完成推出
Thread-97  获取锁:Thread-97:7ecb43e6-af95-47d1-9598-26d2db43de15
Thread-97  执行任务
Thread-97  完成任务，释放锁
Thread-97 完成推出
Thread-13  获取锁:Thread-13:2958075d-6e58-452f-b736-ee20663d3017
Thread-13  执行任务
Thread-13  完成任务，释放锁
Thread-13 完成推出
Thread-9  获取锁:Thread-9:2c1f6177-203e-43b2-81a0-4694c58b1cfb
Thread-9  执行任务
Thread-8  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-9  完成任务，释放锁
Thread-9 完成推出
Thread-20  获取锁:Thread-20:056395e7-5e7e-444a-b5bb-5040c7edc12d
Thread-20  执行任务
Thread-21  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-20  完成任务，释放锁
Thread-20 完成推出
Thread-5  获取锁:Thread-5:076d1c88-3d67-4b0a-8822-9866246f3b0a
Thread-5  执行任务
Thread-0  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-5  完成任务，释放锁
Thread-51  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-5 完成推出
Thread-48  获取锁:Thread-48:c8d0ca49-0b17-43d9-bc21-e5c6819e640e
Thread-48  执行任务
Thread-52  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-44  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-17  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-48  完成任务，释放锁
Thread-48 完成推出
Thread-49  获取锁:Thread-49:7c9fbc26-d959-4f5c-bfcc-43d716f06f5f
Thread-49  执行任务
Thread-45  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-59  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-49  完成任务，释放锁
Thread-49 完成推出
Thread-1  获取锁:Thread-1:7b6b5faa-3465-4252-93a1-041336b91eec
Thread-1  执行任务
Thread-1  完成任务，释放锁
Thread-1 完成推出
Thread-60  获取锁:Thread-60:f7eedc16-6a7b-4652-bf6f-f1c13942a897
Thread-60  执行任务
Thread-4  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-63  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-60  完成任务，释放锁
Thread-60 完成推出
Thread-64  获取锁:Thread-64:a989aaaf-405d-43fa-b423-46a53dc22f32
Thread-64  执行任务
Thread-96  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-64  完成任务，释放锁
Thread-64 完成推出
Thread-65  获取锁:Thread-65:5c3891aa-51ee-44d4-bf00-cb5d6cc95b25
Thread-65  执行任务
Thread-61  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-65  完成任务，释放锁
Thread-65 完成推出
Thread-70  获取锁:Thread-70:7fdff6ca-c792-4a1a-b50b-7e30237d7118
Thread-70  执行任务
Thread-70  完成任务，释放锁
Thread-70 完成推出
Thread-73  获取锁:Thread-73:e909c294-3d50-451b-a77a-44066d30d5bc
Thread-73  执行任务
Thread-66  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-73  完成任务，释放锁
Thread-73 完成推出
Thread-75  获取锁:Thread-75:b18fd2be-ac1f-4585-950d-eee7c1895846
Thread-75  执行任务
Thread-76  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-75  完成任务，释放锁
Thread-75 完成推出
Thread-80  获取锁:Thread-80:e4ca2fa6-f6f4-4940-8808-47ac057deeab
Thread-80  执行任务
Thread-58  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-80  完成任务，释放锁
Thread-80 完成推出
Thread-74  获取锁:Thread-74:662cca2f-fdb3-4cb3-b80a-3154c180bf2c
Thread-74  执行任务
Thread-74  完成任务，释放锁
Thread-74 完成推出
Thread-84  获取锁:Thread-84:ebcba311-2db3-440b-8103-f1727d73711e
Thread-84  执行任务
Thread-83  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-92  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-91  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-84  完成任务，释放锁
Thread-84 完成推出
Thread-77  获取锁:Thread-77:cdd853b1-4490-4b73-b205-022531d59c53
Thread-77  执行任务
Thread-87  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-77  完成任务，释放锁
Thread-77 完成推出
Thread-78  获取锁:Thread-78:c3420af8-bf0b-446a-9a07-ec709f36ebc0
Thread-78  执行任务
Thread-88  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-78  完成任务，释放锁
Thread-78 完成推出
Thread-57  获取锁:Thread-57:25bb0e44-256a-4025-8970-39eb0bf39d6e
Thread-57  执行任务
Thread-57  完成任务，释放锁
Thread-57 完成推出
Thread-2  获取锁:Thread-2:a95241d4-13e5-4fb2-a3b9-461fa6c55e2b
Thread-2  执行任务
Thread-93  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-2  完成任务，释放锁
Thread-2 完成推出
Thread-81  获取锁:Thread-81:1a0de521-c2a2-4485-96c9-f6367e59068f
Thread-81  执行任务
Thread-81  完成任务，释放锁
Thread-81 完成推出
Thread-85  获取锁:Thread-85:6151b661-d193-48c9-8268-4ad55116d51d
Thread-85  执行任务
Thread-85  完成任务，释放锁
Thread-94  获取锁:Thread-94:e610b14a-6774-444f-b897-4081bd83b500
Thread-94  执行任务
Thread-85 完成推出
Thread-89  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-90  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-82  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-86  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-94  完成任务，释放锁
Thread-54  获取锁:Thread-54:f7d1151a-0aa7-4888-9d4a-31c40e162164
Thread-54  执行任务
Thread-94 完成推出
Thread-54  完成任务，释放锁
Thread-54 完成推出
Thread-53  获取锁:Thread-53:afceb431-6306-423f-80d5-3f05cdce29e6
Thread-53  执行任务
Thread-53  完成任务，释放锁
Thread-50  获取锁:Thread-50:8079a307-7a85-4bce-a304-4b669b16eb87
Thread-50  执行任务
Thread-50  完成任务，释放锁
Thread-50 完成推出
Thread-53 完成推出
Thread-55  获取锁:Thread-55:d2adb3e6-d080-4a58-804d-7cfe6846e920
Thread-55  执行任务
Thread-55  完成任务，释放锁
Thread-55 完成推出
Thread-56  获取锁:Thread-56:72f2a419-0ce5-40b1-bb54-87909783abc8
Thread-56  执行任务
Thread-56  完成任务，释放锁
Thread-56 完成推出
Thread-47  获取锁:Thread-47:b22b2bac-8857-4811-ac68-c0280d28796e
Thread-47  执行任务
Thread-47  完成任务，释放锁
Thread-47 完成推出
Thread-42  获取锁:Thread-42:a55cc000-0b8a-41dd-ad3e-99710158ea65
Thread-42  执行任务
Thread-46  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-41  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-37  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-42  完成任务，释放锁
Thread-42 完成推出
Thread-34  获取锁:Thread-34:00470d00-a9fc-4267-911a-629fdd644f10
Thread-34  执行任务
Thread-33  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-38  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-30  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-34  完成任务，释放锁
Thread-34 完成推出
Thread-29  获取锁:Thread-29:c9703eb6-5b31-4161-b243-64ad6d32ffda
Thread-29  执行任务
Thread-26  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-29  完成任务，释放锁
Thread-29 完成推出
Thread-23  获取锁:Thread-23:8acf6150-f1a5-4a1f-9bab-b8b589505a68
Thread-23  执行任务
Thread-23  完成任务，释放锁
Thread-23 完成推出
Thread-22  获取锁:Thread-22:6a37a009-bd44-4fb9-8252-65b4b0518b32
Thread-22  执行任务
Thread-22  完成任务，释放锁
Thread-22 完成推出
Thread-19  获取锁:Thread-19:ddcdc9ab-4e65-44ce-a0a8-b6574c628e7f
Thread-19  执行任务
Thread-18  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-19  完成任务，释放锁
Thread-19 完成推出
Thread-3  获取锁:Thread-3:6f0fd9a7-0027-4813-9eb9-459363c9feab
Thread-3  执行任务
Thread-11  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-3  完成任务，释放锁
Thread-3 完成推出
Thread-15  获取锁:Thread-15:c5778953-b19a-4079-a431-5cd681111a40
Thread-15  执行任务
Thread-39  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-14  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-15  完成任务，释放锁
Thread-10  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-7  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-15 完成推出
Thread-35  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-6  获取锁:Thread-6:0a81dadb-d7aa-4b7e-b389-09b8ca1b9a8e
Thread-6  执行任务
Thread-36  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-6  完成任务，释放锁
Thread-6 完成推出
Thread-31  获取锁:Thread-31:9c61f68b-e87c-4ea2-80b0-f8b6306691b4
Thread-31  执行任务
Thread-32  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-28  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-31  完成任务，释放锁
Thread-25  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-27  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-31 完成推出
Thread-71  获取锁:Thread-71:25934a78-73d2-4376-8749-059a0a1fe84e
Thread-71  执行任务
Thread-72  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-69  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-71  完成任务，释放锁
Thread-71 完成推出
Thread-67  获取锁:Thread-67:bf99b1a8-d867-4aba-96cd-eef02fde201f
Thread-67  执行任务
Thread-68  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-67  完成任务，释放锁
Thread-67 完成推出
Thread-62  获取锁:Thread-62:360af97e-4d35-4875-b496-a881742f7d91
Thread-62  执行任务
Thread-62  完成任务，释放锁
Thread-62 完成推出
Thread-95  获取锁:Thread-95:5b6bdd0d-0242-4fae-87de-cd3ff26c10f2
Thread-95  执行任务
Thread-95  完成任务，释放锁
Thread-95 完成推出
Thread-8  获取锁:Thread-8:3c6f02a4-2044-4934-874d-de1ae227f741
Thread-8  执行任务
Thread-8  完成任务，释放锁
Thread-8 完成推出
Thread-21  获取锁:Thread-21:858dc555-19f9-423d-9384-3bb626ec2faf
Thread-21  执行任务
Thread-21  完成任务，释放锁
Thread-21 完成推出
Thread-51  获取锁:Thread-51:3e56e439-e529-4c3b-989e-ba2de1bdac7c
Thread-51  执行任务
Thread-44  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-0  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-52  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-17  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-51  完成任务，释放锁
Thread-51 完成推出
Thread-45  获取锁:Thread-45:f0c66416-a218-492b-93e0-376ea2cafe75
Thread-45  执行任务
Thread-45  完成任务，释放锁
Thread-4  获取锁:Thread-4:580b23ff-193c-45e4-ac38-3e69ed4d43a1
Thread-4  执行任务
Thread-45 完成推出
Thread-59  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-4  完成任务，释放锁
Thread-63  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-96  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-4 完成推出
Thread-61  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-66  获取锁:Thread-66:4b665b15-d869-4cb2-b9bd-7dc6cfd041cf
Thread-66  执行任务
Thread-76  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-66  完成任务，释放锁
Thread-66 完成推出
Thread-58  获取锁:Thread-58:1ed59868-5df6-4ace-949a-d7e2b6251b7a
Thread-58  执行任务
Thread-58  完成任务，释放锁
Thread-58 完成推出
Thread-83  获取锁:Thread-83:5f0ed941-a213-4e8b-9f48-1e9c11a224f2
Thread-83  执行任务
Thread-91  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-92  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-83  完成任务，释放锁
Thread-83 完成推出
Thread-88  获取锁:Thread-88:85981817-d90a-46f6-bb08-fee48e60325e
Thread-88  执行任务
Thread-93  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-88  完成任务，释放锁
Thread-88 完成推出
Thread-89  获取锁:Thread-89:8c09c432-f6cf-4224-9af8-6da23fe41569
Thread-89  执行任务
Thread-87  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-89  完成任务，释放锁
Thread-89 完成推出
Thread-82  获取锁:Thread-82:45f1efe0-a664-439c-ba87-291ce7869414
Thread-90  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-82  执行任务
Thread-86  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-82  完成任务，释放锁
Thread-82 完成推出
Thread-37  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-46  获取锁:Thread-46:e0189b23-fc71-4218-9727-d34a00b5bfa2
Thread-46  执行任务
Thread-41  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-46  完成任务，释放锁
Thread-33  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-46 完成推出
Thread-38  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-26  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-30  获取锁:Thread-30:d34fd4ed-c251-42f8-a322-cb925c523368
Thread-30  执行任务
Thread-30  完成任务，释放锁
Thread-30 完成推出
Thread-18  获取锁:Thread-18:c51f402e-057f-4617-a177-36f7f38ffaaa
Thread-18  执行任务
Thread-18  完成任务，释放锁
Thread-18 完成推出
Thread-11  获取锁:Thread-11:73e867a1-e30e-4e73-85ae-e6e69ccca673
Thread-11  执行任务
Thread-11  完成任务，释放锁
Thread-11 完成推出
Thread-39  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-14  获取锁:Thread-14:10fadfea-e7b2-44de-93cb-1abffec78afc
Thread-14  执行任务
Thread-14  完成任务，释放锁
Thread-35  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-14 完成推出
Thread-10  获取锁:Thread-10:e04f76dc-3abb-461f-97e7-1db5e3e6019e
Thread-10  执行任务
Thread-32  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-7  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-25  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-27  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-36  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-28  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-72  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-10  完成任务，释放锁
Thread-69  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-68  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-10 完成推出
Thread-17  获取锁:Thread-17:e31fd0f7-290f-4d4e-95e8-3c490bc9adeb
Thread-17  执行任务
Thread-44  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-52  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-17  完成任务，释放锁
Thread-0  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-17 完成推出
Thread-59  获取锁:Thread-59:2a64c175-c459-48ce-8e89-75b7fd50fcc8
Thread-59  执行任务
Thread-96  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-59  完成任务，释放锁
Thread-61  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-63  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-76  获取锁:Thread-76:f752bbd0-68dc-4503-8adb-a8219823fde2
Thread-76  执行任务
Thread-59 完成推出
Thread-76  完成任务，释放锁
Thread-76 完成推出
Thread-91  获取锁:Thread-91:47f7f7fa-2ba4-4211-b34d-818c871f30cd
Thread-91  执行任务
Thread-92  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-91  完成任务，释放锁
Thread-91 完成推出
Thread-93  获取锁:Thread-93:2418d59d-2152-4699-ba7d-d80a236ea396
Thread-93  执行任务
Thread-93  完成任务，释放锁
Thread-93 完成推出
Thread-87  获取锁:Thread-87:e52f78af-b61c-4bfb-89b5-1444fe232799
Thread-87  执行任务
Thread-87  完成任务，释放锁
Thread-87 完成推出
Thread-90  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-86  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-2
Thread-37  获取锁:Thread-37:91f7efed-0270-4f74-a4dc-929279518ad0
Thread-37  执行任务
Thread-37  完成任务，释放锁
Thread-26  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-41  获取锁:Thread-41:284d7c37-4a2d-475d-be02-78e95b2e6242
Thread-41  执行任务
Thread-38  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-1
Thread-33  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：-1
Thread-37 完成推出
Thread-41  完成任务，释放锁
Thread-41 完成推出
Thread-39  获取锁:Thread-39:d68efb03-f0b4-4e56-9a47-3863954c726d
Thread-39  执行任务
Thread-39  完成任务，释放锁
Thread-39 完成推出
Thread-35  获取锁:Thread-35:2be7800d-62bb-4090-8fa7-a8bde5817e7f
Thread-35  执行任务
Thread-35  完成任务，释放锁
Thread-35 完成推出
Thread-7  获取锁:Thread-7:053755f4-6af9-4d59-838f-344d2345667e
Thread-7  执行任务
Thread-7  完成任务，释放锁
Thread-7 完成推出
Thread-69  获取锁:Thread-69:1884f542-e2fd-40f3-85f7-ab099f9c81e4
Thread-69  执行任务
Thread-69  完成任务，释放锁
Thread-69 完成推出
Thread-28  获取锁:Thread-28:d0d446ee-0cc8-46b5-8266-0af09ad51135
Thread-28  执行任务
Thread-28  完成任务，释放锁
Thread-28 完成推出
Thread-27  获取锁:Thread-27:7f50d3dd-642b-4904-80e1-86942db672da
Thread-27  执行任务
Thread-27  完成任务，释放锁
Thread-27 完成推出
Thread-25  获取锁:Thread-25:aa0faa33-24c6-4e0a-b564-308fde0af9a4
Thread-25  执行任务
Thread-25  完成任务，释放锁
Thread-32  获取锁:Thread-32:1ba54955-4b48-442d-850e-806f894e64b0
Thread-32  执行任务
Thread-36  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-68  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-72  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-25 完成推出
Thread-32  完成任务，释放锁
Thread-32 完成推出
Thread-52  获取锁:Thread-52:6bc5e341-83c9-4320-8dcf-87f3f7c297f3
Thread-52  执行任务
Thread-44  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-52  完成任务，释放锁
Thread-52 完成推出
Thread-0  获取锁:Thread-0:f8562368-1559-4204-87a1-c3b1221d574b
Thread-0  执行任务
Thread-0  完成任务，释放锁
Thread-0 完成推出
Thread-96  获取锁:Thread-96:cfacee5f-3b8e-4ae9-9442-c88cf28583b5
Thread-96  执行任务
Thread-63  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-61  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-96  完成任务，释放锁
Thread-96 完成推出
Thread-92  获取锁:Thread-92:756c7374-894e-4e90-a003-cbd2629ee8bc
Thread-92  执行任务
Thread-92  完成任务，释放锁
Thread-92 完成推出
Thread-86  获取锁:Thread-86:ffcda81d-958d-468c-848e-69bcb0752c03
Thread-86  执行任务
Thread-90  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-86  完成任务，释放锁
Thread-86 完成推出
Thread-38  获取锁:Thread-38:8e91d124-d73a-44ca-8e2f-23803c5a451f
Thread-38  执行任务
Thread-26  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-33  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-38  完成任务，释放锁
Thread-38 完成推出
Thread-36  获取锁:Thread-36:98374a8d-1dda-4763-9420-9b477725ba37
Thread-36  执行任务
Thread-36  完成任务，释放锁
Thread-36 完成推出
Thread-72  获取锁:Thread-72:439e8e70-a1d3-4017-8841-3da6de42d24b
Thread-72  执行任务
Thread-72  完成任务，释放锁
Thread-72 完成推出
Thread-68  获取锁:Thread-68:c070d2ba-ab20-43b6-ad68-01614702ba3c
Thread-68  执行任务
Thread-68  完成任务，释放锁
Thread-68 完成推出
Thread-44  获取锁:Thread-44:0b9dbf04-7506-4b3d-91a4-168c2381f062
Thread-44  执行任务
Thread-44  完成任务，释放锁
Thread-44 完成推出
Thread-61  获取锁:Thread-61:66815858-6673-405f-acc8-ee78aad0e3ab
Thread-61  执行任务
Thread-63  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-61  完成任务，释放锁
Thread-61 完成推出
Thread-90  获取锁:Thread-90:17c4cdff-2127-4def-b97d-c78819e0d3de
Thread-90  执行任务
Thread-90  完成任务，释放锁
Thread-90 完成推出
Thread-26  获取锁:Thread-26:260c89a7-4cfd-4cf5-96ef-13f2e851c3fd
Thread-26  执行任务
Thread-33  lock_withTimeout获取锁竞争失败，休息1秒，继续尝试获取,锁ttl剩余：100
Thread-26  完成任务，释放锁
Thread-26 完成推出
Thread-63  获取锁:Thread-63:65832aa2-2e31-439b-a76b-590914d34d6c
Thread-63  执行任务
Thread-63  完成任务，释放锁
Thread-63 完成推出
Thread-33  获取锁:Thread-33:26b2d195-b9c3-41cb-be38-0081fcf5ae5a
Thread-33  执行任务
Thread-33  完成任务，释放锁
Thread-33 完成推出
![复制代码](http://common.cnblogs.com/images/copycode.gif)
