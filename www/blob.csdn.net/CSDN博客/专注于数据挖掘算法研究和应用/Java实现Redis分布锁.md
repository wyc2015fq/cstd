# Java实现Redis分布锁 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月19日 14:46:28[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4760








1、背景：

在多线程环境下，通常会使用锁来保证有且只有一个线程来操作共享资源。比如：

object obj = new object();

lock (obj) 

{ 

//操作共享资源 

}

利用操作系统提供的锁机制，可以确保多线程或多进程下的并发唯一操作。但如果在多机环境下就不能满足了，当A，B两台机器同时操作C机器的共享资源时，就需要第三方的锁机制来保证在分布式环境下的资源协调，也称分布式锁。

在分布式环境下（多节点主机）加锁处理场景，如：秒杀、全局递增ID等等，需要对资源（变量）同步互斥。大部分的解决方案是基于DB实现的，Redis为单进程单线程模式，采用队列模式将并发访问变成串行访问，且多客户端对Redis的连接并不存在竞争关系。由于Redis是单线程模型，命令操作原子性，所以利用这个特性可以很容易的实现分布式锁。



2、Redis有三个最基本属性来保证分布式锁的有效实现：


安全性： 互斥，在任何时候，只有一个客户端能持有锁。

活跃性A：没有死锁，即使客户端在持有锁的时候崩溃，最后也会有其他客户端能获得锁，超时机制。

活跃性B：故障容忍，只有大多数Redis节点时存活的，客户端仍可以获得锁和释放锁。

使用Redis实现分布式锁，有两个重要函数需要介绍


1）SETNX命令（SET if Not eXists）

语法：

SETNX key value

功能：

当且仅当 key 不存在，将 key 的值设为 value ，并返回1；若给定的 key 已经存在，则 SETNX 不做任何动作，并返回0。



2）GETSET命令

语法：

GETSET key value

功能：

将给定 key 的值设为 value ，并返回 key 的旧值 (old value)，当 key 存在但不是字符串类型时，返回一个错误，当key不存在时，返回nil。





3、代码：



```java
package ct.tool;

import redis.clients.jedis.Jedis;

public class RedisDisLock {
	
	private static final long expired = 1000;//1秒超时
	//上锁
	public static boolean acquireLock(Jedis jedis,String lock) {
	    // 1. 通过SETNX试图获取一个lock
	    boolean success = false;
	    long value = System.currentTimeMillis() + expired + 1;      
	    long acquired = jedis.setnx(lock, String.valueOf(value));
	    jedis.expire(lock, 1);//设置1秒超时
	    //SETNX成功，则成功获取一个锁
	    if (acquired == 1)  success = true;
	    //SETNX失败，说明锁被其他客户端保持，检查其是否已经超时
	    /*else {
	        long oldValue = Long.valueOf(jedis.get(lock));	       
	        if (oldValue < System.currentTimeMillis()) {//超时
	        	//获取上一个锁到期时间，并设置现在的锁到期时间，
                //只有一个线程才能获取上一个线上的设置时间，因为jedis.getSet是同步的
	            String getValue = jedis.getSet(lock, String.valueOf(value));
	            if (getValue !=null) {
	            	if (Long.valueOf(getValue) == oldValue) 
		                success = true; 
		            else success = false;// 已被其他进程捷足先登了
	            }            
	        }else //未超时，则直接返回失败
	        	success = false;
	    }        */
	    return success;      
	}
	 
	//释放锁
	public static void releaseLock(Jedis jedis,String lock) {    
	    //long current = System.currentTimeMillis();       
	    // 避免删除非自己获取得到的锁
	    //if (current < Long.valueOf(jedis.get(lock)))
	        jedis.del(lock); 
	}

}
```



代码应用中，要共享的代码段前加：





```java
//枷锁
	    	boolean lockFlag=true;
	    	while(lockFlag){//循环等待拿锁
	    		if (RedisDisLock.acquireLock(jd,"o2o")) lockFlag=false;
	    	}
```


业务处理后，释放：





```java
RedisDisLock.releaseLock(jd, "o2o");
```










