# MapReduce基础开发之三字段处理并输出Hive表 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月28日 11:36:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4373
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









 1、MR设计和开发

   1)设计：

     输入：用户名 | 数字ip | 时间戳 |  url

     MR处理：正则表达式匹配url，满足则解析url并转换ip和时间戳，

     输出：用户名|点分制IP|日期时间|URL域名|URL参数；

   2)开发，见源码；

     测试文件：/tmp/fjs/in/testdata.txt

     输出文件：/tmp/fjs/out/part-r-00000.bz2

     注意：开发中务必避免陷入两个误区，一个是本地文件处理和hdfs文件处理不同； 另一个是Map和Reduce函数是在不同节点上执行，参数要能共享；


2、Hive表：外部表tmp_fjs_adslurl，location=/tmp/fjs/out/

   1)建表脚本：hive>

     CREATE EXTERNAL TABLE `tmp_fjs_adslurl`(`account` string,`ip` string,`time_stamp` string,`domain` string,`para` string) 

     ROW FORMAT DELIMITED FIELDS TERMINATED BY '|' 

     STORED AS INPUTFORMAT 'org.apache.hadoop.mapred.TextInputFormat' 

     OUTPUTFORMAT 'org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat'

     LOCATION 'hdfs://nameservice-ha/tmp/fjs/out';

   2)查询脚本：hive>select count(*) from tmp_fjs_adslurl; 




3、代码：

    1）MapReduce代码

    其中URLLocation类解析URL提取域名和参数串采用字符串截取，可参考网上通用方法，如http://volunteer521.iteye.com/blog/1685942，这里是直接用了别人代码，未经授权不便发布。



```java
import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class AdslUrl {
	public static class UrlMapper extends Mapper<Object, Text, Text, Text>{
		
		private Text oKey = new Text();
		private Text oValue=new Text();
		Util util=new Util();
		public void map(Object key, Text value, Context context)throws IOException, InterruptedException {
			String[] iValue=value.toString().split("\\|");//获取行，并按照|分隔符提取
			oKey.set(iValue[0].trim());//用户名作为输出key
			String ip=util.Long2IP(iValue[1].trim());//数字ip转化为点分制
			String dt=util.timestamp2date(iValue[2].trim());//时间戳转日期时间
			//解析url，提取域名和参数及其值
			if(util.regular(iValue[3])){//url正则匹配
				URLocation parser = new URLocation(" "+iValue[3].trim());//头部加空格
				String domain=parser.getPath();//域名
				String para=parser.getQuery();//参数串，&分割
				oValue.set(ip+"|"+dt+"|"+domain+"|"+para);
				context.write(oKey, oValue);
			}
		}	
	}
	
	public static class UrlReducer extends Reducer<Text,Text,Text,Text> {
		private Text oKey = new Text();
		public void reduce(Text key, Iterable<Text> values,Context context) throws IOException, InterruptedException {
			for (Text val : values) {
				String str=key.toString()+"|"+val.toString();
				oKey.set(str);
				context.write(oKey, new Text(""));
			}
		}
	}
	
	public static void main(String[] args) throws Exception {
	
		Configuration conf = new Configuration();
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: AdslUrl <in> <out>");
			System.exit(2);
		}
		
		Job job = new Job(conf, "parse url");
		job.setJarByClass(AdslUrl.class);
		job.setNumReduceTasks(1);//设置reduce输入文件一个，方便查看结果
		job.setMapperClass(UrlMapper.class);
		job.setCombinerClass(UrlReducer.class);
		job.setReducerClass(UrlReducer.class);
		
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
```




   2）Util类处理字段



```java
import java.io.BufferedReader;
import java.io.FileReader;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Util {
	//数字型IP转为点分十进制IP
	public String Long2IP(String _ipLong){
		StringBuffer ipInfo = new StringBuffer(); 
		long mask[] = {0x000000FF,0x0000FF00,0x00FF0000,0xFF000000}; 
		long num = 0; 
		try{
			Long ipLong = Long.parseLong(_ipLong);
			ipInfo.setLength(0);
			for(int i=0;i<4;i++){ 
				num = (ipLong & mask[i])>>(i*8); 
				if(i>0) ipInfo.insert(0,".");   
				ipInfo.insert(0,Long.toString(num,10)); 
			}
			return ipInfo.toString(); 
		} catch (Exception ex){return "";}
	}
	
	//时间戳转日期时间
	public String timestamp2date(String _timeStamp){
		String dateFormat = "yyyyMMddHHmmss";
		SimpleDateFormat fm = new SimpleDateFormat(dateFormat);;
		
		if (_timeStamp.equals("")){return "";}
		
		try{
			long timeStamp = Long.parseLong(_timeStamp);
			String dt = fm.format(new Date(timeStamp*1000));
			return dt;
		} catch (Exception ex){return "";}
	}
	
	//正则过滤域名和参数
	public boolean regular(String url){
		//正则表达式
		String regEx="baidu|sina|qq";
		
		Pattern pattern = Pattern.compile(regEx,Pattern.CASE_INSENSITIVE);
		Matcher matcher = pattern.matcher(url);
		if(matcher.find()) return true;

		return false;
	}
}
```








