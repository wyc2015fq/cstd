
# ZooKeeper之ZkClient使用示例 - bitcarmanlee的博客 - CSDN博客


2018年03月30日 18:11:10[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：579


在我们实际的生产环境中，zookpeeper是一个很关键的组件，因为zk是用来注册发现服务的服务，zk集群的稳定性与可靠性直接决定了其他服务的稳定。
ZkClient是一个开源的ZooKeeper客户端，是在原生的ZooKeeper API接口之上进行包装，是一个更易使用的ZooKeeper客户端。ZkClient在内部实现了Session超时重连、Watcher反复注册等功能，使得ZooKeeper客户端的繁琐细节对开发人员透明。
实际项目中，我们就使用ZkClient来对zk进行相关操作。为了方便测试，特意在本地开启了一个zk的server并使用zkclient来进行操作。
## 1.安装Zookeeper并启动服务
首先从网上下载到zookeeper的压缩包，然后解压。
cd到解压的目录以后，bin目录里有zkServer.sh，然后执行`./zkServer.sh start`，zk服务就已经在本机启动。
![这里写图片描述](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果出现如下页面，则表明此时zk服务已正常启动。
[
](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2.用ZkClient监控子节点的变化情况
[
](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)b按惯例，直接上代码。
[

](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
public static void subchild() throws InterruptedException {
        ZkClient zkClient = new ZkClient(
```
```python
"localhost:2181"
```
```python
,
```
```python
5000
```
```python
)
```
```python
;
```
```python
String path =
```
```python
"/mipush"
```
```python
;
```
```python
zkClient
```
```python
.subscribeChildChanges
```
```python
(path, new IZkChildListener() {
            @Override
            public void handleChildChange(String parentPath, List<String> currentChilds) throws Exception {
                System
```
```python
.out
```
```python
.println
```
```python
(parentPath +
```
```python
"'s child changed, currentChilds: "
```
```python
+　currentChilds)
```
```python
;
```
```python
}
        })
```
```python
;
```
```python
if(zkClient
```
```python
.exists
```
```python
(path)) {
            zkClient
```
```python
.deleteRecursive
```
```python
(path)
```
```python
;
```
```python
// zkClient
```
```python
.delete
```
```python
(path)
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(
```
```python
"delete "
```
```python
+ path +
```
```python
" successful"
```
```python
)
```
```python
;
```
```python
}
        zkClient
```
```python
.createPersistent
```
```python
(path)
```
```python
;
```
```python
Thread
```
```python
.sleep
```
```python
(
```
```python
1000
```
```python
)
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(
```
```python
"now child path is: "
```
```python
+ zkClient
```
```python
.getChildren
```
```python
(path))
```
```python
;
```
```python
zkClient
```
```python
.createPersistent
```
```python
(path +
```
```python
"/p1"
```
```python
)
```
```python
;
```
```python
Thread
```
```python
.sleep
```
```python
(
```
```python
1000
```
```python
)
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(
```
```python
"now child path is: "
```
```python
+ zkClient
```
```python
.getChildren
```
```python
(path))
```
```python
;
```
```python
zkClient
```
```python
.createPersistent
```
```python
(path +
```
```python
"/p2"
```
```python
,
```
```python
"abc"
```
```python
)
```
```python
;
```
```python
Thread
```
```python
.sleep
```
```python
(
```
```python
1000
```
```python
)
```
```python
;
```
```python
System
```
```python
.out
```
```python
.println
```
```python
(
```
```python
"now child path is: "
```
```python
+ zkClient
```
```python
.getChildren
```
```python
(path))
```
```python
;
```
```python
}
```
[
](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)将上部分代码成功运行起来以后，结果如下：
[
](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
Node /mipush deleted.
Node /mipush changed,
```
```python
new
```
```python
data:
```
```python
null
```
```python
/mipush
```
```python
's
```
```python
child changed, currentChilds: []
now child path
```
```python
is
```
```python
: []
/mipush
```
```python
's
```
```python
child changed, currentChilds: [p1]
now child path
```
```python
is
```
```python
: [p1]
/mipush
```
```python
's
```
```python
child changed, currentChilds: [p1, p2]
now child path
```
```python
is
```
```python
: [p1, p2]
```
[
](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 3.用ZkClient监控节点数据变化情况
[

](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
public
```
```python
static
```
```python
void
```
```python
changedata
```
```python
() throws InterruptedException {
        ZkClient zkClient =
```
```python
new
```
```python
ZkClient(
```
```python
"localhost:2181"
```
```python
,
```
```python
5000
```
```python
);
        String path =
```
```python
"/mipush"
```
```python
;
```
```python
if
```
```python
(zkClient.exists(path)) {
            zkClient.deleteRecursive(path);
```
```python
// zkClient.delete(path);
```
```python
System.
```
```python
out
```
```python
.println(
```
```python
"delete "
```
```python
+ path +
```
```python
" successful"
```
```python
);
        }
        zkClient.createPersistent(path,
```
```python
"abc"
```
```python
);
        zkClient.subscribeDataChanges(path,
```
```python
new
```
```python
IZkDataListener() {
            @Override
```
```python
public
```
```python
void
```
```python
handleDataChange
```
```python
(String dataPath, Object data) throws Exception {
                System.
```
```python
out
```
```python
.println(
```
```python
"Node "
```
```python
+ dataPath +
```
```python
" changed, new data: "
```
```python
+ data);
            }
            @Override
```
```python
public
```
```python
void
```
```python
handleDataDeleted
```
```python
(String dataPath) throws Exception {
                System.
```
```python
out
```
```python
.println(
```
```python
"Node "
```
```python
+ dataPath +
```
```python
" deleted."
```
```python
);
            }
        });
```
```python
// 读取节点数据
```
```python
System.
```
```python
out
```
```python
.println(
```
```python
"now data is: "
```
```python
+ zkClient.readData(path).toString());
        zkClient.writeData(path,
```
```python
"def"
```
```python
);
        Thread.sleep(
```
```python
1000
```
```python
);
        System.
```
```python
out
```
```python
.println(
```
```python
"Node exists: "
```
```python
+ zkClient.exists(path));
        zkClient.delete(path);
        System.
```
```python
out
```
```python
.println(
```
```python
"Node exists: "
```
```python
+ zkClient.exists(path));
    }
```
[
](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)运行结果如下：
[

](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```python
delete
```
```python
/mipush successful
now data
```
```python
is
```
```python
: abc
Node /mipush changed, new data: def
Node
```
```python
exists
```
```python
:
```
```python
true
```
```python
Node
```
```python
exists
```
```python
:
```
```python
false
```
[            ](https://img-blog.csdn.net/20180330180118511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JpdGNhcm1hbmxlZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

