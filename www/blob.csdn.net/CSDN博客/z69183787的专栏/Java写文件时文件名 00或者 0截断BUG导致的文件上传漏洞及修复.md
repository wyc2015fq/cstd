# Java写文件时文件名\00或者\0截断BUG导致的文件上传漏洞及修复 - z69183787的专栏 - CSDN博客
2014年07月09日 11:30:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4309
Java在上面两种环境写文件时，会因为\00截断而无法正确为新生成的文件命名。比如用户需要的用户名abc.jsp .jpg，但经过00截断后，生成的文件的名称变为abc.jsp , 因此我们在涉及到上传的文件名没更改名称或者可自定义目录的时候加以利用
**测试环境:**
1.windows7(x64)+tomcat7+jdk1.6
2.Linux3.0(ubuntu11.10)(x86)+tomcat7+jdk1.7
Java在上面两种环境写文件时，会因为00截断而无法正确为新生成的文件命名。比如用户需要的用户名abc.jsp .jpg，但经过00截断后，生成的文件的名称变为abc.jsp , 因此我们在涉及到上传的文件名没更改名称或者可自定义目录的时候加以利用。
测试发送的头部数据如下：
POST /simpleUpload/write.jsp HTTP/1.1
Accept: application/x-shockwave-flash, image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, */*
Accept-Language: zh-cn
Content-Type: application/x-www-form-urlencoded
Accept-Encoding: gzip, deflate
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)
Host: 192.168.200.142:8084
Content-Length: 17
Connection: Keep-Alive
Cache-Control: no-cache
Cookie: JSESSIONID=D2EC5F95AD581EB5FD3A860FC4CE640
name=abc.jsp .jpg(注意在上传前，这里的空格需要我们用十六进制编辑器将其变为00)
测试的服务端代码如下:
<%@page import="java.io.*"%>
<%
    out.clear();
    String filename = request.getParameter("name");
    if (filename != null) {
        String path = application.getRealPath("/");
        String p=path + "/" + filename;
        File uploadfile = new File(p);
        if (!uploadfile.exists()) {
            uploadfile.createNewFile();
        }
        out.println("System Name:"+System.getProperty("os.name"));
        out.println("1.The information of UploadFile:");
        if(uploadfile!=null){    
            out.println("   a.the UploadFile  exists!");
            out.println("   b.The path of UploadFile:    "+uploadfile.getAbsolutePath());
            out.println("   c.The name of UploadFile:    "+uploadfile.getName());
            p=uploadfile.getAbsolutePath().substring(0,uploadfile.getAbsolutePath().length()-5);
            File bugFile=new File(p);
            out.println("2.The information of BugFile:");
            if(bugFile.exists()){ 
                out.println("   a.The BugFile  exists!");
                out.println("   b.The path of BugFile:    "+bugFile.getAbsolutePath());
                out.println("   c.The name of BugFile:   "+bugFile.getName());
            }else{
                out.println("The BugFile: "+bugFile+"  does't exist!");
            }
            File uploadfile2 = new File(p+uploadfile.getAbsolutePath().substring(uploadfile.getAbsolutePath().length()-5));
            out.println("3.Assure whether the nonexistent  UploadFile exists because of the java API or not:");
            if(uploadfile2.exists()){
                out.println("   a.The nonexistent  UploadFile  exists!");
                out.println("   b.The path of nonexistent  UploadFile:    "+uploadfile2.getAbsolutePath());
                out.println("   c.The name of nonexistent  UploadFile:   "+uploadfile2.getName());
            }else{
                out.println("The nonexistent  UploadFile: "+uploadfile2+" does't exist!");
            }
        }else
            out.println("The UploadFile: "+uploadfile+"  isn't uploaded successfully!");
    } else {
        out.println("Null name!");
    }
    out.flush();
%>
1.在windows7(x64)+tomcat7+jdk1.6环境下提交的数据返回结果的截图：
![](http://up.2cto.com/2012/1227/20121227105730944.jpg)
2.在Linux3.0(ubuntu11.10)(x86)+tomcat7+jdk1.7环境下提交的数据返回结果的截图：
![](http://up.2cto.com/2012/1227/20121227105731558.jpg)
从上图我们可以看到：
1点成功了,表示文件已经上传成功了,并且文件名abc.jsp\00.jpg没变，且java认为这个文件存在的。
2点也成功了，表明 abc.jps存在.
3我们用abc.jsp组全\00.jpg去确认这个文件是否存在，结果java认为存在。
注:(这里的00表示16进制字符)
当我们打开对应的目录时，发现只有abc.jsp存在。这说明文件名00截断是JAVA的原因。而不是系统的原因。
为了不让web shell由于这个[漏洞](http://www.2cto.com/)而得以上传，推荐你使用fckeditor的方法，用一个正则表达式替换用户可以定义的路径名或者文件名，代码如下
filename = filename.replaceAll("\\/|\\/|\\||:|\\?|\\*|\"|<|>|\\p{Cntrl}", "_");（正则表达中\\p{Cntrl}这个是处理00字符的。）
**POSIX字符类(仅应用于us-ascii)**
**\P{name}, name是以上名称中的一个. 表示名称name指定字符集外的字符, [^\p{name}]**
