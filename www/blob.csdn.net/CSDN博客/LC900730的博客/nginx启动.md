# nginx启动 - LC900730的博客 - CSDN博客
2017年05月30日 21:25:12[lc900730](https://me.csdn.net/LC900730)阅读数：147
## 1.启动
执行/usr/local/nginx/sbin/nginx,默认读取/usr/local/nginx/conf/nginx.conf(实际上在没有显式指定conf，会使用–conf-path=PATH指定的nginx.conf)
## 2.指定配置文件启动
/usr/local/nginx/sbin/nginx   -c   /tmp/nginx.conf
## 3.指定安装目录的启动方式
/usr/local/nginx/sbin/nginx   -p   /usr/local/nginx
## 另行指定全局配置项的启动方式
可以通过-g参数临时指定一些全局配置项，使新的配置项生效； 
/usr/local/nginx/sbin/nginx   -g   “pid   /var/nginx/test.pid”  
-g的约束条件是指定的配置项不能与默认路径下的nginx.conf配置项冲突，否则无法启动；如nginx.conf中 pid logs/nginx.pid是不能存在于默认的nginx.conf中； 
另一个约束条件是-g方式启动的Nginx服务执行其他命令也必须带上-g。如停止nginx服务，需要
/usr/local/nginx/sbin/nginx   -g   “pid   /var/nginx/test.pid” -s    stop 
## -V
使用-V参数除了可以显示Nginx版本信息外，还可以显示配置编译阶段的信息，如GCC编译器的版本、操作系统的版本、执行configure时的参数等
## 块配置项
events{} 
http{}… 
events/http/server/location/upstream等都是块配置项
#### 配置项的语法
配置项名 配置项1  配置项2
- 1.首先在行首的是配置项名，这些配置项名必须是Nginx的某一个模块想要处理的，否则Nginx会认为配置项文件出现了非法配置项名。配置项输入结束后，以空格作为分隔符。
- 2.其次是配置项的值，可以是数字或者字符串
### server_names_hash_bucket_size
为了提高快速寻找到相应的server_name的能力，Nginx使用散列表来存储server_name。server_names_hash_bucket_size设置了每个散列桶占用的内存大小；
### server_names_hash_max_size
server_names_hash_max_size会影响散列表的冲突率。server_names_hash_max_size越大，消耗的内存越多，但散列key的冲突率会降低，检索速度也会更快。
### root
location /download/ { 
    root /opt/web/html; 
} 
在上述配置项中，如果有一个请求的URI是/download/index/test.html,那么web服务器会返回/opt/web/html/download/index/test.html文件的内容
## 3.1：如何调用http模块
worker进程在一个for循环中反复调用事件模块检测网络事件。当事件模块检测到某个客户端发起的TCP请求(接收到SYN包),将会为它建立TCP连接，成功建立连接后根据nginx.conf文件的配置会交由HTTP框架处理。HTTP框架会试图接收完整的HTTP头部，并在接收到完整的HTTP头部后将请求分发到具体的HTTP模块中处理。HTTP模块在处理请求结束时，大多会向客户端发送响应，此时会依次调用所有的HTTP过滤模块，每个过滤模块会根据配置文件决定自己的行为。
## Nginx中数据结构
#### 3.1整型封装
typedef intptr_t    ngx_int_t; 
typedef uintptr_t    ngx_uint_t;
#### 3.2字符串封装
typedef struct{ 
    size_t  len; 
    u_char *data; 
}ngx_str_t;
其中data指针指向字符串起始地址，len表示字符串的有效长度，因为指向的不是普通字符串，未必以’\0’结束，所以需要len来指定长度。 
r->method_name就是一个 ngx_str_t类型的变量; 
如if(0==ngx_strncmp( 
        r->method_name.data,”PUT”,r->method_name.len)) 
        {….} 
        其实ngx_strncmp就是strcnmp函数，为了跨平台进行封装 
    #define ngx_strcnmp(s1,s2,n)    strcnmp((const char *)s1, (const char *)s2, n) 
    如GET/test?a=1 http/1.1\r\n存储到内存地址0x1d0b0110，只需要把r->method_name设置为{len=3，data=0x1d0b0110} 
#### 3.3 ngx_list_t数据结构
    ngx_list_t是nginx封装的链表容器，如HTTP头部就是用ngx_list_t来存储的。 
typedef struct ngx_list_part_s    ngx_list_part_t
struct ngx_list_part_s{ 
    void  *elts; 
    ngx_uint_t  nelts; 
    ngx_list_part_t *next;
} 
typedef struct { 
    ngx_list_part_t *last; 
    ngx_list_part_t part; 
    size_t size; 
    ngx_uint_t nalloc; 
    ngx_pool_t *pool; 
}ngx_list_t 
ngx_list_t描述整个链表，而ngx_list_part_t只描述链表的一个元素。 
ngx_list_t不是一个单纯的链表；其实一个链表容器，存储的元素又是一个数组。 
- last:指向链表的最后一个数组元素
- part:指向链表的首个数组元素
- size:链表中的每一个ngx_list_part_t元素都是一个数组。因为数组存储的是某种类型的数据结构，且ngx_list_t是非常灵活的数据结构，所以它不会限制存储什么样的数据，只是通过size限制每一个数组元素所占的空间大小，用户所要存储的数据所占字节数必须<=size
- nalloc:链表中的数组元素一旦分配后是不可以更改的。nalloc表示每个ngx_list_part_t数组的容量，即最多存储多少个数据。 
- pool:链表中管理内存分配的内存池对象，用户要存放的数据占用的内存都是由pool分配的
