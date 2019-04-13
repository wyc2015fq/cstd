
# 高级数据结构及应用 —— 使用 bitmap 进行字符串去重 - Zhang's Wikipedia - CSDN博客


2018年06月03日 10:34:05[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：820


bitmap 即为由单个元素为 boolean（0/1, 0 表示未出现，1 表示已经出现过）的数组。
如果C/C++ 没有原生的 boolean 类型，可以用 int 或 char 来作为 bitmap 使用，如果我们要判断某字符（char）是否出现过，
使用 int 作为 bitmap 的底层数据结构，bitmap 即为 int 数组，一个 int 长度为 32 个 bit 位，
c / 32 ⇒ bitmap 中的第几个 int
c % 32 ⇒ bitmap 中的某 int 中的第几个 bit 位；
使用 char 作为 bitmap 的底层数据结构，bitmap 即为 char 数组，一个 char 长度为 8 个 bit 位；
c / 8 ⇒ bitmap 中的第几个 char
c % 8 ⇒ bitmap 中某 char 中的第几个 bit 位；
ASCII
A-Z：65-90
a-z：97-122
如果使用 char 作为 bitmap 的替代底层数据结构，为了实现字符串的去重需要 char 的长度为多少呢？122/8+1 ⇒ 16。如果使用 int 作为 bitmap 的底层实现，则需要 int 数组的长度为 122/32 + 1 ⇒ 4
1. int 作为底层数据结构
voiddedup(constchar* src,char* dst)
{unsignedintexists[4] = {0};inti =0, j =0;unsignedintmask;charc;while(src[i])
    {
        c = src[i];
        mask =1<< (c %32);if((exists[c /32] & mask) ==0)
        {
            dst[j++] = c;
            exists[c /32] |= mask;
        }
        i++;
    }
    dst[j] ='\0';
}2. 使用 char 作为底层数据结构
voiddedup(constchar* src,char* dst)
{unsignedcharexists[16] = {0};inti =0, j =0;unsignedintmask;charc;while(src[i])
    {
        c = src[i];
        mask =1<< (c %8);if((exists[c /8] & mask) ==0)
        {
            dst[j++] = c;
            exists[c /8] |= mask;
        }
        i++;
    }
    dst[j] ='\0';
}

