
# libuv学习(三) : Networking - Wesley's Blog - CSDN博客


2018年08月19日 23:24:39[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：86


## Introduction
把 libuv 核心原理弄懂之后(**异步机制, 事件驱动, loop, handle**), Network 部分其实也没什么难的了, libuv 相当于在 network 部分首先简化了一下系统的接口, 然后再加上了自己的异步机制.
下面重点介绍一下在libuv中的 tcp 和 udp. 除此之外, libuv 还提供了诸如`DNS`,`Network Interface`等接口方便开发. 就不在此赘述
## TCP
### TCP Server
分为以下几个步骤:
uv_tcp_init初始化TCP handle
uv_tcp_bind给 handle 绑定 addr
调用uv_listen, 开始监听, 并且在这个函数中声明了当有新 connection 到来时的回调函数
调用uv_accept接受 connection
使用Stream Operation(之前第二讲有提到过) 来与 Client 交流
**完整代码:**
`#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128
uv_loop_t *loop;
struct sockaddr_in addr;
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
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}
void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
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
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }
    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, NULL);
    }
}
int main() {
    loop = uv_default_loop();
    uv_tcp_t server;
    uv_tcp_init(loop, &server);
    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        return 1;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}`
### TCP Client
Client部分调用`uv_tcp_connect`就可以了, 并设置回调函数
`uv_tcp_t * socket = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
uv_tcp_init(loop, socket);
uv_connect_t * connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
struct sockaddr_in dest;
uv_ip4_addr("127.0.0.1", 80, &dest);
uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);`
## UDP
UDP是无连接协议, 所以libuv不是提供的 stream 的处理方式. 而是提供`uv_udp_t`handle (用于接收) 和`uv_udp_send_t`request (用于发送)
**udp-dhcp/main.c 完整代码**
`#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
uv_loop_t *loop;
uv_udp_t send_socket;
uv_udp_t recv_socket;
void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = malloc(suggested_size);
  buf->len = suggested_size;
}
void on_read(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const struct sockaddr *addr, unsigned flags) {
    if (nread < 0) {
        fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) req, NULL);
        free(buf->base);
        return;
    }
    char sender[17] = { 0 };
    uv_ip4_name((const struct sockaddr_in*) addr, sender, 16);
    fprintf(stderr, "Recv from %s\n", sender);
    // ... DHCP specific code
    unsigned int *as_integer = (unsigned int*)buf->base;
    unsigned int ipbin = ntohl(as_integer[4]);
    unsigned char ip[4] = {0};
    int i;
    for (i = 0; i < 4; i++)
        ip[i] = (ipbin >> i*8) & 0xff;
    fprintf(stderr, "Offered IP %d.%d.%d.%d\n", ip[3], ip[2], ip[1], ip[0]);
    free(buf->base);
    uv_udp_recv_stop(req);
}
uv_buf_t make_discover_msg() {
    uv_buf_t buffer;
    alloc_buffer(NULL, 256, &buffer);
    memset(buffer.base, 0, buffer.len);
    // BOOTREQUEST
    buffer.base[0] = 0x1;
    // HTYPE ethernet
    buffer.base[1] = 0x1;
    // HLEN
    buffer.base[2] = 0x6;
    // HOPS
    buffer.base[3] = 0x0;
    // XID 4 bytes
    buffer.base[4] = (unsigned int) random();
    // SECS
    buffer.base[8] = 0x0;
    // FLAGS
    buffer.base[10] = 0x80;
    // CIADDR 12-15 is all zeros
    // YIADDR 16-19 is all zeros
    // SIADDR 20-23 is all zeros
    // GIADDR 24-27 is all zeros
    // CHADDR 28-43 is the MAC address, use your own
    buffer.base[28] = 0xe4;
    buffer.base[29] = 0xce;
    buffer.base[30] = 0x8f;
    buffer.base[31] = 0x13;
    buffer.base[32] = 0xf6;
    buffer.base[33] = 0xd4;
    // SNAME 64 bytes zero
    // FILE 128 bytes zero
    // OPTIONS
    // - magic cookie
    buffer.base[236] = 99;
    buffer.base[237] = 130;
    buffer.base[238] = 83;
    buffer.base[239] = 99;
    // DHCP Message type
    buffer.base[240] = 53;
    buffer.base[241] = 1;
    buffer.base[242] = 1; // DHCPDISCOVER
    // DHCP Parameter request list
    buffer.base[243] = 55;
    buffer.base[244] = 4;
    buffer.base[245] = 1;
    buffer.base[246] = 3;
    buffer.base[247] = 15;
    buffer.base[248] = 6;
    return buffer;
}
void on_send(uv_udp_send_t *req, int status) {
    if (status) {
        fprintf(stderr, "Send error %s\n", uv_strerror(status));
        return;
    }
}
int main() {
    loop = uv_default_loop();
    uv_udp_init(loop, &recv_socket);
    struct sockaddr_in recv_addr;
    uv_ip4_addr("0.0.0.0", 68, &recv_addr);
    uv_udp_bind(&recv_socket, (const struct sockaddr *)&recv_addr, UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket, alloc_buffer, on_read);
    uv_udp_init(loop, &send_socket);
    struct sockaddr_in broadcast_addr;
    uv_ip4_addr("0.0.0.0", 0, &broadcast_addr);
    uv_udp_bind(&send_socket, (const struct sockaddr *)&broadcast_addr, 0);
    uv_udp_set_broadcast(&send_socket, 1);
    uv_udp_send_t send_req;
    uv_buf_t discover_msg = make_discover_msg();
    struct sockaddr_in send_addr;
    uv_ip4_addr("255.255.255.255", 67, &send_addr);
    uv_udp_send(&send_req, &send_socket, &discover_msg, 1, (const struct sockaddr *)&send_addr, on_send);
    return uv_run(loop, UV_RUN_DEFAULT);
}`本文简化翻译自[uvbook](https://github.com/nikhilm/uvbook)

