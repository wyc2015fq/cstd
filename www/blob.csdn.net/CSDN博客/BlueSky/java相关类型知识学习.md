# java相关类型知识学习 - BlueSky - CSDN博客
2015年12月30日 14:14:32[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：358
## set<string
Set是集合的意思，是同种对象的集合。
<String>说明这种对象都是String类型的对象。
Set本身是接口，所以需要实现，可以这样定义：
Set<String> set = new HashSet<String>();
String [s1](https://www.baidu.com/s?wd=s1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dWrjRzmyFhnvnzPWKBmywW0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHRsPWbvrHc3) = "hello";
String s2 = "world";
set.add([s1](https://www.baidu.com/s?wd=s1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dWrjRzmyFhnvnzPWKBmywW0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHRsPWbvrHc3));
set.add(s2);
这样就添加了两个元素。
HashSet是无序的，而TreeSet是有序的，[s1](https://www.baidu.com/s?wd=s1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1dWrjRzmyFhnvnzPWKBmywW0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnHRsPWbvrHc3)和s2添加后会有按照比较的大小排序
（前提String可以比较大小……）
## iterator(迭代器)
迭代器（Iterator）
　　迭代器是一种设计模式，它是一个对象，它可以遍历并选择序列中的对象，而开发人员不需要了解该序列的底层结构。迭代器通常被称为“轻量级”对象，因为创建它的代价小。
　　Java中的Iterator功能比较简单，并且只能单向移动：
　　(1) 使用方法iterator()要求容器返回一个Iterator。第一次调用Iterator的next()方法时，它返回序列的第一个元素。注意：iterator()方法是java.lang.Iterable接口,被Collection继承。
　　(2) 使用next()获得序列中的下一个元素。
　　(3) 使用hasNext()检查序列中是否还有元素。
　　(4) 使用remove()将迭代器新返回的元素删除。
　　Iterator是Java迭代器最简单的实现，为List设计的ListIterator具有更多的功能，它可以从两个方向遍历List，也可以从List中插入和删除元素。
好多类都实现了Iterable接口，这样对象就可以调用iterator()方法。 
一般都是结合着用，比如 
HashMap类就实现了Iterable接口，而要访问或打印出Map中所有内容时，就可以这样： HashMap hashMap; 
Iterator iter = hashMap.iterator(); 
while(iter.hashNext()) { 
String s = iter.next(); 
} 
迭代器应用：
 list l = new ArrayList();
 l.add("aa");
 l.add("bb");
 l.add("cc");
 for (Iterator iter = l.iterator(); iter.hasNext();) {
  String str = (String)iter.next();
  System.out.println(str);
 }
 /*迭代器用于while循环
 Iterator iter = l.iterator();
 while(iter.hasNext()){
  String str = (String) iter.next();
  System.out.println(str);
 }
 */
