# Apache HTTP配置反向代理入门 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2017年05月12日 15:03:32[gloomyfish](https://me.csdn.net/jia20003)阅读数：9274








### Apache HTTP配置反向代理入门

反向代理（Reverse Proxy）方式是指以代理服务器来接受internet上的连接请求，然后将请求转发给内部网络上的服务器或者外部网络上其它IP地址服务器，并将从服务器上得到的结果返回给internet上请求连接的客户端，此时代理服务器对外就表现为一个服务器。

### 配置反向代理

客户端通过Internet请求HTTP页面，当请求到达Apache代理网关服务器， 代理服务器根据URL请求地址，转发到对应的服务器上，获取内容返回给代理服务器，从而返回给客户端。 
![这里写图片描述](https://img-blog.csdn.net/20170512145914563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Apache支持反向代理配置，只需要如下几步， 
### 第一步 - 下载与安装

下载apache-http服务器，我下载的是这个httpd-2.2.25-win32-x86-openssl-0.9.8y.msi， 我也看到很多人下载了源码，然后自己编译，本人C++水平有限，所以还是选择了直接下载installer， 双击开始安装，需要填写域名与服务器名称，如果有就真实填写，如果没有均填写localhost，邮件地址随便写一个，安装结束之后，打开浏览器，输入安装时候填写的域名，结果显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20170512150008587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
说明apache htt安装成功。
第二步:配置代理映射 

到安装好的apache文件目录conf文件下，找到httpd.conf文件

找到如下配置，去掉#可以启动HTTP反向代理功能 

LoadModule proxy_module modules/mod_proxy.so 

LoadModule proxy_http_module modules/mod_proxy_http.so
在文件最后加上如下一段 
![这里写图片描述](https://img-blog.csdn.net/20170512150112620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我是下载了Tomcat， 启动Tomcat，然后通过apache访问Tomcat/webapps下面自带的examples，配置的代理映射如上。重启apache之后，访问jsp样例显示在浏览器如下： 
![这里写图片描述](https://img-blog.csdn.net/20170512150137346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果还有其它的地址需要映射，直接按行在添加在之间即可








