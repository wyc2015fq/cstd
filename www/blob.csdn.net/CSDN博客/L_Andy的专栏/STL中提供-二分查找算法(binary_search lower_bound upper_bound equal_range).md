# STL中提供-二分查找算法(binary_search lower_bound upper_bound equal_range) - L_Andy的专栏 - CSDN博客

2014年07月23日 15:18:26[卡哥](https://me.csdn.net/L_Andy)阅读数：1052



STL包含四种不同的二分查找算法，binary_search    lower_bound  upper_bound   equal_range.他们作用的range是已sorted。

binary_search试图在已排序的[first, last)中寻找元素value。如果[first, last)内有等价于value的元素，它会返回true，否则返回false，它不返回查找位置。

lower_bound它试图在已排序的[first,last)中寻找元素value。如果[first, last)具有等价于value的元素，lower_bound返回一个iterator指向其中第一个元素。如果没有这样的元素存在，它便返回假设这样的元素存在的话，会出现的位置。即指向第一个不小于value的元素。如果value大于[first, last)的任何一个元素，则返回last。

upper_bound它试图在已排序的[first,last)中寻找value，返回可安插value的最后一个合适的位置。如果value存在，lower_bound 返回的是指向该元素的iterator。相较之下upper_bound并不这么做，它返回value可被安插的最后一个合适位置。如果value存在，那么它返回的iterator将指向value的下一个位置，而非value自身。

equal_range的返回值本质上结合了lower_bound和upper_bound两者的返回值。其返回值是一对iterator i 和 j ， 其中i是value可安插的第一个位置，j则是value可安插的最后一个位置。可以推演出：[i，j)中的每个元素都等价于value，而且[i, j)是[first, last)之中符合上述性质的一个最大子区间。  算法lower_bound返回该range的第一个iterator， 算法upper_bound返回该range的past-the-end iterator，算法equal_range则是以pair的形式将两者都返回。


**[cpp]**[view
 plain](http://blog.csdn.net/zhongkeli/article/details/6883288#)[copy](http://blog.csdn.net/zhongkeli/article/details/6883288#)

- #include <algorithm>
- #include <iostream>
- #include <iterator>
- #include <vector>
- usingnamespace std;  
- int main()  
- {  
-     vector<int> v;  
-     vector<int>::iterator iter;  
-     pair<vector<int>::iterator, vector<int>::iterator> vecpair;  
- 
- 
- for(int i = 1; i<= 20; i++) {  
-         v.push_back(i%6);  
-     }  
-     sort(v.begin(), v.end());  
-     cout << "array: " << endl << "  ";  
-     copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));  
-     cout << endl << endl;  
- 
- 
- /*  lower_bound */
-     cout << "lower_bound function, value = 3: " << endl;  
-     iter = lower_bound(v.begin(), v.end(), 3);  
-     cout << "  [first, iter] = ";  
-     copy(v.begin(), iter, ostream_iterator<int>(cout, " "));  
-     cout << endl;  
-     cout << "  [iter, last] = ";  
-     copy(iter, v.end(), ostream_iterator<int>(cout, " "));  
-     cout << endl << endl;  
- 
- 
- /*  upper_bound */
-     cout << "upper_bound function, value = 3: " << endl;  
-     iter = upper_bound(v.begin(), v.end(), 3);  
-     cout << "  [first, iter] = ";  
-     copy(v.begin(), iter, ostream_iterator<int>(cout, " "));  
-     cout << endl;  
-     cout << "  [iter, last] = ";  
-     copy(iter, v.end(), ostream_iterator<int>(cout, " "));  
-     cout << endl << endl;  
- 
- 
- /*  equal_range */
-     cout << "euqual_range function value = 3: " << endl;  
-     vecpair = equal_range(v.begin(), v.end(), 3);  
- 
- 
-     cout << " [vecpair->first, vecpair->second] = ";  
-     copy(vecpair.first, vecpair.second, ostream_iterator<int>(cout, " "));  
-     cout << endl << endl;  
- 
- 
- /*  binary_search */
-     cout << "binary_search function value = 3: " << endl;  
-     cout << "3 is " << (binary_search(v.begin(), v.end(), 3) ? "": "not ") << " in array" << endl;  
-     cout << endl;  
- 
- 
- /*  binary_search */
-     cout << "binary_search function value = 6: " << endl;  
-     cout << "6 is " << (binary_search(v.begin(), v.end(), 6) ? "": "not ") << " in array" << endl;  
-     cout << endl;  
- }  
- 
- 
- ./bsearch   
- array:   
-   0 0 0 1 1 1 1 2 2 2 2 3 3 3 4 4 4 5 5 5   
- 
- 
- lower_bound function, value = 3:   
-   [first, iter] = 0 0 0 1 1 1 1 2 2 2 2   
-   [iter, last] = 3 3 3 4 4 4 5 5 5   
- 
- 
- upper_bound function, value = 3:   
-   [first, iter] = 0 0 0 1 1 1 1 2 2 2 2 3 3 3   
-   [iter, last] = 4 4 4 5 5 5   
- 
- 
- euqual_range function value = 3:   
-  [vecpair->first, vecpair->second] = 3 3 3   
- 
- 
- binary_search function value = 3:   
- 3 is  in array  
- 
- 
- binary_search function value = 6:   
- 6 is not  in array  

