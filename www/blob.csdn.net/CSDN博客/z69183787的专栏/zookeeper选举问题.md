# zookeeper选举问题 - z69183787的专栏 - CSDN博客
2017年01月26日 11:57:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：486
两种选举制度
1.持久-直到节点挂掉
2.轮留选举
这里我使用的是持久的方式
public class ZKConnectionUtil {
    private  static CuratorFramework client ;
    private static Object lock = new Object();
    private static LeaderLatch leaderLatch;
    private static Logger logger = LoggerFactory.getLogger(ZKConnectionUtil.class);
    /**
     * @描述：是否是leader
     * @return
     * @return boolean
     * @exception
     * @createTime：2016年11月22日
     * @author: songqinghu
     */
    public static boolean isLeader(CuratorFramework client){
        return leaderLatch.hasLeadership();
    }
//            solr.zk.check.namespace=contentcheck
//            solr.zk.check.flag.path=/flag
    /**
     * 
     * @描述：获取单例的zookeeper连接
     * @return void
     * @exception
     * @createTime：2016年8月24日
     * @author: songqinghu
     */
    public static CuratorFramework getZKConnection(){
        if(client ==null){
            synchronized (lock) {
                if(client ==null){
                    int connectionTimeoutMs=5000;
                    String connectString=PropertiesUtil.getProProperties("solr.zk.url","");
                    String namespace=PropertiesUtil.getProProperties("solr.zk.porsche.namespace","");
                    //String connectString="10.125.2.48:3181";
                    //String namespace="contentcheck";
                    //byte[] defaultData=IpUtil.getInNetworkIp().getBytes();
                    RetryPolicy retryPolicy= new RetryNTimes(Integer.MAX_VALUE, 1000);
                    client = CuratorFrameworkFactory.
                            builder().
//        aclProvider(aclProvider).
//        authorization(scheme, auth).
                            connectionTimeoutMs(connectionTimeoutMs).
                            connectString(connectString).
                          //  defaultData(defaultData).
                            namespace(namespace).
                            retryPolicy(retryPolicy).
                            build();
                    client.start();
                    //选举
                    String leaderPath = PropertiesUtil.getProProperties("solr.zk.porsche.leader", "");
                    leaderLatch = new LeaderLatch(client, leaderPath);
                    try {
                        leaderLatch.start();
                    } catch (Exception e) {
                        logger.error("get leaderlatch occor error and error is : " + e);    
                    }  
                }
            }
        }
        return client;
    }
}
原理解析:
在指定节点下每台机器会创建临时节点,机器挂掉,节点会被删除,就会重新选举
