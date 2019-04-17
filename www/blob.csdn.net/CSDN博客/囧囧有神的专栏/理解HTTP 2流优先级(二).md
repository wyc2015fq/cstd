# 理解HTTP/2流优先级(二) - 囧囧有神的专栏 - CSDN博客





2017年03月21日 20:07:20[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：1866








流依赖和优先级也是HTTP/2的关键特性。设计流依赖和优先级基于以下认识：WEB应用的资源有重要性的区别，优先加载重要资源，可以尽快渲染页面，提升用户体验。HTTP/2中，一个源只有一个连接来实现多路复用，所有资源通过一个连接传输，为了避免线头堵塞（Head Of Line Block），这时候资源传输的顺序就更重要了。

实际上优先级解决了http层面的线头堵塞，tcp层面的线头堵塞依然存在

要使用HTTP/2流依赖和优先级，通常情况，我们什么都不需要做。浏览器和HTTP服务器已经帮我们处理好了具体的细节。




# 协议中怎么说？

具体看 http://http2.github.io/http2-spec/#StreamPriority

简单来说，HTTP/2通过依赖和权重来实现优先级。




# 浏览器中怎么做？

以下是《高性能浏览器网络》中的描述。



```
浏览器请求优先级与 HTTP 2.0
浏览器在渲染页面时，并非所有资源都具有相同的优先级:HTML 文档本身对 构建 DOM 不可或缺，CSS 对构建 CSSOM 不可或缺，而 DOM 和 CSSOM 的构 建都可能受到 JavaScript 资源的阻塞(参见 10.1 节的附注栏“DOM、CSSOM 和 JavaScript”)，其他资源(如图片)的优先级都可以降低。
为加快页面加载速度，所有现代浏览器都会基于资源的类型以及它在页面中的位 置排定请求的优先次序，甚至通过之前的访问来学习优先级模式——比如，之前 的渲染如果被某些资源阻塞了，那么同样的资源在下一次访问时可能就会被赋予 更高的优先级。
在 HTTP 1.x 中，浏览器极少能利用上述优先级信息，因为协议本身并不支持多路 复用，也没有办法向服务器通告请求的优先级。此时，浏览器只能依赖并行连接， 且最多只能同时向一个域名发送 6 个请求。于是，在等连接可用期间，请求只能 在客户端排队，从而增加了不必要的网络延迟。理论上，HTTP 管道可以解决这 个问题，只是由于缺乏支持而无法付诸实践。
HTTP 2.0 一举解决了所有这些低效的问题:浏览器可以在发现资源时立即分派请 求，指定每个流的优先级，让服务器决定最优的响应次序。这样请求就不必排队 了，既节省了时间，也最大限度地利用了每个连接。
```









# 服务器中怎么实现



浏览器通过HEADERS帧和PRIORITY帧携带了优先级信息，服务器据此可以生成优先级树，并指导自己的资源分配。资源包括CPU、内存、带宽等等。

HTTP/2协议定义了这颗优先级树的各种特性，却没有给出一个具体实现，如何分配CPU？如何分配内存？数据准备好之后，如何决定传输数据？这些都依赖于工程师去具体实现（当然也可以不实现。。）。

具体实现的方法，在不同服务器中是完全不同的。我们来看看nginx中是怎么实现的吧







## nginx相关配置





指令：http2_streams_index_size

默认：http2_streams_index_size 31

上下文：http、server




所有的stream放到一个简易的hash数组中，

hash数组的大小为http2_streams_index_size+1

hash函数为 ((sid >> 1) & h2scf->streams_index_mask)     等价于sid%32

冲突处理函数为链表


## 基本数据结构



```cpp
struct ngx_http_v2_node_s {                               // 依赖树中的节点数据结构，每个节点封装一个个stream，
    ngx_uint_t                       id;                  // 对应的stream id
    ngx_http_v2_node_t              *index;               // 链表的首节点，
    ngx_http_v2_node_t              *parent;              // 依赖的父节点
    ngx_queue_t                      queue;               // 兄弟节点
    ngx_queue_t                      children;            // 子节点
    ngx_queue_t                      reuse;               //
    ngx_uint_t                       rank;                // 节点在依赖树中的深度
    ngx_uint_t                       weight;              // 流的权重， 取值范围在0-255
    double                           rel_weight;          // 计算出来的真实权重
    ngx_http_v2_stream_t            *stream;              // 指向对应的流
};
```




重点关注下 rank和rel_weight值的意义和计算方式。




nginx通过http2_streams_index_size指令，指定了一个hash表的大小，默认为32

所有的stream，根据sid%32 ，映射到hash表上，通过链表串起来。




假设有以下几个流组成的优先级树



                   ROOT

                        |

           sid=1,weight=128

                        |

           sid=33,weight=64   

            /                         \      

sid=3,weight=256         sid=35,weight=128

                                         \  

                                       sid=67,weight=128





那么在nginx中存储结构就是这个样子的：

![](https://img-blog.csdn.net/20170321193349242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)











## 逻辑处理部分

### 如何决定响应数据的传输顺序

数据准备好了之后，如何决定数据的传输顺序呢？




我们重点关注下ngx_http_v2_node_t结构体中  rank和rel_weight值的意义和计算方式。





rank：节点在依赖树中的深度

rel_weight:  根据父节点的rel_weight和  子节点的weight 计算出来的一个值，计算公式是  weight/256 * (parent's rel_weight)




同样以上文中的例子来计算




|sid|weight|rank|rel_weight|
|----|----|----|----|
|1|128|1|128/256=0.5|
|33|64|2|0.5*64/256=0.125|
|3|256|3|0.125*256/256=0.125|
|35|128|3|0.125*128/256=0.0625|
|67|128|4|0.0625*128/256=0.03125|


数据传输顺序的排序规则是这样的：

1. rank值小的优先级高

2. rank值相同时，rel_weight值大的优先级高




根据这个规则，我们可以将不同流的响应数据排个序

1 > 33 > 3 > 35 > 67




以下代码是，数据准备好之后，将数据frame插入待传输数据链的实现



```cpp
static ngx_inline void
ngx_http_v2_queue_frame(ngx_http_v2_connection_t *h2c,
    ngx_http_v2_out_frame_t *frame)
{
    ngx_http_v2_out_frame_t  **out;

    for (out = &h2c->last_out; *out; out = &(*out)->next) {

        if ((*out)->blocked || (*out)->stream == NULL) {
            break;
        }

        if ((*out)->stream->node->rank < frame->stream->node->rank
            || ((*out)->stream->node->rank == frame->stream->node->rank
                && (*out)->stream->node->rel_weight
                   >= frame->stream->node->rel_weight))
        {
            break;
        }
    }

    frame->next = *out;
    *out = frame;
}
```


可以看到，采用的以上的比较方法。 




代码是采用插入排序实现的，算法复杂度是O(n),  n是待传输数据链的长度。 那么问题来了，当因为发送窗口用完，或其他原因，导致待传输数据链很长的时候， 性能会不会降到很低？



















