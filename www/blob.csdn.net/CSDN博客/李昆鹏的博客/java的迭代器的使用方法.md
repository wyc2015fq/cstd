
# java的迭代器的使用方法 - 李昆鹏的博客 - CSDN博客


2018年03月17日 18:22:50[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：455


**-----------------------java的迭代器-----------------------------------**
**集合的遍历：**
**遍历方法：**
|Iterator|<|E|>
|iterator|()
|返回在此 collection 的元素上进行迭代的迭代器。
|
![](//img-blog.csdn.net/20180317182206609?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


## java.util
## 接口 Iterator<E>
**所有已知子接口：**
ListIterator<E>,XMLEventReader
**所有已知实现类：**
BeanContextSupport.BCSIterator,EventReaderDelegate,Scanner
---

public interface**Iterator<E>**对 collection 进行迭代的迭代器。迭代器取代了 JavaCollections Framework 中的 Enumeration。迭代器与枚举有两点不同：
迭代器允许调用者利用定义良好的语义在迭代期间从迭代器所指向的 collection 移除元素。
方法名称得到了改进。
此接口是JavaCollections Framework的成员。
|方法摘要
|
|boolean
|hasNext|()
|如果仍有元素可以迭代，则返回|true|。
|
|E
|next|()
|返回迭代的下一个元素。
|
|void
|remove|()
|从迭代器指向的 collection 中移除迭代器返回的最后一个元素（可选操作）。
|
**例子：**
**public****class**CollectionTest5 {
**public****static****void**main(String[]args) {
//创建一个集合
Collectioncoll=**new**ArrayList();
**for**(**int**i= 0;i< 10;i++) {
coll.add("张三"+i);
}
//创建集合的迭代器，这个迭代器只能给Coll这个集合来使用
Iteratoriter=coll.iterator();
**while**(iter.hasNext()){
//Objectobj= iter.next();
//shift +Alt+ R可以一起修改同名的变量
Stringstr=(String)iter.next();
System.**out**.println(str);
}
//迭代器只能使用一次，再次使用false
//     boolean isHasNext = iter.hasNext();
//     System.out.println(isHasNext);
//     iter.next();
//如果要再遍历一次集合，需要再次创建一个迭代器
Iteratoriter1=coll.iterator();
**while**(iter1.hasNext()){
//Objectobj= iter.next();
//shift +Alt+ R可以一起修改同名的变量
Stringstr=(String)iter1.next();
//迭代器在遍历的时候，这个集合不能做任何操作
//if("张三3".equals(str))
//coll.add("李四");
System.**out**.println(str);
}
}
}
结果：
张三0
张三1
张三2
张三3
张三4
张三5
张三6
张三7
张三8
张三9
张三0
张三1
张三2
张三3
张三4
张三5
张三6
张三7
张三8
张三9

