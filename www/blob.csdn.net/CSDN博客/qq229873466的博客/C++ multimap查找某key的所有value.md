# C++ multimap查找某key的所有value - qq229873466的博客 - CSDN博客

2016年08月11日 11:47:37[qq229873466](https://me.csdn.net/qq229873466)阅读数：712


```cpp
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

int main(){
	multimap<int, string> mt;
	mt.insert(pair<int,string>(1,"gg"));
	mt.insert(pair<int,string>(2,"ee"));
	mt.insert(pair<int,string>(1,"qq"));
	//方法1
	multimap<int, string>::iterator it;
	pair<multimap<int, string>::iterator, multimap<int, string>::iterator> p = mt.equal_range(1);  
	for (it = p.first;it != p.second; ++it) {
		cout << it->second << endl;
	}
	//方法2
	it = mt.find(1);
	for (int i = 0, len = mt.count(1);i < len; ++i,++it) {
		cout << it->second << endl;
	}
	return 0;
}
```

