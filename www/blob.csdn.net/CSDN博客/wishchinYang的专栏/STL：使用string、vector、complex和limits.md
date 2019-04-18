# STL：使用string、vector、complex和limits - wishchinYang的专栏 - CSDN博客
2013年11月26日 10:36:29[wishchin](https://me.csdn.net/wishchin)阅读数：765
 （有少量修改！）使用到了STL的算法库：
#include<algorithm>
#include<vector> //属于STL库 模板库 
写库的人为了和标准C和C++库区分开 
所有的模板库的文件都没有后缀 
一般如 #include <iostream.h> 
模板库中 #include <iostream>
Tips:在GCC下，除了要用#include <vector>外，还要加一句 unsing namespace std;才能直接使用vector<int>，否则的话，用 std::vector<int>
 xxx;也可以。
要引入名字空间 
方法有三个： 
1, using namespace std; 
2, using std::vector; 
前两个方法放在cpp文件头 ，h的头也可以！
3,楼上那样，在每个地方用： std::vector<int> xx;
C++标准库大量使用了模板。尽管使用模板进行编码并不是必要的，但是使用模板代码却是非常重要的(可以节省大量代码)。本节讨论由C++标准库和STL提供的一些有用的模板类型。充分利用STL非常重要，这也是第7章讨论的主题。
1.1.1　string和basic_string<>
string库是一个涵盖非常广泛的库，它使用模板创建了一系列字符串类型。使用字符串时，在string库和cstring库中，应该优先考虑string库，原因是cstring是旧的C标准库提供的char*字符串。
将string作为一个类型使用非常简单，它实际上是一个专门包含类型char的基本容器类型。事实上，string是使用char类型实例化的模板basic_string <char T>，basic_ string<> 模板也可以用于存储宽字符类型wchar_t，这在ASCII字符集不能完全表示所需的字符集时很有用，例如汉语、日语、芬兰语或者韩国语这些语言。 basic_string<>类表示一个字符序列，它包含了顺序容器的所有常用操作，以及标准的字符串操作，例如拼接。
因为类型string和wstring是分别使用basic_string<char>和basic_string<wchar_t>进行的typedef，所以不需要直接使用模板basic_string<>。
下面的例子说明了string类型的一些特点。
文件 templateString.cpp
// String class to rewrite a sentence.
#include <iostream>
#include <string>
using namespace std;
int main()
{
   string sentence, words[10];
   int pos = 0, old_pos = 0, nwords, i = 0;
   sentence = "Eskimos have 23 ways to ";
   sentence += "describe snow";
   while (pos < sentence.size()) {
      pos = sentence.find(' ', old_pos);
      words[i].assign(sentence, old_pos,
                      pos - old_pos);
      cout << words[i++] << endl;                  // print words
      old_pos = pos + 1;
   }
   nwords = i;
   sentence = "C++ programmers ";
   for (i = 1; i < nwords -1; ++i)
   sentence += words[i] + ' ';
   sentence += "windows";
   cout << sentence << endl;
}
string类型用于从一个句子中捕获一个个单词，最初这个句子中的每个单词都由空格符分隔。空格符的位置通过成员函数find()计算得出，然后成员函数assign()从sentence中选择一个子串。最后，使用重载函数operator=()、 operator+=()和operator+()进行赋值和拼接，创建出一个新的句子。
注意，检查本地系统的文档非常重要，不同的厂商都提供了自己的实现规范。
1.1.2　标准模板库中的vector<>
我们开发了一个通用的类似数组的容器vector<>。完整的 std::vector<>由标准模板库vector提供。在多数情况下，最好使用vector类型代替基本数组类型，原因是：vector 检查数组是否越界，所以比基本数组更安全；vector能够重新分配数组长度并且有许多相关的标准方法，所以比基本数组更灵活；vector的表示与普通数组一样，所以很容易就可以取代基本数组。下面是一个示例：
#include <vector>
using namespace std;
template<class T>
T* find(vector<T> data, T v)
{
   int i;
   for (i = 0; i < data.size(); i++)
      if (data[i] == v);
         return &data[i];
      return 0;                          // indicates failure to find v　
}
注意，上述代码看上去就像是使用了典型的数组，唯一例外的是size()方法返回了vector的长度。
1.1.3　使用complex<>
complex库中的模板complex<>提供了一个复数类型，它与其他数值类型兼容。在早期的C++库中，complex并不是一个模板，它只是基于下列数据描述的一种类型：
class complex{
   // ……methods　
private:
   double x, y;
};
现在complex则由以下模板描述：
template <class SCALAR>
class complex{
   // ……methods
private:
   SCALAR x, y;
};
这使用户能够根据需要决定基本类型的精度。一般来说，这些基本类型是float、double或者long double。下面是用于测试这个类型的简单代码。
文件 complex.cpp
#include <iostream>
#include <complex>
using namespace std;
int main()
{
   complex<double> x(1,2.1), y;
   cout << "x = " << x << endl;
   y = x + 1.0;
   cout << "\ny = " << y << endl;
   // if (x < y) not allowed - no standard definition
   //   cout << "x less than y" << endl;
}
complex类型对科学家和工程师来说很重要，它说明了将C++扩展到新的领域是多么容易的事情。例如，许多科学家使用FORTRAN90编程，原因就是FORTRAN90中有复数类型。因此，C++实际上已经能够取代需要复数类型的FORTRAN程序。
注意注释掉的行是如何使用小于运算符的。在标准库中没有为complex类型定义该运算符，所以在对该模板实例化时编译将失败。如果读者自己定义了这个运算符，可以重载该运算符，运行上述代码。
1.1.4　limits和其他有用的模板
limits库描述了本地系统上的各种基础类型的特性，它通过模板类 numberic_limits<>为所有的数值类型提供了相应的属性信息。在传统的C语言方法中，每一种数据类型的属性都由唯一的宏标识符表示，例如INT_MAX表示本地系统的最大int型值。而在模板类numeric_limits<>中则定义了一个静态函数max()用于返回相应的属性值，例如numeric_ limits<int>::max()返回本地系统的最大int型值。使用模板类可以大大减少用于描述本地系统的符号名的数量。
下面是一个使用numeric_limits<>函数的例子：
文件 limits.cpp
#include <iostream>
#include <limits>
using namespace std;
int main()
{
   cout << numeric_limits<char>::digits << " char\n ";
   cout << numeric_limits<unsigned char>::digits << " u char\n";
   cout << numeric_limits<wchar_t>::digits << " wchar_t\n";
   cout << numeric_limits<int>::max() << " max int\n";
   cout << numeric_limits<double>::max() << " max double " << endl;
}
digits域给出当前类型的位数。
