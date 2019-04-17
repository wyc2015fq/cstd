# 离线轻量级大数据平台Spark之JavaRDD关联join操作 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年01月12日 16:56:13[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5604
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









对两个RDD进行关联操作，如：

1）文件post_data.txt包含：post_id\title\content

2）文件train.txt包含：dev_id\post_id\praise\time

通过post_id关联，提取post_id\content\praise字段，文件字段用不定长多个空格分割；

参考代码如下：



```java
package scs.contest;

import java.util.List;
import java.util.Properties;

import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaPairRDD;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.Optional;
import org.apache.spark.api.java.function.PairFunction;

import scala.Tuple2;

public class pageview {
	public static String path="/home/spark/data";
	public static void main(String[] args) {
	    //第一步：设置环境字符集，避免中文乱码
	    Properties pps=System.getProperties(); 
	    pps.setProperty("file.encoding","UTF-8"); 
	    //第二步：txt数据导入并分割成到训练集和预测集
	    SparkConf sparkConf = new SparkConf().setAppName("pageview");
	    JavaSparkContext jsc = new JavaSparkContext(sparkConf);
	    JavaRDD<String> postdataLines =jsc.textFile(path+"/post_data.txt");//id\title\content
	    JavaRDD<String> trainLines =jsc.textFile(path+"/train.txt");//dev_id\post_id\praise\time
	    JavaPairRDD<String,String> pdLines=postdataLines.mapToPair(new PairFunction<String,String,String>(){
	    	public Tuple2<String, String> call(String t) throws Exception {
	    		String[] st=t.split("\\s+");//多个空格情况的分割
	    		String content="";
	    		for(int i=1;i<st.length;i++){
	    			content+=st[i];
	    		}
	    		return new Tuple2(st[0],content);//title+content都取，title一般为空的情况下
	    	}
	    });
	    JavaPairRDD<String,String> ptLines=trainLines.mapToPair(new PairFunction<String,String,String>(){
	    	public Tuple2<String, String> call(String t) throws Exception {
	    		String[] st=t.split("\\s+");
	    		return new Tuple2(st[1], st[2]);
	    	}
	    });
	    //合并两个JavaPairRDD，得到训练集和预测集
	    JavaPairRDD<String, Tuple2<String, String>> tLines=pdLines.join(ptLines);//训练街
	    Tuple2<String, Tuple2<String, String>>  tpfirst=tLines.first();
	    System.out.println(tpfirst._1+"|"+tpfirst._2()._1+"|"+tpfirst._2()._2);
	    //预测集，Optional<String>=Optional.empty的记录数
	    JavaPairRDD<String, Tuple2<String, Optional<String>>> pLines=pdLines.leftOuterJoin(ptLines); 
	    Tuple2<String, Tuple2<String, String>>  tpfirst1=tLines.first();
	    System.out.println(tpfirst1._1+"|"+tpfirst1._2()._1+"|"+tpfirst1._2()._2);    
	}
}
```


执行结果：





```
507298|无聊的一天又开始了…|1
547732|人生不可能总一帆风顺，总会有一些挫折的。|Optional.empty
```



参考官网的JavaPariRDD的join操作。



