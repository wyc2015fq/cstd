
# libuv学习笔记（14） - paohui0134的博客 - CSDN博客


2016年06月19日 14:45:11[paohui0134](https://me.csdn.net/paohui0134)阅读数：1267


# libuv学习笔记（14）
## uv_pipe_t数据结构以及相关函数（1）
### 数据结构
```python
typedef
```
```python
struct
```
```python
uv_pipe_s uv_pipe_t;
```
```python
struct
```
```python
uv_pipe_s {
  UV_HANDLE_FIELDS
```
```python
//uv_handle_t的成员
```
```python
UV_STREAM_FIELDS
```
```python
//uv_stream_t的相关成员
```
```python
int
```
```python
ipc;
```
```python
//UV_PIPE_PRIVATE_FIELDS展开：
```
```python
HANDLE handle;                                                              
  WCHAR* name;
```
```python
union
```
```python
{
```
```python
struct
```
```python
{
```
```python
//接受用到   服务端相关功能
```
```python
int
```
```python
pending_instances;                                               
      uv_pipe_accept_t* accept_reqs;                                          
      uv_pipe_accept_t* pending_accepts;
    } serv;
```
```python
struct
```
```python
{
```
```python
//连接用到   客户端相关功能
```
```python
uv_timer_t* eof_timer;                                                     
      uv_write_t ipc_header_write_req;
```
```python
int
```
```python
ipc_pid;                                                               
      uint64_t remaining_ipc_rawdata_bytes;
```
```python
struct
```
```python
{
```
```python
void
```
```python
*
```
```python
queue
```
```python
[
```
```python
2
```
```python
];
```
```python
int
```
```python
queue_len;                                                          
      } pending_ipc_info;                                                      
      uv_write_t* non_overlapped_writes_tail;                                 
      uv_mutex_t readfile_mutex;
```
```python
volatile
```
```python
HANDLE readfile_thread;
    }  conn;                             
  } pipe;
};
```
### 相关函数
#### 初始化。导出函数，uv.h中声明，pipe.c中定义
```python
int uv_pipe_init(uv_loop_t
```
```python
*
```
```python
loop
```
```python
, uv_pipe_t
```
```python
*
```
```python
handle
```
```python
, int ipc) 
{
```
```python
//流初始化。pipe也是流的一种实现
```
```python
uv_stream_init(
```
```python
loop
```
```python
, (uv_stream_t
```
```python
*
```
```python
)
```
```python
handle
```
```python
, UV_NAMED_PIPE);
```
```python
handle
```
```python
->
```
```python
reqs_pending
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
handle
```
```python
->
```
```python
handle
```
```python
=
```
```python
INVALID_HANDLE_VALUE;
```
```python
handle
```
```python
->
```
```python
name
```
```python
=
```
```python
NULL
```
```python
;
```
```python
handle
```
```python
->
```
```python
pipe
```
```python
.
```
```python
conn
```
```python
.
```
```python
ipc_pid
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
handle
```
```python
->
```
```python
pipe
```
```python
.
```
```python
conn
```
```python
.
```
```python
remaining_ipc_rawdata_bytes
```
```python
=
```
```python
0
```
```python
;
  QUEUE_INIT(
```
```python
&
```
```python
handle
```
```python
->
```
```python
pipe
```
```python
.
```
```python
conn
```
```python
.
```
```python
pending_ipc_info
```
```python
.
```
```python
queue
```
```python
);
```
```python
handle
```
```python
->
```
```python
pipe
```
```python
.
```
```python
conn
```
```python
.
```
```python
pending_ipc_info
```
```python
.
```
```python
queue_len
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
handle
```
```python
->
```
```python
ipc
```
```python
=
```
```python
ipc;
```
```python
handle
```
```python
->
```
```python
pipe
```
```python
.
```
```python
conn
```
```python
.
```
```python
non_overlapped_writes_tail
```
```python
=
```
```python
NULL
```
```python
;
```
```python
handle
```
```python
->
```
```python
pipe
```
```python
.
```
```python
conn
```
```python
.
```
```python
readfile_thread
```
```python
=
```
```python
NULL
```
```python
;
```
```python
//写请求初始化
```
```python
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
)
```
```python
&
```
```python
handle
```
```python
->
```
```python
pipe
```
```python
.
```
```python
conn
```
```python
.
```
```python
ipc_header_write_req);
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
#### 打开一个文件描述符作为uv_pipe_t.导出函数，uv.h中声明，pipe.c中定义
```python
int
```
```python
uv_pipe_open(uv_pipe_t* pipe, uv_file file) {
  HANDLE os_handle = uv__get_osfhandle(file);
  NTSTATUS nt_status;
  IO_STATUS_BLOCK io_status;
  FILE_ACCESS_INFORMATION access;
  DWORD duplex_flags =
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
(os_handle == INVALID_HANDLE_VALUE)
```
```python
return
```
```python
UV_EBADF;
```
```python
//为了避免关闭stdio的文件描述符0-2，使用复制的文件描述符
```
```python
if
```
```python
(file <=
```
```python
2
```
```python
) {
```
```python
if
```
```python
(!DuplicateHandle(INVALID_HANDLE_VALUE,
                         os_handle,
                         INVALID_HANDLE_VALUE,
                         &os_handle,
```
```python
0
```
```python
,
                         FALSE,
                         DUPLICATE_SAME_ACCESS))
```
```python
return
```
```python
uv_translate_sys_error(GetLastError());
    file = -
```
```python
1
```
```python
;
  }
```
```python
//确定句柄权限。
```
```python
nt_status = pNtQueryInformationFile(os_handle,
                                      &io_status,
                                      &access,
```
```python
sizeof
```
```python
(access),
                                      FileAccessInformation);
```
```python
if
```
```python
(nt_status != STATUS_SUCCESS)
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
(pipe->ipc) {
```
```python
//跨进程的管道，必须可写 可读
```
```python
if
```
```python
(!(access.AccessFlags & FILE_WRITE_DATA) ||
        !(access.AccessFlags & FILE_READ_DATA)) {
```
```python
return
```
```python
UV_EINVAL;
    }
  }
```
```python
if
```
```python
(access.AccessFlags & FILE_WRITE_DATA)
    duplex_flags |= UV_HANDLE_WRITABLE;
```
```python
if
```
```python
(access.AccessFlags & FILE_READ_DATA)
    duplex_flags |= UV_HANDLE_READABLE;
```
```python
//设置pipe的句柄
```
```python
if
```
```python
(os_handle == INVALID_HANDLE_VALUE ||
      uv_set_pipe_handle(pipe->loop,
                         pipe,
                         os_handle,
                         file,
                         duplex_flags) == -
```
```python
1
```
```python
) {
```
```python
return
```
```python
UV_EINVAL;
  }
```
```python
//以链接模式初始化pipe
```
```python
uv_pipe_connection_init(pipe);
```
```python
if
```
```python
(pipe->ipc) {
    assert(!(pipe->flags & UV_HANDLE_NON_OVERLAPPED_PIPE));
    pipe->pipe.conn.ipc_pid = uv_parent_pid();
    assert(pipe->pipe.conn.ipc_pid != -
```
```python
1
```
```python
);
  }
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
设置pipe句柄
```python
static
```
```python
int
```
```python
uv_set_pipe_handle(uv_loop_t* loop,
                              uv_pipe_t* handle,
                              HANDLE pipeHandle,
```
```python
int
```
```python
fd,
                              DWORD duplex_flags) {
  NTSTATUS nt_status;
  IO_STATUS_BLOCK io_status;
  FILE_MODE_INFORMATION mode_info;
  DWORD mode = PIPE_READMODE_BYTE | PIPE_WAIT;
  DWORD current_mode =
```
```python
0
```
```python
;
  DWORD err =
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
(!(handle->flags & UV_HANDLE_PIPESERVER) &&
```
```python
//非服务端模式
```
```python
handle->handle != INVALID_HANDLE_VALUE)
```
```python
//句柄不为空
```
```python
return
```
```python
UV_EBUSY;
```
```python
if
```
```python
(!SetNamedPipeHandleState(pipeHandle, &mode, NULL, NULL)) {
    err = GetLastError();
```
```python
if
```
```python
(err == ERROR_ACCESS_DENIED) {
```
```python
/*
       * SetNamedPipeHandleState can fail if the handle doesn't have either
       * GENERIC_WRITE  or FILE_WRITE_ATTRIBUTES.
       * But if the handle already has the desired wait and blocking modes
       * we can continue.
       */
```
```python
if
```
```python
(!GetNamedPipeHandleState(pipeHandle, &current_mode, NULL, NULL,
                                   NULL, NULL,
```
```python
0
```
```python
)) {
```
```python
return
```
```python
-
```
```python
1
```
```python
;
      }
```
```python
else
```
```python
if
```
```python
(current_mode & PIPE_NOWAIT) {
        SetLastError(ERROR_ACCESS_DENIED);
```
```python
return
```
```python
-
```
```python
1
```
```python
;
      }
    }
```
```python
else
```
```python
{
```
```python
/* If this returns ERROR_INVALID_PARAMETER we probably opened
       * something that is not a pipe. */
```
```python
if
```
```python
(err == ERROR_INVALID_PARAMETER) {
        SetLastError(WSAENOTSOCK);
      }
```
```python
return
```
```python
-
```
```python
1
```
```python
;
    }
  }
```
```python
/* Check if the pipe was created with FILE_FLAG_OVERLAPPED. */
```
```python
nt_status = pNtQueryInformationFile(pipeHandle,
                                      &io_status,
                                      &mode_info,
```
```python
sizeof
```
```python
(mode_info),
                                      FileModeInformation);
```
```python
if
```
```python
(nt_status != STATUS_SUCCESS) {
```
```python
return
```
```python
-
```
```python
1
```
```python
;
  }
```
```python
if
```
```python
(mode_info.Mode & FILE_SYNCHRONOUS_IO_ALERT ||
      mode_info.Mode & FILE_SYNCHRONOUS_IO_NONALERT) {
```
```python
/* Non-overlapped pipe. */
```
```python
handle->flags |= UV_HANDLE_NON_OVERLAPPED_PIPE;
  }
```
```python
else
```
```python
{
```
```python
/* Overlapped pipe.  Try to associate with IOCP. */
```
```python
if
```
```python
(CreateIoCompletionPort(pipeHandle,
                               loop->iocp,
                               (ULONG_PTR)handle,
```
```python
0
```
```python
) == NULL) {
      handle->flags |= UV_HANDLE_EMULATE_IOCP;
    }
  }
  handle->handle = pipeHandle;
  handle->u.fd = fd;
  handle->flags |= duplex_flags;
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
#### 绑定
创建一个pipe服务端
```python
int
```
```python
uv_pipe_bind(uv_pipe_t* handle,
```
```python
const
```
```python
char
```
```python
* name) {
  uv_loop_t* loop = handle->loop;
```
```python
int
```
```python
i, err, nameSize;
  uv_pipe_accept_t* req;
```
```python
if
```
```python
(handle->flags & UV_HANDLE_BOUND) {
```
```python
return
```
```python
UV_EINVAL;
  }
```
```python
if
```
```python
(!name) {
```
```python
return
```
```python
UV_EINVAL;
  }
```
```python
if
```
```python
(!(handle->flags & UV_HANDLE_PIPESERVER)) {
    handle->pipe.serv.pending_instances = default_pending_pipe_instances;
  }
```
```python
//根据pending_instances 申请内部accept请求的内存
```
```python
handle->pipe.serv.accept_reqs = (uv_pipe_accept_t*)
    uv__malloc(
```
```python
sizeof
```
```python
(uv_pipe_accept_t) * handle->pipe.serv.pending_instances);
```
```python
if
```
```python
(!handle->pipe.serv.accept_reqs) {
    uv_fatal_error(ERROR_OUTOFMEMORY,
```
```python
"uv__malloc"
```
```python
);
  }
```
```python
//初始化内部accept
```
```python
for
```
```python
(i =
```
```python
0
```
```python
; i < handle->pipe.serv.pending_instances; i++) {
    req = &handle->pipe.serv.accept_reqs[i];
    uv_req_init(loop, (uv_req_t*) req);
    req->type = UV_ACCEPT;
    req->data = handle;
    req->pipeHandle = INVALID_HANDLE_VALUE;
    req->next_pending = NULL;
  }
```
```python
//将名字从utf8转到utf16
```
```python
nameSize = MultiByteToWideChar(CP_UTF8,
```
```python
0
```
```python
, name, -
```
```python
1
```
```python
, NULL,
```
```python
0
```
```python
) *
```
```python
sizeof
```
```python
(WCHAR);
  handle->name = (WCHAR*)uv__malloc(nameSize);
```
```python
if
```
```python
(!handle->name) {
    uv_fatal_error(ERROR_OUTOFMEMORY,
```
```python
"uv__malloc"
```
```python
);
  }
```
```python
if
```
```python
(!MultiByteToWideChar(CP_UTF8,
```
```python
0
```
```python
,
                           name,
                           -
```
```python
1
```
```python
,
                           handle->name,
                           nameSize /
```
```python
sizeof
```
```python
(WCHAR))) {
    err = GetLastError();
```
```python
goto
```
```python
error;
  }
```
```python
//通过FILE_FLAG_FIRST_PIPE_INSTANCE标记创建管道，如果失败，说明管道已经存在
```
```python
handle->pipe.serv.accept_reqs[
```
```python
0
```
```python
].pipeHandle = CreateNamedPipeW(handle->name,
      PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED |
      FILE_FLAG_FIRST_PIPE_INSTANCE,
      PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
      PIPE_UNLIMITED_INSTANCES,
```
```python
65536
```
```python
,
```
```python
65536
```
```python
,
```
```python
0
```
```python
, NULL);
```
```python
if
```
```python
(handle->pipe.serv.accept_reqs[
```
```python
0
```
```python
].pipeHandle == INVALID_HANDLE_VALUE) {
    err = GetLastError();
```
```python
if
```
```python
(err == ERROR_ACCESS_DENIED) {
      err = WSAEADDRINUSE;
```
```python
/* Translates to UV_EADDRINUSE. */
```
```python
}
```
```python
else
```
```python
if
```
```python
(err == ERROR_PATH_NOT_FOUND || err == ERROR_INVALID_NAME) {
      err = WSAEACCES;
```
```python
/* Translates to UV_EACCES. */
```
```python
}
```
```python
goto
```
```python
error;
  }
```
```python
//将新建的管道与uv_pipe_t联系起来
```
```python
if
```
```python
(uv_set_pipe_handle(loop,
                         handle,
                         handle->pipe.serv.accept_reqs[
```
```python
0
```
```python
].pipeHandle,
                         -
```
```python
1
```
```python
,
```
```python
0
```
```python
)) {
    err = GetLastError();
```
```python
goto
```
```python
error;
  }
  handle->pipe.serv.pending_accepts = NULL;
  handle->flags |= UV_HANDLE_PIPESERVER;
  handle->flags |= UV_HANDLE_BOUND;
```
```python
return
```
```python
0
```
```python
;
error:
```
```python
if
```
```python
(handle->name) {
    uv__free(handle->name);
    handle->name = NULL;
  }
```
```python
if
```
```python
(handle->pipe.serv.accept_reqs[
```
```python
0
```
```python
].pipeHandle != INVALID_HANDLE_VALUE) {
    CloseHandle(handle->pipe.serv.accept_reqs[
```
```python
0
```
```python
].pipeHandle);
    handle->pipe.serv.accept_reqs[
```
```python
0
```
```python
].pipeHandle = INVALID_HANDLE_VALUE;
  }
```
```python
return
```
```python
uv_translate_sys_error(err);
}
```
#### 链接命名管道
连接一个命名管道
```python
void
```
```python
uv_pipe_connect(uv_connect_t* req, uv_pipe_t* handle,
```
```python
const
```
```python
char
```
```python
* name, uv_connect_cb cb) {
  uv_loop_t* loop = handle->loop;
```
```python
int
```
```python
err, nameSize;
  HANDLE pipeHandle = INVALID_HANDLE_VALUE;
  DWORD duplex_flags;
```
```python
//初始化请求
```
```python
uv_req_init(loop, (uv_req_t*) req);
  req->type = UV_CONNECT;
  req->handle = (uv_stream_t*) handle;
  req->cb = cb;
```
```python
//utf8转为utf16
```
```python
nameSize = MultiByteToWideChar(CP_UTF8,
```
```python
0
```
```python
, name, -
```
```python
1
```
```python
, NULL,
```
```python
0
```
```python
) *
```
```python
sizeof
```
```python
(WCHAR);
  handle->name = (WCHAR*)uv__malloc(nameSize);
```
```python
if
```
```python
(!handle->name) {
    uv_fatal_error(ERROR_OUTOFMEMORY,
```
```python
"uv__malloc"
```
```python
);
  }
```
```python
if
```
```python
(!MultiByteToWideChar(CP_UTF8,
```
```python
0
```
```python
,
                           name,
                           -
```
```python
1
```
```python
,
                           handle->name,
                           nameSize /
```
```python
sizeof
```
```python
(WCHAR))) {
    err = GetLastError();
```
```python
goto
```
```python
error;
  }
```
```python
//打开已存在的管道
```
```python
pipeHandle = open_named_pipe(handle->name, &duplex_flags);
```
```python
if
```
```python
(pipeHandle == INVALID_HANDLE_VALUE) {
```
```python
if
```
```python
(GetLastError() == ERROR_PIPE_BUSY) {
```
```python
//管道存在，但是没有可用实例，向线程池注册一个任务，等待管道可用
```
```python
if
```
```python
(!QueueUserWorkItem(&pipe_connect_thread_proc,
                             req,
                             WT_EXECUTELONGFUNCTION)) {
        err = GetLastError();
```
```python
goto
```
```python
error;
      }
      REGISTER_HANDLE_REQ(loop, handle, req);
      handle->reqs_pending++;
```
```python
return
```
```python
;
    }
    err = GetLastError();
```
```python
goto
```
```python
error;
  }
  assert(pipeHandle != INVALID_HANDLE_VALUE);
```
```python
//将获取的管道与uv_pipe_t联系起来
```
```python
if
```
```python
(uv_set_pipe_handle(loop,
                         (uv_pipe_t*) req->handle,
                         pipeHandle,
                         -
```
```python
1
```
```python
,
                         duplex_flags)) {
    err = GetLastError();
```
```python
goto
```
```python
error;
  }
  SET_REQ_SUCCESS(req);
```
```python
//将请求添加到loop的请求队列
```
```python
uv_insert_pending_req(loop, (uv_req_t*) req);
  handle->reqs_pending++;
  REGISTER_HANDLE_REQ(loop, handle, req);
```
```python
return
```
```python
;
error:
```
```python
if
```
```python
(handle->name) {
    uv__free(handle->name);
    handle->name = NULL;
  }
```
```python
if
```
```python
(pipeHandle != INVALID_HANDLE_VALUE) {
    CloseHandle(pipeHandle);
  }
```
```python
/* Make this req pending reporting an error. */
```
```python
SET_REQ_ERROR(req, err);
  uv_insert_pending_req(loop, (uv_req_t*) req);
  handle->reqs_pending++;
  REGISTER_HANDLE_REQ(loop, handle, req);
```
```python
return
```
```python
;
}
```
uv_process_req中队connect请求的处理，最终会调用
```python
void
```
```python
uv_process_pipe_connect_req(uv_loop_t
```
```python
*
```
```python
loop
```
```python
, uv_pipe_t
```
```python
*
```
```python
handle
```
```python
,
    uv_connect_t
```
```python
*
```
```python
req) {
  int err;
  assert(
```
```python
handle
```
```python
->
```
```python
type
```
```python
==
```
```python
UV_NAMED_PIPE);
```
```python
//取消请求注册
```
```python
UNREGISTER_HANDLE_REQ(
```
```python
loop
```
```python
,
```
```python
handle
```
```python
, req);
```
```python
//调用回调函数
```
```python
if
```
```python
(req
```
```python
->
```
```python
cb) {
    err
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
(REQ_SUCCESS(req)) {
      uv_pipe_connection_init(
```
```python
handle
```
```python
);
    }
```
```python
else
```
```python
{
      err
```
```python
=
```
```python
GET_REQ_ERROR(req);
    }
    req
```
```python
->
```
```python
cb(req, uv_translate_sys_error(err));
  }
  DECREASE_PENDING_REQ_COUNT(
```
```python
handle
```
```python
);
}
```

