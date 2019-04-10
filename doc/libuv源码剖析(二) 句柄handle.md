# libuv源码剖析(二): 句柄handle

2018年08月26日 23:19:30 [WesleyWang97](https://me.csdn.net/yinanmo5569) 阅读数：118



## Introduction

handle 句柄就类似于事件, 需要被注册到 loop 中. 
在 libuv 中, 事件的基本类型都已经给出了定义, 共同”继承”了 `uv_handle_s`, 这里所说的继承, 是c语言通过 **-_** 来曲线救国实现的

## 抽象基类

抽象基类是 `uv_handle_t`:

```
/* The abstract base class of all handles. */
struct uv_handle_s {
  UV_HANDLE_FIELDS
};
1234
```

其中 `UV_HANDLE_FIELDS` 是宏定义, 里面还宏定义了 `UV_HANDLE_PRIVATE_FIELDS`, 后者字段的定义根据平台的不同而不一致, 这里将在 `unix` 平台下的展开后的定义给出:

```
struct uv_handle_s {
      /* public */
      void* data;                    //句柄携带的数据
      /* read-only */
      uv_loop_t* loop;              //句柄绑定的事件循环
      uv_handle_type type;          //句柄类型
      /* private */
      uv_close_cb close_cb;         //句柄close时的回调
      void* handle_queue[2];        //句柄队列节点
      union {
        int fd;                     //绑定的真实资源索引
        void* reserved[4];
      } u;                                              uv_handle_t* next_closing;    //下一个要被关闭的句柄
      unsigned int flags;           //句柄标识
};
123456789101112131415
```

`void* data` 用来携带任意用户定义的数据, 也就只有这个字段是 `public`的, 这个 public, private 不像c++有严格的限制, 直接使用 private 字段会有编译错误.

## 扩展类

libuv已经定义了很多 `uv_handle_s` 的 Deprived 类:

```
/* Handle types. */
typedef struct uv_loop_s uv_loop_t;
typedef struct uv_handle_s uv_handle_t;
typedef struct uv_stream_s uv_stream_t;
typedef struct uv_tcp_s uv_tcp_t;
typedef struct uv_udp_s uv_udp_t;
typedef struct uv_pipe_s uv_pipe_t;
typedef struct uv_tty_s uv_tty_t;
typedef struct uv_poll_s uv_poll_t;
typedef struct uv_timer_s uv_timer_t;
typedef struct uv_prepare_s uv_prepare_t;
typedef struct uv_check_s uv_check_t;
typedef struct uv_idle_s uv_idle_t;
typedef struct uv_async_s uv_async_t;
typedef struct uv_process_s uv_process_t;
typedef struct uv_fs_event_s uv_fs_event_t;
typedef struct uv_fs_poll_s uv_fs_poll_t;
typedef struct uv_signal_s uv_signal_t;
123456789101112131415161718
```

其实还有 handle 对应的 request, 由于实现方式与 handle 类似, 所以就不在这里再赘述了. 我们来看一个 uv_idle_t 是如何实现继承:

```
struct uv_idle_s {
  UV_HANDLE_FIELDS
  UV_IDLE_PRIVATE_FIELDS
};
1234
```

就是带上了 `UV_HANDLE_FIELDS` 标准字段, 然后再带上了 `UV_IDLE_PRIVATE_FIELDS` 自有的字段, 这样, 就实现了基本的继承关系.

这里来一张UML图来描述一下这种继承关系:

![这里写图片描述](https://img-blog.csdn.net/20180826231907616?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

非常感谢大牛的博客, 参考链接: 
<https://my.oschina.net/fileoptions/blog/1068810>