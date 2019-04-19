# Java FTP上传文件工具类 - 零度的博客专栏 - CSDN博客
2016年07月11日 14:38:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：3390
```
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.Properties;
import java.util.StringTokenizer;
import sun.net.TelnetOutputStream;
import sun.net.ftp.FtpClient;
import sun.net.ftp.FtpProtocolException;
/**
 * FTP上传工具类
 * @author
 
 * @version v1.0
 */
public class FTPUtil {
	private static FtpClient ftpClient = null;
	private static final String url;
	private static final int port;
	private static final String user;
	private static final String password;
	private static final String remoteFilePath;
	static
	{
		Properties FTPPro = ReadFTPProperties.getMsgFromPro();
		url = FTPPro.getProperty("FTP_URL");
		port = Integer.parseInt(FTPPro.getProperty("FTP_PORT"));
		user = FTPPro.getProperty("FTP_USER");
		password = FTPPro.getProperty("FTP_PASSWORD");
		remoteFilePath = FTPPro.getProperty("FTP_REMOTE_FILEPATH");
	}
	/**
	 * 链接FTP
 
	 * @throws FtpProtocolException
	 */
	private static void connectFTP() throws
	FtpProtocolException {
		try
		{
			ftpClient = FtpClient.create();
			ftpClient.connect(new
			InetSocketAddress(url, port));
			ftpClient.login(user, password.toCharArray());
			ftpClient.setBinaryType();
			if
			(!"".equals(remoteFilePath) && remoteFilePath != null) {
				ftpClient.changeDirectory(remoteFilePath);
			}
		} catch
		(IOException e) {
			e.printStackTrace();
		}
	}
	/**
	 * 关闭FTP链接
	 */
	public static void closeFTP() {
		try{
			if(ftpClient != null) {
				ftpClient.close();
			}
		} catch(IOException e) {
			e.printStackTrace();
		}
	}
	/**
	 * 上传文件到FTP
	 * @param file file文件，struts2从页面得到的File类型
 
	 * @param filePath 要保存在FTP上的路径（文件夹）
	 * @param fileName 文件名（test001.jpg）
 
	 * @return 文件是否上传成功
 
	 * @throws Exception
	 */
	public static boolean upload(File file, String filePath, String fileName) {
		TelnetOutputStream to = null;
		FileInputStream fi = null;
		filePath = remoteFilePath + Constants.FILE_SEPARATOR + filePath;
		try{
                	if(file != null) {
				connectFTP();
				if(!isDirExist(filePath.replace("\\", "/"))) {
					createDir(filePath.replace("\\", "/"));
					ftpClient.changeDirectory(filePath.replace("\\", "/"));
				}
				fi = new FileInputStream(file);
				to = (TelnetOutputStream) ftpClient.putFileStream(fileName,true);
				byte[] bytes = new
				byte[1024];
				int i = fi.read(bytes);
				while(i != -1) {
					to.write(bytes);
					i = fi.read(bytes);
				}
			}
			return true;
		} catch(FileNotFoundException e1) {
			return false;
		} catch(IOException e2) {
			return false;
		} catch(Exception e) {
			return	false;
		} finally{
			if(fi != null) {
				try{
					fi.close();
				} catch(IOException e) {
					e.printStackTrace();
				}
			}
			if(to != null) {
				try{
					to.flush();
					to.close();
				} catch(IOException e) {
					e.printStackTrace();
				}
			}
			closeFTP();
		}
	}
	/**
 
	 删除FTP制定目录下的文件
	 @param filePath    文件在FTP存储的路径
	 @param fileName 要删除的文件名称
	 @return 是否删除成功
	 */
	public static boolean deleteFileFtp(String filePath, String fileName) {
		try {
			connectFTP();
			filePath = remoteFilePath + Constants.FILE_SEPARATOR + filePath
					+ Constants.FILE_SEPARATOR;
			if
			(!isDirExist(filePath.replace("\\", "/"))) {
				return
				false;
			}
			ftpClient.changeDirectory(filePath.replace("\\", "/"));
			ftpClient.deleteFile(fileName);
			return
			true;
		} catch(Exception e) {
			e.printStackTrace();
			return false;
		} finally{
			closeFTP();
		}
	}
	/**
	 检查文件夹是否存在
	 @param dir
	 @param ftpClient
 
	 @return
	 */
	private static Boolean isDirExist(String dir) {
		try{
			ftpClient.changeDirectory(dir);
		} catch(Exception e) {
			return	false;
		}
		return true;
	}
	/**
	 创建文件夹
	 @param dir
	 @param ftpClient
	 @throws Exception
	 */
	private static void createDir(String dir) throws Exception {
        ftpClient.setAsciiType();
        StringTokenizer s = new StringTokenizer(dir, "/");
       //sign
        s.countTokens();
        String pathName = "";
        while(s.hasMoreElements()) {
            pathName= pathName + "/"+ (String) s.nextElement();
            try{
                ftpClient.makeDirectory(pathName);
            }catch(Exception e) {
                e= null;
            }
        }
        ftpClient.setBinaryType();
 
    }
}
```
