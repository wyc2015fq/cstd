# Nginx 模块自主开发八: 总结 Nginx框架的流程 - youbingchen的博客 - CSDN博客





2016年07月01日 15:16:06[youbingchen](https://me.csdn.net/youbingchen)阅读数：796








> 
前面两篇博客：[Nginx 模块自主开发六：源码剖析配置文件解析过程](http://blog.csdn.net/youbingchen/article/details/51782491)和 [Nginx 模块自主开发七：源码剖析整个Nginx框架](http://blog.csdn.net/youbingchen/article/details/51789088)从源码层面上解析Nginx框架，这样导致太过于 细节，所以本篇博客将带领你系统上过一遍Nginx框架


# Nginx主框架的流程

当启动Nginx程序时候，Nginx从入口函数Main()开始执行，检查命令行参数和环境变量，初始化ngx_modules数组，然后进入初始化函数ngx_init_cycle()：
- 创建cycle->conf_ctx数组
- 调用 所有 的core模块的create_conf函数指针，创建出配置结构体，用来存储具体参数
- 设置ngx_conf_t的各个字段，作为配置解析的参数传递，里面包含很多解析需要的信息
- 执行ngx_conf_parse()，在模板数组中查找配置指令对应的解析函数并处理
- 反复执行步骤4，直到整个配置文件处理完毕
- 调用core模块init_conf函数指针，初始化core模块的配置
- 创建并初始化所有创建的共享内存
- 开启监听，初始化所有的模块

# 小结

**Nginx框架在ngx_init_cycle()里面只处理核心模块，并没有涉及到具体的模块（比如:event、http模块），这就是Nginx一个很好的 特性：实现模块，往往很多人认为只有面向对象才有 设计模式，在这一点C语言 实现的Nginx也具有这个特点，这样做 ，可以让使用Nginx的人员很容易设计独立的模块出来。  核心模块只有当解析完整个配置文件之后才能确定核心模块的相关配置信息，如果有些参数并没有配置，那么就可以在这个时候进行初始化，设置默认值。**

> 
上面主要还是放在Nginx主框架的流程上面。接下来 就结合 一个具体的模块，来讲解


# HTTP框架流程

> 
ngx_http_module模块定义 了Nginx的 HTTP框架，核心 函数就是解析http块的ngx_http_block()函数，他与ngx_init_cycle()的功能和流程类似：

- 创建ngx_http_conf_ctx_t结构，里面有三个配置数组main_conf，srv_conf，loc_conf；
- 调用所有的http模块的 create_xxx_conf()填充数组。
- 设置ngx_conf_t的各个字段，作为解析http模块的基本 环境 
- 准备 工作，执行每个模块的preconfiguration函数指针
- 调用 ngx_conf_parse()，解析http块内的所有指令、直至解析完毕 （这一步就是上面Nginx框架流程的递归开始，导致反复执行步骤4）
- 调用 init_main_conf()函数指针，初始化main配置数据；
- 调用所有http模块的merge_srv_conf函数指针，合并main和 srv域的配置数据
- 最后执行postconfiguration，完成整个http块的 配置解析

# 小结 

> 
server和location块解析与http块 类似，区别 在于ngx_http_conf_ctx_t结构里数组的填充 ，可以使用上层数据，无须重复调用preconfiguration、postconfiguration 等函数指针




