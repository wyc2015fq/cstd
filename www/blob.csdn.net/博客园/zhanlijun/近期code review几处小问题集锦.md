# 近期code review几处小问题集锦 - zhanlijun - 博客园
# [近期code review几处小问题集锦](https://www.cnblogs.com/LBSer/p/5059790.html)
# 1 线程池使用不当
     我们的调度系统需要将一堆会员分配给相应的人员来处理，流程如以下伪代码所示：
```java;gutter
public void dispatch() {
        while (true) {
            List<Member> memberList = getUnassignedMemberList(); //获取所有未分配的会员
            for(Member each : memberList) { 
                singleDispatch(each);  //为每一个会员分配相应的人员处理
            }
            try {
                Thread.sleep(1000); //休眠1秒后继续分配
            } catch (InterruptedException e) {
            }
        }
    }
```
    为了提高分配的速度，我们打算采用多线程的分配方式。一开始使用的是newCachedThreadPool。
```java;gutter
private static  ExecutorService executor = Executors.newCachedThreadPool();
    public void dispatch() {
        while (true) {
            List<Member> memberList = getUnassignedMemberList(); //获取所有未分配的会员
            for(final Member each : memberList) {
                executor.submit(new Runnable() {
                    @Override
                    public void run() {
                        singleDispatch(each);  //为每一个会员分配相应的人员
                    }
                });
            }
            try {
                Thread.sleep(1000); //休眠1秒后继续分配
            } catch (InterruptedException e) {
            }
        }
    }
```
    在压测时发现，load飙升得很高，通过抓栈发现开启了很多线程。原因是：newCachedThreadPool最大线程数为整型的最大值，每提交一个任务，如果没有线程处理，那就产生一个新的线程。当我们for循环提交任务时，开辟了上百个线程，应用程序马上崩溃。
    既然发现了原因，我们马上将调整线程池为newFixedThreadPool，这里我们可以设置最大线程数为4，队列长度为整型的最大值。
```java;gutter
private static ExecutorService executor = Executors.newFixedThreadPool(4);
```
    但是压测又发现新问题，线程池里的队列长度不断增长，而且分配不断有异常抛出（异常信息为会员已经被分配过）。
    原因是：当未分配会员较多时，可能需要5秒才能分配完，然而executor.submit是异步操作，当休眠1秒钟后，马上又进入下一个循环，队列里又将插入重复的会员，这会导致队列长度不断增长，此外，会导致1个会员被分配后，又继续被分配，导致异常产生。
    解决方法：使用invokeAll这一同步语句，意思是只有当提交任务都被执行完后，才执行后续语句。
```java;gutter
public void dispatch() {
        while (true) {
            List<Callable<String>> tasks = new ArrayList<Callable<String>>();
            List<Member> memberList = getUnassignedMemberList(); //获取所有未分配的会员
            for (final Member each : memberList) {
                tasks.add(new Callable<String>() {
                    @Override
                    public String call() {
                        singleDispatch(each);
                        return "ok";
                    }
                });
            }
            try {
                executor.invokeAll(tasks, 480, TimeUnit.SECONDS); //如果8分钟还未执行完，则超时重新再来（鲁棒性保证）
            }catch (Exception e) {
            }
            try {
                Thread.sleep(1000); //休眠1秒后继续分配
            } catch (InterruptedException e) {
            }
        }
    }
```
# 2 NPE（java.lang.NullPointerException）
## 2.1 情形1
```java;gutter
if(case.getType() == Case.TYPE_SELF) {
            ...
}
```
    这段代码抛出NPE时，直觉认为case为null导致的，后来打日志发现case并不为null，而case.getType()返回值类型为Integer，为null。
    最后发现Case.TYPE_SELF返回的是int类型，而case.getType()是个null，null与int两者一比较就报NPE。
    这个问题的诡异之处我们直觉上认为Case.TYPE_SELF是个Integer，所以导致排查问题花费了些时间，因此一个建议这种常量如Case.TYPE_SELF都改为Integer，然后对象间比较的时候使用equals，并增加null判断，就可以避免出现问题。
```java;gutter
if(null != case.getType() && case.getType().equals(Case.TYPE_SELF))
```
## 2.2 情形2
```java;gutter
Integer leftNum = (null != leftNumMap && !leftNumMap.isEmpty()) ? leftNumMap.get(stat.getDepartmentId()) : 0;
```
    这个也抛NPE，我们排查了半个多小时，百思不得其解，后来查看class文件发现了原因。
    假设我们代码是：
```java;gutter
Map<String, Integer> leftNumMap = new HashMap<String , Integer>();
        Integer leftNum = (null != leftNumMap && !leftNumMap.isEmpty()) ? leftNumMap.get("test") : 0;
```
    反编译后的代码如下：
```java;gutter
HashMap leftNumMap = new HashMap();
        Integer leftNum = Integer.valueOf(null != leftNumMap && !leftNumMap.isEmpty()?((Integer)leftNumMap.get("test")).intValue():0);
```
     为什么会这样呢？《[你真的会用 Java 中的三目运算符吗？](http://blog.jobbole.com/93511/)》一文做了说明。三目运算符的[语法规范](http://docs.oracle.com/javase/specs/jls/se7/html/jls-15.html#jls-15.25)是这样写的：If one of the second and third operands is of primitive type T, and the type of the other is the result of applying boxing conversion (§5.1.7) to T, then the type of the conditional expression is T.
     简单的来说就是：当第二，第三位操作数一个为基本类型一个为对象时，其中的对象就会拆箱为基本类型进行操作。
     所以，结果就是：由于使用了三目运算符，并且第二、第三位操作数一个是基本类型一个是对象。所以对对象进行拆箱操作，由于该对象为null，所以在拆箱过程中调用null.intValue()的时候就报了NPE。
     解决方法很简单：1）要么不用三目运算符，直接使用if else，简单可靠；2）或者将三目运算符操作数都改为对象。
```java;gutter
Integer leftNum = (null != leftNumMap && !leftNumMap.isEmpty()) ? leftNumMap.get(stat.getDepartmentId()) : Integer.valueOf(0);
```
# 3 Map<K, List<V>>用错
     我们代码中经常出现以下这种数据结构，比如key是类目，value是个List，list存储着这个类目下各个子类目的统计数据等。
```java;gutter
Map<String,List<MyClass>> myClassListMap = new HashMap<String,List<MyClass>>()
```
    我们插入该数据结构的时候往往得这样写：
```java;gutter
void putMyObject(String key, Object value) {
    List<Object> myClassList = myClassListMap.get(key);
    if(myClassList == null) {
        myClassList = new ArrayList<object>();
        myClassListMap.put(key,myClassList);
    }
    myClassList.add(value);
}
```
    当我们希望检查List中的对象是否存在，或删除一个对象，那要遍历整个数据结构，需要更多的代码。
    这些代码不仅给可读性带来障碍，更提高了大家出错的概率，比如某一次我们在复杂的业务逻辑中实现putMyObject时，漏写了句:
```java;gutter
myClassListMap.put(key,myClassList);
```
    结果导致花费了大量时间才发现这个问题。
    当然，细致的测试以及code review能避免出现类似问题，但是有没有一种方法既能从技术上来帮助我们避免此类问题，又能节约代码提高代码可读性？
    推荐大家使用google guava包，里面提供了MultiMap数据结构，使用方式如下，非常简洁明了，也不会有机会让我们出错。
```java;gutter
Multimap<String, String> myMultimap = ArrayListMultimap.create();
        myMultimap.put("女装", "内衣");
        myMultimap.put("女装", "羽绒服");
        myMultimap.put("女装", "风衣");
        myMultimap.put("男装", "皮夹克");
        // 获取key "女装"对应的list
        Collection<String> womenDressList = myMultimap.get("女装");
        // 删除key "女装"对应List中的"羽绒服"
        myMultimap.remove("女装", "羽绒服");
```
