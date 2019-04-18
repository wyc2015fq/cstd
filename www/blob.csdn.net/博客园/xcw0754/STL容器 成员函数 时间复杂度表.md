# STL容器  成员函数 时间复杂度表 - xcw0754 - 博客园
# [STL容器  成员函数 时间复杂度表](https://www.cnblogs.com/xcw0754/p/4266609.html)
||**Sequence containers**|**Associative containers**|| | | | | | | |
|----|----|----|----|----|----|----|----|----|----|----|
|Headers|**<vector>**|**<deque>**|**<list>**|**<set>**||**<bitset>**| | | | |
|Members|**complex**|**vector**|**deque**|**list**|**set**|**multiset**|**map**|**multimap**|**bitset**| |
||*constructor*|*****|constructor|constructor|constructor|constructor|constructor|constructor|constructor|constructor|
|*destructor*|**O(n)**|destructor|destructor|destructor|destructor|destructor|destructor|destructor|| |
|operator=|**O(n)**|operator=|operator=|operator=|operator=|operator=|operator=|operator=|operators| |
|iterators|begin|**O(1)**|begin|begin|begin|begin|begin|begin|begin||
|end|**O(1)**|end|end|end|end|end|end|end|| |
|rbegin|**O(1)**|rbegin|rbegin|rbegin|rbegin|rbegin|rbegin|rbegin|| |
|rend|**O(1)**|rend|rend|rend|rend|rend|rend|rend|| |
|capacity|size|*****|size|size|size|size|size|size|size|size|
|max_size|*****|max_size|max_size|max_size|max_size|max_size|max_size|max_size|| |
|empty|**O(1)**|empty|empty|empty|empty|empty|empty|empty|| |
|resize|**O(n)**|resize|resize|resize|||||| |
|element access|front|**O(1)**|front|front|front||||||
|back|**O(1)**|back|back|back|||||| |
|operator[]|*****|operator[]|operator[]||||operator[]||operator[]| |
|at|**O(1)**|at|at||||||| |
|modifiers|assign|**O(n)**|assign|assign|assign||||||
|insert|*****|insert|insert|insert|insert|insert|insert|insert|| |
|erase|*****|erase|erase|erase|erase|erase|erase|erase|| |
|swap|**O(1)**|swap|swap|swap|swap|swap|swap|swap|| |
|clear|**O(n)**|clear|clear|clear|clear|clear|clear|clear|| |
|push_front|**O(1)**||push_front|push_front|||||| |
|pop_front|**O(1)**||pop_front|pop_front|||||| |
|push_back|**O(1)**|push_back|push_back|push_back|||||| |
|pop_back|**O(1)**|pop_back|pop_back|pop_back|||||| |
|observers|key_comp|**O(1)**||||key_comp|key_comp|key_comp|key_comp||
|value_comp|**O(1)**||||value_comp|value_comp|value_comp|value_comp|| |
|operations|find|**O(log n)**||||find|find|find|find||
|count|**O(log n)**||||count|count|count|count|count| |
|lower_bound|**O(log n)**||||lower_bound|lower_bound|lower_bound|lower_bound|| |
|upper_bound|**O(log n)**||||upper_bound|upper_bound|upper_bound|upper_bound|| |
|equal_range|**O(log n)**||||equal_range|equal_range|equal_range|equal_range|| |
|*unique members*||capacity reserve||spliceremoveremove_ifuniquemergesort reverse|||||setresetflip to_ulongto_string test anynone| |

