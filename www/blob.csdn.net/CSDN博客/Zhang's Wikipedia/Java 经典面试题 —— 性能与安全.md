
# Java 经典面试题 —— 性能与安全 - Zhang's Wikipedia - CSDN博客


2018年05月30日 21:35:08[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：383



## 0. 安全
线程安全：fail-fast ⇒ java.util.concurrent（ArrayList是非线程安全的）
fail-fast 机制是 java 集合(Collection)中的一种错误机制。当多个线程对同一个集合的内容进行操作时，就可能会产生fail-fast事件。
例如：当某一个线程A通过 iterator 去遍历某集合的过程中，若该集合的内容被其他线程所改变了；那么线程A访问集合时，就会抛出 ConcurrentModificationException 异常，产生 fail-fast 事件。
privatestaticList<String>list=newArrayList<String>();privatestaticList<String>list=newCopyOnWriteArrayList<String>();
1. 性能
String、StringBuffer 与 StringBuilder
两个字符串相加，str1+str2，相当于执行：
StringBuilder strBuilder1 =newStringBuilder(str1);
strBuilder1.append(str2);Stringresult = strBuilder1.toString();执行到最后，我们所需要的内容只有result这一个对象，中间出现的strBuilder1 就成为了垃圾回收的目标。如果在一个循环中使用字符串的加号，导致的系统开销就是不可忽略的了。
常量字符串连接（”hello” + “world”），编译器会直接帮你连上，而不是到运行时再连。效率最高；
虽然 StringBuffer 是线程安全的，一般字符串相加不会有多线程操作，所以推荐使用 StringBuilder。
事实上我们在java里写的字符串相加的操作被编译后都是通过new一个StringBuffer或StringBuilder对象来操作的，所以不会产生大量的String对象
Map 的迭代方法：entrySet 还是 keySet
[Iterate through a HashMap [duplicate]](https://stackoverflow.com/questions/1066589/iterate-through-a-hashmap?noredirect=1&lq=1)
如果只关注 key， 不关心 value，keySet 为最佳
for(String key : map.keySet()) {
    //...}如果只关注 values，values() 最佳；
for(Object value : map.values()) {
    //...}如果即使用 key，又使用 value，entrySet() 为最佳；
for(Map.Entry<String, Object> entry : map.entrySet()) {
    String key = entry.getKey();
    Object value = entry.getValue();
    //...}

