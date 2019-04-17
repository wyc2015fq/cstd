# TimeTunnel ZooKeeperServer - zj360202的专栏 - CSDN博客





2012年12月07日 13:11:15[zj360202](https://me.csdn.net/zj360202)阅读数：944








```java
通过自带的ZooKeeperServerForTest对ZooKeeperServer进行测试
```

```java
/**
   * 测试ZooKeeperServer
   */
  private static final Logger LOG =
	        Logger.getLogger(ZooKeeperServerMain.class);

  private NIOServerCnxn.Factory cnxnFactory;

	public static void main(String[] args) {
		  ServerConfig config = new ServerConfig();
	    if (args.length == 1) {
	        try {
				config.parse(args[0]);
			} catch (ConfigException e) {
				e.printStackTrace();
			}
	    } else {
	        config.parse(args);
	    }
		  try {
			 //args[0]=8020,args[1]="d:\\",args[2]=1000
			new ZooKeeperServerForTest(8020,"d:\\",1000).test(args,config);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void test(String[] args,ServerConfig config) throws IOException{
		  
		  
		  LOG.info("Starting server");
	    try {
	        // Note that this thread isn't going to be doing anything else,
	        // so rather than spawning another thread, we will just call
	        // run() in this thread.
	        // create a file logger url from the command line args
	        ZooKeeperServer zkServer = new ZooKeeperServer();
	
	        FileTxnSnapLog ftxn = new FileTxnSnapLog(new
	               File(config.getDataLogDir()), new File(config.getDataDir()));
	        zkServer.setTxnLogFactory(ftxn);
	        zkServer.setTickTime(config.getTickTime());
	        zkServer.setMinSessionTimeout(config.getMinSessionTimeout());
	        zkServer.setMaxSessionTimeout(config.getMaxSessionTimeout());
	        cnxnFactory = new NIOServerCnxn.Factory(config.getClientPortAddress(),
	                config.getMaxClientCnxns());
	        cnxnFactory.startup(zkServer);
	        cnxnFactory.join();
	        if (zkServer.isRunning()) {
	            zkServer.shutdown();
	        }
	    } catch (InterruptedException e) {
	        // warn, but generally this is ok
	        LOG.warn("Server interrupted", e);
	    }
	}
```

运行结果：

```java
12/12/07 11:31:07 INFO server.ZooKeeperServerMain: Starting server
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:zookeeper.version=3.3.2-1031432, built on 11/05/2010 05:32 GMT
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:host.name=2S4PKZGC5RFRU9Z
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:java.version=1.7.0_07
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:java.vendor=Oracle Corporation
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:java.home=C:\Program Files (x86)\Java\jre7
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:java.class.path=F:\workspace\timetunnel\bin;E:\TimeTunnel\apps\dfswriter\lib\commons-io-1.4.jar;E:\TimeTunnel\apps\dfswriter\lib\commons-pool-1.4.jar;E:\TimeTunnel\apps\dfswriter\lib\FilterApi.jar;E:\TimeTunnel\apps\dfswriter\lib\hadoop-0.19.2-core.jar;E:\TimeTunnel\apps\dfswriter\lib\hadoop-0.20.2-core.jar;E:\TimeTunnel\apps\dfswriter\lib\hbase-0.90.3.jar;E:\TimeTunnel\apps\dfswriter\lib\libthrift-0.5.0-20100116.jar;E:\TimeTunnel\apps\dfswriter\lib\libthrift-0.7.0.jar;E:\TimeTunnel\apps\dfswriter\lib\quartz-all-1.8.3.jar;E:\TimeTunnel\apps\dfswriter\lib\TTClient.jar;E:\TimeTunnel\apps\dfswriter\lib\zookeeper-3.3.2.jar;E:\TimeTunnel\apps\savefile\lib\libthrift-0.5.0-20100116.jar;E:\TimeTunnel\apps\savefile\lib\quartz-all-1.8.3.jar;E:\TimeTunnel\idl\lib\javax.jms_1.1.0.jar;E:\TimeTunnel\idl\lib\jmxri-1.2.1.jar;E:\TimeTunnel\idl\lib\jmxtools-1.2.1.jar;E:\TimeTunnel\idl\lib\libthrift-0.5.0-20100116.jar;E:\TimeTunnel\router\lib\libthrift-0.5.0.jar;E:\TimeTunnel\router\lib\timetunnel2-cluster-0.1.0.jar;E:\TimeTunnel\router\lib\zk-recipes.jar;E:\TimeTunnel\router\lib\zookeeper-3.2.2.jar;E:\eclipselib\junit-4.11.jar;E:\eclipselib\zookeeper-3.3.5.jar;E:\eclipse\plugins\org.apache.log4j_1.2.15.v201012070815.jar
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:java.library.path=C:\Program Files (x86)\Java\jre7\bin;C:\Windows\Sun\Java\bin;C:\Windows\system32;C:\Windows;C:/Program Files (x86)/Java/jre7/bin/client;C:/Program Files (x86)/Java/jre7/bin;C:/Program Files (x86)/Java/jre7/lib/i386;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Windows7Master;E:\Program Files\TortoiseSVN\bin;C:\Program Files (x86)\Java\jdk1.7.0_07/bin;C:\Program Files (x86)\Java\jdk1.7.0_07/jre/bin;D:\apache-ant-1.8.4\bin;$ANT_HOME\bin;$HADOOP_HOME\bin;E:\eclipse;;.
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:java.io.tmpdir=C:\Users\ADMINI~1\AppData\Local\Temp\
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:java.compiler=<NA>
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:os.name=Windows 7
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:os.arch=x86
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:os.version=6.1
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:user.name=Administrator
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:user.home=C:\Users\Administrator
12/12/07 11:31:07 INFO server.ZooKeeperServer: Server environment:user.dir=F:\workspace\timetunnel
12/12/07 11:31:07 INFO server.ZooKeeperServer: tickTime set to 1000
12/12/07 11:31:07 INFO server.ZooKeeperServer: minSessionTimeout set to -1
12/12/07 11:31:07 INFO server.ZooKeeperServer: maxSessionTimeout set to -1
12/12/07 11:31:07 INFO server.NIOServerCnxn: binding to port 0.0.0.0/0.0.0.0:8020
12/12/07 11:31:07 INFO persistence.FileSnap: Reading snapshot d:\version-2\snapshot.0
12/12/07 11:31:07 INFO persistence.FileTxnSnapLog: Snapshotting: 0
```

执行完成后会在d:下面生成version-2文件夹和snapshot.0文件

具体执行过程是：

 1.对new NIOServerCnxn.Factory(config.getClientPortAddress(),config.getMaxClientCnxns());一个绑定端口的

```java
public Factory(InetSocketAddress addr, int maxcc) throws IOException {
            super("NIOServerCxn.Factory:" + addr);
            setDaemon(true);
            maxClientCnxns = maxcc;
            this.ss = ServerSocketChannel.open();
            ss.socket().setReuseAddress(true);
            LOG.info("binding to port " + addr);
            ss.socket().bind(addr);
            ss.configureBlocking(false);
            ss.register(selector, SelectionKey.OP_ACCEPT);
        }
```



2.对cnxnFactory.startup(zkServer);进行分析  跳转到org.apache.zookeeper.server.NIOServerCnxn

```java
public void startup(ZooKeeperServer zks) throws IOException,
                InterruptedException {
            start();//启动线程
            zks.startdata();//创建文件夹和文件
            zks.startup();
            setZooKeeperServer(zks);
        }
```

3.对org.apache.zookeeper.server.NIOServerCnxn.startup(zks)里面的zks.startdata()进行分析 跳转到org.apache.zookeeper.server.ZooKeeperServer

```java
public void startdata() 
    throws IOException, InterruptedException {
        //check to see if zkDb is not null
        if (zkDb == null) {
            zkDb = new ZKDatabase(this.txnLogFactory);
        }  
        if (!zkDb.isInitialized()) {
            loadData();//创建文件夹和文件
        }
    }
```

4.对org.apache.zookeeper.server.ZooKeeperServer.startdata()内的zkDb = new ZKDatabase(this.txnLogFactory)进行分析，跳转到org.apache.zookeeper.server.ZKDatabase

```java
public ZKDatabase(FileTxnSnapLog snapLog) {
        dataTree = new DataTree();
        sessionsWithTimeouts = new ConcurrentHashMap<Long, Integer>();
        this.snapLog = snapLog;
    }
```

5.对org.apache.zookeeper.server.ZooKeeperServer.startdata()内的loadData()进行分析，跳转到相同类的loadData()方法中

```java
/**
     *  Restore sessions and data
     */
    public void loadData() throws IOException, InterruptedException {
        setZxid(zkDb.loadDataBase());
        // Clean up dead sessions
        LinkedList<Long> deadSessions = new LinkedList<Long>();
        for (long session : zkDb.getSessions()) {
          if (zkDb.getSessionWithTimeOuts().get(session) == null) {
                deadSessions.add(session);
            }
        }
        zkDb.setDataTreeInit(true);
        for (long session : deadSessions) {
            // XXX: Is lastProcessedZxid really the best thing to use?
            killSession(session, zkDb.getDataTreeLastProcessedZxid());
        }
        // Make a clean snapshot
        takeSnapshot();//保存日志信息
    }
```

6.对org.apache.zookeeper.server.ZooKeeperServer.loadData()内的takeSnapshot()进行分析，跳转到相同类的takeSnapshot()方法中

```java
public void takeSnapshot(){
        try {
            txnLogFactory.save(zkDb.getDataTree(), zkDb.getSessionWithTimeOuts()); //通过FileTxnSnapLog(文本文件快照日志类)的保存方法保存到相应的文件中，做日志信息的持久化操作
        } catch (IOException e) {
            LOG.fatal("Severe unrecoverable error, exiting", e);
            // This is a severe error that we cannot recover from,
            // so we need to exit
            System.exit(10);
        }
    }
```

7.对org.apache.zookeeper.server.ZooKeeperServer.takeSnapshot()内的txnLogFactory.save(zkDb.getDataTree(), zkDb.getSessionWithTimeOuts())进行分析，跳转到相同类的org.apache.zookeeper.server.persistence.FileTxnSnapLog.save()方法中

```java
public void save(DataTree dataTree,
            ConcurrentHashMap<Long, Integer> sessionsWithTimeouts)
        throws IOException {
        long lastZxid = dataTree.lastProcessedZxid; //默认是0，值是取ProcessTxnResult.TxnHeader.zxid不确定是什么东西
        LOG.info("Snapshotting: " + Long.toHexString(lastZxid));
        File snapshot=new File(
                snapDir, Util.makeSnapshotName(lastZxid)); //snapDir具体取值看8.的解释  snap="d:\\version-2\\"
        snapLog.serialize(dataTree, sessionsWithTimeouts, snapshot); //写数据
        
    }
```

8.snapDir:是通过ZooKeeperServerForTest.test(String[] args,ServerConfig config)【用来测试的方法】中的FileTxnSnapLog ftxn = new FileTxnSnapLog(new File(config.getDataLogDir()), new File(config.getDataDir()));就已经生成的,而其中的dataLogDir=dataDir="d:\\",下面是实现源码org.apache.zookeeper.server.ServerConfig.parse(String[]
 args)，配置config

```java
public void parse(String[] args) {//输入的参数是args[] = {8020,"d:\\",1000}
        if (args.length < 2 || args.length > 4) {
            throw new IllegalArgumentException("Invalid args:"
                    + Arrays.toString(args));
        }

        clientPortAddress = new InetSocketAddress(Integer.parseInt(args[0])); //0.0.0.0/0.0.0.0:8020
        dataDir = args[1];//args[1]="d:\\"
        dataLogDir = dataDir;
        if (args.length == 3) {
            tickTime = Integer.parseInt(args[2]);
        }
        if (args.length == 4) {
            maxClientCnxns = Integer.parseInt(args[3]);
        }
    }
```

下面是org.apache.zookeeper.server.persistence.FileTxnSnapLog(File dataDir, File snapDir)源码，生成snapDir

```java
public FileTxnSnapLog(File dataDir, File snapDir) throws IOException {
        this.dataDir = new File(dataDir, version + VERSION);  //VERSION:public final static int VERSION = 2;
        this.snapDir = new File(snapDir, version + VERSION);  //version:public final static String version = "version-";
        if (!this.dataDir.exists()) {
            if (!this.dataDir.mkdirs()) {
                throw new IOException("Unable to create data directory "
                        + this.dataDir);
            }
        }
        if (!this.snapDir.exists()) {
            if (!this.snapDir.mkdirs()) {
                throw new IOException("Unable to create snap directory "
                        + this.snapDir);
            }
        }
        txnLog = new FileTxnLog(this.dataDir);
        snapLog = new FileSnap(this.snapDir);
    }
```

9.我们现在要分析一下7.中的Util.makeSnapshotName(lastZxid)方法的作用，跳转到org.apache.zookeeper.server.persistence.Util.makeSnapshotName(long zxid)

```java
/**
     * Creates a snapshot file name.
     * 
     * @param zxid used as a suffix
     * @return file name
     */
    public static String makeSnapshotName(long zxid) {
        return "snapshot." + Long.toHexString(zxid);//snapshot.0
    }
```

到这来，文件夹及文件都创建好了




 下面看下数据的生成数的生成就是直接new DataTree()生成了一个

  ''（/）//空字符串或"/"作为根的节点

   ........../zookeeper //一级子节点

             ........../quota           //全路径是   /zookeeper/quota

```
public DataTree() {
        /* Rather than fight it, let root have an alias */
        nodes.put("", root);
        nodes.put(rootZookeeper, root);

        /** add the proc node and quota node */
        root.addChild(procChildZookeeper);
        nodes.put(procZookeeper, procDataNode);

        procDataNode.addChild(quotaChildZookeeper);
        nodes.put(quotaZookeeper, quotaDataNode);
    }
```

生成完了DataTree()的数据后通过  7. 中的  snapLog.serialize(dataTree, sessionsWithTimeouts, snapshot);//写数据



调用到new DataTree()的方法的地方在 3.  zkDb = new ZKDatabase(this.txnLogFactory);

```
public ZKDatabase(FileTxnSnapLog snapLog) {
        dataTree = new DataTree();
        sessionsWithTimeouts = new ConcurrentHashMap<Long, Integer>();
        this.snapLog = snapLog;
    }
```

目前就分析到这来，生成的文件数据内容

![](https://img-my.csdn.net/uploads/201212/07/1354859499_7527.jpg)



