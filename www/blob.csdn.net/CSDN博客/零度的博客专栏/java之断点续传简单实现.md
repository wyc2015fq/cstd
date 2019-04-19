# java之断点续传简单实现 - 零度的博客专栏 - CSDN博客
2016年07月01日 17:48:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：4759
        断点续传主要是使用http协议中range的属性来取得资源的部分内容，由于一般服务是不对外直接提供url访问的，一般都是通过id，在servlet中输出byte[]来实现，所以要想实现断点续传一般要自己实现服务端和客户端，客户端保持文件的下载或上传状态，（保存在本地或者数据库中）。再进行中断时保持中断状态，在进行续传时，首先读出文件的状态，然后设置range属性信息发送续传请求。服务器收到续传请求，读取range属性值，从文件中读取数据，发送到客户端。上述是基本的原理，上传下载原理相同。
       下面模拟两次从服务器中现在文件。
       客户端：分多次取得部分文件内容，利用RandomAccessFile文件中读取数据，写到输出流中。
```
package org.nercita.zmx.servlet;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
public class LoadClient {
	
	public static void load(int start, int end) throws MalformedURLException, FileNotFoundException{
		
		String endpoint = "http://localhost:8080/RestDemo/servlet/LoadServlet?id=5";
			
		URL url = new URL(endpoint);
		try {
			HttpURLConnection conn = (HttpURLConnection) url.openConnection();
			conn.setRequestProperty("Content-Type","text/plain; charset=UTF-8"); 
			conn.setRequestProperty("RANGE","bytes="+start+"-"+end); //header中增加range属性			
			conn.connect();
			System.out.println(conn.getResponseCode());
			System.out.println(conn.getContentLength());
			System.out.println(conn.getContentType());
			InputStream ins = (InputStream)conn.getContent();	
			String fileName=conn.getHeaderField("Content-Disposition");
			fileName = new String(fileName.getBytes("ISO8859-1"), "UTF-8");
			fileName=fileName.substring(fileName.lastIndexOf("\\")+1);
			System.out.println(fileName);
			RandomAccessFile raFile = new RandomAccessFile("E:\\"+fileName, "rw");
			raFile.seek(start);			
			byte[] buffer = new byte[4096];
			int len = -1;
			while((len = ins.read(buffer))!=-1){
				raFile.write(buffer,0,len);
			}
			raFile.close();
			conn.disconnect();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static void main(String[] args) throws IOException {
		
		load(0,1000);
		load(1001,0);
	
	}
}
```
服务端：获取分析range属性，从收入流读取内容字节流，利用RandomAccessFile输出到文件中。其中还解决了中文文件名乱码的问题，由于http协议中规定，当在网络中传输时，setHeader方法中的字符只能按ISO8859-1传输，所以这时候就要把Unicode字符转换成了ISO8859-1的编码传到客户端，客户端进行解码。否则会出现乱码。
```
package org.nercita.zmx.servlet;
import java.io.IOException;
import java.io.RandomAccessFile;
import javax.servlet.ServletException;
import javax.servlet.ServletOutputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
public class LoadServlet extends HttpServlet {
	private static final long serialVersionUID = 237208504975097723L;
	public void doGet(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		int id = Integer.parseInt(request.getParameter("id"));
		System.out.println(request.getContextPath());
		String path = request.getServletContext().getRealPath("/load");
		String filename = "" ;
		if (id == 1)
			filename = path+"JDK_API_1_5_zh_CN.CHM";
		else if (id == 2)
			filename = path+"JDK_API_1_6_zh_CN.CHM";
		else if (id == 3)
			filename = path+"tomcat.gif";
		else
			filename = path+"\\断点续传text.txt";
		RandomAccessFile raFile = new RandomAccessFile(filename, "r");
		
		String range = request.getHeader("RANGE");
		int start=0, end=0;
		if(null!=range && range.startsWith("bytes=")){
			String[] values =range.split("=")[1].split("-");
			start = Integer.parseInt(values[0]);
			end = Integer.parseInt(values[1]);
		}
		int requestSize=0;
		if(end!=0 && end > start){
			requestSize = end - start + 1;
			response.addHeader("content-length", ""+(requestSize));
		} else {
			requestSize = Integer.MAX_VALUE;
		}
		
		byte[] buffer = new byte[4096];			
		response.setContentType("application/x-download");		
		filename = new String(filename.getBytes("UTF-8"), "ISO8859-1");
		response.addHeader("Content-Disposition", "attachment;filename="+filename);
		ServletOutputStream os = response.getOutputStream();
		int needSize = requestSize;
		raFile.seek(start);
		while(needSize > 0){
			int len = raFile.read(buffer);
			if(needSize < buffer.length){
				os.write(buffer,0,needSize);
			} else {
				os.write(buffer,0,len);
				if(len < buffer.length){
					break;
				}
			}
			needSize -= buffer.length;
		}
			
		raFile.close();
		os.close();
	}
	public void doPost(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
    
		 this.doGet(request, response);
	}
}
```
             上述代码，已通过本人测试，有兴趣的可以一试！

