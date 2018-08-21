//********************************************************************************
//文件：PMInclude.h
//作者：feiren
//时间：2011-8-20
//用途：封装引用对象
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMINCLUDE_H__
#define __PMINCLUDE_H__
class PMThread;
struct TBuffer;
class TJSONIterator;
class TJSONConstIterator;
class TJSON;

typedef map<int,PMThread*> MapThread;
typedef vector<PMThread*> VectorThread;
typedef list<PMThread*> ListThread;

#endif