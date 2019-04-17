# ngx lua模块源码简单解析 - 囧囧有神的专栏 - CSDN博客





2014年07月11日 11:45:10[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：12646








对nginx lua模块的整个流程，原理简单解析。由于nginx lua模块相关配置，指令，API非常多，所以本文档只以content_by_lua指令举例说明。

读本文档最好配合读源码. 不适合对nginx和lua一点都不了解的人看。


# **1.相关配置**

详细配置见 https://github.com/openresty/lua-nginx-module#installation


# **2.源码解析**

src/ngx_http_lua_module.c为模块主入口文件

模块类型：NGX_HTTP_MODULE

|**解析配置阶段（从上到下按执行顺序）    **||
|----|----|
|函数名|作用|
|ngx_http_lua_create_main_conf|create main configuration 创建main级别配置ngx_http_lua_main_conf_t 见本文档2.1.1|
|ngx_http_lua_create_loc_conf|create location configuration 创建location级别配置ngx_http_lua_loc_conf_t 见本文档2.1.2|
|ngx_http_lua_content_by_lua|配置设置解析 见本文档2.2.1|
|ngx_http_lua_init_main_conf|init main configuration 解析完配置后，如果某项配置没配，给予默认值 见本文档2.2.2|
|ngx_http_lua_merge_loc_conf|merge location configuration 合并location配置 见本文档2.2.3|
|ngx_http_lua_init|Postconfiguration 配置解析完后，进行的配置检查操作 见本文档2.2.4|
|**处理请求阶段**||
|ngx_http_lua_content_handler(ngx_http_request_t *r)|处理请求体，创建执行lua code的上下文环境|
|ngx_http_lua_content_handler_inline(ngx_http_request_t *r)|这个函数被ngx_http_lua_content_handler调用。本函数实际调用lua code来处理请求 见本文档2.3|


## 2.1配置

由于配置太多。 使用content_by_lua指令举例



```cpp
static ngx_command_t ngx_http_lua_cmds[] = {
/* content_by_lua <inline script> */
    { ngx_string("content_by_lua"),
      NGX_HTTP_LOC_CONF|NGX_HTTP_LIF_CONF|NGX_CONF_TAKE1,
      ngx_http_lua_content_by_lua,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      (void *) ngx_http_lua_content_handler_inline },
。。。
}
```



**content_by_lua语法** ：         content_by_lua <lua-script-str>
**默认值**：      无
**上下文**：       location, location if
**说明**：           行为类似与一个“content handler”，给每个请求执行定义于lua-script-str中的lua code。这lua code也许会调用API calls，并且在一个独立的全局环境（例如 沙盒）中作为一个新的spawned coroutine执行

不要将这个指令和其他content handler指令在相同的location中使用。例如这个指令和proxy_pass指令不能同时使用在相同的location中




### 2.1.1 Main configuration

结构体 ngx_http_lua_main_conf_t 部分成员 配置项



|**ngx_http_lua_main_conf_t**||
|----|----|
|lua_State *lua|Postconfiguration阶段ngx_http_lua_init中被创建。Lua的状态机|
|ngx_str_t lua_path;|Lua_package_path指定，指定的脚本所使用的Lua模块搜索路径|
|ngx_str_t lua_cpath;|Lua_package_cpath指定，设置Lua c模块搜索路径|
|||
|ngx_http_lua_conf_handler_pt init_handler|Value为ngx_http_lua_init_by_inline init_by_lua中指定Value为ngx_http_lua_init_by_file init_by_lua_file中指定在Postconfiguration阶段ngx_http_lua_init中执行|
|Ngx_str_t init_src|Init_by_lua[_file] 指定lua源码，在Postconfiguration阶段ngx_http_lua_init中执行|
|||
|unsigned requires_capture_filter:1|ngx_http_lua_content_by_lua中赋值为1|
|||








### 2.1.2 Location configuration



ngx_http_lua_loc_conf_t部分成员，配置项



|ngx_http_lua_loc_conf_t||
|----|----|
|ngx_flag_t force_read_body|是否强制读取request body配置lua_need_request_body|
|ngx_flag_t enable_code_cache|是否使能code cache 默认是1 配置lua_code_cache|
|||
|ngx_http_handler_pt content_handler|Content_by_lua 指令 Cmd->post ngx_http_lua_content_handler_inline|
|ngx_http_complex_value_t content_src|Content_by_lua 的lua code字符串|
|u_char *content_src_key|Cached key for content_src 指令content_by_lua lua code的key。内容为 “nhli_” + md5 + ‘\0’值|
|Ngx_flag_t check_client_abort|检查client是否关闭连接 配置lua_check_client_abort|
|||





## 2.2 配置解析

### 2.2.1 content_by_lua指令解析

ngx_http_lua_content_by_lua


合法性检查

1.将lua code保存在location配置中，key是“nhli_” + md5(src) + ‘\0’

2.注册在content handler阶段执行的函数ngx_http_lua_content_handler




```cpp
llcf->content_src.value = value[1];

llcf->content_src_key = p;
p = ngx_copy(p, NGX_HTTP_LUA_INLINE_TAG, NGX_HTTP_LUA_INLINE_TAG_LEN);
p = ngx_http_lua_digest_hex(p, value[1].data, value[1].len);
*p = '\0';

llcf->content_handler = (ngx_http_handler_pt) cmd->post;
lmcf->requires_capture_filter = 1; // main conf
clcf->handler = ngx_http_lua_content_handler; // register location content handler
```



### 2.2.2 Main部分配置默认值

ngx_http_lua_init_main_conf

如果以下配置没被显式指定，那么将被设置成以下值
**ngx_http_lua_main_conf_t**

regex_cache_max_entries    1024

regex_match_limit                   0

max_pending_timers              1024

max_running_timers               256


### 2.2.3 Loc部分配置合并

如果在http 里面和location里面都有这个配置，那么就以location配置为准

### 2.2.4 解析完配置后初始化操作

在函数ngx_http_lua_init中

功能：

1.创建lua_State

2.注册nginx api for lua。 注册lua code cache regex cache socket pool等全局变量

3. 执行init_by_lua指令中lua code






```
if (multi_http_blocks || lmcf->requires_capture_filter) {// require_capture_filter 为1
        rc = ngx_http_lua_capture_filter_init(cf);//
        if (rc != NGX_OK) {
            return rc;
        }
　　}
lmcf->postponed_to_rewrite_phase_end = 0;
if (lmcf->lua == NULL) {
　　ngx_http_lua_init_vm(cf, lmcf);  // init lua vm
　　lmcf->init_handler(cf->log, lmcf, lmcf->lua); // init_by_lua中的内容
}
```

**注册nginx api for lua**

**详见本文档4.2**


## 2.3 请求处理

### 2.3.1 Content handler源码解析

1.编译lua code成字节码，或者从code cache中得到字节码（code cache开关打开）

2.如果code cache打开，存储字节码

3.新建coroutine

4.将全局变量设为coroutine字节码的环境

5.将request结构体指针存入coroutine全局变量

6.执行coroutine






```
ngx_http_lua_content_handler_inline {
　　/*  load Lua inline script (w/ cache) sp = 1 */
　　rc = ngx_http_lua_cache_loadbuffer(L, llcf->content_src.value.data,
                                       llcf->content_src.value.len,
                                       llcf->content_src_key,
                                       "content_by_lua",
                                       llcf->enable_code_cache ? 1 : 0);
　　return ngx_http_lua_content_by_chunk(L, r);
}
```



ngx_http_handler->ngx_http_core_run_phases->ngx_http_core_content_phase->ngx_http_lua_content_handler

# **3. 使用接口**

Nginx解析完请求后，找到了对应虚拟主机的配置，通常情况下会经过以下几个阶段的处理：





|NGX_HTTP_POST_READ_PHASE:读取请求内容阶段||
|----|----|
|NGX_HTTP_SERVER_REWRITE_PHASE:Server请求地址重写阶段||
|NGX_HTTP_FIND_CONFIG_PHASE:配置查找阶段||
|NGX_HTTP_REWRITE_PHASE:Location请求地址重写阶段|Rewrite_by_lua rewrite_by_lua_file set_by_lua|
|NGX_HTTP_POST_REWRITE_PHASE:请求地址重写提交阶段||
|NGX_HTTP_PREACCESS_PHASE:访问权限检查准备阶段||
|NGX_HTTP_ACCESS_PHASE:访问权限检查阶段|Access_by_lua access_by_lua_file|
|NGX_HTTP_POST_ACCESS_PHASE:访问权限检查提交阶段||
|NGX_HTTP_TRY_FILES_PHASE:配置项try_files处理阶段||
|NGX_HTTP_CONTENT_PHASE:内容产生阶段|Content_by_lua content_by_lua_file|
|NGX_HTTP_LOG_PHASE:日志模块处理阶段|Log_by_lua log_by_lua_file|


 内容产生阶段完成以后，生成的输出会被传递到filter模块去进行处理。filter模块也是与location相关的。所有的fiter模块都被组织成一条链。输出会依次穿越所有的filter，直到有一个filter模块的返回值表明已经处理完成。

比如gzip有关的。




# **4. C语言嵌入lua原理**

C作为程序语言， lua作为扩展语言

C和lua之间通信关键内容是一个虚拟的栈。几乎所有的API调用都是对栈上的值进行操作。数据交换也通过栈。

## 4.1 协程(coroutine)

每个nginx work进程有一个lua_State结构，在Postconfiguration阶段创建，见4.2.4

lua_State表示一个独立的lua虚拟机，会创建自己的栈。

所有的nginx api for lua保存在lua_State的全局变量中。





|LUA_GLOBALSINDEX 全局变量，保存所有API 公共变量||
|----|----|
|__ngx_req|ngx_http_request_t 结构体指针|
|ngx.arg|所有参数|
|ngx.req|请求相关的函数|
|ngx.log|日志相关的函数|
|ngx.OK ngx.DONE ngx.AGAIN|定义的一些返回值|
|。。。。。。||
|||
|LUA_REGISTRYINDEX 保存一些公共数据Lua 提供了一个注册表，这是一个预定义出来的表，可以用来保存任何 C 代码想保存的 Lua 值。 这个表可以用伪索引 LUA_REGISTRYINDEX 来定位||
|ngx_http_lua_coroutines_key|保存所有协程的table|
|ngx_http_lua_ctx_tables_key|Lua request ctx data table|
|ngx_http_lua_socket_pool_key|Lua socket connection pool table 连接池|
|ngx_http_lua_regex_cache_key|Lua precompiled regex object cache 正则表达式对象池|
|ngx_http_lua_code_cache_key|register table to cache user code|



lua_State *lua_newthread (lua_State *L)

创建一个coroutine. 将其压入堆栈。返回维护这个coroutine的lua_State指针，

新的状态机继承共享原有状态机中的所有对象（比如一些 table）， 但是它有独立的执行堆栈。




## 4.2 nginx与lua之间的数据交互

### 4.2.1 将request结构体指针存入lua全局变量

代码位置：lua-nginx-module/src/ngx_http_lua_util.h:ngx_http_lua_set_req

这段代码只有三行，在每个请求过来后，lua code执行前，都会执行




```cpp
lua_pushliteral(L, "__ngx_req");//将字符串"__ngx_req"压栈
    lua_pushlightuserdata(L, r);// 将request指针压栈r压栈
　　lua_rawset(L, LUA_GLOBALSINDEX);
　　//将request指针存入全局变量  global[“__ngx_req”] = r
```



lua_rawset类似于 lua_settable (lua_State *L, int index);， 

作一个等价于 t[k] = v 的操作， 这里 t 是一个给定有效索引 index 处的值， v 指栈顶的值， 而 k 是栈顶之下的那个值。


### 4.2.2 从lua全局变量得到request结构体指针

代码位置：slua-nginx-module/src/ngx_http_lua_util.h:ngx_http_lua_get_req

为ngx_http_lua_set_req的反操作



```cpp
{
    ngx_http_request_t    *r;
　　lua_pushliteral(L, "__ngx_req");//将字符串"__ngx_req"压栈
    lua_rawget(L, LUA_GLOBALSINDEX);//将全局变量global[“__ngx_req”]压栈
    r = lua_touserdata(L, -1);//global[“__ngx_req”]即为light user data类型，将这个指针转为ngx_http_request_t *类型
    lua_pop(L, 1);//从栈顶弹出一个元素
    return r;
}
```




### 4.2.3 注册C函数，处理request指针

所有的nginx api for lua注册在

lua-nginx-module/src/ngx_http_lua_util.c:ngx_http_lua_inject_ngx_api中

与request有关的注册在

lua-nginx-module/src/ngx_http_lua_util.c:ngx_http_lua_inject_req_api中

**举例说，注册处理method的函数ngx.req.get_method：**






```cpp
lua_createtable(L, 0 /* narr */, 23 /* nrec */);    /* .req */
{
//将处理request结构体的c函数压栈
lua_pushcfunction(L, ngx_http_lua_ngx_req_get_method);
//给.req table赋值，相当于.req[“get_method”]=ngx_http_lua_ngx_req_get_method，并将栈首元素出栈
lua_setfield(L, -2, "get_method");
}
//等价于ngx[“req”] = req table
lua_setfield(L, -2, "req");
```




注册函数的写法有统一的格式：



```cpp
static int
ngx_http_lua_ngx_req_get_method(lua_State *L)
{
    int                      n;
    ngx_http_request_t      *r;
    n = lua_gettop(L);
    if (n != 0) {
        return luaL_error(L, "only one argument expected but got %d", n);
    }
    r = ngx_http_lua_get_req(L);//从lua全局变量得到request结构体指针，见4.2.2
    if (r == NULL) {
        return luaL_error(L, "request object not found");
    }
    ngx_http_lua_check_fake_request(L, r);//检查r合法性

    lua_pushlstring(L, (char *) r->method_name.data, r->method_name.len);//将method压栈
    return 1;
}
```




### 4.2.4 在lua脚本中调用C函数，处理request

在4.3.3中，我们注册了ngx.req.get_method方法。我们可以在nginx的配置文件中调用。

语法: method_name = ngx.req.get_method()

上下文: set_by_lua, rewrite_by_lua*, access_by_lua*, content_by_lua*, header_filter_by_lua**

获取当前请求的请求方法名。得到像“GET”和”POST”这样的字符串

如果当前的请求是nginx 子请求，那么子请求的方法名将返回


配置举例



```
server {
    listen 80 default_server;
    listen 443;
    proxy_http_version 1.1;
    proxy_set_header Upgrade $http_upgrade;
    proxy_set_header Connection $connection_upgrade;
    proxy_set_header Host $host;
    location / {
        content_by_lua '
            method = ngx.req.get_method();
            ngx.say(method);
        ';
    }
}
```



结果：



```
root@root:~/nginx1.4.3/logs$ curl -i http://127.0.0.1
HTTP/1.1 200 OK
Server: NexQloud/2.0.0
Date: Mon, 30 Jun 2014 11:21:33 GMT
Content-Type: application/octet-stream
Transfer-Encoding: chunked
Connection: keep-alive

GET
```





## C如何从lua中得到数据

在4.2.4中content_by_lua指令指定的lua语句中，调用了ngx.say() 函数，

ngx.say函数定义在lua-nginx-module/src/ngx_http_lua_output.c:ngx_http_lua_ngx_say

实际上调用的是ngx_http_lua_ngx_echo函数。

这个函数调用了lua_tolstring lua_toboolean lua_touserdata等函数，将lua栈中的数据弹出栈，转化成C里面的数据，然后nginx做相应的处理，






**int lua_toboolean (lua_State *L, int index) **                                                                             把指定的索引处的的 Lua 值转换为一个 C 中的 boolean 值（ 0 或是 1 ）。

**lua_tocfunction**                                                                                                                              把给定索引处的 Lua 值转换为一个 C 函数。 这个值必须是一个 C 函数；如果不是就返回 NULL 。
**lua_tointeger**                                                                                                                                 把给定索引处的 Lua 值转换为 lua_Integer 这样一个有符号整数类型。 这个 Lua 值必须是一个数字或是一个可以转换为数字的字符串 （参见 §2.2.1）； 否则，lua_tointeger
 返回 0 
**lua_tolstring **                                                                                                                                  把给定索引处的 Lua 值转换为一个 C 字符串。 如果 len 不为 NULL ， 它还把字符串长度设到 *len 中。 这个 Lua 值必须是一个字符串或是一个数字； 否则返回返回 NULL

**lua_tonumber**                                                                                                                                把给定索引处的 Lua 值转换为 lua_Number 这样一个 C 类型。就是double类型
**lua_tothread**                                                                                                                                   把给定索引处的值转换为一个 Lua 线程
**lua_touserdata**                                                                                                                              如果给定索引处的值是一个完整的 userdata ，函数返回内存块的地址。 如果值是一个 light userdata ，那么就返回它表示的指针。 否则，返回 NULL 。
**Lua_type**                                                                                                                                          LUA_TNIL ， LUA_TNUMBER ， LUA_TBOOLEAN ， LUA_TSTRING ， LUA_TTABLE ， LUA_TFUNCTION ， LUA_TUSERDATA
 ， LUA_TTHREAD ， LUA_TLIGHTUSERDATA


ngx_http_lua_ngx_echo函数调用了

ngx_http_lua_ngx_echo






```cpp
{
。。。
　　for (i = 1; i <= nargs; i++) {
　　        type = lua_type(L, i);
　　        switch (type) {
　　            case LUA_TNUMBER:
　　            case LUA_TSTRING:
　　                lua_tolstring(L, i, &len);
　　                size += len;
　　                break;
　　            case LUA_TTABLE:
　　                size += ngx_http_lua_calc_strlen_in_table(L, i, i,
                                                   0 /* strict */);
　　                break;
　　            case LUA_TLIGHTUSERDATA:
　　                dd("userdata: %p", lua_touserdata(L, i));
　　                if (lua_touserdata(L, i) == NULL) {
　　                    size += sizeof("null") - 1;
　　                    break;
　　                }
　　                continue;
　　            default:
　　                msg = lua_pushfstring(L, "string, number, boolean, nil, "
　　                                      "ngx.null, or array table expected, "
　　                                      "but got %s", lua_typename(L, type));
　　                return luaL_argerror(L, i, msg);
　　        }
　　    }
　　。。。
}
```





