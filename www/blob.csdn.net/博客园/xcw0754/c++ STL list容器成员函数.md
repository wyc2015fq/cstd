# c++ STL list容器成员函数 - xcw0754 - 博客园
# [c++ STL list容器成员函数](https://www.cnblogs.com/xcw0754/p/4273130.html)
list是一个双链表。
|函数|描述|
|----|----|
|void l.assign (int n, const val)void l.assign (it first, it last)|将链表l初始化为n个相同的val元素。将链表l初始化为从first到last指针所指向的这一段区域内的元素。It指针同l是一个类型的。方便将另一个链表中的某一段连续的元素copy到新链表。|
|reference l.back ()|返回最后一个元素。经测试，可以直接l.back()=元素。|
|It l.begin()|返回第1个元素的地址。|
|Void l.clear()|清空链表。|
|bool l.empty()|判断链表中是否有元素，若有，返回true。|
|It l.end()|返回最后一个元素的下一个元素的地址，即此地址没有实际元素。|
|it l.erase (it pos);it l.erase (it first, it last);|删除链表中it指针所指向的元素，返回被删元素的下一个元素地址。删除从first到last这一段的所有元素，返回最后一个被删元素的下一个元素地址。|
|reference l.front ()|返回第一个元素。假如a=l.front()，那么a只是一个备份，对a修改不影响链表。|
|get_allocator()|一直不知道此物何用？|
|it l.insert (it pos, 元素类型val)void l.insert (it pos, int n,元素类型 val)void l.insert (it pos, it first, it last)|在pos的前一位置插入val元素，返回该元素的地址。Val元素是不会被修改的，只是拷贝。在pos的前一位置插入n个val元素。在pos的前一位置插入first到last之间的一段元素。可以是自己或其他链表中的。|
|int l.max_size()|返回链表可装入的元素数量上限，一般数字很大。经测，数字已包含了链表中存在的元素。|
|void l.merge (list& x)void l.merge (list& x, Compare comp)|将链表x接在l之后，合并为一个链表l，合并完后链表x中的元素为0了，但x仍存在。（不排序）将两链表合并在一起，按照comp函数的排序规则来排序。|
|list& l.operator= (const list& x)|将整个链表x中所有元素赋给链表l，相当于清空链表l，再将x赋过去。不改变x的数据。注意括号内是参数x。|
|void l.pop_back()|删除链表最后一个元素。|
|void l.pop_front()|删除链表第一个元素。|
|void l.push_back (const value_type& val)|将元素val插入到链表的尾部。|
|void l.push_front (const value_type& val)|将元素val插入到链表的头部。|
|it l.rbegin()|返回“反向链表”的第1个元素的地址。即返回链表的最后一个元素的地址。|
|void l.remove (const value_type& val)|删除值等于val的元素，得一模一样才能删除，估计比对的是整块内存的二进制。与erase不同。|
|void l.remove_if (Predicate pred)|功能：删除元素。将链表中每个元素单独传入pred函数，只要返回的结果为true，就删除。pred不用加括号，它类似于sort里的comp。|
|it l.rend()|返回“反向链表”的最后一个元素的下一个位置，这么说可能不正确，这个函数主要用于反向链表的操作。返回的是地址，该地址没有元素。|
|void l.resize (size_type n, value_type val = value_type())|重新设置链表的元素个数，若大了，会插入默认元素，若少了，或者删除部分元素。标准是已存元素。|
|Void l.reverse()|将所有元素的顺序置为相反，比如升序变为降序。|
|Int l.size()|返回已存在的元素个数，如果之前刚调用过resize，那size的大小就是resize所设置的。|
|Void l.sort(comp)Void l.sort()|排序，按照comp函数所返回的那样，若true则不改变位置，若false则交换位置。排序，按照默认的方法排序，一般为增序，比如整数、字符串比较的是ascii码。|
|void l.splice (it pos, list& x)void l.splice (it pos, list& x, it i)void l.splice (it pos, list& x, it first, it last)|将链表x中所有的元素“剪切”到pos处，其实只是指针操作，不用赋值。将链表x中的元素i“剪切”到pos处，只是单个元素。将链表x中first到last这段元素都迁移到l中的pos处。这些操作都会在链表l中增加新元素，并在x中删除相应元素。|
|void l.swap (list& x)|将两个链表互换名字，元素数量可不同，但类型得相同。|
|void l.unique()void l.unique (Predicate pred)|删除重复的多余元素，使每个元素唯一。相同元素的第一个都不会删。以pred函数为依据，删除重复的元素，当pred返回true时则删除其中一个。|
|bool operator== (list<T,Alloc>& lhs,list<T,Alloc>& rhs);bool operator!= ( list<T,Alloc>& lhs, list<T,Alloc>& rhs);bool operator<  (list<T,Alloc>& lhs,list<T,Alloc>& rhs);bool operator<= (list<T,Alloc>& lhs, list<T,Alloc>& rhs);bool operator>  (list<T,Alloc>& lhs, list<T,Alloc>& rhs);bool operator>= ( list<T,Alloc>& lhs, list<T,Alloc>& rhs);|一堆不常用的符号。直接比较，比如a==b。|
|void swap (list<T,Alloc>& x, list<T,Alloc>& y)|交换两个链表。与l.swap是一样的效果。|

