# 使用HttpUrlConnection进行post请求上传文件 - 零度的博客专栏 - CSDN博客
2016年05月11日 15:20:45[零度anngle](https://me.csdn.net/zmx729618)阅读数：555标签：[http](https://so.csdn.net/so/search/s.do?q=http&t=blog)
个人分类：[Http/HttpURLConnection																[Java](https://blog.csdn.net/zmx729618/article/category/3108443)](https://blog.csdn.net/zmx729618/article/category/6191435)
                
使用HttpUrlConnection模拟post表单进行文件上传平时很少使用，比较麻烦。
原理是： 分析文件上传的数据格式，然后根据格式构造相应的发送给服务器的字符串。
格式如下：这里的httppost123是我自己构造的字符串，可以是其他任何的字符串
----------httppost123 （\r\n）
Content-Disposition: form-data; name="img"; filename="t.txt" （\r\n）
Content-Type: application/octet-stream （\r\n）
（\r\n）
sdfsdfsdfsdfsdf （\r\n）
----------httppost123 （\r\n）
Content-Disposition: form-data; name="text" （\r\n）
（\r\n）
text tttt （\r\n）
----------httppost123-- （\r\n）
（\r\n）
上面的（\r\n）表示各个数据必须以（\r\n）结尾。
具体Java代码如下：
Java代码  ![收藏代码](http://314858770.iteye.com/images/icon_star.png)
- import java.io.ByteArrayOutputStream;  
- import java.io.DataOutputStream;  
- import java.io.File;  
- import java.io.FileInputStream;  
- import java.io.InputStream;  
- import java.net.HttpURLConnection;  
- import java.net.SocketTimeoutException;  
- import java.net.URL;  
- import java.net.URLEncoder;  
- import java.util.HashMap;  
- import java.util.Iterator;  
- import java.util.Map;  
- import java.util.Set;  
- 
- import javax.imageio.ImageIO;  
- import javax.imageio.ImageReader;  
- import javax.imageio.stream.ImageInputStream;  
- 
- publicclass HttpPostUtil {  
-     URL url;  
-     HttpURLConnection conn;  
-     String boundary = "--------httppost123";  
-     Map<String, String> textParams = new HashMap<String, String>();  
-     Map<String, File> fileparams = new HashMap<String, File>();  
-     DataOutputStream ds;  
- 
- public HttpPostUtil(String url) throws Exception {  
- this.url = new URL(url);  
-     }  
- //重新设置要请求的服务器地址，即上传文件的地址。
- publicvoid setUrl(String url) throws Exception {  
- this.url = new URL(url);  
-     }  
- //增加一个普通字符串数据到form表单数据中
- publicvoid addTextParameter(String name, String value) {  
-         textParams.put(name, value);  
-     }  
- //增加一个文件到form表单数据中
- publicvoid addFileParameter(String name, File value) {  
-         fileparams.put(name, value);  
-     }  
- // 清空所有已添加的form表单数据
- publicvoid clearAllParameters() {  
-         textParams.clear();  
-         fileparams.clear();  
-     }  
- // 发送数据到服务器，返回一个字节包含服务器的返回结果的数组
- publicbyte[] send() throws Exception {  
-         initConnection();  
- try {  
-             conn.connect();  
-         } catch (SocketTimeoutException e) {  
- // something
- thrownew RuntimeException();  
-         }  
-         ds = new DataOutputStream(conn.getOutputStream());  
-         writeFileParams();  
-         writeStringParams();  
-         paramsEnd();  
-         InputStream in = conn.getInputStream();  
-         ByteArrayOutputStream out = new ByteArrayOutputStream();  
- int b;  
- while ((b = in.read()) != -1) {  
-             out.write(b);  
-         }  
-         conn.disconnect();  
- return out.toByteArray();  
-     }  
- //文件上传的connection的一些必须设置
- privatevoid initConnection() throws Exception {  
-         conn = (HttpURLConnection) this.url.openConnection();  
-         conn.setDoOutput(true);  
-         conn.setUseCaches(false);  
-         conn.setConnectTimeout(10000); //连接超时为10秒
-         conn.setRequestMethod("POST");  
-         conn.setRequestProperty("Content-Type",  
- "multipart/form-data; boundary=" + boundary);  
-     }  
- //普通字符串数据
- privatevoid writeStringParams() throws Exception {  
-         Set<String> keySet = textParams.keySet();  
- for (Iterator<String> it = keySet.iterator(); it.hasNext();) {  
-             String name = it.next();  
-             String value = textParams.get(name);  
-             ds.writeBytes("--" + boundary + "\r\n");  
-             ds.writeBytes("Content-Disposition: form-data; name=\"" + name  
-                     + "\"\r\n");  
-             ds.writeBytes("\r\n");  
-             ds.writeBytes(encode(value) + "\r\n");  
-         }  
-     }  
- //文件数据
- privatevoid writeFileParams() throws Exception {  
-         Set<String> keySet = fileparams.keySet();  
- for (Iterator<String> it = keySet.iterator(); it.hasNext();) {  
-             String name = it.next();  
-             File value = fileparams.get(name);  
-             ds.writeBytes("--" + boundary + "\r\n");  
-             ds.writeBytes("Content-Disposition: form-data; name=\"" + name  
-                     + "\"; filename=\"" + encode(value.getName()) + "\"\r\n");  
-             ds.writeBytes("Content-Type: " + getContentType(value) + "\r\n");  
-             ds.writeBytes("\r\n");  
-             ds.write(getBytes(value));  
-             ds.writeBytes("\r\n");  
-         }  
-     }  
- //获取文件的上传类型，图片格式为image/png,image/jpg等。非图片为application/octet-stream
- private String getContentType(File f) throws Exception {  
- 
- //      return "application/octet-stream";  // 此行不再细分是否为图片，全部作为application/octet-stream 类型
-         ImageInputStream imagein = ImageIO.createImageInputStream(f);  
- if (imagein == null) {  
- return"application/octet-stream";  
-         }  
-         Iterator<ImageReader> it = ImageIO.getImageReaders(imagein);  
- if (!it.hasNext()) {  
-             imagein.close();  
- return"application/octet-stream";  
-         }  
-         imagein.close();  
- return"image/" + it.next().getFormatName().toLowerCase();//将FormatName返回的值转换成小写，默认为大写
- 
-     }  
- //把文件转换成字节数组
- privatebyte[] getBytes(File f) throws Exception {  
-         FileInputStream in = new FileInputStream(f);  
-         ByteArrayOutputStream out = new ByteArrayOutputStream();  
- byte[] b = newbyte[1024];  
- int n;  
- while ((n = in.read(b)) != -1) {  
-             out.write(b, 0, n);  
-         }  
-         in.close();  
- return out.toByteArray();  
-     }  
- //添加结尾数据
- privatevoid paramsEnd() throws Exception {  
-         ds.writeBytes("--" + boundary + "--" + "\r\n");  
-         ds.writeBytes("\r\n");  
-     }  
- // 对包含中文的字符串进行转码，此为UTF-8。服务器那边要进行一次解码
- private String encode(String value) throws Exception{  
- return URLEncoder.encode(value, "UTF-8");  
-     }  
- publicstaticvoid main(String[] args) throws Exception {  
-         HttpPostUtil u = new HttpPostUtil("http://localhost:3000/up_load");  
-         u.addFileParameter("img", new File(  
- "D:\\素材\\圆月.jpg"));  
-         u.addTextParameter("text", "中文");  
- byte[] b = u.send();  
-         String result = new String(b);  
-         System.out.println(result);  
- 
-     }  
- 
- }  
后台使用ruby进行接收
ruby代码如下：
Ruby代码  ![收藏代码](http://314858770.iteye.com/images/icon_star.png)
- require "cgi"
- class UpLoadController < ApplicationController  
-     protect_from_forgery :except=>:index
- 
- def index  
-         img = params[:img]  
- if img.kind_of? String
-             logger.debug "img string : #{img}"
- else
-             logger.debug "Content-Type:#{img.content_type}"
-             logger.debug "or:#{CGI.unescape(img.original_filename)}"
- end
-         text = params[:text]  
-         logger.debug "text:#{CGI.unescape(text)}"
-         render :text=>"OK"
- end
- end
日志输入如下：
 Content-Type:image/jpeg
or:圆月.jpg
text:中文
如果不把中文转成UTF-8的格式进行传输，则后台显示中文乱码。
同样，如果其他参数包含中文，则也应当先转码。
当然，具体什么编码要和后台接收的编码一致。
