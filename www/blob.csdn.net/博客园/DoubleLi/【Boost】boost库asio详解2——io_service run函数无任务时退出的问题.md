# 【Boost】boost库asio详解2——io_service::run函数无任务时退出的问题 - DoubleLi - 博客园






io_service::work类可以使io_service::run函数在没有任务的时候仍然不返回，直至work对象被销毁。



**[cpp]**[view plain](http://blog.csdn.net/huang_xw/article/details/8471057#)[copy](http://blog.csdn.net/huang_xw/article/details/8471057#)

[print](http://blog.csdn.net/huang_xw/article/details/8471057#)[?](http://blog.csdn.net/huang_xw/article/details/8471057#)

- void test_asio_nowork()  
- {  
-     boost::asio::io_service ios;  
- 
-     PRINT_DEBUG("ios before");  
-     ios.run();  
-     PRINT_DEBUG("ios end");  
- }  
- 
- void test_asio_work()  
- {  
-     boost::asio::io_service ios;  
- // 增加一个work对象  
-     boost::asio::io_service::work work(ios);  
- 
-     PRINT_DEBUG("ios before");  
- // 当没有任务时，ios.run()也不会马上返回  
-     ios.run();  
-     PRINT_DEBUG("ios end");  
- }  



test_asio_nowork()执行结果:



**[cpp]**[view plain](http://blog.csdn.net/huang_xw/article/details/8471057#)[copy](http://blog.csdn.net/huang_xw/article/details/8471057#)

[print](http://blog.csdn.net/huang_xw/article/details/8471057#)[?](http://blog.csdn.net/huang_xw/article/details/8471057#)

- 2013-01-05 22:43:19 334 [4868] DEBUG - ios before  
- 2013-01-05 22:43:19 341 [4868] DEBUG - ios end  



test_asio_work()的执行结果:



**[cpp]**[view plain](http://blog.csdn.net/huang_xw/article/details/8471057#)[copy](http://blog.csdn.net/huang_xw/article/details/8471057#)

[print](http://blog.csdn.net/huang_xw/article/details/8471057#)[?](http://blog.csdn.net/huang_xw/article/details/8471057#)

- 2013-01-05 22:43:48 514 [7852] DEBUG - ios before  
- 


没有任务时, 也会阻塞.









