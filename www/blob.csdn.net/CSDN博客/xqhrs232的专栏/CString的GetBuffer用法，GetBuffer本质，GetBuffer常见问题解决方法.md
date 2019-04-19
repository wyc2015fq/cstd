# CString的GetBuffer用法，GetBuffer本质，GetBuffer常见问题解决方法 - xqhrs232的专栏 - CSDN博客
2015年01月13日 17:51:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：724
原文地址::[http://blog.csdn.net/lewutian/article/details/6787024](http://blog.csdn.net/lewutian/article/details/6787024)
**一．函数原型**
CString::GetBuffer
LPTSTR GetBuffer( int nMinBufLength );
throw( CMemoryException );
**Return Value**
An **LPTSTR **pointer to the object’s (null-terminated) character buffer.
**Parameters**
nMinBufLength
The minimum size of the character buffer in characters. This value does not include space for a null terminator.
**Remarks**
Returns a pointer to the internal character buffer for the **CString **object. The returned **LPTSTR** is not** const**and thus allows direct modification of **CString **contents.
If you use the pointer returned by **GetBuffer** to change the string contents, you must call [ReleaseBuffer](http://blog.csdn.net/lewutian/article/details/6787024)before
 using any other **CString** member functions.
**二．函数作用及使用范围**
对一个CString变量，你可以使用的唯一合法转换符是LPCTSTR，直接转换成非常量指针（LPTSTR-[const] char*）是错误的。正确的得到一个指向缓冲区的非常量指针的方法是调用GetBuffer()方法。
GetBuffer()主要作用是将字符串的缓冲区长度锁定，releaseBuffer则是解除锁定，使得CString对象在以后的代码中继续可以**实现长度自适应增长**的功能。
CString ::GetBuffer有两个重载版本:
LPTSTR GetBuffer( );LPTSTR GetBuffer(int nMinBufferLength);
在第二个版本中，当设定的长度小于原字符串长度时，nMinBufLength = nOldLen，该参数会被忽
略，不分配内存，指向原CString；当设定的长度大于原字符串本身的长度时就要重新分配（reallocate）一块比较大的空间出来。而调用第一个版本时，应如通过传入0来调用第二个版本一样。
是否需要在GetBufer后面调用ReleaseBuffer()，是根据你的后面的程序是否需要继续使用该字符串变量，并且是否动态改变其长度而定的。如果你GetBuffer以后程序自函数就退出，局部变量都不存在了，调用不调用ReleaseBuffer没什么意义了。
最典型的应用就是读取文件：
CFile file;
// FILE_NAME 为实现定义好的文件名称
if(file.Open(FILE_NAME,CFile::modeRead))
{
       CString szContent;
       int nFileLength = file.GetLength();
       file.Read(szContent.GetBuffer(nFileLength),nFileLength);
       szContent.ReleaseBuffer();
       // 取得文件內容放在szContent中，我们之后可以对其操作
}
**三．测试**
以下就CString::GetBuffer，做简单测试：
**测试1：**
// example for CString::GetBuffer
#include <stdio.h>
#include <afx.h>
void main(void)
{    
       CString s( "abcd" );
       printf("(1)before GetBuffer:\n");
       printf("CString s.length=%d\n",s.GetLength());
       printf("CString s=%s\n",s);
       LPTSTR p = s.GetBuffer( 2 );
       printf("(2)after GetBuffer and before ReleaseBuffer:\n");
       printf("LPTSTR p=%s\n",p);    
       printf("p.length=%d\n",strlen(p));
printf("CString s=%s\n",s);      
       printf("CString s.length=%d\n",s.GetLength());
       s.ReleaseBuffer( );
       printf("(3)after ReleaseBuffer:\n");
       printf("LPTSTR p=%s\n",p);    
       printf("p.length=%d\n",strlen(p));
printf("CString s=%s\n",s);
       printf("CString s.length=%d\n",s.GetLength());    
}
**测试结果1：**
(1)before GetBuffer:
CString s.length=4
CString s=abcd
(2)after GetBuffer and before ReleaseBuffer:
LPTSTR p=abcd
p.length=4
CString s=abcd
CString s.length=4
(3)after ReleaseBuffer:
LPTSTR p=abcd
p.length=4
CString s=abcd
CString s.length=4
Press any key to continue
**测试2：**
将LPTSTR p = s.GetBuffer( 2 ); 修改为：LPTSTR p = s.GetBuffer( 10 );
测试结果同1。
**测试3：**
在测试二的LPTSTR p = s.GetBuffer( 10 );后添加  p[5]='f';
测试结果同1。
**测试4：**
将测试三的p[5]='f';修改为p[4]='e';
**测试结果4：**
(1)before GetBuffer:
CString s.length=4
CString s=abcd
(2)after GetBuffer and before ReleaseBuffer:
LPTSTR p=abcde屯屯?
p.length=10
CString s=abcde屯屯?
CString s.length=4
(3)after ReleaseBuffer:
LPTSTR p=abcde屯屯?
p.length=10
CString s=abcde屯屯?
CString s.length=10
Press any key to continue
很显然(2)after GetBuffer and before ReleaseBuffer:中 CString s.length=4结果有问题。
**注意**：以上测试是在_MBCS环境下，如果换成_UNICODE则结果有可能不同。
**参考：**
《CString GetBuffer()》
[http://blog.csdn.net/hbyh/archive/2007/09/15/1786574.aspx](http://blog.csdn.net/hbyh/archive/2007/09/15/1786574.aspx)
《[CString之GetBuffer问题](http://bbs.tongji.net/thread-379834-1-1.html)》
[http://game.tongji.net/thread-379834-1-1.html](http://game.tongji.net/thread-379834-1-1.html)
《CString的GetBuffer》
[http://www.programfan.com/blog/article.asp?id=40755](http://www.programfan.com/blog/article.asp?id=40755)
《CString GetBuffer() and ReleaseBuffer()》
[http://blog.csdn.net/guanchanghui/archive/2006/09/13/1217096.aspx](http://blog.csdn.net/guanchanghui/archive/2006/09/13/1217096.aspx)
《CString::GetBuffer()与CString::ReleaseBuffer到底有什么用？》
[http://topic.csdn.net/t/20060313/22/4612156.html](http://topic.csdn.net/t/20060313/22/4612156.html)

LPTSTR   CString::GetBuffer(int   nMinBufLength) 
{ 
ASSERT(nMinBufLength   > =   0); 
if   (GetData()-> nRefs   >   1   ||   nMinBufLength   >   GetData()-> nAllocLength) 
{ 
#ifdef   _DEBUG 
//   give   a   warning   in   case   locked   string   becomes   unlocked 
if   (GetData()   !=   _afxDataNil   &&   GetData()-> nRefs   <   0) 
TRACE0( "Warning:   GetBuffer   on   locked   CString   creates   unlocked   CString!\n "); 
#endif 
//   we   have   to   grow   the   buffer 
CStringData*   pOldData   =   GetData(); 
int   nOldLen   =   GetData()-> nDataLength;       //   AllocBuffer   will   tromp   it 
if   (nMinBufLength   <   nOldLen) 
nMinBufLength   =   nOldLen; 
AllocBuffer(nMinBufLength); 
memcpy(m_pchData,   pOldData-> data(),   (nOldLen+1)*sizeof(TCHAR)); 
GetData()-> nDataLength   =   nOldLen; 
CString::Release(pOldData); 
} 
ASSERT(GetData()-> nRefs   <=   1); 
//   return   a   pointer   to   the   character   storage   for   this   string 
ASSERT(m_pchData   !=   NULL); 
return   m_pchData; 
} 
void   CString::ReleaseBuffer(int   nNewLength) 
{ 
CopyBeforeWrite();     //   just   in   case   GetBuffer   was   not   called 
if   (nNewLength   ==   -1) 
nNewLength   =   lstrlen(m_pchData);   //   zero   terminated 
ASSERT(nNewLength   <=   GetData()-> nAllocLength); 
GetData()-> nDataLength   =   nNewLength; 
m_pchData[nNewLength]   =   '\0 '; 
}
=============
看了很多人写的程序,包括我自己写的一些代码，发现很大的一部分bug是关于MFC类中的CString的错误用法的.出现这种错误的原因主要是对CString的实现机制不是太了解。
CString是对于原来标准c中字符串类型的一种的包装。因为，通过很长时间的编程，我们发现,很多程序的bug多和字符串有关,典型的有：缓冲溢出、内存泄漏等。而且这些bug都是致命的，会造成系统的瘫痪。因此c++里就专门的做了一个类用来维护字符串指针。标准c++里的字符串类是string，在microsoft MFC类库中使用的是CString类。通过字符串类，可以大大的避免c中的关于字符串指针的那些问题。
这里我们简单的看看Microsoft MFC中的CString是如何实现的。当然，要看原理，直接把它的代码拿过来分析是最好的。MFC里的关于CString的类的实现大部分在strcore.cpp中。
CString就是对一个用来存放字符串的缓冲区和对施加于这个字符串的操作封装。也就是说，CString里需要有一个用来存放字符串的缓冲区，并且有一个指针指向该缓冲区，该指针就是LPTSTR m_pchData。但是有些字符串操作会增建或减少字符串的长度，因此为了减少频繁的申请内存或者释放内存，CString会先申请一个大的内存块用来存放字符串。这样，以后当字符串长度增长时，如果增加的总长度不超过预先申请的内存块的长度，就不用再申请内存。当增加后的字符串长度超过预先申请的内存时，CString先释放原先的内存，然后再重新申请一个更大的内存块。同样的，当字符串长度减少时，也不释放多出来的内存空间。而是等到积累到一定程度时，才一次性将多余的内存释放。
还有，当使用一个CString对象a来初始化另一个CString对象b时，为了节省空间，新对象b并不分配空间，它所要做的只是将自己的指针指向对象a的那块内存空间，只有当需要修改对象a或者b中的字符串时，才会为新对象b申请内存空间，这叫做写入复制技术(CopyBeforeWrite)。
这样，仅仅通过一个指针就不能完整的描述这块内存的具体情况，需要更多的信息来描述。
首先，需要有一个变量来描述当前内存块的总的大小。
其次，需要一个变量来描述当前内存块已经使用的情况。也就是当前字符串的长度
另外，还需要一个变量来描述该内存块被其他CString引用的情况。有一个对象引用该内存块，就将该数值加一。
CString中专门定义了一个结构体来描述这些信息:
struct CStringData
{
 long nRefs;             // reference count
 int nDataLength;        // length of data (including terminator)
 int nAllocLength;       // length of allocation
 // TCHAR data[nAllocLength]
 TCHAR* data()           // TCHAR* to managed data
  { return (TCHAR*)(this+1); }
};
实际使用时，该结构体的所占用的内存块大小是不固定的，在CString内部的内存块头部，放置的是该结构体。从该内存块头部开始的sizeof(CstringData)个BYTE后才是真正的用于存放字符串的内存空间。这种结构的数据结构的申请方法是这样实现的:
pData = (CStringData*) new BYTE[sizeof(CStringData) + (nLen+1)*sizeof(TCHAR)];
pData->nAllocLength = nLen;
其中nLen是用于说明需要一次性申请的内存空间的大小的。
从代码中可以很容易的看出，如果想申请一个256个TCHAR的内存块用于存放字符串，实际申请的大小是：
sizeof(CStringData)个BYTE ＋ (nLen+1)个TCHAR
其中前面sizeof(CstringData)个BYTE是用来存放CstringData信息的。后面的nLen＋1个TCHAR才是真正用来存放字符串的，多出来的一个用来存放’/0’。
 CString中所有的operations的都是针对这个缓冲区的。比如LPTSTR CString::GetBuffer(int nMinBufLength)，它的实现方法是:
首先通过CString::GetData()取得CStringData对象的指针。该指针是通过存放字符串的指针m_pchData先后偏移sizeof(CstringData)，从而得到了CStringData的地址。
然后根据参数nMinBufLength给定的值重新实例化一个CStringData对象，使得新的对象里的字符串缓冲长度能够满足nMinBufLength。
然后在重新设置一下新的CstringData中的一些描述值。C
最后将新CStringData对象里的字符串缓冲直接返回给调用者。
这些过程用C++代码描述就是：
 if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength)
 {
  // we have to grow the buffer
  CStringData* pOldData = GetData();
  int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
  if (nMinBufLength < nOldLen)
   nMinBufLength = nOldLen;
  AllocBuffer(nMinBufLength);
  memcpy(m_pchData, pOldData->data(), (nOldLen+1)*sizeof(TCHAR));
  GetData()->nDataLength = nOldLen;
  CString::Release(pOldData);
 }
 ASSERT(GetData()->nRefs <= 1);
 // return a pointer to the character storage for this string
 ASSERT(m_pchData != NULL);
 return m_pchData;
很多时候，我们经常的对大批量的字符串进行互相拷贝修改等，CString 使用了CopyBeforeWrite技术。使用这种方法，当利用一个CString对象a实例化另一个对象b的时候，其实两个对象的数值是完全相同的，但是如果简单的给两个对象都申请内存的话，对于只有几个、几十个字节的字符串还没有什么，如果是一个几K甚至几M的数据量来说，是一个很大的浪费。
因此CString 在这个时候只是简单的将新对象b的字符串地址m_pchData直接指向另一个对象a的字符串地址m_pchData。所做的额外工作是将对象a的内存应用CStringData:: nRefs加一。
CString::CString(const CString& stringSrc)
{
  m_pchData = stringSrc.m_pchData;
  InterlockedIncrement(&GetData()->nRefs);
}
这样当修改对象a或对象b的字符串内容时，首先检查CStringData:: nRefs的值，如果大于一(等于一，说明只有自己一个应用该内存空间)，说明该对象引用了别的对象内存或者自己的内存被别人应用，该对象首先将该应用值减一，然后将该内存交给其他的对象管理，自己重新申请一块内存，并将原来内存的内容拷贝过来。
其实现的简单代码是：
void CString::CopyBeforeWrite()
{
 if (GetData()->nRefs > 1)
 {
  CStringData* pData = GetData();
  Release();
  AllocBuffer(pData->nDataLength);
memcpy(m_pchData, pData->data(),
  (pData- >nDataLength+1)*sizeof(TCHAR));
 }
}
其中Release 就是用来判断该内存的被引用情况的。
void CString::Release()
{
 if (GetData() != _afxDataNil)
 {
  if (InterlockedDecrement(&GetData()->nRefs) <= 0)
   FreeData(GetData());
 }
}
当多个对象共享同一块内存时，这块内存就属于多个对象，而不在属于原来的申请这块内存的那个对象了。但是，每个对象在其生命结束时，都首先将这块内存的引用减一，然后再判断这个引用值，如果小于等于零时，就将其释放，否则，将之交给另外的正在引用这块内存的对象控制。
CString使用这种数据结构，对于大数据量的字符串操作，可以节省很多频繁申请释放内存的时间，有助于提升系统性能。
通过上面的分析，我们已经对CString的内部机制已经有了一个大致的了解了。总的说来MFC中的CString是比较成功的。但是，由于数据结构比较复杂(使用CStringData)，所以在使用的时候就出现了很多的问题，最典型的一个就是用来描述内存块属性的属性值和实际的值不一致。出现这个问题的原因就是CString为了方便某些应用，提供了一些operations，这些operation可以直接返回内存块中的字符串的地址值，用户可以通过对这个地址值指向的地址进行修改，但是，修改后又没有调用相应的operations1使CStringData中的值来保持一致。比如，用户可以首先通过operations得到字符串地址，然后将一些新的字符增加到这个字符串中，使得字符串的长度增加，但是，由于是直接通过指针修改的，所以描述该字符串长度的CStringData中的nDataLength却还是原来的长度，因此当通过GetLength获取字符串长度时，返回的必然是不正确的。
存在这些问题的operations下面一一介绍。
1. GetBuffer
很多错误用法中最典型的一个就是CString:: GetBuffer ()了.查了MSDN,里面对这个operation的描述是:
 Returns a pointer to the internal character buffer for the CString object. The returned LPTSTR is not const and thus allows direct modification of CString contents。
这段很清楚的说明，对于这个operation返回的字符串指针，我们可以直接修改其中的值:
 CString str1("This is the string 1");――――――――――――――――1
 int nOldLen = str1.GetLength();―――――――――――――――――2
 char* pstr1 = str1.GetBuffer( nOldLen );――――――――――――――3
 strcpy( pstr1, "modified" );――――――――――――――――――――4
 int nNewLen = str1.GetLength();―――――――――――――――――5
通过设置断点，我们来运行并跟踪这段代码可以看出，当运行到三处时，str1的值是”This is the string 1”,并且nOldLen的值是20。当运行到5处时，发现，str1的值变成了”modified”。也就是说，对GetBuffer返回的字符串指针，我们将它做为参数传递给strcpy，试图来修改这个字符串指针指向的地址，结果是修改成功，并且CString对象str1的值也响应的变成了” modified”。但是，我们接着再调用str1.GetLength()时却意外的发现其返回值仍然是20，但是实际上此时str1中的字符串已经变成了”
 modified”,也就是说这个时候返回的值应该是字符串” modified”的长度8！而不是20。现在CString工作已经不正常了！这是怎么回事？
很显然，str1工作不正常是在对通过GetBuffer返回的指针进行一个字符串拷贝之后的。
再看MSDN上的关于这个operation的说明，可以看到里面有这么一段话:
If you use the pointer returned by GetBuffer to change the string contents, you must call ReleaseBuffer before using any other CString member functions.
 原来在对GetBuffer返回的指针使用之后需要调用ReleaseBuffer，这样才能使用其他CString的operations。上面的代码中，我们在4－5处增建一行代码:str2.ReleaseBuffer(),然后再观察nNewLen,发现这个时候已经是我们想要的值8了。
从CString的机理上也可以看出:GetBuffer返回的是CStringData对象里的字符串缓冲的首地址。根据这个地址，我们对这个地址里的值进行的修改，改变的只是CStringData里的字符串缓冲中的值， CStringData中的其他用来描述字符串缓冲的属性的值已经不是正确的了。比如此时CStringData:: nDataLength很显然还是原来的值20，但是现在实际上字符串的长度已经是8了。也就是说我们还需要对CStringData中的其他值进行修改。这也就是需要调用ReleaseBuffer()的原因了。
正如我们所预料的，ReleaseBuffer源代码中显示的正是我们所猜想的:
 CopyBeforeWrite();  // just in case GetBuffer was not called
 if (nNewLength == -1)
  nNewLength = lstrlen(m_pchData); // zero terminated
 ASSERT(nNewLength <= GetData()->nAllocLength);
 GetData()->nDataLength = nNewLength;
 m_pchData[nNewLength] = '/0';
其中CopyBeforeWrite是实现写拷贝技术的，这里不管它。
下面的代码就是重新设置CStringData对象中描述字符串长度的那个属性值的。首先取得当前字符串的长度，然后通过GetData()取得CStringData的对象指针，并修改里面的nDataLength成员值。
但是，现在的问题是，我们虽然知道了错误的原因，知道了当修改了GetBuffer返回的指针所指向的值之后需要调用ReleaseBuffer才能使用CString的其他operations时，我们就能避免不在犯这个错误了。答案是否定的。这就像虽然每一个懂一点编程知识的人都知道通过new申请的内存在使用完以后需要通过delete来释放一样，道理虽然很简单，但是，最后实际的结果还是有由于忘记调用delete而出现了内存泄漏。
实际工作中，常常是对GetBuffer返回的值进行了修改，但是最后却忘记调用ReleaseBuffer来释放。而且，由于这个错误不象new和delete人人都知道的并重视的，因此也没有一个检查机制来专门检查，所以最终程序中由于忘记调用ReleaseBuffer而引起的错误被带到了发行版本中。
要避免这个错误，方法很多。但是最简单也是最有效的就是避免这种用法。很多时候，我们并不需要这种用法，我们完全可以通过其他的安全方法来实现。
比如上面的代码，我们完全可以这样写:
 CString str1("This is the string 1");
 int nOldLen = str1.GetLength();
 str1 = "modified";
 int nNewLen = str1.GetLength();
但是有时候确实需要，比如:
我们需要将一个CString对象中的字符串进行一些转换,这个转换是通过调用一个dll里的函数Translate来完成的，但是要命的是，不知道什么原因，这个函数的参数使用的是char*型的:
DWORD Translate( char* pSrc, char *pDest, int nSrcLen, int nDestLen );
这个时候我们可能就需要这个方法了:
CString strDest;
Int nDestLen = 100;
DWORD dwRet = Translate( _strSrc.GetBuffer( _strSrc.GetLength() ), 
 strDest.GetBuffer(nDestLen),
 _strSrc.GetLength(), nDestlen );
_strSrc.ReleaseBuffer();
strDest.ReleaseBuffer();
if ( SUCCESSCALL(dwRet)  )
{
}
if ( FAILEDCALL(dwRet) )
{
}
的确，这种情况是存在的，但是，我还是建议尽量避免这种用法，如果确实需要使用，请不要使用一个专门的指针来保存GetBuffer返回的值，因为这样常常会让我们忘记调用ReleaseBuffer。就像上面的代码，我们可以在调用GetBuffer之后马上就调用ReleaseBuffer来调整CString对象。
2. LPCTSTR
关于LPCTSTR的错误常常发生在初学者身上。
例如在调用函数
DWORD Translate( char* pSrc, char *pDest, int nSrcLen, int nDestLen );
时，初学者常常使用的方法就是:
int nLen = _strSrc.GetLength();
DWORD dwRet = Translate( (char*)(LPCTSTR)_strSrc), 
 (char*)(LPCTSTR)_strSrc),
 nLen,
 nLen);
if ( SUCCESSCALL(dwRet)  )
{
}
if ( FAILEDCALL(dwRet) )
{
}
他原本的初衷是将转换后的字符串仍然放在_strSrc中，但是，当调用完Translate以后之后再使用_strSrc时，却发现_strSrc已经工作不正常了。检查代码却又找不到问题到底出在哪里。
其实这个问题和第一个问题是一样的。CString类已经将LPCTST重载了。在CString中LPCTST实际上已经是一个operation了。对LPCTST的调用实际上和GetBuffer是类似的，直接返回CStringData对象中的字符串缓冲的首地址。
其C++代码实现是:
_AFX_INLINE CString::operator LPCTSTR() const
 { return m_pchData; }
因此在使用完以后同样需要调用ReleaseBuffer()。
但是，这个谁又能看出来呢?
其实这个问题的本质原因出在类型转换上。LPCTSTR返回的是一个const char*类型，因此使用这个指针来调用Translate编译是不能通过的。对于一个初学者，或者一个有很长编程经验的人都会再通过强行类型转换将const char*转换为char*。最终造成了CString工作不正常，并且这样也很容易造成缓冲溢出。
通过上面对于CString机制和一些容易出现的使用错误的描述，可以使我们更好的使用CString。
=======================
**CString str = "abcde\0cde";输出字符串的值为: abcde**
而字符串的长度为 s.GetLength() 的值为: 5
这是因为CString对象在赋值时只检查到'\0',后面的忽略了, 也就是说实际对象str内容为"abcde".
而str真正的存储空间为6(字符串以'\0'结尾).
所以说在字符长度和实际的空间是不一样的. 好!别跑!
请看下面有趣的程序:
CString str = "hello";
LPSTR pf = (LPSTR)(LPCSTR)s;
LPSTR pa = s.GetBuffer(0);
        你可以测得 pf == pa;
LPSTR pb = s.GetBuffer(10);
        你可以测得 pf != pb;
为什么:
我们都知道(LPSTR)(LPCSTR)s 实际指向对象str的实际字符串的内存地址, GetBuffer() 函数中的参数(其实就是重新申请的字符串的长度)如果小于等于先前的字符串长度, 则不会重新分配内存使用原来的内存所以 pf == pa, 如果大于先前的字符串长度, 则重新追加内存(也就是要复制原来的内容),
所以pf != pb.
注意GetBuffer()函数中的参数为重新申请的字符串的长度, 实际内存的大小应再加1.
CString s = "hello";
LPSTR pf = s.GetBuffer(0);
strcpy(pf,"hi");
这时对象str 的内容为 "hi"
但是s.GetLength()的值为5, 如果加上一条语句:
s.ReleaseBuffer();
则s.GetLength()的值为2
解释: 
CString对象在内存中用一个计数器来维持可用缓冲区的大小
void ReleaseBuffer( int nNewLength = -1 )
     {
          if( nNewLength == -1 )
          {
               nNewLength = StringLength( m_pszData );
          }
          SetLength( nNewLength );
     }
很明显ReleaseBuffer的作用就是更新字符串的长度。 CString内，GetLength获取字符串长度并不是动态计算的，而是在赋值操作后计算并保存在一个int变量内的，当通过GetBuffer直接修改CString时，那个int变量并不可能自动更新，于是便有了ReleaseBuffer.
CString s = "hello";
LPSTR pf = s.GetBuffer(0);
strcpy(pf,"hi");
LPSTR ps =  (LPSTR)(LPCSTR)s;    字符串缓冲区的首地址
*(ps+2) = 'x';
  则字符串的实际内容为:    "hixlo"
*(ps+6) = 'a';        出错, 因为对象s的实际空间为 6
而
CString s = "hello";
LPSTR pf = s.GetBuffer(10);
strcpy(pf,"hi");
LPSTR ps =  (LPSTR)(LPCSTR)s;    字符串缓冲区的首地址
*(ps+2) = 'x';
*(ps+5)= '\0';
 则字符串的实际内容还是为:    "hixlo"
*(ps+6) = 'a';         可以因为s对象的实际空间为11
说白了  ReleaseBuffer就是更新赋值之后的字符串的长度, 而实际空间没有根本的变化, GetBuffer才是使内存空间大小变化的罪魁祸首.
 //=============================================================================================================
备注::
1》MFC下的CString类好用，但用的不好真的是各种看不明白的异常都会出来！！！

