# node中的模块系统 - weixin_33985507的博客 - CSDN博客
2018年12月17日 09:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
#### 1.http:搭建后台服务器
```
1.引入http模板
const http = require('http');
2.搭建服务
var server = http.createServer(function(request,response){
  res.write();//响应的内容
  res.end();//响应结束
  req.url();//请求的路径
})
3.指定端口号
server.listen(8080)
```
#### fs 文件操作模块
##### 读取文件
```
const fs = require('fs');
fs.readFile(文件名,funtion(err,data){//err错误 data数据
  if(err){
    console.log('错误')
  }else{
    console.log(data.toString())//转化为字符串
  }
})
```
##### 写文件
```
var fs = require('fs');
fs.writeFile('文件名','内容',function(err){
  console.log(err)//如果没有报错报null；若有错则报错
})
```
#### fs模板结合http模板请求不同文件
```
const http = require('http');
const fs = require('fs');
var server = http.createServer(function(req,res){
  var file_name = './www'+req.url;
  fs.readFile(file_name,function(err,data){
    if(err){
        console.log(err)
    }else{
        res.write(data)
    }
    res.end()
  })
})
server.listen(8080);
```
