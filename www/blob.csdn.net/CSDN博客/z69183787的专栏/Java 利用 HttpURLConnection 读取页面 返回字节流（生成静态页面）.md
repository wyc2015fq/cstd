# Java 利用 HttpURLConnection 读取页面 返回字节流（生成静态页面） - z69183787的专栏 - CSDN博客
2012年10月29日 16:18:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10344
## 注：若需要被静态化的 页面中 使用了 response.sendRedirect跳转，则最后静态页面为 最终跳转后的页面。
## 而那些 使用js 跳转的 比如 window.location.href 则 无效，直接作为js代码在生成的静态页面中，并执行。
```java
//保存为文件
```
```java
import java.io.*;
public class GenerateIndexPage {
	
	protected static String defaultToFile = "frame/main_Null.html";
	protected static String defaultFromFile = "http://localhost:8080/stfb/frame/main_Null.jsp";
    String result = "";
	
	public String genHtml(String fromFile,String toFile) throws Exception {
		if("".equals(fromFile)||fromFile==null){
			fromFile = defaultFromFile;
		}
		if("".equals(toFile)||toFile==null){
			toFile = defaultToFile;
		}
		java.net.URL url = new java.net.URL(fromFile);
		java.net.HttpURLConnection conn =(java.net.HttpURLConnection) url.openConnection();
		try{
			if (conn.getResponseCode() == 200) {
			
				java.io.InputStream is = (java.io.InputStream) conn.getContent();
				try{
					ConfigInfo cfn = new ConfigInfo();
					String server_path = cfn.getValue("server_path");
					String savePath = server_path+"\\"+toFile;
					FileOutputStream baos = new FileOutputStream(new File(savePath));
					int buffer = 1024;
					byte[] b = new byte[buffer];
					int n = 0;
					while ((n = is.read(b, 0, buffer)) > 0) {
						baos.write(b, 0, n);
					}
					//String s = new String(baos.toByteArray(), WEATHER_HTML_CHARSET);
					is.close();
					baos.close();
					result = "生成成功";
				}catch(Exception e){
					result="写文件过程出错，取消生成。";
				}
			}else{
				result="获得链接过程出错，取消生成。";			
			}
		}catch(Exception e){
				e.printStackTrace();
				result="获得内容过程出错，取消生成。";
		}
		return result;
	}
	
	
}
```
//返回字节流
```java
public String getHtml(JspWriter out) throws Exception {
	//System.setProperty("http.proxyHost", "isaserver");System.setProperty("http.proxyPort", "80");
	java.net.URL url = new java.net.URL("http://weather.china.com.cn/city/58362_zx.html");
	java.net.HttpURLConnection conn = (java.net.HttpURLConnection) url.openConnection();
	if (conn.getResponseCode() == 200) {
		java.io.InputStream is = (java.io.InputStream) conn.getContent();
		java.io.ByteArrayOutputStream baos = 
			new java.io.ByteArrayOutputStream();
		
		int buffer = 1024;
		byte[] b = new byte[buffer];
		int n = 0;
		while ((n = is.read(b, 0, buffer)) > 0) {
			baos.write(b, 0, n);
		}
		String s = new String(baos.toByteArray(), "UTF-8");
		is.close();
		baos.close();
		return s;
	}
	return "";
}
```

