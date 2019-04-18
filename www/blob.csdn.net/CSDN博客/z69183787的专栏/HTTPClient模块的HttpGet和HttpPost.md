# HTTPClient模块的HttpGet和HttpPost - z69183787的专栏 - CSDN博客
2015年07月17日 14:15:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1766
     Android SDK集成了Apache HttpClient模块。要注意的是，这里的Apache HttpClient模块是HttpClient 4.0（org.apache.http.*），而不是常见的Jakarta Commons HttpClient 3.x（org.apache.commons.httpclient.*）。
           HttpClient常用HttpGet和HttpPost这两个类，分别对应Get方式和Post方式。

           无论是使用HttpGet，还是使用HttpPost，都必须通过如下3步来访问HTTP资源。
           1.创建HttpGet或HttpPost对象，将要请求的URL通过构造方法传入HttpGet或HttpPost对象。
           2.使用DefaultHttpClient类的execute方法发送HTTP GET或HTTP POST请求，并返回HttpResponse对象。
           3.通过HttpResponse接口的getEntity方法返回响应信息，并进行相应的处理。
           如果使用HttpPost方法提交HTTP POST请求，则需要使用HttpPost类的setEntity方法设置请求参数。参数则必须用NameValuePair[]数组存储。
**   HttpGet**
**[java]**[view
 plain](http://blog.csdn.net/caesardadi/article/details/8621595#)[copy](http://blog.csdn.net/caesardadi/article/details/8621595#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/323498/fork)
- public String doGet()  
-     {  
-         String uriAPI = "http://XXXXX?str=I+am+get+String";  
-         String result= "";  
- //      HttpGet httpRequst = new HttpGet(URI uri);
- //      HttpGet httpRequst = new HttpGet(String uri);
- //      创建HttpGet或HttpPost对象，将要请求的URL通过构造方法传入HttpGet或HttpPost对象。
-         HttpGet httpRequst = new HttpGet(uriAPI);  
- 
- //      new DefaultHttpClient().execute(HttpUriRequst requst);
- try {  
- //使用DefaultHttpClient类的execute方法发送HTTP GET请求，并返回HttpResponse对象。
-             HttpResponse httpResponse = new DefaultHttpClient().execute(httpRequst);//其中HttpGet是HttpUriRequst的子类
- if(httpResponse.getStatusLine().getStatusCode() == 200)  
-             {  
-                 HttpEntity httpEntity = httpResponse.getEntity();  
-                 result = EntityUtils.toString(httpEntity);//取出应答字符串
- // 一般来说都要删除多余的字符 
-                 result.replaceAll("\r", "");//去掉返回结果中的"\r"字符，否则会在结果字符串后面显示一个小方格  
-             }  
- else
-                         httpRequst.abort();  
-            } catch (ClientProtocolException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-             result = e.getMessage().toString();  
-         } catch (IOException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-             result = e.getMessage().toString();  
-         }  
- return result;  
-     }  
**  HttpPost**
              如果使用HttpPost方法提交HTTP POST请求，则需要使用HttpPost类的setEntity方法设置请求参数。参数则必须用NameValuePair[]数组存储。
**[java]**[view
 plain](http://blog.csdn.net/caesardadi/article/details/8621595#)[copy](http://blog.csdn.net/caesardadi/article/details/8621595#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/323498/fork)
- public String doPost()  
-     {  
-         String uriAPI = "http://XXXXXX";//Post方式没有参数在这里  
-         String result = "";  
-         HttpPost httpRequst = new HttpPost(uriAPI);//创建HttpPost对象
- 
-         List <NameValuePair> params = new ArrayList<NameValuePair>();  
-         params.add(new BasicNameValuePair("str", "I am Post String"));  
- 
- try {  
-             httpRequst.setEntity(new UrlEncodedFormEntity(params,HTTP.UTF_8));  
-             HttpResponse httpResponse = new DefaultHttpClient().execute(httpRequst);  
- if(httpResponse.getStatusLine().getStatusCode() == 200)  
-             {  
-                 HttpEntity httpEntity = httpResponse.getEntity();  
-                 result = EntityUtils.toString(httpEntity);//取出应答字符串
-             }  
-         } catch (UnsupportedEncodingException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-             result = e.getMessage().toString();  
-         }  
- catch (ClientProtocolException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-             result = e.getMessage().toString();  
-         }  
- catch (IOException e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-             result = e.getMessage().toString();  
-         }  
- return result;  
-     }  
          以发送连接请求时，需要设置链接超时和请求超时等参数，否则会长期停止或者崩溃。
**[java]**[view
 plain](http://blog.csdn.net/caesardadi/article/details/8621595#)[copy](http://blog.csdn.net/caesardadi/article/details/8621595#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/323498/fork)
- HttpParams httpParameters = new BasicHttpParams();  
- HttpConnectionParams.setConnectionTimeout(httpParameters, 10*1000);//设置请求超时10秒
- HttpConnectionParams.setSoTimeout(httpParameters, 10*1000); //设置等待数据超时10秒
- HttpConnectionParams.setSocketBufferSize(params, 8192);  
- HttpClient httpclient = new DefaultHttpClient(httpParameters); //此时构造DefaultHttpClient时将参数传入
- 
- 
- 
- 由于是联网，在AndroidManifest.xml中添加网络连接的权限  
- <uses-permission android:name="android.permission.INTERNET"/>  
- 
