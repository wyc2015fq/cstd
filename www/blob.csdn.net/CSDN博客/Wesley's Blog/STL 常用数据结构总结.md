
# STL 常用数据结构总结 - Wesley's Blog - CSDN博客


2018年10月14日 14:09:36[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：52


## 常用数据结构
对于面试的人来说, 基本上会用到的就下面几种数据结构, 如果可以使用STL的话就使用下面几种:**stack, priority_queue, queue, vector, list, set, map, unordered_map**
## stack(栈)
主要操作: push, top, pop
`#include <stack>
using namespace std;
int main(){
    stack<int> s;
    s.push(1);
    int top = s.top();
    s.pop();
}`
## priority_queue(优先队列,堆)
主要操作, push, top, pop
`#include <queue>
#include <cstdio>
using namespace std;
int main(){
    priority_queue<int> maxHeap;
    priority_queue<int, vector<int>, greater<int> > minHeap;
    maxHeap.push(1);
    maxHeap.push(2);
    int top = maxHeap.top();
    printf("%d", top);
    maxHeap.pop();
}`
## queue(队列)
主要操作, push, top, pop
`#include <queue>
#include <cstdio>
using namespace std;
int main(){
    queue<int> q;
    q.push(1);
    int front = q.front();
    q.pop();
}`
## vector(可变长数组)
主要操作, push_back, reserve, [], erase
`#include <vector>
#include <cstdio>
using namespace std;
int main(){
    vector<int> arr;
    arr.reserve(100);
    arr.push_back(1);
    for (auto &&val : arr) {
        printf("%d\t", val);
    }
}`
## list(双向链表)
主要操作, push_back, insert, erase
`#include <list>
#include <cstdio>
using namespace std;
int main(){
    list<int> linklist;
    linklist.push_back(1);
    linklist.insert(++linklist.begin(), 2);
    for (auto &&item : linklist) {
        printf("%d\t", item);
    }
    linklist.erase(linklist.begin());
}`
## set(集合)
主要操作, insert, erase, find
`#include <set>
#include <cstdio>
using namespace std;
int main(){
    set<int> s;
    s.insert(1);
    s.insert(1);
    s.insert(s.begin(), 2);
    for (auto &&item : s) {
        printf("%d\t", item);
    }
    s.erase(s.begin());
    set<int>::iterator iter = s.find(2);
    printf("%d", *iter);
}`
## map(红黑树)
主要操作: insert, [], find, erase
`#include <map>
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;
int main(){
    map<string, int> m;
    m.insert(make_pair("wesley", 100));
    printf("%d\n", m["wesley"]);
    map<string, int>::iterator iter;
    iter = m.find("wesley");
    if (iter != m.end()){
        printf("name: %s, grade: %d\n", (iter->first).c_str(), iter->second);
    }
    m.erase("wesley");
}`
## unordered_map(哈希表)
主要操作: insert, [], find, erase
`#include <unordered_map>
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;
int main(){
    unordered_map<string, int> m;
    m.insert(make_pair("wesley", 100));
    printf("%d\n", m["wesley"]);
    unordered_map<string, int>::iterator iter;
    iter = m.find("wesley");
    if (iter != m.end()){
        printf("name: %s, grade: %d\n", (iter->first).c_str(), iter->second);
    }
    m.erase("wesley");
}`

