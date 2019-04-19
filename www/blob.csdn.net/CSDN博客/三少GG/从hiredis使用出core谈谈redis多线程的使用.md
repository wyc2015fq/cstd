# 从hiredis使用出core谈谈redis多线程的使用 - 三少GG - CSDN博客
2014年05月27日 20:31:37[三少GG](https://me.csdn.net/scut1135)阅读数：1199
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)
# 从hiredis使用出core谈谈redis多线程的使用
发表于3个月前(2014-02-25 15:29)   阅读（607） | 评论（[3](http://my.oschina.net/jungleliu0923/blog/202948#comments)） **11**人收藏此文章, 我要收藏
赞0
摘要hireedis多线程出core原因
目录[-]
- [1、情景描述](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h1_1)
- [1.1 使用场景](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h2_2)
- [1.2 初步实现方案](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h2_3)
- [1.3 结果](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h2_4)
- [2、线下复现](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h1_5)
- [2.1 代码](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h2_6)
- [2.2 编译执行](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h2_7)
- [2.3 原因分析](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h2_8)
- [3. 终极解决方案](http://my.oschina.net/jungleliu0923/blog/202948#OSC_h1_9)
在实际工作中，我需要使用redis的客户端去连接redis，于是选择了hiredis客户端(公司强推)。  hiRedis 是 Redis 官方指定的 C 语言客户端开发包，支持 Redis 完整的命令集、管线以及事件驱动编程。
# 1、情景描述
## 1.1 使用场景
      一个epool模型的服务器不断接受外界请求，这个服务器框架给用户预留一个回调函数（多线程），回调函数为用户自己去实现的业务逻辑，其中redis的使用就需要在这个回调函数内部实现。
## 1.2 初步实现方案
在程序启动的时候，我就初始化redis的连接，获得hiredis句柄。然后把hiredis句柄传入到线程函数里面。让其做相应的业务逻辑。
## 1.3 结果
很不幸，一次请求都没问题，做压力测试，同时开20个线程访问，程序立即出core。
线上出core如下：
|`01`|```(gdb) bt```|
|`02`|```#0  0x000000302af2e2ed in raise () from /lib64/tls/libc.so.6```|
|`03`|```#1  0x000000302af2fa3e in abort () from /lib64/tls/libc.so.6```|
|`04`|```#2  0x000000302af62db1 in __libc_message () from /lib64/tls/libc.so.6```|
|`05`|```#3  0x000000302af6888e in _int_free () from /lib64/tls/libc.so.6```|
|`06`|```#4  0x000000302af6a12d in _int_realloc () from /lib64/tls/libc.so.6```|
|`07`|```#5  0x000000302af6b39c in realloc () from /lib64/tls/libc.so.6```|
|`08`|```#6  0x0000000000dc2269 in sdscatlen (s=Variable "s" is not available.```|
|`09`|```) at sds.c:97```|
|`10`|```#7  0x0000000000dc1d40 in __redisAppendCommand (c=0x16fa1d0, cmd=Variable "cmd" is not available.```|
|`11`|```) at hiredis.c:1186```|
|`12`|```#8  0x0000000000dc1d97 in redisvAppendCommand (c=0x16fa1d0, format=Variable "format" is not available.```|
|`13`|```) at hiredis.c:1206```|
|`14`|```#9  0x0000000000dc1eed in redisvCommand (c=0x16fa1d0, format=Variable "format" is not available.```|
|`15`|```) at hiredis.c:1267```|
|`16`|```#10 0x0000000000dc1fb6 in redisCommand (c=Variable "c" is not available.```|
|`17`|```) at hiredis.c:1276```|
|`18`|```#11 0x0000002b1a8e6310 in Default_Handler::get_batch_redis (this=0x1ff41f0, redis_ins=0x175a7d0, dataid=6202, buf_num=12, res_num=6, key_sign=0x2bd67cb3c8,```|
|`19`|`    ````res_lens=0x2bd5f54208, res_buf=0x2bd5f54398````""````) at default_handler.cpp:659```|
|`20`|```#12 0x0000002b1a9134df in Default_Ms_Handler::get_digest (this=0x1ff41f0) at default_ms_handler.cpp:646```|
|`21`|```#13 0x000000000092910c in do_proc () at gss_work.cpp:1107```|
|`22`|```#14 0x000000000091c91f in thread_main () at gss_net.cpp:188```|
|`23`|```#15 0x0000000000bc10e9 in default_native () at ubserver_app.cpp:283```|
|`24`|```#16 0x0000000000bbc676 in eppool_consume (pool=0x2230b90, data=0x22188f0) at eppool.cpp:649```|
|`25`|```#17 0x0000000000bbc4d1 in _eppool_workers (param=0x22188f0) at eppool.cpp:604```|
|`26`|```#18 0x000000302b80610a in start_thread () from /lib64/tls/libpthread.so.0```|
|`27`|```#19 0x000000302afc6003 in clone () from /lib64/tls/libc.so.6```|
|`28`|```#20 0x0000000000000000 in ?? ()```|
当时经过多次尝试。把连接放入到了每个线程中。那么就不会出core了。
# 2、线下复现
因为不方便公开公司代码，所以我写一个类似的代码来复现这个case。
## 2.1 代码
代码主要有testredis.cpp和Makefile（自己指定hiredis目录）。用法是 ./redis
 -n [num] -h [host] -p [port]， n为host数目，多个host用"-"进行分割。
testredis.cpp
|`001`|`/***************************************************************************`|
|`002`|` ``*`|
|`003`|` ````* Copyright (c) 2014 Baidu.com, Inc. All Rights Reserved```|
|`004`|` ``*`|
|`005`|` ``**************************************************************************/`|
|`006`|` `|
|`007`|` `|
|`008`|` `|
|`009`|`/**`|
|`010`|` ````* @file redistest.cpp```|
|`011`|` ````* @author liujun05(com@baidu.com)```|
|`012`|` ````* @date 2014/02/25 10:28:44```|
|`013`|` ````* @brief```|
|`014`|` ``* `|
|`015`|` ``**/`|
|`016`||
|`017`|`#include<unistd.h>`|
|`018`|```#include <stdio.h>```|
|`019`|```#include <hiredis.h>```|
|`020`|```#include <stdlib.h>```|
|`021`|```#include <string.h>```|
|`022`|```#include <pthread.h>```|
|`023`||
|`024`||
|`025`|```#ifndef uint32```|
|`026`|```#define uint32 unsigned int```|
|`027`|`#endif`|
|`028`||
|`029`|```#define MAX_REDIS_SERVER_CNT 10```|
|`030`|```#define MAX_REDIS_IPS 1024```|
|`031`||
|`032`|`typedef``struct``_redis_conf_t`|
|`033`|`{`|
|`034`|`    ````uint32 redis_num;```|
|`035`|`    ``char``redis_ips[MAX_REDIS_IPS];`|
|`036`|`    ``char``redis_ip_array[MAX_REDIS_SERVER_CNT][MAX_REDIS_IPS];`|
|`037`|`    ````uint32 redis_port;```|
|`038`||
|`039`|```} redis_conf;```|
|`040`||
|`041`|`typedef``struct``_redis_data_t`|
|`042`|`{`|
|`043`|`    ````uint32 redis_num;```|
|`044`|`    ````redisContext *rc[MAX_REDIS_SERVER_CNT];```|
|`045`|`}redis_data;`|
|`046`||
|`047`|```redis_conf g_cfg;```|
|`048`|```redis_data g_data;```|
|`049`||
|`050`|`void``show_usage()`|
|`051`|`{`|
|`052`|`    ``printf``(````"usage: ./redis -n [num] -h [host] -p [port]\n"````);`|
|`053`|`}`|
|`054`||
|`055`|```/** 解析参数 */```|
|`056`|`int``main_parse_option(``int``argc, ``char``**argv)`|
|`057`|`{`|
|`058`|`    ``int``c;`|
|`059`|`    ````//reset 获取参数的位置，多次调用时这个会出现问题```|
|`060`|`    ``while````((c = getopt(argc, argv,````"h:p:n:"````)) != -1)```|
|`061`|`    ``{`|
|`062`|`        ``switch``(c)`|
|`063`|`        ``{`|
|`064`|`        ``case``'h'``:`|
|`065`|`            ``sprintf````(g_cfg.redis_ips, optarg);```|
|`066`|`            ``break``;`|
|`067`|`        ``case``'p'``:`|
|`068`|`            ````g_cfg.redis_port =````atoi``(optarg);`|
|`069`|`            ``break``;`|
|`070`|`        ``case``'n'``:`|
|`071`|`            ````g_cfg.redis_num =````atoi``(optarg);`|
|`072`|`            ``break``;`|
|`073`|`        ``default``:`|
|`074`|`            ``show_usage();`|
|`075`|`            ``fflush``(stdout);`|
|`076`|`            ``return``-1;`|
|`077`|`        ``}`|
|`078`|`    ``}`|
|`079`|`    ``return``0;`|
|`080`|`}`|
|`081`||
|`082`|`void````* test_thread1(````void````* data)```|
|`083`|`{`|
|`084`|`    ````redis_data* redis_ins = (redis_data*)data;```|
|`085`|`    ````redisReply *reply;```|
|`086`|`    ``for``(``int````i=0; i<redis_ins->redis_num; i++)```|
|`087`|`    ``{`|
|`088`|`        ````reply = (redisReply *)redisCommand( redis_ins->rc[i] ,``````"SET %s %s"````, ``"foo"``, ````"hello world"````);`|
|`089`|`        ``freeReplyObject(reply);`|
|`090`|`    ``}`|
|`091`|`}`|
|`092`||
|`093`|`int``init_data()`|
|`094`|`{`|
|`095`|`    ````g_data.redis_num = 0;```|
|`096`|`    ``struct````timeval timeout = { 1, 500000 };``````// 1.5 seconds```|
|`097`||
|`098`|`    ``char````*ptok = NULL;```|
|`099`|`    ``char````*part = strtok_r(g_cfg.redis_ips,````"-"````, &ptok);```|
|`100`|`    ``int````num = 0;```|
|`101`|`    ``while``(part)`|
|`102`|`    ``{`|
|`103`|`        ``strcpy````(g_cfg.redis_ip_array[num++], part);```|
|`104`|`        ````part = strtok_r(NULL,````"-"````, &ptok);```|
|`105`|`    ``}`|
|`106`||
|`107`|`    ``if````(num != g_cfg.redis_num || num > MAX_REDIS_SERVER_CNT)```|
|`108`|`    ``{`|
|`109`|`        ``printf``(````"ip num[%d] not equal redis_num[%d] or not vaild\n"``````, num, g_cfg.redis_num);```|
|`110`|`    ``}`|
|`111`||
|`112`|`    ````g_data.redis_num = (num > MAX_REDIS_SERVER_CNT ) ? MAX_REDIS_SERVER_CNT : num;```|
|`113`|`    ``int````i= 0;```|
|`114`||
|`115`|`    ``for````(i=0; i<g_data.redis_num; i++)```|
|`116`|`    ``{`|
|`117`|`        ````g_data.rc[i] = redisConnectWithTimeout( g_cfg.redis_ip_array[i], g_cfg.redis_port , timeout);```|
|`118`|`        ``if````( g_data.rc[i] == NULL || g_data.rc[i]->err)```|
|`119`|`        ``{`|
|`120`|`            ``printf``(````"content to redis server[%s:%u], error[%s]\n"````,`|
|`121`|`                    ````g_cfg.redis_ip_array[i], g_cfg.redis_port, g_data.rc[i]->errstr```|
|`122`|`            ``);`|
|`123`|`            ``goto``exit``;`|
|`124`|`        ``}`|
|`125`|`    ``}`|
|`126`|`    ``return``0;`|
|`127`||
|`128`|`exit``:`|
|`129`|`   ``for``(``int````j=0; j<i; j++)```|
|`130`|`   ``{`|
|`131`|`        ``if````(g_data.rc[j] != NULL)```|
|`132`|`        ``{`|
|`133`|`            ``redisFree(g_data.rc[j]);`|
|`134`|`        ``}`|
|`135`|`   ``}`|
|`136`|`   ``return``-1;`|
|`137`|`}`|
|`138`||
|`139`||
|`140`|`int``destory_data()`|
|`141`|`{`|
|`142`|`   ``for``(``int````j=0; j<g_data.redis_num; j++)```|
|`143`|`   ``{`|
|`144`|`        ``if````(g_data.rc[j] != NULL)```|
|`145`|`        ``{`|
|`146`|`            ``redisFree(g_data.rc[j]);`|
|`147`|`        ``}`|
|`148`|`   ``}`|
|`149`|`}`|
|`150`||
|`151`|`int``main(``int``argc, ``char````** argv)```|
|`152`|`{`|
|`153`|`    ````g_cfg.redis_ips[0] =````'\0'``;`|
|`154`|`    ````g_cfg.redis_port = 6379;```|
|`155`|`    ````g_cfg.redis_num = 0;```|
|`156`|`    ``if````( 0 != main_parse_option(argc, argv) )```|
|`157`|`    ``{`|
|`158`|`        ``show_usage();`|
|`159`|`        ``return``-1;`|
|`160`|`    ``}`|
|`161`||
|`162`|`    ``if````(  0 == g_cfg.redis_num || g_cfg.redis_num >  MAX_REDIS_SERVER_CNT )```|
|`163`|`    ``{`|
|`164`|`        ``printf``(````"the reids num[%u] is not vaild\n"``````, g_cfg.redis_num);```|
|`165`|`        ``show_usage();`|
|`166`|`        ``return``0;`|
|`167`|`    ``}`|
|`168`||
|`169`|`    ``int````ret = init_data();```|
|`170`|`    ``if````( ret != 0)```|
|`171`|`    ``{`|
|`172`|`        ``printf``(````"init num fail\n"````);`|
|`173`|`        ``return``-1;`|
|`174`|`    ``}`|
|`175`||
|`176`||
|`177`|`    ````pthread_t t[100];```|
|`178`|`    ``for``(``int````i=0; i<100; i++)```|
|`179`|`    ``{`|
|`180`|`        ````pthread_create(&t[i], NULL, test_thread1, &g_data);```|
|`181`|`    ``}`|
|`182`||
|`183`|`    ``for``(``int````i=0; i<100; i++)```|
|`184`|`    ``{`|
|`185`|`        ````pthread_join(t[i], NULL);```|
|`186`|`    ``}`|
|`187`||
|`188`|`    ``destory_data();`|
|`189`|`    ``return``0;`|
|`190`|`}`|
|`191`||
|`192`||
|`193`||
|`194`|```/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */```|
Makefile
|`1`|```redis: testredis.cpp```|
|`2`|`    ````g++ -g testredis.cpp -I./hiredis -L./hiredis -lhiredis -lpthread -o redis```|
|`3`||
|`4`|`clean:`|
|`5`|`    ``rm``redis`|
## 2.2 编译执行
|`1`|`liujun05@cq01-rdqa-dev012.cq01:~/``test````/hiredis$ ./redis -n2 -h10.48.46.26-10.46.175.102```|
|`2`|```*** glibc detected *** double````free````or corruption (!prev): 0x000000000050aa80 ***```|
|`3`|```Aborted (core dumped)```|
可以看到出core了
|`01`|```(gdb) bt```|
|`02`|```#0  0x000000302af2e2ed in raise () from /lib64/tls/libc.so.6```|
|`03`|```#1  0x000000302af2fa3e in abort () from /lib64/tls/libc.so.6```|
|`04`|```#2  0x000000302af62db1 in __libc_message () from /lib64/tls/libc.so.6```|
|`05`|```#3  0x000000302af6888e in _int_free () from /lib64/tls/libc.so.6```|
|`06`|```#4  0x000000302af68bd6 in free () from /lib64/tls/libc.so.6```|
|`07`|```#5  0x0000000000403c75 in redisBufferWrite (c=0x50a010, done=0x571c008c) at hiredis.c:1162```|
|`08`|```#6  0x0000000000403d3e in redisGetReply (c=0x50a010, reply=0x571c00b8) at hiredis.c:1195```|
|`09`|```#7  0x0000000000403f62 in redisvCommand (c=0x50a010, format=Variable "format" is not available.```|
|`10`|```) at hiredis.c:1296```|
|`11`|```#8  0x0000000000404006 in redisCommand (c=Variable "c" is not available.```|
|`12`|```) at hiredis.c:1313```|
|`13`|```#9  0x00000000004013e7 in test_thread1 (data=0x509ba0) at testredis.cpp:88```|
|`14`|```#10 0x000000302b80610a in start_thread () from /lib64/tls/libpthread.so.0```|
|`15`|```#11 0x000000302afc6003 in clone () from /lib64/tls/libc.so.6```|
|`16`|```#12 0x0000000000000000 in ?? ()```|
虽然出core位置不一致，但是经过查看代码，出core的原因应该是一致的。
## 2.3 原因分析
从堆栈5可以看到 hiredis.c的1162行出的core，打开hiredis.c
|`1`|```1160         }````else``if````(nwritten > 0) {```|
|`2`|`1161             ``if````(nwritten == (````signed````)sdslen(c->obuf)) {```|
|`3`|```1162                 sdsfree(c->obuf);```|
|`4`|```1163                 c->obuf = sdsempty();```|
|`5`|```1164             }````else``{`|
|`6`|```1165                 c->obuf = sdsrange(c->obuf,nwritten,-1);```|
|`7`|```1166             }```|
可以看到的确在1152行对c->obuf进行了一次free导致出core。
我们分析下调用关系，首先调用redisCommand.
|`1`|`1309 ``void````*redisCommand(redisContext *c,````const``char````*format, ...) {```|
|`2`|`1310     ``va_list``ap;`|
|`3`|`1311     ``void````*reply = NULL;```|
|`4`|`1312     ``va_start``(ap,format);`|
|`5`|```1313     reply = redisvCommand(c,format,ap);```|
|`6`|`1314     ``va_end``(ap);`|
|`7`|`1315     ``return``reply;`|
|`8`|```1316 }```|
然后调用redisvCommand
|`1`|`1303 ``void````*redisvCommand(redisContext *c,````const``char``*format, ``va_list````ap) {```|
|`2`|`1304     ``if````(redisvAppendCommand(c,format,ap) != REDIS_OK)```|
|`3`|`1305         ``return``NULL;`|
|`4`|`1306     ``return``__redisBlockForReply(c);`|
|`5`|```1307 }```|
接着调用redisvAppendCommand
|`01`|`<span></span>1233 ``int````redisvAppendCommand(redisContext *c,````const``char``*format, ``va_list````ap) {```|
|`02`|`1234     ``char``*cmd;`|
|`03`|`1235     ``int``len;`|
|`04`|`1236`|
|`05`|```1237     len = redisvFormatCommand(&cmd,format,ap);```|
|`06`|`1238     ``if````(len == -1) {```|
|`07`|```1239         __redisSetError(c,REDIS_ERR_OOM,``````"Out of memory"````);`|
|`08`|`1240         ``return``REDIS_ERR;`|
|`09`|```1241     }```|
|`10`|`1242`|
|`11`|`1243     ``if````(__redisAppendCommand(c,cmd,len) != REDIS_OK) {```|
|`12`|`1244         ``free``(cmd);`|
|`13`|`1245         ``return``REDIS_ERR;`|
|`14`|```1246     }```|
|`15`|`1247`|
|`16`|`1248     ``free``(cmd);`|
|`17`|`1249     ``return``REDIS_OK;`|
|`18`|```1250 }```|
这里，我们需要care调用__redisAppendCommand.
|`01`|`1220 ``int````__redisAppendCommand(redisContext *c,````char``*cmd, ``size_t````len) {```|
|`02`|```1221     sds newbuf;```|
|`03`|`1222`|
|`04`|```1223     newbuf = sdscatlen(c->obuf,cmd,len);```|
|`05`|`1224     ``if````(newbuf == NULL) {```|
|`06`|```1225         __redisSetError(c,REDIS_ERR_OOM,``````"Out of memory"````);`|
|`07`|`1226         ``return``REDIS_ERR;`|
|`08`|```1227     }```|
|`09`|`1228`|
|`10`|```1229     c->obuf = newbuf;```|
|`11`|`1230     ``return``REDIS_OK;`|
|`12`|```1231 }```|
问题出现了。
对于任意一个多线程，他传入的redisContext* c都是一个，那么他们也公用同一个c->obuf，这里很明显，线程数据是耦合的。
当一个线程调用sdsfree c->obuf，其他任意一个线程使用c->obuf都会导致出core. 这也是我所谓的hiredis对多线程支持的不好的地方。
# 3. 终极解决方案
那么，如果我一定要在多线程中通过hiredis客户端调用redis呢。有没有方案了，答案肯定是有，只不过性能稍差。
原先的做法是先获得hiredis连接句柄，然后把句柄传入到多线程中，让多线程使用。现在改成在线程里面连接获得hiredis句柄，然后再进行使用。当然，代价是对于每个请求，都需要去连接redis服务器，加大了网络开销的同时还加大了redis的请求。
redis是单线程异步模型，hiredis这个客户端看来也只支持单线程。希望后续有redis的相关程序猿来改进相应问题，在hiredis使用多线程需要慎重。
