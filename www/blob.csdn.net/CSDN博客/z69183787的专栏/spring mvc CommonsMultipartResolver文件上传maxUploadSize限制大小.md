# spring mvc CommonsMultipartResolver文件上传maxUploadSize限制大小 - z69183787的专栏 - CSDN博客
2015年03月09日 10:58:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：28770
第一步：配置sping 
Xml代码  ![收藏代码](http://913.iteye.com/images/icon_star.png)
- <beanid="multipartResolver"
- class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
- <!--1024*200即200k-->
- <propertyname="maxUploadSize"value="204800"/>
- <!--resolveLazily属性启用是为了推迟文件解析，以便在UploadAction 中捕获文件大小异常-->
- <propertyname="resolveLazily"value="true"/>
- </bean>
第二步：在上传action中自己捕获异常 
Java代码  ![收藏代码](http://913.iteye.com/images/icon_star.png)
- @RequestMapping
- publicvoid execute(  
- @RequestParam(required = false) MultipartFile file,  
- @RequestParam(value = "file_info_id", required = false) Integer fileInfoId,  
-             ModelMap model, HttpServletRequest request) throws Exception {  
- 
- if (file == null || file.isEmpty()) {  
- return;  
-         }  
- byte[] bytes = file.getBytes();  
-             ……………………  
- ………………  
- }  
- @ExceptionHandler(Exception.class)         
- public ModelAndView handleException(Exception ex,HttpServletRequest request) {       
-          Map<Object, Object> model = new HashMap<Object, Object>();  
- if (ex instanceof MaxUploadSizeExceededException){  
-                         model.put("errors", "文件应不大于 "+  
-                        getFileKB(((MaxUploadSizeExceededException)ex).getMaxUploadSize()));  
-                      } else{  
-                         model.put("errors", "不知错误: " + ex.getMessage());  
-                     }  
- returnnew ModelAndView("/common/file/upload", (Map) model);  
- 
-     }    
- 
- private String getFileKB(long byteFile){  
- if(byteFile==0)  
- return"0KB";  
- long kb=1024;  
- return""+byteFile/kb+"KB";  
-     }  
- private String getFileMB(long byteFile){  
- if(byteFile==0)  
- return"0MB";  
- long mb=1024*1024;  
- return""+byteFile/mb+"MB";  
-     }  
第三步：界面 
Html代码  ![收藏代码](http://913.iteye.com/images/icon_star.png)
- <scripttype="text/javascript">
- $(function() {  
-     $('#frmupload1').submit(function() {  
-         if ($('#file1').val() == '') {  
-             alert('请选择上传导入文件!');  
-             $('#file1').focus();  
-             return false;  
-         }else{  
-             if(!isvalidatefile($('#file1').val()))  
-                   return false;  
- 
-         }  
-     });  
-     $('#frmupload2').submit(function() {  
-         if ($('#file2').val() == '') {  
-             alert('请选择上传导入文件!');  
-             $('#file2').focus();  
-             return false;  
-         }else{  
-             if(!isvalidatefile($('#file2').val()))  
-                   return false;  
- 
-         }  
-     });  
- });  
- 
- function isvalidatefile(obj) {  
- 
-     var extend = obj.substring(obj.lastIndexOf(".") + 1);  
-     //alert(extend);  
-     if (extend == "") {  
-     } else {  
-         if (!(extend == "xls" )) {  
-             alert("请上传后缀名为xls(Excel2003)或xlsx(Excel2007)的文件!");  
- 
-             return false;  
-         }  
-     }  
-     return true;  
- }  
- <body>
- <h1>上传文件</h1>
- <formaction=""method="post"enctype="multipart/form-data"onsubmit="return checkSubmit();">
- 
- <p>请选择文件：</p>
- 
- <p ${not empty errors ?"style='color : red;'":""}>${errors}</p>
- <inputtype="file"name="file"id="file"/><inputtype="submit"value="确定"/>
- </form>
- </body>
参考来源： 
[spring MVC上传文件使用了common.fileupload组件](http://blog.csdn.net/walkerjong/article/details/7211133)
[使用Spring MVC和注释配置来实现文件上传](http://www.open-open.com/home/space-668-do-blog-id-5708.html)
