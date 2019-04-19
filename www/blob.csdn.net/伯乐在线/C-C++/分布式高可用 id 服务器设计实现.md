# 分布式高可用 id 服务器设计实现 - 文章 - 伯乐在线
原文出处： [lxyfirst](http://www.cppblog.com/lxyfirst/archive/2015/09/17/211854.html)
服务端/后台开发中如何生成id是每个开发者都会遇到的问题，在电商、游戏领域尤其突出。如何保证生成id的唯一性、可靠性、高可用性，如何组织id的格式，在不同的应用场景和限制下实现方式也不尽相同。
我们的应用场景类似电商，在一个订单的生命周期内，有多个逻辑需要生成各自的id，还要考虑到可读性和灵活性，我们决定实现一个独立的id服务。
首先，id服务必须具有高可用性，业务逻辑处理中创建id失败是不可接受的，所以id服务必须分布式部署，有多个节点同时对外服务，一个节点失败则重试其他节点，保证成功创建id。
在分布式系统中保证数据的一致性成本是很高的，为了简化设计和实现，每个节点都设计成对等的、独立的，不需要保持数据同步。
其次，id服务必须可靠，数据不能丢失，因此数据的存储放在独立的mysql数据库中，使用replace方式更新数据，id服务本身记录更新日志。
最后，id服务必须灵活，可以自定义id格式，可以高效灵活的实现客户端，因此通讯协议使用json over udp方式，在id服务端使用lua实现id格式的灵活定义。
### ID 规则
具体规则有lua脚本定义，修改脚本后需要reload生效，需要实现4个函数
- min_counter : 计数器最小值
- max_counter : 计数器最大值
- reset_seconds : 计数器重置周期
- create_id : 根据计数器、自定义参数和时间参数创建ID。
例如：


```
function min_counter()
        return 0
    end
    function max_counter()
        return 9999
    end
    function reset_seconds()
        return 86400
    end
    function create_id(counter,now,salt)
        local seq = counter:generate_counter()
        local new_id = string.format("%01d%02d%02d%04d",now:year()%10 ,now:month(),now:day(),seq)
        return new_id
    end
```
### 接口
采用udp协议，数据格式为json ，字段定义：
- action: 请求类型 get： 创建ID ， monitor：监控
- rule_name: 规则名字， 由服务端定义
- app_name : 应用名或命名空间 ， 客户端自定义，rule_name和app_name一起决定生成ID的唯一性
- salt : 自定义参数 ，可选项 ，
- seq : 自定义参数，可选项，原样返回
例如:
- 创建ID请求: {“action”:”get”,”rule_name”:”o2o”,”app_name”:”test”}
- 响应：{“code”:0,”message”:”success”,”data”:”505140001″}
- 监控请求：{“action”:”monitor”,”rule_name”:”o2o”,”app_name”:”test”}
- 响应：{“code”:0,”message”:”ok”,”data”:{“counter”:3,”node_offset”:1}}
### 性能
id服务器使用c++实现，性能测试做的比较简单，因为性能不是id服务的主要关注点， 简单以php为客户端进行测试。
4个php并发进程，每个进程不停发送20万个请求，测试结果：


```
total:200000 fail:0 min:0.000214 max:0.087330 avg:0.000393
 total:200000 fail:0 min:0.000215 max:0.087129 avg:0.000391
 total:200000 fail:0 min:0.000221 max:0.087252 avg:0.000391
 total:200000 fail:0 min:0.000218 max:0.087484 avg:0.000391
 说明 min : 最小耗时(秒) max : 最大耗时(秒) avg : 平均耗时(秒)
```
服务器TPS达到近1万/秒时，平均延迟在0.3毫秒。
经过在生产环境使用，运行稳定，现在将整个系统开源出来，欢迎试用，有任何意见和建议欢迎反馈到 lxyfirst＠163.com 。
项目源代码位置 : [https://github.com/lxyfirst/id_server](https://github.com/lxyfirst/id_server)
版本更新9.19
1.增加数据落地的预保存和批量保存机制，一方面减少数据库压力，一方面增加异步保存的可靠性。
2.由于主线程和数据库线程只需要传递sql语句，将线程间通信由pipe方式改为eventfd + lockfree queue方式。
