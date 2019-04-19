# C++实现字符串逆序和字母替换算法 - fanyun的博客 - CSDN博客
2018年10月31日 22:13:35[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：132
1.实现一个将字符串逆序和字母替换的算法
2.思路
基本思路是，先判断字符的结束标志'\0'，然后从该位置向前输出。
3.实现代码如下
(1).字母逆序
```cpp
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
```
(2).把字符串中的小写字母替换为大写
```cpp
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
```
