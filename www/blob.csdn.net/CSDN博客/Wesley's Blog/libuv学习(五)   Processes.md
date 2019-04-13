
# libuv学习(五) : Processes - Wesley's Blog - CSDN博客


2018年08月22日 21:15:35[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：216


## Introduction
对于**基于事件(event-based)**的程序来说, 有个限制是没办法很好地利用多核,提高CPU使用率.**即使能够使用多线程编程来分发 handle, 但是每个 loop 还是只有一个线程**.
这时候, 使用多进程就能够分担 loop 的压力. 并且通过**多进程 + 通信**的方法, 会比**多线程 + 共享内存**的方法更加安全, 易于开发.
## 创建进程
### 创建子进程
创建子进程通过`uv_spawn`实现, 通过`uv_process_option`传递进程args, 至于为什么不直接通过`uv_process_t`来传参, 可能是想保证 handle 的大致一致性吧, 如果就 process 这个 handle 有一堆乱七八糟的似乎也不太好.
举个例子, 创建一个子进程, 子进程工作是创建一个文件夹:
`#include <stdio.h>
#include <inttypes.h>
#include <uv.h>
uv_loop_t *loop;
uv_process_t child_req;
uv_process_options_t options;
void on_exit(uv_process_t *req, int64_t exit_status, int term_signal) {
    fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
    uv_close((uv_handle_t*) req, NULL);
}
int main() {
    loop = uv_default_loop();
    char* args[3];
    args[0] = "mkdir";
    args[1] = "test-dir";
    args[2] = NULL;
    options.exit_cb = on_exit;
    options.file = "mkdir";
    options.args = args;
    int r;
    if ((r = uv_spawn(loop, &child_req, &options))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return 1;
    } else {
        fprintf(stderr, "Launched process with ID %d\n", child_req.pid);
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}`可以看出, libuv几乎让所有事情都变成异步了,真是一切皆回调. 甚至都不放过创建子进程, 那么子进程创建之后是否还与原来进程的 loop 有关系呢? 应该是没关系的, 是一个新的命令创建出来的崭新的进程.
### 分离子进程
想要分离子进程很简单, 将`option`中的 flag 设置为`UV_PROCESS_DETACHED`就可以了. 这样子进程就与父进程脱离了关系.
`int main() {
    loop = uv_default_loop();
    char* args[3];
    args[0] = "sleep";
    args[1] = "100";
    args[2] = NULL;
    options.exit_cb = NULL;
    options.file = "sleep";
    options.args = args;
    options.flags = UV_PROCESS_DETACHED;
    int r;
    if ((r = uv_spawn(loop, &child_req, &options))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return 1;
    }
    fprintf(stderr, "Launched sleep with PID %d\n", child_req.pid);
    uv_unref((uv_handle_t*) &child_req);
    return uv_run(loop, UV_RUN_DEFAULT);
}`
## 进程间通信 ICP
IPC的话有很多种方式, 诸如 socket, pipe, signal, share memory 等等. libuv将IPC也做成了异步回调的方式.
libuv 包装了 pipe , 不过这里的 pipe 不是通常意义上的管道(linux 中的`|`), 而是`local domain socket`, 在初始化完成之后, 之后的处理也与 TCP 无差别
`#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
uv_loop_t *loop;
typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;
void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}
void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = malloc(suggested_size);
  buf->len = suggested_size;
}
void echo_write(uv_write_t *req, int status) {
    if (status < 0) {
        fprintf(stderr, "Write error %s\n", uv_err_name(status));
    }
    free_write_req(req);
}
void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
        req->buf = uv_buf_init(buf->base, nread);
        uv_write((uv_write_t*) req, client, &req->buf, 1, echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    }
    free(buf->base);
}
void on_new_connection(uv_stream_t *server, int status) {
    if (status == -1) {
        // error!
        return;
    }
    uv_pipe_t *client = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));
    uv_pipe_init(loop, client, 0);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, NULL);
    }
}
void remove_sock(int sig) {
    uv_fs_t req;
    uv_fs_unlink(loop, &req, "echo.sock", NULL);
    exit(0);
}
int main() {
    loop = uv_default_loop();
    uv_pipe_t server;
    uv_pipe_init(loop, &server, 0);
    signal(SIGINT, remove_sock);
    int r;
    if ((r = uv_pipe_bind(&server, "echo.sock"))) {
        fprintf(stderr, "Bind error %s\n", uv_err_name(r));
        return 1;
    }
    if ((r = uv_listen((uv_stream_t*) &server, 128, on_new_connection))) {
        fprintf(stderr, "Listen error %s\n", uv_err_name(r));
        return 2;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}`由于博文是用来介绍libuv的, 就不详细介绍IPC了. 总而言之, 就是在事件队列的基础上封装了包括创建进程, IPC等.
本文简化翻译自[uvbook](https://github.com/nikhilm/uvbook)

