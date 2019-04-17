# 程序员编程艺术：第四章、现场编写类似strstr/strcpy/strpbrk的函数 - 结构之法 算法之道 - CSDN博客





2011年05月13日 16:05:00[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：47061
所属专栏：[程序员编程艺术](https://blog.csdn.net/column/details/taopp.html)









> 
第四章、现场编写类似strstr/strcpy/strpbrk的函数








前奏

    有网友向我反应，之前三章（[**http://t.cn/hgVPmH**](http://t.cn/hgVPmH)）的面试题目，是否有点太难了。诚如他所说，绝大部分公司的面试题不会像微软等公司的面试题目出的那么变态，或复杂。

    面试考察的是你对基础知识的掌握程度，及编程能力是否过硬的一种检测，所以，扎实基础知识，提高编程能力，比去看什么所谓的面经，或去背面试题目的答案强多了。

    很多中、小型公司自己的创造能力，包括人力，物力资源都有限，所以，他们的面试题目除了copy一些大公司的题库之外（当然，考察你对基础知识的掌握情况，是肯定不会放过的），还有一个途径就是让你在限定时间内（如十分钟），当场实现一些类似strcpy/strcat/strpbrk等库函数，这个主要看你对细节的把握，以及编程能力是否之扎实了。

    同时，本章里出现的代码（除了第4节的c标准库部分源码）都是个人限定在短时间内（正好，突出现场感）编写的，很多问题，难免有所考虑不周。所以，如果你发现本章任何一段代码有任何问题，恳请不吝指正。


第一节、字符串查找
**1.1题目描述：**给定一个字符串A，要求在A中查找一个子串B。
如A="ABCDF"，要你在A中查找子串B=“CD”。

**分析：**比较简单，相当于实现strstr库函数，主体代码如下：



```cpp
//在字符串中查找指定字符串的第一次出现，不能找到则返回-1      
int strstr(char *string, char *substring)      
{     
	if (string == NULL || substring == NULL)        
		return -1;        

	int lenstr = strlen(string);     
	int lensub = strlen(substring);     

	if (lenstr < lensub)        
		return -1;         

	int len = lenstr - lensub;  
	int i,j;
	for (i = 0; i <= len; i++)   //复杂度为O(m*n)     
	{     
		for (j = 0; j < lensub; j++)     
		{     
			if (string[i+j] != substring[j])     
				break;     
		}     
		if (j == lensub)     
			return i + 1;     
	}     
	return -1;     
}
```



    读者反馈@xiaohui5319：楼主啊，对于你那个strstr的函数，我觉得有点小问题。我查了一下C标准库的源码，它给的声明是这样的，两个参数都有const。


> 
char *

STRSTR (const char *haystack_start, const char *needle_start)


    而且标准库中没有调用strlen函数，因为假如你是标准库的设计者，strlen()函数还没设计出来，你怎么去计算两个字符串的长度？是不是只能通过指针移动来实现，我觉得这些都是微软要考察的地方。

    此外：还有int lenstr=strlen(string);这是不安全的？
    strlen函数的返回类型是size_t型，也就是无符号整型，假如我的数组长度很长（假如是用堆分配的，可以很大很大），长过2的31次方减1的话，会发生一处，你这lenstr就会变成负值了
    用size_t类型最保险。

    以后，本编程艺术系列中有任何问题，暂未来得及及时修正，请读者多加思考，多加辨明。

    上述程序已经实现了在字符串中查找第一个子串的功能，时间复杂度为O（n*m），也可以用KMP算法，复杂度为O(m+n)。为人打通思路，提高他人创造力，我想，这是狂想曲与其它的面试解答所不同的地方，也是我们写狂想曲系列文章的意义与价值之所在。

**1.2、题目描述**

在一个字符串中找到第一个只出现一次的字符。如输入abaccdeff，则输出b。 

代码则可以如下编写：



```cpp
//查找第一个只出现一次的字符，     
//copyright@ yansha     
//July、updated，2011.04.24.     
char FirstNotRepeatChar(char* pString)     
{     
	if(!pString)     
		return '\0';     

	const int tableSize = 256;    
	//有点要提醒各位注意，一般常数的空间消耗，如这里的256，我们也认为此空间复杂度为O（1）。  
	int hashTable[tableSize] = {0}; //存入数组，并初始化为0     

	char* pHashKey = pString;     
	while(*(pHashKey) != '\0')     
		hashTable[*(pHashKey++)]++;     

	while(*pString != '\0')     
	{     
		if(hashTable[*pString] == 1)     
			return *pString;     

		pString++;     
	}     
	return '\0';  //没有找到满足条件的字符，退出     
}
```



代码二，bitmap：



```cpp
# include<stdio.h>  
# include<string.h>  

const int N = 26;  
int bit_map[N];  

void findNoRepeat(char *src)  
{  
	int pos;  
	char *str = src;  
	int i ,len = strlen(src);  

	//统计  
	for(i = 0 ; i < len ;i ++)  
		bit_map[str[i]-'a'] ++;  

	//从字符串开始遍历 其bit_map==1 那么就是结果  
	for(i = 0 ; i < len ; i ++)  
	{  
		if(bit_map[str[i]-'a'] == 1)  
		{  
			printf("%c",str[i]);  
			return ;  
		}  
	}  
}  

int main()  
{     
	char *src = "abaccdeff";  
	findNoRepeat(src);  
	printf("\n");  
	return 0;  
}
```




第二节、字符串拷贝
题目描述：
要求实现库函数strcpy，
原型声明：extern char *strcpy(char *dest,char *src); 
功能：把src所指由NULL结束的字符串复制到dest所指的数组中。 　
说明：src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。 　
返回指向dest的指针。

    分析：如果编写一个标准strcpy函数的总分值为10，下面给出几个不同得分的答案：




```cpp
//得2分     
void strcpy( char *strDest, char *strSrc )     
{     
	while( (*strDest++ = * strSrc++) != '\0' );     
}      

//得4分     
void strcpy( char *strDest, const char *strSrc )      
{     
	//将源字符串加const，表明其为输入参数，加2分     
	while( (*strDest++ = * strSrc++) != '\0' );     
}      

//得7分     
void strcpy(char *strDest, const char *strSrc)      
{     
	//对源地址和目的地址加非0断言，加3分     
	assert( (strDest != NULL) && (strSrc != NULL) );     
	while( (*strDest++ = * strSrc++) != '\0' );     
}      

//得9分     
//为了实现链式操作，将目的地址返回，加2分！     
char * strcpy( char *strDest, const char *strSrc )      
{     
	assert( (strDest != NULL) && (strSrc != NULL) );     
	char *address = strDest;      
	while( (*strDest++ = * strSrc++) != '\0' );      
	return address;     
}    

//得10分，基本上所有的情况，都考虑到了  
//如果有考虑到源目所指区域有重叠的情况，加1分！     
char * strcpy( char *strDest, const char *strSrc )      
{     
	if(strDest == strSrc) { return strDest; }  
	assert( (strDest != NULL) && (strSrc != NULL) );     
	char *address = strDest;      
	while( (*strDest++ = * strSrc++) != '\0' );      
	return address;     
}
```




第三节、小部分库函数的实现
    考察此类编写同库函数一样功能的函数经常见于大大小小的IT公司的面试题目中，以下是常见的字符串库函数的实现，希望，对你有所帮助，有任何问题，欢迎不吝指正：



```cpp
//@yansha:字串末尾要加结束符'\0'，不然输出错位结果  
char *strncpy(char *strDes, const char *strSrc, unsigned int count)      
{      
    assert(strDes != NULL && strSrc != NULL);      
    char *address = strDes;      
    while (count-- && *strSrc != '\0')      
        *strDes++ = *strSrc++;   
    *strDes = '\0';  
    return address;      
}   

//查找字符串s中首次出现字符c的位置   
char *strchr(const char *str, int c)   
{   
    assert(str != NULL);   
    for (; *str != (char)c; ++ str)   
        if (*str == '\0')   
            return NULL;   
        return str;   
}   

int strcmp(const char *s, const char *t)   
{   
    assert(s != NULL && t != NULL);   
    while (*s && *t && *s == *t)   
    {   
        ++ s;   
        ++ t;   
    }   
    return (*s - *t);   
}   

char *strcat(char *strDes, const char *strSrc)   
{   
    assert((strDes != NULL) && (strSrc != NULL));   
    char *address = strDes;   
    while (*strDes != '\0')   
        ++ strDes;   
    while ((*strDes ++ = *strSrc ++) != '\0')   
        NULL;   
    return address;   
}   

int strlen(const char *str)   
{   
    assert(str != NULL);   
    int len = 0;   
    while (*str ++ != '\0')   
        ++ len;   
    return len;   
}   

//此函数，梦修改如下     
char *strdup_(char *strSrc)     
//将字符串拷贝到新的位置     
{     
    if(strSrc!=NULL)     
    {     
        char *start=strSrc;     
        int len=0;     
        while(*strSrc++!='\0')     
            len++;     

        char *address=(char *)malloc(len+1);     
        assert(address != NULL);  

        while((*address++=*start++)!='\0');      
        return address-(len+1);      
    }     
    return NULL;     
}     

//多谢laoyi19861011指正  
char *strstr(const char *strSrc, const char *str)   
{   
    assert(strSrc != NULL && str != NULL);   
    const char *s = strSrc;   
    const char *t = str;   
    for (; *strSrc != '\0'; ++ strSrc)   
    {   
        for (s = strSrc, t = str; *t != '\0' && *s == *t; ++s, ++t)   
            NULL;   
        if (*t == '\0')   
            return (char *) strSrc;   
    }   
    return NULL;   
}   

char *strncat(char *strDes, const char *strSrc, unsigned int count)   
{   
    assert((strDes != NULL) && (strSrc != NULL));   
    char *address = strDes;   
    while (*strDes != '\0')   
        ++ strDes;   
    while (count -- && *strSrc != '\0' )   
        *strDes ++ = *strSrc ++;   
    *strDes = '\0';   
    return address;   
}   

int strncmp(const char *s, const char *t, unsigned int count)   
{   
    assert((s != NULL) && (t != NULL));   
    while (*s && *t && *s == *t && count --)   
    {   
        ++ s;   
        ++ t;   
    }   
    return (*s - *t);   
}   

char *strpbrk(const char *strSrc, const char *str)   
{   
    assert((strSrc != NULL) && (str != NULL));   
    const char *s;   
    while (*strSrc != '\0')   
    {   
        s = str;   
        while (*s != '\0')   
        {   
            if (*strSrc == *s)   
                return (char *) strSrc;   
            ++ s;   
        }   
        ++ strSrc;   
    }   
    return NULL;   
}   

int strcspn(const char *strSrc, const char *str)   
{   
    assert((strSrc != NULL) && (str != NULL));   
    const char *s;   
    const char *t = strSrc;   
    while (*t != '\0')   
    {   
        s = str;   
        while (*s != '\0')   
        {   
            if (*t == *s)   
                return t - strSrc;   
            ++ s;   
        }   
        ++ t;   
    }   
    return 0;   
}   

int strspn(const char *strSrc, const char *str)   
{   
    assert((strSrc != NULL) && (str != NULL));   
    const char *s;   
    const char *t = strSrc;   
    while (*t != '\0')   
    {   
        s = str;   
        while (*s != '\0')   
        {   
            if (*t == *s)   
                break;   
            ++ s;   
        }   
        if (*s == '\0')   
            return t - strSrc;   
        ++ t;   
    }   
    return 0;   
}   

char *strrchr(const char *str, int c)   
{   
    assert(str != NULL);   
    const char *s = str;   
    while (*s != '\0')   
        ++ s;   
    for (-- s; *s != (char) c; -- s)   
        if (s == str)   
            return NULL;   
        return (char *) s;   
}   

char* strrev(char *str)   
{   
    assert(str != NULL);   
    char *s = str, *t = str, c;   
    while (*t != '\0')   
        ++ t;   
    for (-- t; s < t; ++ s, -- t)   
    {   
        c = *s;   
        *s = *t;   
        *t = c;   
    }   
    return str;   
}   

char *strnset(char *str, int c, unsigned int count)   
{   
    assert(str != NULL);   
    char *s = str;   
    for (; *s != '\0' && s - str < count; ++ s)   
        *s = (char) c;   
    return str;   
}   

char *strset(char *str, int c)   
{   
    assert(str != NULL);   
    char *s = str;   
    for (; *s != '\0'; ++ s)   
        *s = (char) c;   
    return str;   
}   

//@heyaming  
//对原 strtok 的修改，根据MSDN,strToken可以为NULL.实际上第一次call strtok给定一字串，  
//再call strtok时可以输入NULL代表要接着处理给定字串。  
//所以需要用一 static 保存没有处理完的字串。同时也需要处理多个分隔符在一起的情况。  
char *strtok(char *strToken, const char *str)  
{  
    assert(str != NULL);  
    static char *last;  

    if (strToken == NULL && (strToken = last) == NULL)  
        return (NULL);  

    char *s = strToken;  
    const char *t = str;  
    while (*s != '\0')  
    {  
        t = str;  
        while (*t != '\0')  
        {  
            if (*s == *t)  
            {  
                last = s + 1;  
                if (s - strToken == 0) {  
                    strToken = last;  
                    break;  
                }  
                *(strToken + (s - strToken)) = '\0';  
                return strToken;  
            }  
            ++ t;  
        }  
        ++ s;  
    }  
    return NULL;  
}  

char *strupr(char *str)   
{   
    assert(str != NULL);   
    char *s = str;   
    while (*s != '\0')   
    {   
        if (*s >= 'a' && *s <= 'z')   
            *s -= 0x20;   
        s ++;   
    }   
    return str;   
}   

char *strlwr(char *str)   
{   
    assert(str != NULL);   
    char *s = str;   
    while (*s != '\0')   
    {   
        if (*s >= 'A' && *s <= 'Z')   
            *s += 0x20;   
        s ++;   
    }   
    return str;   
}   

void *memcpy(void *dest, const void *src, unsigned int count)   
{   
    assert((dest != NULL) && (src != NULL));   
    void *address = dest;   
    while (count --)   
    {   
        *(char *) dest = *(char *) src;   
        dest = (char *) dest + 1;   
        src = (char *) src + 1;   
    }   
    return address;   
}   

void *memccpy(void *dest, const void *src, int c, unsigned int count)   
{   
	assert((dest != NULL) && (src != NULL));   
	while (count --)   
	{   
		*(char *) dest = *(char *) src;   
		if (* (char *) src == (char) c)   
			return ((char *)dest + 1);   
		dest = (char *) dest + 1;   
		src = (char *) src + 1;   
	}   
	return NULL;   
}   

void *memchr(const void *buf, int c, unsigned int count)   
{   
	assert(buf != NULL);   
	while (count --)   
	{   
		if (*(char *) buf == c)   
			return (void *) buf;   
		buf = (char *) buf + 1;   
	}   
	return NULL;   
}   

int memcmp(const void *s, const void *t, unsigned int count)   
{   
	assert((s != NULL) && (t != NULL));   
	while (*(char *) s && *(char *) t && *(char *) s == *(char *) t && count --)   
	{   
		s = (char *) s + 1;   
		t = (char *) t + 1;   
	}   
	return (*(char *) s - *(char *) t);   
}   

//@big：  
//要处理src和dest有重叠的情况，不是从尾巴开始移动就没问题了。  
//一种情况是dest小于src有重叠，这个时候要从头开始移动，  
//另一种是dest大于src有重叠，这个时候要从尾开始移动。  
void *memmove(void *dest, const void *src, unsigned int count)   
{  
	assert(dest != NULL && src != NULL);  
	char* pdest = (char*) dest;  
	char* psrc = (char*) src;  

	//pdest在psrc后面，且两者距离小于count时，从尾部开始移动. 其他情况从头部开始移动  
	if (pdest > psrc && pdest - psrc < count)   
	{  
		while (count--)   
		{  
			*(pdest + count) = *(psrc + count);  
		}  
	} else   
	{  
		while (count--)   
		{  
			*pdest++ = *psrc++;  
		}  
	}  
	return dest;  
}  

void *memset(void *str, int c, unsigned int count)   
{   
	assert(str != NULL);   
	void *s = str;   
	while (count --)   
	{   
		*(char *) s = (char) c;   
		s = (char *) s + 1;   
	}   
	return str;   
}
```

**测试**：以上所有的函数，都待进一步测试，有任何问题，欢迎任何人随时不吝指出。


第四节、c标准库部分源代码

    为了给各位一个可靠的参考，以下，我摘取一些c标准框里的源代码，以飨各位：



```cpp
char * __cdecl strcat (char * dst,const char * src)  
{  
    char * cp = dst;  

    while( *cp )  
        cp++;                   /* find end of dst */  

    while( *cp++ = *src++ ) ;       /* Copy src to end of dst */  

    return( dst );                  /* return dst */  

}  

int __cdecl strcmp (const char * src,const char * dst)  
{  
    int ret = 0 ;  

    while( ! (ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)  
        ++src, ++dst;  

    if ( ret < 0 )  
        ret = -1 ;  
    else if ( ret > 0 )  
        ret = 1 ;  

    return( ret );  
}  

size_t __cdecl strlen (const char * str)  
{  
    const char *eos = str;  

    while( *eos++ ) ;  

    return( (int)(eos - str - 1) );  
}  

char * __cdecl strncat (char * front,const char * back,size_t count)  
{  
    char *start = front;  

    while (*front++)  
        ;  
    front--;  

    while (count--)  
        if (!(*front++ = *back++))  
            return(start);  

        *front = '\0';  
        return(start);  
}  

int __cdecl strncmp (const char * first,const char * last,size_t count)  
{  
    if (!count)  
        return(0);  

    while (--count && *first && *first == *last)  
    {  
        first++;  
        last++;  
    }  

    return( *(unsigned char *)first - *(unsigned char *)last );  
}  

/* Copy SRC to DEST.  */  
char *  
strcpy (dest, src)  
char *dest;  
const char *src;  
{  
    reg_char c;  
    char *__unbounded s = (char *__unbounded) CHECK_BOUNDS_LOW (src);  
    const ptrdiff_t off = CHECK_BOUNDS_LOW (dest) - s - 1;  
    size_t n;  

    do  
    {  
        c = *s++;  
        s[off] = c;  
    }  
    while (c != '\0');  

    n = s - src;  
    (void) CHECK_BOUNDS_HIGH (src + n);  
    (void) CHECK_BOUNDS_HIGH (dest + n);  

    return dest;  
}  

char * __cdecl strncpy (char * dest,const char * source,size_t count)  
{  
    char *start = dest;  

    while (count && (*dest++ = *source++))    /* copy string */  
        count--;  

    if (count)                              /* pad out with zeroes */  
        while (--count)  
            *dest++ = '\0';  

        return(start);  
}
```



有关编程艺术的修订

    程序员编程艺术系列的Github地址已于今天建立：[https://github.com/nateriver520/The-Art-Of-Programming-By-July](https://github.com/nateriver520/The-Art-Of-Programming-By-July)，我们急切的想得到读者的反馈，意见，建议，以及更好的思路，算法，和代码优化的建议。所以，如果你发现了本编程艺术系列中的任何一题，任何一章中的错误，问题，与漏洞，欢迎随时fork到本github中，thanks。

**版权所有，本人对本blog内所有任何内容享有版权及著作权。实要转载，请以链接形式注明出处。**



