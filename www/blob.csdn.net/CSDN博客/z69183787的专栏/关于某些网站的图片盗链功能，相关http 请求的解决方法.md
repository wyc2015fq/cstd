# 关于某些网站的图片盗链功能，相关http 请求的解决方法 - z69183787的专栏 - CSDN博客
2012年11月23日 14:49:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1193
其实 关键就是 header中 的 Referer 属性，可以通过firebug 查看头信息 并获得Referer，但有些情况下可能不能获取，就只能通过抓包的方法来获得了。
```java
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
public class refererPost {
	public static void main(String[] args){
		String urls = "http://113.107.42.72:88/images/uploadimg/rw201209231804.jpg";
		URL url = null;
		HttpURLConnection http = null;
		try {
			url = new URL(urls);
			http = (HttpURLConnection) url.openConnection();
			http.setDoInput(true);
			http.setDoOutput(true);
			http.setUseCaches(false);
			http.setConnectTimeout(50000);
			http.setReadTimeout(50000);
			http.setRequestMethod("GET");
			http.setRequestProperty("Accept", "image/png,image/*;q=0.8,*/*;q=0.5");
			http.setRequestProperty("Accept-Encoding", "gzip");  
			http.setRequestProperty("Referer", "http://dmm.hk/");  
			String param = "zs=dddddddddddddddddd";
			//http.getOutputStream().write(param.getBytes());    
			//http.getOutputStream().flush();
			//http.getOutputStream().close();
			System.out.println("getResponseCode====="+http.getResponseCode());
			if (http.getResponseCode() == 200) {
				File f = new File("D://a.jpg");
				System.out.println("111111111111111111111111111111");
				BufferedInputStream in = new BufferedInputStream(
			http.getInputStream());
			int c = 0;
			BufferedOutputStream b = new BufferedOutputStream(new FileOutputStream(f));
			while((c=in.read())!=-1){
				b.write(c);
				b.flush();
			}
			
			b.close();
			in.close();
			}
		} catch (Exception e) {
			System.out.println("err");
		} finally {
		if (http != null)
			http.disconnect();
		}
	}
	}
```

