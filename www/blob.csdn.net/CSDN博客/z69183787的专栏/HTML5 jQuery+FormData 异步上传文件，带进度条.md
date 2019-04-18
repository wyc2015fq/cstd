# HTML5 jQuery+FormData 异步上传文件，带进度条 - z69183787的专栏 - CSDN博客
2018年02月24日 16:40:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：470
[http://blog.csdn.net/michaelwubo/article/details/50865246](http://blog.csdn.net/michaelwubo/article/details/50865246)
利用jQuery和html5的FormData异步上传文件的好处是：
- 实现很简单
- 很方便地支持进度条
- 很方便地进行扩展和美化
先看看效果图：
![](https://img-blog.csdn.net/20180224163614475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20180224163620410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
实现步骤如下：
第一步：配置好SpringMVC + servlet3.0 文件上传所需要的各种资源，参考：[http://blog.csdn.net/clementad/article/details/49533189](http://blog.csdn.net/clementad/article/details/49533189)
第二步：上传页面的html代码：
```
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<link href="../resources/css/common.css" rel="stylesheet" />
<script src="../resources/js/jquery-2.1.4.js"></script>
</head>
<body>
<h2>HTML5异步上传文件，带进度条</h2>
<form method="post" enctype="multipart/form-data">
其他需要提交的信息：<input type="text" name="otherInfo"/><br/><br/>
选择要上传的文件：<br/>
<input type="file" name="file" /><span></span><br/>
<input type="file" name="file" /><span></span><br/>
</form>
<br/><br/>
<input type="button" value="上传吧" onclick="upload()"/>
<br/><br/>
上传进度：<progress></progress><br/>
<p id="progress">0 bytes</p>
<p id="info"></p>
</body>
</html>
```
第三步：异步上传的JavaScript代码（注释很详细）：
```java
<script>
var totalSize = 0;
//绑定所有type=file的元素的onchange事件的处理函数
$(':file').change(function() {
var file = this.files[0]; //假设file标签没打开multiple属性，那么只取第一个文件就行了
name = file.name;
size = file.size;
type = file.type;
url = window.URL.createObjectURL(file); //获取本地文件的url，如果是图片文件，可用于预览图片
$(this).next().html("文件名：" + name + " 文件类型：" + type + " 文件大小：" + size + " url: " + url);
totalSize += size;
$("#info").html("总大小: " + totalSize + "bytes");
});
function upload() {
//创建FormData对象，初始化为form表单中的数据。需要添加其他数据可使用formData.append("property", "value");
var formData = new FormData($('form')[0]);
//ajax异步上传
$.ajax({
url: "http://localhost:8080/MyJavaStudio/servlet/file/upload",
type: "POST",
data: formData,
xhr: function(){ //获取ajaxSettings中的xhr对象，为它的upload属性绑定progress事件的处理函数
myXhr = $.ajaxSettings.xhr();
if(myXhr.upload){ //检查upload属性是否存在
//绑定progress事件的回调函数
myXhr.upload.addEventListener('progress',progressHandlingFunction, false);
}
return myXhr; //xhr对象返回给jQuery使用
},
success: function(result){
$("#result").html(result.data);
},
contentType: false, //必须false才会自动加上正确的Content-Type
processData: false //必须false才会避开jQuery对 formdata 的默认处理
});
}
//上传进度回调函数：
function progressHandlingFunction(e) {
if (e.lengthComputable) {
$('progress').attr({value : e.loaded, max : e.total}); //更新数据到进度条
var percent = e.loaded/e.total*100;
$('#progress').html(e.loaded + "/" + e.total+" bytes. " + percent.toFixed(2) + "%");
}
}
</script>
```
第四步：SpringMVC写好接受和保持文件的Controller方法：
```java
/** 
 * 文件上传 
 * @author XuJijun 
 * 
 */  
@RestController  
@RequestMapping("/servlet/file")  
public class FileUploadController {  
      
    /** 
     * 保存文件的目录，放在web目录、或一个指定的绝对目录下 
     */  
     private static final String SAVE_DIR = "uploadFiles";  
      
     /** 
      *  
      * @param request 
      * @param response 
      * @param p form表单中，type="text"的input控件，内容通过这个参数传送过来，以input控件中的name属性来区分 
      * @return JSON表示的处理结果 
      * @throws ServletException 
      * @throws IOException 
      */  
    @RequestMapping("/upload")  
    public JsonResult upload(HttpServletRequest request, HttpServletResponse response, @RequestParam Map<String, Object> p)  
            throws ServletException, IOException {  
  
        // 获取 web application的绝对路径  
        String appPath = request.getServletContext().getRealPath("");  
          
        // 构造文件存放的路径  
        String savePath = appPath + File.separator + SAVE_DIR;  
  
        // 如果文件存放路径不存在，则mkdir一个  
        File fileSaveDir = new File(savePath);  
        if (!fileSaveDir.exists()) {  
            fileSaveDir.mkdirs();  
        }  
  
        List<String> fileNames = new ArrayList<>();  
          
        //循环所有的part，把part中的文件保存到硬盘中  
        for (Part part : request.getParts()) {  
            String fileName = part.getSubmittedFileName();  
              
            //form表单中的每个input，都在一个不同的part中，  
            //所以需要判断通过fileName是否为空，过滤掉其他类型的input（比如type="text"）：  
            if(!StringUtils.isEmpty(fileName)){   
                part.write(savePath + File.separator + fileName);  
                fileNames.add(fileName);  
            }  
        }  
  
        Map<String, Object> resultData = new HashMap<>();  
        resultData.put("savePath", savePath);  
        resultData.put("files", fileNames);  
          
        return new JsonResult("200", "文件上传成功！", resultData);  
    }  
       
    /** 
     * 从content-disposition头中获取源文件名 
     *  
     * content-disposition头的格式如下： 
     * form-data; name="dataFile"; filename="PHOTO.JPG" 
     *  
     * @param part 
     * @return 
     */  
    @SuppressWarnings("unused")  
    private String extractFileName(Part part) {  
        String contentDisp = part.getHeader("content-disposition");  
        String[] items = contentDisp.split(";");  
        for (String s : items) {  
            if (s.trim().startsWith("filename")) {  
                return s.substring(s.indexOf("=") + 2, s.length()-1);  
            }  
        }  
        return "";  
    }  
  
}
```
最后那个私有方法可以不用的，只是为了演示如何直接获取request header中的数据。
最后，验证上传过程中的网络消息：
上传的消息头和数据：
![](https://img-blog.csdn.net/20180224164000486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可见，对于表单中的3个input，http request payload中对应有3个part来上传数据。
关于http协议的更多介绍，可参考：[http://blog.csdn.net/clementad/article/details/41620765](http://blog.csdn.net/clementad/article/details/41620765)
Controller处理后的返回结果（JSON格式）：
![](https://img-blog.csdn.net/20180224164011788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
