# 基于Apache FTP实现的文件上传下载工具 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年04月18日 11:08:49[boonya](https://me.csdn.net/boonya)阅读数：5491








基于Apache FTP实现的文件上传下载工具 ，上传文件时需要考虑以下问题：

（1）、 FTP服务器是否存在改目录，如果不存在目录则需要创建目录。

（2）、判断上传文件是否已经存在，如果存在是需要删除覆盖上传还是续传。

以下示例代码（注：未实现续传功能）。



```java
package com.scengine.wtms.utils.ftp;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.SocketException;
import java.net.URLEncoder;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.net.PrintCommandListener;
import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPClientConfig;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.ftp.FTPReply;

import com.scengine.wtms.utils.Log;

public class OLDFashionedFTP
{
	private FTPClient ftp;

	/**
	 * 对象构造 设置将过程中使用到的命令输出到控制台
	 */
	public OLDFashionedFTP()
	{
		ftp = new FTPClient();
		this.ftp.addProtocolCommandListener(new PrintCommandListener(new PrintWriter(System.out)));
	}

	/**
	 * 用户FTP账号登录
	 * 
	 * @param url
	 *            FTP地址
	 * @param port
	 *            FTP端口
	 * @param username
	 *            用户名
	 * @param password
	 *            密 码
	 * @return true/false 成功/失败
	 * @throws SocketException
	 * @throws IOException
	 */
	private boolean login(String url, int port, String username, String password) throws SocketException, IOException
	{
		int reply;
		// 1. 连接FTP服务器
		// 如果采用默认端口，可以使用ftp.connect(url)的方式直接连接FTP服务器
		ftp.connect(url, port);

		// 2. 设置编码
		// 下面三行代码必须要，而且不能改变编码格式，否则不能正确下载中文文件
		ftp.setControlEncoding("UTF-8");
		FTPClientConfig conf = new FTPClientConfig(FTPClientConfig.SYST_NT);
		conf.setServerLanguageCode("zh");

		// 3. 登录ftp
		ftp.login(username, password);

		// 看返回的值是不是230，如果是，表示登陆成功
		reply = ftp.getReplyCode();

		// 以2开头的返回值就会为真
		if (!FTPReply.isPositiveCompletion(reply))
		{
			ftp.disconnect();
			Log.getLogger(this.getClass()).info(">>>>>>>>>>>>>>>>连接服务器失败!");
			return false;
		}
		Log.getLogger(this.getClass()).info(">>>>>>>>>>>>>>>>>登陆服务器成功!");
		return true;
	}

	/**
	 * 释放FTP
	 */
	public void disconnect()
	{
		if (ftp.isAvailable())
		{
			try
			{
				ftp.logout(); // 退出FTP
			} catch (IOException e)
			{
				Log.getLogger(this.getClass()).error("FTP登录退出异常:" + e.getMessage());
			}
		}
		if (ftp.isConnected())
		{
			try
			{
				// 断开连接
				ftp.disconnect();
			} catch (IOException e)
			{
				Log.getLogger(this.getClass()).error("FTP断开连接异常:" + e.getMessage());
			}
		}
	}

	/**
	 * FTP文件上传
	 * 
	 * @param url
	 *            FTP地址
	 * @param port
	 *            FTP端口
	 * @param username
	 *            FTP用户名
	 * @param password
	 *            FTP密码
	 * @param localAdr
	 *            上传文件名
	 * @param remoteAdr
	 *            指定的FTP目录
	 * @return
	 * @throws IOException
	 */
	public boolean uploadFile(String url, int port, String username, String password, String localAdr, String remoteAdr) throws IOException
	{

		// 初始表示上传失败
		boolean success = false;
		/******验证用户登录信息*****/
		try
		{
			success = login(url, port, username, password);

			Log.getLogger(this.getClass()).info("FTP用户登录：" + (success ? "成功!" : "失败!"));
			if (!success)
			{
				return success;
			}
		} catch (IOException e)
		{
			Log.getLogger(this.getClass()).error("上传文件异常:" + e.getMessage());
			return success;
		}
		// 设置PassiveMode传输
		ftp.enterLocalPassiveMode();

		// 设置FTP文件类型为二进制，如果缺省该句 传输txt正常 但图片和其他格式的文件传输出现乱码
		ftp.setFileType(FTP.BINARY_FILE_TYPE);

		/*****对远程目录的处理******/
		String remoteFileName = remoteAdr;

		if (remoteAdr.contains("/"))
		{
			remoteFileName = remoteAdr.substring(remoteAdr.lastIndexOf("/") + 1);
			String directory = remoteAdr.substring(0, remoteAdr.lastIndexOf("/") + 1);

			if (!directory.equalsIgnoreCase("/") && !ftp.changeWorkingDirectory(directory))
			{

				// 如果远程目录不存在，则递归创建远程服务器目录
				int start = 0, end = 0;

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

					String subDirectory = remoteAdr.substring(start, end);

					if (!ftp.changeWorkingDirectory(subDirectory))
					{

						if (ftp.makeDirectory(subDirectory))
						{

							ftp.changeWorkingDirectory(subDirectory);

						} else
						{
							Log.getLogger(this.getClass()).info("创建目录失败");
							return false;
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

		/*****执行文件上传******/
		InputStream input = null;
		try
		{
			File f = new File(localAdr);
			// 得到目录的相应文件列表
			FTPFile[] fs = ftp.listFiles(remoteFileName);

			Log.getLogger(this.getClass()).info("上传文件个数:" + fs.length + "  ,文件名称:" + localAdr);

			input = new FileInputStream(f);
			// 保存文件remoteFileName
			success = ftp.storeFile(remoteFileName, input);
			
		} catch (IOException e)
		{
			if (input != null)	input.close();
			Log.getLogger(this.getClass()).info("上传文件失败:" + e.getMessage());
		} finally
		{
			if (input != null)	input.close();
			this.disconnect();
			Log.getLogger(this.getClass()).info("保存标识>>>" + success + "文件名称:" + localAdr + (success ? "上传成功!" : "上传失败!"));
		}
		return success;
	}

	/**
	 * 删除FTP文件和目录
	 * 
	 * @param url
	 *            FTP地址
	 * @param port
	 *            FTP端口
	 * @param username
	 *            用户名
	 * @param password
	 *            密 码
	 * @param remoteAdr
	 *            文件路径
	 * @param localAdr
	 *            文件名称
	 * @return true/false 成功/失败
	 */
	public boolean deleteDirectory(String url, int port, String username, String password, String remoteAdr)
	{
		boolean success = false;

		try
		{
			success = login(url, port, username, password);
			Log.getLogger(this.getClass()).info("FTP用户登录：" + (success ? "成功!" : "失败!"));
			if (!success)
			{
				return success;
			}

			//String remoteAdr_ = new String(remoteAdr.getBytes("UTF-8"), "ISO-8859-1");

			// 转到指定上传目录
			ftp.changeWorkingDirectory(remoteAdr);

			FTPFile[] fs = ftp.listFiles(); // 得到目录的相应文件列表
			if(fs.length>0)
			{
				success = ftp.removeDirectory(remoteAdr);

			}

		} catch (IOException e)
		{
			Log.getLogger(this.getClass()).error(e.getMessage());
		} finally
		{
			this.disconnect();
		}

		return success;
	}

	/**
	 * 删除FTP文件
	 * 
	 * @param url
	 *            FTP地址
	 * @param port
	 *            FTP端口
	 * @param username
	 *            用户名
	 * @param password
	 *            密 码
	 * @param remoteAdr
	 *            文件路径
	 * @return true/false 成功/失败
	 */
	public boolean deleteFile(String url, int port, String username, String password, String remoteAdr)
	{
		boolean success = false;

		try
		{
			success = login(url, port, username, password);
			Log.getLogger(this.getClass()).info("FTP用户登录：" + (success ? "成功!" : "失败!"));
			if (!success)
			{
				return success;
			}

			//String remoteAdr_ = new String(remoteAdr.getBytes("UTF-8"), "ISO-8859-1");

			// 得到目录的相应文件列表
			FTPFile[] fs = ftp.listFiles(remoteAdr);; 
			
			if(fs.length>0)
			{
				// remoteAdr_->remoteAdr
				success =ftp.deleteFile(remoteAdr);
			}
		} catch (IOException e)
		{
			Log.getLogger(this.getClass()).error(e.getMessage());
		} finally
		{
			this.disconnect();
		}
		return success;
	}

	/**
	 * 下载FTP文件
	 * 
	 * @param url
	 *            FPT地址
	 * @param port
	 *            FTP端口
	 * @param username
	 *            用户名
	 * @param password
	 *            密 码
	 * @param remoteremoteAdr
	 *            远程路径
	 * @param localAdr
	 *            文件名称
	 * @param outputStream文件输出流
	 * @param response
	 *            Http响应
	 * @return true/false 成功/失败
	 */
	public boolean downFile(String url, int port, String username, String password, String remoteremoteAdr, String localAdr, HttpServletResponse response)
	{
		boolean success = false;
		try
		{
			success = login(url, port, username, password);
			Log.getLogger(this.getClass()).info("FTP用户登录：" + (success ? "成功!" : "失败!"));
			if (!success)
			{
				return success;
			}
			// 转移到FTP服务器目录
			ftp.changeWorkingDirectory(remoteremoteAdr);
			// 得到目录的相应文件列表
			FTPFile[] fs = ftp.listFiles();
			
			for (FTPFile ftpFile : fs)
			{
				if (ftpFile.getName().equals(localAdr))
				{
					// 这个就就是弹出下载对话框的关键代码
					response.setHeader("Content-disposition", "attachment;localAdr=" + URLEncoder.encode(localAdr, "UTF-8"));
					// 将文件保存到输出流outputStream中
					File f=new File(localAdr);
					OutputStream os=new FileOutputStream(f);
					ftp.retrieveFile(new String(ftpFile.getName().getBytes("UTF-8"), "ISO-8859-1"), os);
					os.flush();
					os.close();
				}
			}
			success = true;
		} catch (IOException e)
		{
			e.printStackTrace();
		} finally
		{
			this.disconnect();
		}
		return success;
	}

	/**
	 * 读取FTP文件内容
	 * 
	 * @param url
	 *            FPT地址
	 * @param port
	 *            FTP端口
	 * @param username
	 *            用户名
	 * @param password
	 *            密 码
	 * @param remoteremoteAdr
	 *            远程路径
	 * @param localAdr
	 *            文件名称
	 * @return String 文件内容
	 */
	public String readFileContent(String url, int port, String username, String password, String remoteremoteAdr, String localAdr)
	{
		String content = null;
		try
		{
			boolean success = login(url, port, username, password);
			Log.getLogger(this.getClass()).info("FTP用户登录：" + (success ? "成功!" : "失败!"));
			if (success)
			{
				// 转移到FTP服务器目录
				ftp.changeWorkingDirectory(remoteremoteAdr); 
				
				// 得到目录的相应文件列表
				FTPFile[] fs = ftp.listFiles();
				
				for (FTPFile ftpFile : fs)
				{
					if (ftpFile.getName().equals(localAdr))
					{
						// 这个就就是弹出下载对话框的关键代码
						// 将文件保存到输出流outputStream中
						File f=new File(localAdr);
						ftp.retrieveFile(new String(ftpFile.getName().getBytes("UTF-8"), "ISO-8859-1"), new FileOutputStream(f));
						ByteArrayOutputStream bos = new ByteArrayOutputStream();
						ftp.retrieveFile(ftpFile.getName(), bos);
						bos.flush();
						bos.close();

						content = new String(bos.toByteArray(), "UTF-8");
					}
				}
			}
		} catch (IOException e)
		{
			Log.getLogger(FTPUtils.class).error(e.getMessage());
		} finally
		{
			this.disconnect(); 
		}
		return content;
	}

	/**
	 * 判断是否重名的方法
	 * 
	 * @param localAdr
	 *            文件名称
	 * @param fs
	 *            文件列表数组
	 * @return
	 */
	public static boolean isDirExist(String localAdr, FTPFile[] fs)
	{
		for (FTPFile ftpFile : fs)
		{
			if (ftpFile.getName().equals(localAdr))
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * 根据重名判断的结果 生成新的文件的名称 更改的重名为 原有名字+[n], n表示数字
	 * 
	 * @param localAdr
	 *            文件名称
	 * @param fs
	 *            FTP文件列表
	 * @return
	 */
	public static String rename(String localAdr, FTPFile[] fs)
	{
		int n = 0;
		// 创建一个可变的字符串对象 即StringBuffer对象，把localAdr值付给该对象
		StringBuffer localAdr_ = new StringBuffer("");
		localAdr_ = localAdr_.append(localAdr);

		// 重名时改名，遍历存在同名的文件
		while (isDirExist(localAdr_.toString(), fs))
		{
			n++;
			String a = "[" + n + "]";
			// 最后一出现小数点的位置
			int b = localAdr_.lastIndexOf(".");
			// 最后一次"["出现的位置
			int c = localAdr_.lastIndexOf("[");
			if (c < 0)
			{
				c = b;
			}

			// 文件的名字
			StringBuffer name = new StringBuffer(localAdr_.substring(0, c));
			// 后缀的名称
			StringBuffer suffix = new StringBuffer(localAdr_.substring(b + 1));
			localAdr_ = name.append(a).append(".").append(suffix);
		}
		return localAdr_.toString();
	}

	public static void main(String[] args)
	{
		OLDFashionedFTP ftp = new OLDFashionedFTP();
		try
		{
			ftp.uploadFile("192.168.1.200", 21, "duser", "HTPDuserXP32", "C:\\Users\\Administrator\\Desktop\\backgroud_change.html", "/htmls/backgroud_change.html");
		} catch (IOException e)
		{
			Log.getLogger(OLDFashionedFTP.class).error(e.getMessage());
		}
	}

}
```






