# ExecutorService对象的shutdown()和shutdownNow()的区别 - 零度的博客专栏 - CSDN博客
2016年05月17日 15:41:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：1414
        可以关闭 ExecutorService，这将导致其拒绝新任务。提供两个方法来关闭 ExecutorService。shutdown() 方法在终止前允许执行以前提交的任务，而 shutdownNow() 方法阻止等待任务启动并试图停止当前正在执行的任务。在终止时，执行程序没有任务在执行，也没有任务在等待执行，并且无法提交新任务。应该关闭未使用的 ExecutorService 以允许回收其资源。 
        下列方法分两个阶段关闭 ExecutorService。第一阶段调用 shutdown 拒绝传入任务，然后调用 shutdownNow（如有必要）取消所有遗留的任务： 
**[java]**[view
 plain](http://blog.csdn.net/jdsjlzx/article/details/9079199#)[copy](http://blog.csdn.net/jdsjlzx/article/details/9079199#)
- void shutdownAndAwaitTermination(ExecutorService pool) {  
-   pool.shutdown(); // Disable new tasks from being submitted
- try {  
- // Wait a while for existing tasks to terminate
- if (!pool.awaitTermination(60, TimeUnit.SECONDS)) {  
-       pool.shutdownNow(); // Cancel currently executing tasks
- // Wait a while for tasks to respond to being cancelled
- if (!pool.awaitTermination(60, TimeUnit.SECONDS))  
-           System.err.println("Pool did not terminate");  
-     }  
-   } catch (InterruptedException ie) {  
- // (Re-)Cancel if current thread also interrupted
-     pool.shutdownNow();  
- // Preserve interrupt status
-     Thread.currentThread().interrupt();  
-   }  
- }  
        shutdown调用后，不可以再submit新的task，已经submit的将继续执行。
        shutdownNow试图停止当前正执行的task，并返回尚未执行的task的list。
