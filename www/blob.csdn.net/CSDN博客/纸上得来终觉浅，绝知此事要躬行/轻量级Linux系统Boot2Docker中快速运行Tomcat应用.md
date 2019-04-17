# 轻量级Linux系统Boot2Docker中快速运行Tomcat应用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月05日 16:27:34[boonya](https://me.csdn.net/boonya)阅读数：351标签：[docker																[boot2docker																[tomcat																[app](https://so.csdn.net/so/search/s.do?q=app&t=blog)
个人分类：[Docker](https://blog.csdn.net/boonya/article/category/6559498)





前面说了Boot2Docker的安装，这里将验证Boot2Docker快速运行Tomcat应用（因为Tomcat运行依赖jdk所以，Tomcat的镜像已集成jdk）。需要注意Docker中运行了Tomcat请保持窗口，否则Tomcat会死掉。

## Docker简单命令



```
# 查看docker的版本信息 
$ docker version 

# 查看安装docker的信息 
$ docker info 

# 查看本机Docker中存在哪些镜像 
$ docker images 

# 检索image 
$ docker search ubuntu:14.04 

# 在docker中获取ubuntu镜像 
$ docker pull ubuntu:14.04 

# 显示一个镜像的历史 
$ docker history birdben/ubuntu:v1 

# 列出一个容器里面被改变的文件或者目 
$ docker diff birdben/ubuntu:v1 

# 从一个容器中取日志 
$ docker logs birdben/ubuntu:v1 

# 显示一个运行的容器里面的进程信息 
$ docker top birdben/ubuntu:v1 

# 从容器里面拷贝文件/目录到本地一个路径 
$ docker cp ID:/container_path to_path 

# 列出当前所有正在运行的容器 
$ docker ps 

# 列出所有的容器 
$ docker ps -a 

# 列出最近一次启动的容器 
$ docker ps -l 

# 查看容器的相关信息 
$ docker inspect $CONTAINER_ID
```

以上命令以备下面使用。



## 获取Tomcat容器

可以通过docker search [container]进行查询，如获取Tomcat：


`docker search tomcat`![](https://img-blog.csdn.net/20170705160647941)




安装Tomcat需要使用如下命令：


`docker pull tomcat`


或者直接获取Tomcat8：



`docker pull tomcat:8`
使用docker images可以查看当前下载的镜像。



## 运行Tomcat容器

运行Tomcat 需要执行以下命令：


`docker run -p 8080:8080 tomcat:8`


![](https://img-blog.csdn.net/20170705161414942)

运行成功后可以看到：

![](https://img-blog.csdn.net/20170705161818108)


## Tomcat容器应用

首先需要切换用户到root并创建app目录：



```
# 切换用户到root
sudo -i

# cd /usr/local创建app目录
mkdir app
```
注意:创建好了/usr/local/app之后docker用户并没有权限去写入，需要将/usr/local/app目录权限交给docker用户：chown docker /usr/local/app。


弄好之后我们上传一个index.html文件到/usr/local/app路径下。index.html代码：



```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
<title>Docker test VueJS</title>
</head>
<body>
   <h3> VueJS Example  </h3>
    <div id="demo">
        <div v-for="c in category">
            {{c.name}}
            <input v-for="item in c.items" v-model="item.value" />
        </div>
        <div v-for="item in items">
            <input v-model="item.value" />
        </div>
        <div v-for="item in category | get_items">
            <input v-model="item.value" />
        </div>
        {{category | json}}
    </div>
    <script src="http://cdn.bootcss.com/vue/1.0.17/vue.min.js"></script>
    <script>
        'use strict';

        new Vue({
            el: '#demo',
            data: {
                category: [
                    {
                        name: 'cate0',
                        items: [
                            {
                                value: 'cate0 item0'
                            }
                        ]
                    },
                    {
                        name: 'cate1',
                        items: [
                             {
                                 value: 'cate1 item0'
                             }
                        ]
                    }
                ]
            },
            computed: {
                items() {
                    return this.$eval('category | get_items')
                    //var arr = []
                    //this.category.forEach(c => c.items.forEach(t => arr.push(t)))
                    //return arr
                }
            },
            filters: {
                get_items: function (category) {
                    var arr = []
                    category.forEach(c => c.items.forEach(t => arr.push(t)))
                    return arr
                }
            },
        })
    </script>
</body>
</html>
```

容器内运行应用于运行Tomcat相似，只是需要加入项目路径映射到Tomcat的webapps目录下：




```
docker run --privileged=true -v /usr/local/app:/usr/local/tomcat/webapps/app -p 8080:8080 tomcat:8
```
![](https://img-blog.csdn.net/20170705162458872)
项目文件所在路径：user/local/app


项目名称：app（对应到webapps下面的app）

8080:8080前者是对外访问端口，后者是Tomcat容器端口。

此时访问app应用下的index.html,可以看到如下图所示页面：

![](https://img-blog.csdn.net/20170705164355191)

## 验证web后台应用

创建一个maven项目webtest并编写一个Hello Servlet程序。

![](https://img-blog.csdn.net/20170705171722882)

pom.xml:



```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.boonya.app</groupId>
  <artifactId>webtest</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>war</packaging>
  
  <dependencies>
    <!-- https://mvnrepository.com/artifact/javax.servlet/servlet-api -->
	<dependency>
	    <groupId>javax.servlet</groupId>
	    <artifactId>servlet-api</artifactId>
	    <version>2.5</version>
	    <scope>provided</scope>
	</dependency>
  </dependencies>
  
</project>
```

web.xml:




```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns="http://java.sun.com/xml/ns/javaee" xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd" version="3.0">
  <display-name>Archetype Created Web Application</display-name>
  <welcome-file-list>
    <welcome-file>index.html</welcome-file>
    <welcome-file>index.htm</welcome-file>
    <welcome-file>index.jsp</welcome-file>
  </welcome-file-list>
  <servlet>
    <description></description>
    <display-name>Hello</display-name>
    <servlet-name>Hello</servlet-name>
    <servlet-class>com.boonya.app.Hello</servlet-class>
  </servlet>
  <servlet-mapping>
    <servlet-name>Hello</servlet-name>
    <url-pattern>/Hello</url-pattern>
  </servlet-mapping>
</web-app>
```

Hello.java:




```java
package com.boonya.app;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class Hello
 */
public class Hello extends HttpServlet {
	private static final long serialVersionUID = 1L;

    /**
     * Default constructor. 
     */
    public Hello() {
        // TODO Auto-generated constructor stub
    }

	/**
	 * @see HttpServlet#doGet(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		response.getWriter().append("Served at: ").append(request.getContextPath());
		response.getWriter().write("<h3 style='color:red'>com.boonya.app.Hello extends HttpServlet</h3>");
	}

	/**
	 * @see HttpServlet#doPost(HttpServletRequest request, HttpServletResponse response)
	 */
	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		// TODO Auto-generated method stub
		doGet(request, response);
	}

}
```

通过maven打包run as install 或命令：mvn clean package。之后将打包好的META-INF和WEB-INF放入/usr/local/app下，并重启Tomcat容器，访问servlet得到如下图结果：


![](https://img-blog.csdn.net/20170705172246857)


上面的显示与浏览器有关，正常情况下应该显示如下：

![](https://img-blog.csdn.net/20170705173128448)


说明web应用是可以正常运行的。以下是正在运行的容器：

![](https://img-blog.csdn.net/20170705174415947)

停止容器：

![](https://img-blog.csdn.net/20170705174825172)


注意：/usr/local/app放war包需要修改后面映射路径(war需要放置于webapps下，即对应的/usr/local/app:/usr/local/tomcat/webapps下，而不是/usr/local/app:/usr/local/tomcat/webapps/app)，如果需要远程maven插件部署需要开启Tomcat的远程部署功能和账号的配置。

## 提交Tomcat容器



```
Connection established.
To escape to local shell, press 'Ctrl+Alt+]'.

                        ##         .
                  ## ## ##        ==
               ## ## ## ## ##    ===
           /"""""""""""""""""\___/ ===
      ~~~ {~~ ~~~~ ~~~ ~~~~ ~~~ ~ /  ===- ~~~
           \______ o           __/
             \    \         __/
              \____\_______/
 _                 _   ____     _            _
| |__   ___   ___ | |_|___ \ __| | ___   ___| | _____ _ __
| '_ \ / _ \ / _ \| __| __) / _` |/ _ \ / __| |/ / _ \ '__|
| |_) | (_) | (_) | |_ / __/ (_| | (_) | (__|   <  __/ |
|_.__/ \___/ \___/ \__|_____\__,_|\___/ \___|_|\_\___|_|
Boot2Docker version 17.06.0-ce, build HEAD : 0672754 - Thu Jun 29 00:06:31 UTC 2017
Docker version 17.06.0-ce, build 02c1d87
docker@default:~$ docker ps -a
CONTAINER ID        IMAGE               COMMAND             CREATED              STATUS              PORTS                    NAMES
87998f56e870        tomcat:8            "catalina.sh run"   About a minute ago   Up About a minute   0.0.0.0:8080->8080/tcp   romantic_shannon
docker@default:~$ docker commit 8799  apptomcat:1.0
sha256:249dc3f5dfb0ac37eec60581b09bd606d127f7644eda2d10c89428dd932aa75b
docker@default:~$ docker ps -a
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS                    NAMES
87998f56e870        tomcat:8            "catalina.sh run"   3 minutes ago       Up 3 minutes        0.0.0.0:8080->8080/tcp   romantic_shannon
docker@default:~$ docker images
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
apptomcat           1.0                 249dc3f5dfb0        20 seconds ago      334MB
tomcat              8                   a2fbbcebd67e        5 days ago          334MB
tomcat              latest              a2fbbcebd67e        5 days ago          334MB
```

自定义的Tomcat容器（apptomcat）就算建好了(删除imgage只需要执行docker rmi [imageid])。](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)](https://so.csdn.net/so/search/s.do?q=boot2docker&t=blog)](https://so.csdn.net/so/search/s.do?q=docker&t=blog)




