
#include "stdafx.h"
#include "PMJson.h"
#include <cassert>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <float.h>

#define JSONPUSH if (type_==nullValue)\
	{\
	type_=arrayValue;\
	value_.data_.data_.array_=NewArray();\
	}\
	if (type_==arrayValue)\
	{\
	TJSON *j= Create(data);\
	value_.data_.data_.array_->push_back(j);\
	j->father=this;\
	return j;\
	}\
	return NULL;

const TJSON TJSON::null;
const int TJSON::minInt = int( ~(unsigned int(-1)/2) );;
const int TJSON::maxInt = int( unsigned int(-1)/2 );
const unsigned int TJSON::maxUInt = unsigned int(-1);

static int ReadFile( const char *filePath,char *&content ,unsigned int &length)
{	
	if(filePath==NULL)return -1;
	FILE *fp=fopen(filePath,"rb");
	if(!fp)return NULL;
	fseek(fp,0,SEEK_END);
	length=ftell(fp);
	fseek(fp,0,SEEK_SET);
	content=(char*)malloc(length+1);
	fread(content,1,length,fp);
	fclose(fp);
	content[length]=0;
	return length;
}

static bool WriteFile( const char *filePath,const char *content,unsigned int length )
{
	if(filePath==NULL||content==NULL)return false;
	FILE *fp=fopen(filePath,"wb");
	if(!fp)return false;	
	int len=0;
	if(length>0)len=length;
	else len=strlen(content);
	fwrite(content,1,len,fp);
	fclose(fp);	
	return true;
}
TJSON* TJSON::Create()
{
	return NewJSON();
}

TJSON* TJSON::Create( bool data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;
}

TJSON* TJSON::Create( int data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;	
}

TJSON* TJSON::Create( unsigned int data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;
}

TJSON* TJSON::Create( INT64 data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;
}

TJSON* TJSON::Create( double data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;
}

TJSON* TJSON::Create( const char* data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;
}

TJSON* TJSON::Create( const string &data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;
}

TJSON* TJSON::Create( void *data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;
}

TJSON* TJSON::Create( const TJSON &data )
{
	TJSON* j=NewJSON();
	*j=data;
	return j;
}

TJSONIterator::TJSONIterator( const TNAMEJSON::iterator &state )
{
	map_=state;	
	isArray_=0;
}

TJSONIterator::TJSONIterator( const TJSONS::iterator &state )
{
	array_=state;
	isArray_=1;
}

TJSONIterator::TJSONIterator( const TJSONIterator &other )
{
	isArray_=other.isArray_;
	if(other.isArray_==1)
	{
		array_=other.array_;
	}else if(other.isArray_==0)
	{
		map_=other.map_;
	}
}

TJSONIterator::TJSONIterator():isArray_(-1)
{

}

TJSONIterator& TJSONIterator::operator=( const TJSONIterator &other )
{
	if (this!=&other)
	{
		isArray_=other.isArray_;
		if(other.isArray_)
		{
			array_=other.array_;
		}else if(other.isArray_==0)
		{
			map_=other.map_;
		}
	}
	return *this;
}

bool TJSONIterator::operator==( const TJSONIterator &other )
{
	if(other.isArray_==-1||isArray_==-1)return true;
	if(other.isArray_)
	{
		return array_==other.array_;
	}else 
	{
		return map_==other.map_;
	}
}

bool TJSONIterator::operator!=( const TJSONIterator &other )
{
	if(other.isArray_==-1||isArray_==-1)return false;
	if(other.isArray_)
	{
		return array_!=other.array_;
	}else
	{
		return map_!=other.map_;
	}
}

bool TJSONIterator::operator>( const TJSONIterator &other )
{
	if(other.isArray_)
	{
		return array_>other.array_;
	}
	return false;
}

bool TJSONIterator::operator<( const TJSONIterator &other )
{
	if(other.isArray_)
	{
		return array_<other.array_;
	}
	return false;
}

bool TJSONIterator::operator>=( const TJSONIterator &other )
{
	if(other.isArray_)
	{
		return array_>=other.array_;
	}
	return false;
}

bool TJSONIterator::operator<=( const TJSONIterator &other )
{
	if(other.isArray_)
	{
		return array_<=other.array_;
	}
	return false;
}

int TJSONIterator::operator-( const TJSONIterator &other )
{
	if(other.isArray_)
	{
		return array_-other.array_;
	}
	return 0;
}

TJSON& TJSONIterator::operator*()
{
	if(isArray_)
	{
		return **array_;
	}else
	{
		return *map_->second;
	}
}

TJSON* TJSONIterator::operator->()
{
	if(isArray_)
	{
		return *array_;
	}else
	{
		return map_->second;
	}
}

TJSONIterator& TJSONIterator::operator++()
{
	if(isArray_)
	{
		++array_;
	}else
	{
		++map_;
	}
	return *this;
}

TJSONIterator TJSONIterator::operator++( int )
{
	TJSONIterator temp(*this);
	if(isArray_)
	{
		++array_;
	}else
	{
		++map_;
	}
	return temp;
}
TJSONIterator& TJSONIterator::operator--()
{
	if(isArray_)
	{
		--array_;
	}else
	{
		--map_;
	}
	return *this;
}

TJSONIterator TJSONIterator::operator--( int )
{
	TJSONIterator temp(*this);
	if(isArray_)
	{
		--array_;
	}else
	{
		--map_;
	}
	return temp;
}

string TJSONIterator::GetName()
{
	if(!isArray_)
	{
		return map_->first;
	}
	return "";
}

TJSONIterator TJSONIterator::operator+( int distance )
{
	TJSONIterator temp(*this);
	if(temp.isArray_)
	{
		temp.array_+=distance;
	}else
	{
		if(distance>0)
		{
			for (int i=0;i<distance;i++)
			{
				++temp.map_;
			}
		}else if(distance<0)
		{
			for (int i=0;i>distance;i--)
			{
				--temp.map_;
			}
		}
	}
	return temp;
}

TJSONIterator TJSONIterator::operator-( int distance )
{
	TJSONIterator temp(*this);
	if(temp.isArray_)
	{
		temp.array_-=distance;
	}else
	{
		if(distance>0)
		{
			for (int i=0;i<distance;i++)
			{
				--temp.map_;
			}
		}else if(distance<0)
		{
			for (int i=0;i>distance;i--)
			{
				++temp.map_;
			}
		}
	}
	return temp;
}


TJSONConstIterator::TJSONConstIterator():isArray_(-1)
{

}

TJSONConstIterator::TJSONConstIterator( const TNAMEJSON::const_iterator &state ):isArray_(0)
{
	map_=state;
}

TJSONConstIterator::TJSONConstIterator( const TJSONS::const_iterator &state ):isArray_(1)
{
	array_=state;
}

TJSONConstIterator::TJSONConstIterator( const TJSONConstIterator &other )
{
	isArray_=other.isArray_;
	if(other.isArray_==1)
	{
		array_=other.array_;
	}else if(other.isArray_==0)
	{
		map_=other.map_;
	}
}

TJSONConstIterator& TJSONConstIterator::operator=( const TJSONConstIterator &other )
{
	if (this!=&other)
	{
		isArray_=other.isArray_;
		if(other.isArray_==1)
		{
			array_=other.array_;
		}else if(other.isArray_==0)
		{
			map_=other.map_;
		}
	}
	return *this;
}

bool TJSONConstIterator::operator==( const TJSONConstIterator &other )
{
	if(other.isArray_==-1||isArray_==-1)return true;
	if(other.isArray_)
	{
		return array_==other.array_;
	}else
	{
		return map_==other.map_;
	}
	return 0;
}

bool TJSONConstIterator::operator!=( const TJSONConstIterator &other )
{
	if(other.isArray_==-1||isArray_==-1)return false;
	if(other.isArray_)
	{
		return array_!=other.array_;
	}else
	{
		return map_!=other.map_;
	}
	return 0;
}

bool TJSONConstIterator::operator>( const TJSONConstIterator &other )
{
	if(other.isArray_)
	{
		return array_>other.array_;
	}
	return 0;
}

bool TJSONConstIterator::operator<( const TJSONConstIterator &other )
{
	if(other.isArray_)
	{
		return array_<other.array_;
	}
	return 0;
}

bool TJSONConstIterator::operator>=( const TJSONConstIterator &other )
{
	if(other.isArray_)
	{
		return array_>=other.array_;
	}
	return 0;
}

bool TJSONConstIterator::operator<=( const TJSONConstIterator &other )
{
	if(other.isArray_)
	{
		return array_<=other.array_;
	}
	return 0;
}

int TJSONConstIterator::operator-( const TJSONConstIterator &other )
{
	if(other.isArray_)
	{
		return array_-other.array_;
	}
	return 0;
}

const TJSON& TJSONConstIterator::operator*()
{
	if(isArray_)
	{
		return **array_;
	}else
	{
		return *map_->second;
	}
}

const TJSON* TJSONConstIterator::operator->()
{
	if(isArray_)
	{
		return *array_;
	}else
	{
		return map_->second;
	}
}

TJSONConstIterator& TJSONConstIterator::operator++()
{
	if(isArray_)
	{
		++array_;
	}else
	{
		++map_;
	}
	return *this;
}

TJSONConstIterator TJSONConstIterator::operator++( int )
{
	TJSONConstIterator temp(*this);
	if(isArray_)
	{
		++array_;
	}else
	{
		++map_;
	}
	return temp;
}
TJSONConstIterator& TJSONConstIterator::operator--()
{
	if(isArray_)
	{
		--array_;
	}else
	{
		--map_;
	}
	return *this;
}

TJSONConstIterator TJSONConstIterator::operator--( int )
{
	TJSONConstIterator temp(*this);
	if(isArray_)
	{
		--array_;
	}else
	{
		--map_;
	}
	return temp;
}

std::string TJSONConstIterator::GetName()
{
	if(!isArray_)
	{
		return map_->first;
	}
	return "";
}

TJSONConstIterator TJSONConstIterator::operator+( int distance )
{
	TJSONConstIterator temp(*this);
	if(temp.isArray_)
	{
		temp.array_+=distance;
	}else
	{
		if(distance>0)
		{
			for (int i=0;i<distance;i++)
			{
				++temp.map_;
			}
		}else if(distance<0)
		{
			for (int i=0;i>distance;i--)
			{
				--temp.map_;
			}
		}
	}
	return temp;
}

TJSONConstIterator TJSONConstIterator::operator-( int distance )
{
	TJSONConstIterator temp(*this);
	if(temp.isArray_)
	{
		temp.array_-=distance;
	}else
	{
		if(distance>0)
		{
			for (int i=0;i<distance;i++)
			{
				--temp.map_;
			}
		}else if(distance<0)
		{
			for (int i=0;i>distance;i--)
			{
				++temp.map_;
			}
		}
	}
	return temp;
}


TJSON::TJSON():type_(nullValue),father(NULL)
{
	value_.int64=0;
}

TJSON::TJSON( bool data ):type_(booleanValue),father(NULL)
{
	value_.int64=0;
	value_.data_.data_.bool_=0;
}

TJSON::TJSON( int data ):type_(intValue),father(NULL)
{
	value_.int64=0;
	value_.data_.data_.int_=data;
}

TJSON::TJSON( unsigned int data ):type_(uintValue),father(NULL)
{
	value_.int64=0;
	value_.data_.data_.uint_=data;
}

TJSON::TJSON( INT64 data ):type_(int64Value),father(NULL)
{
	value_.int64=data;
}

TJSON::TJSON( double data ):type_(realValue),father(NULL)
{
	value_.real_=data;	
}

TJSON::TJSON( const char* data ):type_(stringValue),father(NULL)
{	
	value_.int64=0;
	if (data)
	{
		value_.data_.data_.string_=NewString(data);
	}
}

TJSON::TJSON( const string &data ):type_(stringValue),father(NULL)
{
	value_.int64=0;
	value_.data_.data_.string_=NewString(data.c_str());
}

TJSON::TJSON( void *data ):type_(pointerValue),father(NULL)
{
	value_.int64=0;
	value_.data_.data_.pointer_=data;
}

TJSON::TJSON( const TJSON &data )
{
	*this=data;	
}

TJSON::TJSON( TBuffer &data ):type_(binaryValue),father(NULL)
{
	value_.int64=0;
	value_.bdata_.define_=(char*)malloc(data.useLen);
	memcpy(value_.bdata_.define_,data.buffer,data.useLen);
	value_.bdata_.length_=data.useLen;
}

TJSON & TJSON::operator=( bool data )
{
	Clear();
	type_=booleanValue;
	value_.data_.data_.bool_=data;
	return *this;
}

TJSON & TJSON::operator=( int data )
{
	Clear();
	type_=intValue;
	value_.data_.data_.int_=data;
	return *this;
}

TJSON & TJSON::operator=( unsigned int data )
{
	Clear();
	type_=uintValue;
	value_.data_.data_.uint_=data;
	return *this;
}

TJSON & TJSON::operator=( INT64 data )
{
	Clear();
	type_=int64Value;
	value_.int64=data;
	return *this;
}

TJSON & TJSON::operator=( double data )
{
	Clear();
	type_=realValue;
	value_.real_=data;
	return *this;
}

TJSON & TJSON::operator=( const char* data )
{
	Clear();
	type_=stringValue;
	value_.data_.data_.string_=NewString(data);
	return *this;
}

TJSON & TJSON::operator=( const string &data )
{
	Clear();
	type_=stringValue;
	value_.data_.data_.string_=NewString(data.c_str());
	return *this;
}

TJSON & TJSON::operator=( void *data )
{
	Clear();
	type_=pointerValue;
	value_.data_.data_.pointer_=data;
	return *this;
}

TJSON & TJSON::operator=( TBuffer &data )
{
	Clear();
	type_=binaryValue;
	value_.bdata_.define_=(char*)malloc(data.useLen);
	memcpy(value_.bdata_.define_,data.buffer,data.useLen);
	value_.bdata_.length_=data.useLen;
	return *this;
}

TJSON & TJSON::operator=( const TJSON &data )
{
	if (this!=&data)
	{
		Clear();
		type_=data.type_;
		value_.int64=data.value_.int64;
		switch(data.type_)
		{
		case arrayValue:
			{
				TJSONS *array = NewArray();
				value_.data_.data_.array_=array;
				TJSON *child=NULL;
				TJSONS::const_iterator it = data.value_.data_.data_.array_->begin();
				TJSONS::const_iterator itEnd = data.value_.data_.data_.array_->end();				
				for (;it!=itEnd;it++)
				{
					child=NewJSON();
					*child=**it;
					child->father=this;
					array->push_back(child);
				}
			}			
			break;
		case objectValue:
			{
				TNAMEJSON *map_ = NewMap();
				value_.data_.data_.map_=map_;
				TJSON *child=NULL;
				TNAMEJSON::const_iterator it = data.value_.data_.data_.map_->begin();
				TNAMEJSON::const_iterator itEnd = data.value_.data_.data_.map_->end();	
				TNAMEJSON::_Pairib pb;
				for (;it!=itEnd;it++)
				{
					child=NewJSON();
					*child=*it->second;
					child->father=this;
					pb=map_->insert(make_pair(it->first,child));
					child->iter=pb.first;
				}
			}
			break;
		case stringValue:
			if(data.value_.data_.data_.string_)value_.data_.data_.string_=NewString(data.value_.data_.data_.string_);
			break;
		}		
	}
	return *this;
}

bool TJSON::operator==( bool data )const
{
	if (type_==booleanValue)return value_.data_.data_.bool_==data;
	return false;
}

bool TJSON::operator==( int data )const
{
	if (type_==intValue)return value_.data_.data_.int_==data;
	return false;
}

bool TJSON::operator==( unsigned int data )const
{
	if (type_==uintValue)return value_.data_.data_.uint_==data;
	return false;
}

bool TJSON::operator==( INT64 data )const
{
	if (type_==int64Value)return value_.int64==data;
	return false;
}

bool TJSON::operator==( double data )const
{
	if (type_==realValue)return value_.real_==data;
	return false;
}

bool TJSON::operator==( const char* data )const
{
	if (type_==stringValue)
	{
		if (value_.data_.data_.string_==data)return true;
		if (value_.data_.data_.string_==NULL||data==NULL)return false;
		return 0==strcmp(value_.data_.data_.string_,data);
	}
	return false;
}

bool TJSON::operator==( const string &data )const
{
	if (type_==stringValue)
	{
		if (value_.data_.data_.string_==NULL)return false;
		return data==value_.data_.data_.string_;
	}
	return false;
}

bool TJSON::operator==( void *data )const
{
	if (type_==pointerValue)return value_.data_.data_.pointer_==data;
	return false;
}

bool TJSON::operator==( const TJSON &other )const
{
	if (this==&other)return true;
	if (type_==other.type_)
	{
		switch (type_)
		{
		case nullValue:
			return other.GetType()==nullValue;
		case booleanValue:
			return *this==other.ToBool();
		case intValue:
			return *this==other.ToInt() &&value_.data_.define_ == other.value_.data_.define_;
		case uintValue:
			return *this==other.ToUInt();
		case int64Value:
			return *this==other.ToInt64();
		case realValue:
			return *this==other.ToDouble();
		case pointerValue:
			return *this==other.ToPointer();
		case stringValue:
			return *this==other.ToCString();
		case arrayValue:
		case objectValue:
			{
				if (GetChildCount()!=other.GetChildCount())return false;
				TJSON::const_iterator it=begin();
				TJSON::const_iterator itEnd=end();
				TJSON::const_iterator it1=other.begin();
				TJSON::const_iterator itEnd1=other.end();
				for (;it!=itEnd;it++,it1++)
				{
					if (!(*it==*it1))	return false;
				}
				return true;
			}
		}
	}
	return false;
}

bool TJSON::operator!=( bool data )const
{
	return !(*this==data);
}

bool TJSON::operator!=( int data )const
{
	return !(*this==data);
}

bool TJSON::operator!=( unsigned int data )const
{
	return !(*this==data);
}

bool TJSON::operator!=( INT64 data )const
{
	return !(*this==data);
}

bool TJSON::operator!=( double data )const
{
	return !(*this==data);
}

bool TJSON::operator!=( const char* data )const
{
	return !(*this==data);
}

bool TJSON::operator!=( const string &data )const
{
	return !(*this==data);
}

bool TJSON::operator!=( void *data )const
{
	return !(*this==data);
}

bool TJSON::operator!=( const TJSON &other )const
{
	return !(*this==other);
}

bool TJSON::operator<=( int data )const
{
	if (type_==intValue)return data<=value_.data_.data_.int_;
	return false;
}

bool TJSON::operator<=( unsigned int data )const
{
	if (type_==uintValue)return data<=value_.data_.data_.uint_;
	return false;
}

bool TJSON::operator<=( INT64 data )const
{
	if (type_==int64Value)return data<=value_.int64;
	return false;
}

bool TJSON::operator<=( double data )const
{
	if (type_==realValue)return data<=value_.real_;
	return false;
}

bool TJSON::operator<=( const char* data )const
{
	if (type_==stringValue&&data&&value_.data_.data_.string_)return 0<=strcmp(data,value_.data_.data_.string_);
	return false;
}

bool TJSON::operator<=( const string &data )const
{
	if (type_==stringValue&&value_.data_.data_.string_)return data<=value_.data_.data_.string_;
	return false;
}

bool TJSON::operator>=( int data )const
{
	if (type_==intValue)return data>=value_.data_.data_.int_;
	return false;
}

bool TJSON::operator>=( unsigned int data )const
{
	if (type_==uintValue)return data>=value_.data_.data_.uint_;
	return false;
}

bool TJSON::operator>=( INT64 data )const
{
	if (type_==int64Value)return data>=value_.int64;
	return false;
}

bool TJSON::operator>=( double data )const
{
	if (type_==realValue)return data>=value_.real_;
	return false;
}

bool TJSON::operator>=( const char* data )const
{
	if (type_==stringValue&&data&&value_.data_.data_.string_)return 0>=strcmp(data,value_.data_.data_.string_);
	return false;
}

bool TJSON::operator>=( const string &data )const
{
	if (type_==stringValue&&value_.data_.data_.string_)return data>=value_.data_.data_.string_;
	return false;
}
bool TJSON::operator<( int data )const
{
	if (type_==intValue)return data<value_.data_.data_.int_;
	return false;
}

bool TJSON::operator<( unsigned int data )const
{
	if (type_==uintValue)return data<value_.data_.data_.uint_;
	return false;
}

bool TJSON::operator<( INT64 data )const
{
	if (type_==int64Value)return data<value_.int64;
	return false;
}

bool TJSON::operator<( double data )const
{
	if (type_==realValue)return data<value_.real_;
	return false;
}

bool TJSON::operator<( const char* data )const
{
	if (type_==stringValue&&data&&value_.data_.data_.string_)return 0<strcmp(data,value_.data_.data_.string_);
	return false;
}

bool TJSON::operator<( const string &data )const
{
	if (type_==stringValue&&value_.data_.data_.string_)return data<value_.data_.data_.string_;
	return false;
}

bool TJSON::operator>( int data )const
{
	if (type_==intValue)return data>value_.data_.data_.int_;
	return false;
}

bool TJSON::operator>( unsigned int data )const
{
	if (type_==uintValue)return data>value_.data_.data_.uint_;
	return false;
}

bool TJSON::operator>( INT64 data )const
{
	if (type_==int64Value)return data>value_.int64;
	return false;
}

bool TJSON::operator>( double data )const
{
	if (type_==realValue)return data>value_.real_;
	return false;
}

bool TJSON::operator>( const char* data )const
{
	if (type_==stringValue&&data&&value_.data_.data_.string_)return 0>strcmp(data,value_.data_.data_.string_);
	return false;
}

bool TJSON::operator>( const string &data )const
{
	if (type_==stringValue&&value_.data_.data_.string_)return data>value_.data_.data_.string_;
	return false;
}

bool TJSON::operator>( const TJSON &other ) const
{
	if (this==&other)return true;
	if (type_==other.type_)
	{
		switch (type_)
		{
		case booleanValue:
			return *this>other.ToBool();
		case intValue:
			return *this>other.ToInt();
		case uintValue:
			return *this>other.ToUInt();
		case int64Value:
			return *this>other.ToInt64();
		case realValue:
			return *this>other.ToDouble();
		case pointerValue:
			return *this>other.ToPointer();
		case stringValue:
			return *this>other.ToCString();
		}
	}
	return false;
}

bool TJSON::operator<( const TJSON &other ) const
{
	if (this==&other)return true;
	if (type_==other.type_)
	{
		switch (type_)
		{
		case booleanValue:
			return *this<other.ToBool();
		case intValue:
			return *this<other.ToInt();
		case int64Value:
			return *this<other.ToInt64();
		case uintValue:
			return *this<other.ToUInt();
		case realValue:
			return *this<other.ToDouble();
		case pointerValue:
			return *this<other.ToPointer();
		case stringValue:
			return *this<other.ToCString();
		}
	}
	return false;
}

bool TJSON::operator<=( const TJSON &other ) const
{
	if (this==&other)return true;
	if (type_==other.type_)
	{
		switch (type_)
		{
		case booleanValue:
			return *this<=other.ToBool();
		case intValue:
			return *this<=other.ToInt();
		case uintValue:
			return *this<=other.ToUInt();
		case int64Value:
			return *this<=other.ToInt64();
		case realValue:
			return *this<=other.ToDouble();
		case pointerValue:
			return *this<=other.ToPointer();
		case stringValue:
			return *this<=other.ToCString();
		}
	}
	return false;
}

bool TJSON::operator>=( const TJSON &other ) const
{
	if (this==&other)return true;
	if (type_==other.type_)
	{
		switch (type_)
		{
		case booleanValue:
			return *this>=other.ToBool();
		case intValue:
			return *this>=other.ToInt();
		case uintValue:
			return *this>=other.ToUInt();
		case int64Value:
			return *this>=other.ToInt64();
		case realValue:
			return *this>=other.ToDouble();
		case pointerValue:
			return *this>=other.ToPointer();
		case stringValue:
			return *this>=other.ToCString();
		}
	}
	return false;
}

ValueType TJSON::GetType() const
{
	return type_;	
}

bool TJSON::IsNull() const
{
	return type_==nullValue;
}

bool TJSON::IsBool() const
{
	return type_==booleanValue;
}

bool TJSON::IsInt() const
{
	return type_==intValue;
}

bool TJSON::IsUInt() const
{
	return type_==uintValue;
}

bool TJSON::IsInt64() const
{
	return type_==int64Value;
}

bool TJSON::IsDouble() const
{
	return type_==realValue;
}

bool TJSON::IsString() const
{
	return type_==stringValue;
}

bool TJSON::IsPointer() const
{
	return type_==pointerValue;
}

bool TJSON::ToBool() const
{
	switch (type_)
	{
	case booleanValue:
		return value_.data_.data_.bool_;
	case intValue:
		return value_.data_.data_.int_!=0;
	case uintValue:
		return value_.data_.data_.uint_!=0;
	case int64Value:
		return value_.int64!=0;
	case realValue:
		return (int)value_.real_!=0;
	case pointerValue:
		return value_.data_.data_.int_!=0;
	case stringValue:
		if(value_.data_.data_.string_)return string("true")==value_.data_.data_.string_;
	}
	return false;	
}

int TJSON::ToInt() const
{
	switch (type_)
	{
	case booleanValue:
		return value_.data_.data_.bool_;
	case intValue:
		return value_.data_.data_.int_;
	case uintValue:
		return (int)value_.data_.data_.uint_;
	case int64Value:
		return (int)value_.int64;
	case realValue:
		return (int)value_.real_;
	case pointerValue:
		return value_.data_.data_.int_;
	case stringValue:
		if(value_.data_.data_.string_)return atoi(value_.data_.data_.string_);
	}
	return 0;
}

unsigned int TJSON::ToUInt() const
{
	switch (type_)
	{
	case booleanValue:
		return value_.data_.data_.bool_;
	case uintValue:
		return value_.data_.data_.uint_;
	case intValue:
		return (unsigned int)value_.data_.data_.int_;
	case int64Value:
		return (unsigned int)value_.int64;
	case realValue:
		return (unsigned int)value_.real_;
	case pointerValue:
		return value_.data_.data_.int_;
	case stringValue:
		if(value_.data_.data_.string_)return (unsigned int)atoi(value_.data_.data_.string_);
	}
	return 0;
}

INT64 TJSON::ToInt64() const
{
	switch (type_)
	{
	case booleanValue:
		return value_.data_.data_.bool_;
	case intValue:
		return value_.data_.data_.int_;
	case uintValue:
		return value_.data_.data_.uint_;
	case int64Value:
		return value_.int64;
	case realValue:
		return (INT64)value_.real_;
	case pointerValue:
		return value_.data_.data_.int_;
	case stringValue:
		if(value_.data_.data_.string_)return atoi(value_.data_.data_.string_);
	}
	return 0;
}

double TJSON::ToDouble() const
{
	switch (type_)
	{
	case booleanValue:
		return value_.data_.data_.bool_;
	case intValue:
		return value_.data_.data_.int_;
	case uintValue:
		return value_.data_.data_.uint_;
	case int64Value:
		return (double)value_.int64;
	case realValue:
		return value_.real_;
	case pointerValue:
		return value_.data_.data_.int_;
	case stringValue:
		if(value_.data_.data_.string_)return atof(value_.data_.data_.string_);
	}
	return 0;
}

char* TJSON::ToCString() const
{
	if(type_==stringValue)return value_.data_.data_.string_;
	return NULL;
}

std::string TJSON::ToString() const
{
	switch (type_)
	{
	case booleanValue:
		return value_.data_.data_.bool_?"true":"false";
	case intValue:
		{
			char buf[20]={0};
			itoa(value_.data_.data_.int_,buf,10);
			return buf;
		}	
	case uintValue:
		{
			char buf[20]={0};
			ultoa(value_.data_.data_.uint_,buf,10);
			return buf;
		}
	case int64Value:
		{			
			char buf[30]={0};
			sprintf(buf, "%lld", value_.int64) ;
			return buf;
		}		
	case realValue:
		{			
			char buf[20]={0};	
			if(value_.real_-(long)value_.real_==0)
			{
				sprintf(buf, "%ld", (long)value_.real_) ;
			}else
			{
				sprintf(buf, "%lf", value_.real_) ;
				for (int i=19;i>0;i--)
				{
					if(buf[i]=='0')
					{
						buf[i]=0;
					}else if(buf[i]==0)
					{
					}else
					{
						break;
					}
				}
			}			
			return buf;
		}		
	case stringValue:
		if(value_.data_.data_.string_)return value_.data_.data_.string_;
	}	
	return "";
}

void* TJSON::ToPointer() const
{
	if (type_==pointerValue)return value_.data_.data_.pointer_;
	return NULL;
}

TBuffer TJSON::ToBinary() const
{
	TBuffer buffer;
	if (type_==binaryValue)
	{
		buffer.buffer=value_.bdata_.define_;
		buffer.size=value_.bdata_.length_;
		buffer.useLen=buffer.size;
	}
	return buffer;
}

void TJSON::SetDefine( void *data )
{
	if (type_!=int64Value&&type_!=realValue)
	{
		value_.data_.define_ = data;
	}
}

void* TJSON::GetDefine() const
{
	if (type_!=int64Value&&type_!=realValue)
	{
		return value_.data_.define_;
	}
	return NULL;
}

string TJSON::GetName() const
{
	if (father&&father->type_==objectValue&&iter != father->value_.data_.data_.map_->end())
	{
		return iter->first;
	}
	return "";
}

int TJSON::GetChildCount() const
{
	if (type_==objectValue)return value_.data_.data_.map_->size();
	if (type_==arrayValue)return value_.data_.data_.array_->size();
	return 0;
}

void TJSON::SetChildCount(int n)
{
	assert(type_==nullValue||type_==arrayValue);
	if (type_==nullValue)
	{
		type_=arrayValue;
		value_.data_.data_.array_=NewArray();
	}
	TJSON *j;
	for (int i=0;i<n;i++)
	{
		j=NewJSON();
		value_.data_.data_.array_->push_back(j);
		j->father=this;	
	}	
}

TJSON * TJSON::GetChild() const
{
	if(GetChildCount()>0)
	{
		if (type_==objectValue)return value_.data_.data_.map_->begin()->second;
		if (type_==arrayValue)return *value_.data_.data_.array_->begin();
	}	
	return NULL;
}

TJSON * TJSON::GetFather() const
{
	return father;
}

TJSON * TJSON::GetLastChild() const
{
	if(GetChildCount()>0)
	{
		if (type_==objectValue)return (--value_.data_.data_.map_->end())->second;
		if (type_==arrayValue)
		{
			TJSONS::iterator it=value_.data_.data_.array_->end();
			return *(--it);
		}
	}	
	return NULL;
}

bool TJSON::IsMember( const char *key ) const
{
	char *p=(char*)key;
	if (type_==objectValue)return value_.data_.data_.map_->count(p)>0;
	return false;
}

bool TJSON::IsMember( const std::string &key ) const
{
	return IsMember(key.c_str());
}

TJSON* TJSON::Push( int data )
{
	JSONPUSH
}

TJSON* TJSON::Push( unsigned int data )
{
	JSONPUSH
}

TJSON* TJSON::Push( INT64 data )
{
	JSONPUSH
}

TJSON* TJSON::Push( double data )
{
	JSONPUSH
}

TJSON* TJSON::Push( const char* data )
{
	JSONPUSH
}

TJSON* TJSON::Push( const string &data )
{
	JSONPUSH
}

TJSON* TJSON::Push( const TJSON &data )
{
	JSONPUSH
}

void TJSON::Push( int index , TJSON *json )
{
	assert(type_==nullValue||type_==indexValue);
	if (type_==nullValue)
	{
		type_=indexValue;
		value_.data_.data_.imap_= new TINDEXJSON;
	}
	(*value_.data_.data_.imap_)[index] = json;
}

void TJSON::Insert( int index , TJSON *json )
{
	assert(type_==nullValue||type_==arrayValue);
	if (type_==nullValue)
	{
		type_=arrayValue;
		value_.data_.data_.array_= new TJSONS;
	}
	value_.data_.data_.array_->insert(value_.data_.data_.array_->begin()+index,json);	
}

void TJSON::Insert( const string &name , TJSON *json )
{
	assert(type_==nullValue||type_==objectValue);
	if (type_==nullValue)
	{
		type_=objectValue;
		value_.data_.data_.map_= new TNAMEJSON;
	}
	(*value_.data_.data_.map_)[name] = json;	
}

void TJSON::Push( TJSON *data )
{
	assert(type_==nullValue||type_==arrayValue);
	if (type_==nullValue)
	{
		type_=arrayValue;
		value_.data_.data_.array_=NewArray();
	}
	value_.data_.data_.array_->push_back(data);
	//data->father = this;
}

TJSON& TJSON::Push()
{
	assert(type_==nullValue||type_==arrayValue);
	if (type_==nullValue)
	{
		type_=arrayValue;
		value_.data_.data_.array_=NewArray();
	}
	TJSON *data =NewJSON();
	value_.data_.data_.array_->push_back(data);
	data->father = this;
	return *data;
}

TJSON* TJSON::Push( TBuffer &data )
{
	assert(type_==nullValue||type_==arrayValue);
	if (type_==nullValue)
	{
		type_=arrayValue;
		value_.data_.data_.array_=NewArray();
	}
	TJSON *jnData =new TJSON();	
	jnData->type_ = binaryValue;
	jnData->value_.bdata_.define_ = data.buffer;
	jnData->value_.bdata_.length_ = data.useLen;
	value_.data_.data_.array_->push_back(jnData);
	jnData->father = this;
	return jnData;
}

void TJSON::Push( TRect<int> &data )
{
	if (type_==nullValue)
	{
		type_=arrayValue;
		value_.data_.data_.array_=NewArray();
	}
	if (type_==arrayValue)
	{
		TJSON *j= Create(data.left);
		value_.data_.data_.array_->push_back(j);
		j->father=this;
		j= Create(data.top);
		value_.data_.data_.array_->push_back(j);
		j->father=this;
		j= Create(data.right);
		value_.data_.data_.array_->push_back(j);
		j->father=this;
		j= Create(data.bottom);
		value_.data_.data_.array_->push_back(j);
		j->father=this;
	}
}

TJSON* TJSON::Front()
{
	if (type_==arrayValue&&!value_.data_.data_.array_->empty())
	{
		return value_.data_.data_.array_->front();
	}
	return NULL;
}

const TJSON* TJSON::Front() const
{
	if (type_==arrayValue&&!value_.data_.data_.array_->empty())
	{
		return value_.data_.data_.array_->front();
	}
	return NULL;
}
TJSON* TJSON::Back()
{
	if (type_==arrayValue&&!value_.data_.data_.array_->empty())
	{
		return value_.data_.data_.array_->back();
	}
	return NULL;
}

const TJSON* TJSON::Back() const
{
	if (type_==arrayValue&&!value_.data_.data_.array_->empty())
	{
		return value_.data_.data_.array_->back();
	}
	return NULL;
}
void TJSON::Pop(bool bfree)
{
	if (type_==arrayValue&&!value_.data_.data_.array_->empty())
	{
		if(bfree)DeleteJSON(value_.data_.data_.array_->back());
		value_.data_.data_.array_->pop_back();
	}	
}

TJSON * TJSON::NewJSON()
{
	return  new TJSON;	
}

char * TJSON::NewString(const char *data)
{
	if(!data)return NULL;
	return strdup(data);
}

TNAMEJSON * TJSON::NewMap()
{
	return new TNAMEJSON;
}

TJSONS * TJSON::NewArray()
{
	return new TJSONS;
}

void TJSON::DeleteString()
{
	if(value_.data_.data_.string_)
	{
		free(value_.data_.data_.string_);
		value_.data_.data_.string_=NULL;
	}
}

void TJSON::DeleteBinary()
{
	if(value_.bdata_.define_)
	{
		delete[] value_.bdata_.define_;
		value_.bdata_.define_=NULL;
	}
	value_.bdata_.length_=0;
}

void TJSON::DeleteMap()
{
	if(value_.data_.data_.map_)
	{
		TNAMEJSON::iterator it = value_.data_.data_.map_->begin();
		TNAMEJSON::iterator itEnd = value_.data_.data_.map_->end();	
		for (;it!=itEnd;it++)
		{
			DeleteJSON(it->second);			
		}
		delete value_.data_.data_.map_;
		value_.data_.data_.map_=NULL;
	}
}

void TJSON::DeleteArray()
{
	if(value_.data_.data_.array_)
	{
		TJSONS::iterator it = value_.data_.data_.array_->begin();
		TJSONS::iterator itEnd = value_.data_.data_.array_->end();	
		for (;it!=itEnd;it++)
		{
			DeleteJSON(*it);
		}
		delete value_.data_.data_.array_;
		value_.data_.data_.array_=NULL;
	}
}

void TJSON::DeleteJSON( TJSON *json )
{
	json->Clear();
	delete json;
}

void TJSON::DeleteItem(unsigned int index ,bool bfree)
{
	if (type_==arrayValue&&index<value_.data_.data_.array_->size())
	{
		TJSONS::iterator it=value_.data_.data_.array_->begin()+index;
		if(bfree)DeleteJSON(*it);
		value_.data_.data_.array_->erase(it);
	}
}

void TJSON::DeleteItem( const char *key ,bool bfree)
{
	if (type_==objectValue)
	{
		TNAMEJSON::iterator it=value_.data_.data_.map_->find(key);
		if(it!=value_.data_.data_.map_->end())
		{
			if(bfree)DeleteJSON(it->second);
			value_.data_.data_.map_->erase(it);
		}
	}
}

void TJSON::DeleteItem( const string &key ,bool bfree)
{
	if (type_==objectValue)
	{
		TNAMEJSON::iterator it=value_.data_.data_.map_->find(key);
		if(it!=value_.data_.data_.map_->end())
		{
			if(bfree)DeleteJSON(it->second);
			value_.data_.data_.map_->erase(it);
		}
	}
}

void TJSON::DeleteItem( TJSON *item,bool bfree/*=true*/ )
{
	if (type_==objectValue)
	{
		if(item->iter!=value_.data_.data_.map_->end())
		{			
			value_.data_.data_.map_->erase(item->iter);
		}		
	}else if (type_==arrayValue)
	{
		for (TJSONS::iterator it = value_.data_.data_.array_->begin();it != value_.data_.data_.array_->end();it++)
		{
			if(*it == item)
			{
				value_.data_.data_.array_->erase(it);
				break;
			}
		}
	}
	if(bfree)DeleteJSON(item);
}

TJSON& TJSON::operator[](unsigned int index )
{
	return At(index);
}

TJSON& TJSON::operator[]( const string &key )
{
	return At(key);
}

const TJSON& TJSON::operator[](unsigned int index ) const
{
	assert( type_ == arrayValue );
	assert( 0<=index&&value_.data_.data_.array_->size()>index);
	return *((*value_.data_.data_.array_)[index]);	
}

const TJSON& TJSON::operator[]( const string &key ) const
{
	assert( type_ == objectValue );
	TNAMEJSON::const_iterator it = value_.data_.data_.map_->find(key);
	assert( it!=value_.data_.data_.map_->end() );
	return *it->second;
}

TJSON* TJSON::Find( const char *key )
{
	if ( type_ == objectValue )
	{
		TNAMEJSON::iterator it = value_.data_.data_.map_->find(key);
		if (it!=value_.data_.data_.map_->end())
		{		
			return it->second;
		}
	}
	return NULL;
}

TJSON* TJSON::Find( const string &key )
{
	if ( type_ == objectValue )
	{
		TNAMEJSON::iterator it = value_.data_.data_.map_->find(key);
		if (it!=value_.data_.data_.map_->end())
		{		
			return it->second;
		}
	}
	return NULL;
}

const TJSON* TJSON::Find( const char *key ) const
{
	if ( type_ == objectValue )
	{
		TNAMEJSON::const_iterator it = value_.data_.data_.map_->find(key);
		if (it!=value_.data_.data_.map_->end())
		{		
			return it->second;
		}
	}
	return NULL;
}

const TJSON* TJSON::Find( const string &key ) const
{
	if ( type_ == objectValue )
	{
		TNAMEJSON::const_iterator it = value_.data_.data_.map_->find(key);
		if (it!=value_.data_.data_.map_->end())
		{		
			return it->second;
		}
	}
	return NULL;
}

TJSON::Members TJSON::GetMemberNames() const
{
	assert( type_ == nullValue  ||  type_ == objectValue );
	if ( type_ == nullValue )
		return TJSON::Members();
	Members members;
	members.reserve( value_.data_.data_.map_->size() );
	TNAMEJSON::const_iterator it = value_.data_.data_.map_->begin();
	TNAMEJSON::const_iterator itEnd = value_.data_.data_.map_->end();
	for ( ; it != itEnd; ++it )
		members.push_back( std::string( (*it).first.c_str() ) );
	return members;
}

TJSON::iterator TJSON::begin()
{	
	if (type_ == arrayValue)
	{
		return iterator(value_.data_.data_.array_->begin());
	}else if( type_ == objectValue )
	{
		return iterator(value_.data_.data_.map_->begin());
	}
	return iterator();
}

TJSON::const_iterator TJSON::begin() const
{
	if (type_ == arrayValue)
	{
		return const_iterator(value_.data_.data_.array_->begin());
	}else if( type_ == objectValue )
	{
		return const_iterator(value_.data_.data_.map_->begin());
	}
	return const_iterator();	
}

TJSON::iterator TJSON::end()
{
	//assert( type_ == arrayValue  ||  type_ == objectValue );
	if (type_ == arrayValue)
	{
		return iterator(value_.data_.data_.array_->end());
	}else if( type_ == objectValue )
	{
		return iterator(value_.data_.data_.map_->end());
	}
	return iterator();
}

TJSON::const_iterator TJSON::end() const
{
	if (type_ == arrayValue)
	{
		return const_iterator(value_.data_.data_.array_->end());
	}else if( type_ == objectValue )
	{
		return const_iterator(value_.data_.data_.map_->end());
	}
	return const_iterator();
}

void TJSON::Clear()
{
	switch (type_)
	{
	case arrayValue:
		DeleteArray();
		break;
	case objectValue:
		DeleteMap();
		break;
	case stringValue:
		DeleteString();
		break;
	case binaryValue:
		DeleteBinary();
		break;
	case indexValue:
		delete value_.data_.data_.imap_;
		break;
	}	
	type_=nullValue;
	value_.int64=0;
}

void TJSON::RemoveAll()
{
	switch (type_)
	{
	case arrayValue:
		value_.data_.data_.array_->clear();
		break;
	case objectValue:
		value_.data_.data_.map_->clear();
		break;
	case indexValue:
		value_.data_.data_.imap_->clear();
		break;
	}
}


TJSON * TJSON::NewArraryObject()
{
	assert(type_==arrayValue);
	TJSON *j=NewJSON();
	value_.data_.data_.array_->push_back(j);
	j->father=this;
	return j;	
}

bool TJSON::Replace(unsigned int index,TJSON *data ,bool bInsert )
{
	if (type_==arrayValue&&data)
	{
		if(0<=index&&value_.data_.data_.array_->size()>index)
		{
			if((*value_.data_.data_.array_)[index]!=data)
			{
				DeleteJSON((*value_.data_.data_.array_)[index]);
				(*value_.data_.data_.array_)[index]=data;
				data->father=this;
			}			
			return true;
		}else if(bInsert)
		{
			data->father=this;
			value_.data_.data_.array_->push_back(data);	
			return true;
		}
	}
	return false;
}

bool TJSON::Replace( const char *name,TJSON *data ,bool bInsert )
{
	if (type_==objectValue&&data)
	{
		TNAMEJSON::iterator it = value_.data_.data_.map_->find(name);	
		if (it!=value_.data_.data_.map_->end())
		{
			if(it->second!=data)
			{
				DeleteJSON(it->second);
				data->iter= it;
				data->father = this;
				it->second=data;
			}

			return true;
		}else if(bInsert)
		{			
			data->father=this;
			data->iter =value_.data_.data_.map_->insert(make_pair(name,data)).first;	
			return true;
		}
	}
	return false;
}

bool TJSON::Replace( const string &name,TJSON *data ,bool bInsert )
{
	if (type_==objectValue&&data)
	{
		TNAMEJSON::iterator it = value_.data_.data_.map_->find(name);	
		if (it!=value_.data_.data_.map_->end())
		{
			if(it->second!=data)
			{
				DeleteJSON(it->second);
				data->iter= it;
				data->father = this;
				it->second=data;
			}			
			return true;
		}else if(bInsert)
		{			
			data->father=this;
			data->iter =value_.data_.data_.map_->insert(make_pair(name,data)).first;	
			return true;
		}
	}
	return false;
}

TJSON * TJSON::Clone( bool bNodeOnly/*=false*/ )
{
	TJSON *j=NewJSON();
	*j=*this;
	return j;
}

bool TJSON::IsArray() const
{
	return type_==arrayValue;
}

bool TJSON::IsObject() const
{
	return type_==objectValue;	
}

bool TJSON::IsBinary() const
{
	return type_==binaryValue;	
}

bool TJSON::Empty()const
{
	if (type_==nullValue)
	{
		return true;
	}else if (type_==arrayValue)
	{
		return value_.data_.data_.array_->empty();
	}else if(type_==objectValue)
	{
		return value_.data_.data_.map_->empty();
	}else if(type_==indexValue)
	{
		return value_.data_.data_.imap_->empty();
	}
	return true;
}

void TJSON::Write( const char *fileName,bool bFormat/*=false*/ ) const
{
	TBuffer buffer;
	buffer.Initialize();
	print_value(buffer,0,bFormat);
	buffer.Write(fileName);
	buffer.Clear();
}

void TJSON::SetObject()
{
	assert(type_==nullValue);
	type_=objectValue;
	value_.data_.data_.map_=NewMap();
}

void TJSON::SetArray()
{
	assert(type_==nullValue);
	type_=arrayValue;
	value_.data_.data_.array_=NewArray();
}

void TJSON::SetBinary(const char *value,unsigned int length)
{
	assert(type_==nullValue);
	type_=binaryValue;
	value_.bdata_.length_=length;
	value_.bdata_.define_=(char*)malloc(length);
	memcpy(value_.bdata_.define_,value,length);
}


void TJSON::parse( std::istream& sin )
{
	std::string doc;
	std::getline(sin, doc, (char)EOF);
	parse(doc.c_str());
}

void TJSON::Parse( const std::string &document )
{
	parse(document.c_str());
}

void TJSON::FromString( const char *data )
{
	parse(data);
}

void TJSON::Read( const char *fileName )
{
	char *data=NULL;
	unsigned int length=0;
	if(-1==::ReadFile(fileName,data,length))return;
	parse(data);
	delete[] data;
}

const char * ep=NULL;	//错误地点
void TJSON::parse( const char *value )
{
	ep=0;parse_value(skip(value));
}
const char *TJSON::parse_number(const char *num)
{
	const char *s=num;int sign=1;
	bool bdouble=false;
	if (*num=='-') sign=-1,num++;
	while (*num>='0' && *num<='9')num++;
	if (*num=='.') {num++;bdouble=true;	while (*num>='0' && *num<='9')num++;}
	if (*num=='e' || *num=='E')		/* Exponent? */
	{	
		bdouble=true;num++;if (*num=='+') num++;	else if (*num=='-') num++;		/* With sign? */
		while (*num>='0' && *num<='9') num++;	/* Number? */
	}
	if (bdouble)
	{		
		char *e;
		type_ = realValue; value_.real_ = strtod(s,&e);
	}else
	{
		INT64 n64=_atoi64(s);
		if(n64>=0)
		{
			if(n64>INT64(maxUInt)){value_.int64=n64;type_=int64Value;}
			else if(n64>INT64(maxInt)){value_.data_.data_.uint_=(unsigned int)n64;type_=uintValue;}
			else
			{
				value_.data_.data_.int_=(int)n64;type_=intValue;
				if(sign==-1)
					value_.data_.define_=(void*)1;
			}
		}else
		{
			if(n64<INT64(minInt)){value_.int64=n64;type_=int64Value;}
			else{value_.data_.data_.int_=(int)n64;type_=intValue;}
		}
		
	}
	return num;
}
const char *TJSON::skip(const char *in) 
{	
loop:
	while (in && *in && (unsigned char)*in<=32) in++; 
	if(in &&*in =='/'&&(*(in+1) =='/'||*(in+1) =='*'))
	{
		in = RemoveComment(in);
	}
	if(in &&*in && ((unsigned char)*in<=32||(*in =='/'&&(*(in+1) =='/'||*(in+1) =='*'))))goto loop;
	return in;
}

const char *TJSON::RemoveComment( const char *in )
{
	in+=2;
	if(*(in-1) =='*')//多行注释
	{
		while(*in)
		{
			if(*in=='*'&&*(in+1)=='/')
			{
				in+=2;
				break;
			}else
			{
				in++;
			}
		}
	}else//单行注释
	{
		while(*in)
		{
			if(*in=='\r')
			{
				in+=*(in+1)=='\n' ? 2:1;
				break;
			}else if(*in =='\n')
			{
				in++;
				break;
			}else
			{
				in++;
			}
		}
	}		
	return in;
}

/* Parse the input text into an unescaped cstring, and populate item. */
const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
const char *TJSON::parse_string(const char *str,char* &name)
{
	const char *ptr=str+1;char *ptr2;char *out;int len=0;unsigned uc;
	if (*str!='\"') {ep=str;return 0;}	/* not a string! */

	while (*ptr!='\"' && *ptr && ++len) if (*ptr++ == '\\') ptr++;	/* Skip escaped quotes. */

	out=(char*)malloc(len+1);	/* This is how long we need for the string, roughly. */
	if (!out) return 0;

	ptr=str+1;ptr2=out;
	while (*ptr!='\"' && *ptr)
	{
		if (*ptr!='\\') *ptr2++=*ptr++;
		else
		{
			ptr++;
			switch (*ptr)
			{
			case 'b': *ptr2++='\b';	break;
			case 'f': *ptr2++='\f';	break;
			case 'n': *ptr2++='\n';	break;
			case 'r': *ptr2++='\r';	break;
			case 't': *ptr2++='\t';	break;
			case 'u':	 /* transcode utf16 to utf8. DOES NOT SUPPORT SURROGATE PAIRS CORRECTLY. */
				sscanf(ptr+1,"%4x",&uc);	/* get the unicode char. */
				len=3;if (uc<0x80) len=1;else if (uc<0x800) len=2;ptr2+=len;

				switch (len) {
			case 3: *--ptr2 =((uc | 0x80) & 0xBF); uc >>= 6;
			case 2: *--ptr2 =((uc | 0x80) & 0xBF); uc >>= 6;
			case 1: *--ptr2 =(uc | firstByteMark[len]);
				}
				ptr2+=len;ptr+=4;
				break;
			default:  *ptr2++=*ptr; break;
			}
			ptr++;
		}
	}
	*ptr2=0;
	if (*ptr=='\"') ptr++;
	name=out;
	return ptr;
}
const char *TJSON::parse_string(const char *str)
{
	char *out=NULL;
	const char *ptr=parse_string(str,out);
	if (0==ptr)return 0;
	value_.data_.data_.string_=out;	
	type_=stringValue;
	return ptr;	
}
/* Parser core - when encountering text, process appropriately. */
const char *TJSON::parse_value(const char *value)
{
	if (!value)						return 0;	/* Fail on null. */
	if (!strncmp(value,"null",4))	{  return value+4; }
	if (!strncmp(value,"false",5))	{ type_=booleanValue; value_.data_.data_.bool_=false; return value+5; }
	if (!strncmp(value,"true",4))	{ type_=booleanValue; value_.data_.data_.bool_=true;  return value+4; }
	if (!strncmp(value,"BF",2))		{ return parse_binary_file(value); }
	if (*value=='\"')				{ return parse_string(value); }
	if (*value=='-' || (*value>='0' && *value<='9'))	{ return parse_number(value); }
	if (*value=='[')				{ return parse_array(value); }
	if (*value=='{')				{ return parse_object(value); }
	if (*value=='B')				{ return parse_binary(value); }
	if (*value=='W')				{ return parse_well(value); }
	ep=value;return 0;	/* failure. */
}

const char * TJSON::parse_binary( const char *value )
{
	unsigned int blenth=0;
	value=getNumber(value+1,blenth);
	if(value==NULL)
	{
		return NULL;
	}
	SetBinary(value+1,blenth);
	value=value+1+blenth;
	return value;
}

const char * TJSON::parse_binary_file( const char *value )
{
	value+=2;
	type_=bFileValue;
	value=getNumber(value,value_.fdata_.length_);
	if(value==NULL)
	{
		return NULL;
	}
	value=getNumber(value+1,value_.fdata_.position_);
	if(value==NULL)
	{
		return NULL;
	}
	return value;
}

const char * TJSON::getNumber( const char *value,unsigned int &number )
{	
	if(*value>='0'&&*value<='9')
	{
		number=0;
		while(*value>='0'&&*value<='9')
		{
			number=number*10+*value-'0';
			value++;
		}
		return value;
	}
	ep=value;
	return NULL;
}

const char * TJSON::parse_well( const char *value )
{
	type_=wellValue;
	unsigned int flength=0;
	value=getNumber(value+1,flength);
	if(value==NULL)
	{
		return NULL;
	}
	value_.wdata_.file_=(char*)malloc(flength+1);
	memcpy(value_.wdata_.file_,value,flength);
	value_.wdata_.file_[flength]=0;
	value+=flength;
	return value;
}

const char *TJSON::parse_array(const char *value)
{
	if (*value!='[')	{ep=value;return 0;}	/* not an array! */
	SetArray();
	value=skip(value+1);
	if (*value==']') return value+1;	/* empty array. */
	TJSON *child=NewArraryObject();
	value=skip(child->parse_value(skip(value)));	/* skip any spacing, get the value. */
	if (!value) return 0;
	while (*value==',')
	{
		child=NewArraryObject();
		value=skip(child->parse_value(skip(value+1)));
		if (!value) return 0;	/* memory fail */
	}

	if (*value==']') return value+1;	/* end of array */
	ep=value;return 0;	/* malformed. */
}

/* Build an object from the text. */
const char *TJSON::parse_object(const char *value)
{
	TJSON *child;
	char *name=NULL;
	if (*value!='{')	{ep=value;return 0;}	/* not an object! */

	SetObject();
	value=skip(value+1);
	if (*value=='}') return value+1;	/* empty object. */
	value=skip(parse_string(skip(value),name));
	if (!value) return 0;

	child=&((*this)[name]);
	free(name);
	if (*value!=':') {ep=value;return 0;}	/* fail! */
	value=skip(child->parse_value(skip(value+1)));	/* skip any spacing, get the value. */
	if (!value) return 0;

	while (*value==',')
	{
		//1、获取名称
		value=skip(parse_string(skip(value+1),name));
		if (!value) return 0;
		child=&((*this)[name]);
		free(name);
		if (*value!=':') {ep=value;return 0;}	/* fail! */
		//2、获取值
		value=skip(child->parse_value(skip(value+1)));	/* skip any spacing, get the value. */
		if (!value) return 0;
	}

	if (*value=='}') return value+1;/* end of object */

	ep=value;return 0;	/* malformed. */
}

/* Render a cJSON item/entity/structure to text. */
TBuffer TJSON::Print(bool format)	const			
{
	TBuffer buffer;
	buffer.Initialize();
	print_value(buffer,0,format);
	buffer.MemoryReduce();
	return buffer;
}
/* Render a value to text. */
bool TJSON::print_value(TBuffer &out,int depth,int fmt)const
{
	bool ret=true;
	switch (type_)
	{
	case nullValue:		out+="null";	break;
	case booleanValue:	if(value_.data_.data_.bool_)out+="true";else out+="false";	break;
	case intValue:	
	case uintValue:	
	case int64Value:	
	case pointerValue:
	case realValue:		ret=print_number(out);break;
	case binaryValue:	ret=print_binary(out);break;
	case stringValue:	ret=print_string(out);break;
	case arrayValue:	ret=print_array(out,depth,fmt);break;
	case objectValue:	ret=print_object(out,depth,fmt);break;
	}
	return ret;
}
bool TJSON::print_string(TBuffer &out)	const
{
	bool ret=false; 
	char *strbuf=print_string_ptr(value_.data_.data_.string_);
	if(NULL!=strbuf)
	{
		out+=strbuf;
		free(strbuf);
		ret=true;
	}
	return ret;
}
char *TJSON::print_string_ptr(const char *str)const
{
	const char *ptr;char *ptr2,*out;int len=0;unsigned char token;

	if (!str) return NULL;
	ptr=str;while ((token=*ptr) && ++len) {if (strchr("\"\\\b\f\n\r\t",token)) len++; else if (token<32) len+=5;ptr++;}

	out=(char*)malloc(len+3);
	if (!out) return 0;

	ptr2=out;ptr=str;
	*ptr2++='\"';
	while (*ptr)
	{
		if ((unsigned char)*ptr>31 && *ptr!='\"' && *ptr!='\\') *ptr2++=*ptr++;
		else
		{
			*ptr2++='\\';
			switch (token=*ptr++)
			{
			case '\\':	*ptr2++='\\';	break;
			case '\"':	*ptr2++='\"';	break;
			case '\b':	*ptr2++='b';	break;
			case '\f':	*ptr2++='f';	break;
			case '\n':	*ptr2++='n';	break;
			case '\r':	*ptr2++='r';	break;
			case '\t':	*ptr2++='t';	break;
			default: sprintf(ptr2,"u%04x",token);ptr2+=5;	break;	/* escape and print */
			}
		}
	}
	*ptr2++='\"';*ptr2++=0;
	return out;
}
/* Render an array to text */
bool TJSON::print_array(TBuffer &out,int depth,int fmt)const
{
	TJSONS *vs=value_.data_.data_.array_;
	out+="[";
	TJSONS::const_iterator it=vs->begin();
	TJSONS::const_iterator itEnd=vs->end();
	for (;it!=itEnd;it++)
	{
		if((*it)->print_value(out,depth,fmt))
		{
			if (*it!=vs->back())
			{
				if(fmt)out+=", ";else out+=",";
			}
		}else
		{
			return false;
		}
	}
	out+="]";
	return true;
}
/* Render an object to text. */
bool TJSON::print_object(TBuffer &out,int depth,int fmt)const
{
	TNAMEJSON *nv=value_.data_.data_.map_;
	char *name=NULL;
	out+="{";if (fmt)out+="\n";depth++;
	TNAMEJSON::const_iterator it=nv->begin();
	TNAMEJSON::const_iterator itEnd=nv->end();
	int i=nv->size();
	for (;it!=itEnd;it++)
	{
		i--;
		name=print_string_ptr(it->first.c_str());
		if(NULL==name)return false;
		if (fmt) for (int j=0;j<depth;j++) out+="\t";
		out+=name;free(name);out+=":";if (fmt) out+="\t";
		if (it->second->print_value(out,depth,fmt))
		{
			if(i>0)out+=",";
			if (fmt) out+="\n";
		}else
		{
			return false;
		}
	}
	if (fmt) for (int i=0;i<depth-1;i++) out+="\t";
	out+="}";
	return true;
}
/* Render the number nicely from the given item into a string. */
bool TJSON::print_number(TBuffer &out)const
{
	char str[64]={0};
	if ( uintValue==type_ )
	{
		sprintf(str,"%u",value_.data_.data_.int_);
	}
	else if ( int64Value==type_ )
	{
		_i64toa(value_.int64,str,10);  
		//sprintf(str,"%lld",value_.int64);
	}
	else if (realValue==type_)
	{
		double d=value_.real_;
		if (fabs(floor(d)-d)<=DBL_EPSILON)			sprintf(str,"%.0f",d);
		else if (fabs(d)<1.0e-6 || fabs(d)>1.0e9)	sprintf(str,"%e",d);
		else										sprintf(str,"%f",d);
	}else
	{
		if((int)value_.data_.define_==1){out+="-";}
		sprintf(str,"%d",value_.data_.data_.int_);
	}
	out+=str;
	return true;
}


bool TJSON::print_binary( TBuffer &out) const
{
	out+="B";
	out+=value_.bdata_.length_;
	out+=" ";
	out.Add(value_.bdata_.define_,value_.bdata_.length_);
	return true;
}

void TJSON::Rename( const string &name )
{
	if(father)
	{
		assert( father->type_ == objectValue );
		value_.data_.data_.map_->erase(GetName());
		iter = value_.data_.data_.map_->insert(make_pair(name,this)).first;
	}
}

TJSON::iterator TJSON::GetIterator()
{
	assert( father && father->type_ == objectValue );
	return iterator(iter);
}

TJSON::const_iterator TJSON::GetIterator() const
{
	assert( father && father->type_ == objectValue );
	return const_iterator(iter);
}

TINDEXJSON & TJSON::GetIndexs()
{
	assert(type_==nullValue||type_==indexValue);
	if (type_==nullValue)
	{
		type_=indexValue;
		value_.data_.data_.array_=NewArray();
	}
	return *value_.data_.data_.imap_;
}

bool TJSON::IsParticular() const
{
	return type_==intValue && value_.data_.data_.int_ ==0 && (int)value_.data_.define_ == 1;
}

void TJSON::SetParticular()
{
	Clear();
	type_=intValue;
	value_.data_.data_.int_ =0;
	value_.data_.define_ = (void*)1;
}

TJSON& TJSON::At( unsigned int index )
{
	assert( type_ == nullValue  ||  type_ == arrayValue );
	if (type_==nullValue)
	{
		value_.data_.data_.array_=NewArray();
		type_=arrayValue;
	}
	if (0<=index&&value_.data_.data_.array_->size()>index)
	{
		return *((*value_.data_.data_.array_)[index]);
	}
	TJSON *j=NewJSON();
	value_.data_.data_.array_->push_back(j);
	j->father=this;
	return *j;
}

const TJSON& TJSON::At( unsigned int index ) const
{
	assert( type_ == arrayValue );
	assert( 0<=index&&value_.data_.data_.array_->size()>index);
	return *((*value_.data_.data_.array_)[index]);	
}

TJSON& TJSON::At( const string &key )
{
	assert( type_ == nullValue  ||  type_ == objectValue );
	if (type_==nullValue)
	{
		value_.data_.data_.map_=NewMap();
		type_=objectValue;
	}
	TJSON *j=NULL;
	TNAMEJSON::iterator it = value_.data_.data_.map_->find(key);
	if (it==value_.data_.data_.map_->end())
	{
		j=NewJSON();
		TNAMEJSON::_Pairib pb = value_.data_.data_.map_->insert(make_pair(key,j));
		j->iter=pb.first;
	}else
	{
		j=it->second;
	}
	j->father=this;
	return *j;
}

const TJSON& TJSON::At( const string &key ) const
{
	assert( type_ == objectValue );
	TNAMEJSON::const_iterator it = value_.data_.data_.map_->find(key);
	assert( it!=value_.data_.data_.map_->end() );
	return *it->second;
}

TRect<int> TJSON::GetRect(unsigned int index) const
{
	assert( type_ == arrayValue );
	if (0<=index&&value_.data_.data_.array_->size()>index+3)
	{
		TJSONS &jsons = (*value_.data_.data_.array_);
		return TRect<int>(jsons[index]->ToInt(), jsons[index+1]->ToInt(),jsons[index+2]->ToInt(),jsons[index+3]->ToInt());
	}else
	{
		return TRect<int>(0,0,0,0);
	}
}

TPoint<int> TJSON::GetPoint(unsigned int index) const
{
	assert( type_ == arrayValue );
	if (0<=index&&value_.data_.data_.array_->size()>index+3)
	{
		TJSONS &jsons = (*value_.data_.data_.array_);
		return TPoint<int>(jsons[index]->ToInt(),jsons[index+1]->ToInt());
	}else
	{
		return TPoint<int>(0,0);
	}
}

TSize<int> TJSON::GetSize( unsigned int index ) const
{
	assert( type_ == arrayValue );
	if (0<=index&&value_.data_.data_.array_->size()>index+3)
	{
		TJSONS &jsons = (*value_.data_.data_.array_);
		return TSize<int>(jsons[index]->ToInt(),jsons[index+1]->ToInt());
	}else
	{
		return TSize<int>(0,0);
	}
}

void TJSON::DeleteRange( TJSON &jnRange )
{
	if(jnRange.type_ == nullValue || type_ == nullValue)return;
	assert(type_ == arrayValue && jnRange.type_ == arrayValue);
	set<TJSON*> setRange;
	TJSONS *jnDeleteRange = jnRange.value_.data_.data_.array_;
	for (TJSONS::iterator it = jnDeleteRange->begin(); it != jnDeleteRange->end(); it++)
	{
		if(!(*it)->IsNull())
		{
			setRange.insert(*it);
		}
	}
	TJSONS *newArray = NewArray();
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSON  *jnData;
	for (TJSONS::iterator it = oldArray->begin(); it != oldArray->end(); it++)
	{
		jnData = *it;
		if(setRange.find(jnData)==setRange.end())
		{
			newArray->push_back(jnData);
		}
	}
	delete oldArray;
	value_.data_.data_.array_ = newArray;
}

void TJSON::RangeReplace( TJSON &jnRange,TJSON &jnNew )
{
	if(type_ != arrayValue)return;
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSONS *newArray = jnNew.value_.data_.data_.array_;
	TJSONS::iterator it = oldArray->begin();
	TJSONS::iterator itEnd = oldArray->end();

	TJSONS::iterator itFrom = oldArray->begin();
	TJSONS::iterator itTo = oldArray->end();

	if (!jnRange.Empty())
	{
		int nCount = jnRange.GetChildCount();
		if(nCount==1)
		{
			TJSON *jnFirst   = &jnRange.At(0);
			for (; it != itEnd; it++)
			{
				if(jnFirst==*it)
				{
					itFrom = it;break;			
				}
			}
			oldArray->insert(oldArray->erase(itFrom),newArray->begin(),newArray->end());
		}else
		{
			TJSON *jnFirst   = &jnRange.At(0);
			TJSON *jnSencond = &jnRange.At(jnRange.GetChildCount()-1);
			for (; it != itEnd; it++)
			{
				if(jnFirst==*it)
				{
					itFrom = it;				
				}
				if (jnSencond==*it)
				{
					itTo = it;
					break;
				}
			}
			if (itTo != oldArray->end())itTo++;
			oldArray->insert(oldArray->erase(itFrom,itTo),newArray->begin(),newArray->end());
		}		
	}else
	{
		oldArray->insert(oldArray->end(),newArray->begin(),newArray->end());
	}
}

void TJSON::RangeReplace( TJSON *jnPosition,TJSON &jnNew )
{
	if(type_ != arrayValue)return;
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSONS *newArray = jnNew.value_.data_.data_.array_;
	TJSONS::iterator it = oldArray->begin();
	TJSONS::iterator itEnd = oldArray->end();
	for (; it != itEnd; it++)
	{
		if(jnPosition==*it)
		{
			break;
		}
	}
	if(it != itEnd)
	{
		oldArray->insert(oldArray->erase(it),newArray->begin(),newArray->end());
	}else
	{
		oldArray->insert(oldArray->end(),newArray->begin(),newArray->end());
	}
}

void TJSON::RangeReplace( int iFrom,int iTo,TJSON &jnNew )
{
	if(type_ != arrayValue)return;
	TJSONS *oldArray = value_.data_.data_.array_;
	
	TJSONS::iterator it = oldArray->begin();
	int size = oldArray->size();
	iFrom = iFrom>size?size:iFrom;
	iTo   = iTo>size?size:iTo;
	if(jnNew.IsArray())
	{
		TJSONS *newArray = jnNew.value_.data_.data_.array_;
		oldArray->insert(oldArray->erase(it+iFrom,it+iTo),newArray->begin(),newArray->end());
	}else
	{
		oldArray->erase(it+iFrom,it+iTo);
	}
}

void TJSON::RangeReplace( int iPos,TJSON &jnNew )
{
	if(type_ != arrayValue)return;
	TJSONS *oldArray = value_.data_.data_.array_;	
	TJSONS::iterator it = oldArray->begin();
	int size = oldArray->size();
	iPos = iPos>size?size:iPos;
	if(jnNew.IsArray())
	{
		TJSONS *newArray = jnNew.value_.data_.data_.array_;
		oldArray->insert(oldArray->erase(it+iPos),newArray->begin(),newArray->end());
	}else
	{
		oldArray->erase(it+iPos);
	}	
}

void TJSON::RangeReplace( int iFrom,TJSON &jnOld,TJSON &jnNew )
{
	if(type_ != arrayValue)return;
	TJSONS *oldArray = value_.data_.data_.array_;	
	TJSONS::iterator it = oldArray->begin() + iFrom;
	if(!jnOld.Empty())//先删除
	{
		it = oldArray->erase(it,it+jnOld.GetChildCount());
	}
	if(!jnNew.Empty())//后添加
	{
		TJSONS *newArray = jnNew.value_.data_.data_.array_;
		oldArray->insert(it,newArray->begin(),newArray->end());
	}
}

void TJSON::RangeInsert( int iPos,TJSON &jnNew )
{
	assert(type_ == arrayValue || type_ == nullValue);
	if(type_==nullValue)
	{
		value_.data_.data_.array_=NewArray();
		type_=arrayValue;
	}
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSONS *newArray = jnNew.value_.data_.data_.array_;
	TJSONS::iterator it    = oldArray->begin() + iPos;
	oldArray->insert(it,newArray->begin(),newArray->end());
}

void TJSON::RangeInsert( TJSON *jnPosition,TJSON &jnNew,bool back/*=true*/ )
{	
	assert(type_ == arrayValue || type_ == nullValue);
	if(type_==nullValue)
	{
		value_.data_.data_.array_=NewArray();
		type_=arrayValue;
	}
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSONS *newArray = jnNew.value_.data_.data_.array_;
	TJSONS::iterator it    = oldArray->begin();
	TJSONS::iterator itEnd = oldArray->end();
	for (; it != itEnd; it++)if(jnPosition==*it)break;
	if(it != itEnd)
	{
		if(back)it++;
		oldArray->insert(it,newArray->begin(),newArray->end());
	}else
	{
		oldArray->insert(it,newArray->begin(),newArray->end());
	}
}

void TJSON::RangeInsert( TJSON &jnNew )
{
	assert(type_ == arrayValue || type_ == nullValue);
	if(type_==nullValue)
	{
		value_.data_.data_.array_=NewArray();
		type_=arrayValue;
	}
	if (jnNew.IsArray())
	{		
		TJSONS *oldArray = value_.data_.data_.array_;
		TJSONS *newArray = jnNew.value_.data_.data_.array_;
		oldArray->insert(oldArray->begin(),newArray->begin(),newArray->end());
	}
}

void TJSON::RangeInsert( TJSON &jnNew,TJSON *jnFrom,TJSON *jnTo )
{
	assert(type_ == arrayValue || type_ == nullValue);
	if(type_==nullValue)
	{
		value_.data_.data_.array_=NewArray();
		type_=arrayValue;
	}
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSONS *newArray = jnNew.value_.data_.data_.array_;
	TJSONS::iterator it     = newArray->begin();
	TJSONS::iterator itEnd  = newArray->end();
	TJSONS::iterator itFrom = it;
	TJSONS::iterator itTo   = itEnd;
	for (; it != itEnd; it++)
	{
		if(jnFrom==*it)itFrom = it;
		else if(jnTo == *it)
		{
			itTo = it;
			break;
		}
	}
	if(itTo != itEnd)itTo++;//拷贝它自己
	oldArray->insert(oldArray->begin(),itFrom,itTo);
}

void TJSON::RangeDelete( TJSON &jnNew )
{
	assert(type_ == arrayValue);
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSONS::iterator it = oldArray->begin();
	TJSONS::iterator itEnd = oldArray->end();
	TJSON * jnPosition = &jnNew.At(0);
	for (; it != itEnd; it++)if(jnPosition==*it)break;
	if(it != itEnd)
	{
		oldArray->erase(it,it+jnNew.GetChildCount());
	}
}

void TJSON::RangeDelete( int iPos,int nCount )
{
	assert(type_ == arrayValue);
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSONS::iterator it = oldArray->begin() + iPos;
	oldArray->erase(it,it+nCount);
}

void TJSON::RangeCopy( TJSON &jnRange,int iFrom,int iTo/*=-1*/ )
{
	assert( jnRange.type_ == arrayValue);
	assert(type_ == arrayValue || type_ == nullValue);
	if(type_==nullValue)
	{
		value_.data_.data_.array_=NewArray();
		type_=arrayValue;
	}	
	TJSONS *oldArray = value_.data_.data_.array_;
	TJSONS *newArray = jnRange.value_.data_.data_.array_;
	if (iTo==-1)
	{
		oldArray->insert(oldArray->end(),newArray->begin() + iFrom,newArray->end());
	}else
	{
		if(iTo>(int)newArray->size())iTo = (int)newArray->size();
		oldArray->insert(oldArray->end(),newArray->begin() + iFrom,newArray->begin() + iTo);
	}	
}

void TJSON::RangeCopy( TJSON &jnRange )
{
	if(jnRange.IsArray())
	{
		assert(type_ == arrayValue || type_ == nullValue);
		if(type_==nullValue)
		{
			value_.data_.data_.array_=NewArray();
			type_=arrayValue;
		}	
		TJSONS *oldArray = value_.data_.data_.array_;
		TJSONS *newArray = jnRange.value_.data_.data_.array_;
		oldArray->insert(oldArray->end(),newArray->begin(),newArray->end());	
	}
}

bool TJSON::Equal( TJSON *data ) const
{
	if (!data)return false;
	return *this == *data;
}

TJSON::~TJSON(){}

TJSON* TJSON::ReadFile( const char *fileName )
{
	TJSON* jnItem = NewJSON();
	jnItem->Read(fileName);
	return jnItem;
}

bool TBuffer::Write( const char *filename )
{
	return WriteFile(filename,buffer,useLen);
}

bool TBuffer::Read( const char *filename )
{
	Clear();
	bool ret=-1!=ReadFile(filename,buffer,size);
	useLen=size;
	return ret;
}

void TBuffer::MemoryReduce()
{	
	if(useLen>0)useLen++;
	if(size>useLen+useLen/3)
	{				
		char*p=(char*)malloc(useLen);
		memcpy(p,buffer,useLen);
		free(buffer);
		buffer=p;
		size=useLen;
	}
}

void TBuffer::Clear()
{
	if(buffer)
	{
		free(buffer);
		buffer=NULL;
		size=0;
		useLen=0;
	}
}

void TBuffer::Add( const char *data,int len )
{
	if (data&&len>0)
	{
		if (useLen+len>=size)//重新申请内存
		{
			size=useLen+useLen/PreMallocRatio+len;
			char*p=(char*)malloc(size);
			memcpy(p,buffer,useLen);
			memset(p+useLen,0,size - useLen);
			free(buffer);
			buffer=p;
		}
		memcpy(buffer+useLen,data,len);
		useLen+=len;
	}	
}
