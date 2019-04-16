# Java学习笔记44：常用集合使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年06月07日 10:41:07[initphp](https://me.csdn.net/initphp)阅读数：1316
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









```java
package com.test.www;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;
import java.util.Set;
import java.util.SortedSet;
import java.util.TreeSet;

public class Test {

  
    public static void main(String[] args) {
        //链表
        List<String> userList = new LinkedList<String>();
        userList.add("user1");
        userList.add("user2");
        userList.add("user3");
        userList.add("user4");
        userList.add("user5");
        userList.add("user6");
        ListIterator<String> i = userList.listIterator();
        while(i.hasNext()) {
            System.out.println(i.next());
        }
        
        //ArrayList
        ArrayList<String> arrayList = new ArrayList<String>();
        arrayList.add("Hello World");
        arrayList.add("sadoasodoasd");
        arrayList.add("111111111");
        System.out.println(arrayList.get(0));
        System.out.println(arrayList.get(1));
        System.out.println(arrayList.get(2));
        
        //HashSet 无序
        Set<String> set = new HashSet<String>();
        set.add("asdasd");
        set.add("asdasd1");
        set.add("asdasd2");
        set.add("asdasd3");
        Iterator it = set.iterator();
        while(it.hasNext()){
         System.out.println((String)it.next());
        }
        
        //TreeSet 有序
        SortedSet<String> ssetSet = new TreeSet<String>();
        ssetSet.add("100");
        ssetSet.add("200");
        ssetSet.add("300");
        for (String string : ssetSet) {
            System.out.println(string);
        }
        
        //HashMap
        HashMap<String, String> hashMap = new HashMap<String, String>();
        hashMap.put("user0", "asdadasda");
        hashMap.put("user1", "asdsad11111");
        hashMap.put("user2", "11232130000");
        System.out.println(hashMap.get("user0"));
        System.out.println(hashMap.get("user1"));
        System.out.println(hashMap.get("user2"));
    }
    
    
}
```




