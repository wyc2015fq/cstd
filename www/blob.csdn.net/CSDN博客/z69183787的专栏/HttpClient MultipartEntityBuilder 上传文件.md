# HttpClient MultipartEntityBuilder  上传文件 - z69183787的专栏 - CSDN博客
2017年11月23日 16:45:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4979
http://blog.csdn.net/yan8024/article/details/46531901
http://www.51testing.com/html/56/n-3707756.html
　在HttpCient4.3之后上传文件主要使用的类是位于org.apache.http.entity.mime下的MultipartEntityBuilder（原先的MultipartEntity已经基本弃用了）。
　1.首先是设置上传的模式；
　　setMode(HttpMultipartMode mode)，其中mode主要有BROWSER_COMPATIBLE，RFC6532，STRICT三种，默认值是STRICT。
2.其次是创建MultipartEntityBuilder对象，并添加需要上传的数据；
　　a.利用MultipartEntityBuilder.create()来创建对象；
　　b.addBinaryBody：以二进制的形式添加数据，可以添加File、InputStream、byte[]类型的数据。
　　addBinaryBody(String name, File file, ContentType contentType, String filename)
　　addBinaryBody(String name, InputStream stream, ContentType contentType, String filename)
　　addBinaryBody(String name, byte[] b, ContentType contentType, String filename)
　　c.addTextBody：添加文本数据
　　addTextBody(String name, String text, ContentType contentType)
　　d.addPart：以Key/Value的形式添加ContentBody类型的数据
　　addPart(String name, ContentBody contentBody)
　　更多函数可以参考：https://hc.apache.org/httpcomponents-client-ga/httpmime/apidocs/org/apache/http/entity/mime/MultipartEntityBuilder.html。在GACHA API中，主要是上传图片，所以在测试中选用了addBinaryBody。其中，第一个参数name的值，是服务器已经定义好的，服务器会根据这个字段来读取我们上传的文件流，不匹配则会报错。关于contentType，可以参考：http://tool.oschina.net/commons，不同文件扩展名所对应的类型。而file/stream和fileName，就是我们所要上传文件的信息。当用浏览器的开发者工具查看API请求时，我们会看到这些/参数基本位于API请求中的
 Request Payload字段。
![](http://www.51testing.com/attachments/2016/03/14982672_201603211112331qWhp.jpg)
3.然后是利用build()方法创建一个HttpEntity对象；
　　4.最后将HttpEntity对象添加到指定的URL上，采用HttpPost的setEntity的方法；
　　5.最后的最后，就是调用HttpClient对象发送请求，并获取服务器的响应
![](http://www.51testing.com/attachments/2016/03/14982672_20160321111233245m6.jpg)
 在HttpCient4.3之前上传文件主要使用MultipartEntity这个类，但现在这个类已经不在推荐使用了。随之替代它的类是MultipartEntityBuilder。
## 下面让我们了解一下MultipartEntityBuilder类：
         MultipartEntityBuilder这个类主要用于创建HttpEntity。它的主要方法有：
|**修饰符和类型**|**方法和描述**|
|----|----|
|**MultipartEntityBuilder**|**addBinaryBody**(**String** name, byte[] b)将字节数组以二进制的形式添加数据。|
|**MultipartEntityBuilder**|**addBinaryBody**(**String** name, byte[] b, **ContentType** contentType, **String** filename)将字节数组以二进制的形式添加数据。|
|**MultipartEntityBuilder**|**addBinaryBody**(**String** name, **File** file)将文件以二进制的形式添加数据。|
|**MultipartEntityBuilder**|**addBinaryBody**(**String** name, **File** file, **ContentType** contentType, **String** filename)将文件以二进制的形式添加数据。|
|**MultipartEntityBuilder**|**addBinaryBody**(**String** name, **InputStream** stream)|
|**MultipartEntityBuilder**|**addBinaryBody**(**String** name, **InputStream** stream, **ContentType** contentType, **String** filename)将输入流以二进制的形式添加数据。|
|**MultipartEntityBuilder**|**addPart**(**String** name, **ContentBody** contentBody)添加**ContentBody**类型的数据。|
|**MultipartEntityBuilder**|**addTextBody**(**String** name, **String** text)添加文本数据。|
|**MultipartEntityBuilder**|**addTextBody**(**String** name, **String** text, **ContentType** contentType)以指定的内容类型添加文本数据。|
|**HttpEntity**|**build**()创建一个HttpEntity。|
|**static MultipartEntityBuilder**|**create**()创建一个MultipartEntityBuilder对象。|
|**MultipartEntityBuilder**|**setBoundary**(**String** boundary)设置边界。|
|**MultipartEntityBuilder**|**setCharset**(**Charset** charset)设置请求的编码格式。|
|**MultipartEntityBuilder**|**setLaxMode**()|
|**MultipartEntityBuilder**|**setMode**(**HttpMultipartMode** mode)设置模式。|
|**MultipartEntityBuilder**|**setStrictMode**()|
主要方法说明：
**addBinaryBody、addPart、addTextBody**方法用于添加要上传的数据，从上面的表格中可以发现用于添加数据的方法，都是**key-value**类型。所以在服务器端我们可以通过request.getPart("keyname")方式获取对应key的数据。也可以通过request.getParts()方式获取客户端通过以上三种方法提交所有数据。
1.通过**addBinaryBody**方法直接可以添加**File、InputStream、byte[]**类型的数据。
2.通过**addPart**方法只能添加**ContentBody**类型的数据，在org.apache.http.entity.mime.content包中已经提供了**String**、**File**以及**InputStream**对应的**ContentBody**类型的子类，如FileBody、InputStreamBody、StringBody，通过这些类我们可以将**String**、File以及**InputStream**类型的数据转换成**ContentBody**类型的数据。
3.通过**addTextBody**方法我们可以很方便的添加文本数据。
# 2.通过HttpCient上传文件
Android端需要添加[httpcore-4.3.2.jar](http://download.csdn.net/detail/fengyuzhengfan/8018277)、[httpmime-4.3.5.jar](http://download.csdn.net/detail/fengyuzhengfan/8018281)两个包。两个包缺一不可。
在这里我用的是最新版的**HttpCient**，大家可以从[http://hc.apache.org/downloads.cgi](http://download.csdn.net/detail/fengyuzhengfan/8018289)上下载所需要的jar包，如果上面的网站打不开，大家也不用担心，我已经将项目中所需要的jar包上传到CSDN上《[httpcomponents-client-4.3.5-bin.zip](http://download.csdn.net/detail/fengyuzhengfan/8018289)》需要的朋友可以去下载。
##       Android端项目核心代码：
**[java]**[view plain](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)[copy](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/481524/fork)
- HttpClient client=new DefaultHttpClient();// 开启一个客户端 HTTP 请求 
- HttpPost post = new HttpPost(url);//创建 HTTP POST 请求  
- MultipartEntityBuilder builder = MultipartEntityBuilder.create();  
- //      builder.setCharset(Charset.forName("uft-8"));//设置请求的编码格式
- builder.setMode(HttpMultipartMode.BROWSER_COMPATIBLE);//设置浏览器兼容模式
- int count=0;  
- for (File file:files) {  
- //          FileBody fileBody = new FileBody(file);//把文件转换成流对象FileBody
- //          builder.addPart("file"+count, fileBody);
-     builder.addBinaryBody("file"+count, file);  
-     count++;  
- }         
- builder.addTextBody("method", params.get("method"));//设置请求参数
- builder.addTextBody("fileTypes", params.get("fileTypes"));//设置请求参数
- HttpEntity entity = builder.build();// 生成 HTTP POST 实体      
- post.setEntity(entity);//设置请求参数
- HttpResponse response = client.execute(post);// 发起请求 并返回请求的响应
- if (response.getStatusLine().getStatusCode()==200) {  
- returntrue;  
- }  
- returnfalse;         
**[java]**[view plain](http://blog.csdn.net/yan8024/article/details/46531901#)[copy](http://blog.csdn.net/yan8024/article/details/46531901#)
- HttpClient client=new DefaultHttpClient();// 开启一个客户端 HTTP 请求 
- HttpPost post = new HttpPost(url);//创建 HTTP POST 请求  
- MultipartEntityBuilder builder = MultipartEntityBuilder.create();  
- //      builder.setCharset(Charset.forName("uft-8"));//设置请求的编码格式
- builder.setMode(HttpMultipartMode.BROWSER_COMPATIBLE);//设置浏览器兼容模式
- int count=0;  
- for (File file:files) {  
- //          FileBody fileBody = new FileBody(file);//把文件转换成流对象FileBody
- //          builder.addPart("file"+count, fileBody);
-     builder.addBinaryBody("file"+count, file);  
-     count++;  
- }         
- builder.addTextBody("method", params.get("method"));//设置请求参数
- builder.addTextBody("fileTypes", params.get("fileTypes"));//设置请求参数
- HttpEntity entity = builder.build();// 生成 HTTP POST 实体      
- post.setEntity(entity);//设置请求参数
- HttpResponse response = client.execute(post);// 发起请求 并返回请求的响应
- if (response.getStatusLine().getStatusCode()==200) {  
- returntrue;  
- }  
- returnfalse;         
代码分析：
     上面代码主要实现了多文件上传，为了方便服务器端保存文件，上面代码设置了名称为fileTypes的参数，fileTypes是由上传的文件类型名拼接成的字符串，如”.jpg.png.docx“；
     服务器端可以通过获取名为fileTypes的参数，然后将其拆分成字符数组，即可得到要保存文件的类型。
## 服务器端项目核心代码：
服务器段主要用到Servlet3.0的API，主要用到的方法有：
1.      request.getParameter("");//获取客户端通过addTextBody方法添加的String类型的数据。
2.      request.getPart("");//获取客户端通过**addBinaryBody、addPart、addTextBody**方法添加的指定数据，返回Part类型的对象。
3.      request.getParts();//获取客户端通过**addBinaryBody、addPart、addTextBody**方法添加的所有数据，返回**Collection<Part>**类型的对象。
4.      part.getName();//获取上传文件的名称即上传时指定的key。
5.      part.getSize()//获取上传文件的大小单位为字节。
**[java]**[view plain](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)[copy](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/481524/fork)
- String fileTypes=request.getParameter("fileTypes");//获取客户端上传的所有文件类型
- String[]typeArray=fileTypes.substring(1).split("\\.");//将文件类型字符串拆分成String数组
- try {  
-     Iterator<Part>iterator=request.getParts().iterator();  
- int count=0;  
- while (iterator.hasNext()) {//遍历客户端上传的所有文件              
- if (count>=typeArray.length)break;//如果超出文件类型数组的大小则跳出循环       
-         Part part = (Part) iterator.next();               
- //              System.out.println("part.getSize()"+part.getSize());//获取上传文件的大小
- //              System.out.println("part.getName()"+part.getName());//获取上传文件的名及添加数据时的key名
-         File file=new File("E:\\upload\\"+count+"."+typeArray[count++]);  
-         InputStream inputStream=part.getInputStream();  
-         FileOutputStream fos=new FileOutputStream(file);  
- byte[]buffer=newbyte[1024];  
- int len=0;  
- while ((len=inputStream.read(buffer))!=-1) {  
-             fos.write(buffer,0, len);  
-         }  
-         inputStream.close();  
-         fos.close();                      
-     }  
- }catch (Exception e) {  
- // TODO Auto-generated catch block
-     e.printStackTrace();  
- }  
**[java]**[view plain](http://blog.csdn.net/yan8024/article/details/46531901#)[copy](http://blog.csdn.net/yan8024/article/details/46531901#)
- String fileTypes=request.getParameter("fileTypes");//获取客户端上传的所有文件类型
- String[]typeArray=fileTypes.substring(1).split("\\.");//将文件类型字符串拆分成String数组
- try {  
-     Iterator<Part>iterator=request.getParts().iterator();  
- int count=0;  
- while (iterator.hasNext()) {//遍历客户端上传的所有文件              
- if (count>=typeArray.length)break;//如果超出文件类型数组的大小则跳出循环       
-         Part part = (Part) iterator.next();               
- //              System.out.println("part.getSize()"+part.getSize());//获取上传文件的大小
- //              System.out.println("part.getName()"+part.getName());//获取上传文件的名及添加数据时的key名
-         File file=new File("E:\\upload\\"+count+"."+typeArray[count++]);  
-         InputStream inputStream=part.getInputStream();  
-         FileOutputStream fos=new FileOutputStream(file);  
- byte[]buffer=newbyte[1024];  
- int len=0;  
- while ((len=inputStream.read(buffer))!=-1) {  
-             fos.write(buffer,0, len);  
-         }  
-         inputStream.close();  
-         fos.close();                      
-     }  
- }catch (Exception e) {  
- // TODO Auto-generated catch block
-     e.printStackTrace();  
- }  
代码分析：
         服务器端是通过Servlet实现的，通过调用request.getParameter("fileTypes")方法来获取客户端上传的所有文件类型，然后将文件类型字符串拆分成String数组。通过request.getParts()方法取出客户端通过**addBinaryBody、addPart、addTextBody**上传的所有数据，然后遍历数据集合即可进行文件的保存。
由于事先和客户端协定，添加上传文件的顺序在添加请求参数之前，所以可以根据拆分出的文件类型数组的长度判断出客户端上传文件的个数，因此当上面代码遍历超出了类型数组的长度时程序跳出循环，不再进行文件的保存，因为下面的Part都是些参数，而不是要保存的文件了。
# 程序运行效果图：
![Android使用HttpClient上传数据](https://img-my.csdn.net/uploads/201410/09/1412869959_5789.gif)
# 3.完成项目代码：
MainActivity.java
**[java]**[view plain](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)[copy](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/481524/fork)
- package com.jph.ufh.activity;  
- 
- import java.io.File;  
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import java.util.Map;  
- import com.jph.ufh.R;  
- import com.jph.ufh.service.UploadService;  
- import android.app.Activity;  
- import android.os.Bundle;  
- import android.os.Environment;  
- import android.os.Handler;  
- import android.os.Message;  
- import android.view.View;  
- import android.widget.Toast;  
- 
- /**
-  * 通过httpClient批量上传文件
-  * @author jph
-  * Date：2014.10.09  
-  */
- publicclass MainActivity extends Activity {  
- private ArrayList<File>files;  
- private Map<String, String>params;  
-     Handler mHandler=new Handler(){  
- @Override
- publicvoid handleMessage(Message msg) {  
- // TODO Auto-generated method stub
- switch (msg.what) {  
- case UploadService.UPLOAD_SUCCESS:  
-                 Toast.makeText(MainActivity.this, "上传成功", Toast.LENGTH_LONG).show();  
- break;            
-             }  
- super.handleMessage(msg);  
-         }         
-     };  
- @Override
- protectedvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.activity_main);       
-         files=new ArrayList<File>();  
-         params=new HashMap<String, String>();  
- 
-     }  
- publicvoid upload(View v) {  
-         files.clear();  
-         params.clear();  
-         File file=new File(Environment.getExternalStorageDirectory(),"kaola.jpg");  
-         File file2=new File(Environment.getExternalStorageDirectory(),"test.docx");  
-         File file3=new File(Environment.getExternalStorageDirectory(),"test.jpg");  
-         files.add(file);  
-         files.add(file2);  
-         files.add(file3);  
-         StringBuffer sbFileTypes=new StringBuffer();  
- for (File tempFile:files) {  
-             String fileName=tempFile.getName();  
-             sbFileTypes.append(getFileType(fileName));            
-         }  
-         params.put("fileTypes",sbFileTypes.toString());  
-         params.put("method", "upload");  
-         UploadService uploadService=new UploadService(mHandler);  
-         uploadService.uploadFileToServer(params, files);  
-     }  
- /**
-      * 获取文件的类型
-      * @param fileName ：文件名
-      * @return 文件类型
-      */
- private String getFileType(String fileName) {  
- // TODO Auto-generated method stub
- return fileName.substring(fileName.lastIndexOf("."), fileName.length());  
-     }  
- }  
**[java]**[view plain](http://blog.csdn.net/yan8024/article/details/46531901#)[copy](http://blog.csdn.net/yan8024/article/details/46531901#)
- package com.jph.ufh.activity;  
- 
- import java.io.File;  
- import java.util.ArrayList;  
- import java.util.HashMap;  
- import java.util.Map;  
- import com.jph.ufh.R;  
- import com.jph.ufh.service.UploadService;  
- import android.app.Activity;  
- import android.os.Bundle;  
- import android.os.Environment;  
- import android.os.Handler;  
- import android.os.Message;  
- import android.view.View;  
- import android.widget.Toast;  
- 
- /**
-  * 通过httpClient批量上传文件
-  * @author jph
-  * Date：2014.10.09  
-  */
- publicclass MainActivity extends Activity {  
- private ArrayList<File>files;  
- private Map<String, String>params;  
-     Handler mHandler=new Handler(){  
- @Override
- publicvoid handleMessage(Message msg) {  
- // TODO Auto-generated method stub
- switch (msg.what) {  
- case UploadService.UPLOAD_SUCCESS:  
-                 Toast.makeText(MainActivity.this, "上传成功", Toast.LENGTH_LONG).show();  
- break;            
-             }  
- super.handleMessage(msg);  
-         }         
-     };  
- @Override
- protectedvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.activity_main);       
-         files=new ArrayList<File>();  
-         params=new HashMap<String, String>();  
- 
-     }  
- publicvoid upload(View v) {  
-         files.clear();  
-         params.clear();  
-         File file=new File(Environment.getExternalStorageDirectory(),"kaola.jpg");  
-         File file2=new File(Environment.getExternalStorageDirectory(),"test.docx");  
-         File file3=new File(Environment.getExternalStorageDirectory(),"test.jpg");  
-         files.add(file);  
-         files.add(file2);  
-         files.add(file3);  
-         StringBuffer sbFileTypes=new StringBuffer();  
- for (File tempFile:files) {  
-             String fileName=tempFile.getName();  
-             sbFileTypes.append(getFileType(fileName));            
-         }  
-         params.put("fileTypes",sbFileTypes.toString());  
-         params.put("method", "upload");  
-         UploadService uploadService=new UploadService(mHandler);  
-         uploadService.uploadFileToServer(params, files);  
-     }  
- /**
-      * 获取文件的类型
-      * @param fileName ：文件名
-      * @return 文件类型
-      */
- private String getFileType(String fileName) {  
- // TODO Auto-generated method stub
- return fileName.substring(fileName.lastIndexOf("."), fileName.length());  
-     }  
- }  
**UploadService.java**
**[java]**[view plain](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)[copy](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)[print](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)[?](http://blog.csdn.net/fengyuzhengfan/article/details/39941851#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/481524/fork)
- package com.jph.ufh.service;  
- 
- import java.io.File;  
- import java.io.IOException;  
- import java.util.ArrayList;  
- import java.util.Map;  
- import org.apache.http.HttpEntity;  
- import org.apache.http.HttpResponse;  
- import org.apache.http.client.ClientProtocolException;  
- import org.apache.http.client.HttpClient;  
- import org.apache.http.client.methods.HttpPost;  
- import org.apache.http.entity.mime.HttpMultipartMode;  
- import org.apache.http.entity.mime.MultipartEntityBuilder;  
- import org.apache.http.impl.client.DefaultHttpClient;  
- 
- import android.os.Handler;  
- 
- /**
-  * 采用HttpClient上传文件,支持多文件上传
-  * @author jph
-  * Date:2014.10.09
-  */
- publicclass UploadService {  
- privatestatic String url="http://10.219.57.16:8080/ServerForUpload/ServletForUpload";  
- //  private static String url="http://10.110.6.58:8080/ServerForUpload/ServletForUpload";
- publicstaticfinalint UPLOAD_SUCCESS=0x123;  
- publicstaticfinalint UPLOAD_FAIL=0x124;  
- private Handler handler;  
- public UploadService(Handler handler) {  
- // TODO Auto-generated constructor stub
- this.handler=handler;  
-     }     
- /**
-      * @param params 请求参数，包括请求的的方法参数method如：“upload”，
-      * 请求上传的文件类型fileTypes如：“.jpg.png.docx”
-      * @param files 要上传的文件集合
-      */
- publicvoid uploadFileToServer(final Map<String, String> params, final ArrayList<File>files) {  
- // TODO Auto-generated method stub  
- new Thread(new Runnable() {           
- @Override
- publicvoid run() {  
- // TODO Auto-generated method stub
- try {  
- if (uploadFiles(url,params,files)) {  
-                         handler.sendEmptyMessage(UPLOAD_SUCCESS);//通知主线程数据发送成功
-                     }else {  
- //将数据发送给服务器失败
-                     }  
-                 } catch (Exception e) {  
- // TODO Auto-generated catch block
-                     e.printStackTrace();  
-                 }                 
-             }  
-         }).start();  
-     }  
- /**
-      * @param url servlet的地址
-      * @param params 要传递的参数
-      * @param files 要上传的文件
-      * @return true if upload success else false
-      * @throws ClientProtocolException
-      * @throws IOException
-      */
- privateboolean uploadFiles(String url,Map<String, String>params,ArrayList<File>files) throws ClientProtocolException, IOException {  
-         HttpClient client=new DefaultHttpClient();// 开启一个客户端 HTTP 请求 
-         HttpPost post = new HttpPost(url);//创建 HTTP POST 请求  
-         MultipartEntityBuilder builder = MultipartEntityBuilder.create();  
- //      builder.setCharset(Charset.forName("uft-8"));//设置请求的编码格式
-         builder.setMode(HttpMultipartMode.BROWSER_COMPATIBLE);//设置浏览器兼容模式
- int count=0;  
- for (File file:files) {  
- //          FileBody fileBody = new FileBody(file);//把文件转换成流对象FileBody
- //          builder.addPart("file"+count, fileBody);
-             builder.addBinaryBody("file"+count, file);  
-             count++;  
-         }         
-         builder.addTextBody("method", params.get("method"));//设置请求参数
-         builder.addTextBody("fileTypes", params.get("fileTypes"));//设置请求参数
-         HttpEntity entity = builder.build();// 生成 HTTP POST 实体      
-         post.setEntity(entity);//设置请求参数
-         HttpResponse response = client.execute(post);// 发起请求 并返回请求的响应
- if (response.getStatusLine().getStatusCode()==200) {  
- returntrue;  
-         }  
- returnfalse;         
-     }  
- }  
转载于： :[http://blog.csdn.net/fengyuzhengfan/article/details/39941851](http://blog.csdn.net/fengyuzhengfan/article/details/39941851)
以下待研究：[http://blog.csdn.net/zhouzme/article/details/18940279](http://blog.csdn.net/zhouzme/article/details/18940279)、htp://my.oschina.net/weichou/blog/352753
[](http://my.oschina.net/weichou/blog/352753)
[http://topmanopensource.iteye.com/blog/1605238](http://topmanopensource.iteye.com/blog/1605238)（使用HttpurlConnection上传文件）
