# Jquery解决浏览器跨域问题-Chrome Firefox亲测 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年02月21日 12:02:46[boonya](https://me.csdn.net/boonya)阅读数：158








## 资料参考

参考以下文章基本就全覆盖了：

[轻松搞定JSONP跨域请求](https://www.cnblogs.com/think-in-java/p/7285296.html)

[js ajax跨域被阻止 CORS 头缺少 'Access-Control-Allow-Origin'](https://blog.csdn.net/weixin_42441435/article/details/81480104)

[解决了设置了Access-Control-Allow-Origin: *还是跨域的问题](https://blog.csdn.net/yanzisu_congcong/article/details/80552155)

## 后端解决方式

```java
response.setHeader("Access-Control-Allow-Origin",  request.getHeader("Origin"));
        response.setHeader("Access-Control-Allow-Credentials", "true");
        response.setHeader("Access-Control-Allow-Methods", "POST, GET, PUT, OPTIONS, DELETE");  
        response.setHeader("Access-Control-Allow-Headers", "x-requested-with, Content-Type");
```

更多更全参考：

[跨域拦截Access-Control-Allow-Origin设置多个origin](https://www.cnblogs.com/Java-Starter/p/7603313.html)

## 前端Jquery方式

方法一：

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>jQuery实现JSONP</title>
</head>
<body>
    <div id="mydiv">
        <button id="btn">点击</button>
    </div>
</body>
<script type="text/javascript" src="https://code.jquery.com/jquery-3.1.0.min.js"></script>
<script type="text/javascript">
    $(function(){
        $("#btn").click(function(){

            $.ajax({
                async : true,
                url : "https://api.douban.com/v2/book/search",
                type : "GET",
                dataType : "jsonp", // 返回的数据类型，设置为JSONP方式
                jsonp : 'callback', //指定一个查询参数名称来覆盖默认的 jsonp 回调参数名 callback
                jsonpCallback: 'handleResponse', //设置回调函数名
                data : {
                    q : "javascript", 
                    count : 1
                }, 
                success: function(response, status, xhr){
                    console.log('状态为：' + status + ',状态是：' + xhr.statusText);
                    console.log(response);
                }
            });
        });
    });
	
	function handleResponse(response){
       // 对response数据进行操作代码
	   alert("jsonp success!");
    }
</script>
</html>
```

方法二：

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>jQuery实现JSONP</title>
	<meta http-equiv="Access-Control-Allow-Origin" content="*">
</head>
<body>
    <div id="mydiv">
        <button id="btn">点击</button>
    </div>
</body>
<script type="text/javascript" src="https://code.jquery.com/jquery-3.1.0.min.js"></script>
<script type="text/javascript">
    $(function(){
        $("#btn").click(function(){

           $.getJSON("https://api.douban.com/v2/book/search?q=javascript&count=1&callback=?", function(data){
                console.log(data);
				alert("jsonp success!");
            });
        });
    });
	
</script>
</html>
```





