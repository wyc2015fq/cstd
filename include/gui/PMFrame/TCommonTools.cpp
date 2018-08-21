//********************************************************************************
//文件：TCommonTools.cpp
//作者：feiren
//时间: 2011-8-20
//用途：通用方法封装
//********************************************************************************
#include "StdAfx.h"
#include "string.h"
#include "TCommonTools.h"
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

void Trim(string &str)
{
	Ltrim(str);
	Rtrim(str);
}

void Ltrim(string &str)
{
	str.erase(str.begin(), std::find_if(str.begin(), str.end(),std::not1(std::ptr_fun(::isspace))));
}

void Rtrim(string &str)
{
	str.erase(std::find_if(str.rbegin(), str.rend(),std::not1(std::ptr_fun(::isspace))).base(),	str.end());
}

void ToUpper( string &str )
{
	transform(str.begin(),str.end(),str.begin(),toupper);
}

void ToLower( string &str )
{
	transform(str.begin(),str.end(),str.begin(),tolower);
}
bool RightStrCmp( const char *src,const char *dest ,char stop)
{
	if(dest==NULL||src==NULL)return false;
	int slen=strlen(src)-1;
	int dlen=strlen(dest)-1;
	while (dlen>=0&&slen>=0)
	{		
		if(src[slen]!=dest[dlen])return false;
		if(src[slen]==stop)return true;
		slen--;
		dlen--;
	}
	return dlen<0;
}
bool RightStriCmp( const char *src,const char *dest ,char stop)
{
	if(dest==NULL||src==NULL)return false;
	int slen=strlen(src)-1;
	int dlen=strlen(dest)-1;
	char Stop=tolower(stop);
	char c=0;
	while (dlen>=0&&slen>=0)
	{		
		c=tolower(src[slen]);
		if(c!=tolower(dest[dlen]))return false;
		if(c==Stop)return true;
		slen--;
		dlen--;
	}
	return dlen<0;
}

bool LeftStriCmp( const char *src,const char *dest ,char stop)
{
	if(dest==NULL||src==NULL)return false;	
	char Stop=tolower(stop);
	char c=0;
	while (*src&&*dest)
	{		
		c=tolower(*src);
		if(c!=tolower(*dest))return false;
		if(c==Stop)return true;
		src++;
		dest++;
	}
	return *dest==NULL;
}

bool LeftStrCmp( const char *src,const char *dest ,char stop)
{
	if(dest==NULL||src==NULL)return false;		
	while (*src&&*dest)
	{		
		if(*src!=*dest)return false;
		if(*src==stop)return true;
		src++;
		dest++;
	}
	return *dest==NULL;
}

TStringSpliter::TStringSpliter( const char *buffer,const char *splchs,bool bSort/*=false*/ )
{
	SetData(buffer,splchs,bSort);
}

TStringSpliter::TStringSpliter( const char *buffer,const char *firstsplchs,const char *Secondsplchs )
{
	type=-1;
	data=NULL;
	datas=NULL;
	sortdatas=NULL;
	if (buffer==NULL||firstsplchs==NULL||Secondsplchs==NULL)return;
	int len =strlen(buffer)+1;
	data=new char[len];
	memcpy(data,buffer,len);
	char *str=strtok(data,firstsplchs);
	datas=new vector<char *>;
	sortdatas=new TMapCharStrings;
	while (str)
	{
		datas->push_back(str);
		str=strtok(NULL,firstsplchs);
	}
	char *name,*value;
	for (vector<char *>::iterator it=datas->begin();it!=datas->end();it++)
	{
		name=strtok((*it),Secondsplchs);
		value=strtok(NULL,Secondsplchs);
		(*sortdatas)[name]=value;
	}
	delete datas;
	datas=NULL;
}

TStringSpliter::TStringSpliter():type(0),data(NULL),datas(NULL),sortdatas(NULL)
{

}

void TStringSpliter::SetData( const char *buffer,const char *splchs,bool bSort/*=false*/ )
{
	if (bSort)type=1;
	else type=0;
	data=NULL;
	datas=NULL;
	sortdatas=NULL;
	if (buffer==NULL||splchs==NULL)return;
	int len =strlen(buffer)+1;
	data=new char[len];
	memcpy(data,buffer,len);
	if (bSort)
	{
		char *str=strtok(data,splchs);
		sortdatas=new TMapCharStrings;
		while (str)
		{
			(*sortdatas)[str]=NULL;
			str=strtok(NULL,splchs);
		}
	}else
	{
		char *str=strtok(data,splchs);
		datas=new vector<char *>;
		while (str)
		{
			datas->push_back(str);
			str=strtok(NULL,splchs);
		}
	}
}

TStringSpliter::~TStringSpliter()
{
	if (data)delete[] data;
	if (datas) delete datas;
	if (sortdatas) delete sortdatas;
}

char * TStringSpliter::operator[]( int index )
{
	if (sortdatas!=NULL)
	{
		if (index<0||index>=(int)sortdatas->size())return NULL;
		int i=0;
		TMapCharStrings::iterator it=sortdatas->begin();
		while (it!=sortdatas->end()&&i<index){it++;i++;}
		if (it!=sortdatas->end())
		{
			if (type==-1)return it->second;
			else return (char*)it->first;
		}	
	}else if (datas!=NULL)
	{
		if (index<0||index>=(int)datas->size())return NULL;
		return (*datas)[index];
	}
	return NULL;
}

char * TStringSpliter::operator[](const char* name )
{
	if (sortdatas!=NULL)
	{
		TMapCharStrings::iterator it=sortdatas->find(name);
		if (it!=sortdatas->end())
		{
			return it->second;
		}		
	}
	return NULL;
}

bool TStringSpliter::Find( const char *str )
{
	if (sortdatas!=NULL)
	{				
		TMapCharStrings::iterator it=sortdatas->find(str);		
		if (it!=sortdatas->end())return true;
	}else if (datas!=NULL)
	{
		for (vector<char*>::iterator it=datas->begin();it!=datas->end();it++)
		{
			if(0==strcmp(*it,str))return true;
		}		
	}
	return false;
}

int TStringSpliter::GetCount()
{
	if (datas!=NULL)
	{
		return datas->size();
	}else if (sortdatas!=NULL)
	{
		return sortdatas->size();
	}
	return 0;
}

char* TFile::ReadFile( const char *filePath )
{
	if(filePath==NULL)return NULL;
	FILE *fp=fopen(filePath,"rb");
	if(!fp)return NULL;
	fseek(fp,0,SEEK_END);
	int len=ftell(fp);
	fseek(fp,0,SEEK_SET);
	char *data=new char[len+1];
	fread(data,1,len,fp);
	fclose(fp);
	data[len]=0;
	return data;
}

int TFile::FileSize( const char *filePath )
{
	int Result = 0;
	if(filePath==NULL)return NULL;
	FILE *fp=fopen(filePath,"rb");
	if(!fp)return NULL;
	fseek(fp,0,SEEK_END);
	Result=ftell(fp);
	fclose(fp);
	return Result;
}

char* TFile::ReadByte( const char *filePath,unsigned int &length )
{
	if(filePath==NULL)return NULL;
	FILE *fp=fopen(filePath,"rb");
	if(!fp)return NULL;
	fseek(fp,0,SEEK_END);
	length=ftell(fp);
	fseek(fp,0,SEEK_SET);
	char *data=new char[length];
	fread(data,1,length,fp);
	fclose(fp);
	return data;
}

int TFile::ReadFile( const char *filePath,char *&content )
{
	if(filePath==NULL)return NULL;
	FILE *fp=fopen(filePath,"rb");
	if(!fp)return NULL;
	fseek(fp,0,SEEK_END);
	int len=ftell(fp);
	fseek(fp,0,SEEK_SET);
	content=new char[len+1];
	fread(content,1,len,fp);
	fclose(fp);
	content[len]=0;
	return len;
}

bool TFile::WriteFile( const char *filePath,const char *content,int count/*=-1*/ )
{
	if(filePath==NULL||content==NULL)return false;
	FILE *fp=fopen(filePath,"wb");
	if(!fp)return NULL;	
	int len=0;
	if(count>0)len=count;
	else len=strlen(content);
	fwrite(content,1,len,fp);
	fclose(fp);	
	return true;
}

bool TFile::AppendFile( const char *filePath,const char *content,int count/*=-1*/ )
{
	if(filePath==NULL||content==NULL)return false;
	FILE *fp=fopen(filePath,"ab");
	if(!fp)return NULL;	
	int len=0;
	if(count>0)len=count;
	else len=strlen(content);
	fwrite(content,1,len,fp);
	fclose(fp);	
	return true;
}

bool TFile::AppendFile( const char *filePath,const char *format,... )
{
	if(filePath==NULL||format==NULL)return false;
	char buffer[1024]={0};
	va_list marker;
	va_start( marker, format );     /* Initialize variable arguments. */
	_vsnprintf(buffer,1024,format,marker);
	va_end( marker );              /* Reset variable arguments.      */
	FILE *fp=fopen(filePath,"ab");
	if(!fp)return NULL;	
	int len=strlen(buffer);
	fwrite(buffer,1,len,fp);
	fclose(fp);	
	return true;
}

int TFile::GetPaths( const string &Path,TVecStrings &paths,bool bChildPaths/*=true*/ )
{
	WIN32_FIND_DATAA fd;
	string searchFile = Path+"\\*.*"; 	
	HANDLE hFind = ::FindFirstFileA( searchFile.c_str(), &fd );		
	if(INVALID_HANDLE_VALUE == hFind)return 0;
	do 
	{
		string filename(fd.cFileName);				
		if ( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )  
		{
			if((filename !=".") && (filename != ".."))
			{
				paths.push_back(Path + "\\" + filename);
				if(bChildPaths)
				{
					GetPaths(Path + "\\" + filename ,paths , bChildPaths);
				}
			}						
		}  

	} while (::FindNextFileA (hFind, &fd));
	(void) ::FindClose(hFind);
	return paths.size();
}
static int GetFiles( const string &Path,TVecStrings &files,TStringSpliter& filter,bool bChildPaths/*=true*/ )
{
	WIN32_FIND_DATAA fd;
	string searchFile = Path+"\\*.*"; 	
	HANDLE hFind = ::FindFirstFileA( searchFile.c_str(), &fd );		
	if(INVALID_HANDLE_VALUE == hFind)return 0;
	do 
	{
		string filename(fd.cFileName);				
		if ( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )  
		{
			if(filter.GetCount()==0)
			{
				files.push_back(Path+"\\"+filename);
			}else 
			{
				filename=TFile::GetFileEx(fd.cFileName);
				ToUpper(filename);
				if(filter.Find(filename.c_str()))
				{
					files.push_back(Path+"\\"+fd.cFileName);
				}
			}	
		}else if(bChildPaths&&(filename !=".") && (filename != ".."))
		{
			GetFiles(Path + "\\" + filename ,files,filter,bChildPaths);
		}

	} while (::FindNextFileA (hFind, &fd));
	(void) ::FindClose(hFind);
	return files.size();
}

void Print( const char *format,... )
{
	va_list arg;
	va_start(arg,format);
	char buf[1024]={0};
	vsprintf(buf,format,arg);  
	strcat(buf,"\n");
	printf(buf);
	va_end(arg);

}

 /** 
  *     该函数用于将Unicode数组转为utf8格式! 
  *     @param out   转换结果存放区域指针 
  *     @param outLength   转换结果存放区域大小 
  *     @param in   源串存放区域指针 
  *     @param inLength   源串存放区域大小 
  *     @return int 转换结果在目的串中实际使用的长度，转换失败则返回-1 
  */  
int CStrConvertor::Unicode2Utf8( char* out, int& outLength,const wchar_t * in, int inLength )  
{  
    //------------------------------------------------  
    //参数有效性判断  
    if(out == NULL || in == NULL || inLength<0)  
    {  
        return -1;  
    }  
    int totalNum = 0;  
    for(int i = 0; i < inLength; i++)//计算转换结果实际所需长度  
    {  
        wchar_t unicode = in[i];  
        if (unicode >= 0x0000 && unicode <= 0x007f)  
        {  
            totalNum += 1;  
        }  
        else if (unicode >= 0x0080 && unicode <= 0x07ff)  
        {  
            totalNum += 2;  
        }  
        else if (unicode >= 0x0800 && unicode <= 0xffff)  
        {  
            totalNum += 3;  
        }  
    }  
    if( outLength < totalNum )//参数有效性判断！  
    {  
        return -1;  
    }  
    //------------------------------------------------  
  
    int outsize = 0;//用来计数输出结果的实际大小！  
    char *tmp = out;  
    int i = 0;  
    for (i = 0; i < inLength; i++)  
    {  
        if(outsize>outLength) //空间不足对应处理！  
        {  
            return -1;  
        }  
        wchar_t unicode = in[i];  
          
        if (unicode >= 0x0000 && unicode <= 0x007f)  
        {  
            *tmp = (char)unicode;  
            tmp += 1;  
            outsize += 1;  
        }  
        else if (unicode >= 0x0080 && unicode <= 0x07ff)  
        {  
            *tmp = 0xc0 | (unicode >> 6);  
            tmp += 1;  
            *tmp = 0x80 | (unicode & (0xff >> 2));  
            tmp += 1;  
            outsize += 2;  
        }  
        else if (unicode >= 0x0800 && unicode <= 0xffff)  
        {  
            *tmp = 0xe0 | (unicode >> 12);  
            tmp += 1;  
            *tmp = 0x80 | (unicode >> 6 & 0x00ff);  
            tmp += 1;  
            *tmp = 0x80 | (unicode & (0xff >> 2));  
            tmp += 1;  
            outsize += 3;  
        }  
    }  
    return outsize;  
}  

std::string CStrConvertor::Unicode2Utf8( const wchar_t * in )
{
	string Result;
	//参数有效性判断  
	if(in)  
	{
		int inLength = wcslen(in);
		int outsize = 0;//用来计数输出结果的实际大小！  
		char tmp[4]={0};  
		wchar_t unicode;
		for (int i = 0; i < inLength; i++)  
		{   
			unicode = in[i];  
			if (unicode >= 0x0000 && unicode <= 0x007f)  
			{  
				tmp[0] = (char)unicode;  
				tmp[1] = 0;  
			}  
			else if (unicode >= 0x0080 && unicode <= 0x07ff)  
			{  
				tmp[0] = 0xc0 | (unicode >> 6);  
				tmp[1] = 0x80 | (unicode & (0xff >> 2));  
				tmp[2] = 0; 
			}  
			else if (unicode >= 0x0800 && unicode <= 0xffff)  
			{  
				tmp[0] = 0xe0 | (unicode >> 12);  
				tmp[1] = 0x80 | (unicode >> 6 & 0x00ff);  
				tmp[2] = 0x80 | (unicode & (0xff >> 2)); 
				tmp[3] = 0; 
			}  
			Result += tmp;
		}  
	}  
	return Result;  
}

/** 
  * 该函数用于将utf8数组转为Unicode格式! 
  * 目前该函数返回值为：转换后unicode数据占用的wchar_t的个数（切记不是总char的个数） ！ 
  *     @param out   转换结果存放区域指针 
  *     @param outsize   转换结果存放区域大小 
  *     @param in   源串存放区域指针 
  *     @param insize   源串存放区域大小 
  *     @return int 转换结果在目的串中的长度，转换失败则返回-1 
  */   
int  CStrConvertor::Utf82Unicode(LPWSTR out,  int outsize , LPSTR in,int insize)  
{  
    //-------------------------------------------------------------------------------------------  
    //参数有效性判断  
    if(out == NULL || in == NULL || insize<0)  
    {  
        return -1;  
    }  
  
    int totalNum = 0;  
    char *p = in;  
    for(int i=0;i<insize;i++)  
    {  
        if (*p >= 0x00 && *p <= 0x7f)//说明最高位为'0'，这意味着utf8编码只有1个字节！  
        {  
            p++;  
            totalNum += 1;  
        }  
        else if ((*p & (0xe0))== 0xc0)//只保留最高三位，看最高三位是不是110，如果是则意味着utf8编码有2个字节！  
        {  
            p++;  
            p++;  
            totalNum += 1;  
        }  
        else if ((*p & (0xf0))== 0xe0)//只保留最高四位，看最高三位是不是1110，如果是则意味着utf8编码有3个字节！  
        {  
            p++;  
            p++;  
            p++;  
            totalNum += 1;  
        }  
    }  
    if( outsize < totalNum )//参数有效性判断！  
    {  
        return -1;  
    }  
    //------------------------------------------------  
    int resultsize = 0;  

    p = in;  
    char* tmp = (char *)out;  
    while(*p)  
    {  
        if (*p >= 0x00 && *p <= 0x7f)//说明最高位为'0'，这意味着utf8编码只有1个字节！  
        {  
            *tmp = *p;  
            tmp++;  
            //*tmp = '/0';  
            tmp++;  
            resultsize += 1;  
        }  
        else if ((*p & 0xe0)== 0xc0)//只保留最高三位，看最高三位是不是110，如果是则意味着utf8编码有2个字节！  
        {  
            wchar_t t = 0;  
            char t1 = 0;  
            char t2 = 0;  

            t1 = *p & (0x1f);//高位的后5位！（去除了头部的110这个标志位）  
            p++;  
            t2 = *p & (0x3f);//低位的后6位！（去除了头部的10这个标志位）  

            *tmp = t2 | ((t1 & (0x03)) << 6);  
            tmp++;  
            *tmp = t1 >> 2;//留下其保留的三位  
            tmp++;  
            resultsize += 1;  
        }  
        else if ((*p & (0xf0))== 0xe0)//只保留最高四位，看最高三位是不是1110，如果是则意味着utf8编码有3个字节！  
        {  
            wchar_t t = 0;  
            wchar_t t1 = 0;  
            wchar_t t2 = 0;  
            wchar_t t3 = 0;  
            t1 = *p & (0x1f);  
            p++;  
            t2 = *p & (0x3f);  
            p++;  
            t3 = *p & (0x3f);  

            *tmp = ((t2 & (0x03)) << 6) | t3;  
            tmp++;  
            *tmp = (t1 << 4) | (t2 >> 2);  
            tmp++;  
            resultsize += 1;  
        }  
        p++;  
    }  
    /*不考虑结束符，如果考虑则打开此段！  
    *tmp = '/0';  
    tmp++;  
    *tmp = '/0';  
    resultsize += 2;  
    */  
    return resultsize;  
}  

std::wstring CStrConvertor::Utf82Unicode( const char* in )
{
	wstring Result;
    //参数有效性判断  
    if( in )  
    {  
		int insize = strlen(in);
		const char *p = in;  
		char tmp[4] = {0};   
		char t1 = 0;  
		char t2 = 0; 
		wchar_t t3 = 0;  
		wchar_t t4 = 0;  
		wchar_t t5 = 0;  
		while(*p)  
		{  
			if (*p >= 0x00 && *p <= 0x7f)//说明最高位为'0'，这意味着utf8编码只有1个字节！  
			{  
				tmp[0] = *p;  
				tmp[1] = 0;   
				tmp[2] = 0;
				tmp[3] = 0;
			}  
			else if ((*p & 0xe0)== 0xc0)//只保留最高三位，看最高三位是不是110，如果是则意味着utf8编码有2个字节！  
			{  
				t1 = *p & (0x1f);//高位的后5位！（去除了头部的110这个标志位）  
				p++;  
				t2 = *p & (0x3f);//低位的后6位！（去除了头部的10这个标志位）  

				tmp[0] = t2 | ((t1 & (0x03)) << 6);   
				tmp[1] = t1 >> 2;//留下其保留的三位  
				tmp[2] = 0;
				tmp[3] = 0;
			}  
			else if ((*p & (0xf0))== 0xe0)//只保留最高四位，看最高三位是不是1110，如果是则意味着utf8编码有3个字节！  
			{  
				t3 = *p & (0x1f);  
				p++;  
				t4 = *p & (0x3f);  
				p++;  
				t5 = *p & (0x3f);  
				tmp[0] = ((t4 & (0x03)) << 6) | t5;   
				tmp[1] = (t3 << 4) | (t4 >> 2);  
				tmp[2] = 0;
				tmp[3] = 0;
			}  
			Result += (wchar_t*)tmp;
			p++;  
		} 
    }  
	return Result;    
}

std::string CStrConvertor::Unicode2GBK( const wchar_t * in )
{
	int len = 3*(wcslen(in)+1);
	char *p = (char *)malloc(len*sizeof(char));
	memset(p,0,len);
	WideCharToMultiByte( CP_ACP, 0, in, -1,	p, len, NULL, NULL );
	string Result(p);
	free(p);
	return Result;
}

std::wstring CStrConvertor::GBK2Unicode( const char* in )
{
	int len =strlen(in)+1;
	wchar_t *p = (wchar_t *)malloc(len*sizeof(wchar_t));
	memset(p,0,len);
	MultiByteToWideChar( CP_ACP, 0, in, len, p, len );
	wstring Result(p);
	free(p);
	return Result;
}

void GetDropFiles( const char *pFiles,vector<string> &files )
{
	files.clear();
	while (*pFiles!=NULL)
	{
		files.push_back(pFiles);
		pFiles = pFiles + strlen(pFiles) + 1;
	}
}

void GetDropFiles( const wchar_t *pFiles,vector<string> &files )
{
	files.clear();
	while (*pFiles!=NULL)
	{
		files.push_back(CStrConvertor::Unicode2GBK(pFiles));
		pFiles = pFiles + wcslen(pFiles) + 1;
	}
}

int TFile::GetFiles( const string &Path,TVecStrings &files,const string& filter,bool bChildPaths/*=true*/ )
{
	if(Path=="")return 0;
	string filters=filter;
	ToUpper(filters);
	TStringSpliter strspliter(filters.c_str(),"|");
	return ::GetFiles(Path,files,strspliter,bChildPaths);
}

bool TFile::IsFileExist( const char *filePath )
{
	if(!PathFileExistsA(filePath))return false;
	return !PathIsDirectoryA(filePath);	
}

bool TFile::IsPathExist( const char *Path )
{
	if(!PathFileExistsA(Path))return false;
	return PathIsDirectoryA(Path)!=0;	
}

bool TFile::CreatePath( const char *Path )
{
	if(Path==NULL)return false;
	int len=strlen(Path)+1;
	char *p=new char[len];
	memcpy(p,Path,len);
	char *head=p;
	bool bCreate=false;
	char c=0;
	while (*p)
	{
		if('\\'==*p||'/'==*p)
		{
			if(bCreate)
			{
				c=*p;
				*p=0;
				CreateDirectoryA(head,NULL);
				*p=c;
			}
			bCreate=true;
		}
		p++;
	}
	delete[] head;
	return CreateDirectoryA(Path,NULL)!=0;	
}

bool TFile::NewFile( const char *filePath,int len,char fillChar/*=0*/ )
{
	if(filePath==NULL)return false;
	FILE *fp=fopen(filePath,"wb");
	if(!fp)return NULL;	
	char *content=new char[len];
	memset(content,fillChar,len);
	fwrite(content,1,len,fp);
	fclose(fp);	
	delete[] content;
	return true;
}

std::string TFile::GetFileEx( const char *Path )
{
	string Ex;
	if(Path!=NULL)
	{
		int len=strlen(Path)-1;
		while (len>0)
		{
			if(Path[len]=='.')
			{
				Ex=Path+len+1;
				break;
			}
			len--;
		}
	}
	return Ex;
}

std::string TFile::GetFilePath( const char *Path )
{
	string filePath;	
	if(Path!=NULL)
	{
		int len=strlen(Path)-1;
		char *p=new char[len+2];
		memcpy(p,Path,len+2);
		while (len>0)
		{
			if(p[len]=='\\'||p[len]=='/')
			{
				char c=p[len+1];
				p[len+1]=0;
				filePath=p;
				p[len+1]=c;
				break;
			}
			len--;
		}
		delete[] p;
	}
	return filePath;
}

std::string TFile::GetFileName( const char *Path )
{
	string Ex;
	if(Path!=NULL)
	{
		int len=strlen(Path)-1;
		while (len>0)
		{
			if(Path[len]=='\\'||Path[len]=='/')
			{
				Ex=Path+len+1;
				break;
			}
			len--;
		}
	}
	return Ex;
}

void TFile::FilesFilter( TVecStrings &files,TStringSpliter &filter )
{
	string fileName;
	for (TVecStrings::iterator it = files.begin(); it != files.end();)
	{
		fileName = GetFileEx(it->c_str());
		ToLower(fileName);
		if(!filter.Find(fileName.c_str()))
		{
			it = files.erase(it);
		}else
		{
			it++;
		}
	}
}

TWordLex::TWordLex( const char *_value , bool _bIgnoreSymbol):mSymbol(NULL),mbExpression(false)
{
	mbIgnoreSymbol = _bIgnoreSymbol;
	mData = _value;
	mIndex =(char*) mData.c_str();
}

bool TWordLex::IsKeyword()
{
	char c = *mIndex;
	return ('0'<=c && c<='9')||('a'<=c && c<='z')||('A'<=c && c<='Z')||('_'==c)||('.'==c)||('-'==c)||('/'==c)||('+'==c);
}

std::string TWordLex::GetWord(bool bgetWord)
{
	string result;
	char *p = mIndex;
	if(mIndex)
	{
		//1、先除去空白
		RemoveBlank();
		while(*mIndex)
		{		
			//2、再获取关键字或者符号
			if(IsComment())
			{
				RemoveComment();
				RemoveBlank();
			}else if(IsKeyword())
			{
				result = GetKeyword();
				break;
			}else if(*mIndex=='}')//结束符号
			{
				mIndex++;
				result = "}";
				break;
			}else if(mSymbol&&*mIndex==*mSymbol)
			{
				mIndex++;
				result = mSymbol;
				break;
			}
			else if(mbIgnoreSymbol)
			{		
				mIndex++;
				RemoveBlank();
			}else
			{
				result = GetSymbol();
				break;
			}			
		}
	}	
	if(!bgetWord)
	{
		mIndex = p;
	}
	return result;
}

std::string TWordLex::GetKeyword()
{
	mbExpression = false;
	string word;
	char *p = mIndex;
	do{
		mIndex++;
		if(*mIndex=='-'||*mIndex=='+')mbExpression=true;
	}
	while(IsKeyword());
	char c = *mIndex;	
	*mIndex = 0;
	word = p;
	*mIndex = c;	
	return word;
}

std::string TWordLex::GetSymbol()
{
	string word;
	char *p = mIndex;
	mIndex++;
	char c = *mIndex;	
	*mIndex = 0;
	word = p;
	*mIndex = c;
	return word;
}

void TWordLex::RemoveBlank()
{
	while (*mIndex ==' ' || *mIndex =='\t' || *mIndex =='\r' || *mIndex =='\n')
	{
		mIndex++;
	}
}

bool TWordLex::IsComment()
{
	return *mIndex =='/'&&(*(mIndex+1) =='/'||*(mIndex+1) =='*');
}

void TWordLex::RemoveComment()
{
	mIndex+=2;
	if(*(mIndex-1) =='*')//多行注释
	{
		while(*mIndex)
		{
			if(*mIndex=='*'&&*(mIndex+1)=='/')
			{
				mIndex+=2;
				break;
			}else
			{
				mIndex++;
			}
		}
	}else//单行注释
	{
		while(*mIndex)
		{
			if(*mIndex=='\r')
			{
				mIndex+=*(mIndex+1)=='\n' ? 2:1;
				break;
			}else if(*mIndex =='\n')
			{
				mIndex++;
				break;
			}else
			{
				mIndex++;
			}
		}
	}	
}

void TWordLex::SetmbIgnoreSymbol( bool _bIgnoreSymbol )
{
	mbIgnoreSymbol = _bIgnoreSymbol;
}

void TWordLex::SetReturnSymbol( const char *cSymbol )
{
	mSymbol = cSymbol;
}

bool TWordLex::IsExpression()
{
	return mbExpression;
}

TExpressionLex::TExpressionLex( const char *_value )
{
	mData  = _value;
	mIndex = (char*)mData.c_str();
}

std::string TExpressionLex::GetWord()
{
	string word;
	char *p = mIndex;
	if(*mIndex==0)word;
	mIndex++;
	while (*mIndex)
	{
		if(*mIndex=='-'||*mIndex=='+')break;
		mIndex++;
	}
	char c = *mIndex;	
	*mIndex = 0;
	word = p;
	*mIndex = c;	
	return word;
}
 

