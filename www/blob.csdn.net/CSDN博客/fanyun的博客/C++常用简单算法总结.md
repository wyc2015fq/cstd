# C++常用简单算法总结 - fanyun的博客 - CSDN博客
2018年11月28日 21:26:27[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：131
对之前的一些简单算法，顺手整理一下，分享给想学习算法和C++的朋友，希望对大家有帮助：
```cpp
#include <assert.h>
#include <string.h>
#include <stack>
//简单C\C++算法整理
//1.实现一个算法找到数组中第二大的数
int FindSec( int *p, int len )
{
    assert( nullptr != p );
    int maxv = p[ 0 ];
    int secv = p[ 0 ];
    for( int i = 1; i < len; ++i )
    {
        if( maxv < p[ i ] )
        {
            secv = maxv;
            maxv = p[ i ];
        }
    }
    return secv;
}
//2.实现strcpy.  
char* MyStrCpy( char *pDest, const char *pSrc )  
{  
	if( nullptr == pDest || nullptr == pSrc )  
	{  
		return nullptr;  
	}  
	if( pDest == pSrc )  
	{  
		return pDest;  
	}  
	char *pIter = pDest;  
	while( ( *pIter++=*pSrc++ ) !='\0' );  
	return pDest;  
}  
 
//3.实现strcat.  
char* MyStrCat( char *pDest, const char *pSrc )  
{  
	if( nullptr == pDest || nullptr == pSrc )  
	{  
		return nullptr;  
	}  
 
	char *pIter = pDest + strlen( pDest );  
	while( ( *pIter++=*pSrc++ ) != '\0' );  
	return pDest;  
}  
 
//4.不使用第三个变量交换两个数的值  
void SwapA( int &A, int &B )  
{  
	if( A == B )  
	{  
		return;  
	}  
	A = A + B;  
	B = A - B;  
	A = A - B;  
}  
void SwapB( unsigned int &A, unsigned int &B )  
{  
	if( A == B )  
	{  
		return;  
	}
	A = A ^ B;  
	B = A ^ B;  
	A = A ^ B;  
}  
 
//5.C语言中字符串转数字的方法是什么( atoi )，请实现它  
int Myatoi( const char *pString )  
{  
	assert( nullptr != pString );  
	const int Len = strlen( pString );  
	int Value = 0;  
	int Times = 1;  
	for( int i = Len -1; i >= 0; --i, Times *= 10 )  
	{  
		Value += ( pString[ i ] - '0' ) * Times;  
	}  
	return Value;  
}  
 
//6.实现一个将字符串逆序的方法  
char* MyInverted( char *pDest )  
{  
	assert( nullptr != pDest );  
	const int Len = strlen( pDest );  
	char T = 0;  
	for( int i = 0; i < Len / 2; ++i )  
	{  
		T = pDest[ i ];  
		pDest[ i ] = pDest[ Len - i - 1 ];  
		pDest[ Len - i -1 ] = T;  
	}  
	return pDest;  
}  
 
//7.实现一个将字符串中所有字母转换为大写的方法  
char* MyUpper( char *pDest )  
{  
	assert( nullptr != pDest );  
	for( char *i = pDest; *i != '\0'; ++i )  
	{  
		if( *i < 'a' || *i > 'z' )  
		{  
			continue;  
		}  
		*i -= 'a' - 'A';  
	}  
	return pDest;  
}  
 
//8.已知一个数组已经降序排序请用二分查字法找到其中的某个元素找到返回索引否则返回-1  
int BinarySearch( int *pArray, int Count, int Value )  
{  
	assert( nullptr != pArray );  
	int Left = 0;  
	int Right = Count -1;  
	int Mid = 0;  
	while( Left <= Right )  
	{  
		Mid = ( Left + Right ) / 2;  
		if( Value < pArray[ Mid ] )  
		{  
			Right = Mid - 1;  
		}  
		else if( Value > pArray[ Mid ] )  
		{  
			Left = Mid + 1;  
		}  
		else  
		{  
			return Mid;  
		}  
	}  
	return -1;  
}  
 
struct Node  
{  
	Node *mpNext;  
	int mData;  
};
//9.删除链表中值为Value的所有元素( [Head]->[node1]->[node2]->...[noden] )
void DeleteFromList( Node *pHead, int Value )
{
	Node *pPrev = pHead;
	Node *pNext = pHead->mpNext;
	while( nullptr != pNext )
	{
		if( pNext->mData != Value )
		{
			pPrev = pNext;
			pNext = pNext->mpNext;
		}
		else
		{
			pPrev->mpNext = pNext->mpNext;
			delete pNext;
			pNext = pPrev->mpNext;
		}
	}
}  
 
//10.在链表Index位置插入新的值为Value的元素  
void InsertFromList( Node *pHead, int Index, int Value )  
{
	Node *pIter = pHead;
	for( int i = 0; i < Index && nullptr != pIter; ++i, pIter = pIter->mpNext );
	assert( nullptr != pIter );
	Node *pNew = new Node;
	pNew->mData = Value;
	pNew->mpNext = pIter->mpNext;
	pIter->mpNext = pNew;
}  
 
//11.将链表逆序  
Node* InvertedFromList( Node *pHead )  
{  
	//A->B->C
	Node *pPrev = pHead;            //A
	Node *pNext = pHead->mpNext;        //B
	Node *pNextNext = nullptr;        //C
	while( nullptr != pNext )  
	{
		pNextNext = pNext->mpNext;    //C = B->C
		pNext->mpNext = pPrev;        //B->A
 
		pPrev = pNext;                //A = B
		pNext = pNextNext;            //B = C
	}
	pHead->mpNext = nullptr;//C->B->A->null
	return pPrev;            //return C( new head )
}  
 
//12.判断X年X月X日是这年的第几天  
int GetDay( int Year, int Month, int Day )
{  
	int MonthDays[ 13 ] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };  
 
	if( ( Year % 4 == 0 && Year % 100 != 0 ) || ( Year % 400 == 0 ) )  
	{  
		++MonthDays[ 2 ];  
	}  
 
	int Days = 0;  
	for( int i = 1; i < Month; ++i )  
	{  
		Days += MonthDays[ i ];  
	}  
	Days += Day;  
 
	return Days;  
}
//13.求斐波拉契数列第N项
int GetFibonacci1( int N )
{
	if( 1 == N || 2 == N )
	{
		return 1;
	}
	if( 3 == N )
	{
		return 2;
	}
	int A = 2;
	int B = 3;
	int C = 5;
	for( int i = 0; i < N - 4; ++i )
	{
		C = A + B;
		A = B;
		B = C;
	}
	return C;
}
 
//14.递归求斐波拉契数列数列第N项
int GetFibonacci2( int N )
{
	if( 1 == N || 2 == N )
	{
		return 1;
	}
	return GetFibonacci2( N - 1 ) + GetFibonacci2( N - 2 );
}
//15.实现一个产生[N-M]区间数字的随机方法
int GetRandomRange( int N, int M )
{
	if( N == M )
	{
		return N;
	}
	if( N > M )
	{
		N = N + M;
		M = N - M;
		N = N - M;
	}
	return N + ( rand() % ( M - N + 1 ) );
}
 
//16.实现一个产生[0~1]之间的随机浮点数
double GetRandomRange()
{
	return rand() / static_cast< double >( RAND_MAX );
}
//17.实现一个打印出1-1000之间的所有素数的方法
void PrintfPrime()
{
	//1不是素数
	//2是最小非奇数素数
	//直接从3开始
	printf( "2\n" );
	bool b = false;
	for( int i = 3; i <= 1000; ++i )
	{
		b = true;
		for( int j = 2; j <= i / 2; ++j )
		{
			if( i % j == 0 )
			{
				b = false;
				break;
			}
		}
		if( b )
		{
			printf( "%d\n", i );
		}
	}
}
 
//18.已知Z = X + Y 其中 Z, X, Y 均为无符号int型 定义一个宏判断Z是否已经越界
#define IS_OVER_FLOW( Z, X, Y ) ( Z < ( ( X ) < ( Y ) ? ( Y ) : ( X ) ) )
 
//19.请用栈实现队列
int QueuePop( std::stack< int > &StackA )
{
	std::stack< int > StackB;
	while( false == StackA.empty() )
	{
		StackB.push( StackA.top() );
		StackA.pop();
	}
 
	const int top = StackB.top();
	StackB.pop();
 
	while( false == StackB.empty() )
	{
		StackA.push( StackB.top() );
		StackB.pop();
	}
	return top;
}
 
//20.已知X班X成绩0-100分编写一个方法实现0-59打印不合格,60-69打印合格,70-79打印良好,80-100打印优秀
//不能使用if,:?,switch
void PrintScore( int Score )
{
	assert( Score >= 0 && Score <= 100 );
	const char *pString[] =
	{ 
		"不合格",
		"不合格",
		"不合格",
		"不合格",
		"不合格",
		"不合格",
		"合格",
		"良好",
		"优秀",
		"优秀",
		"优秀",
	};
	printf( "%s\n", pString[ Score / 10 ] );
}
//21.实现strncpy
char *Mystrncpy( char *pDest, const char *pSrc, int Count )
{
	assert( NULL != pDest && NULL != pSrc );
	if( pDest == pSrc )
	{
		return pDest;
	}
	if( Count <= 0 )
	{
		return pDest;
	}
	char *pStart = pDest;
	while( ( Count-- ) > 0 && ( *pStart++=*pSrc++ ) );
	*pStart = '\0';
	return pDest;
} 
//22.C语言中数字转字符串的方法是什么？(itoa)请实现他
char* Myitoa( char *pDest, int val, int radix )
{
	assert( NULL != pDest );
	assert( radix > 1 );
	const bool IsMinu = val < 0;
	char buffer[ 16 ] = {};
	int count = 0;
 
	do
	{
		buffer[ count++ ] = abs(val) % radix;
		val /= radix;
	}
	while( val );
 
 
	if( IsMinu )
	{
		pDest[ 0 ] = '-';
		for( int i = 0; i < count; ++i )
		{
			pDest[ i + 1 ] = '0' + buffer[ count - i - 1 ];
		}
		pDest[ count + 1 ] = '\0';
	}
	else
	{
		for( int i = 0; i < count; ++i )
		{
			pDest[ i ] = '0' + buffer[ count - i - 1 ];
		}
		pDest[ count ] = '\0';
	}
	return pDest;
}
//23.如何判断链表是否有环
bool IsLoop( Node *pHead )
{
	//[H->A->B->C->A]
	assert( NULL != pHead );
	Node *pNext = pHead->mpNext;
	Node *pNextNext = pHead->mpNext;
	while( NULL != pNext && NULL != pNextNext->mpNext )
	{
		pNext = pNext->mpNext;//[ B、C、A ]
		pNextNext = pNextNext->mpNext->mpNext;//[C、B、A]
		if( pNext == pNextNext )
		{
			return true;
		}
	}
	return false;
}
//24.统计出一个字符串每种字母出现的次数要求时间复杂度为O(n)
void CountLetter( const char *pSrc )
{
	int count[ 256 ] = {};
	for( ; *pSrc !='\0'; ++pSrc )
	{
		const char &c = *pSrc;
		if( ( c < 'A' || c > 'z') && ( c < 'a' || c > 'z' ) )
		{
			continue;
		}
		++count[ c ];
	}
} 
//25.选择排序的思想是什么？( 每次找到最大或最小的值放在数组的低位上 )请实现它
void SelectSort( int *pArray, int count )
{
	for( int i = 0; i < count; ++i )
	{
		//默认低位元素最小
		int MinValue = pArray[ i ];
		//默认保存低位元素的索引
		int MinIndex = i;
		//除开第一个元素找是否还有比它还小的元素( 升序 )
		for( int j = i + 1; j < count; ++j )
		{
			//发现找到比它还小的元素重新赋值和保存索引
			if( pArray[ j ] < MinValue )
			{
				MinValue = pArray[ j ];
				MinIndex = j;
			}
		}
		//将找到最小元素放在数组低位上面
		const int Temp = pArray[ i ];
		pArray[ i ] = MinValue;
		pArray[ MinIndex ] = Temp;
	}
}
 
//26.冒泡排序的思想是什么?(升序排序中越小的数往低位走，越大的数往高位走,每次与相邻元素比较导致的特点)请实现它
void BubbleSort( int *pArray, int count )
{
	//eg.[6][8][8][0][9][1]
	//i = 0,j < 5    [6][8][0][8][1][9]
	//i = 1,j < 4    [6][0][8][1][8][9]
	//i = 2,j < 3    [0][6][1][8][8][9]
	//i = 3,j < 2    [0][1][6][8][8][9]
 
	//到此为止已经排序OK了
	//i = 4,j < 1    [0][1][6][8][8][9]
	//i = 5,j < 0    [0][1][6][8][8][9]
	for( int i = 0; i < count; ++i )
	{
		for( int j = 0; j < count - i - 1; ++j )
		{
			if( pArray[ j ] > pArray[ j + 1 ] )
			{
				const int Temp = pArray[ j ];
				pArray[ j ] = pArray[ j + 1 ];
				pArray[ j + 1 ] = Temp;
			}
		}
	}
}
 
//27.已知两个数组有序实现一个方法将他们合并后任然有序
void MergeSort( int *pMerge, int *p1, int p1len, int *p2, int p2len )
{
	assert( nullptr != pMerge && nullptr != p1 && nullptr != p2 );
	int i = 0;
	int j = 0;
	int k = 0;
	while( i < p1len && j < p2len )
	{
		if( p1[ i ] < p2[ j ] )
		{
			pMerge[ k ] = p1[ i ];
			++k;
			++i;
		}
		else
		{
			pMerge[ k ] = p2[ j ];
			++k;
			++j;
		}
	}
	while( i < p1len )
	{
		pMerge[ k ] = p1[ i ];
		++k;
		++i;
	}
	while( j < p2len )
	{
		pMerge[ k ] = p2[ j ];
		++k;
		++j;
	}
}
```
