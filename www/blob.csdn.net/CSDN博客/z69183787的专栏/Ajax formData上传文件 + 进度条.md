# Ajax formData上传文件 + 进度条 - z69183787的专栏 - CSDN博客
2018年02月26日 15:19:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：200
```java
//
require('./lib/ajax');
require('jquery');
var type=0,//0 dp  1 mt
    localHost = location.host.split('.'),
    localUrl = location.origin.search(/xxxx/g) > -1 ? '//g.xxxx.com' > -1 : '//g.xxxx.com';
    if(location.origin.match('meituan')) {
      type=1;
    }
$(".file").on('change',function(es){
   $(".pops").removeClass('hide');
   console.log('kaishi');
   var file = $(".file")[0].files[0]; //文件对象
   uploadVideo(file)
})
function uploadVideo(file){
  console.log('show');
  var name = file.name,       //文件名
      form = new FormData();
      form.append("file", file);
      if(file.size>10000000){
        alert('请上传低于10兆的视频文件')
        window.location.href='upload.html?t=123'
        return
      }
       var xhr = new XMLHttpRequest();
       xhr.upload.addEventListener("progress", uploadProgress, false);
       xhr.addEventListener("load", uploadComplete, false);
       xhr.addEventListener("error", uploadFailed, false);
       xhr.addEventListener("abort", uploadCanceled, false);
       xhr.open("POST", '//g.xxxx.com/xxxx/xxxx/demo/uploadImage');//修改成自己的接口
       xhr.send(form);
}
function uploadProgress(){
  console.log('progress');
  $(".pops").removeClass('hide');
  $(".pops").show();
}
function uploadComplete(evt){
  var data = evt.target.responseText;
  if(typeof data==="string") {
    data = JSON.parse(data);
  }
  var id = data.data;
  $(".dp").show()
  $(".mt").show()
  $(".pops").addClass('hide');
}
function uploadFailed(){
  console.log('uploadFailed');
}
function uploadCanceled(){
  console.log('uploadCanceled');
}
```
