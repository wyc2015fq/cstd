# Struts2文件上传-fileUpload拦截器 - z69183787的专栏 - CSDN博客
2014年06月26日 21:30:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3341
Struts2自带了文件上传拦截器方便进行文件上传，要使用这个功能，必须给使用了Struts2框架的工程添加commons.io包，然后在struts.xml里进行以下设置：
在<package>标签之外添加<constantname="struts.multipart.saveDir"  val ="/temp"/>，这句指定了上传文件的临时存放目录，以这句为例，临时存放目录为应用所在根目录下的temp文件夹，假设我的应用放在D:\ProgramFiles\apache-tomcat-6.0.24\webapps下，那么临时存放目录就是D:\temp。也可以指定一个绝对路径。目录不存在也没关系，会自动创建。
再添加<constantname="struts.multipart.maxSize"  val ="10240000"/>，Struts2的文件上传功能默认最大只能上传2M大小。这句重新指定了上传文件的最大限制，单位是字节。以这句为例，上传文件不能超过10240000字节。注意，不能写成1024*1000*10这种形式。
在需要处理上传文件的Action里配置拦截器：
      <action name="test"class="testAction">
           <!--配置fileUpload的拦截器-->
           <interceptor-refname="fileUpload">
            <!--配置允许上传的文件类型-->
             <paramname="allowedTypes">application/vnd.ms-excel，image/bmp,image/png,image/gif,image/jpeg
</param>
           <!--配置允许上传的文件大小单位字节-->
             <paramname="maximumSize">5242880</param>
           </interceptor-ref>
           <interceptor-refname="defaultStack"/>
         <resultname="test">/TestJsp.jsp</result>
      </action>
2
- 
<?xml version="1.0" encoding="UTF-8"?>  
- 
- 
<!DOCTYPE struts PUBLIC  
- 
    "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"  
- 
    "http://struts.apache.org/dtds/struts-2.0.dtd">  
- 
- 
<struts>  
- 
    <constant name="struts.i18n.encoding" value="GBK"/>  
- 
    <constant name="struts.configuration.xml.reload" value="true" />  
- 
    <constant name="struts.multipart.saveDir" value="e:" />  
- 
- 
    <package name="upload" extends="struts-default">  
- 
- 
        <action name="upload" class="edu.cs.zhaolei.UploadAction">  
- 
            <interceptor-ref name="fileUpload">   fileUpload要放在 defaultStack前面
- 
            <param name="maximumSize">10485760</param>    
- 
            </interceptor-ref>   
- 
            <interceptor-ref name="defaultStack"/>      
- 
            <param name="savePath">/upload</param>  
- 
            <result name="input">/upload.jsp</result>     
- 
            <result>/success.jsp</result>     
- 
        </action>  
- 
- 
    </package>  
- 
</struts>   
fileUpload拦截器必须在defaultStack(默认)拦截器前面配置，Struts2会由上到下执行拦截器。
allowedTypes指定上传的文件类型，但类型不符只会在控制台输出警告语句，程序不会报错。
maximumSize指定上传的文件大小，但文件超出大小只会在控制台输出警告语句，程序不会报错。注意不能写成1024*1024*5的形式。
实现原理
Struts 2是通过Commons FileUpload文件上传。CommonsFileUpload通过将HTTP的数据保存到临时文件夹，然后Struts使用fileUpload拦截器将文件绑定到Action的实例中。从而我们就能够以本地文件方式的操作浏览器上传的文件。
具体实现
前段时间Apache发布了Struts 2.0.6GA，所以本文的实现是以该版本的Struts作为框架的。以下是例子所依赖类包的列表：
![](http://www.blogjava.net/images/blogjava_net/max/20810/r_img1.gif)
清单1 依赖类包的列表
首先，创建文件上传页面FileUpload.jsp，内容如下：
<%@ pagelanguage="java"contentType="text/html;charset=utf-8"pageEncoding="utf-8"%>
<%@ taglibprefix="s"uri="/struts-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML1.0 Transitional//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<htmlxmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>Struts 2 File Upload</title>
</head>
<body>
   <s:formaction="fileUpload"method="POST"enctype="multipart/form-data">
      <s:filename="myFile"label="ImageFile"/>
      <s:textfieldname="caption"label="Caption"/>      
      <s:submit/>
   </s:form>
</body>
</html>
清单2 FileUpload.jsp
在FileUpload.jsp中，先将表单的提交方式设为POST，然后将enctype设为multipart/form-data，这并没有什么特别之处。接下来，<s:file/>标志将文件上传控件绑定到Action的myFile属性。
其次是FileUploadAction.java代码：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)packagetutorial;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importjava.io.BufferedInputStream;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importjava.io.BufferedOutputStream;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importjava.io.File;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importjava.io.FileInputStream;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importjava.io.FileOutputStream;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importjava.io.InputStream;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importjava.io.OutputStream;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importjava.util.Date;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importorg.apache.struts2.ServletActionContext;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)importcom.opensymphony.xwork2.ActionSupport;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)publicclassFileUploadActionextendsActionSupport{
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   privatestaticfinallongserialVersionUID=572146812454l;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   privatestaticfinalintBUFFER_SIZE=16*1024;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   privateFilemyFile;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   privateStringcontentType;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   privateStringfileName;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   privateStringimageFileName;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   privateStringcaption;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)   publicvoidsetMyFileContentType(String contentType){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      this.contentType=contentType;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)   publicvoidsetMyFileFileName(String fileName){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      this.fileName=fileName;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)   publicvoidsetMyFile(File myFile){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      this.myFile=myFile;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)   publicStringgetImageFileName(){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      returnimageFileName;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)   publicStringgetCaption(){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      returncaption;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)    publicvoidsetCaption(String caption){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      this.caption=caption;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)   privatestaticvoidcopy(Filesrc, File dst){
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)      try{
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)          InputStreamin=null;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)         OutputStream out=null;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)          try{              
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)             in=newBufferedInputStream(newFileInputStream(src), BUFFER_SIZE);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)             out=newBufferedOutputStream(newFileOutputStream(dst), BUFFER_SIZE);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)             byte[]buffer=newbyte[BUFFER_SIZE];
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)             while(in.read(buffer)>0){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)                out.write(buffer);
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)              }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)          }finally{
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)             if(null!=in){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)                in.close();
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)              }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)              if(null!=out){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)                out.close();
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)              }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)          }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)       }catch(Exception e){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)         e.printStackTrace();
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)       }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)   privatestaticStringgetExtention(String fileName){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      intpos=fileName.lastIndexOf(".");
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      returnfileName.substring(pos);
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)   @Override
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)   publicStringexecute()    {       
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      imageFileName=newDate().getTime()+getExtention(fileName);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      File imageFile=newFile(ServletActionContext.getServletContext().getRealPath("/UploadImages")+"/"+imageFileName);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      copy(myFile, imageFile);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      returnSUCCESS;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
清单3 tutorial/FileUploadAction.java
在FileUploadAction中我分别写了setMyFileContentType、setMyFileFileName、setMyFile和setCaption四个Setter方法，后两者很容易明白，分别对应FileUpload.jsp中的<s:file/>和<s:textfield/>标志。但是前两者并没有显式地与任何的页面标志绑定，那么它们的值又是从何而来的呢？其实，<s:file/>标志不仅仅是绑定到myFile，还有myFileContentType（上传文件的MIME类型）和myFileFileName（上传文件的文件名，该文件名不包括文件的路径）。因此，<s:filename="xxx"/>对应Action类里面的xxx、xxxContentType和xxxFileName三个属性。
FileUploadAction作用是将浏览器上传的文件拷贝到WEB应用程序的UploadImages文件夹下，新文件的名称是由系统时间与上传文件的后缀组成，该名称将被赋给imageFileName属性，以便上传成功的跳转页面使用。
下面我们就来看看上传成功的页面：
<%@ pagelanguage="java"contentType="text/html;charset=utf-8"pageEncoding="utf-8"%>
<%@ taglibprefix="s"uri="/struts-tags"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML1.0 Transitional//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<htmlxmlns="http://www.w3.org/1999/xhtml">
<head>
   <title>Struts 2 File Upload</title>
</head>
<body>
   <divstyle="padding: 3px; border: solid 1px #cccccc;text-align: center">
      <imgsrc='UploadImages/<s:propertyvalue="imageFileName"/>' />
      <br/>
      <s:propertyvalue="caption"/>
   </div>
</body>
</html>
清单4 ShowUpload.jsp
ShowUpload.jsp获得imageFileName，将其UploadImages组成URL，从而将上传的图像显示出来。
然后是Action的配置文件：
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE struts PUBLIC
    "-//Apache SoftwareFoundation//DTD Struts Configuration 2.0//EN"
   "http://struts.apache.org/dtds/struts-2.0.dtd">
<struts>
   <packagename="fileUploadDemo"extends="struts-default">
      <actionname="fileUpload"class="tutorial.FileUploadAction">
          <interceptor-refname="fileUploadStack"/>
          <resultname="success">/ShowUpload.jsp</result>
      </action>
   </package>
</struts>
清单5 struts.xml
fileUpload Action显式地应用fileUploadStack的拦截器。
最后是web.xml配置文件：
<?xml version="1.0" encoding="UTF-8"?>
<web-appid="WebApp_9"version="2.4"
    xmlns="http://java.sun.com/xml/ns/j2ee"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://java.sun.com/xml/ns/j2eehttp://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
   <display-name>Struts 2 Fileupload</display-name>
   <filter>
      <filter-name>struts-cleanup</filter-name>
      <filter-class>
          org.apache.struts2.dispatcher.ActionContextCleanUp
      </filter-class>
   </filter>
   <filter>
      <filter-name>struts2</filter-name>
      <filter-class>
          org.apache.struts2.dispatcher.FilterDispatcher
      </filter-class>
   </filter>
   <filter-mapping>
      <filter-name>struts-cleanup</filter-name>
      <url-pattern>/*</url-pattern>
   </filter-mapping>
   <filter-mapping>
      <filter-name>struts2</filter-name>
      <url-pattern>/*</url-pattern>
   </filter-mapping>
   <welcome-file-list>
      <welcome-file>index.html</welcome-file>
   </welcome-file-list>
</web-app>
清单6 WEB-INF/web.xml
发布运行应用程序，在浏览器地址栏中键入：[http://localhost:8080/Struts2_Fileupload/FileUpload.jsp](http://localhost:8080/Struts2_Fileupload/FileUpload.jsp)，出现图示页面：
![](http://www.blogjava.net/images/blogjava_net/max/20810/r_img2.gif)
清单7 FileUpload页面
选择图片文件，填写Caption并按下Submit按钮提交，出现图示页面：
![](http://www.blogjava.net/images/blogjava_net/max/20810/r_img3.gif)
清单8 上传成功页面
更多配置
在运行上述例子，如果您留心一点的话，应该会发现服务器控制台有如下输出：
Mar20,20074:08:43PMorg.apache.struts2.dispatcher.Dispatcher getSaveDir
INFO: Unable to find 'struts.multipart.saveDir' property setting.Defaulting to javax.servlet.context.tempdir
Mar20,20074:08:43PMorg.apache.struts2.interceptor.FileUploadInterceptorintercept
INFO: Removing file myFile C:\Program Files\Tomcat5.5\work\Catalina\localhost\Struts2_Fileupload\upload_251447c2_1116e355841__7ff7_00000006.tmp
清单9 服务器控制台输出
上述信息告诉我们，struts.multipart.saveDir没有配置。struts.multipart.saveDir用于指定存放临时文件的文件夹，该配置写在struts.properties文件中。例如，如果在struts.properties文件加入如下代码：
struts.multipart.saveDir=/tmp
清单10 struts配置
这样上传的文件就会临时保存到你根目录下的tmp文件夹中（一般为c:\tmp），如果此文件夹不存在，Struts2会自动创建一个。
错误处理
上述例子实现的图片上传的功能，所以应该阻止用户上传非图片类型的文件。在Struts2中如何实现这点呢？其实这也很简单，对上述例子作如下修改即可。
首先修改FileUpload.jsp，在<body>与<s:form>之间加入“<s:fielderror/>”，用于在页面上输出错误信息。
然后修改struts.xml文件，将Action fileUpload的定义改为如下所示：
      <actionname="fileUpload"class="tutorial.FileUploadAction">
          <interceptor-refname="fileUpload">
             <paramname="allowedTypes">
                 image/bmp,image/png,image/gif,image/jpeg
             </param>
          </interceptor-ref>
          <interceptor-refname="defaultStack"/>         
          <resultname="input">/FileUpload.jsp</result>
          <resultname="success">/ShowUpload.jsp</result>
      </action>
清单11 修改后的配置文件
显而易见，起作用就是fileUpload拦截器的allowTypes参数。另外，配置还引入defaultStack它会帮我们添加验证等功能，所以在出错之后会跳转到名称为“input”的结果，也即是FileUpload.jsp。
发布运行应用程序，出错时，页面如下图所示：
![](http://www.blogjava.net/images/blogjava_net/max/20810/r_img4.gif)
清单12 出错提示页面
上面的出错提示是Struts2默认的，大多数情况下，我们都需要自定义和国际化这些信息。通过在全局的国际资源文件中加入“struts.messages.error.content.type.not.allowed=Thefile you uploaded is not aimage”，可以实现以上提及的需求。对此有疑问的朋友可以参考我之前的文章《[在Struts2.0中国际化(i18n)您的应用程序](http://www.blogjava.net/max/archive/2006/11/01/78536.html)》。
实现之后的出错页面如下图所示：
![](http://www.blogjava.net/images/blogjava_net/max/20810/r_img5.gif)
清单13 自定义出错提示页面
同样的做法，你可以使用参数“maximumSize”来限制上传文件的大小，它对应的字符资源名为：“struts.messages.error.file.too.large”。
字符资源“struts.messages.error.uploading”用提示一般的上传出错信息。
多文件上传
与单文件上传相似，Struts 2实现多文件上传也很简单。你可以将多个<s:file/>绑定Action的数组或列表。如下例所示。
<s:formaction="doMultipleUploadUsingList"method="POST"enctype="multipart/form-data">
   <s:filelabel="File(1)"name="upload"/>
   <s:filelabel="File(2)"name="upload"/>
   <s:filelabel="FIle(3)"name="upload"/>
   <s:submit/>
</s:form>
清单14 多文件上传JSP代码片段
如果你希望绑定到数组，Action的代码应类似：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)  privateFile[]uploads;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)   privateString[] uploadFileNames;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)   privateString[] uploadContentTypes;
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)   publicFile[]getUpload(){returnthis.uploads;}
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicvoidsetUpload(File[] upload){this.uploads=upload;}
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicString[] getUploadFileName(){returnthis.uploadFileNames; }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicvoidsetUploadFileName(String[] uploadFileName){this.uploadFileNames=uploadFileName; }
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicString[] getUploadContentType(){returnthis.uploadContentTypes; }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicvoidsetUploadContentType(String[]uploadContentType){this.uploadContentTypes=uploadContentType; }
清单15 多文件上传数组绑定Action代码片段
如果你想绑定到列表，则应类似：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)  privateList<File>uploads=newArrayList<File>();
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)   privateList<String>uploadFileNames=newArrayList<String>();
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)   privateList<String>uploadContentTypes=newArrayList<String>();
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)   publicList<File>getUpload(){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      returnthis.uploads;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicvoidsetUpload(List<File>uploads){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      this.uploads=uploads;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicList<String>getUploadFileName(){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      returnthis.uploadFileNames;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicvoidsetUploadFileName(List<String>uploadFileNames){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      this.uploadFileNames=uploadFileNames;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicList<String>getUploadContentType(){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      returnthis.uploadContentTypes;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)    publicvoidsetUploadContentType(List<String>contentTypes){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)      this.uploadContentTypes=contentTypes;
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
清单16 多文件上传列表绑定Action代码片段
总结
在Struts 2中实现文件上传的确是轻而易举，您要做的只是使用<s:file/>与Action的属性绑定。这又一次有力地证明了Struts 2的简单易用。
