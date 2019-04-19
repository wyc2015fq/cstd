# CStdioFile的用法详细解析 - xqhrs232的专栏 - CSDN博客
2017年08月14日 00:05:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：230
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.jb51.net/article/41398.htm](http://www.jb51.net/article/41398.htm)
CStdioFile 不支持Duplicate，LockRange，和UnlockRange 这几个CFile 函数。如果在CStdioFile 中调用了这几个函数，将会出现CNoSupported 异常
**关于CStdioFile**CStdioFile继承自CFile，一个CStdioFile 对象代表一个用运行时函数fopen 打开的C 运行时流式文件。
流式文件是被缓冲的，而且可以以文本方式（缺省）或二进制方式打开。文本方式提供对硬回车—换行符对的特殊处理。当你将一个换行符（0x0A）写入一个文本方式的CStdioFile 对象时，字节对（0x0D，0x0A）被发送给该文件。当你读一个文件时，字节对（0x0D，0x0A）被翻译为一个字节（0x0A）。
CStdioFile 不支持Duplicate，LockRange，和UnlockRange 这几个CFile 函数。如果在CStdioFile 中调用了这几个函数，将会出现CNoSupported 异常。
CStdioFile::ReadString(LPTSTR lpsz, UINT nMax);
读取一行文本到缓冲区，遇到“0x0D,0x0A”时停止读取，并且去掉硬回车“0x0D”,保留换行符“0x0A”,在字符串末尾添加“\0”(0x00)。nMax个字符里包含0x00这个字符。
**分析如下：**1）如果nMax <= 字符数，读取(nMax－1)个字符 + 0x00
2）如果nMax    = 字符数 + 1，读取nMax个字符 + 0x00
3）如果nMax    > 字符数，读取nMax个字符 + 0x0A + 0x00
CStdioFile::ReadString(CString &rString);（重载）
读取一行文本到rString，遇到回车换行符停止读取。回车和换行符不读到rString，而且末尾也没有添加“\0”。
CStdioFile::WriteString( LPCTSTR lpsz );（不支持CString直接写入）
将一个缓冲区中的数据写入与CStdioFile 对象关联的文件中。结束的空字符（“\0”）不被写入该文件。lpsz 中的所有换行符都被以一个硬回车换行符对写入该文件，即“\n”被转化成“\r\n”写入到文件里。
CFile的派生类CStdioFile提供了对文件进行流式的操作功能。其中函数void CStdioFile::WriteString( LPCTSTR lpsz )写入一个字符串，需要给字符串lpsz的末尾加上换行标志”\r\n”；函数bool CStdioFile::ReadString(CString &rString )从文件中读取一行，如果文件未读完返回true,否则返回false。 
**比如：写入文件的例子**
复制代码代码如下:
//创建文件
CStdioFile file;
file.Open("ts.txt",CFile::modeCreate|CFile::modeWrite);
//写入文件
CString str;
str.Format("%s\r\n","hello!I am talkingmute!");
file.Seek(0,CFile::end);
file.WriteString( str );
//关闭文件
file.Close();
**比如：读文件的例子**
复制代码代码如下:
CString strText = “”;
CString szLine = “”;
//打开文件
CStdioFile file;
file.Open("ts.txt",CFile::modeRead);
//逐行读取字符串
while( file.ReadString( szLine ) )
{
strText += szLine;
}
MessageBox(strText);
//关闭文件
file.Close();
CStdioFile类的声明保存再afx.h头文件中。
CStdioFile类不支持CFile类中的Duplicate、LockRange、UnlockRange函数，如果你使用了，会得到CNotSupportedException类的错误。
CStringFile类默认的是按照Text模式操作文件。CFile 类默认的是按照二进制模式操作文件。
**这里大致说明一下二进制模式和Text模式的区别。二进制模式：**对于一行的结尾我们必须输入”\r\n”，才能表示回车换行的效果。
**Text模式：**”\r”回车的工作是自动完成的，我们只需要写入”\n”即可。所以我们再使用文本模式时要主要，当我们从外部读入文件时，”\r\n”会被翻译成”\n”，写入文件时，我们对于回车换行只需提供”\n”，即可，”\r\n”会被写入到文件中。
m_pStream成员变量：
打开文件的指针。
**构造函数：**CStdioFile();
CStdioFile(FILE *pOpenStream);
CStdioFile(LPCTSTR lpFileName, UINT nOpenFlags);
throw(CFileException);
FILE *pOpenStream：指的是c运行函数fopen调用后返回的文件指针。
LPCTSTR lpFileName：指的是被打开的文件（绝对地址或相对地址）
UINT nOpenFlags：指的是CFile类中所描述的打开文件的方式。
virtual LPTSTR ReadString(LPTSTR lpsz, UINT nMax);
throw(CFileException);
如果使用该函数读取文本文件，当遇到”\r\n”，停止读取，并去掉”\r”，保留”\n”，并在字符串尾部增加”\0”，nMax的长度包含有”\0”字符，
**实际的分析如下：**如果nMax <= 字符数，读取(nMax-1)个字符+0x00；
如果nMax = 字符数 + 1，读取nMax个字符+0x00；
如果nMax > 字符数，读取nMax个字符+0x0A(”\n”) + 0x00；
如果文件有多行，则当文件没有读完时，返回NOT NULL，读到文件尾，返回NULL。
BOOL ReadString(CString& rString);
throw(CFileException);
读取一行文本到rString中，遇到回车换行符停止读取，回车和换行符均不读到rString中，尾部也不添加”0x00”。
如果文件有多行，则当文件没有读完时，返回TRUE，读到文件尾，返回FALSE。
virtual void WriteString(LPTSTR lpsz);
throw(CFileException);
将缓冲区中的数据写入到与CStdioFile对象相关联的文件中，不支持CString类型数据写入，结束的”\0”不被写入到文件中，lpsz缓冲区中的所有
