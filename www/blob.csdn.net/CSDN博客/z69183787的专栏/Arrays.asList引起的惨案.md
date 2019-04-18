# Arrays.asList引起的惨案 - z69183787的专栏 - CSDN博客
2016年07月05日 22:50:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1176
最近代码中需要对两个数组求交，想当然便用到了List中的retainAll函数，但要将将数组转换成list。代码如下：
String[] abc = new String[] { "abc", "acd", "add" };
String[] abd = new String[] {"acd", "cd", "de"};
List<String> abcList = Arrays.asList(abc);
List<String> abdList = Arrays.asList(abd);
abcList.retainAll(abdList);
没想到执行后却抛出了异常：
java.lang.UnsupportedOperationException
at java.util.AbstractList.remove(AbstractList.java:144)
at java.util.AbstractList$Itr.remove(AbstractList.java:360)
at java.util.AbstractCollection.retainAll(AbstractCollection.java:370)
反编译后才发现原来 Arrays.asList返回的ArrayList并不是java.util.ArrayList，而是在Arrays类中重新定义的一下内部类ArrayList：
private static class ArrayList<E> extends AbstractList<E>
implements RandomAccess, java.io.Serializable
{
private static final long serialVersionUID = -2764017481108945198L;
private final E[] a;
ArrayList(E[] array) {
if (array==null)
throw new NullPointerException();
a = array;
｝
//.....其它的省略
}
这个ArrayList并没有重写remove方法。
修改成如下，问题解决：
String[] abc = new String[] { "abc", "acd", "add" };
String[] abd = new String[] {"acd", "cd", "de"};
List<String> abcList = new ArrayList<String>(Arrays.asList(abc));
List<String> abdList = new ArrayList<String>(Arrays.asList(abd));
abcList.retainAll(abdList);
