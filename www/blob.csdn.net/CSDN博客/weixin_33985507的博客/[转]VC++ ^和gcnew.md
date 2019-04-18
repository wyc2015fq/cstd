# [转]VC++ ^和gcnew - weixin_33985507的博客 - CSDN博客
2012年09月29日 08:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
本文转自：[http://www.blogjava.net/yangtaochina/articles/371024.html](http://www.blogjava.net/yangtaochina/articles/371024.html)
在VS2010中可能是因为.net是对C++又进行了优化。有些代码不同了，现记录于此，防止以后忘掉了。
StreamWriter^ sw = File::CreateText(path);
上面的^可能就是引用的意思，就是原来C++中的指针了。
sw->WriteLine( "Hello" );
其调用方式也是和指针的调用方法的方式是一样的。
Thread^ t = gcnew Thread(gcnew ThreadStart(writeStringThread::ThreadProc));
上面一句代码是VS 中创建线程的代码，看着就很蛋疼，不仅是^ 还有一个gcnew 从字面意思理解的话可能就是编译器自动对对象进行了管理，gc就是垃圾回收嘛。
我在晚上搜索了gcnew说：
C++/CLI中使用gcnew关键字表示在托管堆上分配内存，并且为了与以前的指针区分，用^来替换* ，就语义上来说他们的区别大致如下:
　　1.     gcnew返回的是一个句柄(Handle)，而new返回的是实际的内存地址. 
　　2.     gcnew创建的对象由虚拟机托管，而new创建的对象必须自己来管理和释放.
微软官方解释
gcnew creates an instance of a managed type (reference or value type) on the garbage collected heap. The result of the evaluation of a gcnew expression is a handle (^) to the type being created.
说gcnew 创建了一个实例在GC堆上，导致的结果就是一个句柄类型已经被创建。
