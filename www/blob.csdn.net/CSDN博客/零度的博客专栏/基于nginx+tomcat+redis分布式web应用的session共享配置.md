# 基于nginx+tomcat+redis分布式web应用的session共享配置 - 零度的博客专栏 - CSDN博客
2016年10月28日 14:38:54[零度anngle](https://me.csdn.net/zmx729618)阅读数：661
###  一、前言
       nginx 作为目前最流行的开源反向代理HTTP Server，用于实现资源缓存、web server负载均衡等功能，由于其轻量级、高性能、高可靠等特点在互联网项目中有着非常普遍的应用，相关概念网上有丰富的介绍。分布式web server集群部署后需要实现session共享，针对 tomcat 服务器的实现方案多种多样，比如 tomcat cluster session 广播、nginx IP hash策略、nginx sticky
 module等方案，本文主要介绍了使用 redis 服务器进行 session  统一存储管理的共享方案。
相关应用结构参照下图：
![](http://www.ahlinux.com/uploadfile/2015/0207/20150207105948321.png)
## 二、环境配置
测试环境基于 Linux CentOS 6.5，请先安装 tomcat、redis、nginx 相关环境，不作详细描述，本文测试配置如下：
|name|Version|IP_Port|
|----|----|----|
|nginx|1.6.2|10.129.221.70:80|
|tomcat_1|7.0.54|10.129.221.70:8080|
|tomcat_2|7.0.54|10.129.221.70:9090|
|redis|2.8.19|10.129.221.70:6379|
## 三、构建 tomcat-redis-session-manager-master
1、由于源码构建基于 gradle，请先配置 gradle 环境。
2、从 github 获取 tomcat-redis-session-manager-master 源码，地址如下：
```java
https://github.com/jcoleman/tomcat-redis-session-manager
```
3、找到源码中的 **build.gradle** 文件，由于作者使用了第三方仓库（sonatype），需要注册帐号，太麻烦，注释后直接使用maven中央仓库，同时注释签名相关脚本并增加依赖包的输出脚本 copyJars（dist目录），修改后的**build.gradle  文件如下：**
```java
apply plugin: 'java'
apply plugin: 'maven'
apply plugin: 'signing'
group = 'com.orangefunction'
version = '2.0.0'
repositories {
  mavenCentral()
}
compileJava {
  sourceCompatibility = 1.7
  targetCompatibility = 1.7
}
dependencies {
  compile group: 'org.apache.tomcat', name: 'tomcat-catalina', version: '7.0.27'
  compile group: 'redis.clients', name: 'jedis', version: '2.5.2'
  compile group: 'org.apache.commons', name: 'commons-pool2', version: '2.2'
  //compile group: 'commons-codec', name: 'commons-codec', version: '1.9'
  testCompile group: 'junit', name: 'junit', version: '4.+'
  testCompile 'org.hamcrest:hamcrest-core:1.3'
  testCompile 'org.hamcrest:hamcrest-library:1.3'
  testCompile 'org.mockito:mockito-all:1.9.5'
  testCompile group: 'org.apache.tomcat', name: 'tomcat-coyote', version: '7.0.27'
}
task javadocJar(type: Jar, dependsOn: javadoc) {
  classifier = 'javadoc'
  from 'build/docs/javadoc'
}
task sourcesJar(type: Jar) {
  from sourceSets.main.allSource
  classifier = 'sources'
}
artifacts {
  archives jar
  archives javadocJar
  archives sourcesJar
}
//signing {
//  sign configurations.archives
//}
task copyJars(type: Copy) {
  from configurations.runtime
  into 'dist'  
}
uploadArchives {
  repositories {
    mavenDeployer {
      beforeDeployment { MavenDeployment deployment -> signing.signPom(deployment) }
      //repository(url: "https://oss.sonatype.org/service/local/staging/deploy/maven2/") {
      //  authentication(userName: sonatypeUsername, password: sonatypePassword)
      //}
      //repository(url: "https://oss.sonatype.org/content/repositories/snapshots") {
      //  authentication(userName: sonatypeUsername, password: sonatypePassword)
      //}
      pom.project {
        name 'tomcat-redis-session-manager'
        packaging 'jar'
        description 'Tomcat Redis Session Manager is a Tomcat extension to store sessions in Redis'
        url 'https://github.com/jcoleman/tomcat-redis-session-manager'
        issueManagement {
          url 'https://github.com:jcoleman/tomcat-redis-session-manager/issues'
          system 'GitHub Issues'
        }
        scm {
          url 'https://github.com:jcoleman/tomcat-redis-session-manager'
          connection 'scm:git:git://github.com/jcoleman/tomcat-redis-session-manager.git'
          developerConnection 'scm:git:git@github.com:jcoleman/tomcat-redis-session-manager.git'
        }
        licenses {
          license {
            name 'MIT'
            url 'http://opensource.org/licenses/MIT'
            distribution 'repo'
          }
        }
        developers {
          developer {
            id 'jcoleman'
            name 'James Coleman'
            email 'jtc331@gmail.com'
            url 'https://github.com/jcoleman'
          }
        }
      }
    }
  }
}
```
4、执行gradle命令构建源码，编译输出tomcat-redis-session-manager-master 及依赖jar包
```java
gradle build -x test  copyJars
```
所有输出列表文件如下：
![](http://www.ahlinux.com/uploadfile/2015/0207/20150207105948720.png)
## 四、tomcat 配置
安装配置两台 tomcat web服务器，分别修改 Connector 端口号为8080和9090，并确保都能正常工作，当然如果分布在不同的主机则可以使用相同端口号。
## 五、编写测试页面
为了区别2台tomcat的访问，分别编写页面并打包部署：
1、为tomcat_1编写测试页面，显示 “ response from tomcat_1 ”，同时页面提供按钮显示当前session值，打包并发布到 tomcat_1 服务器；
2、为tomcat_2编写测试页面，显示 “ response from tomcat_2 ”，同时页面提供按钮显示当前session值，打包并发布到 tomcat_2 服务器；
此时分别访问 http://10.129.221.70:8080 和 http://10.129.221.70:9090 地址，因为访问的是不同web服务器，所以各自显示不同的页面内容及session值肯定不同。
## 六、tomcat session manager 配置
修改配置使用 tomcat-redis-session-manager-master 作为 tomcat session 管理器
1、分别将第三步生成的 tomcat-redis-session-manager-master 及依赖jar包覆盖到 tomcat 安装目录的 lib 文件夹
2、分别修改2台 tomcat 的 context.xml 文件，使 tomcat-redis-session-manager-master 作为session管理器，同时指定redis地址和端口。context.xml 增加以下配置：
```java
<Context>
   <Valve className="com.orangefunction.tomcat.redissessions.RedisSessionHandlerValve" />
   <Manager className="com.orangefunction.tomcat.redissessions.RedisSessionManager"
     host="localhost"
     port="6379"
     database="0"
     maxInactiveInterval="60" />
</Context>
```
3、分别重启2台 tomcat 服务器。
## 七、nginx 配置
1、修改 default.conf 配置文件，启用 upstream 负载均衡 tomcat Cluster，默认使用轮询方式。
```java
upstream site {  
  server localhost:8080; 
  server localhost:9090; 
}  
server {
  listen	   80;
  server_name  localhost;
  #charset koi8-r;
  #access_log  /var/log/nginx/log/host.access.log  main;
  location / {
    #root   /usr/share/nginx/html;
    #index  index.html index.htm; 
    index  index_tel.jsp index.jsp index.html index.htm ;  
    proxy_redirect		  off;	
    proxy_set_header		Host			$host;	
    proxy_set_header		X-Real-IP	   $remote_addr;	
    proxy_set_header		X-Forwarded-For $proxy_add_x_forwarded_for;	
    client_max_body_size	10m;	
    client_body_buffer_size 128k;	
    proxy_buffers		   32 4k;  
    proxy_connect_timeout   3;	
    proxy_send_timeout	  30;	
    proxy_read_timeout	  30;   
      proxy_pass http://site; 
  }
  #error_page  404			  /404.html;
  # redirect server error pages to the static page /50x.html
  #
  error_page   500 502 503 504  /50x.html;
  location = /50x.html {
    root   /usr/share/nginx/html;
  }
  # proxy the PHP scripts to Apache listening on 127.0.0.1:80
  #
  #location ~ \.php$ {
  #	proxy_pass   http://127.0.0.1;
  #}
  # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
  #
  #location ~ \.php$ {
  #	root		   html;
  #	fastcgi_pass   127.0.0.1:9000;
  #	fastcgi_index  index.php;
  #	fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
  #	include		fastcgi_params;
  #}
  # deny access to .htaccess files, if Apache's document root
  # concurs with nginx's one
  #
  #location ~ /\.ht {
  #	deny  all;
  #}
}
```
2、nginx 重新加载配置
```java
nginx -s reload
```
`<span style="font-family:Arial;">nginx -s re<span class="operator" style="margin: 0px; padding: 0px;">load</span></span>`
## 八、测试结果
1、访问 [http://10.129.221.70:8080](http://10.129.221.70:8080/) 直接请求到tomcat_1服务器，
显示 “ response from tomcat_1 ”， session 值为 ‘56E2FAE376A47F1C0961D722326B8423’；
2、访问 [http://10.129.221.70:9090](http://10.129.221.70:8080/) 直接请求到tomcat_2服务器，
显示 “ response from tomcat_2 ”， session 值为 ‘56E2FAE376A47F1C0961D722326B8423’；
3、访问 [http://10.129.221.70](http://10.129.221.70:8080/) （默认80端口）请求到 [nginx](http://www.ahlinux.com/nginx/) 反向代理到指定Web服务器，由于默认使用轮询负载方式，
反复刷新页面显示的内容在“ response from [tomcat](http://www.ahlinux.com/tomcat/)_1
 ” 和 “ response from tomcat_2 ”之间切换，但 session  值保持为 ‘56E2FAE376A47F1C0961D722326B8423’；
4、使用 redis-cli 连接 redis [服务器](http://www.ahlinux.com/server/)，查看会显示有
 “56E2FAE376A47F1C0961D722326B8423” key的 session  数据，value为序列化数据。
**![](http://www.ahlinux.com/uploadfile/2015/0207/20150207105949701.png)**
       至此，实现了基于nginx负载均衡下tomcat [集群](http://www.ahlinux.com/cluster/)的session
 一致性。
