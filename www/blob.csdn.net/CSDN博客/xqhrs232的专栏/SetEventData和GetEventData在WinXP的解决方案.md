# SetEventData和GetEventData在WinXP的解决方案 - xqhrs232的专栏 - CSDN博客
2014年04月01日 09:45:01[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：568
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/chenyujing1234/article/details/8196164](http://blog.csdn.net/chenyujing1234/article/details/8196164)
转载自： [http://blog.csdn.net/norains/article/details/5385429](http://blog.csdn.net/norains/article/details/5385429)
//=====================================================================
//TITLE:
//    SetEventData和GetEventData在WinXP的解决方案
//AUTHOR:
//    norains
//DATE:
//    Tuesday 16-March-2010
//Environment:
//     WINDOWS XP
//=====================================================================
    如果你熟悉于WinCE开发，又经常通过SetEventData和GetEventData在进程间传递数据，那么当你想将代码移植到WinXP平台，将会遇到一个十分尴尬的问题：Win32 API根本就没有这两个函数！当然，如果你是采用.net框架，那么这不是问题，这里本文不做讨论。
 回到原点，我们来想想这两个函数的功能。我们可以这么认为，SetEventData是将DWORD数值和事件句柄相关联，而GetEventData则是将关联的数据取出。这些数据，是可以在不同的进程中共享的。
　　这个是不是和内存映射文件有几分类似？既然Win32 API没有这个函数，那么我们就手动来创建！
　　由于事件句柄是与数值相关联地，从算法的简便性考虑，我们可以使用STL::map进行存储，这样可以大大降低我们的工作量。另一方面，因为我们在做映射文件时，有一些信息我们只能通过CreateEvent获取，故我们只能采用类的方式，将这几个函数重载。为描述简便，这个类我们声明为CMUF。
　　先从头开始，我们看看重载的CreateEvent函数。其中的一些要点，我采用注释的方式列于代码中，方便于查看。
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- #include <iostream>
- #include <string>
- #include <map> 
- #include <windows.h>
- #include<iostream>
- #include<sstream>
- #include <algorithm>
- usingnamespace std;  
- 
- 
- class CMUF  
- {  
- private:  
- struct MemFile  
-     {     
- HANDLE hFileMap;  
- HANDLE pMapBuf;  
- DWORD dwBufSize;  
- DWORD dwCount;  
-     };  
- 
- static std::map<HANDLE,string> ms_mpEventHandleToName;  
- static std::map<string,MemFile> ms_mpEventNameToFile;  
- 
- BOOL InitMemFile(const string &strMapName,DWORD dwSize,MemFile &memFile);  
- BOOL CloseHandle(HANDLE hObject);  
- BOOL GetMemFile(HANDLE hEvent,MemFile &memFile);  
- BOOL SetEventData(HANDLE hEvent,DWORD dwData);  
- HANDLE CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes,BOOL bManualReset,BOOL bInitialState,LPCTSTR lpName);  
- DWORD GetEventData(HANDLE hEvent);  
- 
- };  
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- OOL CMUF::InitMemFile(const string &strMapName,DWORD dwSize,MemFile &memFile)  
- {  
- 
- //创建映射文件
- HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,dwSize,strMapName.c_str());  
- if(hFileMap == NULL)  
-     {  
- //ASSERT(hFileMap != NULL);
- return FALSE;  
-     }  
- 
- //从映射文件句柄获得分配的内存空间
- VOID *pMapBuf = MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS,0,0,0);   
- if(pMapBuf == NULL)  
-     {  
- //ASSERT(FALSE);
-         CloseHandle(hFileMap);  
- return FALSE;  
-     }  
- 
- //将内存中的数值设置为0
- DWORD dwData = 0;  
-     memcpy(pMapBuf,&dwData,dwSize);  
- 
- //将数值保存到结构体中
-     memFile.hFileMap = hFileMap;  
-     memFile.pMapBuf = pMapBuf;  
-     memFile.dwBufSize = dwSize;  
-     memFile.dwCount = 1;  
- 
- return TRUE;  
- }  
- 
- 
- BOOL CMUF::CloseHandle(HANDLE hObject)  
- {  
- if(hObject == NULL)  
-     {  
- return FALSE;  
-     }  
- 
- //查找所关闭的句柄是否存在于记录里. 
-     std::map<HANDLE,string>::iterator iterName = ms_mpEventHandleToName.find(hObject);  
- if(iterName != ms_mpEventHandleToName.end())  
-     {  
- 
-         std::map<string,MemFile>::iterator iterFile = ms_mpEventNameToFile.find(iterName->second);  
- if(iterFile != ms_mpEventNameToFile.end())  
-         {   
- //找到记录，数目减一
-             InterlockedDecrement(reinterpret_cast<LONG *>(&iterFile->second.dwCount));  
- 
- 
- if(iterFile->second.dwCount == 0)  
-             {  
- //如果记数为零，则删掉映射文件.
- 
-                 UnmapViewOfFile(iterFile->second.pMapBuf);  
-                 ::CloseHandle(iterFile->second.hFileMap);  
-                 ms_mpEventNameToFile.erase(iterFile);  
-             }  
-         }  
- 
-         ms_mpEventHandleToName.erase(iterName);  
-     }  
- 
- //调用API函数进行真正的关闭。
- return ::CloseHandle(hObject);  
- }  
- 
- 
- BOOL CMUF::GetMemFile(HANDLE hEvent,MemFile &memFile)  
- {  
- if(hEvent == NULL)  
-     {  
- return FALSE;  
-     }  
- 
- //根据句柄查找命名
-     std::map<HANDLE,string>::iterator iterEvent = ms_mpEventHandleToName.find(hEvent);  
- if(iterEvent == ms_mpEventHandleToName.end())  
-     {  
- return FALSE;  
-     }  
- 
- //根据命名来查找映射文件信息
-     std::map<string,MemFile>::iterator iterMem = ms_mpEventNameToFile.find(iterEvent->second);  
- if(iterMem == ms_mpEventNameToFile.end())  
-     {  
- return FALSE;  
-     }  
- 
- //将记录文件中的信息保存到输出变量缓存中。
-     memFile = iterMem->second;  
- 
- return TRUE;  
- }  
- 
- 
- BOOL CMUF::SetEventData(HANDLE hEvent,DWORD dwData)  
- {  
-     MemFile memFile;  
- if(GetMemFile(hEvent,memFile) == FALSE || memFile.pMapBuf == NULL)  
-     {  
- return FALSE;  
-     }  
- 
- //将数值拷贝到内存中
-     memcpy(memFile.pMapBuf,&dwData,memFile.dwBufSize);  
- 
- return TRUE;  
- }  
- 
- DWORD CMUF::GetEventData(HANDLE hEvent)  
- {  
-     MemFile memFile;  
- if(GetMemFile(hEvent,memFile) == FALSE || memFile.pMapBuf == NULL)  
-     {  
- return 0;  
-     }  
- 
- //从内存中获取DWORD数据
- DWORD dwVal = 0;  
-     memcpy(&dwVal,memFile.pMapBuf,memFile.dwBufSize);  
- 
- return dwVal;  
- }  
- 
- 
- 
- HANDLE CMUF::CreateEvent(LPSECURITY_ATTRIBUTES lpEventAttributes,BOOL bManualReset,BOOL bInitialState,LPCTSTR lpName)  
- {  
- BOOL bRes = FALSE;  
- 
- //调用原生的API函数创建事件.
- HANDLE hEvent = ::CreateEvent(lpEventAttributes,bManualReset,bInitialState,lpName);  
- if(hEvent == NULL)  
-     {  
- //如果创建失败，跳转到EXIT
- goto EXIT;  
-     }  
- 
- //先判断当前创建的这个事件是否已经被创建，以方便后续数据的处理。.
- BOOL bIsExisting = (GetLastError() == ERROR_ALREADY_EXISTS);  
- 
- if(bIsExisting == FALSE)  
-     {    
- //这是一个新的的事件
-         string strMemFile;  
- 
- //先判断这个类是否只是内部使用。所谓的内部使用，指的是没有名字的事件，除了通过句柄来进行使用以外，无法通过再次打开获得。
- if(lpName != NULL)  
-         {  
- //因为内存映射文件和事件名是同一个命名空间，所以这两者的名字不能相同。故我们要创建的内存映射文件名为：EVENT_前缀 + 事件名。
-             strMemFile = TEXT("EVENT_");  
-             strMemFile += lpName;  
-         }  
- else
-         {  
- //如果该事件为内部使用，那么也就意味着这内存映射文件也是内部使用。故采用程序句柄的名字+事件名的方式进行内存映射文件的名字确定。
-             ostringstream stream;   
-             stream << GetModuleHandle(NULL) << TEXT("_") << hEvent;  
-             strMemFile = stream.str();  
-         }  
- 
- //通过调用InitMemFile来创建内存映射文件
-         MemFile memFile;  
- if(InitMemFile(strMemFile,sizeof(DWORD),memFile) == FALSE)  
-         {  
- goto EXIT;  
-         }  
- 
- //ms_mpEventHandleToName和ms_mpEventNameToFile是类的静态成员，用来存储事件和映射文件的对应关系。
- if(lpName != NULL)  
-         {  
-             ms_mpEventHandleToName.insert(std::make_pair(hEvent,lpName));  
-             ms_mpEventNameToFile.insert(std::make_pair(lpName,memFile));  
-         }  
- else
-         {  
- //因为内部使用的事件没有名字，所以采用组合的名字作为标识
-             ms_mpEventHandleToName.insert(std::make_pair(hEvent,strMemFile));  
-             ms_mpEventNameToFile.insert(std::make_pair(strMemFile,memFile));  
-         }  
- 
-     }  
- else
-     {  
- //系统判断以该名字命名的事件已经被创建过，所以我们先搜索我们的数据库是否有相应的记录。
-         std::map<HANDLE,string>::iterator iterName = std::find_if(ms_mpEventHandleToName.begin(),ms_mpEventHandleToName.end(),Functor::value_equal<HANDLE,string>(lpName));  
- if(iterName == ms_mpEventHandleToName.end())  
-         {  
- //ASSERT(FALSE);
- 
- goto EXIT;  
-         }  
- 
- 
-         std::map<string,MemFile>::iterator iterFile = ms_mpEventNameToFile.find(iterName->second);  
- if(iterFile == ms_mpEventNameToFile.end())  
-         {  
- //ASSERT(FALSE);
- 
- goto EXIT;  
-         }  
- 
- //找到相应记录的话，则将记数加1
-         InterlockedIncrement(reinterpret_cast<LONG *>(&iterFile->second.dwCount));  
- 
- //即使该名字命名的事件之前已经使用，但第二次创建时，句柄还是不同的，所以我们必须将其保存
-         ms_mpEventHandleToName.insert(std::make_pair(hEvent,lpName));  
-     }  
- 
-     bRes = TRUE;  
- 
- EXIT:  
- 
- if(bRes == FALSE)  
-     {  
-         CloseHandle(hEvent);  
-     }  
- 
- return hEvent;  
- 
- }  
#### 二、ostringstream的使用
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- #include<iostream>
- #include<string>
- #include<sstream>
- usingnamespace std;  
- 
- 
- int testStream()  
- {     
- int i=10;     
-     ostringstream ost;     
-     ost << i;     
-     string temp(ost.str());     
-     cout << temp << endl;  
- 
- 
- return 0;  
- }  
### 三、[find_if](http://blog.csdn.net/yzm365487848/article/details/5516465)
有时我们要在map、vector容器中查找符合条件的记录，map提供一个find的成员函数，但也仅限于查找关键字满足条件的记录，不支持值域的比较。如果我们要在值域中查找记录，该函数就无能无力了。而vector甚至连这样的成员函数都没有提供。所以一般情况下进行值域的查找，要么自己遍历数据，要么求助于STL的find_if函数。前种方法我们这里就不赘述了，只讲find_if函数。
##### 1 -- find_if的STL定义
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- template <class InputIterator, class Predicate>   
- InputIterator find_if(InputIterator first, InputIterator last,Predicate pred)   
- {   
- while (first != last && !pred(*first)) ++first;   
- return first;   
- }   
find_if是一个模板函数，接受两个数据类型：InputItearator迭代器，Predicate用于比较数值的函数或者函数对象（仿函数）。find_if对迭代器要求很低，只需要它支持自增操作即可。当前遍历到的记录符合条件与否，判断标准就是使得pred()为真。至此可能还有些不是很明了，下面举几个例子实际操练下的它的用法。
##### 2 -- find_if在std::map查找时的应用
假如我们有个map对象是这么声明的：
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- std::map<int, std::string> my_map;  
- my_map.insert(std::make_pair(10, "china"));  
- my_map.insert(std::make_pair(20, "usa"));  
- my_map.insert(std::make_pair(30, "english"));  
- my_map.insert(std::make_pair(40, "hongkong"));  
插入值后我们想得到值为”english”的这条记录，要怎样写程序呢？下面是个范例参考下：
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- #include <map> 
- #include <string>  
- #include <algorithm>
- 
- class map_finder   
- {   
- public:   
-     map_finder(const std::string &cmp_string):m_s_cmp_string(cmp_string){}   
- bool operator ()(const std::map<int, std::string>::value_type &pair)   
-     {   
- return pair.second == m_s_cmp_string;   
-     }   
- private:   
- const std::string &m_s_cmp_string;                       
- };   
- 
- int main()   
- {   
-     std::map<int, std::string> my_map;   
-     my_map.insert(std::make_pair(10, "china"));   
-     my_map.insert(std::make_pair(20, "usa"));   
-     my_map.insert(std::make_pair(30, "english"));   
-     my_map.insert(std::make_pair(40, "hongkong"));       
- 
-     std::map<int, std::string>::iterator it = my_map.end();   
-     it = std::find_if(my_map.begin(), my_map.end(), map_finder("english"));   
- if (it == my_map.end())   
-         printf("not found/n");          
- else
-         printf("found key:%d value:%s/n", it->first, it->second.c_str());   
- 
- return 0;           
- }   
class map_finder即用于比较的函数对象，它的核心就是重载的()运算符。**因为每个容器迭代器的*运算符得到的结果都是该容器的value_type值**，所以该运算符的形参就是map迭代器指向的value_type类型的引用。而map的value_type到底是什么类型，就得看下STL的源代码是如何定义的。
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>  
- class map  
- {  
- public:  
- typedef Key key_type;  
- typedef pair<const Key, T> value_type;  
- ......  
- };  
从上面的定义可以看出，map的value_type是std::pair<const Key, t>类型，它的first值就是关键字，second值保存map的值域。
##### 3 -- find_if在std::vector的应用
vector的find_if用法与map的很相似，区别仅仅是二者的value_type不一样而已。我们看下vecotr对value_type的定义。
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- template <class T, class Alloc = alloc>  
- class vector  
- {  
- public:  
- typedef T value_type;  
- typedef value_type* iterator;  
- ......  
- };  
可以看出vector的value_type就是容器的值类型，了解了这点，我们做个vector的find_if示范。
**[cpp]**[view
 plain](http://blog.csdn.net/chenyujing1234/article/details/8196164#)[copy](http://blog.csdn.net/chenyujing1234/article/details/8196164#)
- #include <vector> 
- #include <string> 
- #include <algorithm>
- 
- struct value_t   
- {   
- int a;   
- int b;   
- };   
- 
- class vector_finder   
- {   
- public:   
-     vector_finder(constint a):m_i_a(a){}   
- bool operator ()(const std::vector<struct value_t>::value_type &value)   
-     {   
- return value.a == m_i_a;   
-     }   
- private:   
- int m_i_a;                       
- };   
- 
- int main()   
- {   
-     std::vector<struct value_t> my_vector;   
- struct value_t my_value;   
- 
-     my_value.a = 11; my_value.b = 1000;   
-     my_vector.push_back(my_value);   
- 
-     my_value.a = 12; my_value.b = 1000;   
-     my_vector.push_back(my_value);   
- 
-     my_value.a = 13; my_value.b = 1000;   
-     my_vector.push_back(my_value);   
- 
-     my_value.a = 14; my_value.b = 1000;   
-     my_vector.push_back(my_value);   
- 
-     std::vector<struct value_t>::iterator it = my_vector.end();   
-     it = std::find_if(my_vector.begin(), my_vector.end(), vector_finder(13));   
- if (it == my_vector.end())   
-          printf("not found/n");          
- else
-         printf("found value.a:%d value.b:%d/n", it->a, it->b);   
- 
-     getchar();   
- return 0;           
- }   
- 
