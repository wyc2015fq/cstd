# SpringMVC 文件上传配置，多文件上传，使用的MultipartFile - z69183787的专栏 - CSDN博客
2014年05月27日 06:42:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：13765
[http://blog.csdn.net/swingpyzf/article/details/20230865](http://blog.csdn.net/swingpyzf/article/details/20230865)
基本的SpringMVC的搭建在我的上一篇文章里已经写过了，这篇文章主要说明一下如何使用SpringMVC进行表单上的文件上传以及多个文件同时上传的步骤
SpringMVC 基础教程 框架分析：[http://blog.csdn.net/swingpyzf/article/details/8885459](http://blog.csdn.net/swingpyzf/article/details/8885459)
**SpringMVC 基础教程 简单入门实例：[http://blog.csdn.net/swingpyzf/article/details/8904205](http://blog.csdn.net/swingpyzf/article/details/8904205)**
**文件上传项目的源码下载地址：[http://download.csdn.net/detail/swingpyzf/6979915](http://download.csdn.net/detail/swingpyzf/6979915)**
一、配置文件：
SpringMVC 用的是 的MultipartFile来进行文件上传 所以我们首先要配置MultipartResolver:用于处理表单中的file
**[html]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/20230865#)[copy](http://blog.csdn.net/swingpyzf/article/details/20230865#)[print](http://blog.csdn.net/swingpyzf/article/details/20230865#)[?](http://blog.csdn.net/swingpyzf/article/details/20230865#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/212406/fork)
- <!-- 配置MultipartResolver 用于文件上传 使用spring的CommosMultipartResolver -->
- <beans:beanid="multipartResolver"class="org.springframework.web.multipart.commons.CommonsMultipartResolver"
- p:defaultEncoding="UTF-8"
- p:maxUploadSize="5400000"
- p:uploadTempDir="fileUpload/temp"
- >
- </beans:bean>
其中属性详解：
defaultEncoding="UTF-8" 是请求的编码格式，默认为iso-8859-1
maxUploadSize="5400000" 是上传文件的大小，单位为字节
uploadTempDir="fileUpload/temp" 为上传文件的临时路径
二、创建一个简单的上传表单：
**[html]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/20230865#)[copy](http://blog.csdn.net/swingpyzf/article/details/20230865#)[print](http://blog.csdn.net/swingpyzf/article/details/20230865#)[?](http://blog.csdn.net/swingpyzf/article/details/20230865#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/212406/fork)
- <body>
- <h2>文件上传实例</h2>
- 
- 
- <formaction="fileUpload.html"method="post"enctype="multipart/form-data">
-     选择文件:<inputtype="file"name="file">
- <inputtype="submit"value="提交">
- </form>
- 
- 
- </body>
注意要在form标签中加上enctype="multipart/form-data"表示该表单是要处理文件的,这是最基本的东西，很多人会忘记然而当上传出错后则去找程序的错误，却忘了这一点
三、编写上传控制类
1、创建一个控制类: FileUploadController和一个返回结果的页面list.jsp
2、编写提交表单的action:
**[java]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/20230865#)[copy](http://blog.csdn.net/swingpyzf/article/details/20230865#)[print](http://blog.csdn.net/swingpyzf/article/details/20230865#)[?](http://blog.csdn.net/swingpyzf/article/details/20230865#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/212406/fork)
- //通过Spring的autowired注解获取spring默认配置的request
- @Autowired
- private HttpServletRequest request;  
- 
- /***
-      * 上传文件 用@RequestParam注解来指定表单上的file为MultipartFile
-      * 
-      * @param file
-      * @return
-      */
- @RequestMapping("fileUpload")  
- public String fileUpload(@RequestParam("file") MultipartFile file) {  
- // 判断文件是否为空
- if (!file.isEmpty()) {  
- try {  
- // 文件保存路径
-                 String filePath = request.getSession().getServletContext().getRealPath("/") + "upload/"
-                         + file.getOriginalFilename();  
- // 转存文件
-                 file.transferTo(new File(filePath));  
-             } catch (Exception e) {  
-                 e.printStackTrace();  
-             }  
-         }  
- // 重定向
- return"redirect:/list.html";  
-     }  
- 
- /***
-      * 读取上传文件中得所有文件并返回
-      * 
-      * @return
-      */
- @RequestMapping("list")  
- public ModelAndView list() {  
-         String filePath = request.getSession().getServletContext().getRealPath("/") + "upload/";  
-         ModelAndView mav = new ModelAndView("list");  
-         File uploadDest = new File(filePath);  
-         String[] fileNames = uploadDest.list();  
- for (int i = 0; i < fileNames.length; i++) {  
- //打印出文件名
-             System.out.println(fileNames[i]);  
-         }  
- return mav;  
-     }  

3、使用SpringMVC注解RequestParam来指定表单中的file参数；
4、指定一个用于保存文件的web项目路径
5、通过MultipartFile的transferTo(File dest)这个方法来转存文件到指定的路径。
到此基本的文件上传就结束了。
MultipartFile类常用的一些方法：
String getContentType()//获取文件MIME类型
InputStream getInputStream()//后去文件流
String getName() //获取表单中文件组件的名字
String getOriginalFilename() //获取上传文件的原名
long getSize() //获取文件的字节大小，单位byte
boolean isEmpty() //是否为空
void transferTo(File dest)//保存到一个目标文件中。
四、多文件上传。
多文件上传其实很简单，和上传其他相同的参数如checkbox一样，表单中使用相同的名称，然后action中将MultipartFile参数类定义为数组就可以。
接下来实现：
1、创建一个上传多文件的表单：
**[html]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/20230865#)[copy](http://blog.csdn.net/swingpyzf/article/details/20230865#)[print](http://blog.csdn.net/swingpyzf/article/details/20230865#)[?](http://blog.csdn.net/swingpyzf/article/details/20230865#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/212406/fork)
- <body>
- <h2>上传多个文件 实例</h2>
- 
- 
- <formaction="filesUpload.html"method="post"
- enctype="multipart/form-data">
- <p>
-             选择文件:<inputtype="file"name="files">
- <p>
-             选择文件:<inputtype="file"name="files">
- <p>
-             选择文件:<inputtype="file"name="files">
- <p>
- <inputtype="submit"value="提交">
- </form>
- </body>
2、编写处理表单的action，将原来保存文件的方法单独写一个方法出来方便共用：
**[java]**[view
 plain](http://blog.csdn.net/swingpyzf/article/details/20230865#)[copy](http://blog.csdn.net/swingpyzf/article/details/20230865#)[print](http://blog.csdn.net/swingpyzf/article/details/20230865#)[?](http://blog.csdn.net/swingpyzf/article/details/20230865#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/212406/fork)
- /***
-      * 保存文件
-      * @param file
-      * @return
-      */
- privateboolean saveFile(MultipartFile file) {  
- // 判断文件是否为空
- if (!file.isEmpty()) {  
- try {  
- // 文件保存路径
-                 String filePath = request.getSession().getServletContext().getRealPath("/") + "upload/"
-                         + file.getOriginalFilename();  
- // 转存文件
-                 file.transferTo(new File(filePath));  
- returntrue;  
-             } catch (Exception e) {  
-                 e.printStackTrace();  
-             }  
-         }  
- returnfalse;  
-     }  
- 3、编写action:  
- @RequestMapping("filesUpload")  
- public String filesUpload(@RequestParam("files") MultipartFile[] files) {  
- //判断file数组不能为空并且长度大于0
- if(files!=null&&files.length>0){  
- //循环获取file数组中得文件
- for(int i = 0;i<files.length;i++){  
-                 MultipartFile file = files[i];  
- //保存文件
-                 saveFile(file);  
-             }  
-         }  
- // 重定向
- return"redirect:/list.html";  
-     }  
最后运行项目上传文件:
![](https://img-blog.csdn.net/20140301201521718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU3dpbmdQeXpm/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140301201533671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU3dpbmdQeXpm/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
