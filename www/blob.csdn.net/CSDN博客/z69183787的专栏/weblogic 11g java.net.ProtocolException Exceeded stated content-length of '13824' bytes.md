# weblogic 11g java.net.ProtocolException: Exceeded stated content-length of: '13824' bytes - z69183787的专栏 - CSDN博客
2014年02月07日 18:25:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3712
                
java.net.ProtocolException: Exceeded stated content-length of: '13824' bytes  用jspSmartUpload.jar下载 出错：
原因：
因为weblogic会向response中写东西造成的，解决方式是将此句response.reset();的注释打开，这样在使用response时先将其重置。
<%
 response.reset();//如果在weblogic底下同样要加上此句
 String filePath = request.getParameter("filePath");
 String disName = request.getParameter("disName");
  // 新建一个SmartUpload对象
 SmartUpload su = new SmartUpload();
  // 初始化 
 su.initialize(pageContext);
  // 设定contentDisposition为null以禁止浏览器自动打开文件，
  //保证点击链接后是下载文件。若不设定，则下载的文件扩展名为
  //doc时，浏览器将自动用word打开它。扩展名为pdf时，
  //浏览器将用acrobat打开。
 su.setContentDisposition(null);
  // 下载文件
    String fileName = toUtf8String(disName);
 try{
  su.downloadFile(filePath,null,fileName);
    }
    catch(java.io.FileNotFoundException e){
 %>
<script Language="javascript">
 alert('服务器上未找到要下载的文件！');
</script>
<%}
%>下载完毕！
至此文件下载介绍完毕。
            
