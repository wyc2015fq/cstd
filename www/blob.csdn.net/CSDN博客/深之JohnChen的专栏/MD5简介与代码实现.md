# MD5简介与代码实现 - 深之JohnChen的专栏 - CSDN博客

2008年05月31日 22:46:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：9583


**MD5简介**

      md5的全称是message-digest algorithm 5（信息-摘要算法），在90年代初由mit laboratory for computer science和rsa data security inc的ronald l. rivest开发出来，经md2、md3和md4发展而来。它的作用是让大容量信息在用数字签名软件签署私人密匙前被"压缩"成一种保密的格式（就是把一个任意长度的字节串变换成一定长的大整数）。不管是md2、md4还是md5，它们都需要获得一个随机长度的信息并产生一个128位的信息摘要。虽然这些算法的结构或多或少有些相似，但md2的设计与md4和md5完全不同，那是因为md2是为8位机器做过设计优化的，而md4和md5却是面向32位的电脑。这三个算法的描述和c语言源代码在internet rfcs 1321中有详细的描述（http://www.ietf.org/rfc/rfc1321.txt），这是一份最权威的文档，由ronald l. rivest在1992年8月向ieft提交。

　　rivest在1989年开发出md2算法。在这个算法中，首先对信息进行数据补位，使信息的字节长度是16的倍数。然后，以一个16位的检验和追加到信息末尾。并且根据这个新产生的信息计算出散列值。后来，rogier和chauvaud发现如果忽略了检验和将产生md2冲突。md2算法的加密后结果是唯一的--既没有重复。

　　为了加强算法的安全性，rivest在1990年又开发出[md4](http://baike.baidu.com/view/444142.htm)算法。md4算法同样需要填补信息以确保信息的字节长度加上448后能被512整除（信息字节长度mod 512 = 448）。然后，一个以64位二进制表示的信息的最初长度被添加进来。信息被处理成512位damg?rd/merkle迭代结构的区块，而且每个区块要通过三个不同步骤的处理。den boer和bosselaers以及其他人很快的发现了攻击md4版本中第一步和第三步的漏洞。dobbertin向大家演示了如何利用一部普通的个人电脑在几分钟内找到md4完整版本中的冲突（这个冲突实际上是一种漏洞，它将导致对不同的内容进行加密却可能得到相同的加密后结果）。毫无疑问，md4就此被淘汰掉了。

　　尽管md4算法在安全上有个这么大的漏洞，但它对在其后才被开发出来的好几种信息安全加密算法的出现却有着不可忽视的引导作用。除了md5以外，其中比较有名的还有[sha-1](http://baike.baidu.com/view/94209.htm)、ripe-md以及haval等。

　　一年以后，即1991年，rivest开发出技术上更为趋近成熟的md5算法。它在md4的基础上增加了"安全-带子"（safety-belts）的概念。虽然md5比md4稍微慢一些，但却更为安全。这个算法很明显的由四个和md4设计有少许不同的步骤组成。在md5算法中，信息-摘要的大小和填充的必要条件与md4完全相同。den boer和bosselaers曾发现md5算法中的假冲突（pseudo-collisions），但除此之外就没有其他被发现的加密后结果了。

　　van oorschot和wiener曾经考虑过一个在散列中暴力搜寻冲突的函数（brute-force hash function），而且他们猜测一个被设计专门用来搜索md5冲突的机器（这台机器在1994年的制造成本大约是一百万美元）可以平均每24天就找到一个冲突。但单从1991年到2001年这10年间，竟没有出现替代md5算法的md6或被叫做其他什么名字的新算法这一点，我们就可以看出这个瑕疵并没有太多的影响md5的安全性。上面所有这些都不足以成为md5的在实际应用中的问题。并且，由于md5算法的使用不需要支付任何版权费用的，所以在一般的情况下（非绝密应用领域。但即便是应用在绝密领域内，md5也不失为一种非常优秀的中间技术），md5怎么都应该算得上是非常安全的了。

　　2004年8月17日的美国加州圣巴巴拉的国际密码学会议（Crypto’2004）上，来自中国山东大学的[王小云](http://baike.baidu.com/view/350813.htm)教授做了破译MD5、HAVAL-128、 MD4和RIPEMD算法的报告，公布了MD系列算法的破解结果。宣告了固若金汤的世界通行密码标准MD５的堡垒轰然倒塌，引发了密码学界的轩然大波。

　　MD5破解工程权威网站http://www.md5crk.com/ 是为了公开征集专门针对MD5的攻击而设立的，网站于2004年8月17日宣布：“中国研究人员发现了完整MD5算法的碰撞；Wang, Feng, Lai与Yu公布了MD5、MD4、HAVAL-128、RIPEMD-128几个 Hash函数的碰撞。这是近年来密码学领域最具实质性的研究进展。使用他们的技术，在数个小时内就可以找到MD5碰撞。……由于这个里程碑式的发现，MD5CRK项目将在随后48小时内结束”。

　　MD5用的是哈希函数,在计算机网络中应用较多的不可逆加密算法有RSA公司发明的MD5算法和由美国国家技术标准研究所建议的安全散列算法SHA.

**算法的应用**

　　MD5的典型应用是对一段信息（Message）产生信息摘要（Message-Digest），以防止被篡改。比如，在UNIX下有很多软件在下载的时候都有一个文件名相同，文件扩展名为.md5的文件，在这个文件中通常只有一行文本，大致结构如：

　　　MD5 (tanajiya.tar.gz) = 0ca175b9c0f726a831d895e269332461

　　这就是tanajiya.tar.gz文件的数字签名。MD5将整个文件当作一个大文本信息，通过其不可逆的字符串变换算法，产生了这个唯一的MD5信息摘要。为了让读者朋友对MD5的应用有个直观的认识，笔者以一个比方和一个实例来简要描述一下其工作过程：

大家都知道，地球上任何人都有自己独一无二的指纹，这常常成为公安机关鉴别罪犯身份最值得信赖的方法；与之类似，MD5就可以为任何文件（不管其大小、格式、数量）产生一个同样独一无二的“数字指纹”，如果任何人对文件做了任何改动，其MD5值也就是对应的“数字指纹”都会发生变化。

我们常常在某些软件下载站点的某软件信息中看到其MD5值，它的作用就在于我们可以在下载该软件后，对下载回来的文件用专门的软件（如Windows MD5 Check等）做一次MD5校验，以确保我们获得的文件与该站点提供的文件为同一文件。利用MD5算法来进行文件校验的方案被大量应用到软件下载站、论坛数据库、系统文件安全等方面。

　　MD5的典型应用是对一段Message(字节串)产生fingerprint(指纹)，以防止被“篡改”。举个例子，你将一段话写在一个叫 readme.txt文件中，并对这个readme.txt产生一个MD5的值并记录在案，然后你可以传播这个文件给别人，别人如果修改了文件中的任何内容，你对这个文件重新计算MD5时就会发现（两个MD5值不相同）。如果再有一个第三方的认证机构，用MD5还可以防止文件作者的“抵赖”，这就是所谓的数字签名应用。

　　所以，要遇到了md5密码的问题，比较好的办法是：你可以用这个系统中的md5()函数重新设一个密码，如admin，把生成的一串密码覆盖原来的就行了。

       MD5还广泛用于操作系统的登陆认证上，如Unix、各类BSD系统登录密码、数字签名等诸多方。如在UNIX系统中用户的密码是以MD5（或其它类似的算法）经Hash运算后存储在文件系统中。当用户登录的时候，系统把用户输入的密码进行MD5 Hash运算，然后再去和保存在文件系统中的MD5值进行比较，进而确定输入的密码是否正确。通过这样的步骤，系统在并不知道用户密码的明码的情况下就可以确定用户登录系统的合法性。这可以避免用户的密码被具有系统管理员权限的用户知道。MD5将任意长度的“字节串”映射为一个128bit的大整数，并且是通过该128bit反推原始字符串是困难的，换句话说就是，即使你看到源程序和算法描述，也无法将一个MD5的值变换回原始的字符串，从数学原理上说，是因为原始的字符串有无穷多个，这有点象不存在反函数的数学函数。所以，要遇到了md5密码的问题，比较好的办法是：你可以用这个系统中的md5()函数重新设一个密码，如admin，把生成的一串密码的Hash值覆盖原来的Hash值就行了。  

　　正是因为这个原因，现在被黑客使用最多的一种破译密码的方法就是一种被称为"跑字典"的方法。有两种方法得到字典，一种是日常搜集的用做密码的字符串表，另一种是用排列组合方法生成的，先用MD5程序计算出这些字典项的MD5值，然后再用目标的MD5值在这个字典中检索。我们假设密码的最大长度为8位字节（8 Bytes），同时密码只能是字母和数字，共26+26+10=62个字符，排列组合出的字典的项数则是P(62,1)+P(62,2)….+P(62,8)，那也已经是一个很天文的数字了，存储这个字典就需要TB级的磁盘阵列，而且这种方法还有一个前提，就是能获得目标账户的密码MD5值的情况下才可以。这种加密技术被广泛的应用于UNIX系统中，这也是为什么UNIX系统比一般操作系统更为坚固一个重要原因。

**算法描述**

　　对MD5算法简要的叙述可以为：MD5以512位分组来处理输入的信息，且每一分组又被划分为16个32位子分组，经过了一系列的处理后，算法的输出由四个32位分组组成，将这四个32位分组级联后将生成一个128位散列值。

　　在MD5算法中，首先需要对信息进行填充，使其字节长度对512求余的结果等于448。因此，信息的字节长度（Bits Length）将被扩展至N*512+448，即N*64+56个字节（Bytes），N为一个正整数。填充的方法如下，在信息的后面填充一个1和无数个0，直到满足上面的条件时才停止用0对信息的填充。然后，在在这个结果后面附加一个以64位二进制表示的填充前信息长度。经过这两步的处理，现在的信息字节长度=N*512+448+64=(N+1)*512，即长度恰好是512的整数倍。这样做的原因是为满足后面处理中对信息长度的要求。

　　MD5中有四个32位被称作链接变量（Chaining Variable）的整数参数，他们分别为：A=0x01234567，B=0x89abcdef，C=0xfedcba98，D=0x76543210。

　　当设置好这四个链接变量后，就开始进入算法的四轮循环运算。循环的次数是信息中512位信息分组的数目。

　　将上面四个链接变量复制到另外四个变量中：A到a，B到b，C到c，D到d。

　　主循环有四轮（MD4只有三轮），每轮循环都很相似。第一轮进行16次操作。每次操作对a、b、c和d中的其中三个作一次非线性函数运算，然后将所得结果加上第四个变量，文本的一个子分组和一个常数。再将所得结果向右环移一个不定的数，并加上a、b、c或d中之一。最后用该结果取代a、b、c或d中之一。

以一下是每次操作中用到的四个非线性函数（每轮一个）。

　　　F(X,Y,Z) =(X&Y)|((~X)&Z)

　　　G(X,Y,Z) =(X&Z)|(Y&(~Z))

　　　H(X,Y,Z) =X^Y^Z

　　　I(X,Y,Z)=Y^(X|(~Z))

　　　（&是与，|是或，~是非，^是异或）

　　这四个函数的说明：如果X、Y和Z的对应位是独立和均匀的，那么结果的每一位也应是独立和均匀的。

F是一个逐位运算的函数。即，如果X，那么Y，否则Z。函数H是逐位奇偶操作符。

**具体的一个MD5实现**

=============================头文件Security.h===============================================

/*

使用方法：

char Md5Buffer[33];

CSecurity Security;

Security.MD5("a string",Md5Buffer);

执行完成之后Md5Buffer中即存储了由"a string"计算得到的MD5值

*/

// 下列 ifdef 块是创建使从 DLL 导出更简单的

// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SECURITY_EXPORTS

// 符号编译的。在使用此 DLL 的

// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将

// SECURITY_API 函数视为是从此 DLL 导入的，而此 DLL 则将用此宏定义的

// 符号视为是被导出的。

//在使用该类的地方包含本文件即可

#ifdef SECURITY_EXPORTS

#define SECURITY_API __declspec(dllexport)

#else

#define SECURITY_API __declspec(dllimport)

#endif

/* POINTER defines a generic pointer type */

typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */

typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */

typedef unsigned long int UINT4;

#define PROTO_LIST(list) list

/* MD5 context. */

typedef struct _MD5_CTX

{

UINT4 state[4]; /* state (ABCD) */

UINT4 count[2]; /* number of bits, modulo 2^64 (lsb first) */

unsigned char buffer[64]; /* input buffer */

} MD5_CTX;

static unsigned char PADDING[64]= {

0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};

/* Constants for MD5Transform routine.

*/

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

/* F, G, H and I are basic MD5 functions.

*/

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))

#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))

#define H(x, y, z) ((x) ^ (y) ^ (z))

#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.

*/

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.

Rotation is separate from addition to prevent recomputation.

*/

#define FF(a, b, c, d, x, s, ac) { (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac);(a) = ROTATE_LEFT ((a), (s)); (a) += (b); }

#define GG(a, b, c, d, x, s, ac) { (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }

#define HH(a, b, c, d, x, s, ac) {  (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }

#define II(a, b, c, d, x, s, ac) { (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); (a) = ROTATE_LEFT ((a), (s)); (a) += (b); }

#define TEST_BLOCK_LEN 1000

#define TEST_BLOCK_COUNT 1000

// 此类是从 Security.dll 导出的

class SECURITY_API CSecurity

{

public:

CSecurity(void);

void CSecurity::MD5( const char *string ,char *lpMD5StringBuffer ) ;

private:

  void MD5Transform PROTO_LIST ((UINT4 [4], unsigned char [64]));

  void MD5_memcpy PROTO_LIST ((POINTER, POINTER, size_t));

  void MD5_memset PROTO_LIST ((POINTER, int, size_t));

  void MD5Init PROTO_LIST ((MD5_CTX *));

  void MD5Update PROTO_LIST ((MD5_CTX *, unsigned char *, size_t));

  void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));

  void MDTimeTrial PROTO_LIST ((void));

  void StringAddOne PROTO_LIST ((char *));

  void Encode PROTO_LIST ((unsigned char *, UINT4 *, size_t));

  void Decode PROTO_LIST ((UINT4 *, unsigned char *, size_t));

};

===============================Security.cpp====================================================

// Security.cpp : 定义 DLL 应用程序的入口点。

//

#include "stdafx.h"

#include<stdio.h>

#include<stdlib.h>

#include<string.h>

#include<math.h>

#include<ctype.h>

#include "Security.h"

BOOL APIENTRY DllMain( HANDLE hModule,

       DWORD  ul_reason_for_call,

       LPVOID lpReserved

       )

{

switch (ul_reason_for_call)

{

case DLL_PROCESS_ATTACH:

case DLL_THREAD_ATTACH:

case DLL_THREAD_DETACH:

case DLL_PROCESS_DETACH:

  break;

}

    return TRUE;

}

// 这是已导出类的构造函数。

// 有关类定义的信息，请参阅 Security.h

CSecurity::CSecurity()

{

return;

}

/*

MD5 initialization. Begins an MD5 operation, writing a new context.

*/

void CSecurity::MD5Init( MD5_CTX *context )

{

context->count[0] = context->count[1] = 0;

/*

Load magic initialization constants.

*/

context->state[0] = 0x67452301;

context->state[1] = 0xefcdab89;

context->state[2] = 0x98badcfe;

context->state[3] = 0x10325476;

}

/*

MD5 block update operation. Continues an MD5 message-digest

operation, processing another message block, and updating the

context.

*/

void CSecurity::MD5Update(

       MD5_CTX *context, /* context */

       unsigned char *input, /* input block */

       size_t inputLen /* length of input block */

       )

{

size_t i, index, partLen;

/* Compute number of bytes mod 64 */

index = (size_t)((context->count[0] >> 3) & 0x3F);

/* Update number of bits */

if ((context->count[0] += ((UINT4)inputLen << 3))

  < ((UINT4)inputLen << 3))

  context->count[1]++;

context->count[1] += ((UINT4)inputLen >> 29);

partLen = 64 - index;

/* Transform as many times as possible. */

if (inputLen >= partLen) {

  MD5_memcpy

   ((POINTER)&context->buffer[index], (POINTER)input, partLen);

  MD5Transform (context->state, context->buffer);

  for (i = partLen; i + 63 < inputLen; i += 64)

   MD5Transform (context->state, &input*);  index = 0;}else  i = 0;/* Buffer remaining input */MD5_memcpy  ((POINTER)&context->buffer[index], (POINTER)&input,  inputLen-i);}/*MD5 finalization. Ends an MD5 message-digest operation, writing thethe message digest and zeroizing the context.*/void CSecurity::MD5Final(      unsigned char digest[16], /* message digest */      MD5_CTX *context /* context */      ){unsigned char bits[8];size_t index, padLen;/* Save number of bits */Encode (bits, context->count, 8);/* Pad out to 56 mod 64. */index = (size_t)((context->count[0] >> 3) & 0x3f);padLen = (index < 56) ? (56 - index) : (120 - index);MD5Update (context, PADDING, padLen);/* Append length (before padding) */MD5Update (context, bits, 8);/* Store state in digest */Encode (digest, context->state, 16);/* Zeroize sensitive information. */MD5_memset ((POINTER)context, 0, sizeof (*context));}/*MD5 basic transformation. Transforms state based on block.*/void CSecurity::MD5Transform(       UINT4 state[4],       unsigned char block[64]       ){UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];Decode (x, block, 64);/* Round 1 */FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 *//* Round 2 */GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */GG (d, a, b, c, x[10], S22, 0x2441453); /* 22 */GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 *//* Round 3 */HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */HH (b, c, d, a, x[ 6], S34, 0x4881d05); /* 44 */HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 *//* Round 4 */II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */state[0] += a;state[1] += b;state[2] += c;state[3] += d;/* Zeroize sensitive information. */MD5_memset ((POINTER)x, 0, sizeof (x));}/*Encodes input (UINT4) into output (unsigned char).Assumes len is a multiple of 4.*/void CSecurity::Encode(       unsigned char *output,       UINT4 *input,       size_t len       ){size_t i, j;for (i = 0, j = 0; j < len; i++, j += 4) {  output[j] = (unsigned char)(input & 0xff);  output[j+1] = (unsigned char)((input >> 8) & 0xff);  output[j+2] = (unsigned char)((input >> 16) & 0xff);  output[j+3] = (unsigned char)((input >> 24) & 0xff);}}/*Decodes input (unsigned char) into output (UINT4).Assumes len is a multiple of 4.*/void CSecurity::Decode(       UINT4 *output,       unsigned char *input,       size_t len       ){size_t i, j;for (i = 0, j = 0; j < len; i++, j += 4)  output = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) |  (((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24);}/*Note: Replace "for loop" with standard memcpy if possible.*/void CSecurity::MD5_memcpy(        POINTER output,        POINTER input,        size_t len        ){size_t i;for (i = 0; i < len; i++)  output = input;}/*Note: Replace "for loop" with standard memset if possible.*/void CSecurity::MD5_memset(        POINTER output,        int value,        size_t len        ){size_t i;for (i = 0; i < len; i++)  ((char *)output) = (char)value;}/*Digests a string and prints the result.*/void CSecurity::MD5( const char *string ,char *lpMD5StringBuffer ){MD5_CTX context;unsigned char digest[16];/*char output1[33]; */static char output[33]={""};/*size_t*/size_t len = strlen (string);int i;MD5Init( &context);MD5Update( &context, (unsigned char*)string, len );MD5Final( digest, &context );for (i = 0; i < 16; i++){  sprintf(&(lpMD5StringBuffer[2*i]),"%02x",(unsigned char)digest);  sprintf(&(lpMD5StringBuffer[2*i+1]),"%02x",(unsigned char)(digest<<4));}for(i=0;i<32;i++){  output=lpMD5StringBuffer;}}/*get the string add one.*/void CSecurity::StringAddOne( char * orstring ){size_t len;size_t i,n;len = strlen(orstring);n = len - 1;for(i = n; i >= 0; i--){  if(orstring=='9')  {   orstring = 'A';   break;  }  else if(orstring=='Z')  {   orstring='a';   break;  }  else if(orstring=='z')  {   orstring='0';   continue;  }  else   orstring += 1;  break;}}=============================stdafx.h=====================================// stdafx.h : 标准系统包含文件的包含文件，// 或是常用但不常更改的项目特定的包含文件//#pragma once//导出#define SECURITY_EXPORTS#define WIN32_LEAN_AND_MEAN  // 从 Windows 头中排除极少使用的资料// Windows 头文件:#include <windows.h>// TODO: 在此处引用程序要求的附加头文件============================stdafx.cpp========================================// stdafx.cpp : 只包括标准包含文件的源文件// Security.pch 将成为预编译头// stdafx.obj 将包含预编译类型信息#include "stdafx.h"// TODO: 在 STDAFX.H 中//引用任何所需的附加头文件，而不是在此文件中引用=====================================================================以上程序使用命令：@cl /GD /LD Security.cpp stdafx.cpp 编译即可*

**一些破解MD5的网站**

http://www.md5.com.cn (一亿以上数据)
*http://www.cmd5.comhttp://www.xmd5.com这三个网站都是国人做的。http://md5.mmkey.comhttp://www.md5lookup.com/ (对数字破解比较好)http://md5.rednoize.com (对字母破解比较好)http://nz.md5.crysm.net (从1到4位数字)- 好像关了http://us.md5.crysm.net (美英字典的破解收集和IP地址)http://www.xmd5.org (对数字破解比较好)http://gdataonline.com (估计是基础字典的破解)http://www.hashchecker.com (这个倒是说得很厉害,但是在实际中,很多次我在这里都找不到)http://passcracking.ruhttp://www.milw0rm.com/md5http://plain-text.info (这个是我比较喜欢一个.也请你提交一些已破解的MD5)http://www.securitystats.com/tools/hashcrack.php (多种破解.我试过...没有一次破解出来.非常没用)http://www.schwett.com/md5/ - (破解挪威人(Norwegian)字典)http://passcrack.spb.ru/http://shm.pl/md5/http://www.und0it.com/http://www.neeao.com/md5/http://md5.benramsey.com/http://www.md5decrypt.com/http://md5.khrone.pl/http://www.csthis.com/md5/index.phphttp://www.md5decrypter.com/http://www.md5encryption.com/http://www.md5database.net/http://md5.xpzone.de/http://md5.geeks.li/http://www.hashreverse.com/http://www.cmd5.com/english.aspxhttp://www.md5.altervista.org/http://md5.overclock.ch/biz/index.php?p=md5crack&l=enhttp://alimamed.pp.ru/md5/ (如果你看不懂俄文,我告诉你,把你的MD5放到第2个格子)http://md5crack.it-helpnet.de/index.php?op=add (德国的....)http://cijfer.hua.fi/ (Projects->md5 reverse lookup)http://shm.hard-core.pl/md5/http://www.thepanicroom.org/index.php?view=crackerhttp://rainbowtables.net/services/results.php (我试过破过几次在这里)http://rainbowcrack.com/ (需要捐献)http://www.securitydb.org/cracker/http://passwordsecuritycenter.com/index.php?main_page=product_info&cPath=3&products_id=7 这个是证明他们的破解商品的质量..好让你来买这个破解程序...但是...只要能破解到即可.你又不一定要买..http://0ptix.co.nr/md5lm:http://sys9five.ath.cx:8080/hak5rtables/http://lasecwww.epfl.ch/~oechslin/projects/ophcrack/lm + ntlm:http://plain-text.infohttp://www.securitystats.com/tools/hashcrack.phphttp://rainbowtables.net/services/results.phphttp://rainbowcrack.com/http://passwordsecuritycenter.com/index.php?main_page=product_info&cPath=3&products_id=7md4:http://www.securitystats.com/tools/hashcrack.phphttp://rainbowtables.net/services/results.phphttp://rainbowcrack.com/sha1:http://www.securitystats.com/tools/hashcrack.phphttp://passcrack.spb.ru/http://www.hashreverse.com/http://rainbowtables.net/services/results.phphttp://rainbowcrack.com/http://www.md5encryption.com/http://passcracking.ruhttp://www.shalookup.com/*

密码学领域重大发现:

山东大学王小云教授成功破解MD5

2004年8月17日的美国加州圣巴巴拉，正在召开的国际密码学会议（Crypto’2004）安排了三场关于杂凑函数的特别报告。在国际著名密码学家Eli Biham和Antoine Joux相继做了对SHA-1的分析与给出SHA-0的一个碰撞之后，来自山东大学的王小云教授做了破译MD5、HAVAL-128、 MD4和RIPEMD算法的报告。在会场上，当她公布了MD系列算法的破解结果之后，报告被激动的掌声打断。王小云教授的报告轰动了全场，得到了与会专家的赞叹。报告结束时，与会者长时间热烈鼓掌，部分学者起立鼓掌致敬，这在密码学会议上是少见的盛况。王小云教授的报告缘何引起如此大的反响？因为她的研究成果作为密码学领域的重大发现宣告了固若金汤的世界通行密码标准MD5的堡垒轰然倒塌，引发了密码学界的轩然大波。会议总结报告这样写道：“我们该怎么办？MD5被重创了；它即将从应用中淘汰。SHA-1仍然活着，但也见到了它的末日。现在就得开始更换SHA-1了。”

　　关键词：碰撞＝漏洞＝别人可以伪造和冒用数字签名。

　　Hash函数与数字签名（数字手印）

　　HASH函数，又称杂凑函数，是在信息安全领域有广泛和重要应用的密码算法，它有一种类似于指纹的应用。在网络安全协议中，杂凑函数用来处理电子签名，将冗长的签名文件压缩为一段独特的数字信息，像指纹鉴别身份一样保证原来数字签名文件的合法性和安全性。在前面提到的SHA-1和MD5都是目前最常用的杂凑函数。经过这些算法的处理，原始信息即使只更动一个字母，对应的压缩信息也会变为截然不同的“指纹”，这就保证了经过处理信息的唯一性。为电子商务等提供了数字认证的可能性。

　　安全的杂凑函数在设计时必须满足两个要求：其一是寻找两个输入得到相同的输出值在计算上是不可行的，这就是我们通常所说的抗碰撞的；其二是找一个输入，能得到给定的输出在计算上是不可行的，即不可从结果推导出它的初始状态。现在使用的重要计算机安全协议，如SSL，PGP都用杂凑函数来进行签名，一旦找到两个文件可以产生相同的压缩值，就可以伪造签名，给网络安全领域带来巨大隐患。

　　MD5就是这样一个在国内外有着广泛的应用的杂凑函数算法，它曾一度被认为是非常安全的。然而，王小云教授发现，可以很快的找到MD5的“碰撞”，就是两个文件可以产生相同的“指纹”。这意味着，当你在网络上使用电子签名签署一份合同后，还可能找到另外一份具有相同签名但内容迥异的合同，这样两份合同的真伪性便无从辨别。王小云教授的研究成果证实了利用MD5算法的碰撞可以严重威胁信息系统安全，这一发现使目前电子签名的法律效力和技术体系受到挑战。因此，业界专家普林斯顿计算机教授Edward Felten等强烈呼吁信息系统的设计者尽快更换签名算法，而且他们强调这是一个需要立即解决的问题。

　　国际讲坛　王氏发现艳惊四座

　　面对Hash函数领域取得的重大研究进展，Crypto 2004 会议总主席StorageTek高级研究员Jim Hughes 17 日早晨表示，此消息太重要了，因此他已筹办该会成立24年来的首次网络广播（Webcast ）。Hughes在会议上宣布：“会中将提出三份探讨杂凑碰撞（hash collisions ）重要的研究报告。”其中一份是王小云等几位中国研究人员的研究发现。17日晚，王小云教授在会上把他们的研究成果做了宣读。这篇由王小云、冯登国、来学嘉、于红波四人共同完成的文章，囊括了对MD5、HAVAL-128、 MD4和RIPEMD四个著名HASH算法的破译结果。在王小云教授仅公布到他们的第三个惊人成果的时候，会场上已经是掌声四起，报告不得不一度中断。报告结束后，所有与会专家对他们的突出工作报以长时的热烈掌声，有些学者甚至起立鼓掌以示他们的祝贺和敬佩。当人们掌声渐息，来学嘉教授又对文章进行了一点颇有趣味的补充说明。由于版本问题，作者在提交会议论文时使用的一组常数和先行标准不同；在会议发现这一问题之后，王小云教授立即改变了那个常数，在很短的时间内就完成了新的数据分析，这段有惊无险的小插曲倒更加证明了他们论文的信服力，攻击方法的有效性，反而凸显了研究工作的成功。

　　会议结束时，很多专家围拢到王小云教授身边，既有简短的探讨，又有由衷的祝贺，褒誉之词不绝。包含公钥密码的主要创始人R. L. Rivest和A. Shamir在内的世界顶级的密码学专家也上前表示他们的欣喜和祝贺。

　　国际密码学专家对王小云教授等人的论文给予高度评价。

　　MD5的设计者，同时也是国际著名的公钥加密算法标准RSA的第一设计者R．Rivest在邮件中写道：“这些结果无疑给人非常深刻的印象，她应当得到我最热烈的祝贺，当然，我并不希望看到MD5就这样倒下，但人必须尊崇真理。”

　　Francois Grieu这样说：“王小云、冯登国、来学嘉和于红波的最新成果表明他们已经成功破译了MD4、MD5、HAVAL-128、RIPEMD-128。并且有望以更低的复杂度完成对SHA-0的攻击。一些初步的问题已经解决。他们赢得了非常热烈的掌声。”

　　另一位专家Greg Rose如此评价：“我刚刚听了Joux和王小云的报告，王所使用的技术能在任何初始值下用2^40次hash运算找出SHA-0的碰撞。她在报告中对四种HASH函数都给出了碰撞，她赢得了长时间的起立喝彩，（这在我印象中还是第一次）。…… 她是当今密码学界的巾帼英雄。……（王小云教授的工作）技术虽然没有公开，但结果是无庸质疑的，这种技术确实存在。…… 我坐在Ron Rivest前面，我听到他评论道：‘我们不得不做很多的重新思考了。’”

　　石破天惊　MD5堡垒轰然倒塌

一石击起千层浪，MD5的破译引起了密码学界的激烈反响。专家称这是密码学界近年来“最具实质性的研究进展”，各个密码学相关网站竞相报导这一惊人突破。

　　MD5破解专项网站关闭

　　MD5破解工程权威网站http://www.md5crk.com/是为了公开征集专门针对MD5的攻击而设立的，网站于2004年8月17日宣布：“中国研究人员发现了完整MD5算法的碰撞；Wang, Feng, Lai与Yu公布了MD5、MD4、HAVAL-128、RIPEMD-128几个 Hash函数的碰撞。这是近年来密码学领域最具实质性的研究进展。使用他们的技术，在数个小时内就可以找到MD5碰撞。……由于这个里程碑式的发现，MD5CRK项目将在随后48小时内结束”。

对此， Readyresponse主页专门转载了该报道，几个其它网站也进行了报道。

　　权威网站相继发表评论或者报告这一重大研究成果

　　经过统计，在论文发布两周之内，已经有近400个网站发布、引用和评论了这一成果。国内的许多新闻网站也以“演算法安全加密功能露出破绽 密码学界一片哗然”为题报道了这一密码学界的重大事件,该消息在各新闻网站上多次转载。

　　东方神韵　 MD5终结者来自中国

　　MD5破解工作的主要成员王小云教授是一个瘦弱、矜持的女子，厚厚的镜片透射出双眸中数学的灵光。她于1990年在山东大学师从著名数学家潘承洞教授攻读数论与密码学专业博士，在潘先生、于秀源、展涛等多位著名教授的悉心指导下，她成功将数论知识应用到密码学中，取得了很多突出成果，先后获得863项目资助和国家自然科学基金项目资助，并且获得部级科技进步奖一项，撰写论文二十多篇。王小云教授从上世纪90年代末开始进行HASH函数的研究，她所带领的于红波、王美琴、孙秋梅、冯骐等组成的密码研究小组，同中科院冯登国教授，上海交大来学嘉等知名学者密切协作，经过长期坚持不懈的努力，找到了破解HASH函数的关键技术，成功的破解了MD5和其它几个HASH函数。

　　近年来她的工作得到了山东大学和数学院领导的大力支持，特别投资建设了信息安全实验室。山东大学校长展涛教授高度重视王小云教授突出的科研成果。 2004年6月山东大学领导听取王小云教授的工作介绍后，展涛校长亲自签发邀请函邀请国内知名信息安全专家参加2004年7月在威海举办的“山东大学信息安全研究学术研讨会”，数学院院长刘建亚教授组织和主持了会议，会上王小云教授公布了MD5等算法的一系列研究成果，专家们对她的研究成果给予了充分的肯定，对其坚持不懈的科研态度大加赞扬。一位院士说，她的研究水平绝对不比国际上的差。这位院士的结论在时隔一个月之后的国际密码会上得到了验证，国外专家如此强烈的反响表明，我们的工作可以说不但不比国际上的差，而且是在破解HASH函数方面已领先一步。加拿大CertainKey公司早前宣布将给予发现 MD5算法第一个碰撞人员一定的奖励，CertainKey的初衷是利用并行计算机通过生日攻击来寻找碰撞，而王小云教授等的攻击相对生日攻击需要更少的计算时间。

　　数字认证　你的未来不是梦

　　由于MD5的破译，引发了关于MD5 产品是否还能够使用的大辩论。在麻省理工大学Jeffrey I. Schiller教授主持的个人论坛上，许多密码学家在标题为“Bad day at the hash function factory”的辩论中发表了具有价值的意见。这次国际密码学会议的总主席Jimes Hughes发表评论说“我相信这（破解MD5）是真的，并且如果碰撞存在，HMAC也就不再是安全的了，…… 我认为我们应该抛开MD5了。” Hughes建议，程序设计人员最好开始舍弃MD5。他说：“既然现在这种算法的弱点已暴露出来，在有效的攻击发动之前，现在是撤离的时机。”

　　同样，在普林斯顿大学教授Edwards Felton的个人网站上，也有类似的评论。他说：“留给我们的是什么呢？MD5已经受了重伤；它的应用就要淘汰。SHA-1仍然活着，但也不会很长，必须立即更换SHA-1，但是选用什么样的算法，这需要在密码研究人员达到共识。”

　　密码学家Markku-Juhani称“这是HASH函数分析领域激动人心的时刻。”

　　而著名计算机公司SUN的LINUX专家Val Henson则说：“以前我们说"SHA-1可以放心用，其他的不是不安全就是未知"， 现在我们只能这么总结了："SHA-1不安全，其他的都完了"。

　　针对王小云教授等破译的以MD5为代表的Hash函数算法的报告，美国国家技术与标准局（NIST）于2004年8月24日发表专门评论，评论的主要内容为：“在最近的国际密码学会议（Crypto 2004）上，研究人员宣布他们发现了破解数种HASH算法的方法，其中包括MD4，MD5，HAVAL-128，RIPEMD还有 SHA-0。分析表明，于1994年替代SHA-0成为联邦信息处理标准的SHA-1的减弱条件的变种算法能够被破解；但完整的SHA-1并没有被破解，也没有找到SHA-1的碰撞。研究结果说明SHA-1的安全性暂时没有问题，但随着技术的发展，技术与标准局计划在2010年之前逐步淘汰SHA-1，换用其他更长更安全的算法（如SHA-224、SHA-256、SHA-384和SHA-512）来替代。”

2004年8月28日，十届全国人大常委会第十一次会议表决通过了电子签名法。这部法律规定，可靠的电子签名与手写签名或者盖章具有同等的法律效力。电子签名法的通过，标志着我国首部“真正意义上的信息化法律”已正式诞生，将于2005年4月1日起施行。专家认为，这部法律将对我国电子商务、电子政务的发展起到极其重要的促进作用。王小云教授的发现无异于发现了信息化天空的一个惊人黑洞。我们期待着王小云教授和她的团队能够成就“女娲补天”的壮举，为人类的信息化之路保驾护航。

md5加密算法c实现

////////////////////////////////////////////////////////////////////

/*                 md5.h           */

#ifndef _MD5_H_

#define _MD5_H_

#define R_memset(x, y, z) memset(x, y, z)

#define R_memcpy(x, y, z) memcpy(x, y, z)

#define R_memcmp(x, y, z) memcmp(x, y, z)

typedef unsigned long UINT4;

typedef unsigned char *POINTER;

/* MD5 context. */

typedef struct {
  /* state (ABCD) */  

  /*四个32bits数，用于存放最终计算得到的消息摘要。当消息长度〉512bits时，也用于存放每个512bits的中间结果*/ 

  UINT4 state[4];  

  /* number of bits, modulo 2^64 (lsb first) */   

  /*存储原始信息的bits数长度,不包括填充的bits，最长为 2^64 bits，因为2^64是一个64位数的最大值*/

  UINT4 count[2];

  /* input buffer */

  /*存放输入的信息的缓冲区，512bits*/

  unsigned char buffer[64]; 

} MD5_CTX;

void MD5Init(MD5_CTX *);

void MD5Update(MD5_CTX *, unsigned char *, unsigned int);

void MD5Final(unsigned char [16], MD5_CTX *);

#endif /* _MD5_H_ */

///////////////////////////////////////////////////////////////////////////

/*    md5.cpp   */

#include "stdafx.h"

/* Constants for MD5Transform routine. */

/*md5转换用到的常量，算法本身规定的*/

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

static void MD5Transform(UINT4 [4], unsigned char [64]);

static void Encode(unsigned char *, UINT4 *, unsigned int);

static void Decode(UINT4 *, unsigned char *, unsigned int);

/*

用于bits填充的缓冲区，为什么要64个字节呢？因为当欲加密的信息的bits数被512除其余数为448时，

需要填充的bits的最大值为512=64*8 。

*/

static unsigned char PADDING[64] = {

  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

};

/*

接下来的这几个宏定义是md5算法规定的，就是对信息进行md5加密都要做的运算。

据说有经验的高手跟踪程序时根据这几个特殊的操作就可以断定是不是用的md5

*/

/* F, G, H and I are basic MD5 functions.

 */

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))

#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))

#define H(x, y, z) ((x) ^ (y) ^ (z))

#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.

 */

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.

  Rotation is separate from addition to prevent recomputation.

 */

#define FF(a, b, c, d, x, s, ac) { /

  (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); /

  (a) = ROTATE_LEFT ((a), (s)); /

  (a) += (b); /

 }

#define GG(a, b, c, d, x, s, ac) { /

  (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); /

  (a) = ROTATE_LEFT ((a), (s)); /

  (a) += (b); /

 }

#define HH(a, b, c, d, x, s, ac) { /

  (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); /

  (a) = ROTATE_LEFT ((a), (s)); /

  (a) += (b); /

 }

#define II(a, b, c, d, x, s, ac) { /

  (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); /

  (a) = ROTATE_LEFT ((a), (s)); /

  (a) += (b); /

 }

/* MD5 initialization. Begins an MD5 operation, writing a new context. */

/*初始化md5的结构*/

void MD5Init (MD5_CTX *context)

{
  /*将当前的有效信息的长度设成0,这个很简单,还没有有效信息,长度当然是0了*/

  context->count[0] = context->count[1] = 0;

  /* Load magic initialization constants.*/

  /*初始化链接变量，算法要求这样，这个没法解释了*/

  context->state[0] = 0x67452301;

  context->state[1] = 0xefcdab89;

  context->state[2] = 0x98badcfe;

  context->state[3] = 0x10325476;

}

/* MD5 block update operation. Continues an MD5 message-digest

  operation, processing another message block, and updating the

  context. */

/*将与加密的信息传递给md5结构，可以多次调用

context：初始化过了的md5结构

input：欲加密的信息，可以任意长

inputLen：指定input的长度

*/

void MD5Update(MD5_CTX *context,unsigned char * input,unsigned int  inputLen)

{

 unsigned int i, index, partLen;

/* Compute number of bytes mod 64 */

 /*计算已有信息的bits长度的字节数的模64, 64bytes=512bits。

 用于判断已有信息加上当前传过来的信息的总长度能不能达到512bits，

 如果能够达到则对凑够的512bits进行一次处理*/

 index = (unsigned int)((context->count[0] >> 3) & 0x3F);

/* Update number of bits *//*更新已有信息的bits长度*/

 if((context->count[0] += ((UINT4)inputLen << 3)) < ((UINT4)inputLen << 3))

  context->count[1]++;

 context->count[1] += ((UINT4)inputLen >> 29);

 /*计算已有的字节数长度还差多少字节可以 凑成64的整倍数*/

 partLen = 64 - index;

/* Transform as many times as possible.

  */
 /*如果当前输入的字节数 大于 已有字节数长度补足64字节整倍数所差的字节数*/

 if(inputLen >= partLen)

    {
 /*用当前输入的内容把context->buffer的内容补足512bits*/

  R_memcpy((POINTER)&context->buffer[index], (POINTER)input, partLen);
  /*用基本函数对填充满的512bits（已经保存到context->buffer中） 做一次转换，转换结果保存到context->state中*/

  MD5Transform(context->state, context->buffer);

 /*

 对当前输入的剩余字节做转换（如果剩余的字节<在输入的input缓冲区中>大于512bits的话 ），

 转换结果保存到context->state中

 */

  for(i = partLen; i + 63 < inputLen; i += 64)/*把i+63<inputlen改为i+64<=inputlen更容易理解*/

   MD5Transform(context->state, &input[i]);

        index = 0;

    }

    else

  i = 0;

 /* Buffer remaining input */

 /*将输入缓冲区中的不足填充满512bits的剩余内容填充到context->buffer中，留待以后再作处理*/

 R_memcpy((POINTER)&context->buffer[index], (POINTER)&input[i], inputLen-i);

}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the

  the message digest and zeroizing the context. */

/*获取加密 的最终结果

digest：保存最终的加密串

context：你前面初始化并填入了信息的md5结构

*/

void MD5Final (unsigned char digest[16],MD5_CTX *context)

{

 unsigned char bits[8];

 unsigned int index, padLen;

 /* Save number of bits */

 /*将要被转换的信息(所有的)的bits长度拷贝到bits中*/

 Encode(bits, context->count, 8);

 /* Pad out to 56 mod 64. */

 /* 计算所有的bits长度的字节数的模64, 64bytes=512bits*/

 index = (unsigned int)((context->count[0] >> 3) & 0x3f);
 /*计算需要填充的字节数，padLen的取值范围在1-64之间*/

 padLen = (index < 56) ? (56 - index) : (120 - index);
 /*这一次函数调用绝对不会再导致MD5Transform的被调用，因为这一次不会填满512bits*/

 MD5Update(context, PADDING, padLen);

 /* Append length (before padding) */

 /*补上原始信息的bits长度（bits长度固定的用64bits表示），这一次能够恰巧凑够512bits，不会多也不会少*/

 MD5Update(context, bits, 8);

 /* Store state in digest */

 /*将最终的结果保存到digest中。ok，终于大功告成了*/

 Encode(digest, context->state, 16);

/* Zeroize sensitive information. */

 R_memset((POINTER)context, 0, sizeof(*context));

}

/* MD5 basic transformation. Transforms state based on block. */

/*

对512bits信息(即block缓冲区)进行一次处理，每次处理包括四轮

state[4]：md5结构中的state[4]，用于保存对512bits信息加密的中间结果或者最终结果

block[64]：欲加密的512bits信息

*/

static void MD5Transform (UINT4 state[4], unsigned char block[64])

{

 UINT4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

 Decode(x, block, 64);

 /* Round 1 */

 FF(a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */

 FF(d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */

 FF(c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */

 FF(b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */

 FF(a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */

 FF(d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */

 FF(c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */

 FF(b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */

 FF(a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */

 FF(d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */

 FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */

 FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */

 FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */

 FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */

 FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */

 FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */

 GG(a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */

 GG(d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */

 GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */

 GG(b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */

 GG(a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */

 GG(d, a, b, c, x[10], S22,  0x2441453); /* 22 */

 GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */

 GG(b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */

 GG(a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */

 GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */

 GG(c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */

 GG(b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */

 GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */

 GG(d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */

 GG(c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */

 GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

 /* Round 3 */

 HH(a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */

 HH(d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */

 HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */

 HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */

 HH(a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */

 HH(d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */

 HH(c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */

 HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */

 HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */

 HH(d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */

 HH(c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */

 HH(b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */

 HH(a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */

 HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */

 HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */

 HH(b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

 /* Round 4 */

 II(a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */

 II(d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */

 II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */

 II(b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */

 II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */

 II(d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */

 II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */

 II(b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */

 II(a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */

 II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */

 II(c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */

 II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */

 II(a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */

 II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */

 II(c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */

 II(b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

 state[0] += a;

 state[1] += b;

 state[2] += c;

 state[3] += d;

 /* Zeroize sensitive information. */

 R_memset((POINTER)x, 0, sizeof(x));

}

/* Encodes input (UINT4) into output (unsigned char). Assumes len is

  a multiple of 4. */

/*将4字节的整数copy到字符形式的缓冲区中

output：用于输出的字符缓冲区

input：欲转换的四字节的整数形式的数组

len：output缓冲区的长度，要求是4的整数倍

*/

static void Encode(unsigned char *output, UINT4 *input,unsigned int  len)

{

 unsigned int i, j;

 for(i = 0, j = 0; j < len; i++, j += 4) {

  output[j] = (unsigned char)(input[i] & 0xff);

  output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);

  output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);

  output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);

 }

}

/* Decodes input (unsigned char) into output (UINT4). Assumes len is

  a multiple of 4. */

/*与上面的函数正好相反，这一个把字符形式的缓冲区中的数据copy到4字节的整数中（即以整数形式保存）

output：保存转换出的整数

input：欲转换的字符缓冲区

len：输入的字符缓冲区的长度，要求是4的整数倍

*/

static void Decode(UINT4 *output, unsigned char *input,unsigned int  len)

{

 unsigned int i, j;

 for(i = 0, j = 0; j < len; i++, j += 4)

  output[i] = ((UINT4)input[j]) | (((UINT4)input[j+1]) << 8) |

   (((UINT4)input[j+2]) << 16) | (((UINT4)input[j+3]) << 24);

}

////////////////////////////////////////////////////////////////////////////////

// md5test.cpp : Defines the entry point for the console application.

//

#include "stdafx.h"

#include "string.h"

int main(int argc, char* argv[])

{

 MD5_CTX md5;

 MD5Init(&md5);                         //初始化用于md5加密的结构

 unsigned char encrypt[200];     //存放于加密的信息

 unsigned char decrypt[17];       //存放加密后的结果

 scanf("%s",encrypt);                 //输入加密的字符

 MD5Update(&md5,encrypt,strlen((char *)encrypt));   //对欲加密的字符进行加密

 MD5Final(decrypt,&md5);                                            //获得最终结果

 printf("加密前:%s/n加密后:",encrypt);

 for(int i=0;i<16;i++)

  printf("%2x ",decrypt[i]);

 printf("/n/n/n加密结束!/n");

 return 0;

}

