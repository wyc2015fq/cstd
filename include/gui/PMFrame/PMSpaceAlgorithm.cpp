#include "StdAfx.h"
#include "PMSpaceAlgorithm.h"
#include "math.h"


IntPoint PMSpaceAlgorithm::GetPerpendicularPoint( const IntPoint &start,const IntPoint &end,PMDouble distance)
{
	if(start==end||distance==0)
	{
		return IntPoint(start.x,Up(start.y+distance));
	}else
	{
		PMInt	 deltaY = end.y - start.y;
		PMInt	 deltaX=end.x - start.x;
		PMDouble lenth = sqrt((PMDouble)deltaX*deltaX + deltaY*deltaY);
		PMDouble deltaCos = deltaX/lenth;
		PMDouble deltaSin= deltaY/lenth;
		PMDouble orgX = end.x - distance * deltaCos;
		PMDouble orgY = end.y + distance * deltaSin;
		return IntPoint(Up(orgX),Up(orgY));
	}	
}

PMInt PMSpaceAlgorithm::Round( PMDouble number )
{
	return (PMInt)(number);
}


PMInt PMSpaceAlgorithm::atoi( const string &data )
{
	return ::atoi(data.c_str());
}

PMInt PMSpaceAlgorithm::atoi( const PMChar *data )
{
	return ::atoi(data);
}

std::string PMSpaceAlgorithm::ftoa(PMDouble data)
{
	PMChar buf[100]={0};
	if(IsInteger(data))
	{
		sprintf_s(buf,100,"%ld",(PMLong)data);
	}else
	{
		sprintf_s(buf,100,"%f",data);
	}	
	return buf;
}

std::string PMSpaceAlgorithm::itoa( PMInt data )
{
	PMChar buf[100]={0};
	sprintf_s(buf,100,"%i",data);
	return buf;
}

std::string PMSpaceAlgorithm::itoa( PMLong data )
{
	PMChar buf[100]={0};
	sprintf_s(buf,100,"%ld",data);
	return buf;
}

PMDouble PMSpaceAlgorithm::atof( const PMChar *data )
{
	PMChar *p =(PMChar*) strchr(data,'/');
	if(p)
	{
		PMChar ch=*p;
		*p=0;
		PMDouble d=::atof(data)/::atof(p+1);
		*p=ch;
		return d;
	}else
	{
		return ::atof(data);
	}
}

PMDouble PMSpaceAlgorithm::atof( const string &data )
{
	return atof(data.c_str());
}

PMChar * PMSpaceAlgorithm::GetCharactorDimension( PMInt index )
{
	static PMChar title[100]={0};
	vector<PMInt> datas;
	PMInt k=index,m=0;	
	do
	{
		m=k%26;
		k=k/26;
		datas.push_back(m);
		if(k==1)
		{
			datas.push_back(0);
			break;
		}
	}while (k>0);
	PMInt i=0;
	for (vector<PMInt>::reverse_iterator it = datas.rbegin(); it!= datas.rend();it++)
	{
		title[i]='A'+*it;
		title[i+1]='\0';
		i++;
	} 
	return title;
}

PMColor PMSpaceAlgorithm::GetReverseColor( PMColor sourceColor )
{
	return 0xFFFFFF-sourceColor;
}

PMBool PMSpaceAlgorithm::IsInteger( PMDouble data )
{
	return (data -(PMInt)data)==0;
}

std::string PMSpaceAlgorithm::GetChineseNumber( PMUnint index )
{	
	static PMChar CNNumber[][3]={"零","一","二","三","四","五","六","七","八","九"};
	static PMChar CNSection[][3]={"","十","百","千","万","十","百","千","亿","十","百","千","万","十","百","千","亿"};
	PMChar CV[30]={0};
	PMChar CVS[30];//如果为1就要去掉0
	memset(CVS,1,30);
	PMUnint m=index%10,k=index/10;	
	string data;
	PMInt i=0;
	CV[i++]=m;	
	while(k)
	{
		m=k%10;
		k=k/10;
		CV[i++]=m;
		for (PMInt j=i-1;j>=0;j--)
		{
			if(CV[j]==0)
			{
				CVS[j]=0;
			}else
			{
				if(CV[j+1]==0)CVS[j+1]=1;//留下最后一个0
				break;
			}
		}
	}
	if(i>1&&CV[0]==0)CVS[0]=0;
	for (i--;i>=0;i--)
	{
		if(CVS[i]==1)
		{
			if(CV[i]>0)
			{
				data+=CNNumber[CV[i]];
				data+=CNSection[i];
			}else
			{
				if(i==4||i==8||i==12||i==16)
				{
					data+=CNSection[i];
				}else
				{
					data+="零";
				}
				
			}
		}else if(i==4||i==8||i==12||i==16)
		{
			data+=CNSection[i];
		}
	}
	return data;
}

PMInt PMSpaceAlgorithm::Up( PMDouble number )
{
	if(number>=0)
	{
		if((PMInt)number<number)return (PMInt)number+1;
		else return (PMInt)number;
	}else
	{
		if((PMInt)number>number)return (PMInt)number-1;
		else return (PMInt)number;
	}
}

std::string PMSpaceAlgorithm::UpperCase( const string &data )
{
	string str(data);
	ToUpper(str);
	return str;
}

std::string PMSpaceAlgorithm::LowerCase( const string &data )
{
	string str(data);
	ToLower(str);
	return str;
}

PMInt PMSpaceAlgorithm::GetMostOrder( map<PMInt,PMInt> &numbers,map<PMInt,PMInt> &sequence,PMInt number1,PMInt number2,PMInt number3,PMInt number4 )
{	
	if(number1==number2&&number2==number3&&number1==number4)
	{
		return -1;
	}else
	{
		numbers[number1]++;
		numbers[number2]++;
		numbers[number3]++;
		numbers[number4]++;		
		for (map<PMInt,PMInt>::iterator it = numbers.begin() ; it != numbers.end(); it++)
		{
			sequence[it->second]=it->first;
		}
		return 1;
	}
}

PMInt PMSpaceAlgorithm::GetMostOrder( map<PMInt,PMInt> &numbers,map<PMInt,PMInt> &sequence,PMInt number1, PMInt number2,PMInt number3 )
{
	if(number1==number2&&number2==number3)
	{
		return -1;
	}else
	{
		numbers[number1]++;
		numbers[number2]++;
		numbers[number3]++;	
		for (map<PMInt,PMInt>::iterator it = numbers.begin() ; it != numbers.end(); it++)
		{
			sequence[it->second]=it->first;
		}
		return 1;
	}
}

PMInt PMSpaceAlgorithm::GetScaleLength( const PMChar *data,PMInt len,PMDouble Scale )
{
	PMInt ScaleLen = 0;	
	PMInt Inc = 1;
	if(len>0)
	{
		while (ScaleLen<len)
		{
			if((PMDouble)ScaleLen/len>Scale)break;
			if(data[ScaleLen]<0)
			{
				if(ScaleLen+2<len)ScaleLen+=2;
				else break;
			}
			else
			{
				ScaleLen++;
			}
		}
	}	
	return ScaleLen;
}

PMInt PMSpaceAlgorithm::GetLastWordSize( PMChar *p,PMInt size )
{
	PMInt Result = 0;
	PMInt len    = 0;
	while (len<size)
	{
		Result = len;
		if (p[len]<0||(p[len]=='\n'&&p[len+1]=='\r')||(p[len]=='\r'&&p[len+1]=='\n'))len+=2;
		else len++;
	}
	return size - Result;
}

std::string TMD5::MD5Buffer( const TBuffer &buffer )
{
	return MD5Buffer(buffer.buffer,buffer.useLen);
}

std::string TMD5::MD5Buffer( const PMChar *pszInput, PMUnint nInputSize )
{
	char szDigest[16]; 
	char szResult[56]={0};
	MD5_CTX context;  
	MD5Init (&context); 
	MD5Update (&context, (unsigned char *)pszInput, nInputSize); 
	MD5Final ((unsigned char *)szDigest, &context); 
	int i;
	for (i=0;i<16;i++) sprintf (szResult+strlen(szResult),"%02X",(unsigned char)szDigest[i]);
	return szResult;
}

std::string TMD5::MD5File( const PMChar *pszFileName )
{
	char szDigest[16]; 
	char szResult[56]={0};
	char szFileBuffer[1024]={0};
	unsigned int len = 0; 
	MD5_CTX context;  
	MD5Init (&context); 
	FILE *fp = fopen(pszFileName,"rb");
	if(fp)
	{
		while (true)
		{
			len = fread(szFileBuffer,1,1024,fp);
			if(len>0)MD5Update (&context, (unsigned char *)szFileBuffer, len); 
			else break;
		}
		fclose(fp);
	}
	MD5Final ((unsigned char *)szDigest, &context); 
	int i;
	for (i=0;i<16;i++) sprintf (szResult+strlen(szResult),"%02X",(unsigned char)szDigest[i]);
	return szResult;
}

#define S11 7 
#define S12 12 
#define S13 17 
#define S14 22 
#define S21 5 
#define S22 9 
#define S23 14 
#define S24 20 
#define S31 4 
#define S32 11 
#define S33 16 
#define S34 23 
#define S41 6 
#define S42 10 
#define S43 15 
#define S44 21 

static unsigned char PADDING[64] = { 
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 
}; 

#define   F(x, y, z) (((x) & (y)) | ((~x) & (z))) 
#define   G(x, y, z) (((x) & (z)) | ((y) & (~z))) 
#define   H(x, y, z) ((x) ^ (y) ^ (z)) 
#define   I(x, y, z) ((y) ^ ((x) | (~z))) 

#define   ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n)))) 

#define   FF(a, b, c, d, x, s, ac) {   (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac);   (a) = ROTATE_LEFT ((a), (s));   (a) += (b);   } 
#define   GG(a, b, c, d, x, s, ac) {   (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac);   (a) = ROTATE_LEFT ((a), (s));   (a) += (b);   } 
#define   HH(a, b, c, d, x, s, ac) {   (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac);   (a) = ROTATE_LEFT ((a), (s));   (a) += (b);   } 
#define   II(a, b, c, d, x, s, ac) {   (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac);   (a) = ROTATE_LEFT ((a), (s));   (a) += (b); } 


inline void TMD5::Encode(unsigned char *output, UINT4 *input, unsigned int len) 
{ 
	unsigned int i, j; 

	for (i = 0, j = 0; j < len; i++, j += 4) { 
		output[j]   = (unsigned char)(input[i] & 0xff); 
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff); 
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff); 
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff); 
	} 
} 

inline void TMD5::Decode(UINT4 *output, unsigned char *input, unsigned int len) 
{ 
	unsigned int i, j; 

	for (i = 0, j = 0; j < len; i++, j += 4) 
	{
		output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) | 
			(((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24); 
	}
} 

inline void TMD5::MD5Transform (UINT4 state[4], unsigned char block[64]) 
{ 
	UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16]; 
	Decode (x, block, 64); 
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */ 
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */ 
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */ 
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */ 
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */ 
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */ 
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */ 
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */ 
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */ 
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */ 
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */ 
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */ 
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */ 
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */ 
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */ 
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */ 
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */ 
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */ 
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */ 
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */ 
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */ 
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */ 
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */ 
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */ 
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */ 
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */ 
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */ 
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */ 
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */ 
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */ 
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */ 
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */ 
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */ 
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */ 
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */ 
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */ 
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */ 
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */ 
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */ 
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */ 
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */ 
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */ 
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */ 
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */ 
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */ 
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */ 
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */ 
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */ 
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */ 
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */ 
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */ 
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */ 
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */ 
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */ 
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */ 
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */ 
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */ 
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */ 
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */ 
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */ 
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */ 
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */ 
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */ 
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */ 
	state[0] += a; 
	state[1] += b; 
	state[2] += c; 
	state[3] += d; 
	memset ((POINTER)x, 0, sizeof (x)); 
} 

inline void TMD5::MD5Init(MD5_CTX *context) 
{ 
	context->count[0] = context->count[1] = 0; 
	context->state[0] = 0x67452301; 
	context->state[1] = 0xefcdab89; 
	context->state[2] = 0x98badcfe; 
	context->state[3] = 0x10325476; 
} 

inline void TMD5::MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputLen) 
{ 
	unsigned int i, index, partLen; 

	index = (unsigned int)((context->count[0] >> 3) & 0x3F); 
	if ((context->count[0] += ((UINT4)inputLen << 3))	< ((UINT4)inputLen << 3)) 
		context->count[1]++; 
	context->count[1] += ((UINT4)inputLen >> 29); 
	partLen = 64 - index; 
	if (inputLen >= partLen) 
	{ 
		memcpy((POINTER)&context->buffer[index], (POINTER)input, partLen); 
		MD5Transform(context->state, context->buffer); 

		for (i = partLen; i + 63 < inputLen; i += 64)  
			MD5Transform (context->state, &input[i]); 
		index = 0; 
	} 
	else  
	{
		i = 0;  
	}

	memcpy((POINTER)&context->buffer[index], (POINTER)&input[i], inputLen-i); 
} 

inline void TMD5::MD5Final(unsigned char digest[16], MD5_CTX *context) 
{ 
	unsigned char bits[8]; 
	unsigned int index, padLen; 

	Encode (bits, context->count, 8); 
	index = (unsigned int)((context->count[0] >> 3) & 0x3f); 
	padLen = (index < 56) ? (56 - index) : (120 - index); 
	MD5Update (context, PADDING, padLen); 
	MD5Update (context, bits, 8); 
	Encode (digest, context->state, 16); 
	memset ((POINTER)context, 0, sizeof (*context)); 
} 

void TMD5::MD5Digest(char *pszInput, unsigned long nInputSize, char *pszOutPut) 
{ 
	MD5_CTX context; 
	unsigned int len = strlen (pszInput); 
	MD5Init (&context); 
	MD5Update (&context, (unsigned char *)pszInput, len); 
	MD5Final ((unsigned char *)pszOutPut, &context); 
} 