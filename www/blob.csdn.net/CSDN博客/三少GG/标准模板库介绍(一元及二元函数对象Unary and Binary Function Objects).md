# 标准模板库介绍(一元及二元函数对象Unary and Binary Function Objects) - 三少GG - CSDN博客
2011年12月16日 15:56:11[三少GG](https://me.csdn.net/scut1135)阅读数：1586
第24章标准模板库介绍
Chapter 24 Introducing the Standard Template Library
这一章介绍标准模板库－许多人认为这是最近几年被添加到C++的最重要特性。加入STL(standard template library)是C++标准过程中的一个重要举措。STL提供了普遍目的、模板化了的类和函数，这些类和函数实现了很多普遍和常见的算法和数据结构，例如包含了对向量、列表、队列和堆栈的支持。STL由模板类构造，所以算法和数据结构几乎能应用到任何一种数据类型。
STL是一项使用C++最多变特性的软件工程，为了理解和使用STL，你必须完全明白C++语言，包括指针，引用和模板。坦白地说，描述STL的模板语法看起来相当可怕，它看起来比实际用起来复杂得多。这一章是本书最难的部分，所以当你一开始就发现学习STL很迷惑时不用感到惊讶或失意。耐心研究例子，不要让不熟悉的STL语法掩盖了原本简单的STL。
本章的目的是显现STL的概况，包括它的设计体系、组织、成分和使用它的编程技术。由于STL是一个大库，这里无法详细讨论它的各种特性。不过，第四部分提供有STL的完全参考。
这一章同时也讨论了C++最重要的类：string。string类定义了一种使用字符串的字符串数据类型，就像你使用其它数据类型一样：使用运算符。string类与STL联系很紧密。
STL综述
尽管标准模板库很大并且它的语法可能很可怕，一旦你明白了它的构造和使用它所引进的元素，STL实际上很简单。然而，在看任何例子代码之前，综述一下STL还是有必要的。
标准模板库的核心是三个基础项：容器(containers)、算法(algorithms)和迭代器(iterators)，这些项结合在一起提供能解决不同编程问题的现成解决方案。
容器Containers
容器(containers)是容纳其它对象的对象，并且有几种不同类型。例如，vector类定义了一种动态数组，deque创建了一种双端队列，而list提供了一个线性列表。这些容器被叫做序列容器(sequence
 containers)，因为在STL术语中，序列是一个线性列表。除基本容器外，STL同时也定义了联合容器(associative containers)，这些容器允许通过关键字获取值。例如， map提供了通过唯一键存取值的方法，这样，一个存取“键－值”对的映射map就可以通过使用“键”来取回值。
每个容器类定义了一组可能应用到该容器的函数集合。例如，list包括了插入、删除和合并元素等函数，stack则包括了压入和弹出元素等函数。
算法Algorithms
算法基于容器起作用。它们提供了操作容器内容的方法。它们有初始化、排序、搜索和转换容器内容的能力。很多算法操作容器内某个区间的元素。
迭代器Iterators
迭代器是行为对象，一定程度上像指针。就像指针遍历数组一种，它给了你遍历容器内容的能力。有五种类型的迭代器
|迭代器|允许的操作|
|----|----|
|随机|存值和取值。元素要可以随机访问。|
|双向|存值和取值。向前或向后移动。|
|正向|存值和取值。仅向前移动。|
|输入|取值但不存值。仅向前移动。|
|输出|存值但不取值。仅向前移动。|
一般地，一个有较大访问能力的迭代器可以代替有较少访问能力的迭代器的地方。例如，正向迭代器可以代替输入迭代器。
迭代器就像指针一样运作。你可以递增或递减它们，也可以在它们前面加上*操作符。迭代器使用在各种容器中定义的iterator来声明类型。
STL同时也支持反向迭代器(reverse iterators)。反向迭代器只能是在序列中能反向移动的双向或随机迭代器。
因此，如果一个反向迭代器指在一个序列的末尾，递增后反向迭代器将指向序列的末尾的前一个元素。
当模板描述中提及迭代器变量类型时，本书将使用下面的术语：
|术语|代表|
|----|----|
|BiIter|双向迭代器|
|ForIter|向前迭代器|
|InIter|输入迭代器|
|OutIter|输出迭代器|
|RandIter|随机访问迭代器|
其它STL元素
除了容器、算法和迭代器之外，STL还依赖于其它几种标准组件的支持。这些组件中最主要是分配器、断言、比较函数和函数对象。
每个容器都有定义了一个分配器(allocator)。分配器为容器管理内存分配。默认分配器是类allocator的一个对象，不过如果需要，你可以为特定的应用程序定义你自己的分配器。对于大多数应用，默认分配器已经足够了。
不少算法和容器使用一种叫断言(predicate)的特殊类型函数。一共有两种断言变量：一元的(unary)和二元的(binary)。一元断言带有一个参数，而二元断言带有二个参数。这些函数返回true/false。但返回true或false的精确条件是由你定义的。在本章的剩余部分，当需要一元断言函数时，将用符号UnPred表示。而二元断言函数则用符号BinPred。在二元断言函数中，参数总是以first，second的顺序出现。不管是一元还是二元断言，参数包含了保存在容器的对象类型的值。
一些算法和类使用一种特殊类型的二元断言，这种断言比较两个元素。如果第一个参数比第二个参数小，比较函数返回true。比较函数用符号Comp标示。
除了不同的STL类需要的头文件外，C++标准库包含了支持STL的头文件<utility>和<functional>。例如，能同时容纳一对值的模板类pair是在<utility>中定义的。我们将在这一章后面使用pair。
<functional>中的模板能构建定义了operator()的对象。这些对象叫函数对象(function objects)，在很多时候它们可以代替函数指针。它们是：
|plus|minus|multiplies|divides|modulus|
|----|----|----|----|----|
|negate|equal_to|not_equal_to|greater|greater_equal|
|less|less_equal|logical_and|logical_or|logical_not|
可能用得最广泛的函数对象是用来确定一个对象比另一个对象小的less。函数对象在稍候将讨论STL算法中可以用来代替实际的函数指针。与使用函数指针相比，使用函数对象更能使STL产生高效的代码。
另外两个构成STL的实体是联编（binders）和倒换（negators），联编把一个函数对象绑定到一个参数上，倒换则返回一个断言的补码。
最后一个要知道的术语是适配器adaptor，在STL术语中，适配器把一种东西转换成另一种。比如，容器queue（创建了一个标准队列）是deque容器的适配器。
容器类
就如前面说的，容器是STL对象实际储存数据的地方。由STL定义的容器如表24-1所示。同时也显示了每个容器所需要的头文件。管理字符串的类string也是一个容器，不过它将在本章后面介绍。
|容器|描述|所需要的头文件|
|----|----|----|
|bitset|位的集合|<bitset>|
|deque|双端队列|<deque>|
|list|线性列表|<list|
|map|储存“键－值”对，每个键和一个值相关联|<map>|
|multimap|储存“键－值”对，每个键可以和两个或更多个值关联|<map>|
|multiset|每个元素不必都是独一的集合|<set>|
|priority_queue|优先队列|<queue>|
|queue|队列|<queue>|
|set|每个元素都是独一的集合|<set>|
|stack|堆栈|<stack>|
|vector|动态数组|<vector>|
|表24-1　由STL定义的容器| | |
因为在模板类中声明通用占位符的名字是任意的，容器类声明这些类型用typedef。这使类型名变得具体。以下是常见的一些typedef名：
|size_type|整数类型|
|----|----|
|reference|元素引用|
|const_reference|元素的恒量引用|
|iterator|迭代器|
|const_iterator|恒迭代器|
|reverse_iterator|反向迭代器|
|const_reverse_iterator|恒反向迭代器|
|value_type|容器中保存的值的类型|
|allocator_type|分配器的类型|
|key_type|键的类型|
|key_compare|比较两个键的函数的类型|
|value_compare|比较两个值的函数的类型|
运作的一般理论
虽然STL内部的运作是非常深奥的，但使用STL实际上非常简单。首先，你必须确定你要使用的容器类型。每一个类型有都某些优点和折衷。例如，当需要随机访问、对象类似数组并且不需要太多删除或插入时，vector是非常好用的。list则在插入或删除时有优势，但速度变慢。map提供联合容器，当然，需要有额外的花费。
一旦你选定了容器，你将使用它的成员函数向容器增加元素，访问或修改这些元素，删除元素等。除bitset之外，向容器增加元素时容器会在需要时自动增长，而在移动元素时自动收缩。
可以用几种不同的方法从容器增加或移去元素。比如，不但序列容器(vector，list和deque)而且联合容器(map，multimap，set和multiset)都提供了向容器插入元素的insert()成员函数和从容器中移去元素的erase()成员函数。序列容器还提供了提供了push_back()和pop_back()，它们分别向容器末尾增加元素或从末尾移去元素。这些函数可能是从序列容器单独增加或移去元素的最常用的方法。list和deque还包含了从容器开头增加或移去元素的push_front()和pop_front()。
访问容器中的元素的最常用的一种方法是使用迭代器。序列和联合容器都提供了成员函数begin()和end()，它们各自返回容器开头和末尾的迭代器。当需要访问容器的内容时，这些迭代器非常有用。比如，当要遍历一个容器时，你可以使用begin()获得指向容器开头的迭代器，然后递增它，直到迭代器的值等于end()。
联合容器提供了通过键值定位联合容器中的元素的函数find()，由于联合容器中元素的键和值连在一起，find()就是用来定位容器中的元素的常用函数。
一旦你有一个存储信息的容器，这些信息可使用一种或多种算法来操纵。这些算法不仅允许你以某些规定的形式更改容器的内容，而且允许你把一种序列类型转换成另外一种。
在下面的章节中，你将学习应用这些通用技术到vector、list和map这三种典型容器中。一旦你明白这些容器是如何工作的，使用其它容器就不会有什么困难了。
向量Vectors
可能最通用的容器要算vector了。vector类支持动态数组。这种数组能够在需要时增长。你知道，在C++中，数组的大小在编译时已经固定。虽然这是到目前为止实现数组最高效的方法，但同时也是最受约束的地方，因为数组的大小不能随程序的运行环境不同而在运行时调整。vector通过在需要时分配内存解决了这个难题。虽然vector是动态的，你仍然可以使用标准数组的下标记号来访问元素。
下面是vector的模板格式：
template <class T, class Allocator=allocator <T> > class vector
这里，T是储存的数据的类型，而Allocator指定了分配器，默认是标准分配器。vector具有如下的构造函数：
explicit vector(const Allocator & a=Allocator());
explicit vector( size_type *num*,const T &*val*=T(),const Allocator &*a*=Allocator());
vector(const vector<T,Allocator> &*ob*);
template<class InIter> vector(InIter *start*,InIter *end*, const Allocator &*a*=Allocator());
第一种形式创建一个空向量。第二种形式创建了一个含有*num*个元素、每个元素值都为*val*的向量，*val*的值允许使用默认值。第三种形式创建了一个与*ob*含有相同元素的向量。第四种形式创建了一个具有与迭代器*start*到*end*区间内的元素一样的向量。
出于最大灵活性和可移植性考虑，储存在vector的对象都应该定义一个默认构造函数，同时还必须定义<和= =运算符。某些编译器甚至要求定义其它一些比较运算符（由于实现的不同，更精确的内容你得查阅编译器文档说明）。所有的内置类型自动满足这些要求。
尽管模板语法看起来相当复杂，但声明一个向量一点都不难。这里是一些例子：
vector<int> i;                       //创建一个零长度的整形向量
vector<char> cv(5)                     //创建一个有5个元素的字符向量
vector<char> cv(5,’x’)         //创建一个初始化了5个元素的字符向量
vector<int> iv2(iv)               //从一个整形向量中创建向量
下面的比较运算符已经在vector定义：
= =、<、<=、!=、>、>=
下标运算符[]也已经在vector定义，这允许你标准数组下标记号访问向量中的元素。
vector定义的几个成员函数如表24-2中所示（记住，第四部分包含了STL类的完全参考）。其中最常用的成员函数是size()、begin()、end()、push_back()、insert()和erase()。size()函数返回当前向量的大小。这个函数可以在运行时确定向量的大小，因此非常有用。记住，向量在需要时会增加它的大小。所以向量的大小必须在执行时确定，而非在编译时。
begin()函数返回指向向量开头的迭代器。end()函数返回指向向量末尾的迭代器。如前所说，迭代器与指针很相像，通过使用begin()和end()函数得到指向向量的开头和末尾的迭代器。
push_back()函数把一个值放到向量的末尾。如果需要，向量会增加它的大小来容纳新元素。你也可以用insert()函数向向量的中间插入元素。向量也可以被初始化。不管在什么情况下，一旦向量含有元素，你可以使用数组下标来访问或修改这些元素。你可以使用erase()从向量中移去元素。
|成员|描述|
|----|----|
|reference back();const_reference back() const;|返回向量中最后一个元素的引用。|
|iterator begin();const_iterator begin() const;|返回指向向量中第一个元素的迭代器。|
|void clear();|从向量中移去所有的元素。|
|bool empty() const;|如果调用的向量为空，则返回true，否则返回false。|
|iterator end();const_iterator end() const;|返回指向向量末尾的迭代器。|
|iterator erase(iterator *i*);|移去由*i*所指的元素，返回指向被移去元素的下一个元素的迭代器。|
|iterator erase(iterator *start*,iterator *end*);|移去*start*和*end*之间的元素。返回指向最一个被移去元素的下一个元素的迭代器。|
|reference front();const_reference front() const;|返回向量中第一个元素的引用。|
|iterator insert(iterator *i*,const T &*val*);|在*i*所指元素的前面直接插入*val*，返回指向所插入元素的迭代器。|
|void insert(iterator *i*, size_type *num*, const T &*val*);|在*i*所指元素的前面直接插入*num*个*val*的副本。|
|template <class InIter>void insert(iterator* i*,InIter *start*,InIter* end*);|在*i*所指的元素前面直接插入由*start*和*end*定义的序列。|
|reference operator[](size_type *i*) const;const_reference operator[](size_type *i*) const;|返回由*i*所指元素的引用。|
|void pop_back();|移去向量的最后一个元素。|
|void push_back(const T & *val*);|在向量末尾插入值由*val*指定的元素。|
|size_type size() const;|返回向量当前的元素的数量。|
|表24-2 由vector定义的一些常用成员函数| |
这里是阐明向量基本运作的简短例子。
//向量示例
#include <iostream>
#include <vector>
#include <cctype>
using namespace std;
int main()
{
  vector<char> v(10);//创建一个长度为10的向量
  unsigned int i;
//显示v的原始长度
  cout<<"Size= "<<v.size()<<endl;
//给向量的元素赋值
  for(i=0; i<10; i++) v[i]=i+'a';
//显示向量的内容
  cout<<"Current Contents: /n";
  for(i=0; i<v.size(); i++) cout<< v[i]<< " ";
  cout<<"/n/n";
  cout<<"Expanding vector/n";
/*在向量末尾增加值，向量将在需要时增长。*/
  for(i=0; i<10; i++) v.push_back(i+10+'a');
//显示当前v的大小。
  cout<<"Size now= "<< v.size() <<endl;
//显示向量的内容
  cout<<"Current contents:/n";
  for(i=0;i<v.size();i++) cout<<v[i]<<" ";
 cout<<"/n/n";
//改变向量的内容
  for(i=0; i<v.size();i++) v[i]=toupper(v[i]);
  cout<<"Modified Contents:/n";
  for(i=0;i<v.size();i++) cout<<v[i]<<" ";
  cout<<endl;
  return 0;
}
这个程序的输出如下：
Size= 10
Current Contents:
a b c d e f g h i j
Expanding vector
Size now= 20
Current contents:
a b c d e f g h i j k l m n o p q r s t
Modified Contents:
A B C D E F G H I J K L M N O P Q R S T
让我们来仔细地看这个程序。在main()中，字符向量v在创建时容量被初始化为10，这就是说，v一开始就有10个元素。这一点我们可以通过成员函数size()得到验证。接下来，这10个元素被初始化为字符a到j，然后v的内容被显示出来。注意这里使用了标准数组的下标记号。接着，使用push_back()函数把另外10个元素添加到了向量的末尾。为了容纳这些新元素，v增大。如输入所示，增加元素之后，v的大小变成了20。最后，通过使用标准下标记号，v的元素的值被改变。
这个程序还有另外一个有趣的地方。在显示v的内容的循环中，目标值是v.size()。与数组相比，向量的一个优点是它可以找出当前向量的大小。你不难想像，在不同的环境中，这可能相当有用。
使用迭代器访问向量
你知道，在C++中，数组和指针的联系很紧密。数组可以通过下标或指针访问。在STL中，向量和迭代器的关系与此相类似。你可以通过下标或迭代器访问向量的成员。下面的例子显示如何使用。
//通过迭代器访问向量的元素。
#include <iostream>
#include <vector>
#include <cctype>
using namespace std;
int main()
{
  vector<char> v(10);//创建一个长度为10的向量
  vector<char>::iterator p;//创建一个迭代器
  int i;
//给向量的元素赋值
  p=v.begin();
  i=0;
  while(p!=v.end()){
         *p=i+'a';
         p++;
         i++;
  }
//显示向量的内容
  cout<<"Origianl Contents: /n";
  p=v.begin();
  while(p!=v.end()){
         cout<<*p<<" ";
         p++;
  }
  cout<<"/n/n";
//改变向量的内容
  p=v.begin();
  while(p!=v.end()){
         *p=toupper(*p);
         p++;
  }
//显示向量的内容
  cout<<"Modified Contents:/n";
  p=v.begin();
  while(p!=v.end()){
         cout<<*p<<" ";
         p++;
  }
  cout<<endl;
  return 0;
}
程序的输出是：
Origianl Contents:
a b c d e f g h i j
Modified Contents:
A B C D E F G H I J
在这个程序中，注意迭代器p是如何声明的。类型iterator是在容器类中定义的。这样，得到特定容器的一个迭代器，使用的声明与例子中的声明一样。只需要用容器名限定iterator。在上面的程序中，p通过使用成员函数begin()被初始化指向向量的开头。这个函数返回指向向量开关的迭代器。这个迭代器可以一次访问向量的一个元素，在需要时迭代器可以递增。这种处理方法与指针访问数组的方法非常相似。为了确定已经到达向量的末尾，这里使用了成员函数end()，这个函数返回指向向量最后一个元素后面的迭代器，这样，当p等于v.end()时，向量已经达到末尾。
在向量中插入和删除元素
除了在向量末尾增加值之外，你也可以使用insert()函数向向量中间插入元素。你也可以使用erase()函数移去元素。下面的程序演示了insert()和erase()的使用。
//插入和删除示例
#include <iostream>
#include <vector>
using namespace std;
int main()
{
  vector<char> v(10);
  vector<char> v2;
  char str[]="<Vector>";
  unsigned int i;
//初始化v
  for(i=0; i<10; i++) v[i]=i+'a';
//复制str中的字符到v2
  for(i=0; str[i]; i++) v2.push_back(str[i]);
//显示向量的原始内容
  cout<<"Original contents of v:/n";
  for(i=0;i<v.size();i++) cout<<v[i]<<" ";
  cout<<"/n/n";
  vector<char>::iterator p=v.begin();
  p +=2;//指向第三个元素
//向v插入10个X。
  v.insert(p,10,'X');
//显示插入后的内容
  cout<<"Size after inserting X's="<<v.size()<<endl;
  cout<<"Contents after insert:/n";
  for(i=0;i<v.size();i++) cout<<v[i]<<" ";
  cout<<"/n/n";
//移去这些元素
  p=v.begin();
  p+=2;//指向第三个元素
  v.erase(p,p+10);//移去后面10个元素。
//显示删除后的内容
  cout<<"Size after erase = "<<v.size()<<endl;
  cout<< "Contents after erase:/n";
  for(i=0;i<v.size();i++) cout<<v[i]<<" ";
  cout<<"/n/n";
//把v2插入到v中
  v.insert(p,v2.begin(),v2.end());
  cout<<"Size after v2's insertion = ";
  cout<<v.size()<<endl;
  cout<<"Contents after insert:/n";
  for(i=0;i<v.size();i++) cout<<v[i]<<" ";
  cout<<endl;
  return 0;
}
程序的输入出下：
Original contents of v:
a b c d e f g h i j
Size after inserting X's=20
Contents after insert:
a b X X X X X X X X X X c d e f g h i j
Size after erase = 10
Contents after erase:
a b c d e f g h i j
Size after v2's insertion = 18
Contents after insert:
a b < V e c t o r > c d e f g h i j
这个程序演示了两种形式的insert()，第一次使用时是向v插入10个X，第二次是把另一个向量v2插入到v，第二次的使用是最有意思的，它带有三个迭代器参数，第一个指定了要在调用的容器中插入的地方，后面二个参数指定了被插入序列的起点和终点。
在向量在储存类对象
虽然前面的例子在向量只储存内置类型对象，但向量并不仅限于此。它们可以储存任何类型的对象，包括你自己的创建的类。这里有个使用向量储存对象的例子，对象中保存的是一周来每天的最高温度。。注意DailyTemp定义了默认的构造函数，也提供了<和=
 =的重载版本。记住，这些（或其它）比较操作符可能需要自己定义，这取决于你的编译器对STL的实现。
//在向量中保存类对象
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;
class DailyTemp{
       int temp;
public:
       DailyTemp() { temp = 0; }
       DailyTemp(int x) { temp= x; }
       DailyTemp &operator=(int x) {
              temp=x;return *this;
       }
       double get_temp() { return temp; }
};
bool operator<(DailyTemp a, DailyTemp b)
{
       return a.get_temp()<b.get_temp();
}
bool operator==(DailyTemp a, DailyTemp b)
{
       return a.get_temp() == b.get_temp();
}
int main()
{
       vector<DailyTemp> v;
       unsigned int i;
       for(i=0; i<7; i++)
              v.push_back(DailyTemp(60 + rand()%30));
       cout<<"Fahrenheit temperatures:/n";
       for(i=0; i<v.size(); i++)
              cout<< v[i].get_temp()<< " ";
       cout<<endl;
//从华氏度转换成摄氏度
       for(i=0; i<v.size(); i++)
              v[i]=(int)(v[i].get_temp()-32) * 5/9 ;
       cout<<"Centigrade temperatures:/n";
       for(i=0; i<v.size(); i++)
              cout << v[i].get_temp() << " ";
       return 0;
}
程序的输出如下
Fahrenheit temperatures:
71 77 64 70 89 64 78
Centigrade temperatures:
21 25 17 21 31 17 25
向量具有强大的功能、高安全性和高灵活性，但是与普通的数组相比，效率较低。因此，对于一般的编程任务，普通数组仍然是你的第一选择。但你要看看使用向量带来的好处是超过它的花费。
列表Lists
列表类list支持双向、线性列表，跟支持随机访问的向量不一样，列表只能顺序访问。既然列表是双向的，它们可以从头到尾或从尾到头访问。
列表具有这样的模板格式：
template <class T, class Allocator=allocator<T> > class list
这里，T是储存在列表中的数据的类型。分配器由Allocator指定，默认的分配器是标准分配器。它具有如下的构造函数：
explicit list(const Allocator &*a* = Allocator() );
explicit list(size_type *num*,const T &*val*=T(), const Allocator &*a*=Allocator());
list(const list<T,Allocator> &*ob*);
template <class InIter> list(InIter *start*,InIter* end*,const Allocator & *a*=Allocator());
第一种形式创建一个空列表，第二种形式创建一个有*num*个元素、每个元素值都为*val*的列表，元素值也允许缺省。第三种形式创建一个与*ob*具有同样元素的列表。第四种形式创建一个与迭代器*start*到*end*区间内的元素一样的列表。
以下的比较运算符也已经在列表list中定义了。
= =，<，<=，!=，>，>=
表24－3显示的是列表list常用的一些成员函数。与向量vector一样，可以使用push_back()函数把元素添加到列表中，使用push_front()可以把元素添加到列表的开头，使用insert()可以把元素插入到列表的中间，使用splice()可以把两个列表接合在一起，使用merge()把把一个列表合并到另一个列表中去。
|成员|描述|
|----|----|
|reference back();const_reference back() const;|返回列表中最后一个元素的引用。|
|iterator begin();const_iterator begin() const;|返回指向列表中第一个元素的迭代器。|
|void clear();|从列表中移去所有元素。|
|bool empty() const;|如果调用的列表为空，返回true，否则返回false。|
|iterator end();const_iterator end() const;|返回指向列表末尾的迭代器。|
|iterator erase(iterator *i*);|从列表中移去*i*所指的元素，返回指向被移去元素后面的元素的迭代器。|
|iterator erase(iterator *start*,iterator *end*);|从列表中移去*start*到*end*区间内的元素，返回指向最后一个被移去元素后面的元素的迭代器。|
|reference front();const_reference front() const;|返回列表中第一个元素的引用。|
|iterator insert(iterator *i*,const T &* val*);|直接在*i*所指元素前面插入*val*，返回指向被插入元素的迭代器。|
|void insert(iterator *i*,size_type *num*, const T &*val*);|直接在*i*所指元素前面插入*num*个*val*的副本。|
|template <class InIter> void insert(iterator*i*,InIter *start*,InIter* end*);|直接在*i*所指元素前面插入*start*和*end*区间内的序列。|
|void merge(list<T,Allocator> &ob);template <class Comp>void merge ( list<T, Allocator> &*ob*, Comp*cmpfn*);|把ob中排好序的列表和调用的列表（已排序）合并在一起，结果也是排好序的，合并后，*ob*中的列表为空。在第二种形式中，可以指定比较两个元素大小的比较函数。|
|void pop_back();|移去列表中最后一个元素。|
|void pop_front();|移去列表中第一个元素。|
|void push_back(const T &*val*);|向列表末尾添加一个值为*val*的元素。|
|void push_front(const T &*val*);|向列表头添加一个值为*val*的元素。|
|void reverse();|反转调用的列表。|
|size_type size() const;|返回当前列表中元素的数量。|
|void sort();template < class Comp>void sort( Comp *cmpfn*);|对列表排序。第二种形式使用比较函数*cmpfn*来指定比较元素的大小。|
|void splice( iterator *i*, list<T,Allocator> &*ob*);|*ob*中的内容被插入到调用列表中去，插入位置由*i*指定。操作完成后，*ob*变为空。|
|void splice (iterator *i*,list<T,Allocator &*ob*, iterator *el*);|*el*所指的元素被从*ob*列表中移去，保存到调用的列表中去，保存位置由*i*指定。|
|void splice( iterator *i*, list< T,Allocator &*ob,*iterator *start*, iterator *end*);|*start*到*end*区间内的元素被从*ob*中移去，保存到调用列表中，开始保存的位置由*i*指定。|
|表24-3 列表list常用的一些成员函数| |
出于最大灵活性和可移植性考虑，任何储存在列表中对象都应该定义默认构造函数，也应该定义<操作符和其它可能的比较操作符。对于储存在列表中的对象的精确要求随不同的编译器而不同，所以你需要查阅编译器的文档说明。
这里是列表list的一个简单例子。
//简单列表
#include <iostream>
#include <list>
using namespace std;
int main()
{
       list<int> lst;//创建一个空列表
       int i;
       for(i=0; i<10; i++) lst.push_back(i);
       cout << "Size = "<< lst.size() << endl;
       cout << "Contents: ";
       list<int>::iterator p=lst.begin();
       while(p != lst.end()) {
              cout << *p << " ";
              p++;
       }
       cout << "/n/n";
       //改变列表的内容
       p = lst.begin();
       while(p != lst.end()) {
              *p = *p + 100;
              p++;
       }
       cout << "Contents modified: ";
       p = lst.begin();
       while(p != lst.end()) {
              cout << *p << " ";
              p++;
       }
       return 0;
}
程序产生的输出如下：
Size = 10
Contents: 0 1 2 3 4 5 6 7 8 9
Contents modified: 100 101 102 103 104 105 106 107 108 109
这个程序创建了一个整数列表。首先，一个空列表对象被创建，10个整数被加入列表中，这通过使用push_back()函数来实现，这个函数每次把一个新值添加到已经存在的列表的末尾。下一步，显示列表的大小和列表本身的内容。列表的内容通过迭代器——用下面的代码——显示出来。
       list<int>::iterator p=lst.begin();
       while(p != lst.end()) {
              cout << *p << " ";
              p++;
       }
这里，迭代器p被初始化指向列表的开头。每一次循环中，p递增，p指向下一个元素。当p指向列表的末尾时，循环结束。这部分代码与使用迭代器遍历向量本质上是一样的。在STL代码中，像这样的循环是很普遍的。这种用同样的结构可以访问不同类型的容器的正是STL的强大功能之一。
理解end()
现在是强调这个有点出人意料的容器函数end()的时候了。end()并不是返回容器最后一个元素的指针，它返回指向容器最后一个元素后面的指针。因为，容器中的最后一个元素是由end()-1所指的。这个特点可以让我们编写出非常高效的使用迭代器遍历容器所有元素的算法。当迭代器的具有的值与end()返回的值相同时，我们就知道所有的元素已经被访问过了。然而，由于它有点儿违反直觉，你必须紧记这个特性。比如，仔细考虑下面这个程序，它正、逆序显示了列表的内容。
//理解函数end();
#include <iostream>
#include <list>
using namespace std;
int main()
{
       list<int> lst;//创建一个空列表。
       int i;
       for(i=0; i<10; i++) lst.push_back(i);
       cout << "List printed forwards:/n";
       list<int>::iterator p = lst.begin();
       while(p != lst.end()) {
              cout << *p << " ";
              p++;
       }
       cout << "/n/n";
       cout << "List printed backwards:/n";
       p = lst.end();
       while(p != lst.begin()) {
              p--; //递减
              cout << *p << " ";
       }
       return 0;
}
程序处理结果如下：
List printed forwards:
0 1 2 3 4 5 6 7 8 9
List printed backwards:
9 8 7 6 5 4 3 2 1 0
正向显示列表的代码和我们已经用过的一样。但注意那些用以逆序显示列表内容的代码。迭代器p一开始用end()函数设为指向列表的末尾。由于end()返回的迭代器指向实际上被储存在最后一个对象后面的那个对象，p在使用之前必须先递减。这就是为什么在while循环中p是在cout语句之前递减，而不是在之后。记住：end()并不返回指向列表中最后一个对象的指针，而是返回指向列表中最后一个值之后的指针。
对比push_front()和push_back()
你可以通过向列表开头或末尾添加元素构造一个列表。到目前为止，我们已经用过push_back()向列表末尾添加元素。在列表的开头添加元素就要用push_front()。例如：
//演示push_back()和push_front()的不同
#include <iostream>
#include <list>
using namespace std;
int main()
{
       list<int> lst1,lst2;
       int i;
       for(i=0; i<10; i++) lst1.push_back(i);
       for(i=0; i<10; i++) lst2.push_front(i);
       list<int>::iterator p;
       cout << "Contents of lst1:/n";
       p = lst1.begin();
       while(p != lst1.end()) {
              cout << *p << " ";
              p++;
       }
       cout << "/n/n";
       cout << "Contents of lst2:/n";
       p = lst2.begin();
       while(p != lst2.end()) {
              cout << *p << " ";
              p++;
       }
       return 0;
}
这个程序的处理结果显示如下：
Contents of lst1:
0 1 2 3 4 5 6 7 8 9
Contents of lst2:
9 8 7 6 5 4 3 2 1 0
由于lst2是通过向开头添加元素构造的，结果使得列表的顺序与向末尾添加元素的列表lst1的顺序相反。
排序一个列表
一个列表可以通过调用成员函数sort()排序。下面的程序含有随机整数的列表，然后把它进行排序。
//排序一个列表
#include <iostream>
#include <list>
#include <cstdlib>
using namespace std;
int main()
{
       list<int> lst;
       int i;
       //创建一个含随机整数的列表
       for(i=0; i<10; i++)
              lst.push_back(rand());
       cout << "Original contents:/n";
       list<int>::iterator p=lst.begin();
       while(p != lst.end()) {
              cout << *p << " ";
              p++;
       }
       cout << endl << endl;
       //排序
       lst.sort();
       cout << "Sorted contents:/n";
       p = lst.begin();
       while(p != lst.end()) {
              cout << *p << " ";
              p++;
       }
       return 0;
}
程序处理后的输出如下：
Original contents:
41  18467  6334  26500  19169  15724  11478  29358  26962  24464
Sorted contents:
41  6334  11478  15724  18467  19169  24464  26500  26962  29358
列表与列表合并
一个有序列表可以与另一个合并，结果产生一个有序列表，内容包含两个源列表的内容。新列表在调用的列表中产生，而第二个列表变为空。下面的示例合并了两个列表。第一列表包含了0到9的偶数，第二包含了奇数，这两个列表合并后产生序列0  1  2  3  4  5  6  7  8  9
//合并两个列表
#include <iostream>
#include <list>
using namespace std;
int main()
{
       list<int> lst1, lst2;
       int i;
       //创建一个含随机整数的列表
       for(i=0; i<10; i+=2) lst1.push_back(i);
       for(i=1; i<11; i+=2) lst2.push_back(i);
       cout << "Contents of lst1:/n";
       list<int>::iterator p=lst1.begin();
       while(p != lst1.end()) {
              cout << *p << "  ";
              p++;
       }
       cout << endl << endl;
       cout << "Contents of lst2:/n";
       p=lst2.begin();
       while(p != lst2.end()) {
              cout << *p << "  ";
              p++;
       }
       cout << endl << endl;   
       //现在合并两个列表
       lst1.merge(lst2);
       if(lst2.empty())
              cout << "lst2 is now empty./n";
       cout << "Contents of lst1 after merge:/n";
       p = lst1.begin();
       while(p != lst1.end()) {
              cout << *p << "  ";
              p++;
       }
       return 0;
}
程序产生的输出如下：
Contents of lst1:
0  2  4  6  8
Contents of lst2:
1  3  5  7  9
lst2 is now empty.
Contents of lst1 after merge:
0  1  2  3  4  5  6  7  8  9
举例中另外要注意的一个地方是函数empty()的使用。如果调用为空，它返回true。由于merge()移去了被合并列表的所有元素，合并完成后它变成了空列表，程序的输出验证了这一点。
排序列表中的类对象
这里是一个使用列表储存myclass类对象的例子。注意运算符<、>、!=和==已经被myclass类对象重载（对于有些编译器，你可能不必全部定义这些运算符，但对于另外一些编译器，你可能还要再定义另外一些运算符）。STL使用这些函数确定容器中对象的相等性和顺序。即使一个列表并不是一个有序容器，但在搜索、排序和合并时，它仍然需要一个比较元素的方法。
//在列表中储存类对象
#include <iostream>
#include <list>
#include <string>
using namespace std;
class myclass {
       int a, b;
       int sum;
public:
       myclass() { a = b = 0; }
       myclass(int i, int j) {
              a = i;
              b = j;
              sum = a + b;
       }
       int getsum() { return sum; }
       friend bool operator<(const myclass &o1,
                                            const myclass &o2);
       friend bool operator>(const myclass &o1,
                                            const myclass &o2);
       friend bool operator==(const myclass &o1,
                                            const myclass &o2);
       friend bool operator!=(const myclass &o1,
                                            const myclass &o2);
};
bool operator<(const myclass &o1, const myclass &o2)
{
       return o1.sum < o2.sum;
}
bool operator>(const myclass &o1, const myclass &o2)
{
       return o1.sum > o2.sum;
}
bool operator==(const myclass &o1, const myclass &o2)
{
       return o1.sum == o2.sum;
}
bool operator!=(const myclass &o1, const myclass &o2)
{
       return o1.sum != o2.sum;
}
int main()
{
       int i;
       //创建第一个列表
       list<myclass> lst1;
       for(i=0; i<10; i++) lst1.push_back(myclass(i,i));
       cout<< "First list: ";
       list<myclass>::iterator p=lst1.begin();
       while(p != lst1.end()) {
              cout << p->getsum() << " ";
              p++;
       }
       cout << endl;
       //创建另一个列表
       list<myclass> lst2;
       for(i=0; i<10; i++) lst2.push_back(myclass(i*2, i*3));
       cout << "Second list: ";
       p = lst2.begin();
       while(p != lst2.end()) {
              cout << p->getsum() << " ";
              p++;
       }
       cout << endl;
       //现在合并lst1和lst2
       lst1.merge(lst2);
       //显示合并后的列表。
       cout << "Merged list: ";
       p=lst1.begin();
       while(p != lst1.end()) {
              cout << p->getsum() << " ";
              p++;
       }
       return 0;
}
这个程序创建了两个myclass类对象的列表并显示每一个列表的内容，然后合并两个列表并显示结果。程序的输出如下：
First list: 0 2 4 6 8 10 12 14 16 18
Second list: 0 5 10 15 20 25 30 35 40 45
Merged list: 0 0 2 4 5 6 8 10 10 12 14 15 16 18 20 25 30 35 40 45
映射Maps
映射类map支持联合容器，在这种容器中，单一键与值联系在一起。从本质上看，键仅是你给值所起的名字。一旦值被储存起来，你可以通过它的键名取回。因此，从更一般意义上说，映射是一个“键－值”对列表。映射map的强大之处是你可以用值的键查找值。比如，你可以定义一个映射，在这个映射中，你把人名作为键，而把电话号码作为值。联合容器在编程中已经变得越来越受欢迎。
如前所述，映射map仅能储存单一键，它不允许两个相同的同时存在。如果要创建一个允许非单一键的映射，就要使用多重映射multimap。
映射map具有如下的模板格式：
template <class Key, class T, class Comp = less<Key>,
class Allocator = allocator<pair<const key, T> > class map
在这里，Key是键的数据类型，T是储存（被映射）的值的数据类型，而Comp是比较两个键的函数，默认使用标准的less()函数对象。Allocator是分配器（默认是allocator）。
映射map具有如下的构造函数：
explicit map(const Comp &*cmpfn* = Comp(), const Allocator &*a* = Allocator() );
map(const map<Key, T, Comp, Allocator> &*ob*);
template<class InIter> map(InIter *start*, InIter *end*,
 const Comp &*cmpfn* = Comp(), const Allocator &*a* = Allocator() );
第一种形式构建一个空映射。第二种形式构建一个与*ob*具有同样元素的映射。第三种形式构建一个包含了*start*和*end*区间内相同元素的映射。如果提供了cmpfn函数，则映射的顺序由它决定。
一般来说，任何充当键的对象都必须定义默认构造函数，同时重载操作符<和其它一些必要的比较运算符，具体要求因不同的编译器而异。
下面的比较运算符已经在映射map中定义。
==，<，<=，!=，>，>=
一些映射map的成员函数已经列在表24-4中。描述栏的key_type是键的类型，而value_type代表pair<Key,
 T>。
|成员函数|描述|
|----|----|
|interator begin();const_iterator begin() const;|返回指向映射中第一个元素的迭代器。|
|void clear();|从映射中移去所有元素。|
|size_type count(const key_type &*k*) const;|返回*k*在映射出现的次数（0或1）|
|bool empty() const;|当调用的映射为空时返回true，否则返回false。|
|iterator end();const_iterator end() const;|返回指向列表末尾的迭代器。|
|void erase(iterator *i*);|移去由*i*所指的元素。|
|void erase(iterator *start*,iterator *end*);|移去*start*到*end*区间中的元素。|
|size_type erase(const key_type &*k*) const;|从映射中移去值为*k*的所有元素。|
|iterator find(const key_type &k);const_iterator find(const key_type &k) const;|返回指向指定键的迭代器。如果键没有找到，则返回指向映射末尾的迭代器。|
|iterator insert(iterator *i*,const value_type &*val*);|向i所指位置或者后面插入元素。返回指向插入元素的迭代器。|
|template <class InIter>void insert(InIter start, InIter end);|插入区间内的元素。|
|pair<iterator, bool>insert(const value_type &val);|把val插入调用映射。返回指向插入元素的迭代器。元素在映射中不存在才被插入。当元素被插入时，返回pair<iterator,true>。否则返回pair<iterator,false>。|
|mapped_type & operator[](const key_type &i);|返回被i所指的元素的引用，如果元素不存在，则被插入。|
|size_type size() const;|返回当前表中元素的数量。|
|表24-4 映射map几个常用的成员函数| |
“键－值”对在映射中以pair类型的对象储存，pair类型具有这样的模板形式。
template <class Ktype, class Vtype> struct pair {
typedef Ktype first_type;//键的类型
typedef Vtype second_type;//值的类型
Ktype first;//所包含的键
Vtype second;//所包含的值
//构造函数
pair();
pair(const Ktype &k, const Vtype &v);
template<class A, class B> pair(const<A, B> &ob);
}
如注释所指出，first中的值包含了键，而second包含了与键相关联的值。
使用pair的构造函数或使用make_pair()都可以创建（键－值）对。make_pair()函数是基于做为参数的数据的类型来创建（键－值）对对象的。make_pair()是一个通用函数，具有如下的原型：
template <class *Ktype*, class *Vtype*>
pair<*Ktype*, *Vtype*> make_pair(const *Ktype *&*k*, const *Vtype *&*v*);
可以看出，这个函数返回一个含有由*Ktype*和*Vtype*指定类型的值的对对象。make_pair()的优点是储存的对象的类型是由编译器自动确定的，而不是由你显式指定的。
下面的程序演示了映射的基本用法。它储存了“键－值”对，然后再显示这个储存大写字母和它们的ASCII码的映射。因此，键是一个字符，而值是一个整数，“键－值”对保存类似于
A            65
B            66
C            67
……一旦“键－值”对被储存起来，你只要提供一个键（例如A到Z之间的字符），那么这个字符的ASCII码就被显示出来。
//简单的映射演示
#include <iostream>
#include <map>
using namespace std;
int main()
{
       map<char, int> m;
       int i;
       //把对放入映射中
       for(i=0; i<26; i++) {
              m.insert(pair<char, int>('A'+i,65+i));
       }
       char ch;
       cout << "Enter key: ";
       cin >> ch;
       map<char, int>::iterator p;
       //查找给定键的值
       p = m.find(ch);
       if(p != m.end())
              cout << "Its ASCII value is "<< p->second;
       else
              cout << "Key not in map./n";
       return 0;
}
注意创建“键－值”对的模板类pair的使用。pair指定的数据类型必须与映射中被插入的对相匹配。
一旦映射已经用键和值初始化，你就可以使用find()函数用键名查找值。如果找到匹配的元素，则返回指向这个元素的迭代器，如果没有找到，则返回指向映射末尾的迭代器。当匹配项找到时，键的值被放到pair的成员second中去。
在前述的例子中，“键－值”对使用pair<char, int>被显式构造，虽然用这种方法都没有什么错误，但使用make_pair()函数通常会更容易一点（这个函数基于参数的数据类型来创建对对象）。比如，在前面这个程序中，下面这一行代码也同样向m插入“键－值”对。
m.insert(make_pair((char)( +i),65+i));
这里，为了覆盖i加到'A'上到结果自动被转换成int，char不用省，否则，类型将被自动确定。
在映射中储存类对象Storing Class Object in a Map
与所有的容器一样，你可以用映射储存自定义类的对象。比如，下面的程序创建一个简单的电话簿。也就是说，创建一个姓名到号码映射。可以这样做：创建两个类——name类和number类。由于映射使用键来维护有序表，程序也为name类定义了<操作符。一般来说，你必须为任何一个拿来充当键的类定义<操作符（一些编译器可能要求额外的比较操作符）。
//使用映射创建电话簿
#include <iostream>
#include <map>
#include <cstring>
using namespace std;
class name {
       char str[40];
public:
       name() { strcpy(str,""); }
       name(char *s) { strcpy(str, s); }
       char *get() { return str; }
};
//必须定义少于操作符
bool operator<(name a, name b)
{
       return strcmp(a.get(), b.get()) < 0;
}
class phoneNum {
       char str[80];
public:
       phoneNum() { strcmp(str, ""); }
       phoneNum(char *s) { strcpy(str, s); }
       char *get() { return str; }
};
int main()
{
       map<name, phoneNum> directory;
       //把姓名和号码入到映射
       directory.insert(pair<name, phoneNum>(name("Tom"),
                                   phoneNum("555-4533")));
       directory.insert(pair<name,phoneNum>(name("chris"),
                                   phoneNum("555-9678")));
       directory.insert(pair<name,phoneNum>(name("John"),
                                   phoneNum("555-8195")));
       directory.insert(pair<name,phoneNum>(name("Rachel"),
                                   phoneNum("555-0809")));
       //输入姓名，查找号码
       char str[80];
       cout << "Enter name: ";
       cin >> str;
       map<name, phoneNum>::iterator p;
       p = directory.find(name(str));
       if(p != directory.end())
              cout << "Phone number: " << p->second.get();
       else
              cout << "Name not in directory./n";
       return 0;
}
下面是一个运行例子：
Enter name: Rachel
Phone number: 555-0809
在这个程序中，每个映射的入口是一个0结尾的字符数组。在本章后面，你会见到使用标准字符串string类编写这个程序将会更简单。
算法Algorithms
如前所述，算法在容器上起作用。虽然每个容器为自己的基本操作提供支持，但标准算法提供扩展或更复杂的动作。它们还允许你同时让两个不同类型的容器一起工作。要使用STL算法，你必须在程序中包含<algorithm>。
STL定义了相当大量的算法，表24-5是摘要。所有的算法都是模板函数。这意味着它们可以应用于任何类型的容器。所有的算法将在第四部分讲述。下面这些章节展示了一个典型例子。
|算法|目的|
|----|----|
|adjacent_find|在序列中寻找相邻的匹配元素，返回指向第一个匹配的迭代器。|
|binary_search|在有序序列上执行二分搜索。|
|copy|复制一个序列|
|copy_backward|把序列末尾的元素移到开头，其它与copy()相同。|
|count|返回序列中元素的个数。|
|count_if|返回序列中符合某个断言的元素的个数。|
|equal|确定两个区间是否一样。|
|equal_range|返回序列的区间，在这个区间中，元素能被插入但不会打乱其顺序。|
|fill和fill_n|用指定的值填充区间。|
|find|在区间中查找值并返回指向第一个遇到的元素的迭代器。|
|find_end|在区间中查找一个子序列，返回指向区间中子序列末尾的迭代器。|
|find_first_of|在序列中查找与区间中的元素相匹配的第一个元素。|
|find_if|在区间中查找用户定义的一元断言返回true的元素。|
|for_each|把函数应用到区间内的元素中。|
|generate and generate_n|把生成器函数（generator function）返回的值赋给区间内的元素。|
|includes|确定某个序列是否包含了另一个序列中的所有元素。|
|inplace_merge|合并两个区间，两个区间都必须递增的。生成的序列也是有序的。|
|iter_swap|交换它的两个迭代器参数所指的值。|
|lexicographical_compare|按字母顺序比较两个序列。|
|lower_bound|在序列中查找第一个不比给定的值小的点。|
|make_heap|在序列中创建堆。|
|max|返回两个值中较大的值。|
|max_element|返回指向区间中最大元素的迭代器。|
|merge|合并两个有序序列，结果保存在第三个序列中。|
|mismatch|查找两个序列中第一个不匹配的元素。指向这两个不同元素的迭代器（复数）被返回。|
|next_permutation|构造序列的下一个排列。|
|nth_element|排序一个序列，使所有比给定的元素*E*小的元素排在它的前面，而所有比它大的元素排序它的后面。|
|partial_sort|对区间进行排序。|
|partial_sort_copy|对区间进行排序，然后复制尽可能多的元素去填满目标序列。|
|partition|对序列进行排序，断言返回true的元素排在前面，返回false的元素排在后面。|
|pop_heap|交换第一个和倒数第二个元素后再重堆。|
|prev_permutation|构造序列的上一个排列。|
|push_heap|把元素压到堆尾。|
|random_shuffle|打乱序列。|
|remove, remove_if, remove_copy, and remove_copy_if|从指定的区间中移去元素。|
|replace, replace_copy, replace_if, and repalce_copy_if|代换区间内的元素。|
|reverse and reverse_copy|反转区间的顺序。|
|rotate and rotate_copy|左旋区间的元素。|
|search|在序列中查找子序列。|
|search_n|查找相同元素的数量与指定的数量相同的序列|
|set_difference|产生一个包含两个有序集差集的序列。|
|set_intersection|产生一个包含两个有序集交集的序列。|
|set_union|产生一个包含两个有序集并集的序列。|
|sort|对区间排序。|
|sort_heap|排序指定区间内的堆|
|stable_partition|排列一个序列，使得所有断言返回true的元素排在断言返回false的前面。这是一种稳定分割，即序列中元素相互间的顺序被保留。|
|stable_sort|对区间排序，这种稳定排序，即相同的元素没有被重排。|
|swap|交换两个值。|
|swap_ranges|交换区间内的两个元素。|
|transform|把函数应用到一个元素区间中，输出被保存到一个新序列中。|
|unique and unique_copy|消除区间内的重复元素。|
|upper_bound|在序列查找不比某个值大的最后一个位置。|
|表24-5 STL算法| |
计数Counting
你能在序列上执行的最基本的一个操作是对其内容进行计数。要这样做，你可以使用count()或count_if()。它们的一般形式如下：
template<class InIter, class T>
              ptrdiff_t count(InIter *start*, InIter *end*, count T &*val*);
template<class InIter, class UnPred>
              ptrdiff_t count_if(InIter *start*,InIter *end*, UnPred *pfn*);
类型ptrdiff_t被定义为整数的某种形式。
count()算法返回序列中*start*到*end*区间内值为*val*的元素的个数。count_if()算法返回序列中*start*到*end*区间内一元断言*pfn*返回真的元素的个数。
下面的程序演示了count()函数。
//count()显示
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
using namespace std;
int main()
{
       vector<bool> v;
       unsigned int i;
       for(i=0; i < 10; i++) {
              if(rand() % 2) v.push_back(true);
              else v.push_back(false);
       }
       cout << "Sequence:/n";
       for(i=0; i<v.size(); i++)
              cout << boolalpha << v[i] << " ";
       cout << endl;
       i=count(v.begin(), v.end(), true);
       cout << i << " elements are true./n";
       return 0;
}
程序显示了下面的输出：
Sequence:
true true false false true false false false false false
3 elements are true.
程序开始创建一个储存随机产生的true和false值的向量，接着用count()输出true的个数。
下面的程序演示了count_if()函数。它创建一个包含1到19的向量，然后计算出能被3整除的数的个数。在达到目的，必须自定义一元断言dividesBy3()，当数可以被3整除时它返回true。
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
/*确定数是否可以被3整除的一元断言*/
bool dividesBy3(int i)
{
       if((i%3) == 0) return true;
       return false;
}
int main()
{
       vector<int> v;
       int i;
       for(i=1; i < 20; i++) v.push_back(i);
       cout << "Sequence:/n";
       for(i=0; i<v.size(); i++)
              cout << v[i] << " ";
       cout << endl;
       i = count_if(v.begin(), v.end(), dividesBy3);
       cout << i << " numbers are divisible by 3./n";
       return 0;
}
程序处理结果如下：
Sequence:
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
6 numbers are divisible by 3.
注意一元断言dividesBy3()是如何编写的。所有一元断言接受一个对象作为参数，这个对象与容器中储存的对象类型相同。断言必须基于这个对象返回true或false。
移除或替换元素Removing and Replacing Elements
有时候创建一个只包含原始序列中某些特定项的序列是有用的。remove_copy()就是这样做的一个算法。它的一般形式如下：
template<class InIter, class OutIter, class T>
OutIter remove_copy(InIter *start*, InIter *end*, OutIter *result*, const T &*val*);
算法remove_copy()从指定的区间复制元素，但移去那些值与*val*相同的元素，把结果放到*result*所指的序列，返回的迭代器指向结果末尾。输出容器必须足够容下结果。
在复制时要替换一个元素，可以使用replace_copy()。它的一般形式为：
template<class InIter, class OutIter, class T>
       OutIter replace_copy(InIter *start*, InIter *end*, OutIter *result*, const T &*old*, const T & *new*);
replace_copy算法从指定的区间复制元素，用*new*替换等于*old*的元素。把结果放进*result*所指的地方，返回指向结果末尾的迭代器。输出容器必须足够容下结果。
下面的程序演示了remove_copy()和replace_copy()。它创建一个字符序列，然后移去所有的空格，接着用冒号替换空格。
//演示remove_copy和replace_copy
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
       char str[] = "The STL is power programming.";
       vector<char> v, v2(30);
       unsigned int i;
       for(i=0; str[i]; i++) v.push_back(str[i]);
       //*****演示 remove_copy *****
       cout << "Input sequence:/n";
       for(i=0; i<v.size(); i++) cout << v[i];
       cout << endl;
       //移去所有空格
       remove_copy(v.begin(), v.end(), v2.begin(), ' ');
       cout << "Result after removing spaces:/n";
       for(i=0; i<v2.size(); i++) cout << v2[i];
       cout << endl << endl;
       //****现在演示replace_copy ****
       cout << "Input sequence:/n";
       for(i=0; i<v.size(); i++) cout << v[i];
       cout << endl;
       //用冒号替换空格
       replace_copy(v.begin(), v.end(), v2.begin(), ' ', ':');
       cout << "Result after replacing spaces with colons:/n";
       for(i=0; i<v2.size(); i++) cout << v2[i];
       cout << endl << endl;
       return 0;
}
程序输出如下：
Input sequence:
The STL is power programming.
Result after removing spaces:
TheSTLispowerprogramming.
Input sequence:
The STL is power programming.
Result after replacing spaces with colons:
The:STL:is:power:programming.
反转序列Reversing a Sequence
一个常用的算法是reverse()，它反转一个序列。一般形式为：
template<class BiIter> void reverse(BiIter *start*, BiIter *end*);
reverse()算法反转start和end区间内的顺序。
以下的程序演示了reverse()。
//演示reverse()
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main()
{
       vector<int> v;
       unsigned int i;
       for(i=0; i<10; i++) v.push_back(i);
       cout << "Initial:/n";
       for(i=0; i<v.size(); i++) cout << v[i] << " ";
       cout << endl;
       reverse(v.begin(), v.end());
       cout << "Reversed: ";    
       for(i=0; i<v.size(); i++) cout << v[i] << " ";
       return 0;
}
程序输出如下：
Initial:0 1 2 3 4 5 6 7 8 9
Reversed: 9 8 7 6 5 4 3 2 1 0
转换序列Transforming a Sequence
有一个比较有趣算法是transform()，它根据你提供的函数修改区间中的元素。transform()算法有下面两种形式：
template <class InIter, class OutIter, class Func)
       OutIter transform(InIter *start*, InIter *end*, OutIter *result*, Func *unaryfunc*);
template <class InIter1, class InIter2, class OutIter, class Func)
       OutIter transform(Inter1 *start1*, InIter1 *end1*, InIter2 *start2*,
OutIter *result*, Func *binaryfunc*);
transform()算法把函数应用到某一区间内的元素后把输出结果储存到*result*中。在第一形式中，区间由*start*和*end*指定，被应用的函数由*unaryfunc*指定，这个函数读取参数元素的值然后返回转换后的值。在第二种形式中，二元函数被应用到转换中，这个函数的第一个参数为待转换序列的元素值，第二个参数则从第二序列中取得。两个版本都返回指向结果序列的末尾。
下面程序使用简单转换函数reciprocal()把数字列表的内容转换成它们的倒数。注意结果序列被保存在原始序列的列表中。
//一个转换算法的例子。
#include <iostream>
#include <list>
#include <algorithm>
using namespace std;
//简单的转换函数
double reciprocal(double i) {
       return 1.0/i;//返回倒数
}
int main()
{
       list<double> vals;
       int i;
       //保存值到列表中
       for(i=1; i<10; i++) vals.push_back((double) i);
       cout << "Original contents of vals:/n";
       list<double>::iterator p=vals.begin();
       while(p != vals.end()) {
              cout << *p << " ";
              p++;
       }
       cout << endl;
       //转换值
       p = transform(vals.begin(), vals.end(),
              vals.begin(),reciprocal);
       cout << "Transformed contents of vals:/n";
       p = vals.begin();
       while(p != vals.end()) {
              cout << *p << " ";
              p++;
       }
       return 0;
}
程序的输出结果如下：
Original contents of vals:
1 2 3 4 5 6 7 8 9
Transformed contents of vals:
1 0.5 0.333333 0.25 0.2 0.166667 0.142857 0.125 0.111111
可以看出，vals中的每个元素转换成它的倒数。
使用函数对象Using Function Objects
正如这本章开头所解释的，STL支持（也广泛利用）函数对象。回想一下：函数对象仅仅是定义了operator()的类。STL提供了很多内置的函数对象，例如less、minus等等。它也允许你定义自己的函数对象。坦白说，完全讨论创建和使用函数对象已经超出本书的范围。幸运的是，正如前面的举例所示，你不用创建函数对象都可以广泛使用STL。然而，既然函数对象是STL的主要因素，对函数对象有一个大概了解还是很重要的。
**一元及二元函数对象Unary and Binary Function Objects**
**断言有一元和二元之分，函数对象也有一元和二元。一元函数对象需要一个参数，而二元函数对象则需要二个。你必须使用需要的对象类型。例如，如果某个算法要求一个二元函数对象，则你必须传递一个二元函数对象给它。**
使用内置函数对象Using the Built-in Function Objects
STL提供了内置了类别丰富的函数对象。下面列出了二元函数对象：
|plus|minus|multiplies|divides|modulus|
|----|----|----|----|----|
|equal_to|not_equal_to|greater|greater_equal|less|
|less_equal|logical_and|logical_or|||
这是一元函数对象
|logical_not|negate|
函数对象执行的操作由函数名指定。唯一一个可能不是一看就明白的是negate()，它反转参数的正负符号。
**内置函数对象是重载了operator()的模板类，不管你选择的数据类型是什么，它们都返回指定操作的结果。**比如，要为float类数据调用二元函数对象plus()，使用的语法为：
plus<float> ()
内置函数对象使用头文件<functional>
我们开始看一个简单例子。下面的程序使用了transform()算法（已经在前一节有讨论）和negate()函数对象转换列表中值的符号。
//使用一元函数对象
#include <iostream>
#include <list>
#include <functional>
#include <algorithm>
using namespace std;
int main()
{
       list<double> vals;
       int i;
       //把值放入列表
       for(i=1; i<10; i++) vals.push_back((double)i);
       cout << "Original contents of vals: /n";
       list<double>::iterator p=vals.begin();
       while(p != vals.end()) {
              cout << *p << " ";
              p++;
       }
       cout << endl;
       //使用negate函数对象
       p = transform(vals.begin(), vals.end(),
              vals.begin(),
              negate<double>());//调用函数对象
       cout << "Negated contents of vals:/n";
       p = vals.begin();
       while(p != vals.end()) {
              cout << *p << " ";
              p++;
       }
       return 0;
}
程序产生了下面的输出：
Original contents of vals:
1 2 3 4 5 6 7 8 9
Negated contents of vals:
-1 -2 -3 -4 -5 -6 -7 -8 -9
注意在这个程序中negate()中如何调用的。由于vals是一个值为double类型的列表，negate()是通过negate<double>()来调用的。transform()算法自动为序列中的每一个元素调用negate()。这样，单参数的negate()从序列中取元素做为它的参数。
下面的程序演示了二元函数对象divides()的使用。它创建了两个值为double类型的列表，然后再一个除以另一个。这个程序使用了二元形式的transform()算法。
//使用二元函数对象
#include <iostream>
#include <list>
#include <functional>
#include <algorithm>
using namespace std;
int main()
{
       list<double> vals;
       list<double> divisors;
       int i;
       //把值放入列表
       for(i=10; i<100; i+=10) vals.push_back((double)i);
       for(i=1; i<10; i++) divisors.push_back(3);
       cout << "Original contents of vals: /n";
       list<double>::iterator p=vals.begin();
       while(p != vals.end()) {
              cout << *p << " ";
              p++;
       }
       cout << endl;
       //转换vals
       p = transform(vals.begin(), vals.end(),
                            divisors.begin(), vals.begin(),
                            divides<double>());//调用函数对象
       cout << "Divided contents of vals:/n";
       p = vals.begin();
       while(p != vals.end()) {
              cout << *p << " ";
              p++;
       }
       return 0;
}
程序的输出如下：
Original contents of vals:
10 20 30 40 50 60 70 80 90
Divided contents of vals:
3.33333 6.66667 10 13.3333 16.6667 20 23.3333 26.6667 30
在这个例子中，二元函数对象divides()用第二个序列的元素除第一个序列中相对应的元素。因此，divides()接受的参考顺序为：
divides(*first*, *second*)
这种可以进行推广：当使用二元函数对象时，它的参数顺序是*first*，*second*。
创建函数对象Creating a Function Object
除了使用内置对象之外，你也可以创建自己的函数对象。要这样做，你仅需重载一个类的operator()函数。然而，出于最大灵活性考虑，你要使用下面由STL定义的一个类作为你的函数对象的基类。
template <class Argument, class Result> struct unary_function {
typedef Argument argument_type;
typedef Result result_type;
};
template <class Argument1, class Argument2, class Result>
struct binary_function {
typedef Argument1 first_argument_type;
typedef Argument2 second_argument_type;
typedef Result result_type;
};
这些模板类为函数对象使用的通用数据类型提供了具体的类型名。虽然从技术上来说它们很方便，但它们几乎都是在创建函数对象时使用。
下面的程序演示了一个用户定义的函数对象。它把reciprocal()（前面在演示transform()算法时已经使用过）转成函数对象。
//创建一个倒数函数对象
#include <iostream>
#include <list>
#include <functional>
#include <algorithm>
using namespace std;
//一个简单的函数对象。
class reciprocal: unary_function<double,double> {
public:
       result_type operator() (argument_type i)
       {
              return (result_type) 1.0/i;//返回倒数
       }
};
int main()
{
       list<double> vals;
       int i;
       //把值放进列表中
       for(i=1; i<10; i++) vals.push_back((double)i);
       cout << "Original contents of vals:/n";
       list<double>::iterator p=vals.begin();
       while(p !=vals.end()) {
              cout << *p << " ";
              p++;
       }
       cout << endl;
       //使用倒数函数对象
       p = transform(vals.begin(), vals.end(),
              vals.begin(),
              reciprocal());//调用函数对象
       cout << "Transformed contents of vals: /n";
       p = vals.begin();
       while(p != vals.end()) {
              cout << *p << " ";
              p++;
       }
       return 0;
}
注意reciprocal()两个重要方面：第一，它从基类unary_function继承，这使它可以访问argument_type和result_type这两种类型。第二，它定义了operator()用来返回参数的倒数。**一般来说，要创建一个函数对象，只要求简单地从恰当的基类继承和重载operator()。真的就这么简单。**
使用绑定器Using Binders
当使用二元函数对象时，有时需要把一个值绑定到其中一个参数。这在很多情况下很有用。比如，你可能想从序列中移去所有比某个值（例如8）大的元素。要这样做，你需要使用某个方法把8绑定到函数对象greater()的右操作数中，也就是说，你要greater()去实现比较序列中的每个元素val
 > 8。STL提供了一个机制达到这个目的，这个机制叫绑定器binders。
一共有两个绑定器：bind2nd()和bind1st()。它们的一般形式如下：
bind1st(*binfunc_obj*, *value*)
bind2nd(*bindfunc_obj*, *value*)
在这里，*binfunc_obj*是一个二元函数对象。bind1st()返回一个一元函数对象，在这个函数对象中，*binfunc_obj*的左操作数绑定到*value*中。bind2nd()返回同样返回一元函数对象，但这个对象中，*binfunc_obj*的右操作数绑定到*value*中。bind2nd()绑定器是最常用的。不管哪个情况，绑定器的输出是一个已经绑定到指定值的一元函数对象。
我们使用remove_if()算法来演示绑定器的使用，这个算法根据断言的输出结果从序列中移去元素，它的原型为：
template <class ForIter, class UnPred>
ForIter remove_if(ForIter *start*, ForIter *end*, UnPred *func*);
如果*func*定义的一元断言返回true，算法将从序列的*start*到*end*区间中移去元素。
算法返回指向序列的新末尾的迭代器，新末尾反映了元素的删除。
下面的程序从序列中移去比8大的元素。由于remove_if()所需要的断言是一元的，我们不能简单地使用greater()函数对象，因为greater()是一个二元对象。我们必须使用绑定器bind2nd()把8绑定到greater()的第二个参数中，情况如程序所示：
//演示bind2nd()
#include <iostream>
#include <list>
#include <functional>
#include <algorithm>
using namespace std;
int main()
{
       list<int> lst;
       list<int>::iterator p, endp;
       int i;
       for(i=1; i < 20; i++) lst.push_back(i);
       cout << "Original sequence:/n";
       p = lst.begin();
       while(p != lst.end()) {
              cout << *p << " ";
              p++;
       }
       cout << endl;
       endp=remove_if(lst.begin(), lst.end(), bind2nd(greater<int>(),8));
       cout << "Resulting sequence:/n";
       p = lst.begin();
       while(p != endp) {
              cout << *p << " ";
              p++;
       }
       return 0;
}
程序输出如下：
Original sequence:
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
Resulting sequence:
1 2 3 4 5 6 7 8
如果你要试验这个程序，尝试一下不同的函数对象，绑定不同的值。你会发现，绑定器非常高效地扩展了STL的功能。
最后，有一个与绑定相联系的对象叫取反器*negator*。它们是not1()和not2()。它们返回断言修改后的非（例如补集）（原句为“They
 return the negation (i.e.,the complement of) whatever predicate they modify.”）。它们的一般形式为：
not1(*unary_predicate)*
not2(*binary_predicate*)
例如，如果你把前面的程序代换为
endp = remove_if(lst.begin(), lst.end(),
                            not1(bind2nd(greater<int>(), 8)));
它将从lst移去所有不比8大的元素。
字符串类The string Class
你知道，C++本身并不支持内置的字符串，但是，它提供了两个方式来处理字符串。第一，使用传统的，以0结尾的字符数组，有时这种方式称作C字符串，你已经很熟悉了。第二，使用一个string类型的类对象。这种方式就是我们在这里要学习的。
实际上，string类只是一个非常普通的模板basic_string的一个具体化。事实上有两个basic_string的具体化:string支持8位字符串，wstring支持宽字符串。由于8位字符在一般的编程中是最常用的。在这里我们学习basic_string的string版本。
在看string类之前，明白为什么string成为C++函数库是很重要的。标准类从不被随意加到C++中。事实上，相当数量的重要的思想和讨论一直陪伴着每个新版本。假设C++已经包含了对0结尾的字符数组串的支持，那么一开始可能觉得包含string类是这个惯例的例外。然而，就根本就不是事实。原因是：零结尾的字符串不能被任何标准的C++运算符处理。也不能充当一般的C++表达式。例如，考虑一下这个片段：
char s1[80], s2[80], s3[80];
s1 = "Alpha"; //不能成功
s2 = "Beta"; //不能成功
s3 = s1 + s2; //错误，不允许
如注释所示，C++不允许使用赋值符号给字符数组赋新值（初始化时除外），也不允许使用＋号连接两个字符串。这些操作必须使用函数库。如下所示：
strcpy(s1, "Alpha");
strcpy(s2, "Beta");
strcpy(s3, s1);
strcat(s3, s2);
由于以0结尾的字符数组（字符串类型）有名无实（Since null-terminated character arrays are not technically data types in their own right）。C++操作符不能应用到它们。这使得即使是最基本的字符串操作符都变得笨拙起来。除此之外，使用标准操作符（已经使得标准string类发展）是不能操作0结尾字符串（More
 than anything else, it is the inability to operate on null-terminated strings using the standard C++ operators that has drives the development of a standard string class.）。记住，当你在C++中定义一个类时，你是定义了一种可以与C++环境完美集成新的数据类型。当然，这意味着要重载与新类相联系的操作符。因此，通过添加标准字符串类，跟其它的数据类型一样使用标准运算符来管理字符串就变得可能了。
然而，对于标准字符串类来说，还有一个原因：安全性。在无经验或粗心的程序员手里，很容易跑到以0结尾的字符数组后面去。例如，考虑一下标准字符复制函数strcpy()。这个函数没有检查目标数组的边界。如果原数组包含的字符比目标数组能容纳的字符还要多，那么可能导致程序错误和系统崩溃（这是很有可能的）。你会看到，标准字符串类string可以防止这些错误发生。
最后，标准字符串类被包含一共有三个原因：一致性（一个字符串定义一种数据类型），便利性（可以使用标准C++运算符）和安全性（不会出现数组越界）。记住你没有理由遗弃普通、以0结尾的字符串，它们仍然是实现字符串的最高效方法。但是，当速度不是一个关键因素时，使用新的字符串类string能够给你提供一个安全的、完整集成的管理字符串的方法。
虽然string不是传统STL考虑的一部分，string是C++定义的另一个容器类。也就是说，它支持前面章节描述的算法。然而，string有附加的能力。要使用string类，你必须在程序中包含<string>。
string类非常庞大，包含了很多构造函数和成员函数，此外，很多成员函数还有多种重载形式。正因为这样，我们将检查几个最常用的特性。一旦你对string的工作原理有了大概了解，你自己将会很容易的挖掘string其它部分。
string类支持几个构造函数，最常用的三个构造函数原型是：
string();
string(const char * str);
string(const string & str);
第一种形式创建一个空字符串对象。第二种形式从str所指的0结尾字符串创建字符串对象。第三种形式提供了从0结尾的字符串到字符串对象的转换。
大量运算符已经为string对象定义了，这些包含：
|运算符|意义|
|----|----|
|=|赋值|
|+|连结|
|+=|连结赋值|
|==|相等|
|!=|不相等|
|<|小于|
|<=|小于等于|
|>|大于|
|>=|大于等于|
|[]|下标|
|<<|输出|
|>>|输入|
这些运算符允许string对象使用普通的表达式，而不用调用strcpy()或者strcat()等函数。一般来说，你可以把普通的、0结尾的字符串和string对象混合使用。
“＋”运算符可以用来连结字符串对象与字符串对象、对符串对象和C风格的字符串对象。也就是说，下面几种不同形式是受支持的：
string + string
string + C-string
C-string + string
“＋”运算符也允许把一个字符添加到字符串的末尾。
string类定义了常量npos，值为-1.这个常量表示字符串可能的最长长度。
C++字符串类使得操作字符串极其容易。比如，你可以使用赋值运算符把引号的字符串赋给一个string对象，使用“+”运算符连结字符串，使用比较运算符比较字符串。下面的程序演示了这些操作：
//简短的字符串演示
#include <iostream>
#include <string>
using namespace std;
int main()
{
       string str1("Alpha");
       string str2("Beta");
       string str3("Omega");
       string str4;
       //字符串赋值
       str4 = str1;
       cout << str1 << "/n" << str3 << "/n";
       //连结两个字符串
       str4 = str1 + str2;
       cout << str4 << "/n";
       //连结一个C风格的字符串
       str4 = str1 + " to " + str3;
       cout << str4 << "/n";
       //比较字符串
       if(str3 > str1) cout << "str3 > str1 /n";
       if(str3 == str1 + str2)
              cout << "str3 == str1 + str2/n";
       //字符串对象也可以赋给一个普通字符串
       str1 = "This is a nulll-terminated string./n";
       cout << str1;
       //使用另一个字符串对象创建一个字符串对象
       string str5(str1);
       cout << str5;
       //输入字符串
       cout << "Enter a string: ";
       cin >> str5;
       cout << str5;
       return 0;
}
程序的输出如下：
Alpha
Omega
AlphaBeta
Alpha to Omega
str3 > str1
This is a nulll-terminated string.
This is a nulll-terminated string.
Enter a string: STL
STL
注意字符是通过使用什么来完成这些操作的。比如，“+”是用来连结两个字符串的，而“-”是用来比较两个字符串。如果使用C风格的运算符处理0结尾的字符串，就要调用不大方便的strcat()和strcmp()函数。由于C++string对象能够自由地与C风格的字符串混合使用，所以在程序中使用它们没有什么缺点，只有获得可观的优点。
在前面的程序中还有一个地方需要注意：字符串的大小并没有指定。string对象能够自己调整大小以便容纳字符串。因此，当赋值或连结字符串时，目标字符串将自动增长以便能够容纳新字符串，所以跑到字符串的后面去是不可能的。这种字符串的动态属性是它们比较标准0结尾的字符串（会遭受边界越界）更好的一个地方。
一些字符串成员函数Some string Member Functions
虽然很多简单的字符串操作可以使用字符串操作符来完成，但更复杂和更精细的操作必须使用string的成员函数。由于string有庞大的成员函数，我们将只检验几个最常用的。
基本字符串操作Basic String Manipulations
要把一个字符串赋值给另一个，使用assign()函数。它的两种形式是：
string &assign(const string & *strob*, size_type *start*, size_type *num*);
string &assign(const char **str*, size_type *num*);
第一种形式中，*strob*中*start*所指的位置开始的*num*个字符被赋给调用对象，第二种形式，0结尾的字符串*str*的前面*num*个字符被赋给调用对象。不管是哪种情况，都返回调用对象的引用。当然，使用“＝”号把一个字符串全部赋给另一个要容易得多，你只有对部分字符串进行赋值是才使用assign()函数。
你可以使用append()成员函数把一个字符串的一部分追加到另一个字符串的后面。它的两种形式如下：
string &append(const string &*strob*, size_type *start*, size_type *num*);
string &append(const char **str*, size_type *num*);
这里，*strob*中从*start*所指位置开始的*num*个字符将被追加到调用对象中去。在第二种形式中，0结尾的字符串*str*开始的*num*个字符将被追加到调用对象中。不管是哪种形式，都返回调用对象的引用。当然，使用“＋”号把整个字符串追加到另一个末尾更简单，当只追加一个字符串的一部分时才使用append()函数。
你可以使用insert()和replace()函数向字符插入或替换字符。最常用的原型如下：
string &insert(size_type *start*, const string &*strob*);
string &insert(size_type *start*, const string &*strob*,
                     size_type *insStart*, size_type *num*);
string &replace(size_type *start*, size_type *num*, const string &*strob*);
string &replace(size_type *start*, size_type *orgNum*, const string &*strob*,
                     size_type *replaceStart*, size_type *replaceNum*);
insert()的第一种形式向调用的字符串在*start*位置插入*strob*，insert()的第二种形式是在调用的字符串的*start*位置插入*strob*中*insStart*位置开始的*num*个字符。
replace()的第一种形式是在调用字符串的开始位置替换成*strob*的*num*个字符。第二种形式是用*strob*字符串*replaceStart*位置开始的*replaceNum*个字符替换掉调用字符串*start*位置开始的*orgNum*个字符。两种形式都是返回调用对象的引用。
你可以使用erase()从字符串中移去字符。它的其中一种形式是：
string &erase(size_type *start*=0, size_type *num*=npos);
它从调用字符串的start位置开始移去num个字符，返回调用字符串的引用。
下面的程序演示了insert()，erase()和replace()等函数。
//演示insert(), erase()和replace()
#include <iostream>
#include <string>
using namespace std;
int main()
{
       string str1("String handling C++ style.");
       string str2("STL Power");
       cout << "Initial strings:/n";
       cout << "str1: " << str1 << endl;
       cout << "str2: " << str2 << "/n/n";
       //演示insert();
       cout << "Insert str2 int str1:/n";
       str1.insert(6, str2);
       cout << str1 << "/n/n";
       //演示erase()
       cout << "Remove 9 characters from str1:/n";
       str1.erase(6,9);
       cout << str1 << "/n/n";
       //演示replace()
       cout << "Replace 8 characters in str1 with str2:/n";
       str1.replace(7, 8, str2);
       cout << str1 << endl;
       return 0;
}
程序输出如下：
Initial strings:
str1: String handling C++ style.
str2: STL Power
Insert str2 int str1:
StringSTL Power handling C++ style.
Remove 9 characters from str1:
String handling C++ style.
Replace 8 characters in str1 with str2:
String STL Power C++ style.
查找字符串Searching a String
string类提供了几个成员函数用来查找字符串，其中包括find()和rfind()。这里是这些函数中几个最常用的原型：
size_type find(const string &*strob*, size_type *start*=0) const;
size_type rfind(const string &*strob*, size_type *start*=npos) const
find()在调用字符串的开始位置查找第一次遇到的*strob*，如果找到，find()返回调用字符串中匹配发生的索引，如果没有找到，则返回npos。rfind()与find()相反，它从调用字符串的start位置逆向查找第一次遇到的strob（比如，它在调用字符串中查找最后一次遇到的strob），如果找到，则返回调用字符串中匹配发生的索引。如果没有找到，则返回npos。
这里是使用find()和rfind()的简短程序：
#include <iostream>
#include <string>
using namespace std;
int main()
{
       int i;
       string s1=
              "Quick of Mind, Strong of Body, Pure of Heart";
       string s2;
       i = s1.find("Quick");
       if(i != string::npos) {
              cout << "Match found at " << i << endl;
              cout << "Remaining string is:/n";
              s2.assign(s1, i, s1.size());
              cout << s2;
       }
       cout << "/n/n";
       i = s1.find("Strong");
       if(i !=string::npos){
              cout << "Match found at " << i << endl;
              cout << "Remaining string is:/n";
              s2.assign(s1, i, s1.size());
              cout << s2;
       }
       cout << "/n/n";
       i = s1.find("Pure");
       if(i != string::npos) {
              cout << "Match found at " << i << endl;
              cout << "Remaining string is:/n";
              s2.assign(s1, i, s1.size());
              cout << s2;
       }
       cout << "/n/n";
       //查找“of”
       i = s1.rfind("of");
       if(i != string::npos) {
              cout << "Match found at " << i << endl;
              cout << "Remaining string is:/n";
              s2.assign(s1, i, s1.size());
              cout << s2;
       }
       return 0;
}
程序输出如下：
Match found at 0
Remaining string is:
Quick of Mind, Strong of Body, Pure of Heart
Match found at 15
Remaining string is:
Strong of Body, Pure of Heart
Match found at 31
Remaining string is:
Pure of Heart
Match found at 36
Remaining string is:
of Heart
比较字符串Comparing Strings
要比较一个字符串对象与另一个字符串对象的全部内容，你只需使用前面讨论的已经重载的合理的运算符。然而，当你要比较一个字符串对象的部分时，你需要使用compare()成员函数。如下所示：
int compare(size_type *start*, size_type *num*, const string &*strob*) const;
这里，*strob*中*start*位置开始的*num*个字符将与调用的字符串进行比较。如果调用字符串比strob小，compare()返回小于0。如果调用字符串比strob大，则返回大于0，如果相等，则返回0。
得到0结尾的字符串Obtaining a Null-Terminated String
虽然string对象本身非常有用，但有时需要得到0结尾的字符数组形式的字符串。比如，你可能需要使用一个string对象构造文件名。当要打开文件时，你需要提供一个指向标准的0结尾的字符串的指针。为了解决这个问题，string对象提供了c_str()成员函数。原型如下：
const char *c_str() const;
这个函数一个指向0结尾版本的包含在string对象中的字符串的指针，不要改变这个0结尾的字符串。当string对象发生了其它操作之后，也不保证它仍然有效。
字符串是容器Strings Are Containers
string类符合作为容器的基本要求。因此，它支持普通容器函数，例如begin()，end()和size()。它也支持迭代器。string对象也可被STL算法使用。下面是一个简单例子：
//字符串作为容器
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main()
{
       string str1("Strings handling is easy in C++");
       string::iterator p;
       unsigned int i;
       //使用size()
       for(i=0; i<str1.size(); i++)
              cout << str1[i];
       cout << endl;
       //使用迭代器
       p = str1.begin();
       while(p != str1.end())
              cout << *p++;
       cout << endl;
       //使用count()算法
       i = count(str1.begin(), str1.end(), 'i');
       cout << "There are " << i << " i's in str1./n";
       //使用transform()把字符串转为大写
       transform(str1.begin(), str1.end(), str1.begin(), toupper);
       p = str1.begin();
       while(p != str1.end())
              cout << *p++;
       cout << endl;
       return 0;
}
输出如下:
Strings handling is easy in C++
Strings handling is easy in C++
There are 4 i's in str1.
STRINGS HANDLING IS EASY IN C++
把字符串放进其它容器Putting Strings into Other Containers
即使string是容器，字符串对象通常放在STL容器中，像映射和列表一样。例如，这里有一个更好的办法编写前面的电话簿程序，它使用string对象映射而不使用0结尾字符串容纳姓名或电话号码。
//使用字符串映射创建电话簿
#include <iostream>
#include <map>
#include <string>
using namespace std;
int main()
{
       map<string, string> directory;
       directory.insert(pair<string, string>("Tom", "555-4533"));
       directory.insert(pair<string, string>("Chris", "555-9678"));
       directory.insert(pair<string, string>("John", "555-8195"));
       directory.insert(pair<string, string>("Rachel", "555-0809"));
       string s;
       cout << "Enter name: ";
       cin >> s;
       map<string, string>::iterator p;
       p = directory.find(s);
       if(p != directory.end())
              cout << "Phone number: " << p->second;
       else
              cout << "Name not in directory./n";
       return 0;
}
STL最后思考Final Thoughts on the STL
STL是C++语言的一个重要、不可或缺的部分，很多编程任务都可以使用它来设计。STL把强大功能和灵活结合在一起，虽然它的语法有点复杂，但使用的方便是非常显著的。没有C++程序员会忽略STL，因为它将在将来的编程过程中扮演重要角色。
