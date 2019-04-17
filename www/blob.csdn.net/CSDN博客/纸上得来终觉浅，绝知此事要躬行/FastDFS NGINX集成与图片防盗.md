# FastDFS NGINX集成与图片防盗 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月18日 21:57:12[boonya](https://me.csdn.net/boonya)阅读数：2721








文章来自：

[http://www.javacoder.cn/?p=885](http://www.javacoder.cn/?p=885)

[http://www.javacoder.cn/?p=888](http://www.javacoder.cn/?p=888)


FastDFS 提供了fastdfs-nginx-module模块来支持通过NGINX访问FastDFS中存储的数据，据我所知，fastdfs-nginx-module模块只能通过源码安装到nginx,而不能将其编译成动态模块加载到已经安装好的nginx中

步骤如下：

## **源码编译**

下载:[https://github.com/happyfish100/fastdfs-nginx-module-master.zip](https://github.com/happyfish100/fastdfs-nginx-module-master.zip)

解压: unzip fastdfs-nginx-module-master



```
wget http://nginx.org/download/nginx-1.10.2.tar.gz
tar -xvf nginx-1.10.2.tar.gz
cd nginx-1.10.2
./configure --add-module=/root/fastdfs-nginx-module-master/src --without-http_rewrite_module --without-http_gzip_module --conf-path=/etc/nginx/nginx.conf --pid-path=/etc/nginx/nginx.pid --lock-path=/etc/nginx/nginx.lock
make
make install
```

## **配置修改**

/etc/nginx/nginx.conf中添加如下映射



```
location /M00 {
# root /var/fastdfs/data;
ngx_fastdfs_module;
}
```

将fastdfs-nginx-module-master/src/mod_fastdfs.conf以及fastdfs-5.08/conf/http.conf和fastdfs-5.08/conf/mime.types拷贝到/etc/fdfs目录修改mod_fastdfs.conf文主要改如下项：

base_path：数据的默认存放路径

log_filename日志的存放路径,注意log_filename配置的路径要对nginx用户可读写，而不是当前用户

## **重启nginx**


`/usr/local/nginx/sbin/nginx -s reload`
## **测试**

在浏览器中请求如下路径

192.168.14.153/M00/01/74/wKgOmVh_dH6AE-f3AAA2_mDXnps704.jpg

操作成功

后续操作：

一个nginx只能访问一个storage服务器的数据，所以多个storage服务器要配置多个nginx，然后将nginx按照请求路径中的组id(groupid)进行路由。

## FastDFS 防盗图开启

修改/etc/fdfs/http.conf



```
#开启token校验
http.anti_steal.check_token=true
#设置校验失败后显示的警告图片
http.anti_steal.token_check_fail=/etc/fdfs/anti-steal.jpg
```

java客户端带有一个工具方法来产生token.

用法如下：



```
String configFileName = "src/main/resources/fdfs_client.conf";
ClientGlobal.init(configFileName);
String remote_filename = "M00/01/74/wKgOmVh_dH6AE-f3AAA2_mDXnps704.jpg";
//以秒为单位
int ts = (int)(System.currentTimeMillis()/1000);
ts = ts + 100;
String secret_key = "FastDFS1234567890";
String token = ProtoCommon.getToken(remote_filename, ts, secret_key);
System.out.println("ts:" + ts + ", token:" + token);
```


输出为：ts:1484735390, token:ada4c7f1a65e125e3a55a837d0bff1eb

那么请求的url为:192.168.14.153/M00/01/74/wKgOmVh_dH6AE-f3AAA2_mDXnps704.jpg?ts=1484735390&token=ada4c7f1a65e125e3a55a837d0bff1eb



注意：由于是时间戳比较，确保nginx服务器的时区和时间配置正确。




