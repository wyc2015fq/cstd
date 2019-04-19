# K&R C与ANSI C - Koma Hub - CSDN博客
2019年01月31日 20:26:38[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：31
起初，C语言没有官方标准。1978年由[美国电话电报公司](https://baike.baidu.com/item/%E7%BE%8E%E5%9B%BD%E7%94%B5%E8%AF%9D%E7%94%B5%E6%8A%A5%E5%85%AC%E5%8F%B8)(AT&T）贝尔实验室正式发表了C语言。布莱恩·柯林汉（Brian Kernighan） 和 丹尼斯·里奇（Dennis Ritchie） 出版了一本书，名叫《[The C Programming Language](https://baike.baidu.com/item/The%20C%20Programming%20Language)》。这本书被 C语言开发者们称为**K&R**，很多年来被当作 C语言的非正式的标准说明。人们称这个版本的 C语言为**K&R C**。
K&R C主要介绍了以下特色：
[结构体](https://baike.baidu.com/item/%E7%BB%93%E6%9E%84%E4%BD%93)（struct）类型
长整数（long int）类型
无符号整数（unsigned int）类型
把运算符=+和=-改为+=和-=。因为=+和=-会使得编译器不知道使用者要处理i = -10还是i =- 10，使得处理上产生混淆。
即使在后来[ANSI C](https://baike.baidu.com/item/ANSI%20C)标准被提出的许多年后，K&R C仍然是许多编译器的最 准要求，许多老旧的编译器仍然运行K&R C的标准。
1970到80年代，C语言被广泛应用，从大型主机到小型微机，也衍生了C语言的很多不同版本。
1983年，美国国家标准协会（ANSI）成立了一个委员会X3J11，来制定 C语言标准。 
1989年，美国国家标准协会（ANSI）通过了C语言标准，被称为**ANSI X3.159-1989 "Programming Language C"**。因为这个标准是1989年通过的，所以一般简称**C89标准**。有些人也简称**ANSI C**，因为这个标准是美国国家标准协会（ANSI）发布的。
Which Version of C to Use?
At this point, anyone learning or using C should be working with ANSI C, not K&R C.
The language standard draft was finally adopted by ANSI in December 1989. The international standards organization ISO then adopted the ANSI C standard (unhappily removing the very useful "Rationale" section and making trivial—but very annoy-ing—formatting and paragraph numbering changes). ISO, as an international body, is technically the senior organization, so early in 1990 ANSI readopted ISO C (again exclud-ing the Rationale) back in place of its own version. In principle, therefore, we should say that the C standard adopted by ANSI is ISO C, and we should refer to the language as ISO C. The Rationale is a useful text that greatly helps in understanding the standard, and it's published as a separate document.
