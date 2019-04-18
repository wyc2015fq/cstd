# docker笔记 - container name 冲突 - z69183787的专栏 - CSDN博客
2019年03月13日 17:03:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：57
个人分类：[容器-Docker&K8s](https://blog.csdn.net/z69183787/article/category/8622505)
[https://blog.csdn.net/972301/article/details/80915127](https://blog.csdn.net/972301/article/details/80915127)
Docker试验时候出现错误：
[root@test3] # docker run -d --name mysql-server --network my-network -e MYSQL_ROOT_PASSWORD=secret mysql
/usr/bin/docker-current: Error response from daemon: Conflict. The container name "/mysql-server" is already in use by container bdc8d8c475cb86695c466d23fd7102221f2c040898c2d576f94cd06c93ca811b. You have to remove (or rename) that container to be able to reuse that name..
See '/usr/bin/docker-current run --help'.
直接用docker ps，没发现运行什么。
[root@test3 ~]# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
docker ps -l 查看，发现果然还在。
[root@test3 ~]# docker ps -l
CONTAINER ID        IMAGE               COMMAND                  CREATED             STATUS                     PORTS               NAMES
bdc8d8c475cb        mysql               "docker-entrypoint..."   33 minutes ago      Exited (0) 9 minutes ago                       mysql-server
因为容器已经停了，所以kill没用，需要rm。
直接docker rm
[root@test3 ~]# docker rm bdc8d8c475cb
bdc8d8c475cb
[root@test3 ~]# docker ps -l
CONTAINER ID        IMAGE               COMMAND              CREATED             STATUS              PORTS               NAMES
936a6b85f6fc        httpd               "httpd-foreground"   47 hours ago        Created                                 myhttpd
原来的mysql没了。
再试一次：
[root@dongjingang-test3 ~]# docker run -d --name mysql-server --network mynet -e MYSQL_ROOT_PASSWORD=secret mysql
a107b53edb351c191e63396664d5420350109b81152dba1e5cf22b3cab1c2d4e
可以了。
