# Nginx http模块(学习笔记二十一) - weixin_33985507的博客 - CSDN博客
2018年05月17日 17:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：19
1.HTTP的核心模块.
这些HTTP模块会在编译Nginx时自动编译进来,除非使用configure命令禁止编译这些模块.
(1)alias指令.
该指令用于在URL和文件系统路径之间实现映射.它与root指令类似,但是网页文件的root根目录不会改变,改变的只是请求URL的文件系统路径.
语法:alias file-path| directory-path ;
例: location /i/{alias /spool/w3/images/;}
在示例中,访问URL地址"/i/top.gif",会返回文件"/spool/w3/images/top.gif"
(2)client_body_in_file_only 指令.
语法:client_body_in_file_only on|off
该指令允许将一个客户端的请求内容记录到一个文件中,该文件在请求完成后不会被删除.在内置perl中,该指令可以用于调试$r->request_body_file 方法.
(3)client_body_in_single_buffer 指令
语法:client_body_in_single_buffer
该指令指定是否保持整个内容在一个单一的客户端请求缓冲区中,该指令在使用变量$request_body时被推荐使用.
(4)client_body_buffer_size指令
语法:client_body_buffer_size the_size
例 client_body_buffer_size 128k
该指令指定客户端请求内容的缓冲区大小.如果客户端请求内容大于缓冲区,整个请求内容或部分请求内容将被写人临时文件.缓冲区默认大小相当于网页大小的两倍,为8k 或16k.
(5)client_body_temp_path指令
语法: client_body_temp_path dir-path [level1[level2[level3]
该指令用于指定存放请求内容临时文件的目录.缓存目录最多支持3层子目录.
例: client_body_temp_path /spool/nginx/client_temp 1 2 ;
以上示例的目录结构类似:
/spool/nginx/client_temp/7/45/XXXXX
(6)client_body_timeout指令
语法:client_body_timeout time
该指令用于设置读取客户端请求内容的超时时间.如果超过该指令设置的时间,Nginx将返回"Request time out"错误信息(HTTP状态码为408)
(7)client_header_buffer_size指令
语法:client_header_buffer_size size
该指令用于设置客户端请求的Header头缓冲区大小.对绝大多数请求来说,1KB大小的Header头缓冲区已经足够,但是对于Cookie内容较大的请求来说,可能不够用.
(8)client_header_timeout指令.
语法:client_header_timeout time
该指令用于设置读取客户端请求Header头信息的超时时间,如果超过该指令设置的时间,Nginx将返回"Request time out "错误信息(HTTP状态码为408)
(9)client_max_body_size指令
语法:client_max_body_size size
例: client_max_body_size 300m
该指令用于设置允许接受的客户端请求内容的最大值,即客户端请求Header头信息中设置的Content-Length的最大值.如果超过该指令设置的最大值.Nginx将返回"Request Entity Too Large"错误信息(HTTP状态码为413).当默认值为1MB时,如果Nginx服务器提供上传1MB以上的大文件等操作,则要加大该值.
(10)default_type指令
语法:default_type MIME-type
MIME-type是用来告诉浏览器请求的文件媒体类型,例如:MIME-type名text/plain表示该文件为文本文件,text/html表示该文件为HTML网页文件.如果Nginx无法识别该文件属于何种MIME-type类型,则该文件标记为default_type指令设置的MIME-type
(11)error_page code[code...][=|=answer-code] uri
该指令用于设置如果出现指定的HTTP错误状态码,则返回给客户端显示的对应的URI地址.
例: error_page 404 /404.html ;
(12)index指令
语法: index file [file...]
该指令用于设置Nginx的默认首页文件.
index index.$geo.html index.0.html /index.html
(13)internal指令
该指令用于设置某个location路径只能在Nginx内部使用,外部无法访问.
location /404.html {internal;}
(14)keepalive_timeout [time]指令
该指令可使客户端到服务器端的连接持续有效,当出现对服务器的后继请求时,keep-alive功能可避免建立或重新建立连接.市场上大部分Web服务器,包括IIS,Apache,Nginx,Lighttpd都支持HTTP keep-alive.对于提供静态内容的网站来说,这个功能有用，对于负担重的网站,就比较吃力了.
(15)keepalive_requests指令
语法:keepalive_requests n
设置一个keep-alive连接使用的次数.一次请求结束后,如果该连接使用的次数没有超过keepalive_requests指令设置的请求次数,则服务器并不立即主动断开连接,而是直到达到keepalive_timeout指令设置的时间,才关闭连接.
(16)large_client_header_buffers 指令
该指令用于设置客户端请求的Header头缓冲区大小,默认值为4KB.客户端请求行不能超过large_client_header_buffers指令设置的值.客户端请求的Header头信息不能大于large_client_header_buffers指令设置的缓冲区大小,否则会报414或400错误,如果客户端的Cookie信息较大,则须增加缓冲区大小.
(17)limit_except
该指令用于限制HTTP方法访问location中的内容
例: limit_except GET{
  allow 192.168.1.0/32 ;
  deny all ;
}
(18)limit_rate 指令
语法:limit_rate speed
该指令主要用来限速,限速单位是"字节数/秒",一般在提供HTTP下载等应用中会用到该指令.限速只对一个连接起效,如果客户端开启两个连接下载,下载的速度将是限速值的两倍
例1:限制每个连接的下载速度为100KB/秒
limit_rate 100k ;
例2:在特定条件下开启限速功能
server {
  if($slow)
  {
    set $limit_rate 4k ;
  }
}
(19)limit_rate_after指令
语法:limit_rate_after time
该指令可以设置一个字节数(1MB),下载的字节数大于该值后,limit_rate指令的限速功能将起效.对于MP3在线播放,HTTP方式的Flash FLV视频点播等应用,使用该指令将会起到不错的效果.
例:下载的文件字节数超过1MB后,限速为100kb/秒
limit_rate_after 1m ;
limit_rate 100k ;
(20)listen指令
该指令用于设置虚拟主机监听的服务器地址和端口号,你可以同时设置服务器地址和端口号,也可以指定一个IP地址,或者一个端口号,或者一个服务器名.如果listen指令只设置一个服务器名或IP地址,那么它的默认端口号为80.
(21)location指令
该指令对不同的URI进行不同的配置,既可以使用字符串,也可以使用正则表达式,使用正则表达式,须使用以下前缀.
1)~*,表示不区分大小写的匹配.
2)~,表示区分大小写的匹配.
(22)log_not_found指令
语法:log_not_found [on|off]
该指令用来启用或禁用404错误日志,这个指令可以用来禁止Nginx记录找不到robots.txt和favicon.ico这类文件的错误信息.
(23)log_subrequest指令
语法:log_subrequest [on|off]
该指令用来启动或禁止access_log中记录类似rewrite rules , SSI request等子请求.
(24)root指令
该指令主要用于指定请求的文档根目录.例如,配置内容为location/i/{root /spool/w3;}时,请求URI地址"/i/top.gif"将返回文件"/spool/w3/i/top.gif"的内容给客户端.
2.HTTP核心模块中可以使用的变量.
NginxHTTP核心模块支持一些与Apache变量名称相同的内置变量,例如:$http_user_agent,$http_cookie ,此外还支持一些Nginx特有的其他变量.
$arg_PARAMETER:该变量包含了当查询字符串时,GET请求可变参数的值.
$args:这个变量等于请求行中的参数.
$binary_remote_addr:二进制格式的客户端地址.
$content_length:这个变量等于客户端请求头中的content-length值.
$content_type:这个变量等于客户端请求头中的content-type值.
$cookie_COOKIE:客户端请求Header头中的cookie变量.前缀"$cookie_"加上cookie名称的变量,该变量的值即为cookie名称的值.
$document_root:这个变量等于当前请求所属的root指令设置的文档根目录路径.
$limist_rate:这个变量允许限制连接速率.
$remote_addr:客户端的IP地址.
$remote_port:客户端的端口.
3.HTTP Access模块
HTTP Access模块提供了一个简单的基于host名称的访问控制.通过该模块,可以允许或禁止指定的IP地址或IP段访问某些虚拟主机或目录.
例:
location /{
  deny 192.168.1.1 ;
  allow 192.168.1.0/24;
  allow 10.1.1.0/16 ;
  deny all ;
}
4.HTTP Auth Basic模块
该模块采用基于HTTP基本身份验证的用户名和密码登录方式,来保护你的虚拟主机或目录location /{
  auth_basic "Restricted";
  auth_basic_user_file htpasswd ;
}
auth_basic:用于指定弹出的用户名和密码登录框中提示的名称.
auth_basic_user_file指令:用于设置htpasswd密码文件的名称.
5.HTTP Browser模块
该模块可以根据客户端"User-agent"请求头中的信息创建变量.
$modern_browser:如果浏览器被识别为新浏览器,则等于modern_browser_value指令设置的值.
$ancient_browser:如果浏览器被识别为旧浏览器,则等于ancient_browser_value指令设置的值.
$msie:如果浏览器被鉴定为MSIE浏览器,该变量的值为1.
6.HTTP Charset模块
该模块用来添加文本编码类型到HTTP应答头"Content-Type indicated".此外，该模块还能够将服务器端网页原来的文本编码转换成另一种文本编码,输出给客户端.
例如:
charset windows-1251
source_charset koi8-r
charset指令用于添加文本编码类型到HTTP应答头"Content-Type indicated" .如果编码与source_charset指令设置的编码不一样,将进行重编码.参数为"off".
override_charset on|off
当该指令开启时,如果后端的FASTCGI服务器响应头带有"Content-Type"头信息,将开启编码转换.
7.HTTP Empty Gif模块
该指令可以保持一个1X1像素的透明GIF图片在内存中,当请求该图片时,能够得到非常快得响应速度.
例:location = /_.gif{ empty_gif;}
8.geo模块
geo模块主要用于做全局负载均衡,可以根据不同的客户端IP访问到不同的服务器.一些针对不同地区的客户,使用不同的服务器去处理的需求,可以使用geo模块.
配置如下:
geo $geo{
  default 0 ;
  include conf/geo.conf ;
  127.0.0.1/32 2 ;
  127.0.0.0/24 ru ;
  192.168.1.0/24 1 ;
  10.1.0.0/16  1 ;
}
defalut:任何IP地址,相当于0.0.0.0/0
include:可以引用一个文本文件,里面包含geo的配置内容.
9.Gzip模块
Gzip模块主要用于对返回给客户端的网友采用gzip进行压缩输出.
目前90%的浏览器都支持gzip和deflate两种压缩格式.如果浏览器支持gzip压缩,就会在HTTP请求头中发送一行"Accept-Encoding:gzip,deflate"，这时候Nginx服务器可以输出经过gzip压缩后的页面给浏览器,浏览器再解压.这种方式可以将网络线路上传输的大量数据消减60%以上,不仅节省了服务器带宽,同时加速了用户的下载速度和体验.
(1)gzip指令.
gzip on|off 该指令用于开启或关闭gzip模块
(2)gzip_buffers指令
gzip_buffers 4 4k/8k
设置系统获取几个单位的缓存用于存储gzip的压缩结果数据流,例如4 4k代表以4k为单位,按照原始数据大小以4k为单位的4倍申请内存.如果没有设置,默认值是申请跟原始数据相同大小的内存空间去存储gzip压缩结果.
(3)gzip_comp_level指令
gzip 压缩比,1压缩比最小处理速度最快,9压缩比最大但处理速度最慢(传输快但比较消耗cpu)
(4)gzip_min_length指令
gzip_min_length length
设置允许压缩的页面最小字节数,页面字节数从header头的Content-Length 中进行获取,默认值是0,不管页面多大都压缩,建议设置成大于1k的字节数,小于1k可能会越压越大.
(5)gzip_http_version指令
gzip_http_version 1.0|1.1
识别http的协议版本.由于早期的一些浏览器或http客户端,可能不支持gzip自解压,用户会看到乱码,所以做一些判断还是很有必要的.
(6)gzip_proxied指令
Nginx作为反向代理的时候启用,开启或关闭后端服务器返回的结果,匹配的前提是后端服务器必须要返回包含Via的header头.
(7)gzip_types指令
语法:gzip_types mime-type[mime-type...]
匹配mime类型进行压缩,（无论是否指定）"text/html"类型总是会被压缩的.
10.HTTP Headers模块
这组指令主要用来设置Nginx返回网页内容给用户时,附件的Header头信息.
(1)add_header指令
当HTTP应答状态码为200,204,301,302,或304时,增加指定的Header头,其中Header头的值可以使用变量.
(2)expires指令
使用本指令可以控制HTTP应答中的"Expires"和"Cache-Control"的Header头信息,起到控制浏览器页面缓存的作用.
语法:expires [time|epoch|max|off]
11.HTTP Index模块
该模块可以用于指定虚拟主机目录下的默认首页文件名称.如果指定了多个文件,那么将按照从左到右的顺序逐个查找.可以在列表末尾加上一个绝对路径名的文件.
12.HTTP Referer模块
当浏览器向Web服务器发送请求的时候,一般会带上Referer，告诉服务器我是从哪个页面链接过来的,服务器借此可以获得一些信息用于处理,例如防盗链.因为HTTP Referer头信息是可以通过程序来伪装生成的.所以通过Referer防盗链并非100%可靠.
13.HTTP Limit Zone模块
该模块用于针对条件,进行会话的并发连接数控制,例如限制每个IP的并发连接数.
limit_zone one $binary_remote_addr 10m ;
在以上示例中,定义一个叫"one"的记录区,总容量为10MB,以容量$binary_remote_addr作为会话的判断基准(即一个地址一个会话)
语法: limit_conn zone_name max_clients_per_ip
该指令用于指定一个会话最大的并发连接数.当超过指定的最大并发连接数时,服务器将返回503.
14.HTTP Limit Req模块
该模块允许你对Session会话,单个客户端IP地址,限制指定单位时间内的并发请求数.你可以在一定程度上减轻多应用服务器的DOS恶意攻击.
15.HTTP Log模块
(1)access_log指令.该指令用于设置日志文件的路径,格式和缓冲区大小.使用"off"作为唯一参数,将不记录日志文件.如果没有指定日志格式,将默认采用"combined"格式.缓冲区的大小必须小于写人磁盘文件的原子记录大小.
(2)log_format指令.该指令用来描述日志格式.在日志的格式中,可以使用Nginx的大多数通用变量,以及一些仅在写日志时存在的变量.
16.HTTP Map 模块
该模块允许分类,或者映射一组值到一组不同的值,并将这些值存储在一个变量中.
例:
map $http_host $name{
  hostnames ;
  default 0 ;
  example.com 1 ;
  *.example.com 1 ;
  test.com 2 ;
  *.test.com 2 ;
  .site.com 3 ;
}
17.HTTP Memcached模块
Memcached是一个高性能的分布式内存对象缓存系统,用于动态Web应用以减轻数据库负载.它通过在内存中缓存数据和对象来减少读取数据库的次数,从而提供动态,数据库驱动网站的速度.Memcached基于一个存储键/值对的hashmap.其守护进程(daemon)是用C写的.但是客户端可以用任何语言来编写,并通过Memcached协议与守护进程通信,且它并不提供冗余.当某个服务器停止运行或崩溃了,所有存放在该服务器上的键/值对都将丢失.Memcached(官方网站:http://www.memcached.org/)由Danga Interactive开发,用于提升LiveJournal.com访问速度.LJ每秒动态页面访问量几千次,用户700万.Memcached将数据库负载大幅降低,以便更好地分配资源,更快地访问.
18.HTTP Proxy模块
该模块用于转发请求到其他的服务器.keep-alive是指在HTTP/1.1协议中,同一个连接中发出和接收多次HTTP请求,节省了创建TCP连接过程的时间开销.而HTTP/1.0协议不具备keep-alive请求的能力.因此,在HTTP/1.0协议中,每一个到后端的请求都会创建一个连接,传输完成后会删除这个连接.Nginx采用HTTP/1.1协议与浏览器进行通信,采用HTTP/1.0协议与后端服务器进行通信.
proxy_cache相关指令集
1)proxy_cache
2)proxy_cache_path
3)paroxy_cache_methods
4)proxy_cache_min_uses
5)proxy_cache_valid
6)proxy_cache_use_stale
14.HTTP SSI 模块
SSI,通常称为服务器嵌入.SSI的工作原理:将内容发送到浏览器之前,可以使用"服务器端包含(SSI)"指令将文本,图形或应用程序信息包含到网页中.例如可以使用SSI包含时间/日期戳,版权声明或供客户填写并返回的表单.对于在多个文件中重复出现的文本或图形,使用包含文件是一种简便的方法.将内容存入一个包含文件中即可,而不必将内容输入所有文件。
15.HTTP Userid模块.
该模块相当于Apache的mod_uid模块,主要用于做客户端的身份标识.它主要使用$uid_got和$uid_set变量.
