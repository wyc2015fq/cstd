# CString 成员函数用法大全 - xqhrs232的专栏 - CSDN博客
2013年06月08日 22:23:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：670
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/Caiqinghua/archive/2009/02/16/1391190.html](http://www.cnblogs.com/Caiqinghua/archive/2009/02/16/1391190.html)
相关网帖
1、[VC:CString用法整理](http://www.cnblogs.com/gwazy/archive/2008/01/29/1057638.html)----[http://www.cnblogs.com/gwazy/archive/2008/01/29/1057638.html](http://www.cnblogs.com/gwazy/archive/2008/01/29/1057638.html)
**CString的构造函数**
CString( );
例：CString csStr;
CString( const CString& stringSrc );
例：CString csStr("ABCDEF中文123456");
    CString csStr2(csStr);
CString( TCHAR ch, int nRepeat = 1 );
例：CString csStr('a',5);
//csStr="aaaaa"
CString( LPCTSTR lpch, int nLength );
例：CString csStr("abcdef",3);
//csStr="abc"
CString( LPCWSTR lpsz );
例：wchar_t s[]=L"abcdef";
    CString csStr(s);
//csStr=L"abcdef"
CString( const unsigned char* psz );
例：const unsigned char s[]="abcdef";
    const unsigned char* sp=s;
    CString csStr(sp);
//csStr="abcdef"
CString( LPCSTR lpsz );
例：CString csStr("abcdef");
//csStr="abcdef"
**int GetLength( ) const;**
返回字符串的长度，不包含结尾的空字符。
例：csStr="ABCDEF中文123456";
    printf("%d",csStr.GetLength());       //16
**void MakeReverse( );**
颠倒字符串的顺序
例：csStr="ABCDEF中文123456";
    csStr.MakeReverse();
    cout<<csStr;                  //654321文中FEDCBA
void MakeUpper( );
将小写字母转换为大写字母
例：csStr="abcdef中文123456";
    csStr.MakeUpper();
    cout<<csStr;                  //ABCDEF中文123456
**void MakeLower( );**
将大写字母转换为小写字母
例：csStr="ABCDEF中文123456";
    csStr.MakeLower();
    cout<<csStr;                  //abcdef中文123456
**int Compare( LPCTSTR lpsz ) const;**
区分大小写比较两个字符串，相等时返回0，大于时返回1，小于时返回-1
例：csStr="abcdef中文123456";
    csStr2="ABCDEF中文123456";
    cout<<csStr.CompareNoCase(csStr2);             //0
**int CompareNoCase( LPCTSTR lpsz ) const;**
不区分大小写比较两个字符串，相等时返回0，大于时返回1，小于时返回-1
例：csStr="abcdef中文123456";
    csStr2="ABCDEF中文123456";
    cout<<csStr.CompareNoCase(csStr2);             //-1
**int Delete( int nIndex, int nCount = 1 )**
删除字符，删除从下标nIndex开始的nCount个字符
例：csStr="ABCDEF";
    csStr.Delete(2,3);
    cout<<csStr;              //
 ABF
//当nIndex过大，超出对像所在内存区域时，函数没有任何操作。
//当nIndex为负数时，从第一个字符开始删除。
//当nCount过大，导致删除字符超出对像所在内存区域时，会发生无法预料的结果。
//当nCount为负数时，函数没有任何操作。
**int Insert( int nIndex, TCHAR ch )int Insert( int nIndex, LPCTSTR pstr )**
在下标为nIndex的位置，插入字符或字符串。返回插入后对象的长度
例：csStr="abc";
    csStr.Insert(2,'x');
    cout<<csStr;                     //abxc
    csStr="abc";
    csStr.Insert(2,"xyz");
    cout<<csStr;                     //abxyzc
//当nIndex为负数时，插入在对象开头
//当nIndex超出对象末尾时，插入在对象末尾
**int Remove( TCHAR ch );**
移除对象内的指定字符。返回移除的数目
例：csStr="aabbaacc";
    csStr.Remove('a');
    cout<<csStr;                     //bbcc
**int Replace( TCHAR chOld, TCHAR chNew );int Replace( LPCTSTR lpszOld, LPCTSTR lpszNew );**
替换字串
例：csStr="abcdef";
    csStr.Replace('a','x');
    cout<<csStr;                    //xbcdef
    csStr="abcdef";
    csStr.Replace("abc","xyz");
    cout<<csStr;                    //xyzdef
**void TrimLeft( );void TrimLeft( TCHAR chTarget );void TrimLeft( LPCTSTR lpszTargets );**
从左删除字符，被删的字符与chTarget或lpszTargets匹配，一直删到第一个不匹配的字符为止
例：csStr="aaabaacdef";
    csStr.TrimLeft('a');
    cout<<csStr;                //baacdef
    csStr="aaabaacdef";
    csStr.TrimLeft("ab");
    cout<<csStr;                //cdef
//无参数时删除空格
**void TrimRight( );void TrimRight( TCHAR chTarget );void TrimRight( LPCTSTR lpszTargets );**
从右删除字符，被删的字符与chTarget或lpszTargets匹配，一直删到第一个不匹配的字符为止
例：csStr="abcdeaafaaa";
    csStr.TrimRight('a');
    cout<<csStr;               //abcdeaaf
    csStr="abcdeaafaaa";
    csStr.TrimRight("fa");
    cout<<csStr;                //abcde
//无参数时删除空格
**void Empty( );**
清空
例：csStr="abcdef";
    csStr.Empty();
    printf("%d",csStr.GetLength());    //0
**BOOL IsEmpty( ) const;**
测试对象是否为空，为空时返回零，不为空时返回非零
例：csStr="abc";
    cout<<csStr.IsEmpty();         //0;
    csStr.Empty();
    cout<<csStr.IsEmpty();         //1;
**int Find( TCHAR ch ) const;int Find( LPCTSTR lpszSub ) const;int Find( TCHAR ch, int nStart ) const;int Find( LPCTSTR pstr, int nStart ) const;**
查找字串，nStart为开始查找的位置。未找到匹配时返回-1，否则返回字串的开始位置
例：csStr="abcdef";
    cout<<csStr.Find('b');       //1
    cout<<csStr.Find("de");      //3
    cout<<csStr.Find('b',3);     //-1
    cout<<csStr.Find('b',0);     //1
    cout<<csStr.Find("de",4);    //-1
    cout<<csStr.Find("de",0);    //3
//当nStart超出对象末尾时，返回-1。
//当nStart为负数时，返回-1。
**int FindOneOf( LPCTSTR lpszCharSet ) const;**
查找lpszCharSet中任意一个字符在CString对象中的匹配位置。未找到时返回-1，否则返回字串的开始位置
例：csStr="abcdef";
    cout<<csStr.FindOneOf("cxy");       //2
**CString SpanExcluding( LPCTSTR lpszCharSet ) const;**
返回对象中与lpszCharSet中任意匹配的第一个字符之前的子串
例：csStr="abcdef";
    cout<<csStr.SpanExcluding("cf");    //ab
**CString SpanIncluding( LPCTSTR lpszCharSet ) const;**
从对象中查找与lpszCharSe中任意字符不匹配的字符，并返回第一个不匹配字符之前的字串
例：csStr="abcdef";
    cout<<csStr.SpanIncluding("fdcba");    //abcd
**int ReverseFind( TCHAR ch ) const;**
从后向前查找第一个匹配，找到时返回下标。没找到时返回-1
例：csStr="abba";
    cout<<csStr.ReverseFind('a');        //3
**void Format( LPCTSTR lpszFormat, ... );void Format( UINT nFormatID, ... );**
格式化对象，与C语言的sprintf函数用法相同
例：csStr.Format("%d",13);
    cout<<csStr;                       //13
**TCHAR GetAt( int nIndex ) const;**
返回下标为nIndex的字符，与字符串的[]用法相同
例：csStr="abcdef";
    cout<<csStr.GetAt(2);             //c
//当nIndex为负数或超出对象末尾时，会发生无法预料的结果。
**void SetAt( int nIndex, TCHAR ch );**
给下标为nIndex的字符重新赋值
例：csStr="abcdef";
    csStr.SetAt(2,'x');
    cout<<csStr;                      //abxdef
//当nIndex为负数或超出对象末尾时，会发生无法预料的结果。
**CString Left( int nCount ) const;**
从左取字串
例：csStr="abcdef";
    cout<<csStr.Left(3);           //abc
//当nCount等于0时，返回空。
//当nCount为负数时，返回空。
//当nCount大于对象长度时，返回值与对象相同。
**CString Right( int nCount ) const;**
从右取字串
例：csStr="abcdef";
    cout<<csStr.Right(3);           //def
//当nCount等于0时，返回空。
//当nCount为负数时，返回空。
//当nCount大于对象长度时，返回值与对象相同。
**CString Mid( int nFirst ) const;CString Mid( int nFirst, int nCount ) const;**
从中间开始取字串
例：csStr="abcdef";
    cout<<csStr.Mid(2);           //cdef
    csStr="abcdef";
    cout<<csStr.Mid(2,3);         //cde
//当nFirst为0和为负数时，从第一个字符开始取。
//当nFirst等于对象末尾时，返回空字串。
//当nFirst超出对象末尾时，会发生无法预料的结果。
//当nCount超出对象末尾时，返回从nFirst开始一直到对象末尾的字串
//当nCount为0和为负数时，返回空字串。
**LPTSTR GetBuffer( int nMinBufLength );**
申请新的空间，并返回指针
例：csStr="abcde";
    LPTSTR pStr=csStr.GetBuffer(10);
    strcpy(pStr,"12345");
    csStr.ReleaseBuffer();
    pStr=NULL;
    cout<<csStr                 //12345
//使用完GetBuffer后，必须使用ReleaseBuffer以更新对象内部数据，否则会发生无法预料的结果。
**void ReleaseBuffer( int nNewLength = -1 );**
使用GetBuffer后，必须使用ReleaseBuffer以更新对象内部数据
例：csStr="abc";
    LPTSTR pStr=csStr.GetBuffer(10);
    strcpy(pStr,"12345");
    cout<<csStr.GetLength();       //3(错误的用法)
    csStr.ReleaseBuffer();
    cout<<csStr.GetLength();       //5(正确)
    pStr=NULL;
//CString对象的任何方法都应在ReleaseBuffer之后调用
**LPTSTR GetBufferSetLength( int nNewLength );**
申请新的空间，并返回指针
例：csStr="abc";
    csStr.GetBufferSetLength(20);
    cout<<csStr;                  //abc
    count<<csStr.GetLength();     //20;
    csStr.ReleaseBuffer();
    count<<csStr.GetLength();     //3;
//使用GetBufferSetLength后可以不必使用ReleaseBuffer。
//==============================================
备注::
1>ReverseFind成员函数的功能跟std::wstring的rfind函数功能是类似的
2>特别是左右修剪的Trim函数很有用跟std::wstring的substr函数功能是类似的
