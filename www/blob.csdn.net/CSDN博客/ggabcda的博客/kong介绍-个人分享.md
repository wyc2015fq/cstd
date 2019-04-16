# kong介绍-个人分享 - ggabcda的博客 - CSDN博客





2017年09月03日 13:25:42[ggabcda](https://me.csdn.net/ggabcda)阅读数：5448








# kong简介

## 背景

我们在提供api或微服务时,通常借助openresty nginx进行流量转发或者添加一些规则或功能，但是随着服务数量和引用增多,复杂的网络环境，

使维护变得困难，不容易扩展，一些功能也会在多个服务中重复,我们期望有一个工具来集中管理。

![401614-20170902230130280-722146981.png](http://images2017.cnblogs.com/blog/401614/201709/401614-20170902230130280-722146981.png)

## 简介

Kong 是在客户端和（微）服务间转发API通信的API网关和API服务管理层,通过插件扩展功能。我们可以

可以通过增加更多 Kong Server 机器对 Kong 服务进行水平扩展，通过前置的负载均衡器向这些机器分发请求。
- 官网:https://getkong.org/ 
- github:https://github.com/Mashape/kong 
- kong-dashboard(开源UI):https://www.npmjs.com/package/kong-dashboard 

## 特点
- Kong核心基于OpenResty构建，实现了请求/响应的Lua处理化 
- Kong通过Restful API提供了API/API消费者/插件/upstream/sni/证书的管理; 
- 数据中心用于存储Kong集群节点信息、API、消费者、插件等信息，目前提供了PostgreSQL和Cassandra支持，如果需要高可用建议使用Cassandra；

- 缓存机制(为了避免每次查询数据库，Kong 在第一次请求之行时，尝试在本地的内存里做尽可能多的数据缓存。)、kong集群使kong具备高性能高可用. 

## 结构
- admin: restful api<-->kong server<-(cache)-->db 
- client:port--->[kong server<--(cache)-->db]-->api 

## 安装
- kong:0.11 


```bash
$ sudo yum install epel-release
$ sudo yum install kong-community-edition-0.11.0.*.noarch.rpm --nogpgcheck
```

- PostgreSQL 9.4+ and Cassandra 3.x.x 


```bash
yum install postgresql96-server postgresql96-contrib
```


## 配置
- 配置文件 


```bash
/etc/kong/kong.conf   
/usr/local/kong/nginx.conf
/usr/local/kong/nginx-kong.conf
```

- 模板: 


```bash
kong_defaults.lua
nginx.lua
nginx_kong.lua
```


## 表结构

## kong常用命令
- kong 


```bash
kong --v
kong check /etc/kong/kong.conf
kong health -p /usr/local/kong
kong version
```

- 初始化数据库，插件更新 


```bash
kong migrations up -c /etc/kong/kong.conf
kong migrations list -c /etc/kong/kong.conf
kong migrations reset -c /etc/kong/kong.conf
```

- 配置 


```bash
kong start -c /etc/kong/kong.conf -p /usr/local/kong --nginx-conf custom_nginx.template --run-migrations true
kong start -c /etc/kong/kong.conf --nginx-conf custom_nginx.template
kong quit -p /usr/local/kong
kong stop -p /usr/local/kong
kong reload -c /etc/kong/kong.conf -p /usr/local/kong  --nginx-conf custom_nginx.template
```


## Kong Admin API
- 
method:

[get\patch\put\delete]

- 
管理项：[apis\upstreams\plugins\consumers\certificates\snis]

- 
格式



```bash
get:/apis/{name or id}
get:/apis/
patch:/apis/{name or id}
put:/apis/
delete:/apis/{name or id}
```

- config 


```bash
curl http://localhost:8001/ |jq .
curl http://localhost:8001/status |jq .
```

- /apis/ 


```bash
curl -H "Content-type: application/json" -X POST -d '{"name":"demo","hosts":"demo.com","upstream_url":"http://www.baidu.com"}' "http://localhost:8001/apis/" | jq .
修改hosts-->reload
curl -H "Content-type: application/json" -X POST -d '{"name":"demo","hosts":"demo.com","upstream_url":"http://structuretest.com","preserve_host":"false"}' "http://localhost:8001/apis/" | jq .

curl -H "Content-type: application/json" -X POST -d '{"name":"demo","hosts":"demo.com","upstream_url":"http://demo.upstream"}' "http://localhost:8001/apis/" | jq .
curl -H "Content-type: application/json" -X PATCH -d '{"strip_uri":"true","preserve_host":"true", "https_only":"false","http_if_terminated":"true"}' "http://localhost:8001/apis/demo" |jq .
curl -H "Content-type: application/json" -X POST -d '{"name":"demo","hosts":"demo.com","upstream_url":"http://demo.upstream"}' "http://localhost:8001/apis/" | jq .
curl -X GET "http://localhost:8001/apis/demo" |jq .
curl -X GET "http://localhost:8001/apis/" |jq .
curl -X DELETE "http://localhost:8001/apis/demo" |jq .
```

- /upstreams/ 


```bash
curl -H "Content-type: application/json" -X POST -d '{"name":"demo.upstream","slots":10}' "http://localhost:8001/upstreams/" | jq .
curl -X GET "http://localhost:8001/upstreams/demo.upstream" |jq .
curl -X GET "http://localhost:8001/upstreams" |jq .
curl -X DELETE "http://localhost:8001/upstreams/demo.upstream" |jq .
```

- /upstreams/{name or id}/targets 


```bash
curl -H "Content-type: application/json" -X POST -d '{"target":"192.168.226.129:8090","weight":1}' "http://localhost:8001/upstreams/demo.upstream/targets" | jq .
curl -X GET "http://localhost:8001/upstreams/test.upstream/targets" |jq .
curl -X GET "http://localhost:8001/upstreams" |jq .

curl -X DELETE "http://localhost:8001/upstreams/test.upstream/targets/target"
```

- /plugins/ 


```bash
--修改删除只能通过id;
curl -H "Content-type: application/json" -X POST -d '{"name":"key-auth"}' "http://localhost:8001/apis/demo/plugins/" |jq .
curl -H "Content-type: application/json" -X PATCH -d '{"enabled":"true","config.key_names":"apikey"}' "http://localhost:8001/apis/demo/plugins/9a14eea2-731b-48f7-8cb4-949ea9c0f25a" |jq .
curl -X GET "http://localhost:8001/plugins/" |jq .
curl -X GET "http://localhost:8001/apis/demo/plugins/" |jq .
curl -X DELETE "http://localhost:8001/apis/demo/plugins/9a14eea2-731b-48f7-8cb4-949ea9c0f25a" |jq .
```

- /consumers/ 


```bash
curl -H "Content-type: application/json" -X POST -d '{"username":"demo-user"}' "http://localhost:8001/consumers/" |jq .
curl  -X DELETE "http://localhost:8001/consumers/demo-user" |jq .
curl  -X GET "http://localhost:8001/consumers/demo-user" |jq .
添加key:
curl -H "Content-type: application/json" -X POST -d '{"key":"api_key"}' "http://localhost:8001/consumers/demo-user/key-auth/" |jq .
```

- /certificates/ 


```bash
curl -H "Content-type: application/json" -X POST -d '{"cert":"","key":"","snis":""}' "http://localhost:8001/apis/demo/certificates/" |jq .
```

- /snis/ 


```bash
curl -H "Content-type: application/json" -X POST -d '{"name":"","ssl_certificate_id":""}' "http://localhost:8001/apis/demo/snis/" |jq .
```


## proxy routing 规则
- 配置多项 


```bash
备注：下面是接口响应格式
{
    "name": "my-api",
    "upstream_url": "http://my-api.com",
    "hosts": ["example.com", "service.com"],
    "uris": ["/foo", "/bar"],
    "methods": ["GET"]
}
```

- 遵循最长匹配优先评估, 


```bash
"uris": ["/version/\d+/status/\d+"]
"uris": ["/version"]
"uris": ["/"]

--请求
 GET /version/3/status/3  匹配第一个
 GET /version 匹配第二个
 GET /version/123 匹配第二个
 GET /vs   匹配第三个
```


## Load Balancing reference
- 
DNS based loadbalancing：dns_resolver配置dns ip，解析多个ip后，自动使用基于DNS负载均衡（加权负载均衡器,将做一个简单的循环）,当dns_stale_ttl 超时后请求dns；

- 
Ring-balancer：upstream（slots\orderlist） target(weight)


## 集群
- 0.11版本：当一个节点发生更新到数据库后，其他节点需要间隔db_update_frequency后，更新缓存失效字段。在使用Cassandra数据库db_update_propagation（数据节点传播延时）必须配置。

为防止db_update_frequency后错过一个失效事件，db_cache_ttl全量更新cache 


```bash
db_update_frequency = 5  
db_update_propagation = 0
db_cache_ttl = 3600
```

- 0.10版本：通过 Kong 集群，每个节点能够知道其它节点的存在。并且当一个Kong 节点有对数据更新，该节点有责任通知这个变化给集群里的其他节点，

通知其其他节点把本地内存中的缓存无效，重新从数据库中获取更新后的数据。 


```bash
cluster_listen    通信
cluster_listen_rpc 代理通信
cluster_advertise
cluster_encrypt_key base64编码16字节加密集群通信
cluster_keyring_file
cluster_ttl_on_failure  //失败节点超时后停止被链接；
cluster_profile  //local, lan, wan.

--数据库node表来存储节点信息：
 node
 VM_3_3_centos_0.0.0.0:7946_58b54877bc4a47d884b9986e71f49d8b | 10.100.3.3:7946           | 2017-08-07 04:29:05
 VM_3_2_centos_0.0.0.0:7946_e4147c192bcc4401acf698ca6374d59d | 10.100.3.2:7946           | 2017-06-12 10:19:24
```


## 常用插件
- 8个常用插件 

## 编写插件
- 文件结构 


```bash
基本
simple-plugin
├── handler.lua  一个接口来实现。每个函数是由kong一个请求的生命周期所需的时刻。(对应lua模块的生命周期)
└── schema.lua  插件引入参数类型、规则、校验

涉及数据库
complete-plugin
├── api.lua     与kong restful对接
├── daos.lua   数据库操作
├── handler.lua  //
├── migrations  
│   ├── cassandra.lua
│   └── postgres.lua //kong migrations操作数据初始化
└── schema.lua
```

- 编写rockspec文件: 


```bash
build = {
  type = "builtin",
  modules = {
    ["kong.plugins.my-plugin-addtag.handler"] = "kong/plugins/my-plugin-addtag/handler.lua",
    ["kong.plugins.my-plugin-addtag.schema"] = "kong/plugins/my-plugin-addtag/schema.lua"
  }
}
```



```bash
luarocks make --pack-binary-rock my-plugin-addtag-0.1.0-0.rockspec
```

- 修改kong.conf custom_plugins 挂载，重启; 

## 测试环境使用情况


```bash
http://***********/#/apis
```




