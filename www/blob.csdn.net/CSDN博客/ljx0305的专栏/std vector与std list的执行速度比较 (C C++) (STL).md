# std::vector与std::list的执行速度比较 (C/C++) (STL) - ljx0305的专栏 - CSDN博客
2009年09月05日 21:01:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：2195标签：[vector																[list																[compiler																[insert																[access																[优化](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
STL中的container各有专长，最常用的是std::vector，可以完全取代array，第二常用的是std::list。std::vector的优点在于non-sequential access超快，新增数据于数据后端超快，但insert和erase任意资料则相当缓慢；std::list则是insert和erase速度超快，但non-sequential access超慢，此范例以实际时间比较vector和list间的优缺点。
  1/**//*
  2(C) OOMusou 2006 http://oomusou.cnblogs.com
  3
  4Filename    : VectorVsList.cpp
  5Compiler    : Visual C++ 8.0
  6Description : Demo the performance difference between std::vector and std::list
  7*/
  8#include <iostream>
  9 #include <ctime>
 10#include <vector>
 11 #include <list>
 12
 13// Add to Vector at end
 14void addToVectorAtEnd();
 15// Add to List at end
 16void addToListAtEnd();
 17// Calculate time for adding to end
 18void addToEnd();
 19
 20// Add to Vector anywhere
 21void addToVectorAnywhere();
 22// Add to List anywhere
 23void addToListAnywhere();
 24// Calculate time for adding anywhere
 25void addAnywhere();
 26
 27// Remove from Vector anywhere
 28void removeFromVectorAnywhere();
 29// Remove from List anywhere
 30void removeFromListAnywhere();
 31// Calculate time for removing anywhere
 32void removeAnywhere();
 33
 34// Non-sequential access to Vector
 35void nonSequentialAccessToVector();
 36// Non-sequential access to List
 37void nonSequentialAccessToList();
 38// Caculate time for non-sequential access to 
 39void nonSequentialAccess();
 40
 41std::vector<int> vector1;
 42std::list<int>   list1;
 43
 44int main() {
 45// Calculate time for adding to end
 46  addToEnd();
 47
 48// Calculate time for adding anywhere
 49  addAnywhere();
 50
 51// Calculate time for removing anywhere
 52  removeAnywhere();
 53
 54// Caculate time for non-sequential access to 
 55  nonSequentialAccess();
 56}
 57
 58// Add to Vector at end
 59void addToVectorAtEnd() {
 60for(int i=0; i !=1000000; ++i) {
 61    vector1.push_back(i);
 62  }
 63}
 64
 65// Add to List at end
 66void addToListAtEnd() {
 67for(int i=0; i !=1000000; ++i) {
 68    list1.push_back(i);
 69  }
 70}
 71
 72// Calculate time for adding to end
 73void addToEnd() {
 74  clock_t addToVectorAtEndClock = clock();
 75  addToVectorAtEnd();
 76  addToVectorAtEndClock = clock() - addToVectorAtEndClock;
 77
 78  std::cout <<"Vector Insertion at the end Process time:"<< (double)addToVectorAtEndClock/CLOCKS_PER_SEC <<" sec"<< std::endl;
 79
 80  clock_t addToListAtEndClock = clock();
 81  addToListAtEnd();
 82  addToListAtEndClock = clock() - addToListAtEndClock;
 83
 84  std::cout <<"List Insertion at the end Process time:"<< (double)addToListAtEndClock/CLOCKS_PER_SEC <<" sec"<< std::endl;
 85
 86if (addToVectorAtEndClock < addToListAtEndClock) {
 87    std::cout <<"Insertion at the end : Vector wins"<< std::endl;
 88  }
 89else{
 90    std::cout <<"Insertion at the end : List wins"<< std::endl;
 91  }
 92}
 93
 94// Add to Vector anywhere
 95void addToVectorAnywhere() {
 96// Add to 50000th
 97  std::vector<int>::iterator iter = vector1.begin();
 98
 99for(int i =0; i <=500; ++i) {
100++iter;
101    iter = vector1.insert(iter,i);
102  }
103}
104
105// Add to List anywhere
106void addToListAnywhere() {
107// Add to 50000th
108  std::list<int>::iterator iter = list1.begin();
109
110for(int i =0; i !=500; ++i) {
111++iter;
112    iter = list1.insert(iter,i);
113  }
114}
115
116// Calculate time for adding anywhere
117void addAnywhere() {
118  clock_t addToVectorAnywhereClock = clock();
119  addToVectorAnywhere();
120  addToVectorAnywhereClock = clock() - addToVectorAnywhereClock;
121
122  std::cout <<"Vector Insertion anywhere Process time:"<< (double)addToVectorAnywhereClock/CLOCKS_PER_SEC <<" sec"<< std::endl;
123
124  clock_t addToListAnywhereClock = clock();
125  addToListAnywhere();
126  addToListAnywhereClock = clock() - addToListAnywhereClock;
127
128  std::cout <<"List Insertion anywhere Process time:"<< (double)addToListAnywhereClock/CLOCKS_PER_SEC <<" sec"<< std::endl;
129
130if (addToVectorAnywhereClock < addToListAnywhereClock) {
131    std::cout <<"Insertion anywhere : Vector wins"<< std::endl;
132  }
133else{
134    std::cout <<"Insertion anywhere : List wins"<< std::endl;
135  }
136}
137
138// Remove from Vector anywhere
139void removeFromVectorAnywhere() {
140  std::vector<int>::iterator iter = vector1.begin();
141
142for(int i =0; i !=500; ++i) {
143++iter;
144    iter = vector1.erase(iter);
145  }
146}
147
148// Remove from List anywhere
149void removeFromListAnywhere() {
150  std::list<int>::iterator iter = list1.begin();
151
152for(int i =0; i !=500; ++i) {
153++iter;
154    iter = list1.erase(iter);
155  }
156}
157
158// Calculate time for removing anywhere
159void removeAnywhere() {
160  clock_t removeFromVectorAnywhereClock = clock();
161  removeFromVectorAnywhere();
162  removeFromVectorAnywhereClock = clock() - removeFromVectorAnywhereClock;
163
164  std::cout <<"Vector deletion anywhere Process time:"<< (double)removeFromVectorAnywhereClock/CLOCKS_PER_SEC <<" sec"<< std::endl;
165
166  clock_t removeFromListAnywhereClock = clock();
167  removeFromListAnywhere();
168  removeFromListAnywhereClock = clock() - removeFromListAnywhereClock;
169
170  std::cout <<"List deletion anywhere Process time:"<< (double)removeFromListAnywhereClock/CLOCKS_PER_SEC <<" sec"<< std::endl;
171
172if (removeFromVectorAnywhereClock < removeFromListAnywhereClock) {
173    std::cout <<"Deletion anywhere : Vector wins"<< std::endl;
174  }
175else{
176    std::cout <<"Deletion anywhere : List wins"<< std::endl;
177  }
178}
179
180// Non-sequential access to Vector
181void nonSequentialAccessToVector() {
182for(int i =0; i !=10000; ++i) {
183int m = vector1.at(i);
184  }
185}
186
187// Non-sequential access to List
188void nonSequentialAccessToList() {
189for(int i =0; i !=10000; ++i) {
190    std::list<int>::const_iterator iter = list1.begin();
191for(int k =0; k <= i; ++k) {
192++iter;
193    }
194
195int m = (*iter);
196  }
197}
198
199// Caculate time for non-sequential access to 
200void nonSequentialAccess() {
201  clock_t nonSequentialAccessToVectorClock = clock();
202  nonSequentialAccessToVector();
203  nonSequentialAccessToVectorClock = clock() - nonSequentialAccessToVectorClock;
204
205  std::cout <<"Vector non-sequential access Process time:"<< (double)nonSequentialAccessToVectorClock/CLOCKS_PER_SEC <<" sec"<< std::endl;
206
207  clock_t nonSequentialAccessToListClock = clock();
208  nonSequentialAccessToList();
209  nonSequentialAccessToListClock = clock() - nonSequentialAccessToListClock;
210
211  std::cout <<"List non-sequential access Process time:"<< (double)nonSequentialAccessToListClock/CLOCKS_PER_SEC <<" sec"<< std::endl;
212
213if (nonSequentialAccessToVectorClock < nonSequentialAccessToListClock) {
214    std::cout <<"Non-sequential : Vector wins"<< std::endl;
215  }
216else{
217    std::cout <<"Non-sequential : List wins"<< std::endl;
218  }
219}
执行结果
(Debug mode)
 1Vector Insertion at the end Process time:3.094 sec
 2List Insertion at the end Process time:10.205 sec
 3Insertion at the end : Vector wins
 4Vector Insertion anywhere Process time:5.548 sec
 5List Insertion anywhere Process time:0.01 sec
 6Insertion anywhere : List wins
 7Vector deletion anywhere Process time:4.756 sec
 8List deletion anywhere Process time:0.011 sec
 9Deletion anywhere : List wins
10Vector non-sequential access Process time:0.11 sec
11List non-sequential access Process time:21.12 sec
12Non-sequential : Vector wins
(Release Mode)
 1Vector Insertion at the end Process time:0.06 sec
 2List Insertion at the end Process time:0.39 sec
 3Insertion at the end : Vector wins
 4Vector Insertion anywhere Process time:4.757 sec
 5List Insertion anywhere Process time:0 sec
 6Insertion anywhere : List wins
 7Vector deletion anywhere Process time:4.717 sec
 8List deletion anywhere Process time:0 sec
 9Deletion anywhere : List wins
10Vector non-sequential access Process time:0 sec
11List non-sequential access Process time:0.491 sec
12Non-sequential : Vector wins
可见经过compiler优化后，速度差异非常明显
引用：[http://www.cnblogs.com/oomusou/archive/2006/11/09/554737.html](http://www.cnblogs.com/oomusou/archive/2006/11/09/554737.html)
