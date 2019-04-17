# C的web框架kore方案 - V__KING__的专栏 - CSDN博客





2017年08月29日 14:05:41[v__king__](https://me.csdn.net/V__KING__)阅读数：1202








# 自己实现一个demo客户端

方案：web浏览器作为客户端

## 前端

jquery 用ajax来post 数据

```java
var xmlDocument = [create xml document];
 $.ajax({
   url: "page.php",
   processData: false,
   data: xmlDocument,
   success: handleResponse
 });
```

## 后端

后端使用kore(一个使用C的web框架)。在开发阶段使用现有的llrp库。

### kore准备

#### 1. kore怎么结合c库？

a. libxxx.a是lirp的一个静态的库，需要在kore编译的时候加载进去。应为静态库是在编译的时候加载就到可执行程序里的。 

b. 在编译kore的时候，把libxxx.a和libxxx2.a 加载到程序中去，在Makefile中加入这两个库.

#### 2. kore如何从获取post的数据？

查看kore的API， 找http_populate_post(http_request *req) 

在req中有post的信息，代码如下；

```
#if 0
#define print_dbg(fmt, arg...)
#define print_info(fmt, arg...)
#define print_err(fmt, arg...)
#else
#define print_dbg(fmt, arg...)      printf("[DBG][%s:%d %s]"fmt, __FILE__, __LINE__, __FUNCTION__, ##arg)
#define print_info(fmt, arg...)     printf("[INFO][%s:%d %s]"fmt, __FILE__, __LINE__, __FUNCTION__, ##arg)
#define print_err(fmt, arg...)      printf("[ERR][%s:%d %s]"fmt, __FILE__, __LINE__, __FUNCTION__, ##arg)
#define print_pAddr(p1, p2)         printf("[Addr compare][0x%x :: 0x%x]",p1,p2)
#define print_buf(buf, len)     do{         \
    int i = 0;                              \
    print_info("");                     \
    printf("buf data = {");                 \
    for(i = 0; i < len; i++)                \
        printf("0x%02x, ", *(buf+i));           \
    printf("}\n");                          \
}while(0)

#define print_buf_char(buf, len)     do{         \
    int i = 0;                              \
    print_info("");                     \
    printf("buf data = {");                 \
    for(i = 0; i < len; i++)                \
        printf("%c, ", *(buf+i));           \
    printf("}\n");                          \
}while(0)
#endif


http_populate_post(req);
print_info(" req->host        =%s\n",req->host);
print_info(" req->path        =%s\n",req->path);
print_info(" req->agent       =%s\n",req->agent);
print_info(" req->query_string=%s\n",req->query_string);
print_info(" req->http_body_path=%s\n",req->http_body_path);
print_info(" req->http_body->data=%s\n",req->http_body->data);
print_info(" req->http_body_length=%d\n",req->http_body_length);
print_buf(req->http_body->data, req->http_body_length);
print_buf_char(req->http_body->data, req->http_body_length);
```



