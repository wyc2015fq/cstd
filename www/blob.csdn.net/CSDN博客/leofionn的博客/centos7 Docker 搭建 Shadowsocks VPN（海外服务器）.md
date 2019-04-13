
# centos7 Docker 搭建 Shadowsocks VPN（海外服务器） - leofionn的博客 - CSDN博客


2018年12月27日 18:09:38[leofionn](https://me.csdn.net/qq_36142114)阅读数：572


`## 安装 docker
yum install docker -y
## 启动 docker 服务
service docker start
chkconfig docker on
## 检查 docker 版本
docker -version``docker pull oddrationale/docker-shadowsocks`dockerfile：
\# shadowsocks
\#
\# VERSION 0.0.3
FROM ubuntu:16.04
MAINTAINER Dariel Dato-on <oddrationale@gmail.com>
RUN apt-get update && \
apt-get install -y python-pip libsodium18
RUN pip install shadowsocks==2.8.2
\# Configure container to run as an executable
ENTRYPOINT ["/usr/local/bin/ssserver"]
可以去github上搜索dockerfile或者去dockerhub上找[https://hub.docker.com/r/shadowsocks/shadowsocks-libev](https://hub.docker.com/r/shadowsocks/shadowsocks-libev)
`docker run -d -p 12345:12345 oddrationale/docker-shadowsocks -s 0.0.0.0 -p 12345 -k myvpn -m aes-256-cfb`
> -d
> 参数允许 docker 常驻后台运行

> -p
> 来指定要映射的端口，这里端口号统一保持一致即可。例如：12345

> -s
> 服务器 IP 地址，不用动

> -k
> 后面设置你的 VPN 的密码

> -m
> 指定加密方式


