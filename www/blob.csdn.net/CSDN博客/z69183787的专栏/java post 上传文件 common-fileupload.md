# java post 上传文件 common-fileupload - z69183787的专栏 - CSDN博客
2016年04月26日 16:15:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：466
个人分类：[文件处理-上传](https://blog.csdn.net/z69183787/article/category/2229597)

jsp文件上传的处理
注：这是第二次发表文件上传方面的文章了，目的就是想让大家把这个知识点搞的彻底明白，让它变得再普通！
jsp文件上传大多采用采用开源项目来简化处理，这里列出常用的两个jar包的实现，并进行比较，说明他们的优缺点和应该注意的
问题。
 Commons FileUpload，可以在[http://jakarta.apache.org/commons/fileupload/](http://jakarta.apache.org/commons/fileupload/)下载，这个包需要Commons
 IO的支持，可以在
[http://jakarta.apache.org/commons/io/](http://jakarta.apache.org/commons/io/)下载
 com.oreilly.servlet，可以在[http://www.servlets.com/cos/](http://www.servlets.com/cos/)下载 
Commons FileUpload提供三种文件上传处理方式，DiskFileUpload、ServletFileUpload和 PortletFileUpload三种方式，其中
DiskFileUpload已经在javadoc下已经被标记为过期的方法，建议用 ServletFileUpload代替，而PortletFileUpload需要配合
portlet-api来使用，所以这里我们只介绍 ServletFileUpload，并且这个也是最常用的。
com.oreilly.servlet也提供了三种文件上传的处理方式，MultipartWrapper、MultipartRequest和 MultipartParser三种方式，
其中MultipartWrapper和MultipartRequest的用法基本相同，并且没有 MultipartRequest提供的操作多，所以这里介绍
MultipartRequest，MultipartParser和前两者有些不同，可以用来处理某些特殊情况，例如表单中有两个同名的文件上传选择框
。
我们暂时称三面三种文件上传方式分别为：ServletFileUpload方式（MultipartTestServlet）、 MultipartRequest方式
（MultipartTestServlet2）、MultipartParser方式（MultipartTestServlet3）
代码如下：
test.html
<%@ page language="java" import="java.util.*" contentType="text/html;charset=gbk" pageEncoding="gbk"%>
<html>
  <body>
    <form. action="MultipartTestServlet" enctype="multipart/form-data" method="post">
     <input type="text" name="username" /><br />
     <input type="file" name="myfile" /><br/>
     <input type="file" name="myfile" /><br/>
     <input type="submit" />
    </form>
    <br/><br/><br/><br/>
    <form. action="MultipartTestServlet2" enctype="multipart/form-data" method="post">
     <input type="text" name="username" /><br />
     <input type="file" name="myfile" /><br/>
     <input type="file" name="myfile" /><br/>
     <input type="submit" />
    </form>
    <br/><br/><br/><br/>
    <form. action="MultipartTestServlet3" enctype="multipart/form-data" method="post">
     <input type="text" name="username" /><br />
     <input type="file" name="myfile" /><br/>
     <input type="file" name="myfile" /><br/>
     <input type="submit" />
    </form>
  </body>
</html>
MultipartTestServlet.java
package com.bug.servlet;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUpload;
import org.apache.commons.fileupload.FileUploadException;
import org.apache.commons.fileupload.RequestContext;
import org.apache.commons.fileupload.disk.DiskFileItemFactory;
import org.apache.commons.fileupload.servlet.ServletFileUpload;
import org.apache.commons.fileupload.servlet.ServletRequestContext;
public class MultipartTestServlet extends HttpServlet {
 public MultipartTestServlet() {
  super();
 }
 public void doPost(HttpServletRequest request, HttpServletResponse response)
   throws ServletException, IOException {
  request.setCharacterEncoding("gbk");
  RequestContext requestContext = new ServletRequestContext(request);
  if(FileUpload.isMultipartContent(requestContext)){
   DiskFileItemFactory factory = new DiskFileItemFactory();
   factory.setRepository(new File("c:/tmp/"));
   ServletFileUpload upload = new ServletFileUpload(factory);
   //upload.setHeaderEncoding("gbk");
   upload.setSizeMax(2000000);
   List items = new ArrayList();
    try {
     items = upload.parseRequest(request);
    } catch (FileUploadException e1) {
     System.out.println("文件上传发生错误" + e1.getMessage());
    }
   Iterator it = items.iterator();
   while(it.hasNext()){
    FileItem fileItem = (FileItem) it.next();
    if(fileItem.isFormField()){      
     System.out.println(fileItem.getFieldName() + "   " + fileItem.getName() + "   " + new String
(fileItem.getString().getBytes("iso8859-1"), "gbk"));
    }else{
     System.out.println(fileItem.getFieldName() + "   " + 
        fileItem.getName() + "   " + 
        fileItem.isInMemory() + "    " + 
        fileItem.getContentType() + "   " + 
        fileItem.getSize());
     if(fileItem.getName()!=null && fileItem.getSize()!=0){
      File fullFile = new File(fileItem.getName());
      File newFile = new File("c:/temp/" + fullFile.getName());
      try {
       fileItem.write(newFile);
      } catch (Exception e) {
       e.printStackTrace();
      }
     }else{
      System.out.println("文件没有选择 或 文件内容为空");
     }
    }
   }
  }
 }
}
MultipartTestServlet2.java
package com.bug.servlet;
import java.io.IOException;
import java.util.Enumeration;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import com.oreilly.servlet.MultipartRequest;
import com.oreilly.servlet.multipart.DefaultFileRenamePolicy;
public class MultipartTestServlet2 extends HttpServlet {
 public MultipartTestServlet2() {
  super();
 }
 public void doPost(HttpServletRequest request, HttpServletResponse response)
   throws ServletException, IOException {
  //request.setCharacterEncoding("gbk");  不起作用
  System.out.println("start ");
  MultipartRequest multi = new MultipartRequest(request, "c:/tmp/", 2*1024*1024, "gbk", new
DefaultFileRenamePolicy());
  System.out.println("start ");
  Enumeration filesName = multi.getFileNames();
  Enumeration paramsName = multi.getParameterNames();
  while(paramsName.hasMoreElements()){
   String paramName = (String) paramsName.nextElement();
   System.out.println(multi.getParameter(paramName));
  }
  while(filesName.hasMoreElements()){
   String fileName = (String) filesName.nextElement();
   System.out.println(multi.getFilesystemName(fileName) + "  " +
          multi.getOriginalFileName(fileName) + "  " + 
          multi.getContentType(fileName) + "  ");
   if(multi.getFilesystemName(fileName)!=null && !multi.getFilesystemName(fileName).equals(""))
    System.out.println(multi.getFile(fileName).toURI());
  }
 }
}
MultipartTestServlet3.java
package com.bug.servlet;
import java.io.File;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import com.oreilly.servlet.multipart.FilePart;
import com.oreilly.servlet.multipart.MultipartParser;
import com.oreilly.servlet.multipart.ParamPart;
import com.oreilly.servlet.multipart.Part;
public class MultipartTestServlet3 extends HttpServlet {
 public MultipartTestServlet3() {
  super();
 }
 public void doPost(HttpServletRequest request, HttpServletResponse response)
   throws ServletException, IOException {
      MultipartParser mp = new MultipartParser(request, 2*1024*1024, false, false, "gbk");
      Part part;
      while ((part = mp.readNextPart()) != null) {
        String name = part.getName();
        if (part.isParam()) {
          ParamPart paramPart = (ParamPart) part;
          String value = paramPart.getStringValue();
          System.out.println("param: name=" + name + "; value=" + value);
        }
        else if (part.isFile()) {
          // it's a file part
          FilePart filePart = (FilePart) part;
          String fileName = filePart.getFileName();
          if (fileName != null) {
            long size = filePart.writeTo(new File("c:/tmp/"));
            System.out.println("file: name=" + name + "; fileName=" + fileName +
              ", filePath=" + filePart.getFilePath() + 
              ", contentType=" + filePart.getContentType() + 
              ", size=" + size);
          }
          else { 
           System.out.println("file: name=" + name + "; EMPTY");
          }
          System.out.flush();
        }
      }
    }
}
web.xml中加入
 <servlet>
    <servlet-name>MultipartTestServlet</servlet-name>
    <servlet-class>com.bug.servlet.MultipartTestServlet</servlet-class>
  </servlet>
  <servlet>
    <servlet-name>MultipartTestServlet2</servlet-name>
    <servlet-class>com.bug.servlet.MultipartTestServlet2</servlet-class>
  </servlet>
  <servlet>
    <servlet-name>MultipartTestServlet3</servlet-name>
    <servlet-class>com.bug.servlet.MultipartTestServlet3</servlet-class>
  </servlet>
  <servlet-mapping>
    <servlet-name>MultipartTestServlet</servlet-name>
    <url-pattern>/MultipartTestServlet</url-pattern>
  </servlet-mapping>
  <servlet-mapping>
    <servlet-name>MultipartTestServlet2</servlet-name>
    <url-pattern>/MultipartTestServlet2</url-pattern>
  </servlet-mapping>
  <servlet-mapping>
    <servlet-name>MultipartTestServlet3</servlet-name>
    <url-pattern>/MultipartTestServlet3</url-pattern>
  </servlet-mapping>
问题1、中文问题：
 三种凡是都可以通过自己的方法来设置encoding为gbk开处理和解决中文问题，包括初始化的时候传入gbk作为参数，或是是初始
化后通过setEncoding的方式来实现。
 另外ServletFileUpload方式也可以通过request.setCharacterEncoding("gbk");的方式来实现，而其它两种方式不支持这种方式
。
问题2、文件大小限制
 ServletFileUpload方式可以设置文件大小限制，也可以不用设置，例子中的 upload.setSizeMax(2000000)就可以注释掉。如果
设置upload.setSizeMax(-1)，表明不限制上传的大小。文档中没有指明默认的限制的多少，我在不设置的情况下上传了一个9M的
东西，可以上传，估计默认是不限制大小的。
 而MultipartRequest方式和MultipartParser方式是必须设置文件的上传文件的大小限制的，如果不设置，默认是1M的大小限制。
问题3、文件上传发生错误
 如果文件上传过程中发生任何错误，或者是文件的大小超出了范围，系统都将抛出错误。
 ServletFileUpload方式在upload.parseRequest(request)时抛出错误
 MultipartRequest方式在new MultipartRequest(。。。)时抛出错误
 MultipartParser方式在new MultipartParser(。。。)时抛出错误
问题4、上传同名文件时，他们保存到临时目录是的冲突问题
 ServletFileUpload方式，不会有冲突，系统会把上传得文件按照一定的规则重新命名，保证不会冲突
 MultipartParser方式，会产生冲突，系统会把文件按照上传时的文件名，保存到临时目录下，如果两个用会同时上传文件名相同
的文件，那么就可能会产生冲突，一方把另一方的临时文件给替换了。
 MultipartRequest方式，在初始化时如果提供了一个名称转换策略，就不会有冲突，如果不提桶，就会有冲突。在上面的例子中
我们提供了一个new DefaultFileRenamePolicy()保证了文件名不会有冲突发生。
问题5：表单中有两个同名的文件上传选择框，就像我们例子中的myfile一样，每个表单中有两个name=“myfile”的上传框
 ServletFileUpload方式，可以处理，可以分别得到他们各自的文件，
 MultipartRequest方式，不可以处理，会发生冲突，会有一个上传框的文件覆盖了另外一个。
 MultipartParser方式，可以处理，可以分别得到他们各自的文件
