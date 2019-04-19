# Unicode,GBK,GB2312,UTF-8概念基础 - 三少GG - CSDN博客
2012年10月15日 00:04:04[三少GG](https://me.csdn.net/scut1135)阅读数：1121标签：[语言																[character																[transformation																[java																[postscript																[byte](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=postscript&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=transformation&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)
个人分类：[重构C/C++																[PKU Research](https://blog.csdn.net/scut1135/article/category/801457)](https://blog.csdn.net/scut1135/article/category/621651)
为了构成一个完整的对文字编码的认识和深入把握，以便处理在Java开发过程中遇到的各种问题，特别是乱码问题，我觉得组成一个系列来描述和分析更好一些，包括三篇文章：第一篇：JAVA字符编码系列一：[Unicode,GBK,GB2312,UTF-8概念基础](http://gaozhiyan.spaces.live.com/?m=ViewArticle&id=184)第二篇：JAVA字符编码系列二：[Unicode,ISO-8859,GBK,UTF-8编码及相互转换](http://gaozhiyan.spaces.live.com/?m=ViewArticle&id=185)第三篇：JAVA字符编码系列三：[Java应用中的编码问题](http://gaozhiyan.spaces.live.com/?m=ViewArticle&id=186) 第一篇：JAVA字符编码系列一：Unicode,GBK,GB2312,UTF-8概念基础本部分采用重用，转载一篇文章来完成这部分的目标。来源：holen'blog   
 对字符编码与Unicode,ISO 10646,UCS,UTF8,UTF16,GBK,GB2312的理解地址：http://blog.donews.com/holen/archive/2004/11/30/188182.aspx Unicode:
unicode.org制定的编码机制, 要将全世界常用文字都函括进去.
在1.0中是16位编码, 由U+0000到U+FFFF. 每个2byte码对应一个字符; 在2.0开始抛弃了16位限制, 原来的16位作为基本位平面, 另外增加了16个位平面, 相当于20位编码, 编码范围0到0x10FFFF.
UCS: 
ISO制定的ISO10646标准所定义的 Universal Character Set, 采用4byte编码.
Unicode与UCS的关系:
ISO与unicode.org是两个不同的组织, 因此最初制定了不同的标准; 但自从unicode2.0开始, unicode采用了与ISO 10646-1相同的字库和字码, ISO也承诺ISO10646将不会给超出0x10FFFF的UCS-4编码赋值,
 使得两者保持一致.
UCS的编码方式:
UCS-2, 与unicode的2byte编码基本一样. 
UCS-4, 4byte编码, 目前是在UCS-2前加上2个全零的byte.
**UTF: **Unicode/UCS Transformation Format
UTF-8, 8bit编码, ASCII不作变换, 其他字符做变长编码, 每个字符1-3 byte. 通常作为外码. 有以下优点:
* 与CPU字节顺序无关, 可以在不同平台之间交流
* 容错能力高, 任何一个字节损坏后, 最多只会导致一个编码码位损失, 不会链锁错误(如GB码错一个字节就会整行乱码) 
UTF-16, 16bit编码, 是变长码, 大致相当于20位编码, 值在0到0x10FFFF之间, 基本上就是unicode编码的实现. 它是变长码, 与CPU字序有关, 但因为最省空间, 常作为网络传输的外码.
UTF-16是unicode的preferred encoding. 
UTF-32, 仅使用了unicode范围(0到0x10FFFF)的32位编码, 相当于UCS-4的子集.
**UTF与unicode的关系:**
Unicode是一个字符集, 可以看作为内码.
而UTF是一种编码方式, 它的出现是因为unicode不适宜在某些场合直接传输和处理. UTF-16直接就是unicode编码, 没有变换, 但它包含了0x00在编码内, 头256字节码的第一个byte都是0x00, 在操作系统(C语言)中有特殊意义, 会引起问题. 采用UTF-8编码对unicode的直接编码作些变换可以避免这问题, 并带来一些优点.
**中国国标编码:**
GB 13000: 完全等同于ISO 10646-1/Unicode 2.1, 今后也将随ISO 10646/Unicode的标准更改而同步更改.
GBK: 对GB2312的扩充, 以容纳GB2312字符集范围以外的Unicode 2.1的统一汉字部分, 并且增加了部分unicode中没有的字符. 
GB 18030-2000: 基于GB 13000, 作为Unicode 3.0的GBK扩展版本, 覆盖了所有unicode编码, 地位等同于UTF-8, UTF-16, 是一种unicode编码形式. 变长编码, 用单字节/双字节/4字节对字符编码. GB18030向下兼容GB2312/GBK. 
GB 18030是中国所有非手持/嵌入式计算机系统的强制实施标准.
-------------------------------
什么是 UCS 和 ISO 10646?
国际标准 **ISO 10646** 定义了 **通用字符集 (Universal Character Set, UCS)**. UCS 是所有其他字符集标准的一个超集. 它保证与其他字符集是双向兼容的. 就是说, 如果你将任何文本字符串翻译到 UCS格式, 然后再翻译回原编码, 你不会丢失任何信息.
UCS 包含了用于表达所有已知语言的字符. 不仅包括拉丁语,希腊语, 斯拉夫语,希伯来语,阿拉伯语,亚美尼亚语和乔治亚语的描述, 还包括中文, 日文和韩文这样的象形文字, 以及 平假名, 片假名, 孟加拉语, 旁遮普语果鲁穆奇字符(Gurmukhi), 泰米尔语, 印.埃纳德语(Kannada), Malayalam, 泰国语, 老挝语, 汉语拼音(Bopomofo), Hangul, Devangari, Gujarati, Oriya, Telugu 以及其他数也数不清的语. 对于还没有加入的语言, 由于正在研究怎样在计算机中最好地编码它们,
 因而最终它们都将被加入. 这些语言包括 Tibetian, 高棉语, Runic(古代北欧文字), 埃塞俄比亚语, 其他象形文字, 以及各种各样的印-欧语系的语言, 还包括挑选出来的艺术语言比如 Tengwar, Cirth 和 克林贡语(Klingon). UCS 还包括大量的图形的, 印刷用的, 数学用的和科学用的符号, 包括所有由 TeX, Postscript, MS-DOS，MS-Windows, Macintosh, OCR 字体, 以及许多其他字处理和出版系统提供的字符.
ISO 10646 定义了一个 31 位的字符集. 然而, 在这巨大的编码空间中, 迄今为止只分配了前 65534 个码位 (0x0000 到 0xFFFD). 这个 UCS 的 16位子集称为 **基本多语言面 (Basic Multilingual Plane, BMP)**. 将被编码在 16 位 BMP 以外的字符都属于非常特殊的字符(比如象形文字), 且只有专家在历史和科学领域里才会用到它们. 按当前的计划, 将来也许再也不会有字符被分配到从 0x000000 到 0x10FFFF
 这个覆盖了超过 100 万个潜在的未来字符的 21 位的编码空间以外去了. ISO 10646-1 标准第一次发表于 1993 年, 定义了字符集与 BMP 中内容的架构. 定义 BMP 以外的字符编码的第二部分 ISO 10646-2 正在准备中, 但也许要过好几年才能完成. 新的字符仍源源不断地加入到 BMP 中, 但已经存在的字符是稳定的且不会再改变了.
UCS 不仅给每个字符分配一个代码, 而且赋予了一个正式的名字. 表示一个 UCS 或 Unicode 值的十六进制数, 通常在前面加上 "U+", 就象 U+0041 代表字符"拉丁大写字母A". UCS 字符 U+0000 到 U+007F 与 US-ASCII(ISO 646) 是一致的, U+0000 到 U+00FF 与 ISO 8859-1(Latin-1) 也是一致的. 从 U+E000 到 U+F8FF, 已经 BMP 以外的大范围的编码是为私用保留的.
什么是组合字符?
UCS里有些编码点分配给了 **组合字符**.它们类似于打字机上的无间隔重音键. 单个的组合字符不是一个完整的字符. 它是一个类似于重音符或其他指示标记, 加在前一个字符后面. 因而, 重音符可以加在任何字符后面. 那些最重要的被加重的字符, 就象普通语言的正字法(orthographies of common languages)里用到的那种, 在 UCS 里都有自己的位置, 以确保同老的字符集的向后兼容性. 既有自己的编码位置, 又可以表示为一个普通字符跟随一个组合字符的被加重字符,
 被称为 **预作字符(precomposed characters)**. UCS 里的预作字符是为了同没有预作字符的旧编码, 比如 ISO 8859, 保持向后兼容性而设的. 组合字符机制允许在任何字符后加上重音符或其他指示标记, 这在科学符号中特别有用, 比如数学方程式和国际音标字母, 可能会需要在一个基本字符后组合上一个或多个指示标记.
组合字符跟随着被修饰的字符. 比如, 德语中的元音变音字符 ("拉丁大写字母A 加上分音符"), 既可以表示为 UCS 码 U+00C4 的预作字符, 也可以表示成一个普通 "拉丁大写字母A" 跟着一个"组合分音符":U+0041 U+0308 这样的组合. 当需要堆叠多个重音符, 或在一个基本字符的上面和下面都要加上组合标记时, 可以使用多个组合字符. 比如在泰国文中, 一个基本字符最多可加上两个组合字符.
什么是 UCS 实现级别?
不是所有的系统都需要支持象组合字符这样的 UCS 里所有的先进机制. 因此 ISO 10646 指定了下列三种实现级别:
- 
级别1
不支持组合字符和 Hangul Jamo 字符 (一种特别的, 更加复杂的韩国文的编码, 使用两个或三个子字符来编码一个韩文音节)- 
级别2
类似于级别1, 但在某些文字中, 允许一列固定的组合字符 (例如, 希伯来文, 阿拉伯文, Devangari, 孟加拉语, 果鲁穆奇语, Gujarati, Oriya, 泰米尔语, Telugo, 印.埃纳德语, Malayalam, 泰国语和老挝语). 如果没有这最起码的几个组合字符, UCS 就不能完整地表达这些语言.- 
级别3
支持所有的 UCS 字符, 例如数学家可以在任意一个字符上加上一个 tilde(颚化符号,西班牙语字母上面的～)或一个箭头(或两者都加).
什么是 Unicode?
历史上, 有两个独立的, 创立单一字符集的尝试. 一个是[国际标准化组织(ISO)](http://www.iso.ch/)的 ISO 10646 项目, 另一个是由(一开始大多是美国的)多语言软件制造商组成的协会组织的 [Unicode 项目](http://www.unicode.org/). 幸运的是, 1991年前后,
 两个项目的参与者都认识到, 世界不需要两个不同的单一字符集. 它们合并双方的工作成果, 并为创立一个单一编码表而协同工作. 两个项目仍都存在并独立地公布各自的标准, 但 Unicode 协会和 ISO/IEC JTC1/SC2 都同意保持 Unicode 和 ISO 10646 标准的码表兼容, 并紧密地共同调整任何未来的扩展.
那么 Unicode 和 ISO 10646 不同在什么地方?
Unicode 协会公布的 [Unicode 标准](http://www.unicode.org/unicode/standard/standard.html) 严密地包含了 ISO 10646-1 实现级别3的基本多语言面. 在两个标准里所有的字符都在相同的位置并且有相同的名字.
Unicode 标准额外定义了许多与字符有关的语义符号学, 一般而言是对于实现高质量的印刷出版系统的更好的参考. Unicode 详细说明了绘制某些语言(比如阿拉伯语)表达形式的算法, 处理双向文字(比如拉丁与希伯来文混合文字)的算法和 排序与字符串比较 所需的算法, 以及其他许多东西.
另一方面, ISO 10646 标准, 就象广为人知的 ISO 8859 标准一样, 只不过是一个简单的字符集表. 它指定了一些与标准有关的术语, 定义了一些编码的别名, 并包括了规范说明, 指定了怎样使用 UCS 连接其他 ISO 标准的实现, 比如 ISO 6429 和 ISO 2022. 还有一些与 ISO 紧密相关的, 比如 ISO 14651 是关于 UCS 字符串排序的.
考虑到 Unicode 标准有一个易记的名字, 且在任何好的书店里的 Addison-Wesley 里有, 只花费 ISO 版本的一小部分, 且包括更多的辅助信息, 因而它成为使用广泛得多的参考也就不足为奇了. 然而, 一般认为, 用于打印 ISO 10646-1 标准的字体在某些方面的质量要高于用于打印 Unicode 2.0的. 专业字体设计者总是被建议说要两个标准都实现, 但一些提供的样例字形有显著的区别. ISO 10646-1 标准同样使用四种不同的风格变体来显示表意文字如中文, 日文和韩文 (CJK),
 而 Unicode 2.0 的表里只有中文的变体. 这导致了普遍的认为 Unicode 对日本用户来说是不可接收的传说, 尽管是错误的.
什么是 UTF-8?
首先 UCS 和 Unicode 只是分配整数给字符的编码表. 现在存在好几种将一串字符表示为一串字节的方法. 最显而易见的两种方法是将 Unicode 文本存储为 2 个 或 4 个字节序列的串. 这两种方法的正式名称分别为 UCS-2 和 UCS-4. 除非另外指定, 否则大多数的字节都是这样的(Bigendian convention). 将一个 ASCII 或 Latin-1 的文件转换成 UCS-2 只需简单地在每个 ASCII 字节前插入 0x00. 如果要转换成 UCS-4, 则必须在每个 ASCII
 字节前插入三个 0x00.
在 Unix 下使用 UCS-2 (或 UCS-4) 会导致非常严重的问题. 用这些编码的字符串会包含一些特殊的字符, 比如 '' 或 '/', 它们在 文件名和其他 C 库函数参数里都有特别的含义. 另外, 大多数使用 ASCII 文件的 UNIX 下的工具, 如果不进行重大修改是无法读取 16 位的字符的. 基于这些原因, 在文件名, 文本文件, 环境变量等地方, **UCS-2** 不适合作为 **Unicode** 的外部编码.
在 ISO 10646-1 [Annex R](http://www.cl.cam.ac.uk/~mgk25/ucs/ISO-10646-UTF-8.html) 和 里定义的 **UTF-8** 编码没有这些问题. 它是在 Unix 风格的操作系统下使用 Unicode 的明显的方法.
UTF-8 有一下特性:
- 
UCS 字符 U+0000 到 U+007F (ASCII) 被编码为字节 0x00 到 0x7F (ASCII 兼容). 这意味着只包含 7 位 ASCII 字符的文件在 ASCII 和 UTF-8 两种编码方式下是一样的.
- 
所有 >U+007F 的 UCS 字符被编码为一个多个字节的串, 每个字节都有标记位集. 因此, ASCII 字节 (0x00-0x7F) 不可能作为任何其他字符的一部分.
- 
表示非 ASCII 字符的多字节串的第一个字节总是在 0xC0 到 0xFD 的范围里, 并指出这个字符包含多少个字节. 多字节串的其余字节都在 0x80 到 0xBF 范围里. 这使得重新同步非常容易, 并使编码无国界, 且很少受丢失字节的影响.
- 
可以编入所有可能的 231个 UCS 代码
- 
UTF-8 编码字符理论上可以最多到 6 个字节长, 然而 16 位 BMP 字符最多只用到 3 字节长.
- 
Bigendian UCS-4 字节串的排列顺序是预定的.
- 
字节 0xFE 和 0xFF 在 UTF-8 编码中从未用到.
下列字节串用来表示一个字符. 用到哪个串取决于该字符在 Unicode 中的序号.
U-00000000 - U-0000007F: 0*xxxxxxx* U-00000080 - U-000007FF: 110*xxxxx* 10*xxxxxx* U-00000800 - U-0000FFFF: 1110*xxxx* 10*xxxxxx* 10*xxxxxx* U-00010000 - U-001FFFFF: 11110*xxx* 10*xxxxxx* 10*xxxxxx* 10*xxxxxx* U-00200000
 - U-03FFFFFF: 111110*xx* 10*xxxxxx* 10*xxxxxx* 10*xxxxxx* 10*xxxxxx* U-04000000 - U-7FFFFFFF: 1111110*x* 10*xxxxxx* 10*xxxxxx* 10*xxxxxx* 10*xxxxxx* 10*xxxxxx*
xxx 的位置由字符编码数的二进制表示的位填入. 越靠右的 x 具有越少的特殊意义. 只用最短的那个足够表达一个字符编码数的多字节串. 注意在多字节串中, 第一个字节的开头"1"的数目就是整个串中字节的数目.
**例如**: Unicode 字符 U+00A9 = 1010 1001 (版权符号) 在 UTF-8 里的编码为:
> 
11000010 10101001 = 0xC2 0xA9
而字符 U+2260 = 0010 0010 0110 0000 (不等于) 编码为:
> 
11100010 10001001 10100000 = 0xE2 0x89 0xA0
这种编码的官方名字拼写为 UTF-8, 其中 UTF 代表 **U**CS **T**ransformation **F**ormat. 请勿在任何文档中用其他名字 (比如 utf8 或 UTF_8) 来表示 UTF-8, 当然除非你指的是一个变量名而不是这种编码本身.
什么编程语言支持 Unicode?
在大约 1993 年之后开发的大多数现代编程语言都有一个特别的数据类型, 叫做 Unicode/ISO 10646-1 字符. 在 Ada95 中叫 Wide_Character, 在 Java 中叫 char.
ISO C 也详细说明了处理多字节编码和宽字符 (wide characters) 的机制, 1994 年 9 月 [Amendment 1 to ISO C](http://www.lysator.liu.se/c/na1.html) 发表时又加入了更多. 这些机制主要是为各类东亚编码而设计的, 它们比处理 UCS 所需的要健壮得多. UTF-8 是 ISO C 标准调用多字节字符串的编码的一个例子, *wchar_t* 类型可以用来存放
 Unicode 字符.
