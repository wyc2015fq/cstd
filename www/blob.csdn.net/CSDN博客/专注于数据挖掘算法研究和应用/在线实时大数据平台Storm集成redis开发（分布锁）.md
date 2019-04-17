# 在线实时大数据平台Storm集成redis开发（分布锁） - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月25日 13:06:31[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3628
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、需求场景：spout从ftp列表中拿到未读取的文件读取并发射行到Bolt，bolt进行业务处理后提交下一Bolt入库。用redis主要是：保存文件列表对象，使用分布锁来同步互斥访问共享对象，使文件处理不重复。




2、topo主函数代码：



```java
package ct.topo;


import org.apache.storm.Config;
import org.apache.storm.LocalCluster;
import org.apache.storm.StormSubmitter;
import org.apache.storm.generated.AlreadyAliveException;
import org.apache.storm.generated.AuthorizationException;
import org.apache.storm.generated.InvalidTopologyException;
import org.apache.storm.topology.TopologyBuilder;
import org.apache.storm.tuple.Fields;

import ct.tool.ComUtil;

public class TopoMain {

	public static void main(String[] args) throws InterruptedException {
        
		//Configuration  
        Config conf = new Config();  
        //conf.setMaxSpoutPending(2); //缓存tuple   
        //conf.setMessageTimeoutSecs(5);  //  消息处理延时
        //conf.setNumAckers(2);           //  消息处理acker
        conf.setNumWorkers(3);//设置个进程  
        //提取参数
        conf.put("city", args[0]); //地址，如深圳输入sz
        conf.put("date", args[1]); //日期，如20160808
        //ftp服务器字符串格式：IP|port|username|password
		String strFtpSrv="127.0.0.1|21|name|pwd";
		conf.put("FtpSrv", strFtpSrv);
          
        //Topology definition  
        TopologyBuilder builder = new TopologyBuilder();  
        builder.setSpout("FTPReader",new FTPReader(),3);  
        //根据IMSI字段汇聚到同一task执行
        builder.setBolt("IMSICounter",new IMSICounter(),300).fieldsGrouping("FTPReader", new Fields("imsi"));
        builder.setBolt("DBWriter", new DBWriter(),50).shuffleGrouping("IMSICounter");  
      
        //集群生产模式  
        try {  
        	//storm jar /mnt/dis.jar ct.topo.TopoMain sz 20160825 > /data/storm/log/debug.log
            StormSubmitter.submitTopology("O2OTopo", conf, builder.createTopology());  
        } catch (AlreadyAliveException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        } catch (InvalidTopologyException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        } catch (AuthorizationException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        }  
        //本地调试模式 
        //storm jar /mnt/dis.jar ct.topo.TopoMain sz 20160825 > /data/storm/log/debug.log &
        //conf.setMaxTaskParallelism(1);    
		//LocalCluster cluster = new LocalCluster();    
		//cluster.submitTopology("O2OTopo", conf, builder.createTopology()); 
	}
}
```
3、spout代码：



```java
package ct.topo;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.storm.spout.SpoutOutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichSpout;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Values;
import org.apache.storm.utils.Utils;

import ct.tool.ComUtil;
import ct.tool.FTPHandle;
import ct.tool.RedisDisLock;
import redis.clients.jedis.Jedis;

public class FTPReader extends BaseRichSpout{
	
	private SpoutOutputCollector collector;
	private String FtpSrv=null;
	private String FtpCD=null;
	private Jedis jd;
	
	public void ack(Object msgId) {//成功处理tuple
		//ComUtil.writeLogs("OK:"+msgId);
	}
	public void close() {
		this.jd.close();
	}
	
	public void fail(Object msgId) {//失败处理tuple
		ComUtil.writeLogs("FTPReader.fail:"+msgId+"emit fail!");
	}
	/**
	 * We will create the file and get the collector object
	 * 三个参数，第一个是创建Topology时的配置，第二个是所有的Topology数据，第三个是用来把Spout的数据发射给bolt 
     *  
	 */
	public void open(Map conf, TopologyContext context,SpoutOutputCollector collector) {	
		this.jd = new Jedis("127.0.0.1",6379); //jedis长连接
		//初始化发射器
		this.collector = collector;
		//获取ftp服务器参数和文件读取路径
		FtpSrv=conf.get("FtpSrv").toString();
		String city=conf.get("city").toString();
		String date=conf.get("date").toString();
		FtpCD="/"+city+"/"+date;//文件目录
	}

	/**
	 * ftp下载文件并解压，去除每一行发射
	 * 下载的文件需要过滤已下载
	 */
	public void nextTuple() {	
		//Utils.sleep(200);
		String fileName=null;
		try{//从redis的FtpFileMap对象中获取未读取的文件名
	    	//业务处理，获取一份未读取的文件名字列表	    	
	    	Iterator<String> iter=jd.hkeys("FtpFileMap").iterator(); 
			while (iter.hasNext()){ 
				String key = iter.next(); 
				List<String> list=jd.hmget("FtpFileMap",key);
	            String value=list.get(0);
	            if(value.equals("n")) {
	            	fileName=key;//如果文件未读取，则提取       	
	            	//非阻塞拿锁，nexttuple不能阻塞，本身就是循环，没拿到锁直接返回
	            	if (RedisDisLock.acquireLockNonBlocking(jd,"FtpFileLock")){
	    	    		return;
	    	    	}            	
	            	jd.hset("FtpFileMap", key,"y");//设置为已读，原子性更新操作
	            	//释放锁
	    			RedisDisLock.releaseLock(jd, "FtpFileLock");
	            	break;
	            }
			}	
			jd.close();	
		}catch(Exception e){
			ComUtil.writeLogs("FTPReader.nextTuple:"+e.getMessage());
		}
		try{//对未读取的文件列表下载并解压、读取并发射
			if(fileName!=null){
				String localFileName=FTPHandle.FtpFileDownLoad(FtpSrv,FtpCD,fileName);
				BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(localFileName)));
				String line=null;
				List<String> baselist=jd.lrange("BaseList",0,-1);//返回所有元素
				while((line = reader.readLine()) != null){
					if (line != null){
						String[] fields=line.split("\\|",-1);
						if(fields.length>=12){
							String imsi=fields[4];
							String userno=fields[5];
							String lac=fields[9];
							String ci=fields[10];
							String starttime=fields[11];
							this.collector.emit(new Values(imsi,userno,lac,ci,starttime),imsi);			    	
					    		}
					    	}
						}
					}
				}
				reader.close();
				ComUtil.writeLogs("FTPReader.nextTuple:"+localFileName+":File has been processed successfully!");
			}		
		}catch(Exception e){
			ComUtil.writeLogs("FTPReader.nextTuple:"+e.getMessage());
		}
	}

	
	/**
	 * Declare the output field 
	 */
	public void declareOutputFields(OutputFieldsDeclarer declarer) {
		String[] fields=new String[]{"imsi","userno","lac","ci","starttime"};
		declarer.declare(new Fields(fields));
	}
}
```


4、Bolt代码的业务逻辑就不体现；

5、总结：集成redis，主要是保存共享的对象，然后在对象访问时使用分布锁来互斥操作，锁内是原子性操作，nexttuple要使用非阻塞锁。但加锁会影响性能，在调试过程中出现部分记录发射失败且spout/bolt任务停止情况，需要进一步优化。

      本文提供了一种解决思路，就是集成reids，应用其分布锁，解决多spout重复处理输入文件的情况。









