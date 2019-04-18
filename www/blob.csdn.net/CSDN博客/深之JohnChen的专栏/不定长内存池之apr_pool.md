# 不定长内存池之apr_pool - 深之JohnChen的专栏 - CSDN博客

2011年01月19日 22:51:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6708


内存池可有效降低动态申请内存的次数，减少与内核态的交互，提升系统性能，减少内存碎片，增加内存空间使用率，避免内存泄漏的可能性，这么多的优点，没有理由不在系统中使用该技术。

内存池分类：

1、不定长内存池。典型的实现有apr_pool、obstack。优点是不需要为不同的数据类型创建不同的内存池，缺点是造成分配出的内存不能回收到池中。这是由于这种方案以session为粒度，以业务处理的层次性为设计基础。

2、定长内存池。典型的实现有LOKI、BOOST。特点是为不同类型的数据结构分别创建内存池，需要内存的时候从相应的内存池中申请内存，优点是可以在使用完毕立即把内存归还池中，可以更为细粒度的控制内存块。
与变长的相比，这种类型的内存池更加通用，另一方面对于大量不同的数据类型环境中，会浪费不少内存。但一般系统主要的数据结构都不会很多，并且都是重复申请释放使用，这种情况下，定长内存池的这点小缺点可以忽略了。

一、apr

apr 是 apache 使用的底层库，apache 是跨平台的，其跨平台正是基于 apr。个人觉得，使用apr有两个好处，一是不用担心跨平台（事实上，我从来就不担心，因为我写的程序，从来都不跨平台）。二是 apr 的 pool 很好用。pool 有两个好处，一是可以理解成内存池，在 pool上分配的内存，是不用释放的，pool 销毁的时候自然会释放这些内存（所以销毁（清理）pool变得异常重要，千万不能忘了）。二是可以理解成资源管理器，分配资源后，然后在pool上注册一个释放资源的函数，pool 销毁（清理）的时候，会调用这个函数，释放资源。例如打开了一个文件，可以在 pool 上注册一个关闭文件的函数，让 pool 替你关闭文件。也可以在不销毁（清理）pool 时，手动的释放。具体可以看参考[apr手册](http://apr.apache.org/docs/apr/1.3/files.html)。

APR的核心就是Apache的资源管理（池），我们将在本章的后面部分进行更加详细的介绍。表3-1列出了APR中的所有模块。

表3-1  APR模块
|名称|目的|
|----|----|
|apr_allocator|内存分配，内部使用|
|apr_atomic|原子操作|
|apr_dso|动态加载代码（.so/.dll）|
|apr_env|读取/设定环境变量|
|apr_errno|定义错误条件和宏|
|apr_file_info|文件系统对象和路径的属性|
|apr_file_io|文件系统输入/输出|
|apr_fnmatch|文件系统模式匹配|
|apr_general|初始化/终结，有用的宏|

|名称|目的|
|----|----|
|apr_getopt|命令参数|
|apr_global_mutex|全局锁|
|apr_hash|哈希表|
|apr_inherit|文件句柄继承助手|
|apr_lib|奇数和末端|
|apr_mmap|内存映射|
|apr_network_io|网络输入/输出（套接字）|
|apr_poll|投票|
|apr_pools|资源管理|
|apr_portable|APR到本地映射转换|
|apr_proc_mutex|进程锁|
|apr_random|随机数|
|apr_ring|环数据结构和宏|
|apr_shm|共享内存|
|apr_signal|信号处理|
|apr_strings|字符串操作|
|apr_support|内部支持函数|
|apr_tables|表格和数组函数|
|apr_thread_cond|线程条件|
|apr_thread_mutex|线程锁|
|apr_thread_proc|线程和进程函数|
|apr_thread_rwlock|读写锁|
|apr_time|时间/日期函数|
|apr_user|用户和组ID服务|
|apr_version|APR版本|
|apr_want|标准头文件支持|

表3-2  APU模块
|名称|目的|
|----|----|
|apr_anylock|透明的、任何锁的封装|
|apr_base64|Base-64编码|
|apr_buckets|Buckets/Bucket brigade|
|apr_date|时间字符串解析|
|apr_dbd|针对SQL数据库的常用API|
|apr_dbm|针对DBM数据库的常用API|
|apr_hooks|钩子实现宏|
|apr_ldap|LDAP授权API|
|apr_ldap_init|LDAP初始化API，主要应用在和LDAP服务器的初始安全连接|
|apr_ldap_option|设置LDAP选项的API|
|apr_ldap_url|解析和处理LDAP URL的API|
|apr_md4|MD4编码|
|apr_md5|MD5编码|
|apr_optional|可选函数|
|apr_optional_hooks|可选钩子|
|apr_queue|线程安全的FIFO队列|
|apr_reslist|资源池|
|apr_rmm|可再定位地址的内存|

|名称|目的| |
|----|----|----|
|apr_sdbm|SDBM库| |
|apr_sha1|SHA1编码| |
|apr_strmatch|字符串模式匹配| |
|apr_uri|URI解析/构造| |
|apr_uuid|用户标识| |
|apr_xlate|字符集转换（I18N）| |
|apr_xml|XML解析| |
||||

基本的约定

APR和APR-UTIL采用了一些约定，使得它们的API具有同质性，并且易于使用。

3.3.1  参考手册：API文档和Doxygen

APR和APU在代码层都有非常好的文档。每一个公开函数和数据类型都在定义它们的头文件中进行了注释，使用了doxygen 友好的格式。那些头文件，或者doxygen生成的文档，为程序员提供了完整的API参考手册。如果你安装了doxygen，那么就可以通过make dox命令从源代码中生成你自己版本的APR参考手册。

3.3.2  命名空间

所有的APR和APU的公开接口都使用了字符串前缀“apr_”（数据类型和函数）和“APR_”（宏），这就为APR定义了一个“保留”的命名空间。

在APR命名空间中，绝大部分的APR和APU模块使用了二级命名空间。这个约定通常基于正在讨论的那个模块的名字。例如，模块apr_dbd中的所有函数使用字符串“apr_dbd_”前缀。有时候使用一个明显的描述性的二级命名空间。例如，在模块apr_network_io中套接字操作使用“apr_socket_”前缀。

3.3.3  声明的宏

APR和APU的公开函数使用类似于APR_DECLARE、APU_DECLARE和APR_ DECLARE_NONSTD的宏进行声明。例如：

APR_DECLARE(apr_status_t) apr_initialize(void);

在很多的平台上，这是一个空声明，并且扩展为

apr_status_t apr_initialize(void);

例如在Windows的Visual C++平台上，需要使用它们自己的、非标准的关键字，例如“_dllexport”来允许其他的模块使用一个函数，这些宏就需要扩展以适应这些需要的关键字。

3.3.4  apr_status_t和返回值

在APR和APU中广泛采用的一个约定是：函数返回一个状态值，用来为调用者指示成功或者是返回一个错误代码。这个类型是apr_status_t，在apr_errno.h中定义，并赋予整数值。因此一个APR函数的常见原型就是：

APR_DECLARE(apr_status_t) apr_do_something(…function args…);

返回值应当在逻辑上进行判断，并且实现一个错误处理函数（进行回复或者对错误进行进一步的描述）。返回值APR_SUCCESS意味着成功，我们通常可以用如下的方式进行错误处理结构：

apr_status_t rv;
...
rv = apr_do_something(... args ...);
if (rv != APR_SUCCESS) {
/* 记录一个错误 */
return rv;
}

有时候我们可能需要做得更多。例如，如果do_something是一个非闭塞的I/O操作并且返回APR_EAGAIN，我们可能需要重试这个操作。
有些函数返回一个字符串（char *或者const char *）、一个void *或者void。这些函数就被认为在没有失败条件或者在错误发生时返回一个空指针。

3.3.5  条件编译

本质上说，APR的一些特色可能并不是每个平台都支持的。例如，FreeBSD在5.x版本之前并没有适合Apache的本地线程实现，因此线程在APR中就不被支持（除非编译时手动设置相应的操作）。

为了在这种情况下应用程序依然能够工作，APR为这些情况提供了APR_HAS_*宏。如果一个应用处于这种情况，它应当使用这些宏进行条件编译。例如，一个模块执行了一个操作，这个操作可能导致在多线程环境下的竞争条件，那么它就可能使用以下的方式。

#if APR_HAS_THREADS
rv = apr_thread_mutex_lock(mutex);
if (rv != APR_SUCCESS) {
/* 记录一个错误 */
/* 放弃关键的操作*/
}
#endif

    /* ... 在这里执行关键代码... */

#if APR_HAS_THREAD
apr_thread_mutex_unlock(mutex);
#endif

二、apr_pool内存池。

pool本身并不直接从物理内存中分配或释放,而是通过allocator(内存分配器)来统一管理,可以为新池创建新的allocator(内存分配器),但通常使用默认的全局allocator(内存分配器),这样更有助于统一的内存管理。pool采用的是树形的结构,在初始化内存池(apr_pool_initialize)时,建立根池,和全局allocator(内存分配器),以后建立的都是根结点的子孙结点可以从pool中分配任何大小的内存块,但释放的单位为pool,就是说pool释放之前,从pool分配出的内存不能单独释放,看起来好像有点浪费。这里要注意的是，有些分配的内存块，清除时有特别操作,这样就需要要带清除函数，在分配之后用apr_pool_cleanup_register注册清除时用的函数。特殊的，如果内存块里是线程对象，也不能用一般的清除函数，应该用apr_pool_note_subprocess注册清除操作。

apr_pool中主要有3个对象，allocator、pool、block。pool从allocator申请内存，pool销毁的时候把内存归还allocator，allocator销毁的时候把内存归还给系统，allocator有一个owner成员，是一个pool对象，allocator的owner销毁的时候，allocator被销毁。在apr_pool中并无block这个单词出现，这里大家可以把从pool从申请的内存称为block，使用apr_palloc申请block，block只能被申请，没有释放函数，只能等pool销毁的时候才能把内存归还给allocator，用于allocator以后的pool再次申请。

常见函数：

对系统内存池初始化,全局的,一个进程只要初始化一次

apr_status_tapr_pool_initialize (void)

销毁内存池对象,及内部的结构和子内存池

voidapr_pool_terminate (void)

创建一个新的内存池

apr_status_tapr_pool_create_ex (apr_pool_t **newpool, apr_pool_t *parent, apr_abortfunc_t abort_fn, apr_allocator_t *allocator)

创建一个新的内存池,apr_pool_create_ex的使用默认参数简化版

apr_status_tapr_pool_create (apr_pool_t **newpool, apr_pool_t *parent)

获取内存池使用的内存分配器

apr_allocator_t *apr_pool_allocator_get (apr_pool_t *pool)

清除一个内存池的内容,清除后内容为空，但可以再使用

voidapr_pool_clear (apr_pool_t *p)

释构一个内存池

voidapr_pool_destroy (apr_pool_t *p)

从池中分配内存

void *apr_palloc (apr_pool_t *p, apr_size_t size)

从池中分配内存,并将分配出来的内存置0

void *apr_pcalloc (apr_pool_t *p, apr_size_t size)

设置内存分配出错时的调用函数

voidapr_pool_abort_set (apr_abortfunc_t abortfunc, apr_pool_t *pool)

获取内存分配出错时的调用函数

apr_abortfunc_tapr_pool_abort_get (apr_pool_t *pool)

获取池的父池

apr_pool_t *apr_pool_parent_get (apr_pool_t *pool)

判断a是否是b的祖先

intapr_pool_is_ancestor (apr_pool_t *a, apr_pool_t *b)

为内存池做标签

voidapr_pool_tag (apr_pool_t *pool, const char *tag)

设置与当前池关联的数据

apr_status_tapr_pool_userdata_set (const void *data, const char *key, apr_status_t(*cleanup)(void *), apr_pool_t *pool)

设置与当前池关联的数据,与apr_pool_userdata_set类似,但内部不拷贝数据的备份,如常量字符串时就有用

apr_status_tapr_pool_userdata_setn (const void *data, const char *key, apr_status_t(*cleanup)(void *), apr_pool_t *pool)

获取与当前池关联的数据

apr_status_tapr_pool_userdata_get (void **data, const char *key, apr_pool_t *pool)

注册内存块的清除函数，每块销毁时要特别处理的都要注册下，在cleanups里加入一个项

voidapr_pool_cleanup_register (apr_pool_t *p, const void *data, apr_status_t(*plain_cleanup)(void *), apr_status_t(*child_cleanup)(void *))

删除内存块的清除函数，从cleanups里移除一个项，放入free_cleanups中

voidapr_pool_cleanup_kill (apr_pool_t *p, const void *data, apr_status_t(*cleanup)(void *))

用新的child_cleanup，替换原来老的child_cleanup

voidapr_pool_child_cleanup_set (apr_pool_t *p, const void *data, apr_status_t(*plain_cleanup)(void *), apr_status_t(*child_cleanup)(void *))

执行内存块的清除函数，进从清除函数的队列cleanups中删除

apr_status_tapr_pool_cleanup_run (apr_pool_t *p, void *data, apr_status_t(*cleanup)(void *))

一个空的内存块清除函数

apr_status_tapr_pool_cleanup_null (void *data)

执行所有的子清除函数child_cleanup

voidapr_pool_cleanup_for_exec (void)

带调试信息内存池函数,功能跟上面的一样，只是多了调试信息

apr_status_tapr_pool_create_ex_debug (apr_pool_t **newpool, apr_pool_t *parent, apr_abortfunc_t abort_fn, apr_allocator_t *allocator, const char *file_line)

voidapr_pool_clear_debug (apr_pool_t *p, const char *file_line)

voidapr_pool_destroy_debug (apr_pool_t *p, const char *file_line)

void *apr_palloc_debug (apr_pool_t *p, apr_size_t size, const char *file_line)

void *apr_pcalloc_debug (apr_pool_t *p, apr_size_t size, const char *file_line)

一般可以不调用创建allocator的函数，而是使用的默认全局allocator。但是apr_pool提供了一系列函数操作allocator，可以自己调用这些函数：

apr_allocator_create

apr_allocator_destroy

apr_allocator_alloc

apr_allocator_free创建销毁allocator 

apr_allocator_owner_set

apr_allocator_owner_get设置获取owner 

apr_allocator_max_free_set 设置pool销毁的时候内存是否直接归还到操作系统的阈值

apr_allocator_mutex_set

apr_allocator_mutex_get设置获取mutex，用于多线程

apr_pool的一个大缺点就是从池中申请的内存不能归还给内存池，只能等pool销毁的时候才能归还。为了弥补这个缺点，apr_pool的实际使用中，可以申请拥有不同生命周期的内存池。

三、实例

#include"stdafx.h"

#include"apr_pools.h"

#include<new>

#pragmacomment(lib,"libapr-1.lib")

intmain(intargc, char* argv[])

{

apr_pool_t *root;

apr_pool_initialize();//初始化全局分配子（allocator），并为它设置mutext，以用于多线程环境，初始化全局池，指定全局分配子的owner是全局池

apr_pool_create(&root,NULL);//创建根池(默认父池是全局池)，根池生命期为进程生存期。分配子默认为全局分配子

{

apr_pool_t *child;

apr_pool_create(&child,root);//创建子池，指定父池为root。分配子默认为父池分配子

void *pBuff=apr_palloc(child,sizeof(int));//从子池分配内存

int *pInt=new (pBuff)int(5);//随便举例下基于已分配内存后，面向对象构造函数的调用。

printf("pInt=%d/n",*pInt);

{

typedefstructStudentInfo{

charszName[20];

boolnSex;

};

apr_pool_t *grandson;

apr_pool_create(&grandson,root);

void *pBuff2=apr_palloc(grandson,sizeof(StudentInfo));

StudentInfo *pSI=new (pBuff2)StudentInfo();

strcpy(pSI->szName,"zhangsan");

pSI->nSex = 1;

printf("Name=%s,sex=%d/n",pSI->szName,pSI->nSex);

apr_pool_destroy(grandson);

}

apr_pool_destroy(child);//释放子池，将内存归还给分配子

}

apr_pool_destroy(root);//释放父池，

apr_pool_terminate();//释放全局池，释放全局allocator，将内存归还给系统

return 0;

}

参考资料：

[apr](http://apr.apache.org/)官方网站：http://apr.apache.org/

