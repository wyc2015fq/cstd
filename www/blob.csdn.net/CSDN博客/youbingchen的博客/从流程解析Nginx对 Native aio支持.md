# 从流程解析Nginx对 Native aio支持 - youbingchen的博客 - CSDN博客





2016年06月27日 16:12:18[youbingchen](https://me.csdn.net/youbingchen)阅读数：2153








强烈建议这篇博客和[从源码解析Nginx对 Native aio支持](http://blog.csdn.net/youbingchen/article/details/51767587)一起阅读

# aio(真正的异步IO)简介

Linux-native aio 比传统的 POSIX aio 功能更丰富一些，重要的一点是能通过内核加速提供高性能。直接用 Linux-native aio API 比较晦涩，为了方便使用和开发 Linux-native aio 应用程序我们可以用 libaio/libaio-devel 库。不过 nginx 的作者没有用这些库，因为 nginx 需要 eventfd()，而 libaio 库里只有 0.3.107 版本起才支持 eventfd；nginx 也没有用 glibc，因为 glibc 要到 2.8 版本才支持 eventfd()，为了减少对库的依赖性，nginx 干脆直接用 Linux-native aio API (system calls).这里说到了 eventfd()，eventfd 是 Linux-native aio 其中的一个 API，用来生成 file descriptors，这些 file descriptors 可为应用程序提供更高效 “等待／通知” 的事件机制。和 pipe 作用相似，但比 pipe 更好，一方面它只用到一个 file descriptor（pipe 要用两个），节省了内核资源；另一方面，eventfd 的缓冲区管理要简单得多，pipe 需要不定长的缓冲区，而 eventfd 全部缓冲只有定长 8 bytes. 

# 几种常见的aio

### gcc AIO

是通过阻塞IO+线程池来实现的。主要的几个函数是aio_read/aio_write/aio_return。 

优点：支持平台多，兼容性好，无需依赖第三方库，阻塞IO可以利用到操作系统的PageCache。 

缺点：据说有一些bug和陷阱，一直未解决
### Libeio

ibev的作者开发的AIO实现，与gcc aio类似也是使用阻塞IO+线程池实现的。优缺点类似上面

### Linux Native Aio

由操作系统内核提供的AIO。NativeAio是真正的AIO，完全非阻塞异步的，而不是用阻塞IO和线程池模拟。主要的几个系统调用为io_submit/io_setup/io_getevents。 

优点：由操作系统提供，读写操作可以直接投递到硬件，不会浪费CPU。 

缺点：仅支持Linux，必须使用DirectIO，所以无法利用到操作系统的PageCache。对于写文件来说nativeaio的作用不大，应为本身写文件就是先写到PageCache上，直接返回，没有IO等待。
# 异步I/O

> 
异步IO就是把IO提交给系统，让系统替你做，做完了再用某种方式通知你(通过信号，或者其他异步方式通知，这时候，操作系统已经帮你完成IO操作，具体来说就是你那个作为传入参数的的buffer的指针所指向的空间已经读到了数据或者你的buffer的数据已经写出去了）


# 非阻塞I/O

> 
非阻塞IO就是你要通过某种方式不定时地向系统询问你是否可以开始做某个IO(轮询啊)，当可以开始后，是要自己来完成IO(也就是说还要自己调用一次read来填充buffer或者write来不buffer的数据写出去)


# 小结

所以异步I/O和 阻塞I/O之间没有必然关系。 
**注意：Linux内核级别的文件异步I/O是不支持缓存操作的，也就是说，即使需要操作的文件块在Linux文件缓存存在，也不会通过读取、更改缓存中的文件块来代替实际对磁盘的操作。目前，Nginx仅支持在读取文件时使用异步I/O，因为正常写入文件时往往是写入内存中就 立刻返回，而使用异步I/O写入时速度会明显下降**

在Nginx中，文件异步I/O事件完成的通知也集成到epoll中，是通过IOCB_FLAG_RESFD标志位完成的。

# 流程

![nginx_aio](https://img-blog.csdn.net/20160627161112696)

> 
ngx_epoll_aio_init方法会把 异步I/O与epoll结合起来，当某一个异步事件完成后，ngx_eventfd句柄就处于可用的状态，这样epoll_wait在返回ngx_eventfd_event事件后就会调用它的 回调方法ngx_epoll_eventfd_handler处理已经完成的异步I/O事件 
![nginx_aio2](https://img-blog.csdn.net/20160627161151713)

  那么，怎样向异步I/O上下文中提交异步I/O操作，就是利用ngx_linux_aio.read.c文件中的ngx_file_aio_read方法，在打开文件异步操作之后，这个方法将会负责磁盘文件的读取。
```cpp
// 文件名 ngx_linux_aio_read.c
ssize_t
ngx_file_aio_read(ngx_file_t *file, u_char *buf, size_t size, off_t offset,
    ngx_pool_t *pool)
{
    ngx_err_t         err;
    struct iocb      *piocb[1];
    ngx_event_t      *ev;
    ngx_event_aio_t  *aio;

    if (!ngx_file_aio) {
        return ngx_read_file(file, buf, size, offset);
    }

    if (file->aio == NULL && ngx_file_aio_init(file, pool) != NGX_OK) {
        return NGX_ERROR;
    }

    aio = file->aio;
    ev = &aio->event;

    if (!ev->ready) {
        ngx_log_error(NGX_LOG_ALERT, file->log, 0,
                      "second aio post for \"%V\"", &file->name);
        return NGX_AGAIN;
    }

    ngx_log_debug4(NGX_LOG_DEBUG_CORE, file->log, 0,
                   "aio complete:%d @%O:%uz %V",
                   ev->complete, offset, size, &file->name);

    if (ev->complete) {
        ev->active = 0;
        ev->complete = 0;

        if (aio->res >= 0) {
            ngx_set_errno(0);
            return aio->res;
        }

        ngx_set_errno(-aio->res);

        ngx_log_error(NGX_LOG_CRIT, file->log, ngx_errno,
                      "aio read \"%s\" failed", file->name.data);

        return NGX_ERROR;
    }

    ngx_memzero(&aio->aiocb, sizeof(struct iocb));
    /*这里设置的aiocb的成员就是iocb类型，aio_data设置为这个ngx_event_t事件的指针，这样，从io_getevents方法获取的io_event对象中data也是这个指针*/
    aio->aiocb.aio_data = (uint64_t) (uintptr_t) ev;
    aio->aiocb.aio_lio_opcode = IOCB_CMD_PREAD;
    aio->aiocb.aio_fildes = file->fd;
    aio->aiocb.aio_buf = (uint64_t) (uintptr_t) buf;
    aio->aiocb.aio_nbytes = size;
    aio->aiocb.aio_offset = offset;
    aio->aiocb.aio_flags = IOCB_FLAG_RESFD;
    aio->aiocb.aio_resfd = ngx_eventfd;
   /*设置事件的回调函数为 ngx_file_aio_event_handler，它的调用关系是这样 :epoll_wait中调用ngx_epoll_eventfd_handler，方法将当前事件放入到ngx_posted_events队列，在延后执行的队列 中 调用ngx_file_aio_event_handler方法*/
    ev->handler = ngx_file_aio_event_handler;

    piocb[0] = &aio->aiocb;
   /*调用io_submit向ngx_aio_ctx异步I/O上下文中添加1个事件，返回1表示成功*/
    if (io_submit(ngx_aio_ctx, 1, piocb) == 1) {
        ev->active = 1;
        ev->ready = 0;
        ev->complete = 0;

        return NGX_AGAIN;
    }

    err = ngx_errno;

    if (err == NGX_EAGAIN) {
        return ngx_read_file(file, buf, size, offset);
    }

    ngx_log_error(NGX_LOG_CRIT, file->log, err,
                  "io_submit(\"%V\") failed", &file->name);

    if (err == NGX_ENOSYS) {
        ngx_file_aio = 0;
        return ngx_read_file(file, buf, size, offset);
    }

    return NGX_ERROR;
}
```

ngx_event_aio_t 结构体的定义

```cpp
// 文件名 ngx_event.h
struct ngx_event_aio_s {
    void                      *data;
    // 这是真正由业务模块实现的方法，在异步I/O事件完成后调用
    ngx_event_handler_pt       handler;
    ngx_file_t                *file;

#if (NGX_HAVE_AIO_SENDFILE)
    ssize_t                  (*preload_handler)(ngx_buf_t *file);
#endif

    ngx_fd_t                   fd;

#if (NGX_HAVE_EVENTFD)
    int64_t                    res;
#endif

#if !(NGX_HAVE_EVENTFD) || (NGX_TEST_BUILD_EPOLL)
    ngx_err_t                  err;
    size_t                     nbytes;
#endif

    ngx_aiocb_t                aiocb;
    ngx_event_t                event;
};
```

> 
ngx_file_aio_read方法会向异步I/O上下文添加事件，epoll_wait在通过 ngx_eventfd描述符 检测到异步I/O事件后，会调用ngx_epoll_eventfd_handler方法将io_event事件取出来，放入ngx_posted_events队列中延后执行。ngx_posted_events队列 中 的事件执行时，则会调用ngx_file_aio_event_handler方法 。


```cpp
static void
ngx_file_aio_event_handler(ngx_event_t *ev)
{
    ngx_event_aio_t  *aio;

    aio = ev->data;

    ngx_log_debug2(NGX_LOG_DEBUG_CORE, ev->log, 0,
                   "aio event handler fd:%d %V", aio->fd, &aio->file->name);

    aio->handler(ev);
}
```

> 
这里 调用了 ngx_event_aio_t的结构体的handler回调的办法，这个 回调方法是由真正 的业务模块实现的 。也就是，任意一个业务模块要想使用文件异步I/O，就可以实现handler 方法，这样在文件 异步操作完成后，该方法就会被回调。


对于http模块aio->handler回调指向函数ngx_http_copy_aio_event_handler(),而后续的流程主要将aio从磁盘读到缓存的数据发送到最终的客户端 

ngx_http_copy_aio_event_handler() -> ngx_http_request_handler() -> ngx_http_writer() -> ngx_http_output_filter() -> ngx_http_top_body_filter()







