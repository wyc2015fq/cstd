# Map 遍历的两种方式 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年01月21日 15:01:14[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：70


首先 说明一下： List 接口 有 iterator()和 listiterator()方法，Set 接口只有 iterator() 方法. Map 既没有iterator() 也没有 listiterator().  但Map 为我们提供了 KeySet() 获得建的值放入Set 集合中 以及 get(Obect key) 通过key 的值 获取 对应的value 值，通过这两个方法我们就 可以
 遍历 map 集合。  除此之外，还提供了map.entrySet() 方法，  将键值对  放入 Set<Map.Entry<K, V>>  中， Map.Entry 是Map 中 的 一个 内部 接口， 为我们 提供了， getKey() 和 getValue() 两个方法，可以分别获得 建和 值 。 

方法一：

通过KeySet方法 获得 Map 集合中的 key 值，然后 遍历key 通过 map.get（Object key）获得对应的value 值。

Set<String> keySet = map.keySet();
Iterator<String> keyIterator = keySet.iterator();

while(keyIterator.hasNext()){

Object keyObject = keyIterator.next();
Student student = (Student) map.get(keyObject);

System.out.println(keyObject+":"+ student);
}



方法二：

Set<Map.Entry<String,Student>>  EntrySet = map.entrySet();
Iterator<Map.Entry<String,Student>> entryIterator = EntrySet.iterator();

            while(entryIterator.hasNext()){

Entry<String, Student> entry =  (Entry<String, Student>) entryIterator.next();

System.out.println(entry.getKey()+":"+ entry.getValue());
}



