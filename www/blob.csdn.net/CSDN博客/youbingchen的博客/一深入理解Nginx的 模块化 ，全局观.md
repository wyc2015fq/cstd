# 一深入理解Nginx的 模块化 ，全局观 - youbingchen的博客 - CSDN博客





2016年06月15日 14:44:28[youbingchen](https://me.csdn.net/youbingchen)阅读数：4159








# Nginx源码目录结构

```
.
├── auto            自动检测系统环境以及编译相关的脚本
│   ├── cc          关于编译器相关的编译选项的检测脚本
│   ├── lib         nginx编译所需要的一些库的检测脚本
│   ├── os          与平台相关的一些系统参数与系统调用相关的检测
│   └── types       与数据类型相关的一些辅助脚本
├── conf            存放默认配置文件，在make install后，会拷贝到安装目录中去
├── contrib         存放一些实用工具，如geo配置生成工具（geo2nginx.pl）
├── html            存放默认的网页文件，在make install后，会拷贝到安装目录中去
├── man             nginx的man手册
└── src             存放nginx的源代码
    ├── core        nginx的核心源代码，包括常用数据结构的定义，以及nginx初始化运行的核心代码如main函数
    ├── event       对系统事件处理机制的封装，以及定时器的实现相关代码
    │   └── modules 不同事件处理方式的模块化，如select、poll、epoll、kqueue等
    ├── http        nginx作为http服务器相关的代码
    │   └── modules 包含http的各种功能模块
    ├── mail        nginx作为邮件代理服务器相关的代码
    ├── misc        一些辅助代码，测试c++头的兼容性，以及对google_perftools的支持
    |—— stream      nginx支持传输层的 协议：如TCP、UDP协议，UNINX域socket。这个模块是在nginx1.9以后版本才有的。
    └── os          主要是对各种不同体系统结构所提供的系统函数的封装，对外提供统一的系统调用接口。
```

# Nginx模块的类型

关于这个分类，大家各说风云：
- 1.`event module:`搭建 独立于操作系统的事件处理机制的框架，以及 提供各种具体事件的处理。包括`ngx_events_module,ngx_event_core_module,ngx_epoll_module`等。nginx 具体 使用何种事件处理 模块，这 很大依赖具体的操作系统和编译选项。
- 2.`phase handler:`此类型模块也被直接称为`handler`模块，主要负责处理客户端请求并产生待响应的内容，比如`ngx_http_static_moduler`模块，负责客户端的静态页面请求处理并将对应的磁盘 文件准备为响应内容输出。
- 3.`output filter:`也称为`filter`模块，主要 负责处理输出的内容，包括修改输出内容，可以实现对输出的所有的 html的 页面添加 预定义的footbar一类的工作，或者对输出的 图片的URL进行替换的 工作。
- 4.`upstream:`upstream模块实现反向代理，将真正的请求 转发到服务器上，并 从后端服务器上读取响应，发回客户端。`upstream`模块 是一种特殊的`handler`,只不过响应内容也不是真正由自己产生的，而是从后端服务器上读取的。
- 5.`load-balancer:`负载均衡模块，实现特定的 算法，在 众多的后端 服务器中，选择一个服务器出来作为某个请求的转发服务器。

# 配置模块的设计

![这里写图片描述](https://img-blog.csdn.net/20160615144317358)

> 
配置模块是所有 模块的基础，它实现了最基本的配置项 的解析功能（也就是解析 nginx.conf） 

  核心模块的类型是`ngx_core_module`


核心 模块的定义

```cpp
typedef struct {
    ngx_str_t             name;  //核心模块名称
    void               *(*create_conf)(ngx_cycle_t *cycle);  //解析配置项前，Nginx框架会调用create_conf方法
    char               *(*init_conf)(ngx_cycle_t *cycle, void *conf); //解析配置项后，Nginx框架会调用init_conf方法
} ngx_core_module_t;
```

# Nginx模块的加载
- 1.Nginx模块 是 被编译进入Nginx，并不像Apache那样去编译一个so文件，在配置里面指定是否加载。
- 2.解析配置文件时，Nginx的各个模块都有机会去接手处理某个请求，但是URL请求的模块只能有一个 。
- 3.Nginx启动时框架的处理流程 
![这里写图片描述](https://img-blog.csdn.net/20160615144349874)
建议对比这篇博客一起看[Ngnix 的代码分析](http://blog.csdn.net/youbingchen/article/details/51626351)
# Nginx模块的主要轮廓

![nginx_outline](https://img-blog.csdn.net/20160615145948975)




