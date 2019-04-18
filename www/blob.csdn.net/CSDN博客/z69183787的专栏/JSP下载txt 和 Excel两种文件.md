# JSP下载txt 和 Excel两种文件 - z69183787的专栏 - CSDN博客
2013年10月24日 11:42:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2805
１　下载txt文件　
这个花了我不少时间　原因是用ie下载txt文件时是在页面中直接打开了文件．虽然查了一些资料，也看了别人的解决方案，可还是解决不了问题，最后发现是一个字母惹的祸：少写一个字母　嘿嘿　够马虎！！！
代码如下：
OutputStream o=response.getOutputStream();
   byte b[]=new byte[500];
   File fileLoad=new File("e:/test.txt");
    response.setContentType("application/octet-stream");
   response.setHeader("content-disposition","attachment; filename=text.txt");
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
2 下载excel文件　
跟下载txt文件时的唯一区别是ContentType值的设置不同：
OutputStream o=response.getOutputStream();
   byte b[]=new byte[500];
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
这两个本来是放在一起的，因为我的页面中需要判断是下载的txt文件还是xls文件　　在这里给分开了　需要注意的是，最后两句一定要加上，否则会出现getOutputStream()错误的！！！！

实例：
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
down2.jsp:
<%@ page language="java" import="java.util.*,java.io.* " pageEncoding="UTF-8"%>
<%
request.setCharacterEncoding("utf-8");
String filename = request.getParameter("filename");
filename = new String(filename.getBytes("ISO-8859-1"),"UTF-8"); 
System.out.println(filename);
OutputStream o=response.getOutputStream();
byte b[]=new byte[500];
/** * 得到文件的当前路径 * @param args */ 
String   serverpath=request.getRealPath("\\"); 
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
%>
