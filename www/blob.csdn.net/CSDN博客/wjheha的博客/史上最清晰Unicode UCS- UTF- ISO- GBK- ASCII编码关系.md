# 史上最清晰Unicode/UCS-/UTF-/ISO-/GBK-/ASCII编码关系 - wjheha的博客 - CSDN博客
2017年04月11日 17:52:03[wjheha](https://me.csdn.net/wjheha)阅读数：1746
## 史上最清晰Unicode/UCS-/UTF-/ISO-/GBK-/ASCII编码关系
  最近看到了Java的流概念，出现了字节流和字符流，看来看去还是不太懂两者的区别，索性一下子从头开始，先把字符串、编码的概念搞清楚。
### ASCII
**我们知道，在计算机内部，所有的信息最终都表示为一个二进制的序列。**每一个二进制位（bit）有0和1两种状态，因此八个二进制位就可以组合出256种状态，这被称为一个字节（byte）。也就是说，一个字节一共可以用来表示256种不同的状态或者符号。如果我们制作一张对应表格，对于每一个8位二进制序列，都对应唯一的一个符号。每一个状态对应一个符号，就是256个符号，从0000000到11111111。
  上个世纪60年代，美国制定了一套字符编码，对英语字符与二进制位之间的关系，做了统一规定。这被称为ASCII码，一直沿用至今。 
  ASCII码一共规定了128个字符的编码，比如空格”SPACE”是32（二进制00100000），大写的字母A是65（二进制01000001）。这128个符号（包括32个不能打印出来的控制符号），只占用了一个字节的后面7位，最前面的1位统一规定为0。
### ISO/IEC 8859-1(15)
**先解释一下ISO,**
ISO国际标准化组织（英语：International Organization for Standardization),[https://zh.wikipedia.org/wiki/%E5%9C%8B%E9%9A%9B%E6%A8%99%E6%BA%96%E5%8C%96%E7%B5%84%E7%B9%94](https://zh.wikipedia.org/wiki/%E5%9C%8B%E9%9A%9B%E6%A8%99%E6%BA%96%E5%8C%96%E7%B5%84%E7%B9%94)
ISO 8859-1，正式编号为ISO/IEC 8859-1:1998，又称Latin-1或“西欧语言”，是国际标准化组织内ISO/IEC 8859的第一个8位字符集。它以ASCII为基础，在空置的0xA0-0xFF的范围内，加入96个字母及符号，藉以供使用附加符号的拉丁字母语言使用。
ISO 8859-15，正式编号为ISO/IEC 8859-15:1999，又称Latin-9，俗称Latin-0，是国际标准化组织内ISO/IEC 8859的其中一个8位字符集。
**接着ASCII:**  英语用128个符号编码就够了，但是用来表示其他语言，128个符号是不够的。比如，在法语中，字母上方有注音符号，它就无法用ASCII码表示。于是，一些欧洲国家就决定，利用字节中闲置的最高位编入新的符号。比如，法语中的é的编码为130（二进制10000010）。这样一来，这些欧洲国家使用的编码体系，可以表示最多256个符号。
  还有好多ISO-8859XXX什么鬼的，基本就是加强版，多添加几个编码字符罢了。
在知乎上看到一个解释，很生动有趣，贴在这里。 
Unicode 和 UTF-8 有何区别？[https://www.zhihu.com/question/23374078](https://www.zhihu.com/question/23374078)
![这里写图片描述](https://img-blog.csdn.net/20170411145346449?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### GBK-
  随着计算机的普及，我天朝人民也需要使用计算机了，等中国人们得到计算机时，已经没有可以利用的字节状态来表示汉字，况且有6000多个常用汉字需要保存呢。 
  但是这难不倒智慧的中国人民，我们不客气地把那些127号之后的奇异符号们直接取消掉, 规定：一个小于127的字符的意义与原来相同，但两个大于127的字符连在一起时，就表示一个汉字，前面的一个字节（他称之为高字节）从0xA1用到 0xF7，后面一个字节（低字节）从0xA1到0xFE，这样我们就可以组合出大约7000多个简体汉字了。 
  在这些编码里，我们还把数学符号、罗马希腊的字母、日文的假名们都编进去了，连在 ASCII 里本来就有的数字、标点、字母都统统重新编了两个字节长的编码，这就是常说的”全角”字符，而原来在127号以下的那些就叫”半角”字符了。 中国人民看到这样很不错，于是就把这种汉字方案叫做 “GB2312“。GB2312 是对 ASCII 的中文扩展。
      但是中国的汉字太多了，我们很快就就发现有许多人的人名没有办法在这里打出来，特别是某些很会麻烦别人的国家领导人。于是我们不得不继续把GB2312 没有用到的码位找出来老实不客气地用上。  
后来还是不够用，于是干脆不再要求低字节一定是127号之后的内码，只要第一个字节是大于127就固定表示这是一个汉字的开始，不管后面跟的是不是扩展字符集里的内容。结果扩展之后的编码方案被称为 GBK 标准，GBK包括了GB2312 的所有内容，同时又增加了近20000个新的汉字（包括繁体字）和符号。 后来少数民族也要用电脑了，于是我们再扩展，又加了几千个新的少数民族的字，GBK扩成了 GB18030。从此之后，中华民族的文化就可以在计算机时代中传承了。 中国的程序员们看到这一系列汉字编码的标准是好的，于是通称他们叫做 “DBCS“（Double Byte Charecter Set  
双字节字符集）。在DBCS系列标准里，最大的特点是两字节长的汉字字符和一字节长的英文字符并存于同一套编码方案里，因此他们写的程序为了支持中文处理，必须要注意字串里的每一个字节的值，如果这个值是大于127的，那么就认为一个双字节字符集里的字符出现了。那时候凡是受过加持，会编程的计算机僧侣 们都要每天念下面这个咒语数百遍： “一个汉字算两个英文字符！一个汉字算两个英文字符……”
**好，到这里，我们已经发现出现了好多编码方案，大家你搞你一套，我搞我一套，结果乱了套。**
  因为当时各个国家都像中国这样搞出一套自己的编码标准，结果互相之间谁也不懂谁的编码，谁也不支持别人的编码，连大陆和台湾这样只相隔了150海里，使用着同一种语言的兄弟地区，也分别采用了不同的 DBCS  
编码方案——当时的中国人想让电脑显示汉字，就必须装上一个”汉字系统”，专门用来处理汉字的显示、输入的问题，但是那个台湾的愚昧封建人士写的算命程序 
就必须加装另一套支持 BIG5 编码的什么”倚天汉字系统”才可以用，装错了字符系统，显示就会乱了套！这怎么办？而且世界民族之林中还有那些一时用不上电脑的穷苦人民，他们的文字又怎么办？ 真是计算机的巴比伦塔命题啊！正在这时，大天使加百列及时出现了——一个叫 ISO  
（国际标谁化组织）的国际组织决定着手解决这个问题。他们采用的方法很简单：废了所有的地区性编码方案，重新搞一个包括了地球上所有文化、所有字母和符号的编码！他们打算叫它”Universal Multiple-Octet Coded Character Set”，简称 UCS, 俗称 “unicode“。
### Unicode（中文：万国码、国际码、统一码、单一码）
**终于到Unicode了，这是本文重点要解释的地方，也是造成大多数人困惑的地方。**
  先看维基百科的解释，觉得已经很清楚了。
Unicode 
[https://zh.wikipedia.org/wiki/Unicode](https://zh.wikipedia.org/wiki/Unicode)
这里，拿出最重要的几句话。
**大概来说，Unicode编码系统可分为编码方式和实现方式两个层次。**
- 
**统一码（Unicode）的编码方式与ISO 10646的通用字符集概念相对应。**
目前实际应用的统一码版本对应于UCS-2，使用16位的编码空间。也就是每个字符占用2个字节。这样理论上一共最多可以表示216（即65536）个字符。基本满足各种语言的使用。实际上当前版本的统一码并未完全使用这16位编码，而是保留了大量空间以作为特殊使用或将来扩展。
- 
**Unicode的实现方式不同于编码方式。**
一个字符的Unicode编码是确定的。但是在实际传输过程中，由于不同系统平台的设计不一定一致，以及出于节省空间的目的，对Unicode编码的实现方式有所不同。Unicode的实现方式称为Unicode转换格式（Unicode Transformation Format，简称为UTF）
## 这句话“**统一码（Unicode）的编码方式**与**ISO 10646的通用字符集**概念相对应”什么意思呢？
- 先解释通用字符集，通用字符集（英语：Universal Character Set, UCS）是由ISO制定的ISO 10646（或称ISO/IEC 10646）标准所定义的标准字符集。 
看一下通用字符集UCS的维基百科： 
[https://zh.wikipedia.org/wiki/%E9%80%9A%E7%94%A8%E5%AD%97%E7%AC%A6%E9%9B%86](https://zh.wikipedia.org/wiki/%E9%80%9A%E7%94%A8%E5%AD%97%E7%AC%A6%E9%9B%86)
**为什么整出了两个对应字符集呢？下面给出了解释。**
///////////////////////////////////////////////////////////////////////////////////////////// 
历史上存在两个独立的尝试创立单一字符集的组织，即
- 国际标准化组织（ISO）于1984年创建的ISO/IEC 
  JTC1/SC2/WG2，其含义是International Organization for Standardization / International Electrotechnical Commission, Joint Technical Committee #1 [Information Technology], Subcommittee #2 [Coded Character Sets], Working Group #2 [Multi-octet codes]). ISO 10646表示这是ISO 646的扩展。
- 由Xerox、Apple等软件制造商于1988年组成的统一码联盟。前者开发的ISO/IEC 10646项目，后者开发的统一码项目。因此最初制定了不同的标准。
1991年前后，两个项目的参与者都认识到，世界不需要两个不兼容的字符集。于是，它们开始合并双方的工作成果，并为创立一个单一编码表而协同工作。1991年，不包含CJK统一汉字集的Unicode 1.0发布。随后，CJK统一汉字集的制定于1993年完成，发布了ISO 10646-1:1993，即Unicode 1.1。 
从Unicode 2.0开始，Unicode采用了与ISO 10646-1相同的字库和字码；ISO也承诺，ISO 10646将不会替超出U+10FFFF的UCS-4编码赋值，以使得两者保持一致。两个项目仍都独立存在，并独立地公布各自的标准。但统一码联盟和ISO/IEC JTC1/SC2都同意保持两者标准的码表兼容，并紧密地共同调整任何未来的扩展。在发布的时候，Unicode一般都会采用有关字码最常见的字体，但ISO 10646一般都尽可能采用Century字体。* 
/////////////////////////////////////////////////////////////////////////////////////////////////////
**由此可知，这俩家统一以后，用了同一个通用的字符集**
2.然后解释Unicode编码方式 
**最最最重要！！！！，Unicode有编码方式和实现方式。**
**注意这里的 “编码” 和前面ASCII/ISO-88591/GBK “编码”的区别。这里(Unicode)的编码是指，根据通用字符集，将符号（A 、8、+）对应着某一个编号，并不涉及具体的存储（实现）。Unicode只是一个符号集，它只规定了符号的二进制代码，却没有规定这个二进制代码应该如何存储。也就是说，这里相对于之前的编码概念，又向上抽象了一层，用下图表示：**
![这里写图片描述](https://img-blog.csdn.net/20170411175026108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**观察上图，Unicode编码方式其实是指从Unicode字符到Unicode代码点（编号）的一个编码过程。**这里的代码点是指码位或称编码位置，即英文的code point或code position。可想而知，Unicode字符集包含了世界各国的文字符号，给它们都分配了一个编号，这个编号数值相当大。
目前，Unicode的编码空间从U+0000到U+10FFFF（U+表示16进制），共有1,112,064个码位（code point），将这Unicode字符分为17组编排，每组称为平面（Plane），则每平面拥有65536（即216）个代码点。然而目前只用了少数平面。第一个平面称为**基本多语言平面**（Basic Multilingual Plane, BMP），或称第零平面（Plane 0）。其他平面称为辅助平面（Supplementary Planes）。基本多文种平面（Basic Multilingual Plane, BMP）编码从U+0000至U+FFFF。
Unicode的字符平面映射 
[https://zh.wikipedia.org/wiki/Unicode%E5%AD%97%E7%AC%A6%E5%B9%B3%E9%9D%A2%E6%98%A0%E5%B0%84](https://zh.wikipedia.org/wiki/Unicode%E5%AD%97%E7%AC%A6%E5%B9%B3%E9%9D%A2%E6%98%A0%E5%B0%84)
将第0平面代码点数值用二进制来表示，则刚好需要2个字节16位。这正是**UCS-2**的定义：用16位来表示代码点（编号）。但是，如果要表示所有的1,112,064个码位，16位显然不够，所以后期出现了**UCS-4**的编码方式，也就是使用了4个字节来表示所有的字符对应的代码点数值。UCS-4足以用来表示所有的Unicode的字码空间，其最大的码位为十六进制的7FFFFFFF，所以其空间约20亿个码位。有些人认为保留如此大的字码空间却只为了对应这很小的码集是浪费，所以又有了后期的改进。
UCS-2 
[https://zh.wikipedia.org/wiki/UTF-16](https://zh.wikipedia.org/wiki/UTF-16)
UCS-4 
[https://zh.wikipedia.org/wiki/UTF-32](https://zh.wikipedia.org/wiki/UTF-32)
到此为止，Unicode的编码方式终于说完了，
## 这就话**“Unicode的实现方式不同于编码方式”**是什么意思？
上述可知，得到了Unicode编码方式的解释，那实现呢？
对于第0平面字符，如果直接将字符对应的代码点（）进行最终二进制序列的存储，就是UTF-16的实现方式。那这样UCS-2和UTF-16不就是一个东西了吗？然而，并不是，
基本多语言平面内，从U+D800到U+DFFF之间的码位区块是永久保留不映射到Unicode字符。UTF-16就利用保留下来的0xD800-0xDFFF区段的码位来对辅助平面的字符的码位进行编码。
这句话是什么意思呢?
- 对于代码点落在基本多语言平面的字符，其UCS-2编码得到代码点，然后再向下走采用UTF-16实现，最终得到的二进制就是代码点的二进制表示，从这一点意义来看，UTF-16和UCS-2完全可以合二为一，把两个过程变成一个。
- 对于代码点落在辅助平面的字符，UTF-16也想用来表示，前面讨论过，Unicode字符一共有1,112,064，除掉65536（实际更少），还有很多，所以UTF-16采用了**一对**16比特来表示。辅助平面中的码位从U+10000到U+10FFFF，共计FFFFF个，即220=1,048,576个，需要20位来表示，如果用两个16位长的整数组成的序列来表示，第一个整数（称为前导代理）要容纳上述20位的前10位，第二个整数（称为后尾代理）容纳上述20位的后10位。 
这样的表示如何与第0平面相区别呢？刚刚已经说了，UCS-2代码点留了0xD800-0xDFFF没有映射，所以高位的10比特的值（值的范围为0..0x3FF）被加上0xD800，低位的10比特的值（值的范围也是0..0x3FF）被加上0xDC00，这样就能区分一个16位是表示0平面还是辅助平面。从这一层意义来看。UCS-2和UTF-16又是不同的。
综上所述，由于UCS-2和UTF-16紧密的关系，大家经常把这两者搞成一个东西，这里，我们记得平常所说的Unicode编码指的是UCS-2的实现，也就是UTF-16编码。另外再注意一下大小端的实现，little endian和big endian。
UCS-4和UTF-32是同样的道理，只不过4个字节足够了，可以直接进行一一对应，代码点直接作为二进制存储，维基百科行[https://zh.wikipedia.org/wiki/UTF-32](https://zh.wikipedia.org/wiki/UTF-32)
UCS-2 /UTF-16 
[https://zh.wikipedia.org/wiki/UTF-16](https://zh.wikipedia.org/wiki/UTF-16)
UCS-4 /UTF-32 
[https://zh.wikipedia.org/wiki/UTF-32](https://zh.wikipedia.org/wiki/UTF-32)
到此为止，Unicode的编码方式终于说完了，
#### UTF-8
UTF-8就是变长存储，把字符转成代码点后，针对代码点（每一个字符）进行变长编码。 
![这里写图片描述](https://img-blog.csdn.net/20170411172915783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2poZWhh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
终于要写完了，值的提一下，Java中的char类型，在内存中、JVM存储的就是字符的代码点，没有UTF-16/UTF-8的编码实现，就是简简单单、大家都一样的代码点数值的二进制形式。
贴上一个链接，写的超好， 
Java 平台中的增补字符：[https://www.oschina.net/question/12_12216](https://www.oschina.net/question/12_12216)
参考链接： 
[https://www.ibm.com/developerworks/cn/java/j-lo-chinesecoding/](https://www.ibm.com/developerworks/cn/java/j-lo-chinesecoding/)
[http://blog.csdn.net/gjb724332682/article/details/43229563](http://blog.csdn.net/gjb724332682/article/details/43229563)
[http://blog.csdn.net/u013905744/article/details/52334706](http://blog.csdn.net/u013905744/article/details/52334706)
[http://blog.csdn.net/LogicTeamLeader/article/details/52434840](http://blog.csdn.net/LogicTeamLeader/article/details/52434840)
[http://guoxinmiao8.blog.sohu.com/129816401.html](http://guoxinmiao8.blog.sohu.com/129816401.html)
[https://zh.wikipedia.org/wiki/Unicode](https://zh.wikipedia.org/wiki/Unicode)
[http://www.ruanyifeng.com/blog/2007/10/ascii_unicode_and_utf-8.html](http://www.ruanyifeng.com/blog/2007/10/ascii_unicode_and_utf-8.html)
[http://xiaowei-qi-epro-com-cn.iteye.com/blog/1927862](http://xiaowei-qi-epro-com-cn.iteye.com/blog/1927862)
