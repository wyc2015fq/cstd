# web 图片上传实现本地预览 - weixin_33985507的博客 - CSDN博客
2018年06月06日 17:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
在说上传之前先说说如何替换or美化浏览器自带的简陋上传按钮（自定义自己的上传按钮 如:img）:
  1.将自定义上传按钮上方添加 input file 框，实现input实现透明处理。
  2.对自定义上传按钮添加事件。隐藏真正input框。用事件来触发：
     function imgBtn(){
      return document.getElementById("inputid").click();   
    }
现在浏览器基本都支持H5 。针对H5的代码：
```
var docObj=document.getElementById("inputid");//file文本框id
var imgObjPreview=document.getElementById("imgId");//预显示的图片
imgObjPreview.src = window.URL.createObjectURL(docObj.files[0]);
```
 针对ie的代码：
```
docObj.select();
docObj.blur();
var localImagId = document.getElementById("divid or imgid");
var imgSrc = docObj.createTextRange().text;
try{         
   localImagId.style.filter="progid:DXImageTransform.Microsoft.AlphaImageLoader(sizingMethod=scale)";
   localImagId.filters.item("DXImageTransform.Microsoft.AlphaImageLoader").src = imgSrc;
}
catch(e)
{
   console.log(e);
   alert("您上传的图片格式不正确，请重新选择!");
   return false;
}
```
很多大型网站上都使用到了这个滤镜，它是IE滤镜的一种，其主要作用就是对图片进行透明处理。虽然FireFox和IE7以上的IE浏览器已经支持透明的PNG图片，但是就IE5-IE6而言还是有一定的意义。
语法：
```
filter : progid:DXImageTransform.Microsoft.AlphaImageLoader ( enabled=bEnabled , sizingMethod=sSize , src=sURL )
```
属性：
　　enabled :　可选项。布尔值(Boolean)。设置或检索滤镜是否激活。true | false （这个基本可以忽略，被禁止了那还搞什么……）
　　　　　true :　 默认值。滤镜激活。
　　　　　false :　 滤镜被禁止。
   sizingMethod : 可选项。字符串(String)。设置或检索滤镜作用的对象的图片在对象容器边界内的显示方式。（可以无视这行说明，关键是下面的参数。一般来说单独图片且精度比较高的使用scale比较合适，除非您把所有的要用到的图片都放到一张图那就当我没说过。但是那样做的话这张png图片的体积估计不会很小。另，这个滤镜只是加载，其填充方式还是可以受CSS控制的。）
　　　　　crop :　剪切图片以适应对象尺寸。
　　　　　image :　默认值。增大或减小对象的尺寸边界以适应图片的尺寸。
　　　　　scale :　缩放图片以适应对象的尺寸边界。
　　src :　必选项。字符串(String)。（指定图片的路径。要注意的是这个路径是指加载滤镜的页面相对于图片的路径而不是css文件相对于图片的路径。这跟一般的图片加载有区别。）使用实例：
```
.login_logo {
　　background-image: url(../Image/login_logo.png);
　　_background-image: none;
　　_filter:progid:DXImageTransform.Microsoft.AlphaImageLoader(enabled=true, sizingMethod=scale, src='/Public/Image/login_logo.png');　　
}
```
上面那样预览,图片的临时格式是这样的:blob:http://XXX.com/3fdc2df8-bb2d-479a-bb45-796f976d0b27
参考: https://www.cnblogs.com/hzxy-blog/p/6410204.html
上面怎么点击浏览按钮后,图片显示在网页中了,这个用到了, 浏览按钮的各个事件 可以参考下...
```
<body>  
<input type="file" id="input">  
</body>  
<script>  
    document.getElementById("input").addEventListener("focus",function () {  
        console.log("focus");  
    });  
  
    document.getElementById("input").addEventListener("mousedown",function () {  
        console.log("mousedown");  
    });  
  
    document.getElementById("input").addEventListener("mouseup",function () {  
        console.log("mouseup");  
    });  
  
    document.getElementById("input").addEventListener("input",function () {  
        console.log("input");  
    });  
  
    document.getElementById("input").addEventListener("change",function () {  
        console.log("change"); 
    });  
  
    document.getElementById("input").addEventListener("blur",function () {  
        console.log("blur");  
    });  
  
    document.getElementById("input").addEventListener("click",function () {  
        console.log("click");  
    });  
  
  
</script>
```
 onchange可以这样写:(本地就可以浏览要上传的文件了)
```
var docObj=document.getElementById("uploaderInput");//获取input=file 按钮对象
var imgObjPreview=document.getElementById("imgId");//获取一个img的对象
imgObjPreview.src = window.URL.createObjectURL(docObj.files[0]);//把file选中的图片临时地址复制给img的src-- 很容易理解
//和上面没关系--这个直接把一个li元素包着图片显示,
let bg = window.URL.createObjectURL(docObj.files[0]);//ES6 定义一个变量
let el = `<li class="weui-uploader__file" style="background-image:url(${bg})"></li>`;//ES6 新特性,模板字符串替换变量, 原来JavaScript需要用+号拼接,很麻烦...
```
 或一个现成写法:
```
function clickup(obj1,obj2)
{
    //获取点击的文本框
    var file = document.getElementById("uploaderInput");
    //存放图片的父级元素
    var imgContainer = document.getElementsByClassName(obj1)[0];
    //获取的图片文件
    var fileList = file.files;
    //文本框的父级元素
    var input = document.getElementsByClassName(obj2)[0];
    var imgArr = [];
    //遍历获取到得图片文件
    for (var i = 0; i < fileList.length; i++) {
        let imgUrl = window.URL.createObjectURL(file.files[i]);
        alert(imgUrl);
        $("#uploaderFiles").append(`<li class="weui-uploader__file" style="background-image:url(${imgUrl})"></li>`);
        //imgArr.push(imgUrl);
        //var img = document.createElement("img");
        //img.setAttribute("src", imgArr[i]);
}
```
web 图片上传实现本地预览
也可以这样 : 
# [html之file标签 --- 图片上传前预览 -- FileReader](https://www.cnblogs.com/tandaxia/p/5125275.html)
https://www.cnblogs.com/tandaxia/p/5125275.html
