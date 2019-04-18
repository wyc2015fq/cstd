# twemproxy explore - gauss的专栏 - CSDN博客
2015年06月27日 11:30:59[gauss](https://me.csdn.net/mathlmx)阅读数：318
twemproxy,也叫nutcraker。是一个twtter开源的一个redis和memcache代理服务器。 redis作为一个高效的缓存服务器，非常具有应用价值。但是当使用比较多的时候，就希望可以通过某种方式 统一进行管理。避免每个应用每个客户端管理连接的松散性。同时在一定程度上变得可以控制。 搜索了不少的开源代理项目，知乎实现的python分片客户端。node的代理中间层，还有各种restfull的开源代理。
- 
RedBrige
- C + epoll实现的一个小的webserver
- redis自身执行lua脚本的功能来执行redis命令
- 访问时在url中带上lua脚本文件的名字，直接调用执行该lua脚本
- 本质是通过http协议远程执行lua脚本
- 
Webdis
- libevent, http-parser...实现的小型web服务器
- C 语言实现，通过unix-socket,TCP调用redis命令。
- 访问方法： `/cmd/key/arg0,arg1,...` 实质是对redis命令的简单变换
- 
redis-proxy
- 使用node写的redis代理层。
- 支持主从节点的失败处理（可以仔细研究）
- 测试后发现性能为原生的1/3
- 
twemproxy
- 
支持失败节点自动删除
- 可以设置重新连接该节点的时间
- 可以设置连接多少次之后删除该节点
- 该方式适合作为cache存储
- 
支持设置HashTag
- 通过HashTag可以自己设定将两个KEYhash到同一个实例上去。
- 
减少与redis的直接连接数
- 保持与redis的长连接
- 可设置代理与后台每个redis连接的数目
- 
自动分片到后端多个redis实例上
- 多种hash算法（部分还没有研究明白)
- 可以设置后端实例的权重
- 
避免单点问题
- 可以平行部署多个代理层.client自动选择可用的一个
- 
支持redis pipelining request
- 
支持状态监控
- 可设置状态监控ip和端口，访问ip和端口可以得到一个json格式的状态信息串
- 可设置监控信息刷新间隔时间
- 
高吞吐量
- 连接复用，内存复用。
- 将多个连接请求，组成reids pipelining统一向redis请求。
另外可以修改redis的源代码，抽取出redis中的前半部分，作为一个中间代理层。最终都是通过linux下的epoll 事件机制提高并发效率，其中nutcraker本身也是使用epoll的事件机制。并且在性能测试上的表现非常出色。
配置部署建议: 编译时候打开logging模块。
redis部署知识: AOF；一种记录redis写操作的文件，用于恢复redis数据。
那么说一下搭建环境的方法：https://github.com/twitter/twemproxy
To build nutcracker from [distribution tarball](http://code.google.com/p/twemproxy/downloads/list):
```
$ ./configure
$ make
$ sudo make install
```
To build nutcracker from [distribution tarball](http://code.google.com/p/twemproxy/downloads/list) in *debug mode*:
```
$ CFLAGS="-ggdb3 -O0" ./configure --enable-debug=full
$ make
$ sudo make install
```
To build nutcracker from source with *debug logs enabled* and *assertions disabled*:
```
$ git clone git@github.com:twitter/twemproxy.git
$ cd twemproxy
$ autoreconf -fvi
$ ./configure --enable-debug=log
$ make
$ src/nutcracker -h
```
我使用的是第一种方法（需要下载他的nutcracker包），原因是服务器基本包缺的太多。
 ---------------------------------------------------------------------------
安装完需要配置（我只配置了本地36一台机子，#后边是注释）
redis1: #服务器池成员名字
  listen: 192.168.6.36:19999 #twemproxy启动端口
  hash: fnv1a_64 #具体的hash函数
  distribution: ketama #hash算法-如何分布key，关系到均匀分布key的算法，影响命中key等性能，这里是ketama算法
  auto_eject_hosts: true #是否在节点无法响应的时候临时找出，区分save data的slave和cache data的slave
  redis: true #是否是redis的proxy
  server_retry_timeout: 2000 #重试时间ms
  server_failure_limit: 1 #节点故障多少次后摘除
  servers: # redis节点（ip:端口号:权重）
   - 192.168.6.36:6379:1
-----------------------------------------------------------------------------------
 安装配置完成后，可以用nutcracker -t来检查配置文件
                                        nutcracker -d后天启动服务
                                        nucracker -D和nucracker stat查看代理状态
