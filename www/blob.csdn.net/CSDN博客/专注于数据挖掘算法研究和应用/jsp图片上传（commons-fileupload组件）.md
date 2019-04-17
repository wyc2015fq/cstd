# jsp图片上传（commons-fileupload组件） - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年05月31日 08:31:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：11232








1.开发环境：

 1）eclipse3.2+tomcat5.5；

 2）创建dynamic web project；

 3）下载：

Commons FileUpload 可以在[http://jakarta.apache.org/commons/fileupload/](http://jakarta.apache.org/commons/fileupload/)下载

附加的Commons IO  可以在[http://jakarta.apache](http://jakarta.apache.org/commons/io/)[.org/commons/io/](http://jakarta.apache.org/commons/io/)下载

将commons-fileupload-1.2.1.jar commons-io-1.4.jar拷贝到WebContent/WEB-INF//lib目录；





2.前台：

<form method="post" enctype="multipart/form-data" action="upload.jsp" target="_blank">
<%-- 类型enctype用multipart/form-data，这样可以把文件中的数据作为流式数据上传，不管是什么文件类型，均可上传--%>
<table> 
<tr><td>作品：<input type="file" name="upfile" size="50"></td></tr>
<tr><td>作者：<input type="text" name="author" size="22">
        标题：<input type="text" name="title" size="22">
        <input type="submit" name="submit" value="上 传"></td></tr>
<tr><td>备注：上传的jpg图片（显示扩展名为.jpg）大小不能超过4M！</td></tr>
</table>
</form>

3.后台：

1)引用：

<%@ page import="org.apache.commons.fileupload.servlet.ServletFileUpload"%>
<%@ page import="org.apache.commons.fileupload.disk.DiskFileItemFactory"%>
<%@ page import="org.apache.commons.fileupload.*"%>

2)代码：

String id=null;//上传记录id
 String destinationfileName=null;//目标文件名
 String author=null;
 String title=null;
 int flag=0;//上传标志

    String uploadPath =request.getSession().getServletContext().getRealPath("/")+"upload/";//图片上传路径   
    String tempPath = request.getSession().getServletContext().getRealPath("/")+"upload/temp/";//图片临时上传路径    
    String imagePath=request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+request.getContextPath()+"/"; //   图片网络相对路径 
    if(!new File(uploadPath).isDirectory()) new File(uploadPath).mkdirs();//   文件夹不存在就自动创建：
    if(!new File(tempPath).isDirectory())new File(tempPath).mkdirs(); 
    try {
     DiskFileUpload fu = new DiskFileUpload(); 
     fu.setSizeMax(4194304);//   设置最大文件尺寸，这里是4MB   
      fu.setSizeThreshold(4096);//   设置缓冲区大小，这里是4kb 
     fu.setRepositoryPath(tempPath);//   设置临时目录： 
      List fileItems = fu.parseRequest(request);//   得到所有的文件：
        Iterator i = fileItems.iterator();   
     while(i.hasNext()) {//   依次处理表单中每个域
            FileItem file = (FileItem)i.next();//   获得用户上传时文件名 
   if (file.isFormField()){  //获得文本域表单数据       
    if("author".equals(file.getFieldName())) author=codeToString(file.getString());
    if("title".equals(file.getFieldName())) title=codeToString(file.getString());
    continue;//非file域不处理
          }    
   String sourcefileName = file.getName();
      if( sourcefileName.endsWith(".jpg")){//生成上传后的文件名     
      Random rd = new Random();
      Calendar time = Calendar.getInstance();
      id=String.valueOf(time.get(Calendar.YEAR))
           + String.valueOf(time.get(Calendar.MONTH)+1)
            + String.valueOf(time.get(Calendar.DAY_OF_MONTH))
            + String.valueOf(time.get(Calendar.HOUR_OF_DAY))
            + String.valueOf(time.get(Calendar.MINUTE))
                                 + String.valueOf(time.get(Calendar.SECOND))
                                 + String.valueOf(rd.nextInt(100));
      destinationfileName=id+".jpg";
               File fTmp=new File(uploadPath+destinationfileName);
               file.write(fTmp);
               //out.print("<img src="+imagePath+"upload/"+destinationfileName+">");
      flag=1;//上传成功标志1
           }else{out.print("上传失败，只能上传jpg文件！"); }
       }
       //out.print("<script>location.href=/"demo.jsp/";</script>");
    }catch (IOException e) {out.print("上传失败！");}
    out.flush();
    out.close();



4.主要解决点：

1)问题：form设置enctype="multipart/form-data"，request.getParameters函数无法获取表单域值；
    2)解决方案：

if (file.isFormField()){  //获得文本域表单数据       
         if("author".equals(file.getFieldName())) author=codeToString(file.getString());
        if("title".equals(file.getFieldName())) title=codeToString(file.getString());
    continue;//非file域不处理
        }   



