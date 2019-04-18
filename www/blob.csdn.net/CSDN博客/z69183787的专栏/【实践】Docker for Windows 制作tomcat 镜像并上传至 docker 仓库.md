# 【实践】Docker for Windows 制作tomcat 镜像并上传至 docker 仓库 - z69183787的专栏 - CSDN博客
2019年02月02日 11:43:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：124
### **准备工序**
1、官网下载Docker windows 版
截止到2019年2月2，链接是 [https://hub.docker.com/editions/community/docker-ce-desktop-windows](https://hub.docker.com/editions/community/docker-ce-desktop-windows)，exe文件，双击安装即可
2、注册一个docker账户
要上传你自己的docker容器，首先需要有自己的docker账户。 
https://hub.docker.com/ ，注册完后登录，效果如下，感觉和github差不多
![](https://img-blog.csdnimg.cn/20190202110541772.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
3、安装第一步下载的docker for windows后启动，会发现多了一个鲸鱼的小图标：
有很多相关的功能，比如登录，查看仓库，还有和k8s的配合操作等等 ，switch to Windows container可以在linux和windows环境内进行切换，有一些功能可能只在linux上可以使用。
![](https://img-blog.csdnimg.cn/20190202110718649.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
### **实践操作**
1、启动 cmd，输入**docker **，可以看到，已经安装完毕了
![](https://img-blog.csdnimg.cn/20190202111353184.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
2、使用 **docker search tomcat** 查找 所有镜像中 和tomcat 相关的 ，
选择stars最高的 执行 **docker pull tomcat**  拉取tomcat镜像到本地
![](https://img-blog.csdnimg.cn/20190202111551463.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
3、此时，使用 **docker images **命令可以查看当前docker内拥有的所有镜像，可以看到之前下载的tomcat
![](https://img-blog.csdnimg.cn/20190202111702995.png)
4、启动tomcat，看看能否看到正常安装的tomcat的启动页面，执行命令
**docker run -it --name dockerDemo -p 8080:8080 tomcat **，
这里我们启动一个交互式的容器，名字是**dockerDemo **，-p 8080:8080表示将容器的8080端口映射到主机的8080端口，这样我们只要访问主机的8080端口就可以访问到容器的服务了。（主机端口：容器端口）
出现熟悉的tomcat命令行启动界面，此时输入localhost:8080发现tomcat 已经正常启动
![](https://img-blog.csdnimg.cn/20190202112256983.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190202112442201.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
5、此时启动一个新的cmd，我们来查看下 此时docker容器的当前进程与 实际 tomcat所处的目录结构
**docker ps  **可以看到当前docker中启动的所有容器，及对应的容器信息，此处我们要关注的是containerid 容器ID，之后的操作都需要基于这个关键的ID
![](https://img-blog.csdnimg.cn/2019020211263555.png)
可以看到我们之前启动的tomcat 和对应的 容器ID，红框内显示了上文说的 主机8080端口映射到容器内的8080。
**docker exec -it 20dae1362bc7 /bin/bash** 可以进入容器内部，查看具体容器在机器上的目录及位置，为之后上传war包做准备
可以看到，进入目录后在/usr/local/tomcat ；底层目录和我们正常安装的tomcat并无两样，再次输入 **exit **可以退出容器
![](https://img-blog.csdnimg.cn/20190202113600336.png)
6、接下来就是最关键的如何将本地的war包或者项目置入容器内的tomcat中，docker也提供了相关命令
注：war包大家可以自行准备，可以简单的跑起来的demo即可
**docker cp E:\zone\dockerdemo\target\docker-demo.war 20dae1362bc7:/usr/local/tomcat/webapps**
简单介绍一下docker cp 复制命令，docker cp 源地址（本机war包目录地址） 目标地址（容器ID:容器内部地址）
执行完毕后，可以看到tomcat目录下 已经有了
![](https://img-blog.csdnimg.cn/20190202114107580.png)
此时我们打开 [http://localhost:8080/docker-demo/index.jsp](http://localhost:8080/docker-demo/index.jsp) 发现 ，页面已经可以访问了
![](https://img-blog.csdnimg.cn/20190202114156825.png)
大功告成！
7、至此，简单的tomcat在docker内的部署流程已经完成了，但是会有一个问题，每次容器重启后，之前容器内部放置的文件都会消失，每次重新执行run命令启动容器，相当于一个新的黑盒，无法保留历史文件，所以就引申出了一个新的问题，需要打包成一个images进行持续性管理！
8、在6的基础上，执行以下命令
首先要提交修改，输入指令（xxxxx是容器ID，zh69183787/demo，‘/’前面是自己的docker账号名，后面是给容器取的名字），每次commit后都会生成新的镜像，拥有独立的imageId
**docker commit 20dae1362bc7 zh69183787/test **
![](https://img-blog.csdnimg.cn/20190202121011325.png)
默认生成的镜像tag是latest，之后是none，如果想使用自己的tag的话，可以在 zh69183787/test:v2，加上tag
此时，输入 **docker images **就会发现，已经生成了一个新的镜像images，tag就是commit时选填项，v2
![](https://img-blog.csdnimg.cn/20190202125832324.png)
之后就可以基于这个镜像进行文件的管理了。
9、最后还可以通过docker hub  将自己的镜像传入远程docker 仓库中，和github类似的一套机制
**docker login**  登录 docker 账户，我这边由于docker 鲸鱼小图标启动后自己进行了登录，所以就直接生效了
![](https://img-blog.csdnimg.cn/20190202124316774.png)
将仓库上传至镜像 **docker push zh69183787/test:latest **（latest是标签，docker images后TAG一栏就是了）
![](https://img-blog.csdnimg.cn/201902021245023.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
最后看一眼 docker hub
![](https://img-blog.csdnimg.cn/2019020212452322.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
如果之前commit了不同镜像并且有不同的tag，点击某一个images后会发现tag下有N个提交版本
![](https://img-blog.csdnimg.cn/20190202125931611.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==,size_16,color_FFFFFF,t_70)
有了 ok，之后可以通过** docker pull zh69183787/test，**拉取远程仓库的 images 了，感觉 commit 和 tag版本 都和git异曲同工，可以自行了解一下细节
![](https://img-blog.csdnimg.cn/20190202124643790.png)
参考：
[新手超详细记录:Docker for Windows打包制作tomcat镜像并部署java web项目](https://blog.csdn.net/ivyusing/article/details/79153017)
[Docker的Windows容器初体验](https://yq.aliyun.com/articles/62375)
[在Docker中利用Tomcat快速部署web应用的方法示例](https://www.jb51.net/article/155005.htm)
