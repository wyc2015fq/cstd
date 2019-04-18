# 基于Redis的限流系统的设计（LUA） - z69183787的专栏 - CSDN博客
2018年08月08日 21:11:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：671
[https://www.cnblogs.com/AndyAo/p/8144049.html](https://www.cnblogs.com/AndyAo/p/8144049.html)
本文讲述基于Redis的限流系统的设计，主要会谈及限流系统中限流策略这个功能的设计；在实现方面，算法使用的是令牌桶算法来，访问Redis使用lua脚本。
## 1、概念
> 
In computer networks, rate limiting is used to control the rate of traffic sent or received by a network interface controller and is used to prevent DoS attacks
用我的理解翻译一下：限流是对系统的出入流量进行控制，防止大流量出入，导致资源不足，系统不稳定。
限流系统是对资源访问的控制组件，控制主要的两个功能：限流策略和熔断策略，对于熔断策略，不同的系统有不同的熔断策略诉求，有的系统希望直接拒绝、有的系统希望排队等待、有的系统希望服务降级、有的系统会定制自己的熔断策略，很难一一列举，所以本文只针对限流策略这个功能做详细的设计。
针对找出超出速率阈值的请求这个功能，限流系统中有两个基础概念：资源和策略。
- 资源 ：或者叫稀缺资源，被流量控制的对象；比如写接口、外部商户接口、大流量下的读接口
- 策略 ：限流策略由限流算法和可调节的参数两部分组成
> 
熔断策略：超出速率阈值的请求的处理策略，是我自己理解的一个叫法，不是业界主流的说法。
## 2、限流算法
- 限制瞬时并发数
- 限制时间窗最大请求数
- 令牌桶
### 2.1、限制瞬时并发数
定义：瞬时并发数，系统同时处理的请求/事务数量
优点：这个算法能够实现控制并发数的效果
缺点：使用场景比较单一，一般用来对入流量进行控制
java伪代码实现：
```java
AtomicInteger atomic = new AtomicInteger(1)
try {
if(atomic.incrementAndGet() > 限流数) {
//熔断逻辑
} else {
//处理逻辑
}
} finally {
atomic.decrementAndGet();
}
```
### 2.2、限制时间窗最大请求数
定义：时间窗最大请求数，指定的时间范围内允许的最大请求数
优点：这个算法能够满足绝大多数的流控需求，通过时间窗最大请求数可以直接换算出最大的QPS（QPS = 请求数/时间窗）
缺点：这种方式可能会出现流量不平滑的情况，时间窗内一小段流量占比特别大
lua代码实现：
```
--- 资源唯一标识
local key = KEYS[1]
--- 时间窗最大并发数
local max_window_concurrency = tonumber(ARGV[1])
--- 时间窗
local window = tonumber(ARGV[2])
--- 时间窗内当前并发数
local curr_window_concurrency = tonumber(redis.call('get', key) or 0)
if current + 1 > limit then
return false
else
redis.call("INCRBY", key,1)
if window > -1 then
redis.call("expire", key,window)
end
return true
end
```
### 2.3、令牌桶
![令牌桶](https://ws3.sinaimg.cn/large/006tNc79gy1flk2kudl1oj318w0ymwgp.jpg)
算法描述
- 假如用户配置的平均发送速率为r，则每隔1/r秒一个令牌被加入到桶中
- 假设桶中最多可以存放b个令牌。如果令牌到达时令牌桶已经满了，那么这个令牌会被丢弃
- 当流量以速率v进入，从桶中以速率v取令牌，拿到令牌的流量通过，拿不到令牌流量不通过，执行熔断逻辑
属性
- 长期来看，符合流量的速率是受到令牌添加速率的影响，被稳定为：r
- 因为令牌桶有一定的存储量，可以抵挡一定的流量突发情况 
	- M是以字节/秒为单位的最大可能传输速率。 M>r
- T max = b/(M-r) 承受最大传输速率的时间
- B max = T max * M 承受最大传输速率的时间内传输的流量
优点：流量比较平滑，并且可以抵挡一定的流量突发情况
因为我们限流系统的实现就是基于令牌桶这个算法，具体的代码实现参考下文。
## 3、工程实现
### 3.1、技术选型
- mysql:存储限流策略的参数等元数据
- redis+lua:令牌桶算法实现
> 
说明：因为我们把redis 定位为：缓存、计算媒介，所以元数据都是存在db中
### 3.2、架构图
![限流系统](https://ws2.sinaimg.cn/large/006tNc79gy1flk2ktfbtzj30iy0ocab8.jpg)
### 3.3、 数据结构
|字段|描述|
|----|----|
|name|令牌桶的唯一标示|
|apps|能够使用令牌桶的应用列表|
|max_permits|令牌桶的最大令牌数|
|rate|向令牌桶中添加令牌的速率|
|created_by|创建人|
|updated_by|更新人|
限流系统的实现是基于redis的，本可以和应用无关，但是为了做限流元数据配置的统一管理，by应用维度管理和使用，在数据结构中加入了apps这个字段，出现问题，排查起来也比较方便。
### 3.4、代码实现
3.4.1、代码实现遇到的问题
参考令牌桶的算法描述，一般思路是在RateLimiter-client放一个重复执行的线程，线程根据配置往令牌桶里添加令牌，这样的实现由如下缺点：
- 需要为每个令牌桶配置添加一个重复执行的线程
- 重复的间隔精度不够精确：线程需要每1/r秒向桶里添加一个令牌，当r >1000 时间线程执行的时间间隔根本没办法设置（从后面性能测试的变现来看RateLimiter-client 是可以承担 QPS > 5000 的请求速率）
3.4.2、解决方案
基于上面的缺点，参考了google的guava中RateLimiter中的实现，我们使用了触发式添加令牌的方式。
![令牌桶实现](https://ws4.sinaimg.cn/large/006tNc79gy1flk2ks5gafj30u50kggmj.jpg)
算法描述
- 基于上述的令牌桶算法
- 将添加令牌改成触发式的方式，取令牌的是做添加令牌的动作
- 在去令牌的时候，通过计算上一次添加令牌和当前的时间差，计算出这段时间应该添加的令牌数，然后往桶里添加 
	- curr_mill_second = 当前毫秒数
- last_mill_second = 上一次添加令牌的毫秒数
- r = 添加令牌的速率
- reserve_permits = (curr_mill_second-last_mill_second)/1000 * r
- 添加完令牌之后再执行取令牌逻辑
3.4.3、 lua代码实现
```
--- 获取令牌
--- 返回码
--- 0 没有令牌桶配置
--- -1 表示取令牌失败，也就是桶里没有令牌
--- 1 表示取令牌成功
--- @param key 令牌（资源）的唯一标识
--- @param permits 请求令牌数量
--- @param curr_mill_second 当前毫秒数
--- @param context 使用令牌的应用标识
local function acquire(key, permits, curr_mill_second, context)
local rate_limit_info = redis.pcall("HMGET", key, "last_mill_second", "curr_permits", "max_permits", "rate", "apps")
local last_mill_second = rate_limit_info[1]
local curr_permits = tonumber(rate_limit_info[2])
local max_permits = tonumber(rate_limit_info[3])
local rate = rate_limit_info[4]
local apps = rate_limit_info[5]
--- 标识没有配置令牌桶
if type(apps) == 'boolean' or apps == nil or not contains(apps, context) then
return 0
end
local local_curr_permits = max_permits;
--- 令牌桶刚刚创建，上一次获取令牌的毫秒数为空
--- 根据和上一次向桶里添加令牌的时间和当前时间差，触发式往桶里添加令牌
--- 并且更新上一次向桶里添加令牌的时间
--- 如果向桶里添加的令牌数不足一个，则不更新上一次向桶里添加令牌的时间
if (type(last_mill_second) ~= 'boolean' and last_mill_second ~= false and last_mill_second ~= nil) then
local reverse_permits = math.floor(((curr_mill_second - last_mill_second) / 1000) * rate)
local expect_curr_permits = reverse_permits + curr_permits;
local_curr_permits = math.min(expect_curr_permits, max_permits);
--- 大于0表示不是第一次获取令牌，也没有向桶里添加令牌
if (reverse_permits > 0) then
redis.pcall("HSET", key, "last_mill_second", curr_mill_second)
end
else
redis.pcall("HSET", key, "last_mill_second", curr_mill_second)
end
local result = -1
if (local_curr_permits - permits >= 0) then
result = 1
redis.pcall("HSET", key, "curr_permits", local_curr_permits - permits)
else
redis.pcall("HSET", key, "curr_permits", local_curr_permits)
end
return result
end
```
关于限流系统的所有实现细节，我都已经放到github上，gitbub地址：[https://github.com/wukq/rate-limiter](https://github.com/wukq/rate-limiter)，有兴趣的同学可以前往查看，由于笔者经验与知识有限，代码中如有错误或偏颇，欢迎探讨和指正。
### 3.4.4、管理界面
前面的设计中，限流的配置是和应用关联的，为了更够更好的管理配置，需要一个统一的管理页面去对配置进行管控：
- 按应用对限流配置进行管理
- 不同的人分配不同的权限；相关人员有查看配置的权限，负责人有修改和删除配置的权限
![限流管理平台](https://ws4.sinaimg.cn/large/006tNc79gy1flhkk8zmwyj31kw0fvjtj.jpg)
### 3.5、性能测试
配置：aws-elasticcache-redis 2核4g
因为Ratelimiter-client的功能比较简单，基本上是redis的性能打个折扣。
- 单线程取令牌：Ratelimiter-client的 QPS = 250/s
- 10个线程取令牌：Ratelimiter-client的 QPS = 2000/s
- 100个线程取令牌：Ratelimiter-client的 QPS = 5000/s
## 4、总结
限流系统从设计到实现都比较简单，但是确实很实用，用四个字来形容就是：短小强悍，其中比较重要的是结合公司的权限体系和系统结构，设计出符合自己公司规范的限流系统。
不足：
- redis 我们用的是单点redis，只做了主从，没有使用redis高可用集群（可能使用redis高可用集群，会带来新的问题）
- 限流系统目前只做了应用层面的实现，没有做[接口网关](https://mp.weixin.qq.com/s/57sno2kfSrVWVMGFXyASjQ)上的实现
- 熔断策略需要自己定制，如果实现的好一点，可以给一些常用的熔断策略模板
> 
最后鸣谢一下贡献自己想法的“前人”：guava Ratelimiter、聊聊高并发系统之限流特技--开涛、API 调用次数限制实现--yigwoo
参考书籍：
1.《Redis 设计与实现》 
2.《Lua编程指南》
参考文章：
- [https://en.wikipedia.org/wiki/Token_bucket](https://en.wikipedia.org/wiki/Token_bucket)
- [redis官网](https://redis.io/)
- [lua编码规范](http://andrewliu.in/2016/09/04/Lua%E7%BC%96%E7%A0%81%E8%A7%84%E8%8C%83/)
- [聊聊高并发系统之限流特技](http://jinnianshilongnian.iteye.com/blog/2305117)
- [API 调用次数限制实现](https://zhuanlan.zhihu.com/p/20872901)
- [guava Ratelimiter 实现](https://github.com/google/guava/blob/master/guava/src/com/google/common/util/concurrent/RateLimiter.java)
原文转载：http://blog.csdn.net/u013870094/article/details/78620300
