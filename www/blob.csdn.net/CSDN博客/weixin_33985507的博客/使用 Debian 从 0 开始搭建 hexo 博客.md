# 使用 Debian 从 0 开始搭建 hexo 博客 - weixin_33985507的博客 - CSDN博客
2019年02月12日 22:53:14[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
Hexo 是一个快速、简洁且高效的博客框架。Hexo 使用 Markdown（或其他渲染引擎）解析文章，在几秒内，即可利用靓丽的主题生成静态网页。
Hexo 产生的静态文件只要放到任何支持 html 的空间或者服务器均可访问。主要的选择方案有以下两种
- GitHub Pages
- VPS
> GitHub Pages 本用于介绍托管在GitHub的项目，不过，由于他的空间免费稳定，用来做搭建一个博客再好不过了。
每个帐号只能有一个仓库来存放个人主页，而且仓库的名字必须是username/username.github.io，这是特殊的命名约定。你可以通过[http://username.github.io](http://username.github.io) 来访问你的个人主页。）
Github Pages 好处是完全免费，搭建并部署到教程可以参考  [Hexo+Github Pages搭建个人独立博客。](https://www.mlio.net/github-hexo.html)我个人的方案是第二种，部署到自己的服务器。
## 安装 Hexo
### 更新软件包
```
apt-get update
apt-get upgrade
```
### 安装依赖
Hexo 依赖于 Node.js 和 Git，需要先安装。
#### 安装 Git
`apt install git-core -y`
查看 git 版本
`git --version`
#### 安装 Node.js
使用以下命令安装 Node.js
`wget -qO- https://raw.github.com/creationix/nvm/master/install.sh | sh`
更新
`source ~/.profile`
安装 Node.js
`nvm install stable`
查看node.js版本
`node --version`
查看npm版本
`npm --version`
### 安装 Hexo
`npm install hexo-cli -g`
在  hexo  目录下初始化 hexo 博客，也可以是任意你想要的名字
`hexo init hexo`
进入博客根目录，并且安装相关插件依赖等
```
cd hexo
npm install
```
安装完成后需要用一下命令
```
hexo g # 渲染 Source 目录下文件问静态页面
hexo s # 本地跑一个 server 来看博客效果。
```
然后可以在 `http://localhost:4000/` 查看运行效果。
## 配置服务器环境
**服务器环境我选择使用 Debian + Nginx 环境。 **
**必须先执行 cd 把目录切换到root后才能执行下面操作 **
### 安装 Nginx
Nginx 是一个高性能的 HTTP 和反向代理服务器，同时也是一个 IMAP/POP3/SMTP 代理服务器。
执行命令安装Nginx
`apt-get install nginx`
### 启动 Nginx
Nginx 安装完成后需要手动启动
`service nginx start`
配置完成后，访问使用浏览器服务器 ip ，如果能看到以下界面，表示运行成功。
**顺便提下 Nginx 配置参数**
**start nginx**
`service nginx start`
**stop nginx**
`service nginx stop`
**other parameters**
`reload        restart       start         status        stop`
### 配置虚拟主机
虚拟主机（Virtual Host）可以在一台服务器上绑定多个域名，架设多个不同的网站，一般在开发机或者要部署多个小网站的服务器上需要配置虚拟主机。
#### 创建新的网站目录
Nginx 默认把网页文件存在 /var/www/html 目录。
在 `/var/www/html/` 目录下创建 index.html 文件。写上以下内容，用于测试虚拟主机运行情况。
```
<html>
    <head>
        <title>Welcome to Blog!</title>
    </head>
    <body>
        <h1>Hello World!</h1>
    </body>
</html>
```
#### 创建虚拟主机配置文件
在 /etc/nginx/conf.d/ 创建虚拟主机配置文件 hexo.conf，并且填写以下代码
```
server {
    listen 80;
    listen [::]:80;
    root /var/www/html;
    index index.html index.htm index.nginx-debian.html;
    server_name www.abc.com abc.com;
    location / {
    try_files $uri $uri/ =404;
    }
}
```
如果需要部署SSL，先把SSL证书和Key上传到VPS你喜欢的文件夹，并且填写以下代码
```
server {
    listen 80 default backlog=2048;
    listen 443 ssl;
    server_name abc.com;
    if ($scheme = http ) {
    return 301 https://www.$host$request_uri;
    }
    #ssl on;    #注释掉
    ssl_certificate /etc/nginx/ssl/ssl.crt;
    ssl_certificate_key /etc/nginx/ssl/ssl.key;
    ssl_session_timeout 5m;
    ssl_protocols TLSv1 TLSv1.1 TLSv1.2;
    ssl_ciphers AESGCM:ALL:!DH:!EXPORT:!RC4:+HIGH:!MEDIUM:!LOW:!aNULL:!eNULL;
    ssl_prefer_server_ciphers on;
    location / {
    root /var/www/html;     #根目录的相对位置
    index index.html index.htm;
    }
}
```
重启 Nginx 服务器，使服务器设定生效
`service nginx restart`
如果执行 `service nginx restart`时提示如下错误
```
Job for nginx.service failed because the control process exited with error code.
See "systemctl status nginx.service" and "journalctl -xe" for details.，
```
执行 `nginx -t`
如果提示下面这句错误命令
```
nginx: [emerg] a duplicate default server for 0.0.0.0:80 in /etc/nginx/sites-enabled/default:22
nginx: configuration file /etc/nginx/nginx.conf test failed
```
那么执行 `cd /etc/nginx/sites-enabled`进入`sites-enabled`文件夹
执行`rm default`删除default
再执行`nginx -t`出现下面命令
```
nginx: the configuration file /etc/nginx/nginx.conf syntax is ok
nginx: configuration file /etc/nginx/nginx.conf test is successful
```
执行下面的命令重启nginx
`service nginx restart`
查看 nginx 的状态
`service nginx status`
OK，Nginx重新加载成功，可以继续了
进入浏览器输入自己的域名，能看到以下结果就表示虚拟主机配置成功。
## 部署 Hexo 到服务器
Hexo 可以使用 git 方式部署。由于第一步已经安装完成Git，所以直接继续
### 配置服务器环境
创建空白 git 仓库，并且设置 git hook
```
cd ~
mkdir hexo.git && cd hexo.git
git init --bare
```
在 `/root/hexo.git/hooks` 创建配置文件 post-receive，并且填写以下代码
```
#!/bin/bash
GIT_REPO=/root/hexo.git  #git仓库
TMP_GIT_CLONE=/tmp/hexo
PUBLIC_WWW=/var/www/html #网站目录
rm -rf ${TMP_GIT_CLONE}
git clone $GIT_REPO $TMP_GIT_CLONE
rm -rf ${PUBLIC_WWW}/*
cp -rf ${TMP_GIT_CLONE}/* ${PUBLIC_WWW}
```
赋予脚本的执行权限
```
cd /root/hexo.git/hooks
chmod +x post-receive
```
### 配置本机环境
在博客目录下运行下面命令，安装 git 部署工具。
```
cd
cd hexo
npm install hexo-deployer-git --save
```
修改博客的配置文件 _config.yml，修改deploy选项：
```
deploy:
  type: git
  message: update
  repo: root@1.1.1.1:/root/hexo.git
  branch: master
```
然后运行 `hexo g -d` 部署本地渲染网页到服务器上。
## 错误
**如果提示下面这句错误命令**
```
INFO  Deploying: git
INFO  Setting up Git deployment...
Initialized empty Git repository in /root/hexo/.deploy_git/.git/
*** Please tell me who you are.
Run
  git config --global user.email "you@example.com"
  git config --global user.name "Your Name"
to set your account's default identity.
Omit --global to set the identity only in this repository.
fatal: unable to auto-detect email address (got 'root@debian.(none)')
FATAL Something's wrong. Maybe you can find the solution here: http://hexo.io/docs/troubleshooting.html
Error: Spawn failed
    at ChildProcess.<anonymous> (/root/hexo/node_modules/hexo-util/lib/spawn.js:52:19)
    at ChildProcess.emit (events.js:197:13)
    at Process.ChildProcess._handle.onexit (internal/child_process.js:254:12)
```
解决方案: 见提示就知道， 要您填上你得注册的邮箱和昵称，例如：
```
git config --global user.email "you@example.com" #自行更换自己的邮箱
git config --global user.name "Your Name" #自行更换自己的昵称
```
