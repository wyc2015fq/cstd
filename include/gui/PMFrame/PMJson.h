//********************************************************************************
//文件：PMJSON.h
//作者：feiren
//时间：2011-8-20
//用途：封装Json的基本功能
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMJSON_H__
#define __PMJSON_H__

#include "PMBaseObject.h"
#include "PMInclude.h"
#include <windows.h>
#include <ShellAPI.h>
#include <string>
#include <sstream>
#include <fstream>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#pragma warning (disable : 4996)

class TJSON;
#define PreMallocSize 120*1024
#define PreMallocRatio 5

typedef map<int,TJSON*> TINDEXJSON; 
typedef map<string,TJSON*> TNAMEJSON; 
typedef vector<TJSON*> TJSONS; 
enum ValueType
{
	nullValue = 0, ///< 'null' value
	intValue,      ///< signed integer value
	uintValue,	   ///< unsigned integer value
	int64Value,    ///< long long value
	realValue,     ///< double value
	stringValue,   ///< UTF-8 string value
	booleanValue,  ///< bool value
	pointerValue,  ///< pointer value
	indexValue,	   ///< index value
	binaryValue,   ///< binary value---->len,pdata
	bFileValue,    ///< binary file value---->len,position
	wellValue,	   ///< well value---->char*,pointer
	arrayValue,    ///< array value (ordered list)
	objectValue    ///< object value (collection of name/value pairs).
};
struct TBuffer 
{	
	TBuffer():size(0),useLen(0),buffer(NULL){}
	TBuffer(unsigned int presize):size(presize),useLen(0){buffer=(char*)malloc(presize);memset(buffer,0,presize);}
	void Initialize(){useLen=0;size=PreMallocSize;buffer=(char*)malloc(PreMallocSize);memset(buffer,0,PreMallocSize);}
	void Add(const char *data,int len);
	void operator +=(const char *data)
	{
		Add(data,strlen(data));
	}
	void operator +=(unsigned long data)
	{
		char buf[32]={0};
		ultoa(data,buf,10);
		Add(buf,strlen(buf));
	}
	void operator +=(unsigned int data)
	{
		char buf[32]={0};
		ultoa(data,buf,10);
		Add(buf,strlen(buf));
	}
	void Clear();	
	void MemoryReduce();	
	bool Write(const char *filename);
	bool Read(const char *filename);
	char *buffer;
	unsigned int size;
	unsigned int useLen;
};

class TJSONIterator
{
public:	
	TJSONIterator();
	TJSONIterator(const TNAMEJSON::iterator &state );	
	TJSONIterator(const TJSONS::iterator &state );	
	TJSONIterator(const TJSONIterator &other);
	TJSONIterator& operator =(const TJSONIterator &other);	
	bool operator ==(const TJSONIterator &other);	
	bool operator !=(const TJSONIterator &other);
	bool operator >(const TJSONIterator &other);	
	bool operator <(const TJSONIterator &other);
	bool operator >=(const TJSONIterator &other);	
	bool operator <=(const TJSONIterator &other);
	int operator -(const TJSONIterator &other);
	TJSON& operator *();
	TJSON* operator ->();
	TJSONIterator& operator ++();
	TJSONIterator& operator --();
	TJSONIterator operator ++(int);
	TJSONIterator operator --(int);
	TJSONIterator operator +(int distance);
	TJSONIterator operator -(int distance);

	string GetName();
private:
	TJSONS::iterator array_;
	TNAMEJSON::iterator map_;
	char isArray_;
};

class TJSONConstIterator
{
public:	
	TJSONConstIterator();
	TJSONConstIterator(const TNAMEJSON::const_iterator &state );	
	TJSONConstIterator(const TJSONS::const_iterator &state );	
	TJSONConstIterator(const TJSONConstIterator &other);
	TJSONConstIterator& operator =(const TJSONConstIterator &other);	
	bool operator ==(const TJSONConstIterator &other);	
	bool operator !=(const TJSONConstIterator &other);
	bool operator >(const TJSONConstIterator &other);	
	bool operator <(const TJSONConstIterator &other);
	bool operator >=(const TJSONConstIterator &other);	
	bool operator <=(const TJSONConstIterator &other);
	int operator -(const TJSONConstIterator &other);
	const TJSON& operator *();
	const TJSON* operator ->();
	TJSONConstIterator& operator ++();
	TJSONConstIterator& operator --();
	TJSONConstIterator operator ++(int);
	TJSONConstIterator operator --(int);
	TJSONConstIterator operator +(int distance);
	TJSONConstIterator operator -(int distance);
	string GetName();
private:
	TJSONS::const_iterator array_;
	TNAMEJSON::const_iterator map_;
	char isArray_;
};


class TJSON
{
	static char *NewString(const char *data);
	static TNAMEJSON *NewMap();			
	static TJSONS *NewArray();	
	static TJSON *NewJSON();
	void DeleteString();
	void DeleteMap();			
	void DeleteArray();	
	void DeleteBinary();
	
public:
	typedef TJSONIterator iterator;
	typedef TJSONConstIterator const_iterator;
	typedef std::vector<std::string> Members;
	static const TJSON null;
	static const int minInt;
	static const int maxInt;
	static const unsigned int maxUInt;
	static TJSON* Create();
	static TJSON* Create(bool data);
	static TJSON* Create(int data);
	static TJSON* Create(unsigned int data);
	static TJSON* Create(INT64 data);
	static TJSON* Create(double data);
	static TJSON* Create(const char* data);
	static TJSON* Create(const string &data);
	static TJSON* Create(void *data);
	static TJSON* Create(const TJSON &data);
	static void   DeleteJSON(TJSON *json);	
	static TJSON* ReadFile(const char *fileName);

	TJSON();
	TJSON(bool data);
	TJSON(int data);
	TJSON(unsigned int data);
	TJSON(INT64 data);
	TJSON(double data);
	TJSON(const char* data);
	TJSON(const string &data);
	TJSON(void *data);
	TJSON(const TJSON &data);
	TJSON(TBuffer &data);
	virtual ~TJSON();
	TJSON &operator=( bool data );
	TJSON &operator=( int data );
	TJSON &operator=( unsigned int data );
	TJSON &operator=( INT64 data );
	TJSON &operator=( double data );
	TJSON &operator=( const char* data );
	TJSON &operator=( const string &data );
	TJSON &operator=( void *data );
	TJSON &operator=( const TJSON &data );
	TJSON &operator=( TBuffer &data );

	bool operator==( bool data )const;
	bool operator==( int data )const;
	bool operator==( unsigned int data )const;
	bool operator==( INT64 data )const;
	bool operator==( double data )const;
	bool operator==( const char* data )const;
	bool operator==( const string &data )const;
	bool operator==( void *data )const;
	bool operator==( const TJSON &other )const;

	bool operator!=( bool data )const;
	bool operator!=( int data )const;
	bool operator!=( unsigned int data )const;
	bool operator!=( INT64 data )const;
	bool operator!=( double data )const;
	bool operator!=( const char* data )const;
	bool operator!=( const string &data )const;
	bool operator!=( void *data )const;
	bool operator!=( const TJSON &other )const;

	bool operator<=( int data )const;
	bool operator<=( unsigned int data )const;
	bool operator<=( INT64 data )const;
	bool operator<=( double data )const;
	bool operator<=( const char* data )const;
	bool operator<=( const string &data )const;

	bool operator>=( int data )const;
	bool operator>=( unsigned int data )const;
	bool operator>=( INT64 data )const;
	bool operator>=( double data )const;
	bool operator>=( const char* data )const;
	bool operator>=( const string &data )const;

	bool operator<( int data )const;
	bool operator<( unsigned int data )const;
	bool operator<( INT64 data )const;
	bool operator<( double data )const;
	bool operator<( const char* data )const;
	bool operator<( const string &data )const;

	bool operator>( int data )const;
	bool operator>( unsigned int data )const;
	bool operator>( INT64 data )const;
	bool operator>( double data )const;
	bool operator>( const char* data )const;
	bool operator>( const string &data )const;

	bool operator <( const TJSON &other ) const;
	bool operator <=( const TJSON &other ) const;
	bool operator >=( const TJSON &other ) const;
	bool operator >( const TJSON &other ) const;

	ValueType GetType()const;										//获取类型	
	bool	Equal(TJSON *data)const;											//判断是否有效
	bool	IsNull()const;											//判断是否有效
	bool	IsBool()const;											//判断是否bool类型
	bool	IsInt()const;											//判断是否int类型
	bool	IsUInt()const;											//判断是否UInt类型
	bool	IsInt64()const;											//判断是否INT64类型
	bool	IsDouble()const;										//判断是否double类型
	bool	IsString()const;										//判断是否char*类型
	bool	IsPointer()const;										//判断是否void*类型	
	bool	IsArray()const;											//判断是否是数组
	bool	IsObject()const;										//判断是否是对象
	bool	IsBinary()const;										//判断是否是二进制数据
	bool	ToBool()const;											//转化成bool类型
	bool	IsParticular()const;									//是否特殊数据-0
	void	SetParticular();										//设置-0
	int     ToInt()const;											//转化成int类型	
	unsigned int ToUInt()const;										//转化成UInt类型
	INT64   ToInt64()const;											//转化成INT64类型
	double  ToDouble()const;										//转化成double类型
	char*   ToCString()const;										//转化成char*类型
	string  ToString()const;										//转化成string类型
	void*   ToPointer()const;										//转化成void*类型
	TBuffer ToBinary()const;										//转化成二进制数据
	void    SetDefine(void *data);									//获取void*类型的定义,除了double和INT64两种类型，其他类型都可以存储定义
	void*   GetDefine()const;										//获取void*类型的定义,除了double和INT64两种类型，其他类型都可以存储定义
	string  GetName()const;											//获取名称
	int     GetChildCount()const;									//获取孩子个数
	void    SetChildCount(int n);									//设置孩子个数，json只能是数组
	TJSON * GetChild()const;										//获取获取第一个孩子
	TJSON * GetLastChild()const;									//获取获取最后一个孩子
	TJSON * GetFather()const;										//获取其父亲节点
	bool    Empty()const;											//判断子节点是否为空
	void    SetObject();											//将节点设置为对象			
	void    SetArray();												//将节点设置为数组
	void    SetBinary(const char *value,unsigned int length);		//将节点设置为二进制数据
	//高级功能
	bool    IsMember( const char *key ) const;						//判断是否是成员
	bool    IsMember( const std::string &key ) const;				//判断是否是成员
	TJSON * Clone(bool bNodeOnly=false);							//将此节点树拷贝成为另一棵树
	void    DeleteItem(unsigned int index,bool bfree=true);			//删除子节点树，bfree控制是否释放内存
	void    DeleteItem(const char *key,bool bfree=true);			//删除子节点树，bfree控制是否释放内存
	void    DeleteItem(const string &key,bool bfree=true);			//删除子节点树，bfree控制是否释放内存
	void    DeleteItem(TJSON *item,bool bfree=true);				//删除子节点树，bfree控制是否释放内存
	TJSON& operator [](unsigned int index);							//查找数组内的对象，如果不存在则创建新对象并添加到数组后面	
	TJSON& operator [](const string &key);							//查找数组内的对象，如果不存在则创建新对象并添加到数组后面
	const TJSON& operator [](unsigned int index)const;				//查找数组内的对象	
	const TJSON& operator [](const string &key)const;				//查找数组内的对象
	TJSON&			At( unsigned int index );						//查找数组内的对象，如果不存在则创建新对象并添加到数组后面
	const TJSON&	At( unsigned int index )const;					//查找数组内的对象
	TJSON&			At( const string &key );						//查找数组内的对象，如果不存在则创建新对象并添加到数组后面
	const TJSON&	At( const string &key )const;					//查找数组内的对象
	TRect<int>		GetRect(unsigned int index=0)const;				//获取矩形
	TPoint<int>		GetPoint(unsigned int index=0)const;			//获取点
	TSize<int>		GetSize(unsigned int index=0)const;				//获取点
	TJSON*          Find(const char *key);							//查找对象成员
	TJSON*          Find(const string &key);						//查找对象成员
	const TJSON*    Find(const char *key)const;						//查找对象成员
	const TJSON*    Find(const string &key)const;					//查找对象成员
	Members         GetMemberNames() const;							//获取所有的子对象名称
	iterator        begin();										//获取数组或对象的开始迭代器
	iterator        end();											//获取数组或对象的结束迭代器
	iterator        GetIterator();
	const_iterator  GetIterator()const;
	const_iterator  begin()const;									//获取数组或对象的开始迭代器
	const_iterator  end()const;										//获取数组或对象的结束迭代器
	bool		 Replace(unsigned int index,TJSON *data,bool bInsert=false);//替换指定对象，bInsert表示不存在则添加		
	bool		 Replace(const char *name,TJSON *data,bool bInsert=false);	//替换指定对象，bInsert表示不存在则添加
	bool		 Replace(const string &name,TJSON *data,bool bInsert=false);//替换指定对象，bInsert表示不存在则添加
	void		 RangeReplace(TJSON &jnRange,TJSON &jnNew);					//只是用来数组的批量替换
	void		 RangeReplace(int iFrom,TJSON &jnOld,TJSON &jnNew);		//只是用来数组的批量替换
	void		 RangeReplace(int iFrom,int iTo,TJSON &jnNew);				//只是用来数组的批量替换
	void		 RangeReplace(int iPos,TJSON &jnNew);						//只是用来数组的批量替换
	void		 RangeReplace(TJSON *jnPosition,TJSON &jnNew);				//只是用来数组的批量替换
	void		 RangeInsert(int iPos,TJSON &jnNew);						//只是用来数组的插入
	void		 RangeInsert(TJSON *jnPosition,TJSON &jnNew,bool back=true);//只是用来数组的插入
	void		 RangeInsert(TJSON &jnNew,TJSON *jnFrom,TJSON *jnTo);		//只是用来数组的插入
	void		 RangeInsert(TJSON &jnNew);									//只是用来数组的插入
	void		 RangeDelete(TJSON &jnNew);									//只是用来数组的批量删除
	void		 RangeDelete(int iPos,int nCount);							//只是用来数组的批量删除
	void		 RangeCopy(TJSON &jnRange,int iFrom,int iTo=-1);			//只是用来数组的批量拷贝
	void		 RangeCopy(TJSON &jnRange);									//只是用来数组的批量拷贝
	void		 Clear();													//将json数据删除并清空，变成空对象
	void		 RemoveAll();												//将json数据移除
	void		 Rename(const string &name);								//重命名
	TJSON *		 NewArraryObject();
	//数组的添加、删除操作
	TJSON*       Push( int data );									//在数组后添加数据
	TJSON*		 Push( unsigned int data );							//在数组后添加数据
	TJSON*		 Push( INT64 data );								//在数组后添加数据
	TJSON*		 Push( double data );								//在数组后添加数据
	TJSON*		 Push( const char* data );							//在数组后添加数据
	TJSON*		 Push( const string &data );						//在数组后添加数据
	TJSON*		 Push( const TJSON &data );							//在数组后添加数据			
	TJSON*		 Push( TBuffer &data );								//在数组后添加数据		
	TJSON*		 Front();											//获取数组第一个子节点
	TJSON*		 Back();											//获取数组最后一个子节点
	const TJSON* Front()const;										//获取数组第一个子节点
	const TJSON* Back()const;										//获取数组最后一个子节点
	void         Pop(bool bfree=true);								//删除数组最后一个节点，bfree控制是否释放
	void         DeleteRange(TJSON &jnRange);						//删除数组的一批数据
	//字符串操作
	void		 FromString(const char *data);
	TBuffer      Print(bool format)const;							//将json转化成字符串,bFormat表示是否格式化	
	void         parse( std::istream& sin);							//从流里解析出json							
	void         parse( const char *Data);							//从字符串里解析出json
	void         Parse(const std::string &document);				//从字符串里解析出json
	void         Read(const char *fileName);						//从文件里面读json
	void         Write(const char *fileName,bool bFormat=false)const;//将json写到文件里,bFormat表示是否格式化

	TINDEXJSON & GetIndexs();
	void         Push(int index , TJSON *json);
	void         Push( TJSON *data );								//在数组后添加数据
	TJSON&       Push();											//在数组后添加数据
	void		 Push( TRect<int> &data );							//在数组后添加数据
	void		 Insert(int index , TJSON *json);					//插入数据
	void		 Insert(const string &name , TJSON *json);				//插入数据
private:

	const char *parse_value(const char *value);						//从value里面解析json
	const char *parse_object(const char *value);					//解析对象
	const char *parse_array(const char *value);						//解析数组
	const char *parse_binary(const char *value);					//解析二进制数据
	const char *parse_binary_file(const char *value);				//解析二进制文件
	const char *parse_well(const char *value);						//解析嵌套的json数据
	const char *parse_string(const char *str);						//解析字符串
	const char *parse_string(const char *str,char* &name);			//解析字符串
	const char *parse_number(const char *num);						//解析数字
	const char *skip(const char *in);								//忽略空字符
	bool        IsComment(const char *in);
	const char *RemoveComment(const char *in);
	bool        print_value(TBuffer &out,int depth,int fmt)const;	//将json序列化
	bool        print_string(TBuffer &out)const;					//打印字符串
	char *      print_string_ptr(const char *str)const;				//打印字符串
	bool        print_binary(TBuffer &out)const;					//打印二进制数据
	bool        print_array(TBuffer &out,int depth,int fmt)const;	//打印数组
	bool        print_object(TBuffer &out,int depth,int fmt)const;	//打印对象
	bool        print_number(TBuffer &out)const;					//打印数字
private:
	const char * getNumber(const char *value,unsigned int &number);	
private:
	union ValueHolder
	{
		INT64 int64;												//存储64位数字
		double real_;												//存储double数据
		struct BData												//存储二进制数据,只存储小于10M的数据				
		{
			unsigned int length_;									//数据长度
			char *define_;											//存储数据			
		}bdata_;	
		struct FData 												//存储二进制数据,只存储大于10M的数据	
		{
			unsigned int length_;									//数据长度
			unsigned int position_;									//数据的位置			
		}fdata_;
		struct WData 					
		{
			char *file_;											//json文件名称
			TJSON *define_;											//存储打开的json			
		}wdata_;	
		struct SingleData 					
		{
			union Single
			{
				int int_;											//存储int数据
				unsigned int uint_;									//存储UInt数据
				bool bool_;											//存储bool数据
				char *string_;										//存储char*数据
				TNAMEJSON *map_;									//存储object数据
				TINDEXJSON *imap_;									//存储排序的整形索引数据
				TJSONS *array_;										//存储array数据
				void *pointer_;										//存储指针数据
			}data_;
			void *define_;											//存储定义数据			
		}data_;		
	} value_;
	TNAMEJSON::iterator iter;										//存储迭代器数据,方便获取本节点的名称
	/*TJSONS::iterator */
	TJSON *father;													//存储父亲节点
	ValueType type_ : 8;											//存储其节点类型	
};

#endif