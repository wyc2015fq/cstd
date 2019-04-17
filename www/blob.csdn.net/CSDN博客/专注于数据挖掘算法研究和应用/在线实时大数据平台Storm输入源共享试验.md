# 在线实时大数据平台Storm输入源共享试验 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月09日 17:03:18[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3985
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、背景：topology程序提交集群模式运行试验，验证在同一文件输入源情况下，worker之间是否会重复输入处理，以及数据变量能否在不同worker之间共享，如果文件新增数据，topology会不会获取最新数据处理。

2、试验代码：



```java
package cn.wc;

import org.apache.storm.Config;
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
		conf.setNumWorkers(3);//设置3个进程
		conf.put("wordsFile", args[0]);
		conf.put("output", args[1]);
		
        //Topology definition
		TopologyBuilder builder = new TopologyBuilder();
		builder.setSpout("word-reader",new WordReader(),3);
		builder.setBolt("word-normalizer",new WordNormalizer(),3).setNumTasks(6).shuffleGrouping("word-reader");
		builder.setBolt("word-counter", new WordCounter(),3).fieldsGrouping("word-normalizer", new Fields("word"));
	
		//集群模式
		try {
			StormSubmitter.submitTopology("Getting-Started-Toplogie", conf, builder.createTopology());
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
			this.fileReader = new FileReader(conf.get("wordsFile").toString());
		} catch (FileNotFoundException e) {
			throw new RuntimeException("Error reading file ["+conf.get("wordFile")+"]");
		}
		//初始化发射器
		this.collector = collector;
	}

	/**
	 * Declare the output field "word"
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
	String output=null;

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
		output=stormConf.get("output").toString();
	}

	@Override
	public void declareOutputFields(OutputFieldsDeclarer declarer) {}


	@Override
	public void execute(Tuple input, BasicOutputCollector collector) {
		String str = input.getString(0);
		/**
		 * If the word dosn't exist in the map we will create
		 * this, if not We will add 1 
		 */
		if(!counters.containsKey(str)){
			counters.put(str, 1);
		}else{
			Integer c = counters.get(str) + 1;
			counters.put(str, c);
		}
		//写入文件
		try{
			File file=new File(output);
	        if(!file.exists())
	            file.createNewFile();
	        FileOutputStream out=new FileOutputStream(file,true);        
	        for(Map.Entry<String, Integer> entry : counters.entrySet()){
	            StringBuffer sb=new StringBuffer();
	            sb.append(entry.getKey()+": "+entry.getValue());
	            sb.append("\r\n");
	            out.write(sb.toString().getBytes("utf-8"));
	        }  
		}catch (IOException e){e.printStackTrace();}
		      
	}
}
```


3、结果分析：

集群环境下执行：storm jar /mnt/wc.jar cn.wc.TopologyMain /mnt/words.txt /tmp/topo.log

/*并行和通信试验：

 * 设置worker为3，启动3个进程来服务这个topology

 * spout/bolt的线程线程设置为3，默认对应一个task，就是一个进程跑一个task，总共有9个；

 * 现在对word-normalizer这个bolt设置任务6个，那就是每个进程分2个，现在总共12个task；

 * 总的来说：worker进程有3个，executor线程有9个，task任务有12个；

 * 输入：/mnt/words.txt 输出：/tmp/topo.log

*/

1）storm list发现task是15个，不是12个，怎么算就有点疑惑了；

2）输入的词汇，明显被重复统计3次，也就是说3个executor在同一文件输入源下，不会自动去协调输入记录从而排斥；

3）topology程序中设置的变量，无法再executor之间共享；

4）输入的文件新增词汇，topology没有及时去获取统计，当然topology仍然在集群中运行




4、总结：

      1）一个topology被提交到不同节点的不同worker（进程）分布执行，要按照独立进程来看；

      2）worker内要有自己唯一的输入源，同时要确保输入源是持续提供；

      3）要在worker之间共享数据变量，只能通过其他办法，如redis来存储；

      也就是说：topology被提交到集群分布式执行，不同worker之间是独立进程运作。








