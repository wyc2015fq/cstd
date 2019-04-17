# 详细解说 STL 排序(Sort) - 在思索中前行！ - CSDN博客





2016年04月05日 16:21:30[_Tham](https://me.csdn.net/txl16211)阅读数：546








作者[【C++ Programmer's Cookbook】](http://www.cppblog.com/mzty/archive/2005/12/15/1770.html)



## 详细解说 STL 排序(Sort)
作者[Winter](http://www.stlchina.org/twiki/bin/view.pl/Main/WinterWen)
- [详细解说 STL 排序(Sort)](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#STL_Sort)- [0 前言: STL，为什么你必须掌握](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#0_STL)
- [1 STL提供的Sort 算法](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#1_STL_Sort)- [1.1 所有sort算法介绍](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#1_1_sort)
- [1.2 sort 中的比较函数](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#1_2_sort)
- [1.3 sort 的稳定性](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#1_3_sort)
- [1.4 全排序](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#1_4)
- [1.5 局部排序](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#1_5)
- [1.6 nth_element 指定元素排序](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#1_6_nth_element)
- [1.7 partition 和stable_partition](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#1_7_partition_stable_partition)

- [2 Sort 和容器](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#2_Sort)
- [3 选择合适的排序函数](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#3)
- [4 小结](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#4)
- [5 参考文档](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#5)


*一切复杂的排序操作，都可以通过STL方便实现* !
### 0 前言: STL，为什么你必须掌握
对于程序员来说，数据结构是必修的一门课。从查找到排序，从链表到二叉树，几乎所有的算法和原理都需要理解，理解不了也要死记硬背下来。幸运的是这些理论都已经比较成熟，算法也基本固定下来，不需要你再去花费心思去考虑其算法原理，也不用再去验证其准确性。不过，等你开始应用计算机语言来工作的时候，你会发现，面对不同的需求你需要一次又一次去用代码重复实现这些已经成熟的算法，而且会一次又一次陷入一些由于自己疏忽而产生的bug中。这时，你想找一种工具，已经帮你实现这些功能，你想怎么用就怎么用，同时不影响性能。你需要的就是STL,
 标准模板库！



西方有句谚语：不要重复发明轮子！


STL几乎封装了所有的数据结构中的算法，从链表到队列，从向量到堆栈，对hash到二叉树，从搜索到排序，从增加到删除......可以说，如果你理解了STL，你会发现你已不用拘泥于算法本身，从而站在巨人的肩膀上去考虑更高级的应用。


排序是最广泛的算法之一，本文详细介绍了STL中不同排序算法的用法和区别。

### 1 STL提供的Sort 算法
C++之所以得到这么多人的喜欢，是因为它既具有面向对象的概念，又保持了C语言高效的特点。STL 排序算法同样需要保持高效。因此，对于不同的需求，STL提供的不同的函数，不同的函数，实现的算法又不尽相同。
#### 1.1 所有sort算法介绍
所有的sort算法的参数都需要输入一个范围，[begin, end)。这里使用的迭代器(iterator)都需是随机迭代器(RadomAccessIterator), 也就是说可以随机访问的迭代器，如：it+n什么的。（partition 和stable_partition 除外）

如果你需要自己定义比较函数，你可以把你定义好的仿函数(functor)作为参数传入。每种算法都支持传入比较函数。以下是所有STL sort算法函数的名字列表:
|[函数名](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms?sortcol=0&table=1&up=0#sorted_table)|[功能描述](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms?sortcol=1&table=1&up=0#sorted_table)|
|----|----|
|sort|对给定区间所有元素进行排序|
|stable_sort|对给定区间所有元素进行稳定排序|
|partial_sort|对给定区间所有元素部分排序|
|partial_sort_copy|对给定区间复制并排序|
|nth_element|找出给定区间的某个位置对应的元素|
|is_sorted|判断一个区间是否已经排好序|
|partition|使得符合某个条件的元素放在前面|
|stable_partition|相对稳定的使得符合某个条件的元素放在前面|


其中nth_element 是最不易理解的，实际上，这个函数是用来找出第几个。例如：找出包含7个元素的数组中排在中间那个数的值，此时，我可能不关心前面，也不关心后面，我只关心排在第四位的元素值是多少。

#### 1.2 sort 中的比较函数
当你需要按照某种特定方式进行排序时，你需要给sort指定比较函数，否则程序会自动提供给你一个比较函数。
vector < int > vect;
//...
sort(vect.begin(), vect.end());
//此时相当于调用
sort(vect.begin(), vect.end(), less<int>() );
上述例子中系统自己为sort提供了less仿函数。在STL中还提供了其他仿函数，以下是仿函数列表:|[名称](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms?sortcol=0&table=2&up=0#sorted_table)|[功能描述](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms?sortcol=1&table=2&up=0#sorted_table)|
|----|----|
|equal_to|相等|
|not_equal_to|不相等|
|less|小于|
|greater|大于|
|less_equal|小于等于|
|greater_equal|大于等于|
需要注意的是，这些函数不是都能适用于你的sort算法，如何选择，决定于你的应用。另外，不能直接写入仿函数的名字，而是要写其重载的()函数：less<int>()
greater<int>()
当你的容器中元素时一些标准类型（int float char)或者string时，你可以直接使用这些函数模板。但如果你时自己定义的类型或者你需要按照其他方式排序，你可以有两种方法来达到效果：一种是自己写比较函数。另一种是重载类型的'<'操作赋。



```cpp
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
using namespace std;

class myclass {
        public:
        myclass(int a, int b):first(a), second(b){}
        int first;
        int second;
        bool operator < (const myclass &m)const {
                return first < m.first;
        }
};

bool less_second(const myclass & m1, const myclass & m2) {
        return m1.second < m2.second;
}

int main() {
        
        vector< myclass > vect;
        for(int i = 0 ; i < 10 ; i ++){
                myclass my(10-i, i*3);
                vect.push_back(my);
        }
        for(int i = 0 ; i < vect.size(); i ++) 
        cout<<"("<<vect[i].first<<","<<vect[i].second<<")\n";
        sort(vect.begin(), vect.end());
        cout<<"after sorted by first:"<<endl;
        for(int i = 0 ; i < vect.size(); i ++) 
        cout<<"("<<vect[i].first<<","<<vect[i].second<<")\n";
        cout<<"after sorted by second:"<<endl;
        sort(vect.begin(), vect.end(), less_second);
        for(int i = 0 ; i < vect.size(); i ++) 
        cout<<"("<<vect[i].first<<","<<vect[i].second<<")\n";
        
        return 0 ;
}
```

知道其输出结果是什么了吧：(10,0)
(9,3)
(8,6)
(7,9)
(6,12)
(5,15)
(4,18)
(3,21)
(2,24)
(1,27)
after sorted by first:
(1,27)
(2,24)
(3,21)
(4,18)
(5,15)
(6,12)
(7,9)
(8,6)
(9,3)
(10,0)
after sorted by second:
(10,0)
(9,3)
(8,6)
(7,9)
(6,12)
(5,15)
(4,18)
(3,21)
(2,24)
(1,27)

#### 1.3 sort 的稳定性
你发现有sort和stable_sort，还有 partition 和stable_partition， 感到奇怪吧。其中的区别是，带有stable的函数可保证相等元素的原本相对次序在排序后保持不变。或许你会问，既然相等，你还管他相对位置呢，也分不清楚谁是谁了？这里需要弄清楚一个问题，这里的相等，是指你提供的函数表示两个元素相等，并不一定是一摸一样的元素。



例如，如果你写一个比较函数:

bool less_len(const string &str1, const string &str2)
{
        return str1.length() < str2.length();
}
此时，"apple" 和 "winter" 就是相等的，如果在"apple" 出现在"winter"前面，用带stable的函数排序后，他们的次序一定不变，如果你使用的是不带"stable"的函数排序，那么排序完后，"Winter"有可能在"apple"的前面。


#### 1.4 全排序
全排序即把所给定范围所有的元素按照大小关系顺序排列。用于全排序的函数有


template <class RandomAccessIterator>
void sort(RandomAccessIterator first, RandomAccessIterator last);

template <class RandomAccessIterator, class StrictWeakOrdering>
void sort(RandomAccessIterator first, RandomAccessIterator last,
StrictWeakOrdering comp);

template <class RandomAccessIterator>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last);

template <class RandomAccessIterator, class StrictWeakOrdering>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last, 
StrictWeakOrdering comp);
在第1，3种形式中，sort 和 stable_sort都没有指定比较函数，系统会默认使用operator< 对区间[first,last)内的所有元素进行排序, 因此，如果你使用的类型义军已经重载了operator<函数，那么你可以省心了。第2, 4种形式，你可以随意指定比较函数，应用更为灵活一些。来看看实际应用：

班上有10个学生，我想知道他们的成绩排名。

```cpp
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
using namespace std;

class student{
        public:
        student(const string &a, int b):name(a), score(b){}
        string name;
        int score;
        bool operator < (const student &m)const {
                return score< m.score;
        }
};

int main() {
        vector< student> vect;
        student st1("Tom", 74);
        vect.push_back(st1);
        st1.name="Jimy";
        st1.score=56;
        vect.push_back(st1);
        st1.name="Mary";
        st1.score=92;
        vect.push_back(st1);
        st1.name="Jessy";
        st1.score=85;
        vect.push_back(st1);
        st1.name="Jone";
        st1.score=56;
        vect.push_back(st1);
        st1.name="Bush";
        st1.score=52;
        vect.push_back(st1);
        st1.name="Winter";
        st1.score=77;
        vect.push_back(st1);
        st1.name="Andyer";
        st1.score=63;
        vect.push_back(st1);
        st1.name="Lily";
        st1.score=76;
        vect.push_back(st1);
        st1.name="Maryia";
        st1.score=89;
        vect.push_back(st1);
        cout<<"------before sort..."<<endl;
        for(int i = 0 ; i < vect.size(); i ++) cout<<vect[i].name<<":\t"<<vect[i].score<<endl;
        stable_sort(vect.begin(), vect.end(),less<student>());
        cout <<"-----after sort ...."<<endl;
        for(int i = 0 ; i < vect.size(); i ++) cout<<vect[i].name<<":\t"<<vect[i].score<<endl;
        return 0 ;
}
```
其输出是：------before sort...
Tom:    74
Jimy:   56
Mary:   92
Jessy:  85
Jone:   56
Bush:   52
Winter: 77
Andyer: 63
Lily:   76
Maryia: 89
-----after sort ....
Bush:   52
Jimy:   56
Jone:   56
Andyer: 63
Tom:    74
Lily:   76
Winter: 77
Jessy:  85
Maryia: 89
Mary:   92
sort采用的是成熟的"快速排序算法"(目前大部分STL版本已经不是采用简单的快速排序，而是结合内插排序算法)。[注1](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms#MyNote1)，可以保证很好的平均性能、复杂度为n*log(n)，由于单纯的快速排序在理论上有最差的情况，性能很低，其算法复杂度为n*n，但目前大部分的STL版本都已经在这方面做了优化，因此你可以放心使用。stable_sort采用的是"归并排序"，分派足够内存是，其算法复杂度为n*log(n),
 否则其复杂度为n*log(n)*log(n)，其优点是会保持相等元素之间的相对位置在排序前后保持一致。
#### 1.5 局部排序
局部排序其实是为了减少不必要的操作而提供的排序方式。其函数原型为：
template <class RandomAccessIterator>
void partial_sort(RandomAccessIterator first, 
RandomAccessIterator middle,
RandomAccessIterator last);

template <class RandomAccessIterator, class StrictWeakOrdering>
void partial_sort(RandomAccessIterator first,
RandomAccessIterator middle,
RandomAccessIterator last, 
StrictWeakOrdering comp);

template <class InputIterator, class RandomAccessIterator>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
RandomAccessIterator result_first,
RandomAccessIterator result_last);

template <class InputIterator, class RandomAccessIterator, 
class StrictWeakOrdering>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
RandomAccessIterator result_first,
RandomAccessIterator result_last, Compare comp);
理解了sort 和stable_sort后，再来理解partial_sort 就比较容易了。先看看其用途: 班上有10个学生，我想知道分数最低的5名是哪些人。如果没有partial_sort，你就需要用sort把所有人排好序，然后再取前5个。现在你只需要对分数最低5名排序，把上面的程序做如下修改：
stable_sort(vect.begin(), vect.end(),less<student>());
替换为：
partial_sort(vect.begin(), vect.begin()+5, vect.end(),less<student>());
输出结果为：------before sort...
Tom:    74
Jimy:   56
Mary:   92
Jessy:  85
Jone:   56
Bush:   52
Winter: 77
Andyer: 63
Lily:   76
Maryia: 89
-----after sort ....
Bush:   52
Jimy:   56
Jone:   56
Andyer: 63
Tom:    74
Mary:   92
Jessy:  85
Winter: 77
Lily:   76
Maryia: 89
这样的好处知道了吗？当数据量小的时候可能看不出优势，如果是100万学生，我想找分数最少的5个人......

partial_sort采用的堆排序（heapsort），它在任何情况下的复杂度都是n*log(n). 如果你希望用partial_sort来实现全排序，你只要让middle=last就可以了。


partial_sort_copy其实是copy和partial_sort的组合。被排序(被复制)的数量是[first, last)和[result_first, result_last)中区间较小的那个。如果[result_first, result_last)区间大于[first, last)区间，那么partial_sort相当于copy和sort的组合。

#### 1.6 nth_element 指定元素排序
nth_element一个容易看懂但解释比较麻烦的排序。用例子说会更方便：
班上有10个学生，我想知道分数排在倒数第4名的学生。
如果要满足上述需求，可以用sort排好序，然后取第4位（因为是由小到大排), 更聪明的朋友会用partial_sort, 只排前4位，然后得到第4位。其实这是你还是浪费，因为前两位你根本没有必要排序，此时，你就需要nth_element:
template <class RandomAccessIterator>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
RandomAccessIterator last);

template <class RandomAccessIterator, class StrictWeakOrdering>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
RandomAccessIterator last, StrictWeakOrdering comp);
对于上述实例需求，你只需要按下面要求修改1.4中的程序：
stable_sort(vect.begin(), vect.end(),less<student>());
替换为：
nth_element(vect.begin(), vect.begin()+3, vect.end(),less<student>());
运行结果为：------before sort...
Tom:    74
Jimy:   56
Mary:   92
Jessy:  85
Jone:   56
Bush:   52
Winter: 77
Andyer: 63
Lily:   76
Maryia: 89
-----after sort ....
Jone:   56
Bush:   52
Jimy:   56
Andyer: 63
Jessy:  85
Mary:   92
Winter: 77
Tom:    74
Lily:   76
Maryia: 89
第四个是谁？Andyer，这个倒霉的家伙。为什么是begin()+3而不是+4? 我开始写这篇文章的时候也没有在意，后来在[ilovevc](http://www.stlchina.org/bbs/viewpro.php?uid=350) 的提醒下，发现了这个问题。begin()是第一个，begin()+1是第二个，...
 begin()+3当然就是第四个了。
#### 1.7 partition 和stable_partition
好像这两个函数并不是用来排序的，'分类'算法，会更加贴切一些。partition就是把一个区间中的元素按照某个条件分成两类。其函数原型为：
template <class ForwardIterator, class Predicate>
ForwardIterator partition(ForwardIterator first,
ForwardIterator last, Predicate pred)
template <class ForwardIterator, class Predicate>
ForwardIterator stable_partition(ForwardIterator first, ForwardIterator last, 
Predicate pred);
看看应用吧：班上10个学生，计算所有没有及格（低于60分）的学生。你只需要按照下面格式替换1.4中的程序：
stable_sort(vect.begin(), vect.end(),less<student>());
替换为：
student exam("pass", 60);
stable_partition(vect.begin(), vect.end(), bind2nd(less<student>(), exam));
其输出结果为：------before sort...
Tom:    74
Jimy:   56
Mary:   92
Jessy:  85
Jone:   56
Bush:   52
Winter: 77
Andyer: 63
Lily:   76
Maryia: 89
-----after sort ....
Jimy:   56
Jone:   56
Bush:   52
Tom:    74
Mary:   92
Jessy:  85
Winter: 77
Andyer: 63
Lily:   76
Maryia: 89
看见了吗，Jimy，Jone, Bush(难怪说美国总统比较笨 ![smile](http://www.stlchina.org/twiki/pub/TWiki/SmiliesPlugin/smile.gif) )都没有及格。而且使用的是stable_partition,
 元素之间的相对次序是没有变.
### 2 Sort 和容器
STL中标准容器主要vector, list, deque, string, set, multiset, map, multimay， 其中set, multiset, map, multimap都是以树结构的方式存储其元素详细内容请参看：[学习STL
 map, STL set之数据结构基础](http://stl.winterxy.com/html/000039.html). 因此在这些容器中，元素一直是有序的。

这些容器的迭代器类型并不是随机型迭代器，因此，上述的那些排序函数，对于这些容器是不可用的。上述sort函数对于下列容器是可用的：
- vector
- string
- deque
如果你自己定义的容器也支持随机型迭代器，那么使用排序算法是没有任何问题的。

对于list容器，list自带一个sort成员函数list::sort(). 它和算法函数中的sort差不多，但是list::sort是基于指针的方式排序，也就是说，所有的数据移动和比较都是此用指针的方式实现，因此排序后的迭代器一直保持有效（vector中sort后的迭代器会失效).



### 3 选择合适的排序函数
为什么要选择合适的排序函数？可能你并不关心效率(这里的效率指的是程序运行时间), 或者说你的数据量很小， 因此你觉得随便用哪个函数都无关紧要。

其实不然，即使你不关心效率，如果你选择合适的排序函数，你会让你的代码更容易让人明白，你会让你的代码更有扩充性，逐渐养成一个良好的习惯，很重要吧 ![smile](http://www.stlchina.org/twiki/pub/TWiki/SmiliesPlugin/smile.gif) 。


如果你以前有用过C语言中的qsort, 想知道qsort和他们的比较，那我告诉你，qsort和sort是一样的，因为他们采用的都是快速排序。从效率上看，以下几种sort算法的是一个排序，效率由高到低（耗时由小变大）：
- partion
- stable_partition
- nth_element
- partial_sort
- sort
- stable_sort
记得，以前翻译过Effective STL的文章，其中对[如何选择排序函数](http://stl.winterxy.com/html/000026.html)总结的很好：- 若需对vector, string, deque, 或 array容器进行全排序，你可选择sort或stable_sort；
- 若只需对vector, string, deque, 或 array容器中取得top n的元素，部分排序partial_sort是首选.
- 若对于vector, string, deque, 或array容器，你需要找到第n个位置的元素或者你需要得到top n且不关系top n中的内部顺序，nth_element是最理想的；
- 若你需要从标准序列容器或者array中把满足某个条件或者不满足某个条件的元素分开，你最好使用partition或stable_partition；
- 若使用的list容器，你可以直接使用partition和stable_partition算法，你可以使用list::sort代替sort和stable_sort排序。若你需要得到partial_sort或nth_element的排序效果，你必须间接使用。正如上面介绍的有几种方式可以选择。
总之记住一句话： 如果你想节约时间，不要走弯路, 也不要走多余的路!
### 4 小结
讨论技术就像个无底洞，经常容易由一点可以引申另外无数个技术点。因此需要从全局的角度来观察问题，就像观察STL中的sort算法一样。其实在STL还有make_heap, sort_heap等排序算法。本文章没有提到。本文以实例的方式，解释了STL中排序算法的特性，并总结了在实际情况下应如何选择合适的算法。


### 5 参考文档
[条款31：如何选择排序函数](http://stl.winterxy.com/html/000026.html)
[The Standard Librarian:
 Sorting in the Standard Library](http://www.cuj.com/documents/s=7992/cujcexp1908austern/)
[Effective STL中文版](http://www.stlchina.org/documents/EffectiveSTL/index.html)
[Standard Template Library Programmer's
 Guide](http://www.stlchina.org/stl_doc/) vvvv






