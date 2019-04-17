# Docker Registry 本地私有化仓库搭建和使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月12日 16:12:07[boonya](https://me.csdn.net/boonya)阅读数：488








使用私有Docker仓库来实现构建镜像的保存，可以实现本地化镜像仓库的管理。一个Registry （注册表）是一个存储和内容交付系统，它的名字叫Docker镜像，可以在不同的标记版本中使用。如果您的注册中心调用是高级的，那么使用Docker组合文件来部署它可能更容易，而不是依赖于特定的Docker运行调用，即使用docker-compose.yml作为模板。


## Docker Registry搭建步骤



```
####（0）下载公共镜像到主机
docker pull registry:2

####（1）创建证书
mkdir -p certs && openssl req -newkey rsa:4096 -nodes -sha256 -keyout certs/domain.key  -x509 -days 365 -out certs/domain.crt


####（2）复制domain.crt到指定目录
cp ./certs/domain.crt /etc/docker/certs.d/localhost:5000/ca.crt


####（3）建立登录认证
mkdir auth
docker run --entrypoint htpasswd registry:2 -Bbn boonya 123456 > auth/htpasswd


###（4）重启docker
systemctl restart docker  或 service docker restart

####（5）运行仓库镜像容器
docker run -d -p 5000:5000 --restart=always --name registry -v `pwd`/auth:/auth -e "REGISTRY_AUTH=htpasswd" -e "REGISTRY_AUTH_HTPASSWD_REALM=Registry Realm"  -e REGISTRY_AUTH_HTPASSWD_PATH=/auth/htpasswd   -v `pwd`/certs:/certs   -e REGISTRY_HTTP_TLS_CERTIFICATE=/certs/domain.crt  -e REGISTRY_HTTP_TLS_KEY=/certs/domain.key registry:2
```

注意：/etc/docker/certs.d/localhost:5000需要自己创建。


## 验证私有仓库登录



```
[root@mysqlserver docker]# docker login localhost:5000
Username: boonya
Password: 
Email: boonya@sina.com
WARNING: login credentials saved in /root/.docker/config.json
Login Succeeded
[root@mysqlserver docker]# docker ps
CONTAINER ID        IMAGE               COMMAND                CREATED              STATUS              PORTS                    NAMES
d74a289cc77b        registry:2          "/entrypoint.sh /etc   About a minute ago   Up About a minute   0.0.0.0:5000->5000/tcp   registry            
[root@mysqlserver docker]#
```




## 指定镜像的tag标签

命令： docker tag tomcat:latest localhost:5000/tomcat




```
[root@mysqlserver docker]# docker images
REPOSITORY                                          TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
boonyadocker/centos-jira                            latest              b9a222f4c079        6 days ago          2.335 GB
boonyadocker/centos-microservice-discovery-eureka   latest              069f26cf4360        7 days ago          725.2 MB
boonyadocker/centos-mycat                           latest              ec847b95d08e        12 days ago         581 MB
boonyadocker/boot2docker-tomcat                     latest              31ad702ceffa        3 weeks ago         2.602 GB
boonyadocker/boot2docker-jenkins                    latest              a0090abfd7c8        3 weeks ago         2.685 GB
boonyadocker/centos-jenkins                         latest              1250a4fe0292        3 weeks ago         659 MB
boonyadocker/centos-jenkins                         allow-remote        47e38ec3f8bc        4 weeks ago         658.7 MB
boonyadocker/centos-tomcat                          allow-remote        a3cf71b7dd49        4 weeks ago         575.9 MB
tomcat                                              latest              70b2ded797df        4 weeks ago         292.4 MB
boot2docker/boot2docker                             latest              898cd2a4a97e        4 weeks ago         2.219 GB
centos                                              latest              f3b88ddaed16        5 weeks ago         192.5 MB
jenkins                                             latest              de57ef56dca0        6 weeks ago         809.9 MB
openjdk                                             8-jre               cdcc2156bae7        6 weeks ago         275.1 MB
jetbrains/teamcity-server                           latest              47728ea3474b        6 weeks ago         1.308 GB
registry                                            2                   7d1c600724ef        7 weeks ago         33.19 MB
boonyadocker/jenkins                                latest              e14944ad9023        8 weeks ago         654.2 MB
boonyadocker/tomcat-allow-remote                    latest              96d1fe00ad03        9 weeks ago         571.4 MB
java                                                8                   d11c3799fa6a        7 months ago        643.1 MB
linuxkonsult/postgres                               latest              f12477f77aea        2 years ago         1.023 GB
[root@mysqlserver docker]# docker tag tomcat:latest localhost:5000/tomcat
[root@mysqlserver docker]# docker images
REPOSITORY                                          TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
boonyadocker/centos-jira                            latest              b9a222f4c079        6 days ago          2.335 GB
boonyadocker/centos-microservice-discovery-eureka   latest              069f26cf4360        7 days ago          725.2 MB
boonyadocker/centos-mycat                           latest              ec847b95d08e        12 days ago         581 MB
boonyadocker/boot2docker-tomcat                     latest              31ad702ceffa        3 weeks ago         2.602 GB
boonyadocker/boot2docker-jenkins                    latest              a0090abfd7c8        3 weeks ago         2.685 GB
boonyadocker/centos-jenkins                         latest              1250a4fe0292        3 weeks ago         659 MB
boonyadocker/centos-jenkins                         allow-remote        47e38ec3f8bc        4 weeks ago         658.7 MB
boonyadocker/centos-tomcat                          allow-remote        a3cf71b7dd49        4 weeks ago         575.9 MB
localhost:5000/tomcat                               latest              70b2ded797df        4 weeks ago         292.4 MB
tomcat                                              latest              70b2ded797df        4 weeks ago         292.4 MB
boot2docker/boot2docker                             latest              898cd2a4a97e        4 weeks ago         2.219 GB
centos                                              latest              f3b88ddaed16        5 weeks ago         192.5 MB
jenkins                                             latest              de57ef56dca0        6 weeks ago         809.9 MB
openjdk                                             8-jre               cdcc2156bae7        6 weeks ago         275.1 MB
jetbrains/teamcity-server                           latest              47728ea3474b        6 weeks ago         1.308 GB
registry                                            2                   7d1c600724ef        7 weeks ago         33.19 MB
boonyadocker/jenkins                                latest              e14944ad9023        8 weeks ago         654.2 MB
boonyadocker/tomcat-allow-remote                    latest              96d1fe00ad03        9 weeks ago         571.4 MB
java                                                8                   d11c3799fa6a        7 months ago        643.1 MB
linuxkonsult/postgres                               latest              f12477f77aea        2 years ago         1.023 GB
[root@mysqlserver docker]#
```




## 推送镜像到私有仓库

命令： docker push localhost:5000/tomcat


```
[root@mysqlserver docker]# docker push localhost:5000/tomcat
The push refers to a repository [localhost:5000/tomcat] (len: 1)
70b2ded797df: Image already exists 
70b2ded797df: Buffering to Disk 
70bf91505e3c: Image successfully pushed 
a27d728008fe: Image successfully pushed 
a27d728008fe: Buffering to Disk 
94c806bc18ab: Image already exists 
5b823419013c: Image already exists 
993771cec505: Image already exists 
14355a73da28: Image already exists 
57f6e2f26619: Image already exists 
6cba05cd7b44: Image successfully pushed 
6cba05cd7b44: Buffering to Disk 
58bb607be243: Image successfully pushed 
58bb607be243: Buffering to Disk 
4bfb511fff05: Image already exists 
fb1c78b0c025: Image already exists 
b97cbbd11e9d: Image already exists 
6ff79eeda1a3: Image already exists 
aca8aae83159: Image successfully pushed 
aca8aae83159: Buffering to Disk 
9f2e0711a74b: Image already exists 
603b3e1e65d5: Image successfully pushed 
47eeff89a4c5: Image successfully pushed 
47eeff89a4c5: Buffering to Disk 
24c3e8da65ec: Image already exists 
3edea4741e90: Image already exists 
c006673af15c: Image already exists 
76fa688312af: Image successfully pushed 
afd646163e30: Image successfully pushed 
afd646163e30: Buffering to Disk 
52a9e823b27a: Image successfully pushed 
6cb9ca50a0b8: Image successfully pushed 
4a316b0debcb: Image successfully pushed 
4a316b0debcb: Buffering to Disk 
8ee7f64d0d9f: Image successfully pushed 
Digest: sha256:92f39e139fac8fbf8ef1e046ff75af955ebf231e871fe819261b4768b664026b
[root@mysqlserver docker]#
```



## Docker 使用docker-compose.yml模板



```
registry:
  restart: always
  image: registry:2
  ports:
    - 5000:5000
  environment:
    REGISTRY_HTTP_TLS_CERTIFICATE: /certs/domain.crt
    REGISTRY_HTTP_TLS_KEY: /certs/domain.key
    REGISTRY_AUTH: htpasswd
    REGISTRY_AUTH_HTPASSWD_PATH: /auth/htpasswd
    REGISTRY_AUTH_HTPASSWD_REALM: Registry Realm
  volumes:
    - /path/data:/var/lib/registry
    - /path/certs:/certs
    - /path/auth:/auth
```





更多使用方法请参考：

[https://docs.docker.com/registry/](https://docs.docker.com/registry/)


[https://docs.docker.com/registry/deploying/#copy-an-image-from-docker-hub-to-your-registry](https://docs.docker.com/registry/deploying/#copy-an-image-from-docker-hub-to-your-registry)





## Docker web UI镜像管理

拉取UI镜像：


`docker pull abh1nav/dockerui:latest`启动UI镜像容器：

```
docker run -d -p 9000:9000 -v /var/run/docker.sock:/docker.sock \
--name dockerui abh1nav/dockerui:latest -e="/docker.sock"
```
启动UI镜像容器建立在TCP协议之上：


```
docker run -d -p 9000:9000 --name dockerui \
abh1nav/dockerui:latest -e="http://<dockerd host ip>:4243"
```

访问浏览器UI管理界面：`http://<dockerd host ip>:9000`
![](https://img-blog.csdn.net/20170920151619333)

![](https://img-blog.csdn.net/20170920151713059)


![](https://img-blog.csdn.net/20170920151724848)

![](https://img-blog.csdn.net/20170920151744607)

![](https://img-blog.csdn.net/20170920151756488)

参考：[https://hub.docker.com/r/abh1nav/dockerui/](https://hub.docker.com/r/abh1nav/dockerui/)




