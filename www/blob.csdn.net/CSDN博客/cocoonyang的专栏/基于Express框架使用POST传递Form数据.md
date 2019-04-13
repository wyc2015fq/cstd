
# 基于Express框架使用POST传递Form数据 - cocoonyang的专栏 - CSDN博客


2016年08月25日 09:01:15[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：12243个人分类：[nodejs																](https://blog.csdn.net/cocoonyang/article/category/1301193)



## 客户端
## 使用Form发送数据
在客户端Html文件中Form代码如下：

```python
<!-- POST test  -->
<form action="/test" method="post"  id="foo"  >
    <input type="text" name="username">
    <input type="password" name="password">
    <input type="submit">
</form>
```
在服务器端处理'/test' POST请求的代码如下：

```python
var bodyParser = require('body-parser');
// ... 
// create application/json parser
var jsonParser = bodyParser.json()
// create application/x-www-form-urlencoded parser
var urlencodedParser = bodyParser.urlencoded({ extended: false })
// ... 
// 
//  request from form of the html file
// 
app.post('/test', urlencodedParser, function(req, res) {  
    if (!req.body) return res.sendStatus(400);
    console.log('Username: ' + req.body.username);
    console.log('Password: ' + req.body.password);
    res.send('Welcome, ' + req.body.username); 
});
```

## 客户端
## 使用Node.js发送数据
在客户端使用Node.js发送Form数据的代码

```python
const http = require('http');
var data = {  
        username: 'foo',  
        password: "test"  
}; 
 
var postData = require('querystring').stringify(data);  
console.log( postData );  
function form()
{
    var options = {  
        method: "POST",  
        host: "localhost",  
        port: 80,  
        path: "/test",  
        headers: {  
            "Content-Type": 'application/x-www-form-urlencoded',  
            "Content-Length": postData.length  
        }  
    };  
  
    var body = ''; 
    var request = http.request( options, function(res) {  
        // show results  
        console.log('STATUS: ' + res.statusCode);    
        res.setEncoding('utf8');  
        res.on('data', function(chunk) {  
            body += chunk;
            console.log('BODY: ' + chunk);  
        });  
  
        res.on('end', function(err) {  
            console.log( ' complete.');  
        });  
    });  
        request.on("error", function(e) {  
            console.log('upload Error: ' + e.message);  
        })  
   
    request.write( postData );
    request.end();  
}
form();
```

## 客户端
## 使用
## jQuery
## 发送数据
客户端使用jQuery的 $.ajax post数据，

```python
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Post Data</title>
    <script src="jquery.min.js" type="text/javascript"></script>
    <script src="client.js" type="text/javascript"></script>
</head>
<body>
<!-- POST test  -->
<form action='/update' method='post' id='foo' > Parameters
<table border="0">
  <tr>
    <td> File Name</td>
    <td> <input type="text" name="filename" value = "foo.txt" /></td>
  </tr>
</table> 
</form>
<button name="button1" id='startButton' > Update</button>
</body>
</html>
```
client.js
```python
$(document).ready(function(){ 
     //try joining the server when the user clicks the connect button
     $("#startButton").click(function () {
      var filename = $("#input[name=filename]").val();
 
	$.ajax({
	type: 'POST',
        url: "/update",
        // dataType: "jsonp",
        data:  { "filename": filename}  ,        
        jsonpCallback: 'callback',  
        success: function (data) {
            // ...
        },
        error: function (xhr, status, error) {
            console.log('Error: ' + error.message);
        },
      });
 
    });
});
```
server端使用node.js解析数据
```python
//
// Modules 
var express = require('express'); 
var bodyParser = require('body-parser'); 
//
//  Global variables 
var app = express();  
// body parser
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

/* POST /update listing. */
app.post('/update', function(req, res, next) {
    // Checking require
    if (!req.body) return res.sendStatus(400);	
	
    console.log('filename: ' + req.body.filename);  
    res.redirect('./');
});
```
参考文献：
https://github.com/expressjs/body-parser



