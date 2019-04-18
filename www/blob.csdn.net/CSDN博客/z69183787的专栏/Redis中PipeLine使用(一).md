# Redis中PipeLine使用(一) - z69183787的专栏 - CSDN博客
2016年08月30日 18:13:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3724
(一)简介
Redis客户端与Redis之间使用TCP协议进行连接，一个客户端可以通过一个socket连接发起多个请求命令。每个请求命令发出后client通常会阻塞并等待redis服务处理，redis处理完后请求命令后会将结果通过响应报文返回给client,因此当执行多条命令的时候都需要等待上一条命令执行完毕才能执行，如：get ‘0’,get ‘1’,get ‘2’
![这里写图片描述](https://img-blog.csdn.net/20151012195302323)
其执行过程如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151012195816208)
而管道（pipeline）可以一次性发送多条命令并在执行完后一次性将结果返回，pipeline通过减少客户端与redis的通信次数来实现降低往返延时时间，其过程如下图所示 
![这里写图片描述](https://img-blog.csdn.net/20151012200300532)
(二)普通方式与pipeline的比较
1.测试环境： 
redis版本2.4.15 
jedis版本2.2.1
2.测试代码 
分别向redis中插入100、500、1000、2000、5000、10000数据
```java
long start=System.currentTimeMillis();
        for (int i = 0; i <50000; i++) {
            redis.set(String.valueOf(i),String.valueOf(i));
        }
        long end=System.currentTimeMillis();
        logger.info("the total time is:"+(end-start));
        Pipeline pipe=redis.pipelined();
        long start_pipe=System.currentTimeMillis();
        for (int i = 0; i <50000; i++) {
            pipe.set(String.valueOf(i),String.valueOf(i));
        }
        pipe.sync();
        long end_pipe=System.currentTimeMillis();
        logger.info("the pipe total time is:"+(end_pipe-start_pipe));
```
测试结果对比： 
![这里写图片描述](https://img-blog.csdn.net/20151012211748600)
