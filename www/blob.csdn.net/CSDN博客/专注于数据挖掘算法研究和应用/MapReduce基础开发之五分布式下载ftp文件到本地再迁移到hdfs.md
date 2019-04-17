# MapReduce基础开发之五分布式下载ftp文件到本地再迁移到hdfs - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月01日 11:48:13[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2864
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
为利用Hadoop集群平台的分布存储和计算能力，基于MapReduce将ftp文件分布式下载并上传到HDFS中。


1、文件移动流程：ftp服务器->datanode本地目录->HDFS目录；


2、实现主要基于两个设计思想：

   1）将FTP服务器文件列表作为MapReduce处理的数据对象，按照文件名分布到不同Reduce节点下载和上传到HDFS中；

   2）在每个datanode节点都建立一个本地文件保存目录，最好是统一路径名，这样每个Reduce节点都把FTP服务器文件下载到该目录下；


3、代码主要过程：

   1）驱动类中先读取FTP服务器上要下载的文件列表，并移入到hdfs中，作为Map函数的输入；

   2）Map函数处理文件列表，获取文件名字，作为Reduce函数输入；

   3）Reduce函数根据输入的文件名去下载ftp服务器上对应的文件，并下载到datanode节点的统一本地目录，再将本地目录文件上传到HDFS中；


4、主要技术点：

   1）FTPClient实现ftp文件下载；

   2）hadoop的IOUtils类实现文件从本地上传到HDFS；


5、准备工作

   1）ftp服务器端口、用户名和密码、下载文件目录；

      linux下ftp命令：进入$ftp ip/常用命令：ls/cd/put/get/mput/mget

   2）每个节点统一建立本地目录/tmp/fjs/localftp，保存ftp服务器上下载的文件；

   3）Namenode上建立HDFS保存文件的目录/tmp/fjs/ftp；

   4）Namenode上建立HDFS保存文件列表的目录/tmp/fjs/in，即Map函数的输入数据；


6、具体代码：

   1）主类FtpMR：驱动类加MapReduce类；



```java
package ct.gd;

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

public class FtpMR {
		public static class FtpMap extends Mapper<Object,Text,Text,Text>{
			private Text _key = new Text();
			private Text _value = new Text();		
			public void map(Object key,Text value,Context context) throws IOException,InterruptedException{
				String line = value.toString();
				//tag是随机值，目的是将文件均匀分到各节点下载，随机范围根据集群节点数，这里是0-100内
				//假设下载文件有1000个，100随机范围，集群有100个节点，那每个节点均匀可能获得10个文件下载，
				//map输出的<key,value>，输入reduce时，key值相同的会形成value list，因此设计该随机key值
				String tag = ComMethod.getRandomNbr();
				_key.set(tag);
				_value.set(line);
				context.write(_key,_value);
	        }
		}

	    public static class FtpReduce extends Reducer<Text,Text,Text,Text>{
	    	public void reduce(Text key,Iterable<Text> values,Context context)throws IOException,InterruptedException{
	    		String ftpSrv=context.getConfiguration().get("ftpSrv");//获取ftp服务器连接信息
	    		String outputPath=context.getConfiguration().get("outputPath");//获取hdfs存放文件的目录
	    		FtpUtil fu=new FtpUtil();	    		
	    		for(Text value:values){
	    			String filename=value.toString();//输入的value是ftp服务器上的文件名
	    			String localFile=fu.DownFileToLocal(ftpSrv,filename);//下载文件到本地目录，并返回文件保存的路径
	    			if (localFile!=null) fu.WriteFileToHDFS(localFile,ComMethod.changeToDir(outputPath),filename);//本地文件上传到hdfs中
	    		}
	    	}
	    }
	    
	public static void main(String[] args) throws Exception {			
			Configuration conf = new Configuration();
			String[] otherArgs = new GenericOptionsParser(conf, args).getRemainingArgs();
			if (otherArgs.length != 2) {
				System.err.println("Usage: FtpMR <in> <out>");
				System.exit(2);
			}
			String inputPath=otherArgs[0];//FTP服务器保存文件列表的文件目录
			String outputPath=otherArgs[1];//下载的ftp文件保存在hdfs中的目录
			FtpUtil fu=new FtpUtil();
			//ftp服务器字符串格式：IP|port|username|password|file directory
			String strFtpSrv="IP|port|name|password|directory";		
			//获取ftp服务器上文件列表，保存到hdfs的inputPath目录下
			if(!fu.getFtpFileList(strFtpSrv,inputPath)){
				System.err.println("下载ftp服务器文件列表失败");
				System.exit(2);
			}
			//将ftp服务器的参数作为参数传递到Reduce中
			conf.set("ftpSrv", strFtpSrv);
			//将hdfs上保存下载文件的目录传递到Reduce中
			conf.set("outputPath", outputPath);
			
			Job job = new Job(conf, "FtpToHdfs");
			job.setJarByClass(FtpMR.class);
			//job.setNumReduceTasks(1);//设置reduce输入文件一个，方便查看结果
			
			job.setMapperClass(FtpMap.class);
			job.setReducerClass(FtpReduce.class);
			
			job.setOutputKeyClass(Text.class);
			job.setOutputValueClass(Text.class);
			
			FileInputFormat.addInputPath(job, new Path(inputPath));
			FileOutputFormat.setOutputPath(job, new Path(outputPath));
			
			System.exit(job.waitForCompletion(true) ? 0 : 1);
		}
}
```




   2）接口类FtpUtil：主要处理ftp文件下载和写入hdfs中；



```java
package ct.gd;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.ftp.FTPReply;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;


public class FtpUtil {
	
	/*下载文件列表处理函数，开始*/
	public boolean getFtpFileList(String strFtpSrv,String inputPath){//从ftp服务器上读取文件列表
		String[] FtpSrvConn=strFtpSrv.split("\\|");//截取ftp服务器连接信息 
		FTPClient ftp = new FTPClient();
	    try {  
	        ftp.connect(FtpSrvConn[0], Integer.parseInt(FtpSrvConn[1])); //url和port   
	        ftp.login(FtpSrvConn[2], FtpSrvConn[3]); //name和password
	        int reply = ftp.getReplyCode();  
	        if (!FTPReply.isPositiveCompletion(reply)) {  
	            ftp.disconnect();  
	            return false;  
	        }  
	        String remotePath=FtpSrvConn[4];//Ftp服务器上文件目录
	        ftp.changeWorkingDirectory(remotePath);
	        FTPFile[] fs = ftp.listFiles(remotePath);
	        StringBuffer buffer = new StringBuffer();
	        for(FTPFile ff:fs){
	        	String fileName = ff.getName();
	        	buffer.append(fileName+"\n");
	        }
	        if(writeBufferToHDFSFile(buffer, inputPath)){
	        	ftp.logout();
	        	return true;
	        }
	        ftp.logout();
	    } catch (IOException e) {System.out.println(e.getMessage());} 
	    finally {  
	        if (ftp.isConnected()) {  
	            try {  ftp.disconnect();  } catch (IOException ioe) { System.out.println(ioe.getMessage()); }  
	        }  
	    }  
	    return false;  
	}
	private boolean writeBufferToHDFSFile(StringBuffer buffer, String inputPath){//将文件列表写到hdfs中
		Configuration conf =  new Configuration();
		FileSystem fs = null;
		String fileName="fileLists.txt";
		try {
			fs = FileSystem.get(conf);
			inputPath = ComMethod.changeToDir(inputPath) + fileName;
			Path fsInputPath=new Path(inputPath);
			FSDataOutputStream outputStream = fs.create(fsInputPath);
			outputStream.write(buffer.toString().getBytes("UTF-8"));
			outputStream.flush();
			outputStream.sync();
			outputStream.close();
			return true;
		} catch (IOException e) {System.out.println(e.getMessage());}
		return false;
	}
	/*下载文件列表处理函数，结束*/
	/*下载文件处理函数，开始*/
	public String DownFileToLocal(String ftpSrv,String filename){
		//在节点上创建本地保存下载文件的目录
		String localPath="/tmp/fjs/localftp";
		File localDir = new File(localPath);//如果不存在就创建
        if(!localDir.exists()){
        	localDir.mkdirs();
        }
        FTPClient ftp = new FTPClient();
        String[] FtpSrvConn=ftpSrv.split("\\|");//截取ftp服务器连接信息
	    try {  
	    	ftp.connect(FtpSrvConn[0], Integer.parseInt(FtpSrvConn[1])); //url和port   
	        ftp.login(FtpSrvConn[2], FtpSrvConn[3]); //name和password
	        int reply = ftp.getReplyCode();  
	        if (!FTPReply.isPositiveCompletion(reply)) {  
	            ftp.disconnect();  
	            return null;
	        }  
	        String remotePath=FtpSrvConn[4];//Ftp服务器上文件目录
	        ftp.changeWorkingDirectory(remotePath);
    		String localFilePath = ComMethod.changeToDir(localPath) + filename;
    		File localFile = new File(localFilePath);
        	OutputStream is = new FileOutputStream(localFile);
        	ftp.retrieveFile(filename, is);//下载
        	is.close();
        	ftp.logout();
        	return localFilePath;
	    } catch (IOException e) {  
	    	System.err.println(e.getMessage());
	    } finally {  
	        if (ftp.isConnected()) {  
	            try {  ftp.disconnect();  } catch (IOException ioe) { }  
	        }  
	    }  
        return null; 
	}
	/*下载文件处理函数，结束*/
	/*上传文件到hdfs处理函数，开始*/
	public void  WriteFileToHDFS(String localFile,String outputPath,String filename){
		Configuration conf =  new Configuration();
		FileSystem fs = null;
		
		try {
			fs=FileSystem.get(conf);
			InputStream in = new BufferedInputStream(new FileInputStream(localFile));
			String ouputFile = outputPath + filename;//hdfs存放文件路劲和名字
			OutputStream out = fs.create(new Path(ouputFile));
			IOUtils.copyBytes(in, out, 1024*1024,true);//迁移
			out.flush();
			if(out!=null) out.close();
			if(in!=null) in.close();
			//删除本地文件
			File _outputFileName = new File(localFile);
			if(_outputFileName.exists()) _outputFileName.delete();
		} catch (IOException e) {e.printStackTrace();}	
	}
	/*上传文件到hdfs处理函数，结束*/
	
	public static void main(String[] args) throws Exception { 
		
	} 
}
```




   3）通用函数类ComMethod：主要是一些通用字符处理函数；



```java
package ct.gd;

import java.util.Random;

public class ComMethod {
	
	public static String changeToDir(String dirPath){//目录最后是否有/
    	if(dirPath.charAt(dirPath.length()-1)!='/'){
    		dirPath = dirPath + "/";
    	}
    	return dirPath;
    }
	
	public static String getRandomNbr(){//获取随机数
		Random rand = new Random();
		String nbr = String.valueOf(rand.nextInt(100));
		return nbr;
	}
	
	
}
```






7、执行结果

   1）执行命令：yarn jar /mnt/mr.jar /tmp/fjs/in /tmp/fjs/ftp

   2）hadoop fs -ls /tmp/fjs/in 可以看到文件列表文件

   3）hadoop fs -ls /tmp/fjs/ftp 可以看到下载的文件

   4）每个节点ls -l /tmp/fjs/localftp，如果文件都迁入hdfs，应该为空




