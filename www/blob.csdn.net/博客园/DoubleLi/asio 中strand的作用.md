# asio 中strand的作用 - DoubleLi - 博客园







**[cpp]**[view plain](http://blog.csdn.net/huang_xw/article/details/8469851#)[copy](http://blog.csdn.net/huang_xw/article/details/8469851#)

[print](http://blog.csdn.net/huang_xw/article/details/8469851#)[?](http://blog.csdn.net/huang_xw/article/details/8469851#)

- namespace  
- {  
- // strand提供串行执行, 能够保证线程安全, 同时被post或dispatch的方法, 不会被并发的执行.   
- // io_service不能保证线程安全  
-     boost::asio::io_service m_service;  
-     boost::asio::strand m_strand(m_service);  
-     boost::mutex m_mutex;  
- 
- void print(int id)  
-     {  
- // boost::mutex::scoped_lock lock(m_mutex);  
- static int count = 0;  
-         PRINT_DEBUG("id: " << boost::lexical_cast<std::string>(id));  
-         PRINT_DEBUG("count: " << boost::lexical_cast<std::string>(++count));  
-     }  
- 
- void ioRun1()  
-     {  
- while(true)  
-         {  
-             m_service.run();  
-         }  
-     }  
- 
- void ioRun2()  
-     {  
- while(true)  
-         {  
-             m_service.run();  
-         }  
-     }  
- 
- void strand_print1()  
-     {  
- // PRINT_DEBUG("Enter print1");  
-         m_strand.dispatch(boost::bind(print, 1));  
- // PRINT_DEBUG("Exit print1");  
-     }  
- 
- void strand_print2()  
-     {  
- // PRINT_DEBUG("Enter print2");  
-         m_strand.post(boost::bind(print, 2));  
- // PRINT_DEBUG("Exit print2");  
-     }  
- 
- void strand_print3()  
-     {  
- // PRINT_DEBUG("Enter print3");                
-         m_strand.post(boost::bind(print, 3));  
- // PRINT_DEBUG("Exit print3");  
-     }  
- 
- void strand_print4()  
-     {  
- // PRINT_DEBUG("Enter print4");  
-         m_strand.post(boost::bind(print, 4));  
- // PRINT_DEBUG("Exit print4");  
-     }  
- 
- // 将上面的m_strand换成m_service后，  
- void service_print1()  
-     {  
- // PRINT_DEBUG("Enter print1");  
-         m_service.dispatch(boost::bind(print, 1));  
- // PRINT_DEBUG("Exit print1");  
-     }  
- 
- void service_print2()  
-     {  
- // PRINT_DEBUG("Enter print2");  
-         m_service.post(boost::bind(print, 2));  
- // PRINT_DEBUG("Exit print2");  
-     }  
- 
- void service_print3()  
-     {  
- // PRINT_DEBUG("Enter print3");                
-         m_service.post(boost::bind(print, 3));  
- // PRINT_DEBUG("Exit print3");  
-     }  
- 
- void service_print4()  
-     {  
- // PRINT_DEBUG("Enter print4");  
-         m_service.post(boost::bind(print, 4));  
- // PRINT_DEBUG("Exit print4");  
-     }  
- }  
- 
- void test_strand()  
- {  
-     boost::thread ios1(ioRun1);  
-     boost::thread ios2(ioRun2);  
- 
-     boost::thread t1(strand_print1);  
-     boost::thread t2(strand_print2);  
-     boost::thread t3(strand_print3);  
-     boost::thread t4(strand_print4);  
- 
-     t1.join();  
-     t2.join();  
-     t3.join();  
-     t4.join();  
- 
-     m_server.run();  
- }  
- 
- void test_service()  
- {  
-     boost::thread ios1(ioRun1);  
-     boost::thread ios2(ioRun2);  
- 
-     boost::thread t1(service_print1);  
-     boost::thread t2(service_print2);  
-     boost::thread t3(service_print3);  
-     boost::thread t4(service_print4);  
- 
-     t1.join();  
-     t2.join();  
-     t3.join();  
-     t4.join();  
- 
-     m_service.run();  
- }  

test_strand的执行结果:



**[cpp]**[view plain](http://blog.csdn.net/huang_xw/article/details/8469851#)[copy](http://blog.csdn.net/huang_xw/article/details/8469851#)

[print](http://blog.csdn.net/huang_xw/article/details/8469851#)[?](http://blog.csdn.net/huang_xw/article/details/8469851#)

- 2013-01-05 17:25:34 626 [8228] DEBUG - id: 4  
- 2013-01-05 17:25:34 631 [8228] DEBUG - count: 1  
- 2013-01-05 17:25:34 634 [5692] DEBUG - id: 1  
- 2013-01-05 17:25:34 637 [5692] DEBUG - count: 2  
- 2013-01-05 17:25:34 640 [5692] DEBUG - id: 2  
- 2013-01-05 17:25:34 642 [5692] DEBUG - count: 3  
- 2013-01-05 17:25:34 646 [5692] DEBUG - id: 3  
- 2013-01-05 17:25:34 649 [5692] DEBUG - count: 4  



test_ioserivice的执行结果:



**[cpp]**[view plain](http://blog.csdn.net/huang_xw/article/details/8469851#)[copy](http://blog.csdn.net/huang_xw/article/details/8469851#)

[print](http://blog.csdn.net/huang_xw/article/details/8469851#)[?](http://blog.csdn.net/huang_xw/article/details/8469851#)

- 2013-01-05 17:26:28 071 [3236] DEBUG - id: 1  
- 2013-01-05 17:26:28 071 [5768] DEBUG - id: 2  
- 2013-01-05 17:26:28 071 [5108] DEBUG - id: 3  
- 2013-01-05 17:26:28 076 [3236] DEBUG - count: 1  
- 2013-01-05 17:26:28 079 [5768] DEBUG - count: 2  
- 2013-01-05 17:26:28 083 [5108] DEBUG - count: 3  
- 2013-01-05 17:26:28 087 [3236] DEBUG - id: 4  
- 2013-01-05 17:26:28 099 [3236] DEBUG - count: 4  



从结果可以看到, 在test_strand中print中两个打印函数成对执行, 在test_ioservice两个打印函数就没有线程安全可言了.
如果要保证test_ioservice同步, 就要加上mutex, 在代码中被注释的那句. 

注意从日志的线程号中可知: 真正执行print()是主线程, ios1, ios2, 而t1, t2, t3, t4线程只是往ioservice的队列中加入任务.









