# Zookeeper客户端API之修改删除节点（十） - z69183787的专栏 - CSDN博客
2018年02月25日 11:17:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：234
[http://blog.csdn.net/wo541075754/article/details/67631965](http://blog.csdn.net/wo541075754/article/details/67631965)
## 修改节点
### 方法
同步方法：
```java
public Stat setData(final String path, byte data[], int version)
```
- 1
异步方法：
```java
public void setData(final String path, byte data[], int version,
            StatCallback cb, Object ctx)
```
- 1
- 2
### 参数说明
|参数|说明|
|----|----|
|path|操作节点路径|
|data[]|要覆盖现有节点内容的字节数组|
|version|指定更新节点的数据版本。当为-1时表示任何版本|
|cb|注册一个回调函数|
|ctx|传递上下文信息|
### 其他说明
1、更新操作中的版本参数如果为-1，则表示更新操作针对任何版本均可。当更新版本不为-1，且不等于节点的目前版本，则更新失败。 
2、即使data[]的数据跟原有节点内的数据一样，进行更新操作之后，节点的信息也会发生变化，比如版本信息和更新时间。 
3、异步操作与同步操作的区别可参考之前章节的类似接口。
## 删除节点
### 方法
同步删除：
`public void delete(final String path, int version)`- 1
异步删除：
```
public void delete(final String path, int version, VoidCallback cb,
            Object ctx)
```
- 1
- 2
### 参数说明
|参数|说明|
|----|----|
|path|操作节点路径|
|version|指定更新节点的数据版本。当为-1时表示任何版本|
|cb|注册一个回调函数|
|ctx|传递上下文信息|
### 其他说明
1、版本操作同修改节点使用方法。 
2、如果一个节点下面有子节点，需先删除子节点，然后才能删除父节点。
