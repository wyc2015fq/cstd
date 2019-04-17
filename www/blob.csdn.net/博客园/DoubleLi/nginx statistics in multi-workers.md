# nginx statistics in multi-workers - DoubleLi - 博客园






1.      从网上下载多进程统计补丁。https://github.com/arut/nginx-patches

2.      Copy补丁文件per-worker-listener到nginx代码目录下。如\nginx-1.7.2

3.      打补丁：patch -p1 < per-worker-listener

4.      make &make install

5.      修改配置文件：

daemon on;
master_process on;
worker_processes 5;



events{
    worker_connections 1024;
    accept_mutex off;
}



#HTTP
http{

   upstream redis_pool {
        server localhost:6379;
        keepalive 1024 single;
    }

    server {
        listen 8080;
        listen 10000 per_worker;//第一个worker从10000开始

    }

}

6.     运行Nginx，并测试：

[http://localhost:10000/stat](http://localhost:10000/stat)

[http://localhost:10001/stat](http://localhost:10000/stat)

[http://localhost:10002/stat](http://localhost:10000/stat)

[http://localhost:10003/stat](http://localhost:10000/stat)

[http://localhost:10004/stat](http://localhost:10000/stat)











