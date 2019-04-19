# 被废弃的dispatch_get_current_queue - 月若无涯的专栏 - CSDN博客
2015年04月24日 13:55:23[月若无涯](https://me.csdn.net/u010124617)阅读数：620标签：[iOS																[GCD](https://so.csdn.net/so/search/s.do?q=GCD&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/u010124617/article/category/1393991)
想说说被废弃的dispatch_get_current_queue，不得不提一个概念：可重入。
        什么是可重入，来自维基百科：若一个程序或子程序可以“安全的被并行执行(Parallel computing)”，则称其为可重入（reentrant或re-entrant）的。
即当该子程序正在运行时，可以再次进入并执行它（并行执行时，个别的执行结果，都符合设计时的预期）。
若一个函数是可重入的，则该函数：
- 不能含有静态（全局）非常量数据
- 不能返回静态（全局）非常量数据的地址
- 只能处理由调用者提供的数据
- 不能依赖于单实例模式资源的锁
- 不能调用(call)不可重入的函数(有呼叫(call)到的函数需满足前述条件)
       有时候我们很希望知道当前执行的queue是谁，比如UI操作需要放在main queue中执行。如果可以知道当前工作的queue是谁，就可以很方便的指定一段代码操作在特定的queue中执行。 dispatch_get_current_queue() 正好能帮上忙。于是乎，在指定的queue中做一些操作，就可以非常清晰的实现：
- void func(dispatch_queue_t queue, dispatch_block_t block)  
- {  
- if (dispatch_get_current_queue() == queue) {  
-         block();  
-     }else{  
-         dispatch_sync(queue, block);  
-     }  
- }  
然后潜意识里，觉得这个函数是可重入的。
但当target queue恰好是current queue时，同步阻塞会导致死锁。
- - (void)deadLockFunc  
- {  
-     dispatch_queue_t queueA = dispatch_queue_create("com.yiyaaixuexi.queueA", NULL);  
-     dispatch_queue_t queueB = dispatch_queue_create("com.yiyaaixuexi.queueB", NULL);  
-     dispatch_sync(queueA, ^{  
-         dispatch_sync(queueB, ^{  
-             dispatch_block_t block = ^{  
- //do something
-             };  
-             func(queueA, block);  
-         });  
-     });  
- }  
问题出在GCD队列本身是不可重入的，串行同步队列的层级关系，是出现问题的根本原因。
为了防止类似的误用，苹果在iOS6废弃了dispatch_get_current_queue()方法。强大的dispatch_get_current_queue()也只能当作一个调试工具了。
那么应该如何保证GCD方法可重入呢？
- dispatch_queue_set_specific 标记队列
- 递归锁
分别给出两种方法的示例代码片段：
1）dispatch_queue_set_specific
- dispatch_queue_t queueA = dispatch_queue_create("com.yiyaaixuexi.queueA", NULL);  
-    dispatch_queue_t queueB = dispatch_queue_create("com.yiyaaixuexi.queueB", NULL);  
-    dispatch_set_target_queue(queueB, queueA);  
- 
- staticint specificKey;  
-    CFStringRef specificValue = CFSTR("queueA");  
-    dispatch_queue_set_specific(queueA,  
-                                &specificKey,  
-                                (void*)specificValue,  
-                                (dispatch_function_t)CFRelease);  
- 
-    dispatch_sync(queueB, ^{  
-        dispatch_block_t block = ^{  
- //do something
-        };  
-        CFStringRef retrievedValue = dispatch_get_specific(&specificKey);  
- if (retrievedValue) {  
-            block();  
-        } else {  
-            dispatch_sync(queueA, block);  
-        }  
-    });  
2）递归锁
- void dispatch_reentrant(void (^block)())  
- {  
- staticNSRecursiveLock *lock = nil;  
- static dispatch_once_t onceToken;  
-     dispatch_once(&onceToken, ^{  
-         lock = [[NSRecursiveLock alloc]init];  
-     });  
-     [lock lock];  
-     block();  
-     [lock unlock];  
- }  
- 
-     dispatch_queue_t queueA = dispatch_queue_create("com.yiyaaixuexi.queueA", NULL);  
-     dispatch_block_t block = ^{  
- //do something
-     };  
-     dispatch_sync(queueA, ^{  
-         dispatch_reentrant(block);  
-     });   
- 
