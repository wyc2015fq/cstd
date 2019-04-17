# Apache FTP文件上传、下载、修改文件名、删除 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年04月24日 11:15:50[boonya](https://me.csdn.net/boonya)阅读数：11733








 前言：Apache FTP 是应用比较广泛的FTP上传客户端工具，它易于操作，代码简略，结构清晰，是做FTP文件客户端管理软件的优先之选。FTP的操作包括：FTP文件上传（断点续传）、FTP文件下载、FTP文件重命名、FTP文件删除，这些操作已经将FTP应用管理的方式发挥的淋漓尽致了，So 我一直都用此种方式来实现FTP文件服务器的管理工作；下附FTP工具代码。

### 1、FTP文件操作状态枚举类



```java
package com.scengine.wtms.utils.ftp;

public enum FTPStatus
{
	File_Exits(0), Create_Directory_Success(1), Create_Directory_Fail(2), Upload_From_Break_Success(3), Upload_From_Break_Faild(4), Download_From_Break_Success(5), Download_From_Break_Faild(6), Upload_New_File_Success(7), Upload_New_File_Failed(8), Delete_Remote_Success(9), Delete_Remote_Faild(10),Remote_Bigger_Local(11),Remote_smaller_local(12),Not_Exist_File(13),Remote_Rename_Success(14),Remote_Rename_Faild(15),File_Not_Unique(16);

	private int status;

	public int getStatus()
	{
		return status;
	}

	public void setStatus(int status)
	{
		this.status = status;
	}

	FTPStatus(int status)
	{
		this.status = status;
	}
}
```



### 2、FTP文件操作工具代码



```java
package com.scengine.wtms.utils.ftp;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.net.PrintCommandListener;
import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.ftp.FTPReply;
import com.scengine.wtms.utils.Log;

public class FTPUtils
{
	private FTPClient ftpClient = new FTPClient();

	/**
	 * 对象构造 设置将过程中使用到的命令输出到控制台
	 */
	public FTPUtils()
	{
		this.ftpClient.addProtocolCommandListener(new PrintCommandListener(new PrintWriter(System.out)));
	}

	/**
	 * 
	 * java编程中用于连接到FTP服务器
	 * 
	 * @param hostname
	 *            主机名
	 * 
	 * @param port
	 *            端口
	 * 
	 * @param username
	 *            用户名
	 * 
	 * @param password
	 *            密码
	 * 
	 * @return 是否连接成功
	 * 
	 * @throws IOException
	 */

	public boolean connect(String hostname, int port, String username, String password) throws IOException
	{

		ftpClient.connect(hostname, port);

		if (FTPReply.isPositiveCompletion(ftpClient.getReplyCode()))
		{

			if (ftpClient.login(username, password))
			{
				return true;
			}
		}
		disconnect();
		return false;

	}

	/**
	 * 删除远程FTP文件
	 * 
	 * @param remote
	 *            远程文件路径
	 * @return
	 * @throws IOException
	 */
	public FTPStatus delete(String remote) throws IOException
	{
		ftpClient.enterLocalPassiveMode();

		ftpClient.setFileType(FTP.BINARY_FILE_TYPE);

		FTPStatus result = null;

		FTPFile[] files = ftpClient.listFiles(remote);
		if (files.length == 1)
		{
			boolean status = ftpClient.deleteFile(remote);
			result = status ? FTPStatus.Delete_Remote_Success : FTPStatus.Delete_Remote_Faild;
		}
		else
		{
			result = FTPStatus.Not_Exist_File;
		}
		Log.getLogger(this.getClass()).info("FTP服务器文件删除标识："+result);
		return result;
	}
	
	/**
	 * 重命名远程FTP文件
	 * 
	 * @param name
	 *            新远程文件名称(路径-必须保证在同一路径下)
	 *            
	 * @param remote
	 *            远程文件路径
	 *            
	 * @return  是否成功
	 * 
	 * @throws IOException
	 */
	public FTPStatus rename(String name,String remote) throws IOException
	{
		ftpClient.enterLocalPassiveMode();

		ftpClient.setFileType(FTP.BINARY_FILE_TYPE);

		FTPStatus result = null;

		FTPFile[] files = ftpClient.listFiles(remote);
		if (files.length == 1)
		{
			boolean status = ftpClient.rename(remote, name);
			result = status ? FTPStatus.Remote_Rename_Success : FTPStatus.Remote_Rename_Faild;
		}
		else
		{
			result = FTPStatus.Not_Exist_File;
		}
		Log.getLogger(this.getClass()).info("FTP服务器文件名更新标识："+result);
		return result;
	}
	
	/**
	 * 
	 * 从FTP服务器上下载文件
	 * 
	 * @param fileName
	 *            下载文件的名字(包括后缀名)
	 * 
	 * @param remote
	 *            远程文件路径
	 * 
	 * @param local
	 *            本地文件路径
	 * 
	 * @return 是否成功
	 * 
	 * @throws IOException
	 */

	public FTPStatus download(String fileName,String remote,HttpServletResponse response) throws IOException
	{
		// 开启输出流弹出文件保存路径选择窗口
		response.setContentType("application/octet-stream");
		
        response.setContentType("application/OCTET-STREAM;charset=UTF-8");
        
        response.setHeader("Content-Disposition", "attachment;filename=" +fileName);

		ftpClient.enterLocalPassiveMode();

		ftpClient.setFileType(FTP.BINARY_FILE_TYPE);
		
		FTPStatus result;
		
		OutputStream out = response.getOutputStream();
		
		boolean status = ftpClient.retrieveFile(remote, out);
		
		result=status?FTPStatus.Download_From_Break_Success:FTPStatus.Download_From_Break_Faild;
		
		Log.getLogger(this.getClass()).info("FTP服务器文件下载标识："+result);
		
		out.close();
		
		return result;
	}

	/**
	 * 
	 * 从FTP服务器上下载文件
	 * 
	 * @param remote
	 *            远程文件路径
	 * 
	 * @param local
	 *            本地文件路径
	 * 
	 * @return 是否成功
	 * 
	 * @throws IOException
	 */

	@SuppressWarnings("resource")
	public FTPStatus download(String remote, String local) throws IOException
	{

		ftpClient.enterLocalPassiveMode();

		ftpClient.setFileType(FTP.BINARY_FILE_TYPE);

		FTPStatus result;

		File f = new File(local);

		FTPFile[] files = ftpClient.listFiles(remote);

		if (files.length != 1)
		{
			Log.getLogger(this.getClass()).info("远程文件不唯一");
			return FTPStatus.File_Not_Unique;
		}

		long lRemoteSize = files[0].getSize();

		if (f.exists())
		{
			OutputStream out = new FileOutputStream(f, true);
			Log.getLogger(this.getClass()).info("本地文件大小为:" + f.length());

			if (f.length() >= lRemoteSize)
			{

				Log.getLogger(this.getClass()).info("本地文件大小大于远程文件大小，下载中止");
				return FTPStatus.Remote_smaller_local;

			}

			ftpClient.setRestartOffset(f.length());

			boolean status = ftpClient.retrieveFile(remote, out);
			result=status?FTPStatus.Download_From_Break_Success:FTPStatus.Download_From_Break_Faild;
			out.close();

		} else
		{
			OutputStream out = new FileOutputStream(f);
			boolean status = ftpClient.retrieveFile(remote, out);
			result=status?FTPStatus.Download_From_Break_Success:FTPStatus.Download_From_Break_Faild;
			out.close();
		}

		return result;

	}

	/**
	 * 
	 * 上传文件到FTP服务器，支持断点续传
	 * 
	 * @param local
	 *            本地文件名称，绝对路径
	 * 
	 * @param remote
	 *            远程文件路径，使用/home/directory1/subdirectory/file.ext
	 *            按照Linux上的路径指定方式，支持多级目录嵌套，支持递归创建不存在的目录结构
	 * 
	 * @return 上传结果
	 * 
	 * @throws IOException
	 */

	@SuppressWarnings("resource")
	public FTPStatus upload(String local, String remote) throws IOException
	{
		// 设置PassiveMode传输
		ftpClient.enterLocalPassiveMode();

		// 设置以二进制流的方式传输
		ftpClient.setFileType(FTP.BINARY_FILE_TYPE);

		FTPStatus result;

		// 对远程目录的处理
		String remoteFileName = remote;

		if (remote.contains("/"))
		{

			remoteFileName = remote.substring(remote.lastIndexOf("/") + 1);

			String directory = remote.substring(0, remote.lastIndexOf("/") + 1);

			if (!directory.equalsIgnoreCase("/") && !ftpClient.changeWorkingDirectory(directory))
			{

				// 如果远程目录不存在，则递归创建远程服务器目录

				int start = 0;

				int end = 0;

				if (directory.startsWith("/"))
				{

					start = 1;

				} else
				{

					start = 0;

				}

				end = directory.indexOf("/", start);

				while (true)
				{

					String subDirectory = remote.substring(start, end);

					if (!ftpClient.changeWorkingDirectory(subDirectory))
					{

						if (ftpClient.makeDirectory(subDirectory))
						{

							ftpClient.changeWorkingDirectory(subDirectory);

						} else
						{

							Log.getLogger(this.getClass()).info("创建目录失败");

							return FTPStatus.Create_Directory_Fail;

						}

					}

					start = end + 1;

					end = directory.indexOf("/", start);

					// 检查所有目录是否创建完毕

					if (end <= start)
					{

						break;

					}

				}

			}

		}

		// 检查远程是否存在文件

		FTPFile[] files = ftpClient.listFiles(remoteFileName);

		if (files.length == 1)
		{

			long remoteSize = files[0].getSize();

			File f = new File(local);

			long localSize = f.length();

			if (remoteSize == localSize)
			{

				return FTPStatus.File_Exits;

			} else if (remoteSize > localSize)
			{

				return FTPStatus.Remote_Bigger_Local;

			}

			// 尝试移动文件内读取指针,实现断点续传

			InputStream is = new FileInputStream(f);

			if (is.skip(remoteSize) == remoteSize)
			{

				ftpClient.setRestartOffset(remoteSize);

				if (ftpClient.storeFile(remote, is))
				{

					return FTPStatus.Upload_From_Break_Success;

				}

			}

			// 如果断点续传没有成功，则删除服务器上文件，重新上传

			if (!ftpClient.deleteFile(remoteFileName))
			{

				return FTPStatus.Delete_Remote_Faild;

			}

			is = new FileInputStream(f);

			if (ftpClient.storeFile(remote, is))
			{

				result = FTPStatus.Upload_New_File_Success;

			} else
			{

				result = FTPStatus.Upload_New_File_Failed;

			}

			is.close();

		} else
		{

			InputStream is = new FileInputStream(local);

			if (ftpClient.storeFile(remoteFileName, is))
			{

				result = FTPStatus.Upload_New_File_Success;

			} else
			{

				result = FTPStatus.Upload_New_File_Failed;

			}

			is.close();
		}

		return result;

	}

	/**
	 * 
	 * 断开与远程服务器的连接
	 * 
	 * @throws IOException
	 */

	public void disconnect() throws IOException
	{

		if (ftpClient.isConnected())
		{
			ftpClient.disconnect();
		}

	}

	public static void main(String[] args)
	{
		FTPUtils myFtp = new FTPUtils();
		try
		{

			myFtp.connect("192.168.1.200", 21, "duser", "HTPDuserXP32");

			Log.getLogger(FTPUtils.class).info(myFtp.upload("C:\\Users\\Administrator\\Desktop\\swing.drawer.jar", "/jars/swing.drawer.jar"));

			myFtp.disconnect();

		} catch (IOException e)
		{

			Log.getLogger(FTPUtils.class).info("FTP上传文件异常：" + e.getMessage());

		}

	}

}
```







