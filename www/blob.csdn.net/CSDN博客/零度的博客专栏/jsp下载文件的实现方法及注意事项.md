# jsp下载文件的实现方法及注意事项 - 零度的博客专栏 - CSDN博客
2016年05月13日 17:21:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：4349
       jsp中实现文件下载，最简单的方式是在网页上做超级链接，如：<a href="music/abc.mp3">点击下载</a>。 但是，这样服务器上的目录资源会直接暴露给最终用户，会给网站带来一些不安全的因素。 因此，可以采用其它方式实现下载，常使用的有以下两种： 
      1、RequestDispatcher的方式进行； 
      2、采用文件流输出的方式下载（推荐）。 
1、采用RequestDispatcher的方式：
Jsp代码  ![收藏代码](http://aguu125.iteye.com/images/icon_star.png)
- <%  
-   response.setContentType("application/x-download");//设置为下载application/x-download  
-   String filedownload = "/要下载的文件名"; //即将下载的文件的相对路径  
-   String filedisplay = "最终要显示给用户的保存文件名"; //下载文件时显示的文件保存名称  
-   String filenamedisplay = URLEncoder.encode(filedisplay,"UTF-8");  
-   response.addHeader("Content-Disposition","attachment;filename=" + filenamedisplay); 
-   try{  
-        RequestDispatcher dis = application.getRequestDispatcher(filedownload);  
-        if(dis!= null){  
-            dis.forward(request,response);  
-        }  
-        response.flushBuffer();  
-   }catch(Exception e){  
-        e.printStackTrace();  
-   }finally{  
- 
-   }  
- %>  
2、采用文件流输出的方式下载 
Jsp代码  ![收藏代码](http://aguu125.iteye.com/images/icon_star.png)
- <%@page language="java" contentType="application/x-msdownload" pageEncoding="gb2312"%>  
- <%  
-   //关于文件下载时采用文件流输出的方式处理：  
-   //加上response.reset()，并且所有的％>后面不要换行，包括最后一个；  
-   response.reset();//可以加也可以不加  
-   response.setContentType("application/x-download");  
- //application.getRealPath("/main/mvplayer/CapSetup.msi");获取的物理路径  
- 
-   String filedownload = "想办法找到要提供下载的文件的物理路径＋文件名";  
-   String filedisplay = "给用户提供的下载文件名";  
-   String filedisplay = URLEncoder.encode(filedisplay,"UTF-8");  
-   response.addHeader("Content-Disposition","attachment;filename=" + filedisplay);  
- 
-   java.io.OutputStream outp = null;  
-   java.io.FileInputStream in = null;  
-   try {  
-         outp = response.getOutputStream();  
-         in = new FileInputStream(filenamedownload);  
-         byte[] b = new byte[1024];  
-         int i = 0;  
-         while((i = in.read(b)) > 0) {  
-                outp.write(b, 0, i);  
-          }  
-         outp.flush();  
-         //要加以下两句话，否则会报错  
-         //java.lang.IllegalStateException: getOutputStream() has already been called for //this response    
-         out.clear();  
-         out = pageContext.pushBody();  
-  }  catch(Exception e){  
-          System.out.println("Error!");  
-          e.printStackTrace();  
-   }finally{  
-          if(in != null){  
-                in.close();  
-                in = null;  
-           }  
-          //这里不能关闭    
-          //if(outp != null)  {  
-                //outp.close();  
-                //outp = null;  
-          //}  
-   }  
- %>  
       对于第二种方法，我认为应该是比较常用的。不过有几个地方是值得我们注意的： 
       1、采用第二种方法的主要优点是实际文件的存放路径对客户端来说是透明的。 
       这个文件可以存在于任何你的服务器能够取得到的地方，而客户端不一定能直接得到。例如文件来自于数据库或者内部网络的一个FTP服务器。还句话说，这种方式可以实现隐藏实际文件的URL地址。
        2、为了防止客户端浏览器直接打开目标文件。
       例如在装了MS Office套件的Windows中的IE浏览器可能就会直接在IE浏览器中打开你想下载的doc或者xls文件。你必须在响应头里加入强制下载的MIME类型：response.setContentType("application/force-download");
 //设置为下载application/force-download。这样，就可以保证在用户点击下载链接的时候浏览器一定会弹出提示窗口来询问你是下载还是直接打开并允许你选择要打开的应用程序，除非你设置了浏览器的一些默认行为。 或者，你想让客户端自行处理各种不同的文件类型，你可以在服务器的配置文件中配置MIME类型映射，通过简单的判断文件后缀名来处理。例如，在Tomcat中设置MIME响应类型： 如果文件在客户端中的响应程序类型和期望不一致，修改$TOMCAT_HOME\conf\web.xml文件中的如下部分
 ： 
```
<span style="font-family:Arial;font-size:12px;"><mime-mapping> 
  <extension>zip</extension> 
  <mime-type>application/zip</mime-type> 
</mime-mapping> 
<mime-mapping> 
  <extension>mht</extension> 
  <mime-type>message/rfc822</mime-type> 
</mime-mapping> 
…… </span>
```
        3、在响应头中尽量不要设置浏览器缓存期限。 
        有时候用户在点击了下载链接后，在弹出窗口中，用户想直接点击“打开”，而不想保存到指定路径。这时候如果我们在响应头中限制了不允许使用浏览器缓存（即总是刷新），在IE浏览器中我们将无法直接打开该文件。因为限制了不允许使用缓存，浏览器无法将文件保存到临时文件夹（即缓存）。也就是说，在响应头中不要进行如下的设置（已注释）： 
```
<span style="font-family:Arial;font-size:12px;">        //response.addHeader("pragma","NO-cache"); 
        //response.addHeader("Cache-Control","no-cache"); 
        //response.addDateHeader("Expries",0);</span>
```
        4、文件名为中文或其他unicode字符时的处理。 
        有时候提供下载的文件名中包含中文字符或者其他unicode字符，会导致浏览器无法正确的采用默认的文件名保存文件。我们应该记住在响应头中包含filename字段并采用ISO8859-1编码（推荐）或者采用UTF-8编码： 
        response.setHeader("Content-disposition","attachment; filename="+new String(filename.getBytes("UTF-8"),"iso8859-1")); //采用ISO8859-1编码 
        response.setHeader("Content-disposition","attachment; filename="+URLEncoder.encode(filename, "UTF-8")); //采用UTF-8编码 
        但是，这种方式在不同的浏览器中表现也有所不同。例如在IE和Firefox中，采用ISO8859-1编码可以正确显示文件名，而在Opera中不管采用那种编码，默认保存的文件名都无法做到正确显示。 所以，最好的方法其实就是尽量在文件名中使用ascii编码。 
        5、由于采用流的方式进行输入输出，我们必须保证在使用完毕后关闭流的资源。 
         一般我们把关闭流的操作放在finally块中，以保证在程序段结束前一定会关闭流的资源。如下面代码：
```
<span style="font-family:Arial;font-size:12px;">InputStream is = null; 
ServletOutputStream sos = null; 
try { 
       is = ...; //通过某种方式读进数据到输入流 
       os = response.getOutputStream(); //打开输出流 
       byte[] buff = new byte[2048]; 
       int bytesRead=0; 
       while(-1 != (bytesRead = bis.read(buff, 0, buff.length))) { 
             os.write(buff,0,bytesRead); 
             os.flush(); 
        } 
} catch(Exception ex) { 
  //TODO something with Exception 
} finally { 
  if(is != null) { 
          is.close(); //关闭输入流 
  } 
  if(os != null) { 
          os.close(); //关闭输入流 
  } 
}</span>
```

