# Zookeeper——3、使用zkClient操作zookeeper - 隔壁老王的专栏 - CSDN博客





2016年12月03日 23:59:46[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：12100








zkClient在原生API的基础上进行了封装，简化了zookeeper的复杂性。（[zookeeper文章所需的jar包](http://download.csdn.net/detail/haoyuyang/9703158)）

**创建客户端方法：ZKClient(Arguments)**

参数1：zkServers，zookeeper服务器地址，用“,”分隔。

参数2：sessionTimeout，会话超时时间，单位毫秒，默认为30000ms。

参数3：connectionTimeout，连接超时时间。

参数4：IZkConnection接口的实现类。

参数5：zkSerializer，自定义序列化实现。

```java
public class ZkClientBase {
    static final String CONNECT_ADDR = "192.168.1.104:2181,192.168.1.105:2181,192.168.1.107:2181";
    static final int SESSION_TIMEOUT = 5000;

    public static void main(String[] args) throws InterruptedException {
        ZkClient zkClient = new ZkClient(new ZkConnection(CONNECT_ADDR), SESSION_TIMEOUT);

        //1.create和delete方法
        /*zkClient.createEphemeral("/temp"); //创建临时节点，会话失效后删除
        zkClient.createPersistent("/super/c1", true); //创建持久化节点，true表示如果父节点不存在则创建父节点
        Thread.sleep(10000);
        zkClient.delete("/temp"); //删除节点
        zkClient.deleteRecursive("/super"); //递归删除，如果该节点下有子节点，会把子节点也删除
        */

        //2.设置path和data，并读取子节点和每个节点的内容
        /*zkClient.createPersistent("/super", "1234"); //创建并设置节点的值
        zkClient.createPersistent("/super/c1", "内容一");
        zkClient.createPersistent("/super/c2", "内容二");
        List<String> children = zkClient.getChildren("/super");
        for(String child : children) {
            System.out.print(child + "：");
            String childPath = "/super/" + child;
            String data = zkClient.readData(childPath); //读取指定节点的值
            System.out.println(data);
        }*/

        //3.更新和判断节点是否存在
        zkClient.writeData("/super/c1", "新内容"); //修改指定节点的值
        String cData = zkClient.readData("/super/c1");
        System.out.println(cData);
        System.out.println(zkClient.exists("/super/c1")); //判断指定节点是否存在

        zkClient.close();
    }
}
```



在以上的代码中，我们可以发现没有与zookeeper原生API中类似Watcher、watch参数，这也就说明使用zkClient，开发人员无需关心反复注册Watcher的问题。zkClient给开发人员提供了一套监听方式，我们可以使用监听节点的方式进行操作，剔除了繁琐的反复Watcher操作，简化了代码的复杂度。

**subscribeChildChanges方法（只监听节点的变化）：**

参数1：path路径

参数2：实现了IZkChildListener接口的类，只需重写handleChildChanges(String parentPath, List<String> currentChilds)方法。其中parentPath为所监听节点的全路径，currentChilds为最新的子节点列表（相对路径）。

IZkClientListener事件针对于下面三个事件触发：

新增子节点、减少子节点、删除节点

```java
public class SubscribeChildChanges {
    static final String CONNECT_ADDR = "192.168.1.104:2181";
    static final int SESSION_TIMEOUT = 50000;

    public static void main(String[] args) throws InterruptedException {
        ZkClient zkClient = new ZkClient(new ZkConnection(CONNECT_ADDR, SESSION_TIMEOUT));
        zkClient.subscribeChildChanges("/super", (parentPath, currentChilds) -> {
            System.out.println("parentPath：" + parentPath);
            System.out.println("currentChilds：" + currentChilds);
        });

        Thread.sleep(3000);
        zkClient.createPersistent("/super");
        Thread.sleep(1000);
        zkClient.createPersistent("/super/c1", "内容一");
        Thread.sleep(1000);
        zkClient.createPersistent("/super/c2", "内容二");
        Thread.sleep(1000);
        zkClient.delete("/super/c2");
        Thread.sleep(1000);
        zkClient.deleteRecursive("/super");
        Thread.sleep(Integer.MAX_VALUE);
    }
}
```



**subscribeDataChanges方法（只监听节点数据的变化）：**

参数1：path路径

参数2：实现了IZkDataListener接口的类，需要重新handleDataDeleted(String path)方法（节点删除时触发）和handleDataChanges(String path, Object data)方法（节点数据改变时触发）。经测试，该接口只会对所监控的path的数据变化，子节点数据发送变化不会被监控到。

```java
public class SubscribeDataChanges {
    static final String CONNECT_ADDR = "192.168.1.104:2181";
    static final int SESSION_TIMEOUT = 50000;

    public static void main(String[] args) throws InterruptedException {
        ZkClient zkClient = new ZkClient(new ZkConnection(CONNECT_ADDR, SESSION_TIMEOUT));
        zkClient.subscribeDataChanges("/super", new IZkDataListener() {
            @Override
            public void handleDataChange(String s, Object o) throws Exception {
                System.out.println("变更节点为：" + s + "，变更数据为：" + o);
            }

            @Override
            public void handleDataDeleted(String s) throws Exception {
                System.out.println("删除的节点为：" + s);
            }
        });
        zkClient.createPersistent("/super", "123");
        Thread.sleep(3000);
        zkClient.writeData("/super", "456", -1);
        Thread.sleep(1000);
        zkClient.createPersistent("/super/c1", "789"); //不会被监控到
        zkClient.deleteRecursive("/super");
        Thread.sleep(Integer.MAX_VALUE);
    }
}
```
运行结果：



![](https://img-blog.csdn.net/20161204162349589)



