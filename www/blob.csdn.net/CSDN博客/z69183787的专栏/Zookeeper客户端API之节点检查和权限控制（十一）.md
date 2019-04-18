# Zookeeper客户端API之节点检查和权限控制（十一） - z69183787的专栏 - CSDN博客
2018年02月25日 11:17:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：133
个人分类：[分布式-Zookeeper](https://blog.csdn.net/z69183787/article/category/5568407)
[http://blog.csdn.net/wo541075754/article/details/67636455](http://blog.csdn.net/wo541075754/article/details/67636455)
本篇博客介绍一下原生API的节点检查是否存在和节点操作权限控制。Zookeeper提供的方法的使用方式与前面增删改查操作类似，具体使用案例不再逐一用实例说明。
## 节点检查是否存在
原生API的提供4个相应的方法，通过这些方法，可以检查节点是否存在，返回节点Stat信息，对节点进行注册监听事件等操作。
### 方法
```java
public Stat exists(final String path, Watcher watcher)
```
- 1
```java
public Stat exists(String path, boolean watch)
```
- 1
```java
public void exists(final String path, Watcher watcher,
            StatCallback cb, Object ctx)
```
- 1
- 2
```java
public void exists(String path, boolean watch, StatCallback cb, Object ctx)
```
- 1
### 参数说明
|参数|说明|
|----|----|
|path|操作节点路径|
|watcher|注册Watcher，用于监听节点创建、节点删除、节点更新事件。|
|watch|是否使用默认watcher|
|cb|注册一个回调函数|
|ctx|传递上下文信息|
### 其他说明
1、无论节点是否存在使用exists方法都可以进行监听注册。节点不存在时注册监听之后，当节点被创建则会通知客户端。 
2、指定节点的子节点的变化不会通知客户端。
## 权限控制
在以前的实例中我们通过API创建的节点都是默认创建后可以随意操作。但在实际生产过程中，往往是多个应用使用同一套Zookeeper服务。而且，不同应用系统很少使用共同的数据，这时就需要通过权限来控制不同服务直接访问操作节点的权限。
Zookeeper提供了多种权限控制模式：world、auth、digest、ip和super。
创建完成会话之后，调用Zookeeper提供的addAuthInfo方法给会话操作添加权限，后续的其他操作都会添加此权限信息。此会话创建的节点，其他无权限的会话将无法操作，以此达到权限控制的目的。
### 特殊操作
其他操作都遵循上面所说的权限规则，唯独删除权限有所区别。 
删除权限的作用范围为节点的子节点，即当对一个节点赋予权限之后，依然可以自由的删除此节点，但是此节点下面的子节点需要对应的权限才能够删除。
