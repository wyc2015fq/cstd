
# jQuery读书笔记---jQuery中Ajax--序列化元素、Ajax全局事件 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月20日 11:44:12[Jlins](https://me.csdn.net/dyllove98)阅读数：1327标签：[javascript																](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)个人分类：[jquery																](https://blog.csdn.net/dyllove98/article/category/1349854)




serialize()方法：
作用于一个jQuery对象，它能够将DOM元素内容序列化为字符串，用于Ajax请求。![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html><html><headlang="en"><metacharset="UTF-8"><scripttype="text/javascript"src="../../js/jquery-2.1.3.js"></script><style>*{margin:0;padding:0;}body{font-size:12px;}.comment{margin-top:10px;padding:10px;border:1px solid \#ccc;background:\#DDD;}.comment h6{font-weight:700;font-size:14px;}.para{margin-top:5px;text-indent:2em;background:\#DDD;}</style><title></title></head><body><formid="form1"action="\#"><p>评论:</p><p>姓名:<inputtype="text"name="username"id="username"/></p><p>内容:<textareaname="content"id="content"rows="2"cols="20"></textarea></p><p><inputtype="button"id="send"value="提交"></p></form><divclass="comment">已有评论:</div><divid="resText"></div></body><scripttype="text/javascript">$(function() {
        $("\#send").click(function() {//这种方式在只有少量的字段的表单中，勉强还可以使用，但如果表单元素越来越复杂，使用这种方式在增大工作量的同时//也使表单缺乏弹性。jQuery为这一常用操作提供了一个简化的方法----serialize()//$.get("get1.jsp", {//username: encodeURI($("\#username").val()),//content: encodeURI($("\#content").val())//}, function (data, textStatus) {//$("\#resText").html(decodeURI(data));//});//serialize()方法作用于一个jQuery对象，它能够将DOM元素内容序列化为字符串，用于Ajax请求。$.get("get1.jsp", $("\#form1").serialize(),function(data, textStatus) {
                $("\#resText").html(data);
            })
        });/**
         * 因为serialize()方法作用于jQuery对象，所以不光只有表单能使用它，其他选择器选取的元素也都能使用它，如以下jQuery代码：
         *      $(":checkbox,:radio").serialize();
         *      把复选框和单选框的值序列化为字符串形式，只会将选中的值序列化
         *
         *
         * serializeArray()方法
         *  该方法不是返回字符串，而是将DOM元素序列化后，返回JSON格式的数据。
         *
         *
         * $.param()方法。它是serialize()方法的核心，用来对一个数组或对象按照key/value进行序列化。
         *
         * 例如：
         *      var obj = {a:1, b:2, c:3};
         *      var k = $.param(obj);
         *      alert(k);//输出a=1&b=2&c=3
         *
         **/})</script></html>![复制代码](http://common.cnblogs.com/images/copycode.gif)
Ajax全局事件：
|方法名称|说明|
|ajaxComplete(callback)|Ajax请求完成时执行的函数|
|ajaxError(callback)|Ajax请求发生错误时执行的函数，捕捉到的错误可以作为最后一个参数传递|
|ajaxSend(callback)|Ajax请求发送前执行的函数|
|ajaxSuccess(callback)|Ajax请求成功时执行的函数|


**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)


