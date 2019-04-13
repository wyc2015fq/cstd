
# 腾讯云上安装kafka，远程无法访问 - leofionn的博客 - CSDN博客


2019年03月14日 15:44:01[leofionn](https://me.csdn.net/qq_36142114)阅读数：30


腾讯云上安装kafka，远程总是无法访问，本地机器可以。
host.name=内网地址      \#kafka绑定的interface
advertised.listeners=PLAINTEXT://外网映射地址:9092    \# 注册到zookeeper的地址和端口


