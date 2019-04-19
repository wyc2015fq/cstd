# Java文件下载 - 零度的博客专栏 - CSDN博客
2016年07月12日 11:06:04[零度anngle](https://me.csdn.net/zmx729618)阅读数：473标签：[文件下载](https://so.csdn.net/so/search/s.do?q=文件下载&t=blog)
个人分类：[文件上传/下载](https://blog.csdn.net/zmx729618/article/category/6306113)
                
前台：
```
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
 <%
 String path = request.getContextPath();
 String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
 %>
 <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
 <html>
 <head>
     <base href="<%=basePath%>">
     <title>My JSP 'index.jsp' starting page</title>
     <script type="text/javascript">
        function download(filename){
          var url = encodeURI("down2.jsp?filename="+filename);
          window.location.href= url;
        }
     </script>
 </head>
 <body>
普通测试：
<a href="javascript:void(0)" onclick="download('a.txt');">Down a.txt</a>   
中文文件名测试：
<a href="javascript:void(0)" onclick="download('中文.txt');">Down 中文.txt</a>   
普通转向：
<a href="a.txt">down.txt</a>   
 </body>
 </html>
```
后台：
```
request.setCharacterEncoding("utf-8");
 String filename = request.getParameter("filename");
 filename = new String(filename.getBytes("ISO-8859-1"),"UTF-8"); 
 System.out.println(filename);
 OutputStream o=response.getOutputStream();
 byte b[]=new byte[1024];
 /** * 得到文件的当前路径 * @param args */ 
 String   serverpath=request.getRealPath("\\"); 
 File fileLoad=new File(serverpath+filename);
 response.setContentType("application/octet-stream");
 response.setHeader("content-disposition","attachment; filename="+filename);
 long fileLength=fileLoad.length();
 String length1=String.valueOf(fileLength);
 response.setHeader("Content_Length",length1);
 FileInputStream in=new FileInputStream(fileLoad);
 int n;
 while((n=in.read(b))!=-1){
    o.write(b,0,n);
 }
 in.close();
 out.clear();
 out = pageContext.pushBody();
```
另附下载excel文件：
```
OutputStream o=response.getOutputStream();
    byte b[]=new byte[1024];
    File fileLoad=new File("e:/text.xls");
    response.reset();
   
    response.setContentType("application/vnd.ms-excel");
    response.setHeader("content-disposition","attachment; filename=text.xls");
    long fileLength=fileLoad.length();
    String length1=String.valueOf(fileLength);
    response.setHeader("Content_Length",length1);
    FileInputStream in=new FileInputStream(fileLoad);
    int n;
    while((n=in.read(b))!=-1){
     o.write(b,0,n);
    }
   
    in.close();
    out.clear();
    out = pageContext.pushBody();
```
        区别仅仅是是内容类型的不同。
        注意，如果在jsp页面中直接使用上述下载代码。 
out.clear(); out = pageContext.pushBody(); 这两句不可少，为什么不加入了就报异常: java.lang.IllegalStateException: getOutputStream() has already been called for this
 response。因为pushBody()的作用是保存当前的out对象，并更新PageContext中Page范围内Out对象。至于为什么要加上这句话，是因为JSP容器在处理完成请求后会调用releasePageConter方法释放所有的PageContestObject，并且同时调用getWriter方法。由于getWriter方法与在JSP页面中使用流相关的getOutputStream方法冲突，所以会造成这种异常，解决方法就是楼上给的一样，只需要在JSP页面的最后加上这两条语句。
