# $(document).ready和window.onload区别 - weixin_33985507的博客 - CSDN博客
2018年04月11日 15:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
在上一篇文章中[为什么script标签一般放在body下面](https://www.jianshu.com/p/86250c123e53)，结尾我们给出了解决办法，那这些解决办法到底是什么意思呢？
## 1.区别##
在Jquery里面，我们可以看到两种写法:
```
$(function(){}) 和$(document).ready(function(){})
```
这两种效果是一样的。
指的是：表示文档结构已经加载完成（不包含图片等非**媒体文件**）；
而
```
window.onload=function(){}
```
指的是：页面包含图片等文件在内的所有元素都加载完成。
一般情况，一个**页面响应加载的顺序**是，域名解析-加载html-加载js和css-加载图片等其他媒体信息。
```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Document</title>
    <script src="http://static-online.zhaogangtest.com/publicresource/js/jquery.min.js"></script>
    <script>
    $(document).ready(function() {
        console.log(1) 
    })
    window.onload = function() {
        console.log(3)
    }
    </script>
</head>
<body>
    <div id="test">
        我是html结构，我要展示到页面上！
    </div>
</body>
</html>
<script>
console.log(2)
</script>
//结果为2 1 3
```
再次测试
```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Document</title>
    <script src="http://static-online.zhaogangtest.com/publicresource/js/jquery.min.js"></script>
    <script>
    $(document).ready(function() {
        console.log(1) 
    })
    window.onload = function() {
        console.log(3)
    }
    </script>
</head>
<body>
    <div id="test">
        <img src="http://www.ctoutiao.com/uploads/2016/11/03/1d726591cb02cf684c30f3a279769181.jpg" alt="">
        我是html结构，我要展示到页面上！
    </div>
</body>
</html>
<script>
console.log(2)
</script>
由于图片很大，可以看见加载的过程。先出现2 1，图片加载完出现3
```
所以正验证上面所说的页面响应加载顺序，ready事件在加载js和css之后，加载图片等其他媒体信息之前。注，这里的加载css之后并不是css加载完成。而是，开始请求css，但由于css是异步加载的，ready事件是在dom加载完成，css加载完成之前就触发。
onload事件在加载图片等其他媒体信息之后。
**注：如果script写在上面，一般用ready事件，即文档结构加载完后就加载js，而不是等到图片加载完再执行js，这样反倒浪费了时间。最好直接写在下面，这样就不用写ready事件，而且默认直接加载完dom就加载下面的js**
下面方式jquery中ready事件的源码：
```
document.ready = function (callback) {
    ///兼容FF,Google
    if (document.addEventListener) {
        document.addEventListener('DOMContentLoaded', function () {
            document.removeEventListener('DOMContentLoaded', arguments.callee, false);
            callback();
        }, false)
    }
     //兼容IE
    else if (document.attachEvent) {
        document.attachEvent('onreadystatechange', function () {
              if (document.readyState == "complete") {
                        document.detachEvent("onreadystatechange", arguments.callee);
                        callback();
               }
        })
    }
    else if (document.lastChild == document.body) {
        callback();
    }
}
```
通过源码我们可以知道，本质就是调用原生的DOMContentLoaded事件
- 当 DOMContentLoaded 事件触发时，仅当DOM加载完成，不包括样式表，图片。
注:这里的dom加载完成，并没有渲染到屏幕上，只有css加载完才会渲染到屏幕上。具体css渲染原理可以戳这。
