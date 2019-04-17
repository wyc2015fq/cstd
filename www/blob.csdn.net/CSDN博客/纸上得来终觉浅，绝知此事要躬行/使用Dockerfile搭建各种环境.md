# 使用Dockerfile搭建各种环境 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月08日 23:20:41[boonya](https://me.csdn.net/boonya)阅读数：859








安装Docker

我的操作系统：Ubuntu14.04,我们平时安装docker因为使用的是命令:

    $ sudo apt-get update

    $ sudo apt-get install -y docker.io

    $ sudo ln -sf /usr/bin/docker.io /usr/local/bin/docker

    $ sudo sed -i '$acomplete -F _docker docker' /etc/bash_completion.d/docker.io  

如果使用操作系统自带的包安装Docker,使用的是比较旧的安装版本，要使用最新的docker推荐如下方式：

（1）使用阿里云容器提供的下载：

https://cr.console.aliyun.com/?spm=5176.100239.blogcont29941.12.DMQU50#/accelerator

（2）执行：curl -sSL http://acs-public-mirror.oss-cn-hangzhou.aliyuncs.com/docker-engine/internet | sh -

（3）这里阿里云还提供了Docker加速器，可以看到是修改配置文件，修改镜像地址，这里的镜像地址经过试验很慢，也是国外的，所以这里我们不使用他提供的加速器。我们使用：https://www.daocloud.io/mirror#

（4）执行：curl -sSL https://get.daocloud.io/daotools/set_mirror.sh | sh -s http://aa09d22e.m.daocloud.io

经过测试这个镜像加速器还是很快的。

卸载docker，但是会保留镜像：

sudo apt-get purge docker.io

Dockerfile搭建tomcat服务

1.创建一个目录构建环境

mkdir test/web

2.上传tomcat和jdk到该目录下

    apache-tomcat-7.0.63.tar.gz  

    jdk-7u76-linux-x64.tar.gz  

3.编辑Dockerfile:vim Dockerfile

    #pull down centos image

    FROM centos

    MAINTAINER wanglei_dev@cyou-inc.com


    #添加tomcat和jdk到镜像中

    ADD ./apache-tomcat-7.0.63.tar.gz /root

    ADD ./jdk-7u76-linux-x64.tar.gz /root


    #设置环境变量

    ENV JAVA_HOME /root/jdk1.7.0_76

    ENV PATH $JAVA_HOME/bin:$PATH


    #定义在容器启动之后的运行程序

    ENTRYPOINT /root/apache-tomcat-7.0.63/bin/startup.sh && tail -F /root/apache-tomcat-7.0.63/logs/catalina.out  

4.构建镜像

    sudo docker build -t myCentos:tomcat-centos --rm=true .

    此时屏幕输出如下：

    Sending build context to Docker daemon 470.4 MB

    Sending build context to Docker daemon

    Step 0 : FROM centos

     ---> d83a55af4e75

    Step 1 : MAINTAINER test@test.com

     ---> Running in 955747d64da5

     ---> 1619dc8f6d58

    ................

    70/logs/catalina.out

     ---> Running in fe48acf12d70

     ---> 52076383f11b

    Removing intermediate container fe48acf12d70

    Successfully built 52076383f11b  

注意：-t 选择指定生成镜像的用户名和标签 --rm=true 指定在生成镜像过程中删除中间产生的临时容器

5.查看新产生的镜像

    root@ubuntu:/# docker images

    REPOSITORY          TAG                 IMAGE ID            CREATED             VIRTUAL SIZE

    nginx_dockerfile    centos              110a02c542ee        2 hours ago         357.9 MB

    mycentos            tomcat-centos       50a61817c5a4        3 hours ago         502.4 MB

    ubuntu              latest              bd92ca350bbd        14 hours ago        122 MB

    centos              latest              1c1b67b33c28        3 weeks ago         196.6 MB  

6.运行镜像

    [root@wls12c web]$ docker run -d -p 8090:8080 50a6

    8260fa22aa32126f613a6b64f121e78545ceae01da58c61396968bfafeda3706  

注意：-p指定主机端口8090和容器端口8080绑定，即 -p 主机端口：容器端口 -d指定容器运行后与当前tty分离，后台运行

最终验证：通过访问：http://宿主机IP:8090,即可看见我们熟悉的tomcat首页了

Dockerfile搭建nginx服务(互联tomcat容器)

1.创建目录构建nginx环境

    mkdir -p /root/nginx_centos

    cd /root/nginx_centos

2.准备run.sh文件，用于启动ssh和apache服务脚本

    #!/bin/bash

    /usr/sbin/sshd &

    /usr/local/nginx/sbin/nginx

注意：这个.sh脚本，最好以#!/bin/bash，我试过写了一下备注，一直报错

3.编写Dockerfile文件

    #基于centos生成镜像

    FROM centos


    #镜像创建者信息

    MAINTAINER wanglei_dev@cyou-inc.com


    #切换目录

    WORKDIR /usr/local/src

    #安装wget

    RUN yum install -y wget


    #下载并解压源码包

    RUN wget http://nginx.org/download/nginx-1.8.0.tar.gz

    RUN tar -zxvf nginx-1.8.0.tar.gz

    WORKDIR nginx-1.8.0


    #编译安装nginx

    RUN yum install -y gcc make pcre-devel zlib-devel

    RUN ./configure   --prefix=/usr/local/nginx   --with-pcre

    RUN make

    RUN make install


    #启动nginx服务

    RUN /usr/local/nginx/sbin/nginx


    #修改nginx配置文件，以非daemon方式启动

    RUN echo "daemon off;" >>/usr/local/nginx/conf/nginx.conf


    #复制服务器启动脚本并设置权限

    ADD run.sh /usr/local/sbin/run.sh

    RUN chmod 755 /usr/local/sbin/run.sh


    #设置生成容器时需要执行的脚本

    CMD ["/usr/local/sbin/run.sh"]

    #开放22,80,443端口

    EXPOSE 22

    EXPOSE 80

    EXPOSE 443

注意：在Dockerfile文件中更换当前目录不使用cd,要用WORKDIR

4.根据Dockerfile生成镜像

docker build -t nginx_dockerfile:centos .

5.查看镜像

    root@ubuntu:/# docker images

    REPOSITORY          TAG                 IMAGE ID            CREATED             VIRTUAL SIZE

    nginx_dockerfile    centos              110a02c542ee        2 hours ago         357.9 MB  

6.根据镜像生成容器并测试

docker run -d --name myNginxDocker -p 2224:22 -p 8001:80 -p 4443:443 --link myDocker:tomcatDoker nginx_dockerfile:centos  

将容器的22端口、80端口和443端口分别映射到到宿主机上的2224端口、8001端口和4443端口 查看新生成的容器：docker ps -a

测试：

curl localhost:8001

同时我们可以主机上访问会看到我们熟悉的nginx的欢迎界面

7.验证两个容器互通

登录docker容器：

docker exec -it myNginxDocker /bin/bash  

ping 我们刚才--link后面跟的myDocker:tomcatDoker，这里是容器名：别名，我们可以直接在nginx的容器中：

ping tomcatDoker

Docker Compose

1.简介

Compose

2.安装

    wanglei@ubuntu:~$ sudo curl -L https://github.com/docker/compose/releases/download/1.16.1/docker-compose-`uname -s`-`uname -m` -o /usr/local/bin/docker-compose

      % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current

                                     Dload  Upload   Total   Spent    Left  Speed

    100   617    0   617    0     0    522      0 --:--:--  0:00:01 --:--:--   523

      0     0    0     0    0     0      0      0 --:--:--  0:00:21 --:--:--     0curl: (7) Failed to connect to github-production-release-asset-2e65be.s3.amazonaws.com port 443: Connection refused   

我们可以看到由于网络原因，连接失败，那么我们可以自行下载相关版本移动到相关目录

使用echo命令输出下载地址：

    wanglei@ubuntu:~$ echo "https://github.com/docker/compose/releases/download/1.16.1/docker-compose-`uname -s`-`uname -m`"

    https://github.com/docker/compose/releases/download/1.16.1/docker-compose-Linux-x86_64

将下载的文件导入，移动到相关目录下，并修改权限：

    wanglei@ubuntu:~$ sudo mv docker-compose-Linux-x86_64 /usr/local/bin/docker-compose  

    wanglei@ubuntu:/usr/local/bin$ sudo chmod +x /usr/local/bin/docker-compose  

Docker构建，运行，发布一个springBoot应用

1.开放API（可选）

因为我们在开发阶段，要不停的构建发布项目，那么我们的测试机器就也要不停地发布构建，这样很麻烦，所以这里我们开放一个docker的API接口，通过这个接口连接到我们的Maven，这样就省去了很多构建的时间。

ubuntu下我们执行如下：

    vim /etc/default/docker

    DOCKER_OPTS="-H unix:///var/run/docker.sock -H 0.0.0.0:4243"  

centos下我们执行如下：

    vim /etc/sysconfig/docker

    other_args="-H tcp://0.0.0.0:5555 -H unix:///var/run/docker.sock"

注意：0.0.0.0表示对所有IP都开放相应的端口

2.windows下设置DOCKER_HOST:

环境变量设置如下：

DOCKER_HOST=tcp://192.168.254.132:4243  

验证docker的端口是否已经开放：telnet ip 端口

3.创建springboot项目-pom文件设置插件

    <?xml version="1.0" encoding="UTF-8"?>

    <project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"

    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">

        <modelVersion>4.0.0</modelVersion>


        <groupId>com.example</groupId>

        <artifactId>helloworld</artifactId>

        <version>0.0.1-SNAPSHOT</version>

        <packaging>jar</packaging>


        <name>helloworld</name>

        <description>Demo project for Spring Boot</description>


        <parent>

            <groupId>org.springframework.boot</groupId>

            <artifactId>spring-boot-starter-parent</artifactId>

            <version>1.5.7.RELEASE</version>

            <relativePath/> <!-- lookup parent from repository -->

        </parent>


        <properties>

            <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>

            <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>

            <java.version>1.8</java.version>

            <!--docker镜像前缀-->

            <docker.image.prefix>helloworld</docker.image.prefix>

        </properties>


        <dependencies>

            <dependency>

                <groupId>org.springframework.boot</groupId>

                <artifactId>spring-boot-starter-web</artifactId>

            </dependency>


            <dependency>

                <groupId>org.springframework.boot</groupId>

                <artifactId>spring-boot-starter-test</artifactId>

                <scope>test</scope>

            </dependency>

        </dependencies>


        <build>

            <finalName>${project.name}</finalName>

            <plugins>

                <plugin>

                    <groupId>org.springframework.boot</groupId>

                    <artifactId>spring-boot-maven-plugin</artifactId>

                    <configuration>

                        <finalName>${project.name}</finalName>

                        <fork>true</fork>

                    </configuration>

                </plugin>

                <!--docker插件,docker-maven-plugi用于构建maven的Docker image-->

                <plugin>

                    <groupId>com.spotify</groupId>

                    <artifactId>docker-maven-plugin</artifactId>

                    <version>0.4.3</version>

                    <configuration>

                        <!--指定镜像名称-->

                        <imageName>${docker.image.prefix}/${project.artifactId}</imageName>

                        <!--指定Dockerfile的位置-->

                        <dockerDirectory>src/main/docker</dockerDirectory>

                        <!--在构建镜像的时候使用到的文件需要和Dockerfile放在一起，一般应用jar包需要纳入-->

                        <resources>

                            <resource>

                                <targetPath>/</targetPath>

                                <directory>${project.build.directory}</directory>

                                <include>${project.build.finalName}.jar</include>

                            </resource>

                        </resources>

                    </configuration>

                </plugin>

            </plugins>

        </build>

    </project>

4.创建springboot项目-程序主入口

    @SpringBootApplication

    @RestController

    public class HelloworldApplication {


        public static void main(String[] args) {

            SpringApplication.run(HelloworldApplication.class, args);

        }


        @RequestMapping("/")

        public String home(){

            return "Hello Docker World";

        }

    }

5.运行程序确保程序没有问题

编译：mvn package

运行：java -jar target/docker-spring-boot-1.0.0.jar

访问项目： http://localhost:8080/

6.将项目容器化

Docker 使用 Dockerfile 文件格式来指定 image 层，编写Dockerfile,创建文件 src/main/docker/Dockerfile:

    #基于那个镜像

    FROM anapsix/alpine-java

    #FROM frolvlad/alpine-oraclejdk8:slim


    #指定临时文件目录为/tmp

    VOLUME /tmp


    #拷贝文件到目录中

    ADD helloworld-0.0.1-SNAPSHOT.jar app.jar

    RUN bash -c 'touch /app.jar'


    #开放端口

    EXPOSE 8080

    #环境变量（BOOTAPP_OPTS用于传入Java运行时的变量，例如test,dev）

    ENV BOOTAPP_OPTS ""


    #配置容器启动命令

    # docker run -d -p 8080:8080 -e "spring.profiles.active=dev" --name sample helloworld/helloworld

    # docker run -d -p 8080:8080 -e "SPRING_PROFILES_ACTIVE=dev" --name sample helloworld/helloworld

    ENTRYPOINT java ${BOOTAPP_OPTS} -Djava.security.egd=file:/dev/./urandom -jar app.jar

7.构建Docker Image



运行docker Image:

docker run -p 8082:8080 -t springbootDocker

验证成功：访问宿主（主机IP）我们可以看到springboot的输出

---------------------

作者：jlnu_wanglei

来源：CSDN

原文：https://blog.csdn.net/jlnu_wanglei/article/details/78265365

版权声明：本文为博主原创文章，转载请附上博文链接！



