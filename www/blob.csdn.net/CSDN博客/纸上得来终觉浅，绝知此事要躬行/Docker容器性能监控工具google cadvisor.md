# Docker容器性能监控工具google/cadvisor - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月30日 10:05:39[boonya](https://me.csdn.net/boonya)阅读数：322








原文地址：[https://hub.docker.com/r/google/cadvisor/](https://hub.docker.com/r/google/cadvisor/)

![](https://img-blog.csdnimg.cn/2018103009554823.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

cAdvisor（Container Advisor）为容器用户提供了对其运行容器的资源使用和性能特征的理解。 它是一个运行守护程序，用于收集，聚合，处理和导出有关正在运行的容器的信息。 具体而言，对于每个容器，它保留资源隔离参数，历史资源使用和完整历史资源使用的直方图。 此数据按容器和机器范围导出。

[https://github.com/google/cadvisor](https://github.com/google/cadvisor)



官方cAdvisor版本基于Linux构建，并通过临时图像导出，这保证了镜像大小更小。

 Dockefile: [https://github.com/google/cadvisor/blob/master/deploy/Dockerfile](https://github.com/google/cadvisor/blob/master/deploy/Dockerfile)

```bash
FROM alpine:3.7
MAINTAINER dengnan@google.com vmarmol@google.com vishnuk@google.com jimmidyson@gmail.com stclair@google.com

ENV GLIBC_VERSION "2.28-r0"

RUN apk --no-cache add ca-certificates wget device-mapper findutils && \
    apk --no-cache add zfs --repository http://dl-3.alpinelinux.org/alpine/edge/main/ && \
    apk --no-cache add thin-provisioning-tools --repository http://dl-3.alpinelinux.org/alpine/edge/main/ && \
    wget -q -O /etc/apk/keys/sgerrand.rsa.pub https://alpine-pkgs.sgerrand.com/sgerrand.rsa.pub && \
    wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/${GLIBC_VERSION}/glibc-${GLIBC_VERSION}.apk && \
    wget https://github.com/sgerrand/alpine-pkg-glibc/releases/download/${GLIBC_VERSION}/glibc-bin-${GLIBC_VERSION}.apk && \
    apk add glibc-${GLIBC_VERSION}.apk glibc-bin-${GLIBC_VERSION}.apk && \
    /usr/glibc-compat/sbin/ldconfig /lib /usr/glibc-compat/lib && \
    echo 'hosts: files mdns4_minimal [NOTFOUND=return] dns mdns4' >> /etc/nsswitch.conf && \
    rm -rf /var/cache/apk/*

# Grab cadvisor from the staging directory.
ADD cadvisor /usr/bin/cadvisor

EXPOSE 8080
ENTRYPOINT ["/usr/bin/cadvisor", "-logtostderr"]
```

注：在linux系统下才能很好的运行不支持windows.

每个cAdvisor版本都有标记。 我们还有2个用于测试和稳定音轨的标签：
-      latest：最新稳定版本，这是最新官方支持的版本。
-      canary：定期从HEAD构建的图像。 潜在的不稳定！

 我们还有一个自动构建的cAdvisor版本的cAdvisor，它是从HEAD不断构建的。 这可以在google / cadvisor-canary镜像中找到。 由于其大小和易变性，不建议用于生产。

**补充快速操作：**

```
# 获取镜像
docker pull google/cadvisor

# 运行容器
docker run --volume=/:/rootfs:ro --volume=/var/run:/var/run:rw --volume=/sys:/sys:ro --volume=/var/lib/docker/:/var/lib/docker:ro --publish=8080:8080 --detach=true --privileged=true --name=cadvisor --restart=always google/cadvisor:latest

# 运行容器简写方式
docker run -v /:/rootfs:ro -v /var/run:/var/run:rw -v /sys:/sys:ro -v /var/lib/docker/:/var/lib/docker:ro -p 8080:8080 --detach=true --privileged=true --name=cadvisor --restart=always google/cadvisor:latest

# 查看容器日志
docker logs -f cadvisor

# 访问说明
#curl http://host:8080
```





