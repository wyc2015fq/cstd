# nodeJs + vue.js 小案例 - z69183787的专栏 - CSDN博客
2018年08月30日 10:10:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：479
[https://www.cnblogs.com/zl-127/p/6543973.html](https://www.cnblogs.com/zl-127/p/6543973.html)
配置好 node js环境
创建app.js文件，代码如下
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
var http = require("http");
var fs = require('fs');
var url = require('url');
http.createServer(function(request, response) {
    // 解析请求，包括文件名
    var pathname = url.parse(request.url).pathname;
    // 输出请求的文件名
    console.log("Request for " + pathname + " received.");
    // 从文件系统中读取请求的文件内容
    fs.readFile(pathname.substr(1), function(err, data) {
        if (err) {
            console.log(err);
            // HTTP 状态码: 404 : NOT FOUND
            // Content Type: text/plain
            response.writeHead(404, { 'Content-Type': 'text/html' });
        } else {
            // HTTP 状态码: 200 : OK
            // Content Type: text/plain
            response.writeHead(200, { 'Content-Type': 'text/html' });
            // 响应文件内容
            response.write(data.toString());
        }
        //  发送响应数据
        response.end();
    });
}).listen(8888);
console.log('Server running at http://127.0.0.1:8888/index.html');
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
下载vue.js文件放在 js目录下，在根目录创建index.html文件，项目文件结构如下
![](https://images2015.cnblogs.com/blog/732194/201703/732194-20170313172010635-958359542.png)
index.html页面代码如下
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title></title>
</head>
<body>
    <!--这是我们的View-->
    <div id="app">
        <p>{{ message }}</p>
        <input type="text" v-model="message" />
        <h1>Hello, Vue.js!</h1>
        <h1 v-if="yes">Yes!</h1>
        <h1 v-if="no">No!</h1>
        <h1 v-if="age >= 25">Age: {{ age }}</h1>
        <h1 v-if="name.indexOf('jack') >= 0">Name: {{ name }}</h1>
    </div>
</body>
<script src="js/vue.js"></script>
<script>
    // 这是我们的Model
    var exampleData = {
        message: 'Hello World!',
        yes: true,
        no: false,
        age: 28,
        name: 'keepfooljack'
    }
    // 创建一个 Vue 实例或 "ViewModel"
    // 它连接 View 与 Model
    new Vue({
        el: '#app',
        data: exampleData
    })
</script>
</html>
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
打开终端，cd到项目目录下，执行 node app.js
在浏览器打开    http://127.0.0.1:8888/index.html，可以看到如下结果
![](https://images2015.cnblogs.com/blog/732194/201703/732194-20170313172049276-1022516521.png)
