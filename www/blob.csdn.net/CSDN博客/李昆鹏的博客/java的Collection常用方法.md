
# java的Collection常用方法 - 李昆鹏的博客 - CSDN博客


2018年03月17日 14:31:01[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：242


**-----------------------java的Collection常用方法---------------------**
**集合的方法：**
**删除方法：**
|void
|clear|()
|移除此列表中的所有元素。
|
|boolean
|remove|(|Object|o)
|从此 collection 中移除指定元素的单个实例，如果存在的话（可选操作）。
|
|boolean
|removeAll|(|Collection|<?> c)
|移除此 collection 中那些也包含在指定 collection 中的所有元素（可选操作）。
|
**例子：**
**public****class**CollectionTest3 {
**public****static****void**main(String[]args) {
//创建一个集合
Collectioncoll=**new**ArrayList();
**for**(**int**i= 0;i< 10;i++) {
coll.add("张三"+i);
}
System.**out**.println(coll);
//删除集合中的所有元素
/*coll.clear();
System.out.println(coll);*/
//删除集合中指定的元素
/*coll.remove("张三6");
System.out.println(coll);*/
//创建一个集合
Collectioncoll1=**new**ArrayList();
**for**(**int**i= 0;i< 4;i++) {
coll1.add("张三"+i);
}
//从大集合中删除小集合中相同的内容
coll.removeAll(coll1);
System.**out**.println(coll);
}
}
**结果：**
[张三0,张三1,张三2,张三3,张三4,张三5,张三6,张三7,张三8,张三9]
[张三4,张三5,张三6,张三7,张三8,张三9]
**判断方法：**
|boolean
|contains|(|Object|o)
|如果此 collection 包含指定的元素，则返回|true|。
|
|boolean
|containsAll|(|Collection|<?> c)
|如果此 collection 包含指定 collection 中的所有元素，则返回|true|。
|
|boolean
|isEmpty|()
|如果此 collection 不包含元素，则返回|true|。
|
**数组转换方法：**
|Object|[]
|toArray|()
|返回包含此 collection 中所有元素的数组。
|
**例子：**
**public****class**CollectionTest4 {
**public****static****void**main(String[]args) {
//创建一个集合
Collectioncoll=**new**ArrayList();
**for**(**int**i= 0;i< 10;i++) {
coll.add("张三"+i);
}
//是否包含某个元素
//System.out.println(coll.contains("张三1"));
**boolean**b=coll.contains("张三1");
System.**out**.println(b);
//创建一个集合
Collection coll1=**new**ArrayList();
**for**(**int**i= 0;i< 3;i++) {
coll1.add("张三"+i);
}
//大集合中是否包含小集合的内容
**boolean**b1=coll.containsAll(coll1);
System.**out**.println(b1);
//清空契合的元素
//coll.clear();
//判断集合中是否有元素，空位true，否则为false
**boolean**isEmpty=coll.isEmpty();
System.**out**.println(isEmpty);
//把集合转换成数组
Object[]objs=coll.toArray();
System.**out**.println(objs);
//遍历数组
**for**(**int**i= 0;i<objs.length;i++) {
System.**out**.println(objs[i]);
}
}
}
**结果：**
true
true
false
[Ljava.lang.Object;@70dea4e
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


