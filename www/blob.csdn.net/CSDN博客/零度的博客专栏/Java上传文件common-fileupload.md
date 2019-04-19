# Java上传文件common-fileupload - 零度的博客专栏 - CSDN博客
2016年07月11日 14:23:31[零度anngle](https://me.csdn.net/zmx729618)阅读数：8035
        jsp文件上传大多采用开源项目Commons FileUpload来简化处理，可以在[http://jakarta.apache.org/commons/fileupload/](http://jakarta.apache.org/commons/fileupload/)下载，这个包需要Commons
 IO的支持，可以在[http://jakarta.apache.org/commons/io/](http://jakarta.apache.org/commons/io/)下载。Commons FileUpload提供三种文件上传处理方式，DiskFileUpload、ServletFileUpload和
 PortletFileUpload三种方式，其中DiskFileUpload已经在javadoc下已经被标记为过期的方法，建议用 ServletFileUpload代替，而PortletFileUpload需要配合portlet-api来使用，所以这里我们只介绍 ServletFileUpload，并且这个也是最常用的。使用如下：
   test.jsp：      
```
<%@ page language="java" import="java.util.*" contentType="text/html;charset=gbk" pageEncoding="gbk"%>
<html>
   <body>
     <form action="/MultipartTestServlet" enctype="multipart/form-data" method="post">
      <input type="text" name="username" /><br />
      <input type="file" name="myfile" /><br/>
      <input type="file" name="myfile" /><br/>
      <input type="submit" />
     </form>
   </body>
</html>
```
MultipartTestServlet.java：
```
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
      request.setCharacterEncoding("utf-8");
      RequestContext requestContext = new ServletRequestContext(request);
   
      if(FileUpload.isMultipartContent(requestContext)){
    
            DiskFileItemFactory factory = new DiskFileItemFactory();
            factory.setRepository(new File("c:/tmp/"));
            ServletFileUpload upload = new ServletFileUpload(factory);
            //upload.setHeaderEncoding("utf-8");
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
                       System.out.println(fileItem.getFieldName() + "   " + fileItem.getName() + "   " + new String
                                           (fileItem.getString().getBytes("iso8859-1"), "gbk"));
                }else{
                       System.out.println(fileItem.getFieldName() + "   " + 
                               fileItem.getName() + "   " + fileItem.isInMemory() + "    " + 
                               fileItem.getContentType() + "   " + fileItem.getSize());
      
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
```
