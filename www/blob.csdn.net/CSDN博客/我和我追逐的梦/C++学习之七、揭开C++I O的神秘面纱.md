
# C++学习之七、揭开C++I-O的神秘面纱 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月30日 18:40:06[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3621


**揭开C++I/O的神秘面纱**
C++通过流（stream）机制来提供比C更先进的输入输出方法。每个流都有一个相关联的源和目的。包括控制台流、文件流、字符串流等。
**1.控制台流<iostream>:**
**流输出：**cout<< ，可输出包含int、指针、double、字符、string等类型。
输出流方法：
put() 和write()是原始的输出方法。这二个方法风别接受一个字符或字符数组，而不是取已经定义了某种输出行为的对象或者变量。传递给这二个方法的数据按照原来的格式输出，没有进行任何专门的格式化或者处理。转义字符（如\n）仍然按照正确的形式输出，但不会发生多态输出。
flush()：向输出流写数据时，流不必立即把数据写到目的中。大部分的输出流都会进入缓冲区（buffer），或者累积数据，而不是数据一到来就写出。当下列条件之一发生时，流会刷新输出（flush），或者写出累积数据：
a．到达一个标记，比如endl标记。
b．流超出了作用域，因此被撤销。
c．对应的输入流请求输入（也就是说，利用cin输入时，cout会刷新输出）。
d．流缓冲区已满。
e．明确告诉流要刷新输出其缓冲区。就是flush()方法。
cout<<”abc”;
cout.flush();
不是所有的输出流都会缓冲。比如，cerr流就没有对其输出进行缓冲。
**处理输出错误：**输出错误可能会在各种情况下产生。可能你试图向不存在或者已经指定为只读的文件写入数据。可能一个磁盘错误阻止了写操作成功执行，或者由于某种原因控制台进入了锁定状态。等等。可以调用方法good()来来确定流是否是好的。
cout.flush();
if(cout.good())
cout<<”cout good!”<<endl;
使用good()能够很容易地获取关于流有效性的基本信息，但是它不能告诉你为什么流是不可用的。方法bad()，它提供了更多的信息，如果放回true，表示发生了一个致命的错误（不同于诸如文件结束等非致命错误）。还有一个方法fail()，如果最近的操作失败，它返回true，意味着下一个操作也会失败。
cout.flush();
if(cout.fail())
cerr<<”unable to flush to standard out”<<endl;
要重置流的错误状态，使用clear()方法
cout.clear();
这里讨论的几种方法也适合其他流。
**输出控制符**：C++的流不仅可以识别数据，还可以识别出控制符（manipulator），这些控制符是一些对象，它们可以改变流的行为，而不是为流提供数据，或者除了提供数据之外还可以改变流的行为。比如endl控制符，它封装了数据与行为。它告诉流输出一个回车符并刷新输出其缓冲区。
a．boolalpha和noboolalpha，告诉流把bool类型的值输出为true或false(boolalpha)和1或0（noboolalpha）。默认为noboolalpha。
b．hex、oct、dec。分别以十六进制、八进制、十进制输出数字。
c．setprecision。设置输出小数时的小数位数。这是一个参数化的控制符，可以接受一个参数。
d．setw。设置输出数值数据时的字段宽度。这是一个参数化的控制符。
e．setfill。数字宽度小于指定的宽度时，这个控制符可以指定填充空位的字符。。这是一个参数化的控制符。
f．Showpoint和noshowpoing。对于没有小数部分的浮点数或者双精度数，分别表示显示和不显示小数点。
流也可以通过一些与之等效的方法来提供同样的功能，比如setPrecision()。
**流输入：**cin>> 它之后捕获第一个空白符之前的字符。
注意，即使cout没有使用endl或flush()明确刷新缓冲区，文本仍然会写到控制台上，因为使用cin就会立刻刷新输入cout缓冲区。
**输入方法：**
**get()，**此方法允许从流输入原始数据。仅仅返回流中的下一个字符。经常用于避免使用>>操作符时发生的自动词法分析。
下面可以读入有几个单词组成的字符串：
string readName(istream & inStream)
{
string name;
while(inStream.good())
{
int text = inStream.get();
if(next == EOF)  break;
name += next;
}
return  name;
}
首先，函数的参数是istream的引用，而不是其const引用，因为从流中读取数据的方法会改变所读取的具体流（会改变流的位置），所以它不是const方法。因此不能对const引用调用这些方法。第二，get()返回值存储在int中，而不是char中，因为get可以返回特殊的非字符值，比如EOF(文件结束)。会隐性转换为char。
另一个版本：
string readName(istream & inStream)
{
string name;
char   text;
while inStream.get(text))
{
name += next;
}
return name;
}
**unget()：**对于大部分目的而言，理解输入流的正确方法是把它看作是单向的清洗槽。数据落入其中并进入变量。方法unget会打破这种模型，它允许把数据推回到清洗槽中。以此unget会引起流后退一个位置，其本质是把最后一个字符读回到流中。
char ch1,ch2,ch3;
cin>>ch1>>ch2>>ch3;
cin.unget();
char ch4;
cin>>ch4;
//ch4==ch3
**putback()：**就向unget一样，允许在流中后退一个字符。二者的区别在于，putback取流中要后退的字符作为参数。
char ch1;
cin>>ch1;
cin.putback(ch1);
//ch1是输入流中得下一个字符。
**peek()：**允许预览下一个值。
int next = cin.peek();
if(isdigit(next) code;
else othercode;
**getline()：**从流中读入一行不超过指定值的数据。
char buffer[max+1];
cin.getline(buffer,max);
getline方法会删除换行字符。所得到的字符串不包括换行字符。而get方法会把换行字符留在输入流中。
还有一个getline方法可以用于C++字符串。
string myStr;
std::getline(cin,myStr);
**处理输入错误：**不光有前面的good()之类的方法，还有一个eof()方法，它用来判断是否到了流的结尾（文件结尾）。是，返回true，否则，返回false。
下段程序使用了从流中读取数据和处理错误的常用模式。
\#include<iostream>
\#include<fstream>
\#include<string>
using namespace std;
int main()
{
int sum = 0;
if(!cin.good())
{
cout<<”Standard input is in a bad state!”<<endl;
exit(1);
}
int number;
while(true)
{
cin>>number;
if(cin.good())
sum += number;
else if(cin.eof()) break;
else
{
cin.clear();
string badToken;
cin>>badToken;
cerr<<”WARNING:Bad input encounter:”<<badToken<<endl;
}
}
cout<<”The sum is :”<<sum<<endl;
return 0;
}
**输入控制符：**
a．oolalpha和noboolalpha，告诉流把bool类型的值输入为true或false(boolalpha)和1或0（noboolalpha）。默认为noboolalpha。
b．hex、oct、dec。分别以十六进制、八进制、十进制读取数字。
c．skipws和noskipws。告诉流在做词法分析时，忽略空白符或者把空白符读入作为一个空白符token。
d．ws。这是一个便利控制符，它只会忽略流中当前位置的一串空白。
**2.   输入与输出对象**
可以通过操作符重载，来控制输入输出对象的信息。
**3.   字符串流**
字符串流提供了一种对string应用流语义的方法。采用这种方法，可以有一个表示文本数据的内存中流。如果多个线程都向同一个字符串提供数据，或者需要把一个string传递给不同的函数，而同时还要维护当前的读取位置，在这样一些应用中，这种方法就很有用。因为流有内置的词法分析功能，所以字符串流对于解析文本也很有用。
类ostringstream和istringstream分别用于向字符串写数据和从字符串读数据。它们都在<sstream>头文件中定义的。分别继承了ostream和istream同样的行为。
**4.   文件流 见C++文件操作学习**
补充：
**seek()函数：**可以移动到输入或输出流的任意位置。输入流为seekg()，输出流为seekp()。每种类型的流都有二个查找方法。可以在流中查找绝对位置或相对位置。位置以字节为单位。
移动到流的开始位置：ios_base::beg
移动到流的结束位置：ios_base::end
移动到流的中间常量：ios_base::cur
带一个参数：
outStream.seekp(ios_base::beg)或inStream.seekg(ios_base::beg)
带二个参数（第一个参数表示要移多少位置，第二个参数表示移动的起点）：
移动到相对于流的开头的第二个字符：
outStream.seekp(2, ios_base::beg);
移动到相对于流的倒数的第二个字符：
isStream.seekg(-2,ios_base::end);
**tell()函数：**查询流的当前位置，返回ios_base::pos_type。输入流为tellg()，输出流为tellp()。

**5.   链接流**
可以在任何输入流与输出流之间建立链接。从而提供一种“一旦访问就刷新输出”的行为。换句话说，从输入流请求数据时，与其链接的输出流会自动刷新输出。这个行为可以用于所有的流，但是对于可能相互依赖的文件流尤其有用。
流链接用方法tie()来实现。要把输出流绑定到一个输入流上，可以在输入流上调用tie()，并把输出流的地址传递给它。要断开这个链接，传递NULL即可。
下面这段程序把一个文件的输出流绑到了另一个文件的输入流上。也可以把它绑定到同一个文件的输入流上，不过要同时读写一个文件，双向I/O可能更好。
\#include<iostream>
\#include<fstream>
\#include<string>
using namespace std;
int main()
{
ifstream inFile(“input.txt”);
ofstream outFile(“output.txt”);
inFile.tie(&outFile);
outFile<<”Hello there!”;
string text;
inFile>>text;
return 0;
}
方法flush()是在ostream基类上定义的，所以也可以把一个输出流绑定到另一个输出流上。
outFile.tie(&antherOutFile);
这种关系表示，每次向一个文件写数据时，就会向另一个文件写入已经发送的缓冲数据。可以使用这种机制保持二个相关文件之间的同步。
**6.   双向流**
前面一直把输入流，输出流最为独立的二个类来讨论的。实际上还有一种流可以同时进行输入输出。双向流可以同时作为输入流输出流操作。双向流是iostream的子类，所以也是istream和ostream的子类。
通过stringstream类，也可以一双向方式访问字符串流。
双向流对于读位置和写位置分别有单独的指针。在读操作与写操作之间切换时，需要查找正确的位置。
**7.   国际化**
**宽字符：**把字符看做是一个字节的问题就是，并不是所有编程语言和字符集都能用8为二进制为或1个字节来表示。幸运的是，C++提供了宽字符，wchar_t。它会像char一样工作，唯一区别的是字符串和字符直接量都有一个前缀字母L，来指示应该使用宽字符编码。
wchar_t letter = L’m’;
string有wstring，ofstream有wofstream，cin、cout、cerr有wcin、wcout、wcerr。等等。
**非西方字符集：**Unicode编码。可以查看相关资料。
**本地化环境与方面：**各国之间表示数据的唯一差别就是字符集。但是即使使用同一个字符集的国家之间也有差别，比如美国与英国之间类似日期和货币之类的。
标准C++提供了一种内置机制，他可以把特定地方的数据分组到一个本地化环境中。本地化环境是特定位置相关设置的集合。各个设置称为一个方面（facet）。C++还提供了定制或者增加方面的方法。
下面把美国英语本地化环境（en_US）附加到宽字符控制台输出流上。
wcout.imbue(locale(“en_US”));
默认的本地化环境不是美国英语，而是经典本地化环境，它使用ANSI C的约定。
比如，如果根本没有设置本地化环境，或者设置默认的本地化环境，并且要输出一个数字，那么输出时不带任何标点。
wcout.imbue(locale(“C”));
wcout<<32767<<endl;
32767
但是如果使用的本地化环境是美国英语，数字就会用美国英语的标点格式化。
wcout.imbue(locale(“en_US”));
wcout<<32767<<endl;
32,767
大多数操作系统都有一种机制来确定用户定义的本地化环境，在C++中，可以向本地化环境对象构造函数传递一个空字符串，从而由用户环境创建一个本地化环境。一旦创建了这个对象，就可以查询本地化环境。
locale loc(“”);
if(loc.name().find(“en_US”) == string::npos&&loc.name().find(“United States”)==string::npos))
----doing something----
根据本地化环境的名字来确定本地化环境，还不一定能够正确地确定用户是否的确在这个地方。，但是可以提供一条线索。
**使用方面：**
可以使用函数是std::use_facet()来获取特定本地化环境的特定方面。比如，下面用来检索英国本地化环境的标准货币符号方面。(需要使用头文件<locale>
use_facet<moneypunct<wchar_t> >(locale(“en_GB”));


