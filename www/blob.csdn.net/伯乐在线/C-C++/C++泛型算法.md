# C++泛型算法 - 文章 - 伯乐在线
原文出处： [wangxu_zju_2010](http://blog.csdn.net/wangxu_zju_2010/article/details/7591071)
本文主要讨论C++标准库中的泛型算法(generic algorithm)。泛型算法是使用容器的强有力的辅助工具。
如果文中有错误或遗漏之处，敬请指出，谢谢！
标准库为容器类型定义的操作很少，并没有为每个容器实现更多的操作。因为这部分操作可以抽象出来为所有的容器工作，那就是泛型算法。所谓“泛型”是指这些算法可以应用于多种容器类型上，而容器内的元素类型也可以多样化。标准库提供了100多个泛型算法，主要定义于头文件<algorithm>中，还有一组泛化的算术算法定义于头文件<numeric>中。
大多数泛型算法是工作于容器的一对迭代器所标识的范围，并完全通过迭代器来实现其功能。这段由迭代器指定的范围称为“输入范围”。带有输入范围参数的算法总是使用前两个参数标记该范围，分别指向要处理的第一个元素和最后一个元素的下一个位置。
这些算法一般可划分为只读算法、改写元素算法或对元素重新排序算法，下面分别叙述之。
#### 只读算法
**find 算法**


```
template<class InIt, class T>
      InIt find(InIt first, InIt last, const T& val);
```
查询迭代器指定范围[first, last)范围内是否有val值。如果有，则返回该值对应的迭代器；否则，返回last表示查找失败。
**accumulate 算法**


```
template<class InIt, class T>
      T accumulate(InIt first, InIt last, T val);
   template<class InIt, class T, class Pred>
      T accumulate(InIt first, InIt last, T val, Pred pr);
```
累加迭代器指定范围[first, last)范围内所有元素，再加上累加的初值val，返回累加的结果。第二个函数自定义操作：val = pr(val, *it)。
注：用于指定累加起始值的第三个参数是必要的，因为算法对将要累加的元素类型一无所知，没有别的办法创建合适的起始值或者关联的类型。
**find_first_of 算法**


```
template<class FwdIt1, class FwdIt2>
      FwdIt1 find_first_of(FwdIt1 first1, FwdIt1 last1, FwdIt2 first2, FwdIt2 last2);
   template<class FwdIt1, class FwdIt2, class Pred>
      FwdIt1 find_first_of(FwdIt1 first1, FwdIt1 last1, FwdIt2 first2, FwdIt2 last2, Pred pr);
```
查询第一段范围内与第二段范围内任意元素匹配的元素的位置。如果找到，返回该元素对应的迭代器；否则，返回last1。第二个函数使用判断：pr(*it1, *it2)来代替第一个函数中的判断：*it1 == *it2。
#### 写容器元素的算法
在使用写元素的算法时，必须确保算法所写的序列至少足以存储要写入的元素。有些算法直接将数据写入到输入序列，另外一些则带有一个额外的迭代器参数指定写入目标。这类算法将目标迭代器用作输出的位置。还有第三种算法将指定数目的元素写入某个序列。
**写入输入序列的元素**
写入到输入序列的算法本质上是案例的，因为只会写入与指定输入范围数量相同的元素。如fill算法：


```
template<class FwdIt, class T>
      void fill(FwdIt first, FwdIt last, const T& x);
```
这个算法将指定范围内的每个元素都设定为给定的值。如果输入范围有效，则可以安全写入。这个算法只会对输入范围内已存在的元素进行写入操作。
**不检查写入操作的算法**
这类算法如fill_n算法：


```
template<class OutIt, class Size, class T>
      void fill_n(OutIt first, Size n, const T& x);
```
该算法从迭代器指向的元素开始，将指定数量的元素设置为给定的值。如果目标范围内的某些元素不存在，则该操作未定义。如下面的代码将发生不可预料的结果：


```
vector<int> vec;              // empty vector
   fill_n(vec.begin(), 10, 0);   // disaster behavior
```
注：对指定数目的元素做写入运算，或者写到目标迭代的算法，都不检查目标的大小是否足以存储要写入的元素。
**back_inserter**
确保算法有足够的元素存储输出数据的一种方法是使用插入迭代器(insert iterator)。插入迭代器是可以给基础容器添加元素的迭代器。通常，用迭代器给容器元素赋值时，被赋值的是迭代器所指向的元素。而使用插入迭代器赋值时，则会在容器中添加一个新元素，其值等于赋值运算的右操作数的值。
back_inserter函数是迭代器适配器，其使用一个对象作为实参，并生成一个适应其实参行为的新对象。比如，在下例中，传递给back_inserter的实参是一个容器的引用。back_inserter生成一个绑定在该容器上的插入迭代器。在试图通过这个迭代器给元素赋值时，赋值运算将调用push_back在容器中添加一个具有指定值的元素。因此，用back_inserter改写上面的代码可以有效地工作：


```
vector<int> vec;                     // empty vector
   fill_n(back_inserter(vec), 10, 0);   // ok: appends 10 elements to vec
```
**写入到目标迭代器的算法**
第三类算法向目标迭代器写入未知个数的元素。这类算法最简单的如copy算法：


```
template<class InIt, class OutIt>
      OutIt copy(InIt first, InIt last, OutIt x);
```
copy算法带有三个迭代器参数：前两个指定输入范围，第三个指向目标序列的第一个元素。
**算法的_copy版本**
有些算法提供所谓的“_copy”版本。这些算法对输入序列的元素做处理，但不修改原来的元素，而是创建一个新序列存储元素的处理结果
例如，replace算法：


```
template<caass FwdIt, class T>
void replace(FwdIt first, FwdIt last, const T& vold, const T& vnew);
```
该算法指定范围[first, last)内的所有元素值为vold替换为vnew。
如果不想改变原序列，可以用replace_copy算法：


```
template<class InIt, class OutIt, class T>
OutIt replace_copy(InIt first, InIt last, OutIt x, const T& vold, const T& vnew);
```
这个算法接受第三个迭代器参数，指定保存替换后的序列的目标位置。例如：


```
vector<int> vec;
replace(ilist.begin(), ilist.end(), back_inserter(ivec), 1, 10);
```
调用该函数后，ilist没有改变，而ivec存储ilist的一份替换后的副本。
对容器元素重新排序的算法
**sort算法**
这里只介绍sort和stable_sort这个类排序算法：


```
template<class RanIt>
void sort(RanIt first, RanIt last);
template<class RanIt, class Pred>
void sort(RanIt first, RanIt last, Pred pr);
template<class RanIt>
void stable_sort(RanIt first, RanIt last);
template<class RanIt, class Pred>
void stable_sort(RanIt first, RanIt last, Pred pr);
```
sort排序算法是最一般的类型，而stable_sort排序算法是稳定排序。
**unique和unique_copy**
unique函数“删除”指定范围内的重复元素。注意：这里的“删除”不是真正意义上的删除，只是在有重复元素时，把后面的元素向前移动覆盖了原来的元素。函数返回的迭代器指向无重复元素序列最后一个元素的下一个位置。而unique_copy是它的“_copy”版本，返回的是生成的序列的最后一个元素的下一个位置。


```
template<class FwdIt>
FwdIt unique(FwdIt first, FwdIt last);
template<class FwdIt, class Pred>
FwdIt unique(FwdIt first, FwdIt last, Pred pr);
template<class InIt, class OutIt>
OutIt unique_copy(InIt first, InIt last, OutIt x);
template<class InIt, class OutIt, class Pred>
OutIt unique_copy(InIt first, InIt last, OutIt x, Pred pr);
```
注意：unique调用后，原序列的前面部分是无重复元素的序列，而后半部分是剩下没有被覆盖的序列。这里，需要手动删除后面的元素序列，范围由返回的迭代器和容器末端决定。
#### 迭代器
**插入迭代器**
插入迭代器是一种迭代器适配器，带有一个容器参数，并生成一个迭代器，用于在指定的容器中插入元素。通过插入迭代器赋值时，迭代器将会插入一个新的元素。C++语言提供了三种插入器，其差别在于插入元素的位置不同：
1）back_inserter，创建使用push_back实现插入的迭代器；
2）front_inserter，使用push_front实现插入；
3）inserter，使用insert实现插入操作。除了所关联的容器外，inserter还带有第二个实参：指向插入起始位置的迭代器。
front_inserter的操作类似于back_inserter：该函数将创建一个迭代器，调用它所关联的基础容器的push_front成员函数代替赋值操作。注意：只有当容器提供push_front操作时，才能使用front_inserter。在vector或其他没有push_front运算的容器上使用front_inserter，将产生错误。
inserter将产生在指定位置实现插入的迭代器，inserter总是在它的迭代器参数所标明的位置前面插入新元素。看看下面的例子：


```
list<int> ilst, ilst2, ilst3; //empty lists
// after this loop ilst contains: 1 2 3 4
for (list<int>::value_type i = 0; i != 4; ++i)
ilst.push_front(i + 1);
// after copy ilst2 contains: 4 3 2 1
copy (ilst.begin(), ilst.end(), front_inserter(ilst2));
// after copy ilst3 contains: 1 2 3 4
copy (ilst.begin(), ilst.end(), inserter(ilst3, ilst3.begin()));
```
**iostream 迭代器**
虽然iostream类型不是容器，但标准库同样提供了在iostream对象上使用的迭代器：istream_iterator用于读取读入流，而ostream_iterator用于写输出流。这些迭代器将它们所对应的流视为特定类型的元素序列。使用流迭代器时，可以用泛型算法从流对象中读数据（或将数据写到流对象中）。
![1](http://jbcdn2.b0.upaiyun.com/2016/12/ec38db8f6736868f95d414bbc06d91a0.jpg)
流迭代器只定义了最基本的迭代器操作：自增、解引用和赋值。此外，可比较两个istream迭代器是否相等（或不等）。而ostream迭代器则不提供比较运算。
![2](http://jbcdn2.b0.upaiyun.com/2016/12/5bdac55d3d5e3b68d79997a43a1e6479.jpg)
使迭代器向前移动。通常，前缀版本使迭代器在流中向前移动，并返回对加1后的迭代器的引用。it++ 而后缀版本使迭代器在流中向前移动后，返回原值。
流迭代器是类模板：任何已定义输入操作符(>>操作符)的类型都可以定义istream_iterator。类似地，任何已定义输出操作符(<<操作符)的类型也可以ostream_iterator。
istream_iterator使用举例：


```
#include <iostream>
#include <vector>
#include <iterator>
using namespace std;
int main() {
istream_iterator<int> in_iter(cin);
istream_iterator<int> eof;
//vector<int> vec(in_iter, eof); //do the same work as following loop
vector<int> vec;
while (in_iter != eof)
vec.push_back(*in_iter++);
vector<int>::const_iterator it = vec.begin();
for(; it != vec.end(); ++it)
cout<<*it<<endl;
return 0;
}
```
ostream_iterator使用举例：


```
#include <iostream>
#include <iterator>
using namespace std;
int main() {
ostream_iterator<string> out_iter(cout, "/n");
istream_iterator<string> in_iter(cin), eof;
while (in_iter != eof)
*out_iter++ = *in_iter++;
return 0;
}
```
流迭代器的限制：
1）不可能从ostream_iterator对象读入，也不可能写到istream_iterator对象中；
2）一旦给ostream_iterator对象赋了一个值，写入就提交了。赋值后，没有办法再改变这个值。此外，ostream_iterator对象中每个不同的值都只能正好输出一次。
3）ostream_iterator没有->操作符。
与算法一起使用流迭代器，如下面的示例实现从标准输入读取一些数，然后将不重复的数写到标准输出：


```
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;
int main() {
istream_iterator<int> in_it(cin), eof;
vector<int> vec(in_it, eof);
sort(vec.begin(), vec.end());
ostream_iterator<int> out_it(cout, " ");
unique_copy(vec.begin(), vec.end(), out_it);
return 0;
}
```
**反向迭代器**
反向迭代器是一种反向遍历容器的迭代器。也就是，从最后一个元素到第一个元素遍历容器。反向迭代器将自增（和自减）的含义反过来了：对于反向迭代器，++运算将访问前一个元素，而–运算则访问下一个元素。
1）反向迭代器需要使用自减操作符：标准容器上的迭代器（reverse_iterator)既支持自增运算，也支持自减运算。但是，流迭代器由于不能反向遍历流，因此流迭代器不能创建反向迭代器。
2）可以通过reverse_iterator::base()将反向迭代器转换为普通迭代器使用，从逆序得到普通次序。如下面的例子所示：


```
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
using namespace std;
int main() {
string str = "this 'sentence' is a test";
cout<<"String: "<<str<<endl;
string::iterator it1 = find(str.begin(), str.end(), '/'');
string::iterator it2 = find(++it1, str.end(), '/'');
// output: sentence
cout<<"B-E: "<<string(it1, it2)<<endl; 
string::reverse_iterator rit1 = find(str.rbegin(), str.rend(), '/'');
string::reverse_iterator rit2 = find(++rit1, str.rend(), '/'');
// output: ecnetnes
cout<<"R-B-E 1: "<<string(rit1, rit2)<<endl;
// output: sentence
cout<<"R-B-E 2: "<<string(rit2.base(), rit1.base())<<endl;
return 0;
}
```
**const 迭代器**
在标准库中，有输入范围的泛型算法要求其两个迭代器类型完全一样，包括const属性。要么都是const，要么都是非const，否则无法通过编译。同样，它们的返回值迭代器也与参数类型保持一致。
**迭代器分类**
不同的迭代器支持不同的操作集，而各种算法也要求相应的迭代器具有最小的操作集。因此，可以将算法的迭代器分为下面五类：
![3](http://jbcdn2.b0.upaiyun.com/2016/12/1a8f9e7bef87ba7396d4015613926031.jpg)
除了输出迭代器，其他类别的迭代器形成了一个层次结构：需要低级类别迭代器的地方，可使用任意一种更高级的迭代器。例如，对于需要输入迭代器的算法，可传递前向、双向或随机访问迭代器调用该算法。而反之则不行。注意：向算法传递无效的迭代器类别所引起的错误，无法保证会在编译时被捕获到。
map, set, list类型提供双向迭代器，而string, vector和deque容器上定义的迭代器都是随机访问迭代器，用作访问内置数组元素的指针也是随机访问迭代器。istream_iterator是输入迭代器，ostream_iterator是输出迭代器。
另外，虽然map和set类型提供双向迭代器，但关联容器只能使用这部分算法的一个子集。因为关联容器的键是const对象。因此，关联容器不能使用任何写序列元素的算法。只能使用与关联容器绑在一起的迭代器来提供用于读操作的实参。因此，在处理算法时，最好将关联容器上的迭代器视为支持自减运算的输入迭代器，而不是完整的双向迭代器。
**泛型算法的结构**
就像所有的容器都建立在一致的设计模式上一样，算法也具有共同的设计基础。
算法最基本的性质是需要使用的迭代器种类。
另一种算法分类方法是前面介绍的按实现的功能分类：只读算法，不改变元素的值和顺序；给指定元素赋新值的算法；将一个元素的值移给另一个元素的算法。
另外，算法还有两种结构上的算法模式：一种模式是由算法所带的形参定义；另一种模式则通过两种函数命名和重载的规范定义。
**算法的形参模式**
大多数算法采用下面四种形式之一：


```
alg (beg, end, other parms);
alg (beg, end, dest, other parms);
alg (beg, end, beg2, other parms);
alg (beg, end, beg2, end2, other parms);
```
其中，alg是算法名，[beg, end)是输入范围，beg, end, dest, beg2, end2都是迭代器。
对于带有单个目标迭代器的算法：dest形参是一个迭代器，用于指定存储输出数据的目标对象。算法假定无论需要写入多少个元素都是安全的。注意：调用这类算法时，算法是将输出内容写到容器中已存在的元素上，所以必须确保输出容器中有足够大的容量存储输出数据，这也正是通过使用插入迭代器或者ostream_iterator来调用这些算法的原因。
对于带第二个输入序列的算法：beg2和end2标记了完整的输出范围。而只有beg2的算法将beg2视为第二个输入范围的首元素，算法假定以beg2开始的范围至少与beg和end指定的范围一样大。
**算法的命名规范**
包括两种重要模式：第一种模式包括测试输入范围内元素的算法，第二种模式则应用于输入范围内元素的重新排序的算法。
1）区别带有一个值或一个谓词函数参数的算法版本
很多算法通过检查其输入范围内的元素实现其功能。这些算法通常要用到标准关系操作符：== 或 < 。其中的大部分算法都提供了第二个版本的算法，允许程序员提供比较或测试函数取代默认的操作符的使用。
例如， 排序算法默认使用 < 操作符，其重载版本带有一个额外的形参，表示取代默认的 < 操作符。


```
sort (beg, end); // use < operator to sort the elements
sort (beg, end, comp); // use function named comp to sort the elements
```
又如，查找算法默认使用 == 操作符。标准库为这类算法提供另外命名的（而非重载的）版本，带有谓词函数形参。对于带有谓词函数形参的算法，其名字带有后缀 _if：


```
find (beg, end, val); // find first instance of val in the input range
find_if (beg, end, pred); // find first instance for which pred is true
```
标准库为这类算法提供另外命名的版本，而非重载版本，原因在于这两种版本的算法带有相同的参数个数，容易导致二义性。
2）区别是否实现复制的算法版本
默认情况下，算法将重新排列的写回其范围。标准库也为这类算法提供了另外命名的版本，将元素写到指定的输出目标。此版本的算法在名字中添加 _copy后缀，例如：


```
reverse (beg, end);
reverse_copy (beg, end, dest);
```
第一个版本将输入序列中的元素反向重新排列；而第二个版本将复制输入序列中的元素，并将它们以逆序存储到dest开始的序列中。
**容器特有的算法**
list容器上的迭代器是双向的，而不是随机访问类型。由于list容器不支持随机访问，因此，在此容器上不能使用需要随机访问迭代器的算法。如sort类算法。其它有些算法，如merge, remove, reverse, unique等，虽然可以用在list上，但性能太差。list容器结合自己的结构专门实现了更为高效的算法。因此，对于list对象，应该优先使用list容器特有的成员版本，而不是泛型算法。
下表列出了list容器特有的操作：
![4](http://jbcdn2.b0.upaiyun.com/2016/12/36780628e33b670ca61f5aa28ea91834.jpg)
list容器特有的算法与其泛型算法版本之间有两个重要的差别：1）remove和unique的list版本修改了其关联的基础容器：真正删除了指定的元素；2）list容器提供的merge和splice操作会破坏它们的实参。使用泛型算法的merge版本，合并的序列将写入目标迭代器指向的对象，而它的两个输入序列保持不变。
