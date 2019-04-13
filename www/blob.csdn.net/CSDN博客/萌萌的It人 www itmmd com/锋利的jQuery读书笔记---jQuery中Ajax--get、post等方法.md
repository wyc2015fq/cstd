
# 锋利的jQuery读书笔记---jQuery中Ajax--get、post等方法 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月20日 11:43:55[Jlins](https://me.csdn.net/dyllove98)阅读数：1993标签：[javascript																](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)个人分类：[javaScript																](https://blog.csdn.net/dyllove98/article/category/1158758)



load()方法通常用来从Web服务器上获取静态的数据文件，然而这并不能体现ajax的全部价值。
在项目中，如果需要传递一些参数给服务器中的页面，那么可以使用$.get()或者$.post()方法(或者是$.ajax()方法)
$.get()方法使用GET方式来进行异步请求。结构为：$.get(url [, data] [, callback] [, type])
$.get()方法参数解释如下:
|参数名称|类型|说明|
|url|String|请求的HTML页的URL地址|
|data(可选)|Object|发送至服务器的key/value数据会作为QueryString附加到请求URL中|
|callback(可选)|Function|载入成功时回调函数(只有当Response的返回状态是success才调用该方法)自动将请求结果和状态传递给该方法|
|type(可选)|String|服务器端返回内容的格式，包括xml、html、script、json、text和_default|
$.post()方法。
$.post()和$.get()方法的结构和使用方式相同，不过它们之间仍然有以下区别：
GET请求会将参数跟在URL后进行传递，而POST请求则是作为HTTP消息的实体内容发送给Web服务器。
GET方式对传输的数据有大小限制(通常不大于2KB)，而使用POST方式传递的数据量要比GET方式大得多(理论上不受限制)
GET方式请求的数据会被浏览器缓存起来，因此其他人就可以从浏览器的历史记录中读取到这些数据，例如账号和密码等。在某种情况下，GET方式会带来严重的安全性问题，而POST方式相对来说可以避免这些问题
GET方式和POST方式传递的数据在服务器端的获取也不相同。
$.getScript()：jQuery提供了此方法来直接加载js文件，与加载一个HTML片段一样简单方便，并且不需要对JavaScript文件进行处理，JavaScript文件会自动执行。
jQuery代码如下：
$(function () {
        $("\#send").click(function () {
            $.getScript("test.js");
        });
})
与其他ajax方法一样，$.getScript()方法也有回调函数，它会在JavaScript文件载入成功后运行。
例如：想载入jQuery官方颜色动画插件(jquery.color.js)，成功后给元素绑定颜色变化动画：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><headlang="en"><metacharset="UTF-8"><scripttype="text/javascript"src="../../js/jquery-2.1.3.js"></script><style>*{margin:0;padding:0;}body{font-size:12px;}.comment{margin-top:10px;padding:10px;border:1px solid \#ccc;background:\#DDD;}.comment h6{font-weight:700;font-size:14px;}.para{margin-top:5px;text-indent:2em;background:\#DDD;}.block{width:80px;height:80px;background:\#DDD;}</style><title></title></head><body><buttonid="go">运行</button><divclass="block"></div></body><scripttype="text/javascript">$(function() {
        $.getScript("jquery.color.js",function() {
            $("\#go").click(function() {
                $(".block").animate({backgroundColor:'pink'},1000)
                        .animate({backgroundColor:'blue'},1000);
            });
        })
    })</script></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)

$.getJson()：该方法用于加载JSON文件，用法与$.getScript()相同。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><headlang="en"><metacharset="UTF-8"><scriptsrc="../../js/jquery-2.1.3.js"></script><style>*{margin:0;padding:0;}body{font-size:12px;}.comment{margin-top:10px;padding:10px;border:1px solid \#ccc;background:\#DDD;}.comment h6{font-weight:700;font-size:14px;}.para{margin-top:5px;text-indent:2em;background:\#DDD;}</style><title></title></head><body><br/><p><inputtype="button"id="send"value="加载"/></p><divclass="comment">已有评论：</div><divid="resText"></div></body><scripttype="text/javascript">$(function() {
        $('\#send').click(function() {
            $.getJSON('test.json',function(data) {
                $('\#resText').empty();varhtml='';
                $.each( data  ,function(commentIndex, comment) {
                    html+='<div class="comment"><h6>'+comment['username']+':</h6><p class="para">'+comment['content']+'</p></div>';
                })
                $('\#resText').html(html);
            })
        })
    })</script></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)
test.json文件为：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
[
  {"username": "张三","content": "沙发."},
  {"username": "李四","content": "板凳."},
  {"username": "王五","content": "地板."}
]![复制代码](http://common.cnblogs.com/images/copycode.gif)

使用JSONP形式的回调函数来加载其他网站的JSON数据。例如：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html>
<html>
<head lang="en">
    <meta charset="UTF-8">
    <script src="../../js/jquery-2.1.3.js"></script>
    <style>
        * { margin:0; padding:0;}
        body { font-size:12px;}
        .para {
            width:100px;
            height:100px;
            margin:5px;
            border:0;
        }</style>
    <title></title>
</head>
<body>
<p>
    <input type="button" id="send" value="加载"/>
</p>
<div id="resText" >
</div>
</body>
<script type="text/javascript">$(function() {
        $('\#send').click(function() {
            $.getJSON("https://api.flickr.com/services/feeds/photos_public.gne?tags=car&tagmode=any&format=json&jsoncallback=?",function(data){
                        $.each(data.items,function( i,item ){
                            $("<img class='para'/> ").attr("src", item.media.m).appendTo("\#resText");if( i == 3) {returnfalse;
                            }
                        });
                    }
            );
        })
    })/**
     *  JSONP(JSON with Padding)是一个非官方的协议，它允许在服务器端集成Script tags返回至客户端，通过JavaScript Callback的形式实现跨域访问
     *  上述的url地址并不能请求到数据，仅用作说明。
     **/</script>
</html>![复制代码](http://common.cnblogs.com/images/copycode.gif)
注意：
jQuery将自动把URL里的回调函数，例如"url?callback=?"中的后一个”?“替换为正确的函数名，以执行回调函数。
JSONP(JSON with Padding)是一个非官方的协议，它允许在服务器端集成Script tags返回至客户端，通过JavaScript Callback的形式实现跨域访问。由于JSON只是一种含有简单括号结构的纯文本，因此许多通道都可以交换JSON消息。而由于同源策略的限制，开发人员不能在于外部服务器进行通信的时候使用XMLHttpRequest。而JSONP是一种可以绕过同源策略的方法，即通过使用JSON和<script>标记相结合的方法，从服务器端直接返回可执行的JavaScript函数调用或者JavaScript对象。目前JSONP已经成为各大公司的Web应用程序跨域首选。
$.ajax()方法时jQuery最底层的ajax实现。它的结构为:
$.ajax(options).该方法只有一个参数，但在这个对象里包含了$.ajax()方法所需要的请求设置以及回调函数等信息，参数以key/value的形式存在，所有参数都是可选的，
常用参数列表为：
|参数名称|类型|说明|
|url|String|(默认为当前页地址)发送请求的地址|
|type|String|请求方式(post或get)默认为get。注意其他HTTP请求方法，如PUT何DELETE也可以使用，但仅部分浏览器支持|
|timeout|Number|设置请求超时时间(毫秒)。此设置将覆盖$.ajaxSetup()方法的全局设置|
|data|Object或String|发送到服务器的数据。如果不是字符串，将自动转换为字符串格式。GET请求中将附加在url后。防止这种自动转换，可以查看processData选项。对象必须为key/value格式，例如{foo1:"bar1", foo2:"bar2"}转换为&foo1=bar1&foo2=bar2。如果是数组，jQuery将自动为不同值对应同一个名称。例如{foo:["bar1", "bar2"]}转换为&foo=bar1&foo=bar2|
|dataType|String|预期服务器返回的数据类型。如果不指定，jQuery将自动根据HTTP包MIME信息返回responseXML或responseText，并作为回调函数参数传递。可用的类型如下。
|xml：返回XML文档，可用jQuery处理
|html：返回纯文本HTML信息；包含的script标签会在插入DOM时执行
|script：返回纯文本JavaScript代码。不会自动缓存结果。除非设置了cache参数。注意在远程请求时(不在同一个域下)，所有POST请求都将转为GET请求。
|json：返回JSON数据
|jsonp：JSONP格式。使用JSONP形式调用函数时，例如myurl?callback=?，jQuery将自动替换后一个”?“为正确的函数名，以执行回调函数。
|text：返回纯文本字符串
|
|beforeSend|Function|发送请求前可以修改XMLHttpRequest对象的函数，例如添加自定义HTTP头。在beforeSend中如果返回false可以取消本次Ajax请求。XMLHttpRequest对象是唯一的参数。
|function(XMLHttpRequest) {
|this;//调用本次Ajax请求时传递的options参数
|}
|
|complete|Function|请求完成后调用的回调函数(请求成功或失败时均调用)
|参数：XMLHttpRequest对象和一个描述成功请求类型的字符串。
|function(XMLHttpRequest, textStatue){
|this;//调用本次Ajax请求时传递的options参数
|}
|
|success|Function|请求成功后调用的回调函数，有两个参数。
|(1)由服务器返回，并根据dataType参数进行处理后的数据
|(2)描述状态的字符串
|function(data, textStatus) {
|//data可能是xmlDoc、jsonObj、html、text等
|this;//调用本次Ajax请求时传递的options参数
|}
|
|error|Function|请求失败时被调用的函数。该函数有3个参数，即XMLHttpRequest对象、错误信息、捕获的错误对象(可选)。
|Ajax事件函数如下：
|function(XMLHttpRequest, textStatus, errorThrown) {
|//通常情况下textStatus和errorThrown只有其中一个包含信息
|this;//调用本次Ajax请求时传递的options参数
|}
|
|global|Boolean|默认为true。表示是否触发全局Ajax事件。设置为false将不会触发全局Ajax事件，AjaxStart或AjaxStop可用于控制各种Ajax事件
|
**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)


