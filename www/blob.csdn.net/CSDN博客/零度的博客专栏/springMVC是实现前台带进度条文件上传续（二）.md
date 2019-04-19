# springMVC是实现前台带进度条文件上传续（二） - 零度的博客专栏 - CSDN博客
2016年10月26日 11:45:44[零度anngle](https://me.csdn.net/zmx729618)阅读数：1710
      之前写过一遍基于springMVC是实现前台带进度条文件，但是该文章仅是代码层面的，没有做过多解释，今天看到一篇文章正好对上篇文章进行了较为详细的解释，文章原文如下：
        文件上传应该大部分人都接触过，应该大部分人都接触过，一般都是基于commons-fileupload组件来实现，SpringMVC的文件上传功能也是在commons-fileupload组件提供的功能上面做了一些包装功能，使文件上传开发更容易方便。
       看下上传效果图：
![](http://dl2.iteye.com/upload/attachment/0107/7660/1c86d74f-a8c8-3382-b1b9-e0273a86ae88.jpg)

项目运行效果：
![](http://dl2.iteye.com/upload/attachment/0107/7662/f7b85af2-15f7-371e-a903-10a62f516015.jpg)
项目源码地址，发布即可用：[https://github.com/wuskyfantasy/fileupload.sys](https://github.com/wuskyfantasy/fileupload.sys)
如果下载不了也可以下截fileupload.sys-master.zip附件。
项目使用spring mvc+mybatis实现，数据库采用mysql，具体参考README就可以发布起来。 
**一、实现普通文件上传**
        我们知道，类DispatcherServlet是SpringMVC的入口，在其doDispatch方法里面，我们可以看到它会先去检查有没配置multipartResolver 
![](http://dl2.iteye.com/upload/attachment/0107/7669/8598a815-bfad-3e79-858e-4a4c34ad3366.png)
如果有的话会先执行它的resolveMultipart方法：
![](http://dl2.iteye.com/upload/attachment/0107/7673/5aafef38-56ce-38ad-85d7-2217115d059c.png)
因此，我们在配置文件中添加：
![](http://dl2.iteye.com/upload/attachment/0107/7671/3d22c4ce-c526-306d-99e7-552970e05bf4.png)
则可使用SpringMVC自带的CommonsMultipartResolver实现多个文件上传的功能。
**二、实现进度条**
        现在实现带进度条的文件上传一般都是在客户端用flash上传组件计算，或者是通过客户端插件的方式。前者上传组件有大小的限制，后者对于我们系统来说实现过于复杂，开发难度稍大。因此我们采用服务器端计算文件上传进度，客户端轮询的方式。
Commons-fileupload组件自带了文件上传进度的监听器，类FileUploadBase提供了它的set方法。
ProgressListener是一个接口，我们需要自己实现它的update方法，参数pBytesRead表示已经上传到服务器的字节数，pContentLength表示所有文件的总大小，pItems表示第几个文件：
![](http://dl2.iteye.com/upload/attachment/0107/7676/5f89250e-5154-36e1-b8d4-a97bd47e2055.png)
文件上传进度监听器实现类，[FileUploadProgressListener.java](https://github.com/wuskyfantasy/fileupload.sys/blob/master/src/main/java/com/sunshine/fusys/listener/FileUploadProgressListener.java)。
 SpringMVC没有实现监听器，所以如果要监听的话得自己扩展CommonsMultipartResolver类，在newFileUpload里面加入代码设置自己实现的监听器： 
![](http://dl2.iteye.com/upload/attachment/0107/7678/60a63beb-05fa-380e-9709-f45ddc72fc87.png)
ServletFileUploadExt是对ServletFileUpload类进行的扩展，后面会提到。这样在文件上传的过程中，监听器将得到通知已上传的字节数：
![](http://dl2.iteye.com/upload/attachment/0107/7680/f53a5d99-bf5d-3fb9-91cc-7b81d99dff9b.png)
![](http://dl2.iteye.com/upload/attachment/0107/7682/9eac8761-7e54-339c-ac6b-929a92bc73fe.png)
自己实现的Resolver类为[CustomMultipartResolver](https://github.com/wuskyfantasy/fileupload.sys/blob/master/src/main/java/com/sunshine/fusys/controllers/CustomMultipartResolver.java)，在这个类中需要在每一次上传请求中设定处理上传进度的监听器，并处理文件上传。 
Java代码 ![复制代码](http://ysj5125094.iteye.com/images/icon_copy.gif)![收藏代码](http://ysj5125094.iteye.com/images/icon_star.png)![](http://ysj5125094.iteye.com/images/spinner.gif)
- protected FileUpload newFileUpload(FileItemFactory fileItemFactory) {    
-   ServletFileUpload upload = new ServletFileUpload(fileItemFactory);    
-   upload.setSizeMax(-1);    
- if (request != null) {    
-      HttpSession session = request.getSession();  
- //注意：这里的listener不能公用，即不能放到方法外面
-      FileUploadProgressListener progressListener =   
- new FileUploadProgressListener(session);  
-      upload.setProgressListener(progressListener);    
-   }    
- return upload;    
- }  
```java
<span style="font-family:Arial;font-size:12px;">protected FileUpload newFileUpload(FileItemFactory fileItemFactory) {  
  ServletFileUpload upload = new ServletFileUpload(fileItemFactory);  
  upload.setSizeMax(-1);  
  if (request != null) {  
     HttpSession session = request.getSession();
     //注意：这里的listener不能公用，即不能放到方法外面
     FileUploadProgressListener progressListener = 
     new FileUploadProgressListener(session);
     upload.setProgressListener(progressListener);  
  }  
  return upload;  
}</span>
```
在配置文件spring-mvc-context.xml中配置使用自定义的Resolver。 
Xml代码 ![复制代码](http://ysj5125094.iteye.com/images/icon_copy.gif)![收藏代码](http://ysj5125094.iteye.com/images/icon_star.png)![](http://ysj5125094.iteye.com/images/spinner.gif)
- <beanid="multipartResolver"class="com.sunshine.fusys.controllers.CustomMultipartResolver">
- <propertyname="defaultEncoding"value="UTF-8"/>
- <propertyname="maxUploadSize"value="1000000000000"/>
- </bean>
<span style="font-family:Arial;font-size:12px;"><bean id="multipartResolver"   class="com.sunshine.fusys.controllers.CustomMultipartResolver">
    <property name="defaultEncoding" value="UTF-8" />
    <property name="maxUploadSize" value="1000000000000" />
</bean></span>
最终在上传时，会通过commons-fileupload的组件进行上传时，会把上传进度读取到设定的session中，最后通过一个controller访问这个设定的进度值，就可以获取了。获取进度的controller为[ProgressController](https://github.com/wuskyfantasy/fileupload.sys/blob/master/src/main/java/com/sunshine/fusys/controllers/ProgressController.java)。客户端采用异步的方式获取进度信息进行展示，具体参考源码[index-userFile.jsp](https://github.com/wuskyfantasy/fileupload.sys/blob/master/src/main/webapp/WEB-INF/views/main/index-userFile.jsp)。
三**、取消文件上传**
        取消文件上传实现方式为上传时保存上传输入流的引用，取消时关闭流，让输入流产生IO异常或者数组越界异常，同时捕获这些异常，则可取消文件上传。
最后补一个配置，spring mvc的项目访问时默认访问某个指定页面或者controller的配置如下：
Xml代码 ![复制代码](http://ysj5125094.iteye.com/images/icon_copy.gif)![收藏代码](http://ysj5125094.iteye.com/images/icon_star.png)![](http://ysj5125094.iteye.com/images/spinner.gif)
- //转向controller  
- <mvc:view-controllerpath="/"view-name="redirect:/indexMain/index"/>
<span style="font-family:Arial;font-size:12px;">//转向controller
<mvc:view-controller path="/"  view-name="redirect:/indexMain/index" /></span>
或者 
Xml代码 ![复制代码](http://ysj5125094.iteye.com/images/icon_copy.gif)![收藏代码](http://ysj5125094.iteye.com/images/icon_star.png)![](http://ysj5125094.iteye.com/images/spinner.gif)
- //转向index.jsp  
- <mvc:view-controllerpath="/"view-name="/index"/>
<span style="font-family:Arial;font-size:12px;">//转向index.jsp
<mvc:view-controller path="/"  view-name="/index" /></span>
具体可参考配置文件：[spring-mvc-context.xml](https://github.com/wuskyfantasy/fileupload.sys/blob/master/src/main/resources/spring/spring-mvc-context.xml)
