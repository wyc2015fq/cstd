
# libuv学习(二) : FileSystem - Wesley's Blog - CSDN博客


2018年08月19日 20:55:18[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：125


## Introduction
简单的文件系统读写是通过`uv_fs_*`函数`uv_fs_t`结构体实现的.
libuv`FileSystem Operation`和`Socket Operation`操作不一样, socket 操作使用的是**异步非阻塞**的方法, 而`FileSystem Operation`在内部使用的是**阻塞的函数**, 但是是通过**异步的方法调用的**
## 读写文件
获取文件描述符:
int uv_fs_open(uv_loop_t * loop, uv_fs_t * req, const char * path, int flags, int mode, uv_fs_cb cb)
读取文件:
int uv_fs_read(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb)其中uv_file file一般填写的是打开之后的文件描述符. 在调用cb回调函数之前, 缓冲区会被自动填满. 如果 read 回调函数中req->result为0,则代表遇到了EOF
写文件:
int uv_fs_write(uv_loop_t* loop, uv_fs_t* req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb)
关闭文件:
int uv_fs_close(uv_loop_t * loop, uv_fs_t * req, uv_file file, uv_fs_cb cb)
回调函数函数签名:
void callback(uv_fs_t * req);
完整代码:
`void on_read(uv_fs_t *req);
uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;
static char buffer[8];
static uv_buf_t iov;
void on_write(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Write error: %s\n", uv_strerror((int)req->result));
    }
    else {
        fprintf(stdout, "[on_write] req->Result = %d\n", req->result);
        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
    }
}
void on_read(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
    }
    else if (req->result == 0) {
        fprintf(stdout, "[on_read] req->Result = %d\n", req->result);
        uv_fs_t close_req;
        // synchronous
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
    }
    else if (req->result > 0) {
        iov.len = req->result;
        fprintf(stdout, "[on_read] req->Result = %d\n", req->result);
        uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
    }
}
void on_open(uv_fs_t *req) {
    // The request passed to the callback is the same as the one the call setup
    // function was passed.
    assert(req == &open_req);
    if (req->result >= 0) {
        fprintf(stdout, "[on_open] req->Result = %d\n", req->result);
        iov = uv_buf_init(buffer, sizeof(buffer));
        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
    }
    else {
        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
    }
}
int main(int argc, char **argv) {
    uv_fs_open(uv_default_loop(), &open_req, argv[1], O_RDONLY, 0, on_open);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&read_req);
    uv_fs_req_cleanup(&write_req);
    return 0;
}`
## 文件系统操作
几乎所有的文件系统操作在 libuv 中都有对应的操作, 与read/write类似, 返回结果保存在了`uv_fs_t.result`中:
`UV_EXTERN int uv_fs_close(uv_loop_t* loop, uv_fs_t* req, uv_file file,
    uv_fs_cb cb);
UV_EXTERN int uv_fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path,
    int flags, int mode, uv_fs_cb cb);
UV_EXTERN int uv_fs_read(uv_loop_t* loop, uv_fs_t* req, uv_file file,
    void* buf, size_t length, int64_t offset, uv_fs_cb cb);
UV_EXTERN int uv_fs_unlink(uv_loop_t* loop, uv_fs_t* req, const char* path,
    uv_fs_cb cb);
UV_EXTERN int uv_fs_write(uv_loop_t* loop, uv_fs_t* req, uv_file file,
    void* buf, size_t length, int64_t offset, uv_fs_cb cb);
UV_EXTERN int uv_fs_mkdir(uv_loop_t* loop, uv_fs_t* req, const char* path,
    int mode, uv_fs_cb cb);
UV_EXTERN int uv_fs_rmdir(uv_loop_t* loop, uv_fs_t* req, const char* path,
    uv_fs_cb cb);
UV_EXTERN int uv_fs_readdir(uv_loop_t* loop, uv_fs_t* req,
    const char* path, int flags, uv_fs_cb cb);
UV_EXTERN int uv_fs_stat(uv_loop_t* loop, uv_fs_t* req, const char* path,
    uv_fs_cb cb);
UV_EXTERN int uv_fs_fstat(uv_loop_t* loop, uv_fs_t* req, uv_file file,
    uv_fs_cb cb);
UV_EXTERN int uv_fs_rename(uv_loop_t* loop, uv_fs_t* req, const char* path,
    const char* new_path, uv_fs_cb cb);
UV_EXTERN int uv_fs_fsync(uv_loop_t* loop, uv_fs_t* req, uv_file file,
    uv_fs_cb cb);
UV_EXTERN int uv_fs_fdatasync(uv_loop_t* loop, uv_fs_t* req, uv_file file,
    uv_fs_cb cb);
UV_EXTERN int uv_fs_ftruncate(uv_loop_t* loop, uv_fs_t* req, uv_file file,
    int64_t offset, uv_fs_cb cb);
UV_EXTERN int uv_fs_sendfile(uv_loop_t* loop, uv_fs_t* req, uv_file out_fd,
    uv_file in_fd, int64_t in_offset, size_t length, uv_fs_cb cb);
UV_EXTERN int uv_fs_chmod(uv_loop_t* loop, uv_fs_t* req, const char* path,
    int mode, uv_fs_cb cb);
UV_EXTERN int uv_fs_utime(uv_loop_t* loop, uv_fs_t* req, const char* path,
    double atime, double mtime, uv_fs_cb cb);
UV_EXTERN int uv_fs_futime(uv_loop_t* loop, uv_fs_t* req, uv_file file,
    double atime, double mtime, uv_fs_cb cb);
UV_EXTERN int uv_fs_lstat(uv_loop_t* loop, uv_fs_t* req, const char* path,
    uv_fs_cb cb);
UV_EXTERN int uv_fs_link(uv_loop_t* loop, uv_fs_t* req, const char* path,
    const char* new_path, uv_fs_cb cb);`
## 缓冲区与流(Buffers and Streams)
libuv 中基本的 I/O 工具是流`uv_stream_t`. TCP 套接字, UDP 套接字, 文件, 管道, 和进程间通信都可以作为 流 的子类.
流 (Streams) 通过每个子类特定的函数来初始化,然后可以通过如下函数进行操作:
`int uv_read_start(uv_stream_t*, uv_alloc_cb alloc_cb, uv_read_cb read_cb);
int uv_read_stop(uv_stream_t*);
int uv_write(uv_write_t* req, uv_stream_t* handle, uv_buf_t bufs[], int bufcnt, uv_write_cb cb);`基于流的函数比上面介绍的文件系统相关的函数更容易使用, libuv 在调用`uv_read_start`后会自动从流中读取数据, 直到调用了`uv_read_stop`.
完整代码:
`#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <uv.h>
typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;
uv_loop_t *loop;
uv_pipe_t stdin_pipe;
uv_pipe_t stdout_pipe;
uv_pipe_t file_pipe;
/* 分配释放内存 */
void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    *buf = uv_buf_init((char*) malloc(suggested_size), suggested_size);
}
void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}
/* 写完之后的回调函数 */
void on_stdout_write(uv_write_t *req, int status) {
    free_write_req(req);
}
void on_file_write(uv_write_t *req, int status) {
    free_write_req(req);
}
/* 这个相当于触发写事件, 调用 uv_write 来异步写*/
void write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb cb) {
    //write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
    //req->buf = uv_buf_init((char*) malloc(size), size);
    //memcpy(req->buf.base, buf.base, size);
    //uv_write((uv_write_t*) req, (uv_stream_t*)dest, &req->buf, 1, cb);
    uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));
    uv_buf_t *buffer = (uv_buf_t*) malloc(sizeof(uv_buf_t));
    *buffer = uv_buf_init((char*)malloc(size), size);
    memcpy(buffer->base, buf.base, size);
    uv_write(req, (uv_stream_t*)dest, buffer, 1, cb);
}
/* 读入数据时回调, 向file和stdout写输入内容 */
void read_stdin(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0){
        if (nread == UV_EOF){
            // end of file
            uv_close((uv_handle_t *)&stdin_pipe, NULL);
            uv_close((uv_handle_t *)&stdout_pipe, NULL);
            uv_close((uv_handle_t *)&file_pipe, NULL);
        }
    } else if (nread > 0) {
        write_data((uv_stream_t *)&stdout_pipe, nread, *buf, on_stdout_write);
        write_data((uv_stream_t *)&file_pipe, nread, *buf, on_file_write);
    }
    // OK to free buffer as write_data copies it.
    if (buf->base)
        free(buf->base);
}
int main(int argc, char **argv) {
    loop = uv_default_loop();
    uv_pipe_init(loop, &stdin_pipe, 0);
    uv_pipe_open(&stdin_pipe, 0);
    uv_pipe_init(loop, &stdout_pipe, 0);
    uv_pipe_open(&stdout_pipe, 1);
    uv_fs_t file_req;
    int fd = uv_fs_open(loop, &file_req, argv[1], O_CREAT | O_RDWR, 0644, NULL);
    uv_pipe_init(loop, &file_pipe, 0);
    uv_pipe_open(&file_pipe, fd);
    uv_read_start((uv_stream_t*)&stdin_pipe, alloc_buffer, read_stdin);
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}`
### libuv接口设计有些不合理的地方
libuv的接口设计还是有不合理之处的,以uvbook中`uvtee.c`为例,通过定义一个`write_req_t`结构体,然后在回调函数`on_stdout_write`中,通过`uv_write_t`结构体”私自夹带”了用户数据`buf`, 这是利用本身 c 语言的特性完成的. 也就相当于框架默许了在`request`之后可能会有 buf 数据,而是否有自己定, 而不是在函数中通过显示规定来设置, 这样来说对于接口的稳定性不是很有利.
`typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;
void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}
void on_stdout_write(uv_write_t *req, int status) {
    free_write_req(req);
}
void write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb cb) {
    write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
    req->buf = uv_buf_init((char*) malloc(size), size);
    memcpy(req->buf.base, buf.base, size);
    uv_write((uv_write_t*) req, (uv_stream_t*)dest, &req->buf, 1, cb);
    /*
    错误的写法, 如果这样写, buff的地址没办法传给回调函数, 也就没办法合理释放. 所以只能依赖上面夹带数据的方式传输给回调函数
    uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));
    uv_buf_t *buffer = (uv_buf_t*) malloc(sizeof(uv_buf_t));
    *buffer = uv_buf_init((char*)malloc(size), size);
    memcpy(buffer->base, buf.base, size);
    uv_write(req, (uv_stream_t*)dest, buffer, 1, cb);
    */
}`在大牛云风的博客中也有提到:[https://blog.codingnow.com/2012/01/libuv.html](https://blog.codingnow.com/2012/01/libuv.html)
## 监听文件改变
通过`uv_fs_event_t`来监听文件的改变.
例如以下代码,在文件发生改变(包括重命名或者文件变化, 可以监听所有子目录)时, 执行相应的命令:
`./onchange <command> <file1> [file2] ...`**onchange.c:**
`#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
uv_loop_t *loop;
const char *command;
void run_command(uv_fs_event_t *handle, const char *filename, int events, int status) {
    char path[1024];
    size_t size = 1023;
    // Does not handle error if path is longer than 1023.
    uv_fs_event_getpath(handle, path, &size);
    path[size] = '\0';
    fprintf(stderr, "Change detected in %s: ", path);
    if (events & UV_RENAME)
        fprintf(stderr, "renamed");
    if (events & UV_CHANGE)
        fprintf(stderr, "changed");
    fprintf(stderr, " %s\n", filename ? filename : "");
    system(command);
}
int main(int argc, char **argv) {
    if (argc <= 2) {
        fprintf(stderr, "Usage: %s <command> <file1> [file2 ...]\n", argv[0]);
        return 1;
    }
    loop = uv_default_loop();
    command = argv[1];
    while (argc-- > 2) {
        fprintf(stderr, "Adding watch on %s\n", argv[argc]);
        uv_fs_event_t *fs_event_req = malloc(sizeof(uv_fs_event_t));
        uv_fs_event_init(loop, fs_event_req);
        // The recursive flag watches subdirectories too.
        uv_fs_event_start(fs_event_req, run_command, argv[argc], UV_FS_EVENT_RECURSIVE);
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}`本文简化翻译自[uvbook](https://github.com/nikhilm/uvbook)

