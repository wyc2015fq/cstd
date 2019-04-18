# 移动开发之浅析cocos2d-x的中文支持问题 - tkokof1的专栏 - CSDN博客

2012年04月04日 22:35:01[tkokof1](https://me.csdn.net/tkokof1)阅读数：27886标签：[移动开发																[facet																[null																[alignment																[跨平台																[translation](https://so.csdn.net/so/search/s.do?q=translation&t=blog)](https://so.csdn.net/so/search/s.do?q=跨平台&t=blog)](https://so.csdn.net/so/search/s.do?q=alignment&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=facet&t=blog)](https://so.csdn.net/so/search/s.do?q=移动开发&t=blog)
个人分类：[游戏																[随性](https://blog.csdn.net/tkokof1/article/category/642708)](https://blog.csdn.net/tkokof1/article/category/642710)


移动开发之浅析cocos2d-x的中文支持问题

  题记：这阵子一直在学习cocos2d-x，其跨平台的特性确实让人舒爽，引擎的框架概念也很成熟，虽然相应的第三方工具略显单薄，但也无愧是一件移动开发的利器啊，有兴趣的朋友有时间就多[了解](#_top)一下吧 ：）

  使用引擎的过程中遇到的问题很多，中文支持便是一例，虽然问题本身并不复杂，但是网上的相关资料都比较简单零散，自己捣鼓了几下，觉得有必要的整理一下，以此稍稍方便一下遇到相似问题的朋友，也算是自己的一点点学习总结，可惜自己水平有限，讲的不当甚至错误的地方请细心的朋友不吝指出，俗话说的好嘛：贻笑大方事小，误人子弟事大也 ：）

  一. 不就是用用setlocale嘛，简单！

  之前自己并未深入过C/C++对于多字符编码的支持问题，但也算是尝试过一些相关的示例程序，譬如这个中文版的Hello，World：

#include <cstdio>

#include <clocale>

using namespace std;

int main()

{

    // first we have to set the locale

    setlocale(LC_ALL, "chs");

    // then we use wprintf to output the Chinese string

    wprintf(L"%ls\n", L"世界，你好");

    // that's all :)

    return 0;

}

  程序的逻辑很简单，使用setlocale设置地域信息，然后使用wprintf输出对应的中文信息，一切似乎都非常简单自然。（注：如果使用GCC编译上述代码时，需要将源文件类型改为UTF-8格式，否则会提示解码错误，这是GCC的一个源文件限制）

  类似的，这里也有一个相同功能的C++程序：

#include <iostream>

using namespace std;

int main()

{

    // first we have to set the output stream locale

    wcout.imbue(locale("chs"));

    // then we use wcout to output the Chinese string

    wcout << L"世界，你好" << endl;

    // that's all :)

    return 0;

}

  代码的原理部分与之前的代码如出一辙，在此不再赘述，不过比较令我费解的是这段代码在CodeBlocks+MinGW4.4.1环境下，运行时会引起崩溃（由wcout.imbue(locale("chs"))这行代码引起），具体原因没有细究，似乎是GCC的一个BUG，如有朋友了解，请不吝告知，多谢 ：）

  关于以上问题的更多的一些信息可以在[这里](http://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html)找到。

  好了，小小尝试了一下之后，就让我们开始整一整cocos2d-x中的中文了，巧的是cocos2d-x开发包（有兴趣的朋友可以从[这里](http://www.cocos2d-x.org/projects/cocos2d-x/wiki/Download)下载）的示例程序中恰好也有一个HelloWorld，那么事不宜迟，就让我们将这个程序修改为中文界面吧 ：）

  二. char 或者 wchar_t，这是一个问题

  首先让我们来看看cocos2d-x中原始的HelloWorld里涉及到的文本输出代码是哪些（如果你从未接触过cocos2d，推荐先熟悉一下其中的基本概念，网上优秀的入门教程很多，譬如[这里](http://www.cnblogs.com/andyque/)）：

 /////////////////////////////

// 3. add your codes below...

// add a label shows "Hello World"

// create and initialize a label

    CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Hello World", "Arial", 24);

// ask director the window size

CCSize size = CCDirector::sharedDirector()->getWinSize();

// position the label on the center of the screen

pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

// add the label as a child to this layer

this->addChild(pLabel, 1);

  注释都很明了，大致意思便是创建一个CCLabelTTF，然后设置对应位置，并加入场景的child队列，就这样~~~在此给张示意例图：

![](https://img-my.csdn.net/uploads/201204/04/1333550195_8714.jpg)

  OK，到此我们的当前任务已经非常明显了，就是要将图中所示的“Hello World”改为“你好世界”，就是这么简单 ：）

  简单，让我们首先来一次最直接的修改：

    /////////////////////////////

// 3. add your codes below...

// add a label shows "Hello World"

// create and initialize a label

    CCLabelTTF* pLabel = CCLabelTTF::labelWithString(L"你好世界", "Arial", 24);

// ask director the window size

CCSize size = CCDirector::sharedDirector()->getWinSize();

// position the label on the center of the screen

pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

// add the label as a child to this layer

this->addChild(pLabel, 1);

  想法虽然美好，可惜编译器无情的抱怨了：没有对应的调用接口……让我们来看一下labelWithString这个静态函数的声明：

  /** creates a CCLabelTTF from a fontname, alignment, dimension and font size */

  static CCLabelTTF * labelWithString(const char *label, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);

  /** creates a CCLabelTTF from a fontname and font size */

  static CCLabelTTF * labelWithString(const char *label, const char *fontName, float fontSize);

  我们暂时不用细究labelWithString的第一个重载版本，其实现了字体显示的更多细节控制，我们目前关心的是在源程序中我们调用的第二个版本，非常明显的一点便是其第一个文本参数，是一个常量char指针，而我们之前做的简单修改，实际上是传入了一个常量wchar_t指针，自然便出现了重载函数不存在的情况……那么接下来我们应该如何做呢？将wchar_t强制转换为char类型，恐怕没有这么简单，考虑ASCII字符A，其相应的wchar_t内容编码可能是这样的0x00|0x41，将其强制转换为char之后，其相应内容其实并没有改变，传给labelWithString只是相当于一个空字符串而已……那么让我们来修改labelWithString以让他支持wchar_t的调用或者自己动手扩展类似的接口，诚然，以上方案都是可行的，但是都不是那么简洁……

  唉，char 或者 wchar_t，这是一个问题啊~~~

  三. 拥抱UTF-8

  其实，我们还有更简单的方案，那就是使用UTF-8编码，在此让我简单的引用一段维基百科上关于UTF-8的简介：

  UTF-8（8-bit Unicode Transformation Format）是一种针对[Unicode](http://zh.wikipedia.org/wiki/Unicode)的可变长度[字符编码](http://zh.wikipedia.org/wiki/%E5%AD%97%E7%AC%A6%E7%BC%96%E7%A0%81)（[定长码](http://zh.wikipedia.org/w/index.php?title=%E5%AE%9A%E9%95%BF%E7%A0%81&action=edit&redlink=1)），也是一种[前缀码](http://zh.wikipedia.org/w/index.php?title=%E5%89%8D%E7%BC%80%E7%A0%81&action=edit&redlink=1)。它可以用来表示Unicode标准中的任何字符，且其编码中的第一个[字节](http://zh.wikipedia.org/wiki/%E5%AD%97%E8%8A%82)仍与[ASCII](http://zh.wikipedia.org/wiki/ASCII)相容，这使得原来处理ASCII字符的[软件](http://zh.wikipedia.org/wiki/%E8%BB%9F%E9%AB%94)无须或只须做少部份修改，即可继续使用。 

  看到了UTF-8编码的重要特性之一吗：与ASCII兼容！这就是我们想要的 ：） 那么事不宜迟，让我们马上动手将L“你好世界”转换为UTF-8编码，并传入labelWithString试一试吧：

  等等，在转码之前也许你会问：不管UTF-8本身的编码方式如何，之前我们硬编码进程序中的L“你好世界”这几个字符原来是什么编码呢？如果我们连原始编码都不了解，谈何转码一说呢？呵呵，你说的非常正确！只可惜关于C++中使用什么编码方式存储多字节字符并没有统一规定，各个编译器期间都有区别，可能是UTF-16，可能是UTF-32，当然，也有可能是UTF-8（这种情况下强制转换就可行了，尽管仍然很Ugly……），不过幸运的是，Win32的API ：WideCharToMultiByte为我们屏蔽了这些复杂性，让我们马上来试一试：

inline std::string WideByte2UTF8(const wstring& text)

{

int asciisize = ::WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.size(), NULL, 0, NULL, NULL);

if (asciisize == ERROR_NO_UNICODE_TRANSLATION ||

asciisize == 0)  

{  

return string();

}

char* resultstring = new char[asciisize];

int convresult = ::WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.size(), resultstring, asciisize, NULL, NULL);

if (convresult != asciisize)

{  

return string();

}  

std::string buffer(resultstring, convresult);

delete[] resultstring;

return buffer;

}

// add a label shows "Hello World"

// create and initialize a label

std::string text = WideByte2UTF8(L"你好世界");

CCLabelTTF* pLabel = CCLabelTTF::labelWithString(text.c_str(), "Arial", 24);

  哈哈，我们的第一步尝试成功了，来看一下截图：

![](https://img-my.csdn.net/uploads/201204/04/1333550202_5035.jpg)

  四. 还有其他更好的法子吗？

  上面的代码虽然可以完成任务，但是用到了平台特有的API，对于像cocos2d-x这般旨在跨平台的引擎而言，其实并不能算作很好的解决方案，那么我们还有什么其他法子吗？

  1.wcstombs/mbstowcs怎么样？

  其实C函数库中存在类似于WideCharToMultiByte的函数，他们就是wcstombs/mbstowcs，让我们马上来试一试：

//! convert wide string to string

inline std::string WStr2Str(const std::wstring& ws)

{

std::string curLocale = setlocale(LC_ALL, NULL); // get current locale

setlocale(LC_ALL, "chs");

const wchar_t* tSource = ws.c_str();

size_t tDsize = 2 * ws.size() + 1;

char* tDest = new char[tDsize];

memset(tDest,0,tDsize);

wcstombs(tDest,tSource,tDsize);

std::string result = tDest;

delete[] tDest;

setlocale(LC_ALL, curLocale.c_str());

return result;

}

// add a label shows "Hello World"

// create and initialize a label

std::string text = WStr2Str(L"你好世界");

CCLabelTTF* pLabel = CCLabelTTF::labelWithString(text.c_str(), "Arial", 24);

  不幸的是以上方案失败了：

![](https://img-my.csdn.net/uploads/201204/04/1333550208_4500.jpg)

  失败的原因其实很简单，wcstombs是根据当前设置的locale进行转换，由于此处我们设置的locale是“chs”，所以装换的过程是根据GBK编码进行的（就简体中文而言），并非是我们理想中的UTF-8，自然得不到我们期望的结果，那么为什么我们此处不设置locale为UTF-8呢，这是因为：Windows CRT 不支持~~~（唉……），但是我们至此可以找到一个这种方案：通过条件编译实现对应函数的装换，即在Win32平台上调用WideCharToMultiByte，其他支持UTF-8 locale的平台则调用wcstombs，嗯哼，貌似这个方案部分做到了跨平台的特性（对于那些不支持UTF-8 locale的非Win32平台还是不行~~~） ：）

  2. Boost提供的 [utf8_codecvt_facet](http://www.boost.org/doc/libs/1_37_0/libs/serialization/doc/codecvt.html) 怎么样？

  Boost中提供的[utf8_codecvt_facet](http://www.boost.org/doc/libs/1_37_0/libs/serialization/doc/codecvt.html)可以完成类似的UTF-8编码转换，可惜作为标准IO库中codecvt 的一个扩展，与string/wstring协作时并不是那么直观，有兴趣的朋友可以试上一试，个人感觉不是特别好 ：（

  3. 第三库如何？

  其实cocos2d-x的开发包内置了用于编码转换的[iconv](http://www.gnu.org/software/libiconv/)库，我们可以尝试一下，当然，更加重量级的还有[ICU](http://site.icu-project.org/)，如果你有兴趣的也可以研习一番，在此我们便就近试一下iconv吧：

bool IConvConvert(const char *from_charset, const char *to_charset, const char *inbuf, int inlen, char *outbuf, int outlen) 

{

    iconv_t cd = iconv_open(to_charset, from_charset);

if (cd == 0) return false;

const char **pin = &inbuf;

char **pout = &outbuf;

memset(outbuf,0,outlen);

size_t ret = iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);

iconv_close(cd);

return ret == (size_t)(-1) ? false : true;

}

std::string IConvConvert_GBKToUTF8(const std::string& str)

{

const char* textIn = str.c_str();

char textOut[256];

bool ret = IConvConvert("gb2312", "utf-8", textIn, strlen(textIn),textOut, 256);

return ret ? string(textOut) : string();

}

// add a label shows "Hello World"

// create and initialize a label

std::string text = IConvConvert_GBKToUTF8("你好世界");

CCLabelTTF* pLabel = CCLabelTTF::labelWithString(text.c_str(), "Arial", 24);

  哈哈，这次我们成功了：

![](https://img-my.csdn.net/uploads/201204/04/1333550202_5035.jpg)

  不过iconv的GNU许可倒是值得考量一下，对于一些用户可能产生困扰，当然如果考虑清楚，并且程序规模不存在问题时，我们还是推荐iconv的，否则cocos2d-x也不会自带iconv了 ：）

  4. 自给自足，丰衣足食

  虽然软件行业从不提倡重造车轮，但是如果你还不满意上述的各类方法，那么这也算是一条路子了，优点是够轻量、够可控等等；缺点同样明显，很脆弱、花时间等等，当然如果你确定考虑清楚了，那就放手干吧，这里给个示例示例：

//! convert from wstring to UTF8 using self-coding-converting

inline void WStrToUTF8(std::string& dest, const wstring& src){

dest.clear();

for (size_t i = 0; i < src.size(); i++){

wchar_t w = src[i];

if (w <= 0x7f)

dest.push_back((char)w);

else if (w <= 0x7ff){

dest.push_back(0xc0 | ((w >> 6)& 0x1f));

dest.push_back(0x80| (w & 0x3f));

}

else if (w <= 0xffff){

dest.push_back(0xe0 | ((w >> 12)& 0x0f));

dest.push_back(0x80| ((w >> 6) & 0x3f));

dest.push_back(0x80| (w & 0x3f));

}

else if (sizeof(wchar_t) > 2 && w <= 0x10ffff){

dest.push_back(0xf0 | ((w >> 18)& 0x07)); // wchar_t 4-bytes situation

dest.push_back(0x80| ((w >> 12) & 0x3f));

dest.push_back(0x80| ((w >> 6) & 0x3f));

dest.push_back(0x80| (w & 0x3f));

}

else

dest.push_back('?');

}

}

//! simple warpper

inline std::string WStrToUTF8(const std::wstring& str){

std::string result;

WStrToUTF8(result, str);

return result;

}

// add a label shows "Hello World"

// create and initialize a label

std::string text = WStrToUTF8(L"你好世界");

CCLabelTTF* pLabel = CCLabelTTF::labelWithString(text.c_str(), "Arial", 24);

  情况OK ：）

![](https://img-my.csdn.net/uploads/201204/04/1333550202_5035.jpg)

  5. 还有其他法子？

目前我能想到的法子就这么多了，什么，你还有其他法子，好吧，尽量晒出来吧，让我也开开眼界 ：）

五. 最后说几句

  Cocos2d-x的中文支持到此也算聊完了，有兴趣的朋友可以看看，以上内容也算是自己的一些学习历程，在此一一列出，希望能给朋友一些借鉴 ：）

  That's All ：）

部分参考（排名部分先后）：

1.[http://stackoverflow.com/questions/148403/utf8-to-from-wide-char-conversion-in-stl](http://stackoverflow.com/questions/148403/utf8-to-from-wide-char-conversion-in-stl)

2.[http://www.boost.org/doc/libs/1_48_0/libs/serialization/doc/codecvt.html](http://www.boost.org/doc/libs/1_48_0/libs/serialization/doc/codecvt.html)

3.[http://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html](http://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html)

4.[http://www.gnu.org/software/libiconv/](http://www.gnu.org/software/libiconv/)

5.[http://site.icu-project.org/](http://site.icu-project.org/)

6.[http://blog.csdn.net/sjy88813/article/details/6662879](http://blog.csdn.net/sjy88813/article/details/6662879)

7.[http://www.linuxquestions.org/questions/programming-9/wstring-utf8-conversion-in-pure-c-701084/](http://www.linuxquestions.org/questions/programming-9/wstring-utf8-conversion-in-pure-c-701084/)

8.[http://zh.wikipedia.org/wiki/UTF-8](http://zh.wikipedia.org/wiki/UTF-8)


