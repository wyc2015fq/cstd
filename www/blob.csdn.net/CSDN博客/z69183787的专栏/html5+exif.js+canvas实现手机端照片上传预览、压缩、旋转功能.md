# html5+exif.js+canvas实现手机端照片上传预览、压缩、旋转功能 - z69183787的专栏 - CSDN博客
2015年12月15日 15:35:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9711
html5+canvas进行移动端手机照片上传时，发现ios手机上传竖拍照片会逆时针旋转90度，横拍照片无此问题；Android手机没这个问题。
因此解决这个问题的思路是：获取到照片拍摄的方向角，对非横拍的ios照片进行角度旋转修正。
利用exif.js读取照片的拍摄信息，详见  http://code.ciaoca.com/javascript/exif-js/
这里主要`用到Orientation属性。`
``Orientation`属性说明如下：`
``
|旋转角度|参数|
|----|----|
|0°|1|
|顺时针90°|6|
|逆时针90°|8|
|180°|3|
html5页面：
**[html]**[view
 plain](http://blog.csdn.net/linlzk/article/details/48654835#)[copy](http://blog.csdn.net/linlzk/article/details/48654835#)[print](http://blog.csdn.net/linlzk/article/details/48654835#)[?](http://blog.csdn.net/linlzk/article/details/48654835#)
- <!DOCTYPE html>
- <html>
- <head>
- <metacharset="utf-8">
- <metaname="viewport"content="initial-scale=1.0, maximum-scale=1.0, user-scalable=no"/>
- <title>图片上传</title>
- <scripttype="text/javascript"src="js/jquery-1.8.3.js"></script>
- <scripttype="text/javascript"src="js/uploadPicture/mobileBUGFix.mini.js"></script>
- <scripttype="text/javascript"src="js/uploadPicture/uploadImage.js"></script>
- <scripttype="text/javascript"src="js/exif.js"></script>
- </head>
- <body>
- <divstyle="height: 50px; line-height: 50px;text-align: center;border-bottom: 1px solid #171E28;">
-             上传图片:  
- <inputtype="file"accept="image/*"id="uploadImage"capture="camera"onchange="selectFileImage(this);"/>
- </div>
- <divstyle="margin-top: 10px;">
- <imgalt="preview"src=""id="myImage"/>
- </div>
- </body>
- </html>
自己写的js：
**[javascript]**[view
 plain](http://blog.csdn.net/linlzk/article/details/48654835#)[copy](http://blog.csdn.net/linlzk/article/details/48654835#)[print](http://blog.csdn.net/linlzk/article/details/48654835#)[?](http://blog.csdn.net/linlzk/article/details/48654835#)
- function selectFileImage(fileObj) {  
- var file = fileObj.files['0'];  
- //图片方向角 added by lzk
- var Orientation = null;  
- 
- if (file) {  
-         console.log("正在上传,请稍后...");  
- var rFilter = /^(image\/jpeg|image\/png)$/i; // 检查图片格式
- if (!rFilter.test(file.type)) {  
- //showMyTips("请选择jpeg、png格式的图片", false);
- return;  
-         }  
- // var URL = URL || webkitURL;
- //获取照片方向角属性，用户旋转控制
-         EXIF.getData(file, function() {  
- // alert(EXIF.pretty(this));
-             EXIF.getAllTags(this);   
- //alert(EXIF.getTag(this, 'Orientation')); 
-             Orientation = EXIF.getTag(this, 'Orientation');  
- //return;
-         });  
- 
- var oReader = new FileReader();  
-         oReader.onload = function(e) {  
- //var blob = URL.createObjectURL(file);
- //_compress(blob, file, basePath);
- var image = new Image();  
-             image.src = e.target.result;  
-             image.onload = function() {  
- var expectWidth = this.naturalWidth;  
- var expectHeight = this.naturalHeight;  
- 
- if (this.naturalWidth > this.naturalHeight && this.naturalWidth > 800) {  
-                     expectWidth = 800;  
-                     expectHeight = expectWidth * this.naturalHeight / this.naturalWidth;  
-                 } elseif (this.naturalHeight > this.naturalWidth && this.naturalHeight > 1200) {  
-                     expectHeight = 1200;  
-                     expectWidth = expectHeight * this.naturalWidth / this.naturalHeight;  
-                 }  
-                 alert(expectWidth+','+expectHeight);  
- var canvas = document.createElement("canvas");  
- var ctx = canvas.getContext("2d");  
-                 canvas.width = expectWidth;  
-                 canvas.height = expectHeight;  
-                 ctx.drawImage(this, 0, 0, expectWidth, expectHeight);  
-                 alert(canvas.width+','+canvas.height);  
- 
- var base64 = null;  
- var mpImg = new MegaPixImage(image);  
-                     mpImg.render(canvas, {  
-                         maxWidth: 800,  
-                         maxHeight: 1200,  
-                         quality: 0.8,  
-                         orientation: Orientation  
-                     });  
- 
-                 base64 = canvas.toDataURL("image/jpeg", 0.8);  
- 
- //uploadImage(base64);
-                 $("#myImage").attr("src", base64);  
-             };  
-         };  
-         oReader.readAsDataURL(file);  
-     }  
- }  
用到的第三方js文件：mobileBUGFix.mini.js
测试demo下载地址：
[http://download.csdn.net/detail/linlzk/9127441](http://download.csdn.net/detail/linlzk/9127441)
