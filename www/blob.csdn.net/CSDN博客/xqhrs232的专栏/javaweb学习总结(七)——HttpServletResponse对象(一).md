# javaweb学习总结(七)——HttpServletResponse对象(一) - xqhrs232的专栏 - CSDN博客
2018年11月12日 17:04:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：44
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/xdp-gacl/p/3789624.html](https://www.cnblogs.com/xdp-gacl/p/3789624.html)
相关文章
1、[JavaWeb学习总结（六）—HttpServletResponse](https://www.cnblogs.com/zydev/p/6012337.html)----[https://www.cnblogs.com/zydev/p/6012337.html](https://www.cnblogs.com/zydev/p/6012337.html)
2、httpservletrequest和httpservletresponse的理解----[https://www.aliyun.com/jiaocheng/773064.html](https://www.aliyun.com/jiaocheng/773064.html)
3、[HttpServletResponse对象](https://www.cnblogs.com/lxboy2009/p/5935251.html)----[https://www.cnblogs.com/lxboy2009/p/5935251.html](https://www.cnblogs.com/lxboy2009/p/5935251.html)
Web服务器收到客户端的http请求，会针对每一次请求，分别创建一个用于代表请求的request对象、和代表响应的response对象。
request和response对象即然代表请求和响应，那我们要获取客户机提交过来的数据，只需要找request对象就行了。要向客户机输出数据，只需要找response对象就行了。
## 一、HttpServletResponse对象介绍
![](https://img-blog.csdnimg.cn/20181112170418765.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
　　HttpServletResponse对象代表服务器的响应。这个对象中封装了向客户端发送数据、发送响应头，发送响应状态码的方法。查看HttpServletResponse的API，可以看到这些相关的方法。
### 1.1、负责向客户端(浏览器)发送数据的相关方法
![](https://img-blog.csdnimg.cn/20181112170418870.png)
### 1.2、负责向客户端(浏览器)发送响应头的相关方法
![](https://img-blog.csdnimg.cn/20181112170418874.png)
![](https://img-blog.csdnimg.cn/20181112170418895.png)
### 1.3、负责向客户端(浏览器)发送响应状态码的相关方法
![](https://img-blog.csdnimg.cn/20181112170418928.png)
### 1.4、响应状态码的常量
　　HttpServletResponse定义了很多状态码的常量(具体可以查看Servlet的API)，当需要向客户端发送响应状态码时，可以使用这些常量，避免了直接写数字，常见的状态码对应的常量：
　　状态码404对应的常量
![](https://img-blog.csdnimg.cn/20181112170418934.png)
　　状态码200对应的常量
![](https://img-blog.csdnimg.cn/20181112170418941.png)
　　状态码500对应的常量
![](https://img-blog.csdnimg.cn/20181112170418986.png)
## 二、HttpServletResponse对象常见应用
###  2.1、使用OutputStream流向客户端浏览器输出中文数据
**使用OutputStream流输出中文注意问题：**
　　在服务器端，数据是以哪个码表输出的，那么就要控制客户端浏览器以相应的码表打开，比如：outputStream.write("中国".getBytes("UTF-8"));使用OutputStream流向客户端浏览器输出中文，以UTF-8的编码进行输出，此时就要控制客户端浏览器以UTF-8的编码打开，否则显示的时候就会出现中文乱码，那么在服务器端如何控制客户端浏览器以以UTF-8的编码显示数据呢？可以通过设置响应头控制浏览器的行为，例如：response.setHeader("content-type", "text/html;charset=UTF-8");通过设置响应头控制浏览器以UTF-8的编码显示数据。
**范例：使用OutputStream流向客户端浏览器输出"中国"这两个汉字**
 1 package gacl.response.study;
 2 
 3 import java.io.IOException;
 4 import java.io.OutputStream;
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.HttpServlet;
 7 import javax.servlet.http.HttpServletRequest;
 8 import javax.servlet.http.HttpServletResponse;
 9 
10 public class ResponseDemo01 extends HttpServlet {
11 
12     private static final long serialVersionUID = 4312868947607181532L;
13 
14     public void doGet(HttpServletRequest request, HttpServletResponse response)
15             throws ServletException, IOException {
16         outputChineseByOutputStream(response);//使用OutputStream流输出中文
17     }
18     
19     /**
20      * 使用OutputStream流输出中文
21      * @param request
22      * @param response
23      * @throws IOException 
24      */
25     public void outputChineseByOutputStream(HttpServletResponse response) throws IOException{
26         /**使用OutputStream输出中文注意问题：
27          * 在服务器端，数据是以哪个码表输出的，那么就要控制客户端浏览器以相应的码表打开，
28          * 比如：outputStream.write("中国".getBytes("UTF-8"));//使用OutputStream流向客户端浏览器输出中文，以UTF-8的编码进行输出
29          * 此时就要控制客户端浏览器以UTF-8的编码打开，否则显示的时候就会出现中文乱码，那么在服务器端如何控制客户端浏览器以以UTF-8的编码显示数据呢？
30          * 可以通过设置响应头控制浏览器的行为，例如：
31          * response.setHeader("content-type", "text/html;charset=UTF-8");//通过设置响应头控制浏览器以UTF-8的编码显示数据
32          */
33         String data = "中国";
34         OutputStream outputStream = response.getOutputStream();//获取OutputStream输出流
35         response.setHeader("content-type", "text/html;charset=UTF-8");//通过设置响应头控制浏览器以UTF-8的编码显示数据，如果不加这句话，那么浏览器显示的将是乱码
36         /**
37          * data.getBytes()是一个将字符转换成字节数组的过程，这个过程中一定会去查码表，
38          * 如果是中文的操作系统环境，默认就是查找查GB2312的码表，
39          * 将字符转换成字节数组的过程就是将中文字符转换成GB2312的码表上对应的数字
40          * 比如： "中"在GB2312的码表上对应的数字是98
41          *         "国"在GB2312的码表上对应的数字是99
42          */
43         /**
44          * getBytes()方法如果不带参数，那么就会根据操作系统的语言环境来选择转换码表，如果是中文操作系统，那么就使用GB2312的码表
45          */
46         byte[] dataByteArr = data.getBytes("UTF-8");//将字符转换成字节数组，指定以UTF-8编码进行转换
47         outputStream.write(dataByteArr);//使用OutputStream流向客户端输出字节数组
48     }
49 
50     public void doPost(HttpServletRequest request, HttpServletResponse response)
51             throws ServletException, IOException {
52         doGet(request, response);
53     }
54 
55 }
 运行结果如下：
![](https://img-blog.csdnimg.cn/20181112170419364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
　　客户端浏览器接收到数据后，就按照响应头上设置的字符编码来解析数据，如下所示：
![](https://img-blog.csdnimg.cn/2018111217041944.png)
### 2.2、使用PrintWriter流向客户端浏览器输出中文数据
**使用PrintWriter流输出中文注意问题：**
　　在获取PrintWriter输出流之前首先使用"response.setCharacterEncoding(charset)"设置字符以什么样的编码输出到浏览器，如：response.setCharacterEncoding("UTF-8");设置将字符以"UTF-8"编码输出到客户端浏览器，然后再使用response.getWriter();获取PrintWriter输出流，这两个步骤不能颠倒，如下：
1 response.setCharacterEncoding("UTF-8");//设置将字符以"UTF-8"编码输出到客户端浏览器
2 /**
3 * PrintWriter out = response.getWriter();这句代码必须放在response.setCharacterEncoding("UTF-8");之后
4 * 否则response.setCharacterEncoding("UTF-8")这行代码的设置将无效，浏览器显示的时候还是乱码
5 */
6 PrintWriter out = response.getWriter();//获取PrintWriter输出流
　　然后再使用response.setHeader("content-type", "text/html;charset=字符编码");设置响应头，控制浏览器以指定的字符编码编码进行显示，例如：
1 //通过设置响应头控制浏览器以UTF-8的编码显示数据，如果不加这句话，那么浏览器显示的将是乱码
2 response.setHeader("content-type", "text/html;charset=UTF-8");
　　除了可以使用response.setHeader("content-type", "text/html;charset=字符编码");设置响应头来控制浏览器以指定的字符编码编码进行显示这种方式之外，还可以用如下的方式来模拟响应头的作用
1 /**
2 * 多学一招：使用HTML语言里面的<meta>标签来控制浏览器行为，模拟通过设置响应头控制浏览器行为
3  *response.getWriter().write("<meta http-equiv='content-type' content='text/html;charset=UTF-8'/>");
4 * 等同于response.setHeader("content-type", "text/html;charset=UTF-8");
5 */
6 response.getWriter().write("<meta http-equiv='content-type' content='text/html;charset=UTF-8'/>");
**范例：使用PrintWriter流向客户端浏览器输出"中国"这两个汉字**
 1 package gacl.response.study;
 2 
 3 import java.io.IOException;
 4 import java.io.OutputStream;
 5 import java.io.PrintWriter;
 6 import javax.servlet.ServletException;
 7 import javax.servlet.http.HttpServlet;
 8 import javax.servlet.http.HttpServletRequest;
 9 import javax.servlet.http.HttpServletResponse;
10 
11 public class ResponseDemo01 extends HttpServlet {
12 
13     private static final long serialVersionUID = 4312868947607181532L;
14 
15     public void doGet(HttpServletRequest request, HttpServletResponse response)
16             throws ServletException, IOException {
17         outputChineseByPrintWriter(response);//使用PrintWriter流输出中文
18     }
19 
20     /**
21      * 使用PrintWriter流输出中文
22      * @param request
23      * @param response
24      * @throws IOException 
25      */
26     public void outputChineseByPrintWriter(HttpServletResponse response) throws IOException{
27         String data = "中国";
28         
29         //通过设置响应头控制浏览器以UTF-8的编码显示数据，如果不加这句话，那么浏览器显示的将是乱码
30         //response.setHeader("content-type", "text/html;charset=UTF-8");
31         
32         response.setCharacterEncoding("UTF-8");//设置将字符以"UTF-8"编码输出到客户端浏览器
33         /**
34          * PrintWriter out = response.getWriter();这句代码必须放在response.setCharacterEncoding("UTF-8");之后
35          * 否则response.setCharacterEncoding("UTF-8")这行代码的设置将无效，浏览器显示的时候还是乱码
36          */
37         PrintWriter out = response.getWriter();//获取PrintWriter输出流
38         /**
39          * 多学一招：使用HTML语言里面的<meta>标签来控制浏览器行为，模拟通过设置响应头控制浏览器行为
40          * out.write("<meta http-equiv='content-type' content='text/html;charset=UTF-8'/>");
41          * 等同于response.setHeader("content-type", "text/html;charset=UTF-8");
42          */
43         out.write("<meta http-equiv='content-type' content='text/html;charset=UTF-8'/>");
44         out.write(data);//使用PrintWriter流向客户端输出字符
45     }
46     
47     public void doPost(HttpServletRequest request, HttpServletResponse response)
48             throws ServletException, IOException {
49         doGet(request, response);
50     }
51 }
 　　当需要向浏览器输出字符数据时，使用PrintWriter比较方便，省去了将字符转换成字节数组那一步。
### 2.3、使用OutputStream或者PrintWriter向客户端浏览器输出数字
**比如有如下的代码：**
 1 package gacl.response.study;
 2 
 3 import java.io.IOException;
 4 import java.io.OutputStream;
 5 import java.io.PrintWriter;
 6 
 7 import javax.servlet.ServletException;
 8 import javax.servlet.http.HttpServlet;
 9 import javax.servlet.http.HttpServletRequest;
10 import javax.servlet.http.HttpServletResponse;
11 
12 public class ResponseDemo01 extends HttpServlet {
13 
14     private static final long serialVersionUID = 4312868947607181532L;
15 
16     public void doGet(HttpServletRequest request, HttpServletResponse response)
17             throws ServletException, IOException {
18         
19         outputOneByOutputStream(response);//使用OutputStream输出1到客户端浏览器
20         
21     }
22 
23     /**
24      * 使用OutputStream流输出数字1
25      * @param request
26      * @param response
27      * @throws IOException 
28      */
29     public void outputOneByOutputStream(HttpServletResponse response) throws IOException{
30         response.setHeader("content-type", "text/html;charset=UTF-8");
31         OutputStream outputStream = response.getOutputStream();
32         outputStream.write("使用OutputStream流输出数字1：".getBytes("UTF-8"));
33         **outputStream.write(1);**
34     }
35     
36 }
　　运行上面代码显示的结果如下：
![](https://img-blog.csdnimg.cn/20181112170419352.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
　　运行的结果和我们想象中的不一样，数字1没有输出来，下面我们修改一下上面的outputOneByOutputStream方法的代码，修改后的代码如下：
 1     /**
 2      * 使用OutputStream流输出数字1
 3      * @param request
 4      * @param response
 5      * @throws IOException 
 6      */
 7     public void outputOneByOutputStream(HttpServletResponse response) throws IOException{
 8         response.setHeader("content-type", "text/html;charset=UTF-8");
 9         OutputStream outputStream = response.getOutputStream();
10         outputStream.write("使用OutputStream流输出数字1：".getBytes("UTF-8"));
11         //outputStream.write(1);
12        ** outputStream.write((1+"").getBytes());**13     }
**1+""**这一步是将数字1和一个空字符串相加，这样处理之后，数字1就变成了字符串1了，然后再将字符串1转换成字节数组使用OutputStream进行输出，此时看到的结果如下：
![](https://img-blog.csdnimg.cn/20181112170419330.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
　　这次可以看到输出来的1了，这说明了一个问题：**在开发过程中，如果希望服务器输出什么浏览器就能看到什么，那么在服务器端都要以字符串的形式进行输出**。
　　如果使用PrintWriter流输出数字，那么也要先将数字转换成字符串后再输出，如下：
 1     /**
 2      * 使用PrintWriter流输出数字1
 3      * @param request
 4      * @param response
 5      * @throws IOException 
 6      */
 7     public void outputOneByPrintWriter(HttpServletResponse response) throws IOException{
 8         response.setHeader("content-type", "text/html;charset=UTF-8");
 9         response.setCharacterEncoding("UTF-8");
10         PrintWriter out = response.getWriter();//获取PrintWriter输出流
11         out.write("使用PrintWriter流输出数字1：");
12         out.write(1+"");
13     }
###  2.4、文件下载
　　文件下载功能是web开发中经常使用到的功能，使用HttpServletResponse对象就可以实现文件的下载
文件下载功能的实现思路：
　　1.获取要下载的文件的绝对路径
　　2.获取要下载的文件名
　　3.设置content-disposition响应头控制浏览器以下载的形式打开文件
　　4.获取要下载的文件输入流
　　5.创建数据缓冲区
　　6.通过response对象获取OutputStream流
　　7.将FileInputStream流写入到buffer缓冲区
　　8.使用OutputStream将缓冲区的数据输出到客户端浏览器
**范例：使用Response实现文件下载**
 1 package gacl.response.study;
 2 import java.io.FileInputStream;
 3 import java.io.FileNotFoundException;
 4 import java.io.FileReader;
 5 import java.io.IOException;
 6 import java.io.InputStream;
 7 import java.io.OutputStream;
 8 import java.io.PrintWriter;
 9 import java.net.URLEncoder;
10 import javax.servlet.ServletException;
11 import javax.servlet.http.HttpServlet;
12 import javax.servlet.http.HttpServletRequest;
13 import javax.servlet.http.HttpServletResponse;
14 /**
15  * @author gacl
16  * 文件下载
17  */
18 public class ResponseDemo02 extends HttpServlet {
19 
20     public void doGet(HttpServletRequest request, HttpServletResponse response)
21             throws ServletException, IOException {
22         downloadFileByOutputStream(response);//下载文件，通过OutputStream流
23     }
24 
25     /**
26      * 下载文件，通过OutputStream流
27      * @param response
28      * @throws FileNotFoundException
29      * @throws IOException
30      */
31     private void downloadFileByOutputStream(HttpServletResponse response)
32             throws FileNotFoundException, IOException {
33         //1.获取要下载的文件的绝对路径
34         String realPath = this.getServletContext().getRealPath("/download/1.JPG");
35         //2.获取要下载的文件名
36         String fileName = realPath.substring(realPath.lastIndexOf("\\")+1);
37         //3.设置content-disposition响应头控制浏览器以下载的形式打开文件
38         response.setHeader("content-disposition", "attachment;filename="+fileName);
39         //4.获取要下载的文件输入流
40         InputStream in = new FileInputStream(realPath);
41         int len = 0;
42         //5.创建数据缓冲区
43         byte[] buffer = new byte[1024];
44         //6.通过response对象获取OutputStream流
45         OutputStream out = response.getOutputStream();
46         //7.将FileInputStream流写入到buffer缓冲区
47         while ((len = in.read(buffer)) > 0) {
48         //8.使用OutputStream将缓冲区的数据输出到客户端浏览器
49             out.write(buffer,0,len);
50         }
51         in.close();
52     }
53 
54     public void doPost(HttpServletRequest request, HttpServletResponse response)
55             throws ServletException, IOException {
56         doGet(request, response);
57     }
58 }
**运行结果如下所示：**
![](https://img-blog.csdnimg.cn/20181112170419321.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
**范例：****使用Response实现中文文件下载**
**　　下载中文文件时，需要注意的地方就是中文文件名要使用URLEncoder.encode方法进行编码(URLEncoder.encode(fileName, "字符编码"))，否则会出现文件名乱码。**
 1 package gacl.response.study;
 2 import java.io.FileInputStream;
 3 import java.io.FileNotFoundException;
 4 import java.io.FileReader;
 5 import java.io.IOException;
 6 import java.io.InputStream;
 7 import java.io.OutputStream;
 8 import java.io.PrintWriter;
 9 import java.net.URLEncoder;
10 import javax.servlet.ServletException;
11 import javax.servlet.http.HttpServlet;
12 import javax.servlet.http.HttpServletRequest;
13 import javax.servlet.http.HttpServletResponse;
14 /**
15  * @author gacl
16  * 文件下载
17  */
18 public class ResponseDemo02 extends HttpServlet {
19 
20     public void doGet(HttpServletRequest request, HttpServletResponse response)
21             throws ServletException, IOException {
22         downloadChineseFileByOutputStream(response);//下载中文文件
23     }
24 
25     /**
26      * 下载中文文件,中文文件下载时，文件名要经过URL编码，否则会出现文件名乱码
27      * @param response
28      * @throws FileNotFoundException
29      * @throws IOException
30      */
31     private void downloadChineseFileByOutputStream(HttpServletResponse response)
32             throws FileNotFoundException, IOException {
33         String realPath = this.getServletContext().getRealPath("/download/张家界国家森林公园.JPG");//获取要下载的文件的绝对路径
34         String fileName = realPath.substring(realPath.lastIndexOf("\\")+1);//获取要下载的文件名
35         //设置content-disposition响应头控制浏览器以下载的形式打开文件，中文文件名要使用URLEncoder.encode方法进行编码，否则会出现文件名乱码
36         response.setHeader("content-disposition", "attachment;filename="+URLEncoder.encode(fileName, "UTF-8"));
37         InputStream in = new FileInputStream(realPath);//获取文件输入流
38         int len = 0;
39         byte[] buffer = new byte[1024];
40         OutputStream out = response.getOutputStream();
41         while ((len = in.read(buffer)) > 0) {
42             out.write(buffer,0,len);//将缓冲区的数据输出到客户端浏览器
43         }
44         in.close();
45     }
46     
47     public void doPost(HttpServletRequest request, HttpServletResponse response)
48             throws ServletException, IOException {
49         doGet(request, response);
50     }
51 }
运行结果如下所示：
![](https://img-blog.csdnimg.cn/20181112170419410.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
　　文件下载注意事项：**编写文件下载功能时推荐使用OutputStream流，避免使用PrintWriter流，因为OutputStream流是字节流，可以处理任意类型的数据，而PrintWriter流是字符流，只能处理字符数据，如果用字符流处理字节数据，会导致数据丢失。**
**范例：使用PrintWriter流下载文件**
 1 package gacl.response.study;
 2 import java.io.FileInputStream;
 3 import java.io.FileNotFoundException;
 4 import java.io.FileReader;
 5 import java.io.IOException;
 6 import java.io.InputStream;
 7 import java.io.OutputStream;
 8 import java.io.PrintWriter;
 9 import java.net.URLEncoder;
10 import javax.servlet.ServletException;
11 import javax.servlet.http.HttpServlet;
12 import javax.servlet.http.HttpServletRequest;
13 import javax.servlet.http.HttpServletResponse;
14 /**
15  * @author gacl
16  * 文件下载
17  */
18 public class ResponseDemo02 extends HttpServlet {
19 
20     public void doGet(HttpServletRequest request, HttpServletResponse response)
21             throws ServletException, IOException {
22         downloadFileByPrintWriter(response);//下载文件，通过PrintWriter流
23     }
24 
25     /**
26      * 下载文件，通过PrintWriter流，虽然也能够实现下载，但是会导致数据丢失，因此不推荐使用PrintWriter流下载文件
27      * @param response
28      * @throws FileNotFoundException
29      * @throws IOException
30      */
31     private void downloadFileByPrintWriter(HttpServletResponse response)
32             throws FileNotFoundException, IOException {
33         String realPath = this.getServletContext().getRealPath("/download/张家界国家森林公园.JPG");//获取要下载的文件的绝对路径
34         String fileName = realPath.substring(realPath.lastIndexOf("\\")+1);//获取要下载的文件名
35         //设置content-disposition响应头控制浏览器以下载的形式打开文件，中文文件名要使用URLEncoder.encode方法进行编码
36         response.setHeader("content-disposition", "attachment;filename="+URLEncoder.encode(fileName, "UTF-8"));
37         FileReader in = new FileReader(realPath);
38         int len = 0;
39         char[] buffer = new char[1024];
40         PrintWriter out = response.getWriter();
41         while ((len = in.read(buffer)) > 0) {
42             out.write(buffer,0,len);//将缓冲区的数据输出到客户端浏览器
43         }
44         in.close();
45     }
46     
47     public void doPost(HttpServletRequest request, HttpServletResponse response)
48             throws ServletException, IOException {
49         doGet(request, response);
50     }
51 }
运行结果如下：
![](https://img-blog.csdnimg.cn/20181112170419416.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
正常弹出下载框，此时我们点击【保存】按钮将文件下载下来，如下所示：
![](https://img-blog.csdnimg.cn/20181112170419458.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
　　可以看到，只下载了5.25MB，而这张图片的原始大小却是
![](https://img-blog.csdnimg.cn/20181112170419514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
　　这说明在下载的时候数据丢失了，所以下载不完全，所以这张图片虽然能够正常下载下来，但是却是无法打开的，因为丢失掉了部分数据，如下所示：
![](https://img-blog.csdnimg.cn/20181112170419541.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
 　　所以使用PrintWriter流处理字节数据，会导致数据丢失，这一点千万要注意，因此**在编写下载文件功能时，要使用OutputStream流，避免使用PrintWriter流，因为OutputStream流是字节流，可以处理任意类型的数据，而PrintWriter流是字符流，只能处理字符数据，如果用字符流处理字节数据，会导致数据丢失。**
