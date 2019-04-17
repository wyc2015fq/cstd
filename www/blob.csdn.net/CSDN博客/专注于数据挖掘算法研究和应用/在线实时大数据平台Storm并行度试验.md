# 在线实时大数据平台Storm并行度试验 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月10日 11:17:58[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3618
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









集群模式试验：同一文件输入数据如何处理，数据变量共享

1）集群模式一个worker内一个spout一个Bolt

jps:1个worker

storm list:1个wokers，4个tasks

2）集群模式一个worker内一个spout 两个Bolt

jps:1个worker

storm list:1个wokers，6个tasks

不同bolt线程之间对变量counter是互斥读写的。试验证明多bolt可以同时对同一变量进行操作。

3）集群模式一个worker内两个spout两个Bolt

jps:1个worker

storm list:1个wokers，7个tasks

试验证明多spout对同一文件输入源会重复处理数据，spout线程间对输入源不会互斥读取。

基于以上三个试验，说明在同一进程内（worker），spout线程间对输入不能互斥（会重复处理数据，只能每个spout提供不同输入源），bolt线程间对变量是互斥的。可以理解，进程内部对多线程共享变量是有互斥控制，但对外部数据是不控制（spout是获取外部数据的）。

4）集群模式两个worker内一个spout一个Bolt

jps:2个worker

storm list:2个wokers，5个tasks

这个试验意义不大，主要是观察worker和task数。只有一个spout不会重复处理数据。

5）集群模式两个worker内一个spout两个Bolt

jps:2个worker

storm list:2个wokers，7个tasks

这个试验意义不大，主要是观察worker和task数。只有一个spout不会重复处理数据，多bolt间可以互斥访问变量。

6）集群模式两个worker内两个spout两个Bolt

jps:2个worker

storm list:2个wokers，8个tasks

多spout会重复读取同一输入源的数据。跨进程不能共享变量。

通过上面试验可以得出：

1）进程间（worker）是不能共享互斥访问变量；

2）线程间（spout）是不能共享互斥读取同一文件；

3）线程间（bolt）是可以共享互斥访问变量；

![](https://img-blog.csdn.net/20160810111211485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




通过上面这个图，更好理解：

1）多个spout要提供不同输入源，同一文件会重复处理；

2）多个bolt间可以汇聚统计不同spout发射过来的同主题数据；

通过上面试验，实际上，对spout/bolt框架还是不能够全面了解，那些代码是storm框架控制，那些是自己控制，搞明白这个，就是在变量定义以及数据是否存储到磁盘来共享设计topology。

代码如下：



```java
package cn.wc;

import org.apache.storm.Config;
import org.apache.storm.LocalCluster;
import org.apache.storm.StormSubmitter;
import org.apache.storm.generated.AlreadyAliveException;
import org.apache.storm.generated.AuthorizationException;
import org.apache.storm.generated.InvalidTopologyException;
import org.apache.storm.topology.TopologyBuilder;
import org.apache.storm.tuple.Fields;

public class TopologyMain {
	public static void main(String[] args) throws InterruptedException {
         
		//Configuration  
        Config conf = new Config();  
        conf.setNumWorkers(1);//设置2个进程  
        conf.put("inpath", args[0]); //输入文件路径
        conf.put("outpath", args[1]); //输出结果路径
          
        //Topology definition  
        TopologyBuilder builder = new TopologyBuilder();  
        builder.setSpout("word-reader",new WordReader(),2);  
        builder.setBolt("word-normalizer",new WordNormalizer(),2).shuffleGrouping("word-reader");  
        builder.setBolt("word-counter", new WordCounter(),2).fieldsGrouping("word-normalizer", new Fields("word"));  
      
        //集群模式  
        try {  
        	//storm jar /mnt/wc.jar cn.wc.TopologyMain /tmp/topoin.txt /tmp/topoout.log
            StormSubmitter.submitTopology("topoword", conf, builder.createTopology());  
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
	}
}
```

```java
package cn.wc;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Map;

import org.apache.storm.spout.SpoutOutputCollector;
import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseRichSpout;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Values;

//Spout作为数据源，它实现了IRichSpout接口，功能是读取一个文本文件并把它的每一行内容发送给bolt。
public class WordReader extends BaseRichSpout {

	private SpoutOutputCollector collector;
	private FileReader fileReader;
	private boolean completed = false;
	boolean ass=false;
	
	public void ack(Object msgId) {
		System.out.println("OK:"+msgId);
	}
	public void close() {}
	
	public void fail(Object msgId) {
		System.out.println("FAIL:"+msgId);
	}

	/**
	 * The only thing that the methods will do It is emit each 
	 * file line
	 * spout最主要的方法，读取文本文件，并把它的每一行发射出去（给bolt） 
     * 这个方法会不断被调用，为了降低它对CPU的消耗，当任务完成时让它sleep一下 
	 */
	public void nextTuple() {
		/**
		 * The nextuple it is called forever, so if we have been readed the file
		 * we will wait and then return
		 */
		if(completed){
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				//Do nothing
			}
			return;
		}
		String str;
		//Open the reader
		BufferedReader reader = new BufferedReader(fileReader);
		try{
			//Read all lines
			while((str = reader.readLine()) != null){
				/**
				 * By each line emmit a new value with the line as a their
				 * 发射每一行，Values是一个ArrayList的实现 
				 */
				if(str=="a" && ass) return;//如果对行值为a已经处理，就返回
				if(str=="a" && !ass) {//用于判断跨进程是否可以共享变量
					ass = true;
					this.collector.emit(new Values(str),str);			
				}else
					this.collector.emit(new Values(str),str);
			}
		}catch(Exception e){
			throw new RuntimeException("Error reading tuple",e);
		}finally{
			completed = true;
		}
	}

	/**
	 * We will create the file and get the collector object
	 * 三个参数，第一个是创建Topology时的配置，第二个是所有的Topology数据，第三个是用来把Spout的数据发射给bolt 
     *  
	 */
	public void open(Map conf, TopologyContext context,SpoutOutputCollector collector) {
		try {
			//获取创建Topology时指定的要读取的文件路径  
			this.fileReader = new FileReader(conf.get("inpath").toString());
		} catch (FileNotFoundException e) {
			throw new RuntimeException("Error reading file ["+conf.get("inpath")+"]");
		}
		//初始化发射器
		this.collector = collector;
	}

	/**
	 * Declare the output field "line"
	 */
	public void declareOutputFields(OutputFieldsDeclarer declarer) {
		declarer.declare(new Fields("line"));
	}
}
```

```java
package cn.wc;

import org.apache.storm.topology.BasicOutputCollector;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseBasicBolt;
import org.apache.storm.tuple.Fields;
import org.apache.storm.tuple.Tuple;
import org.apache.storm.tuple.Values;

//Spout已经成功读取文件并把每一行作为一个tuple(在Storm数据以tuple的形式传递)发射过来，我们这里需要创建两个bolt分别来负责解析每一行和对单词计数。
//Bolt中最重要的是execute方法，每当一个tuple传过来时它便会被调用。
public class WordNormalizer extends BaseBasicBolt {

	public void cleanup() {}

	/**
	 * The bolt will receive the line from the
	 * words file and process it to Normalize this line
	 * 
	 * The normalize will be put the words in lower case
	 * and split the line to get all words in this 
	 * bolt中最重要的方法，每当接收到一个tuple时，此方法便被调用 
     * 这个方法的作用就是把文本文件中的每一行切分成一个个单词，并把这些单词发射出去（给下一个bolt处理） 
	 */
	public void execute(Tuple input, BasicOutputCollector collector) {
        String sentence = input.getString(0);
        String[] words = sentence.split(" ");
        for(String word : words){
            word = word.trim();
            if(!word.isEmpty()){
                word = word.toLowerCase();
                collector.emit(new Values(word));
            }
        }
	}
	

	/**
	 * The bolt will only emit the field "word" 
	 */
	public void declareOutputFields(OutputFieldsDeclarer declarer) {
		declarer.declare(new Fields("word"));
	}
}
```

```java
package cn.wc;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import org.apache.storm.task.TopologyContext;
import org.apache.storm.topology.BasicOutputCollector;
import org.apache.storm.topology.OutputFieldsDeclarer;
import org.apache.storm.topology.base.BaseBasicBolt;
import org.apache.storm.tuple.Tuple;

public class WordCounter extends BaseBasicBolt {

	Integer id;
	String name;
	Map<String, Integer> counters;
	String outpath=null;

	/**
	 * At the end of the spout (when the cluster is shutdown
	 * We will show the word counters
	 * Topology执行完毕的清理工作，比如关闭连接、释放资源等操作都会写在这里 
	 */
	@Override
	public void cleanup() {
		/*System.out.println("-- Word Counter ["+name+"-"+id+"] --");
		for(Map.Entry<String, Integer> entry : counters.entrySet()){
			System.out.println(entry.getKey()+": "+entry.getValue());
		}*/
	}

	/**
	 * On create 
	 */
	@Override
	public void prepare(Map stormConf, TopologyContext context) {
		this.counters = new HashMap<String, Integer>();
		this.name = context.getThisComponentId();
		this.id = context.getThisTaskId();
		outpath=stormConf.get("outpath").toString();
	}

	@Override
	public void declareOutputFields(OutputFieldsDeclarer declarer) {}


	@Override
	public void execute(Tuple input, BasicOutputCollector collector) {
		String str = input.getString(0);
		Integer c=1;
		/**
		 * If the word dosn't exist in the map we will create
		 * this, if not We will add 1 
		 */
		if(!counters.containsKey(str)){
			counters.put(str, 1);
		}else{
			c = counters.get(str) + 1;
			counters.put(str, c);
		}
		//写入文件
		try{
			File file=new File(outpath);
	        if(!file.exists())
	            file.createNewFile();
	        
	        FileOutputStream out=new FileOutputStream(file,true);   
	        StringBuffer sb=new StringBuffer();
	        sb.append(str+": "+c);
	        sb.append("\r\n");
	        out.write(sb.toString().getBytes("utf-8"));        
		}catch (IOException e){e.printStackTrace();}
		      
	}
}
```








