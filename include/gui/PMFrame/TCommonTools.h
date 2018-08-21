//********************************************************************************
//文件：TCommonTools.h
//作者：feiren
//时间：2011-8-20
//用途：通用方法封装
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************

#if !defined(AFX_TCommonTools_H__CC1B8D31_40DF_471F_8F41_9EF21DCE2F57__INCLUDED_)
#define AFX_TCommonTools_H__CC1B8D31_40DF_471F_8F41_9EF21DCE2F57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning (disable :4996)
#include <set>
#include <map>
#include <list>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

#define AssignSubStr(Left,str,len) PMChar  __CCDD__ = (str)[len];str[len] = 0;Left = str;(str)[len] = __CCDD__;
#define AssignMiddleStr(Left,str,from,to) PMChar  __CCDD__ = (str)[to];str[to] = 0;Left = str+from;(str)[to] = __CCDD__;

class TSort
{
public:
	bool operator() (const char *_A, const char *_B) const
	{
		return strcmp(_A,_B)<0?true:false;
	}
};
//**********************************************************************************
//作者：NRC_DouNingBo
//说明：utf8转化unicode、unicode转化utf8
//**********************************************************************************
class CStrConvertor  
{  
public:                                    
	static int Unicode2Utf8(LPSTR cBuf, int& iCBuf,LPCWSTR uBuf, int iUBuf);  
	static int Utf82Unicode(LPWSTR pDst,  int nDstLen , LPSTR pSrc,int nSrcLen);  
	static string Unicode2Utf8(const wchar_t * in);  
	static wstring Utf82Unicode(const char* in);  
	static string Unicode2GBK(const wchar_t * in);  
	static wstring GBK2Unicode(const char* in);  
}; 
typedef set<string> TSetStrings;
typedef set<char *,TSort> TSetCStrings;
typedef vector<string> TVecStrings;
typedef vector<char*> TVecCStrings;
typedef map<const char *,char* ,TSort> TMapCharStrings;
typedef map<string,char*> TMapCStrings;
typedef map<string,int> TMapInts;
typedef map<string,string> TMapStrings;
typedef multimap<string,string> TMutiStrings;

bool LeftStrCmp(const char *src,const char *dest,char stop=0);
bool LeftStriCmp(const char *src,const char *dest,char stop=0);
bool RightStrCmp(const char *src,const char *dest,char stop=0);
bool RightStriCmp(const char *src,const char *dest,char stop=0);

void Trim(string &str);
void Ltrim(string &str);
void Rtrim(string &str);
void ToUpper(string &str);
void ToLower(string &str);

void GetDropFiles(const char *pFiles,vector<string> &files);//获取DROPFILES 结构的文件路径c:\temp1.txt'\0'c:\temp2.txt'\0''\0'
void GetDropFiles(const wchar_t *pFiles,vector<string> &files);//获取DROPFILES 结构的文件路径c:\temp1.txt'\0'c:\temp2.txt'\0''\0'
/*************************************************************************
类说明:
1、将字符串分割成为数组,如:"a.b.c.d"
2、将字符串分割成为键值对,如:"a:b|c:d|e:f"
*************************************************************************/
class TStringSpliter
{
public:
	TStringSpliter();
	TStringSpliter(const char *buffer,const char *splchs,bool bSort=false);
	TStringSpliter(const char *buffer,const char *firstsplchs,const char *Secondsplchs);
	~TStringSpliter();
	void SetData(const char *buffer,const char *splchs,bool bSort=false);
	char *operator[](int index);
	char *operator[](const char* name);
	int GetCount();
	bool Find(const char *str);	
private:
	int type;
	char *data;
	vector<char *> *datas;
	TMapCharStrings *sortdatas;
};

class TFile
{
public:
	static char*	ReadByte(const char *filePath,unsigned int &length);
	static char*	ReadFile(const char *filePath);
	static int		ReadFile(const char *filePath,char *&content);
	static int		FileSize(const char *filePath);
	static bool		WriteFile(const char *filePath,const char *content,int count=-1);
	static bool		AppendFile(const char *filePath,const char *content,int count=-1);
	static bool		AppendFile(const char *filePath,const char *format,...);
	static int		GetPaths(const string &Path,TVecStrings &paths,bool bChildPaths=true);
	static void		FilesFilter(TVecStrings &files,TStringSpliter &filter);
	static int		GetFiles(const string &Path,TVecStrings &files,const string& filter="",bool bChildPaths=true);
	static bool		IsFileExist(const char *filePath);
	static bool		IsPathExist(const char *Path);
	static bool		CreatePath(const char *Path);
	static bool		NewFile(const char *filePath,int len,char fillChar=0);
	static string	GetFileName(const char *Path);
	static string	GetFilePath(const char *Path);
	static string	GetFileEx(const char *Path);
};

void Print(const char *format,...);

class TWordLex
{
public:
	TWordLex(const char *_value, bool _bIgnoreSymbol = true);
	string GetWord(bool bgetWord=true);
	void SetmbIgnoreSymbol(bool _bIgnoreSymbol);
	void SetReturnSymbol(const char *cSymbol);
	bool IsExpression();
private:
	bool IsComment();
	bool IsKeyword();
	string GetKeyword();
	string GetSymbol();
	void RemoveBlank();
	void RemoveComment();

	const char *mSymbol;
	string		mData;
	char	   *mIndex;
	bool		mbIgnoreSymbol;
	bool        mbExpression;
};

class TExpressionLex
{
public:
	TExpressionLex(const char *_value);
	string GetWord();
private:
	string		mData;
	char	   *mIndex;
};

enum TTokenType
{
	TOKEN_NONE = 0,
	TOKEN_CHINESE,
	TOKEN_SYMBOL,
	TOKEN_TAB,
	TOKEN_ENTER,
	TOKEN_WORD,
	TOKEN_CONTENT,
	TOKEN_PICTURE
};

struct TTexToken 
{
	TTexToken(){Clear();}
	int type;
	char *p;
	char c;
	int len;
	char *GetString(){c=p[len];p[len]=0;return p;}
	void Restore(){p[len]=c;}
	void Clear(){type = TOKEN_NONE;p=NULL;c=0;len=0;}
};

class TTextWordLex
{
public:
	void SetData(char *data){mData = data;}
	TTexToken *GetToken()
	{
		mToken.Clear();
		if(mData&&*mData)
		{
			if(*mData<0) GetChineseWord();
			else if(*mData=='\t')GetTab();
			else if(*mData=='\r'||*mData=='\n')GetEnter();
			else if(IsWord(*mData))GetWord();
			else GetSymbol();
		}
		return &mToken;
	}
	TTexToken *GetTokenEx()
	{
		mToken.Clear();
		if(mData&&*mData)
		{
			if(*mData=='\t')GetTab();
			else if(*mData=='\r'||*mData=='\n')GetEnter();
			else GetContent();
		}
		return &mToken;
	}
private:
	bool IsWord(char c)
	{
		return (c>='0'&&c<='9')||('a'<=c&&c<='z')||('A'<=c&&c<='Z')||c=='_';
	}
	void GetChineseWord()
	{
		mToken.type = TOKEN_CHINESE;
		mToken.p=mData;
		mData+=2;
		mToken.len=2;
	}
	void GetSymbol()
	{
		mToken.type = TOKEN_SYMBOL;
		mToken.p=mData;
		mData++;
		mToken.len++;
	}
	void GetTab()
	{
		mToken.type = TOKEN_TAB;
		mToken.p=mData;
		mData++;
		mToken.len=1;
	}
	void GetEnter()
	{
		mToken.type = TOKEN_ENTER;
		mToken.p=mData;
		mData++;
		mToken.len++;
		if(*mToken.p!=*mData && (*mData=='\r'||*mData=='\n'))
		{
			mData++;
			mToken.len++;
		}		
	}
	void GetWord()
	{
		mToken.type = TOKEN_WORD;
		mToken.p=mData;
		while(IsWord(*mData))
		{
			mData++;
			mToken.len++;
		}
	}
	void GetContent()
	{
		mToken.type = TOKEN_CONTENT;
		mToken.p    = mData;
		while(*mData&&*mData!='\t'&&*mData!='\r'&&*mData!='\n')
		{
			mData++;
			mToken.len++;
		}
	}
	char	 *mData;
	TTexToken mToken;
};
#endif