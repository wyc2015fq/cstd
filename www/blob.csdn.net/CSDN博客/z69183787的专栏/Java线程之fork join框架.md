# Java线程之fork/join框架 - z69183787的专栏 - CSDN博客
2017年04月15日 11:52:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1033
fork/join框架是用多线程的方式实现分治法来解决问题。fork指的是将问题不断地缩小规模，join是指根据子问题的计算结果，得出更高层次的结果。
fork/join框架的使用有一定的约束条件：
1. 除了fork()  和  join()方法外，线程不得使用其他的同步工具。线程最好也不要sleep()
2. 线程不得进行I/O操作
3. 线程不得抛出checked exception
此框架有几个核心类：ForkJoinPool是实现了工作窃取[算法](http://lib.csdn.net/base/datastructure)的线程池。ForkJoinTask是任务类，他有2个子类：RecursiveAction无返回值，RecursiveTask有返回值，在定义自己的任务时，一般都是从这2类中挑一个，通过继承的方式定义自己的新类。由于ForkJoinTask类实现了Serializable接口，因此，定义自己的任务类时，应该定义serialVersionUID属性。
在编写任务时，推荐的写法是这样的：
**[java]**[view
 plain](http://blog.csdn.net/andycpp/article/details/8903155#)[copy](http://blog.csdn.net/andycpp/article/details/8903155#)
- If (problem size > default size){  
- task s = divide(task);  
- execute(tasks);  
- } else {  
- resolve problem using another algorithm;  
- }  
ForkJoinPool实现了工作窃取算法（work-stealing），线程会主动寻找新创建的任务去执行，从而保证较高的线程利用率。它使用守护线程（deamon）来执行任务，因此无需对他显示的调用shutdown()来关闭。一般情况下，一个程序只需要唯一的一个ForkJoinPool，因此应该按如下方式创建它：
static final ForkJoinPool mainPool = new ForkJoinPool(); //线程的数目等于CPU的核心数
下面给出一个非常简单的例子，功能是将一个数组中每一个元素的值加1。具体实现为：将大数组不断分解为更短小的子数组，当子数组长度不超过10的时候，对其中所有元素进行加1操作。
**[java]**[view
 plain](http://blog.csdn.net/andycpp/article/details/8903155#)[copy](http://blog.csdn.net/andycpp/article/details/8903155#)
- publicclass Test {  
- 
- publicfinalstatic ForkJoinPool mainPool = new ForkJoinPool();  
- 
- publicstaticvoid main(String[] args){  
- int n = 26;  
- int[] a = newint[n];  
- for(int i=0; i<n; i++) {  
-             a[i] = i;  
-         }  
-         SubTask task = new SubTask(a, 0, n);  
-         mainPool.invoke(task);  
- for(int i=0; i<n; i++) {  
-             System.out.print(a[i]+" ");  
-         }  
-     }  
- }  
- 
- class SubTask extends RecursiveAction {  
- 
- privatestaticfinallong serialVersionUID = 1L;  
- 
- privateint[] a;  
- privateint beg;  
- privateint end;  
- 
- public SubTask(int[] a, int beg, int end) {  
- super();  
- this.a = a;  
- this.beg = beg;  
- this.end = end;  
-     }  
- 
- @Override
- protectedvoid compute() {  
- if(end-beg>10) {  
- int mid = (beg+end) / 2;  
-             SubTask t1 = new SubTask(a, beg, mid);  
-             SubTask t2 = new SubTask(a, mid, end);  
-             invokeAll(t1, t2);  
-         }else {  
- for(int i=beg; i<end; i++) {  
-                 a[i] = a[i] + 1;  
-             }  
-         }  
-     }  
- }  
例子2，任务拥有返回值。随机生成一个数组，每个元素均是0-999之间的整数，统计该数组中每个数字出现1的次数的和。
实现方法，将该数组不断的分成更小的数组，直到每个子数组的长度为1，即只包含一个元素。此时，统计该元素中包含1的个数。最后汇总，得到数组中每个数字共包含了多少个1。
**[java]**[view
 plain](http://blog.csdn.net/andycpp/article/details/8903155#)[copy](http://blog.csdn.net/andycpp/article/details/8903155#)
- publicclass Test {  
- 
- publicfinalstatic ForkJoinPool mainPool = new ForkJoinPool();  
- 
- publicstaticvoid main(String[] args){  
- int n = 26;  
- int[] a = newint[n];  
-         Random rand = new Random();  
- for(int i=0; i<n; i++) {  
-             a[i] = rand.nextInt(1000);  
-         }  
-         SubTask task = new SubTask(a, 0, n);  
- int count = mainPool.invoke(task);  
- for(int i=0; i<n; i++) {  
-             System.out.print(a[i]+" ");  
-         }  
-         System.out.println("\n数组中共出现了" + count + "个1");  
-     }  
- }  
- 
- class SubTask extends RecursiveTask<Integer> {  
- 
- privatestaticfinallong serialVersionUID = 1L;  
- 
- privateint[] a;  
- privateint beg;  
- privateint end;  
- 
- public SubTask(int[] a, int beg, int end) {  
- super();  
- this.a = a;  
- this.beg = beg;  
- this.end = end;  
-     }  
- 
- @Override
- protected Integer compute() {  
- int result = 0;  
- if(end-beg>1) {  
- int mid = (beg+end)/2;  
-             SubTask t1 = new SubTask(a, beg, mid);  
-             SubTask t2 = new SubTask(a, mid, end);  
-             invokeAll(t1, t2);  
- try {  
-                 result = t1.get()+t2.get();  
-             } catch (InterruptedException | ExecutionException e) {  
-                 e.printStackTrace();  
-             }  
-         } else {  
-             result = count(a[beg]);  
-         }  
- return result;  
-     }  
- 
- //统计一个整数中出现了几个1
- privateint count(int n) {  
- int result = 0;  
- while(n>0) {  
- if(n % 10==1) {  
-                 result++;  
-             }  
-             n = n / 10;  
-         }  
- return result;  
-     }  
- }  
例子3，异步执行任务。前面两个例子都是同步执行任务，当启动任务后，主线程陷入了阻塞状态，直到任务执行完毕。若创建新任务后，希望当前线程能继续执行而非陷入阻塞，则需要异步执行。ForkJoinPool线程池提供了execute()方法来异步启动任务，而作为任务本身，可以调用fork()方法异步启动新的子任务，并调用子任务的join()方法来取得计算结果。需要注意的是，异步使用ForkJoin框架，无法使用“工作窃取”算法来提高线程的利用率，针对每个子任务，系统都会启动一个新的线程。
本例的功能是查找硬盘上某一类型的文件。给定文件扩展名后，将硬盘上所有该类型的文件名打印显示出来。作为主程序，启动任务后，继续显示任务的执行进度，每3秒钟打印显示一个黑点，表示任务在继续。最后，当所有线程都结束了，打印显示结果。
**[java]**[view
 plain](http://blog.csdn.net/andycpp/article/details/8903155#)[copy](http://blog.csdn.net/andycpp/article/details/8903155#)
- publicclass ThreadLocalTest {  
- 
- publicstaticvoid main(String[] args) throws Exception {  
-         Path p = Paths.get("D:/");  
-         List<Path> roots = (List<Path>) FileSystems.getDefault().getRootDirectories();  
-         List<Path> result = new ArrayList<>();  
-         List<MyTask> tasks = new ArrayList<>();  
-         ForkJoinPool pool = new ForkJoinPool();  
- for(Path root:roots) {  
-             MyTask t = new MyTask(root, "pdf");  
-             pool.execute(t);  
-             tasks.add(t);  
-         }  
- 
-         System.out.print("正在处理中");  
- while(isAllDone(tasks) == false) {  
-             System.out.print(". ");  
-             TimeUnit.SECONDS.sleep(3);  
-         }  
- 
- for(MyTask t:tasks) {  
-             result.addAll(t.get());  
-         }  
- 
- for(Path pp:result) {  
-             System.out.println(pp);  
-         }  
-     }  
- 
- privatestaticboolean isAllDone(List<MyTask> tasks) {  
- boolean result = true;  
- for(MyTask t:tasks) {  
- if(t.isDone() == false) {  
-                 result = false;  
- break;  
-             }  
-         }  
- return result;  
-     }  
- }  
- 
- class MyTask extends RecursiveTask<List<Path>> {  
- 
- privatestaticfinallong serialVersionUID = 1L;  
- 
- private Path path;  
- private String fileExtention;  
- 
- public MyTask(Path path, String fileExtention) {  
- super();  
- this.path = path;  
- this.fileExtention = fileExtention;  
-     }  
- 
- @Override
- protected List<Path> compute() {  
-         List<Path> result = new ArrayList<>();  
- try {  
-             DirectoryStream<Path> paths = Files.newDirectoryStream(path);  
-             List<MyTask> subTasks = new ArrayList<>();  
- for(Path p:paths) {  
- if(Files.isDirectory(p)) {  
-                     MyTask t = new MyTask(p, fileExtention);  
-                     t.fork();  
-                     subTasks.add(t);  
-                 }elseif(Files.isRegularFile(p)) {  
- if(p.toString().toLowerCase().endsWith("."+fileExtention)) {  
-                         result.add(p);  
-                     }  
-                 }  
-             }  
- 
- for(MyTask t:subTasks) {  
-                 result.addAll(t.join());  
-             }  
-         } catch (IOException e) {  
-         }  
- return result;  
-     }  
- }  
