# RESTful Web Service(三):使用ajax创建客户端 - z69183787的专栏 - CSDN博客
2012年10月27日 12:25:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1038
上个文章中使用eclipse创建的web service。根据rest的思想，每一个资源都有唯一标识的URI。在REST Web service explorer 中可以看到每个资源的URI。
这里放出一些调用的例子：
1.get请求获得Category xml 数据。
$("#btncall").click(function (){
    $.ajax({
        url:"/restblog/services/category",
        type:"GET",
        **dataType:'xml'**,
        success :function (data){
            var tablehtml="<table class='tableresult' border='1' bordercolor='#ccc'><tr><th>编号</th><th>名称</th></tr>";
            $("category",data).each(function (i,category){
                var id=$("id",category).text();
                var name=$("name",category).text();
                tablehtml+=("<tr><td>"+id+"</td><td>"+name+"</td></tr>");
            });
            tablehtml+="</table>";
        $(tablehtml).appendTo("#result");
        },error:function (){
            alert("get category list failed!");
    }});
});
jquery的ajax调用有一个参数dataType是期望得到的数据格式，目前版本的可选值为xml,json,html,jsonp.默认的是html。指定了该属性，jquery会自动解析数据格式。
上面的例子接受到的数据形如：
<?xml version="1.0" encoding="UTF-8"?> 
   <Categories> 
       <category> 
           <id>1</id> 
           <name>第一个分类</name> 
       </category> 
       <category> 
           <id>2</id> 
           <name>第二个分类</name> 
       </category> 
   </Categories>
data是一个xml对象，在没有jquery的情况下解析xml是一件很麻烦的事情。但是使用jquery，xml解析起来和html一样。
$("category",data)是查找标签名(tagName)为category节点，返回jquery对象。注意获得节点值用的是text()方法。
还得注意的地方就是url 这里的路径和请求jsp的规则是一样。
2.发送json的例子。
$("#btnAddbyjson").click(function (){
    $.ajax({
        url:"/restblog/services/category/add",
        **type:"PUT",        data:"{\"id\":1,\"name\":\"category1\"}",        contentType:'application/json',**
        success :function (result){
            alert(result);
        },error:function (){
            alert("add category failed!");
    }});
});
注意type: 通常的ajax调用都只是GET POST 这里用的是PUT，要回过神来。
contentType:与服务方法上的Consume 标注属性一样。在ajax调用的时候会在请求的Header添加{"contentType","application/json"}的键/值标明发送的数据格式是json。
即便发送的数据格式是json,但是真正的数据是以字符串的形式在网络传输的，所以data属性只是json的字符串，由服务器端解析成Category对象。这里的json字符串格式比较严格，不能适用单引号' ,这里要注意。
data属性即可以是字符串又可以是json,jquery会根据contentType解析成需要发送的格式。
下面的截图是在FireBug下看到的数据发送情况：
![1](http://www.blogjava.net/images/blogjava_net/Hafeyang/WindowsLiveWriter/RESTfulWebServiceajax_10732/1_thumb.jpg)
这张图旨在说明发送的数据不是一个键/值类型。区别与下面的例子。
3.POST的表单提交。
$("#btnAdd").click(function (){
    $.ajax({
        url:"/restblog/services/category/addbyname",
        type:"POST",
        data:{'categoryname':$("#txtName").val()},
        success :function (result){
            alert(result);
        },error:function (){
            alert("add category failed!");
    }});
});
addbyname方法的categoryname标识为@FormParam 所以用POST提交的数据被当做Form提交的数据。下面的截图说明浏览器发送的是一个标准的键/值数据
![2](http://www.blogjava.net/images/blogjava_net/Hafeyang/WindowsLiveWriter/RESTfulWebServiceajax_10732/2_thumb.jpg)
4.调用带@Context标识参数的方法
$("#btncommonProcess").click(function (){
    $.ajax({
        url:"/restblog/services/category/commonProcess",
        type:"GET",
        data:"a=b&a=e&c=d",
        success :function (result){
            alert(result);
        },error:function (){
            alert("commonProcess failed!");
    }});
});[](http://11011.net/software/vspaste)
服务端的代码见上一篇文章。
控制台上打印：
c : [d]
a : [b, e]
### 总结
REST Web service 不但提供了标准的调用方法，同时拥有非常简便的ajax调用。由于请求的纯数据，所以他更适合于ajax调用。而且的客户端可以跨平台，跨应用。他的优势是无可比拟的，我相信他的未来不可估量。
