# Hashtable的遍历方法 - z69183787的专栏 - CSDN博客
2015年07月10日 17:57:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1162
今天遇到用hashtable取值时发现取得的值是排过序的，而我并希望它排序输出，因为它是倒序输出，查一下原因是因为我用下面的第一种方法取值的，将hashtable的值排序输出了，这个真是我疏忽的问题，原来hashtable还具有自动排序的功能.于是我从网上查了些资料，并整理记录下来：
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Iterator;
public class TestHashTable {
    static Hashtable<String, String> ht = new Hashtable<String, String>();
    public static void main(String[] args) { 
for(int i = 0;i<100;i++){
ht.put("key"+i, "value"+i);
}
for(Iterator itr = ht.keySet().iterator(); itr.hasNext();){
String key = (String) itr.next();
String value = (String) ht.get(key);
System.out.println(key+"--"+value);
}
Enumeration e1 = ht.elements();
while (e1.hasMoreElements()) {
System.out.println(e1.nextElement());
}
Enumeration e2 = ht.keys();
while (e2.hasMoreElements()) {
String key = (String) e2.nextElement();
          System.out.println(key +"---"+ht.get(key));
}
    }
}
如果要不按照他的hashcode排序建议用list ，Arraylist
