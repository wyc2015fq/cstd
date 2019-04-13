
# 1.06  docker Compose多容器部署 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:33:52[尹成](https://me.csdn.net/yincheng01)阅读数：87个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**1.docker部署wordpress**
将dockerHub上的wordpress和mysql拉取到本地
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151839748.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151845944.png)
创建mysql的container，端口不需要映射，数据库是wordpress去访问
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151851256.png)
创建wordpress的container，映射到本地的8080端口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151855751.png)
访问IP：8080，可以看到wordpress的欢迎页
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151905414.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
选中文，继续
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515191254.png)
填写信息，点击安装
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151920973.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
点登录，通过两个image成功部署了wordpress
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151927758.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151933835.png)
[](https://img-blog.csdnimg.cn/20181205151927758.png)2.docker compose介绍
[](https://img-blog.csdnimg.cn/20181205151927758.png)多容器的APP很难部署和管理
docker compose就类似于批处理
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151942295.png)
docker compose是一个命令行工具，可以通过一个yml格式的文件定义多容器的docker应用
通过一条命令就可以根据yml文件的定义去创建或者管理多个容器
docer compose版本2只能用于单机，版本3可以用于多机
docker-compose.yml是默认的名字，文件中有如下三种重要的概念
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151951431.png)
Services：一个service代表一个container，这个container可以由dokerHub的image创建，也可以由本地的Dockerfile build出来的image创建
Services的启动类似于docker run，可以给其指定network和volume，所以可以给service指定network和volume的引用
3.docker compose安装和使用
下载docker compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205151958503.png)
赋权限
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152004552.png)
查看版本，显示就安装好了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152013521.png)
在某个目录创建docker-compose.yml文件，这里在/home下创建，写入内容
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152019363.png)
version: '3'
services:
  wordpress:
    image: wordpress
    ports:
      - 8080:80
    environment:
      WORDPRESS_DB_HOST: mysql
      WORDPRESS_DB_PASSWORD: admin
    networks:
      - my-bridge
  mysql:
    image: mysql:5.5
    environment:
      MYSQL_ROOT_PASSWORD: admin
      MYSQL_DATABASE: wordpress
    volumes:
      - mysql-data:/var/lib/mysql
    networks:
      - my-bridge
volumes:
  mysql-data:
networks:
  my-bridge:
    driver: bridge启动docker-compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152049549.png)
另一个窗口查看容器网络状态
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152055740.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152059768.png)
在另一个会话，查看当前docker进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152106511.png)
查看docker-compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152113656.png)
停止docker-compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152118656.png)
启动docker-compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152123609.png)
停止并删除docker-compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515212828.png)
后台运行docker-compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152134934.png)
交互运行docker-compose的方法
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152143299.png)
找到之前写的flask-redis，进入目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152148472.png)
创建docker-compose.yml，写入如下内容
version: "3"
services:
  redis:
    image: redis
  web:
    build:
      context: .
      dockerfile: Dockerfile
    ports:
      - 8080:5000
    environment:
      REDIS_HOST: redis启动docker-compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152220164.png)
启动后，浏览器访问IP：8080，可以看到如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152224900.png)
停止并删除
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152232455.png)
4.容器扩展和负载均衡
[](https://img-blog.csdnimg.cn/20181205152232455.png)启动并后台运行compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152243126.png)
查看进程，此时有一个redis和一个web
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152247875.png)
扩展web的应用部署
但是运行完会报错，8080端口占用
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152252825.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152301970.png)
停止并删除compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515232451.png)
删除docker-compose.yml中的如下两行
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152333527.png)
启动并创建3个web服务
运行中也可以扩展，实现了高效快速扩展
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515234014.png)
查看进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152344968.png)
假如有多个web，同时访问一个redis，还需要一个负载均衡器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152350429.png)
修改之前的app.py如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152359139.png)
修改之前的Dockerfile如下
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152403963.png)
修改之前的docker-compose.yml如下，最后加一段haproxy
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152409897.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
停止并删除compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152418606.png)
运行compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152425404.png)
尝试访问
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152431587.png)
动态将web扩展成3个
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120515243785.png)
查看进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152441610.png)
尝试访问
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152447799.png)
当访问高峰过去，还可以减少集群节点
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152453795.png)
查看进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152500559.png)
停止并删除compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152505446.png)
5.复杂应用部署
有两个对外的App，votingApp是对外投票用的，resultsApp会实时显示候选人得票情况
votingApp访问量大，先把投票结果写到redis缓存，java去取结果写到数据库
resultsApp从数据库获取最新的投票结果
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152512962.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
进入目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152519399.png)
启动docker-compose
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152524711.png)
浏览器访问5000和5001端口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152528995.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152533638.png)
关闭并删除
-![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205152544674.png)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

