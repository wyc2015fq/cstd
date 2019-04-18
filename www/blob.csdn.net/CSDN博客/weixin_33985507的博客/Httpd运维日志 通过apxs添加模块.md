# Httpd运维日志:通过apxs添加模块 - weixin_33985507的博客 - CSDN博客
2015年08月26日 23:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
**Brief　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　**
  在部署Httpd时为方便管理和安全等原因，我们仅会安装所需的模块，那么后期功能扩展时则需要通过Httpd内置提供的apxs程序来进行模块添加。
  而apxs程序则位于apache/bin目录下。
**Premise　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　**
  首先我们的平台必须支持DSO特性，而且Httpd必须已经内建了mod_so模块。
** DSO(Dynamic Shared Object，动态共享对象)**
  　　是一种动态连接/加载的机制，从而可以在运行时将编译成特殊格式的代码加载到可执行程序的地址空间。然后通过mod_so提供的LoadModule指令在运行时加载到Httpd服务器中。
**mod_so模块**
  　　作用：运行时加载DSO模块（Httpd进程启动时可加载DSO模块来扩展功能）
        后缀：.so或.dll
  检查是否已内建mod_so模块： `> httpd -l | grep mod_so`
**Install module by apxs　　　　　　　　　　　　　　　　　　　　　　**
  apxs的全称为APache eXtenSion作为Httpd服务器编译和安装扩展模块的工具，用于编译一个或多个程序或目标代码文件为动态共享对象。
`> apxs -i -a -c <file>`
  <file>可以是C源码文件(.c)、地址独立的目标代码文件(.o)甚至是一个库(.a)。
  Httpd默认支持的模块C源码文件位于apache/modules/metadata/目录下，如mod_headers.c (若要配置请求头、响应头的信息必须通过它)
`> /app/apache/bin/apxs -i -a -c /app/apache/modules/metadata/mod_headers.c`   执行后在/app/apache/modules/下多了一个mod_headers.so文件
  然后我们再在/app/apache/conf/httpd.conf中添加
```
LoadModule headers_module /modules/mod_headers.so
<IfModule headers_module>
    Header add Access-Control-Allow-Origin *
    Header add Access-Control-Allow-Method "POST,GET,OPTIONS"
</IfModule>
```
**Thanks　　　　　　　　　　　　　　　　　　　　　　　　　　　　**
http://man.chinaunix.net/newsoft/ApacheMenual_CN_2.2new/dso.html
http://blog.51yip.com/apachenginx/871.html
http://man.chinaunix.net/newsoft/ApacheMenual_CN_2.2new/programs/apxs.html
