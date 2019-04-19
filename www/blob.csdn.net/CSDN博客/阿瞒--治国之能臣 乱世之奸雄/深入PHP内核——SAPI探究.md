# 深入PHP内核——SAPI探究 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2018年03月07日 11:56:54[baofenny](https://me.csdn.net/jiaminbao)阅读数：243
**摘要：**PHP作为一门简单而强大的语言，能够提供很多Web适用的语言特性。从实践出发，继弱类型变量原理探究后，王帅将继续带大家弄清PHP内核中的一些常用部分，本期则是SAPI的深入理解。
SAPI是Server Application Programming Interface（服务器应用编程接口）的缩写。PHP通过SAPI提供了一组接口，供应用和PHP内核之间进行数据交互。
简单的讲，就像函数的输入和输出一样，我们通过Linux命令行执行一段PHP代码，本质是Linux的Shell通过PHP的SAPI传入一组参数，Zend引擎执行后，返回给shell，由shell显示出来的过程。同样的，通过Apache调用PHP，通过Web服务器给SAPI传入数据，Zend引擎执行后，返回给Apache，由Apache显示在页面上。
![](http://cms.csdnimg.cn/article/201409/26/54250531d9869_middle.jpg)
**图1. PHP架构图**
PHP提供很多种形式的接口，包括apache、apache2filter、apache2handler、caudium、cgi 、cgi-fcgi、cli、cli-server、continuity、embed、isapi、litespeed、milter、nsapi、phttpd pi3web、roxen、thttpd、tux和webjames。但是常用的只有5种形式，CLI/CGI（命令行）、Multiprocess（多进程）、Multithreaded（多线程）、FastCGI和Embedded（内嵌）。
PHP提供了一个函数查看当前SAPI接口类型：
```php
string php_sapi_name ( void )
```
### **PHP的运行和加载**
无论使用哪种SAPI，在PHP执行脚本前后，都包含一系列事件：Module的Init(MINT)和Shutdown(MSHUTDOWN)，Request 的Init(RINT)和Shutdown(RSHUTDOWN)。 第一阶段是PHP模块初始化阶段（MINT），可以初始化扩展内部变量、分配资源和注册资源处理器，在整个PHP实例生命周期内，该过程只执行一次。
什么是PHP模块？通过上面的PHP架构图，在PHP中可以使用get_loaded_extensions 函数来查看所有编译并加载的模块/扩展，相当于CLI模式下的php -m。
以PHP的Memcached扩展源代码为例：
```php
PHP_MINIT_FUNCTION(memcached) { 	
	zend_class_entry ce;  	
	memcpy(&memcached_object_handlers,zend_get_std_object_handlers(), sizeof(zend_object_handlers)); 	
memcached_object_handlers.clone_obj = NULL;  	/* 执行了一些类似的初始化操作 */  	
return SUCCESS; 
}
```
第二阶段是请求初始化阶段（RINT），在模块初始化并激活后，会创建PHP运行环境，同时调用所有模块注册的RINT函数，调用每个扩展的请求初始化函数 ，设定特定的环境变量、分配资源或执行其他任务，如审核。
```php
PHP_RINIT_FUNCTION(memcached) { 	
	/* 执行一些关于请求的初始化 */  	
	return SUCCESS; 
}
```
第三阶段，请求处理完成后，会调用PHP_RSHUTDOWN_FUNCTION进行回收，这是每个扩展的请求关闭函数，执行最后的清理工作。Zend引擎执行清理过程、垃圾收集、对之前的请求期间用到的每个变量执行unset。请求完成可能是执行到脚本完成，也可能是调用die()或exit()函数完成
第四阶段，当PHP生命周期结束时候，PHP_MSHUTDOWN_FUNCTION对模块进行回收处理，这是每个扩展的模块关闭函数，用于关闭自己的内核子系统。
```php
PHP_MSHUTDOWN_FUNCTION(memcached) { /* 执行关于模块的销毁工作 */ UNREGISTER_INI_ENTRIES(); return SUCCESS; }
```
### **常见的运行模式**
常见的SAPI模式有五种：
- CLI和CGI模式（单进程模式）
- 多进程模式
- 多线程模式
- FastCGI模式
- 嵌入式
**1. CLI/CGI模式**
CLI和CGI都属于单进程模式，PHP的生命周期在一次请求中完成。也就是说每次执行PHP脚本，都会执行第二部分讲的四个INT和Shutdown事件。
![](http://cms.csdnimg.cn/article/201409/26/54251087a4b70.jpg)
**图2. CGI/CLI生命周期**
**2. 多进程模式（Multiprocess）**
多进程模式可以将PHP内置到Web Server中，PHP可以编译成Apache下的prefork MPM模式和APXS模块，当Apache启动后，会fork很多子进程，每个子进程拥有自己独立的进程地址空间。
![](http://cms.csdnimg.cn/article/201409/26/54251138db3d3.jpg)
**图3. 多进程模式生命周期**
在一个子进程中，PHP的生命周期是调用MINT启动后，执行多次请求（RINT/RSHUTDOWN)，在Apache关闭或进程结束后，才会调用MSHUTDOWN进行回收阶段。 
![](http://cms.csdnimg.cn/article/201409/26/542511593efc6.jpg)
**图4. 多进程的生命周期**
多进程模型中，每个子进程都是独立运行，没有代码和数据共享，因此一个子进程终止退出和重新生成，不会影响其他子进程的稳定。
**3. 多线程模式（Multithreaded)**
Apache2的Worker MPM采用了多线程模型，在一个进程下创建多个线程，在同一个进程地址空间执行。
![](http://cms.csdnimg.cn/article/201409/26/5425121315567.jpg)
**图5. 多线程生命周期**
**4. FastCGI模式**
在我们用的Nginx+PHP-FPM用的就是FastCGI模式，Fastcgi是一种特殊的CGI模式，是一种常驻进程类型的CGI，运行后可以Fork多个进程，不用花费时间动态的Fork子进程，也不需要每次请求都调用MINT/MSHUTDOWN。PHP通过PHP-FPM来管理和调度FastCGI的进程池。Nginx和PHP-FPM通过本地的TCP Socket和Unix Socket 进行通信。
![](http://cms.csdnimg.cn/article/201409/26/5425123c7442f.jpg)
**图6. FastCGI模式生命周期**
PHP-FPM进程管理器自身初始化，启动多个CGI解释器进程等待来自Nginx的请求。当客户端请求达到PHP-FPM，管理器选择到一个CGI进程进行处理，Nginx将CGI环境变量和标准输入发送到一个PHP-CIG子进程。PHP-CGI子进程处理完成后，将标准输出和错误信息返回给Nginx，当PHP-CGI子进程关闭连接时，请求处理完成。PHP-CGI子进程等待着下一个连接。
可以想象CGI的系统开销有多大。每一个Web 请求PHP都必须重新解析php.ini、载入全部扩展并始化全部数据结构。使用FastCGI，所有这些都只在进程启动时发生一次。另外，对于数据库和Memcache的持续连接可以工作。
**5. 内嵌模式（Embedded）**
Embed SAPI是一种特殊的SAPI，允许在C/C++语言中调用PHP提供的函数。这种SAPI和CLI模式一样，按照Module Init => Request Init => Request => Request Shutdown => Module Shutdown的模式运行。
Embed SAPI可以调用PHP丰富的类库，也可以实现高级玩法，比如可以查看PHP的OPCODE（PHP执行的中间码，Zend引擎的指令，由PHP代码生成）。
详细请见： [http://www.laruence.com/2008/09/23/539.html](http://www.laruence.com/2008/09/23/539.html)
### **SAPI的运行机制**
我们以CGI为例，看一下SAPI的运行机制。
```php
static sapi_module_struct cgi_sapi_module = {     
	"cgi-fcgi",                     /* 输出给php_info()使用 */     "CGI/FastCGI",                  /* pretty name */      
	php_cgi_startup,                /* startup 当SAPI初始化时，首先会调用该函数 */     
	php_module_shutdown_wrapper,    /* shutdown  关闭函数包装器，它用来释放所有的SAPI的数据结构、内存等，调用php_module_shutdown */      
	sapi_cgi_activate,              /* activate  此函数会在每个请求开始时调用，它会做初始化，资源分配 */     
	sapi_cgi_deactivate,            /* deactivate  此函数会在每个请求结束时调用，它用来确保所有的数据都得到释放 */      
	sapi_cgi_ub_write,              /* unbuffered write  不缓存的写操作(unbuffered write)，它是用来向SAPI外部输出数据 */     
	sapi_cgi_flush,                 /* flush  刷新输出，在CLI模式下通过使用C语言的库函数fflush实现*/     NULL,                           /* get uid */     
	sapi_cgi_getenv,                /* getenv 根据name查找环境变量 */      
	php_error,                      /* error handler 注册错误处理函数  */      
	NULL,                           /* header handler PHP调用header()时候被调用 */     
	sapi_cgi_send_headers,          /* send headers handler 发送头部信息*/     
	NULL,                           /* send header handler 发送一个单独的头部信息 */      
	sapi_cgi_read_post,             /* read POST data  当请求的方法是POST时，程序获取POST数据，写入$_POST数组 */     
	sapi_cgi_read_cookies,          /* read Cookies 获取Cookie值  */      
	sapi_cgi_register_variables,    /* register server variables 给$_SERVER添加环境变量 */     
	sapi_cgi_log_message,           /* Log message 输出错误信息 */     
	NULL,                           /* Get request time */     
	NULL,                           /* Child terminate */      
	STANDARD_SAPI_MODULE_PROPERTIES 
};
```
由上面代码可见，PHP的SAPI像是面向对象中基类，SAPI.h和SAPI.c包含的函数是抽象基类的声明和定义，各个服务器用的SAPI模式，则是继承了这个基类，并重新定义基类方法的子类。
### **总结**
PHP的SAPI是Zend引擎提供的一组标准交互接口，通过注册初始化、析构、输入、输出等接口，我们可以将应用程序运行在Zend引擎上，也可以把PHP嵌入到类似Apache的Web Server中。PHP常见的SAPI模式有五种，CGI/CLI模式、多进程模式、多线程模式、FastCGI模式和内嵌模式。
了解PHP的SAPI机制意义重大，帮助我们理解PHP的生命周期，并了解如何更好的通过C/C++为PHP编写扩展，并在生命周期中找到提高系统性能的方式。
