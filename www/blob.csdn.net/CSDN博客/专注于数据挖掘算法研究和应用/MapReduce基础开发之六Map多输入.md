# MapReduce基础开发之六Map多输入 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月05日 10:37:19[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2772
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
在实际MapReduce开发中，会遇到需要数据多输入路径并有对应的Map函数来处理，这需要MultipleInputs.addInputPath(job, path, inputFormatClass, mapperClass)函数。



本文模拟从不同地市中获取数据，并根据按照地市区号输出记录，具体见代码。






```java
import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class DPIDigAdsl {
	
	public static class DPIDigMapperADSLGZ extends Mapper<Object, Text, Text, Text>{
		private Text oKey=new Text();
		public void map(Object key, Text value, Context context)throws IOException, InterruptedException {
			String strKey=DPIUtil.ADSLMapHandle(value.toString(),"020");//广州
			if(!strKey.isEmpty()){
				oKey.set(strKey);
				context.write(oKey,new Text(""));
			}
		}
	}	
	
	public static class DPIDigMapperADSLFS extends Mapper<Object, Text, Text, Text>{
		private Text oKey=new Text();
		public void map(Object key, Text value, Context context)throws IOException, InterruptedException {
			String strKey=DPIUtil.ADSLMapHandle(value.toString(),"0757");//佛山
			if(!strKey.isEmpty()){
				oKey.set(strKey);
				context.write(oKey,new Text(""));
			}
		}
	}	
	
	public static class DPIDigReducer extends Reducer<Text,Text,Text,Text> {
		public void reduce(Text key, Iterable<Text> values,Context context) throws IOException, InterruptedException {
			context.write(key, new Text(""));
		}
	}
	
	public static void main(String[] args) throws Exception {
	
		Configuration conf = new Configuration();
		//设置驱动类
		Job job = new Job(conf, "DPI dig");
		job.setJarByClass(DPIDigAdsl.class);
		
		//多输入路径对应多map函数
		String inPathgz="/gz";
		String inPathfs="/fs";
		MultipleInputs.addInputPath(job, new Path(inPathgz), TextInputFormat.class, DPIDigMapperADSLGZ.class);
		MultipleInputs.addInputPath(job, new Path(inPathfs), TextInputFormat.class, DPIDigMapperADSLFS.class);
				
		//设置Reduce函数、输出数据类型、输出路径
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		job.setReducerClass(DPIDigReducer.class);
		job.setNumReduceTasks(1);//设置reduce输入文件一个，方便查看结果
		String outPath="/tmp/fjs/dpi";
		outPath=DPIUtil.changeToDir(outPath)+"adsl";
		FileOutputFormat.setOutputPath(job, new Path(outPath));
		
				
		System.exit(job.waitForCompletion(true) ? 0 : 1);
	}
}
```

```java
import java.net.MalformedURLException;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class DPIUtil {
	
	//Map处理
	public static String ADSLMapHandle(String value,String cityCode){
		String strKey="";//返回
		//获取字段值
		String[] strDPIs=value.split("\\|");//获取行，并按照|分隔符提取
		if(strDPIs.length < 10 ) return strKey;//数据不合规，直接返回
		String date=DPIUtil.DatetimeToDate(strDPIs[0]);//上网时间STARTDATE提取出YYYYMMDD，如20160430
		String account=strDPIs[1];//acc_nbr或account		
		String city=cityCode;//LATN_ID或city提取，如020、0755
		String url=strDPIs[7];//url提取，含域名和参数及值
		String domain=DPIUtil.hostFromUrl(strDPIs[7]);//Domain域名提取，如www.jd.com		
		String cookie=strDPIs[9];//cookie提取，含域名和参数及值
		//定义正则表达式
		String[] regExs={".*.;^imei$;^\\d{15}$",
				        ".*.; ^meid$;^\\d{14}$|^\\d{16}$",
				        ".*.; ^imsi$;.*.",
				        ".*.; ^biz$;.*."};
		//匹配正则表达式
		for(String regEx:regExs){
			String regExDomain=regEx.split(";")[0];//域名正则表达式
			Pattern patDomain=Pattern.compile(regExDomain);
			if (domain==null) domain="";
			Matcher matDomain = patDomain.matcher(domain);
			if(matDomain.find()){//域名匹配
				String regExPara=regEx.split(";")[1];//参数正则表达式
				Pattern patPara=Pattern.compile(regExPara);
				String regExParaVal=regEx.split(";")[2];//参数值正则表达式
				Pattern patParaVal=Pattern.compile(regExParaVal);
				//解析URL和cookie,提取参数和值
				Pattern patSplit= Pattern.compile("[?&]+"); //以多条件分割字符串 
			    String[] strSigns = patSplit.split(url+"?"+cookie);
			    for (String strSign:strSigns){
			    	if(strSign.contains("=") && strSign.split("=").length>1){//para=value参数及其值提取
			    		String para=strSign.split("=")[0];//等号右边参数
			    		if (para==null) para="";
			    		Matcher matPara = patPara.matcher(para);
			    		String paraVal=strSign.split("=")[1];//等号左边参数值
			    		if (paraVal==null) paraVal="";
			    		Matcher matParaVal = patParaVal.matcher(paraVal);
			    		if(matPara.find() && matParaVal.find()){
			    				strKey=account+"|"+date+"|"+city+"|"+domain+"|"+para+"|"+paraVal;
			    				return strKey;
			    		}    		
				    }
			    }	
			}			
		}
		return strKey;
	}
	
	//时间戳转日期时间
	public static String timestamp2date(String _timeStamp){
		String dateFormat = "yyyyMMddHHmmss";
		SimpleDateFormat fm = new SimpleDateFormat(dateFormat);
		if (_timeStamp.equals("")){return "";}
		try{
			long timeStamp = Long.parseLong(_timeStamp);
			String dt = fm.format(new Date(timeStamp*1000));
			return dt;
		} catch (Exception ex){return "";}
	}
	//截取时间日期字符串的前8位，输出日期
	public static String DatetimeToDate(String _datetime){
		return _datetime.substring(0,8);//从第一个字符开始，共8个字符输出
	}
	
	//从url中提取域名
	public static String hostFromUrl(String _url){
		String host=null;
		try {
			host = new URL(_url).getHost().toLowerCase();// 此处获取值转换为小写
		} catch (MalformedURLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return host;
	}
	
	//hadoop目录规范/
	public static String changeToDir(String dirPath){//目录最后是否有/
    	if(dirPath.charAt(dirPath.length()-1)!='/'){
    		dirPath = dirPath + "/";
    	}
    	return dirPath;
    }
	
		
	public static void main(String[] args) throws Exception { 
		
	} 
}
```






