# 彻底解密 C++ 宽字符 - 文章 - 伯乐在线
原文出处： [wangjieest](http://blog.csdn.net/wangjieest/article/details/7454642)
#### 1、从 char 到 wchar_t
“这个问题比你想象中复杂”
**从字符到整数**
char 是一种整数类型，这句话的含义是，char所能表示的字符在C/C++中都是整数类型。好，接下来，很多文章就会举出一个典型例子，比如，’a’的数值就是0x61。这种说法对吗？如果你细心的读过K&R和BS对于C和C++描述的原著，你就会马上反驳道，0x61只是’a’的ASCII值，并没有任何规定C/C++的char值必须对应ASCII。C/C++甚至没有规定char占几位，只是规定了sizeof(char)等于1。
当然，目前大部分情况下，char是8位的，并且，在ASCII范围内的值，与ASCII对应。
**本地化策略集（locale）**
“将 ‘a’翻译成0x61的整数值”，“将ASCII范围内的编码与char的整数值对应起来”，类似这样的规定，是特定系统和特定编译器制定的，C/C++ 中有个特定的名词来描述这种规定的集合：本地化策略集（locale。也有翻译成“现场”）。而翻译——也就是代码转换（codecvt）只是这个集合中的一个，C++中定义为策略（facet。也有翻译为“刻面”）
**C/C++ 的编译策略**
“本地化策略集”是个很好的概念，可惜在字符和字符串这个层面上，C/C++并不使用（C++的locale通常只是影响流（stream）），C/C++使用更直接简单的策略：硬编码。
简单的说，字符（串）在程序文件（可执行文件，非源文件）中的表示，与在程序执行中在内存中的表示一致。考虑两种情况：
A、char c = 0x61;
B、char c = ‘a’;
情况A下，编译器可以直接认识作为整数的c，但是在情况B下，编译器必须将’a’翻译成整数。编译器的策略也很简单，就是直接读取字符（串）在源文件中的编码数值。比如：


```
const char* s = "中文abc";
```
这段字符串在GB2312（Windows 936），也就是我们的windows默认中文系统源文件中的编码为：


```
0xD6 0xD0 0xCE 0xC4 0x61 0x62 0x63
```
在UTF-8，也就是Linux默认系统源文件中的编码为：


```
0xE4 0xB8 0xAD 0xE6 0x96 0x87 0x61 0x62 0x63
```
一般情况下，编译器会忠实于源文件的编码为s赋值，例外的情况比如VC会自作聪明的把大部分其他类型编码的字符串转换成GB2312（除了像UTF-8 without signature这样的幸存者）。
程序在执行的时候，s也就保持是这样的编码，不会再做其他的转换。
**宽字符 wchar_t**
正如char没有规定大小，wchar_t同样没有标准限定，标准只是要求一个wchar_t可以表示任何系统所能认识的字符，在win32 中，wchar_t为16位；Linux中是32位。wchar_t同样没有规定编码，因为Unicode的概念我们后面才解释，所以这里只是提一下，在 win32中，wchar_t的编码是UCS-2BE；而Linux中是UTF-32BE（等价于UCS-4BE），不过简单的说，在16位以内，一个字符的这3种编码值是一样的。因此：


```
const wchar_t* ws = L"中文abc";
```
的编码分别为：


```
0x4E2D 0x6587 0x0061 0x0062 0x0063 //win32，16位
0x00004E2D 0x00006587 0x00000061 0x00000062 0x00000063 //Linux，32位
```
大写的L是告诉编译器：这是宽字符串。所以，这时候是需要编译器根据locale来进行翻译的。
比如，在Windows环境中，编译器的翻译策略是GB2312到UCS-2BE；Linux环境中的策略是UTF-8到UTF-32BE。
这时候就要求源文件的编码与编译器的本地化策略集中代码翻译的策略一致，例如VC只能读取GB2312的源代码（这里还是例外，VC太自作聪明了 ，会将很多其他代码在编译时自动转换成GB2312），而gcc只能读取UTF-8的源代码（这里就有个尴尬，MinGW运行win32下，所以只有 GB2312系统才认；而MinGW却用gcc编写，所以自己只认UTF-8，所以结果就是，MinGW的宽字符被废掉了）。
宽字符（串）由编译器翻译，还是被硬编码进程序文件中。
#### 2、Unicode 和 UTF
**Unicode 和 UCS**
Unicode 和UCS是两个独立的组织分别制定的一套编码标准，但是因为历史的原因，这两套标准是完全一样的。Unicode这个词用得比较多的原因可能是因为比较容易记住，如果没有特别的声明，在本文所提及的Unicode和UCS就是一个意思。Unicode的目标是建立一套可以包含人类所有语言文字符号你想得到想不到的各种东西的编码，其编码容量甚至预留了火星语以及银河系以外语言的空间——开个玩笑，反正简单的说，Unicode编码集足够的大，如果用计算机单位来表示，其数量比3个字节大一些，不到4个字节。
**Unicode和UTF**
因为Unicode包含的内容太多，其编码在计算机中的表示方法就成为了一个有必要研究的问题。传统编码，比如标准的7位ASCII，在计算机中的表示方法就是占一个字节的后7位，这似乎是不需要解释就符合大家习惯的表示方法。但是当今Unicode的总数达到32位（计算机的最小单位是字节，所以大于3字节，就只能至少用4字节表示），对于大部分常用字符，比如Unicode编码只占一个字节大小的英语字母，占两个字节大小汉字，都用4个字节来储存太奢侈了。另外，如果都用4字节直接表示，就不可避免的出现为0的字节。而我们知道，在C语言中，0x00的字节就是’\0’，表示的是一个字符串（char字符串，非wchar_t）的结束，换句话说，C风格的char字符串无法表示Unicode。
因为类似的种种问题，为Unicode在计算机中的编码方法出现了，这就是UTF；所对应的，为UCS编码实现的方式也有自己的说法。一般来说，UTF-x，x表示这套编码一个单位至少占用x位，因为Unicode最长达到32位，所以 UTF-x通常是变长的——除了UTF-32；而UCS-y表示一个单位就占用y个字节，所以能表示当今Unicode的UCS-y只有UCS-4，但是因为历史的原因，当Unicode还没那么庞大的时候，2个字节足够表示，所以有UCS-2，现在看来，UCS-2所能表示的Unicode只是当今 Unicode的一个子集。
也就是说，如果某种编码，能根据一定的规则算法，得到Unicode编码，那么这种编码方式就可以称之为UTF。
**UTF-8 和 Windows GB2312**
UTF- 8是一套“聪明”的编码，可能用1，2，3，4个字节表示。通过UTF-8的算法，每一个字节表示的信息都很明确：这是不是某个Unicode编码的第一个字节；如果是第一个字节，这是一个几位Unicode编码。这种“聪明”被称为UTF-8的自我同步，也是UTF-8成为网络传输标准编码的原因。
另外，UTF-8也不会出现0字节，所以可以表示为char字符串，所以可以成为系统的编码。Linux系统默认使用UTF-8编码。
Windows GB2312一般自称为GB2312，其实真正的名字应该是Windows Codepage 936，这也是一种变长的编码：1个字节表示传统的ASCII部分；汉字部分是两个字节的GBK（国标扩（展），拼音声母）。Codepage 936也可以表示为char字符串，是中文Windows系统的默认编码。
我们在第1节中看到的


```
const char* s = "中文abc";
```
在Windows中的编码就是Codepage 936；在Linux中的编码就是UTF-8。
需要注意的是，Codepage 936不像UTF，跟Unicode没有换算的关系，所以只能通过“代码页”技术查表对应。
**UTF-16 和 UCS-2**
UTF- 16用2个字节或者4个字节表示。在2个字节大小的时候，跟UCS-2是一样的。UTF-16不像UTF-8，没有自我同步机制，所以，编码大位在前还是小位在前，就成了见仁见智的问题。我们在第1节中，“中”的UCS-2BE（因为是两个字节，所以也就是UTF-16BE）编码是0x4E2D，这里的 BE就是大位在后的意思（也就是小位在前了），对应的，如果是UCS-2LE，编码就成了0x2D4E。
Windows中的wchar_t就是采用UCS-2BE编码。需要指出的是，C++标准中对wchar_t的要求是要能表示所有系统能识别的字符。Windows自称支持Unicode，但是其wchar_t却不能表示所有的Unicode，由此违背了C++标准。
**UTF-32 和 UCS-4**
UTF- 32在目前阶段等价于UCS-4，都用定长的4个字节表示。UTF-32同样存在BE和LE的问题。Linux的wchar_t编码就是UTF- 32BE。在16位以内的时候，UTF-32BE的后两位（前两位是0x00 0x00）等价于UTF-16BE也就等价于UCS-2BE
**BOM**
为了说明一个文件采用的是什么编码，在文件最开始的部分，可以有BOM，比如0xFE 0xFF表示UTF-16BE，0xFF 0xFE 0x00 0x00表示UTF-32LE。UTF-8原本是不需要BOM的，因为其自我同步的特性，但是为了明确说明这是UTF-8（而不是让文本编辑器去猜），也可以加上UTF-8的BOM：0xEF 0xBB 0xBF
以上内容都讲述得很概略，详细信息请查阅维基百科相关内容。
#### 3、利用C运行时库函数转换
**std::locale**
通过前面两节的知识，我们知道了在C/C++中，字符（串）和宽字符（串）之间的转换不是简单的，固定的数学关系，宽窄转换依赖于本地化策略集（locale）。换句话说，一个程序在运行之前并不知道系统的本地化策略集是什么，程序只有在运行之后才通过locale获得当时的本地化策略集。
C有自己的locale函数，我们这里直接介绍C++的locale类。
先讨论locale的构造函数：


```
locale() throw();
```
这个构造函数是获得当前程序的locale，用法如下：


```
std::locale app_loc = std::locale();
```
或者（这是构造对象的两种表示方式，后同）


```
std::locale app_loc;
```
另外一个构造函数是：


```
explicit locale(const char* name);
```
这个构造函数以name的名字创建新的locale。重要的locale对象有：
std::locale sys_loc(“”); //获得当前系统环境的locale
std::locale C_loc(“C”); 或者 std::locale C_loc = std::locale::classic(); //获得C定义locale
std::locale old_loc = std::locale::global(new_loc); //将new_loc设置为当前全局locale，并将原来的locale返回给old_loc
除了这些，其它的name具体名字依赖于C++编译器和操作系统，比如Linux下gcc中文系统的locale名字为”zh_CN.UTF-8″，中文Windows可以用”chs”（更加完整的名字可以用name()函数查看）。
**mbstowcs() 和 wcstombs()**
这两个C运行时库函数依赖于全局locale进行转换，所以，使用前必须先设置全局locale。
std::locale已经包含在<iostream>中了，再加上我们需要用到的C++字符串，所以包含<string>。
我们先看窄到宽的转换函数：


```
const std::wstring s2ws(const std::string& s)
{
    std::locale old_loc =
        std::locale::global(std::locale(""));
    const char* src_str = s.c_str();
    const size_t buffer_size = s.size() + 1;
    wchar_t* dst_wstr = new wchar_t[buffer_size];
    wmemset(dst_wstr, 0, buffer_size);
    mbstowcs(dst_wstr, src_str, buffer_size);
    std::wstring result = dst_wstr;
    delete []dst_wstr;
    std::locale::global(old_loc);
    return result;
}
```
我们将全局locale设置为系统locale，并保存原来的全局locale在old_loc中。
在制定转换空间缓存大小的时候，考虑如下：char是用1个或多个对象，也就是1个或者多个字节来表示各种符号：比如，GB2312用1个字节表示数字和字母，2个字节表示汉字；UTF-8用一个字节表示数字和字母，3个字节表示汉字，4个字节表示一些很少用到的符号，比如音乐中G大调符号等。wchar_t是用1个对象（2字节或者4字节）来表示各种符号。因此，表示同样的字符串，宽字符串的大小（也就是wchar_t对象的数量）总是小于或者等于窄字符串大小（char对象数量）的。+1 是为了在最后预留一个值为0的对象，以便让C风格的char或者wchar_t字符串自动截断——这当然是宽串大小等于窄串大小的时候才会用上的，大部分时候，字符串早在前面某个转换完毕的位置就被0值对象所截断了。
最后我们将全局locale设置回原来的old_loc。
窄串到宽串的转换函数：


```
const std::string ws2s(const std::wstring& ws)
{
    std::locale old_loc =
        std::locale::global(std::locale(""));
    const wchar_t* src_wstr = ws.c_str();
    size_t buffer_size = ws.size() * 4 + 1;
    char* dst_str = new char[buffer_size];
    memset(dst_str, 0, buffer_size);
    wcstombs(dst_str ,src_wstr, buffer_size);
    std::string result = dst_str;
    delete []dst_str;
    std::locale::global(old_loc);
    return result;
}
```
这里考虑转换空间缓存大小的策略正好相反，在最极端的情况下，所有的wchar_t都需要4个char来表示，所以最大的可能就是4倍加1。
这两个函数在VC和gcc中都能正常运行（MinGW因为前面说到的原因不支持宽字符的正常使用），在VC中会给出不安全的警告，这是告诉给那些弄不清宽窄转换实质的人的警告，对于了解到目前这些知识的你我来说，这就是啰嗦了。
#### 4、利用 codecvt 和 use_facet 转换
**locale 和 facet**
C++ 的locale框架比C更完备。C++除了一个笼统本地策略集locale，还可以为locale指定具体的策略facet，甚至可以用自己定义的 facet去改造一个现有的locale产生一个新的locale。如果有一个facet类NewFacet需要添加到某个old_loc中形成新 new_loc，需要另外一个构造函数，通常的做法是：


```
std::locale new_loc(old_loc, new NewFacet);
```
标准库里的标准facet都具有自己特有的功能，访问一个locale对象中特定的facet需要使用模板函数use_facet：
template <class Facet> const Facet& use_factet(const locale&);
换一种说法，use_facet把一个facet类实例化成了对象，由此就可以使用这个facet对象的成员函数。
**codecvt**
codecvt就是一个标准facet。在C++的设计框架里，这是一个通用的代码转换模板——也就是说，并不是仅仅为宽窄转换制定的。


```
templat <class I, class E, class State> class std::codecvt: public locale, public codecvt_base{...};
```
I表示内部编码，E表示外部编码，State是不同转换方式的标识，如果定义如下类型：


```
typedef std::codecvt<wchar_t, char, mbstate_t> CodecvtFacet;
```
那么CodecvtFacet就是一个标准的宽窄转换facet，其中mbstate_t是标准宽窄转换的State。
**内部编码和外部编码**
我们考虑第1节中提到的C++编译器读取源文件时候的情形，当读到L”中文abc”的时候，外部编码，也就是源文件的编码，是GB2312或者UTF-8的 char，而编译器必须将其翻译为UCS-2BE或者UTF-32BE的wchar_t，这也就是程序的内部编码。如果不是宽字符串，内外编码都是 char，也就不需要转换了。类似的，当C++读写文件的时候 ，就会可能需要到内外编码转换。事实上，codecvt就正是被文件流缓存basic_filebuf所使用的。理解这一点很重要，原因会在下一小节看到。
**CodecvtFacet 的 in() 和 out()**
因为在CodecvtFacet中，内部编码设置为wchar_t，外部编码设置为char，转换模式是标准宽窄转换mbstate_t，所以，类方法in()就是从char标准转换到wchar_t，out()就是从 wchar_t标准转换到char。这就成了我们正需要的内外转换函数。


```
result in(State& s, const E* from, const E* from_end, const E*& from_next, I* to, I* to_end, I*& to_next) const;
result out(State& s, const I* from, const I* from_end, const I*& from_next, E* to, E* to_end, E*& to_next) const;
```
其中，s是非const引用，保存着转换位移状态信息。这里需要重点强调的是，因为转换的实际工作交给了运行时库，也就是说，转换可能不是在程序的主进程中完成的，而转换工作依赖于查询s的值，因此，如果s在转换结束前析构，就可能抛出运行时异常。所以，最安全的办法是，将s设置为全局变量！
const的3个指针分别是待转换字符串的起点，终点，和出现错误时候的停点（的下一个位置）；另外3个指针是转换目标字符串的起点，终点以及出现错误时候的停点（的下一个位置）。
代码如下：


```
//头文件
//Filename string_wstring_cppcvt.hpp
#ifndef STRING_WSTRING_CPPCVT_HPP
#define STRING_WSTRING_CPPCVT_HPP
#include <iostream>
#include <string>
const std::wstring s2ws(const std::string& s);
const std::string ws2s(const std::wstring& s);
#endif
//实现
#include "string_wstring_cppcvt.hpp"
mbstate_t in_cvt_state;
mbstate_t out_cvt_state;
const std::wstring s2ws(const std::string& s)
{
    std::locale sys_loc("");
    const char* src_str = s.c_str();
    const size_t BUFFER_SIZE = s.size() + 1;
    wchar_t* intern_buffer = new wchar_t[BUFFER_SIZE];
    wmemset(intern_buffer, 0, BUFFER_SIZE);
    const char* extern_from = src_str;
    const char* extern_from_end = extern_from + s.size();
    const char* extern_from_next = 0;
    wchar_t* intern_to = intern_buffer;
    wchar_t* intern_to_end = intern_to + BUFFER_SIZE;
    wchar_t* intern_to_next = 0;
    typedef std::codecvt<wchar_t, char, mbstate_t> CodecvtFacet;
    CodecvtFacet::result cvt_rst =
        std::use_facet<CodecvtFacet>(sys_loc).in(
            in_cvt_state,
            extern_from, extern_from_end, extern_from_next,
            intern_to, intern_to_end, intern_to_next);
    if (cvt_rst != CodecvtFacet::ok) {
        switch(cvt_rst) {
            case CodecvtFacet::partial:
                std::cerr << "partial";
                break;
            case CodecvtFacet::error:
                std::cerr << "error";
                break;
            case CodecvtFacet::noconv:
                std::cerr << "noconv";
                break;
            default:
                std::cerr << "unknown";
        }
        std::cerr    << ", please check in_cvt_state."
                    << std::endl;
    }
    std::wstring result = intern_buffer;
    delete []intern_buffer;
    return result;
}
const std::string ws2s(const std::wstring& ws)
{
    std::locale sys_loc("");
    const wchar_t* src_wstr = ws.c_str();
    const size_t MAX_UNICODE_BYTES = 4;
    const size_t BUFFER_SIZE =
                ws.size() * MAX_UNICODE_BYTES + 1;
    char* extern_buffer = new char[BUFFER_SIZE];
    memset(extern_buffer, 0, BUFFER_SIZE);
    const wchar_t* intern_from = src_wstr;
    const wchar_t* intern_from_end = intern_from + ws.size();
    const wchar_t* intern_from_next = 0;
    char* extern_to = extern_buffer;
    char* extern_to_end = extern_to + BUFFER_SIZE;
    char* extern_to_next = 0;
    typedef std::codecvt<wchar_t, char, mbstate_t> CodecvtFacet;
    CodecvtFacet::result cvt_rst =
        std::use_facet<CodecvtFacet>(sys_loc).out(
            out_cvt_state,
            intern_from, intern_from_end, intern_from_next,
            extern_to, extern_to_end, extern_to_next);
    if (cvt_rst != CodecvtFacet::ok) {
        switch(cvt_rst) {
            case CodecvtFacet::partial:
                std::cerr << "partial";
                break;
            case CodecvtFacet::error:
                std::cerr << "error";
                break;
            case CodecvtFacet::noconv:
                std::cerr << "noconv";
                break;
            default:
                std::cerr << "unknown";
        }
        std::cerr    << ", please check out_cvt_state."
                    << std::endl;
    }
    std::string result = extern_buffer;
    delete []extern_buffer;
    return result;
}
```
最后补充说明一下std::use_facet<CodecvtFacet>(sys_loc).in()和 std::use_facet<CodecvtFacet>(sys_loc).out()。sys_loc是系统的locale，这个 locale中就包含着特定的codecvt facet，我们已经typedef为了CodecvtFacet。用use_facet对CodecvtFacet进行了实例化，所以可以使用这个 facet的方法in()和out()。
#### 5、利用 fstream 转换
C++的流和本地化策略集
BS在设计C++流的时候希望其具备智能化，并且是可扩展的智能化，也就是说，C++的流可以“读懂”一些内容。比如：


```
std::cout << 123 << "ok" << std::endl;
```
这句代码中，std::cout是能判断出123是int而”ok”是const char[3]。利用流的智能，甚至可以做一些基础类型的转换，比如从int到string，string到int：


```
std::string str("123");
std::stringstream sstr(str);
int i;
sstr >> i;
int i = 123;
std::stringstream sstr;
sstr << i;
std::string str = sstr.str();
```
尽管如此，C++并不满足，C++甚至希望流能“明白”时间，货币的表示法。而时间和货币的表示方法在世界范围内是不同的，所以，每一个流都有自己的 locale在影响其行为，C++中叫做激活（imbue，也有翻译成浸染）。而我们知道，每一个locale都有多个facet，这些facet并非总是被use_facet使用的。决定使用哪些facet的，是流的缓存basic_streambuf及其派生类basic_stringbuf和 basic_filebuf。我们要用到的facet是codecvt，这个facet只被basic_filebuf使用——这就是为什么只能用 fstream来实现宽窄转换，而无法使用sstream来实现的原因。


```
//头文件
//filename string_wstring_fstream.hpp
#ifndef STRING_WSTRING_FSTREAM_HPP
#define STRING_WSTRING_FSTREAM_HPP
#include <string>
const std::wstring s2ws(const std::string& s);
const std::string ws2s(const std::wstring& s);
#endif
//实现：
#include <string>
#include <fstream>
#include "string_wstring_fstream.hpp"
const std::wstring s2ws(const std::string& s)
{
    std::locale sys_loc("");
    std::ofstream ofs("cvt_buf");
    ofs << s;
    ofs.close();
    std::wifstream wifs("cvt_buf");
    wifs.imbue(sys_loc);
    std::wstring wstr;
    wifs >> wstr;
    wifs.close();
    return wstr;
}
const std::string ws2s(const std::wstring& s)
{
    std::locale sys_loc("");
    std::wofstream wofs("cvt_buf");
    wofs.imbue(sys_loc);
    wofs << s;
    wofs.close();
    std::ifstream ifs("cvt_buf");
    std::string str;
    ifs >> str;
    ifs.close();
    return str;
}
```
在窄到宽的转化中，我们先使用默认的本地化策略集（locale）将s通过窄文件流ofs传入文件，这是char到char的传递，没有任何转换；然后我们打开宽文件流wifs，并用系统的本地化策略集（locale）去激活（imbue）之，流在读回宽串wstr的时候，就是char到wchar_t的转换，并且因为激活了sys_loc，所以实现标准窄到宽的转换。
在宽到窄的转化中，我们先打开的是宽文件流wofs，并且用系统的本地化策略集 sys_loc激活（imbue）之，这时候，因为要写的文件cvt_buf是一个外部编码，所以执行了从wchar_t到char的标准转换。读回来的文件流从char到char，不做任何转换。
#### 6、国际化策略
**硬编码的硬伤**
我们现在知道，C/C++的宽窄转换是依赖系统的locale的，并且在运行时完成。考虑这样一种情况，我们在简体中文Windows下编译如下语句：


```
const char* s = "中文abc";
```
根据我们之前的讨论，编译器将按照Windows Codepage936（GB2312）对这个字符串进行编码。如果我们在程序中运行宽窄转换函数，将s转换为宽字符串ws，如果这个程序运行在简体中文环境下是没问题的，将执行从GB2312到UCS-2BE的转换；但是，如果在其他语言环境下，比如是繁体中文BIG5，程序将根据系统的locale执行从BIG5到UCS-2BE的转换，这显然就出现了错误。
**补救**
有没有补救这个问题的办法呢？一个解决方案就是执行不依赖locale的宽窄转换。实际上，这就已经不是宽窄转换之间的问题了，而是编码之间转换的问题了。我们可以用GNU的libiconv实现任意编码间的转换，对于以上的具体情况，指明是从GB2312到UCS-2BE就不会出错。（请参考本人前面的章节：win32下的libiconv），但这显然是一个笨拙的策略：我们在简体中文Windows下必须使用GB2312到UCS-2BE版本的宽窄转换函数；到了BIG5环境下，就必须重新写从BIG5到UCS-2BE的宽窄转换函数。
**Windows 的策略**
Windows的策略是淘汰了窄字符串，干脆只用宽字符串。所有的硬编码全部加上特定宏，比如TEXT()，如果程序是所谓Unicode编译，在编译时就翻译为UCS2-BE——Windows自称为Unicode编程，其本质是使用了UCS-2BE的16位宽字符串。
**Linux 的策略**
Linux下根本就不存在这个问题！因为各种语言的Linux都使用UTF-8的编码，所以，无论系统locale如何变化，窄到宽转换的规则一直是UTF-8到UTF32-BE 。
**跨平台策略**
因为在16位的范围内，UTF32-BE的前16位为0，后16位与UCS2-BE是一样的，所以，即使wchar_t的sizeof()不一样，在一般情况下，跨平台使用宽字符（串）也应该是兼容的。但是依然存在潜在的问题，就是那些4字节的UTF32编码。
**gettext 策略**
以上都是将ASCII及以外的编码硬编码在程序中的办法。GNU的gettext提供了另外一种选择：在程序中只硬编码ASCII，多语言支持由gettext函数库在运行时加载。（对gettext的介绍请参考本人前面的章节：Win32下的GetText）。 gettext的多语言翻译文件不在程序中，而是单独的提出来放在特定的位置。gettext明确的知道这些翻译文件的编码，所以可以准确的告诉给系统翻译的正确信息，而系统将这些信息以当前的系统locale编码成窄字符串反馈给程序。例如，在简体中文Windows中，gettext的po文件也可以以UTF-8储存，gettext将po文件翻译成mo文件，确保mo文件在任何系统和语言环境下都能够正确翻译。在运行是传给win32程序的窄串符合当前locale，是GB2312。gettext让国际化的翻译更加的方便，缺点是目前我没找到支持宽字符串的版本（据说是有ugettext()支持宽字符串），所以要使用gettext只能使用窄字符串。但是gettext可以转换到宽字符串，而且不会出现宽窄转换的问题，因为gettext是运行时根据locale翻译的。例如：


```
const char* s = gettext("Chinese a b c");
```
其中”Chinese a b c”在po中的翻译是”中文abc”
使用依赖locale的运行时宽窄转换函数：


```
const std::wstring wstr = s2ws(s);
```
运行时调用该po文件对应的mo文件，在简体中文环境下就以GB2312传给程序，在繁体中文中就以BIG5传给程序，这样s2ws()总能够正常换算编码。
**更多**
在本文的最后，我想回到C++的stream问题上。用fstream转换如此的简单，sstream却不支持。改造一个支持codecvt的string stream需要改造basic_stringbuf。basic_stringbuf和basic_filebuf都派生自 basic_streambuf，所不同的是basic_filebuf在构造和open()的时候调用了codecvt，只需要在 basic_stringbuf中添加这个功能就可以了。说起来容易，实际上是需要重新改造一个STL模板，尽管这些模板源代码都是在标准库头文件中现成的，但是我还是水平有限，没有去深究了。另外一个思路是构建一个基于内存映射的虚拟文件，这个框架在boost的iostreams库中，有兴趣的朋友可以深入的研究。
（完）
