# 四种List实现类的对比总结 - weixin_33985507的博客 - CSDN博客
2018年12月25日 14:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
此文已由作者赵计刚授权网易云社区发布。
欢迎访问[网易云社区](https://sq.163yun.com/blog?tag=M_tg_1007_65)，了解更多网易技术产品运营经验。
**1、ArrayList**
- 
非线程安全
- 
基于对象数组
- 
get(int index)不需要遍历数组，速度快；
- 
iterator()方法中调用了get(int index)，所以速度也快
- 
set(int index, E e)不需要遍历数组，速度快
- 
add方法需要考虑扩容与数组复制问题，速度慢
- 
remove(Object o)需要遍历数组，并复制数组元素，速度慢
- 
remove(int index)不需要遍历数组，需要复制数组元素，但不常用
- 
contain(E)需要遍历数组
**2、LinkedList**
- 
非线程安全
- 
基于环形双向链表
- 
get(int index)需要遍历链表，速度慢；
- 
iterator()方法中调用了get(int index)，所以速度也慢
- 
set(int index, E e)方法中调用了get(int index)，所以速度也慢
- 
add方法不需要考虑扩容与数组复制问题，只需创建新对象，再将新对象的前后节点的指针指向重新分配一下就好，速度快
- 
remove(Object o)需要遍历链表，但不需要复制元素，只需将所要删除的对象的前后节点的指针指向重新分配一下以及将所要删除的对象的三个属性置空即可，速度快
- 
remove(int index)需要遍历链表，但不需要复制元素，只需将所要删除的对象的前后节点的指针指向重新分配一下以及将所要删除的对象的三个属性置空即可，但不常用
- 
contain(E)需要遍历链表
**3、Vector（线程安全的ArrayList）**
- 
线程安全
- 
扩容机制与ArrayList不同
**4、Stack（继承于Vector）**
- 
线程安全
- 
效率低下，可采用双端队列Deque或LinkedList来实现，Deque用的较多
**总结：**
- 
在查询（get）、遍历（iterator）、修改（set）使用的比较多的情况下，用ArrayList
- 
在增加（add）、删除（remove）使用比较多的情况下，用LinkedList
- 
在需要线程安全而且对效率要求比较低的情况下，使用Vector，当然，实现ArrayList线程安全的方法也有很多，以后再说
- 
在需要使用栈结构的情况下，使用Deque，Stack废弃就行了
[免费领取验证码、内容安全、短信发送、直播点播体验包及云服务器等套餐](https://www.163yun.com/free?tag=M_tg_1007_65)
更多网易技术、产品、运营经验分享请[点击](https://sq.163yun.com/blog?tag=M_tg_1007_65)。
相关文章：
【推荐】 [从golang的垃圾回收说起（下篇）](https://www.jianshu.com/p/d82a84a0b632)
