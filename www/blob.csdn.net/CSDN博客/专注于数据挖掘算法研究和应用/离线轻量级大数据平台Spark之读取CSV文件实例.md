# 离线轻量级大数据平台Spark之读取CSV文件实例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年11月09日 16:16:48[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6278
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









Spark的RDD数据集很适合处理轻量文件，一般场景下是excel文件，可以将excel文件另存为CSV（逗号分隔），Spark读取CSV文件形成RDD。

1、序列化类Record，用于保存字段



```java
package sk.sql;

import java.io.Serializable;

public class Record implements Serializable {
		String area;
		String orderid;
		String content;
		String datetime;
		// constructor , getters and setters 
		public Record(String area, String orderid, String content, String datetime) {
			// TODO Auto-generated constructor stub
			this.area=area;
			this.orderid=orderid;
			this.content=content;
			this.datetime=datetime;
		}
		public String getarea(){
			return this.area;
		}
		public void setarea(String area){
			this.area=area;
		}
		public String getorderid(){
			return this.orderid;
		}
		public void setorderid(String orderid){
			this.orderid=orderid;
		}
		public String getcontent(){
			return this.content;
		}
		public void setcontent(String content){
			this.content=content;
		}
		public String getdatetime(){
			return this.datetime;
		}
		public void setdatetime(String datetime){
			this.datetime=datetime;
		}
}
```


2、读取CSV文件并形成RDD代码



```java
package sk.sql;

import java.util.List;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.Function;

public class CSVDemo {
	
	public static void main(String[] args) {
		SparkConf conf = new SparkConf().setAppName("CSVDemo");
	    JavaSparkContext sc = new JavaSparkContext(conf);
		JavaRDD<String> data = sc.textFile("/tmp/10all.csv");
		//SQLContext sqlContext = new SQLContext(sc);

		JavaRDD<Record> rdd_records = data.map(new Function<String, Record>() {
		      public Record call(String line) throws Exception {
		         // Here you can use JSON
		         // Gson gson = new Gson();
		         // gson.fromJson(line, Record.class);
		         String[] fields = line.split(",");
		         if(fields.length<7 ) return null;
		         Record sd = new Record(fields[1],fields[2],fields[4],fields[6]);
			     return sd;
		      }
		});
		List<Record> lr=rdd_records.collect();
		for(Record rd:lr){
			if(rd!=null)
				System.out.println(rd.area+"|"+rd.orderid+"|"+rd.content+"|"+rd.datetime);
		}
		//rdd_records.saveAsTextFile("/tmp/10all.txt");
		sc.stop();
	}
	
}
```








