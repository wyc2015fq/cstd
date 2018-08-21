//********************************************************************************
//文件：PMSpaceAlgorithm.h
//作者：feiren
//时间：2011-8-20
//用途：封装常用的算法
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMSPACEALGORITHM_H__
#define __PMSPACEALGORITHM_H__

#include "math.h"
#include "PMType.h"
#pragma once

class PMSpaceAlgorithm
{
public:
	static string	GetChineseNumber(PMUnint index);
	static PMBool	IsInteger(PMDouble data);
	static PMInt	atoi(const PMChar *data);
	static PMInt	atoi(const string &data);
	static string	itoa(PMLong data);
	static string	itoa(PMInt data);
	static string	ftoa(PMDouble data);
	static PMDouble atof(const string &data);
	static PMDouble atof(const PMChar *data);
	static PMInt	Round(PMDouble number);
	static PMInt	Up(PMDouble number);
	static IntPoint GetPerpendicularPoint(const IntPoint &start,const IntPoint &end,PMDouble distance);
	static PMChar *	GetCharactorDimension(int index);	
	static PMColor	GetReverseColor(PMColor sourceColor);
	static string	UpperCase(const string &data);
	static string	LowerCase(const string &data);
	static PMInt	GetMostOrder(map<PMInt,PMInt> &numbers,map<PMInt,PMInt> &sequence,PMInt number1,
								 PMInt number2,PMInt number3,PMInt number4);//获取出现频率最多的序号
	static PMInt	GetMostOrder(map<PMInt,PMInt> &numbers,map<PMInt,PMInt> &sequence,PMInt number1,
								 PMInt number2,PMInt number3);//获取出现频率最多的序号
	static PMInt	GetScaleLength(const PMChar *data,PMInt len,PMDouble Scale);
	static PMInt	GetLastWordSize(PMChar *p,PMInt size);

};

class TMD5
{
private:
	typedef unsigned char *POINTER; 
	typedef unsigned short int UINT2; 
	typedef unsigned long int UINT4; 
	typedef struct   
	{ 
		UINT4 state[4]; 
		UINT4 count[2]; 
		unsigned char buffer[64]; 
	} MD5_CTX; 
public:
	string MD5Buffer(const TBuffer &buffer);
	string MD5Buffer(const PMChar *pszInput, PMUnint nInputSize);
	string MD5File(const PMChar *pszFileName);
private:
	void MD5Init(MD5_CTX *); 
	void MD5Update(MD5_CTX *, unsigned char *, unsigned int); 
	void MD5Final(unsigned char[16], MD5_CTX *); 
	void MD5Digest(char *pszInput, unsigned long nInputSize, char *pszOutPut);
	void Encode(unsigned char *output, UINT4 *input, unsigned int len);
	void Decode(UINT4 *output, unsigned char *input, unsigned int len);
	void MD5Transform (UINT4 state[4], unsigned char block[64]) ;
};
#endif