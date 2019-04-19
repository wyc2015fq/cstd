# 正确释放Vector的内存 - 文章 - 伯乐在线
**伯乐在线注：**本文来自文章作者（[@_羊仔](http://weibo.com/kidlets)）的投稿（[原文](http://kidlet.org/2013/06-03/vector.html)）。
今天在看微博的时候， 有人提出了一个对于Vector内存泄露的疑问（ [Link](http://weibo.com/1986953781/zzIQoyhcM)）。
博主采用 Vector存储一些数据，但是发现在执行 clear() 之后内存并没有释放，于是怀疑产生了内存泄露。随后有人回复：
“vector 的 clear 不影响 capacity , 你应该 swap 一个空的 vector。”
开始并不知道回复者在说什么，于是在谷歌上搜索 `vector swap clear` 发现已经有类似的问题出现了，并且给出了一些解决方案。
原来这样的问题在 《Effective STL》中的“条款17”已经指出了
当vector、string大量插入数据后，即使删除了大量数据（或者全部都删除，即clear） 并没有改变容器的容量（capacity），所以仍然会占用着内存。 为了避免这种情况，我们应该想办法改变容器的容量使之尽可能小的符合当前 数据所需（shrink to fit）
《Effective STL》给出的解决方案是：

C++
```
vector<type> v;
//.... 这里添加许多元素给v
//.... 这里删除v中的许多元素
vector<type>(v).swap(v);
//此时v的容量已经尽可能的符合其当前包含的元素数量
//对于string则可能像下面这样
string(s).swap(s);
```
即先创建一个临时拷贝与原先的vector一致，值得注意的是，此时的拷贝 其容量是尽可能小的符合所需数据的。紧接着将该拷贝与原先的vector v进行 交换。好了此时，执行交换后，临时变量会被销毁，内存得到释放。此时的v即为原先 的临时拷贝，而交换后的临时拷贝则为容量非常大的vector（不过已经被销毁）
为了证明这一点，我写了一个程序，如下：

C++
```
#include <iostream>
#include <vector>
using namespace std;
vector <string> v;
char ch;
int main ()
{
    for(int i=0; i<1000000; i++)
        v.push_back("abcdefghijklmn");
    cin >> ch;
    // 此时检查内存情况 占用54M
    v.clear();
    cin >> ch;
    // 此时再次检查， 仍然占用54M
    cout << "Vector 的 容量为" << v.capacity() << endl;
    // 此时容量为 1048576
    vector<string>(v).swap(v);
    cout << "Vector 的 容量为" << v.capacity() << endl;
    // 此时容量为0
    cin >> ch;
    // 检查内存，释放了 10M+ 即为数据内存
    return 0;
}
```
### 总结
从这个事情说明，自己对STL的了解还非常的不够 平时对vector的清除都懂得采用 clear 方法。
另一方面 对于STL的设计思想也有些了解，在创建一个vector后 vector的实际容量一般会比给数据要大，这样做应该是避免过多的 重新分配内存吧。
当然，上面这种方法虽然释放了内存，但是同时也增加了拷贝数据的时间消耗。 不过一般需要重新调整容量的情况都是 vector本身元素较少的情况，所以 时间消耗可以忽略不计。
因此建议以后大家都将调用 `clear()` 改为 `swap()` 吧。
