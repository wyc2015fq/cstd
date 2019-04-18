# nginx 更改配置client_max_body_size  nginx.conf 修改默认限制上传附件大小 - z69183787的专栏 - CSDN博客
2018年10月16日 11:25:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：342
**Nginx 上传大文件超时解决办法**
情况如下：用nginx作代理服务器，上传大文件时（本人测试上传50m的文件），提示上传超时或文件过大。
原因是nginx对上传文件大小有限制，而且默认是1M。另外，若上传文件很大，还要适当调整上传超时时间。
解决方法是在nginx的配置文件下，加上以下配置：
[?](https://www.jb51.net/article/119444.htm#)
|123456|`client_max_body_size     50m; //文件大小限制，默认1m``client_header_timeout    1m; ``client_body_timeout      1m; ``proxy_connect_timeout     60s; ``proxy_read_timeout      1m; ``proxy_send_timeout      1m;`|
每个参数的意思：
**client_max_body_size**
限制请求体的大小，若超过所设定的大小，返回413错误。
**client_header_timeout**
读取请求头的超时时间，若超过所设定的大小，返回408错误。
**client_body_timeout**
读取请求实体的超时时间，若超过所设定的大小，返回413错误。
**proxy_connect_timeout**
http请求无法立即被容器(tomcat, netty等)处理，被放在nginx的待处理池中等待被处理。此参数为等待的最长时间，默认为60秒，官方推荐最长不要超过75秒。
**proxy_read_timeout**
http请求被容器(tomcat, netty等)处理后，nginx会等待处理结果，也就是容器返回的response。此参数即为服务器响应时间，默认60秒。
**proxy_send_timeout**
http请求被服务器处理完后，把数据传返回给Nginx的用时，默认60秒。
**------------------------------------------------------------------------------------------------------------------------------------------------------------------------**
nginx.conf
在nginx使用过程中，上传文件的过程中，通常需要设置nginx报文大小限制。避免出现413 Request Entity Too Large。
于是奇葩的问题被我们遇到了，详细配置请参考下面。我们的问题是，无论**client_max_body_size设置在哪里，nginx －s reload后，依然一直报413.多次尝试reload，始终无效。最终决定kill 进程，restart，终于好了。**
**由此可见，nginx reload并不一定好使。有时候，为了保险起见。restart比较靠谱。不知道别人有没有遇到同样的问题。希望对大家有帮助！～～**
设置如下：
Context:`http`, `server`, `location`
Sets the maximum allowed size of the client request body, specified in the “Content-Length” request header field.
If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client.
Please be aware that browsers cannot correctly display this error.
Setting `*size*` to 0 disables checking of client request body size.
可以选择在http{ }中设置：client_max_body_size   20m;
 也可以选择在server{ }中设置：client_max_body_size   20m;
还可以选择在location{ }中设置：client_max_body_size   20m;
三者到区别是：http{} 中控制着所有nginx收到的请求。而报文大小限制设置在server｛｝中，则控制该server收到的请求报文大小，同理，如果配置在location中，则报文大小限制，只对匹配了location 路由规则的请求生效。
     http{
#控制全局nginx所有请求报文大小
#client_max_body_size   20m;
                server{
#控制该server的所有请求报文大小
#client_max_body_size   20m;
                        location a {
                        }
                        location b{
#控制满足该路由规则的请求报文大小
#client_max_body_size   20m;
                        }
                }
                server {
                }
     }
