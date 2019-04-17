# hadoop迭代 - zj360202的专栏 - CSDN博客





2013年03月15日 14:23:16[zj360202](https://me.csdn.net/zj360202)阅读数：846








```java
package com.yourmall.cloud.platform.log.visttime.mr;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;

import com.yourmall.cloud.common.utils.DateUtils;
import com.yourmall.cloud.hadoop.utils.HadoopUtils;
import com.yourmall.cloud.platform.log.pv.base.LogPartitionerBase;

public class PlatFormMemberVisitTimeForDetailPVMR extends LogPartitionerBase implements Tool {  
	 
	private static final String SPLIT_1 = "#";
	private static final String SPLIT_2 = ",";
	private static final String SPLIT_3 = "\t";
	
    /** 
     * Map 
     *  
     * @author Administrator 
     */  
    public static class MemberVisitTimeForDetailStep1Map extends Mapper<Object, Text, Text, Text> {  
        
        
    	protected void map(Object key, Text value, Context context)  
                throws java.io.IOException, InterruptedException { 
        	System.out.println("--map1--");
    		String arr[] = value.toString().split("\001");  //平台，平台首页，平台类目页，会员访问次数
//            //1.ip 2.time 3.title 4.url 5.hostname 6.referrer 7.uvid 8.isnewvisitor 9.scr 10.spm_pre 11.spm_url 12.spm_cnt 
//        	//13.spm_url 14.userid 15.categoryid 16.shopcategoryid 17.brandid 18.adid 19.shopid 20.itemid 21.searchtype 22.searchcount
//            
////            for(int i = 0;i < arr.length;i++){
////            	System.out.print(arr[i]+"  ");
////            }
////            System.out.println(arr.length);
            if (arr.length < 1||arr[13]==null||arr[13].equals("")) { //如果userid值不存在
                return;  
            }  
//          
    		String val_str = "";
            for(int i = 0;i < arr.length;i++){
            	val_str += arr[i];
            	if(i<arr.length-1)val_str+=SPLIT_1;
            }
            context.write(new Text(arr[13]), new Text(val_str));
    	}  
    }  
     
  
    /** 
     * reduce 
     *  
     * @author Administrator 
     */  
    public static class MemberVisitTimeForDetailStep1Reduce extends Reducer<Text, Text, Text, Text> {  
        protected void reduce(Text key, Iterable<Text> value,  Context context)  
                throws java.io.IOException, InterruptedException {
        	System.out.println("------------reduce1");
            StringBuffer sb = new StringBuffer();  
            while (value.iterator().hasNext()) {  
                sb.append(value.iterator().next().toString() + SPLIT_2);  
            }  
            sb.deleteCharAt(sb.length()-1);
            context.write(key, new Text(sb.toString()));
        }  
    }  
    
    /** 
     * Map 
     *  
     * @author Administrator 
     */  
    public static class MemberVisitTimeForDetailStep2Map extends Mapper<LongWritable, Text, Text, Text> {  
        protected void map(LongWritable key, Text value,  Context context)  
                throws java.io.IOException, InterruptedException {  
        	
        	System.out.println("----------map2---------");
        	String[] datas = value.toString().split("\t");
        	if(datas.length < 2){
        		return;
        	}
        	
//            System.out.println("----------value---------"+value.toString());
        	
        	String ks = datas[0];
        	int k = 1;
        	
        	
        	String arr[] = datas[1].split(SPLIT_2);  //平台，平台首页，平台类目页，会员访问次数
        	//1.ip 2.time 3.title 4.url 5.hostname 6.referrer 7.uvid 8.isnewvisitor 9.scr 10.spm_pre 11.spm_url 12.spm_cnt 
        	//13.islogin 14.userid 15.categoryid 16.shopcategoryid 17.brandid 18.adid 19.shopid 20.itemid 21.searchtype 22.searchcount
        	String[] data = arr[0].split(SPLIT_1);
            //time和spm_url后面不需要了
        	String value_ = "";
        	
        	String ds = data[1].substring(0, 4)+data[1].substring(5, 7)+data[1].substring(8, 10)+","+data[1].substring(11, 13);
        	
            String first = ds+","+ks+","+k;
            System.out.println("key----------"+ds+","+ks+","+k);
            System.out.println("arr----"+arr[0]);
            if (arr.length < 1) {  
                return;  
            } else if(arr.length == 1) {
            	value_ = arr[0];
            	context.write(new Text(first), new Text(value_));
            }
            else{
	            long diff = 0l;
	            
	            value_ = arr[arr.length - 1];
	            
	            String[] data1 = null;
	            String[] data2 = null;
	            
	            for(int i = arr.length - 1;i >= 1;i--){
	            	data1 = arr[i-1].split(SPLIT_1);
	            	data2 = arr[i].split(SPLIT_1);
	            	System.out.println(data2[1]+"---"+data1[1]);
	            	first = ds+","+ks+","+k;
	            	System.out.println("spm_url----"+data1[10]+"---i--"+i);
	            	diff = DateUtils.compDiffBetweenTwoDate(data2[1], data1[1],"yyyy-MM-dd hh:mm:ss");
	            	if(diff>=1800*1000||data1[10]==null||data1[10].equals("")){//如果两个时间差30分钟或spm_url为空算另一次访问
	            		k = k+1;
	            		context.write(new Text(first), new Text(value_)); 
	            		value_ = "";
	            	}
	            	value_ += SPLIT_2+arr[i-1];
	            	first = ds+","+ks+","+k;
	            	if(i==1){
	            		context.write(new Text(first), new Text(value_));
	            	}
	            }
            }
        }
        
    }  
     
  
    /** 
     * reduce 
     * 最后生成的记录格式是
     * 20130201,10,userid,visitTime,i
     * 日期,小时,访客ID,访问第几次,一个自增常数(用来区分每一次访问一条记录，放到hive表里去)
     *  
     * @author Administrator 
     */  
    public static class MemberVisitTimeForDetailStep2Reduce extends Reducer<Text, Text, Text, Text> {  
        protected void reduce(Text key, Iterable<Text> value,  Context context)  
                throws java.io.IOException, InterruptedException {  
        	System.out.println("----------reduce2---------");
        	
        	String[] vs = null;
        	context.getConfiguration().set(TextOutputFormat.SEPERATOR, SPLIT_3);
        	int i ;
            for (Text val : value) { 
            	i = 1;
            	String keyStr = "";
                vs = val.toString().split(SPLIT_2);
                for(String v : vs){
                	System.out.println(key.toString()+"----"+v.toString());
                	v=v.replaceAll(SPLIT_1, SPLIT_3);
                	keyStr = key.toString()+"\t"+i;
                	if(v!=null&&!v.equals("")){
	                	context.write(new Text(keyStr), new Text(v.toString()));
                	}
                	i++;
                }
            } 
        }  
    } 
    
    public static void main(String[] args) throws Exception 
    {
//    	args = new String[3];
//    	args[0]="param.type=D";
//    	args[1]="param.startRow=20130228";
//    	args[2]="param.stopRow=20130228";
    	int exitedCode = ToolRunner.run( new PlatFormMemberVisitTimeForDetailPVMR(), args);
    	 
        System.exit(exitedCode);
        
    }
  
    @Override
	public int run(String[] arg0) throws Exception {  
    	
    	String year = conf.getRaw("param.year");
		String month = conf.get("param.month");
		String day = conf.get("param.day");
//		String hour = conf.get("param.hour");
		
		for(String arg:arg0){
			if(arg.startsWith("param.")) {
				String[] args = arg.split("=");
				if(args.length==2){
					if(arg.startsWith("param.year")){
						year = args[1];
					}else if(arg.startsWith("param.month")){
						month = args[1];
					}else if(arg.startsWith("param.day")){
						day = args[1];
					}else if(arg.startsWith("param.hour")){
//						hour = args[1];
					}
				}
			}
		}
		
		String pathStr = year+"/"+month+"/"+day+"/";
		String path1 = "hdfs://nameservice1/warehouse/log/displayLog/"+pathStr;
		String path2 = path1+"/tmp/";
		String path3 = "hdfs://nameservice1/warehouse/log/member_visit_time_detail/"+pathStr;
//		String path1 = "d:/testhadoop/hadoopData";
//		String path2 = "d:/testhadoop/hadoopDataOut1";
//		String path3 = "d:/testhadoop/hadoopDataOut2";
    	Path inputPath = new Path(path1);
    	Path outputPath1 = new Path(path2);
    	
    	
    	Configuration conf1 = new Configuration();
    	Job job1 = Job.getInstance(conf1,"");
    	job1.setMapperClass(MemberVisitTimeForDetailStep1Map.class);
    	job1.setMapOutputKeyClass(Text.class);
    	job1.setMapOutputValueClass(Text.class);
    	
    	job1.setReducerClass(MemberVisitTimeForDetailStep1Reduce.class);
    	job1.setOutputKeyClass(Text.class);
    	job1.setOutputValueClass(Text.class);
    	
    	job1.setJarByClass(PlatFormMemberVisitTimeForDetailPVMR.class);
//    	job1.setPartitionerClass(LogPartitionerAsTwo.class);
//    	job1.setGroupingComparatorClass(LogTwoWritableComparator.class);
    	
    	
    	FileInputFormat.addInputPath(job1, inputPath);
    	FileOutputFormat.setOutputPath(job1, outputPath1);
    	
    	job1.waitForCompletion(true);
    	
    	System.out.println("--------------job2----------");
//    	
    	//part-r-00000
    	Path inputPath2 = new Path(path2);//放到第一个job执行完后面，不然找不到path2，path2是目录路径，不是文件路径
    	Path outputPath2 = new Path(path3);
    	
    	Configuration conf2 = new Configuration();
    	
    	Job job2 = Job.getInstance(conf2,"");
    	job2.setMapperClass(MemberVisitTimeForDetailStep2Map.class);
    	job2.setReducerClass(MemberVisitTimeForDetailStep2Reduce.class);
    	job2.setMapOutputKeyClass(Text.class);
    	job2.setMapOutputValueClass(Text.class);
    	job2.setOutputKeyClass(Text.class);
    	job2.setOutputValueClass(Text.class);
    	job2.setJarByClass(PlatFormMemberVisitTimeForDetailPVMR.class);//不加这个会报内部mapper找不到
//    	job2.setPartitionerClass(LogPartitionerAsTwo.class);
//    	job2.setGroupingComparatorClass(LogTwoWritableComparator.class);
    	
    	
    	FileInputFormat.addInputPath(job2, inputPath2);
    	FileOutputFormat.setOutputPath(job2, outputPath2);
    	
    	int i = job2.waitForCompletion(true)?0:1;
    	HadoopUtils.delete(conf2, outputPath1);//去除中间结果
    	
    	return i;
    }  
    
}
```




