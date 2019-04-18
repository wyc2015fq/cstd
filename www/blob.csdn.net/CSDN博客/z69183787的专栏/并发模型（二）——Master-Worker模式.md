# 并发模型（二）——Master-Worker模式 - z69183787的专栏 - CSDN博客
2017年09月28日 16:46:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：492
﻿﻿
    Master-Worker模式是常用的并行模式之一，它的核心思想是，系统有两个进程协作工作：Master进程，负责接收和分配任务；Worker进程，负责处理子任务。当Worker进程将子任务处理完成后，结果返回给Master进程，由Master进程做归纳汇总，最后得到最终的结果。
**一、什么是Master-Worker模式：**
该模式的结构图：
![](https://img-blog.csdn.net/20140930173140802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG1kY3N6aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  结构图：
![](https://img-blog.csdn.net/20140930173039078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbG1kY3N6aA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Worker：用于实际处理一个任务；
Master：任务的分配和最终结果的合成；
Main：启动程序，调度开启Master。
**二、代码实现：**
    下面的是一个简易的Master-Worker框架实现。
（1）Master部分：
**[java]**[view
 plain](http://blog.csdn.net/lmdcszh/article/details/39698189#)[copy](http://blog.csdn.net/lmdcszh/article/details/39698189#)
- package MasterWorker;  
- 
- import java.util.HashMap;  
- import java.util.Map;  
- import java.util.Queue;  
- import java.util.concurrent.ConcurrentHashMap;  
- import java.util.concurrent.ConcurrentLinkedQueue;  
- 
- publicclass Master {  
- 
- //任务队列
- protected Queue<Object> workQueue= new ConcurrentLinkedQueue<Object>();  
- //Worker进程队列
- protected Map<String ,Thread> threadMap= new HashMap<String ,Thread>();  
- //子任务处理结果集
- protected Map<String ,Object> resultMap= new ConcurrentHashMap<String, Object>();  
- //是否所有的子任务都结束了
- publicboolean isComplete(){  
- for(Map.Entry<String , Thread> entry:threadMap.entrySet()){  
- if(entry.getValue().getState()!=Thread.State.TERMINATED){  
- returnfalse;  
-             }  
- 
-         }  
- returntrue ;  
-     }  
- 
- //Master的构造，需要一个Worker进程逻辑，和需要Worker进程数量
- public Master(Worker worker,int countWorker){  
- 
-         worker.setWorkQueue(workQueue);  
-         worker.setResultMap(resultMap);  
- for(int i=0;i<countWorker;i++){  
-             threadMap.put(Integer.toString(i),  new Thread(worker, Integer.toString(i)));  
-         }  
- 
-     }  
- 
- //提交一个任务
- publicvoid submit(Object job){  
-         workQueue.add(job);  
-     }  
- 
- 
- //返回子任务结果集
- public Map<String ,Object> getResultMap(){  
- return resultMap;  
-     }  
- 
- 
- //开始运行所有的Worker进程，进行处理
- publicvoid execute(){  
- for(Map.Entry<String , Thread> entry:threadMap.entrySet()){  
-              entry.getValue().start();  
- 
-          }  
-     }  
- 
- 
- }  
（2）Worker进程实现：
**[java]**[view
 plain](http://blog.csdn.net/lmdcszh/article/details/39698189#)[copy](http://blog.csdn.net/lmdcszh/article/details/39698189#)
- package MasterWorker;  
- 
- import java.util.Map;  
- import java.util.Queue;  
- 
- publicclass Worker  implements Runnable{  
- 
- //任务队列，用于取得子任务
- protected Queue<Object> workQueue;  
- //子任务处理结果集
- protected Map<String ,Object> resultMap;  
- publicvoid setWorkQueue(Queue<Object> workQueue){  
- this.workQueue= workQueue;  
-     }  
- 
- publicvoid setResultMap(Map<String ,Object> resultMap){  
- this.resultMap=resultMap;  
-     }  
- //子任务处理的逻辑，在子类中实现具体逻辑
- public Object handle(Object input){  
- return input;  
-     }  
- 
- 
- @Override
- publicvoid run() {  
- 
- while(true){  
- //获取子任务
-             Object input= workQueue.poll();  
- if(input==null){  
- break;  
-             }  
- //处理子任务
-             Object re = handle(input);  
-             resultMap.put(Integer.toString(input.hashCode()), re);  
-         }  
-     }  
- 
- }  

（3）运用这个小框架计算1——100的立方和，PlusWorker的实现：
**[java]**[view
 plain](http://blog.csdn.net/lmdcszh/article/details/39698189#)[copy](http://blog.csdn.net/lmdcszh/article/details/39698189#)
- package MasterWorker;  
- 
- publicclass PlusWorker extends Worker {  
- 
- @Override
- public Object handle(Object input) {  
- 
-         Integer i =(Integer)input;  
- return i*i*i;  
-     }  
- 
- 
- }  

（4）进行计算的Main函数：
**[java]**[view
 plain](http://blog.csdn.net/lmdcszh/article/details/39698189#)[copy](http://blog.csdn.net/lmdcszh/article/details/39698189#)
- package MasterWorker;  
- 
- import java.util.Map;  
- import java.util.Set;  
- 
- publicclass Main {  
- 
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
- //固定使用5个Worker，并指定Worker
-         Master m = new Master(new PlusWorker(), 5);  
- //提交100个子任务
- for(int i=0;i<100;i++){  
-             m.submit(i);  
-         }  
- //开始计算
-         m.execute();  
- int re= 0;  
- //保存最终结算结果
-         Map<String ,Object> resultMap =m.getResultMap();  
- 
- //不需要等待所有Worker都执行完成，即可开始计算最终结果
- while(resultMap.size()>0 || !m.isComplete()){  
-             Set<String> keys = resultMap.keySet();  
-             String key =null;  
- for(String k:keys){  
-                 key=k;  
- break;  
-             }  
-             Integer i =null;  
- if(key!=null){  
-                 i=(Integer)resultMap.get(key);  
-             }  
- if(i!=null){  
- //最终结果
-                 re+=i;  
-             }  
- if(key!=null){  
- //移除已经被计算过的项
-                 resultMap.remove(key);  
-             }  
- 
-         }  
- 
- 
-     }  
- 
- }  
**三、总结：**
    Master-Worker模式是一种将串行任务并行化的方案，被分解的子任务在系统中可以被并行处理，同时，如果有需要，Master进程不需要等待所有子任务都完成计算，就可以根据已有的部分结果集计算最终结果集。
