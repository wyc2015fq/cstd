# 一个简单的string类，读书看报系列(一) - weixin_33985507的博客 - CSDN博客
2013年06月28日 18:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
对于这个类，写过程序的都知道应该含有的方法是
初始化、销毁、拼接、求长度、清除、判断是否为空等、还有一些操作符重载
# 一、先看初始化：
可以想到应该有默认构造的的、带有字符串的、带有默认字符的、还有一个拷贝构造函数
string()
string(const char * sc, intlength);
string(const char * sc);
string(char c);
string(const string& s);
~string();
我们可以分别写5个initialize函数，去对应这些构造函数
```cpp
void initialize()  { data = emptystr; }
    void initialize(const char*, int);
    void initialize(const char*);
    void initialize(char);
    void initialize(const string& s);
```
# 二、销毁
~string()
写一个函数finalize，让其在销毁的时候执行
# 三、成员变量
很简单就一个char*data就足够了，写成protected类型的
# 四、看看他的实现
写一个myString.cpp去对应这个ptypes.h
```cpp
void string::initialize(const char* sc)
{
	initialize(sc, hstrlen(sc));
}
void string::initialize(const char* sc, int initlen)
{
	if (initlen <= 0 || sc == nil)
	{
		data = emptystr;
	}
	else
	{
		_alloc(initlen);
		memmove(data, sc, initlen);
	}
}
void string::initialize(char c)
{
	_alloc(1);
	data[0] = c;
}
void string::initialize(const string& s)
{
	data = s.data;
	pincrement(&STR_REFCOUNT(data));
}
```
那么现在构造函数算是完成了
看看里面，动态分配内存的函数(这里也是string的成员函数)
```cpp
void _alloc(int);
void _realloc(int);
void _free(int);
```
从上面上看用处显而易见
```cpp
void string::_alloc(int numchars)
{
	if (numchars <= 0)
	{
		stringoverflow();
	}
	size_t a = quantize(numchars);
#ifdef DEBUG
	stralloc += a;
#endif
	data = (char*)(memalloc(a)) + strrecsize;
	STR_LENGTH(data) = numchars;
	STR_REFCOUNT(data) = 1;
	data[numchars] = 0;
}
```
```cpp
void string::_realloc(int numchars)
{
 	 if (numchars <= 0 || STR_LENGTH(data) <= 0)
 	 {
		 stringoverflow();
 	 }
	 int a = quantize(numchars);
	 int b = quantize(STR_LENGTH(data));
	 if (a != b)
	 {
#ifdef DEBUG
		 stralloc += a - b;
#endif
		 data = (char*)(memrealloc(data - strrecsize, a)) + strrecsize;
	 }
	 STR_LENGTH(data) = numchars;
	 data[numchars] = 0;
}
```
```cpp
void string::_free(int numchars)
{
	_freestrbuf(data);
	data = emptystr;
}
```
具体到细节里面有可以在框架接口common.h中加入内存处理的函数：
```cpp
//
// memory management (undocumented)
// hides some BSD* incompatibility issues
//
ptpublic void* ptdecl memalloc(uint a);
ptpublic void* ptdecl memrealloc(void* p, uint a);
ptpublic void  ptdecl memfree(void* p);
ptpublic void  ptdecl memerror();
ptpublic int   ptdecl memquantize(int);
ptpublic int   __PFASTCALL pincrement(int* target);
ptpublic int   __PFASTCALL pdecrement(int* target);
```
咱们用pmem.cpp去实现这里的函数
```cpp
#include "stdio.h"
#include "stdlib.h"
#include "common.h"
// dynamic reallocation policy for strings and lists
int ptdecl memquantize(int a)
{
……
}
void* ptdecl memalloc(uint a)
{
……
}
void  ptdecl memerror()
{
……
}
void* ptdecl memrealloc(void* p, uint a)
{
……
}
void  ptdecl memfree(void* p)
{
……
}
```
下来是string应该实现的操作：比如指派assgin函数
```cpp
void assign(const char* sc, int initlen);
void assign(const char* sc);
void assign(char c);
void assign(const string& s);
```
这个函数只需要和构造函数和_alloc这些函数配合起来使用就行
下来是重载操作符：
```cpp
string operator+ (const char* sc) const;
string operator+ (char c) const;
string operator+ (const string& s) const;
bool operator== (const char* sc)  const		{ return (strcmp(sc, data) == 0); }
bool operator== (char c) const;
bool operator== (const string& s) const;
bool operator!= (const char* sc)  const		{ return !(*this == sc); }
bool operator!= (char c) const			{ return !(*this == c);  }
bool operator!= (const string& s) const		{ return !(*this == s); }
operator const char*() const			{ return data; }
```

[]操作符是，因为都需要进行条件判断，所以写成一个函数idx，进行长度的检查
```cpp
void idx(int index) const 	 	{ if(unsigned(index) >= unsigned(STR_LENGTH(data))) idxerror(); }
char&	operator[] (int i)	 	{ idx(i); return unique(*this)[i]; }
const char& operator[] (int i) const 	{ idx(i); return data[i]; }
```

还有一类的操作符重载是友元函数类型的：
```cpp
friend bool operator== (const char* sc, const string& s);
friend bool operator== (char c, const string& s);
friend bool operator!= (const char* sc, const string& s);
friend bool operator!= (char c, const string& s);
```
因为他的实现很简单，只是调用原来已经被重载的=号操作符
```cpp
inline bool operator== (const char* sc, const string& s)		{ return s == sc; }
inline bool operator== (char c, const string& s)			{ return s == c; }
inline bool operator!= (const char* sc, const string& s)		{ return s != sc; }
inline bool operator!= (char c, const string& s)			{ return s != c; }
```
还有一类是需要导出的，所以前面跟上stdcall
```cpp
ptpublic friend string operator+(const char* sc, const string& s);
ptpublic friend string operator+(char c, const string& s);
```
假如你看一下他的调用方式，你就应该知道他为何这么写了：
```cpp
// test ptpublic friend string operator+(const char* sc, const string& s);
s2 = "str" + s1 + "ing";
// test ptpublic friend string operator+(char c, const string& s);
s2 = 's' + s1;
```
下来是类的友元函数，他们不需要进行外部调用，但是需要访问类的成员：
```cpp
friend int length(const string& s);
friend int refcount(const string& s);
friend void clear(string& s);
friend char* ptdecl unique(string& s);
friend int pos(const string& s1, const string& s);
friend void assgin(string& s, const char* buf, int len);
```
这些可以写得很简单：
```cpp
inline int length(const string& s)						{ return STR_LENGTH(s.data); }
inline int refcount(const string& s)						{ return STR_REFCOUNT(s.data); }
inline void assgin(string& s, const char* buf, int len)			        { s.assign(buf, len); }
inline void clear(string& s)							{ s.finalize(); }
inline bool isempty(const string& s)						{ return length(s) == 0; }
inline int  pos(const string& s1, const string& s)				{ return pos(s1.data, s); }
```
下面的这些函数需要被外部调用，又需要访问成员变量的：
```cpp
ptpublic friend int    ptdecl pos(const char* s1, const string& s);
ptpublic friend int    ptdecl pos(char s1, const string& s);
ptpublic friend int    ptdecl rpos(char s1, const string& s);
ptpublic friend void   ptdecl concat(string& s, const char* sc, int catlen);
ptpublic friend void   ptdecl concat(string& s, const char  c);
ptpublic friend void   ptdecl concat(string& s, const string& s1);
ptpublic friend string ptdecl copy(const string& s, int from, int cnt);
ptpublic friend string ptdecl copy(const string& s, int from);
ptpublic friend char*  ptdecl setlength(string& s, int newlen);
ptpublic friend void   ptdecl del(string& s, int at, int cnt);
ptpublic friend void   ptdecl del(string& s, int from);
ptpublic friend void   ptdecl ins(const char* s1, int s1len, string& s, int at);
ptpublic friend void   ptdecl ins(const char* s1, string& s, int at);
ptpublic friend void   ptdecl ins(char s1, string& s, int at);
ptpublic friend void   ptdecl ins(const string& s1, string& s, int at);
ptpublic friend bool   ptdecl contains(const char* s1, int s1len, const string& s, int at);
ptpublic friend bool   ptdecl contains(const char* s1, const string& s, int at);
ptpublic friend bool   ptdecl contains(char s1, const string& s, int at);
ptpublic friend bool   ptdecl contains(const string& s1, const string& s, int at);
```
咱们可以另外写一个pstrmanip.cpp去实现上面的函数，其实还可以把一些操作符重载也写在这里：
```cpp
#include "stdlib.h"
#include "ptypes.h"
string string::operator+ (const char* sc) const
{
}
string string::operator+ (char c) const
{
}
string string::operator+ (const string& s) const
{
}
string operator+(const char* sc, const string& s)
{
}
string operator+(char c, const string& s)
{
}
bool string::operator== (char c) const
{
}
bool string::operator== (const string& s) const
{
}
int ptdecl pos(const char* sc, const string& s)
{
}
int ptdecl pos(char c, const string& s)
{
}
int ptdecl rpos(char c, const string& s)
{
}
string ptdecl copy(const string& s, int from, int cnt)
{
}
string ptdecl copy(const string& s, int from)
{
}
void ptdecl del(string& s, int from, int cnt)
{
}
void ptdecl del(string& s, int from)
{
}
void ptdecl ins(const char* s1, int s1len, string& s, int at)
{
}
void ptdecl ins(const char* s1, string& s, int at)
{
}
void ptdecl ins(char s1, string& s, int at)
{
}
void ptdecl ins(const string& s1, string& s, int at)
{
}
bool ptdecl contains(const char* s1, int s1len, const string& s, int at)
{
}
bool ptdecl contains(const char* s1, const string& s, int at)
{
}
bool ptdecl contains(char s1, const string& s, int at)
{
}
bool ptdecl contains(const string& s1, const string& s, int at)
{
}
```
这样整个类就被分开了，但是有个缺点。。用助手不好使了= = 
