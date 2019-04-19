# 标准模板库（STL）使用入门（上） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [柒柒](http://www.jobbole.com/members/chen1991) 翻译，[toolate](http://www.jobbole.com/members/toolate) 校稿。未经许可，禁止转载！
英文出处：[TopCoder](https://www.topcoder.com/community/data-science/data-science-tutorials/power-up-c-with-the-standard-template-library-part-1/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
或许你已经把 C++ 作为主要的编程语言用来解决 TopCoder 上的问题。这意味着你已经简单使用过了 STL，因为数组和字符串都是作为 STL 对象传递给函数。也许你已经注意到了，很多程序员写代码比你快得多，也更简洁。
或许你还不是但想成为一名 C++ 程序猿，因为这种编程语言功能很强大还有丰富的库（也许是因为在 TopCoder 的练习室里和竞赛中看到了很多非常精简的解决方案）。
无论过去如何，这篇文章都会有所帮助。在这里，我们将回顾标准模板库（Standard Template Library—STL，一个非常有用的工具，有时甚至能在算法竞赛中为你节省大量时间）的一些强大特性。
要熟悉 STL，最简单的方式就是从容器开始。
### 容器
无论何时需要操作大量元素，都会用到某种容器。C语言只有一种内置容器：数组。
问题不在于数组有局限性（例如，不可能在运行时确定数组大小）。相反，问题主要在于很多任务需要功能更强大的容器。
例如，我们可能需要一个或多个下列操作：
- 向容器添加某种字符串
- 从容器中移除一个字符串
- 确定容器中是否存在某个字符串
- 从容器中返回一些互不相同的元素
- 对容器进行循环遍历，以某种顺序获取一个附加字符串列表。
当然，我们可以在一个普通数组上实现这些功能。但是，这些琐碎的实现会非常低效。你可以创建树结构或哈希结构来快速解决问题，但是想想：这种容器的实现是取决于即将存储的元素类型吗？例如，我们要存储平面上的点而不是字符串的话，是不是要重写这个模块才能实现功能？
如果不是，那我们可以一劳永逸地为这种容器开发出接口，然后对任何数据类型都能使用。简言之，这就是 STL 容器的思想。
### 前言
程序要使用 STL 时，应包含（#include）适当的标准头文件。对大部分容器来说，标准头文件的名称和容器名一致，且不需扩展名。比如说，如果你要用栈（stack），只要在程序最开头添加下面这行代码：


C++
```
#include <stack>
```
容器类型（还有算法、运算符和所有 STL也一样）并不是定义在全局命名空间，而是定义在一个叫“std”的特殊命名空间里。在包含完所有头文件之后，写代码之前添加下面这一行：


C++
```
using namespace std;
```
还有另一个很重要的事情要记住：容器类型也是模板参数。在代码中用“尖括号”(‘<’/’>’)指明模板参数。比如：


C++
```
vector<int> N;
```
如果要进行嵌套式的构造，确保“方括号”之间不是紧挨着——留出一个空格的位置。（译者：C++11新特性支持两个尖括号之间紧挨着，不再需要加空格）


C++
```
vector< vector<int> > CorrectDefinition;
vector<vector<int>> WrongDefinition; // Wrong: compiler may be confused by 'operator >>'
```
### Vector
最简单的 STL 容器就是 vector。Vector 只是一个拥有扩展功能的数组。顺便说一下，vector 是唯一向后兼容 C 代码的容器——这意味着 vector 实际上**就是****数组**，只是拥有一些额外特性。

C++
```
vector<int> v(10);
 for(int i = 0; i < 10; i++) {
      v[i] = (i+1)*(i+1);
 }
 for(int i = 9; i > 0; i--) {
      v[i] -= v[i-1];
 }
```
实际上，当你敲下


C++
```
vector<int> v;
```
就创建了一个空 vector。注意这样的构造方式：


C++
```
vector<int> v[10];
```
这里我们把’V’声明成一个存放了 10 个 vector<int> 类型元素的数组，初始化为空。大部分情况下，这不是我们想要的。在这里用圆括号代替方括号。Vector 最常使用的特性就是获取容器大小。

```
int elements_count = v.size();
```
有两点要注意：首先，size() 函数返回的值是无符号的，这点有时会引起一些问题。因此，我经常定义宏，有点像 sz(C) （把C 的大小作为一个普通的带符号整型返回）这样的。其次，如果你想知道容器是否为空，把 vector 的 size() 返回值和0比较不是一个好的做法。你最好使用 empty() 函数：

C++
```
bool is_nonempty_notgood = (v.size() >= 0); // Try to avoid this
bool is_nonempty_ok = !v.empty();
```
这是因为，不是所有容器都能在常量时间内返回自己的大小，而且你绝不应该为了确定链表中至少包含一个节点元素就对一条双链表中的所有元素计数。
另一个 vector 中经常使用的函数是 push_back。Push_back 函数向 vector 尾部添加一个元素，容器长度加 1。思考下面这个例子：


C++
```
vector<int> v;
 for(int i = 1; i < 1000000; i *= 2) {
      v.push_back(i);
 }
 int elements_count = v.size();
```
别担心内存分配问题——vector 不会一次只分配一个元素的空间。相反，每次用 push_back 添加新元素时，vector 分配的内存空间总是比它实际需要的更多。你应该担心的唯一一件事情是内存使用情况，但在 TopCoder 上这点可能不是问题。（后面再进一步探讨 vector 的内存策略）
当你需要重新改变 vector 的大小时，使用 resize() 函数：


C++
```
vector<int> v(20);
 for(int i = 0; i < 20; i++) {
      v[i] = i+1;
 }
 v.resize(25);
 for(int i = 20; i < 25; i++) {
      v[i] = i*2;
 }
```
Resize() 函数让 vector 只存储所需个数的元素。如果你需要的元素个数少于 vector 当前存储的个数，剩余那些元素就会被删除。如果你要求 vector 变大，使用这个函数也会扩大它的长度，并用 0 填充新创建的元素。
注意，如果在使用了 resize() 后又用了 push_back()，那新添加的元素就会位于新分配内存的**后面**，而不是被**放入**新分配的内存当中。上面的例子得到的 vector 大小是25，如果在第二个循环中使用 push_back()，那vector 的大小最后会是30。


C++
```
vector<int> v(20);
 for(int i = 0; i < 20; i++) {
      v[i] = i+1;
 }
 v.resize(25);
 for(int i = 20; i < 25; i++) {
      v.push_back(i*2); //把下标值写入元素 [25..30), not [20..25) ! <
 }
```
使用 clear() 函数来清空 vector。这个函数使 vector 包含 0 个元素。它并不是让所有元素的值为0——注意——它是完全删除所有元素，成为空容器。
有很多种方式初始化 vector。你也许用另一个 vector 来创建新的 vector：


C++
```
vector<int> v1;
 // ...
 vector<int> v2 = v1;
 vector<int> v3(v1);
```
上面的例子中，v2 和 v3 的初始化过程一样。如果你想创建指定大小的 vector，使用下面的构造函数：

```
vector<int> Data(1000);
```
上面的例子中，变量 data 创建后将包含1,000 个0值元素。记得使用圆括号，而不是方括号。如果你想用其他东西来初始化 vector，你可以这么写：


C++
```
vector<string> names(20, “Unknown”);
```
记住，你可以创建任何类型的 vector。多维数组很重要。通过 vector 创建二维数组，最简单的方式就是创建一个存储 vector 元素的 vector。

```
vector< vector<int> > Matrix;
```
你现在应该清楚如何创建一个给定大小的二维 vector：


C++
```
int N, N;
 // ...
 vector< vector<int> > Matrix(N, vector<int>(M, -1));
```
这里，我们创建了一个 N*M 的矩阵，并用 -1 填充所有位置上的值。向 vector 添加数据的最简单方式是使用 push_back()。但是，万一我们想在除了尾部以外的地方添加数据呢？Insert() 函数可以实现这个目的。同时还有 erase() 函数来删除元素。但我们得先讲讲迭代器。
你还应该记住另一个非常重要的事情：当 vector 作为参数传给某个函数时，实际上是复制了这个 vector（也就是值传递）。在不需要这么做的时候创建新的 vector 可能会消耗大量时间和内存。实际上，很难找到一个任务需要在传递 vector 为参数时对其进行复制。因此，永远不要这么写：


C++
```
void some_function(vector<int> v) { // Never do it unless you’re sure what you do!
      // ...
 }
```
相反，使用下面的构造方法（引用传递）：


C++
```
void some_function(const vector<int>& v) { // OK
      // ...
 }
```
如果在函数里要改变 vector 中的元素值，那就去掉‘const’修饰符。


C++
```
int modify_vector(vector<int>& v) { // Correct
      V[0]++;
 }
```
### 键值对
在讨论迭代器之前，先说说键值对（pairs）。STL 中广泛使用键值对。一些简单的问题，像 TopCoder SRM 250 和 500 分值的简单题，通常需要一些简单的数据结构，它们都非常适合用 pair 来构造。STL 中的 std::pair 就是一个元素对。最简单的形式如下：


C++
```
template<typename T1, typename T2> struct pair {
      T1 first;
      T2 second;
 };
```
普通的 pair<int,int> 就是一对整型值。来点更复杂的，pair<string,pair<int,int>> 就是一个字符串和两个整型组成的值对。第二种情况也许能这么用：


C++
```
pair<string, pair<int,int> > P;
 string s = P.first; // extract string
 int x = P.second.first; // extract first int
 int y = P.second.second; // extract second int
```
键值对的最大优势就在于它们有内置操作来比较 pair 对象。键值对优先对比第一个元素值，再比较第二个元素。如果第一个元素不相等，那结果就只取决于第一个元素之间的比较；只有在第一个元素相等时才比较第二个元素。使用 STL 的内置函数，可以轻易地对数组（或 vector）对进行排序。
例如，如果要对存放整型值坐标点的数组排序，使得这些点排列成一个多边形，一种很好的思路就是把点放入 vector<pair<double, pair<int, int>>>，其中每个元素表示成 {polar angle,{x, y}}（点的极角和点的坐标值）。调用 STL 的排序函数可以按你的期望对点进行排序。
关联容器中也广泛使用 pair，这点会在文章后面提及。
### 迭代器
什么是迭代器？STL 迭代器是访问容器数据的最普通的方式。思考这个简单的问题：将包含 N 个整型（int）的数组 A 倒置。从类 C 语言的方案开始：


C++
```
void reverse_array_simple(int *A, int N) {
      int first = 0, last = N-1; // First and last indices of elements to be swapped
      While(first < last) { // Loop while there is something to swap
           swap(A[first], A[last]); // swap(a,b) is the standard STL function
           first++; // Move first index forward
           last--; // Move last index back
      }
 }
```
对你来说这些代码应该一目了然。很容易用指针来重写：


C++
```
void reverse_array(int *A, int N) {
      int *first = A, *last = A+N-1;
      while(first < last) {
           Swap(*first, *last);
           first++;
           last--;
      }
 }
```
看看这个代码的主循环，它对指针‘first’和‘last’只用了四种不同的操作：
- 比较指针（first < last），
- 通过指针取值（*first,*last），
- 指针自增，以及
- 指针自减
现在，想象你正面临第二个问题：将一个双链表翻转，或部分翻转。第一个程序使用了下标，肯定不行。至少效率不够，因为不可能在常数时间内通过下标获取双链表中的元素值，必须花费 O(N) 的时间复杂度，所以整个算法的时间复杂度是 O(N^2)。
但是你看：第二个程序对任何类似指针（pointer-like）的对象都能奏效。唯一的要求是，对象能够执行上面所列出的四种操作：取值（一元运算符 *），对比（<），和自增/自减（++/–）。拥有这些属性并和容器相关联的对象就叫迭代器。任何 STL 容器都可以通过迭代器遍历。尽管 vector 不常用，但对其他类型的容器很重要。
那么，我们现在讨论的这个东西是什么？一个语法上很像指针的对象。为迭代器定义如下操作：
- 从迭代器取值，int x = *it；
- 让迭代器自增和自减 it1++，it2–；
- 通过‘！=’和‘<’来比较迭代器大小；
- 向迭代器添加一个常量值 it += 20；（向前移动了 20 个元素位置）
- 获取两个迭代器之间的差值，int n = it2 – it1；
和指针不同，迭代器提供了许多更强大的功能。它们不仅能操作任何类型的容器，还能执行范围检查并分析容器的使用。
当然，迭代器的最大优势就是极大地增加了代码重用性：基于迭代器写的算法在大部分的容器上都能使用，而且，自己写的容器要是提供了迭代器，就能作为参数传给各种各样的标准函数。
不是所有类型的迭代器都会提供所有潜在的功能。实际上，存在所谓的“常规迭代器”和“随机存取迭代器”两种分类。简单地说，常规迭代器可以用‘==’和‘！=’来做比较运算，而且还能自增和自减。它们不能做减法，也不能在常规迭代器上做加法。基本上来说，不可能对所有类型的容器都在常数时间范围内实现以上描述的操作。尽管如此，翻转数组的函数应该这么写：


C++
```
template<typename T> void reverse_array(T *first, T *last) {
      if(first != last) {
           while(true) {
                swap(*first, *last);
                first++;
                if(first == last) {
                     break;
                }
                last--;
                if(first == last) {
                     break;
                }
           }
      }
 }
```
这个程序和前面一个程序的主要差别在于，我们没有在迭代器上进行“<”比较，只用了“==”比较。再次强调，如果你对函数原型感到惊讶（发现函数原型和实际不同），不要慌张：模板只是声明函数的一种方式，对任何恰当的参数类型都是有效的。
对指向任意对象类型的指针和所有常规迭代器来说，这个函数应该都能完美运行。
还是回到 STL 上吧。STL 算法常常使用两个迭代器，称为“begin”和“end”。尾部迭代器不指向最后一个对象，而是指向第一个无效对象，或是紧跟在最后一个对象后面的对象。这一对迭代器使用起来通常很方便。
每一个 STL 容器都有 begin() 和 end() 两个成员函数，分别返回容器的初始迭代器和尾部迭代器。
基于这些原理，只有容器 c 为空时，“c.begin() == c.end()”才成立，而“c.end() – c.begin()”总是会等于 c.size()。（后一句只有在迭代器可以做减法运算时才有效，例如，begin() 和 end() 都返回随机存取迭代器，但不是所有容器的这两个函数都这样。见前面的双向链表示例。）
兼容 STL 的翻转函数应该这么写：


C++
```
template<typename T> void reverse_array_stl_compliant(T *begin, T *end) {
      // We should at first decrement 'end'
      // But only for non-empty range
      if(begin != end)
      {
           end--;
           if(begin != end) {
                while(true) {
                     swap(*begin, *end);
                     begin++;
                     If(begin == end) {
                          break;
                     }
                     end--;
                     if(begin == end) {
                          break;
                     }
                }
           }
      }
 }
```
注意，这个函数和标准函数 std::reverse(T begin, T end) 的功能一样，这个标准函数可以在算法模块找到（头文件要包含 #include <algorithm>）。
另外，只要对象定义了足够的功能函数，任何对象都可以作为迭代器传递给 STL 算法和函数。这些就是模板的强大来源。看下面的例子：


C++
```
vector<int> v;
 // ...
 vector<int> v2(v);
 vector<int> v3(v.begin(), v.end()); // v3 equals to v2
 int data[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 };
 vector<int> primes(data, data+(sizeof(data) / sizeof(data[0])));
```
最后一行代码用一个普通数组 C 构造了一个 vector。不带下标的‘data’作为一个指向数组头的指针。‘data + N’指向第 N 个元素，因此，当 N 表示数组大小时，‘data + N’就指向第一个不在数组内的元素，那么‘data + length of data’可以作为数组‘data’的尾部迭代器。表达式‘sizeof(data)/sizeof(data[0])’返回数组 data 的大小，但只在少数情况下才成立。因此，除非是用这种方法构造的容器，否则不要在任何其他情况下使用这个表达式来获取容器大小。
此外，我们甚至可以像下面这样构造容器：


C++
```
vector<int> v;
 // ...
 vector<int> v2(v.begin(), v.begin() + (v.size()/2));
```
构造的vector容器 v2 等于v 的前半部分。下面是翻转函数 reverse() 的示例：


C++
```
int data[10] = { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19 };
 reverse(data+2, data+6); // the range { 5, 7, 9, 11 } is now { 11, 9, 7, 5 };
```
每个容器都有 rbegin()/rend() 函数，它们返回反向迭代器（和正常迭代器的指向相反）。反向迭代器用来从后往前地遍历容器。因此：


C++
```
vector<int> v;
vector<int> v2(v.rbegin()+(v.size()/2), v.rend());
```
上面用 v 的前半部分来构造 v2，但顺序上前后颠倒。要创建一个迭代器对象，必须指定类型。在容器的类型后面加上“::iterator”、“::const_iterator”、“::reverse_iterator”或“::const_reverse_iterator”就可以构建迭代器的类型。因此，可以这样遍历 vector：

C++
```
vector<int> v;
 // ...
 // Traverse all container, from begin() to end()
 for(vector<int>::iterator it = v.begin(); it != v.end(); it++) {
      *it++; // Increment the value iterator is pointing to
 }
```
我推荐使用‘!=’而不是‘<’，使用‘empty()’而不要用‘size() != 0’——对于某些容器类型来说，无法高效地确定迭代器的前后顺序。
现在你了解了 STL 算法 reverse()。很多 STL 算法的声明方式相同：得到一对迭代器（一个范围的初始迭代器和尾部迭代器），并返回一个迭代器。
Find() 算法在一个区间内寻找合适的元素。如果找到了合适的元素，就返回指向第一个匹配元素的迭代器。否则，返回的值指向区间的尾部。看代码：


C++
```
vector<int> v;
 for(int i = 1; i < 100; i++) {
      v.push_back(i*i);
 }
 if(find(v.begin(), v.end(), 49) != v.end()) {
      // ...
 }
```
要得到被找到元素的下标，必须用 find() 返回的结果减去初始迭代器：


C++
```
int i = (find(v.begin(), v.end(), 49) - v.begin();
 if(i < v.size()) {
      // ...
 }
```
使用 STL 算法时，记得在源码中加上 #include <algorithm>。
Min_element 和 max_element 算法分别返回指向最小值元素和最大值元素的迭代器。要得到最小/最大值元素的值，就像在函数 find() 中一样，用 *min_element(…) 和 *max_elment(…)，在数组中减去一个容器或范围的初始迭代器来取得下标值：


C++
```
int data[5] = { 1, 5, 2, 4, 3 };
 vector<int> X(data, data+5);
 int v1 = *max_element(X.begin(), X.end()); // Returns value of max element in vector
 int i1 = min_element(X.begin(), X.end()) – X.begin; // Returns index of min element in vector
 int v2 = *max_element(data, data+5); // Returns value of max element in array
 int i3 = min_element(data, data+5) – data; // Returns index of min element in array
```
现在，你可以看到一个有效的宏定义如下：

C++
```
#define all(c) c.begin(), c.end()
```
不要将宏定义中的右边部分全部放到圆括号中去——那是错的！
另一个很好的算法是 sort()，使用很简单。思考下面的示例：

C++
```
vector<int> X;
 // ...
 sort(X.begin(), X.end()); // Sort array in ascending order
 sort(all(X)); // Sort array in ascending order, use our #define
 sort(X.rbegin(), X.rend()); // Sort array in descending order using with reverse iterators
```
### 编译 STL 程序
在这里有必要指出 STL 的错误信息。由于 STL 分布在源代码中，那编译器就必须创建有效的可执行文件，而 STL 的一个特性就是错误信息不可读。例如，如果你把一个 vector<int> 作为常引用参数（当你应该这么做的时候）传递给某个函数：


C++
```
void f(const vector<int>& v) {
      for(
           vector<int>::iterator it = v.begin(); // hm... where’s the error?..
           // ...
      // ...
 }
```
这里的错误是，你正试图对一个定义了 begin() 成员函数的常量对象创建非常量迭代器（因为识别这种错误比实际更正它更难）。正确的代码是这样：


C++
```
void f(const vector<int>& v) {
      int r = 0;
      // Traverse the vector using const_iterator
      for(vector<int>::const_iterator it = v.begin(); it != v.end(); it++) {
           r += (*it)*(*it);
      }
      return r;
 }
```
尽管如此，还是来说说‘typeof’,它是 GNU C++ 非常重要的特性。在编译过程中，这个运算符会被替换成表达式的类型。思考下面的示例：


C++
```
typeof(a+b) x = (a+b);
```
这句代码创建了变量 x，它的类型和表达式 （a + b）的类型一致。注意，对任何类型的 STL 容器来说，typeof(v.size()) 得到的值都是无符号的。但在Topcoder 上，typeof 最重要的应用是遍历容器。思考下列宏定义：


C++
```
#define tr(container, it) 
      for(typeof(container.begin()) it = container.begin(); it != container.end(); it++)
```
使用这些宏，我们可以遍历每一种容器而不仅仅是 vector。这些宏会为常量对象生成 const_iterator，为非常量对象生成常规迭代器，而你永远不会在这里出错。


C++
```
void f(const vector<int>& v) {
      int r = 0;
      tr(v, it) {
           r += (*it)*(*it);
      }
      return r;
 }
```
注意：为了提高可读性，在 #define 这一行我并没有添加额外的圆括号。阅读文章的后续部分得到更多关于 #define 的正确表述，你可以在练习室里面自己试试。
Vector 不需要真的遍历宏定义，但对于更复杂的数据类型（不支持下标，迭代器是获取数据的唯一方式）来说很方便。我们稍后会在文章中谈及这一点。
### Vector 中的数据操作
可以用 insert() 函数往 vector 中插入一个元素：


C++
```
vector<int> v;
 // ...
 v.insert(1, 42); // Insert value 42 after the first
```
从第二个（下标为1的元素）往后的所有元素都要右移一位，从而空出一个位置给新插入的元素。如果你打算添加很多元素，那多次右移并不可取——明智的做法是单次调用 insert()。因此，insert() 有一种区间形式：


C++
```
vector<int> v;
 vector<int> v2;
 // ..
 // Shift all elements from second to last to the appropriate number of elements.
 // Then copy the contents of v2 into v.
 v.insert(1, all(v2));
```
Vector 还有一个成员函数 erase，它有两种形式。猜猜都是什么：


C++
```
erase(iterator);
erase(begin iterator, end iterator);
```
第一个例子删除 vector 中的单个元素，第二个例子用两个迭代器指定区间并从vector 中删除整个区间内的元素。
### 字符串（string）
这是一个操纵字符串的特殊容器。这个字符串容器稍微不同于 vector<char>。绝大部分的不同在于字符串控制函数和内存管理策略。字符串有不支持迭代器的子串函数 substring()，只支持下标：


C++
```
string s = "hello";
string s1 = s.substr(0, 3), // "hel"
       s2 = s.substr(1, 3), // "ell"
       s3 = s.substr(0, s.length()-1), "hell"
       s4 = s.substr(1); // "ello"
```
谨防对空串执行（s.length() – 1），因为 s.length() 的返回值不带符号，而 unsigned(0) – 1 得到的结果绝对不是你想的那样。
### Set
总是很难决定要先描述哪种容器——set 还是 map。我的观点是，如果读者了解一些算法的基本知识，从‘set’开始会更容易理解。
思考我们需要一个拥有下列特性的容器：
- 添加一个元素，但不允许和已有元素重复[复制？]
- 移除元素
- 获取元素个数（不同元素的个数）
- 检查集合中是否存在某个元素
这个操作的使用相当频繁。STL 为此提供了特殊容器——set。Set 可以在 O(log N)（其中 N 是 set 中对象的个数）的时间复杂度下添加、移除元素，并检查特定元素是否存在。向 set 添加元素时，如果和已有元素值重复，那新添加的元素就会被抛弃。在常数时间复杂度 O(1) 下返回 set 的元素个数。我们将在后面讨论 set 和 map 的算法实现——现在，我们研究一下函数接口：


C++
```
set<int> s;
 for(int i = 1; i <= 100; i++) {
      s.insert(i); // Insert 100 elements, [1..100]
 }
 s.insert(42); // does nothing, 42 already exists in set
 for(int i = 2; i <= 100; i += 2) {
      s.erase(i); // Erase even values
 }
 int n = int(s.size()); // n will be 50
```
Set 不使用 push_back() 成员函数。这样是有道理的：因为 set 中元素的添加顺序并不重要，因此这里用不上 push_back()。
由于 set 不是线性容器，不可能用下标获取 set 中的元素。因此，遍历 set 元素的唯一方法就是使用迭代器。


C++
```
// Calculate the sum of elements in set
 set<int> S;
 // ...
 int r = 0;
 for(set<int>::const_iterator it = S.begin(); it != S.end(); it++) {
      r += *it;
 }
```
在这里使用遍历宏会更简洁。为什么？想象一下你有这样的容器 set<pair<string,pair<int,vector<int>>>>，怎么遍历呢？写迭代器的类型名称？天呐，还是用我们为遍历迭代器类型而定义的宏吧。


C++
```
set< pair<string, pair< int, vector<int> > > SS;
 int total = 0;
 tr(SS, it) {
      total += it->second.first;
 }
```
注意这样的语法‘it->second.first’。由于‘it’是一个迭代器，所以我们必须在运算前从‘it’得到对象。因此，正确的语法是‘(*it).second.first’。无论如何，写‘something->’总是比写‘(*something)’更容易。完整的解释会很长——只要记住，对迭代器而言两种语法都允许。
使用‘find()’成员函数确定集合 set 中是否存在某个元素。不要搞混了，因为 STL 中有很多‘find()’。有一个全局算法‘find()’，输入两个迭代器和一个元素，它能工作在 O(N) 的线性时间复杂度下。你可能会用它来搜索 set 中的元素，但是明明存在一个 O(log N) 时间复杂度的算法，为何要用一个 O(N) 的算法呢？在 set 和 map （还包括 multiset/multimap、hash_map/hash_set等容器）中搜索元素时，不要使用全局的搜索函数 find() ——反而应该使用成员函数‘set::find()’。作为‘顺序的’find函数，set::find 会返回一个迭代器，不论这个迭代器指向被找到的元素，还是指向‘end()’。因此，像这样检查元素是否存在：


C++
```
set<int> s;
 // ...
 if(s.find(42) != s.end()) {
      // 42 presents in set
 }
 else {
      // 42 not presents in set
 }
```
作为成员函数被调用时，另一个工作在 O(log N) 时间复杂度下的算法是计数函数 count。有的人认为这样


C++
```
if(s.count(42) != 0) {
      // …
 }
```
或者甚至这样


C++
```
if(s.count(42)) {
      // …
 }
```
写更方便。个人来说，我不这么想。在 set/map 中使用 count() 没有意义：元素要么存在，要么不存在。对我来说，我更愿意使用下面两个宏：


C++
```
#define present(container, element) (container.find(element) != container.end())
#define cpresent(container, element) (find(all(container),element) != container.end())
```
（记住 all(c) 代表“c.begin(), c.end()”）
这里，‘present()’用成员函数‘find()’ （比如 set/map 等等）来返回容器中是否存在某个元素，而‘cpresent’则是为 vector 定义的。
使用 erase() 函数从 set 中删除一个元素。


C++
```
set<int> s;
 // …
 s.insert(54);
 s.erase(29);
```
Erase() 函数也有区间操作形式：


C++
```
set<int> s;
 // ..
 set<int>::iterator it1, it2;
 it1 = s.find(10);
 it2 = s.find(100);
 // Will work if it1 and it2 are valid iterators, i.e. values 10 and 100 present in set.
 s.erase(it1, it2); // Note that 10 will be deleted, but 100 will remain in the container
```
Set 有一个区间构造函数：


C++
```
int data[5] = { 5, 1, 4, 2, 3 };
set<int> S(data, data+5);
```
这样可以轻松避免 vector 中的重复元素，然后排序：


C++
```
vector<int> v;
 // …
 set<int> s(all(v));
 vector<int> v2(all(s));
```
这里，‘v2’将和‘v’包含相同元素，但以升序排列，并且移除了重复元素。任何可比较的元素都可以存储在 set中。这个在后面解释。
### Map
Map 有两种解释。简单版本如下：


C++
```
map<string, int> M;
 M["Top"] = 1;
 M["Coder"] = 2;
 M["SRM"] = 10;
 int x = M["Top"] + M["Coder"];
 if(M.find("SRM") != M.end()) {
      M.erase(M.find("SRM")); // or even M.erase("SRM")
 }
```
很简单，对吧？
实际上，map 非常像 set，除了一点——它包含的不只是值而是键值对 pair<key, value>。Map 保证最多只有一个键值对拥有指定键。另一个很讨喜的地方是， map 定义了下标运算符 []。
用宏‘tr()’可以轻易遍历 map。注意，迭代器是键值对 std::pair。因此，用 it->second 来取值，示例如下：


C++
```
map<string, int> M;
 // …
 int r = 0;
 tr(M, it) {
      r += it->second;
 }
```
不要通过迭代器来更改 map 元素的键，因为这可能破坏 map 内部数据结构的完整性（见下面的解释）。
在 map::find() 和 map::operator [] 之间有一个重要的区别。Map::find() 永远不会改变 map 的内容，而操作符 [] 则会在元素不存在时创建一个新元素。有时这样做很方便，但当你不想添加新元素时，在循环中多次使用操作符 [] 绝对不是好主意。这就是为什么把 map 作为常引用参数传递给某个函数时，可能不用操作符 [] 的原因：


C++
```
void f(const map<string, int>& M) {
      if(M["the meaning"] == 42) { // Error! Cannot use [] on const map objects!
      }
      if(M.find("the meaning") != M.end() && M.find("the meaning")->second == 42) { // Correct
           cout << "Don't Panic!" << endl;
      }
 }
```
### 关于 Map 和 Set 的注意事项
从内部看，map 和 set 几乎都是以红黑树的结构存储。我们确实不必担忧内部结构，要记住的是，遍历容器时 map 和 set 的元素总是按升序排列。而这也是为何在遍历 map 或 set时，极力不推荐改变键值的原因：如果所做的修改破坏了元素间的顺序，这至少会导致容器的算法失效。
但在解决 TopCoder 的问题时，几乎都会用上 map 和 set 的元素总是有序这个事实。
另一件重要的事情是，map 和 set 的迭代器都定义了运算符 ++ 和 –。因此，如果 set 里存在值 42，而它不是第一个也不是最后一个元素，那下列代码会奏效：


C++
```
set<int> S;
 // ...
 set<int>::iterator it = S.find(42);
 set<int>::iterator it1 = it, it2 = it;
 it1--;
 it2++;
 int a = *it1, b = *it2;
```
这里的‘a’包含 42 左边的第一个相邻元素，而‘b’则包含右边的第一个相邻元素。
### 进一步讨论算法
是时候稍微深入探讨算法。大部分算法都声明在标准头文件 #include <algorithm> 中。首先，STL 提供了三种很简单的算法：min(a, b)、max(a, b)、swap(a, b)。这里，min(a, b) 和 max(a, b) 分别返回两个元素间的最小值和最大值，而 swap(a, b) 则交换两个元素的值。
算法 sort() 的使用也很普遍。调用 sort(begin, end) 按升序对一个区间的元素进行排序。注意，sort() 需要随机存取迭代器，因此它不能作用在所有类型的容器上。无论如何，你很可能永远都不会对已然有序的 set 调用 sort()。
你已经了解了算法 find()。调用 find(begin, end, element) 返回‘element’首次出现时对应的迭代器，如果找不到则返回 end。和 find(…) 相反，count(begin, end, element) 返回一个元素在容器或容器的某个范围内出现的次数。记住，set 和 map 都有成员函数 find() 和 count()，它们的时间复杂度是 O(log N)，而 std::find() 和 std::count() 的时间复杂度是 O(N)。
其他有用的算法还有 next_permutation() 和 prev_permutation()。先说说 next_permutation。调用 next_permutation(begin, end) 令区间 [begin, end) 保存区间元素的下一个全排列顺序，如果当前顺序已是最后一种全排列则返回 false。当然， next_permutation 使得许多任务变得相当简单。如果你想验证所有的全排列方式，只要这么写：


C++
```
vector<int> v;
 for(int i = 0; i < 10; i++) {
      v.push_back(i);
 }
 do {
      Solve(..., v);
 } while(next_permutation(all(v));
```
在第一次调用 next_permutation(…) 之前，别忘了确保容器中的元素已排序。元素的初始状态应该形成第一个全排列状态；否则，某些全排列状态会被遗漏，得不到验证。
### 字符串流
你常常需要进行一些字符串的处理、输入或输出，C++ 为此提供了两个有趣的对象：‘istringstream’和‘ostringstream’。这两个对象都声明在标准头文件 #include <sstream> 中。
对象 istringstream 允许你从一个字符串读入，就像从一个标准输入读数据一样。直接看源码：

C++
```
void f(const string& s) 
{
      // Construct an object to parse strings
      istringstream is(s);
      // Vector to store data
      vector<int> v;
      // Read integer while possible and add it to the vector
      int tmp;
      while(is >> tmp) 
      {
           v.push_back(tmp);
      }
 }
```
对象 ostringstream 用来格式化输出。代码如下：


C++
```
string f(const vector<int>& v) 
{
      // Constucvt an object to do formatted output
      ostringstream os;
      // Copy all elements from vector<int> to string stream as text
      tr(v, it) 
      {
           os << ' ' << *it;
      }
      // Get string from string stream
      string s = os.str();
      // Remove first space character
      if(!s.empty()) 
      { // Beware of empty string here
           s = s.substr(1);
      }
      return s;
}
```
### 总结
为了继续探讨 STL，我将总结后面会用到的模板列表。这会简化代码示例的阅读，并且希望能提高你的 TopCoder 技巧。模板和宏的简短列表如下：


C++
```
typedef vector<int> vi;
 typedef vector<vi> vvi;
 typedef pair<int,int> ii;
 #define sz(a) int((a).size())
 #define pb push_back
 #defile all(c) (c).begin(),(c).end()
 #define tr(c,i) for(typeof((c).begin() i = (c).begin(); i != (c).end(); i++)
 #define present(c,x) ((c).find(x) != (c).end())
 #define cpresent(c,x) (find(all(c),x) != (c).end())
```
由于容器 vector<int> 的使用相当普遍，因此在列表中一并列出。实际上我发现，给许多容器（尤其是 vector<string>、vector<ii>、vector<pair<double, ii>>等等）定义简短的别称非常方便。但上面的列表只给出了理解后文所需的宏。还有一点要牢记：当 #define 左侧的符号出现在右侧时，为了避免很多棘手的问题，应该在上面加上一对圆括号。
