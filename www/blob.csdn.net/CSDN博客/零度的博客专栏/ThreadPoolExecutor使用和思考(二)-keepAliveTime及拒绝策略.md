# ThreadPoolExecutor使用和思考(二)-keepAliveTime及拒绝策略 - 零度的博客专栏 - CSDN博客
2017年03月28日 14:08:01[零度anngle](https://me.csdn.net/zmx729618)阅读数：2420
通过上篇文章，我们可以总结出：ThreadPoolExecutor中额定的“工人”数量由corePoolSize决定，当任务数量超过额定工人数量时，将任务缓存在BlockingQueue之中，当发现如果连queue中也放不下时（可能是因为使用有界queue，也可能是使用SynchronousQueue），ThreadPoolExecutor会请求“老板”再派几个“工人”过来。
接下来发生的事情有两种情况：
- 任务不再过来了 - keepAliveTime
- 任务仍然继续过来 - RejectedExecutionHandler
===============================神奇分割线==================================
**keepAliveTime**
jdk中的解释是：当线程数大于核心时，此为终止前多余的空闲线程等待新任务的最长时间。
有点拗口，其实这个不难理解，在使用了“池”的应用中，大多都有类似的参数需要配置。比如数据库连接池，DBCP中的maxIdle，minIdle参数。
什么意思？接着上面的解释，后来向老板派来的工人始终是“借来的”，俗话说“有借就有还”，但这里的问题就是什么时候还了，如果借来的工人刚完成一个任务就还回去，后来发现任务还有，那岂不是又要去借？这一来一往，老板肯定头也大死了。
合理的策略：既然借了，那就多借一会儿。直到“某一段”时间后，发现再也用不到这些工人时，便可以还回去了。这里的某一段时间便是keepAliveTime的含义，TimeUnit为keepAliveTime值的度量。
**RejectedExecutionHandler**
另一种情况便是，即使向老板借了工人，但是任务还是继续过来，还是忙不过来，这时整个队伍只好拒绝接受了。
RejectedExecutionHandler接口提供了对于拒绝任务的处理的自定方法的机会。在ThreadPoolExecutor中已经默认包含了4中策略，因为源码非常简单，这里直接贴出来。
**CallerRunsPolicy**：线程调用运行该任务的 execute 本身。此策略提供简单的反馈控制机制，能够减缓新任务的提交速度。
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- publicvoid rejectedExecution(Runnable r, ThreadPoolExecutor e) {  
- if (!e.isShutdown()) {  
-                 r.run();  
-             }  
-         }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这个策略显然不想放弃执行任务。但是由于池中已经没有任何资源了，那么就直接使用调用该execute的线程本身来执行。
**AbortPolicy：**处理程序遭到拒绝将抛出运行时 `RejectedExecutionException`
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- publicvoid rejectedExecution(Runnable r, ThreadPoolExecutor e) {  
- thrownew RejectedExecutionException();  
-         }  
![](http://static.blog.csdn.net/images/save_snippets.png)
 这种策略直接抛出异常，丢弃任务。
**DiscardPolicy：**不能执行的任务将被删除
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- publicvoid rejectedExecution(Runnable r, ThreadPoolExecutor e) {  
-         }  
![](http://static.blog.csdn.net/images/save_snippets.png)
 这种策略和AbortPolicy几乎一样，也是丢弃任务，只不过他不抛出异常。
**DiscardOldestPolicy：**如果执行程序尚未关闭，则位于工作队列头部的任务将被删除，然后重试执行程序（如果再次失败，则重复此过程）
Java代码  ![收藏代码](http://dongxuan.iteye.com/images/icon_star.png)
- publicvoid rejectedExecution(Runnable r, ThreadPoolExecutor e) {  
- if (!e.isShutdown()) {  
-                 e.getQueue().poll();  
-                 e.execute(r);  
-             }  
-         }  
![](http://static.blog.csdn.net/images/save_snippets.png)
该策略就稍微复杂一些，在pool没有关闭的前提下首先丢掉缓存在队列中的最早的任务，然后重新尝试运行该任务。这个策略需要适当小心。
设想:如果其他线程都还在运行，那么新来任务踢掉旧任务，缓存在queue中，再来一个任务又会踢掉queue中最老任务。
总结：
keepAliveTime和maximumPoolSize及BlockingQueue的类型均有关系。如果BlockingQueue是无界的，那么永远不会触发maximumPoolSize，自然keepAliveTime也就没有了意义。
反之，如果核心数较小，有界BlockingQueue数值又较小，同时keepAliveTime又设的很小，如果任务频繁，那么系统就会频繁的申请回收线程。
