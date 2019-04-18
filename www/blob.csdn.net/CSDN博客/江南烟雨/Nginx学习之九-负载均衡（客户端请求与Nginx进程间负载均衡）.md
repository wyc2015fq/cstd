# Nginx学习之九-负载均衡（客户端请求与Nginx进程间负载均衡） - 江南烟雨 - CSDN博客
2013年07月08日 15:19:09[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：6413标签：[Linux																[Nginx																[负载均衡																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=负载均衡&t=blog)](https://so.csdn.net/so/search/s.do?q=Nginx&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)
个人分类：[Nginx](https://blog.csdn.net/xiajun07061225/article/category/1461389)
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
## 本文介绍的负载均衡是针对的客户端请求在多个Nginx进程之间的均衡。注意与客户端请求在多个后端服务器之间的均衡相区别。
## 负载均衡问题的产生
在nginx中，建立连接的时候，会设计负载均衡问题。在多个子进程争抢处理一个新连接事件时，一定只有一个worker子进程最终会成功建立连接，随后它会一直处理这个连接直到连接关闭。那么，就有可能出现这样的情况：有的子进程建立并处理了大部分连接，而有的子进程只处理了少量连接。这对多核CPU架构下的应用是很不利的。因为子进程之间应该是平等的，每个子进程应该尽量独占一个CPU核心。子进程间负载不均衡，必定会影响整个服务的性能。
## 如何解决负载均衡问题
与惊群问题的解决方法一样，只有打开了accept_mutex锁，才能实现子进程间的负载均衡。同时post事件机制也是解决负载均衡问题的关键。可以参考前一篇文章[http://blog.csdn.net/xiajun07061225/article/details/9260535](http://blog.csdn.net/xiajun07061225/article/details/9260535)。
在ngx_event_accept方法建立新连接的过程中，初始化了一个全局变量ngx_accept_disabled。它就是负载均衡机制实现的关键阈值。其定义（/src/event/ngx_event.c）：
ngx_int_t             ngx_accept_disabled;
初始化（sr/event/ngx_event_accept.c）是在函数ngx_event_accept中：
ngx_accept_disabled = ngx_cycle->connection_n / 8  - ngx_cycle->free_connection_n;
这样，在nginx启动的时候其实是个负值：-7/8 * ngx_cycle->connection_n。
依据这个值进行负载均衡的核心代码是在函数ngx_process_events_and_timers中（src/event/ngx.event.c）：
         //负载均衡处理
        if (ngx_accept_disabled > 0) {
            ngx_accept_disabled--;
        } else {
            //调用ngx_trylock_accept_mutex方法，尝试获取accept锁
            if (ngx_trylock_accept_mutex(cycle) == NGX_ERROR) {
                return;
            }
解释一下：
当ngx_accept_disabled 为负数时，不会触发负载均衡操作，正常获取accept锁，试图处理新连接。
当ngx_accept_disabled 为正数时，会触发负载均衡操作。nginx此时不再处理新连接事件，取而代之的仅仅是把变量ngx_accept_disabled 减一，这表示既然经过一轮事件处理，那么负载肯定有所减小，所以要相应调整这个值。
即，当当前使用的连接超过总连接数的7/8的时候才会被触发，值越大，表示负载越重。每次调用process_events的时候只会将ngx_accept_disabled减一，直到ngx_accept_disabled 降到0,即使用的连接数降到总连接数的7/8。这样就减少了该worker进程处理新连接的机会，这样其他较空闲的worker进程就有机会去处理更多的新连接，以达到整个web服务器的均衡效果。
Nginx默认将accept_mutex配置项设置为accept_mutex on。
## 参考资料
《深入理解Nginx》
[http://www.cppblog.com/converse/archive/2009/12/08/102816.html](http://www.cppblog.com/converse/archive/2009/12/08/102816.html)
