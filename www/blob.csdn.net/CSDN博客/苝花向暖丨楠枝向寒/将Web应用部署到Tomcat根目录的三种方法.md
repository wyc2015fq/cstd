# 将Web应用部署到Tomcat根目录的三种方法 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月29日 13:52:39[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：43


## 将应用部署到Tomcat根目录的三种方法

> 
将应用部署到Tomcat根目录的目的是可以通过“[http://[ip]:[port]](https://blog.csdn.net/)”直接访问应用，而不是使用“[http://[ip]:[port]/[appName]](https://blog.csdn.net/%5BappName%5D)”上下文路径进行访问。

## 方法一：（最简单直接的方法）

> 
**删除**原 webapps/ROOT 目录下的所有文件，将应用下的所有文件和文件夹复制到ROOT文件夹下。

## 方法二：

> 
**删除**原webapps/ROOT 目录下的所有文件，修改文件“conf/server.xml”，在Host节点下增加如下Context的内容配置：

- 
`<Host name="localhost" appBase="webapps" unpackWARs="true" autoDeploy="true"`

- 
`xmlValidation="false" xmlNamespaceAware="false">`

- 
`......`

- 
`<Context path="" docBase="C:/apache-tomcat-6.0.32/myapps/bc.war"></Context>`

- 
`</Host>`

- 

**注意：**

> 
1）path 的值设置为空； 

2）应用不要放到tomcat的webapps目录下(如上述配置是放到自定义的文件夹myapps内的)，否则访问时路径会有问题； 

3）docBase指定到绝对路径。

如配置此设置后重启tomcat，如果docBase指向的是war文件，则会自动将war解压到 webapps/ROOT 目录；如果docBase指向的是应用已解压好的目录，如 docBase=”C:/apache-tomcat-6.0.32/myapps/bc” ，则tomcat不会生成 webapps/ROOT 目录（这种情况下，这样就可以不用删除 webapps/ROOT 目录，但 webapps/ROOT 目录内的内容是无法访问的），访问时将直接使用docBase指定的目录。

## 方法三：

> 
与方法二类似，但不是修改全局配置文件“conf/server.xml，而是在“conf/Catalina/localhost”目录下增加新的文件”ROOT.xml”（注意大小写），文件内容如下：

- 
`<?xml version="1.0" encoding="UTF-8"?>`

- 
`<Context path="" docBase="C:/apache-tomcat-6.0.32/myapps/bc.war" reloadable="true">`

- 
`</Context>`

- 

**注意：**

> 
Tomcat5.0以下版本在d:/Tomcat/conf/Catalina/localhost目录下会自动生成了一个ROOT.xml文件，但是5.0以上版本已不再生成此文件。

