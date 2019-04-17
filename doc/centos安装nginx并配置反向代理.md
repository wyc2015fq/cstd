# centos安装nginx并配置反向代理

## 第一步 - 添加Nginx存储库

```
sudo yum install epel-release
```

## 第二步 - 安装Nginx

```
sudo yum install nginx
```

## 第三步 - 启动Nginx并防火墙放行

```
sudo systemctl start nginx
```

如果运行失败：

```
ps -ef | grep nginx
查询出来pid，然后
kill pid
```

再次运行

```
sudo systemctl start nginx
```

如果您正在运行防火墙，请运行以下命令以允许HTTP和HTTPS通信：

```
sudo firewall-cmd --permanent --zone=public --add-service=http 
sudo firewall-cmd --permanent --zone=public --add-service=https
sudo firewall-cmd --reload
```

系统启动运行nginx

```
sudo systemctl enable nginx
```

## 第四步 - 设置反向代理

进入/etc/nginx

```
vi nginx.conf
server {
　　　　listen 80;
　　　　server_name localhost;

　　　　location / {
　　　　　　proxy_pass http://localhost:3000;
　　　　　　proxy_set_header Host $host;
　　　　　　proxy_set_header X-Real-IP $remote_addr;
　　　　　　proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
　　　　　　proxy_set_header Cookie $http_cookie;
　　　　}
　　}
```

localhost:3000 更改为你所需要的IP和端口

centos可能设置了反向代理，会显示502错误，可运行以下命令

```
setsebool -P httpd_can_network_connect 1
```