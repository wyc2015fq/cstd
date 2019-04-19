# Nginx 负载均衡策略之加权轮询分析 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [lvyilong316](http://www.jobbole.com/members/lvyilong316) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
nginx做为方向代理时，能够为后端服务器提供负载均衡的功能，其中加权轮询策略使是其默认的负载均衡策略。
![](http://blog.chinaunix.net/attachment/201505/20/28541347_1432126551F3C9.png)
直观上就是将来自客户的请求按照每个服务器的权值进行负载均衡（权值大的服务器处理的请求也应该多）。那么对于每次客户的请求如何选取后端服务器才能确保这种均衡呢？nginx采用加权轮询策略时选取后端服务器的核心代码是ngx_http_upstream_get_peer函数(位于ngx_http_upstream_round_robin.c中)。下面对这段代码进行分析。
- **代码说明：**
(1)      peer[n].weight：后端服务器初始权重。
(2)      peer[n].current_weight：后端服务器当前权重，初始情况等于peer[n].weight。
(3)      peers->number：后端服务器的个数
(4)      peers->peer[0]：一个数组的第一个元素，这个数组的每个元素对应一个后端服务器。
(5)      一旦某个后端服务器n被选中后，会在其他处理函数中执行peer[n].current_weight–。
(6)      代码18行乘以1000是为了避免浮点处理，所以直接报被除数放大1000倍，也就是间接把精度提升到小数点后三位，注意这里是权值的比较，因此把两边权值都放大1000倍并不会影响最终的比较结果。


```
static ngx_uint_t ngx_http_upstream_get_peer(ngx_http_upstream_rr_peers_t *peers)
{
    ngx_uint_t i, n, reset = 0;
    ngx_http_upstream_rr_peer_t *peer;
    peer = &peers->peer[0]; //peer指向后端服务器列表
 
    for ( ;; ) {
        for (i = 0; i < peers->number; i++) {
            if (peer[i].current_weight <= 0) {
                continue;
            }
            n = i; //n为第一个current_weight大于0的服务器下标
            while (i < peers->number - 1) {
                i++; //i从n的下一个服务器开始遍历
                if (peer[i].current_weight <= 0) {
                    continue;
                }
                if (peer[n].current_weight * 1000 / peer[i].current_weight
                    > peer[n].weight * 1000 / peer[i].weight) //选取后端服务器的关键
                {
                    return n;
                }
                n = i;
            }
            if (peer[i].current_weight > 0) {
                n = i;
            }
            return n;
        }
        if (reset++) { //初始为0，所以第二次循环到此条件才成立，注意是后置自增。
            return 0;
        }
        for (i = 0; i < peers->number; i++) {
            peer[i].current_weight = peer[i].weight;
        }
    }
}
```
- **分析:**
18~19行代码是选取后端服务器的关键，那么这个条件是如何确保选取后端服务器负载均衡呢？
假设有三台后端服务器A、B、C，它们的权值分别为5、3、1。那么执行过程如下：
(1)      第一次请求由于peer[n].current_weight= peer[n].weight&&peer[i].current_weight= peer[i].weight，所以代码18行的条件始终不成立。13行的while循环到i=2时退出。接着执行到25代码行条件成立，n=i=2，所以第一次选中服务器C，之后服务器C的current_weight–,当前权值变为0。
(2)      第二次请求到来时，A、B、C的权值为5、3、0。代码执行到14行时，i=1，n=0，此时由于A和B的current_weight和weight相同，条件依然不成立，23行使n=i=1，然后i++变为2，但代码15行条件成立(C的current_weight为0),继续循环到13行代码不成立。此时跳出13行的while循环，执行到18行返回n=1，即选择服务器B。
(3)      第三次请求到达时，A、B、C的权值为5、2、0。执行到代码14行时n=0，i=1，随后18行条件成立（peer[n].current_weight=5，peer[i].current_weight=2，peer[n].weight=5，peer[i].weight=3）,所以19行返回n=0，即选中服务器A。
(4)      ……
(5)      随后请求处理类似，知道所有服务器current_weight都等于0。此时第8行的for循环跳出，执行第30行条件不成立，执行33行，再次将current_weight重置为初始值。
这样一个过程确保链接的处理按照服务器配置的权重来均衡。
- **注意**
可以看出nginx每次选出的服务器并不一定是当前权重(处理能力)最大的，如上分析第一次请求选取的并不是服务器A，而是C，但就总体效果而言如果请求数量足够多最终可以实现让客户的请求在整体上根据服务器的权值在各个服务器上按照对应比例分布。
- **应用**
应用这个负载均衡逻辑就可以实现对客户端的请求按照服务器的处理能力(权重)进行负载均衡了。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/09/cf8e8b52a0e2fe66ce4100c5b2b99ed4.png)
