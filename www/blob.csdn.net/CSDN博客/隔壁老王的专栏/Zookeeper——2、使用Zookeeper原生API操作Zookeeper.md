# Zookeeper——2、使用Zookeeper原生API操作Zookeeper - 隔壁老王的专栏 - CSDN博客





2016年12月02日 17:05:43[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：8098








zookeeper的javaclient可以使我们更轻松的实现对zookeeper的各种操作，要使用java操作zookeeper，需要引入zookeeper-3.4.5.jar和zkclient-0.1.jar。zookeeper-3.4.5.jar是官方提供的JAVA API，zkclient-0.1.jar则是在原生API基础上进行扩展的开源Java客户端。（[zookeeper文章所需的jar包](http://download.csdn.net/detail/haoyuyang/9703158)）

客户端可以通过创建一个zookeeper实例来连接zookeeper服务器，Zookeeper(Arguments)方法（一共有4个构造方法，根据参数不同），参数说明如下：

connectString：连接服务器列表，用“,”分割。

sessionTimeout：心跳检测时间周期，单位毫秒。

watcher：事件处理通知器。

canBeReadOnly：标识当前会话是否支持只读。

sessionId和sessionPassword：提供连接zookeeper服务器的sessionId和密码，通过这两个参数确定唯一一台客户端，目的是可以提供重复会话。

**注意：zookeeper客户端和服务器会话的建立是一个异步的过程，也就是说在程序中，程序方法在处理完客户端初始化后立即返回（即程序继续往下执行代码，这样，在大多数情况下并没有真正的构建好一个可用会话，在会话的生命周期处于“CONNECTING”时才算真正的建立完毕，所以需要使用到多线程中的一个工具类CountDownLatch）。**

先看代码，代码中和后面有说明。HelloWorld入门：

```java
public class ZookeeperBase {

	/** zookeeper地址 */
	static final String CONNECT_ADDR = "192.168.80.88:2181,192.168.80.87:2181,192.168.80.86:2181";
	/** session超时时间 */
	static final int SESSION_OUTTIME = 2000;//ms 
	/** 信号量，阻塞程序执行，用于等待zookeeper连接成功，发送成功信号 */
	static final CountDownLatch connectedSemaphore = new CountDownLatch(1);
	
	public static void main(String[] args) throws Exception{
		
		ZooKeeper zk = new ZooKeeper(CONNECT_ADDR, SESSION_OUTTIME, new Watcher(){
			@Override
			public void process(WatchedEvent event) {
				//获取事件的状态
				KeeperState keeperState = event.getState();
				EventType eventType = event.getType();
				//如果是建立连接
				if(KeeperState.SyncConnected == keeperState){
					if(EventType.None == eventType){
						//如果建立连接成功，则发送信号量，让后续阻塞程序向下执行
						connectedSemaphore.countDown();
						System.out.println("zk 建立连接");
					}
				}
			}
		});

		//进行阻塞
		connectedSemaphore.await();
		
		System.out.println("..");
		//创建父节点
//		zk.create("/testRoot", "testRoot".getBytes(), Ids.OPEN_ACL_UNSAFE, CreateMode.PERSISTENT);
		
		//创建子节点，使用EPHEMERAL，主程序执行完成后该节点被删除，只在本次会话内有效，可以用作分布式锁。
//		zk.create("/testRoot/children", "children data".getBytes(), Ids.OPEN_ACL_UNSAFE, CreateMode.EPHEMERAL);
		
		//获取节点信息
//		byte[] data = zk.getData("/testRoot", false, null);
//		System.out.println(new String(data));
//		System.out.println(zk.getChildren("/testRoot", false));
		
		//修改节点的值，-1表示跳过版本检查，其他正数表示如果传入的版本号与当前版本号不一致，则修改不成功，删除是同样的道理。
//		zk.setData("/testRoot", "modify data root".getBytes(), -1);
//		byte[] data = zk.getData("/testRoot", false, null);
//		System.out.println(new String(data));		
		
		//判断节点是否存在
//		System.out.println(zk.exists("/testRoot/children", false));
		//删除节点
//		zk.delete("/testRoot/children", -1);
//		System.out.println(zk.exists("/testRoot/children", false));
		
		zk.close();
		
		
		
	}
	
}
```

说明：

原生的zookeeper API提供了两种创建节点的方法，同步和异步创建节点方式。

**同步方式：**

参数1，节点路径（名称）：/nodeName。不允许递归创建节点，在父节点不存在的情况下，不允许创建子节点。

参数2，节点内容：要求类型是字节数组，也就是说不支持序列话方式，如果需要实现序列化，可使用java相关序列化框架，如Hessian，Kryo。

参数3，节点权限：使用Ids.OPEN_ACL_UNSAFE开放权限即可。

参数4，节点类型：创建节点的类型，CreateMode.*，提供了如下所示的四种节点类型：

①PERSISTENT（持久节点）

②PERSISTENT_SEQUENTIAL（持久顺序节点）

③EPHEMERAL（临时节点，本次会话有效）

④EPHEMERAL_SEQUENTIAL（临时顺序节点，本次会话有效）

异步方式（在同步方法参数的基础上增加两个参数）：

参数5，回调方法：注册一个异步回调方法，要实现AsynCallBack.StringCallBack接口，重写processResult(int rc, String path, Object ctx, String name)方法，当节点创建完成后执行此方法。

①rc：服务端响应码，0表示调用成功、-4表示端口连接、-110表示指定节点存在、-112表示会话已过期。

②path：接口调用时传入的数据节点的路径参数。

③ctx：调用接口传入的ctx值。

④name：实际在服务端创建的节点的名称。

参数6，传递给回调方法的参数，一般为上下文（Context）信息。

![](https://img-blog.csdn.net/20161203194801146)


Watcher、ZK状态、事件类型：

zookeeper有watch事件，是一次性触发的。当watch监视的数据发生变化时，通知在创建zookeeper是设置了Watcher的客户端。Watcher类监视的事件类型和状态类型如下所示：

事件类型（znode节点相关）：

①EventType.NodeCreated：节点创建

②EventType.NodeDataChanged：节点数据变更

③EventType.NodeChildrenChanged：子节点变更

④EventType.NodeDeleted：节点删除

状态类型（客户端实例相关）：

①KeeperState.Disconnected：未连接

②KeeperState.SyncConnected：已连接

③KeeperState.AuthFailed：认证失败

④KeeperState.Expired：会话失效

```java
public class ZooKeeperWatcher implements Watcher {

	/** 定义原子变量 */
	AtomicInteger seq = new AtomicInteger();
	/** 定义session失效时间 */
	private static final int SESSION_TIMEOUT = 10000;
	/** zookeeper服务器地址 */
	private static final String CONNECTION_ADDR = "192.168.80.88:2181";
	/** zk父路径设置 */
	private static final String PARENT_PATH = "/testWatch";
	/** zk子路径设置 */
	private static final String CHILDREN_PATH = "/testWatch/children";
	/** 进入标识 */
	private static final String LOG_PREFIX_OF_MAIN = "【Main】";
	/** zk变量 */
	private ZooKeeper zk = null;
	/** 信号量设置，用于等待zookeeper连接建立之后 通知阻塞程序继续向下执行 */
	private CountDownLatch connectedSemaphore = new CountDownLatch(1);

	/**
	 * 创建ZK连接
	 * @param connectAddr ZK服务器地址列表
	 * @param sessionTimeout Session超时时间
	 */
	public void createConnection(String connectAddr, int sessionTimeout) {
		this.releaseConnection();
		try {
			zk = new ZooKeeper(connectAddr, sessionTimeout, this);
			System.out.println(LOG_PREFIX_OF_MAIN + "开始连接ZK服务器");
			connectedSemaphore.await();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 关闭ZK连接
	 */
	public void releaseConnection() {
		if (this.zk != null) {
			try {
				this.zk.close();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * 创建节点
	 * @param path 节点路径
	 * @param data 数据内容
	 * @return 
	 */
	public boolean createPath(String path, String data) {
		try {
			//设置监控(由于zookeeper的监控都是一次性的所以 每次必须设置监控)
			this.zk.exists(path, true);
			System.out.println(LOG_PREFIX_OF_MAIN + "节点创建成功, Path: " + 
							   this.zk.create(	/**路径*/ 
									   			path, 
									   			/**数据*/
									   			data.getBytes(), 
									   			/**所有可见*/
								   				Ids.OPEN_ACL_UNSAFE, 
								   				/**永久存储*/
								   				CreateMode.PERSISTENT ) + 	
							   ", content: " + data);
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	/**
	 * 读取指定节点数据内容
	 * @param path 节点路径
	 * @return
	 */
	public String readData(String path, boolean needWatch) {
		try {
			return new String(this.zk.getData(path, needWatch, null));
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}
	}

	/**
	 * 更新指定节点数据内容
	 * @param path 节点路径
	 * @param data 数据内容
	 * @return
	 */
	public boolean writeData(String path, String data) {
		try {
			System.out.println(LOG_PREFIX_OF_MAIN + "更新数据成功，path：" + path + ", stat: " +
								this.zk.setData(path, data.getBytes(), -1));
		} catch (Exception e) {
			e.printStackTrace();
		}
		return false;
	}

	/**
	 * 删除指定节点
	 * 
	 * @param path
	 *            节点path
	 */
	public void deleteNode(String path) {
		try {
			this.zk.delete(path, -1);
			System.out.println(LOG_PREFIX_OF_MAIN + "删除节点成功，path：" + path);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 判断指定节点是否存在
	 * @param path 节点路径
	 */
	public Stat exists(String path, boolean needWatch) {
		try {
			return this.zk.exists(path, needWatch);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * 获取子节点
	 * @param path 节点路径
	 */
	private List<String> getChildren(String path, boolean needWatch) {
		try {
			return this.zk.getChildren(path, needWatch);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * 删除所有节点
	 */
	public void deleteAllTestPath() {
		if(this.exists(CHILDREN_PATH, false) != null){
			this.deleteNode(CHILDREN_PATH);
		}
		if(this.exists(PARENT_PATH, false) != null){
			this.deleteNode(PARENT_PATH);
		}		
	}
	
	/**
	 * 收到来自Server的Watcher通知后的处理。
	 */
	@Override
	public void process(WatchedEvent event) {
		
		System.out.println("进入 process 。。。。。event = " + event);
		
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		
		if (event == null) {
			return;
		}
		
		// 连接状态
		KeeperState keeperState = event.getState();
		// 事件类型
		EventType eventType = event.getType();
		// 受影响的path
		String path = event.getPath();
		
		String logPrefix = "【Watcher-" + this.seq.incrementAndGet() + "】";

		System.out.println(logPrefix + "收到Watcher通知");
		System.out.println(logPrefix + "连接状态:\t" + keeperState.toString());
		System.out.println(logPrefix + "事件类型:\t" + eventType.toString());

		if (KeeperState.SyncConnected == keeperState) {
			// 成功连接上ZK服务器
			if (EventType.None == eventType) {
				System.out.println(logPrefix + "成功连接上ZK服务器");
				connectedSemaphore.countDown();
			} 
			//创建节点
			else if (EventType.NodeCreated == eventType) {
				System.out.println(logPrefix + "节点创建");
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				this.exists(path, true);
			} 
			//更新节点
			else if (EventType.NodeDataChanged == eventType) {
				System.out.println(logPrefix + "节点数据更新");
				System.out.println("我看看走不走这里........");
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.out.println(logPrefix + "数据内容: " + this.readData(PARENT_PATH, true));
			} 
			//更新子节点
			else if (EventType.NodeChildrenChanged == eventType) {
				System.out.println(logPrefix + "子节点变更");
				try {
					Thread.sleep(3000);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.out.println(logPrefix + "子节点列表：" + this.getChildren(PARENT_PATH, true));
			} 
			//删除节点
			else if (EventType.NodeDeleted == eventType) {
				System.out.println(logPrefix + "节点 " + path + " 被删除");
			}
			else ;
		} 
		else if (KeeperState.Disconnected == keeperState) {
			System.out.println(logPrefix + "与ZK服务器断开连接");
		} 
		else if (KeeperState.AuthFailed == keeperState) {
			System.out.println(logPrefix + "权限检查失败");
		} 
		else if (KeeperState.Expired == keeperState) {
			System.out.println(logPrefix + "会话失效");
		}
		else ;

		System.out.println("--------------------------------------------");

	}

	/**
	 * <B>方法名称：</B>测试zookeeper监控<BR>
	 * <B>概要说明：</B>主要测试watch功能<BR>
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {

		//建立watcher
		ZooKeeperWatcher zkWatch = new ZooKeeperWatcher();
		//创建连接
		zkWatch.createConnection(CONNECTION_ADDR, SESSION_TIMEOUT);
		//System.out.println(zkWatch.zk.toString());
		
		Thread.sleep(1000);
		
		// 清理节点
		zkWatch.deleteAllTestPath();
		
		if (zkWatch.createPath(PARENT_PATH, System.currentTimeMillis() + "")) {
			
			Thread.sleep(1000);
			
			
			// 读取数据，在操作节点数据之前先调用zookeeper的getData()方法是为了可以watch到对节点的操作。watch是一次性的，
			// 也就是说，如果第二次又重新调用了setData()方法，在此之前需要重新调用一次。
			System.out.println("---------------------- read parent ----------------------------");
			//zkWatch.readData(PARENT_PATH, true);
			
			/** 读取子节点，设置对子节点变化的watch，如果不写该方法，则在创建子节点是只会输出NodeCreated，而不会输出NodeChildrenChanged，
				也就是说创建子节点时没有watch。
				如果是递归的创建子节点，如path="/p/c1/c2"的话，getChildren(PARENT_PATH, ture)只会在创建c1时watch，输出c1的NodeChildrenChanged，
				而不会输出创建c2时的NodeChildrenChanged，如果watch到c2的NodeChildrenChanged，则需要再调用一次getChildren(String path, true)方法，
				其中path="/p/c1"
			*/
			System.out.println("---------------------- read children path ----------------------------");
			zkWatch.getChildren(PARENT_PATH, true);

			// 更新数据
			zkWatch.writeData(PARENT_PATH, System.currentTimeMillis() + "");
			
			Thread.sleep(1000);
			
			// 创建子节点，同理如果想要watch到NodeChildrenChanged状态，需要调用getChildren(CHILDREN_PATH, true)
			zkWatch.readData(CHILDREN_PATH, true);
			zkWatch.createPath(CHILDREN_PATH, System.currentTimeMillis() + "");
			
			Thread.sleep(1000);
			
			zkWatch.writeData(CHILDREN_PATH, System.currentTimeMillis() + "");
		}
		
		Thread.sleep(50000);
		// 清理节点
		zkWatch.deleteAllTestPath();
		Thread.sleep(1000);
		zkWatch.releaseConnection();
	}

}
```

**Watcher的特性：一次性、客户端串行执行、轻量。**

一次性：对于ZK的Watcher，只需要记住一点：Zookeeper的watch事件是一次性触发的。当watch监视的数据发生变化时，通知设置了该watch的客户端，即watcher。由于zookeeper的监视都是一次性的，所以每次必须设置监控。

客户端串行执行：客户端Watcher回调的过程是一个串行同步的过程，这为我们保证了顺序，同时需要注意一点，千万不要因为一个Watcher的处理逻辑影响了这个客户端的Watcher回调。

轻量：WatchedEvent是Zookeeper整个Wacher通知机制的最小通知单元，整个数据结构只包含三部分：通知状态、事件类型和节点路径。也就是说Watcher通知非常的简单，只会告诉客户端发生了事件而不会告知其具体内容，需要客户端自己去获取，比如NodeDataChanged事件，Zookeeper只会通知客户端指定节点的数据发生了变更，而不会直接提供具体的数据内容。

**Zookeeper的ACL（AUTH）：**

ACL（Access Control List），Zookeeper作为一个分布式协调框架，其内部存储的都是一些关乎分布式系统运行时状态的元数据，尤其是涉及到一些分布式锁、Master选举和协调等应用场景。我们需要有效的保障Zookeeper中的数据安全，Zookeeper提供了一套完善的ACL权限控制机制来保障数据的安全。

Zookeeper提供了三种模式，权限模式、授权对象、权限：

**权限模式：**Scheme，开发人员经常使用如下四种权限模式：

①IP：ip模式通过ip地址粒度来进行权限控制，例如配置了：ip:192.168.1.107，即表示权限控制都是针对这个ip地址的，同时也支持按网段分配，比如：192.168.1.*。

②Digest：digest是最常用的权限控制模式，也更符合对权限的认知。其类似于“username:password”形式的权限控制标识进行权限配置。Zookeeper会对形成的权限标识先后进行两次编码处理，分别是SHA-1加密算法和BASE64编码。

③World：World是一种最开放的权限控制模式。这种模式可以看做为特殊的digest，它仅仅是一个标识而已。

④Super：超级用户模式。在超级用户模式下可以对Zookeeper进行任意操作。

**权限对象：**指的是权限赋予给用户或者一个指定的实体，例如IP地址或机器等。在不同的模式下，授权对象是不同的。这种模式和授权对象一一对应。

**权限：**权限就是指那些通过权限检测后可以被允许执行的操作，在Zookeeper中，对数据的操作权限分为以下五大类：

**CREATE、DELETE、READ、WRITE、ADMIN**

```java
public class ZookeeperAuth implements Watcher {

	/** 连接地址 */
	final static String CONNECT_ADDR = "192.168.80.88:2181";
	/** 测试路径 */
	final static String PATH = "/testAuth";
	final static String PATH_DEL = "/testAuth/delNode";
	/** 认证类型 */
	final static String authentication_type = "digest";
	/** 认证正确方法 */
	final static String correctAuthentication = "123456";
	/** 认证错误方法 */
	final static String badAuthentication = "654321";
	
	static ZooKeeper zk = null;
	/** 计时器 */
	AtomicInteger seq = new AtomicInteger();
	/** 标识 */
	private static final String LOG_PREFIX_OF_MAIN = "【Main】";
	
	private CountDownLatch connectedSemaphore = new CountDownLatch(1);
	
	@Override
	public void process(WatchedEvent event) {
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		if (event==null) {
			return;
		}
		// 连接状态
		KeeperState keeperState = event.getState();
		// 事件类型
		EventType eventType = event.getType();
		// 受影响的path
		String path = event.getPath();
		
		String logPrefix = "【Watcher-" + this.seq.incrementAndGet() + "】";

		System.out.println(logPrefix + "收到Watcher通知");
		System.out.println(logPrefix + "连接状态:\t" + keeperState.toString());
		System.out.println(logPrefix + "事件类型:\t" + eventType.toString());
		if (KeeperState.SyncConnected == keeperState) {
			// 成功连接上ZK服务器
			if (EventType.None == eventType) {
				System.out.println(logPrefix + "成功连接上ZK服务器");
				connectedSemaphore.countDown();
			} 
		} else if (KeeperState.Disconnected == keeperState) {
			System.out.println(logPrefix + "与ZK服务器断开连接");
		} else if (KeeperState.AuthFailed == keeperState) {
			System.out.println(logPrefix + "权限检查失败");
		} else if (KeeperState.Expired == keeperState) {
			System.out.println(logPrefix + "会话失效");
		}
		System.out.println("--------------------------------------------");
	}
	/**
	 * 创建ZK连接
	 * 
	 * @param connectString
	 *            ZK服务器地址列表
	 * @param sessionTimeout
	 *            Session超时时间
	 */
	public void createConnection(String connectString, int sessionTimeout) {
		this.releaseConnection();
		try {
			zk = new ZooKeeper(connectString, sessionTimeout, this);
			//添加节点授权
			zk.addAuthInfo(authentication_type,correctAuthentication.getBytes());
			System.out.println(LOG_PREFIX_OF_MAIN + "开始连接ZK服务器");
			//倒数等待
			connectedSemaphore.await();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * 关闭ZK连接
	 */
	public void releaseConnection() {
		if (this.zk!=null) {
			try {
				this.zk.close();
			} catch (InterruptedException e) {
			}
		}
	}
	
	/**
	 * 
	 * <B>方法名称：</B>测试函数<BR>
	 * <B>概要说明：</B>测试认证<BR>
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		
		ZookeeperAuth testAuth = new ZookeeperAuth();
		testAuth.createConnection(CONNECT_ADDR,2000);
		List<ACL> acls = new ArrayList<ACL>(1);
		for (ACL ids_acl : Ids.CREATOR_ALL_ACL) {
			acls.add(ids_acl);
		}

		try {
			zk.create(PATH, "init content".getBytes(), acls, CreateMode.PERSISTENT);
			System.out.println("使用授权key：" + correctAuthentication + "创建节点："+ PATH + ", 初始内容是: init content");
		} catch (Exception e) {
			e.printStackTrace();
		}
		try {
			zk.create(PATH_DEL, "will be deleted! ".getBytes(), acls, CreateMode.PERSISTENT);
			System.out.println("使用授权key：" + correctAuthentication + "创建节点："+ PATH_DEL + ", 初始内容是: init content");
		} catch (Exception e) {
			e.printStackTrace();
		}

		// 获取数据
		getDataByNoAuthentication();
		getDataByBadAuthentication();
		getDataByCorrectAuthentication();

		// 更新数据
		updateDataByNoAuthentication();
		updateDataByBadAuthentication();
		updateDataByCorrectAuthentication();

		// 删除数据
		deleteNodeByBadAuthentication();
		deleteNodeByNoAuthentication();
		deleteNodeByCorrectAuthentication();
		//
		Thread.sleep(1000);
		
		deleteParent();
		//释放连接
		testAuth.releaseConnection();
	}
	/** 获取数据：采用错误的密码 */
	static void getDataByBadAuthentication() {
		String prefix = "[使用错误的授权信息]";
		try {
			ZooKeeper badzk = new ZooKeeper(CONNECT_ADDR, 2000, null);
			//授权
			badzk.addAuthInfo(authentication_type,badAuthentication.getBytes());
			Thread.sleep(2000);
			System.out.println(prefix + "获取数据：" + PATH);
			System.out.println(prefix + "成功获取数据：" + badzk.getData(PATH, false, null));
		} catch (Exception e) {
			System.err.println(prefix + "获取数据失败，原因：" + e.getMessage());
		}
	}

	/** 获取数据：不采用密码 */
	static void getDataByNoAuthentication() {
		String prefix = "[不使用任何授权信息]";
		try {
			System.out.println(prefix + "获取数据：" + PATH);
			ZooKeeper nozk = new ZooKeeper(CONNECT_ADDR, 2000, null);
			Thread.sleep(2000);
			System.out.println(prefix + "成功获取数据：" + nozk.getData(PATH, false, null));
		} catch (Exception e) {
			System.err.println(prefix + "获取数据失败，原因：" + e.getMessage());
		}
	}

	/** 采用正确的密码 */
	static void getDataByCorrectAuthentication() {
		String prefix = "[使用正确的授权信息]";
		try {
			System.out.println(prefix + "获取数据：" + PATH);
			
			System.out.println(prefix + "成功获取数据：" + zk.getData(PATH, false, null));
		} catch (Exception e) {
			System.out.println(prefix + "获取数据失败，原因：" + e.getMessage());
		}
	}

	/**
	 * 更新数据：不采用密码
	 */
	static void updateDataByNoAuthentication() {

		String prefix = "[不使用任何授权信息]";

		System.out.println(prefix + "更新数据： " + PATH);
		try {
			ZooKeeper nozk = new ZooKeeper(CONNECT_ADDR, 2000, null);
			Thread.sleep(2000);
			Stat stat = nozk.exists(PATH, false);
			if (stat!=null) {
				nozk.setData(PATH, prefix.getBytes(), -1);
				System.out.println(prefix + "更新成功");
			}
		} catch (Exception e) {
			System.err.println(prefix + "更新失败，原因是：" + e.getMessage());
		}
	}

	/**
	 * 更新数据：采用错误的密码
	 */
	static void updateDataByBadAuthentication() {

		String prefix = "[使用错误的授权信息]";

		System.out.println(prefix + "更新数据：" + PATH);
		try {
			ZooKeeper badzk = new ZooKeeper(CONNECT_ADDR, 2000, null);
			//授权
			badzk.addAuthInfo(authentication_type,badAuthentication.getBytes());
			Thread.sleep(2000);
			Stat stat = badzk.exists(PATH, false);
			if (stat!=null) {
				badzk.setData(PATH, prefix.getBytes(), -1);
				System.out.println(prefix + "更新成功");
			}
		} catch (Exception e) {
			System.err.println(prefix + "更新失败，原因是：" + e.getMessage());
		}
	}

	/**
	 * 更新数据：采用正确的密码
	 */
	static void updateDataByCorrectAuthentication() {

		String prefix = "[使用正确的授权信息]";

		System.out.println(prefix + "更新数据：" + PATH);
		try {
			Stat stat = zk.exists(PATH, false);
			if (stat!=null) {
				zk.setData(PATH, prefix.getBytes(), -1);
				System.out.println(prefix + "更新成功");
			}
		} catch (Exception e) {
			System.err.println(prefix + "更新失败，原因是：" + e.getMessage());
		}
	}

	/**
	 * 不使用密码 删除节点
	 */
	static void deleteNodeByNoAuthentication() throws Exception {

		String prefix = "[不使用任何授权信息]";

		try {
			System.out.println(prefix + "删除节点：" + PATH_DEL);
			ZooKeeper nozk = new ZooKeeper(CONNECT_ADDR, 2000, null);
			Thread.sleep(2000);
			Stat stat = nozk.exists(PATH_DEL, false);
			if (stat!=null) {
				nozk.delete(PATH_DEL,-1);
				System.out.println(prefix + "删除成功");
			}
		} catch (Exception e) {
			System.err.println(prefix + "删除失败，原因是：" + e.getMessage());
		}
	}

	/**
	 * 采用错误的密码删除节点
	 */
	static void deleteNodeByBadAuthentication() throws Exception {

		String prefix = "[使用错误的授权信息]";

		try {
			System.out.println(prefix + "删除节点：" + PATH_DEL);
			ZooKeeper badzk = new ZooKeeper(CONNECT_ADDR, 2000, null);
			//授权
			badzk.addAuthInfo(authentication_type,badAuthentication.getBytes());
			Thread.sleep(2000);
			Stat stat = badzk.exists(PATH_DEL, false);
			if (stat!=null) {
				badzk.delete(PATH_DEL, -1);
				System.out.println(prefix + "删除成功");
			}
		} catch (Exception e) {
			System.err.println(prefix + "删除失败，原因是：" + e.getMessage());
		}
	}

	/**
	 * 使用正确的密码删除节点
	 */
	static void deleteNodeByCorrectAuthentication() throws Exception {

		String prefix = "[使用正确的授权信息]";

		try {
			System.out.println(prefix + "删除节点：" + PATH_DEL);
			Stat stat = zk.exists(PATH_DEL, false);
			if (stat!=null) {
				zk.delete(PATH_DEL, -1);
				System.out.println(prefix + "删除成功");
			}
		} catch (Exception e) {
			System.out.println(prefix + "删除失败，原因是：" + e.getMessage());
		}
	}

	/**
	 * 使用正确的密码删除节点
	 */
	static void deleteParent() throws Exception {
		try {
			Stat stat = zk.exists(PATH_DEL, false);
			if (stat == null) {
				zk.delete(PATH, -1);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
```















