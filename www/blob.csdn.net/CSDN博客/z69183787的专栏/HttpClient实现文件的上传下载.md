# HttpClient实现文件的上传下载 - z69183787的专栏 - CSDN博客
2015年07月17日 14:22:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2095
## 1 HTTP
    HTTP 协议可能是现在 Internet 上使用得最多、最重要的协议了，越来越多的 Java 应用程序需要直接通过 HTTP 协议来访问网络资源。
    虽然在 JDK 的 java.net 包中已经提供了访问 HTTP 协议的基本功能，但是对于大部分应用程序来说，JDK 库本身提供的功能还不够丰富和灵活。HttpClient 用来提供高效的、最新的、功能丰富的支持 HTTP 协议的客户端编程工具包，并且它支持 HTTP 协议最新的版本和建议。
    一般的情况下我们都是使用Chrome或者其他浏览器来访问一个WEB服务器，用来浏览页面查看信息或者提交一些数据、文件上传下载等等。所访问的这些页面有的仅仅是一些普通的页面，有的需要用户登录后方可使用，或者需要认证以及是一些通过加密方式传输，例如HTTPS。目前我们使用的浏览器处理这些情况都不会构成问题。但是一旦我们有需求不通过浏览器来访问服务器的资源呢？那该怎么办呢？
    下面以本地客户端发起文件的上传、下载为例做个小Demo。HttpClient有两种形式，一种是org.apache.http下的，一种是org.apache.commons.httpclient.HttpClient。
## 2 文件上传
    文件上传可以使用两种方式实现，一种是PostMethod方式，一种是HttpPost方式。两者的处理大同小异。PostMethod是使用FileBody将文件包装流包装起来，HttpPost是使用FilePart将文件流包装起来。在传递文件流给服务端的时候，都可以同时传递其他的参数。
### 2.1 客户端处理
#### 2.1.1 PostMethod方式
     将文件封装到FilePart中，放入Part数组，同时，其他参数可以放入StringPart中，这里没有写，只是单纯的将参数以setParameter的方式进行设置。此处的HttpClient是org.apache.commons.httpclient.HttpClient。
```java
public void upload(String localFile){
	File file = new File(localFile);
          PostMethod filePost = new PostMethod(URL_STR);
          HttpClient client = new HttpClient();
          
          try {
              // 通过以下方法可以模拟页面参数提交
              filePost.setParameter("userName", userName);
              filePost.setParameter("passwd", passwd);
 
             Part[] parts = { new FilePart(file.getName(), file) };
             filePost.setRequestEntity(new MultipartRequestEntity(parts, filePost.getParams()));
             
             client.getHttpConnectionManager().getParams().setConnectionTimeout(5000);
             
             int status = client.executeMethod(filePost);
             if (status == HttpStatus.SC_OK) {
                 System.out.println("上传成功");
             } else {
                 System.out.println("上传失败");
             }
         } catch (Exception ex) {
             ex.printStackTrace();
         } finally {
             filePost.releaseConnection();
         }
     }
```
记得搞完之后，要通过releaseConnection释放连接。 
#### 2.1.2 HttpPost方式
     这种方式，与上面类似，只不过变成了FileBody。上面的Part数组在这里对应HttpEntity。此处的HttpClient是org.apache.http.client.methods下的。
```java
public void upload(String localFile){
          CloseableHttpClient httpClient = null;
          CloseableHttpResponse response = null;
          try {
              httpClient = HttpClients.createDefault();
              
              // 把一个普通参数和文件上传给下面这个地址 是一个servlet
              HttpPost httpPost = new HttpPost(URL_STR);
              
             // 把文件转换成流对象FileBody
             FileBody bin = new FileBody(new File(localFile));
 
             StringBody userName = new StringBody("Scott", ContentType.create(
                     "text/plain", Consts.UTF_8));
             StringBody password = new StringBody("123456", ContentType.create(
                     "text/plain", Consts.UTF_8));
 
             HttpEntity reqEntity = MultipartEntityBuilder.create()
                     // 相当于<input type="file" name="file"/>
                     .addPart("file", bin)
                     
                     // 相当于<input type="text" name="userName" value=userName>
                     .addPart("userName", userName)
                     .addPart("pass", password)
                     .build();
 
             httpPost.setEntity(reqEntity);
 
             // 发起请求 并返回请求的响应
             response = httpClient.execute(httpPost);
             
             System.out.println("The response value of token:" + response.getFirstHeader("token"));
                 
             // 获取响应对象
             HttpEntity resEntity = response.getEntity();
             if (resEntity != null) {
                 // 打印响应长度
                 System.out.println("Response content length: " + resEntity.getContentLength());
                 // 打印响应内容
                 System.out.println(EntityUtils.toString(resEntity, Charset.forName("UTF-8")));
             }
             
             // 销毁
             EntityUtils.consume(resEntity);
         }catch (Exception e){
             e.printStackTrace();
         }finally {
             try {
                 if(response != null){
                     response.close();
                 }
             } catch (IOException e) {
                 e.printStackTrace();
             }
             
             try {
                 if(httpClient != null){
                     httpClient.close();
                 }
             } catch (IOException e) {
                 e.printStackTrace();
             }
         }
     }
```
### 2.2 服务端处理
     无论客户端是哪种上传方式，服务端的处理都是一样的。在通过HttpServletRequest获得参数之后，把得到的Item进行分类，分为普通的表单和File表单。    
     通过ServletFileUpload 可以设置上传文件的大小及编码格式等。
     总之，服务端的处理是把得到的参数当做HTML表单进行处理的。     
```java
public void processUpload(HttpServletRequest request, HttpServletResponse response){
          File uploadFile = new File(uploadPath);
          if (!uploadFile.exists()) {
              uploadFile.mkdirs();
          }
  
          System.out.println("Come on, baby .......");
          
          request.setCharacterEncoding("utf-");  
         response.setCharacterEncoding("utf-");  
           
         //检测是不是存在上传文件  
         boolean isMultipart = ServletFileUpload.isMultipartContent(request);  
           
         if(isMultipart){  
             DiskFileItemFactory factory = new DiskFileItemFactory();  
             
             //指定在内存中缓存数据大小,单位为byte,这里设为Mb  
             factory.setSizeThreshold(*);  
            
             //设置一旦文件大小超过getSizeThreshold()的值时数据存放在硬盘的目录   
             factory.setRepository(new File("D:\\temp"));  
             
             // Create a new file upload handler  
             ServletFileUpload upload = new ServletFileUpload(factory);  
            
             // 指定单个上传文件的最大尺寸,单位:字节，这里设为Mb    
             upload.setFileSizeMax( *  * );    
             
             //指定一次上传多个文件的总尺寸,单位:字节，这里设为Mb  
             upload.setSizeMax( *  * );     
             upload.setHeaderEncoding("UTF-");
               
             List<FileItem> items = null;  
               
             try {  
                 // 解析request请求  
                 items = upload.parseRequest(request);  
             } catch (FileUploadException e) {  
                 e.printStackTrace();  
             }  
             
             if(items!=null){  
                 //解析表单项目  
                 Iterator<FileItem> iter = items.iterator();  
                 while (iter.hasNext()) {  
                     FileItem item = iter.next(); 
                     
                     //如果是普通表单属性  
                     if (item.isFormField()) {  
                         //相当于input的name属性   <input type="text" name="content">  
                         String name = item.getFieldName();
                         
                         //input的value属性  
                         String value = item.getString();
                         
                         System.out.println("属性:" + name + " 属性值:" + value);  
                     }  
                     //如果是上传文件  
                     else {  
                         //属性名  
                         String fieldName = item.getFieldName();  
                         
                         //上传文件路径  
                         String fileName = item.getName();  
                         fileName = fileName.substring(fileName.lastIndexOf("/") + );// 获得上传文件的文件名  
                         
                         try {  
                             item.write(new File(uploadPath, fileName));  
                        } catch (Exception e) {  
                             e.printStackTrace();  
                        }  
                     } 
                 }  
             }  
         }  
         
         response.addHeader("token", "hello");
     }
```
服务端在处理之后，可以在Header中设置返回给客户端的简单信息。如果返回客户端是一个流的话，流的大小必须提前设置！
response.setContentLength((int) file.length());
## 3 文件下载
     文件的下载可以使用HttpClient的GetMethod实现，还可以使用HttpGet方式、原始的HttpURLConnection方式。
### 3.1 客户端处理
####  3.1.1 GetMethod方式
此处的HttpClient是org.apache.commons.httpclient.HttpClient。
```java
public void downLoad(String remoteFileName, String localFileName) {
          HttpClient client = new HttpClient();
          GetMethod get = null;
          FileOutputStream output = null;
          
          try {
              get = new GetMethod(URL_STR);
              get.setRequestHeader("userName", userName);
              get.setRequestHeader("passwd", passwd);
             get.setRequestHeader("fileName", remoteFileName);
 
             int i = client.executeMethod(get);
 
             if (SUCCESS == i) {
                 System.out.println("The response value of token:" + get.getResponseHeader("token"));
 
                 File storeFile = new File(localFileName);
                 output = new FileOutputStream(storeFile);
                 
                 // 得到网络资源的字节数组,并写入文件
                 output.write(get.getResponseBody());
             } else {
                 System.out.println("DownLoad file occurs exception, the error code is :" + i);
             }
         } catch (Exception e) {
             e.printStackTrace();
         } finally {
             try {
                 if(output != null){
                     output.close();
                 }
             } catch (IOException e) {
                 e.printStackTrace();
             }
             
             get.releaseConnection();
             client.getHttpConnectionManager().closeIdleConnections();
         }
     }
```
#### 3.1.2 HttpGet方式
此处的HttpClient是org.apache.http.client.methods下的。
```java
public void downLoad(String remoteFileName, String localFileName) {
          DefaultHttpClient httpClient = new DefaultHttpClient();
          OutputStream out = null;
          InputStream in = null;
          
          try {
              HttpGet httpGet = new HttpGet(URL_STR);
  
              httpGet.addHeader("userName", userName);
             httpGet.addHeader("passwd", passwd);
             httpGet.addHeader("fileName", remoteFileName);
 
             HttpResponse httpResponse = httpClient.execute(httpGet);
             HttpEntity entity = httpResponse.getEntity();
             in = entity.getContent();
 
             long length = entity.getContentLength();
             if (length <= ) {
                 System.out.println("下载文件不存在！");
                 return;
             }
 
             System.out.println("The response value of token:" + httpResponse.getFirstHeader("token"));
 
             File file = new File(localFileName);
             if(!file.exists()){
                 file.createNewFile();
             }
             
             out = new FileOutputStream(file);  
             byte[] buffer = new byte[];
             int readLength = ;
             while ((readLength=in.read(buffer)) > ) {
                 byte[] bytes = new byte[readLength];
                 System.arraycopy(buffer, , bytes, , readLength);
                 out.write(bytes);
             }
             
             out.flush();
             
         } catch (IOException e) {
             e.printStackTrace();
         } catch (Exception e) {
             e.printStackTrace();
         }finally{
             try {
                 if(in != null){
                     in.close();
                 }
             } catch (IOException e) {
                 e.printStackTrace();
             }
             
             try {
                 if(out != null){
                     out.close();
                 }
             } catch (IOException e) {
                 e.printStackTrace();
             }
         }
     }
```
#### 3.1.3 HttpURLConnection方式
```java
<span style="font-size:14px;">public void download(String remoteFileName, String localFileName) {
          FileOutputStream out = null;
          InputStream in = null;
          
          try{
              URL url = new URL(URL_STR);
              URLConnection urlConnection = url.openConnection();
              HttpURLConnection httpURLConnection = (HttpURLConnection) urlConnection;
              
             // true -- will setting parameters
             httpURLConnection.setDoOutput(true);
             // true--will allow read in from
             httpURLConnection.setDoInput(true);
             // will not use caches
             httpURLConnection.setUseCaches(false);
             // setting serialized
             httpURLConnection.setRequestProperty("Content-type", "application/x-java-serialized-object");
             // default is GET                        
             httpURLConnection.setRequestMethod("POST");
             httpURLConnection.setRequestProperty("connection", "Keep-Alive");
             httpURLConnection.setRequestProperty("Charsert", "UTF-");
             //  min
             httpURLConnection.setConnectTimeout();
             //  min
             httpURLConnection.setReadTimeout();
 
             httpURLConnection.addRequestProperty("userName", userName);
             httpURLConnection.addRequestProperty("passwd", passwd);
             httpURLConnection.addRequestProperty("fileName", remoteFileName);
 
             // connect to server (tcp)
             httpURLConnection.connect();
 
             in = httpURLConnection.getInputStream();// send request to
                                                                 // server
             File file = new File(localFileName);
             if(!file.exists()){
                 file.createNewFile();
             }
 
             out = new FileOutputStream(file);  
             byte[] buffer = new byte[];
             int readLength = ;
             while ((readLength=in.read(buffer)) > ) {
                 byte[] bytes = new byte[readLength];
                 System.arraycopy(buffer, , bytes, , readLength);
                 out.write(bytes);
             }
             
             out.flush();
         }catch(Exception e){
             e.printStackTrace();
         }finally{
             try {
                 if(in != null){
                     in.close();
                 }
             } catch (IOException e) {
                 e.printStackTrace();
             }
             
             try {
                 if(out != null){
                     out.close();
                 }
             } catch (IOException e) {
                 e.printStackTrace();
             }
         }
     }</span>
```
### 3.2 服务端处理
     尽管客户端的处理方式不同，但是服务端是一样的。
```java
public void processDownload(HttpServletRequest request, HttpServletResponse response){
          int BUFFER_SIZE = ;
          InputStream in = null;
          OutputStream out = null;
          
          System.out.println("Come on, baby .......");
          
          try{
              request.setCharacterEncoding("utf-");  
             response.setCharacterEncoding("utf-");  
             response.setContentType("application/octet-stream");
             
             String userName = request.getHeader("userName");
             String passwd = request.getHeader("passwd");
             String fileName = request.getHeader("fileName");
             
             System.out.println("userName:" + userName);
             System.out.println("passwd:" + passwd);
             System.out.println("fileName:" + fileName);
             
             //可以根据传递来的userName和passwd做进一步处理，比如验证请求是否合法等             
             File file = new File(downloadPath + "\\" + fileName);
             response.setContentLength((int) file.length());
             response.setHeader("Accept-Ranges", "bytes");
             
             int readLength = ;
             
             in = new BufferedInputStream(new FileInputStream(file), BUFFER_SIZE);
             out = new BufferedOutputStream(response.getOutputStream());
             
             byte[] buffer = new byte[BUFFER_SIZE];
             while ((readLength=in.read(buffer)) > ) {
                 byte[] bytes = new byte[readLength];
                 System.arraycopy(buffer, , bytes, , readLength);
                 out.write(bytes);
             }
             
             out.flush();
             
             response.addHeader("token", "hello ");
              
         }catch(Exception e){
             e.printStackTrace();
              response.addHeader("token", "hello ");
         }finally {
             if (in != null) {
                 try {
                     in.close();
                 } catch (IOException e) {
                 }
             }
             if (out != null) {
                 try {
                     out.close();
                 } catch (IOException e) {
                 }
             }
         }
     }
```
##  4 小结
    HttpClient最基本的功能就是执行Http方法。一个Http方法的执行涉及到一个或者多个Http请求/Http响应的交互，通常这个过程都会自动被HttpClient处理，对用户透明。用户只需要提供Http请求对象，HttpClient就会将http请求发送给目标服务器，并且接收服务器的响应，如果http请求执行不成功，httpclient就会抛出异常。所以在写代码的时候注意finally的处理。    
    所有的Http请求都有一个请求列（request line），包括方法名、请求的URI和Http版本号。HttpClient支持HTTP/1.1这个版本定义的所有Http方法：GET,HEAD,POST,PUT,DELETE,TRACE和OPTIONS。上面的上传用到了Post，下载是Get。
    目前来说，使用org.apache.commons.httpclient.HttpClient多一些。看自己了~
