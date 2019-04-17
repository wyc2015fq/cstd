# Java基于FTPClient上传文件到FTP服务器 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月29日 16:27:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2593








1、上传文件到FTP服务器，用FTPClient类，引入commons-net-3.1.jar包

2、参考代码：



```java
//上传ftp
    public static boolean uploadFile(String filename) {
    	boolean success = false;
    	FTPClient ftp = new FTPClient();
    	try {
    		int reply;
    		ftp.connect("IP");//连接FTP服务器，默认端口
    		ftp.login("account", "pwd");//登录
    		reply = ftp.getReplyCode();
    		if (!FTPReply.isPositiveCompletion(reply)) {
    			ftp.disconnect();
    			return success;
    		}
    		ftp.changeWorkingDirectory("/");
    		FileInputStream in=new FileInputStream(new File("D://tmp//img//"+filename)); 
    		ftp.storeFile(filename, in);  
    		in.close();
    		ftp.logout();
    		success = true;
    	} catch (IOException e) {e.printStackTrace();} 
    	finally {
    		if (ftp.isConnected()) {
    			try {
    				ftp.disconnect();
    			} catch (IOException ioe) {}
    		}
    	}
    	return success;
    }
```





