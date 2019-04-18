# Http通信 - 深之JohnChen的专栏 - CSDN博客

2013年08月03日 15:04:22[byxdaz](https://me.csdn.net/byxdaz)阅读数：4686


一、Android平台网络相关API接口

a) java.net.*(标准Java接口)

java.net.*提供与联网有关的类，包括流、数据包套接字(socket)、Internet协议、常见Http处理等。比如：创建URL，以及URLConnection/HttpURLConnection对象、设置链接参数、链接到服务器、向服务器写数据、从服务器读取数据等通信。这些在Java网络编程中均有涉及。

b) Org.apache接口

对于大部分应用程序而言JDK本身提供的网络功能已远远不够，这时就需要Android提供的Apache HttpClient了。它是一个开源项目，功能更加完善，为客户端的Http编程提供高效、最新、功能丰富的工具包支持。

c)Android.net.*(Android网络接口)

常常使用此包下的类进行Android特有的网络编程，如：访问WiFi，访问Android联网信息，邮件等功能。

二、服务器端返回客户端的内容有三种方式

a）以HTML代码的形式返回。

b）以XML字符串的形式返回，做Android开发时这种方式比较多。返回的数据需要通过XML解析（SAX、DOM，Pull,等）器进行解析。

c）以json对象的方式返回。

三、HTTP程序

Android中提供的HttpURLConnection和HttpClient接口可以用来开发HTTP程序。

1、
HttpURLConnection接口
首先需要明确的是，Http通信中的POST和GET请求方式的不同。GET可以获得静态页面，也可以把参数放在URL字符串后面，传递给服务器。而POST方法的参数是放在Http请求中。因此，在编程之前，应当首先明确使用的请求方法，然后再根据所使用的方式选择相应的编程方式。

1.1 HttpURLConnection连接URL
创建一个URL对象

URL url = new URL([http://www.baidu.com](http://www.baidu.com/));

利用HttpURLConnection对象从网络中获取网页数据

HttpURLConnection conn = (HttpURLConnection) url.openConnection();

设置连接超时，如果网络不好,Android系统在超过默认时间会收回资源中断操作.

conn.setConnectTimeout(6*1000);

对响应码进行判断

if (conn.getResponseCode() != 200) //从Internet获取网页,发送请求,将网页以流的形式读回来，-返回的响应码200,是成功

throw new RuntimeException("请求url失败");

得到网络返回的输入流

InputStream is = conn.getInputStream();

String result = readData(is, "GBK"); //文件流输入出文件用outStream.write

conn.disconnect();

1.2向服务器端发送请求参数
步骤：
1)创建URL对象:URL realUrl = new URL(requestUrl);

2)通过HttpURLConnection对象,向网络地址发送请求

HttpURLConnection conn = (HttpURLConnection) realUrl.openConnection();

3)设置允许输出:conn.setDoOutput(true);

4)设置不使用缓存:conn.setUseCaches(false);

5)设置使用POST的方式发送:conn.setRequestMethod("POST");

6)设置维持长连接:conn.setRequestProperty("Connection", "Keep-Alive");

7)设置文件字符集:conn.setRequestProperty("Charset", "UTF-8");

8)设置文件长度:conn.setRequestProperty("Content-Length", String.valueOf(data.length));

9)设置文件类型:conn.setRequestProperty("Content-Type","application/x-www-form-urlencoded");

10）设置HTTP请求头

conn.setRequestProperty("Accept“，” image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/x-shockwave-flash, application/xaml+xml, application/vnd.ms-xpsdocument,
 application/x-ms-xbap, application/x-ms-application, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*"）；

设置语言：conn.setRequestProperty("Accept-Language“，"zh-CN"）；

conn.setRequestProperty("

conn.setRequestProperty("

11)以流的方式输出.

1.3.向服务器端发送xml数据（也称为实体Entity）
XML格式是通信的标准语言,Android系统也可以通过发送XML文件传输数据.

1)将生成的XML文件写入到byte数组中,并设置为UTF-8:byte[] xmlbyte
 = xml.toString().getBytes("UTF-8");

2)创建URL对象,并指定地址和参数:URL url = new URL([http://localhost:8080/itcast/contanctmanage.do?method=readxml](http://localhost:8080/itcast/contanctmanage.do?method=readxml));

3)获得链接:HttpURLConnection conn = (HttpURLConnection) url.openConnection();

4)设置连接超时:conn.setConnectTimeout(6* 1000);

5)设置允许输出conn.setDoOutput(true);

6)设置不使用缓存:conn.setUseCaches(false);

7)设置以POST方式传输:conn.setRequestMethod("POST");

8)维持长连接:conn.setRequestProperty("Connection", "Keep-Alive");

9)设置字符集:conn.setRequestProperty("Charset", "UTF-8");

10)设置文件的总长度:conn.setRequestProperty("Content-Length", String.valueOf(xmlbyte.length));

11)设置文件类型:conn.setRequestProperty("Content-Type","text/xml; charset=UTF-8");

12)以文件流的方式发送xml数据:outStream.write(xmlbyte);
总结:

--在对大文件的操作时,要将文件写到SDCard上面,不要直接写到手机内存上.

--操作大文件是,要一遍从网络上读,一遍要往SDCard上面写,减少手机内存的使用.

--对文件流操作完,要记得及时关闭.

--发送POST请求必须设置允许输出
--不要使用缓存,容易出现问题.

--在开始用HttpURLConnection对象的setRequestProperty()设置,就是生成HTML文件头.

--我们使用的是用HTML的方式传输文件,这个方式只能传输一般在5以下的文件.

--传输大文件不适合用HTML的方式,传输大文件我们要面向Socket编程.确保程序的稳定性
--将地址和参数存到byte数组中:byte[] data = params.toString().getBytes();

2、利用Apache的HttpClient实现Android客户端发送实体Entity

以上为直接利用HTTP协议来实现的，其实Android已经集成了第三方开源项目-------org.apache.http.client.HttpClient，可以直接参考它提供的API使用。

// http地址

String httpUrl = "http://192.168.1.110:8080/httpget.jsp?par=HttpClient_android_Get"; 

//HttpGet连接对象

HttpGet httpRequest = new HttpGet(httpUrl); 

//取得HttpClient对象

HttpClient httpclient = new DefaultHttpClient();  

//请求HttpClient，取得HttpResponse 

HttpResponse httpResponse = httpclient.execute(httpRequest); 

//请求成功

if (httpResponse.getStatusLine().getStatusCode() == HttpStatus.SC_OK) 

{  

//取得返回的字符串

String strResult = EntityUtils.toString(httpResponse.getEntity()); 

mTextView.setText(strResult); 

}  

else 

{  

mTextView.setText("请求错误!"); 

}  

}

使用POST方法进行参数传递时，需要使用NameValuePair来保存要传递的参数。另外，还需要设置所使用的字符集。

// http地址

 String httpUrl = "http://192.168.1.110:8080/httpget.jsp"; 

//HttpPost连接对象

 HttpPost httpRequest = new HttpPost(httpUrl); 

 //使用NameValuePair来保存要传递的Post参数

 List<NameValuePair> params = new ArrayList<NameValuePair>(); 

 //添加要传递的参数

 params.add(new BasicNameValuePair("par", "HttpClient_android_Post")); 

 //设置字符集

 HttpEntity httpentity = new UrlEncodedFormEntity(params, "gb2312"); 

 //请求httpRequest 

  httpRequest.setEntity(httpentity); 

  //取得默认的HttpClient 

  HttpClient httpclient = new DefaultHttpClient(); 

  //取得HttpResponse 

  HttpResponse httpResponse = httpclient.execute(httpRequest); 

  //HttpStatus.SC_OK表示连接成功

  if (httpResponse.getStatusLine().getStatusCode() == HttpStatus.SC_OK) 

  {  

   //取得返回的字符串

   String strResult = EntityUtils.toString(httpResponse.getEntity()); 

   mTextView.setText(strResult); 

   }  

   else 

   {  

     mTextView.setText("请求错误!"); 

   }  

