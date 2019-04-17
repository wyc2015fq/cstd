# Java常见编程陷阱总结 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年11月18日 17:01:59[boonya](https://me.csdn.net/boonya)阅读数：569








# 1.首先什么是陷阱

　　简洁的定义：

 　　　　 陷阱，是指那些能够正常编译，但是在执行时却产生事与愿违的，有时候甚至是灾难性后果的程序代码。

　　广义的定义：

  　　　　任何可能导致程序员把大量的时间浪费在开发工具的使用上而不是最终软件的进展上的语言特性、API或系统，都可以称呼为陷阱。

# 2、陷阱的分类



![](http://pic002.cnblogs.com/images/2012/457851/2012102415313828.jpg)

# 3、分析陷阱三重奏

　　a.症状或者问题

  　　首先找到是哪一个代码造成的问题，陷阱的类型是什么。

　　b.问题的根源

  　　这个是揭示陷阱最重要的一个部分，我们要深入底层，了解可能导致程序员绊脚的详细内部工作过程、无效的假设或者API的缺陷。

　　c.解决方案

 　　 这个是分析陷阱的最后一个步骤，最终给出一个程序实现和运行结果。

# 4、示例讲解

例子1：找奇数

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 　　//找奇数
 2     public static boolean isOdd(int i) {
 3         return i % 2 == 1;
 4     }
 5     
 6     public static void main(String[] args) {
 7         System.out.println(isOdd(1));
 8         System.out.println(isOdd(2));
 9         System.out.println(isOdd(3));
10         System.out.println(isOdd(-1));
11         System.out.println(isOdd(-2));
12     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

你会发现所有的负数在isOdd()方法里返回的都是false

更正方法：

```
1 　　public static boolean isOdd(int i) {
2         return i % 2 != 0;
3     }
```



例子2：浮点数相减

```
1 　　public static void main(String[] args) {
2         System.out.println(2.0-1.1);
3     }
```

你觉得得数会是多少？0.9？还是0.9000.....?

答案是：0.8999999999999999 可是我要的是0.9啊！ 那怎么怎么办呢？

```
1 　　public static void main(String[] args) {
2         System.out.println(new BigDecimal("2.0").subtract(new BigDecimal("1.1")));
3     }
```

这样就实现了变成0.9了

如果你觉得太麻烦了那你还能这样：



```
1 System.out.printf("%.1f",2.0-1.1);
```



例子3、长整数

```
1 public static void main(String[] args) {
2         final long MICROS_PER_DAY = 24 * 60 * 60 * 1000 * 1000;
3         final long MILLIS_PER_DAY = 24 * 60 * 60 * 1000;
4         System.out.println(MICROS_PER_DAY / MILLIS_PER_DAY);
5     }
```

你一看觉得得数是多少？ 很多人回答会是 1000 因为大家都能算的出来  但是你运行发现得数是5  你会说MICROS_PER_DAY 是long啊 应该不会溢出啊，

这是因为在java中 24 默认是int型的 那么24 * 60 * 60 * 1000 * 1000都会是int型的 当然就会溢出 最后会把溢出后的值给MICROS_PER_DAY 所以最后的值会莫名其妙

改正：

```
1 public static void main(String[] args) {
2         final long MICROS_PER_DAY = 24L * 60 * 60 * 1000 * 1000;
3         final long MILLIS_PER_DAY = 24L * 60 * 60 * 1000;
4         System.out.println(MICROS_PER_DAY / MILLIS_PER_DAY);
5     }
```

NO.4  互换内容

```
1 public static void main(String[] args) {
2         int x = 1234;
3         int y = 5678;
4         x ^= y^= x^= y;
5         System.out.println(" x = " + x + " , y =" + y);
6     }
```

在c或者C++中有人见过这样的交换两个数的方法 。但是在java中有点不相同 结果是  x = 0 , y =1234

其实它要的原理是这样的：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 public static void main(String[] args) {
2         int x = 1234;
3         int y = 5678;
4         y = x ^ y;
5         x = x ^ y;
6         y = x ^ y;
7         System.out.println(" x = " + x + " , y =" + y);
8     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

当由于java和c/C++存在差异所以才会出错。

更正：

```
1 public static void main(String[] args) {
2         int x = 1234;
3         int y = 5678;
4         y = (x ^= (y^= x)) ^ y;
5         System.out.println(" x = " + x + " , y =" + y);
6     }
```

No 5 ：随机数的问题

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 public static void main(String[] args) {
 2         Random random = new Random();
 3         StringBuffer sb = null;
 4         switch (random.nextInt(3)) {
 5         case 1:
 6             sb = new StringBuffer('P');
 7             break;
 8         case 2:
 9             sb = new StringBuffer('M');
10             break;
11         default:
12             sb = new StringBuffer('G');
13             break;
14         }
15         
16         sb.append('a');
17         sb.append('i');
18         sb.append('n');
19         System.out.println(sb.toString());
20     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

你觉得会打印出什么？ 很多人会说是 "Pain" 或者 "Main" 或者 "Gain" 。

其实都错了  答案只有一个："ain"

其实主要的原因来自StringBuffer的构成方法 当你使用 new StringBuffer('G')是'G'会解释成int 型  所以调用的构造方法是：

`**[StringBuffer](http://www.cnblogs.com/yixiwenwen/admin/)**(int capacity) 构造一个不带字符，但具有指定初始容量的字符串缓冲区。`

而不是

`**[StringBuffer](http://www.cnblogs.com/yixiwenwen/admin/)**([String](http://www.cnblogs.com/yixiwenwen/admin/) str) 构造一个字符串缓冲区，并将其内容初始化为指定的字符串内容`

所以改正的方法不言而喻了。

NO.6  无情的增量操作

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 public static void main(String[] args) {
2         int j = 0 ;
3         for (int i = 0; i < 100 ; i++) {
4             j = j++;
5         }
6         System.out.println(j);
7         
8     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

这个题目你一个不注意就会喊出 输出100 ；

其实答案是 0  这里的 j = j++; 从第一次赋值开始 j 就是被赋给了0 因为j++是后缀++ 赋值的时候还没有增值。

NO.7  整数边界的问题

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 public static final int MAX = Integer.MAX_VALUE;
 2     public static final int START = MAX - 100;
 3     public static void main(String[] args) {
 4         int j = 0 ;
 5         for (int i = START; i <= MAX ; i++) {
 6             j++;
 7         }
 8         System.out.println(j);
 9         
10     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



你可能会很快的回答100 然后细心地看了下 说 101 

其实答案都不是  程序在无限循环着 

这是因为Integer.MAX_VALUE + 1 = Integer.MIN_VALUE 所以在for循环中 i=MAX 后 i++ 这时的 i 变成Integer.MIN_VALUE 了

 依然满足条件

要循环结束 在for中的 i <= MAX 变成 i < MAX 即可。



 NO.8  计数器的问题

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 public static void main(String[] args) {
2         int minutes = 0 ;
3         for (int ms = 0; ms < 60 * 60 * 1000; ms++) {
4             if(ms % 60*1000 == 0) {
5                 minutes ++ ;
6             }
7         }
8         System.out.println(minutes);
9     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

 你有可能一下子就说出了是 输出 60 因为能整除 60 * 1000 的只能是 1 * 60 * 1000 ，2 * 60 * 1000 ....一直到 60 * 60 * 1000 一共60 个 

但是 答案让你失望了  是60000

其实 这个的陷阱是在 ms % 60*1000 == 0 的操作符的优先级上  % 和 * 是同一级的 所以其实他是(ms % 60) *1000 == 0 这样的 满足ms % 60 这个就多了



NO.9  优柔寡断的返回值

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1     public static void main(String[] args) {
 2         System.out.println(decisor());
 3     }
 4     
 5     public static boolean decisor(){
 6         try {
 7             return true;
 8         } finally {
 9             return false;
10         }
11     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

你觉得会返回的是什么？

答案是 false；

那你再看一个：

NO.10  你好，再见

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1     public static void main(String[] args) {
2         try {
3             System.out.println("Hello world");
4             System.exit(0);
5         } finally{
6             System.out.println("GoodBye world");
7         }
8     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

你又会觉得会返回的是什么？

答案是 仅仅只有 Hello world

NO.11  打开Javac进程



```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 public static void main(String[] args) {
 2         try {
 3             Runtime runtime = Runtime.getRuntime();
 4             Process proc = runtime.exec("C:\\Program Files\\Internet Explorer\\iexplore.exe");
 5             int exitVal = proc.exitValue();
 6             System.out.println("process exitVal :" + exitVal);
 7         } catch (IOException e) {
 8             e.printStackTrace();
 9         }
10     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



运行时你会发现IE启动了  但是抛异常了

```
1 Exception in thread "main" java.lang.IllegalThreadStateException: process has not exited
2     at java.lang.ProcessImpl.exitValue(Native Method)
3     at Test.main(Test.java:9)
```

这个异常的原因是在我们打开IE的时候就执行了proc.exitValue()即返回退出时的值。可是我们还没有退出IE啊 所以才会报异常

解决方法：给成
int exitVal = proc.waitFor();
这样看起来没什么啊，是啊 很好理解 那你再看一个

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 public static void main(String[] args) {
 2         try{            
 3             Runtime rt = Runtime.getRuntime();
 4             Process proc = rt.exec("javac");
 5             int exitVal = proc.waitFor();
 6             System.out.println("Process exitValue: " + exitVal);
 7         } catch (Throwable t){
 8             t.printStackTrace();
 9         }
10     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

你运行的时候会发现程序会死在那里： 这是因为javac会一直等待它要执行的东西 所以一直在等待：

改正：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 public static void main(String[] args) {
 2         try{            
 3             Runtime rt = Runtime.getRuntime();
 4             Process proc = rt.exec("javac");
 5             InputStream stderr = proc.getErrorStream();
 6             InputStreamReader isr = new InputStreamReader(stderr);
 7             BufferedReader br = new BufferedReader(isr);
 8             String line = null;
 9             System.out.println("<ERROR>");
10             while ( (line = br.readLine()) != null)
11                 System.out.println(line);
12             System.out.println("</ERROR>");
13             int exitVal = proc.waitFor();
14             System.out.println("Process exitValue: " + exitVal);
15         } catch (Throwable t){
16             t.printStackTrace();
17         }
18     }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

运行结果：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

<ERROR>
用法：javac <选项> <源文件>
其中，可能的选项包括：
  -g                         生成所有调试信息
  -g:none                    不生成任何调试信息
  -g:{lines,vars,source}     只生成某些调试信息
  -nowarn                    不生成任何警告
  -verbose                   输出有关编译器正在执行的操作的消息
  -deprecation               输出使用已过时的 API 的源位置
  -classpath <路径>            指定查找用户类文件和注释处理程序的位置
  -cp <路径>                   指定查找用户类文件和注释处理程序的位置
  -sourcepath <路径>           指定查找输入源文件的位置
  -bootclasspath <路径>        覆盖引导类文件的位置
  -extdirs <目录>              覆盖安装的扩展目录的位置
  -endorseddirs <目录>         覆盖签名的标准路径的位置
  -proc:{none,only}          控制是否执行注释处理和/或编译。
  -processor <class1>[,<class2>,<class3>...]要运行的注释处理程序的名称；绕过默认的搜索进程
  -processorpath <路径>        指定查找注释处理程序的位置
  -d <目录>                    指定存放生成的类文件的位置
  -s <目录>                    指定存放生成的源文件的位置
  -implicit:{none,class}     指定是否为隐式引用文件生成类文件 
  -encoding <编码>             指定源文件使用的字符编码
  -source <版本>               提供与指定版本的源兼容性
  -target <版本>               生成特定 VM 版本的类文件
  -version                   版本信息
  -help                      输出标准选项的提要
  -Akey[=value]              传递给注释处理程序的选项
  -X                         输出非标准选项的提要
  -J<标志>                     直接将 <标志> 传递给运行时系统

</ERROR>
Process exitValue: 2

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

NO.12  日志粒度的控制

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 public class BadLogger {
 2     private Logger m_log = null; 
 3 
 4     public BadLogger(Level l){
 5         ConsoleHandler ch = new ConsoleHandler();
 6         m_log = Logger.getLogger("no2.BadLogger.logger");
 7         ch.setLevel(l);
 8     }
 9 
10     public void test(){                          
11         System.out.println("The level for the log is: "+ m_log.getLevel());  
12         m_log.finest("This is a test for finest");
13         m_log.finer("This is a test for finer");
14         m_log.fine("This is a test for fine");
15         m_log.info("This is a test for info");
16         m_log.warning("This is a warning test");
17         m_log.severe("This is a severe test");
18     }
19 
20     public static void main(String[] args){
21         Level loglevel = Level.INFO;
22         if ( args.length !=0 ){
23             if ( args[0].equals("ALL") ){
24                 loglevel = Level.ALL;
25             }
26             else if ( args[0].equals("FINE") ){
27                 loglevel = Level.FINE;
28             }
29             else if ( args[0].equals("FINEST") ){
30                 loglevel = Level.FINEST;
31             }
32             else if ( args[0].equals("WARNING") ){
33                 loglevel = Level.WARNING;
34             }
35             else if ( args[0].equals("SEVERE") ) {
36                 loglevel = Level.SEVERE;
37             }
38         }
39         BadLogger logex = new BadLogger(loglevel);
40         logex.test(); 
41     }
42 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

这是错误的写法  你会发现不管你传什么参数结果都是：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 The level for the log is: null
2 2012-10-24 19:21:26 no2.BadLogger test
3 信息: This is a test for info
4 2012-10-24 19:21:26 no2.BadLogger test
5 警告: This is a warning test
6 2012-10-24 19:21:26 no2.BadLogger test
7 严重: This is a severe test

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

改正：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 public class BadLogger {
 2     private Logger m_log = null; 
 3 
 4     public BadLogger(Level l){
 5         ConsoleHandler ch = new ConsoleHandler();
 6         m_log = Logger.getLogger("no2.BadLogger.logger");
 7         m_log.addHandler(ch);
 8         m_log.setLevel(l); 
 9         m_log.setUseParentHandlers(false);
10         ch.setLevel(l);
11     }
12 
13     public void test(){                          
14         System.out.println("The level for the log is: "+ m_log.getLevel());  
15         m_log.finest("This is a test for finest");
16         m_log.finer("This is a test for finer");
17         m_log.fine("This is a test for fine");
18         m_log.info("This is a test for info");
19         m_log.warning("This is a warning test");
20         m_log.severe("This is a severe test");
21     }
22 
23     public static void main(String[] args){
24         Level loglevel = Level.INFO;
25         if ( args.length !=0 ){
26             if ( args[0].equals("ALL") ){
27                 loglevel = Level.ALL;
28             }
29             else if ( args[0].equals("FINE") ){
30                 loglevel = Level.FINE;
31             }
32             else if ( args[0].equals("FINEST") ){
33                 loglevel = Level.FINEST;
34             }
35             else if ( args[0].equals("WARNING") ){
36                 loglevel = Level.WARNING;
37             }
38             else if ( args[0].equals("SEVERE") ) {
39                 loglevel = Level.SEVERE;
40             }
41         }
42         BadLogger logex = new BadLogger(loglevel);
43         logex.test(); 
44     }
45 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

这个时候输入ALL这个参数结果就为：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 The level for the log is: ALL
 2 2012-10-24 19:23:31 no2.BadLogger test
 3 最好: This is a test for finest
 4 2012-10-24 19:23:31 no2.BadLogger test
 5 较好: This is a test for finer
 6 2012-10-24 19:23:31 no2.BadLogger test
 7 良好: This is a test for fine
 8 2012-10-24 19:23:31 no2.BadLogger test
 9 信息: This is a test for info
10 2012-10-24 19:23:31 no2.BadLogger test
11 警告: This is a warning test
12 2012-10-24 19:23:31 no2.BadLogger test
13 严重: This is a severe test

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

 NO.13  遍历容器的一些陷阱

three哪去了？

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

public class BadVisitor {
    public static void main(String[] args) {
        Vector v = new Vector();
        v.add("one");
        v.add("two");
        v.add("three"); 
        v.add("four");
        Enumeration enume = v.elements();
        while (enume.hasMoreElements()){
            String s = (String) enume.nextElement();
            if (s.equals("two"))
                v.remove("two");
            else{
                System.out.println(s);
            }
        }
        System.out.println("What's really there...");
        enume = v.elements();
        while (enume.hasMoreElements()){
            String s = (String) enume.nextElement();
            System.out.println(s);            
        }
    }
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

运行结果：

```
1 one
2 four
3 What's really there...
4 one
5 three
6 four
```

改正：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 public class GoodVisitor {
 2     public static void main(String[] args) {
 3         Vector v = new Vector();
 4         v.add("one"); v.add("two"); v.add("three"); v.add("four");
 5         Iterator iter = v.iterator();
 6         while (iter.hasNext()){
 7             String s = (String) iter.next();
 8             if (s.equals("two"))
 9                 iter.remove();
10             else{
11                 System.out.println(s);
12             }
13         }
14         System.out.println("What's really there...");
15         iter = v.iterator();
16         while (iter.hasNext()){
17             String s = (String) iter.next();
18             System.out.println(s);            
19         }
20     }
21 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)

转自地址：http://www.cnblogs.com/yixiwenwen/archive/2012/10/24/2737473.html
```



