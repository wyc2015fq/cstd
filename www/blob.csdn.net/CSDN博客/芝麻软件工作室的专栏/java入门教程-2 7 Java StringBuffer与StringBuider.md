
# java入门教程-2.7 Java StringBuffer与StringBuider -  芝麻软件工作室的专栏 - CSDN博客


2016年05月24日 09:31:09[seven-soft](https://me.csdn.net/softn)阅读数：225个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



String 的值是不可变的，每次对String的操作都会生成新的String对象，不仅效率低，而且耗费大量内存空间。
StringBuffer类和String类一样，也用来表示字符串，但是StringBuffer的内部实现方式和String不同，在进行字符串处理时，不生成新的对象，在内存使用上要优于String。
StringBuffer 默认分配16字节长度的缓冲区，当字符串超过该大小时，会自动增加缓冲区长度，而不是生成新的对象。
StringBuffer不像String，只能通过 new 来创建对象，不支持简写方式，例如：StringBufferstr1=newStringBuffer();// 分配16个字节长度的缓冲区
StringBufferstr2==newStringBuffer(512);// 分配512个字节长度的缓冲区
// 在缓冲区中存放了字符串，并在后面预留了16个字节长度的空缓冲区
StringBufferstr3=newStringBuffer("www.weixueyuan.net");

## StringBuffer类的主要方法
StringBuffer类中的方法主要偏重于对于字符串的操作，例如追加、插入和删除等，这个也是StringBuffer类和String类的主要区别。实际开发中，如果需要对一个字符串进行频繁的修改，建议使用 StringBuffer。
#### 1) append() 方法
append() 方法用于向当前字符串的末尾追加内容，类似于字符串的连接。调用该方法以后，StringBuffer对象的内容也发生改变，例如：StringBufferstr=newStringBuffer(“biancheng100”);
str.append(true);

则对象str的值将变成”biancheng100true”。注意是str指向的内容变了，不是str的指向变了。
字符串的”+“操作实际上也是先创建一个StringBuffer对象，然后调用append()方法将字符串片段拼接起来，最后调用toString()方法转换为字符串。
这样看来，String的连接操作就比StringBuffer多出了一些附加操作，效率上必然会打折扣。
但是，对于长度较小的字符串，”+“操作更加直观，更具可读性，有些时候可以稍微牺牲一下效率。
#### 2)  deleteCharAt()
deleteCharAt() 方法用来删除指定位置的字符，并将剩余的字符形成新的字符串。例如：StringBufferstr=newStringBuffer("abcdef");
str.deleteCharAt(3);

该代码将会删除索引值为3的字符，即”d“字符。
你也可以通过delete()方法一次性删除多个字符，例如：StringBufferstr=newStringBuffer("abcdef");
str.delete(1,4);

该代码会删除索引值为1~4之间的字符，包括索引值1，但不包括4。
#### 3) insert() 方法
insert() 用来在指定位置插入字符串，可以认为是append()的升级版。例如：StringBufferstr=newStringBuffer("abcdef");
str.insert(3,"xyz");

最后str所指向的字符串为 abcdxyzef。
#### 4) setCharAt() 方法
setCharAt() 方法用来修改指定位置的字符。例如：StringBufferstr=newStringBuffer("abcdef");
str.setCharAt(3,'z');

该代码将把索引值为3的字符修改为 z，最后str所指向的字符串为 abczef。
以上仅仅是部分常用方法的简单说明，更多方法和解释请查阅API文档。
## String和StringBuffer的效率对比
为了更加明显地看出它们的执行效率，下面的代码，将26个英文字母加了10000次。publicclassDemo{
publicstaticvoidmain(String[]args){
Stringfragment="abcdefghijklmnopqrstuvwxyz";
inttimes=10000;
// 通过String对象
longtimeStart1=System.currentTimeMillis();
Stringstr1="";
for(inti=0;i<times;i++){
str1+=fragment;
}
longtimeEnd1=System.currentTimeMillis();
System.out.println("String: "+(timeEnd1-timeStart1)+"ms");
// 通过StringBuffer
longtimeStart2=System.currentTimeMillis();
StringBufferstr2=newStringBuffer();
for(inti=0;i<times;i++){
str2.append(fragment);
}
longtimeEnd2=System.currentTimeMillis();
System.out.println("StringBuffer: "+(timeEnd2-timeStart2)+"ms");
}
}

运行结果：
String: 5287ms
StringBuffer: 3ms
结论很明显，StringBuffer的执行效率比String快上千倍，这个差异随着叠加次数的增加越来越明显，当叠加次数达到30000次的时候，运行结果为：
String: 35923ms
StringBuffer: 8ms
所以，强烈建议在涉及大量字符串操作时使用StringBuffer。
## StringBuilder类
StringBuilder类和StringBuffer类功能基本相似，方法也差不多，主要区别在于StringBuffer类的方法是多线程安全的，而StringBuilder不是线程安全的，相比而言，StringBuilder类会略微快一点。
StringBuffer、StringBuilder、String中都实现了CharSequence接口。
CharSequence是一个定义字符串操作的接口，它只包括length()、charAt(int index)、subSequence(int start, int end) 这几个API。
StringBuffer、StringBuilder、String对CharSequence接口的实现过程不一样，如下图所示：![](http://www.weixueyuan.net/uploads/allimg/141129/1-141129111JTX.png)
图1  对CharSequence接口的实现
可见，String直接实现了CharSequence接口；StringBuilder 和 StringBuffer都是可变的字符序列，它们都继承于AbstractStringBuilder，实现了CharSequence接口。
## 总结
线程安全：StringBuffer：线程安全
StringBuilder：线程不安全
速度：
一般情况下，速度从快到慢为 StringBuilder > StringBuffer > String，当然这是相对的，不是绝对的。
使用环境：操作少量的数据使用 String；
单线程操作大量数据使用 StringBuilder；
多线程操作大量数据使用 StringBuffer。

