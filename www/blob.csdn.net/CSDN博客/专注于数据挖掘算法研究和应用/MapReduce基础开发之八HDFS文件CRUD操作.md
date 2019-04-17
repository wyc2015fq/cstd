# MapReduce基础开发之八HDFS文件CRUD操作 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月15日 14:45:46[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2073
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









HDFS文件操作的基础代码。



```java
package com.hive;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.BlockLocation;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;
import org.apache.hadoop.fs.FileStatus;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.LocatedFileStatus;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.fs.RemoteIterator;
import org.apache.hadoop.hdfs.DistributedFileSystem;
import org.apache.hadoop.hdfs.protocol.DatanodeInfo;

public class HdfsCRUD {
	
	public static void main(String[] args) {//提交集群执行yarn jar
	    //ls
		//String srcPath="/tmp/fjs/dpi1/";
		//HdfsCRUD.list(srcPath);
		//cat
		//String file="/tmp/fjs/in/test.txt";
		//HdfsCRUD.readFile(file);
		//HdfsCRUD.getModificationTime(file);
		//HdfsCRUD.getBlockLocations(file);
		//HdfsCRUD.getHostnames();
		String dir="/tmp/fjs/in/hdfs";
		HdfsCRUD.mkdir(dir);
	}
	
	/*
	 * hdoop fs -ls命令
	 */
	public static void list(String srcPath) { 
		Configuration conf = new Configuration();  
		FileSystem fs = null;
		try {
			fs = FileSystem.get(conf);
			RemoteIterator<LocatedFileStatus>rmIterator = fs.listLocatedStatus(new Path(srcPath));  
            while (rmIterator.hasNext()) {  
                      Path path = rmIterator.next().getPath();  
                      if(fs.isDirectory(path)){  
                    	  System.out.println("-----------DirectoryName: "+path.getName());  
                      }  
                      else if(fs.isFile(path)){  
                    	  System.out.println("-----------FileName: "+path.getName());  
                      }  
            }  
		} catch (IOException e) {System.out.println(e.getMessage());}
	}  
	/*
	 * hdoop fs -cat命令
	 */
	public static void readFile(String file){  
		Configuration conf = new Configuration();  
		FileSystem fs = null;
        try {  
                 fs= FileSystem.get(conf);  
                 Path path = new Path(file);  
                 if(!fs.exists(path)){  
                	 System.out.println("file'"+ file+"' doesn't exist!");  
                	 return ;  
                 }  
                 FSDataInputStream in = fs.open(path);  
                 String filename = file.substring(file.lastIndexOf('/') + 1, file.length()); 
                 OutputStream out = new BufferedOutputStream(new FileOutputStream(new File("/tmp/"+filename)));   

                 byte[] b = new byte[1024];  
                 int numBytes = 0;  
                 while ((numBytes = in.read(b)) > 0) {  
                          out.write(b,0,numBytes);  
                 }  
                 in.close();  
                 out.close();  
                 fs.close();  
        }catch (IOException e)  {System.out.println(e.getMessage());}  
   }  
	
	/**  
     * Gets the information about the file modified time.  
     */  
    public static void getModificationTime(String source){ 
    	try{
    		Configuration conf = new Configuration();   
        	FileSystem fs = FileSystem.get(conf);  
        	Path srcPath = new Path(source);   
        	// Check if the file alreadyexists  
        	if (!(fs.exists(srcPath))) {  
        		System.out.println("No such destination " + srcPath);
        		return;  
        	}  
        	// Get the filename out of thefile path  
        	String filename = source.substring(source.lastIndexOf('/') + 1, source.length()); 
        	FileStatus fileStatus = fs.getFileStatus(srcPath);  
        	long modificationTime =fileStatus.getModificationTime(); 
        	System.out.println("modified datetime: " + System.out.format("File %s; Modification time :%2$tI:%2$tM:%2$tS%n",filename,modificationTime));       
    	}catch (IOException e)  {System.out.println(e.getMessage());}  	
    }  
    
    /**  
     * Gets the file block location info  
     */  
    public static void getBlockLocations(String source){  
    	try{
              Configuration conf = new Configuration();  
              FileSystem fs = FileSystem.get(conf);  
              Path srcPath = new Path(source);  
                 
              // Check if the file alreadyexists  
              if (!(fs.exists(srcPath))) {  
                       System.out.println("No such destination " + srcPath);  
                       return;  
              }  
              // Get the filename out of thefile path  
              String filename = source.substring(source.lastIndexOf('/') + 1, source.length());  
                 
              FileStatus fileStatus = fs.getFileStatus(srcPath);  
                 
              BlockLocation[] blkLocations = fs.getFileBlockLocations(fileStatus, 0, fileStatus.getLen());  
              int blkCount = blkLocations.length;  
                 
              System.out.println("File :" + filename + "stored at:");  
              for (int i=0; i < blkCount; i++) {  
                       String[] hosts = blkLocations[i].getHosts();  
                       System.out.println("host ip:" +System.out.format("Host %d: %s %n", i, hosts));  
              }  
    	}catch (IOException e)  {System.out.println(e.getMessage());} 
    }  
    /*
     * 获取Hadoop集群中data node的DNS主机名
     */
    public static void getHostnames (){  
    	try{
    		Configuration config = new Configuration();  
            FileSystem fs = FileSystem.get(config);  
            DistributedFileSystem hdfs = (DistributedFileSystem) fs;  
            DatanodeInfo[] dataNodeStats = hdfs.getDataNodeStats();  
               
            String[]names = new String[dataNodeStats.length];  
            for (int i = 0; i < dataNodeStats.length; i++) {  
                     names[i]= dataNodeStats[i].getHostName();  
                     System.out.println("datenode hostname:"+(dataNodeStats[i].getHostName()));  
            }  
    	}catch (IOException e)  {System.out.println(e.getMessage());} 
    }  
	
    /*
     * hadoop fs -mkdir命令
     */
    public static void mkdir(String dir){  
        Configuration conf = new Configuration();  
        FileSystem fs = null;  
        try {  
        	fs= FileSystem.get(conf);  
        	Path path = new Path(dir); 
        	if(!fs.exists(path)){ 
        		fs.mkdirs(path);  
        		System.out.println("create directory '"+dir+"' successfully!");  
        		}else{ 
        			System.out.println("directory '"+dir+"' exits!");  
        		}  
        }catch (IOException e) {  
        	System.out.println("FileSystem get configuration with anerror"); 
        	e.printStackTrace();  
        }finally{  
                 if(fs!= null){  
                          try {  
                        	  fs.close();  
                          }catch (IOException e) { 
                        	  System.out.println(e.getMessage());  
                        	  new RuntimeException(e);  
                          }  
                 }  
        }  
    }  
    /*
     * 本地文件上传到hdfs 
     * hadoop fs -put命令
     */
    public void copyFromLocal (String source, String dest) {  
        
        Configuration conf = new Configuration();  
        FileSystem fs=null;  
        try {  
        	fs= FileSystem.get(conf); 
        	Path srcPath = new Path(source);  
        	Path dstPath = new Path(dest);
        	// Check if the file alreadyexists  
        	if (!(fs.exists(dstPath))) {  
        		System.out.println("dstPathpath doesn't exist" ); 
        		System.out.println("No such destination " + dstPath);  
        		return;  
        	}  
        	// Get the filename out of thefile path  
        	String filename = source.substring(source.lastIndexOf('/') + 1, source.length());
        	try{  
        		//if the file exists in the destination path, it will throw exception.  
        		//fs.copyFromLocalFile(srcPath,dstPath);
        		//remove and overwrite files with the method  
        		//copyFromLocalFile(booleandelSrc, boolean overwrite, Path src, Path dst)  
        		fs.copyFromLocalFile(false, true, srcPath, dstPath); 
        		System.out.println("File " + filename + "copied to " + dest); 
        	}catch(Exception e){  
        		System.out.println(e.getMessage());  
        		new RuntimeException(e);  
        	}finally{  
        		fs.close();  
        	}  
        }catch (IOException e1) {  
        	System.out.println(e1.getMessage());
        	new RuntimeException(e1);  
        }  
    }  
    /*
     * 添加一个文件到指定的目录下
     */
    public void addFile(String source, String dest)  {  
        // Conf object will readthe HDFS configuration parameters  
        Configuration conf = new Configuration();  
        FileSystem fs=null;  
        try {  
        	fs= FileSystem.get(conf);  
        	// Get the filename out of thefile path  
        	String filename = source.substring(source.lastIndexOf('/') + 1, source.length()); 
        	// Create the destination pathincluding the filename. 
        	if (dest.charAt(dest.length() - 1) != '/') { 
        		dest= dest + "/" + filename;  
        	}else {  
        		dest= dest + filename;  
        	}  
        	// Check if the file alreadyexists  
        	Path path = new Path(dest);  
        	if (fs.exists(path)) {  
        		System.out.println("File " + dest + " already exists"); 
        		return; 
        	}  
        	
        	// Create a new file and writedata to it.
        	FSDataOutputStream out = fs.create(path);  
        	InputStream in = new BufferedInputStream(new FileInputStream(new File(source)));
        	byte[] b = new byte[1024];  
        	int numBytes = 0;  
        	//In this way read and write datato destination file.
        	while ((numBytes = in.read(b)) > 0) {
        		out.write(b,0, numBytes);  
        	}  
        	in.close();  
        	out.close(); 
        	fs.close();  
        }catch (IOException e) {  
        	System.out.println(e.getMessage());
        	new RuntimeException(e);  
        }  
    }  
    /*
     * 重新命名hdfs中的文件名称
     */
    public void renameFile (String fromthis, String tothis){  
        Configuration conf = new Configuration();      
        FileSystem fs=null;  
        try {  
        	fs= FileSystem.get(conf); 
        	Path fromPath = new Path(fromthis);  
        	Path toPath = new Path(tothis);  
        	if (!(fs.exists(fromPath))) {  
        		System.out.println("No such destination " + fromPath);
        		return;  
        	}  
        	if (fs.exists(toPath)) {  
        		System.out.println("Already exists! " + toPath);  
        		return;  
        	}  
        	try{  
        		boolean isRenamed = fs.rename(fromPath,toPath);     //renames file name indeed.
        		if(isRenamed){ 
        			System.out.println("Renamed from " + fromthis + " to " + tothis); 
        		} 
        	}catch(Exception e){  
        		System.out.println(e.getMessage()); 
        		new RuntimeException(e);  
        	}finally{   fs.close(); } 
        }catch (IOException e1) {  
        	System.out.println(e1.getMessage()); 
        	new RuntimeException(e1);  
        } 
    }  
    /*
     * 删除指定的一个文件
     * hadoop fs -rm -r命令
     */
    public void deleteFile(String file)  {  
        Configuration conf = new Configuration();  
        FileSystem fs=null;  
        try {  
        	fs= FileSystem.get(conf);  
        	Path path = new Path(file);  
        	if (!fs.exists(path)) { 
        		System.out.println("File " + file + " does not exists");  
        		return;  
        	}  
                 /* 
                  * recursively delete the file(s) if it is adirectory. 
                  * If you want to mark the path that will bedeleted as 
                  * a result of closing the FileSystem. 
                  *  deleteOnExit(Path f) 
                  */  
        	fs.delete(new Path(file), true);  
        	fs.close();  
        }catch (IOException e) { 
        	System.out.println(e.getMessage()); 
        	new RuntimeException(e);  
        }  
    }  

}
```







