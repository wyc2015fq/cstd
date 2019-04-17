# HashMap的三种输出方式 - westbrook1998的博客 - CSDN博客





2018年09月08日 23:22:17[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：285








```
import java.util.*;

public class test {

    public static void main(String[] args) {
        HashMap<Integer,String> hashMap=new HashMap<>();
        hashMap.put(10,"a");
        hashMap.put(5,"c");
        hashMap.put(18,"b");
        //三种遍历方式
        //1. 使用keySet方法遍历key，再通过key获取value
        for(int key:hashMap.keySet()){
            System.out.println(key+" "+hashMap.get(key));
        }
        //2. 通过entrySet方法获取Map的内部类Entry的一个Set集合
        //   再通过iterator方法获取一个迭代器，再通过迭代器迭代输出
        Iterator iterator=hashMap.entrySet().iterator();
        while(iterator.hasNext()){
            Map.Entry<Integer,String> entry=(Map.Entry<Integer, String>)iterator.next();
            System.out.println(entry.getKey()+" "+entry.getValue());
        }
        //3. 同样通过entrySet，不过不用迭代器，直接foreach输出
        for(Map.Entry<Integer,String> entry:hashMap.entrySet()){
            System.out.println(entry.getKey()+" "+entry.getValue());
        }
    }
}
```



