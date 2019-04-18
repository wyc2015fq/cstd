# 简单理解java.lang.IllegalStateException异常:简单分析和简单解决方案 - z69183787的专栏 - CSDN博客
2014年02月07日 18:31:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8930
                
我们在做文件上传或者下载，或者过滤等操作时，可能要用到页面的输出流.
例如在JSP使用：
　　　　response.reset();
　　　　response.setContentType(”application/vnd.ms-excel”);
　　　　OutputStream s = response.getOutputStream();　　
抛出异常：
ERROR [Engine] StandardWrapperValve[jsp]: Servlet.service() for servlet jsp threw exception
java.lang.IllegalStateException: getOutputStream() has already been called for this response
从网上找了下资料，综合一下原因分析：
这是web容器生成的servlet代码中有out.write(””)，这个和JSP中调用的response.getOutputStream()产生冲突.
即Servlet规范说明，不能既调用 response.getOutputStream()，又调用response.getWriter()，无论先调用哪一个，在调用第二个时候应会抛出 IllegalStateException，因为在jsp中，out变量是通过response.getWriter得到的，在程序中既用了response.getOutputStream，又用了out变量，故出现以上错误。
解决方案：
1.在程序中添加：
out.clear();
out = pageContext.pushBody();
就可以了；
2，不要在%］［%之间写内容包括空格和换行符
3，在页面写入图片的时候，需要flush（）
　OutputStream utput=response.getOutputStream();
　output.flush();
4，在页面确定写入<meta. http-equiv=”Content-Type” content=”text/html; charset=gb2312”>
            
