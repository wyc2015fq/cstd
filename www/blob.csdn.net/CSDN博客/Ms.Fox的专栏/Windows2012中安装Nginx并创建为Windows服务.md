# Windows2012中安装Nginx并创建为Windows服务 - Ms.Fox的专栏 - CSDN博客
2019年02月19日 10:42:00[lulu-lu](https://me.csdn.net/smbluesky)阅读数：23
## 安装Nginx
下载windows版nginx （http://nginx.org/download/nginx-1.10.0.zip），之后解压到需要放置的位置（D:\xampp\nginx）
![](https://images2015.cnblogs.com/blog/582266/201605/582266-20160518202406248-863802489.png)
## 将Nginx设置为Windows服务
需要借助"Windows Service Wrapper"小工具，项目地址： [https://github.com/kohsuke/winsw](https://github.com/kohsuke/winsw)
下载地址： [http://repo.jenkins-ci.org/releases/com/sun/winsw/winsw/1.18/winsw-1.18-bin.exe](http://repo.jenkins-ci.org/releases/com/sun/winsw/winsw/1.18/winsw-1.18-bin.exe)
下载该工具后，将其放在 Nginx安装目录下，并重命名为nginx-service.exe，创建配置文件nginx-service.xml（名字要和工具名一样），
创建nginx-service.exe.config（为支持NET 4.0 runtime，默认只支持NET 2.0 runtime）
**文件结构如下：**
![](https://images2015.cnblogs.com/blog/582266/201605/582266-20160518205139404-131502706.png)
nginx-service.xml 内容如下：
<service>
  <id>nginx</id>
  <name>Nginx Service</name>
  <description>High Performance Nginx Service</description>
  <logpath>D:\xampp\nginx\logs</logpath>
  <log mode="roll-by-size">
    <sizeThreshold>10240</sizeThreshold>
    <keepFiles>8</keepFiles>
  </log>
  <executable>D:\xampp\nginx\nginx.exe</executable>
  <startarguments>-p D:\xampp\nginx</startarguments>
  <stopexecutable>D:\xampp\nginx\nginx.exe</stopexecutable>
  <stoparguments>-p D:\xampp\nginx -s stop</stoparguments>
</service>
nginx-service.exe.config 内容如下：
<configuration>
  <startup>
    <supportedRuntime version="v2.0.50727" />
    <supportedRuntime version="v4.0" />
  </startup>
  <runtime>
    <generatePublisherEvidence enabled="false"/> 
  </runtime>
</configuration>
在cmd中运行如下命令安装windows服务
D:\xampp\nginx\nginx-service.exe install
之后就可以在Windows服务面板中启动服务了
![](https://images2015.cnblogs.com/blog/582266/201605/582266-20160518210626669-569821058.png)
浏览器中就可以正常访问了
![](https://images2015.cnblogs.com/blog/582266/201605/582266-20160518210944341-508049914.png)
