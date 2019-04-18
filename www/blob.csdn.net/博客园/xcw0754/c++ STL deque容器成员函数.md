# c++ STL deque容器成员函数 - xcw0754 - 博客园
# [c++ STL deque容器成员函数](https://www.cnblogs.com/xcw0754/p/4263488.html)
deque是双向队列，即可以在头部插入删除，也可以在尾部插入删除。内部并不连续，这一点和vector并不一样。可能第1个元素和第2个元素的地址是不连在一起的。在使用时用it迭代器会安全一点。
这是c++ 98标准的，不是c++11的。11标准新加的函数没在这里说明。里面大部分函数是经过测试的才得出的结论。
|函数(下面将iterator简化为it)|描述|
|----|----|
|Void c.assign(it beg,it end)void c.assign(it n,元素类型  elem)|将拷贝[beg; end)区间中的数据用覆盖式地赋值给c。第2个参数是不包含元素的。将n个elem的赋值给c。这两个函数均会将原来的容器清空再赋值。Iterator是c相同的容器类型。|
|it c.at(int idx)|传回索引idx所指的数据，idx从0开始，如果idx越界，抛出out_of_range。|
|reference c.back()|传回最后一个数据，不检查这个数据是否存在。只传不删。注意返回值与begin不同。|
|it c.begin()|传回迭代器中的第一个数据地址。返回的是第1个数据的地址，即iterator型。|
|Void c.clear()|移除容器中所有数据，被清空了，再访问里面的数据会出错。|
|Void c.empty()|判断容器是否为空。返回值为bool型。若空则返true，若非空返false。|
|it c.end()|指向迭代器中的最后一个数据的下一个数据的地址，该地址是没有元素的，与begin搭配的。若容器为空，返回与begin同样地址。|
|It c.erase(it pos)it c.erase(it beg,it end)|删除pos位置的数据，传回下一个数据的位置。删除[beg,end)区间的数据，传回下一个数据的位置。第2个参数是最后一个元素的首地址，即该地址是有元素的。|
|Reference c.front()|传回容器中第一个数据，不检查这个数据是否存在。只传不删。|
|c.get_allocator()|使用构造函数返回一个拷贝。//此函数暂不知道怎么用，windows下不太一样。原型allocator_type get_allocator() const;|
|It c.insert(it pos,元素类型 elem) void c.insert(it pos,int n,元素类型 elem)void c.insert(it pos,it beg,it end)|在pos位置插入一个elem元素拷贝，传回新数据位置。逻辑上pos处的元素就往后移了。在pos位置插入n个elem元素。在pos位置插入在[beg,end)区间的数据，这些数据也是属于容器中的。end没有实际元素。|
|Int c.max_size()|返回容器中可容纳多少个元素。不同类型的deque，可容纳数量不同。|
|Void c.pop_back()|删除最后一个数据。|
|Void c.pop_front()|删除头部数据。|
|Void c.push_back(元素类型 elem)|在尾部加入一个数据。|
|Void c.push_front(元素类型 elem)|在头部插入一个数据。|
|It c.rbegin()|传回一个逆向队列的第一个数据的地址。使用这两个可以逆向输出。|
|It c.rend()|传回一个逆向队列的最后一个数据的下一个位置的地址。|
|Void c.resize(int num，可选的默认值)|重新指定队列的元素数量为num个。2种情况：1.若num比存在的元素个数要少，则删除尾部的元素以调整个数。2.若num比实际存在的元素要多，则在实际元素之后填充默认值以调整个数。（第2个元素为可选，若没有则默认为该类型的默认值）|
|Int c.size()|返回容器中目前存放了多少个元素。(这个和max_size不同）|
|Void c1.swap(deuqe c2) void swap(deque c1,deque c2)|将c1和c2中的元素互换，c1和c2都是同一类型的deque。作用同上，这个可以直接用。|
|deque<Elem> cdeque<Elem> c1(deque c2)Deque<Elem> c(int n)Deque<Elem> c(int n, 类型 elem)Deque<Elem> c(it beg,it end)c.~deque<Elem>()|创建一个空的deque。Elem可以为int、char等等很多种类型。以c2初始化为c1，相当于复制出一个队列副本。创建一个deque，含有n个数据，数据均由缺省值在创建时初始化。创建一个含有n个elem拷贝的deque。创建一个以[beg;end)区间的deque。销毁所有数据，释放内存，但是c还可以继续使用，只不过包含的元素个数为0。|

