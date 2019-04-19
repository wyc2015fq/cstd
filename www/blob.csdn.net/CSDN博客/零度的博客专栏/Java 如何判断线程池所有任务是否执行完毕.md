# Java 如何判断线程池所有任务是否执行完毕 - 零度的博客专栏 - CSDN博客
2016年10月10日 16:16:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：3620
### shutdown
void **shutdown**()
启动一次顺序关闭，执行以前提交的任务，但不接受新任务。如果已经关闭，则调用没有其他作用。
** 抛出：**
`[SecurityException](http://write.blog.csdn.net/postedit)` - 如果安全管理器存在并且关闭，此 ExecutorService 可能操作某些不允许调用者修改的线程（因为它没有保持[`RuntimePermission`](http://write.blog.csdn.net/postedit)("modifyThread")），或者安全管理器的 checkAccess 方法拒绝访问。
### isTerminated
boolean **isTerminated**()如果关闭后所有任务都已完成，则返回 true。注意，除非首先调用 shutdown 或 shutdownNow，否则 isTerminated 永不为 true。
- **返回：**
如果关闭后所有任务都已完成，则返回 true

**[java]**[view plain](http://blog.csdn.net/lisheng19870305/article/details/44747741#)[copy](http://blog.csdn.net/lisheng19870305/article/details/44747741#)[print](http://blog.csdn.net/lisheng19870305/article/details/44747741#)
-  * 采用线程池开启多个子线程，主线程等待所有的子线程执行完毕  
-  */  
- publicstaticvoid moreThread() {  
- try {  
- int threadNum = 0;  
- for (int i = 0; i < 10; i++) {  
-             threadNum++;  
- 
- finalint currentThreadNum = threadNum;  
-             exe.execute(new Runnable() {  
- 
- @Override
- publicvoid run() {  
- try {  
-                         System.out.println("子线程[" + currentThreadNum + "]开启");  
-                         Thread.sleep(1000*10);  
-                     } catch (InterruptedException e) {  
-                         e.printStackTrace();  
-                     }finally{  
-                         System.out.println("子线程[" + currentThreadNum + "]结束");  
-                     }  
-                 }  
-             });    
-         }  
- 
-         System.out.println("已经开启所有的子线程");  
-         exe.shutdown();  
-         System.out.println("shutdown()：启动一次顺序关闭，执行以前提交的任务，但不接受新任务。");  
- while(true){  
- if(exe.isTerminated()){  
-                 System.out.println("所有的子线程都结束了！");  
- break;  
-             }  
-             Thread.sleep(1000);    
-         }  
- 
-     } catch (InterruptedException e) {  
-         e.printStackTrace();  
-     }finally{  
-         System.out.println("主线程结束");  
-     }  
- }  
[]()
```java
* 采用线程池开启多个子线程，主线程等待所有的子线程执行完毕
	 */
	public static void moreThread() {
		try {
			int threadNum = 0;
			for (int i = 0; i < 10; i++) {
				threadNum++;
				
				final int currentThreadNum = threadNum;
				exe.execute(new Runnable() {
					
					@Override
					public void run() {
						try {
							System.out.println("子线程[" + currentThreadNum + "]开启");
							Thread.sleep(1000*10);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}finally{
							System.out.println("子线程[" + currentThreadNum + "]结束");
						}
					}
				});  
			}
			
			System.out.println("已经开启所有的子线程");
			exe.shutdown();
			System.out.println("shutdown()：启动一次顺序关闭，执行以前提交的任务，但不接受新任务。");
			while(true){
				if(exe.isTerminated()){
					System.out.println("所有的子线程都结束了！");
					break;
				}
				Thread.sleep(1000);  
			}
			
		} catch (InterruptedException e) {
			e.printStackTrace();
		}finally{
			System.out.println("主线程结束");
		}
	}
```

**[java]**[view plain](http://blog.csdn.net/lisheng19870305/article/details/44747741#)[copy](http://blog.csdn.net/lisheng19870305/article/details/44747741#)[print](http://blog.csdn.net/lisheng19870305/article/details/44747741#)
- 子线程[1]开启  
- 子线程[6]开启  
- 子线程[2]开启  
- 子线程[5]开启  
- 子线程[9]开启  
- 已经开启所有的子线程  
- 子线程[3]开启  
- 子线程[7]开启  
- 子线程[10]开启  
- shutdown()：启动一次顺序关闭，执行以前提交的任务，但不接受新任务。  
- 子线程[4]开启  
- 子线程[8]开启  
- 子线程[6]结束  
- 子线程[3]结束  
- 子线程[7]结束  
- 子线程[2]结束  
- 子线程[1]结束  
- 子线程[5]结束  
- 子线程[9]结束  
- 子线程[4]结束  
- 子线程[10]结束  
- 子线程[8]结束  
- 所有的子线程都结束了！  
- 主线程结束  
