# redis分布式锁处理并发问题 - z69183787的专栏 - CSDN博客
2018年08月05日 12:30:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：210
[https://blog.csdn.net/d1562901685/article/details/54881862](https://blog.csdn.net/d1562901685/article/details/54881862)
# redis锁处理并发问题
redis锁处理高并发问题十分常见，使用的时候常见有几种错误，和对应的解决办法，在此进行自己的总结和整理。
- **set方式**
- **setnx方式**
- **setnx+getset方式**
**set方式**
作为redis小白，一开始能想到的使用redis加锁的方式就是set。 
加锁：redis中set一个值，set（lock，1）; 
并发处理：其他线程必须拿到这个值，才可以往下进行，否则等待。
```
while(jedis.exists(lock)){
        Thread.sleep(500);
    }
    set(lock，1);
    执行业务代码;
    jedis.del(lock);
```
释放锁：执行完业务代码之后，释放redis锁，jedis.del(lock) 
防止死锁：set(lock,1) —>3秒后未释放，则自动释放setex(lock, 3, 1) 
**问题：**高并发情况下，进程同时获取锁状态为null，同时设置，锁之间相互覆盖，但是俩进程仍在并发执行业务代码。 
**setnx方式**
后来发现有setnx的原子操作命令，锁存在不能设置值，返回0；锁不存在，则设置锁，返回1； 
加锁：jedis.setnx(lock, 1) 
并发处理：
```
while(jedis.setnx(lock,1)==0){
    Thread.sleep(300);
}
执行业务代码;
jedis.del(lock);
```
释放锁：执行完业务代码之后，释放redis锁，jedis.del(lock) 
**问题**：当进程执行出现问题，锁未释放，则其他进程永远处于阻塞状态，出现死锁。 
防止死锁：加锁时带上时间戳，setnx(lock, 时间戳+超时时间)
```
while(jedis.setnx(lock,now+超时时间)==0){
        if(jedis.get(lock)<now){
            jedis.del(lock);
            jedis.setnx(lock,now+超时时间);
            break;
        }else{
            Thread.sleep(300);
        }
    }
    执行业务代码;
    jedis.del(lock);
```
**问题**：当俩进程同时读到发现锁超时，都去释放锁，相互覆盖，则俩进程同时获得锁，仍并发执行业务代码。 
**setnx+getset方式**
为解决上面的问题，可以使用getset命令，getset设置键值，并返回原来的键值。 
加锁：setnx(lock, 时间戳+超时时间) 
解决并发：
```
while(jedis.setnx(lock, now+超时时间)==0）{
    if(now>jedis.get(lock) && now>jedis.getset(lock, now+超时时间)){
        break;
    }else{
        Thread.sleep(300);
    }
}
执行业务代码;
jedis.del(lock);
```
