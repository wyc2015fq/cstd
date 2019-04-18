# jQuery插件AjaxFileUpload 例子 - z69183787的专栏 - CSDN博客
2013年11月20日 16:16:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3069
AjaxFileUpload.js下载地址: [http://www.phpletter.com/Our-Projects/AjaxFileUpload/](http://www.phpletter.com/Our-Projects/AjaxFileUpload/)
Jquery.js下载地址: [http://jquery.com/](http://jquery.com/)
页面代码： 
Java代码  ![收藏代码](http://walistudio-163-com.iteye.com/images/icon_star.png)
- <html>  
-     <!-- 引入相关的js文件，相对路径  -->  
-     <script type="text/javascript" src="js/jquery.js"></script>  
-       <script type="text/javascript" src="js/ajaxfileupload.js"></script>  
- 
-     <!-- 执行上传文件操作的函数 -->  
-       <script type="text/javascript">  
-           function ajaxFileUpload(){  
-                $.ajaxFileUpload(  
-                    {  
-                 url:'update.do?method=uploader',            //需要链接到服务器地址
-                 secureuri:false,  
-                 fileElementId:'houseMaps',                        //文件选择框的id属性
-                 dataType: 'xml',                                     //服务器返回的格式，可以是json
-                 success: function (data, status)            //相当于java中try语句块的用法
-                 {       
-                     $('#result').html('添加成功');  
-                 },  
-                 error: function (data, status, e)            //相当于java中catch语句块的用法
-                 {  
-                     $('#result').html('添加失败');  
-                 }  
-             }  
- 
-                );  
- 
-           }  
-       </script>  
-   </head>  
- 
-   <body>  
-       <form method="post" action="update.do?method=uploader" enctype="multipart/form-data">   
-         <input type="file" id="houseMaps" name="houseMaps"/>  
-         <input type="button" value="提交" onclick="ajaxFileUpload()"/>  
-     </form>  
-     <div id="result"></div>  
- 
-   </body>  
- </html>  
服务器代码： 
Java代码  ![收藏代码](http://walistudio-163-com.iteye.com/images/icon_star.png)
- publicclass UpdateAction extends DispatchAction {  
- 
- public ActionForward uploader(ActionMapping mapping, ActionForm form,  
-             HttpServletRequest request, HttpServletResponse response) {  
-         UpFormForm upFormForm = (UpFormForm) form;  
-         FormFile ff = upFormForm.getHouseMaps();  
- try {  
-             InputStream is = ff.getInputStream();  
-             File file = new File("D:/" + ff.getFileName());            //指定文件存储的路径和文件名
-             OutputStream os = new FileOutputStream(file);  
- 
- byte[] b = newbyte[1024];  
- int len = 0;  
- while((len = is.read(b)) != -1){  
-                 os.write(b, 0, len);  
-             }  
-             os.close();  
-             is.close();  
-         } catch (Exception e) {  
-             e.printStackTrace();  
- 
-         }  
- 
- returnnull;  
-     }  
- }  
主要参数说明： 
1，url表示处理文件上传操作的文件路径，可以测试URL是否能在浏览器中直接访问，如上：upload.php 
2，fileElementId表示文件域ID，如上：fileToUpload 
3，secureuri是否启用安全提交，默认为false 
4，dataType数据数据，一般选json，javascript的原生态 
5，success提交成功后处理函数 
6，error提交失败处理函数 
上面有两个方法，一个动态加载小图标提示函数loading()和ajaxFileUpload文件上传$.ajaxFileUpload()函数，这与我们使用jQuery.ajax()函数差不多，使用很简单，这里我省略了PHP处理上传文件，使用 jQuery插件AjaxFileUpload实现ajax文件就这么简单。 
同时我们需要了解相关的错误提示 
1，SyntaxError: missing ; before statement错误 
如果出现这个错误就需要检查url路径是否可以访问 
2，SyntaxError: syntax error错误 
如果出现这个错误就需要检查处理提交操作的PHP文件是否存在语法错误 
3，SyntaxError: invalid property id错误 
如果出现这个错误就需要检查属性ID是否存在 
4，SyntaxError: missing } in XML expression错误 
如果出现这个错误就需要检查文件域名称是否一致或不存在 
5，其它自定义错误 
大家可使用变量$error直接打印的方法检查各参数是否正确，比起上面这些无效的错误提示还是方便很多。 
使用jQuery插件AjaxFileUpload实现无刷新上传文件非常实用，由于其简单易用，因些这个插件相比其它文件上传插件使用人数最多，非常值得推荐。 
引用： 
http://blog.csdn.net/hadesgin/article/details/7593124 
http://www.cnblogs.com/pcajax/archive/2009/12/16/1663329.html
