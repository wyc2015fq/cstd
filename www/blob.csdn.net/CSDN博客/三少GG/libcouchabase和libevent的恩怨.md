# libcouchabase和libevent的恩怨 - 三少GG - CSDN博客
2014年09月03日 23:10:56[三少GG](https://me.csdn.net/scut1135)阅读数：852
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)

libcouchbase Could not locate plugin library
Hi,
I had a simple store/get libcouchbase program running successfully in C++. It was compiled, and linked with the -lcouchbase flag.
I then added the -levent flag (in addition to the -libcouchbase) at the end so I could start doing some event loops, but didn't add any code (only changed the flag). The program compiles and links successfully. However, for some reason I now get this error:
 Error 27, "Temporary failure on the client" on every request. I removed the -levent and it works, added it back and it doesn't - multiple times. I tried both on libcb for 1.8 and 2.0 and it's the same issue.
How can I fix this?
Thanks
AED
[http://www.couchbase.com/communities/q-and-a/trying-use-libevent](http://www.couchbase.com/communities/q-and-a/trying-use-libevent)
4)
**libevent** (optional)
libcouchbase allows plugins to different event notification frameworks.
 The default framework for UNIX-like systems is libevent, so unless you're going to create your own plugin** you might want to install this.** Please note that the default for
 windows is something else so you don't need this at all for windows.
************************
https://github.com/chitika/cberl/blob/master/c_src/cb.c
