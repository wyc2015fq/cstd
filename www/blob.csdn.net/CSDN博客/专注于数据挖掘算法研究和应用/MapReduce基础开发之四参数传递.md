# MapReduce基础开发之四参数传递 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月29日 15:40:13[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3695
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
Map和Reduce函数是在各节点进行，如果要在MapReduce数据加工中使用共同参数，要如何传参呢？方法有二：


1、Configuration类的set和get的方法读取xml/txt文件设置或自己配置，再通过Map和Reduce函数的Context来获取；

2、基于org.apache.hadoop.io.DefaultStringifier类的Store函数和Load函数，通过Writable接口序列化和反序列化实现；


这里采用方法1，模拟读取txt文件获取参数并传到Map函数。具体代码如下：



```java
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;

public class AdslDPI {
	
	public static class DPIMapper extends Mapper<Object, Text, Text, Text>{
		private Text oKey=new Text();
		public void map(Object key, Text value, Context context)throws IOException, InterruptedException {
			String[] iValue=value.toString().split("\\|");//获取行，并按照|分隔符提取
			if(iValue.length>10){
				String account=iValue[1];//源文件每行第2个字段为adsl账号，作为输出key
				String url=iValue[7];//源文件每行第8个字段为url
				String cookike=iValue[10];//源文件每行第11个字段为cookie
				//获取正则表达式匹配，conf所set得值
				String regEx = context.getConfiguration().get("regEx");
				//String regEx=".*imei.*|.*meid.*|.*imsi.*|.*biz.*";//定义正则表达式
				Pattern patMatch = Pattern.compile(regEx,Pattern.CASE_INSENSITIVE);			
				Pattern patSplit= Pattern.compile("[?&]+"); //以多条件分割字符串 
			    String[] strsUrl = patSplit.split(url); //解析URL
			    for (String strUrl:strsUrl){
			    	if(strUrl.contains("=")){//para=value参数及其值提取
			    		String[] paras=strUrl.toString().split("=");
			    		if(paras.length>1){
			    			Matcher matUrl = patMatch.matcher(paras[0]);//匹配参数中含正则表达式
			    			if(matUrl.find()){
			    				oKey.set(account+"|"+paras[0]+"|"+paras[1]);
								context.write(oKey,new Text(""));
			    			}
			    		}
				    }
			    }
			    String[] strsCookie = patSplit.split(cookike); //解析cookie
			    for (String strCookie:strsCookie){
			    	if(strCookie.contains("=")){//para=value参数及其值提取
			    		String[] paras=strCookie.toString().split("=");
			    		if(paras.length>1){
			    			Matcher matCookie = patMatch.matcher(paras[0]);//匹配参数中含正则表达式
			    			if(matCookie.find()){
			    				oKey.set(account+"|"+paras[0]+"|"+paras[1]);
								context.write(oKey,new Text(""));
			    			}
			    		}
				    }
			    }	
			}
		}
	}	
	
	public static class DPIReducer extends Reducer<Text,Text,Text,Text> {
		private Text oKey=new Text();
		public void reduce(Text key, Iterable<Text> values,Context context) throws IOException, InterruptedException {
			//获取正则表达式匹配，conf所set得值，这里只是将参数直接输出，目的是验证reduce函数也获取到参数
			String regEx = context.getConfiguration().get("regEx");
			String outStr=key.toString()+"|"+regEx;
			oKey.set(outStr);
			context.write(oKey, new Text(""));
		}
	}
	
	public static void main(String[] args) throws Exception {
	
		Configuration conf = new Configuration();
		//读取txt文件，提取正则表达式
		//String path = AdslDPI.class.getProtectionDomain().getCodeSource().getLocation().getFile(); //获取jar包所在目录  
		//path = java.net.URLDecoder.decode(path, "UTF-8");
		//path=path+System.getProperty("file.separator")+"para.txt";
		String path=System.getProperty("user.dir")+System.getProperty("file.separator")+"para.txt";//获取当前用户工作目录
		String regEx=new String("");
		try{  
			FileReader fileRd=new FileReader(path);
			BufferedReader bufRd=new BufferedReader(fileRd);
			String line=new String("");
			while((line=bufRd.readLine())!=null){
				regEx=regEx+line+"|";
			}		
		}catch(Exception e){  e.printStackTrace();  }
		regEx=regEx.substring(0, regEx.length()-1);//截取最后一个|字符
		conf.set("regEx", regEx);//传递参数
		//获取输入和输出目录
		String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
		if (otherArgs.length != 2) {
			System.err.println("Usage: AdslDPI <in> <out>");
			System.exit(2);
		}
		//设置驱动函数和mapreduce函数
		Job job = new Job(conf, "parse url and cookies");
		job.setJarByClass(AdslDPI.class);
		job.setNumReduceTasks(1);//设置reduce输入文件一个，方便查看结果
		
		job.setMapperClass(DPIMapper.class);
		job.setCombinerClass(DPIReducer.class);
		job.setReducerClass(DPIReducer.class);
		//设置输出<key,value>数据类型
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		//设置输入输出目录
		FileInputFormat.addInputPath(job, new Path(otherArgs[0]));
		FileOutputFormat.setOutputPath(job, new Path(otherArgs[1]));
		
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
```





