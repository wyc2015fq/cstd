# nginx平台初探(五) - gauss的专栏 - CSDN博客
2015年11月01日 11:26:12[gauss](https://me.csdn.net/mathlmx)阅读数：444
# 其他模块 (40%)
Nginx的模块种类挺多的，除了HTTP模块，还有一些核心模块和mail系列模块。核心模块主要是做一些基础功能，比如Nginx的启动初始化，event处理机制，错误日志的初始化，ssl的初始化，正则处理初始化。
mail模块可以对imap，pop3，smtp等协议进行反向代理，这些模块本身不对邮件内容进行处理。
## core模块 (40%)
### Nginx的启动模块 (40%)
启动模块从启动Nginx进程开始，做了一系列的初始化工作，源代码位于src/core/nginx.c，从main函数开始:
- 时间、正则、错误日志、ssl等初始化
- 读入命令行参数
- OS相关初始化
- 读入并解析配置
- 核心模块初始化
- 创建各种暂时文件和目录
- 创建共享内存
- 打开listen的端口
- 所有模块初始化
- 启动worker进程
## event模块 (40%)
### event的类型和功能 (40%)
Nginx是以event（事件）处理模型为基础的模块。它为了支持跨平台，抽象出了event模块。它支持的event处理类型有：AIO（异步IO），/dev/poll（Solaris 和Unix特有），epoll（Linux特有），eventport（Solaris 10特有），kqueue（BSD特有），poll，rtsig（实时信号），select等。
event模块的主要功能就是，监听accept后建立的连接，对读写事件进行添加删除。事件处理模型和Nginx的非阻塞IO模型结合在一起使用。当IO可读可写的时候，相应的读写事件就会被唤醒，此时就会去处理事件的回调函数。
特别对于Linux，Nginx大部分event采用epoll EPOLLET（边沿触发）的方法来触发事件，只有listen端口的读事件是EPOLLLT（水平触发）。对于边沿触发，如果出现了可读事件，必须及时处理，否则可能会出现读事件不再触发，连接饿死的情况。
typedef struct {
        /* 添加删除事件 */
        ngx_int_t  (*add)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
        ngx_int_t  (*del)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
        ngx_int_t  (*enable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
        ngx_int_t  (*disable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
        /* 添加删除连接，会同时监听读写事件 */
        ngx_int_t  (*add_conn)(ngx_connection_t *c);
        ngx_int_t  (*del_conn)(ngx_connection_t *c, ngx_uint_t flags);
        ngx_int_t  (*process_changes)(ngx_cycle_t *cycle, ngx_uint_t nowait);
        /* 处理事件的函数 */
        ngx_int_t  (*process_events)(ngx_cycle_t *cycle, ngx_msec_t timer,
                                   ngx_uint_t flags);
        ngx_int_t  (*init)(ngx_cycle_t *cycle, ngx_msec_t timer);
        void       (*done)(ngx_cycle_t *cycle);
} ngx_event_actions_t;
上述是event处理抽象出来的关键结构体，可以看到，每个event处理模型，都需要实现部分功能。最关键的是add和del功能，就是最基本的添加和删除事件的函数。
### accept锁 (40%)
Nginx是多进程程序，80端口是各进程所共享的，多进程同时listen 80端口，势必会产生竞争，也产生了所谓的“惊群”效应。当内核accept一个连接时，会唤醒所有等待中的进程，但实际上只有一个进程能获取连接，其他的进程都是被无效唤醒的。所以Nginx采用了自有的一套accept加锁机制，避免多个进程同时调用accept。Nginx多进程的锁在底层默认是通过CPU自旋锁来实现。如果操作系统不支持自旋锁，就采用文件锁。
Nginx事件处理的入口函数是ngx_process_events_and_timers()，下面是部分代码，可以看到其加锁的过程：
if (ngx_use_accept_mutex) {
        if (ngx_accept_disabled > 0) {
                ngx_accept_disabled--;
        } else {
                if (ngx_trylock_accept_mutex(cycle) == NGX_ERROR) {
                        return;
                }
                if (ngx_accept_mutex_held) {
                        flags |= NGX_POST_EVENTS;
                } else {
                        if (timer == NGX_TIMER_INFINITE
                                || timer > ngx_accept_mutex_delay)
                        {
                                timer = ngx_accept_mutex_delay;
                        }
                }
        }
}
在ngx_trylock_accept_mutex()函数里面，如果拿到了锁，Nginx会把listen的端口读事件加入event处理，该进程在有新连接进来时就可以进行accept了。注意accept操作是一个普通的读事件。下面的代码说明了这点：
(void) ngx_process_events(cycle, timer, flags);
if (ngx_posted_accept_events) {
        ngx_event_process_posted(cycle, &ngx_posted_accept_events);
}
if (ngx_accept_mutex_held) {
        ngx_shmtx_unlock(&ngx_accept_mutex);
}
ngx_process_events()函数是所有事件处理的入口，它会遍历所有的事件。抢到了accept锁的进程跟一般进程稍微不同的是，它被加上了NGX_POST_EVENTS标志，也就是说在ngx_process_events() 函数里面只接受而不处理事件，并加入post_events的队列里面。直到ngx_accept_mutex锁去掉以后才去处理具体的事件。为什么这样？因为ngx_accept_mutex是全局锁，这样做可以尽量减少该进程抢到锁以后，从accept开始到结束的时间，以便其他进程继续接收新的连接，提高吞吐量。
ngx_posted_accept_events和ngx_posted_events就分别是accept延迟事件队列和普通延迟事件队列。可以看到ngx_posted_accept_events还是放到ngx_accept_mutex锁里面处理的。该队列里面处理的都是accept事件，它会一口气把内核backlog里等待的连接都accept进来，注册到读写事件里。
而ngx_posted_events是普通的延迟事件队列。一般情况下，什么样的事件会放到这个普通延迟队列里面呢？我的理解是，那些CPU耗时比较多的都可以放进去。因为Nginx事件处理都是根据触发顺序在一个大循环里依次处理的，因为Nginx一个进程同时只能处理一个事件，所以有些耗时多的事件会把后面所有事件的处理都耽搁了。
除了加锁，Nginx也对各进程的请求处理的均衡性作了优化，也就是说，如果在负载高的时候，进程抢到的锁过多，会导致这个进程被禁止接受请求一段时间。
比如，在ngx_event_accept函数中，有类似代码：
ngx_accept_disabled = ngx_cycle->connection_n / 8
              - ngx_cycle->free_connection_n;
ngx_cycle->connection_n是进程可以分配的连接总数，ngx_cycle->free_connection_n是空闲的进程数。上述等式说明了，当前进程的空闲进程数小于1/8的话，就会被禁止accept一段时间。
### 定时器 (40%)
Nginx在需要用到超时的时候，都会用到定时器机制。比如，建立连接以后的那些读写超时。Nginx使用红黑树来构造定期器，红黑树是一种有序的二叉平衡树，其查找插入和删除的复杂度都为O(logn)，所以是一种比较理想的二叉树。
定时器的机制就是，二叉树的值是其超时时间，每次查找二叉树的最小值，如果最小值已经过期，就删除该节点，然后继续查找，直到所有超时节点都被删除。
## mail模块
### mail模块的实现
### mail模块的功能
# 模块开发高级篇(30%)
## 变量(80%)
### 综述
在Nginx中同一个请求需要在模块之间数据的传递或者说在配置文件里面使用模块动态的数据一般来说都是使用变量，比如在HTTP模块中导出了host/remote_addr等变量，这样我们就可以在配置文件中以及在其他的模块使用这个变量。在Nginx中，有两种定义变量的方式，一种是在配置文件中,使用set指令，一种就是上面我们提到的在模块中定义变量，然后导出.
在Nginx中所有的变量都是与HTTP相关的(也就是说赋值都是在请求阶段)，并且基本上是同时保存在两个数据结构中，一个就是hash表(可选)，另一个是数组. 比如一些特殊的变量，比如arg_xxx/cookie_xxx等，这些变量的名字是不确定的(因此不能内置)，而且他们还是只读的(不能交由用户修改)，如果每个都要放到hash表中的话(不知道用户会取多少个),会很占空间的，因此这些变量就没有hash,只有索引.这里要注意，用户不能定义这样的变量，这样的变量只存在于Nginx内部.
对应的变量结构体是这样子(每一个变量都是一个ngx_http_variable_s结构体)的：
struct ngx_http_variable_s {
    ngx_str_t                     name;   /* must be first to build the hash */
    ngx_http_set_variable_pt      set_handler;
    ngx_http_get_variable_pt      get_handler;
    uintptr_t                     data;
    ngx_uint_t                    flags;
    ngx_uint_t                    index;
};
其中name表示对应的变量名字，set/get_handler表示对应的设置以及读取回调，而data是传递给回调的参数，flags表示变量的属性，index提供了一个索引(数组的脚标)，从而可以迅速定位到对应的变量。set/get_handler只有在真正读取设置变量的时候才会被调用.
这里要注意flag属性,flag属性就是由下面的几个属性组合而成:
#define NGX_HTTP_VAR_CHANGEABLE   1
#define NGX_HTTP_VAR_NOCACHEABLE  2
#define NGX_HTTP_VAR_INDEXED      4
#define NGX_HTTP_VAR_NOHASH       8
- NGX_HTTP_VAR_CHANGEABLE表示这个变量是可变的.Nginx有很多内置变量是不可变的，比如arg_xxx这类变量，如果你使用set指令来修改，那么Nginx就会报错.
- NGX_HTTP_VAR_NOCACHEABLE表示这个变量每次都要去取值，而不是直接返回上次cache的值(配合对应的接口).
- NGX_HTTP_VAR_INDEXED表示这个变量是用索引读取的.
- NGX_HTTP_VAR_NOHASH表示这个变量不需要被hash.
而变量在Nginx中的初始化流程是这样的:
- 首先当解析HTTP之前会调用ngx_http_variables_add_core_vars(pre_config)来将HTTP core模块导出的变量(http_host/remote_addr...)添加进全局的hash key链中.
- 解析完HTTP模块之后，会调用ngx_http_variables_init_vars来初始化所有的变量(不仅包括HTTP core模块的变量，也包括其他的HTTP模块导出的变量,以及配置文件中使用set命令设置的变量),这里的初始化包括初始化hash表,以及初始化数组索引.
- 当每次请求到来时会给每个请求创建一个变量数组(数组的个数就是上面第二步所保存的变量个数)。然后只有取变量值的时候，才会将变量保存在对应的变量数组位置。
### 创建变量
在Nginx中，创建变量有两种方式，分别是在配置文件中使用set指令，和在模块中调用对应的接口，在配置文件中创建变量比较简单，因此我们主要来看如何在模块中创建自己的变量。
在Nginx中提供了下面的接口，可以供模块调用来创建变量。
ngx_http_variable_t *ngx_http_add_variable(ngx_conf_t *cf, ngx_str_t *name, ngx_uint_t flags);
这个函数所做的工作就是将变量 “name”添加进全局的hash key表中,然后初始化一些域，不过这里要注意，对应的变量的get/set回调，需要当这个函数返回之后，显式的设置,比如在split_clients模块中的例子:
var = ngx_http_add_variable(cf, &name, NGX_HTTP_VAR_CHANGEABLE);
if (var == NULL) {
        return NGX_CONF_ERROR;
}
//设置回调
var->get_handler = ngx_http_split_clients_variable;
var->data = (uintptr_t) ctx;
而对应的回调函数原型是这样的:
typedef void (*ngx_http_set_variable_pt) (ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
typedef ngx_int_t (*ngx_http_get_variable_pt) (ngx_http_request_t *r,
    ngx_http_variable_value_t *v, uintptr_t data);
回调函数比较简单，第一个参数是当前请求，第二个是需要设置或者获取的变量值，第三个是初始化时的回调指针，这里我们着重来看一下ngx_http_variable_value_t,下面就是这个结构体的原型:
typedef struct {
    unsigned    len:28;
    unsigned    valid:1;
    unsigned    no_cacheable:1;
    unsigned    not_found:1;
    unsigned    escape:1;
    u_char     *data;
} ngx_variable_value_t;
这里主要是data域，当我们在get_handle中设置变量值的时候，只需要将对应的值放入到data中就可以了，这里data需要在get_handle中分配内存,比如下面的例子(ngx_http_fastcgi_script_name_variable),就是fastcgi_script_name变量的get_handler代码片段:
v->len = f->script_name.len + flcf->index.len;
v->data = ngx_pnalloc(r->pool, v->len);
if (v->data == NULL) {
        return NGX_ERROR;
}
p = ngx_copy(v->data, f->script_name.data, f->script_name.len);
ngx_memcpy(p, flcf->index.data, flcf->index.len);
### 使用变量
Nginx的内部变量指的就是Nginx的官方模块中所导出的变量，在Nginx中，大部分常用的变量都是CORE HTTP模块导出的。而在Nginx中，不仅可以在模块代码中使用变量，而且还可以在配置文件中使用。
假设我们需要在配置文件中使用http模块的host变量，那么只需要这样在变量名前加一个$符号就可以了($host).而如果需要在模块中使用host变量，那么就比较麻烦，Nginx提供了下面几个接口来取得变量:
ngx_http_variable_value_t *ngx_http_get_indexed_variable(ngx_http_request_t *r,
    ngx_uint_t index);
ngx_http_variable_value_t *ngx_http_get_flushed_variable(ngx_http_request_t *r,
    ngx_uint_t index);
ngx_http_variable_value_t *ngx_http_get_variable(ngx_http_request_t *r,
    ngx_str_t *name, ngx_uint_t key);
他们的区别是这样子的，ngx_http_get_indexed_variable和ngx_http_get_flushed_variable都是用来取得有索引的变量，不过他们的区别是后一个会处理NGX_HTTP_VAR_NOCACHEABLE这个标记，也就是说如果你想要cache你的变量值，那么你的变量属性就不能设置NGX_HTTP_VAR_NOCACHEABLE,并且通过ngx_http_get_flushed_variable来获取变量值.而ngx_http_get_variable和上面的区别就是它能够得到没有索引的变量值.
通过上面我们知道可以通过索引来得到变量值，可是这个索引该如何取得呢，Nginx也提供了对应的接口：
ngx_int_t ngx_http_get_variable_index(ngx_conf_t *cf, ngx_str_t *name);
通过这个接口，就可以取得对应变量名的索引值。
接下来来看对应的例子，比如在http_log模块中，如果在log_format中配置了对应的变量，那么它会调用ngx_http_get_variable_index来保存索引:
static ngx_int_t
ngx_http_log_variable_compile(ngx_conf_t *cf, ngx_http_log_op_t *op,
    ngx_str_t *value)
{
    ngx_int_t  index;
    //得到变量的索引
    index = ngx_http_get_variable_index(cf, value);
    if (index == NGX_ERROR) {
        return NGX_ERROR;
    }
    op->len = 0;
    op->getlen = ngx_http_log_variable_getlen;
    op->run = ngx_http_log_variable;
    //保存索引值
    op->data = index;
    return NGX_OK;
 }
然后http_log模块会使用ngx_http_get_indexed_variable来得到对应的变量值,这里要注意，就是使用这个接口的时候，判断返回值，不仅要判断是否为空，也需要判断value->not_found,这是因为只有第一次调用才会返回空，后续返回就不是空，因此需要判断value->not_found:
static u_char *
ngx_http_log_variable(ngx_http_request_t *r, u_char *buf, ngx_http_log_op_t *op)
{
    ngx_http_variable_value_t  *value;
    //获取变量值
    value = ngx_http_get_indexed_variable(r, op->data);
    if (value == NULL || value->not_found) {
            *buf = '-';
            return buf + 1;
    }
    if (value->escape == 0) {
            return ngx_cpymem(buf, value->data, value->len);
    } else {
            return (u_char *) ngx_http_log_escape(buf, value->data, value->len);
    }
 }
## upstream
### 使用subrequest访问upstream
### 超越upstream
#### event机制
#### 例讲（主动健康检查模块）
## 使用lua模块
