# DelayQueue 队列 - z69183787的专栏 - CSDN博客
2016年07月28日 15:43:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：334
个人分类：[Java-并发集合](https://blog.csdn.net/z69183787/article/category/5783255)

 DelayQueue----一种有序队列，特点就是只有在队列中的元素到期后才能取出。
              1.内存中哪些对象到了超时时间，需要从内存中清除出去。
              2.服务器连接中有哪些连接很长时间未操作，需要关闭这些连接
              3.任务中有哪些到了执行时间，该进行调度了。
简单的方法就是写一个线程不断去检查每一项是否到了时间，但这种方法在队列中的元素非常多的情况下会不准确或太耗费性能，用DelayQueue可完美地解决这个问题。
假设镇上开了一家无限大的网吧，每人交一块钱可以上一分钟网(过年时的网吧坑爹程度差不多就这样)；
             网吧比较老式没有先进的管理系统，为了知道哪个家伙时间到该下机了，老板得一台一台地去看，上网的人一多等老板把整个网吧转一遍有些家伙早超时了，而且老板要一遍一遍地检查也累个半死；
             有一天某个人给老板开发了个简单的程序，这个程序可以自动将上机的人排序，并在恰当的时间告诉老板哪个家伙时间到了。这个程序大概会是这样：
**[java]**[view
 plain](http://blog.csdn.net/yjl49/article/details/7088121#)[copy](http://blog.csdn.net/yjl49/article/details/7088121#)
- publicclass Wangming implements Delayed {  
- 
- private String name;  
- //身份证
- private String id;  
- //截止时间
- privatelong endTime;  
- 
- public Wangming(String name,String id,long endTime){  
- this.name=name;  
- this.id=id;  
- this.endTime=endTime;  
-     }  
- 
- public String getName(){  
- returnthis.name;  
-     }  
- 
- public String getId(){  
- returnthis.id;  
-     }  
- 
- /**
-      * 用来判断是否到了截止时间,
* getDelay(TimeUnit unit)方法应该返回以纳秒为单位的时间差,而不是如博主所写的毫秒为单位的时间差
-      */
- @Override
- publiclong getDelay(TimeUnit unit) {  
- // TODO Auto-generated method stub
- return endTime-System.currentTimeMillis();  
-     }  
- 
- /**
-      * 相互批较排序用
-      */
- @Override
- publicint compareTo(Delayed o) {  
- // TODO Auto-generated method stub
-         Wangming jia = (Wangming)o;  
- return endTime-jia.endTime>0?1:0;  
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/yjl49/article/details/7088121#)[copy](http://blog.csdn.net/yjl49/article/details/7088121#)
- publicclass WangBa implements Runnable {  
- 
- private DelayQueue<Wangming> queue = new DelayQueue<Wangming>();  
- publicboolean yinye =true;  
- 
- publicvoid shangji(String name,String id,int money){  
-         Wangming man = new Wangming(name,id,1000*60*money+System.currentTimeMillis());        
-         System.out.println("网名"+man.getName()+" 身份证"+man.getId()+"交钱"+money+"块,开始上机...");  
- this.queue.add(man);  
-     }  
- 
- publicvoid xiaji(Wangming man){  
-         System.out.println("网名"+man.getName()+" 身份证"+man.getId()+"时间到下机...");  
-     }  
- 
- @Override
- publicvoid run() {  
- // TODO Auto-generated method stub
- while(yinye){  
- try {  
-                 System.out.println("检查ing");  
-                 Wangming man = queue.take();  
-                 xiaji(man);  
-             } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- 
- publicstaticvoid main(String args[]){  
- try{  
-             System.out.println("网吧开始营业");  
-             WangBa siyu = new WangBa();  
-             Thread shangwang = new Thread(siyu);  
-             shangwang.start();  
- 
-             siyu.shangji("路人甲", "123", 1);  
-             siyu.shangji("路人乙", "234", 2);  
-             siyu.shangji("路人丙", "345", 1);  
-         }  
- catch(Exception ex){  
- 
-         }  
- 
-     }  
- }  
运行结果：
**[html]**[view
 plain](http://blog.csdn.net/yjl49/article/details/7088121#)[copy](http://blog.csdn.net/yjl49/article/details/7088121#)
- 网吧开始营业  
- 检查ing  
- 网名路人甲 身份证123交钱1块,开始上机...  
- 网名路人乙 身份证234交钱2块,开始上机...  
- 网名路人丙 身份证345交钱1块,开始上机...  
- 网名路人甲 身份证123时间到下机...  
- 检查ing  
- 网名路人丙 身份证345时间到下机...  
- 检查ing  
- 网名路人乙 身份证234时间到下机...  
- 检查ing  
从上面的运行结果可以看出，程序并没有一遍一遍地扫描队列中的对象，而是在队列中没有对象符合条件时阻塞。
有了这个程序老板邪恶地笑了，他这下子可以舒舒服服地坑爹了。
至于这个好不好用呢，用过才知道，尤其是在并发量比较大的情况下。
DelayQueue 的内部是一个优先队列，从数据结构上看是一个类有序二叉树：
offer、poll、remove() 和 add------ O(log(n)) 时间；
remove(Object) 和 contains(Object) ------线性时间；
peek、element 和 size-------固定时间
