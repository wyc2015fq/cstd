# BrokerBootstrap作用调试 - zj360202的专栏 - CSDN博客





2012年12月07日 15:11:36[zj360202](https://me.csdn.net/zj360202)阅读数：3054标签：[TimeTunnel																[Timetunnel](https://so.csdn.net/so/search/s.do?q=Timetunnel&t=blog)](https://so.csdn.net/so/search/s.do?q=TimeTunnel&t=blog)
个人分类：[TimeTunnel](https://blog.csdn.net/zj360202/article/category/1298101)








```java
执行com.taobao.timetunnel.bootstrap.BrokerBootstrap 的main方法报错
```

```java
Starting a reliable broker.
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:zookeeper.version=3.3.2-1031432, built on 11/05/2010 05:32 GMT
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:host.name=2S4PKZGC5RFRU9Z
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:java.version=1.7.0_07
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:java.vendor=Oracle Corporation
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:java.home=C:\Program Files (x86)\Java\jre7
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:java.class.path=F:\workspace\timetunnel\bin;E:\TimeTunnel\apps\dfswriter\lib\commons-io-1.4.jar;E:\TimeTunnel\apps\dfswriter\lib\commons-pool-1.4.jar;E:\TimeTunnel\apps\dfswriter\lib\FilterApi.jar;E:\TimeTunnel\apps\dfswriter\lib\hadoop-0.19.2-core.jar;E:\TimeTunnel\apps\dfswriter\lib\hadoop-0.20.2-core.jar;E:\TimeTunnel\apps\dfswriter\lib\hbase-0.90.3.jar;E:\TimeTunnel\apps\dfswriter\lib\libthrift-0.5.0-20100116.jar;E:\TimeTunnel\apps\dfswriter\lib\libthrift-0.7.0.jar;E:\TimeTunnel\apps\dfswriter\lib\quartz-all-1.8.3.jar;E:\TimeTunnel\apps\dfswriter\lib\TTClient.jar;E:\TimeTunnel\apps\dfswriter\lib\zookeeper-3.3.2.jar;E:\TimeTunnel\apps\savefile\lib\libthrift-0.5.0-20100116.jar;E:\TimeTunnel\apps\savefile\lib\quartz-all-1.8.3.jar;E:\TimeTunnel\idl\lib\javax.jms_1.1.0.jar;E:\TimeTunnel\idl\lib\jmxri-1.2.1.jar;E:\TimeTunnel\idl\lib\jmxtools-1.2.1.jar;E:\TimeTunnel\idl\lib\libthrift-0.5.0-20100116.jar;E:\TimeTunnel\router\lib\libthrift-0.5.0.jar;E:\TimeTunnel\router\lib\timetunnel2-cluster-0.1.0.jar;E:\TimeTunnel\router\lib\zk-recipes.jar;E:\TimeTunnel\router\lib\zookeeper-3.2.2.jar;E:\eclipselib\junit-4.11.jar;E:\eclipselib\zookeeper-3.3.5.jar;E:\eclipse\plugins\org.apache.log4j_1.2.15.v201012070815.jar;E:\eclipselib\google-gson-2.2.2\gson-2.2.2.jar;E:\eclipselib\commons-cli-1.2.jar;E:\eclipselib\commons-lang-2.5.jar;E:\eclipselib\commons-codec-1.6.jar
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:java.library.path=C:\Program Files (x86)\Java\jre7\bin;C:\Windows\Sun\Java\bin;C:\Windows\system32;C:\Windows;C:/Program Files (x86)/Java/jre7/bin/client;C:/Program Files (x86)/Java/jre7/bin;C:/Program Files (x86)/Java/jre7/lib/i386;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Windows7Master;E:\Program Files\TortoiseSVN\bin;C:\Program Files (x86)\Java\jdk1.7.0_07/bin;C:\Program Files (x86)\Java\jdk1.7.0_07/jre/bin;D:\apache-ant-1.8.4\bin;$ANT_HOME\bin;$HADOOP_HOME\bin;E:\eclipse;;.
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:java.io.tmpdir=C:\Users\ADMINI~1\AppData\Local\Temp\
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:java.compiler=<NA>
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:os.name=Windows 7
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:os.arch=x86
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:os.version=6.1
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:user.name=Administrator
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:user.home=C:\Users\Administrator
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Client environment:user.dir=F:\workspace\timetunnel
12/12/07 14:40:59 INFO zookeeper.ZooKeeper: Initiating client connection, connectString=192.168.15.36:2181 sessionTimeout=10000 watcher=com.taobao.timetunnel.zookeeper.ZooKeeperConnector@132b73b
12/12/07 14:40:59 INFO zookeeper.ClientCnxn: Opening socket connection to server /192.168.15.36:2181
12/12/07 14:41:09 WARN zookeeper.ClientCnxn: Session 0x0 for server null, unexpected error, closing socket connection and attempting reconnect
java.net.ConnectException: Connection refused: no further information
	at sun.nio.ch.SocketChannelImpl.checkConnect(Native Method)
	at sun.nio.ch.SocketChannelImpl.finishConnect(Unknown Source)
	at org.apache.zookeeper.ClientCnxn$SendThread.run(ClientCnxn.java:1119)
12/12/07 14:41:10 INFO zookeeper.ClientCnxn: Opening socket connection to server 192.168.15.36/192.168.15.36:2181
12/12/07 14:41:11 WARN zookeeper.ClientCnxn: Session 0x0 for server null, unexpected error, closing socket connection and attempting reconnect
java.net.ConnectException: Connection refused: no further information
	at sun.nio.ch.SocketChannelImpl.checkConnect(Native Method)
	at sun.nio.ch.SocketChannelImpl.finishConnect(Unknown Source)
	at org.apache.zookeeper.ClientCnxn$SendThread.run(ClientCnxn.java:1119)
12/12/07 14:41:13 INFO zookeeper.ClientCnxn: Opening socket connection to server 192.168.15.36/192.168.15.36:2181
12/12/07 14:41:14 WARN zookeeper.ClientCnxn: Session 0x0 for server null, unexpected error, closing socket connection and attempting reconnect
java.net.ConnectException: Connection refused: no further information
	at sun.nio.ch.SocketChannelImpl.checkConnect(Native Method)
	at sun.nio.ch.SocketChannelImpl.finishConnect(Unknown Source)
	at org.apache.zookeeper.ClientCnxn$SendThread.run(ClientCnxn.java:1119)
```



com.taobao.timetunnel.bootstrap.BrokerBootstrap代码

```java
package com.taobao.timetunnel.bootstrap;

import static java.net.InetAddress.getLocalHost;

import java.io.File;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;

import com.taobao.timetunnel.broker.EfficientThriftBroker;
import com.taobao.timetunnel.broker.ReliableThriftBroker;
import com.taobao.timetunnel.broker.ThriftBroker;
import com.taobao.timetunnel.center.Center;
import com.taobao.timetunnel.center.ZookeeperCenter;
import com.taobao.util.MemoryMonitor;
import com.taobao.util.PropertiesHelper;
import com.taobao.util.SizeConverter;
import com.taobao.util.PropertiesHelper.Converter;

/**
 * {@link BrokerBootstrap}
 * 
 * @author <a href=mailto:jushi@taobao.com>jushi</a>
 * @created 2010-11-18
 * 
 */
public class BrokerBootstrap {

  /**
   * @param args
   */
  public static void main(final String[] args) {
    String conf = "conf.properties";
    if (args.length == 1) conf = args[0];

    try {
      final ThriftBroker<ByteBuffer> thriftBroker =
        createThriftBrokerWith(new PropertiesHelper(new File(conf)));
      addShutdownHook(thriftBroker);
      Thread.currentThread().setName("reliable-broker");
      thriftBroker.start();
    } catch (final Exception e) {
      printUsageAndExit(new File(conf), e);
    }
  }

  private static void addShutdownHook(final ThriftBroker<ByteBuffer> thriftBroker) {
    Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {

      @Override
      public void run() {
        thriftBroker.stop();
      }
    }, "shutdown-broker-hook"));
  }

  private static Center createCenterWith(final PropertiesHelper helper) {
    return new ZookeeperCenter(helper.getString("zookeeper.connectString"),
                               helper.getInt("zookeeper.sessionTimeout"));
  }

  private static MemoryMonitor createMonitorWith(final PropertiesHelper helper) {
    final long max = MemoryMonitor.max() / 100;
    final long shortage = max * helper.getInt("memory.shortageRatio", 45);
    final long abundant = max * helper.getInt("memory.abundantRatio", 75);
    return new MemoryMonitor(shortage, abundant);
  }

  private static Converter<Integer> sizeConverter = new SizeConverter();

  private static ThriftBroker<ByteBuffer> createThriftBrokerWith(final PropertiesHelper helper) throws UnknownHostException {
    final File data = new File(helper.getString("broker.home", "."), "data");//创建以"broker.home"对应路径下的data文件
    data.mkdirs();

    final Center center = createCenterWith(helper);
    final MemoryMonitor monitor = createMonitorWith(helper);

    ThriftBroker<ByteBuffer> thriftBroker = null;
    final String host = helper.getString("broker.host", getLocalHost().getHostName());
    final int port = helper.getInt("external.port");
    final String group = helper.getString("broker.group");
    final int syncPoint = helper.getInt("group.syncPoint", 200);
    final int maxMessageSize = helper.get(sizeConverter, "broker.maxMessageSize", "4K");

    int chunkCapacity = helper.get(sizeConverter, "chunk.capacity", "64M");
    int chunkBuffer = helper.get(sizeConverter, "chunk.buffer", "32K");
    if (helper.contains("internal.port")) {
      thriftBroker =
        new ReliableThriftBroker(center,
                                 host,
                                 port,
                                 helper.getInt("internal.port"),
                                 group,
                                 syncPoint,
                                 maxMessageSize,
                                 chunkCapacity,
                                 chunkBuffer,
                                 monitor,
                                 data);
      System.out.println("Starting a reliable broker.");
    } else {
      thriftBroker =
        new EfficientThriftBroker(center,
                                  host,
                                  port,
                                  group,
                                  syncPoint,
                                  monitor,
                                  data,
                                  maxMessageSize,
                                  chunkCapacity,
                                  chunkBuffer);
      System.out.println("Starting a efficient broker.");
    }
    thriftBroker.setMaxReadBufferBytes(helper.getInt("broker.maxReadBufferBytes", Integer.MAX_VALUE));
    thriftBroker.setWorkThread(helper.getInt("broker.workThread", 5));
    return thriftBroker;
  }

  private static void printUsageAndExit(final File file, final Exception e) {
    e.printStackTrace();
    System.err.println("Invalid conf file path, please check your conf file " + file);
    System.err.println("Usage : CMD [conf file path]");
    System.exit(-1);
  }

}
```



conf.propertites

```java
# brokerçhomeè·¯å¾, é»è®¤ä¸ºå½åç®å½, brokeräº§çæä¹åæ°æ®ä¼å­å¨è¿ä¸ªç®å½ä¸
broker.home = /usr/local/timetunnel/broker/target
# brokerç», åç»ä»¥ä¾¿åçº§æå¡.
broker.group = group1
# è®¾ç½®brokerå¯¹å¤æå¡(è®¢éåå¸)çç«¯å£
external.port = 9999
# è®¾ç½®brokerå¯¹åæå¡(æ°æ®å¯é åæ­¥)çç«¯å£, ä¸è®¾ç½®åbrokerä¾æ§å¯ä»¥åç¹è¿è¡ 
internal.port = 9998
# zookeeperæ¯brokeréç¾¤çåè°è, å¿é¡»éç½®å¨è¿æ¥å°ååç«¯å£, eg : 192.168.1.132:33561,192.168.1.133:33561/root
zookeeper.connectString =192.168.15.36:2181
# è¿æ¥zookeeperä¼è¯è¶æ¶æ¶é´, åä½æ¯«ç§, å®åºè¯¥æ¯zookeeperæå¡å¨éç½®çtickTimeç2~20å 
zookeeper.sessionTimeout = 10000
# brokerçä¸»æºå, ä¹å¯ä»¥æ¯ip, ç³»ç»ä¼èªå¨å, ä¸è¬æ ééç½®
broker.host = 192.168.15.36
######################## ä»¥ä¸æ¯å¯å¨å¿ééç½®é¡¹ ######################### 
######################## ä»¥ä¸æ¯è°ä¼å¯ééç½®é¡¹ #########################
# ä¸ä¸ªè®¢ééå¸¸ä¸ç»clientæ¥è·å, ä¸ºä¿è¯æ¯ä¸ªclientè·åçæ°éè¿ä¹å¹³å, å°±éè¦å¨ç´¯è®¡ä¸å®æ°éæ¶è¿è¡åæ­¥, è¿ä¸ªåæ­¥ç¹é»è®¤ä¸º200 
# group.syncPoint = 200
# ä¸ºé¿ååå­æº¢åº, brokerä¼æ ¹æ®åå­çæ¶èæåµ, å°é¨åæ°æ®äº¤æ¢å°ç£çæä»¶ä¸­, åå­ä¸­ä¿å­å¶æ°æ®çç´¢å¼
# å½åå­ç­ç¼ºæ¶å¼å§äº¤æ¢, ç´å°åå­åè£, ä¸é¢æ¯åå«éç½®ç­ç¼ºååè£çåå­æ¯ä¾å¼, æå¤§ä¸º99, æå°ä¸º1.
# åå­ç­ç¼ºæ¯ä¾é»è®¤ä¸º45
# memory.shortageRatio = 45
# åå­åè£æ¯ä¾é»è®¤ä¸º75
# memory.abundantRatio = 75
# è®¾ç½®brokerä¸­thrift serveræå¡ççº¿ç¨æ°, é»è®¤æ¯5, å¯ä»¥æ ¹æ®åºç¨è¦æ±éå½è°æ´
# broker.workThread = 5
# è®¾ç½®brokerä¸­thrift serveræå¤§è¯»ååå­æ°, é»è®¤ä¸º2ç31æ¬¡æ¹, ä¸è¬æåµä¸æ éä¿®æ¹ 
# broker.maxReadBufferBytes = 
# è®¾ç½®brokerä¸­åä¸ªæ¶æ¯çæå¤§å¼, é»è®¤ä¸º4K, åä½å¯ä»¥æ¯KæM
# broker.maxMessageSize = 4K
# åå­äº¤æ¢ç£ççæä»¶å¤§å°, é»è®¤64M, åä½å¯ä»¥æ¯KæM
# chunk.capacity = 64M 
# åå­äº¤æ¢ç£ççæä»¶ç¼å­, é»è®¤32K, åä½å¯ä»¥æ¯KæM
# chunk.buffer = 32K
```







