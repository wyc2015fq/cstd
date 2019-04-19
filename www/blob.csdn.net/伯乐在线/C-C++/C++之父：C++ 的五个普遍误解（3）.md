# C++之父：C++ 的五个普遍误解（3） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Sheng Gordon](http://www.jobbole.com/members/shenggordon) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Bjarne Stroustrup](http://isocpp.org/blog/2014/12/myths-3)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
[为了增加您冬天阅读的乐趣，我们很荣幸地奉上Bjarne Stroustrup大神的这个包含3个部分的系列文章。[第一部分在这里](http://blog.jobbole.com/82460/)；[第二部分在这里](http://blog.jobbole.com/82461/)。今天我们正好在圣诞节之前完成这个系列。请欣赏。]
**1. 简介**
本系列包括 3 篇文章，我将向大家展示并澄清关于C++的五个普遍的误解：
- 1. “要理解C++，你必须先学习C”
- 2. “C++是一门面向对象的语言”
- 3. “为了软件可靠性，你需要垃圾回收”
- 4. “为了效率，你必须编写底层代码”
- 5. “C++只适用于大型、复杂的程序”
如果你深信上述误解中的任何一个，或者有同事深信不疑，那么这篇短文正是为你而写。对某些人，某些任务，在某些时间，其中一些误解曾经只是正确的。然而，在如今的C++，应用广泛使用的最先进的ISO C++ 2011编译器和工具，它们只是误解。
我认为这些误解是“普遍的”，是因为我经常听到。偶尔，它们有原因来支持，但是它们经常地被作为明显的、不需要理由的支持地表达出来。有时，它们成为某些场景下不考虑使用C++的理由。
每一个误解，都需要一大篇文章，甚至一本书来澄清，但是这里我的目标很简单，就是抛出问题，并简明地陈述我的原因。
前三个误解在我的前两篇文章中呈现。
### 5. 误解4：“为了效率，你必须编写底层代码”
许多人相信高效率的代码必须是底层代码。一些人甚至认为底层代码天生就是高效的（“如果代码很丑陋，那它一定很高效！一定有人花费了大量时间和精力来优化它！”）。当然，你仅仅使用底层代码是可以写出高效代码的，并且有时在直接处理机器资源时必须使用底层代码。然而，你一定要衡量一下你的工作是否有价值：现代C++编译器非常高效，而现代机器架构非常复杂。如果必须使用底层代码，一定要通过接口封装起来，以便于使用。通常，通过高层接口封装底层代码，会带来更好的优化（如，避免“滥用”底层代码）。在关注效率的场合，首先尝试用高层抽象来呈现需要的解决方案，而不要不加考虑地使用比特位和指针。
**5.1 C语言的qsort()**
考虑一个简单的例子。如果你需要对一组浮点数执行降序排序，你可以写一段代码来实现。然而，除非你有极端特殊的需求（如，有内存容纳不下的大量数据），这样做就太天真了。数十年来，我们一直有性能可接受的排序算法库。我最不喜欢的就是ISO标准C的qsort()算法：

C++
```
int greater(const void* p, const void* q)  // three-way compare
{
  double x = *(double*)p;  // get the double value stored at the address p
  double y = *(double*)q;
  if (x>y) return 1;
  if (x<y) return -1;
  return 0;
}
void do_my_sort(double* p, unsigned int n)
{
  qsort(p,n,sizeof(*p),greater);
}
int main()
{
  double a[500000];
  // ... fill a ...
  do_my_sort(a,sizeof(a)/sizeof(*a));  // pass pointer and number of elements
  // ...
}
```
如果你不是一个C程序员，或者你没有使用过qsort，那么需要解释一下；qsort接收4个参数：
- 指向顺序存储字节的指针
- 数据元素个数
- 每个数据元素的字节数
- 一个比较函数，参数是指向数据元素首个字节的指针
注意，这个接口丢失了信息。我们真正地目的不是对字节数据排序。我们要对浮点数排序，但是qsort并不知道，因此我们不得不提供如何比较浮点数以及浮点数占用的字节个数等信息。当然，如果编译器知道这些信息就更好了。然而，qsort的低层次接口阻止了编译器使用类型信息。不得不显式地声明简单信息也会增加出现错误的机会。qsort()函数的两个整数参数的顺序写错了吗？如果我写错了，编译器不会注意到。我的compare()函数的返回值是否遵循了C的3路比较的默认约定呢？
如果你阅读了qsort()函数的一个工业强度实现（请阅读一下），你会发现它努力地去弥补缺少的信息。例如，用交换一定数量字节的方式，来取代更有效率的浮点数交换。间接地调用比较函数也很耗时，除非编译器使用常量方式传递指针。
**5.2 C++’s sort()**
比较一下qsort()的C++等价实现，sort()：

C++
```
void do_my_sort(vector<double>& v)
{
  sort(v,[](double x, double y) { return x>y; });  // sort v in decreasing order
}
int main()
{
  vector<double> vd;
  // ... fill vd ...
  do_my_sort(v);
  // ...
}
```
这里不需要太多解释。vector知道它的长度，因此我们不需要再显式地传递元素个数了。我们不会“丢失”元素类型信息，因此也不需要处理元素的字节数。默认地，sort()以升序排序，因此我需要指定比较条件，就像在qsort()中做的一样。这里，我传递了一个使用>符号比较浮点数的lambda表达式。通常，这个lambda表达式会被我所知道的所有C++编译器内链编译，因此实际上比较操作变成了一个greater-than的机器指令；这里没有（低效的）间接函数调用。
我使用了sort()的容器版本，以避免显式地使用迭代器。即，避免像下面这样写：

C++
```
std::sort(v.begin(),v.end(),[](double x, double y) { return x>y; });
```
我也可以更进一步，使用C++14的比较对象：

C++
```
sort(v,greater<>()); // sort v in decreasing order
```
哪个版本更快呢？你可以不使用任何性能优化指令，编译C版本的qsort()和C++版本，因此这是一个真正的编程风格的比较，而不是语言的比较。标准库实现似乎一直使用与sort和qsort相同的算法，因此这是一个编程风格的比较，而不是算法的比较。当然，不同的编译器和库实现会给出不同的结果，但是对于每种实现，我们对不同层次抽象的结果有一个合理的认识。
最近我运行了这个例子，并且发现sort()版本比qsort()版本快2.5倍。你会因为编译器和机器的差别，得到不同的结果，但是我从来没看到qsort打败过sort。我甚至看到过sort比qsort快10倍。为什么呢？很明显C++标准库sort相比qsort，是一个更高层次的抽象，更通用和灵活。它类型安全，并使存储类型，元素类型和排序算法参数化。它里面看不到指针，类型转换，长度，或者字节。C++标准库STL，包括sort，努力地尝试不丢失任何信息，从而得到了良好的内链和优化效果。
普适性和高层代码能够击败低层代码。当然不是一直这样，但是sort/qsort的比较不是一个孤立案例。总是从高层，精准和类型安全的版本开始解决方案。（仅当）如果需要时则优化。
### 6. 误解5：“C++只适用于大型、复杂的程序”
C++是一个巨型的语言。它定义的大小和C#与Java差不多。但是这并不意味着你必须知道每一个细节，或者在每一个程序中都用到所有特性。考虑一个只使用标准库基本组件的例子：

C++
```
set<string> get_addresses(istream& is)
{
  set<string> addr;
  regex pat { R"((\w+([.-]\w+)*)@(\w+([.-]\w+)*))"}; // email address pattern
  smatch m;
  for (string s; getline(is,s); )                    // read a line
    if (regex_search(s, m, pat))                     // look for the pattern
      addr.insert(m[0]);                             // save address in set
  return addr;
}
```
我假设你知道正则表达式。如果不知道，现在或许是一个阅读它的好时机。注意，我使用move语法来对返回潜在地大量字符串进行简化和提升效率（译者注：move语法在[本系列第二篇](http://blog.jobbole.com/82461/)讲解）。所有的标准库容器都支持move构造方法，因此这里不需要使用new。
为了能正常工作，我需要引用适当的标准库组件：

C++
```
#include<string>
#include<set>
#include<iostream>
#include<sstream>
#include<regex>
using namespace std;
```
测试一下：

C++
```
istringstream test {  // a stream initialized to a sting containing some addresses
  "asasasa\n"
  "bs@foo.com\n"
  "ms@foo.bar.com$aaa\n"
  "ms@foo.bar.com aaa\n"
  "asdf bs.ms@x\n"
  "\(bs.ms@x\)goo\n"
  "cft foo-bar.ff@ss-tt.vv@yy asas"
  "qwert\n"
};
int main()
{
  auto addr = get_addresses(test);  // get the email addresses
  for (auto& s : addr)              // write out the addresses
    cout << s << '\n';
}
```
这仅仅是一个例子。可以很容易地修改get_addresses()，把regex模式作为参数，从而它能够找到URL或者其他任何东西。也可以很容易地修改get_addresses()，使在每行文字识别超过一个模式。虽然C++是为灵活性和通用性而设计的，但不是每个程序都是一个完整的库，或者应用程序框架。然而，这里的关键点是，从流数据中提取邮件地址任务能够被简单地实现，并很容易测试。
**6.1 库**
对任何语言，只使用语言内置特性（如if，for和+）编写程序是相当乏味的。通常，会给出适当的库（如图形，路线规划和数据库），可以让几乎所有的任务都能够在合理的工作量内完成。
ISO C++标准库相对小一些（相对于商业库），但是“就在那里”，有大量的开源和商业库。例如，利用（开源或有版权的）库，如Boost[3]，POCO[2]，AMP[4]，TBB[5]，Cinder[6]，vxWidgets[7]，和CGAL[8]，很多通用和专业的任务变得简单。作为例子，让我们修改上面的程序，从网页内读取URL。首先，我们改变get_addresses()来查找符合模式的任意字符串：

C++
```
set<string> get_strings(istream& is, regex pat)
{
  set<string> res;
  smatch m;
  for (string s; getline(is,s); )  // read a line
  if (regex_search(s, m, pat))
    res.insert(m[0]);              // save match in set
  return res;
}
```
这很简单。接下来，我们需要考虑如何登录到网页并读取文件。Boost有一个库，asio，可以与网页通讯：

C++
```
#include “boost/asio.hpp” // get boost.asio
```
需要连接到web服务器：

C++
```
int main()
try {
  string server = "www.stroustrup.com";
  boost::asio::ip::tcp::iostream s {server,"http"};  // make a connection
  connect_to_file(s,server,"C++.html");    // check and open file
  regex pat {R"((http://)?www([./#\+-]\w*)+)"}; // URL
  for (auto x : get_strings(s,pat))    // look for URLs
    cout << x << '\n';
}
catch (std::exception& e) {
  std::cout << "Exception: " << e.what() << "\n";
  return 1;
}
```
查看[www.stroustrup.com](http://www.stroustrup.com)网站上的C++.html文件，内容如下：


```
http://www-h.eng.cam.ac.uk/help/tpl/languages/C++.html
http://www.accu.org
http://www.artima.co/cppsource
http://www.boost.org
...
```
我使用了set类型，因此URL会以字母顺序打印出来。
我在一个函数（connect_to_file()）中偷偷地“隐藏”了检测和HTTP连接管理，这并非不切实际：

C++
```
void connect_to_file(iostream& s, const string& server, const string& file)
  // open a connection to server and open an attach file to s
  // skip headers
{
  if (!s)
    throw runtime_error{"can't connect\n"};
  // Request to read the file from the server:
  s << "GET " << "http://"+server+"/"+file << " HTTP/1.0\r\n";
  s << "Host: " << server << "\r\n";
  s << "Accept: */*\r\n";
  s << "Connection: close\r\n\r\n";
  // Check that the response is OK:
  string http_version;
  unsigned int status_code;
  s >> http_version >> status_code;
  string status_message;
  getline(s,status_message);
  if (!s || http_version.substr(0, 5) != "HTTP/")
    throw runtime_error{ "Invalid response\n" };
  if (status_code!=200)
    throw runtime_error{ "Response returned with status code" };
  // Discard the response headers, which are terminated by a blank line:
  string header;
  while (getline(s,header) && header!="\r")
    ;
}
```
由于这是最常见的，我没有从头开始。HTTP连接管理大部分是从Christopher Kohlhoff的asio文档[9]中复制来的。
**6.2 Hello，World！**
C++是一种编译型语言，设计它的首要目标是在关注性能和可靠性的场合，提供良好、可维护的代码（如，基础设施[10]）。它不是有意在小型程序中，直接和解释型或小型编译“脚本”语言竞争。的确，这类语言（如JavaScript，或Java）通常是用C++实现的。但是有很多只有数十行或几百行的很有用的C++程序。
这里提供一个能简单尝试的“Hello，World”例子，而不是（只）关注一个库聪明和先进的部分。简单安装一个最小化的库，写一个最多一页的“Hello，World”例子，展示一个库能够做什么。在某些时候我们都是菜鸟。这里，我C++版本的“Hello，World”是：

C++
```
#include<iostream>
int main()
{
  std::cout << "Hello, World\n";
}
```
在展示ISO C++和标准库的时候，我发现了更长、更复杂、更无趣的版本。
### 7. 误解的多种“用途”
在现实中误解有时是有基础的。对每一个误解，某些人都会有多次经验或情形导致他们有理由相信。今天，我认为它们是完全错误的、被误解的，实话实说。一个问题是，误解通常是为了支撑一个目的——或者它们已经消失了。这5个误解扮演着多个角色：
- 提供安慰：不需要改变；不需要尝试变革。这样感到很舒适。变化可能失败，因此相信新事物不可行会更好。
- 在开始一个新项目时，这样可以节省时间：如果你（你自己）知道C++是什么，你不需要再花费时间学习新知识。你不需要尝试新技术。你不需要衡量潜在的性能障碍。你不需要培训新的开发者。
- 你可以不必学习C++：如果这些误解是真的，你究竟为什么要学习C++？
- 促进替换语言和技术：如果这些误解是真的，那么明显需要替换。
但是这些误解不是真的，因此理智地改进现状，替换C++，或者避免现代C++编程风格，都不能依靠这些借口。从旧的C++观点来看（用熟悉的语言子集和技巧）可能会舒服一点，但是软件就是这样，改变是必须的。我们可以比C，“带类的C”，C++98等做的更好。
坚持曾经正确的看法，并不是没有代价。相比现代的代码，它的维护代码更高。老的编译器和工具集相比依赖现代结构化代码的现代工具，更加低效和难以分析。
现代C++（C++11，C++14）以及它支持的编程技术，与“常见的误解”所代表的不同，并且比它要好的多。
如果你深信这些误解之一，不要马上就相信我的话，认为它是错误的。尝试。测试。通过你关心的一些问题，衡量“老方法”和新的替换思路。尝试真正地把握学习新工具和技术的时机，使用新方法写代码的时机，应用现代代码的时机。不要忘记与坚持“老方法”比较可能的维护代价。澄清误解的最佳方法是拿出证据。我在这里只呈现例子和做出讨论。
不，这不是一场“C++很完美”的讨论。C++并不完美；它不是对每个人、每件事都最好的语言。其他语言也不是。接受C++现在的样子，而不是20年前它的样子，也不是某些人声明它是什么样子。为了做出理性的选择，拿出一些真正的信息——只要时间允许——亲自尝试目前的C++如何处理你遇到的问题。
### 8. 总结
不要在没有证据的情况下，相信C++的这些“常识”或使用它。这篇文章呈现了最频繁表达的5个观点，并逐一澄清，说明它们“仅仅是误解”：
- 1. “要理解C++，你必须先学习C”
- 2. “C++是一门面向对象的语言”
- 3. “为了软件可靠性，你需要垃圾回收”
- 4. “为了效率，你必须编写底层代码”
- 5. “C++只适用于大型、复杂的程序”
它们是有害的。
### 9. 反馈
还有疑问？告诉我原因。你遇到过其他什么误解了吗？为什么他们相信误解而不是实际经验？你有哪些证据可能揭露一个误解呢？
### 10. 参考文献
- 1. ISO/IEC 14882:2011 Programming Language C++
- 2. POCO libraries: [http://pocoproject.org/](http://pocoproject.org/)
- 3. Boost libraries: [http://www.boost.org/](http://www.boost.org/)
- 4. AMP: C++ Accelerated Massive Parallelism. [http://msdn.microsoft.com/en-us/library/hh265137.aspx](http://msdn.microsoft.com/en-us/library/hh265137.aspx)
- 5. TBB: Intel Threading Building Blocks. [www.threadingbuildingblocks.org/](http://www.threadingbuildingblocks.org/)
- 6. Cinder: A library for professional-quality creative coding. [http://libcinder.org/](http://libcinder.org/)
- 7. vxWidgets: A Cross-Platform GUI Library. [www.wxwidgets.org](http://www.wxwidgets.org)
- 8. Cgal – Computational Geometry Algorithms Library. [www.cgal.org](http://www.cgal.org)
- 9. Christopher Kohlhoff : Boost.Asio documentation. [http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio.html](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio.html)
- 10. B. Stroustrup: Software Development for Infrastructure. Computer, vol. 45, no. 1, pp. 47-58, Jan. 2012, doi:10.1109/MC.2011.353.
- 11. Bjarne Stroustrup: The C++ Programming Language (4th Edition). Addison-Wesley. ISBN 978-0321563842. May 2013.
- 12. Bjarne Stroustrup: A Tour of C++. Addison Wesley. ISBN 978-0321958310. September 2013.
- 13. B. Stroustrup: Programming: Principles and Practice using C++ (2nd edition). Addison-Wesley. ISBN 978-0321992789. May 2014.
