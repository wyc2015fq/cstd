# PHP 内核分析（1）：sapi_module_struct - 文章 - 伯乐在线
原文出处： [叶剑峰](http://www.cnblogs.com/yjf512/p/6084963.html)
这里阅读的php版本为PHP-7.1.0 RC3，阅读代码的平台为linux
首先是寻找php的入口，php有很多种模式，apache，php-fpm, cli模式，我要入手的话，只能先从最简单的cli模型开始。
那么，我需要先寻找


```
php -r 'echo 12;'
```
这个命令是如何执行的。
首先还是寻找main入口，由于我们看的是命令行的php程序。所以，这个入口在sapi/cli/php_cli.c中。
首先是定义一系列的变量


```
int c;
zend_file_handle file_handle;
int behavior = PHP_MODE_STANDARD;
char *reflection_what = NULL;
volatile int request_started = 0;
volatile int exit_status = 0;
char *php_optarg = NULL, *orig_optarg = NULL;
int php_optind = 1, orig_optind = 1;
char *exec_direct=NULL, *exec_run=NULL, *exec_begin=NULL, *exec_end=NULL;
char *arg_free=NULL, **arg_excp=&arg_free;
char *script_file=NULL, *translated_path = NULL;
int interactive=0;
int lineno = 0;
const char *param_error=NULL;
int hide_argv = 0;
```
然后是这个
sapi_module_struct *sapi_module = &cli_sapi_module;
这是一个sapi_module_struct结构，这个结构是sapi中最重要的数据结构。它的定义在main/SAPI.h中。
下面是增加了注释的代码：


```
struct _sapi_module_struct {  // SAPI模块结构
    char *name; // 应用层名称，比如cli,cgi等
    char *pretty_name; // 应用层更易读的名字，比如cli对应的就是Command Line Interface
    int (*startup)(struct _sapi_module_struct *sapi_module); // 当一个应用要调用php的时候，这个模块启动的时候会调用的函数
    int (*shutdown)(struct _sapi_module_struct *sapi_module); // 当一个应用要调用php的时候，这个模块结束的时候会调用的函数
    int (*activate)(void); // 在处理每个request的时候，激活需要调用的函数
    int (*deactivate)(void); // 在处理完每个request的时候，收尾时候要调用的函数
    size_t (*ub_write)(const char *str, size_t str_length); // 这个函数告诉php如何输出数据
    void (*flush)(void *server_context); // 提供给php的刷新缓存的函数指针
    zend_stat_t *(*get_stat)(void); // 用来判断要执行文件的权限，来判断是否有执行权限
    char *(*getenv)(char *name, size_t name_len); // 获取环境变量的方法
    void (*sapi_error)(int type, const char *error_msg, ...) ZEND_ATTRIBUTE_FORMAT(printf, 2, 3); // 错误处理方法
    int (*header_handler)(sapi_header_struct *sapi_header, sapi_header_op_enum op, sapi_headers_struct *sapi_headers); // 这个函数会在我们调用header()的时候被调用
    int (*send_headers)(sapi_headers_struct *sapi_headers); // 发送所有的header
    void (*send_header)(sapi_header_struct *sapi_header, void *server_context); // 单独发送某一个header
    size_t (*read_post)(char *buffer, size_t count_bytes); // 如何获取HTTP POST中的数据
    char *(*read_cookies)(void);  // 如何获取cookie中的数据
    void (*register_server_variables)(zval *track_vars_array); // 这个函数可以给$_SERVER中获取变量
    void (*log_message)(char *message, int syslog_type_int); // 输出错误信息函数
    double (*get_request_time)(void); // 获取请求时间的函数
    void (*terminate_process)(void);  // TODO: 调用exit的时候调用的方法
    char *php_ini_path_override;  // PHP的ini文件被复写了所复写的地址
    void (*default_post_reader)(void); // 这里和前面的read_post有个差别，read_post负责如何获取POST数据，而这里的函数负责如何解析POST数据
    void (*treat_data)(int arg, char *str, zval *destArray); // 对数据进行处理，比如进行安全过滤等。 default_post_reader/tread_data/input_filter是三个能对输入进行过滤和处理的函数
    char *executable_location; // 执行的地理位置
    int php_ini_ignore; // 是否不使用任何ini配置文件，比如php －n 就将这个位置设置为1
    int php_ini_ignore_cwd; // 不在当前路径寻找php.ini
    int (*get_fd)(int *fd); // 获取执行文件的fd
    int (*force_http_10)(void); // 强制使用http1.0
    int (*get_target_uid)(uid_t *); // 获取执行程序的uid
    int (*get_target_gid)(gid_t *); // 获取执行程序的gid
    unsigned int (*input_filter)(int arg, char *var, char **val, size_t val_len, size_t *new_val_len); // 对输入进行过滤。比如将输入参数填充到自动全局变量$_GET, $_POST, $_COOKIE中
    void (*ini_defaults)(HashTable *configuration_hash); // 默认的ini配置
    int phpinfo_as_text; // 是否打印phpinfo信息
    char *ini_entries; // 有没有附带的ini配置，比如使用php -d date.timezone=America/Adak,可以在命令行中设置时区
    const zend_function_entry *additional_functions; // 每个SAPI模块特有的一些函数注册，比如cli的cli_get_process_title
    unsigned int (*input_filter_init)(void); // TODO:
};
```
那么我们看下cli的SAPI的module是什么样子的呢？
其中我把里面原先有的STANDARD_SAPI_MODULE_PROPERTIES宏给解出来展示如下：


```
static sapi_module_struct cli_sapi_module = {
    "cli",                            /* name */
    "Command Line Interface",        /* pretty name */
    php_cli_startup,                /* startup */
    php_module_shutdown_wrapper,    /* shutdown */
    NULL,                            /* activate */
    sapi_cli_deactivate,            /* deactivate */
    sapi_cli_ub_write,                /* unbuffered write */
    sapi_cli_flush,                    /* flush */
    NULL,                            /* get uid */
    NULL,                            /* getenv */
    php_error,                        /* error handler */
    sapi_cli_header_handler,        /* header handler */
    sapi_cli_send_headers,            /* send headers handler */
    sapi_cli_send_header,            /* send header handler */
    NULL,                            /* read POST data */
    sapi_cli_read_cookies,          /* read Cookies */
    sapi_cli_register_variables,    /* register server variables */
    sapi_cli_log_message,            /* Log message */
    NULL,                            /* Get request time */
    NULL,                            /* Child terminate */
    NULL, /* php_ini_path_override   */ 
    NULL, /* default_post_reader     */ 
    NULL, /* treat_data              */ 
    NULL, /* executable_location     */ 
    0,    /* php_ini_ignore          */ 
    0,    /* php_ini_ignore_cwd      */ 
    NULL, /* get_fd                  */ 
    NULL, /* force_http_10           */ 
    NULL, /* get_target_uid          */ 
    NULL, /* get_target_gid          */ 
    NULL, /* input_filter            */ 
    NULL, /* ini_defaults            */ 
    0,    /* phpinfo_as_text;        */ 
    NULL, /* ini_entries;            */ 
    NULL, /* additional_functions    */ 
    NULL  /* input_filter_init       */
};
```
有几个点可以总结：
cli模式是不需要发送header的，所以对应header处理的三个函数


```
sapi_cli_header_handler
sapi_cli_send_headers
sapi_cli_send_header
```
实际上都是空实现。
cookie也是同样道理


```
sapi_cli_read_cookies
```
其他的一些定义的函数，等到我们遇到的时候再分析吧。
# main
回到main函数，根据上面的那个结构，我们就理解了


```
argv = save_ps_args(argc, argv); //这里获取一次当前执行进程的参数，环境变量等。为的是对特定平台，修正下argv变量以供后续使用。
cli_sapi_module.additional_functions = additional_functions; // cli模式特有的函数
```
# signal


```
#ifdef HAVE_SIGNAL_H
#if defined(SIGPIPE) && defined(SIG_IGN)
     // 忽略SIGPIPE是为了如果php是socket的客户端，那么当服务端关闭的话，会返回一个PIPE的信号，为的是当前的程序不会因为这个而结束
    signal(SIGPIPE, SIG_IGN); 
#endif
#endif
```
# 参考
[http://php.find-info.ru/php/016/ch23lev1sec1.html](http://php.find-info.ru/php/016/ch23lev1sec1.html)
[http://foio.github.io/php-sapi/](http://foio.github.io/php-sapi/)
