# 利用HTML5，前端js实现图片压缩 - =朝晖= - 博客园
# [利用HTML5，前端js实现图片压缩](https://www.cnblogs.com/dhcn/p/7120725.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/qazwsx2345/article/details/21827553
主要用了两个[HTML5](http://lib.csdn.net/base/html5)的 API，一个file，一个canvas，压缩主要使用cnavas做的，file是读取文件，之后把压缩好的照片放入内存，最后内存转入表单下img.src，随着表单提交。
照片是自己用单反拍的，5M多，压缩下面3张分别是600多kb，400多kb，300kb的最后那张失真度很大了，压缩效率蛮高的。
```
<!DOCTYPE html>  
<html>  
<head>  
    <meta charset="utf-8"/>  
    <title>File API Test</title>  
    <script type="text/javascript" src="js/jquery-1.11.0.min.js"></script>  
    <script type="text/javascript" src="js/JIC.js"></script>  
    <style>  
        #test{  
            display: none;  
        }  
    </style>  
</head>  
<body>  
<input type="file" id="fileImg" >  
<form>  
    <img src="" id="test" alt="">  
</form>  
<script>  
    function handleFileSelect (evt) {  
        // var filebtn = document.getElementById(id);  
        // console.log(filebtn);  
        // var files = filebtn.target.files;  
        // console.log(filebtn.target);  
        // console.log(files);  
        var files = evt.target.files;  
        for (var i = 0, f; f = files[i]; i++) {  
   
          // Only process image files.  
          if (!f.type.match('image.*')) {  
            continue;  
          }  
   
          var reader = new FileReader();  
   
          // Closure to capture the file information.  
          reader.onload = (function(theFile) {  
            return function(e) {  
              // Render thumbnail.  
              // console.log(evt.target.files[0]);  
              // console.log(e.target);  
              console.log(e.target.result);  
              var i = document.getElementById("test");  
              i.src = event.target.result;  
              console.log($(i).width());  
              console.log($(i).height());  
              $(i).css('width',$(i).width()/10+'px');  
              //$(i).css('height',$(i).height()/10+'px');  
              console.log($(i).width());  
              console.log($(i).height());  
              var quality =  50;  
              i.src = jic.compress(i,quality).src;  
              console.log(i.src);  
              i.style.display = "block";  
            };  
          })(f);  
   
          // Read in the image file as a data URL.  
          reader.readAsDataURL(f);  
        }  
      }  
   
    document.getElementById('fileImg').addEventListener('change', handleFileSelect, false);  
</script>  
</body>  
</html>  
   
  
var jic = {  
        /**  
         * Receives an Image Object (can be JPG OR PNG) and returns a new Image Object compressed  
         * @param {Image} source_img_obj The source Image Object  
         * @param {Integer} quality The output quality of Image Object  
         * @return {Image} result_image_obj The compressed Image Object  
         */  
   
        compress: function(source_img_obj, quality, output_format){  
               
             var mime_type = "image/jpeg";  
             if(output_format!=undefined && output_format=="png"){  
                mime_type = "image/png";  
             }  
               
   
             var cvs = document.createElement('canvas');  
             //naturalWidth真实图片的宽度  
             cvs.width = source_img_obj.naturalWidth;  
             cvs.height = source_img_obj.naturalHeight;  
             var ctx = cvs.getContext("2d").drawImage(source_img_obj, 0, 0);  
             var newImageData = cvs.toDataURL(mime_type, quality/100);  
             var result_image_obj = new Image();  
             result_image_obj.src = newImageData;  
             return result_image_obj;  
        },  
       function   ****(***){}  
}
```

