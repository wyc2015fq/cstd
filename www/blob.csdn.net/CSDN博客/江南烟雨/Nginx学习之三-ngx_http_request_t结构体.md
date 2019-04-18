# Nginx学习之三-ngx_http_request_t结构体 - 江南烟雨 - CSDN博客
2013年06月27日 16:41:50[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：15172
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
ngx_http_request_s是nginx中非常重要的一个结构体，贯穿于htpp请求处理的整个过程中。
下面解释了ngx_http_request_s结构体中与HTTP框架相关的重要的成员变量。
```cpp
struct ngx_http_request_s {
    uint32_t                          signature;         /* "HTTP" */
    //请求对应的客户端连接
    ngx_connection_t                 *connection;
    //指向存放所有HTTP模块的上下文结构体的指针数组
    void                            **ctx;
    //指向请求对应的存放main级别配置结构体的指针数组
    void                            **main_conf;
    //指向请求对应的存放srv级别配置结构体的指针数组
    void                            **srv_conf;
    //指向请求对应的存放loc级别配置结构体的指针数组
    void                            **loc_conf;
    /*
     * 在接收完http头部，第一次在业务上处理http请求时，http框架提供的处理方法是ngx_http_process_request。
	 但如果该方法无法一次处理完该请求的全部业务，在归还控制权到epoll时间模块后，该请求再次被回调时，
	 将通过Ngx_http_request_handler方法来处理，而这个方法中对于可读事件的处理就是调用read_event_handler处理请求。
	 也就是说，http模块希望在底层处理请求的读事件时，重新实现read_event_handler方法
    */
    ngx_http_event_handler_pt         read_event_handler;
    //与上面的方法类似
    ngx_http_event_handler_pt         write_event_handler;
#if (NGX_HTTP_CACHE)
    ngx_http_cache_t                 *cache;
#endif
    //upstream机制用到的结构体
    ngx_http_upstream_t              *upstream;
    ngx_array_t                      *upstream_states;
                                         /* of ngx_http_upstream_state_t */
    //这个请求的内存池
    ngx_pool_t                       *pool;
    //用于接收http请求内容的缓冲区，主要接收http头部
    ngx_buf_t                        *header_in;
    //ngx_http_process_request_headers在接收、解析完http请求的头部后，会把解析完的每一个http头部加入到headers_in的headers链表中，同时会构造headers_in中的其他成员
    ngx_http_headers_in_t             headers_in;
    //http模块会把想要发送的http相应信息放到headers_out中，期望http框架将headers_out中的成员序列化为http响应包发送给用户
    ngx_http_headers_out_t            headers_out;
    //接收请求中包体的数据结构
    ngx_http_request_body_t          *request_body;
    //延迟关闭连接的时间
    time_t                            lingering_time;
    //当前请求初始化时的时间
    time_t                            start_sec;
    ngx_msec_t                        start_msec;
    //下面的9个成员是函数ngx_http_process_request_line方法在接收、解析http请求行时解析出的信息
    ngx_uint_t                        method;//方法名
    ngx_uint_t                        http_version;//协议版本
    ngx_str_t                         request_line;
    ngx_str_t                         uri;//用户请求中的uri
    ngx_str_t                         args;//用户请求中的url参数
    ngx_str_t                         exten;//用户请求的文件扩展名
    ngx_str_t                         unparsed_uri;//没有进行URL解码的原始请求
    ngx_str_t                         method_name;//用户请求中的方法名字符串
    ngx_str_t                         http_protocol;//其data成员指向请求中http起始地址
    /*表示需要发送给客户端的http响应。out中保存着由headers_out中序列化后的表示http头部的TCP流。
     * 在调用ngx_http_output_filter方法后，out中还会保存着待发送的http包体，它是实现异步发送http响应的关键。*/
    ngx_chain_t                      *out;
    /*当前请求既有可能是用户发来的请求，也可能是派生出的子请求。
     * 而main标识一系列相关的派生子请求的原始请求。
     * 一般可通过main和当前请求的地址是否相等来判断当前请求是否为用户发来的原始请求。*/
    ngx_http_request_t               *main;
    //当前请求的父请求（不一定是原始请求）
    ngx_http_request_t               *parent;
    //与subrequest子请求相关的功能
    ngx_http_postponed_request_t     *postponed;
    ngx_http_post_subrequest_t       *post_subrequest;
    //所有的子请求都是通过这个单链表链接起来的
    ngx_http_posted_request_t        *posted_requests;
    /*全局的ngx_http_phase_engine_t结构体中定义了一个ngx_http_phase_handler_t回答方法组成的数组。
     * 而phase_handler成员则与该数组配合使用。表示请求下次应当执行phase_handler作为序列号指定的数组中的回调方法*/
    ngx_int_t                         phase_handler;
    //表示NGX_HTTP_CONTENT_PHASE阶段提供给http模块处理请求的一种方式，它指向http模块实现的请求处理方法
    ngx_http_handler_pt               content_handler;
    //在NGX_HTTP_ACCESS_PHASE节点需要判断请求是否具有访问权限时，通过access_code来传递http模块的handler回调方法的返回值，如果为0表示具备权限。否则不具备。
    ngx_uint_t                        access_code;
    ngx_http_variable_value_t        *variables;
#if (NGX_PCRE)
    ngx_uint_t                        ncaptures;
    int                              *captures;
    u_char                           *captures_data;
#endif
    size_t                            limit_rate;
    /* used to learn the Apache compatible response length without a header */
    size_t                            header_size;
    //http请求的全部长度，包括http包体
    off_t                             request_length;
    ngx_uint_t                        err_status;
    ngx_http_connection_t            *http_connection;
#if (NGX_HTTP_SPDY)
    ngx_http_spdy_stream_t           *spdy_stream;
#endif
    ngx_http_log_handler_pt           log_handler;
    //在这个请求中如果打开了某些资源，并需要在请求结束时释放，那么需要把定义的释放资源的方法添加到这个成员
    ngx_http_cleanup_t               *cleanup;
    unsigned                          subrequests:8;
	//引用计数一般都作用于这个请求的原始请求上
    //引用计数，每当派生出子请求时，原始请求的count成员都会加一
    unsigned                          count:8;
    //阻塞标志位，目前仅由aio使用
    unsigned                          blocked:8;
    //标志位：为1表示蛋清请求正在使用异步IO
    unsigned                          aio:1;
    unsigned                          http_state:4;
    /* URI with "/." and on Win32 with "//" */
    unsigned                          complex_uri:1;
    /* URI with "%" */
    unsigned                          quoted_uri:1;
    /* URI with "+" */
    unsigned                          plus_in_uri:1;
    /* URI with " " */
    unsigned                          space_in_uri:1;
    unsigned                          invalid_header:1;
    unsigned                          add_uri_to_alias:1;
    unsigned                          valid_location:1;
    unsigned                          valid_unparsed_uri:1;
    //标志位：为1时表示URL发生过rewrite重写
    unsigned                          uri_changed:1;
    //表示使用rewrite重写URL的次数
    unsigned                          uri_changes:4;
    unsigned                          request_body_in_single_buf:1;
    unsigned                          request_body_in_file_only:1;
    unsigned                          request_body_in_persistent_file:1;
    unsigned                          request_body_in_clean_file:1;
    unsigned                          request_body_file_group_access:1;
    unsigned                          request_body_file_log_level:3;
    unsigned                          subrequest_in_memory:1;
    unsigned                          waited:1;
#if (NGX_HTTP_CACHE)
    unsigned                          cached:1;
#endif
#if (NGX_HTTP_GZIP)
    unsigned                          gzip_tested:1;
    unsigned                          gzip_ok:1;
    unsigned                          gzip_vary:1;
#endif
    unsigned                          proxy:1;
    unsigned                          bypass_cache:1;
    unsigned                          no_cache:1;
    /*
     * instead of using the request context data in
     * ngx_http_limit_conn_module and ngx_http_limit_req_module
     * we use the single bits in the request structure
     */
    unsigned                          limit_conn_set:1;
    unsigned                          limit_req_set:1;
#if 0
    unsigned                          cacheable:1;
#endif
    unsigned                          pipeline:1;
    unsigned                          chunked:1;
    unsigned                          header_only:1;
    //标志位，为1表示当前请求时keepalive请求
    unsigned                          keepalive:1;
    //延迟关闭标志位
    unsigned                          lingering_close:1;
    //标志位：为1表示正在丢弃http请求中的包体
    unsigned                          discard_body:1;
    //标志位：为1表示请求的当前状态是在做内部跳转
    unsigned                          internal:1;
    unsigned                          error_page:1;
    unsigned                          ignore_content_encoding:1;
    unsigned                          filter_finalize:1;
    unsigned                          post_action:1;
    unsigned                          request_complete:1;
    unsigned                          request_output:1;
    //标志位：为1表示发生给客户端的http响应头已经发送
    unsigned                          header_sent:1;
    unsigned                          expect_tested:1;
    unsigned                          root_tested:1;
    unsigned                          done:1;
    unsigned                          logged:1;
    //标志位，表示缓冲中是否有待发送内容
    unsigned                          buffered:4;
    unsigned                          main_filter_need_in_memory:1;
    unsigned                          filter_need_in_memory:1;
    unsigned                          filter_need_temporary:1;
    unsigned                          allow_ranges:1;
#if (NGX_STAT_STUB)
    unsigned                          stat_reading:1;
    unsigned                          stat_writing:1;
#endif
    /* used to parse HTTP headers */
    //状态机解析http时使用state来表示当前的解析状态，需要检查是否构成完成的http请求行
    ngx_uint_t                        state;
    ngx_uint_t                        header_hash;
    ngx_uint_t                        lowcase_index;
    u_char                            lowcase_header[NGX_HTTP_LC_HEADER_LEN];
    u_char                           *header_name_start;
    u_char                           *header_name_end;
    u_char                           *header_start;
    u_char                           *header_end;
    /*
     * a memory that can be reused after parsing a request line
     * via ngx_http_ephemeral_t
     */
    u_char                           *uri_start;
    u_char                           *uri_end;
    u_char                           *uri_ext;
    u_char                           *args_start;
    u_char                           *request_start;
    u_char                           *request_end;
    u_char                           *method_end;
    u_char                           *schema_start;
    u_char                           *schema_end;
    u_char                           *host_start;
    u_char                           *host_end;
    u_char                           *port_start;
    u_char                           *port_end;
    unsigned                          http_minor:16;
    unsigned                          http_major:16;
};
```

