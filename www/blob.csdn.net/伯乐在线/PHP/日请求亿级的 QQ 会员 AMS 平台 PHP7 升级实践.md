# 日请求亿级的 QQ 会员 AMS 平台 PHP7 升级实践 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [徐汉彬](http://www.jobbole.com/members/xhb7636553) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
QQ会员活动运营平台（AMS），是QQ会员增值运营业务的重要载体之一，承担海量活动运营的Web系统。AMS是一个主要采用PHP语言实现的活动运营平台， CGI日请求3亿左右，高峰期达到8亿。然而，在之前比较长的一段时间里，我们都采用了比较老旧的基础软件版本，就是PHP5.2+Apache2.0（2008年的技术）。尤其从去年开始，随着AMS业务随着QQ会员增值业务的快速增长，性能压力日益变大。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/3c6890f5585e2467e774c6dbe98b3b0c.png)
于是，自2015年5月，我们就开始规划PHP底层升级，最终的目标是升级到PHP7。那时，PHP7尚处于研发阶段，而我们讨论和预研就已经开始了。
## 一、PHP7的学习和预研
1. HHVM和JIT
2015年就PHP性能优化的方案，有另外一个比较重要的角色，就是由Facebook开源的HHVM（HipHop Virtual Machine，HHVM是一个Facebook开源的PHP虚拟机）。HHVM使用JIT（Just In Time，即时编译是种软件优化技术，指在运行时才会去编译字节码为机器码）的编译方式以及其他技术，让PHP代码的执行性能大幅提升。据传，可以将PHP5版本的原生PHP代码提升5-10倍的执行性能。
HHVM起源于Facebook公司，Facebook早起的很多代码是使用PHP来开发的，但是，随着业务的快速发展，PHP执行效率成为越来越明显的问题。为了优化执行效率，Facebook在2008年就开始使用HipHop，这是一种PHP执行引擎，最初是为了将 Fackbook的大量PHP代码转成 C++，以提高性能和节约资源。使用HipHop的PHP代码在性能上有数倍的提升。后来，Facebook将HipHop平台开源，逐渐发展为现在的 HHVM。
HHVM成为一个PHP性能优化解决方案时，PHP7还处于研发阶段。曾经看过部分同学对于HHVM的交流，性能可以获得可观的提升，但是服务运维和PHP语法兼容有一定成本。有一阵子，JIT成为一个呼声很高的东西，很多技术同学建议PHP7也应该通过JIT来优化性能。
2015年7月，我参加了中国PHPCON，听了惠新宸关于PHP7内核的技术分享。实际上，在2013年的时候，惠新宸（PHP7内核开发者）和Dmitry（另一位PHP语言内核开发者之一）就曾经在PHP5.5的版本上做过一个JIT的尝试（并没有发布）。PHP5.5的原来的执行流程，是将PHP代码通过词法和语法分析，编译成opcode字节码（格式和汇编有点像），然后，Zend引擎读取这些opcode指令，逐条解析执行。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/b3b8ab4384588890bf901f471aa62307.png)
而他们在opcode环节后引入了类型推断（TypeInf），然后通过JIT生成ByteCodes，然后再执行。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/a72117db868a6b7c492aca82ac4fbfba.png)
于是，在benchmark（测试程序）中得到非常好的结果，实现JIT后性能比PHP5.5提升了8倍。然而，当他们把这个优化放入到实际的项目WordPress（一个开源博客项目）中，却几乎看不见性能的提升。原因在于测试项目的代码量比较少，通过JIT产生的机器码也不大，而真实的WordPress项目生成的机器码太大，引起CPU缓存命中率下降（CPU Cache Miss）。
总而言之，JIT并非在每个场景下都是点石成金的利器，而脱离业务场景的性能测试结果，并不一定具有代表性。
从官方放出Wordpress的PHP7和HHVM的性能对比可以看出，两者基本处于同一水平。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/eee1b555ff4e6399adde157bea7757cb.jpg)
2. PHP7在性能方面的优化
PHP7是一个比较底层升级，比起PHP5.6的变化比较大，而就性能优化层面，大致可以汇总如下：
（1）将基础变量从struct（结构体）变为union（联合体），节省内存空间，间接减少CPU在内存分配和管理上的开销。
（2）部分基础变量（zend_array、zend_string等）采用内存空间连续分配的方式，降低CPU Cache Miss的发生的概率。CPU从CPU Cache获取数据和从内存获取，它们之间效率相差可以高达100倍。举一个近似的例子，系统从内存读取数据和从磁盘读取数据的效率差别很大，CPU Cache Miss类似遇到缺页中断。
（3）通过宏定义和内联函数（inline），让编译器提前完成部分工作。无需在程序运行时分配内存，能够实现类似函数的功能，却没有函数调用的压栈、弹栈开销，效率会比较高。
… …
3. AMS平台技术选型的背景
就提升PHP的性能而言，可以选择的是2015年就可直接使用的HHVM或者是2015年底才发布正式版的PHP7。会员AMS是一个访问量级比较大的一个Web系统，经过四年持续的升级和优化，积累了800多个业务功能组件，还有各种PHP编写的公共基础库和脚本，代码规模也比较大。
我们对于PHP版本对代码的向下兼容的需求是比较高的，因此，就我们业务场景而言，PHP7良好的语法向下兼容，正是我们所需要的。因此，我们选择以PHP7为升级的方案。
## 二、PHP7升级面临的风险和挑战
对于一个已经现网在线的大型公共Web服务来说，基础公共软件升级，通常是一件吃力不讨好的工作，做得好，不一定被大家感知到，但是，升级出了问题，则需要承担比较重的责任。为了尽量减少升级的风险，我们必须先弄清楚我们的升级存在挑战和风险。
于是，我们整理了升级挑战和风险列表：
（1）Apache2.0和PHP5.2这两个2008-2009年的基础软件版本比较古老，升级到Apache2.4和PHP7，版本升级跨度比较大，时间跨度相差7-8年，因此，兼容性问题挑战比较高。实际上，我们公司的现网PHP服务，很多都停留在PHP5.2和PHP5.3的版本，版本偏低。
（2）AMS大量使用自研tphplib扩展，tphplib很早在公司内部就没有人维护了，这个扩展之前只有PHP5.3和PHP5.2的编译so版本，并且，部分扩展没有支持线程安全。支持线程安全，是因为我们以前的Apache使用了prefork模式，而我们希望能够使用Apache2.4的Event模式（2014年中，在prefork和worker之后，推出的多进程线程管理模式，对于支持高并发，有更良好的表现）。
（3）语法兼容性问题，从PHP5.2到PHP7的跨度过大，即使PHP官方号称在向下兼容方面做到99%，但是，我们的代码规模比较大，它仍然是一个未知的风险。
（4）新软件面临的风险，将Apache和PHP这种基础软件升级到最新的版本，而这些版本的部分功能可能存在未知的风险和缺陷。
部分同学可能会建议采用Nginx会是更优的选择，的确，单纯比较Nginx和Apache在高并发方面的性能，Nginx的表现更优。但是就PHP的CGI而言，Nginx+php-ftpm和Apache+mod_php两者并没有很大的差距。另一方面，我们因为长期使用Apache，在技术熟悉和经验方面积累更多，因此，它可能不是最佳的选择，但是，具体到我们业务场景，算是比较合适的一个选择。
## 三、版本升级实施过程
1. 高跨度版本升级方式
从一个2008年的Apache2.0直接升级到2016年的Apache2.4，这个跨度过于大，甚至使用的http.conf的配置文件都有很多的不同，这里的需要更新的地方比较多，未知的风险也是存在的。于是，我们的做法，是先尝试将Apache2.0升级到Apach2.2，调整配置、观察稳定性，然后再进一步尝试到Apach2.4。所幸的是，Apache（httpd）是一个比较特别的开源社区，他们之前一直同时维护这两个分支版本的Apache（2.2和2.4），因此，即使是Apache2.2也有比较新的版本。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/86ad7e4b217b9dda7a5be951c765b929.png)
于是，我们先升级了一个PHP5.2+Apache2.2，对兼容性进行了测试和观察，确认两者之间是可以比较平滑升级后，我们开始进行Apache2.4的升级方案。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/35f2bb24ec0e9f6be12c854b0d031ab6.jpg)
PHP5.2的升级，我们也采用相同的思路，我们先将PHP5.2升级至PHP5.6（当时，PHP7还是beta版本），然后再将PHP5.6升级到PHP7，以更平滑的方式，逐步解决不同的问题。
于是，我们的升级计划变为：
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/e1778cfa1347d633757af19716039d59.png)
Apache2.4编译为动态MPM的模式（支持通过httpd配置切换prefork/worker/event模式），根据现网风险等实时降级。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/e876eaf89cd664ced76649e80cfe0932.png)
Prefork、Worker、Event三者粗略介绍：
（1） prefork，多进程模式，1个进程服务于1个用户请求，成本比较高。但是，稳定性最高，不需要支持线程安全。
（2） worker，多进程多线程模式，1个进程含有多个worker线程，1个worker线程服务于1个用户请求，因为线程更轻量，成本比较低。但是，在KeepAlive场景下，worker资源会被client占据，无法响应其他请求（空等待）。
（3） event，多进程多线程模式，1个进程也含有多个worker线程，1个worker线程服务于1个用户请求。但是，它解决了KeepAlive场景下的worker线程被占据问题，它通过专门的线程来管理这些KeepAlive连接，然后再分配“工作”给具体处理的worker，工作worker不会因为KeepAlive而导致空等待。
关于Event模式的官方介绍：
[http://httpd.apache.org/docs/2.4/mod/event.html](http://httpd.apache.org/docs/2.4/mod/event.html)
（部分同学可能会有event模式不支持https的印象，那个说法其实是2年多以前的国内部分技术博客的说法，目前的版本是支持的，详情可以浏览官方介绍）
开启动态切换模式的方法，就是在编译httpd的时候加上：
–enable-mpms-shared=all
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/26a6ed4802bab81a7286bc83a617a789.png)
从PHP5.2升级到PHP5.6相对比较容易，我们主要的工作如下：
（1）清理了部分不再使用的老扩展
（2）解决掉线程安全问题
（3）将cmem等api编译到新的版本
（4）PHP代码语法基于PHP5.6的兼容（实际上变化不大）
（5）部分扩展的同步调整。apc扩展变为zend_opcache和apcu，以前的apc是包含了编译缓存和用户内存操作的功能，在PHP比较新版本里，被分解为独立的两个扩展。
从PHP5.6升级到PHP7.0的工作量就比较多，也相对比较复杂，因此，我们制定了每一个阶段的升级计划：
（1）技术预研，PHP7升级准备。
（2）环境编译和搭建，下载相关的编译包，搭建完整的编译环境和测试环境。（编译环境还是需要比较多的依赖so）
（3）兼容升级和测试。PHP7扩展的重新编译和代码兼容性工作，AMS功能验证，性能压测。
（4）线上灰度。打包为pkg的安装包，编写相关的安装shell安装执行代码（包括软链接、解决一些so依赖）。然后，灰度安装到现网，观察。
（5）正式发布。扩大灰度范围，全量升级。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/39612b580849840d1e3e94f42dc63329.png)
因为从PHP5.2升级到PHP5.6的过程中，很多问题已经被我们提前解决了，所以，PHP7的升级主要难点在于tphplib扩展的编译升级。
涉及主要的工作包括：
（1）PHP5.6的扩展到PHP7.0的比较大幅度改造升级（工作量比较大的地方）
（2）兼容apcu的内存操作函数的改名。PHP5的时候，我们使用的apc前缀的函数不可用了，同步变为apcu前缀的函数（需要apcu扩展）。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/08d6106e5d41cbbfbaea7d7736eea622.png)
（3）语法兼容升级。实际上工作量不算大，从PHP5.6升级到PHP7变化并不多。
我们大概在2016年4月中旬份完成了PHP7和Apache的编译工作， 4月下旬进行现网灰度，5月初全量发布到其中一个现网集群。
2. 升级过程中的错误调试方法
在升级和重新编译PHP7扩展时，如果执行结果不符合预期或者进程core掉，很多错误都是无法从error日志里看见的，不利于分析问题。可以采用以下几种方法，可以用来定位和分析大部分的问题：
（1） var_dump/exit
从PHP代码层逐步输出信息和执行exit，可以逐步定位到异常执行的PHP函数位置，然后再根据PHP函数名，反查扩展内的实现函数，找到问题。这种方法比较简单，但是效率不高。
（2） gdb –p/gdb c
这种方法主要用于分析进程core的场景，我们采用的编译方式，是将mod_php（PHP变成Apache的子或块的方式），使用gdb –p来监控Apache的服务进程。
命令：ps aux|grep httpd
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/a4e09b76b0b5addfe15d4fe6cef4e84e.png)
gdb调试指定进程：
命令：gdb -p
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/a18b9465a784c81e6556365513632f88.png)
使用c进行捕获，然后构造能够导致core的web请求：
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/0afa5707e15b3d199ae4dddd630acddc.png)
Apache通常是多进程模式，为了让问题比较容易复现，可以在http.con里修改参数，将启动进程数修改为1个（下图中的多个参数都需要调整，以达到只启动单进程单线程的目的）。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/f24e134be731499cea17176cd2ca93b7.png)
当然还有一种更简单的方法，因为Apache本身就支持单进程调试模式的。
./apachectl -k start -X -e debug
然后再通过gdb –p来调试就更简单一些。
（3）通过strace命令查看Apache进程具体在做了些什么事情，根据里面的执行内容，分析和定位问题。
strace -Ttt -v -s1024 -f -p pid（进程id）
备注：执行这些命令，注意权限问题，很可能需要root权限。
## 四、PHP5.6到PHP7.0扩展升级实践记录
1. 数据类型的变化
（1）zval
php7的诞生始于zval结构的变化，PHP7不再需要指针的指针，绝大部分zval**需要修改成zval*。如果PHP7直接操作zval，那么zval*也需要改成zval，Z_*P()也要改成Z_*()，ZVAL_*(var, …)需要改成ZVAL_*(&var, …)，一定要谨慎使用&符号，因为PHP7几乎不要求使用zval*，那么很多地方的&也是要去掉的。
ALLOC_ZVAL，ALLOC_INIT_ZVAL，MAKE_STD_ZVAL这几个分配内存的宏已经被移除了。大多数情况下，zval*应该修改为zval，而INIT_PZVAL宏也被移除了。

PHP
```
/* 7.0zval结构源码 */
/* value字段，仅占一个size_t长度，只有指针或double或者long */
typedef union _zend_value {
zend_long         lval;                /* long value */
double            dval;                /* double value */
zend_refcounted  *counted;
zend_string      *str;
zend_array       *arr;
zend_object      *obj;
zend_resource    *res;
zend_reference   *ref;
zend_ast_ref     *ast;
zval             *zv;
void             *ptr;
zend_class_entry *ce;
zend_function    *func;
struct {
uint32_t w1;
uint32_t w2;
} ww;
} zend_value;
struct _zval_struct {
zend_value        value;            /* value */
union {
。。。
} u1;/* 扩充字段，主要是类型信息 */
union {
… …
} u2;/* 扩充字段，保存辅助信息 */
};
```
（2）整型
直接切换即可：

PHP
```
long->zend_long
/* 定义 */
typedef int64_t zend_long;
/* else */
typedef int32_t zend_long;
```
（3）字符串类型
PHP5.6版本中使用 char* + len的方式表示字符串，PHP7.0中做了封装，定义了zend_string类型：

PHP
```
struct _zend_string {
zend_refcounted_h gc;
zend_ulong        h;                /* hash value */
size_t            len;
char              val[1];
};
```
zend_string和char*的转换：

PHP
```
zend_string *str;
char *cstr = NULL;
size_t slen = 0;
//...
/* 从zend_string获取char* 和 len的方法如下 */
cstr = ZSTR_VAL(str);
slen = ZSTR_LEN(str);
/* char* 构造zend_string的方法 */
zend_string * zstr = zend_string_init("test",sizeof("test"), 0);
```
扩展方法，解析参数时，使用字符串的地方，将‘s’替换成‘S’：

PHP
```
/* 例如 */
zend_string *zstr;
if (zend_parse_parameters(ZEND_NUM_ARGS() , "S", &zstr) == FAILURE)
{
RETURN_LONG(-1);
}
```
（4）自定义对象
源代码：

PHP
```
/* php7.0 zend_object 定义 */
struct _zend_object {
zend_refcounted_h gc;
uint32_t          handle;
zend_class_entry  *ce;
const zend_object_handlers  *handlers;
HashTable        *properties;
zval              properties_table[1];
};
```
zend_object是一个可变长度的结构。因此在自定义对象的结构中，zend_object需要放在最后一项：

PHP
```
/* 例子 */
struct clogger_object {
CLogger *logger;
zend_object  std;// 放在后面
};
/* 使用偏移量的方式获取对象 */
static inline clogger_object *php_clogger_object_from_obj(zend_object *obj) {
return (clogger_object*)((char*)(obj) - XtOffsetOf(clogger_object, std));
}
#define Z_USEROBJ_P(zv) php_clogger_object_from_obj(Z_OBJ_P((zv)))
/* 释放资源时 */
void tphp_clogger_free_storage(zend_object *object TSRMLS_DC)
{
clogger_object *intern = php_clogger_object_from_obj(object);
if (intern->logger)
{
delete intern->logger;
intern->logger = NULL;
}
zend_object_std_dtor(&intern->std);
}
```
（5）数组
7.0中的hash表定义如下，给出了一些注释：

PHP
```
/* 7.0中的hash表结构 */
typedef struct _Bucket { /* hash表中的一个条目 */
zval              val;   /* 删除元素zval类型标记为IS_UNDEF */
zend_ulong        h;                /* hash value (or numeric index)   */
zend_string      *key;              /* string key or NULL for numerics */
} Bucket;
typedef struct _zend_array HashTable;
struct _zend_array {
zend_refcounted_h gc;
union {
struct {
ZEND_ENDIAN_LOHI_4(
zend_uchar    flags,
zend_uchar    nApplyCount,
zend_uchar    nIteratorsCount,
zend_uchar    reserve)
} v;
uint32_t flags;
} u;
uint32_t          nTableMask;
Bucket           *arData; /* 保存所有数组元素 */
uint32_t          nNumUsed; /* 当前用到了多少长度， */
uint32_t          nNumOfElements; /* 数组中实际保存的元素的个数,一旦nNumUsed的值到达nTableSize，PHP就会尝试调整arData数组，让它更紧凑，具体方式就是抛弃类型为UDENF的条目 */
uint32_t          nTableSize; /* 数组被分配的内存大小为2的幂次方（最小值为8） */
uint32_t          nInternalPointer;
zend_long         nNextFreeElement;
dtor_func_t       pDestructor;
};
```
其中，PHP7在zend_hash.h中定义了一系列宏，用来操作数组，包括遍历key、遍历value、遍历key-value等，下面是一个简单例子：

PHP
```
/* 数组举例 */
zval *arr;
zend_parse_parameters(ZEND_NUM_ARGS() , "a", &arr_qos_req);
if (arr)
{
zval *item;
zend_string *key;
ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(arr), key, item) {
/* ... */
}
}
/* 获取到item后，可以通过下面的api获取long、double、string值 */
zval_get_long(item)
zval_get_double(item)
zval_get_string(item)
```
PHP5.6版本中是通过zend_hash_find查找key，然后将结果给到zval **变量，并且查询不到时需要自己分配内存，初始化一个item，设置默认值。
2. PHP7中的api变化
（1）duplicate参数
PHP5.6中很多API中都需要填入一个duplicate参数，表明一个变量是否需要复制一份，尤其是string类的操作，PHP7.0中取消duplicate参数，对于string相关操作，只要有duplicate参数，直接删掉即可。因为PHP7.0中定义了zval_string结构，对字符串的操作，不再需要duplicate值，底层直接使用zend_string_init初始化一个zend_string即可，而在PHP5.6中string是存放在zval中的，而zval的内存需要手动分配。
涉及的API汇总如下：
> 
add_index_string、add_index_stringl、add_assoc_string_ex、add_assoc_stringl_ex、add_assoc_string、add_assoc_stringl、add_next_index_string、add_next_index_stringl、add_get_assoc_string_ex、add_get_assoc_stringl_ex、add_get_assoc_string、add_get_assoc_stringl、add_get_index_string、add_get_index_stringl、add_property_string_ex、add_property_stringl_ex、add_property_string、add_property_stringl、ZVAL_STRING、ZVAL_STRINGL、RETVAL_STRING、RETVAL_STRINGL、RETURN_STRING、RETURN_STRINGL
（2）MAKE_STD_ZVAL
PHP5.6中，zval变量是在堆上分配的，创建一个zval变量需要先声明一个指针，然后使用MAKE_STD_ZVAL进行分配空间。PHP7.0中，这个宏已经取消，变量在栈上分配，直接定义一个变量即可，不再需要MAKE_STD_ZVAL，使用到的地方，直接去掉就好。
（3）ZEND_RSRC_DTOR_FUNC
修改参数名rsrc为res

PHP
```
/* PHP5.6 */
typedef struct _zend_rsrc_list_entry {
void *ptr;
int type;
int refcount;
} zend_rsrc_list_entry;
typedef void (*rsrc_dtor_func_t)(zend_rsrc_list_entry *rsrc TSRMLS_DC);
#define ZEND_RSRC_DTOR_FUNC(name)        void name(zend_rsrc_list_entry *rsrc TSRMLS_DC)
/* PHP7.0 */
struct _zend_resource {
zend_refcounted_h gc;/*7.0中对引用计数做了结构封装*/
int               handle;
int               type;
void             *ptr;
};
typedef void (*rsrc_dtor_func_t)(zend_resource *res);
#define ZEND_RSRC_DTOR_FUNC(name) void name(zend_resource *res)
```
PHP7.0中，将zend_rsrc_list_entry结构升级为zend_resource，在新版本中只需要修改一下参数名称即可。
（4）二级指针宏，即Z_*_PP
PHP7.0中取消了所有的PP宏，大部分情况直接使用对应的P宏即可。
（5）zend_object_store_get_object被取消
根据官方wiki，可以定义如下宏，用来获取object，实际情况看，这个宏用的还是比较频繁的：

PHP
```
static inline user_object *user_fetch_object(zend_object *obj) {
return (user_object *)((char*)(obj) - XtOffsetOf(user_object, std));
}
/* }}} */
#define Z_USEROBJ_P(zv) user_fetch_object(Z_OBJ_P((zv)))
```
（6）zend_hash_exists、zend_hash_find
对所有需要字符串参数的函数，PHP5.6中的方式是传递两个参数（char* + len），而PHP7.0中定义了zend_string，因此只需要一个zend_string变量即可。
返回值变成了zend_bool类型：

PHP
```
/* 例子 */
zend_string * key;
key = zend_string_init("key",sizeof("key"), 0);
zend_bool res_key = zend_hash_exists(itmeArr, key);
```
参考资料：
1、php5 to phpng，[http://yaoguais.com/?s=md/php/php7-vm.md](http://yaoguais.com/?s=md/php/php7-vm.md)
2、PHP扩展开发及内核应用， [http://www.walu.cc/phpbook/10.1.md](http://www.walu.cc/phpbook/10.1.md)
3、PHP 7中新的Hashtable实现和性能改进 ，[http://gywbd.github.io/posts/2014/12/php7-new-hashtable-implementation.html](http://gywbd.github.io/posts/2014/12/php7-new-hashtable-implementation.html)
4、深入理解PHP7之zval， [https://github.com/laruence/php7-internal/blob/master/zval.md](https://github.com/laruence/php7-internal/blob/master/zval.md)
5、官方wiki， [https://wiki.php.net/phpng-upgrading](https://wiki.php.net/phpng-upgrading)
6、php手册 ，[http://php.net/manual/zh/index.php](http://php.net/manual/zh/index.php)
7、PHP7 使用资源包裹第三方扩展的实现及其源码解读 ，[https://mengkang.net/684.html](https://mengkang.net/684.html)
## 五、AMS平台升级PHP7的性能优化成果
现网服务是一个非常重要而又敏感的环境，轻则影响用户体验，重则产生现网事故。因此，我们4月下旬完成PHP7编译和测试工作之后，就在AMS其中一台机器进行了灰度上线，观察了几天后，然后逐步扩大灰度范围，在5月初完成升级。
这个是我们压测AMS一个查询多个活动计数器的压测结果，以及现网CGI机器，在高峰相同TGW流量场景下的CPU负载数据：
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/e1bbed38e109463b2870382c312fdfd8.png)
就我们的业务压测和现网结果来看，和官方所说的性能提升一倍，基本一致。
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/29ce46f17204f9d7293dba24c528c64d.png)
![日请求亿级的QQ会员AMS平台PHP7升级实践 - 徐汉彬Hansion - 技术行者](http://jbcdn2.b0.upaiyun.com/2016/07/64a05f6433be376db08c5aefb4a640fc.png)
AMS平台拥有不少的CGI机器，PHP7的升级和应用给我们带来了性能的提升，可以有效节省硬件资源成本。并且，通过Apache2.4的Event模式，我们也增强了Apache在支持并发方面的能力。
## 六、小结
我们PHP7升级研发项目组，在过去比较长的一个时间段里，经过持续地努力和推进，终于在2016年4月下旬现网灰度，5月初在集群中全量升级，为我们的AMS活动运营平台带来性能上大幅度的提升。
PHP7的革新，对于PHP语言本身而言，具有非凡的意义和价值，这让我更加确信一点，PHP会是一个越来越好的语言。同时，感谢PHP社区的开发者们，为我们业务带来的性能提升。
