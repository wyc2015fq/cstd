# stl的vector，map，list删除元素 - 长歌行 - CSDN博客





2012年12月28日 16:29:19[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：957








vector看成数组，list看成链表，具体的内部存储方法可以参考stl




数据定义



```cpp
#include <map>
 #include <vector>
 #include <list>
 
 using namespace std;
 
 typedef map<int, byte*> ByteMap;
 ByteMap  testMap;
 typedef	vector<int>		IntVec;
 IntVec   testVec;
 typedef list<int>		IntList;
 IntList	 testList;
```


遍历删除vector



```cpp
IntVec::iterator iter = testVec.begin();
 	for (; iter != testVec.end();)
 	{
 		iter = testVec.erase(iter);
 	}
```


遍历删除list



```cpp
IntList::iterator iter = testList.begin();
 	for (; iter != testList.end(); )
 	{
 		//testList.erase(iter++); //可行
 		iter = testList.erase(iter);
 	}
```


初始化map



```cpp
int nMem = 80;
 	int nNum = 4;
 	byte** by = new byte*[nNum];
 	for (int i = 0; i< nNum; i++)
 	{
 		by[i] = new byte[nMem];
 		memset(by[i], i, nMem);
 		testMap[i] = by[i];
 	}
```


遍历删除map



```cpp
ByteMap::iterator iter = testMap.begin();
 	while(iter != testMap.end())
 	{
 		byte* pb = (*iter).second;
 		if (pb)
 		{
 			delete []pb;
 			pb = NULL;
 		}
 		//testMap.erase(iter++);//都可以
 		iter = testMap.erase(iter);
 	}
```


注明：

testMap.erase(iter++)可以

改成testMap.erase(iter); iter++就不行，会在



```cpp
iterator operator++(int)
 			{	// postincrement
 			iterator _Tmp = *this;
 			++*this;
 			return (_Tmp);
 			}
```


崩溃。

大概前者会在++之前给iter建立一个缓存对象，之后再在此基础之上++，而后者erase之后iter就没有实际意义了。如下。

iter = (-17891602,0xfeeefeee <错误的指针>)





二维数组上级指针的内存释放，下级指针已经在map中释放了



```cpp
if (by)
 	{
 		delete []by;
 		by = NULL;
 	}
```











