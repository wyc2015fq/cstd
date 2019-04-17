# FTP压缩文件上传到HDFS大小不一致的问题说明(FTP传输模式) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年04月21日 10:47:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4078
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、问题：将ftp文件服务器上的压缩文件通过内存流直接写入HDFS内，却发现文件不一致，MD5SUM校验也不一致。

2、分析：

FTP的传输有两种方式：ASCII传输模式和二进制数据传输模式。

1)ASCII传输方式 ：假定用户正在拷贝的文件包含的简单ASCII码文本，如果在远程机器上运行的不是UNIX，当文件传输时ftp通常会自动地调整文件的内容以便于把文件解释成另外那台计算机存储文本文件的格式。但是常常有这样的情况，用户正在传输的文件包含的不是文本文件，它们可能是程序、[数据库](http://lib.csdn.net/base/mysql)字处理文件或者压缩文件（尽管字处理文件包含的大部分是文本，其中也包含有指示页尺寸，字库等信息的非打印字符）。在拷贝任何非文本文件之前，用binary 命令告诉ftp逐字拷贝，不要对这些文件进行处理，要用二进制传输。

2）二进制传输模式 ：在二进制传输中，保存文件的位序，以便原始和拷贝的是逐位一一对应的。即使目的地机器上包含位序列的文件是没意义的。例如macintosh以二进制方式传送可执行文件到IBM
 VM系统，在对方系统上，此文件不能执行。（但是，它可以从VM系统上以二进制方式拷贝到另一macintosh，是可以执行的）。

如果你在ASCII方式下传输二进制文件，即使不需要也仍会转译。这会使传输稍微变慢，也会损坏数据，使文件变得不能用。（在大多数计算机上，ASCII方式一般假设每一字符的第一有效位无意义，因为ASCII字符组合不使用它。如果你传输二进制文件，所有的位都是重要的。）如果你知道这两台机器是同样的，则二进制方式对文本文件和数据文件都是有效的。

因此，这就意谓着知道你要传输的是什么类型的数据是重要的，下表给出一些常见文件类型的提示。

常见文件类型

|文件|方式|
|----|----|
|Text fileSpreadsheetDatabase fileWord processor fileProgram source codeElectronic mail messagesUNIX“shell archive”UNIX“tar file”backup fileCompressed fileUnencoded fileExcutable filePostscript file|ASCII大多是二进制大多是二进制，也可能是ASCII大多是二进制，也可能是ASCIIASCIIASCIIASCII二进制二进制二进制ASCII二进制二进制|


很多数据库程序用二进制格式存贮数据，即使数据原本是文本式。所以，除非你知道你的软件的用途，我们建议对数据库文件先用二进制方式试试。然后看看你传输的文件能否正确工作。如果不能，再试用另一方式。可执行的文件一般是二进制文件。




3、解决：上文可以理解压缩文件FTP传输要用二进制，代码参考如下：



```java
package ct.dpi;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPReply;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;

public class DPIFtp {
		public static void main(String[] args) throws IOException { 
			FTPClient ftp = new FTPClient();
	        Configuration conf = new Configuration();
			FileSystem fs = FileSystem.get(conf);
			InputStream ftpInputStream = null;
			OutputStream hdfsOutStream = null;
		    try {  
		    	ftp.connect("IP", 21); //url和port 
		        ftp.login("name","pwd"); //name和password
		        int reply = ftp.getReplyCode();  
		        if (!FTPReply.isPositiveCompletion(reply)) {  
		            ftp.disconnect(); 
		            return ;
		        }  
		        ftp.setFileType(FTP.BINARY_FILE_TYPE);//压缩文件二进制传输
		        //ftp.setFileType(FTP.ASCII_FILE_TYPE);//ASCII文件传输
		        String remotePath="/GZ/20170420/00/";//文件目录
		        String filename="JTBY+0x01+0x0300+000+M-GD-GZ+AT+597+20170420005942.tar.gz";//文件
		        ftp.changeWorkingDirectory(remotePath);
		        ftpInputStream = ftp.retrieveFileStream(filename);//读取/ftp文件
				String ouputFile = "/user/fjs/" + filename;//hdfs存放文件路劲和名字
				hdfsOutStream = fs.create(new Path(ouputFile));
				IOUtils.copyBytes(ftpInputStream, hdfsOutStream, 1024*1024,true);//迁移
				hdfsOutStream.flush();
	        	ftp.logout();  	
	        	ftpInputStream.close();
	        	hdfsOutStream.close();
		    } catch (IOException e) {  
		    	System.err.println(e.getMessage());
		    } finally {  
		        try { 
			        if (ftp.isConnected()) {  
				       ftp.disconnect();  
			        } 
		      		if(hdfsOutStream!=null){
		      			hdfsOutStream.close();
		      		}
					if(ftpInputStream!=null){
						ftpInputStream.close();
					}
		      	} catch (IOException ioe) {
		      		ioe.printStackTrace();
	        	} 
		    }  
		}
}
```





