# xmlInitParser和xmlCleanupParser使用详解 - nosmatch的专栏 - CSDN博客
2012年05月09日 09:34:53[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：714标签：[initialization																[多线程																[library																[application																[api																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=initialization&t=blog)
使用libxml2库的朋友,可能会对它提供的初始化接口(xmlInitParser )和清除资源接口(xmlCleanupParser )感到困惑.因为在它主页中提供的例子里面,各处使用的情况差别很大. 我花了些时间把这两个接口使用方法整理如下:
1. 在单线程(single-threaded)环境中
   xmlInitParser 可以被调用一次,或者被调用多次(多于一次),甚至可以不被调用.
   对于第一种情况,很容易理解,因为接口作用是初始化,而且应该在程序的入口处调用. 这也是推荐的使用方法.
   对于第二种情况,看其接口的实现代码就很容易理解,第二次以后的调用只是判断完标志位后简单地返回.
   --------------------------------------------------------------------------------
   void xmlInitParser(void) {
    if (xmlParserInitialized != 0)
        return;
   //do initialization for xml library
   …
    xmlParserInitialized = 1;
   }
   --------------------------------------------------------------------------------
   对于第三种情况,在作者在mailing list的回答中可找到答案,而且这一点我也已经简单地通过例子验证过.而且在libmxl2自带的很多sample中,都是属于这样的情况.
   --------------------------------------------------------------------------------
[http://mail.gnome.org/archives/xml/2003-May/msg00027.html](http://mail.gnome.org/archives/xml/2003-May/msg00027.html)
   Q:  1. If I *don't* use libxml2's thread-support do I have to call xmlInitParser() only once per application or once per parsing?
   A:  You don't even need to call it. It's done automatically, it's just better to do it explicitly in a thread environment.
   --------------------------------------------------------------------------------
   类似于xmlInitParser(), xmlCleanupParser()也可以被调用一次,或者被调用多次(多于一次),甚至可以不被调用.
    对于第一种情况,很容易理解,因为接口作用是清除资源的, 而且应该在程序的出口处调用. 这也是推荐的使用方法.
   对于第二种情况,看其接口的实现代码就很容易理解,第二次以后的调用只是判断完标志位后简单地返回.
   --------------------------------------------------------------------------------
  void xmlCleanupParser(void) {
    if (!xmlParserInitialized)
        return;
*// do cleanup for xml library*
   …
    xmlParserInitialized = 0;
   }
   --------------------------------------------------------------------------------
   对于第三种情况,这样的使用方法不会对程序造成任何的破坏,但是在xmlInitParser()中分配的部分内存将一直被占用,直至整个程序退出.所以这是一种不会产生影响但也不推荐的使用方法.
2. 在多线程(multi-threaded)环境中
   在多线程环境下,使用它们要比单线程环境下面需要注意更多的问题.
   使用xmlInitParser()必须遵循一下两个原则:
   (1) xmlInitParser()不能在线程中被调用,因为xmlInitParser()不是原子操作,可能会引起线程竞争,导致程序意外.
   (2) xmlInitParser()应该在主线程中被调用,在开始任何线程之前,在程序的入口处.
   从原则上如果整个程序中不调用xmlInitParser()在某些情况是可以的,因为上一节提到,调用任何其他libxml2 API时会检验是否已经初始化,如果没有,将自动进行初始化. 但是如果在整个开始线程之前的进程中都没有调用到libxml2 的任何API,而是在线程开始调用libxml2的API,就会出现初始化时线程竞争的糟糕事情.所以这是非常不推荐的做法.
   当然,在进程(线程开始前)多次调用xmlInitParser()不会产生问题,因为第二次以后的调用只是简单地检查标志位接下来返回.
   使用xmlCleanupParser()必须遵循一下两个原则:
   (1) xmlCleanupParser()不能在线程中被调用,因为先结束的进程会把共享内存清除,接下来尚未结束的的线程就无法正确访问.
   (2) xmlCleanupParser()应该在主线程中被调用,在不再使用libxml2库时,一般在程序的出口处.
   这里需要注意一个问题,如果你无法确定其他用户是否还在使用libxml2库,那么就不要调用xmlCleanupParser(),因为这样最差的情况是浪费了一块内存,直至在程序结束时才能被收回,比起程序崩溃,这样的代价还是值得的.在mailing list中,作者也提到这样的方案.
   同样,在进程(所有进程结束之后)多次调用xmlCleanupParser()不会对程序产生任何影响,第二次以后的调用仅是检查标志位和简单第返回.
   在多线程环境下,推荐的使用方法是:
   ------------------------------------------------------------------------------
  int main ( int argc, char **argv )
  {
*//do library initialization at the beginning of the program*
xmlInitParser();
*   //do other program initialization*
…
*//start thread*
for (i = 0; i < num_threads; i++) {
      ret = pthread_create
    …
   }
*//do other program initialization*
   …
*   //do library cleanup when the program ends up*
xmlCleanupParser(();
   return 0;
 }
 ------------------------------------------------------------------------------ 
以上是我总结的在单线程和多线程环境下使用xmlInitParser()和xmlCleanupParser()时需要注意的几点,欢迎大家补充
