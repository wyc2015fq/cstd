# Map读取不影响修改删除的方式 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月07日 13:03:39[boonya](https://me.csdn.net/boonya)阅读数：180标签：[Java																[Map																[遍历																[修改](https://so.csdn.net/so/search/s.do?q=修改&t=blog)
个人分类：[Java](https://blog.csdn.net/boonya/article/category/878447)





Map操作边遍历边修改是不安全的方式，如何避免呢？只有通过其他结构来处理，将修改和读取分开就OK。


```java
package com.boonya.map;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public class MapTest {

    private Map<Integer, String> map = new HashMap<Integer, String>();

    public void Add(Integer key,String value)
    {
        map.put(key,value);
    }

    public void Remove(Integer key)
    {
        if(map.containsKey(key)){
            map.remove(key);
        }
    }

    public void ReadMapMethodOne(){
        Set<Integer> set= map.keySet();
        for (Integer in : set) {
            //map.keySet()返回的是所有key的值
            String str = map.get(in);//得到每个key多对用value的值
            System.out.println(in + "     " + str);
        }
    }

    public void ReadMapMethodTwo(){
        Iterator<Map.Entry<Integer, String>> it = map.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<Integer, String> entry = it.next();
            System.out.println("key= " + entry.getKey() + " and value= " + entry.getValue());
        }
    }

    public  void ReadMapMethodThree() {
        Set<Map.Entry<Integer,String>> entrySet=map.entrySet();
        for (Map.Entry<Integer, String> entry :entrySet) {
            //Map.entry<Integer,String> 映射项（键-值对）  有几个方法：用上面的名字entry
            //entry.getKey() ;entry.getValue(); entry.setValue();
            //map.entrySet()  返回此映射中包含的映射关系的 Set视图。
            System.out.println("key= " + entry.getKey() + " and value= " + entry.getValue());
        }
    }

    public  void ReadMapMethodFour() {
        for (String value:map.values()){
            System.out.println(" value= " +value);
        }
    }
}
```](https://so.csdn.net/so/search/s.do?q=遍历&t=blog)](https://so.csdn.net/so/search/s.do?q=Map&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)




