# Docker 访问宿主服务路由配置及link实现容器间访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月22日 14:25:17[boonya](https://me.csdn.net/boonya)阅读数：544








宿主机器不能直接ping容器IP，容器本身内部如果设置的127.0.0.1亦指其容器本身，如果容器需要访问宿主，此时不能简单的使用127.0.0.1而应该使用宿主的内网IP地址或公网地址。

### 构建和运行服务和数据库容器

通过Dockerfile构建：

```bash
docker build -t rtvsweb:publish  -f RTVSWeb/Dockerfile .

docker run -it  --privileged=true --link mysql5.7:mysql5.7 -v D:\DockerWorkspace\rtvsweb:/MyData -e MyDataPath=/MyData -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700:19700 -p 19702:19702 -p 19703:19703 rtvsweb:publish
```

我本地运行的效果如下：

![](https://img-blog.csdn.net/20181022144524734?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 添加容器与主机的路由规则

172.17.0.1是默认的容器网关。

![](https://img-blog.csdn.net/20181022132355900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181022132423429?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 测试应用容器访问数据库容器

注意：mysql数据库表的大小写，忽略这个问题可能会导致数据库访问异常。

使用容器访问MySQL配置如下：

![](https://img-blog.csdn.net/20181022142314112?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

本地IP地址172.16.9.299

![](https://img-blog.csdn.net/20181022142127748?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### link容器之间连接访问

配置数据库访问源地址为--link 配置的名称：

![](https://img-blog.csdn.net/20181022144227552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

重启容器：

![](https://img-blog.csdn.net/20181022144122573?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注：容器之间访问都可以通过宿主来操作，IP端口port映射即可。



