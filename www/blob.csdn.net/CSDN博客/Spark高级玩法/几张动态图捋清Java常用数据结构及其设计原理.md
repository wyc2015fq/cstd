# 几张动态图捋清Java常用数据结构及其设计原理 - Spark高级玩法 - CSDN博客
2018年11月26日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：82
**原文:https://www.cnblogs.com/xdecode/p/9321848.html**
**转载自公众号【java进阶架构师】**
最近在整理数据结构方面的知识, 系统化看了下Java中常用数据结构, 突发奇想用动画来绘制数据流转过程.
主要基于jdk8, 可能会有些特性与jdk7之前不相同, 例如LinkedList LinkedHashMap中的双向列表不再是回环的.
HashMap中的单链表是尾插, 而不是头插入等等, 后文不再赘叙这些差异, 本文目录结构如下:
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafic91J6dP7ibiaQ1tp9XmUwI8LbrvhBvRkMOc1pUhDTCL9MU5Igu1FnyJA/640?)
### LinkedList
经典的双链表结构, 适用于乱序插入, 删除. 指定序列操作则性能不如ArrayList, 这也是其数据结构决定的.
**add(E) / addLast(E)**
**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafjic5hw2enAu6w8dOvZfwSFTazvpiaJJcbE4GeyVh6WL1x0ibYtbawibbgg/640?)**
**add(index, E)**
这边有个小的优化, 他会先判断index是靠近队头还是队尾, 来确定从哪个方向遍历链入.
```
1         if (index < (size >> 1)) {
2             Node<E> x = first;
3             for (int i = 0; i < index; i++)
4                 x = x.next;
5             return x;
6         } else {
7             Node<E> x = last;
8             for (int i = size - 1; i > index; i--)
9                 x = x.prev;
10             return x;
11         }
```
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafMgbbqk7j86u8bIMpD1aHrIicffVnkI9CtllD3gO6VfBBm41KaBIFeyA/640?)
**靠队尾**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgaftBDHeaD4laBgic4jXrxL9EJE6V27Rzx1hdm4W3OZnknbdRf8iamroZWw/640?)
**get(index)**
也是会先判断index, 不过性能依然不好, 这也是为什么不推荐用for(int i = 0; i < lengh; i++)的方式遍历linkedlist, 而是使用iterator的方式遍历.
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafRqFCMtctr3s9dW9uy0G4aKDibMJb7DE6hpohWhFbssrTOJTcUo4Gxnw/640?)
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafadkibGMxIZmocYoDIicjxdJAvHm3TtYyNYRXgsBkTJEeV62edW4bFbSw/640?)
**remove(E)**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafrX80olcD7r6GS3uXydib1Q2eiavE6jBtRL21KSWCUTPdic6ssJ78ks6vw/640?)
### ArrayList
底层就是一个数组, 因此按序查找快, 乱序插入, 删除因为涉及到后面元素移位所以性能慢.
**add(index, E)**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgaf0Kq5srtjsVuQcdPiaSab3b36ugXxicM5VSBVpqG8xk3Nsw3tKrdP1ccw/640?)
**扩容**
一般默认容量是10, 扩容后, 会length*1.5.
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgaf2dEyAodgVQMNT1NrqB6hykZYV1ia2ic883cs1yvnyUygLDkS7jgm5vGA/640?)
**remove(E)**
循环遍历数组, 判断E是否equals当前元素, 删除性能不如LinkedList.
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafx714ibDMbdqwQMYNMqjib8zgQkuZqphvdaCjH6AJeOOMlP6KFPKic74dQ/640?)
### Stack
经典的数据结构, 底层也是数组, 继承自Vector, 先进后出FILO, 默认new Stack()容量为10, 超出自动扩容.
**push(E)**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgaf3J2jj2yJS6u6pf6qbCwSiccjoolrQ1jYvM3WRkfqygJOaTx74vibT6fw/640?)
**pop()**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafMcNw5z1lKgMQdhT9XolcoLiaky7AwIOGrYqj5icqBdartlJeFKMlcrog/640?)
### 后缀表达式
Stack的一个典型应用就是计算表达式如 9 + (3 - 1) * 3 + 10 / 2, 计算机将中缀表达式转为后缀表达式, 再对后缀表达式进行计算.
**中缀转后缀**
- 
数字直接输出
- 
栈为空时，遇到运算符，直接入栈
- 
遇到左括号, 将其入栈
- 
遇到右括号, 执行出栈操作，并将出栈的元素输出，直到弹出栈的是左括号，左括号不输出。
- 
遇到运算符(加减乘除)：弹出所有优先级大于或者等于该运算符的栈顶元素，然后将该运算符入栈
- 
最终将栈中的元素依次出栈，输出。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafRKzcSkm0GSRZYyneQTkVLmGg2HNz1HkMrGmUjNEsfdjKaOTSjBJdUA/640?)
**计算后缀表达**
- 
遇到数字时，将数字压入堆栈
- 
遇到运算符时，弹出栈顶的两个数，用运算符对它们做相应的计算, 并将结果入栈
- 
重复上述过程直到表达式最右端
- 
运算得出的值即为表达式的结果
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafyP080icsRJsDtIlicTnfPTYBubtGRB4PSX8ib5LjJ6OGNibibhicNBFNMnIw/640?)
### 队列
与Stack的区别在于, Stack的删除与添加都在队尾进行, 而Queue删除在队头, 添加在队尾.
**ArrayBlockingQueue**
生产消费者中常用的阻塞有界队列, FIFO.
**put(E)**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgaf343ZL2bRIXOa7b25M5TwB7VISicpsjN7ibXKRAlD453IE7zib9tquhKWQ/640?)
**put(E) 队列满了**
```
1         final ReentrantLock lock = this.lock;
2         lock.lockInterruptibly();
3         try {
4             while (count == items.length)
5                 notFull.await();
6             enqueue(e);
7         } finally {
8             lock.unlock();
9         }
```
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafDU95JUO1sevNWeSx23gNpxFaF3AH7XEN2L4zp9sBnrGN3AQpAqZwlQ/640?)
**take()**
当元素被取出后, 并没有对数组后面的元素位移, 而是更新takeIndex来指向下一个元素.
takeIndex是一个环形的增长, 当移动到队列尾部时, 会指向0, 再次循环.
```
1     private E dequeue() {
2         // assert lock.getHoldCount() == 1;
3         // assert items[takeIndex] != null;
4         final Object[] items = this.items;
5         @SuppressWarnings("unchecked")
6         E x = (E) items[takeIndex];
7         items[takeIndex] = null;
8         if (++takeIndex == items.length)
9             takeIndex = 0;
10         count--;
11         if (itrs != null)
12             itrs.elementDequeued();
13         notFull.signal();
14         return x;
15     }
```
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafEPULrpeAfKppOiaXaJF5olOFYBbuiaYfBA7lNxXE18Za99OvDK9cUOgw/640?)
### HashMap
最常用的哈希表, 面试的童鞋必备知识了, 内部通过数组 + 单链表的方式实现. jdk8中引入了红黑树对长度 > 8的链表进行优化, 我们另外篇幅再讲.
**put(K, V****)**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgaflFzo1kCPAPKJgYjnLVClpUJ6oUTMYAPlgb9ktFrpsD4t466ll3g4ug/640?)
**put(K, V) 相同hash值**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafnuicuTL1b0TH5Cdib1ibxiafsCcibUw6fNwRW7EnvwicTqJgpmWCFoeOvia8g/640?)
**resize 动态扩容**
当map中元素超出设定的阈值后, 会进行resize (length * 2)操作, 扩容过程中对元素一通操作, 并放置到新的位置.
具体操作如下:
- 
在jdk7中对所有元素直接rehash, 并放到新的位置.
- 
在jdk8中判断元素原hash值新增的bit位是0还是1, 0则索引不变, 1则索引变成"原索引 + oldTable.length".
```
1     //定义两条链
2     //原来的hash值新增的bit为0的链，头部和尾部
3     Node<K,V> loHead = null, loTail = null;
4     //原来的hash值新增的bit为1的链，头部和尾部
5     Node<K,V> hiHead = null, hiTail = null;
6     Node<K,V> next;
7     //循环遍历出链条链
8     do {
9         next = e.next;
10         if ((e.hash & oldCap) == 0) {
11             if (loTail == null)
12                 loHead = e;
13             else
14                 loTail.next = e;
15             loTail = e;
16         }
17         else {
18             if (hiTail == null)
19                 hiHead = e;
20             else
21                 hiTail.next = e;
22             hiTail = e;
23         }
24     } while ((e = next) != null);
25     //扩容前后位置不变的链
26     if (loTail != null) {
27         loTail.next = null;
28         newTab[j] = loHead;
29     }
30     //扩容后位置加上原数组长度的链
31     if (hiTail != null) {
32         hiTail.next = null;
33         newTab[j + oldCap] = hiHead;
34     }
```
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafEesRWSjCJdF3150HjhZKWDABeuSVww7aCcOb6gdCFaDsdnvicFH5Vicw/640?)
### LinkedHashMap
继承自HashMap, 底层额外维护了一个双向链表来维持数据有序. 可以通过设置accessOrder来实现FIFO(插入有序)或者LRU(访问有序)缓存.
**put(K, V)**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafFibEVA59a1vbaWyuxYv1CkluGAODqD6wrt4bnWtoXSqNIKsk8y1XbEw/640?)
**get(K)**
accessOrder为false的时候, 直接返回元素就行了, 不需要调整位置. 
accessOrder为true的时候, 需要将最近访问的元素, 放置到队尾.
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafPTIsV0brGGzyVrIUFK7eysLnQhC1M4BjpBv3bDNYPpumticyibJ43dAQ/640?)
**removeEldestEntry 删除最老的元素**
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/XaklVibwUKn5XmeIJ2TrXaKy1nOpDVgafVzNlW1BMLMic7wv1lICO2vdB8XDEoGZZXWsON3C8IJUmY1S8vW3JFjw/640?)
推荐阅读：
[深入理解 hashcode 和 hash 算法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485982&idx=1&sn=8f9b0a91203b6d918522cbc8df1912b8&chksm=9f38e936a84f60205ac9a18825f4203c8006e2bd548354aa35605c875221b24254ecbe66e391&scene=21#wechat_redirect)
[Java面试中常问的数据库方面问题](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485041&idx=1&sn=879eead8b97d8182955223dd1c918475&chksm=9f38e559a84f6c4f50948577cb37c8c90523f0e89e7fba9433a7a58d6ded818fff33c696aac9&scene=21#wechat_redirect)
[面试的角度诠释Java工程师（一）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484694&idx=1&sn=075b3e2ceafd31d27c4fc735011b03e8&chksm=9f38e63ea84f6f288407c64349f6c0b6a183c3b61fa50a27d0ad601734880b9a5f9901f29d47&scene=21#wechat_redirect)
四百多位好友在这里交流
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640)
