
# Docker入门笔记（慕课网） - sinat_33731745的博客 - CSDN博客

2017年10月29日 15:21:16[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：907标签：[docker																](https://so.csdn.net/so/search/s.do?q=docker&t=blog)[dockerfle																](https://so.csdn.net/so/search/s.do?q=dockerfle&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=docker&t=blog)个人分类：[docker																](https://blog.csdn.net/sinat_33731745/article/category/7256766)



# 3.1 Dockerfile介绍
### 1.创建一个新文件夹，写Dockerfile
mkdir dl   \#创建文件夹
cd dl
ls
touch Dockerfile  \#touch创建文件
### 2.用vim在Dockerfile中写内容
vim Dockerfile
i \#在vim中做插入操作
FROM alpine:latest
MAINTAINER TAO
CMD echo  "Good Morning,My Angel"
按ESC退出编辑模式
输入：wq退出并保存
3.构建image
docker build -t goodmorning .
build为构建镜像
-t 为给image一个标签，为goodmoring
最后有一个.，为路径名，即把该路径下的所有文件，传给 docker engine，让它产生一个image
回车运行
![这里写图片描述](https://img-blog.csdn.net/20171029151604774?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.查看和运行image
docker images查看是否有生成刚才的image
docker run goodmorning
![这里写图片描述](https://img-blog.csdn.net/20171029152014678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3.2dockerfile实战
1.创建一个新文件夹，写Dockerfile
mkdir dockerfile2   \#创建文件夹
cd dockerfile2
ls
touch Dockerfile  \#touch创建文件
![这里写图片描述](https://img-blog.csdn.net/20171029152705789?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.用vim在Dockerfile中写内容
vim Dockerfile
i \#在vim中做插入操作
FROM ubuntu
MAINTAINER TAO
RUN apt-get update
RUN apt-get install -y nginx
COPY index.html /var/www/html
ENTRYPOINT ["/usr/sbin/nginx","-g","daemon off;"]
EXPOSE 80
基础镜像ubuntu
apt-get更新ubuntu
安装 nginx，-y安装过程中不要来提醒我
拷贝 index.html到容器路径中/var/www/html
按ESC退出编辑模式
输入：wq退出并保存
3.写html
touch index.html
vi index.html
Good Morning,My Angel
退出保存
4.构建image
docker build -t goodmorning2 .
build为构建镜像
-t 为给image一个标签，为goodmoring
最后有一个.，为路径名，即把该路径下的所有文件，传给 docker engine，让它产生一个image
回车运行
5. 运行
docker run -d -p 80:80 goodmorning2
![这里写图片描述](https://img-blog.csdn.net/20171029160722033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171029160801546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3.2.1Dockerfile语法
命令用途|
|---|---|
FROMbase image|
RUN执行命令|
ADD添加文件|
COPY拷贝文件|
CMD执行命令|
EXPOSE暴露端口|
WORKDIR指定路径|
MAINTAINER维护者|
ENV设定环境变量|
ENTRYPOINT容器入口|
USER指定用户|
VOLUMEmount point|
3.3镜像分层
![这里写图片描述](https://img-blog.csdn.net/20171029161844032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171029161859881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3MzE3NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

