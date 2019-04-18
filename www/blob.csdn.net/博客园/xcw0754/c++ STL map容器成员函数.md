# c++ STL map容器成员函数 - xcw0754 - 博客园
# [c++ STL map容器成员函数](https://www.cnblogs.com/xcw0754/p/4273396.html)
map容器用于查找，设置键值和元素值，输入键值，就能得到元素值。map对象中的元素时刻都是有序的，除非无序插入的。它是用平衡树创建的。查找很快。
|函数|描述,注意有r的地方都是不能用it代替的。|
|----|----|
|map (comp,alloc)map (it first, it last, comp,alloc)map (map& x)|Comp为比较函数，用于排序，alloc不用鸟它。两个参数有默认值，可不填，按默认的。创建空map。用一段元素填充，并设置排序函数。拷贝x中的元素并创建新map。估计新的map对象的comp也是和x一样。|
|mapped_type& m.at (key_type& k)|返回键值k对应的元素值，回值引用类型的。|
|it m.begin()|返回第1个元素的地址，也就是有序的首个元素。|
|const_it m.cbegin()|功能同上，但是不能修改此元素。|
|const_it m.cend()|返回最后一个元素的下一个元素的地址，不可修改元素值。若容器为空，则返回同cbegin。|
|Void m.clear()|清空容器内所有元素。|
|int m.count(key_type& k)|若键值k对应的元素存在，则返回1，否则0。|
|const_rit m.crbegin()|返回最后一个元素的地址。|
|const_rit m.crend()|返回首个元素的前一个元素地址，该地址无元素。|
|pair<it,bool> m.emplace (键值,单个元素)|插入。若键值不存在，则插入，并返回一对参数分别为元素地址和true。否则返回已存在的那个元素地址和false。|
|it m.emplace_hint (it pos,单个元素)|插入。若键值不存在，则插入到pos所指位置，并返回该地址。否则返回已存在的那个元素地址。这样插就不一定会保证有序了。|
|bool m.empty()|若容器为空，则返回true，否则false。|
|it m.end()|返回最后一个元素的下一个元素地址。该地址无元素。|
|pair<it,it>             m.equal_range (k)|返回键值k的元素位置，包括首地址和尾地址，这两个指针内只有一个元素。若不存在k，则返回两个begin的地址。|
|void m.erase (it pos)int m.erase (k)void m.erase (it first, it last)|删除pos地址的元素。删除键值为k的元素，返回剩余元素个数。删除一段元素，last所指元素不删。|
|it m.find(k)|寻找键值为k的元素，并用返回其地址。找不到则返回end的地址。|
|Get_allocator|不懂|
|pair<it,bool> m.insert (val)it m.insert (it pos, val)void m.insert (It first, It last)|插入元素。例如mymap.insert ( std::pair<char,int>('a',100) )并返回其地址和true。Pair也可以定义成一个变量形式的。插入val到指定的pos位置，返回pos。若失败，it指向已存在的那个元素。将first到last的一段元素插入进去。|
|key_compare m.key_comp()|返回m的比较对象。类似于返回了一个函数，此函数可用来比较两个map中的元素大小，参数是两个键值。|
|It m.lower_bound (k)|返回键值k的元素的首地址。|
|Int m.max_size()|返回能容纳元素的数量。|
|map& operator= (const map& x)|复制。将一个map对象复制到另一对象。注：1对象可包含多个元素。|
|mapped_type& operator[] (const key_type& k)|可以用下标法查找元素。比如m[10]表示返回键值为10的元素，不输出键值。|
|rit m.rbegin()|返回反向的首元素的地址，得用反向指针。|
|rit m.rend()|返回正向的首元素的地址，也就是反向的尾地址了。|
|int m.size()|返回map中已存在元素的数量。|
|It m.upper_bound (k)|返回键值为k的元素的末地址，该地址是其他元素。|
|void m.swap(map& x)|将两个map对象换名字。|
|value_compare m.value_comp()|返回元素比较函数，可以用于比较值大小。例如mymap.value_comp()(*it, highest)这里highest的类型是pair型的，即std::pair<char,int>|
||bool operator== ( const map<Key,T,Compare,Alloc>& lhs,const map<Key,T,Compare,Alloc>& rhs );bool operator!= ( const map<Key,T,Compare,Alloc>& lhs,const map<Key,T,Compare,Alloc>& rhs );bool operator<  ( const map<Key,T,Compare,Alloc>& lhs,const map<Key,T,Compare,Alloc>& rhs );bool operator<= ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs );bool operator>  ( const map<Key,T,Compare,Alloc>& lhs, const map<Key,T,Compare,Alloc>& rhs );bool operator>= ( const map<Key,T,Compare,Alloc>& lhs,const map<Key,T,Compare,Alloc>& rhs )|
|void swap (map& x, map& y)|交换两个map对象的名字，两对象必须是同类型的。|

