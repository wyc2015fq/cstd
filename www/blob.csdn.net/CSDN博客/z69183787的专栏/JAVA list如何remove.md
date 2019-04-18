# JAVA  list如何remove - z69183787的专栏 - CSDN博客
2013年02月06日 23:08:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：988
在java中对list进行操作很频繁,特别是进行list启遍历,这些操作我们都会，也很熟悉，但是对java中list进行删除元素，remove list中的元素就不怎么熟悉了吧，可以说很陌生，是实际操作中也很容易出错，先看看下面这个java中如何remove list 中的元素吧.
- 
- publicclasstest {
- publicstaticvoidmain(String[]
 args) {
- String str1 =newString("abcde");
- String str2 =newString("abcde");
- String str3 =newString("abcde");
- String str4 =newString("abcde");
- String str5 =newString("abcde");
- List list =newArrayList();
- 
- list.add(str1);
- list.add(str2);
- list.add(str3);
- list.add(str4);
- list.add(str5);
- 
- System.out.println("list.size()="+ list.size());
- for(inti
 =0; i < list.size(); i++) {
- if(((String) list.get(i)).startsWith("abcde"))
 {
- list.remove(i);
- }
- }
- System.out.println("after remove:list.size()="+
 list.size());
- }
- }
大家觉得这个程序打印出来的结果是多少呢？
Java代码
- 运行结果不是：
- 
- list.size()=5
- 
- after remove:list.size()=0
而是:
Java代码
- list.size()=5
- 
- after remove:list.size()=2
这是怎么回事呢？到底要如何remove list 中的元素呢？
原因：List每remove掉一个元素以后，后面的元素都会向前移动，此时如果执行i=i+1，则刚刚移过来的元素没有被读取。
**怎么解决？有三种方法可以解决这个问题：1.倒过来遍历list**
Java代码
- for(inti
 = list.size()-1; i > =0; i--)
 {
- if(((String) list.get(i)).startsWith("abcde"))
 {
- 　　 list.remove(i);
- 　　}
- }
**2.每移除一个元素以后再把i移回来**
**Java代码**
- for(inti
 =0; i < list.size(); i++) {
- if(((String) list.get(i)).startsWith("abcde"))
 {
- 　　 list.remove(i);
- 　　 i=i-1;
- 　　}
- }
3.使用iterator.remove()方法删除
**Java代码**
- for(Iterator it = list.iterator(); it.hasNext();) {
- 　　String str = (String)it.next();
- if(str.equals("chengang")){
- 　　 it.remove();
- 　　}
- }
