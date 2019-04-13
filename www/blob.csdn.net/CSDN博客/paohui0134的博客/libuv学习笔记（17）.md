
# libuv学习笔记（17） - paohui0134的博客 - CSDN博客


2016年06月26日 12:58:40[paohui0134](https://me.csdn.net/paohui0134)阅读数：845


# libuv学习笔记（17）
## uv_fs_event_t数据结构与相关函数
### 数据结构
```python
typedef
```
```python
struct
```
```python
uv_fs_event_s uv_fs_event_t;
```
```python
struct
```
```python
uv_fs_event_s {
  UV_HANDLE_FIELDS
```
```python
//uv_handle_t的成员
```
```python
/* private */
```
```python
char
```
```python
* path;
```
```python
//路径，utf8编码，由libuv申请、释放
```
```python
//UV_FS_EVENT_PRIVATE_FIELDS展开如下：
```
```python
struct
```
```python
uv_fs_event_req_s {                                                  
    UV_REQ_FIELDS                                                             
  } req;
```
```python
//请求
```
```python
HANDLE dir_handle;
```
```python
//文件夹句柄，通过CreateFileW获取
```
```python
int
```
```python
req_pending;
```
```python
//表计量，判断是否开始监听文件
```
```python
uv_fs_event_cb cb;
```
```python
//回调函数
```
```python
WCHAR* filew;/utf16文件名，由libuv申请、释放                                                                   
  WCHAR* short_filew;
```
```python
//utf16编码的短路径文件名，由libuv申请、释放
```
```python
WCHAR* dirw;
```
```python
//utf16编码的文件夹路径，由libuv申请、释放
```
```python
char
```
```python
* buffer;
```
```python
//存放监控返回的信息，由libuv申请、释放
```
```python
};
```
### 相关函数
#### 初始化
```python
int
```
```python
uv_fs_event_init(uv_loop_t* loop, uv_fs_event_t* handle) {
  uv__handle_init(loop, (uv_handle_t*) handle, UV_FS_EVENT);
```
```python
//初始化handle
```
```python
handle->dir_handle = INVALID_HANDLE_VALUE;
  handle->buffer = NULL;
  handle->req_pending =
```
```python
0
```
```python
;
  handle->filew = NULL;
  handle->short_filew = NULL;
  handle->dirw = NULL;
  uv_req_init(loop, (uv_req_t*)&handle->req);
```
```python
//初始化请求
```
```python
handle->req.type = UV_FS_EVENT_REQ;
```
```python
//请求类型
```
```python
handle->req.data = handle;
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
#### 开始event
```python
int
```
```python
uv_fs_event_start(uv_fs_event_t* handle,
                      uv_fs_event_cb cb,
```
```python
const
```
```python
char
```
```python
* path,
```
```python
unsigned
```
```python
int
```
```python
flags) {
```
```python
int
```
```python
name_size, is_path_dir;
  DWORD attr, last_error;
  WCHAR* dir = NULL, *dir_to_watch, *pathw = NULL;
  WCHAR short_path[MAX_PATH];
```
```python
if
```
```python
(uv__is_active(handle))
```
```python
//活动状态，返回
```
```python
return
```
```python
UV_EINVAL;
  handle->cb = cb;
  handle->path = uv__strdup(path);
```
```python
//复制路径
```
```python
if
```
```python
(!handle->path) {
    uv_fatal_error(ERROR_OUTOFMEMORY,
```
```python
"uv__malloc"
```
```python
);
  }
  uv__handle_start(handle);
```
```python
//设为活动状态
```
```python
//utf8转utf16   path  ------》 pathw
```
```python
......
```
```python
//判断路径是文件还是文件夹
```
```python
attr = GetFileAttributesW(pathw);
```
```python
if
```
```python
(attr == INVALID_FILE_ATTRIBUTES) {
    last_error = GetLastError();
```
```python
goto
```
```python
error;
  }
  is_path_dir = (attr & FILE_ATTRIBUTE_DIRECTORY) ?
```
```python
1
```
```python
:
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
(is_path_dir) {
```
```python
//路径是文件夹
```
```python
handle->dirw = pathw;
    dir_to_watch = pathw;
  }
```
```python
else
```
```python
{
```
```python
//路径是文件，获取文件名与路径
```
```python
//转换为短路径
```
```python
if
```
```python
(!GetShortPathNameW(pathw, short_path, ARRAY_SIZE(short_path))) {
      last_error = GetLastError();
```
```python
goto
```
```python
error;
    }
```
```python
//分割路径，获取路径与文件夹
```
```python
if
```
```python
(uv_split_path(pathw, &dir, &handle->filew) !=
```
```python
0
```
```python
) {
      last_error = GetLastError();
```
```python
goto
```
```python
error;
    }
```
```python
if
```
```python
(uv_split_path(short_path, NULL, &handle->short_filew) !=
```
```python
0
```
```python
) {
      last_error = GetLastError();
```
```python
goto
```
```python
error;
    }
    dir_to_watch = dir;
    uv__free(pathw);
```
```python
//释放
```
```python
pathw = NULL;
  }
```
```python
//获取文件夹句柄
```
```python
handle->dir_handle = CreateFileW(dir_to_watch,
                                   FILE_LIST_DIRECTORY,
                                   FILE_SHARE_READ | FILE_SHARE_DELETE |
                                     FILE_SHARE_WRITE,
                                   NULL,
                                   OPEN_EXISTING,
                                   FILE_FLAG_BACKUP_SEMANTICS |
                                     FILE_FLAG_OVERLAPPED,
                                   NULL);
```
```python
if
```
```python
(dir) {
```
```python
//在路径为文件的情况下释放文件夹路径
```
```python
uv__free(dir);
    dir = NULL;
  }
```
```python
//未能获取句柄，返回错误
```
```python
if
```
```python
(handle->dir_handle == INVALID_HANDLE_VALUE) {
    last_error = GetLastError();
```
```python
goto
```
```python
error;
  }
```
```python
//与iocp端口联系起来
```
```python
if
```
```python
(CreateIoCompletionPort(handle->dir_handle,
                             handle->loop->iocp,
                             (ULONG_PTR)handle,
```
```python
0
```
```python
) == NULL) {
    last_error = GetLastError();
```
```python
goto
```
```python
error;
  }
```
```python
//分配handle->buffer内存
```
```python
if
```
```python
(!handle->buffer) {
    handle->buffer = (
```
```python
char
```
```python
*)uv__malloc(uv_directory_watcher_buffer_size);
  }
```
```python
if
```
```python
(!handle->buffer) {
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
memset
```
```python
(&(handle->req.u.io.overlapped),
```
```python
0
```
```python
,
```
```python
sizeof
```
```python
(handle->req.u.io.overlapped));
```
```python
//异步监控文件夹
```
```python
if
```
```python
(!ReadDirectoryChangesW(handle->dir_handle,
                             handle->buffer,
                             uv_directory_watcher_buffer_size,
                             (flags & UV_FS_EVENT_RECURSIVE) ? TRUE : FALSE,
                             FILE_NOTIFY_CHANGE_FILE_NAME      |
                               FILE_NOTIFY_CHANGE_DIR_NAME     |
                               FILE_NOTIFY_CHANGE_ATTRIBUTES   |
                               FILE_NOTIFY_CHANGE_SIZE         |
                               FILE_NOTIFY_CHANGE_LAST_WRITE   |
                               FILE_NOTIFY_CHANGE_LAST_ACCESS  |
                               FILE_NOTIFY_CHANGE_CREATION     |
                               FILE_NOTIFY_CHANGE_SECURITY,
                             NULL,
                             &handle->req.u.io.overlapped,
                             NULL)) {
    last_error = GetLastError();
```
```python
goto
```
```python
error;
  }
  handle->req_pending =
```
```python
1
```
```python
;
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
(handle->path) {
    uv__free(handle->path);
    handle->path = NULL;
  }
```
```python
if
```
```python
(handle->filew) {
    uv__free(handle->filew);
    handle->filew = NULL;
  }
```
```python
if
```
```python
(handle->short_filew) {
    uv__free(handle->short_filew);
    handle->short_filew = NULL;
  }
  uv__free(pathw);
```
```python
if
```
```python
(handle->dir_handle != INVALID_HANDLE_VALUE) {
    CloseHandle(handle->dir_handle);
    handle->dir_handle = INVALID_HANDLE_VALUE;
  }
```
```python
if
```
```python
(handle->buffer) {
    uv__free(handle->buffer);
    handle->buffer = NULL;
  }
```
```python
return
```
```python
uv_translate_sys_error(last_error);
}
```
在uv_process_reqs中对于监控请求的处理会调用uv_process_fs_event_req
```python
void
```
```python
uv_process_fs_event_req(uv_loop_t* loop, uv_req_t* req,
    uv_fs_event_t* handle) {
  FILE_NOTIFY_INFORMATION* file_info;
```
```python
int
```
```python
err, sizew, size;
```
```python
char
```
```python
* filename = NULL;
  WCHAR* filenamew = NULL;
  WCHAR* long_filenamew = NULL;
  DWORD offset =
```
```python
0
```
```python
;
  assert(req->type == UV_FS_EVENT_REQ);
  assert(handle->req_pending);
  handle->req_pending =
```
```python
0
```
```python
;
```
```python
//标记量改为0，表示一个当前监听已被处理
```
```python
//非激活状态下，直接返回
```
```python
//closing状态下，调用uv_want_endgame，返回
```
```python
if
```
```python
(!uv__is_active(handle)) {
```
```python
if
```
```python
(handle->flags & UV__HANDLE_CLOSING) {
      uv_want_endgame(loop, (uv_handle_t*) handle);
    }
```
```python
return
```
```python
;
  }
```
```python
//获取文件变动信息
```
```python
file_info = (FILE_NOTIFY_INFORMATION*)(handle->buffer + offset);
```
```python
if
```
```python
(REQ_SUCCESS(req)) {
```
```python
//成功
```
```python
if
```
```python
(req->u.io.overlapped.InternalHigh >
```
```python
0
```
```python
) {
```
```python
do
```
```python
{
```
```python
//循环对每一个监听到的改变调用回调函数
```
```python
file_info = (FILE_NOTIFY_INFORMATION*)((
```
```python
char
```
```python
*)file_info + offset);
        assert(!filename);
        assert(!filenamew);
        assert(!long_filenamew);
```
```python
//如果监听的是文件夹，或者改变的文件名与监听的文件名匹配，那么调用回调
```
```python
if
```
```python
(handle->dirw ||
```
```python
//监听的是文件夹
```
```python
_wcsnicmp(handle->filew, file_info->FileName,
            file_info->FileNameLength /
```
```python
sizeof
```
```python
(WCHAR)) ==
```
```python
0
```
```python
||
          _wcsnicmp(handle->short_filew, file_info->FileName,
            file_info->FileNameLength /
```
```python
sizeof
```
```python
(WCHAR)) ==
```
```python
0
```
```python
) {
```
```python
if
```
```python
(handle->dirw) {
```
```python
//监听的是文件夹
```
```python
if
```
```python
(file_info->Action != FILE_ACTION_REMOVED &&
              file_info->Action != FILE_ACTION_RENAMED_OLD_NAME) {
```
```python
//构建文件的全路径
```
```python
size = wcslen(handle->dirw) +
                file_info->FileNameLength /
```
```python
sizeof
```
```python
(WCHAR) +
```
```python
2
```
```python
;
              filenamew = (WCHAR*)uv__malloc(size *
```
```python
sizeof
```
```python
(WCHAR));
```
```python
if
```
```python
(!filenamew) {
                uv_fatal_error(ERROR_OUTOFMEMORY,
```
```python
"uv__malloc"
```
```python
);
              }
              _snwprintf(filenamew, size, L
```
```python
"%s\\%.*s"
```
```python
, handle->dirw,
                file_info->FileNameLength /
```
```python
sizeof
```
```python
(WCHAR),
                file_info->FileName);
              filenamew[size -
```
```python
1
```
```python
] = L
```
```python
'\0'
```
```python
;
```
```python
//转换为长路径
```
```python
size = GetLongPathNameW(filenamew, NULL,
```
```python
0
```
```python
);
```
```python
if
```
```python
(size) {
                long_filenamew = (WCHAR*)uv__malloc(size *
```
```python
sizeof
```
```python
(WCHAR));
```
```python
if
```
```python
(!long_filenamew) {
                  uv_fatal_error(ERROR_OUTOFMEMORY,
```
```python
"uv__malloc"
```
```python
);
                }
                size = GetLongPathNameW(filenamew, long_filenamew, size);
```
```python
if
```
```python
(size) {
                  long_filenamew[size] =
```
```python
'\0'
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
                  uv__free(long_filenamew);
                  long_filenamew = NULL;
                }
              }
              uv__free(filenamew);
```
```python
if
```
```python
(long_filenamew) {
```
```python
/* Get the file name out of the long path. */
```
```python
uv_relative_path(long_filenamew,
                                 handle->dirw,
                                 &filenamew);
                uv__free(long_filenamew);
                long_filenamew = filenamew;
                sizew = -
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
{
```
```python
//无法获取长路径
```
```python
filenamew = file_info->FileName;
                sizew = file_info->FileNameLength /
```
```python
sizeof
```
```python
(WCHAR);
              }
            }
```
```python
else
```
```python
{
              filenamew = file_info->FileName;
              sizew = file_info->FileNameLength /
```
```python
sizeof
```
```python
(WCHAR);
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
//监控的是文件，直接使用监控的文件名
```
```python
filenamew = handle->filew;
            sizew = -
```
```python
1
```
```python
;
          }
```
```python
//将文件名转为utf8
```
```python
uv__convert_utf16_to_utf8(filenamew, sizew, &filename);
```
```python
switch
```
```python
(file_info->Action) {
```
```python
case
```
```python
FILE_ACTION_ADDED:
```
```python
case
```
```python
FILE_ACTION_REMOVED:
```
```python
case
```
```python
FILE_ACTION_RENAMED_OLD_NAME:
```
```python
case
```
```python
FILE_ACTION_RENAMED_NEW_NAME:
              handle->cb(handle, filename, UV_RENAME,
```
```python
0
```
```python
);
```
```python
break
```
```python
;
```
```python
case
```
```python
FILE_ACTION_MODIFIED:
              handle->cb(handle, filename, UV_CHANGE,
```
```python
0
```
```python
);
```
```python
break
```
```python
;
          }
          uv__free(filename);
          filename = NULL;
          uv__free(long_filenamew);
          long_filenamew = NULL;
          filenamew = NULL;
        }
        offset = file_info->NextEntryOffset;
      }
```
```python
while
```
```python
(offset && !(handle->flags & UV__HANDLE_CLOSING));
    }
```
```python
else
```
```python
{
      handle->cb(handle, NULL, UV_CHANGE,
```
```python
0
```
```python
);
    }
  }
```
```python
else
```
```python
{
    err = GET_REQ_ERROR(req);
    handle->cb(handle, NULL,
```
```python
0
```
```python
, uv_translate_sys_error(err));
  }
```
```python
if
```
```python
(!(handle->flags & UV__HANDLE_CLOSING)) {
```
```python
//不在关闭状态，继续下一次监听，也就是调用ReadDirectoryChangesW
```
```python
uv_fs_event_queue_readdirchanges(loop, handle);
  }
```
```python
else
```
```python
{
    uv_want_endgame(loop, (uv_handle_t*)handle);
  }
}
```
#### 停止监听
```python
int
```
```python
uv_fs_event_stop(uv_fs_event_t* handle) {
```
```python
if
```
```python
(!uv__is_active(handle))
```
```python
return
```
```python
0
```
```python
;
```
```python
//关闭句柄
```
```python
if
```
```python
(handle->dir_handle != INVALID_HANDLE_VALUE) {
    CloseHandle(handle->dir_handle);
    handle->dir_handle = INVALID_HANDLE_VALUE;
  }
```
```python
//停止handle
```
```python
uv__handle_stop(handle);
```
```python
//释放字符串
```
```python
if
```
```python
(handle->filew) {
    uv__free(handle->filew);
    handle->filew = NULL;
  }
```
```python
if
```
```python
(handle->short_filew) {
    uv__free(handle->short_filew);
    handle->short_filew = NULL;
  }
```
```python
if
```
```python
(handle->path) {
    uv__free(handle->path);
    handle->path = NULL;
  }
```
```python
if
```
```python
(handle->dirw) {
    uv__free(handle->dirw);
    handle->dirw = NULL;
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
uv_fs_event_t提供了对于文件的监控（文件修改、重命名等），可以看出，实际上是监视的文件所在的文件夹。整个流程相对于其他的异步事件要简单一些，特别是停止监视的流程。

