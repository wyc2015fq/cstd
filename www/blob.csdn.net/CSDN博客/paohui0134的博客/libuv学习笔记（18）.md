
# libuv学习笔记（18） - paohui0134的博客 - CSDN博客


2016年06月28日 22:57:52[paohui0134](https://me.csdn.net/paohui0134)阅读数：541标签：[libuv																](https://so.csdn.net/so/search/s.do?q=libuv&t=blog)[fs-poll																](https://so.csdn.net/so/search/s.do?q=fs-poll&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=libuv&t=blog)个人分类：[libuv学习笔记																](https://blog.csdn.net/paohui0134/article/category/6266738)



# libuv学习笔记（18）
## uv_fs_poll_t数据结构与相关函数
### 数据结构
```python
typedef
```
```python
struct
```
```python
uv_fs_poll_s uv_fs_poll_t;
```
```python
struct
```
```python
uv_fs_poll_s {
  UV_HANDLE_FIELDS
```
```python
//uv_handle_t成员
```
```python
/* Private, don't touch. */
```
```python
void
```
```python
* poll_ctx;
};
```
```python
//内部数据结构
```
```python
struct
```
```python
poll_ctx {
  uv_fs_poll_t* parent_handle;
```
```python
/* NULL if parent has been stopped or closed */
```
```python
int
```
```python
busy_polling;
```
```python
unsigned
```
```python
int
```
```python
interval;
  uint64_t start_time;
  uv_loop_t* loop;
  uv_fs_poll_cb poll_cb;
  uv_timer_t timer_handle;
```
```python
//定时器
```
```python
uv_fs_t fs_req;
```
```python
/* TODO(bnoordhuis) mark fs_req internal */
```
```python
uv_stat_t statbuf;
```
```python
char
```
```python
path[
```
```python
1
```
```python
];
```
```python
/* variable length */
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
uv_fs_poll_init(uv_loop_t* loop, uv_fs_poll_t* handle) {
```
```python
//简单的初始化handle
```
```python
uv__handle_init(loop, (uv_handle_t*)handle, UV_FS_POLL);
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
#### 开始监听
```python
int
```
```python
uv_fs_poll_start(uv_fs_poll_t* handle,
                     uv_fs_poll_cb cb,
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
interval) {
```
```python
struct
```
```python
poll_ctx* ctx;
  uv_loop_t* loop;
  size_t len;
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
(uv__is_active(handle))
```
```python
return
```
```python
0
```
```python
;
  loop = handle->loop;
  len =
```
```python
strlen
```
```python
(path);
  ctx = uv__calloc(
```
```python
1
```
```python
,
```
```python
sizeof
```
```python
(*ctx) + len);
```
```python
//分配内存，大小为结构体大小加上字符串长度
```
```python
if
```
```python
(ctx == NULL)
```
```python
return
```
```python
UV_ENOMEM;
  ctx->loop = loop;
  ctx->poll_cb = cb;
  ctx->interval = interval ? interval :
```
```python
1
```
```python
;
```
```python
//设置间隔
```
```python
ctx->start_time = uv_now(loop);
```
```python
//记录起始时间
```
```python
ctx->parent_handle = handle;
```
```python
//与uv_fs_poll_t联系起来
```
```python
memcpy
```
```python
(ctx->path, path, len +
```
```python
1
```
```python
);
```
```python
//路径
```
```python
//初始化定时器
```
```python
err = uv_timer_init(loop, &ctx->timer_handle);
```
```python
if
```
```python
(err <
```
```python
0
```
```python
)
```
```python
goto
```
```python
error;
  ctx->timer_handle.flags |= UV__HANDLE_INTERNAL;
```
```python
//内部使用handle
```
```python
uv__handle_unref(&ctx->timer_handle);
  err = uv_fs_stat(loop, &ctx->fs_req, ctx->path, poll_cb);
```
```python
if
```
```python
(err <
```
```python
0
```
```python
)
```
```python
goto
```
```python
error;
  handle->poll_ctx = ctx;
  uv__handle_start(handle);
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
  uv__free(ctx);
```
```python
return
```
```python
err;
}
```
```python
int
```
```python
uv_fs_stat(uv_loop_t* loop, uv_fs_t* req,
```
```python
const
```
```python
char
```
```python
* path, uv_fs_cb cb) {
```
```python
int
```
```python
err;
```
```python
//初始化请求
```
```python
uv_fs_req_init(loop, req, UV_FS_STAT, cb);
  err = fs__capture_path(req, path, NULL, cb != NULL);
```
```python
if
```
```python
(err) {
```
```python
return
```
```python
uv_translate_sys_error(err);
  }
```
```python
if
```
```python
(cb) {
```
```python
//QUEUE_FS_TP_JOB(loop, req);展开如下：
```
```python
do
```
```python
{                                                                   
      uv__req_register(loop, req);
```
```python
//将uv__fs_work添加到线程池，uv__fs_work中会查询文件状态，uv__fs_done会调用poll_cb，其中会比较上一次与本次的文件状态，若状态改变会调用start的回调函数。poll_cb最终会再次开启定时器
```
```python
uv__work_submit((loop), &(req)->work_req, uv__fs_work, uv__fs_done);   
    }
```
```python
while
```
```python
(
```
```python
0
```
```python
)
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
    fs__stat(req);
```
```python
return
```
```python
req->result;
  }
}
```
#### 停止监听
```python
int uv_fs_poll_stop(uv_fs_poll_t
```
```python
*
```
```python
handle
```
```python
) {
  struct poll_ctx
```
```python
*
```
```python
ctx;
```
```python
if
```
```python
(
```
```python
!
```
```python
uv__is_active(
```
```python
handle
```
```python
))
```
```python
return
```
```python
0
```
```python
;
  ctx
```
```python
=
```
```python
handle
```
```python
->
```
```python
poll_ctx;
  assert(ctx
```
```python
!=
```
```python
NULL
```
```python
);
  assert(ctx
```
```python
->
```
```python
parent_handle
```
```python
!=
```
```python
NULL
```
```python
);
  ctx
```
```python
->
```
```python
parent_handle
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
poll_ctx
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
//如果定时器为激活状态，那么关闭它
```
```python
if
```
```python
(uv__is_active(
```
```python
&
```
```python
ctx
```
```python
->
```
```python
timer_handle))
    uv_close((uv_handle_t
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
ctx
```
```python
->
```
```python
timer_handle, timer_close_cb);
  uv__handle_stop(
```
```python
handle
```
```python
);
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
整个文件状态轮询是通过线程池与定时器实现的
1.首先向线程池注册一个任务uv__fs_work，在这个任务中会查询对应文件的状态，结果保存在uv_fs_t的statbuf中。
2.uv__fs_work完成之后，会通过loop的wq_async向loop发送异步唤醒请求，此处通过uv_mutex_lock来达到线程同步
3.在loop所在线程中，处理唤醒请求，调用uv__fs_done
4.uv__fs_done最终调用poll_cb，在poll_cb中会判断上一次的文件状态（存在poll_ctx的statbuf中）与本次文件状态是否有区别，如果有区别，就会调用用户的回调函数，并将statbuf赋值给poll_ctx的statbuf。poll_cb最终会再次激活定时器
5.定时器的回调函数timer_cb会调用uv_fs_stat再次重复上面的流程
uv_fs_poll_t所用的定时器是没有设置重复间隔的，所以只会执行一次，每次执行完之后，会再次激活。
只有uv__fs_work函数也就是查询文件状态的函数会在线程池中执行，其他的回调函数都在loop所在的线程池中执行，以此来达到线程池安全。

