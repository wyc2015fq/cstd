# Nginx 环境 轻松部署 PHP  Codeigniter框架 - weixin_33985507的博客 - CSDN博客
2017年03月21日 15:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
首先确定你的 nginx 能正常运行  查询 nginx 的运行状态 如下：
![1536293-e624350967657f46.png](https://upload-images.jianshu.io/upload_images/1536293-e624350967657f46.png)
这样的 说明 你的 nginx是在 运行中
然后 找到 nginx 的安装目录 如果 我的 安装目录 在：/etc/nginx  
进入 目录 有以下文件：
![1536293-095f19d16b8d76ce.png](https://upload-images.jianshu.io/upload_images/1536293-095f19d16b8d76ce.png)
conf.d 目录 是你所有项目的 配置文件 所在目录 目录如下：
![1536293-2c02d6d57153f161.png](https://upload-images.jianshu.io/upload_images/1536293-2c02d6d57153f161.png)
我现在 使用的 项目配置 文件 就两个  guoqiang.conf 和 junyunuo.conf  其他的都是 备份文件  其中 junyunuo.conf 配置的是 CI框架   另一个 是 php原生
两个 文件分别 对应以下项目  访问路径为：
http://guoqiang.wangcc.net/
http://junyunuo.wangcc.net/
现在 我们可以 看看 junyunuo.conf 的配置信息：
![1536293-90f6cb353fec261d.png](https://upload-images.jianshu.io/upload_images/1536293-90f6cb353fec261d.png)
配置 的内容 很简单  基本上 就只需要 更改 server_name  和  fastcgi_param SCRIPT_FILENAME 的对应路径 就可以了
配置 完成之后 重启 nginx 就行了  sudo nginx -s reload
server {
listen 80;
server_name junyunuo.wangcc.net;#你的项目域名
root /webservice/www/www.onLine/CheckWorkSystem;#你的项目路径
index index.php;#域名默认 访问的php文件
location / {
try_files $uri $uri/ /index.php;
}
location /index.php{
fastcgi_pass127.0.0.1:9000;
fastcgi_param SCRIPT_FILENAME /webservice/www/www.onLine/CheckWorkSystem/index.php;
fastcgi_param PATH_INFO $fastcgi_path_info;
fastcgi_split_path_info ^(.+\.php)(.*)$;
fastcgi_param PATH_TRANSLATED $document_root$fastcgi_path_info;
includefastcgi.conf;
}
}
