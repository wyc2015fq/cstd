# nginx源码分析之http解码实现 - 囧囧有神的专栏 - CSDN博客





2015年04月02日 17:52:22[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：9291








分析nginx是如何解析并且存储http请求的。对非法甚至恶意请求的识别能力和处理方式。可以发现nginx采用状态机来解析http协议，有一定容错能力，但并不全面


# 相关配置


跟解码有关的配置


|**merge_slashes**||
|----|----|
|语法|merge_slashes on | off|
|默认值|on|
|上下文|http server|
|说明|支持解析请求行时，合并相邻的斜线。例如，一个请求 http://www.example.com/foo//bar/ 将生成如下$uri 值：     on: /foo/bar/     off: /foo//bar/ 要知道，静态location匹配是一个字符串比较，所以如果merge_slashes关闭， 一个类似/foo//bar/的请求将不会匹配location /foo/bar/. 在HttpCoreModule中|
|**underscores_in_headers **||
|语法|underscores_in_headers on | off|
|默认值|Off|
|上下文|http server|
|说明|允许或者不允许headers中的下划线|
|**ignore_invalid_headers **||
|语法|ignore_invalid_headers on | off|
|默认值|On|
|上下文|http server|
|说明|控制是否有无效name的header应该被忽略。 有效的名字是由数字 字母 连字符-  可能有下划线组成， 前后都不能有空格。如果该指令在sever级别被指定，它的值仅当server是默认的那个才使用。 指定的值被应用到所有监听同样的地址和端口的虚拟主机上。|




请求体有关配置


|**client_body_buffer_size**||
|----|----|
|语法|client_body_buffer_size size|
|默认值|8k|16k|
|上下文|http server locatioin|
|说明|指定client request body buffer大小如果request body大小超过buffer大小，那么整个请求体会写入临时文件。默认大小是page 大小2倍。 根据不同平台，可能为8K或者16K。当content-length请求头指定了比buffer size较小的值，那么nginx会使用较小的那个。结果就是，nginx将不会给每个请求分配一个这个size大小的buffer|
|**client_body_in_single_buffer**||
|语法|client_body_in_single_buffer on | off|
|默认值|Off|
|上下文|http server location|
|说明|该指令制定是否保持这整个body在一个client request buffer中。 当使用变量$request_body来减少copy操作时，这个指令是推荐的。注意到，当请求体不能保存在一个buffer中时(看client_body_buffer_size)，这个body将存到磁盘上。|
|**client_body_in_file_only **||
|语法|client_body_in_file_only on | clean | off|
|默认值|off|
|上下文|http server location|
|说明|这个指令强制nginx总是将请求体存入临时磁盘文件，甚至即使这个请求体size为0请注意，如果指令是on，文件在请求完成后也不会移除该指令可以用于debug，和嵌入的perl模块中$r->request_body_file 方法|



# 数据结构

解码的所有结果都保存在request结构里




```cpp
ngx_http_request_t {
ngx_but_t  *header_in;   //  buf，保存请求
Ngx_http_headers_in_t headers_in;   //链表，保存请求中的请求头
Ngx_http_headers_out_t headers_out;  //链表，保存response中的响应头
。。。
}
```




保存请求头的结构


//Ngx_http_headers_in_t 结构



```cpp
typedef struct {
    ngx_list_t                        headers;
    ngx_table_elt_t                  *host;
    ngx_table_elt_t                  *connection;
。。。    
    ngx_str_t                         user;
    ngx_str_t                         passwd;
。。。
    ngx_array_t                       cookies;

    ngx_str_t                         server;
    off_t                             content_length_n;
    time_t                            keep_alive_n;

    unsigned                          connection_type:2;
    unsigned                          chunked:1;
    unsigned                          msie:1;
    unsigned                          msie6:1;
    unsigned                          opera:1;
    unsigned                          gecko:1;
    unsigned                          chrome:1;
    unsigned                          safari:1;
    unsigned                          konqueror:1;
} ngx_http_headers_in_t;
```





# 解码流程

![](https://img-blog.csdn.net/20150402170020365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



ngx_http_process_request_line(ngx_event_t *rev) //请求行 解码总入口


ngx_http_process_request_headers(ngx_event_t *rev) //请求头解码入口


所有请求头的handler在ngx_http_request.c:ngx_http_headers_in中 

例如：

Host头的handler是 ngx_http_process_host

ngx_http_process_host功能是，验证host有效性，查找virtual server，找到对应的server配置。


## 请求初始化

ngx_http_request_t *

ngx_http_create_request(ngx_connection_t *c)


r->header_in = hc->nbusy ? hc->busy[0] : c->buffer;

r->http_state = NGX_HTTP_READING_REQUEST_STATE;

## 解码请求行




```cpp
ngx_http_process_request_line(ngx_event_t *rev)
{
　　ngx_http_read_request_header(r);  //从连接中读取内容，放到header_in buf中,返回读取字节数，或错误码
　　
　　
　　ngx_http_parse_request_line(r, r->header_in);
　　// 状态机解析请求行,将method  schema host port uri  protocol version 分离出来
　　}
```






　　解析状态机图如下：
![](https://img-blog.csdn.net/20150402170149503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



CR=’\r’

LF=’\n’


请求行 对应的正则


|请求行|对应的正则|
|----|----|
|Method|([A-Z_]+)|
|Schema|([a-zA-Z]+)|
|Host|(\[[a-zA-Z0-9:._~!$&\\(\)*+,;=-]*\]|[a-zA-Z0-9.-]*)|
|Port|[0-9]*)?|
|Uri|.%/?#+  不能是’\0’   /(([^CRLF.%/?#+\0 ]+/)*(([^CRLF.%/?#+\0 ]+[%?#]|.%/?#)[^ CRLFH\0]*)?)?(?=CR|LF| +H)|
|Protocol version|HTTP/[1-9]+\.[0-9]+(?=( *)CR?LF)|



ngx_http_process_request_uri    //解析uri的函数，对自特殊字符进行了一些处理

定义：


Complex_uri: uri with “/.#” 

Quoted_uri: uri with “%”

Plus_in_uri: uri with “+”

Space_in_uri: uri with “ ”

Uri_ext： 最后一截uri， 非紧邻/的.后面的部分




逻辑



```cpp
if (r->uri_ext) {
        if (r->args_start) {
            r->exten.len = r->args_start - 1 - r->uri_ext;
        } else {
            r->exten.len = r->uri_end - r->uri_ext;
        }
        r->exten.data = r->uri_ext;
}
```


如果含有/.#%, **那么会给uri重新分配内存，并解析uri**




```cpp
if (r->complex_uri || r->quoted_uri) {
r->uri.data = ngx_pnalloc(r->pool, r->uri.len + 1);  
ngx_http_parse_complex_uri(r, cscf->merge_slashes)
}
```



uri解析的状态机图
![](https://img-blog.csdn.net/20150402171118012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 请求行URI特殊字符处理

![](https://img-blog.csdn.net/20150402171513947?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**Nginx不支持第3部分， 不支持@**

这里指的是6 7 8部分


|%|%后面必须是两个十六进制数字，否则报错 NGX_HTTP_PARSE_INVALID_REQUEST|
|----|----|
|/|相邻重复的斜线，可以合并，有相关配置控制 (merge_slashes on/off控制)|
|/../|会进入上一级目录.例如 /foo/bar/../abc  会变成 /foo/abc/../前面必须有一级，否则会报错，  不会退到第一级/前面|
|.|Uri中的点,在最后一个/后面，而且不紧跟在/后面， 会是r->uri_ext的起始位置， 结束位置在args前面，或者uri结尾|
|#|表示uri的结尾，#后面的全部忽略。  如果没有#, 那么uri的结尾会以 ( *(CR?LF)| H)结束|
|？|问号到#号之间认为都是参数，如果?后没有#，那么问号后都是参数|
|+|如果遇到+  r->plus_in_uri=1|
|\r|后面必须接\n,  表示请求行结束|
|\n|表示请求行结束|



### HTTP0.9的支持

支持http 0.9


如果是协议版本小于1, 那么不会读取请求头


### 支持的方法

方法 

GET 

PUT 


POST 


COPY 

MOVE 

LOCK 

HEAD 

MKCOL 

PATCH

TRACE   NGX_HTTP_NOT_ALLOWED error code 405

DELETE 

UNLOCK 

OPTIONS

PROPFIND 

PROPPATCH




**注意：这里说的支持，是说nginx解码时，能够识别这些方法。但是在后续的处理过程中不一定支持。例如，nginx遇到TRACE方法，会返回405 not allowed**


未知的方法： 如果METHOD字符集符合 [A-B_]+, 会将请求放过去，交给后续处理

如果不符合[A-B_]+, 会报错误 400 bad request



## 解码请求头

ngx_http_process_request_headers(ngx_event_t *rev)


ngx_http_read_request_header // 从网络连接中读取内容

ngx_http_parse_header_line(r, r->header_in, cscf->underscores_in_headers);


请求头 对应的正则

Name    [0-9a-zA-Z-]? 最大长度32， 非法字符被忽略，超过32个的从头覆盖

_是否非法，看allow_underscores配置

value    [^ CRLF\0]+ 没有长度限制，以CRLF结尾




请求头解析的状态机图


![](https://img-blog.csdn.net/20150402172505047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



### 非法头部处理

Header name中包含非法字符，则认为是非法头部，nginx默认抛弃这一行，另外也可由配置ignore_invalid_headers 确定

### 相同头部处理

策略一，第二个以上相同头部忽略 ngx_http_process_header_line

策略二，如果重复，返回400错误 ngx_http_process_unique_header_line

策略三，允许多头存在，用数组保存 ngx_http_process_multi_header_lines eg. X-Forwarded-For Cookie

策略四， 使用最靠后的头


|请求头部|策略|头部类型和值|
|----|----|----|
|Connection|策略四|General如果http版本大于1,那么默认为keep-alive, 否则为close 。 只能有close 或keep-alive两种情况，否则报错|
|Host|策略一|Request headerhttp 1.0以上版本，host不能为空，否则报错|
|User-Agent|策略一|Request headerNginx会去识别是否如下六种（Msie msie6）  opera gecko chrome Safari Konqueror|
|Referer|策略一|Request header|
|Content-Type|策略一|Entity header指明发给接受者的实体主体的媒体类型，或HEAD方法中指明若请求为GET时将发送的媒体类型|
|Range|策略一|Request header|
|Transfer-Encoding|策略一|general|
|Upgrade|策略一|general|
|Accept-Encoding|策略一|Request header|
|X-Real-IP|策略一||
|Accept|策略一|Request header|
|Accept-Language|策略一|Request header|
|Depth|策略一||
|Destination|策略一||
|Overwrite|策略一||
|Date|策略一|general|
|Via||general|
|Keep-Alive|策略一||
|If-Modified-Since|策略二|Request header|
|If-Unmodified-Since|策略二|Request header|
|If-Match|策略二|Request header|
|If-None-Match|策略二|Request header|
|If-Range|策略二|Request header|
|Expect|策略二|Request header|
|Content-Length|策略二|Entity header  必须为数字，必须为正数ngx_http_process_request_header|
|Authorization|策略二|Request header|
|X-Forwarded-For|策略三||
|Cookie|策略三||
|额外的头|头部都会存在list里，怎么处理自己定义||






### 冲突或关联头部处理

transfer_encoding content-length

在ngx_http_process_request_header中，当transfer-encoding value 为chunked content-length会失效。Transfer-encoding值只能为identity或chunked，否则报错



Connection keep-alive

如果connection 为keep-alive，那么keep-alive的值会生效

如果请求头connection没有指明为close， 且http版本大于1, 那么connection默认为keep-alive.

### 不支持多行请求头

由于 HTTP/1.1 协议里的规定,所以实际上 HTTP 协议支持多行请求头,它规定任何一个以空格开头的行,都是接续在前一行之后的内容。例如:

X-Random-Comment: 这是个长句子,

所以我们得换行处理一下,看着更整齐。


Nginx不支持多行请求头


Cookies处理

Nginx没有继续解析cookies值

https://github.com/cloudflare/lua-resty-cookie



### 解码请求体

nginx核心本身不会主动读取请求体，这个工作是交给请求处理阶段的模块来做，但是nginx核心提供了ngx_http_read_client_request_body()接口来读取请求体，另外还提供了一个丢弃请求体的接口-ngx_http_discard_request_body()，在请求执行的各个阶段中，任何一个阶段的模块如果对请求体感兴趣或者希望丢掉客户端发过来的请求体，可以分别调用这两个接口来完成。这两个接口是nginx核心提供的处理请求体的标准接口，如果希望配置文件中一些请求体相关的指令（比如client_body_in_file_only，client_body_buffer_size等）能够预期工作，以及能够正常使用nginx内置的一些和请求体相关的变量（比如$request_body和$request_body_file），一般来说所有模块都必须调用这些接口来完成相应操作，如果需要自定义接口来处理请求体，也应尽量兼容nginx默认的行为。




请求体的读取一般发生在nginx的content handler中，一些nginx内置的模块，比如proxy模块，fastcgi模块，uwsgi模块等，这些模块的行为必须将客户端过来的请求体（如果有的话）以相应协议完整的转发到后端服务进程，所有的这些模块都是调用了ngx_http_read_client_request_body()接口来完成请求体读取。值得注意的是这些模块会把客户端的请求体完整的读取后才开始往后端转发数据。


ngx_http_discard_request_body

# 使用接口



## 可用变量


以下仅为举例，不全


$http_xxxx 类型的变量， xxxx为header name 连接符变成下划线，大写字母变成小写字母

X-Real-IP 变量为 $http_x_real_ip




类似的有

$args_XXXX

$cookie_XXXX




$uri


$request_body

这个变量包含请求体。这个变量出现在proxy或fastcgi_pass所在的location

$request_body_file

Client请求体临时文件名







