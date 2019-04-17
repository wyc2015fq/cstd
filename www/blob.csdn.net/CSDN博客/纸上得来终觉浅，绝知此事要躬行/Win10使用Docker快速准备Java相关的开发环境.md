# Win10使用Docker快速准备Java相关的开发环境 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月05日 10:27:07[boonya](https://me.csdn.net/boonya)阅读数：761








本文目的是要准备：Mysql5.7 ，Redis ，MongoDB ，Apache，Tomcat和Nginx。Docker大大提高了环境准备的效率，不需要繁琐地去等待下载、安装按部就班地去准备了。只需要Docker就可以全部搞定了：Mysql5.7-Redis-MongoDB-Apache-Tomcat-Nginx 这些都可以快速搞定了。从我测试的情况来看，Nginx的静态配置在Windows下还没有Apache方便快捷（主要是Apache不用多余的配置）。对于有些数据和配置，我们需要建立路径映射来实现数据和配置的备份，因为容器每次启动都是一个新的环境。

### Docker 指令说明：

```
用法：docker [OPTIONS]命令

容器的自足运行时

选项：
      --config string客户端配置文件的位置（默认值
                           “C：\\ \\用户\\管理员泊坞窗”）
  -D， - debug启用调试模式
  -H， - 主机列表要连接的守护程序套接字
  -l， - log-level string设置日志记录级别
                           （ “调试” | “信息” | “警告” | “错误” | “致命”）
                           （默认“信息”）
      --tls使用TLS;由--tlsverify暗示
      --tlscacert字符串仅由此CA签名的信任证书（默认值
                           “C：\\ \\用户\\管理员泊坞窗\\ ca.pem”）
      --tlscert string TLS证书文件的路径（默认值
                           “C：\\ \\用户\\管理员泊坞窗\\ cert.pem”）
      --tlskey string TLS密钥文件的路径（默认值
                           “C：\\ \\用户\\管理员泊坞窗\\ key.pem”）
      --tlsverify使用TLS并验证远程
  -v， - version打印版本信息并退出

管理命令：
  config 管理Docker配置
  container 管理容器
  image 管理图像
  network 管理网络
  node 管理Swarm节点
  plugin 管理插件
  secret 管理Docker的秘密
  service 管理服务
  stack管理Docker堆栈
  swarm管理Swarm
  system 管理Docker
  trust管理对Docker镜像的信任
  volume 管理卷

命令：
  attach 将本地标准输入，输出和错误流附加到正在运行的容器
  build 从Dockerfile构建映像
  commit 从容器的更改创建新图像
  cp 在容器和本地文件系统之间复制文件/文件夹
  create 创建一个新容器
  diff 检查容器文件系统上的文件或目录的更改
  events 从服务器获取实时事件
  exec 在正在运行的容器中运行命令
  export 将容器的文件系统导出为tar存档
  history 显示图像的历史记录
  images 列出图像
  import从tarball导入内容以创建文件系统映像
  info显示系统范围的信息
  inspect 返回Docker对象的低级信息
  kill杀死一个或多个正在运行的容器
  load从tar存档或STDIN加载图像
  login 登录Docker注册表
  logout 从Docker注册表注销
  logs 获取容器的日志
  pause 暂停一个或多个容器中的所有进程
  port 列出端口映射或容器的特定映射
  ps 列出容器
  pull 从注册表中提取图像或存储库
  push 将映像或存储库推送到注册表
  rename 重命名容器
  restart 重新启动一个或多个容器
  rm 移除一个或多个容器
  rmi 删除一张或多张图像
  run 在新容器中运行命令
  save 将一个或多个图像保存到tar存档（默认情况下流式传输到STDOUT）
  search 在Docker Hub中搜索图像
  start 启动一个或多个已停止的容器
  stats 显示容器资源使用情况统计信息的实时流
  stop 一个或多个正在运行的容器
  tag 创建引用SOURCE_IMAGE的标记TARGET_IMAGE
  top 显示容器的运行进程
  unause 取消暂停一个或多个容器中的所有进程
  update 更新一个或多个容器的配置
  version 显示Docker版本信息
  wait 阻止，直到一个或多个容器停止，然后打印退出代码

运行'docker COMMAND --help'以获取有关命令的更多信息。
```

### Docker for Mysql5.7

```
==============本地路径映射到docker容器路径下====
# 下载 mysql
docker pull mysql:5.7

# 运行容器：
docker run -p 3306:3306 --name mysql5.7 -v D:\DockerWorkspace\mysql\conf:/etc/mysql/conf.d -v D:\DockerWorkspace\mysql\logs:/logs -v D:\DockerWorkspace\mysql\data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=root -d mysql:5.7

# 进入容器：
docker exec -it mysql5.7 bash

#登录mysql
mysql -u root -p
ALTER USER 'root'@'localhost' IDENTIFIED BY 'root';

#添加远程登录用户
CREATE USER 'boonya'@'%' IDENTIFIED WITH mysql_native_password BY 'boonya';

GRANT ALL PRIVILEGES ON *.* TO 'boonya'@'%';

# 客户端连接测试
boonya/boonya 的远程账号登录。
```

### Docker for Redis

```
=================本地路径映射到docker容器路径下====
# 下载 redis
docker pull redis

#运行容器:
docker run -p 6379:6379 -v D:\DockerWorkspace\redis\data:/data  -d redis redis-server --appendonly yes

#连接进入容器
 docker exec -it container_id redis-cli

# 进行简单的测试
127.0.0.1:6379> set boonya "boonya"
OK
127.0.0.1:6379> get boonya
"boonya"
127.0.0.1:6379>
```

### Docker for MongoDB

```
=================本地路径映射到docker容器路径下====
# 下载 mongodb
docker pull mongo

# 运行容器
docker run --name mongo -p 27017:27017  -d mongo -v D:\DockerWorkspace\mongo\db:/data/db #（此处似有bug不能映射数据目录）
     ====>若启动失败执行：
docker run --name mongo -p 27017:27017  -d mongo

# 进入容器操作

docker exec -it container_id   /bin/bash


# mongo操作创建管理账号
mongo  
 
use admin
 
db.createUser({user:"root",pwd:"root",roles:[{role:'root',db:'admin'}]})   //创建用户,此用户创建成功,则后续操作都需要用户认证
 
exit  

#测试管理账号
 mongo 127.0.0.1/admin -uroot -p

==mongodb默认不需要用户认证!
```

### Docker for Apche

```
=============本地路径映射到docker容器路径下====
# 下载apache httpd

docker pull httpd

# 启动容器

docker run -p 80:80 -v D:\DockerWorkspace\apache\Web:/usr/local/apache2/htdocs/ -v D:\DockerWorkspace\apache\logs:/usr/local/apache2/logs/ -d httpd

# 经验证隐射工作静态目录apache比Nginx要简单.可以实现静态网站的部署
```

### Docker for Tomcat

```
======================本地路径映射到docker容器路径下====
# 下载tomcat
docker pull tomcat

#启动容器

docker run --name tomcat -p 8080:8080 -v D:\DockerWorkspace\tomcat\Web:/usr/local/tomcat/webapps/test -d tomcat  

#访问http://host:8080/test进行验证,具体需不需要别的子目录可以直接在webapps这一级映射路径.
```

### Docker for Nginx

```
==============本地路径映射到docker容器路径下====
# 下载 nginx
docker pull nginx

# 运行容器：
docker run -p 90:80 --name nginx -v D:\DockerWorkspace\nginx\Web:/usr/share/nginx/html -v D:\DockerWorkspace\nginx\nginx.conf:/etc/nginx/nginx.conf -v D:\DockerWorkspace\nginx\logs:/var/log/nginx  -d nginx

#注意 本地需要nginx.conf配置

# nginx内部对应的html地址位于/usr/share/nginx/html

# nginx内部对应的配置文件地址位于/etc/nginx/nginx.conf

# 下面是宿主机上nginx.conf的配置内容

worker_processes  1;
events {
    worker_connections  128;
}
http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    keepalive_timeout  65;
 
 
	
  server {
        listen       80;
        server_name  localhost;
        location / {
            #替换下面的地址为您网页目录地址
            root   /usr/share/nginx/html;
            index  index.html index.htm;
        }
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }
}
```

注意：使用过的宿主端口号不能重复使用，但容器的端口可以是默认监听的，如80端口，在Apache和Nginx都可以是80.

### 下载DockerWorkspace目录

如果需要DockerWorkspace所有目录下载地址：[https://download.csdn.net/download/boonya/10646844](https://download.csdn.net/download/boonya/10646844)



