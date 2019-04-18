# 使用 httpClient 调用新浪微博接口 - z69183787的专栏 - CSDN博客
2014年02月13日 11:48:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4253
```java
/**
 * 
 */
package httpClient;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;
import javax.activation.MimetypesFileTypeMap;
import org.apache.commons.httpclient.Header;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.methods.multipart.FilePart;
import org.apache.commons.httpclient.methods.multipart.MultipartRequestEntity;
import org.apache.commons.httpclient.methods.multipart.Part;
import org.apache.commons.httpclient.methods.multipart.PartBase;
import org.apache.commons.httpclient.methods.multipart.StringPart;
import org.apache.commons.httpclient.params.HttpMethodParams;
/** 
 * @ClassName: WeiboClient 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年2月13日 上午11:09:34 
 *  
 */
public class WeiboClient {
	public static void main(String[] args) throws Exception
    {
        HttpClient client = new HttpClient();   
        //设置代理服务器地址和端口     
     //client.getHostConfiguration().setProxy("proxy_host_addr",proxy_port);
        //使用GET方法，如果服务器需要通过HTTPS连接，那只需要将下面URL中的http换成https
       // HttpMethod method = new GetMethod("http://10.1.14.20:8088/workflowController/service/todo/addTask"); 
        //使用POST方法
       // PostMethod method = new PostMethod("https://api.weibo.com/2/statuses/update.json"); 
        PostMethod method = new PostMethod("https://upload.api.weibo.com/2/statuses/upload.json"); 
      
        List<Header> headers = new ArrayList<Header>();
        headers.add(new Header("Authorization", "OAuth2 " + "2.00Tw_PoBdBYEND600e0aa317Sm_qoC"));
		headers.add(new Header("API-RemoteIP", "http://10.1.41.252"));
		client.getHostConfiguration().getParams()
				.setParameter("http.default-headers", headers);
        String s = "弗格森迭戈迭戈丰东股份的";
        File f = new File("E:\\Users\\zhoushun\\Downloads\\ce187bf40ad162d9f98819ea10dfa9ec8b13cdf6.jpg");
        byte b[] = new byte[(int) f.length()];
        FileInputStream fs = new FileInputStream(f);
        fs.read(b);
        fs.close();
        FileOutputStream fo = new FileOutputStream(new File("D://zs.jpg"));
        fo.write(b);
        fo.close();
    	// ((PostMethod) method).addParameter("status", s);
    	
    	 System.out.println(new MimetypesFileTypeMap().getContentType(f));
		HttpMethodParams param = method.getParams();
		param.setContentCharset("UTF-8");
     
	
		try {
			
			Part[] parts = new Part[2];
				parts[0] = new StringPart("status",URLEncoder.encode(s,"UTF-8"));
				FilePart filePart = new FilePart("pic", f.getName(),
						f, new MimetypesFileTypeMap().getContentType(f),
						"UTF-8");
				filePart.setTransferEncoding("binary");
				parts[parts.length - 1] = filePart;
				
//				parts[1] = new ByteArrayPart(b,
//						"123", "image/jpeg");
				method.setRequestEntity(new MultipartRequestEntity(parts,
					method.getParams()));
			
		} catch (Exception ex) {
			throw new Exception("dsad");
		}
		
		
     client.executeMethod(method);
        //打印服务器返回的状态
     System.out.println(method.getStatusLine());
       //打印返回的信息
     System.out.println();
     InputStream stream = method.getResponseBodyAsStream();
     
     BufferedReader br = new BufferedReader(new InputStreamReader(stream, "UTF-8"));
     StringBuffer buf = new StringBuffer();
     String line;
     while (null != (line = br.readLine())) {
         buf.append(line).append("\n");
     }
     System.out.println(buf.toString());
       //释放连接
     method.releaseConnection();
    }
	
	private static class ByteArrayPart extends PartBase {
		private byte[] mData;
		private String mName;
		public ByteArrayPart(byte[] data, String name, String type)
				throws IOException {
			super(name, type, "UTF-8", "binary");
			mName = name;
			mData = data;
		}
		protected void sendData(OutputStream out) throws IOException {
			out.write(mData);
		}
		protected long lengthOfData() throws IOException {
			return mData.length;
		}
		protected void sendDispositionHeader(OutputStream out)
				throws IOException {
			super.sendDispositionHeader(out);
			StringBuilder buf = new StringBuilder();
			buf.append("; filename=\"").append(mName).append("\"");
			out.write(buf.toString().getBytes());
		}
	}
}
```
