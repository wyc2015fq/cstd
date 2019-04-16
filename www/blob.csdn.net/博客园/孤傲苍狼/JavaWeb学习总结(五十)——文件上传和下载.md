# JavaWeb学习总结(五十)——文件上传和下载 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaWeb学习总结(五十)——文件上传和下载](https://www.cnblogs.com/xdp-gacl/p/4200090.html)



　　在Web应用系统开发中，文件上传和下载功能是非常常用的功能，今天来讲一下JavaWeb中的文件上传和下载功能的实现。

　　对于文件上传，浏览器在上传的过程中是将文件以流的形式提交到服务器端的，如果直接使用Servlet获取上传文件的输入流然后再解析里面的请求参数是比较麻烦，所以一般选择采用apache的开源工具common-fileupload这个文件上传组件。这个common-fileupload上传组件的jar包可以去apache官网上面下载，也可以在struts的lib文件夹下面找到，struts上传的功能就是基于这个实现的。common-fileupload是依赖于common-io这个包的，所以还需要下载这个包。

## 一、开发环境搭建

　　创建一个FileUploadAndDownLoad项目，加入Apache的commons-fileupload文件上传组件的相关Jar包，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/032130161842199.png)

## 二、实现文件上传

### 2.1、文件上传页面和消息提示页面

　　upload.jsp页面的代码如下：

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>文件上传</title>
 6   </head>
 7   
 8   <body>
 9     <form action="${pageContext.request.contextPath}/servlet/UploadHandleServlet" enctype="multipart/form-data" method="post">
10         上传用户：<input type="text" name="username"><br/>
11         上传文件1：<input type="file" name="file1"><br/>
12         上传文件2：<input type="file" name="file2"><br/>
13         <input type="submit" value="提交">
14     </form>
15   </body>
16 </html>
```

　　message.jsp的代码如下：

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>消息提示</title>
 6   </head>
 7   
 8   <body>
 9         ${message}
10   </body>
11 </html>
```

### 2.2、处理文件上传的Servlet

　　UploadHandleServlet的代码如下：

```
1 package me.gacl.web.controller;
  2 
  3 import java.io.File;
  4 import java.io.FileOutputStream;
  5 import java.io.IOException;
  6 import java.io.InputStream;
  7 import java.util.List;
  8 import javax.servlet.ServletException;
  9 import javax.servlet.http.HttpServlet;
 10 import javax.servlet.http.HttpServletRequest;
 11 import javax.servlet.http.HttpServletResponse;
 12 import org.apache.commons.fileupload.FileItem;
 13 import org.apache.commons.fileupload.disk.DiskFileItemFactory;
 14 import org.apache.commons.fileupload.servlet.ServletFileUpload;
 15 
 16 public class UploadHandleServlet extends HttpServlet {
 17 
 18     public void doGet(HttpServletRequest request, HttpServletResponse response)
 19             throws ServletException, IOException {
 20                 //得到上传文件的保存目录，将上传的文件存放于WEB-INF目录下，不允许外界直接访问，保证上传文件的安全
 21                 String savePath = this.getServletContext().getRealPath("/WEB-INF/upload");
 22                 File file = new File(savePath);
 23                 //判断上传文件的保存目录是否存在
 24                 if (!file.exists() && !file.isDirectory()) {
 25                     System.out.println(savePath+"目录不存在，需要创建");
 26                     //创建目录
 27                     file.mkdir();
 28                 }
 29                 //消息提示
 30                 String message = "";
 31                 try{
 32                     //使用Apache文件上传组件处理文件上传步骤：
 33                     //1、创建一个DiskFileItemFactory工厂
 34                     DiskFileItemFactory factory = new DiskFileItemFactory();
 35                     //2、创建一个文件上传解析器
 36                     ServletFileUpload upload = new ServletFileUpload(factory);
 37                      //解决上传文件名的中文乱码
 38                     upload.setHeaderEncoding("UTF-8"); 
 39                     //3、判断提交上来的数据是否是上传表单的数据
 40                     if(!ServletFileUpload.isMultipartContent(request)){
 41                         //按照传统方式获取数据
 42                         return;
 43                     }
 44                     //4、使用ServletFileUpload解析器解析上传数据，解析结果返回的是一个List<FileItem>集合，每一个FileItem对应一个Form表单的输入项
 45                     List<FileItem> list = upload.parseRequest(request);
 46                     for(FileItem item : list){
 47                         //如果fileitem中封装的是普通输入项的数据
 48                         if(item.isFormField()){
 49                             String name = item.getFieldName();
 50                             //解决普通输入项的数据的中文乱码问题
 51                             String value = item.getString("UTF-8");
 52                             //value = new String(value.getBytes("iso8859-1"),"UTF-8");
 53                             System.out.println(name + "=" + value);
 54                         }else{//如果fileitem中封装的是上传文件
 55                             //得到上传的文件名称，
 56                             String filename = item.getName();
 57                             System.out.println(filename);
 58                             if(filename==null || filename.trim().equals("")){
 59                                 continue;
 60                             }
 61                             //注意：不同的浏览器提交的文件名是不一样的，有些浏览器提交上来的文件名是带有路径的，如：  c:\a\b\1.txt，而有些只是单纯的文件名，如：1.txt
 62                             //处理获取到的上传文件的文件名的路径部分，只保留文件名部分
 63                             filename = filename.substring(filename.lastIndexOf("\\")+1);
 64                             //获取item中的上传文件的输入流
 65                             InputStream in = item.getInputStream();
 66                             //创建一个文件输出流
 67                             FileOutputStream out = new FileOutputStream(savePath + "\\" + filename);
 68                             //创建一个缓冲区
 69                             byte buffer[] = new byte[1024];
 70                             //判断输入流中的数据是否已经读完的标识
 71                             int len = 0;
 72                             //循环将输入流读入到缓冲区当中，(len=in.read(buffer))>0就表示in里面还有数据
 73                             while((len=in.read(buffer))>0){
 74                                 //使用FileOutputStream输出流将缓冲区的数据写入到指定的目录(savePath + "\\" + filename)当中
 75                                 out.write(buffer, 0, len);
 76                             }
 77                             //关闭输入流
 78                             in.close();
 79                             //关闭输出流
 80                             out.close();
 81                             //删除处理文件上传时生成的临时文件
 82                             item.delete();
 83                             message = "文件上传成功！";
 84                         }
 85                     }
 86                 }catch (Exception e) {
 87                     message= "文件上传失败！";
 88                     e.printStackTrace();
 89                     
 90                 }
 91                 request.setAttribute("message",message);
 92                 request.getRequestDispatcher("/message.jsp").forward(request, response);
 93     }
 94 
 95     public void doPost(HttpServletRequest request, HttpServletResponse response)
 96             throws ServletException, IOException {
 97 
 98         doGet(request, response);
 99     }
100 }
```

　　在Web.xml文件中注册UploadHandleServlet

```
1 <servlet>
2     <servlet-name>UploadHandleServlet</servlet-name>
3     <servlet-class>me.gacl.web.controller.UploadHandleServlet</servlet-class>
4 </servlet>
5 
6 <servlet-mapping>
7     <servlet-name>UploadHandleServlet</servlet-name>
8     <url-pattern>/servlet/UploadHandleServlet</url-pattern>
9 </servlet-mapping>
```

　　运行效果如下：

![](https://images0.cnblogs.com/blog/289233/201501/032259074816790.gif)

　　文件上传成功之后，上传的文件保存在了WEB-INF目录下的upload目录，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/032301344346495.png)

### 2.3、文件上传的细节

　　上述的代码虽然可以成功将文件上传到服务器上面的指定目录当中，但是文件上传功能有许多需要注意的小细节问题，以下列出的几点需要特别注意的

**1、为保证服务器安全，上传文件应该放在外界无法直接访问的目录下，比如放于WEB-INF目录下。**

**　　2、为防止文件覆盖的现象发生，要为上传文件产生一个唯一的文件名。**

**　　3、为防止一个目录下面出现太多文件，要使用hash算法打散存储。**

**　　4、要限制上传文件的最大值。**

**　　5、要**限制**上传文件的类型，在收到上传文件名时，判断后缀名是否合法。**

　　针对上述提出的5点细节问题，我们来改进一下UploadHandleServlet，改进后的代码如下：

```
1 package me.gacl.web.controller;
  2 
  3 import java.io.File;
  4 import java.io.FileOutputStream;
  5 import java.io.IOException;
  6 import java.io.InputStream;
  7 import java.util.List;
  8 import java.util.UUID;
  9 
 10 import javax.servlet.ServletException;
 11 import javax.servlet.http.HttpServlet;
 12 import javax.servlet.http.HttpServletRequest;
 13 import javax.servlet.http.HttpServletResponse;
 14 import org.apache.commons.fileupload.FileItem;
 15 import org.apache.commons.fileupload.FileUploadBase;
 16 import org.apache.commons.fileupload.ProgressListener;
 17 import org.apache.commons.fileupload.disk.DiskFileItemFactory;
 18 import org.apache.commons.fileupload.servlet.ServletFileUpload;
 19 
 20 /**
 21 * @ClassName: UploadHandleServlet
 22 * @Description: TODO(这里用一句话描述这个类的作用)
 23 * @author: 孤傲苍狼
 24 * @date: 2015-1-3 下午11:35:50
 25 *
 26 */ 
 27 public class UploadHandleServlet extends HttpServlet {
 28 
 29     public void doGet(HttpServletRequest request, HttpServletResponse response)
 30             throws ServletException, IOException {
 31                 //得到上传文件的保存目录，将上传的文件存放于WEB-INF目录下，不允许外界直接访问，保证上传文件的安全
 32                 String savePath = this.getServletContext().getRealPath("/WEB-INF/upload");
 33                 //上传时生成的临时文件保存目录
 34                 String tempPath = this.getServletContext().getRealPath("/WEB-INF/temp");
 35                 File tmpFile = new File(tempPath);
 36                 if (!tmpFile.exists()) {
 37                     //创建临时目录
 38                     tmpFile.mkdir();
 39                 }
 40                 
 41                 //消息提示
 42                 String message = "";
 43                 try{
 44                     //使用Apache文件上传组件处理文件上传步骤：
 45                     //1、创建一个DiskFileItemFactory工厂
 46                     DiskFileItemFactory factory = new DiskFileItemFactory();
 47                     //设置工厂的缓冲区的大小，当上传的文件大小超过缓冲区的大小时，就会生成一个临时文件存放到指定的临时目录当中。
 48                     factory.setSizeThreshold(1024*100);//设置缓冲区的大小为100KB，如果不指定，那么缓冲区的大小默认是10KB
 49                     //设置上传时生成的临时文件的保存目录
 50                     factory.setRepository(tmpFile);
 51                     //2、创建一个文件上传解析器
 52                     ServletFileUpload upload = new ServletFileUpload(factory);
 53                     //监听文件上传进度
 54                     upload.setProgressListener(new ProgressListener(){
 55                         public void update(long pBytesRead, long pContentLength, int arg2) {
 56                             System.out.println("文件大小为：" + pContentLength + ",当前已处理：" + pBytesRead);
 57                             /**
 58                              * 文件大小为：14608,当前已处理：4096
 59                                 文件大小为：14608,当前已处理：7367
 60                                 文件大小为：14608,当前已处理：11419
 61                                 文件大小为：14608,当前已处理：14608
 62                              */
 63                         }
 64                     });
 65                      //解决上传文件名的中文乱码
 66                     upload.setHeaderEncoding("UTF-8"); 
 67                     //3、判断提交上来的数据是否是上传表单的数据
 68                     if(!ServletFileUpload.isMultipartContent(request)){
 69                         //按照传统方式获取数据
 70                         return;
 71                     }
 72                     
 73                     //设置上传单个文件的大小的最大值，目前是设置为1024*1024字节，也就是1MB
 74                     upload.setFileSizeMax(1024*1024);
 75                     //设置上传文件总量的最大值，最大值=同时上传的多个文件的大小的最大值的和，目前设置为10MB
 76                     upload.setSizeMax(1024*1024*10);
 77                     //4、使用ServletFileUpload解析器解析上传数据，解析结果返回的是一个List<FileItem>集合，每一个FileItem对应一个Form表单的输入项
 78                     List<FileItem> list = upload.parseRequest(request);
 79                     for(FileItem item : list){
 80                         //如果fileitem中封装的是普通输入项的数据
 81                         if(item.isFormField()){
 82                             String name = item.getFieldName();
 83                             //解决普通输入项的数据的中文乱码问题
 84                             String value = item.getString("UTF-8");
 85                             //value = new String(value.getBytes("iso8859-1"),"UTF-8");
 86                             System.out.println(name + "=" + value);
 87                         }else{//如果fileitem中封装的是上传文件
 88                             //得到上传的文件名称，
 89                             String filename = item.getName();
 90                             System.out.println(filename);
 91                             if(filename==null || filename.trim().equals("")){
 92                                 continue;
 93                             }
 94                             //注意：不同的浏览器提交的文件名是不一样的，有些浏览器提交上来的文件名是带有路径的，如：  c:\a\b\1.txt，而有些只是单纯的文件名，如：1.txt
 95                             //处理获取到的上传文件的文件名的路径部分，只保留文件名部分
 96                             filename = filename.substring(filename.lastIndexOf("\\")+1);
 97                             //得到上传文件的扩展名
 98                             String fileExtName = filename.substring(filename.lastIndexOf(".")+1);
 99                             //如果需要限制上传的文件类型，那么可以通过文件的扩展名来判断上传的文件类型是否合法
100                             System.out.println("上传的文件的扩展名是："+fileExtName);
101                             //获取item中的上传文件的输入流
102                             InputStream in = item.getInputStream();
103                             //得到文件保存的名称
104                             String saveFilename = makeFileName(filename);
105                             //得到文件的保存目录
106                             String realSavePath = makePath(saveFilename, savePath);
107                             //创建一个文件输出流
108                             FileOutputStream out = new FileOutputStream(realSavePath + "\\" + saveFilename);
109                             //创建一个缓冲区
110                             byte buffer[] = new byte[1024];
111                             //判断输入流中的数据是否已经读完的标识
112                             int len = 0;
113                             //循环将输入流读入到缓冲区当中，(len=in.read(buffer))>0就表示in里面还有数据
114                             while((len=in.read(buffer))>0){
115                                 //使用FileOutputStream输出流将缓冲区的数据写入到指定的目录(savePath + "\\" + filename)当中
116                                 out.write(buffer, 0, len);
117                             }
118                             //关闭输入流
119                             in.close();
120                             //关闭输出流
121                             out.close();
122                             //删除处理文件上传时生成的临时文件
123                             //item.delete();
124                             message = "文件上传成功！";
125                         }
126                     }
127                 }catch (FileUploadBase.FileSizeLimitExceededException e) {
128                     e.printStackTrace();
129                     request.setAttribute("message", "单个文件超出最大值！！！");
130                     request.getRequestDispatcher("/message.jsp").forward(request, response);
131                     return;
132                 }catch (FileUploadBase.SizeLimitExceededException e) {
133                     e.printStackTrace();
134                     request.setAttribute("message", "上传文件的总的大小超出限制的最大值！！！");
135                     request.getRequestDispatcher("/message.jsp").forward(request, response);
136                     return;
137                 }catch (Exception e) {
138                     message= "文件上传失败！";
139                     e.printStackTrace();
140                 }
141                 request.setAttribute("message",message);
142                 request.getRequestDispatcher("/message.jsp").forward(request, response);
143     }
144     
145     /**
146     * @Method: makeFileName
147     * @Description: 生成上传文件的文件名，文件名以：uuid+"_"+文件的原始名称
148     * @Anthor:孤傲苍狼
149     * @param filename 文件的原始名称
150     * @return uuid+"_"+文件的原始名称
151     */ 
152     private String makeFileName(String filename){  //2.jpg
153         //为防止文件覆盖的现象发生，要为上传文件产生一个唯一的文件名
154         return UUID.randomUUID().toString() + "_" + filename;
155     }
156     
157     /**
158      * 为防止一个目录下面出现太多文件，要使用hash算法打散存储
159     * @Method: makePath
160     * @Description: 
161     * @Anthor:孤傲苍狼
162     *
163     * @param filename 文件名，要根据文件名生成存储目录
164     * @param savePath 文件存储路径
165     * @return 新的存储目录
166     */ 
167     private String makePath(String filename,String savePath){
168         //得到文件名的hashCode的值，得到的就是filename这个字符串对象在内存中的地址
169         int hashcode = filename.hashCode();
170         int dir1 = hashcode&0xf;  //0--15
171         int dir2 = (hashcode&0xf0)>>4;  //0-15
172         //构造新的保存目录
173         String dir = savePath + "\\" + dir1 + "\\" + dir2;  //upload\2\3  upload\3\5
174         //File既可以代表文件也可以代表目录
175         File file = new File(dir);
176         //如果目录不存在
177         if(!file.exists()){
178             //创建目录
179             file.mkdirs();
180         }
181         return dir;
182     }
183 
184     public void doPost(HttpServletRequest request, HttpServletResponse response)
185             throws ServletException, IOException {
186 
187         doGet(request, response);
188     }
189 }
```

　　针对上述提出的5点小细节问题进行改进之后，我们的文件上传功能就算是做得比较完善了。

## 三、文件下载

### 3.1、列出提供下载的文件资源

　　我们要将Web应用系统中的文件资源提供给用户进行下载，首先我们要有一个页面列出上传文件目录下的所有文件，当用户点击文件下载超链接时就进行下载操作，编写一个ListFileServlet，用于列出Web应用系统中所有下载文件。

　　ListFileServlet的代码如下：

```
1 package me.gacl.web.controller;
 2 
 3 import java.io.File;
 4 import java.io.IOException;
 5 import java.util.HashMap;
 6 import java.util.Map;
 7 import javax.servlet.ServletException;
 8 import javax.servlet.http.HttpServlet;
 9 import javax.servlet.http.HttpServletRequest;
10 import javax.servlet.http.HttpServletResponse;
11 
12 /**
13 * @ClassName: ListFileServlet
14 * @Description: 列出Web系统中所有下载文件
15 * @author: 孤傲苍狼
16 * @date: 2015-1-4 下午9:54:40
17 *
18 */ 
19 public class ListFileServlet extends HttpServlet {
20 
21     public void doGet(HttpServletRequest request, HttpServletResponse response)
22             throws ServletException, IOException {
23         //获取上传文件的目录
24         String uploadFilePath = this.getServletContext().getRealPath("/WEB-INF/upload");
25         //存储要下载的文件名
26         Map<String,String> fileNameMap = new HashMap<String,String>();
27         //递归遍历filepath目录下的所有文件和目录，将文件的文件名存储到map集合中
28         listfile(new File(uploadFilePath),fileNameMap);//File既可以代表一个文件也可以代表一个目录
29         //将Map集合发送到listfile.jsp页面进行显示
30         request.setAttribute("fileNameMap", fileNameMap);
31         request.getRequestDispatcher("/listfile.jsp").forward(request, response);
32     }
33     
34     /**
35     * @Method: listfile
36     * @Description: 递归遍历指定目录下的所有文件
37     * @Anthor:孤傲苍狼
38     * @param file 即代表一个文件，也代表一个文件目录
39     * @param map 存储文件名的Map集合
40     */ 
41     public void listfile(File file,Map<String,String> map){
42         //如果file代表的不是一个文件，而是一个目录
43         if(!file.isFile()){
44             //列出该目录下的所有文件和目录
45             File files[] = file.listFiles();
46             //遍历files[]数组
47             for(File f : files){
48                 //递归
49                 listfile(f,map);
50             }
51         }else{
52             /**
53              * 处理文件名，上传后的文件是以uuid_文件名的形式去重新命名的，去除文件名的uuid_部分
54                 file.getName().indexOf("_")检索字符串中第一次出现"_"字符的位置，如果文件名类似于：9349249849-88343-8344_阿_凡_达.avi
55                 那么file.getName().substring(file.getName().indexOf("_")+1)处理之后就可以得到阿_凡_达.avi部分
56              */
57             String realName = file.getName().substring(file.getName().indexOf("_")+1);
58             //file.getName()得到的是文件的原始名称，这个名称是唯一的，因此可以作为key，realName是处理过后的名称，有可能会重复
59             map.put(file.getName(), realName);
60         }
61     }
62     
63     public void doPost(HttpServletRequest request, HttpServletResponse response)
64             throws ServletException, IOException {
65         doGet(request, response);
66     }
67 }
```

　　这里简单说一下ListFileServlet中listfile方法，listfile方法是用来列出目录下的所有文件的，listfile方法内部用到了递归，在实际开发当中，我们肯定会在数据库创建一张表，里面会存储上传的文件名以及文件的具体存放目录，我们通过查询表就可以知道文件的具体存放目录，是不需要用到递归操作的，这个例子是因为没有使用数据库存储上传的文件名和文件的具体存放位置，而上传文件的存放位置又使用了散列算法打散存放，所以需要用到递归，在递归时，将获取到的文件名存放到从外面传递到listfile方法里面的Map集合当中，这样就可以保证所有的文件都存放在同一个Map集合当中。

　　在Web.xml文件中配置ListFileServlet

```
1 <servlet>
2      <servlet-name>ListFileServlet</servlet-name>
3      <servlet-class>me.gacl.web.controller.ListFileServlet</servlet-class>
4 </servlet>
5  
6 <servlet-mapping>
7      <servlet-name>ListFileServlet</servlet-name>
8     <url-pattern>/servlet/ListFileServlet</url-pattern>
9 </servlet-mapping>
```

　　展示下载文件的listfile.jsp页面如下：

```
1 <%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
 2 <%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
 3 <!DOCTYPE HTML>
 4 <html>
 5   <head>
 6     <title>下载文件显示页面</title>
 7   </head>
 8   
 9   <body>
10       <!-- 遍历Map集合 -->
11     <c:forEach var="me" items="${fileNameMap}">
12         <c:url value="/servlet/DownLoadServlet" var="downurl">
13             <c:param name="filename" value="${me.key}"></c:param>
14         </c:url>
15         ${me.value}<a href="${downurl}">下载</a>
16         <br/>
17     </c:forEach>
18   </body>
19 </html>
```

　　访问ListFileServlet，就可以在listfile.jsp页面中显示提供给用户下载的文件资源，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/042244025759891.png)

### 3.2、实现文件下载

　　编写一个用于处理文件下载的Servlet，DownLoadServlet的代码如下：

```
1 package me.gacl.web.controller;
 2 
 3 import java.io.File;
 4 import java.io.FileInputStream;
 5 import java.io.IOException;
 6 import java.io.OutputStream;
 7 import java.net.URLEncoder;
 8 import javax.servlet.ServletException;
 9 import javax.servlet.http.HttpServlet;
10 import javax.servlet.http.HttpServletRequest;
11 import javax.servlet.http.HttpServletResponse;
12 
13 public class DownLoadServlet extends HttpServlet {
14 
15     
16     public void doGet(HttpServletRequest request, HttpServletResponse response)
17             throws ServletException, IOException {
18         //得到要下载的文件名
19         String fileName = request.getParameter("filename");  //23239283-92489-阿凡达.avi
20         fileName = new String(fileName.getBytes("iso8859-1"),"UTF-8");
21         //上传的文件都是保存在/WEB-INF/upload目录下的子目录当中
22         String fileSaveRootPath=this.getServletContext().getRealPath("/WEB-INF/upload");
23         //通过文件名找出文件的所在目录
24         String path = findFileSavePathByFileName(fileName,fileSaveRootPath);
25         //得到要下载的文件
26         File file = new File(path + "\\" + fileName);
27         //如果文件不存在
28         if(!file.exists()){
29             request.setAttribute("message", "您要下载的资源已被删除！！");
30             request.getRequestDispatcher("/message.jsp").forward(request, response);
31             return;
32         }
33         //处理文件名
34         String realname = fileName.substring(fileName.indexOf("_")+1);
35         //设置响应头，控制浏览器下载该文件
36         response.setHeader("content-disposition", "attachment;filename=" + URLEncoder.encode(realname, "UTF-8"));
37         //读取要下载的文件，保存到文件输入流
38         FileInputStream in = new FileInputStream(path + "\\" + fileName);
39         //创建输出流
40         OutputStream out = response.getOutputStream();
41         //创建缓冲区
42         byte buffer[] = new byte[1024];
43         int len = 0;
44         //循环将输入流中的内容读取到缓冲区当中
45         while((len=in.read(buffer))>0){
46             //输出缓冲区的内容到浏览器，实现文件下载
47             out.write(buffer, 0, len);
48         }
49         //关闭文件输入流
50         in.close();
51         //关闭输出流
52         out.close();
53     }
54     
55     /**
56     * @Method: findFileSavePathByFileName
57     * @Description: 通过文件名和存储上传文件根目录找出要下载的文件的所在路径
58     * @Anthor:孤傲苍狼
59     * @param filename 要下载的文件名
60     * @param saveRootPath 上传文件保存的根目录，也就是/WEB-INF/upload目录
61     * @return 要下载的文件的存储目录
62     */ 
63     public String findFileSavePathByFileName(String filename,String saveRootPath){
64         int hashcode = filename.hashCode();
65         int dir1 = hashcode&0xf;  //0--15
66         int dir2 = (hashcode&0xf0)>>4;  //0-15
67         String dir = saveRootPath + "\\" + dir1 + "\\" + dir2;  //upload\2\3  upload\3\5
68         File file = new File(dir);
69         if(!file.exists()){
70             //创建目录
71             file.mkdirs();
72         }
73         return dir;
74     }
75     
76     public void doPost(HttpServletRequest request, HttpServletResponse response)
77             throws ServletException, IOException {
78         doGet(request, response);
79     }
80 }
```

　　在Web.xml文件中配置DownLoadServlet

```
1 <servlet>
2       <servlet-name>DownLoadServlet</servlet-name>
3       <servlet-class>me.gacl.web.controller.DownLoadServlet</servlet-class>
4 </servlet>
5  
6 <servlet-mapping>
7       <servlet-name>DownLoadServlet</servlet-name>
8       <url-pattern>/servlet/DownLoadServlet</url-pattern>
9 </servlet-mapping>
```

　　点击【下载】超链接，将请求提交到DownLoadServlet就行处理就可以实现文件下载了，运行效果如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/042303382789974.gif)

　　从运行结果可以看到，我们的文件下载功能已经可以正常下载文件了。

　　关于JavaWeb中的文件上传和下载功能的内容就这么多。









