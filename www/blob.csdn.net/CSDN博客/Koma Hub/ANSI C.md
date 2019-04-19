# ANSI C - Koma Hub - CSDN博客
2019年01月19日 21:49:22[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：129
个人分类：[C/C++																[基础知识](https://blog.csdn.net/Rong_Toa/article/category/7587699)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**目录**
[软件介绍](#%E8%BD%AF%E4%BB%B6%E4%BB%8B%E7%BB%8D)
[历史](#%E5%8E%86%E5%8F%B2)
[C89](#C89)
[C90](#C90)
[C99](#C99)
[C11](#C11)
[编译器的支持](#%E7%BC%96%E8%AF%91%E5%99%A8%E7%9A%84%E6%94%AF%E6%8C%81)
[支持编译器](#%E6%94%AF%E6%8C%81%E7%BC%96%E8%AF%91%E5%99%A8)
ANSI C是由美国国家标准协会（[ANSI](https://baike.baidu.com/item/ANSI/14955)）及国际标准化组织（ISO）推出的关于C语言的标准。ANSI C 主要标准化了现存的实践， 同时增加了一些来自 C++ 的内容 （主要是函数原型） 并支持多国[字符集](https://baike.baidu.com/item/%E5%AD%97%E7%AC%A6%E9%9B%86/946585) （包括备受争议的三字符序列）。 ANSI C 标准同时规定了 C 运行期库例程的标准。
## 软件介绍
1983 年, [美国国家标准协会](https://baike.baidu.com/item/%E7%BE%8E%E5%9B%BD%E5%9B%BD%E5%AE%B6%E6%A0%87%E5%87%86%E5%8D%8F%E4%BC%9A)（ANSI）委任一个委员会 X3J11 对 C 语言 [1] 进行标准化。 经过长期艰苦的过程， 该委员会的工作于 1989 年 12 月 14 日正式被批准为ANSI X3.159-1989 并于 1990 年春天颁布。
一年左右以后, 该标准被接受为国际标准, ISO/IEC 9899:1990, 这个标准甚至在美国国内（在这里它被称作 ANSI/ISO 9899-1990 [1992]）代替了早先的 X3.159。作为一个 ISO 标准, 它会以发行技术刊物和标准附录的形式不断更新。
1994年, 技术刊物 1 (TC1) 修正标准中 40 处地方, 多数都是小的修改或明确, 而标准附录 1 (NA1) 增加了大约 50 页的新材料, 多数是规定国际化支持的新[库函数](https://baike.baidu.com/item/%E5%BA%93%E5%87%BD%E6%95%B0)。1995 年, TC2 增加了一些更多的小修改。
该标准的一个重大修订，“C99”, 已经完成并于2000年的时候被接受。
该标准的数个版本，包括 C99 和原始的 ANSI 标准, 都包括了一个 ``基本原理" (Rational), 解释它的许多决定并讨论了很多细节问题，包括本文中提及的某些内容。
American National Standards Institute（ANSI——美国国家标准学会） 由公司、政府和其他成员组成的自愿组织。它们协商与标准有关的活动，审议美国国家标准，并努力提高美国在国际标准化组织中的地位。此外，ANSI使有关通信和网络方面的国际标准和美国标准得到发展。ANSI是IEC和ISO的成员之一。
ANSI的地位：ANSI是一个准国家式的标准机构，它为那些在特定领域建立标准的组织提供区域许可，如电气电子工程师协会([IEEE](https://baike.baidu.com/item/IEEE))。ANSI是国际标准化委员会（ISO）和国际电工委员会（IEC）5个常任理事成员之一，4个理事局成员之一，参加79%的ISO/TC的活动，参加89%的IEC/TC活动。ANSI 是[泛美技术标准委员会](https://baike.baidu.com/item/%E6%B3%9B%E7%BE%8E%E6%8A%80%E6%9C%AF%E6%A0%87%E5%87%86%E5%A7%94%E5%91%98%E4%BC%9A)（COPANT）和太平洋地区标准会议（PASC）的成员。
## 历史
C 的第一个标准是由ANSI发布的。虽然这份文档后来被[国际标准化组织](https://baike.baidu.com/item/%E5%9B%BD%E9%99%85%E6%A0%87%E5%87%86%E5%8C%96%E7%BB%84%E7%BB%87)(ISO)采纳并且ISO发布的修订版也被ANSI采纳了，但名称ANSI C(而不是 ISO C)仍被广泛使用。一些软件开发者使用**ISO C**，还有一些使用 **Standard C**。
### C89
1983年，美国国家标准协会（ANSI）组成了一个委员会，X3J11，为了创立 C 语言的标准。经过漫长而艰苦的过程，该标准于1989年完成，并在作为ANSI X3.159-1989 "Programming Language C"正式生效。因为这个标准是1989年发布的，所以一般简称C89标准。有些人也把C89标准叫做ANSI C，因为这个标准是美国国家标准协会（ANSI）发布的。
### C90
在1990年，ANSI C89标准被国际标准化组织(ISO)和国际电工委员会(IEC)采纳为国际标准，名叫ISO/IEC 9899:1990 - Programming languages -- C [2] ，有些人简称C90标准。因此，**C89**和**C90**通常指同一个标准，一般更常用C89这种说法。
### C99
在2000年3月，国际标准化组织(ISO)和国际电工委员会(IEC)采纳了第二个C语言标准，名叫ISO/IEC 9899:1999 - Programming languages -- C，简称C99标准。 [3]
### C11
在2011年12月，国际标准化组织(ISO)和国际电工委员会(IEC)采纳了第三个C语言标准，名叫ISO/IEC 9899:2011 - Information technology -- Programming languages -- C，简称C11标准。它是C程序语言的最新标准。 [4]
## 编译器的支持
ANSI C被几乎所有广泛使用的编译器支持。而且多数C代码是在ANSI C基础上写的。任何仅仅使用标准C并且没有任何硬件依赖假设的代码实际上能保证在任何平台上用遵循C标准的编译器编译成功。如果没有这种预防措施，多数程序只能在一种特定的平台或特定的编译器上编译，例如，使用非标准库，例如图形用户界面库，或者有关编译器或平台特定的特性例如数据类型的确切大小和[字节序](https://baike.baidu.com/item/%E5%AD%97%E8%8A%82%E5%BA%8F)。
选择编译
为了减轻K&R C和ANSI C标准，__STDC__("standard c")巨集可以被用来将代码分割为ANSI和K&R部分。
#if __STDC__
extern int getopt(int,char*const*,constchar*);
#else
extern int getopt();
#endif
上面最好使用"#if __STDC__"而不是"#ifdef __STDC__"，因为一些实现可能会把 __STDC__设置为0来表示不遵循ANSI C。"__STDC__"能处理任何没有被巨集替换或者值为0的标示符。因而即使巨集"__STDC__"没有定义来表示不遵循ANSI C，"__STDC__"仍然能像显示的那样工作。
在上面的例子，一个原型中使用了ANSI实现兼容的函数声明，而另一个使用了过时的非原形声明。它们在C99和C90中依旧是ANSI兼容的，但并不被鼓励使用。
## 支持编译器
[GCC](https://baike.baidu.com/item/GCC)
[Microsoft Visual C++](https://baike.baidu.com/item/Microsoft%20Visual%20C%2B%2B) (C90. A few features of C99) (msvc doesn't support C99 at all. for example )
[ARM](https://baike.baidu.com/item/ARM) RealView
参考资料
- 1.[](https://baike.baidu.com/item/ANSI%20C/7657277?fr=aladdin#ref_%5B1%5D_3979609)[KnowledgeBase ME - C/C++ 介绍](https://baike.baidu.com/redirect/08921dUtGBlHGSOVu6BnYscdZ56v7symOVHoZTXs3dSMbXdoLH41wFWUmiBXxTQG3_iE9QLFb0J-OkiD5y9mNw)
- 2.[](https://baike.baidu.com/item/ANSI%20C/7657277?fr=aladdin#ref_%5B2%5D_3979609)[ISO/IEC 9899:1990 - Programming languages -- C](https://baike.baidu.com/redirect/ec32uhojEoLM_YMfJOVPc5Y3UnHM_g5BBeb_lwBaG2qE3KuruAt4ovaM7UxBM2TK5oulmcvlCxv6Ha8P22KVD1D9n8xpa5TSP3WBPHGKFp1HRkwh4NVp) ．国际标准化组织（ISO - International Organization for Standardization）[引用日期2014-11-22]
- 3.[](https://baike.baidu.com/item/ANSI%20C/7657277?fr=aladdin#ref_%5B3%5D_3979609)[ISO/IEC 9899:1999 - Programming languages -- C](https://baike.baidu.com/redirect/bb658JhYe9UwurCVhwow76YesLTS5BrRwPSRZIzdiGrWomUl2-1Pg_pPw0cj2ispe1Y8ETbxhAHvIdf5V_Kyenb3iUZAXHVdn-Q3TxA8mnX21vjygNHA) ．国际标准化组织（ISO - International Organization for Standardization）[引用日期2014-11-22]
- 4.[](https://baike.baidu.com/item/ANSI%20C/7657277?fr=aladdin#ref_%5B4%5D_3979609)[ISO/IEC 9899:2011 - Information technology -- Programming languages -- C](https://baike.baidu.com/redirect/6fc394nR9ENpgyvOlwDZkAJc3PCktZz-kLgSw14Y4a3ATNRso7rJ62cAxHokVHZNWaEWGnjhiEIvC544-VLvDClHW2ZWsHQUs5xc-8SZpTU106nHuEBdsEZpLjLFPqcY9Tm8tOOiXN6Uc89IwWfb9AIIEAU) ．国际标准化组织（ISO - International Organization for Standardization）[引用日期2014-11-22]
以上内容来自百度百科。
