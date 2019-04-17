# Docker 镜像导出和导入 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月11日 13:17:48[boonya](https://me.csdn.net/boonya)阅读数：133








查看镜像：

```
#查看镜像 
docker images
```

导出镜像：

```
# 镜像导出 
docker save -o your_image_file.tar [image_id]
```

导入镜像：

```
#镜像导入 
docker load < your_image_file.tar
```

注：镜像的可导入导出功能使得我们实现任何平台的docker构建应用程序环境成为可能。



