# 0322-express - weixin_33985507的博客 - CSDN博客
2017年03月29日 10:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
# Express 框架核心特性：
可以设置中间件来响应 HTTP 请求。
定义了路由表用于执行不同的 HTTP 请求动作。
可以通过向模板传递参数来动态渲染 HTML 页面。
### 安装 Express
$ npm install express --save
以下几个重要的模块是需要与 express 框架一起安装的：
**body-parser**- node.js 中间件，用于处理 JSON, Raw, Text 和 URL 编码的数据。
**cookie-parser**- 这就是一个解析Cookie的工具。通过req.cookies可以取到传过来的cookie，并把它们转成对象。
**multer**- node.js 中间件，用于处理 enctype="multipart/form-data"（设置表单的MIME编码）的表单数据。
$ cnpm install body-parser --save
$ cnpm install cookie-parser --save
$ cnpm install multer --save
### 请求和响应
Express 应用使用回调函数的参数：**request**和**response**对象来处理请求和响应的数据。
app.get('\',function(req,res){
     //---
})
**Request 对象**- request 对象表示 HTTP 请求，包含了请求查询字符串，参数，内容，HTTP 头部等属性。常见属性有：
req.app：当callback为外部文件时，用req.app访问express的实例
req.baseUrl：获取路由当前安装的URL路径
***req.body / req.cookies：获得「请求主体」/ Cookies***
req.fresh / req.stale：判断请求是否还「新鲜」
req.hostname / req.ip：获取主机名和IP地址
req.originalUrl：获取原始请求URL
req.params：获取路由的parameters
req.path：获取请求路径
req.protocol：获取协议类型
req.query：获取URL的查询参数串
req.route：获取当前匹配的路由
req.subdomains：获取子域名
req.accepts()：检查可接受的请求的文档类型
req.acceptsCharsets / req.acceptsEncodings / req.acceptsLanguages：返回指定字符集的第一个可接受字符编码
req.get()：获取指定的HTTP请求头
req.is()：判断请求头Content-Type的MIME类型
**Response 对象**- response 对象表示 HTTP 响应，即在接收到请求时向客户端发送的 HTTP 响应数据。常见属性有：
res.app：同req.app一样
res.append()：追加指定HTTP头
res.set()在res.append()后将重置之前设置的头
res.cookie(name，value [，option])：设置Cookie
opition: domain / expires / httpOnly / maxAge / path / secure / signed
res.clearCookie()：清除Cookie
res.download()：传送指定路径的文件
res.get()：返回指定的HTTP头
res.json()：传送JSON响应
res.jsonp()：传送JSONP响应
res.location()：只设置响应的Location HTTP头，不设置状态码或者close response
***res.redirect()：设置响应的Location HTTP头，并且设置状态码302***
***res.send()：传送HTTP响应***
res.sendFile(path [，options] [，fn])：传送指定路径的文件 -会自动根据文件extension设定Content-Type
res.set()：设置HTTP头，传入object可以一次设置多个头
res.status()：设置HTTP状态码
res.type()：设置Content-Type的MIME类型
### 路由
我们已经了解了 HTTP 请求的基本应用，而路由决定了由谁(指定脚本)去响应客户端请求。
在HTTP请求中，我们可以通过路由提取出请求的URL以及GET/POST参数。
### 静态文件
Express 提供了内置的中间件**express.static**来设置静态文件如：图片， CSS, JavaScript 等。
你可以使用**express.static**中间件来设置静态文件路径。例如，如果你将图片， CSS, JavaScript 文件放在 public 目录下，你可以这么写：
app.use(express.static('public'));
### GET 方法
以下是在表单中通过 GET 方法提交两个参数，可以使用 server.js 文件内的**process_get**路由器来处理输入：
*app.get('/process_get',function(req,res){*
*//输出 JSON 格式*
*      response={*
*                 first_name:req.query.first_name,   //first_name为input的name属性*
*                 last_name:req.query.last_name*
*                      };*
*      console.log(response);*
*      res.end(JSON.stringify(response));*
*})*
页面提交结果
![5400831-6259eb328e46833a.png](https://upload-images.jianshu.io/upload_images/5400831-6259eb328e46833a.png)
### POST 方法
以下是在表单中通过 POST 方法提交两个参数，可以使用 server.js 文件内的**process_post**路由器来处理输入：
*//创建 application/x-www-form-urlencoded 编码解析*
*var urlencodedParser = bodyParser.urlencoded({extended:false})*
*app.post('/process_post',urlencodedParser,function(req,res){*
*     //输出 JSON 格式*
*     response={*
*                      first_name:req.body.first_name,*
*                      last_name:req.body.last_name*
*                    };*
*     console.log(response);*
*     res.end(JSON.stringify(response));*
*})*
页面提交结果
![5400831-834a3b8827d9893d.png](https://upload-images.jianshu.io/upload_images/5400831-834a3b8827d9893d.png)
### 文件上传
以下是一个用于上传文件的表单，使用 POST 方法，表单 enctype 属性设置为 multipart/form-data。
*app.post('/file_upload',function(req,res){*
*          console.log(req.files[0]);*
*         //上传的文件信息*
*          va rdes_file=__dirname+"/"+req.files[0].originalname;*
*          fs.readFile(req.files[0].path,function(err,data){*
*                          fs.writeFile(des_file,data,function(err){*
*                                     if(err){*
*                                               console.log(err);*
*                                    }else{*
*                                            response = {message:'File uploaded successfully',filename:req.files[0].originalname};}*
*                                     console.log(response);*
*                                     res.end(JSON.stringify(response));*
*                                   });*
*                      });*
*       })*
页面上传结果
![5400831-d028ba31671ec808.png](https://upload-images.jianshu.io/upload_images/5400831-d028ba31671ec808.png)
Cookie 管理
我们可以使用中间件向 Node.js 服务器发送 cookie 信息，以下代码输出了客户端发送的 cookie 信息：
*var express=require('express')*
*var cookieParser=require('cookie-parser')*
*var app=express()*
*app.use(cookieParser())*
*app.get('/',function(req,res){*
*     console.log("Cookies:",req.cookies)*
*})*
*app.listen(8081)*
![5400831-161590d07f5d9a73.png](https://upload-images.jianshu.io/upload_images/5400831-161590d07f5d9a73.png)
