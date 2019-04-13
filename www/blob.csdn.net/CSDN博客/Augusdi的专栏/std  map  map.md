
# std::map::map - Augusdi的专栏 - CSDN博客


2016年10月10日 12:43:27[Augusdi](https://me.csdn.net/Augusdi)阅读数：860


﻿﻿
|explicit|map|(|const|Compare|&|comp|=|Compare|(|)|,
|const|Allocator|&|alloc|=|Allocator|(|)|)|;
|(until C++14)|
|map|(|)|:|map|(|Compare|(|)|)|{|}
|explicit|map|(|const|Compare|&|comp,
|const|Allocator|&|alloc|=|Allocator|(|)|)|;
|(since C++14)|
|explicit|map|(|const|Allocator|&|alloc|)|;
|(1)|(since C++11)|
|(2)|
|template|<|class|InputIterator|>
|map|(|InputIterator first, InputIterator last,
|const|Compare|&|comp|=|Compare|(|)|,
|const|Allocator|&|alloc|=|Allocator|(|)|)|;
|
|template|<|class|InputIterator|>
|map|(|InputIterator first, InputIterator last,
|const|Allocator|&|alloc|)|;
|(since C++14)|
|map|(|const|map|&|other|)|;
|(3)|
|map|(|const|map|&|other,|const|Allocator|&|alloc|)|;
|(3)|(since C++11)|
|map|(|map|&&|other|)|;
|(4)|(since C++11)|
|map|(|map|&&|other,|const|Allocator|&|alloc|)|;
|(4)|(since C++11)|
|(5)|
|map|(|std::|initializer_list|<|value_type|>|init,
|const|Compare|&|comp|=|Compare|(|)|,
|const|Allocator|&|alloc|=|Allocator|(|)|)|;
|(since C++11)|
|map|(|std::|initializer_list|<|value_type|>|init,
|const|Allocator|&|)|;
|(since C++14)|
|
Constructs new container from a variety of data sources and optionally using user supplied allocator`alloc`or comparison function object`comp`.
1)Default constructor. Constructs empty container.
2)Constructs the container with the contents of the range`[first, last)`.
3)Copy constructor. Constructs the container with the copy of the contents of`other`. If`alloc`is not provided, allocator is obtained by callingstd::allocator_traits<allocator_type>::select_on_container_copy_construction(other.get_allocator()).
4)Move constructor. Constructs the container with the contents of`other`using move semantics. If`alloc`is not provided, allocator is obtained by move-construction from the allocator belonging to`other`.
5)Constructs the container with the contents of the initializer list`init`.
|Contents
|[|[hide](http://en.cppreference.com/w/cpp/container/map/map#)|]
|1|[ ](http://en.cppreference.com/w/cpp/container/map/map#Parameters)|Parameters
|2|[ ](http://en.cppreference.com/w/cpp/container/map/map#Complexity)|Complexity
|3|[ ](http://en.cppreference.com/w/cpp/container/map/map#Notes)|Notes
|4|[ ](http://en.cppreference.com/w/cpp/container/map/map#Example)|Example
|5|[ ](http://en.cppreference.com/w/cpp/container/map/map#See_also)|See also
|
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=Template:cpp/container/constructor_ord&action=edit&section=T-1)
### ]
### Parameters
|alloc|-|allocator to use for all memory allocations of this container|
|comp|-|comparison function object to use for all comparisons of keys|
|first, last|-|the range to copy the elements from|
|other|-|another container to be used as source to initialize the elements of the container with|
|init|-|initializer list to initialize the elements of the container with|
|Type requirements|
|-
|InputIterator|must meet the requirements of|InputIterator|.|
|-
|Compare|must meet the requirements of|Compare|.|
|-
|Allocator|must meet the requirements of|Allocator|.|
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=Template:cpp/container/constructor_ord&action=edit&section=T-2)
### ]
### Complexity
1)Constant
2)N log(N)whereN=std::distance(first, last)in general, linear in`N`if the range is already sorted by`value_comp()`.
3)Linear in size of`other`
4)Constant. If`alloc`is given andalloc!=other.get_allocator(), then linear.
5)N log(N)whereN=init.size())in general, linear in`N`if`init`is already sorted by`value_comp()`.
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=Template:cpp/container/constructor_ord&action=edit&section=T-3)
### ]
### Notes
After container move construction (overload(4)), references, pointers, and iterators (other than the end iterator) to`other`remain valid, but refer to elements that are now in*this. The current standard makes this guarantee via the blanket statement in §23.2.1[container.requirements.general]/12, and a more direct guarantee is under consideration via[LWG 2321](http://www.open-std.org/JTC1/SC22/WG21/docs/lwg-active.html#2321).
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=Template:cpp/container/constructor_ord&action=edit&section=T-4)
### ]
### Example

```python
#include <iostream>
#include <string>
#include <iomanip>
#include <map>
 
template<typename Map>
void print_map(Map& m)
{
   std::cout << '{';
   for(auto& p: m)
        std::cout << p.first << ':' << p.second << ' ';
   std::cout << "}\n";
}
 
int main()
{
  // (1) Default constructor
  std::map<std::string, int> map1;
  map1["something"] = 69;
  map1["anything"] = 199;
  map1["that thing"] = 50;
  std::cout << "map1 = "; print_map(map1);
 
  // (2) Range constructor
  std::map<std::string, int> iter(map1.find("anything"), map1.end());
  std::cout << "\niter = "; print_map(iter);
  std::cout << "map1 = "; print_map(map1);
 
  // (3) Copy constructor
  std::map<std::string, int> copied(map1);
  std::cout << "\ncopied = "; print_map(copied);
  std::cout << "map1 = "; print_map(map1);
 
  // (4) Move constructor
  std::map<std::string, int> moved(std::move(map1));
  std::cout << "\nmoved = "; print_map(moved);
  std::cout << "map1 = "; print_map(map1);
 
  // (5) Initializer list constructor
  const std::map<std::string, int> init {
    {"this", 100},
    {"can", 100},
    {"be", 100},
    {"const", 100},
  };
  std::cout << "\ninit = "; print_map(init);
}
```
Output:
map1 = {anything:199 something:69 that thing:50 }
 
iter = {anything:199 something:69 that thing:50 }
map1 = {anything:199 something:69 that thing:50 }
 
copied = {anything:199 something:69 that thing:50 }
map1 = {anything:199 something:69 that thing:50 }
 
moved = {anything:199 something:69 that thing:50 }
map1 = {}
 
init = {be:100 can:100 const:100 this:100 }



