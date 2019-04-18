# TEA加密算法的C/C++实现 - gauss的专栏 - CSDN博客
2014年10月19日 15:50:59[gauss](https://me.csdn.net/mathlmx)阅读数：463
个人分类：[数据结构与算法](https://blog.csdn.net/mathlmx/article/category/605909)
## [TEA加密算法的C/C++实现](http://www.cppblog.com/ant/archive/2007/10/12/31326.html)
***TEA(Tiny Encryption Algorithm)***是一种简单高效的加密算法，以加密解密速度快，实现简单著称。算法真的很简单，TEA算法每一次可以操作64-bit(8-byte)，采用128-bit(16-byte)作为key，算法采用迭代的形式，推荐的迭代轮数是64轮，最少32轮。目前我只知道QQ一直用的是16轮TEA。没什么好说的，先给出C语言的源代码(默认是32轮)：
 1void encrypt(unsigned long*v, unsigned long*k) {
 2    unsigned long y=v[0], z=v[1], sum=0, i;         /* set up */
 3    unsigned long delta=0x9e3779b9;                 /* a key schedule constant */
 4    unsigned long a=k[0], b=k[1], c=k[2], d=k[3];   /* cache key */
 5for (i=0; i <32; i++) {                        /* basic cycle start */
 6        sum += delta;
 7        y += ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
 8        z += ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);/* end cycle */
 9    }
10    v[0]=y;
11    v[1]=z;
12}
13
14void decrypt(unsigned long*v, unsigned long*k) {
15    unsigned long y=v[0], z=v[1], sum=0xC6EF3720, i; /* set up */
16    unsigned long delta=0x9e3779b9;                  /* a key schedule constant */
17    unsigned long a=k[0], b=k[1], c=k[2], d=k[3];    /* cache key */
18for(i=0; i<32; i++) {                            /* basic cycle start */
19        z -= ((y<<4) + c) ^ (y + sum) ^ ((y>>5) + d);
20        y -= ((z<<4) + a) ^ (z + sum) ^ ((z>>5) + b);
21        sum -= delta;                                /* end cycle */
22    }
23    v[0]=y;
24    v[1]=z;
25}
C语言写的用起来当然不方便，没关系，用C++封装以下就OK了：
*util.h*
 1#ifndef UTIL_H
 2#define UTIL_H
 3
 4#include <string>
 5#include <cstdlib>
 6
 7typedef unsigned charbyte;
 8typedef unsigned long ulong;
 9
10/*
11*convert int to hex char.
12*example:10 -> 'A',15 -> 'F'
13*/
14char intToHexChar(int x);
15
16/*
17*convert hex char to int.
18*example:'A' -> 10,'F' -> 15
19*/
20int hexCharToInt(char hex);
21
22using std::string;
23/*
24*convert a byte array to hex string.
25*hex string format example:"AF B0 80 7D"
26*/
27string bytesToHexString(constbyte*in, size_t size);
28
29/*
30*convert a hex string to a byte array.
31*hex string format example:"AF B0 80 7D"
32*/
33size_t hexStringToBytes(const string &str, byte*out);
34
35#endif/*UTIL_H*/
*util.cpp*
 1#include "util.h"
 2#include <vector>
 3
 4using namespace std;
 5
 6char intToHexChar(int x) {
 7staticconstchar HEX[16] = {
 8'0', '1', '2', '3',
 9'4', '5', '6', '7',
10'8', '9', 'A', 'B',
11'C', 'D', 'E', 'F'
12    };
13return HEX[x];
14}
15
16int hexCharToInt(char hex) {
17    hex = toupper(hex);
18if (isdigit(hex))
19return (hex -'0');
20if (isalpha(hex))
21return (hex -'A'+10);
22return0;
23}
24
25string bytesToHexString(constbyte*in, size_t size) {
26    string str;
27for (size_t i =0; i < size; ++i) {
28int t = in[i];
29int a = t /16;
30int b = t %16;
31        str.append(1, intToHexChar(a));
32        str.append(1, intToHexChar(b));
33if (i != size -1)
34            str.append(1, '');
35    }
36return str;
37}
38
39size_t hexStringToBytes(const string &str, byte*out) {
40
41    vector<string> vec;
42    string::size_type currPos =0, prevPos =0;
43while ((currPos = str.find('', prevPos)) != string::npos) {
44        string b(str.substr(prevPos, currPos - prevPos));
45        vec.push_back(b);
46        prevPos = currPos +1;
47    }
48if (prevPos < str.size()) {
49        string b(str.substr(prevPos));
50        vec.push_back(b);
51    }
52    typedef vector<string>::size_type sz_type;
53    sz_type size = vec.size();
54for (sz_type i =0; i < size; ++i) {
55int a = hexCharToInt(vec[i][0]);
56int b = hexCharToInt(vec[i][1]);
57        out[i] = a *16+ b;
58    }
59return size;
60}
*tea.h*
 1#ifndef TEA_H
 2#define TEA_H
 3
 4/*
 5*for htonl,htonl
 6*do remember link "ws2_32.lib"
 7*/
 8#include <winsock2.h>
 9#include "util.h"
10
11class TEA {
12public:
13    TEA(constbyte*key, int round =32, bool isNetByte =false);
14    TEA(const TEA &rhs);
15    TEA& operator=(const TEA &rhs);
16void encrypt(constbyte*in, byte*out);
17void decrypt(constbyte*in, byte*out);
18private:
19void encrypt(const ulong *in, ulong *out);
20void decrypt(const ulong *in, ulong *out);
21    ulong ntoh(ulong netlong) { return _isNetByte ? ntohl(netlong) : netlong; }
22    ulong hton(ulong hostlong) { return _isNetByte ? htonl(hostlong) : hostlong; }
23private:
24int _round; //iteration round to encrypt or decrypt
25    bool _isNetByte; //whether input bytes come from network
26byte _key[16]; //encrypt or decrypt key
27};
28
29#endif/*TEA_H*/
*tea.cpp*
 1#include "tea.h"
 2#include <cstring>//for memcpy,memset
 3
 4using namespace std;
 5
 6TEA::TEA(constbyte*key, int round /*= 32*/, bool isNetByte /*= false*/)
 7:_round(round)
 8,_isNetByte(isNetByte) {
 9if (key !=0)
10        memcpy(_key, key, 16);
11else
12        memset(_key, 0, 16);
13}
14
15TEA::TEA(const TEA &rhs)
16:_round(rhs._round)
17,_isNetByte(rhs._isNetByte) {
18    memcpy(_key, rhs._key, 16);
19}
20
21TEA& TEA::operator=(const TEA &rhs) {
22if (&rhs !=this) {
23        _round = rhs._round;
24        _isNetByte = rhs._isNetByte;
25        memcpy(_key, rhs._key, 16);
26    }
27return*this;
28}
29
30void TEA::encrypt(constbyte*in, byte*out) {
31    encrypt((const ulong*)in, (ulong*)out);
32}
33
34void TEA::decrypt(constbyte*in, byte*out) {
35    decrypt((const ulong*)in, (ulong*)out);
36}
37
38void TEA::encrypt(const ulong *in, ulong *out) {
39
40    ulong *k = (ulong*)_key;
41    register ulong y = ntoh(in[0]);
42    register ulong z = ntoh(in[1]);
43    register ulong a = ntoh(k[0]);
44    register ulong b = ntoh(k[1]);
45    register ulong c = ntoh(k[2]);
46    register ulong d = ntoh(k[3]);
47    register ulong delta =0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
48    register int round = _round;
49    register ulong sum =0;
50
51while (round--) {    /* basic cycle start */
52        sum += delta;
53        y += ((z <<4) + a) ^ (z + sum) ^ ((z >>5) + b);
54        z += ((y <<4) + c) ^ (y + sum) ^ ((y >>5) + d);
55    }    /* end cycle */
56    out[0] = ntoh(y);
57    out[1] = ntoh(z);
58}
59
60void TEA::decrypt(const ulong *in, ulong *out) {
61
62    ulong *k = (ulong*)_key;
63    register ulong y = ntoh(in[0]);
64    register ulong z = ntoh(in[1]);
65    register ulong a = ntoh(k[0]);
66    register ulong b = ntoh(k[1]);
67    register ulong c = ntoh(k[2]);
68    register ulong d = ntoh(k[3]);
69    register ulong delta =0x9E3779B9; /* (sqrt(5)-1)/2*2^32 */
70    register int round = _round;
71    register ulong sum =0;
72
73if (round ==32)
74        sum =0xC6EF3720; /* delta << 5*/
75elseif (round ==16)
76        sum =0xE3779B90; /* delta << 4*/
77else
78        sum = delta * round;
79
80while (round--) {    /* basic cycle start */
81        z -= ((y <<4) + c) ^ (y + sum) ^ ((y >>5) + d);
82        y -= ((z <<4) + a) ^ (z + sum) ^ ((z >>5) + b);
83        sum -= delta;
84    }    /* end cycle */
85    out[0] = ntoh(y);
86    out[1] = ntoh(z);
87}
需要说明的是TEA的构造函数：
TEA(const byte *key, int round = 32, bool isNetByte = false);
1.key - 加密或解密用的128-bit(16byte)密钥。
2.round - 加密或解密的轮数，常用的有64，32，16。
3.isNetByte - 用来标记待处理的字节是不是来自网络，为true时在加密/解密前先要转换成本地字节，执行加密/解密，然后再转换回网络字节。偷偷告诉你，QQ就是这样做的！
最后当然少不了测试代码：
*test.cpp*
 1#include "tea.h"
 2#include "util.h"
 3#include <iostream>
 4
 5using namespace std;
 6
 7int main() {
 8
 9const string plainStr("AD DE E2 DB B3 E2 DB B3");
10const string keyStr("3A DA 75 21 DB E2 DB B3 11 B4 49 01 A5 C6 EA D4");
11constint SIZE_IN =8, SIZE_OUT =8, SIZE_KEY =16;
12byte plain[SIZE_IN], crypt[SIZE_OUT], key[SIZE_KEY];
13
14    size_t size_in = hexStringToBytes(plainStr, plain);
15    size_t size_key = hexStringToBytes(keyStr, key);
16
17if (size_in != SIZE_IN || size_key != SIZE_KEY)
18return-1;
19
20    cout <<"Plain: "<< bytesToHexString(plain, size_in) << endl;
21    cout <<"Key  : "<< bytesToHexString(key, size_key) << endl;
22
23    TEA tea(key, 16, true);
24    tea.encrypt(plain, crypt);
25    cout <<"Crypt: "<< bytesToHexString(crypt, SIZE_OUT) << endl;
26
27    tea.decrypt(crypt, plain);
28    cout <<"Plain: "<< bytesToHexString(plain, SIZE_IN) << endl;
29return0;
30}
运行结果：
Plain: AD DE E2 DB B3 E2 DB B3
Key  : 3A DA 75 21 DB E2 DB B3 11 B4 49 01 A5 C6 EA D4
Crypt: 3B 3B 4D 8C 24 3A FD F2
Plain: AD DE E2 DB B3 E2 DB B3
