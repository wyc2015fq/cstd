
# libuv学习笔记（21） - paohui0134的博客 - CSDN博客


2016年06月30日 21:29:01[paohui0134](https://me.csdn.net/paohui0134)阅读数：915


# libuv学习笔记（21）
## DNS相关功能
### 数据结构
```python
typedef
```
```python
struct
```
```python
uv_getaddrinfo_s uv_getaddrinfo_t;
```
```python
struct
```
```python
uv_getaddrinfo_s {
  UV_REQ_FIELDS
```
```python
//uv_req_t的成员
```
```python
uv_loop_t* loop;
```
```python
//UV_GETADDRINFO_PRIVATE_FIELDS展开：
```
```python
struct
```
```python
uv__work work_req;
```
```python
//线程池任务
```
```python
uv_getaddrinfo_cb getaddrinfo_cb;
```
```python
void
```
```python
* alloc;
```
```python
//分配的内存的指针，方便释放
```
```python
WCHAR* node;                                                             
  WCHAR* service;
```
```python
struct
```
```python
addrinfoW* addrinfow;
```
```python
struct
```
```python
addrinfo* addrinfo;
```
```python
//结果，需要通过uv_freeaddrinfo()释放
```
```python
int
```
```python
retcode;
};
```
```python
typedef
```
```python
struct
```
```python
uv_getnameinfo_s uv_getnameinfo_t;
```
```python
struct
```
```python
uv_getnameinfo_s {
  UV_REQ_FIELDS
```
```python
//uv_req_t的成员
```
```python
uv_loop_t* loop;
```
```python
//UV_GETNAMEINFO_PRIVATE_FIELDS展开：
```
```python
struct
```
```python
uv__work work_req;
```
```python
//线程池任务
```
```python
uv_getnameinfo_cb getnameinfo_cb;
```
```python
struct
```
```python
sockaddr_storage storage;
```
```python
//保存询问的地址
```
```python
int
```
```python
flags;
```
```python
char
```
```python
host[NI_MAXHOST];
```
```python
char
```
```python
service[NI_MAXSERV];
```
```python
int
```
```python
retcode;
};
```
### 相关函数
#### `uv_getaddrinfo，获取地址信息
```python
int
```
```python
uv_getaddrinfo(uv_loop_t* loop,
                   uv_getaddrinfo_t* req,
                   uv_getaddrinfo_cb getaddrinfo_cb,
```
```python
const
```
```python
char
```
```python
* node,
```
```python
//主机名或ip地址
```
```python
const
```
```python
char
```
```python
* service,
```
```python
//服务名或者端口
```
```python
const
```
```python
struct
```
```python
addrinfo* hints) {
```
```python
int
```
```python
nodesize =
```
```python
0
```
```python
;
```
```python
int
```
```python
servicesize =
```
```python
0
```
```python
;
```
```python
int
```
```python
hintssize =
```
```python
0
```
```python
;
```
```python
char
```
```python
* alloc_ptr = NULL;
```
```python
int
```
```python
err;
```
```python
if
```
```python
(req == NULL || (node == NULL && service == NULL)) {
    err = WSAEINVAL;
```
```python
goto
```
```python
error;
  }
  uv_req_init(loop, (uv_req_t*)req);
```
```python
//初始化请求
```
```python
req->getaddrinfo_cb = getaddrinfo_cb;
  req->addrinfo = NULL;
  req->type = UV_GETADDRINFO;
  req->loop = loop;
  req->retcode =
```
```python
0
```
```python
;
```
```python
//获取node与service的长度
```
```python
//此处省略
```
```python
//
```
```python
if
```
```python
(hints != NULL) {
    hintssize = ALIGNED_SIZE(
```
```python
sizeof
```
```python
(
```
```python
struct
```
```python
addrinfoW));
  }
```
```python
//申请内存
```
```python
alloc_ptr = (
```
```python
char
```
```python
*)uv__malloc(nodesize + servicesize + hintssize);
```
```python
if
```
```python
(!alloc_ptr) {
    err = WSAENOBUFS;
```
```python
goto
```
```python
error;
  }
  req->alloc = (
```
```python
void
```
```python
*)alloc_ptr;
```
```python
//将node和service  utf8转换为unicode
```
```python
//都存储在alloc_ptr指向的内存中
```
```python
//此处省略
```
```python
//此处省略
```
```python
//将hints 的内容复制到req->addrinfow
```
```python
//此处省略
```
```python
uv__req_register(loop, req);
```
```python
if
```
```python
(getaddrinfo_cb) {
```
```python
//有回调函数，那么走loop流程
```
```python
uv__work_submit(loop,
                    &req->work_req,
                    uv__getaddrinfo_work,
```
```python
//调用GetAddrInfoW(req->node, req->service, hints, &req->addrinfow);
```
```python
uv__getaddrinfo_done);
```
```python
//
```
```python
return
```
```python
0
```
```python
;
  }
```
```python
else
```
```python
{
```
```python
//没有回调函数，直接调用
```
```python
uv__getaddrinfo_work(&req->work_req);
    uv__getaddrinfo_done(&req->work_req,
```
```python
0
```
```python
);
```
```python
return
```
```python
req->retcode;
  }
error:
```
```python
if
```
```python
(req != NULL) {
    uv__free(req->alloc);
    req->alloc = NULL;
  }
```
```python
return
```
```python
uv_translate_sys_error(err);
}
```
获取信息之后loop调用的回调
```python
static
```
```python
void
```
```python
uv__getaddrinfo_done(
```
```python
struct
```
```python
uv__work* w,
```
```python
int
```
```python
status) {
  uv_getaddrinfo_t* req;
```
```python
int
```
```python
addrinfo_len =
```
```python
0
```
```python
;
```
```python
int
```
```python
name_len =
```
```python
0
```
```python
;
  size_t addrinfo_struct_len = ALIGNED_SIZE(
```
```python
sizeof
```
```python
(
```
```python
struct
```
```python
addrinfo));
```
```python
struct
```
```python
addrinfoW* addrinfow_ptr;
```
```python
struct
```
```python
addrinfo* addrinfo_ptr;
```
```python
char
```
```python
* alloc_ptr = NULL;
```
```python
char
```
```python
* cur_ptr = NULL;
  req = container_of(w, uv_getaddrinfo_t, work_req);
  uv__free(req->alloc);
```
```python
//释放之前申请的内存
```
```python
req->alloc = NULL;
```
```python
if
```
```python
(status == UV_ECANCELED) {
```
```python
//被取消的请求
```
```python
assert(req->retcode ==
```
```python
0
```
```python
);
    req->retcode = UV_EAI_CANCELED;
```
```python
goto
```
```python
complete;
  }
```
```python
if
```
```python
(req->retcode ==
```
```python
0
```
```python
) {
```
```python
// 将addrinfoW转换为addrinfo
```
```python
//此处省略
```
```python
//将名字转换为utf8
```
```python
//此处省略
```
```python
//释放req->addrinfow
```
```python
if
```
```python
(req->addrinfow != NULL) {
    FreeAddrInfoW(req->addrinfow);
    req->addrinfow = NULL;
  }
complete:
  uv__req_unregister(req->loop, req);
```
```python
/* finally do callback with converted result */
```
```python
if
```
```python
(req->getaddrinfo_cb)
```
```python
//调用用户的回调函数
```
```python
req->getaddrinfo_cb(req, req->retcode, req->addrinfo);
}
```
#### 释放地址内存
```python
void
```
```python
uv_freeaddrinfo(
```
```python
struct
```
```python
addrinfo* ai) {
```
```python
char
```
```python
* alloc_ptr = (
```
```python
char
```
```python
*)ai;
  uv__free(alloc_ptr);
}
```
#### 通过地址获取主机名
```python
int uv_getnameinfo(uv_loop_t
```
```python
*
```
```python
loop
```
```python
,
                   uv_getnameinfo_t
```
```python
*
```
```python
req,
                   uv_getnameinfo_cb getnameinfo_cb,
                   const struct sockaddr
```
```python
*
```
```python
addr,
```
```python
//地址
```
```python
int flags) {
```
```python
if
```
```python
(req
```
```python
==
```
```python
NULL
```
```python
||
```
```python
addr
```
```python
==
```
```python
NULL
```
```python
)
```
```python
return
```
```python
UV_EINVAL;
```
```python
if
```
```python
(addr
```
```python
->
```
```python
sa_family
```
```python
==
```
```python
AF_INET) {
    memcpy(
```
```python
&
```
```python
req
```
```python
->
```
```python
storage,
           addr,
           sizeof(struct sockaddr_in));
  }
```
```python
else
```
```python
if
```
```python
(addr
```
```python
->
```
```python
sa_family
```
```python
==
```
```python
AF_INET6) {
    memcpy(
```
```python
&
```
```python
req
```
```python
->
```
```python
storage,
           addr,
           sizeof(struct sockaddr_in6));
  }
```
```python
else
```
```python
{
```
```python
return
```
```python
UV_EINVAL;
  }
  uv_req_init(
```
```python
loop
```
```python
, (uv_req_t
```
```python
*
```
```python
)req);
  uv__req_register(
```
```python
loop
```
```python
, req);
  req
```
```python
->
```
```python
getnameinfo_cb
```
```python
=
```
```python
getnameinfo_cb;
  req
```
```python
->
```
```python
flags
```
```python
=
```
```python
flags;
  req
```
```python
->
```
```python
type
```
```python
=
```
```python
UV_GETNAMEINFO;
  req
```
```python
->
```
```python
loop
```
```python
=
```
```python
loop
```
```python
;
  req
```
```python
->
```
```python
retcode
```
```python
=
```
```python
0
```
```python
;
```
```python
if
```
```python
(getnameinfo_cb) {
    uv__work_submit(
```
```python
loop
```
```python
,
```
```python
&
```
```python
req
```
```python
->
```
```python
work_req,
                    uv__getnameinfo_work,
```
```python
//调用GetNameInfoW
```
```python
uv__getnameinfo_done);
```
```python
return
```
```python
0
```
```python
;
  }
```
```python
else
```
```python
{
    uv__getnameinfo_work(
```
```python
&
```
```python
req
```
```python
->
```
```python
work_req);
    uv__getnameinfo_done(
```
```python
&
```
```python
req
```
```python
->
```
```python
work_req,
```
```python
0
```
```python
);
```
```python
return
```
```python
req
```
```python
->
```
```python
retcode;
  }
}
```
loop回调
```python
static
```
```python
void
```
```python
uv__getnameinfo_done(
```
```python
struct
```
```python
uv__work* w,
```
```python
int
```
```python
status) {
  uv_getnameinfo_t* req;
```
```python
char
```
```python
* host;
```
```python
char
```
```python
* service;
  req = container_of(w, uv_getnameinfo_t, work_req);
  uv__req_unregister(req->loop, req);
  host = service = NULL;
```
```python
if
```
```python
(status == UV_ECANCELED) {
    assert(req->retcode ==
```
```python
0
```
```python
);
    req->retcode = UV_EAI_CANCELED;
  }
```
```python
else
```
```python
if
```
```python
(req->retcode ==
```
```python
0
```
```python
) {
    host = req->host;
    service = req->service;
  }
```
```python
if
```
```python
(req->getnameinfo_cb)
```
```python
//回调用户回调函数
```
```python
req->getnameinfo_cb(req, req->retcode, host, service);
}
```

