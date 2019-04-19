# Java遍历HashMap并修改(remove) - 零度的博客专栏 - CSDN博客
2016年10月12日 11:18:25[零度anngle](https://me.csdn.net/zmx729618)阅读数：851
                
遍历HashMap的方法有多种，比如通过获取map的keySet, entrySet, iterator之后，都可以实现遍历，然而如果在遍历过程中对map进行读取之外的操作则需要注意使用的遍历方式和操作方法。
```
```
|12345678910111213141516171819202122232425|publicclassMapIteratorTest{privatestaticMap<Integer,String>map=newHashMap<Integer,String>();publicstaticvoidmain(String[]args){//initfor(inti=0;i<10;i++){map.put(i,"value"+i);}for(Map.Entry<Integer,String>entry:map.entrySet()){Integerkey=entry.getKey();if(key%2==0){System.out.println("To delete key "+key);map.remove(key);System.out.println("The key "++key+" was deleted");}}System.out.println("map size = "+map.size());for(Map.Entry<Integer,String>entry:map.entrySet()){System.out.println(entry.getKey()+" = "+entry.getValue());}}}|
上面代码的输出结果为
```
```
|1234567|Todelete key0The key0was deletedException inthread"main"java.util.ConcurrentModificationExceptionat java.util.HashMap$HashIterator.nextEntry(HashMap.java:793)at java.util.HashMap$EntryIterator.next(HashMap.java:834)at java.util.HashMap$EntryIterator.next(HashMap.java:832)at com.gpzuestc.collection.MapIteratorTest.main(MapIteratorTest.java:60)|
通过上面的输出可以发现第一个偶数key元素已经被成功remove，异常的抛出位置是在迭代器遍历下一个元素的时候。
如果把上面高亮的遍历代码替换成keySet的方式，通过keySet的remove操作同样会在遍历下个元素时抛出异常，示例如下。
```
```
|12345678|Set<Integer>keySet=map.keySet();for(Integerkey:keySet){if(key%2==0){System.out.println("To delete key "+key);keySet.remove(key);System.out.println("The key "++key+" was deleted");}}|
```
```
|123456|Todelete key0The key0was deletedException inthread"main"java.util.ConcurrentModificationExceptionat java.util.HashMap$HashIterator.nextEntry(HashMap.java:793)at java.util.HashMap$KeyIterator.next(HashMap.java:828)at com.gpzuestc.collection.MapIteratorTest.main(MapIteratorTest.java:49)|
如果要实现遍历过程中进行remove操作，上面两种方式都不能使用，而是需要通过显示获取keySet或entrySet的iterator来实现。
```
```
|1234567891011|Iterator<Map.Entry<Integer,String>>it=map.entrySet().iterator();while(it.hasNext()){Map.Entry<Integer,String>entry=it.next();Integerkey=entry.getKey();if(key%2==0){System.out.println("To delete key "+key);it.remove();System.out.println("The key "++key+" was deleted");}}|
```
```
|12345678910111213141516|Todelete key0The key0was deletedTodelete key2The key2was deletedTodelete key4The key4was deletedTodelete key6The key6was deletedTodelete key8The key8was deletedmap size=51=value13=value35=value57=value79=value9|
## 分析原因
其实上面的三种遍历方式从根本上讲都是使用的迭代器，之所以出现不同的结果是由于remove操作的实现不同决定的。
首先前两种方法都在调用nextEntry方法的同一个地方抛出了异常
```
```
|1234567|finalEntry<K,V>nextEntry(){if(modCount!=expectedModCount)thrownewConcurrentModificationException();Entry<K,V>e=next;......}|
这里modCount是表示map中的元素被修改了几次(在移除，新加元素时此值都会自增)，而expectedModCount是表示期望的修改次数，在迭代器构造的时候这两个值是相等，如果在遍历过程中这两个值出现了不同步就会抛出ConcurrentModificationException异常。
1、HashMap的remove方法实现
```
```
|1234|publicVremove(Objectkey){Entry<K,V>e=removeEntryForKey(key);return(e==null?null:e.value);}|
2、HashMap.KeySet的remove方法实现
```
```
|123|publicbooleanremove(Objecto){returnHashMap.this.removeEntryForKey(o)!=null;}|
3、HashMap.HashIterator的remove方法实现
```
```
|12345678910|publicvoidremove(){if(current==null)thrownewIllegalStateException();if(modCount!=expectedModCount)thrownewConcurrentModificationException();Objectk=current.key;current=null;HashMap.this.removeEntryForKey(k);expectedModCount=modCount;}|
以上三种实现方式都通过调用HashMap.removeEntryForKey方法来实现删除key的操作。在removeEntryForKey方法内只要移除了key modCount就会执行一次自增操作，此时modCount就与expectedModCount不一致了，上面三种remove实现中，只有第三种iterator的remove方法在调用完removeEntryForKey方法后同步了expectedModCount值与modCount相同，所以在遍历下个元素调用nextEntry方法时，iterator方式不会抛异常。
```
```
|1234567891011121314151617181920212223242526|finalEntry<K,V>removeEntryForKey(Objectkey){inthash=(key==null)?0:hash(key.hashCode());inti=indexFor(hash,table.length);Entry<K,V>prev=table[i];Entry<K,V>e=prev;while(e!=null){Entry<K,V>next=e.next;Objectk;if(e.hash==hash&&((k=e.key)==key||(key!=null&&key.equals(k)))){modCount++;size--;if(prev==e)table[i]=next;elseprev.next=next;e.recordRemoval(this);returne;}prev=e;e=next;}returne;}|
## 发散
1、如果是遍历过程中增加或修改数据呢？
增加或修改数据只能通过Map的put方法实现，在遍历过程中修改数据可以，但如果增加新key就会在下次循环时抛异常，因为在添加新key时modCount也会自增。
2、有些集合类也有同样的遍历问题，如ArrayList，通过Iterator方式可正确遍历完成remove操作，直接调用list的remove方法就会抛异常。
```
```
|1234567891011|//会抛ConcurrentModificationException异常for(Stringstr:list){list.remove(str);}//正确遍历移除方式Iterator<String>it=list.iterator();while(it.hasNext()){it.next();it.remove();}|
3、jdk为什么这样设计，只允许通过iterator进行remove操作？
HashMap和keySet的remove方法都可以通过传递key参数删除任意的元素，而iterator只能删除当前元素(current)，一旦删除的元素是iterator对象中next所正在引用的，如果没有通过modCount、 expectedModCount的比较实现快速失败抛出异常，下次循环该元素将成为current指向，此时iterator就遍历了一个已移除的过期数据。
