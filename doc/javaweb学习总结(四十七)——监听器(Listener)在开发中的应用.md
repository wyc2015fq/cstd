## [javaweb学习总结(四十七)——监听器(Listener)在开发中的应用](https://www.cnblogs.com/xdp-gacl/p/3965508.html)

　　监听器在JavaWeb开发中用得比较多，下面说一下监听器(Listener)在开发中的常见应用

## 一、统计当前在线人数

　　在JavaWeb应用开发中，有时候我们需要统计当前在线的用户数，此时就可以使用监听器技术来实现这个功能了。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.listener;
 2 
 3 import javax.servlet.ServletContext;
 4 import javax.servlet.http.HttpSessionEvent;
 5 import javax.servlet.http.HttpSessionListener;
 6 
 7 /**
 8 * @ClassName: OnLineCountListener
 9 * @Description: 统计当前在线用户个数
10 * @author: 孤傲苍狼
11 * @date: 2014-9-10 下午10:01:26
12 *
13 */ 
14 public class OnLineCountListener implements HttpSessionListener {
15 
16     @Override
17     public void sessionCreated(HttpSessionEvent se) {
18         ServletContext context = se.getSession().getServletContext();
19         Integer onLineCount = (Integer) context.getAttribute("onLineCount");
20         if(onLineCount==null){
21             context.setAttribute("onLineCount", 1);
22         }else{
23             onLineCount++;
24             context.setAttribute("onLineCount", onLineCount);
25         }
26     }
27 
28     @Override
29     public void sessionDestroyed(HttpSessionEvent se) {
30         ServletContext context = se.getSession().getServletContext();
31         Integer onLineCount = (Integer) context.getAttribute("onLineCount");
32         if(onLineCount==null){
33             context.setAttribute("onLineCount", 1);
34         }else{
35             onLineCount--;
36             context.setAttribute("onLineCount", onLineCount);
37         }
38     }
39 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 二、自定义Session扫描器

　　当一个Web应用创建的Session很多时，为了避免Session占用太多的内存，我们可以选择手动将这些内存中的session销毁，那么此时也可以借助监听器技术来实现。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.web.listener;
  2 
  3 import java.util.Collections;
  4 import java.util.LinkedList;
  5 import java.util.List;
  6 import java.util.ListIterator;
  7 import java.util.Timer;
  8 import java.util.TimerTask;
  9 import javax.servlet.ServletContextEvent;
 10 import javax.servlet.ServletContextListener;
 11 import javax.servlet.http.HttpSession;
 12 import javax.servlet.http.HttpSessionEvent;
 13 import javax.servlet.http.HttpSessionListener;
 14 
 15 /**
 16 * @ClassName: SessionScanerListener
 17 * @Description: 自定义session扫描器
 18 * @author: 孤傲苍狼
 19 * @date: 2014-9-10 下午10:16:42
 20 * 
 21 */ 
 22 public class SessionScanerListener implements HttpSessionListener,ServletContextListener {
 23 
 24     /**
 25     * @Field: list
 26     *          定义一个集合存储服务器创建的HttpSession
 27     *        LinkedList不是一个线程安全的集合
 28     */ 
 29     /**
 30      * private List<HttpSession> list = new LinkedList<HttpSession>();
 31      * 这样写涉及到线程安全问题,SessionScanerListener对象在内存中只有一个
 32      * sessionCreated可能会被多个人同时调用，
 33      * 当有多个人并发访问站点时，服务器同时为这些并发访问的人创建session
 34      * 那么sessionCreated方法在某一时刻内会被几个线程同时调用，几个线程并发调用sessionCreated方法
 35      * sessionCreated方法的内部处理是往一个集合中添加创建好的session，那么在加session的时候就会
 36      * 涉及到几个Session同时抢夺集合中一个位置的情况，所以往集合中添加session时，一定要保证集合是线程安全的才行
 37      * 如何把一个集合做成线程安全的集合呢？
 38      * 可以使用使用 Collections.synchronizedList(List<T> list)方法将不是线程安全的list集合包装线程安全的list集合
 39      */
 40     //使用 Collections.synchronizedList(List<T> list)方法将LinkedList包装成一个线程安全的集合
 41     private List<HttpSession> list = Collections.synchronizedList(new LinkedList<HttpSession>());
 42     //定义一个对象，让这个对象充当一把锁，用这把锁来保证往list集合添加的新的session和遍历list集合中的session这两个操作达到同步
 43     private Object lock = new Object();
 44             
 45     @Override
 46     public void sessionCreated(HttpSessionEvent se) {
 47         System.out.println("session被创建了!!");
 48         HttpSession session = se.getSession();
 49         
 50         synchronized (lock){
 51             /**
 52              *将该操作加锁进行锁定，当有一个thread-1(线程1)在调用这段代码时，会先拿到lock这把锁，然后往集合中添加session，
 53              *在添加session的这个过程中假设有另外一个thread-2(线程2)来访问了，thread-2可能是执行定时器任务的，
 54              *当thread-2要调用run方法遍历list集合中的session时，结果发现遍历list集合中的session的那段代码被锁住了，
 55              *而这把锁正在被往集合中添加session的那个thread-1占用着，因此thread-2只能等待thread-1操作完成之后才能够进行操作
 56              *当thread-1添加完session之后，就把lock放开了，此时thread-2拿到lock，就可以执行遍历list集合中的session的那段代码了
 57              *通过这把锁就保证了往集合中添加session和变量集合中的session这两步操作不能同时进行，必须按照先来后到的顺序来进行。
 58              */
 59             list.add(session);
 60         }
 61     }
 62 
 63     @Override
 64     public void sessionDestroyed(HttpSessionEvent se) {
 65         System.out.println("session被销毁了了!!");
 66     }
 67 
 68     /* Web应用启动时触发这个事件
 69      * @see javax.servlet.ServletContextListener#contextInitialized(javax.servlet.ServletContextEvent)
 70      */
 71     @Override
 72     public void contextInitialized(ServletContextEvent sce) {
 73         System.out.println("web应用初始化");
 74         //创建定时器
 75         Timer timer = new Timer();
 76         //每隔30秒就定时执行任务
 77         timer.schedule(new MyTask(list,lock), 0, 1000*30);
 78     }
 79 
 80     @Override
 81     public void contextDestroyed(ServletContextEvent sce) {
 82         System.out.println("web应用关闭");
 83     }
 84 }
 85 
 86 /**
 87 * @ClassName: MyTask
 88 * @Description:定时器要定时执行的任务
 89 * @author: 孤傲苍狼
 90 * @date: 2014-9-11 上午12:02:36
 91 *
 92 */ 
 93 class MyTask extends TimerTask {
 94         
 95     //存储HttpSession的list集合
 96     private List<HttpSession> list;
 97     //存储传递过来的锁
 98     private Object lock;
 99     public MyTask(List<HttpSession> list,Object lock){
100         this.list = list;
101         this.lock = lock;
102     }
103     /* run方法指明了任务要做的事情
104      * @see java.util.TimerTask#run()
105      */
106     @Override
107     public void run() {
108             //将该操作加锁进行锁定
109         synchronized (lock) {
110             System.out.println("定时器执行！！");
111             ListIterator<HttpSession> it = list.listIterator();
112             /**
113              * 迭代list集合中的session，在迭代list集合中的session的过程中可能有别的用户来访问，
114              * 用户一访问，服务器就会为该用户创建一个session,此时就会调用sessionCreated往list集合中添加新的session，
115              * 然而定时器在定时执行扫描遍历list集合中的session时是无法知道正在遍历的list集合又添加的新的session进来了，
116              * 这样就导致了往list集合添加的新的session和遍历list集合中的session这两个操作无法达到同步
117              * 那么解决的办法就是把"list.add(session)和while(it.hasNext()){//迭代list集合}"这两段代码做成同步，
118              * 保证当有一个线程在访问"list.add(session)"这段代码时，另一个线程就不能访问"while(it.hasNext()){//迭代list集合}"这段代码
119              * 为了能够将这两段不相干的代码做成同步，只能定义一把锁(Object lock)，然后给这两步操作加上同一把锁，
120              * 用这把锁来保证往list集合添加的新的session和遍历list集合中的session这两个操作达到同步
121              * 当在执行往list集合添加的新的session操作时，就必须等添加完成之后才能够对list集合进行迭代操作，
122              * 当在执行对list集合进行迭代操作时，那么必须等到迭代操作结束之后才能够往往list集合添加的新的session
123              */
124             while(it.hasNext()){
125                 HttpSession session = (HttpSession) it.next();
126                 /**
127                  * 如果当前时间-session的最后访问时间>1000*15(15秒)
128                  * session.getLastAccessedTime()获取session的最后访问时间
129                  */
130                 if(System.currentTimeMillis()-session.getLastAccessedTime()>1000*30){
131                     //手动销毁session
132                     session.invalidate();
133                     //移除集合中已经被销毁的session
134                     it.remove();
135                 }
136             }
137         }
138     }
139 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 以上就是监听器的两个简单应用场景。



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)