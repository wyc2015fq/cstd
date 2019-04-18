# apache的MultipartEntityBuilder文件上传 - z69183787的专栏 - CSDN博客
2017年11月23日 17:34:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1441
[https://www.cnblogs.com/dennyzhangdd/p/6808321.html](https://www.cnblogs.com/dennyzhangdd/p/6808321.html)
本文讲解多文件上传方法，不比较上传有几种方法和效率，而是定向分析apache的httpmime包的MultipartEntityBuilder类，源码包：httpmime-4.5.2.jar
目录
1.多文件上传常用用法
2.注意点
=======正文分割线===========
## 一、常用用法
### 1.上传文件
三个传值方法，addPart、addBinaryBody、addTextBody如下图：
```
1 MultipartEntityBuilder entityBuilder = MultipartEntityBuilder.create();
2 entityBuilder.addBinaryBody("file",new byte[]{},ContentType.DEFAULT_BINARY,"file.jpg");
3 entityBuilder.addPart("owner","111");
4 entityBuilder.addTextBody("paramter1", "aaa");
```
### 2.接收文件
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 //将当前上下文初始化给  CommonsMutipartResolver （多部分解析器）
 2 CommonsMultipartResolver multipartResolver=new CommonsMultipartResolver(request.getSession().getServletContext());
 3  //判断是否多文件上传
 4  if(multipartResolver.isMultipart(request)){
 5      //将request变成多部分request
 6      MultipartHttpServletRequest multiRequest=(MultipartHttpServletRequest)request;
 7      Map<String,Object> map=multiRequest.getParameterMap();
 8      //获取multiRequest 中所有的文件名
 9      Iterator<String> iter=multiRequest.getFileNames();
10      //遍历所有文件
11      while(iter.hasNext()){
12         MultipartFile file=multiRequest.getFile(iter.next().toString());
13         //to do
14      }
15  }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
## 二、注意点
**注意点1，使用addBinaryBody的坑**
首先看看源码：
MultipartEntityBuilder 的 addBinaryBody 有6种不同用法，一般都是上传File即可。着重看下图2个红色名字的方法。
源码如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1     public MultipartEntityBuilder addBinaryBody(
 2             final String name, final byte[] b, final ContentType contentType, final String filename) {
 3         return addPart(name, new ByteArrayBody(b, contentType, filename));
 4     }
 5 
 6     public MultipartEntityBuilder addBinaryBody(
 7             final String name, final byte[] b) {
 8         return addBinaryBody(name, b, ContentType.DEFAULT_BINARY, null);
 9     }
10 
11     public MultipartEntityBuilder addBinaryBody(
12             final String name, final File file, final ContentType contentType, final String filename) {
13         return addPart(name, new FileBody(file, contentType, filename));
14     }
15 
16     public MultipartEntityBuilder addBinaryBody(
17             final String name, final File file) {
18         return addBinaryBody(name, file, ContentType.DEFAULT_BINARY, file != null ? file.getName() : null);
19     }
20 
21     public MultipartEntityBuilder addBinaryBody(
22             final String name, final InputStream stream, final ContentType contentType,
23             final String filename) {
24         return addPart(name, new InputStreamBody(stream, contentType, filename));
25     }
26 
27     public MultipartEntityBuilder addBinaryBody(final String name, final InputStream stream) {
28         return addBinaryBody(name, stream, ContentType.DEFAULT_BINARY, null);
29     }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
如上图：**需要特别注意 传****byte****[]字节数组时，必须使用4参传值,且第四参必须带后缀名**：
entityBuilder.addBinaryBody("file",new byte[]{},ContentType.DEFAULT_BINARY,"fileName.jpg");
如果使用上图中两参的，**最后fileName不传，默认为null,会导致接收文件时multiRequest.getFileNames()（见1.2接收方法代码）为空！就会获取不到文件！**
#### 注意点2 isMultipart：
multipartResolver.isMultipart(request)，这里判断是否为多文件上传，要求
1.方法为POST。
2.contentType以"mutipart/"开头。源码追踪如下：
![](http://images2015.cnblogs.com/blog/584866/201705/584866-20170504171948929-1942919549.png)
![](http://images2015.cnblogs.com/blog/584866/201705/584866-20170504172002711-1682934225.png)
![](http://images2015.cnblogs.com/blog/584866/201705/584866-20170504172100257-913649277.png)
场景：客户端(浏览器)A---->选择文件上传---->服务器B---->中转文件---->服务器C---->返回结果---->服务器B---->客户端A
有时候在项目中需要把上传的文件中转到第三方服务器，第三方服务器提供一个接收文件的接口。
而我们又不想把文件先上传到服务器保存后再通过File来读取文件上传到第三方服务器，我们可以使用HttpClient来实现。
因为项目使用的是Spring+Mybatis框架，文件的上传采用的是MultipartFile，而FileBody只支持File。
所以这里采用MultipartEntityBuilder的addBinaryBody方法以数据流的形式上传。
这里需要引入两个jar包：httpclient-4.4.jar和httpmime-4.4.jar
Maven pom.xml引入
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    <dependency>
        <groupId>org.apache.httpcomponents</groupId>
        <artifactId>httpclient</artifactId>
        <version>4.4</version>
    </dependency>
    <dependency>
        <groupId>org.apache.httpcomponents</groupId>
        <artifactId>httpmime</artifactId>
        <version>4.4</version>
    </dependency>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
上传代码：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    /**
     * 中转文件
     * 
     * @param file
     *            上传的文件
     * @return 响应结果
     */
    public String httpClientUploadFile(MultipartFile file) {
        final String remote_url = "http://192.168.1.99:8080/demo/file/upload";// 第三方服务器请求地址
        CloseableHttpClient httpClient = HttpClients.createDefault();
        String result = "";
        try {
            String fileName = file.getOriginalFilename();
            HttpPost httpPost = new HttpPost(remote_url);
            MultipartEntityBuilder builder = MultipartEntityBuilder.create();
            builder.addBinaryBody("file", file.getInputStream(), ContentType.MULTIPART_FORM_DATA, fileName);// 文件流
            builder.addTextBody("filename", fileName);// 类似浏览器表单提交，对应input的name和value
            HttpEntity entity = builder.build();
            httpPost.setEntity(entity);
            HttpResponse response = httpClient.execute(httpPost);// 执行提交
            HttpEntity responseEntity = response.getEntity();
            if (responseEntity != null) {
                // 将响应内容转换为字符串
                result = EntityUtils.toString(responseEntity, Charset.forName("UTF-8"));
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {
                httpClient.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return result;
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
