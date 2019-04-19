# C++按map的value进行排序 - 数据之美的博客 - CSDN博客
2017年09月18日 16:05:40[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：359
C++中，map是按key值大小排序存储。有时候，我们需要对map的value进行排序，根据value的大小顺序获得key的序列。比较简单的方法就是，重新定义一个新的map，新map的key和value正好是原来map的value和key，这样新的map就按照原来map的value值进行排序。不过这种方法，要是原来的map的value值没有重复的话，是正确的，因为map的key值是无重复的。比较正确的做法是将map转成vector，对利用vector排序。关于原理上的说明，博客（[http://blog.csdn.net/acidgl8757/article/details/17416439](http://blog.csdn.net/acidgl8757/article/details/17416439)）解释的很清楚。在此，整理了一个直接能用的，方便日后使用。
具体代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/idmer/article/details/54022889#)[copy](http://blog.csdn.net/idmer/article/details/54022889#)
- #include "stdafx.h"
- #include <iostream>
- #include <iomanip>
- #include <vector>
- #include <map>
- #include <string>
- #include <algorithm>
- usingnamespace std;  
- 
- typedef pair<string, double> PAIR;    
- 
- struct CmpByValue {    
- bool operator()(const PAIR& lhs, const PAIR& rhs) {    
- return lhs.second < rhs.second;    
-   }    
- };  
- 
- int _tmain(int argc, _TCHAR* argv[])  
- {  
- //原来的map
-     map<string, int> name_score_map;    
-     name_score_map["LiMin"] = 90;    
-     name_score_map["ZiLinMi"] = 79;    
-     name_score_map["BoB"] = 92;    
-     name_score_map.insert(make_pair("Bing",99));    
-     name_score_map.insert(make_pair("Albert",86));   
- 
- //把map中元素转存到vector中   
-     vector<PAIR> name_score_vec(name_score_map.begin(), name_score_map.end());    
- 
- //对vector排序
-     sort(name_score_vec.begin(), name_score_vec.end(), CmpByValue());    
- 
- //排序前
-     map<string, int>::iterator iter_map;  
-     cout << "排序前:" << endl;  
- for(iter_map = name_score_map.begin(); iter_map != name_score_map.end(); iter_map++)  
-         cout << left << setw(10) << iter_map->first << iter_map->second << endl;  
- 
-     cout << "排序后:" << endl;  
- for (int i = 0; i != name_score_vec.size(); ++i) {    
- //可在此对按value排完序之后进行操作
-         cout << left << setw(10) << name_score_vec[i].first << name_score_vec[i].second << endl;    
-     }    
- return 0;  
- }  
结果如图：
![](https://img-blog.csdn.net/20170104194405030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSURNZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://blog.csdn.net/idmer/article/details/54022889#)[](http://blog.csdn.net/idmer/article/details/54022889#)[](http://blog.csdn.net/idmer/article/details/54022889#)[](http://blog.csdn.net/idmer/article/details/54022889#)[](http://blog.csdn.net/idmer/article/details/54022889#)[](http://blog.csdn.net/idmer/article/details/54022889#)
- 
顶
1- 
踩
