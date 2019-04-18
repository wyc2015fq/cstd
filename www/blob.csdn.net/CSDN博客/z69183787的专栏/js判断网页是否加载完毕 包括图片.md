# js判断网页是否加载完毕 包括图片 - z69183787的专栏 - CSDN博客
2013年12月18日 13:45:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16181
```java
<script type="text/javascript" language="JavaScript">   
               
            //: 判断网页是否加载完成   
            document.onreadystatechange = function () {    
                if(document.readyState=="complete") {          
                    document.getElementById('divprogressbar').style.display='none';    
                }   
            }    
               
        </script>
```
做web的同学们经常会碰到客户上传图片将网页内容区撑破了的情况，下面就这个问题我们一种如何使用js处理这个问题的方法，具体思路就是在js判断客户端的图片下载完毕之后适时的对该图片的宽度或者高度做一些处理，js处理图片主要是利用javascript中Image对象，通过 onload 事件和 onreadystatechange 来进行判断。
（1）第一中方法，通过onload事件，比如：
[查看代码](http://www.phpernote.com/javascript-function/647.html#viewSource)
[打印](http://www.phpernote.com/javascript-function/647.html#printSource)
|`1`|```<script type=````"text/javascript"``>`|
|`2`|`var``obj=``new``Image();`|
|`3`|`obj.src=``"[http://www.phpernote.com/uploadfiles/editor/201107240502201179.jpg](http://www.phpernote.com/uploadfiles/editor/201107240502201179.jpg)"``;`|
|`4`|`obj.onload=``function``(){`|
|`5`|`    ``alert(``'图片的宽度为：'``+obj.width+``'；图片的高度为：'``+obj.height);`|
|`6`|`    ``document.getElementById(``"mypic"``).innnerHTML=````"<img src='"````+``this``.src+````"' />"````;`|
|`7`|`}`|
|`8`|`</script>`|
|`9`|```<div id=````"mypic"``>onloading……</div>`|
（2）第二种方法，使用 onreadystatechange 来判断
[查看代码](http://www.phpernote.com/javascript-function/647.html#viewSource)
[打印](http://www.phpernote.com/javascript-function/647.html#printSource)
|`01`|```<script type=````"text/javascript"``>`|
|`02`|`var``obj=``new``Image();`|
|`03`|`obj.src=``"[http://www.phpernote.com/uploadfiles/editor/201107240502201179.jpg](http://www.phpernote.com/uploadfiles/editor/201107240502201179.jpg)"``;`|
|`04`|`obj.onreadystatechange=``function``(){`|
|`05`|`    ``if``(``this``.readyState==``"complete"``){`|
|`06`|`        ``alert(``'图片的宽度为：'``+obj.width+``'；图片的高度为：'``+obj.height);`|
|`07`|`        ``document.getElementById(``"mypic"``).innnerHTML=````"<img src='"````+``this``.src+````"' />"````;`|
|`08`|`    ``}`|
|`09`|`}`|
|`10`|`</script>`|
|`11`|```<div id=````"mypic"````>onloading……</div>```|
```java
function loadImage(url, callback) {
    var img = new Image(); //创建一个Image对象，实现图片的预下载
    img.src = url;
     
    if(img.complete) { // 如果图片已经存在于浏览器缓存，直接调用回调函数
        callback.call(img);
        return; // 直接返回，不用再处理onload事件
    }
    img.onload = function () { //图片下载完毕时异步调用callback函数。
        callback.call(img);//将回调函数的this替换为Image对象
    };
};
```
