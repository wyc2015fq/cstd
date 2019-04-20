# leetcode第一题判断链表是否有环 - Spark高级玩法 - CSDN博客
2018年12月21日 00:04:51[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：113
为啥要刷leetcode？
数据结构表征数据存储的格式及操作数据的方式，了解这些便于我们大数据开发人员设计更好的存储，读取，计算策略。所以在java基础，大数据基础，大数据框架源码等都有一定基础之后应该去追求写出更加精致高效的代码。
最近，在整理java面试题，发现很多java底层，redis的有序set等存储结构，spark ，mr等等等我们常用的工具常见的框架都用到了数据结构与算法。所以，要想彻底搞明白底层原理，编写处时间复杂度比较低的代码，还是要去刷一下数据结构，况且数据结构貌似是bat 数据技术类必须面试的门槛，当然你做平台开发最好也会，不要以为用不到就不去学，只是你还比较菜。
再回到为啥要刷一下leetcode？
老外都在刷，大学生也在刷，自己不刷刷，大数据搞的再好有毛用，也只是底层开发。
此处，应该吐血。。。
于是乎，今天leetcode破处了，第一个题是以前没搞明白的一个题。题目如下：
Given a linked list, determine if it hasa cycle in it.
就是给定一个链表如何判断其中有环。leetcode给出的命题及案例如下：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU1SDnpfQY16M8dMcbfRZVFoGj5lAdXKYjoG5Mh0gPnibI2xmPYs9Gbnp8JuAwcRRNxxsdjvo4WRAA/640)
第一次是毕业的时候面试被问到这个题，一脸懵逼，这不刷题谁会？
最近细思大致思路有三：
- 
穷举。从头遍历到尾，发现指向null，说明没环。这明显不靠谱。。。时间复杂度O(n)
- 
第三方存储。边遍历边将指针存入hashset，并且判断当前指针是否存在于hashset，假如存在确定有环。否则无环。时间复杂度O(n)。
public boolean hasCycle(ListNode head) {
    Set<ListNode> nodesSeen = new HashSet<>();
while (head != null) {
if (nodesSeen.contains(head)) {
returntrue;
        } else {
            nodesSeen.add(head);
        }
        head = head.next;
    }
returnfalse;
}
- 
快慢指针。快指针每次走两步，慢指针走一步。假如无环，慢指针永远无法追上快指针，时间复杂度就是O(n)。假如有环，那么快指针会先掉进环里，在那打圈转，快慢指针会相遇。leetcode 上编写的java代码如下：
ListNode walker = head;
        ListNode runner = head;
while(runner!=null&&runner.next!=null){
            walker = walker.next;
            runner = runner.next.next;
if(walker == runner){
returntrue;
            }
        }
returnfalse;
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU1SDnpfQY16M8dMcbfRZVFIG71GZox36dia2v1Suib3XO0EYmCbedLv93CV8WGibicdqgIaLnicuWmAbg/640)
其实，理解思路的话代码实现是很简单。
做出来了是容易，那么可以来个四连问：
- 
求出环入口地址。
- 
求出来环大小。
- 
求头节点到环入口的长度。
- 
求出环入口到第一次相遇点的距离。
下次分析。。。
推荐极客时间的视频课程：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU1SDnpfQY16M8dMcbfRZVFdx119VaMB6m6IxnysS0DmWEes90CHibh7V8RcBZEWjM8wn0iaJK8DlGw/640)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
