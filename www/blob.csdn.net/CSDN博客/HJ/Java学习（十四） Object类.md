# Java学习（十四） Object类 - HJ - CSDN博客
2018年03月18日 21:12:26[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：133
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
**目录**
**一、Object**
**二、String**
**三、StringBuffer**
**四、StringBuilder**
**五、System**
**六、Rutime**
**七、Calendar**
**八、Math**
**一、Object**
1.返回对象的描述信息
         toString()
2.返回比较对象的内存地址值的结果
         equals()
3.返回对象的哈希码
         hashCode()
**二、String**
创建String类的对象的两种方式：
         1.””直接赋值法
         2.new关键字法
1.字符串对象的比较
        （1）String str = “jack”
                这个语句会先检查字符串常量池是否存放这个”jack”字符串对象，如果没有存在，就会在字符串常量池中创建这个   字符串，如果存在就直接返回该字符串的内存地址值；
        （2）String str1 = newString(“jack”)
              这个语句会创建两个对象，首先会先检查字符串常量池中是否存在这个”jack”字符串对象，如果不存在就会创建，如果存在就返回内存地址值。创建出来之后，new String这个语句就会在堆内存中开辟一个字符串对象。总共两个对象；
2.获取方法
         （1）获取字符串的长度
                  intlength()
         （2）获取特定位置的字符
                  charcharAt(int index)
         （3）获取特定字符的位置
                  intindexOf(String str)
         （4）获取最后一个字符的位置
                  intlastIndexOf(int ch)
3.判断方法
         （1）是否以指定字符结束
                  booleanendsWith(String str)
         （2）是否长度为0
                  booleanisEmpty()
         （3）是否包含指定序列
                  booleancontains(CharSequences)
         （4）是否相等
                  booleanequals(Object anObject)
         （5）忽略大小写是否相等
                  booleanequalsIgnoreCase(String anotherString)
4.转换方法
         （1）将字符数组转换为字符串
                  String(char[]value)
         （2）将字符串转换为字符数组
                  char[]toCharArray()
5.其他方法
       （1）替换
               String replace(charoldChar,char newChar)
       （2）切割
                  String[]split(String regex)
       （3）截取子串
               String substring(intbeginIndex)
               Stringsubstring(int beginIndex,int endIndex)
       （4）转大写
               StringtoUpperCase()
       （5）转小写
               StringtoLowerCase()
       （6）去字符串两边空格
               String trim()
**三、StringBuffer**
由于String是不可变的，所以导致String对象泛滥，在频繁改变字符串对象的应用中，需要使用可变的字符串缓冲区类。
特点：
         1.默认的缓冲区的容量是16，每次扩容增加一倍；
         2.线程安全的所有的缓冲区操作方法都是同步的，效率很低；
1.添加
（1）在创建对象的时候赋值
                  StringBuffer(“jack”)
（2）在缓冲区的尾部添加新的文本对象
                  append()
（3）在指定的下标位置添加新的文本对象
    insert()
2.查看
         （1）返回这个容器的字符串
                  toString()
         （2）返回第一次出现的指定子串在该字符串中的索引
                  indexOf(Stringstr)
         （3）从开始的位置开始截取字符串
                  substring(intstart)
3.修改
         （1）使用给定String中的字符串替换此序列的子串中的字符，该子串从指定的start处开始，一直到索引end-1处的字符
                  replace(intstart,int end,String str)
         （2）指定索引位置替换一个字符
                  setCharAt(intindex,char ch)
4.删除
         （1）清空缓冲区
                  delete(0,sb.length())
         （2）删除指定位置的一个字符
                  deleteChatAt(intindex)
5.反序
         把字符串反序输出
                  reverse()
**四、StringBuilder**
JDK1.5之后提出，线程不安全，但是效率高。用法与StringBuffer类似
**五、System**
1.获取系统的属性
         Propertiesps = System.getProperties();
2.输出系统属性
         ps.list(System.out);
3.获取操作系统名称
         Stringos_name = System.getProperty(“os.name);
         System.out.println(os_name);
4.检测操作系统是否支持该软件
         if(“WindowsXP”.equals(os_name)) {
         System.out.println(“继续安装”);
}else {
         System.out.println(“系统不兼容”);
}
5.获取path环境变量值
System.out.println(System.getenv(“path”));
6.返回以毫秒为单位的当前时间
static longcurrentTimeMillis()
7.退出JVM  0为正常退出
static void exit(intstatus)
**六、Runtime**
主要描述的是应用程序运行的环境
1.获取运行环境的对象
         Runtimerun = Runtime.getRuntime();
2.退出虚拟机
         run.exit()
3.获取可用内存数
         run.longfreeMemory()
4.调用垃圾回收器程序，但是调用该方法的时候不会马上就运行GC
         run.gc()
5.获取JVM最大内存容量
         run.longmaxMemory()
6.获取总内存
         run.longtotalMemory()
7.启动一个字符串命令的进程
         Processexec(String command)
**七、Calendar**
1.构造一个Calendar类
         Calendarcalendar = Calendar.getInstance();
2.获取年、月、日、时、分、秒
         int  year = calendar.get(Calendar.YEAR);
         int  month = calendar.get(Calendar.MONTH) + 1;
         int  day = calendar.get(Calendar.DAY_OF_MONTH);
         int  dayofweek = calendar.get(Calendar.DAY_OF_WEEK);
         int  hour = calendar.get(Calendar.HOUR_OF_DAY);
         int  minute = calendar.get(Calendar.MINUTE);
         int  second = calendar.get(Calendar.SECOND);
3.日期化格式类：SimpleDateFormat
         SimpleDateFormatsm = new SimpleDateFormat(“yyyy年MM月dd日 E a hh时mm分ss秒”);
         System.out.println(sm.format(newDate()));
**八、Math**
1.返回大于等于指定小数的最小整数
        static double ceil(double a)     
2.返回小于等于指定小数的最大整数
        static double floor(double a)
3.四舍五入
        static long round(double)
4.返回大于等于0.0小于1.0的小数
        static double random()
