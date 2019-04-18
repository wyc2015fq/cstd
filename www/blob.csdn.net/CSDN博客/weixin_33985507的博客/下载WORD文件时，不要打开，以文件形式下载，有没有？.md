# 下载WORD文件时，不要打开，以文件形式下载，有没有？ - weixin_33985507的博客 - CSDN博客
2019年01月07日 23:33:19[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：25
**下载WORD文件时，不要打开，以文件形式下载，有没有？**
      因为我有一个11页WORD文件，点击显示内容，很慢，有没有以文件形式下载，请各位高手们帮帮忙，谢谢！
![](http://www.lslnet.com/images/icon_new.gif)关注
![](http://www.lslnet.com/images/icon_new.gif)你把它打包成RAR格式的文件下载就行了吗~~
![](http://www.lslnet.com/images/icon_new.gif)用数据流
![](http://www.lslnet.com/images/icon_new.gif)方法一： 在连接旁边写明 （请单击右楗选择 目标另存为）
            方法二：注明请安装 ie6 sp1 以上版本下载
            方法三：如果服务器支持动态语言，发送 application/octet-stream 的 content-type 文件头，再发送文件
![](http://www.lslnet.com/images/icon_new.gif)我试了楼上的方法，还是不行啊，能否说详细一些呢？
            我的是asp系统。
![](http://www.lslnet.com/images/icon_new.gif)[code]
            直接让客户端浏览器下载已知类型的文件    net_lover（原作）  
            Web开发人员都有过这样的疑问，如何让一个文件，尤其是一个已知类型的文件，发送到客户端，直接提示让浏览者下载，而不是用与它相关联的程序打开。以前我们最常用的办法就是把这样的文件加到链接上，这样可以让浏览者通过点击鼠标右键的目标另存为来下载所链接的文件。但是，这样有两个不足的地方：
            一是：如果浏览器能够识别已下载文件的扩展名，则浏览器就会激活该扩展名所关联的程序来打开所下载的文件。比如：在Windows平台上，如果用户点击的链接链接的是一个“.doc”文件的话，那么，浏览器就会启动Microsoft Word应用程序来打开它。
            二是：如果采用链接的办法的话，任何能看到该链接的人都可以下载该文件，你虽然也可以对所下载的文件进行权限设置，但那样做也不是很方便的。有时候我们需要更为灵活和富有弹性的方式，下面的程序能够很方便地克服以上两方面的不足。
            这种办法是可靠的，但你必须记住：没有授权的用户不能够通过在浏览器地址栏里输入文件的URL来取得该文件的下载权。所以，要下载的文件应该放到虚拟目录之外的一个目录里，比如：如果你的虚拟目录是C:\Mengxianhui\Tomcat4\Website\MyApp的话，那么，存放在该目录和该目录下的任何子目录下所有文件对因特网上的任何用户都是可见的。要直接下载一个文件，我们需要做两件事，第一件事是：设定响应的内容类为“application/octet-stream”，大小写无关。第二件事是：设置HTTP的响应头名字为：Content-Disposition，设定值为：attachment; filename = theFileName。这里的theFileName就是出现在文件下载对话框里的默认文件名，通常和所下载的文件名字相同，但也可以不同。下面，我们就平常最常用的JSP和ASP页面来举一个实际应用的例子。
              TestFileDownload.JSP页面的例子：
              <%
              // 得到文件名字和路径
              String filename = "MengxianhuiDocTest.doc";
              String filepath = "D:\\";
              // 设置响应头和下载保存的文件名
              response.setContentType("APPLICATION/OCTET-STREAM");
              response.setHeader("Content-Disposition",
              "attachment; filename=\"" + filename + "\"");
              // 打开指定文件的流信息
              java.io.FileInputStream fileInputStream =
               new java.io.FileInputStream(filepath + filename);
              // 写出流信息
              int i;
              while ((i=fileInputStream.read()) != -1) {
               out.write(i);
              }
              fileInputStream.close();
              out.close();
            %>
            值得注意的是：在你要下载的文件内容里，除了文件的内容之外，不应该再附加有其它任何的字符，包括空格和回车换行符。我们有时在编写代码的时候，为了使代码清晰可读，往往会添加一些空格、制表符或者回车换行符，这样虽然看起来比较清晰，但有时可能会得不到正确的结果。比如：
            <%@ page import="java.io.*"
            %>  <jsp:useBean id="MyBeanFromMengxianhui" scope="page"
            class="com.Mengxianhui.DownloadBean" />
            应该写成这样：
            <%@ page import="java.io.*"
            %><jsp:useBean id="MyBeanFromMengxianhui" scope="page"
            class="com.Mengxianhui.DownloadBean" />
            TestFileDownload.ASP页面的例子：
            在ASP里，没有提供从文件读取文件流信息的方法，因此，为了得到文件的流信息，我们必须借助其他的工具，最简单的就是编写一个VB或C的DLL组件，让组件返回文件的流信息。下面是一个用VB编写的DLL的例子，工程名字为MengXHFileDownLoad，类模块的名字为BinReadFromFile，类方法readBinFromFile如下：
            Function readBinFromFile(ByVal bfilename As String) As Variant
                 Dim fl As Long
                 Dim FileNum As Long
                 Dim binbyte() As Byte
                 Dim binfilestr As String
                 On Error GoTo errHandler     
                 FileNum = FreeFile     
                 Open bfilename For Binary As #FileNum     
                 fl = FileLen(bfilename)
                 ReDim binbyte(fl)     
                 Get #FileNum, , binbyte     
                 Close #FileNum     
                 readBinFromFile = binbyte
                 Exit Function
            errHandler:
                 Exit Function
            End Function
            把上面的代码编译成MengXHFileDownLoad.DLL，然后注册即可使用。下面以直接下载一个When A Man Loves A Woman.mp3的MP3文件为例子，我们要编写的ASP脚本代码如下：
              <%@ Language=VBScript %>
              <%
               Response.buffer = TRUE
               Response.ContentType = "APPLICATION/OCTET-STREAM"
               Response.AddHeader "Content-Disposition","attachment;filename=When A Man Loves A Woman.mp3"
               Dim varStream, oMyObject
               Set oMyObject = Server.CreateObject("MengXHFileDownLoad.BinReadFromFile")
               varStream = oMyObject.readBinFromFile("E:\MengXianhui\Mp3\When A Man Loves A Woman.mp3")
               Response.BinaryWrite(varStream) 
               Set oMyObject = Nothing 
               Response.End
              %>
            当我们运行上面的TestFileDownload.ASP文件时，浏览器会弹出一个文件下载的对话框，提示我们下载，而不是用默认的MP3播放器打开。
            这种方法也可以把我们的ASP页面生成的HTML源代码保存成一个文件，下面的代码会提示你把ASP执行的结果保存成Test.htm文件。具体的方法是：
              <%
               Response.ContentType = "APPLICATION/OCTET-STREAM"
               Response.AddHeader "Content-Disposition","attachment;filename=Test.htm"
               Response.write "<div style='background-color:navy;color:#FFFFFF'>测试</div>"
               Response.write "<a href='http://lucky.myrice.com'>"
               Response.write "<img src='http://lucky.myrice.com/back.jpg'>【孟宪会之精彩世界】</a>"
               Response.End
              %>
            当我们的文件数目很少时，也可以直接在服务器端进行设置，让这些文件直接下载。具体做法是：在Internet服务管理器里，选“属性”项，然后选“HTTP Headers”标签页进行设置即可！！
            [/code]
![](http://www.lslnet.com/images/icon_new.gif)**ASP 是这样实现的！**
            <%
            '下载函数
            'filename不为空时，将file文件改名为filename下载
            call downloadFile(Request("file"),request("filename"))
            Function downloadFile(strFile,filename) 
            '得到指定文件的路径
            strFilename = server.MapPath(strFile) 
            'response.Write(StrFile)
            'response.End()
            ' 清除缓存 
            Response.Buffer = True 
            Response.Clear 
            ' 创建数据流
            Set s = Server.CreateObject("ADODB.Stream") 
            s.Open 
            ' Set as binary 
            s.Type = 1 
            ' load in the file 
            on error resume next 
            ' 检测文件是否存在
            Set fso = Server.CreateObject("Scripting.FileSystemObject") 
            if not fso.FileExists(strFilename) then 
            Response.Write("<script>alert('您要下载的文件不存在！');history.back();</script>") 
            Response.End 
            end if 
            ' 得到了文件的长度 
            Set f = fso.GetFile(strFilename) 
            intFilelength = f.size 
            s.LoadFromFile(strFilename) 
            if err then 
            Response.Write("<script>alert('"&err.Description&"');history.back();</script>") 
            Response.End 
            end if 
            ' 重新定义浏览窗口头文件，用来下载流文件 
            if filename<>"" then
            Response.AddHeader "Content-Disposition", "attachment; filename=" & filename
            else
            Response.AddHeader "Content-Disposition", "attachment; filename=" & f.name 
            end if
            Response.AddHeader "Content-Length", intFilelength 
            Response.CharSet = "UTF-8" 
            Response.ContentType = "application/octet-stream" 
            ' 在可户端实现下载的功能
            Response.BinaryWrite s.Read 
            Response.Flush  
            ' 最后整理
            s.Close 
            Set s = Nothing 
            End Function 
            %>
