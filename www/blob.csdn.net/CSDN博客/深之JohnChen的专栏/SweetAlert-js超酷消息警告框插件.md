# SweetAlert-js超酷消息警告框插件 - 深之JohnChen的专栏 - CSDN博客

2017年03月04日 09:20:19[byxdaz](https://me.csdn.net/byxdaz)阅读数：1101


SweetAlert是一款使用纯js制作的消息警告框插件。这款消息警告框插件能够很容易的在警告框中插入图片、动画等元素，是替代原生消息警告框的最佳选择。

源代码下载地址：[https://github.com/t4t5/sweetalert](https://github.com/t4t5/sweetalert)

来通过一张gif图片看看SweetAlert的效果：
![SweetAlert消息警告框效果](http://img.htmleaf.com/1412/sweetalert.gif)
### 使用方法

要使用该插件，首先要在html的header中引入以下文件：

```java
<scriptsrc="lib/sweet-alert.min.js"></script>
<linkrel="stylesheet"type="text/css"href="lib/sweet-alert.css">
```

最基本的调用方法：

sweetAlert("Hello world!");

带错误图标的警告框：

sweetAlert("Oops...","Something went wrong!","error");

一个带有确认按钮的警告框，点击确认按钮可触发动画：

```java
sweetAlert({
  title:"Are you sure?",
  text:"You will not be able to recover this imaginary file!",
  type:"warning",
  showCancelButton:true,
  confirmButtonColor:"#DD6B55",
  confirmButtonText:"Yes, delete it!",
  closeOnConfirm:false
}, function(){
  swal("Deleted!",
  "Your imaginary file has been deleted.",
  "success");
});
```


```

```

### 可用参数
|参数|默认值|描述|
|----|----|----|
|title|null(required)|窗口的名称。可以通过对象的"title"属性或第一个参数进行传递。|
|text|null|窗口的描述。可以通过对象的"text"属性或第二个参数进行传递。|
|type|null|窗口的类型。SweetAlert 有4种类型的图标动画："warning", "error", "success" 和 "info".可以将它放在"type"数组或通过第三个参数传递。|
|allowOutsideClick|false|如果设置为“true”，用户可以通过点击警告框以外的区域关闭警告框。|
|showCancelButton|false|如果设置为“true”，“cancel”按钮将显示，点击可以关闭警告框。|
|confirmButtonText|"OK"|该参数用来改变确认按钮上的文字。如果设置为"true"，那么确认按钮将自动将"Confirm"替换为"OK"。|
|confirmButtonColor|"#AEDEF4"|该参数用来改变确认按钮的背景颜色（必须是一个HEX值）。|
|cancelButtonText|"Cancel"|该参数用来改变取消按钮的文字。|
|closeOnConfirm|true|如果希望以后点击了确认按钮后模态窗口仍然保留就设置为"false"。该参数在其他SweetAlert触发确认按钮事件时十分有用。|
|imageUrl|null|添加自定义图片到警告框上。必须是图片的完整路径。|
|imageSize|"80x80"|当设定图片的路径后，你可以设定图片的大小，格式为两个数字中间带个"x"符号。|
|timer|null|警告框自动关闭的时间。单位是ms。|

本文版权属于jQuery之家，转载请注明出处：[http://www.htmleaf.com/jQuery/Lightbox-Dialog/20141218888.html](http://www.htmleaf.com/jQuery/Lightbox-Dialog/20141218888.html)

