# STL - KinboSong的博客 - CSDN博客
2017年03月21日 10:14:37[songjinbo3](https://me.csdn.net/KinboSong)阅读数：195
个人分类：[数据结构与算法](https://blog.csdn.net/KinboSong/article/category/6791889)
容器总共有16种：
array(不常用)、vector、stack（不常用）、deque、queue（不常用）、priority_queue（不常用）、list、forward_list（不常用）、map、multimap、set、multiset、unordered_map、unordered_multimap、unordered_set、unordered_multiset
1、vector容器：封装的数组
参考：[http://blog.csdn.net/phoebin/article/details/3864590](http://blog.csdn.net/phoebin/article/details/3864590)
定义vector<int> vec;
访问vector元素的三种方式：
（1）vec[index]
（2）vec.at(index)
（3）vec<int>::iterator *iter
vec.begin()
vec.end()
vec.size()
vec.push_back(int n)
2、map容器：封装了二叉树，数据是有序的，具有红黑树的有点，遍历、插入和删除都很快。
std::map<int,string> enumMap;
1、赋值方法：
（1）
enumMap[1] = "One"
enumMap[2] = "Two"
（2）
enumMap.insert（map<int,CString>::value_type(2,"Two")）
2、取值方法
(1)string str = enumMap[1]
(2)
map<int,string>::iterator it = enum.find(n) //**时间复杂度为O（n）**
if(it == enumMap.end())  //没有找到键值n
{//}
else
{//找到了}
multimap：multimap容器也是用红黑树对记录型元素数据按照键值的比较关系进行快速的插入、删除、检索，元素的检索是对数级的时间复杂度，与map不同的是，multimap允许将具有重复键值的元素插入容器，元素的键值与元素的映射数据的映射关系是多对多的。
3、unordered_map：内部封装了哈希表，内部的数据是无序的。
4、set：内部封装了红黑树，也是排序好的；但是，里面不含有重复的元素。
5、list：双向链表，内部封装了链表；
6、forward_list：单向链表，内部封装了链表；
7、stack：以vector为基类（不建议使用，很鸡肋）
8、queue
9、deque：deque与vector组织内存的方式不一样。在底层，deque按“页”（page）或“块”（chunk）来分配存储器，每页包含固定数目的元素。而vector只分配一块连续的内存。例如，一个10M字节的vector使用的是一整块10M字节的内存，而deque可以使用一串更小的内存块，比如10块1M的内存。所以不能将deque的地址（如&deque[0]）传递给传统的C
 API，因为deque内部所使用的内存不一定会连续。
STL底层数据结构实现：[http://www.cnblogs.com/hustlijian/p/3611424.html](http://www.cnblogs.com/hustlijian/p/3611424.html)
list vector和dque的区别与联系：[http://blog.csdn.net/gogokongyin/article/details/51178378](http://blog.csdn.net/gogokongyin/article/details/51178378)
