# Go语言实现的API-Gateway - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月13日 14:09:00[boonya](https://me.csdn.net/boonya)阅读数：9783










原文地址：[https://www.oschina.net/p/api-gateway](https://www.oschina.net/p/api-gateway)


Gateway是一个使用go实现的基于HTTP的API 网关。

**特性：**
- 
API 聚合

- 
流控

- 
熔断

- 
负载均衡

- 
健康检查

- 
监控

- 
消息路由

- 
后端管理WebUI


**能做什么：**
- 
规划更友好的URL给调用者。

- 
聚合多个API的结果返回给API调用者，利于移动端，后端可以实现原子接口。

- 
保护后端API服务不会被突发异常流量压垮。

- 
提供熔断机制，使得后端API Server具备自我恢复能力。

- 
借助消息路由能力，实现灰度发布，AB测试。



Github地址：[https://github.com/fagongzi/gateway](https://github.com/fagongzi/gateway)










