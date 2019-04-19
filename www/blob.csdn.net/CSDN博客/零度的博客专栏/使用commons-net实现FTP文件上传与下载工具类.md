# 使用commons-net实现FTP文件上传与下载工具类 - 零度的博客专栏 - CSDN博客
2017年10月20日 09:47:52[零度anngle](https://me.csdn.net/zmx729618)阅读数：2445
java使用commons-net包的FTPClient实现文件的上传下载工具类。
FTPUtil.java
```java
package org.nercita.ltxx.testftp;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.SocketException;
import java.util.Properties;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPReply;
import org.apache.log4j.Logger;
public class FTPUtil {
	
	private static Logger logger = Logger.getLogger(FTPUtil.class);
	
	/**
	 * 获取FTPClient对象
	 * @param ftpHost FTP主机服务器
	 * @param ftpPassword FTP 登录密码
	 * @param ftpUserName FTP登录用户名
	 * @param ftpPort FTP端口 默认为21
	 * @return
	 */
	public static FTPClient getFTPClient(String ftpHost,int ftpPort,
			String ftpUserName, String ftpPassword) {
		FTPClient ftpClient = null;
		try {
			ftpClient = new FTPClient();
			ftpClient.connect(ftpHost, ftpPort);// 连接FTP服务器
			ftpClient.login(ftpUserName, ftpPassword);// 登陆FTP服务器
			if (!FTPReply.isPositiveCompletion(ftpClient.getReplyCode())) {
				logger.info("未连接到FTP,用户名或密码错误!");
				ftpClient.disconnect();
			} else {
				logger.info("FTP连接成功!");
			}
		} catch (SocketException e) {
			e.printStackTrace();
			logger.info("FTP的IP地址可能错误，请正确配置。");
		} catch (IOException e) {
			e.printStackTrace();
			logger.info("FTP的端口错误,请正确配置。");
		}
		return ftpClient;
	}
	
	
	/**
	 * 本地上传文件到FTP服务器
	 * @param ftpPath 远程文件路径FTP
	 * @throws IOException
	 */
	public static void upload(String ftpPath, String ftpUserName, String ftpPassword,
			String ftpHost, int ftpPort, String fileContent, String writeTempFielPath) {
		FTPClient ftpClient = null;
		logger.info("开始上传文件到FTP...");
		try {
			ftpClient = FTPUtil.getFTPClient(ftpHost, ftpPort,ftpUserName, ftpPassword);
			// 设置PassiveMode传输
			ftpClient.enterLocalPassiveMode();
			
			// 设置以二进制流的方式传输
			ftpClient.setFileType(FTPClient.BINARY_FILE_TYPE);
			
			// 对远程目录的处理
			String remoteFileName = ftpPath;
			if(ftpPath.contains("/")) {
				remoteFileName = ftpPath.substring(ftpPath.lastIndexOf("/") + 1);
			}
			// FTPFile[] files = ftpClient.listFiles(new String(remoteFileName));
			// 先把文件写在本地。在上传到FTP上最后在删除
			boolean writeResult = write(remoteFileName, fileContent,writeTempFielPath);
			if (writeResult) {
				File f = new File(writeTempFielPath + "/" + remoteFileName);
				InputStream in = new FileInputStream(f);
				ftpClient.storeFile(remoteFileName, in);
				in.close();
				logger.info("上传文件" + remoteFileName + "到FTP成功!");
				f.delete();
			} else {
				logger.info("上传文件失败!");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}finally{
			try {
				ftpClient.disconnect();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	/**
	 * 把配置文件先写到本地的一个文件中取
	 * @param ftpPath
	 * @param str
	 * @return
	 */
	public static boolean write(String fileName, String fileContext, String writeTempFielPath) {
		try {
			logger.info("写文件到本地缓存!");
			File f = new File(writeTempFielPath + "/" + fileName);
			if(!f.exists()){
				if(!f.createNewFile()){
					logger.info("文件不存在，创建失败!");
				}
			}
			BufferedWriter bw = new BufferedWriter(new FileWriter(f, true));
			bw.write(fileContext.replaceAll("\n", "\r\n"));
			bw.flush();
			bw.close();
			return true;
		} catch (Exception e) {
			logger.error("写文件没有成功!");
			e.printStackTrace();
			return false;
		}
	}
	
	
	/**
	 * 从服务器的FTP路径下读取文件
	 * @param ftpUserName
	 * @param ftpPassword
	 * @param ftpPath
	 * @param FTPServer
	 * @return
	 */
	public static String readFileFromFTP(String ftpUserName, String ftpPassword,
			String ftpPath, String ftpHost, int ftpPort, String fileName) {
		StringBuffer resultBuffer = new StringBuffer();
		InputStream in = null;
		FTPClient ftpClient = null;
		logger.info("开始从绝对路径：" + ftpPath + "读取文件!");
		try {
			ftpClient = FTPUtil.getFTPClient(ftpHost, ftpPort,ftpUserName, ftpPassword);
			ftpClient.setControlEncoding("UTF-8"); // 中文支持
			ftpClient.setFileType(FTPClient.BINARY_FILE_TYPE);
			ftpClient.enterLocalPassiveMode();
			ftpClient.changeWorkingDirectory(ftpPath);
			in = ftpClient.retrieveFileStream(fileName);
		} catch (FileNotFoundException e) {
			logger.error("没有找到" + ftpPath + "文件");
			e.printStackTrace();		
		} catch (SocketException e) {
			logger.error("连接FTP失败!");
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
			logger.error("文件读取错误!");
			e.printStackTrace();
			
		}
		if (in != null) {
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			String data = null;
			try {
				while ((data = br.readLine()) != null) {
					resultBuffer.append(data + "\n");
				}
			} catch (IOException e) {
				logger.error("文件读取错误。");
				e.printStackTrace();
				
			}finally{
				try {
					ftpClient.disconnect();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return resultBuffer.toString();
	}
	
	public static void main(String[] args) {
		int ftpPort = 0;
		String ftpUserName = "";
		String ftpPassword = "";
		String ftpHost = "";
		String ftpPath = "";
		String writeTempFielPath = "";
		try {
			InputStream in = FTPUtil.class.getClassLoader()
					.getResourceAsStream("env.properties");
			if (in == null) {
				logger.info("配置文件env.properties读取失败!");
			} else {
				Properties properties = new Properties();
				properties.load(in);
				ftpUserName = properties.getProperty("ftpUserName");
				ftpPassword = properties.getProperty("ftpPassword");
				ftpHost = properties.getProperty("ftpHost");
				ftpPort = Integer.valueOf(properties.getProperty("ftpPort"));
				ftpPath = properties.getProperty("ftpPath");
				writeTempFielPath = properties.getProperty("writeTempFielPath");
				
				String result = readFileFromFTP(ftpUserName, ftpPassword, ftpPath, ftpHost, ftpPort, "cfg.xml");
				System.out.println("读取配置文件结果为：" + result);
				
			
				ftpPath = ftpPath + "/" + "cfg_new.xml";
				upload(ftpPath, ftpUserName, ftpPassword, ftpHost, ftpPort, result, writeTempFielPath);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
}
```
