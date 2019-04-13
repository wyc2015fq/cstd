
# 动态数组的 C 实现 - forever1dreamsxx--NLP - CSDN博客


2012年08月30日 17:01:59[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：704标签：[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[数据结构																](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)[vector																](https://so.csdn.net/so/search/s.do?q=vector&t=blog)[语言																](https://so.csdn.net/so/search/s.do?q=语言&t=blog)[iterator																](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=语言&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=vector&t=blog)个人分类：[计算机编程																](https://blog.csdn.net/forever1dreamsxx/article/category/1180978)
[
																								](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c&t=blog)
### 原文地址：
### [http://blog.codingnow.com/2009/11/array_c.html](http://blog.codingnow.com/2009/11/array_c.html)
上次谈到了一个常用的 ADT ，[sequence 的 C 实现](http://blog.codingnow.com/2009/11/sequence_c.html)。通常，我们不会直接使用 sequence ，而是用它来实现满足最终需要的数据结构。比如消息队列，比如指令堆栈，等等。一个实现的优秀的 seq 的好处在于，即使你只用到其中一部分功能，也不会因为那些没用的部分损失太多的（时间和空间上的）性能。
今天我想谈另一个更为实用的 ADT ，动态数组。这个在传世神作《[C 语言接口与实现](http://www.douban.com/subject/1230040/)》中也提到过，我不是想说其写的不对，或是讲述的不周全，实现的不漂亮。只是以我的观点来展开相关的问题。
为什么要有数组？C 语言内置了数组、在 C99 中更是允许在堆栈上声明非固定长度的数组。C++ 里以 STL 的形式提供了 vector 模块供程序员使用。
我们面临的问题，不是语言和库为我们提供了多少可能。而是我们无法确定，我们到底至少需要什么？
通常，数组让我们可以随机（以 O(1) 的时间复杂度）访问其内的数据。但是，随机访问不是目的，而是手段。我们究竟需要一个数组解决怎样的问题？面对怎样的问题，需要处理的数据需要以数组方式组织？我这几年始终不敢说自己得到了确切的答案。往往，我们只是直觉上感觉，用一个 vector （使用 C++ ）非常方便，所以就用了。当我在用 C 的时候，我总是随手写上一个 realloc ，来扩展我的数据块。最近一段时间，我觉得有必要归纳一下需求，设计一个通用模块来统一解决问题了。
思考的结果就是：当我们周期性的聚合一堆数据时，我们需要一个数组，且这个数组的长度是不确定的，应该可以动态增长。
数组最重要的特性是可以最高效的遍历，即依次处理数组内部的元素。在 C 语言的层面考虑这个问题时，就应该更细致考虑实现的差别引起的性能差异。比如链表的遍历同样是 O(N) ，但是时间和空间的开销都是大于连续内存空间的数组的。
我们需要快速的清空数组，方便下一个周期聚合新的数据。
我们需要对数据排序，使得按我们的需要的次序去依次处理（遍历）数组中的元素。
有时候，我们需要相对快速的查找。一旦数据是有序的，二分查找可以让时间复杂度减少到 O(log n) 。
其它，我们不再需要。
注：我们往往混淆了另一个需求。很多时候，我们把数组当成了一个高性能却有局限性的 key-value 字典。这个 key 就是数组内数据的数字索引。对于用一个 key （可能是数，也可能不是）快速索引到值的需求，应该放到另一个地方去考虑。
最终，我为 array 这个 ADT 提供了如下的方法：
struct array * array_create(int atom_size);
void array_release(struct array *);
void array_clear(struct array *);
void array_push(struct array *, void *atom);
void array_iterator(struct array *, seqi iter);
void array_sort(struct array *, int (*compar)(void *,void *));这里，暂时没有提供 binarysearch 方法，因为暂时我还没有用到。我还不确定是不是真的有意义，以及最佳的接口定义应该是怎样。
在实现时，我开辟了一整块连续内存保存数据。当使用者 push 一个元素时，采用值复制的方式，复制到 array 内部。然后用 seq 来管理对内部元素的引用（指针）。
当数据区不够大时，采用 realloc 扩展数据区内存。一旦内存地址发生变化，则修改对应的 seq 里的指针。把索引和数据分开存放，可以提高排序的效率（数据交换量比较小）。
sort 函数提供了一个类似 qsort 接口的比较函数。但是，由于和 qsort 的定义有所不同，所以不能直接去调用 qsort 。在 glibc 中，有另一个`qsort_r`可以满足需求。不过我考虑到性能，自己重新实现了一个快速排序。反正，也没几行代码。
这里，由于数据在内存中的位置并不固定。从 seqi 中拿到的指针是不可以被传递保存到别的数据结构体中的。包括 seqi 这个迭代子，根据上篇关于 sequence 实现的描述，也不允许长期保留。你可以认为，这是这个 ADT 的局限性。不过我个人认为，我们每设计一个东西，只应该解决有限的，定义好的需求。而不应该包罗万象。
只做一件事，并把它做好。
云风 提交于 November 20, 2009 01:51 PM|[
固定链接](http://blog.codingnow.com/2009/11/array_c.html)


