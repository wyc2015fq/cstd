# Libevent参考手册第七章：Evbuffers:用于缓冲IO的实用工具 - u013366022的专栏 - CSDN博客
2016年11月30日 15:22:03[slitaz](https://me.csdn.net/u013366022)阅读数：439
Libevent的evbuffer功能通过实现一个字节队列，在队列末尾添加数据，在队列头移除数据。
Evbuffers 是通常用来做缓冲网络 IO 的"缓冲区"部分。他们将不会提供安排 IO 或IO准备就绪时的触发等功能： 而这些功能是bufferevents应该做的。
本章中的函数声明都在event2/buffer.h 中，除非另有说明。
## 创建或释放 evbuffer
**接口**
**struct** evbuffer *evbuffer_new(**void**);
**void** evbuffer_free(**struct** evbuffer
 *buf);
这些函数应该是比较清晰： evbuffer_new() 分配并返回一个新的空的 evbuffer ， evbuffer_free() 函数删除所有的内容。
这些函数从Libevent 0.8版本开始存在。     
## Evbuffers 和线程安全
**接口**
**int** evbuffer_enable_locking(**struct** evbuffer *buf, **void** *lock);**void** evbuffer_lock(**struct** evbuffer *buf);**void** evbuffer_unlock(**struct** evbuffer *buf);
默认情况下，多线程同时访问evbuffer是不安全的。如果您需要执行此操作，您可以在evbuffer上调用 evbuffer_enable_locking()函数。如果此函数的*lock*参数为NULL，则 Libevent 通过evthread_set_lock_creation_callback函数分配一把新锁。否则，它所使用的参数作为该锁。
Evbuffer_lock() 和 evbuffer_unlock() 函数的功能分别是获得或者释放evbuffer 上的锁。您可以使用它们进行一组原子操作。如果evbuffer上没有使用锁，这些函数什么也不做。
（请注意您不需要调用 evbuffer_lock() 和 evbuffer_unlock() 在单个操作上： 单个操作已经是原子操作了。当你有一个以上的操作需要执行时，你只需手动锁定evbuffer，而不需要引入另一个线程。）
这些函数在Libevent 2.0.1-alpha版本中引入。
## 检查 evbuffer
**接口**
size_t evbuffer_get_length(**const****struct** evbuffer *buf);
此函数返回存储在 evbuffer 中的字节的数。
它是在Libevent 2.0.1-alpha版本中引入。
**接口**
size_t evbuffer_get_contiguous_space(**const****struct** evbuffer *buf);
此函数返回evbuffer的前面的连续存储的字节数。在 evbuffer 中的字节可能存储在多个单独的大块的内存 ；此函数返回*第一个*文本块中当前存储的字节的数。
它是在Libevent 2.0.1-alpha版本中引入。
### 将数据添加到 evbuffer： 基础知识
**接口**
**int** evbuffer_add(**struct** evbuffer *buf, **const****void** *data, size_t datlen);
此函数把存储在data中的datlen个字节的数据追加到buf上，成功返回0，失败返回-1。
**接口**
**int** evbuffer_add_printf(**struct** evbuffer *buf, **const****char** *fmt, ...)**int** evbuffer_add_vprintf(**struct** evbuffer *buf, **const****char** *fmt, va_list ap);
这些函数把格式化得数据追加到*buf*的末尾。处理格式参数和其他余下的参数与C 库中"printf"和"vprintf"函数相似。这些函数将返回附加的字节的数。
**接口**
**int** evbuffer_expand(**struct** evbuffer *buf, size_t datlen);
此函数改变内存缓冲区中的最后一个区块，或添加新的区块，以致现在的缓冲区足够大能够容纳 datlen个字节无需再次进行分配。
**示例**
*/* Here are two ways to add "Hello world 2.0.1" to a buffer. */**/* Directly: */*evbuffer_add(buf, "Hello world 2.0.1", 17);*/* Via printf: */*evbuffer_add_printf(buf, "Hello %s %d.%d.%d", "world", 2, 0, 1);
Evbuffer_add() 和 evbuffer_add_printf() 函数在 Libevent 0.8中引入的。evbuffer_expand() 是在 0.9， evbuffer_add_printf() 首次出现在 Libevent 1.1。
## 将数据从一个 evbuffer 移动到另一个
为了提高效率，Libevent 已经优化了用于将数据从一个 evbuffer 移动到另一个evbuffer的函数。
**接口**
**int** evbuffer_add_buffer(**struct** evbuffer *dst, **struct** evbuffer *src);**int** evbuffer_remove_buffer(**struct** evbuffer *src, **struct** evbuffer *dst,    size_t datlen);
Evbuffer_add_buffer() 函数将从*src*的所有的数据移动到*dst*的尾端。成功返回0失败则返回-1。
*Dst*，复制尽可能少的末尾，evbuffer_remove_buffer() 函数移动*src*中datlen个字节的数据到Dst的末尾，尽量少复制。它返回移动的字节的数。
我们推出了 在Libevent 0.8版本中引入evbuffer_add_buffer()函数，evbuffer_remove_buffer() 是 Libevent 2.0.1-alpha 中的新功能。
## 将数据添加到前面的 evbuffer
**接口**
**int** evbuffer_prepend(**struct** evbuffer *buf, **const****void** *data, size_t size);**int** evbuffer_prepend_buffer(**struct** evbuffer *dst, **struct** evbuffer* src);
这些功能的工作方式同evbuffer_add() 和 evbuffer_add_buffer() 一样，只是他们将数据移动到目标缓冲区的*前面*。
应谨慎使用这些函数，决不与一bufferevent共享一evbuffer。他们是 Libevent 2.0.1-alpha 中的新功能。
## 重新排列 evbuffer 的内部的布局
有时你想偷看 evbuffer前N个字节的数据，并将其视为连续的字节数组。为此，您必须首先确保前面的缓冲区真的连续。
**接口**
**unsigned****char** *evbuffer_pullup(**struct** evbuffer *buf, ev_ssize_t size);
evbuffer_pullup() 函数"线性化"*buf**的前*size个字节，复制或移动它们，以确保它们都连续和占用的内存的同一块。如果参数*size*为负数，则函数 “线性化” 整个缓冲区。如果*参数size*大于缓冲区中的字节数，则此函数返回 NULL。否则，evbuffer_pullup() 返回 buf 中第一个字节的指针。
，若参数size比较大，调用 evbuffer_pullup()的运行速度可能会较慢，因为它可能需要将整个缓冲区的内容复制。
**示例**
#include <event2/buffer.h>#include <event2/util.h>#include <string.h>**int** parse_socks4(**struct** evbuffer *buf, ev_uint16_t *port, ev_uint32_t *addr){*    /* Let's parse the start of a SOCKS4 request!  The format is easy:**     * 1 byte of version, 1 byte of command, 2 bytes destport, 4 bytes of**     * destip. */***unsigned****char** *mem;    mem = evbuffer_pullup(buf, 8);**if** (mem == NULL) {*        /* Not enough data in the buffer */***return** 0;    } **else****if** (mem[0] != 4 || mem[1] != 1) {*        /* Unrecognized protocol or command */***return** -1;    } **else** {        memcpy(port, mem+2, 2);        memcpy(addr, mem+4, 4);        *port = ntohs(*port);        *addr = ntohl(*addr);*        /* Actually remove the data from the buffer now that we know we**           like it. */*        evbuffer_drain(buf, 8);**return** 1;    }}
**请注意**
若evbuffer_pullup() 中的参数size等于evbuffer_get_contiguous_space() 所返回的值，则调用evbuffer_pullup()函数不会导致任何数据被复制或者移动。
Evbuffer_pullup() 函数是 Libevent 2.0.1-alpha 中的新功能： 以前版本的 Libevent 不管的任何成本都始终保持 evbuffer 数据的连续性。
## 从 evbuffer 中删除数据
**接口**
**int** evbuffer_drain(**struct** evbuffer *buf, size_t len);**int** evbuffer_remove(**struct** evbuffer *buf, **void** *data, size_t datlen);
evbuffer_remove() 函数将复制和删除前面的*buf*的datlen个字节到内存中*的data*中。如果少于*datlen*个字节可用，函数将复制所有的字节。失败返回的值是-1，否则返回复制的字节数。
evbuffer_drain() 函数跟evbuffer_remove()的功能相似，不同之处在于它不会复制数据： 它只是移除缓冲区的前面数据。成功返回0，失败返回-1。
Libevent0.8首次引入Evbuffer_drain()函数，evbuffer_remove() 函数首次出现在 Libevent 0.9。
## 从 evbuffer中复制的数据
有时，您希望无丢失数据的获取缓冲区的数据副本。例如，您可能要查看某种完整记录是否已到达，无丢失任何数据 （如 evbuffer_remove 会做），或重新排列内部缓冲区（如 evbuffer_pullup() 会做的）
**接口**
ev_ssize_t evbuffer_copyout(**struct** evbuffer *buf, **void** *data, size_t datlen);
Evbuffer_copyout() 的行为就像 evbuffer_remove()，但不会丢失缓冲区中的任何数据。复制前面的*buf*的datlen个字节到内存中*的data*中。如果少于*datlen*个字节可用，函数将复制所有的字节。败返回的值是-1，否则返回复制的字节数。
如果从缓冲区中复制数据速度太慢，请使用 evbuffer_peek()函数代替。
**示例**
#include <event2/buffer.h>
#include <event2/util.h>
#include <stdlib.h>
#include <stdlib.h>
**int** get_record(**struct** evbuffer *buf,
 size_t *size_out, **char** **record_out)
{
*    /* Let's assume that we're speaking some protocol where records*
*       contain a 4-byte size field in network order, followed by that*
*       number of bytes.  We will return 1 and set the 'out' fields if we*
*       have a whole record, return 0 if the record isn't here yet, and*
*       -1 on error.  */*
    size_t buffer_len = evbuffer_get_length(buf);
    ev_uint32_t record_len;
**char** *record;
**if** (buffer_len < 4)
**return** 0; /* The size field hasn't arrived. */
*   /* We use evbuffer_copyout here so that the size field will stay on*
*       the buffer for now. */*
    evbuffer_copyout(buf, &record_len, 4);
*    /* Convert len_buf into host order. */*
    record_len = ntohl(record_len);
**if** (buffer_len < record_len + 4)
**return** 0; /* The record hasn't arrived */
*    /* Okay, _now_ we can remove the record. */*
    record = malloc(record_len);
**if** (record == NULL)
**return** -1;
    evbuffer_drain(buf, 4);
    evbuffer_remove(buf, record, record_len);
    *record_out = record;
    *size_out = record_len;
**return** 1;
}
此函数首次出现在 Libevent 2.0.5-alpha。
## 面向行输入
**接口**
**enum** evbuffer_eol_style {        EVBUFFER_EOL_ANY,        EVBUFFER_EOL_CRLF,        EVBUFFER_EOL_CRLF_STRICT,        EVBUFFER_EOL_LF};**char** *evbuffer_readln(**struct** evbuffer *buffer, size_t *n_read_out,**enum** evbuffer_eol_style eol_style);
很多的互联网协议使用基于行的格式。Evbuffer_readln() 函数提取一个evbuffer前行，并返回一个新分配的NULL结尾的字符串。如果*n_read_out*不为空，**n_read_out*设置为返回的字符串中的字节数。如果不是读的整行，此函数将返回 NULL。复制字符串中不包含行结束符。
Evbuffer_readln() 函数了解4 种行终止格式：
**EVBUFFER_EOL_LF**
行的结尾是一个单一的换行符。（这也就是被称为的"/n"。它的ASCII 值是 0x0A）。
EVBUFFER_EOL_CRLF_STRICT
行的结尾是单个回车,后跟一个换行符。（这也被称为是"/r/n"。ASCII 值是 0x0D 0x0A)。
EVBUFFER_EOL_CRLF
行的结尾是可选的回车，后跟一个换行符。（也就是说，它是"/r/n"或"/n"。这种格式可解析基于文本的互联网协议，因为标准一般规定以"/r/n"终结，但非一致的客户端有时可能只是"/n"。
EVBUFFER_EOL_ANY
行的结尾是任意数量的回车和换行字符中的任何序列。这种格式不是很有用的；它的存在主要是为了向后兼容。
（请注意如果你使用 event_set_mem_functions() 来重写默认的malloc， evbuffer_readln()函数返回的字符串将由你指定的代替malloc方法分配。
**示例**
**char** *request_line;size_t len;request_line = evbuffer_readln(buf, &len, EVBUFFER_EOL_CRLF);**if** (!request_line) {*    /* The first line has not arrived yet. */*} **else** {**if** (!strncmp(request_line, "HTTP/1.0 ", 9)) {*        /* HTTP 1.0 detected ... */*    }    free(request_line);}
Libevent 1.4.14-stable及更高版本可用evbuffer_readln() 接口。
## 在 evbuffer 中搜索
Evbuffer_ptr 结构指向 evbuffer 内的一个位置，并包含可用于循环访问 evbuffer 的数据。
**接口**
**struct** evbuffer_ptr {        ev_ssize_t pos;**struct** {*                /* internal fields */*        } _internal;};
*Pos*字段是公共字段 ；其他部分不应由用户代码使用。它指示 evbuffer 中的从开始偏移offset的位置。
**接口**
**struct** evbuffer_ptr evbuffer_search(**struct** evbuffer *buffer,**const****char** *what, size_t len, **const****struct** evbuffer_ptr *start);**struct** evbuffer_ptr evbuffer_search_range(**struct** evbuffer *buffer,**const****char** *what, size_t len, **const****struct** evbuffer_ptr *start,**const****struct** evbuffer_ptr *end);**struct** evbuffer_ptr evbuffer_search_eol(**struct** evbuffer *buffer,**struct** evbuffer_ptr *start, size_t *eol_len_out,**enum** evbuffer_eol_style eol_style);
Evbuffer_search() 函数扫描的缓冲区寻找*长度为len*字符串*what*的匹配项。它返回包含字符串位置的evbuffer_ptr，如果找不到，则返回-1。如果提供了*start*参数，则搜索的起始位置为start；否则，搜索将从字符串的开头开始。
Evbuffer_search_range() 函数的行为类似evbuffer_search（）函数，不同之处在于它的搜索范围为start与end，在这区间搜索匹配what的字符串，并返回字符串的位置。
Evbuffer_search_eol() 函数检测到行尾，与evbuffer_readln()相似，不同之处是它不是复制一行到输出端，而返回包含行尾字符 (s) 的开始位置的evbuffer_ptr。如果eol_len_out 为非空，它将被设置为终止字符串的长度。
**接口**
**enum** evbuffer_ptr_how {        EVBUFFER_PTR_SET,        EVBUFFER_PTR_ADD};**int** evbuffer_ptr_set(**struct** evbuffer *buffer, **struct** evbuffer_ptr *pos,    size_t position, **enum** evbuffer_ptr_how how);
Evbuffer_ptr_set 函数操作的*buffer*内的*pos*位置。如果*how*是 EVBUFFER_PTR_SET，将指针移到缓冲区中的绝对位置*position*。如果它是EVBUFFER_PTR_ADD，指针将向前移动*postion*字节。此函数成功则返回 0，失败则返回-1。
**示例**
#include <event2/buffer.h>#include <string.h>*/* Count the total occurrences of 'str' in 'buf'. */***int** count_instances(**struct** evbuffer *buf, **const****char** *str){    size_t len = strlen(str);**int** total = 0;**struct** evbuffer_ptr p;**if** (!len)*        /* Don't try to count the occurrences of a 0-length string. */***return** -1;    evbuffer_ptr_set(buf, &p, 0, EVBUFFER_PTR_SET);**while** (1) {         p = evbuffer_search(buf, str, len, &p);**if** (p.pos < 0)**break**;         total++;         evbuffer_ptr_set(buf, &p, 1, EVBUFFER_PTR_ADD);    }**return** total;}
**警告**
修改 evbuffer 或其布局的任何调用会使evbuffer_ptr 的所有值都无效，并使他们不能安全使用。
这些接口都是首次在Libevent 2.0.1-alpha中实现。
## 检测数据而不复制
有时，您想要读取 evbuffer 中的数据，而不将它复制出 （如 evbuffer_copyout()），和并不想重新布局 evbuffer 的内部内存（如 evbuffer_pullup()）。有时您可能要查看evbuffer中间的数据。
您可以执行此操作：
**接口**
**struct** evbuffer_iovec {**void** *iov_base;        size_t iov_len;};**int** evbuffer_peek(**struct** evbuffer *buffer, ev_ssize_t len,**struct** evbuffer_ptr *start_at,**struct** evbuffer_iovec *vec_out, **int** n_vec);
当您调用 evbuffer_peek() 时，你给它evbuffer_iovec 结构数组*vec_out*参数。数组的长度为*n_vec*。它设置了这些结构，以便每个包含一个指向evbuffer 的内部RAM （*iov_base*）文本块的指针，并设置该文本块中的内存的长度。
如果*len*小于 0，evbuffer_peek() 将尝试填充所有你给它的 evbuffer_iovec 结构。否则，它将填充evbuffer_iovec直到所有都填充完，或者填充len个字节）。如果该函数可以给你所要求的所有数据，它将返回实际上使用它的 evbuffer_iovec 结构的数量。否则，将返回它需要的evbuffer_iovec结构的数目，用来填充你所要求的数据。
当*start_at*为 NULL 时，evbuffer_peek() 函数从缓冲区的开头开始。否则，它开始在*start_at*指针.
**示例**
{*    /* Let's look at the first two chunks of buf, and write them to stderr. */***int** n, i;**struct** evbuffer_iovec v[2];    n = evbuffer_peek(buf, -1, NULL, v, 2);**for** (i=0; i<n; ++i) { /* There might be less than two chunks available. */        fwrite(v[i].iov_base, 1, v[i].iov_len, stderr);    }}{*    /* Let's send the first 4906 bytes to stdout via write. */***int** n, i, r;**struct** evbuffer_iovec *v;    size_t written = 0;*    /* determine how many chunks we need. */*    n = evbuffer_peek(buf, 4096, NULL, NULL, 0);*    /* Allocate space for the chunks.  This would be a good time to use**       alloca() if you have it. */*    v = malloc(**sizeof**(**struct** evbuffer_iovec)*n);*    /* Actually fill up v. */*    n = evbuffer_peek(buf, 4096, NULL, v, n);**for** (i=0; i<n; ++i) {        size_t len = v[i].iov_len;**if** (written + len > 4096)            len = 4096 - written;        r = write(1 /* stdout */, v[i].iov_base, len);**if** (r<=0)**break**;*        /* We keep track of the bytes written separately; if we don't,**           we may write more than 4096 bytes if the last chunk puts**           us over the limit. */*        written += len;    }    free(v);}{*    /* Let's get the first 16K of data after the first occurrence of the**       string "start/n", and pass it to a consume() function. */***struct** evbuffer_ptr ptr;**struct** evbuffer_iovec v[1];**const****char** s[] = "start/n";**int** n_written;    ptr = evbuffer_search(buf, s, strlen(s), NULL);**if** (ptr.pos == -1)**return**; /* no start string found. */*    /* Advance the pointer past the start string. */***if** (evbuffer_ptr_set(buf, &ptr, strlen(s), EVBUFFER_PTR_ADD) < 0)**return**; /* off the end of the string. */**while** (n_written < 16*1024) {*        /* Peek at a single chunk. */***if** (evbuffer_peek(buf, -1, &ptr, v, 1) < 1)**break**;*        /* Pass the data to some user-defined consume function */*        consume(v[0].iov_base, v[0].iov_len);        n_written += v[0].iov_len;*        /* Advance the pointer so we see the next chunk next time. */***if** (evbuffer_ptr_set(buf, &ptr, v[0].iov_len, EVBUFFER_PTR_ADD)<0)**break**;    }}
**备注**
·             修改 evbuffer_iovec 所指向的数据可能会导致未定义的行为。
·             如果调用任何函数修改 evbuffer，evbuffer_peek()返回的指针可能会变成无效。
·             如果您的 evbuffer 可以使用多个线程中，请务必在调用evbuffer_peek()之前，用evbuffer_lock()对evbuffer锁定，在完成之后并解锁。
此函数是 Libevent 2.0.2-alpha 中的新功能。
## 直接将数据添加到 evbuffer
有时你想把data直接加入到evbuffer中，而不需要首先写入到字符数组，然后用evbuffer_add()函数加入到evbuffer。这有先进的功能，您可以这样做：evbuffer_reserve_space() 和 evbuffer_commit_space()。就像evbuffer_peek()，这些函数使用的 evbuffer_iovec 结构提供直接访问内存内的 evbuffer的接口。
**接口**
**int** evbuffer_reserve_space(**struct** evbuffer *buf, ev_ssize_t size,**struct** evbuffer_iovec *vec, **int** n_vecs);**int** evbuffer_commit_space(**struct** evbuffer *buf,**struct** evbuffer_iovec *vec, **int** n_vecs);
Evbuffer_reserve_space() 函数可以提供指向evbuffer 内的空间的指针。它至少可以扩展缓冲区*size*个字节。指向这些扩展盘区的指针和他们的长度，将会被存储在您传递的*vec*结构中； *n_vecs*是该数组vec的长度。
*n_vecs*的值必须至少为 1。如果您提供只有一个vector，则 Libevent 将确保有在单一的范围为你申请了一个连续的空间，但是这样可能需要要重新排列缓冲区或浪费一些内存。为有更好的性能，至少提供2 个vector。该函数返回的是为你所需的空间所提供的vetctor的数量。
直到您调用 evbuffer_commit_space()函数，你写进这些vectors的数据都不是缓冲区的一部分，evbuffer_commit_space()函数实际上使得你写的数据作为缓冲区中的数据。如果您要提交比你要求的空间要少，可以减少在任何你可以得到的 evbuffer_iovec 结构的 iov_len 字段。此外，您还可以传递回较少的vectors，比你提供的。成功则evbuffer_commit_space() 函数返回0，失败返回-1.
**说明和注意事项**
·             调用任何函数可以重新排列 evbuffer 或将数据添加到 evbuffer中，evbuffer 将使你从 evbuffer_reserve_space()函数中得到的指针无效。
·             在当前实现中，evbuffer_reserve_space() 不会使用两个以上的vector，无论用户提供多少。这可能在未来的版本中的更改。
·             多次调用 evbuffer_reserve_space()是安全的。
·             如果您的 evbuffer 可用于多个线程，在调用 evbuffer_reserve_space()之前，请务必使用 evbuffer_lock() 锁定，一旦提交并解锁它。
**示例**
*/* Suppose we want to fill a buffer with 2048 bytes of output from a**   generate_data() function, without copying. */***struct** evbuffer_iovec v[2];**int** n, i;size_t n_to_add = 2048;*/* Reserve 2048 bytes.*/*n = evbuffer_reserve_space(buf, n_to_add, v, 2);**if** (n<=0)**return**; /* Unable to reserve the space **for** some reason. */**for** (i=0; i<n && n_to_add > 0; ++i) {   size_t len = v[i].iov_len;**if** (len > n_to_add) /* Don't write more than n_to_add bytes. */      len = n_to_add;**if** (generate_data(v[i].iov_base, len) < 0) {*      /* If there was a data during data generation, we can just stop**         here; no data will be committed to the buffer. */***return**;   }*   /* Set iov_len to the number of bytes we actually wrote, so we**      don't commit too much. */*   v[i].iov_len = len;}*/* We commit the space here.  Note that we give it 'i' (the number of**   vectors we actually used) rather than 'n' (the number of vectors we**   had available. */***if** (evbuffer_commit_space(buf, v, i) < 0)**return**; /* Error committing */
**不好的例子**
*/* Here are some mistakes you can make with evbuffer_reserve().**   DO NOT IMITATE THIS CODE. */***struct** evbuffer_iovec v[2];{*  /* Do not use the pointers from evbuffer_reserve_space() after**     calling any functions that modify the buffer. */*  evbuffer_reserve_space(buf, 1024, v, 2);  evbuffer_add(buf, "X", 1);*  /* WRONG: This next line won't work if evbuffer_add needed to rearrange**     the buffer's contents.  It might even crash your program. Instead,**     you add the data before calling evbuffer_reserve_space. */*  memset(v[0].iov_base, 'Y', v[0].iov_len-1);  evbuffer_commit_space(buf, v, 1);}{*  /* Do not modify the iov_base pointers. */***const****char** *data = "Here is some data";  evbuffer_reserve_space(buf, strlen(data), v, 1);*  /* WRONG: The next line will not do what you want.  Instead, you**     should _copy_ the contents of data into v[0].iov_base. */*  v[0].iov_base = (**char***) data;  v[0].iov_len = strlen(data);*  /* In this case, evbuffer_commit_space might give an error if you're**     lucky */*  evbuffer_commit_space(buf, v, 1);}
自从Libevent 2.0.2-alpha版本，这些函数开始存在。
## Evbuffers 网络 IO
在 Libevent中，使用evbuffers最常见的情况是网络 IO。用于在 evbuffer 上执行网络 IO的接口是：
**接口**
**int** evbuffer_write(**struct** evbuffer *buffer, evutil_socket_t fd);**int** evbuffer_write_atmost(**struct** evbuffer *buffer, evutil_socket_t fd,        ev_ssize_t howmuch);**int** evbuffer_read(**struct** evbuffer *buffer, evutil_socket_t fd, **int** howmuch);
evbuffer_read()函数从套接字fd上读取howmuch个字节添加缓冲区buffer的尾端。
成功则返回读取的字节数，错误则返回-1，读到EOF则返回0。请注意，错误可能表示非阻塞的操作将不会取得成功，您需要检查错误代码为 EAGAIN （或在Windows 为WSAEWOULDBLOCK）。如果*howmuch*是负的，evbuffer_read() 将尝试猜猜能够读多少字节。
Evbuffer_write_atmost() 函数试图写入*howmuch*个在buffer前面的字节到套接字fd。成功则返回写入的字节数，失败则返回-1。就像evbuffer_read()，您需要检查错误代码，看看是否错误是真实的或只是表示无法立即完成非阻塞的IO。如果你给howmuch一个负值，我们尝试把缓冲区的整个内容写入到套接字。
调用 evbuffer_write() 函数同带有负*howmuch*参数的evbuffer_write_atmost() 函数相同：它会尝试尽可能把buffer中的数据写入到套接字。
在 Unix 上，这些函数应支持具有读写的任何文件描述符。在 Windows 上，只支持套接字。
请注意当您使用的 bufferevents，您不需要调用这些 IO 函数 ；bufferevents 代码为您完成它。
evbuffer_write_atmost() 函数出现在Libevent 2.0.1-alpha版本中。
## Evbuffers 和回调
Evbuffers 的用户经常想知道什么时候从一个evbuffer中添加数据或者删除数据。为了提供支持，Libevent 提供了通用的 evbuffer 回调机制。
**接口**
**struct** evbuffer_cb_info {        size_t orig_size;        size_t n_added;        size_t n_deleted;};**typedef****void** (*evbuffer_cb_func)(**struct** evbuffer *buffer,**const****struct** evbuffer_cb_info *info, **void** *arg);
每次从 evbuffer 中添加或删除数据时，将触发evbuffer的回调函数。回调函数的参数有缓冲区buffer、指向 evbuffer_cb_info 结构的指针和用户提供的参数。Evbuffer_cb_info 结构的orig_size字段记录了缓冲区的大小更改以前的字节数；它的 n_added 字段记录了多少字节被添加到缓冲区中，和它的 n_deleted 字段记录删除了多少字节。
**接口**
**struct** evbuffer_cb_entry;**struct** evbuffer_cb_entry *evbuffer_add_cb(**struct** evbuffer *buffer,    evbuffer_cb_func cb, **void** *cbarg);
Evbuffer_add_cb() 函数将回调添加到 evbuffer，并返回不透明的指针，稍后可用于引用此特定回调实例。*cb*参数为能够被触发的回调函数，*cbarg*是用户提供要传递给此回调函数的指针。
您可以在一个evbuffer 上设置多个回调。添加一个新的回调并不会删除旧的回调。
**示例**
#include <event2/buffer.h>#include <stdio.h>#include <stdlib.h>*/* Here's a callback that remembers how many bytes we have drained in**   total from the buffer, and prints a dot every time we hit a**   megabyte. */***struct** total_processed {    size_t n;};**void** count_megabytes_cb(**struct** evbuffer *buffer,**const****struct** evbuffer_cb_info *info, **void** *arg){**struct** total_processed *tp = arg;    size_t old_n = tp->n;**int** megabytes, i;    tp->n += info->n_deleted;    megabytes = ((tp->n) >> 20) - (old_n >> 20);**for** (i=0; i<megabytes; ++i)        putc('.', stdout);}**void** operation_with_counted_bytes(**void**){**struct** total_processed *tp = malloc(**sizeof**(tp));**struct** evbuffer *buf = evbuffer_new();    tp->n = 0;    evbuffer_add_cb(buf, count_megabytes_cb, tp);*    /* Use the evbuffer for a while.  When we're done: */*    evbuffer_free(buf);    free(tp);}
注意在传递过程中释放非空的 evbuffer不能算作抽取（draining）数据，并且释放evbuffer并不释放其回调的用户提供的数据指针。
如果您不想一个回调永久的作用于一个buffer上，您可以*删除*它 （以使它永远离开了），或者禁用 (以将其关闭一段时间）：
**接口**
**int** evbuffer_remove_cb_entry(**struct** evbuffer *buffer,**struct** evbuffer_cb_entry *ent);**int** evbuffer_remove_cb(**struct** evbuffer *buffer, evbuffer_cb_func cb,**void** *cbarg);#define EVBUFFER_CB_ENABLED 1**int** evbuffer_cb_set_flags(**struct** evbuffer *buffer,**struct** evbuffer_cb_entry *cb,                          ev_uint32_t flags);**int** evbuffer_cb_clear_flags(**struct** evbuffer *buffer,**struct** evbuffer_cb_entry *cb,                          ev_uint32_t flags);
删除一个回调有两种方法：1、可以通过在添加回调时返回的evbuffer_cb_entry进行删除(对应函数evbuffer_remove_cb_entry （）)。2、通过你所使用的回调和指针进行删除（对应函数evbuffer_remove_cb()）。evbuffer_remove_cb() 函数成功则返回0，失败则返回-1。.
evbuffer_cb_set_flags() 函数和 evbuffer_cb_clear_flags() 函数分别在一个给定的回调设置一个设置标志或者清除标志。现在，只支持一个用户可见的标志：*EVBUFFER_CB_ENABLED*。默认情况下设置该标志。当此标志被清除，evbuffer 的修改不会引起回调函数的调用。
**接口**
**int** evbuffer_defer_callbacks(**struct** evbuffer *buffer, **struct** event_base *base);
正如bufferevent 回调一样，当evbuffer更改时，你可以不用立即执行evbuffer的回调函数，而是可以*延迟*，并作为一个给定的event base的事件循环的一部分运行。此方法很有用，如果您有多个 evbuffers 的回调可能导致数据添加和移除，并且想要避免粉碎栈。
如果延迟evbuffer 的回调，然后当他们最后被调用时，他们可总结为多个操作的结果。
像 bufferevents一样，evbuffers 是内部引用计数，所以释放evbuffer是安全的的，即使它已推迟尚未执行的回调。
这整个回调系统是 Libevent 2.0.1-alpha 中的新功能。Evbuffer_cb_(set|clear)_flags() 函数出现在2.0.2-alpha的版本中。
## 基于 evbuffer 的 IO 避免数据拷贝
真正的快速的网络编程往往需要做尽可能少的数据拷贝。Libevent 提供了一些机制来帮助实现。
**接口**
**typedef****void** (*evbuffer_ref_cleanup_cb)(**const****void** *data,    size_t datalen, **void** *extra);**int** evbuffer_add_reference(**struct** evbuffer *outbuf,**const****void** *data, size_t datlen,    evbuffer_ref_cleanup_cb cleanupfn, **void** *extra);
此函数通过引用在evbuffer 的末尾添加某个数据块。不执行任何复制：相反，evbuffer只存储一个指针，这个指针指向存储*datlen*个字节*的data*。因此，此指针必须仍然有效，只要 evbuffer 正在使用它。当 evbuffer 不再需要的data时，它将调用“cleanupfn”函数，而指向“data”的指针，datlen，extra作为函数cleanupfn的参数。这个函数成功则返回0，失败则返回-1.
**示例**
#include <event2/buffer.h>#include <stdlib.h>#include <string.h>*/* In this example, we have a bunch of evbuffers that we want to use to**   spool a one-megabyte resource out to the network.  We do this**   without keeping any more copies of the resource in memory than**   necessary. */*#define HUGE_RESOURCE_SIZE (1024*1024)**struct** huge_resource {*    /* We keep a count of the references that exist to this structure,**       so that we know when we can free it. */***int** reference_count;**char** data[HUGE_RESOURCE_SIZE];};**struct** huge_resource *new_resource(**void**) {**struct** huge_resource *hr = malloc(**sizeof**(**struct** huge_resource));    hr->reference_count = 1;*    /* Here we should fill hr->data with something.  In real life,**       we'd probably load something or do a complex calculation.**       Here, we'll just fill it with EEs. */*    memset(hr->data, 0xEE, **sizeof**(hr->data));**return** hr;}**void** free_resource(**struct** huge_resource *hr) {    --hr->reference_count;**if** (hr->reference_count == 0)        free(hr);}**static****void** cleanup(**const****void** *data, size_t len, **void** *arg) {    free_resource(arg);}*/* This is the function that actually adds the resource to the**   buffer. */***void** spool_resource_to_evbuffer(**struct** evbuffer *buf,**struct** huge_resource *hr){    ++hr->reference_count;    evbuffer_add_reference(buf, hr->data, HUGE_RESOURCE_SIZE,        cleanup, hr);}
某些操作系统提供文件写入网不用将数据复制到用户空间的方法。当可用时您可以访问这些机制：
**接口**
**int** evbuffer_add_file(**struct** evbuffer *output, **int** fd, ev_off_t offset,    size_t length);
Evbuffer_add_file() 函数假定它有一个打开的可供读取的文件描述符 （不是套接字）fd。它从文件开始位置*offset*，*添加文件的**length*个字节到output。成功则返回0，失败则返回-1。
**警告**
作为 Libevent 2.0.2-alpha版本，向网络发送数据的唯一可以信赖的方法就是用evbuffer_write*()方法，抽取数据用evbuffer_drain()函数，把数据移动到另一个evbuffer 用evbuffer_*_buffer()函数。您不能可靠地用 evbuffer_remove()从缓冲区中提取数据、线性化数据用evbuffer_pullup()，等等。
如果您的操作系统支持 splice () 或 sendfile()，Libevent 将直接调用 evbuffer_write()函数把数据从*fd*发送到网络，而不在需要把所有数据复制到用户的 RAM（内存）中。如果splice () 或 sendfile()不存在，但有 mmap()，Libevent 将 mmap 该文件，并且您的内核希望可以找出它永远不需要将数据复制到用户空间。否则，Libevent 将只是从磁盘读取数据到 RAM（内存）。
当evbuffer中的数据被刷新，或者evbuffer被释放，则文件描述符将会被关闭。
这两个函数首次在Libevent 2.0.1-alpha版本中引入。evbuffer_add_reference() 函数从2.0.2-alpha版本开始存在。
## 生成一个仅有添加和删除功能的evbuffer
**接口**
**int** evbuffer_freeze(**struct** evbuffer *buf, **int** at_front);**int** evbuffer_unfreeze(**struct** evbuffer *buf, **int** at_front);
你可以使用这两个函数暂时禁止在evbuffer的开头或者结尾的更改。
暂时禁用的前面或 evbuffer 结尾的更改，可以使用这些函数。Bufferevent 代码
使用它们内部来防止输出缓冲区的前面或输入缓冲区的结尾的意外修改。
在 Libevent 2.0.1-alpha中推出的 evbuffer_freeze()函数。
## 过时的 evbuffer 函数
Evbuffer 接口在Libevent 2.0中更改了很多。之前，每个evbuffers 被作为一块连续的 RAM，使访问效率极低。
Event.h头文件常常用来暴露evbuffer的内部结构。这些不在可用，在1.4与2.0版本之间，更改了很多依赖于它们工作的代码。
若要访问的 evbuffer 中的字节数，有 EVBUFFER_LENGTH() 宏。实际的数据可用EVBUFFER_DATA()来访问。这些都是在 event2/buffer_compat.h 中可用。看出来了，虽然： EVBUFFER_DATA(b) 是 evbuffer_pullup (b-1)的别名，但evbuffer_pullup (b-1)的代价非常大。
其他一些过时的接口：
**不推荐使用的接口**
**char** *evbuffer_readline(**struct** evbuffer *buffer);**unsigned****char** *evbuffer_find(**struct** evbuffer *buffer,**const****unsigned****char** *what, size_t len);
Evbuffer_readline() 函数功能像当前 evbuffer_readln （buffer，NULL，EVBUFFER_EOL_ANY）。
Evbuffer_find() 函数将在缓冲区中搜索字符串的第一个匹配项，并返回一个指向它的指针。与 evbuffer_search()不同的是，它只找的第一个字符串的匹配项。为了保持兼容旧代码的使用此函数，它现在 linearizes 整个缓冲区到本地字符串的结尾。
回调接口也是不同的：
**不推荐使用的接口：**
**typedef****void** (*evbuffer_cb)(**struct** evbuffer *buffer,    size_t old_len, size_t new_len, **void** *arg);**void** evbuffer_setcb(**struct** evbuffer *buffer, evbuffer_cb cb, **void** *cbarg);
Evbuffer 一次只能设置一个回调，因此设置新的回调将禁用前的回调，设置回调为NULL比禁用回调的方式要好。
不是使用 evbuffer_cb_info_structure结构， 回调函数使用旧的和新的长度。因此，如果old_len大于 new_len，数据将被耗尽。如果 new_len 大于 old_len，数据被添加。它是不可能被延迟回调，因此在一个单一的回调调用机制中，添加和删除永远不会进行批处理。
过时的职能是在 event2/buffer_compat.h 中仍然可用。
原文：http://www.wangafu.net/~nickm/libevent-book/Ref7_evbuffer.html
