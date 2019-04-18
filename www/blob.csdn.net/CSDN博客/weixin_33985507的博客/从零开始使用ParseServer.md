# 从零开始使用ParseServer - weixin_33985507的博客 - CSDN博客
2017年06月22日 17:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
## 安装
假设我们有一台干净的Linux机器, 这里我们使用ubuntu.
- 需要确认一下npm已经安装
```
apt-get install npm
```
切换源
```
npm config set registry http://registry.cnpmjs.org
```
注意有时候我们会遇到
```
/usr/bin/env: ‘node’: No such file or directory
```
这个问题是因为Ubuntu下node的运行命令是nodejs, 所以我们需要创建一个软连接
```
ln -s /usr/bin/nodejs /usr/bin/node
```
- 
参考 [https://github.com/parse-community/parse-server](https://github.com/parse-community/parse-server)
需要使用npm安装相应的模块.
- 
安装mongodb
```
apt-get install mongodb
```
测试Mongodb
```
lsof -iTCP:27017 -sTCP:LISTEN
```
- 创建config文件, 我们新建一个文件config.json:
```
{
  "appId": "appid1",
  "masterKey": "mk1",
  "appName": "test1",
  "databaseURI": "mongodb://127.0.0.1:27017/parse"
}
```
- 运行如下命令:
```
parse-server config.json
```
可以得到:
```
Configuration loaded from /root/parse/config.json
appId: appid1
masterKey: ***REDACTED***
port: 1337
host: 0.0.0.0
databaseURI: mongodb://127.0.0.1:27017/parse
cloud: ./cloud/main
mountPath: /parse
appName: test1
maxUploadSize: 20mb
userSensitiveFields: ["email"]
serverURL: http://localhost:1337/parse
[10705] parse-server running on http://localhost:1337/parse
```
- 测试
```
curl -X POST -H "X-Parse-Application-Id:appid1" \
-H "Content-Type: application/json" \
-d '{"score":1337,"playerName":"Sean Plott","cheatMode":false}' \
http://127.0.0.1:1337/parse/classes/GameScore
```
## **Dashboard**
参考 [https://github.com/parse-community/parse-dashboard](https://github.com/parse-community/parse-dashboard)
- 创建config文件dashboard-config.json
```
{
  "apps": [
    {
      "serverURL": "http://localhost:1337/parse",
      "appId": "myAppId",
      "masterKey": "myMasterKey",
      "appName": "MyApp"
    }
  ]
}
```
- 运行
```
parse-dashboard --config dashboard-config.json
```
## **Https**和**Nginx**
需要安装nginx
```
apt-get install nginx
```
```
server { 
 listen 443 ssl default_server；
 listen [::]:443 ssl default_server;
 ###其他配置 ssl on;  
 ssl_certificate      /var/www/ssl/server.crt; 
 ssl_certificate_key  /var/www/ssl/server_nopwd.key; 
 location /parse {   
    proxy_pass http://127.0.0.1:1337;         
    proxy_set_header Host $host;  
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;    
  }
…
```
- 再次测试, 注意将ip替换成自己部署的服务
```
curl -k -X POST -H "X-Parse-Application-Id:appid1”
-H "Content-Type: application/json" \
-d '{"score":1337,"playerName":"Sean Plott","cheatMode":false}' \
https://<ip>/parse/classes/GameScore
```
### Dashboard支持https
如果已经将parse server部署到了一个https服务上，dashboard的配置中的链接要更改为相应的链接
同时，dashboard本身需要配置trustProxy参数和users
## 使用Supervisor守护
[Supervisor](http://supervisord.org/)是一个Python写的工具, 用来帮助我们管理多个进程.
首先使用Supervisor的命令创建一个配置文件,
```
echo_supervisord_conf > my_supervisord.conf
```
修改这个配置文件, 具体可以参考官网的说明, 只需要加入其中的三行就可以了.
```
[program:parseserver]
command=parse-server config.json
process_name="ParseServer"
```
注意:如果需要使用web/socket进行远程管理, 还需要配置一下其他项目.
运行
```
supervisord -c supervisord.conf
```
## 移动端使用
- 集成Client SDK
我使用的是iOS的客户端, 所以参考了[http://docs.parseplatform.org/ios/guide/](http://docs.parseplatform.org/ios/guide/)
使用了Carthage, 在执行
```
carthage update
```
后, 需要拖入两个库:
Parse.framework和Bolts.framework
