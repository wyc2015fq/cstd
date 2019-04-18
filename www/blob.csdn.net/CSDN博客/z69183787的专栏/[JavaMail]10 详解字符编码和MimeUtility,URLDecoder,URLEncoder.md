# [JavaMail]10 详解字符编码和MimeUtility,URLDecoder,URLEncoder - z69183787的专栏 - CSDN博客
2018年02月02日 14:21:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1517
**一、邮件中的编码**
**1. Message的header**
Message中的header都是只能存放ASCII码的，当邮件中有非ASCII时，例如邮件的主题是中文时，我们可以把邮件的中文主题以UTF-8（当然也可以使用其它编码，如GB2312）规则来进行编码，并以指定格式的ASCII字符串来表示。
若邮件的主题是“中”，我们想用UTF-8编码。那么“中”字的UTF-8的编码是E4 B8 AD，就用特定格式的ASCII码来表示主题“中”，则是：Subject: =?UTF-8?Q?=E4=B8=AD?=（这个是RFC2047规范的格式）。
**2.BodyPart的Header**
BodyPart中的Header也是只能存放ASCII码的，例如邮件的有一个附件的文件名是中文时，我们可以把附件名进行编码，并以指定格式的ASCII字符串来表示，注意差别是这时候的ASCII字符串格式和前面不同。格式：UTF-8''中（这个是W3C规范的格式）。
为什么在使用一个邮件中使用两种格式，就不得而知了，可能是方便下载附件吧。
**二、Message Header的编码和解码**
**A、编码规则**
"=?" charset "?" encoding "?" encoded-text "?="
charset：字符编码
encoding：
Q --- Quote Printable
B --- BASE64
U --- UUENCODE
最常用的 Content-Transfer-Encoding 有 Base64 和 Quoted-Printable 两种。在对二进制文件或者中文文本进行转化时，Base64 得到的“字节串”比 Quoted-Printable 更短。在对英文文本进行转化时，Quoted-Printable 得到的“字节串”比 Base64 更短。
例1: Subject: =?GB2312?Q?=C4=E3=BA=C3?=
例2: Subject: =?utf-7?B?WFgrWUNkU0swNE9kbjVPRVZiKy0z?=
**B、javax.mail.internet.MimeUtility**
*public class MimeUtility*
这是一个工具类，提供各种MIME相关的功能。
该类有一组根据RFC2047规范来对MIME头进行编码和解码的方法。请注意，在一般情况下，这些方法并不需要被使用，如setSubject和setRecipients时。使用这些“高层次”方法时JavaMail会自动编码和解码。这些方法只有在使用setHeader及getHeader方法操作MIME头时才需要被使用。
RFC 822规定邮件头只能包含US-ASCII字符。当包含有非US-ASCII字符的邮件头必须进行编码以使邮件头只包含US-ASCII字符。基本上，这个过程涉及使用Base64或QP规则来编码某些字符。 RFC 2047中详细说明了这一点。
在JAVA中String是16为unicode字符。ASCII是Unicode的一个子集（占用范围0 - 127）。一个只包含ASCII字符的String是邮件安全的。若String含有非ASCII字符则必须进行编码。此步骤增加了一个额外的复杂性，是因为Unicode还不是一种广泛使用的字符集，有人可能首先想到把String的字符集编码转换成其它一种字符集编码，然后来进行传输。请注意，要获得邮件安全字符串的实际字节（例如，通过SMTP发送），必须做的是：
byte[] bytes = string.getBytes("iso-8859-1");
假设MimeMessage、MimeBodyPart的setHeader和AddHeader方法给header赋的值是一个只包含US-ASCII字符的Unicode字符串。那么这些方法的调用者必须确保他们传递的值不包含非US-ASCII字符。这个类的方法帮忙做这些事情。
MimeMessage、MimeBodyPart的getHeader方法获取header的值时。这些值是按RFC 2047规范编码的，那么就必须解码为Unicode编码。这个类的方法帮忙做这些事情。
几个系统属性严格控制MIME规范的一致性。请注意，这些都不是session的属性，但必须设置为全局系统属性。
**mail.mime.decodetext.strict**属性控制MIME编码字符的解码。MIME规范要求，编码字符必须以一个空白分隔符为开始。一些邮件错误的包含了编码字符在一个词的中间。如果mail.mime.decodetext.strict系统属性设置为false，系统就会企图解码这些非法的编码字符。默认值是true。
**mail.mime.encodeeol.strict**属性控制非”text”类型的MIME Part的Content-Transfer-Encoding的选择。如果在mail.mime.encodeeol.strict的系统属性设置为“true”，必要时将使用这种编码。默认值是true。
**mail.mime.charset**系统属性用来指定默认的MIME字符集。通常情况下，默认的MIME字符集是来自默认的Java字符集，在file.encoding系统属性中指定。大多数应用程序将有没有必要明确地设置默认的MIME字符集。邮件message使用的默认的MIME字符集是不同于存放在系统中文件的字符集。
**mail.mime.ignoreunknownencoding**属性用来控制Content-Transfer-Encoding头中未知值，当递给解码方法，会导致异常。如果设置为“true”，未知的值被忽略和被假设为8bit编码。否则，未知值会导致以抛出MessagingException异常。
**静态方法**
**1.解码**
static InputStream  decode(InputStream is, String encoding)
*Decode the given input stream.*
static String     decodeText(String etext)
*Decode "unstructured" headers, that is, headers that are defined as '*text' as per RFC 822.*
解码根据RFC822定义为*text的“非结构化”头。
这个字符串使用RFC2047中第6.1章中指定的算法解码方式。如果字符集转换失败，则抛出一个UnsupportedEncodingException例外。如果String不是RFC 2047编码格式的头，则原样返回。
static String     decodeWord(String eword)
*The string is parsed using the rules in RFC 2047 and RFC 2231 for parsing an "encoded-word".*
使用RFC 2047和RFC 2231规则对一个编码字符串进行解码。如果解析失败，则抛出一个ParseException例外。否则，就传输解码，然后把编码转换成Unicode字符。如果字符集转换失败，则抛出一个UnsupportedEncodingException例外。
**2.编码**
static OutputStream        encode(OutputStream os, String encoding)
*Wrap an encoder around the given output stream.*
static OutputStream        encode(OutputStream os, String encoding, String filename)
*Wrap an encoder around the given output stream.*
static String     encodeText(String text)
*Encode a RFC 822 "text" token into mail-safe form as per RFC 2047.*
static String     encodeText(String text, String charset, String encoding)
*Encode a RFC 822 "text" token into mail-safe form as per RFC 2047.*
static String     encodeWord(String word)
*Encode a RFC 822 "word" token into mail-safe form as per RFC 2047.*
static String     encodeWord(String word, String charset, String encoding)
*Encode a RFC 822 "word" token into mail-safe form as per RFC 2047.*
**3.获取encoding类型**
static String     getEncoding(DataHandler dh)
*Same as getEncoding(DataSource) except that instead of reading the data from an InputStream it uses the writeTo method to examine the data.*
static String     getEncoding(DataSource ds)
*Get the content-transfer-encoding that should be applied to the input stream of this datasource, to make it mailsafe.*
**3.获取系统字符集编码**
static String     getDefaultJavaCharset()
*Get the default charset corresponding to the system's current default locale.*
**4.**
static String     javaCharset(String charset)
*Convert a MIME charset name into a valid Java charset name.*
static String     mimeCharset(String charset)
*Convert a java charset into its MIME charset name.*
static String     quote(String word, String specials)
*A utility method to quote a word, if the word contains any characters from the specified 'specials' list.*
**三、BodyPart Header的编码和解码**
**A、编码规则**
charset’ language’encoded-text
charset：字符编码
language：语言
language：编码后的ASCII字符串
例1:UTF-8''中
编码用单引号分成了三段，分别是字符集(utf8)、语言(空)和urlencode过的文件名
**B、URLDecoder和URLEncoder**
**1.java.net.URLDecoder**
*public class URLDecoder*
HTML 格式解码的实用工具类。该类包含了static方法decode(String s, String enc)可以将一个字符串从application/x-www-form-urlencoded MIME格式中解码。
假定已编码的字符串中只能是以下字符："a" 到 "z"、"A" 到 "Z"、"0" 到 "9" 和 "-"、"_"、"." 以及 "*"。允许有 "%" 字符，但是将它解释为特殊转义序列的开始。如：中。
转换中使用以下规则：
1.字母数字字符 "a" 到 "z"、"A" 到 "Z" 和 "0" 到 "9" 保持不变。
2.特殊字符 "."、"-"、"*" 和 "_" 保持不变。
3.加号 "+" 转换为空格字符 " "。
4.将把 "%xy" 格式序列视为一个字节，其中 xy 为 8 位的两位十六进制表示形式（xy是16进制数，其中的一位表示一个byte）。然后，所有连续包含一个或多个这些字节序列的子字符串，将被其编码可生成这些连续字节的字符所代替。可以指定对这些字符进行解码的编码机制，或者如果未指定的话，则使用平台的默认编码机制。
该解码器处理非法字符串有两种可能的方法。一种方法是不管该非法字符，另一种方法是抛出 IllegalArgumentException 异常。解码器具体采用哪种方法取决于具体实现。
**static String     decode(String s, String enc)**
使用指定的编码机制对 application/x-www-form-urlencoded 字符串解码。
例子1：
String s=” 中”;
//中是“中”字UTF-8的编码的application/x-www-form-urlencoded MIME表示方式。
URLDecoder.decode(s, "UTF-8");
//则指定用UTF-8编码来进行解码。
例子2：
String s="��";
URLDecoder.decode(s, "GB2312");
//��是“中”字GB2312的编码的application/x-www-form-urlencoded MIME表示方式。则指定用GB2312编码来进行解码。
**2.java.net.URLEncoder**
*public class URLEncoder*
该转换过程正好与 URLEncoder 类使用的过程相反。
**static String     encode(String s, String enc)**
使用指定的编码机制将字符串转换为application/x-www-form-urlencoded格式。
**C、特殊符号的编码和解码**
**1.空格**
空格在邮件中存储或者在网页的URL中则是 。空格用URLEncoder.encode(“ “,”UTF-8”);来进行编码是会被转换为”+”。而在解码时 和+都被解码为空格。
**2.+**
+在邮件中存储仍然是+。那么对含有+的字符串进行解码时就会把+解码为空格，**这样就和事实不符了。**
**3.%**
%在邮件中存储仍然是%。编码和解码没有问题。
**四、字符编码**
    字符编码是计算机技术的基石，想要熟练使用计算机，就必须懂得字符编码的知识。不注意的人可能对这个不在意，但这些名词有时候实在让人迷惑，对想学习计算机知识的人来说，搞懂它也十分重要，我也是在学习中慢慢了解了一些这方面的知识。
**一、ASCII码和非ASCII码**
**1. ASCII码**
在计算机内部，所有的信息最终都表示为一个二进制的字符串。每一个二进制位（bit）有0和1两种状态，因此八个二进制位就可以组合出256种状态，这被称为一个字节（byte）。也就是说，一个字节一共可以用来表示256种不同的状态，每一个状态对应一个符号，就是256个符号，从0000000到11111111。
上个世纪60年代，美国制定了一套字符编码，对英语字符与二进制位之间的关系，做了统一规定。这被称为ASCII码，一直沿用至今。
ASCII码一共规定了128个字符的编码，比如空格“SPACE”是32（十进制的32，用二进制表示就是00100000），大写的字母A是65（二进制01000001）。这128个符号（包括32个不能打印出来的控制符号），只占用了一个字节的后面7位，最前面的1位统一规定为0。具体的可以到这个网页上去查下：http://www.nengcha.com/code/ascii/all/
**2、非ASCII编码**
英语用128个符号编码就够了，但是用来表示其他语言，128个符号是不够的。比如，在法语中，字母上方有注音符号，它就无法用ASCII码表示。于是，一些欧洲国家就决定，利用字节中闲置的最高位编入新的符号。比如，法语中的é的编码为130（二进制10000010）。这样一来，这些欧洲国家使用的编码体系，可以表示最多256个符号。
但是，这里又出现了新的问题。不同的国家有不同的字母，因此，哪怕它们都使用256个符号的编码方式，代表的字母却不一样。比如，130在法语编码中代表了é，在希伯来语编码中却代表了字母Gimel (ג)，在俄语编码中又会代表另一个符号。但是不管怎样，所有这些编码方式中，0—127表示的符号是一样的，不一样的只是128—255的这一段。
至于亚洲国家的文字，使用的符号就更多了，汉字就多达10万左右。一个字节只能表示256种符号，肯定是不够的，就必须使用多个字节表达一个符号。比如，简体中文常见的编码方式是GB2312，使用两个字节表示一个汉字，所以理论上最多可以表示256x256=65536个符号。
**一、Unicode和UTF-8**
**1.Unicode**
正如上一节所说，世界上存在着多种编码方式，同一个二进制数字可以被解释成不同的符号。因此，要想打开一个文本文件，就必须知道它的编码方式，否则用错误的编码方式解读，就会出现乱码。为什么电子邮件常常出现乱码？就是因为发信人和收信人使用的编码方式不一样。解释：同一个文本文件，假设内容是用英语写的，在英语编码的情况下，每个字符会和一个二进制数对应（如00101000类似），然后存到计算机中，这时把这个英语文件发给一个俄语国家的用户，计算机传输的是二进制流，即0101之类的数据，到了俄语用户这方，需要有它的俄语编码方式进行解码，把每个二进制流转为字符显示，由于俄语编码表中对每串二进制流数据的解释方式不同，同一个数据如00101000在英语中可能代表A，而在俄语中则代表B，这样就会产生乱码，这是我个人的理解。
GB2312编码、日文编码等也是非unicode编码，是要通过转换表（codepage）转换成unicode编码的，要不怎么显示出来呢？
可以想象，如果有一种编码，将世界上所有的符号都纳入其中。每一个符号都给予一个独一无二的编码，那么乱码问题就会消失。这就是Unicode，就像它的名字都表示的，这是一种所有符号的编码。
Unicode当然是一个很大的集合，现在的规模可以容纳100多万个符号。每个符号的编码都不一样，比如，U+0639表示阿拉伯字母Ain，U+0041表示英语的大写字母A，U+4E25表示汉字“严”。具体的符号对应表，可以查询unicode.org，或者专门的汉字对应表。
**2. Unicode的问题**
需要注意的是，Unicode只是一个符号集，只是一种规范、标准，它只规定了符号的二进制代码，却没有规定这个二进制代码应该如何存储在计算机上。
比如，汉字“严”的unicode是十六进制数4E25，转换成二进制数足足有15位（100111000100101），也就是说这个符号的表示至少需要2个字节。表示其他更大的符号，可能需要3个字节或者4个字节，甚至更多。
这里就有两个严重的问题，第一个问题是，如何才能区别unicode和ascii？计算机怎么知道三个字节表示一个符号，而不是分别表示三个符号呢？第二个问题是，我们已经知道，英文字母只用一个字节表示就够了，如果unicode统一规定，每个符号用三个或四个字节表示，那么每个英文字母前都必然有二到三个字节是0，这对于存储来说是极大的浪费，文本文件的大小会因此大出二三倍，这是无法接受的。
它们造成的结果是：1）出现了unicode的多种存储方式，也就是说有许多种不同的二进制格式，可以用来表示unicode。2）unicode在很长一段时间内无法推广，直到互联网的出现。
**3.UTF-8**
互联网的普及，强烈要求出现一种统一的编码方式。UTF-8就是在互联网上使用最广的一种unicode的实现方式。其他实现方式还包括UTF-16和UTF-32，不过在互联网上基本不用。重复一遍，这里的关系是，UTF-8是Unicode的实现方式之一，它规定了字符如何在计算机中存储、传输等。
UTF-8最大的一个特点，就是它是一种变长的编码方式。它可以使用1~4个字节表示一个符号，根据不同的符号而变化字节长度。
UTF-8的编码规则很简单，只有二条：
1）对于单字节的符号，字节的第一位设为0，后面7位为这个符号的unicode码。因此对于英语字母，UTF-8编码和ASCII码是相同的。
2）对于n字节的符号（n>1），第一个字节的前n位都设为1，第n+1位设为0，后面字节的前两位一律设为10。剩下的没有提及的二进制位，全部为这个符号的unicode码。
下表总结了编码规则，字母x表示可用编码的位。
|Unicode符号范围|UTF-8编码方式 (十六进制)|（二进制）|
|----|----|----|
|0000 0000-0000|007F|0xxxxxxx|
|0000 0080-0000|07FF|110xxxxx 10xxxxxx|
|0000 0800-0000|FFFF|1110xxxx 10xxxxxx 10xxxxxx|
|0001 0000-0010|FFFF|11110xxx 10xxxxxx 10xxxxxx 10xxxxxx|
 下面，还是以汉字“严”为例，演示如何实现UTF-8编码。
已知“严”的unicode是4E25（100111000100101），根据上表，可以发现4E25处在第三行的范围内（0000 0800-0000 FFFF），因此“严”的UTF-8编码需要三个字节，即格式是“1110xxxx 10xxxxxx 10xxxxxx”。然后，从“严”的最后一个二进制位开始，依次从后向前填入格式中的x，多出的位补0。这样就得到了，“严”的UTF-8编码是“11100100 10111000
 10100101”，这是保存在计算机中的实际数据，转换成十六进制就是E4B8A5，转成十六进制的目的为了便于阅读。
**4. Unicode与UTF-8之间的转换**
通过上一节的例子，可以看到“严”的Unicode码是4E25，UTF-8编码是E4B8A5，两者是不一样的。它们之间的转换可以通过程序实现。
在Windows平台下，有一个最简单的转化方法，就是使用内置的记事本小程序Notepad.exe。打开文件后，点击“文件”菜单中的“另存为”命令，会跳出一个对话框，在最底部有一个“编码”的下拉条。
里面有四个选项：ANSI，Unicode，Unicode big endian 和 UTF-8。
1）ANSI是默认的编码方式。对于英文文件是ASCII编码，对于简体中文文件是GB2312编码（只针对Windows简体中文版，如果是繁体中文版会采用Big5码）。
2）Unicode编码指的是UCS-2编码方式，即直接用两个字节存入字符的Unicode码。这个选项用的little endian格式。
3）Unicode big endian编码与上一个选项相对应。我在下一节会解释little endian和big endian的涵义。
4）UTF-8编码，也就是上一节谈到的编码方法。
选择完”编码方式“后，点击”保存“按钮，文件的编码方式就立刻转换好了。
**5. Little endian和Big endian**
上一节已经提到，Unicode码可以采用UCS-2格式直接存储。以汉字”严“为例，Unicode码是4E25，需要用两个字节存储，一个字节是4E，另一个字节是25。存储的时候，4E在前，25在后，就是Big endian方式；25在前，4E在后，就是Little endian方式。
那么很自然的，就会出现一个问题：计算机怎么知道某一个文件到底采用哪一种方式编码？
Unicode规范中定义，每一个文件的最前面分别加入一个表示编码顺序的字符，这个字符的名字叫做”零宽度非换行空格“（ZERO WIDTH NO-BREAK SPACE），用FEFF表示。这正好是两个字节，而且FF比FE大1。
如果一个文本文件的头两个字节是FE FF，就表示该文件采用大头方式；如果头两个字节是FF FE，就表示该文件采用小头方式。
**6. 实例**
下面，举一个实例。
打开”记事本“程序Notepad.exe，新建一个文本文件，内容就是一个”严“字，依次采用ANSI，Unicode，Unicode big endian 和 UTF-8编码方式保存。
然后，用文本编辑软件UltraEdit中的”十六进制功能“，观察该文件的内部编码方式。
1）ANSI：文件的编码就是两个字节“D1 CF”，这正是“严”的GB2312编码，这也暗示GB2312是采用大头方式存储的。
2）Unicode：编码是四个字节“FF FE 25 4E”，其中“FF FE”表明是小头方式存储，真正的编码是4E25。
3）Unicode big endian：编码是四个字节“FE FF 4E 25”，其中“FE FF”表明是大头方式存储。
4）UTF-8：编码是六个字节“EF BB BF E4 B8 A5”，前三个字节“EF BB BF”表示这是UTF-8编码，后三个“E4B8A5”就是“严”的具体编码，它的存储顺序与编码顺序是一致的。
**7.解决的问题：**
一、如何在中文系统中运行非Unicode编码程序？
有很多意大利文版（除英文版）学习软件、百科全书等软件在中文系统上会出现乱码，解决方法：
WindowsXP内核是Unicode编码，支持多语种，对于Unicode编码的应用程序会正常显示原文（因为windows核心是用unicode代码写的，所以不存在问题），但是，很多程序不是用Unicode编码写的，这时WindowsXP系统可以指定以特定的编码运行非Unicode编码程序，中文版WindowsXP默认的是“简体中文GB2312”。你只需在控制面板--〉区域和语言选项--〉高级--〉为非Unicode程序的语言选择“意大利语”，即可正确运行意大利文版的游戏程序。分析：我理解的流程是这样：程序------>意大利语编码（转换表codepage）------>解释成unicode识别的编码（通过指定的转换表将非
 Unicode 的字符编码转换为同一字符对应的系统内部使用的 Unicode 编码）------>被系统翻译成意大利文（因为每个unicode编码对应了相应的意大利文字），便可以正常显示了。
**三、ISO-8859-1**
ISO-8859-1编码是单字节编码，向下兼容ASCII，其编码范围是0x00-0xFF，0x00-0x7F之间完全和ASCII一致，0x80-0x9F之间是控制字符，0xA0-0xFF之间是文字符号。
ISO-8859-1收录的字符除ASCII收录的字符外，还包括西欧语言、希腊语、泰语、阿拉伯语、希伯来语对应的文字符号。欧元符号出现的比较晚，没有被收录在ISO-8859-1当中。
因为ISO-8859-1编码范围使用了单字节内的所有空间，在支持ISO-8859-1的系统中传输和存储其他任何编码的字节流都不会被抛弃。换言之，把其他任何编码的字节流当作ISO-8859-1编码看待都没有问题。这是个很重要的特性，MySQL数据库默认编码是Latin1就是利用了这个特性。ASCII编码是一个7位的容器，ISO-8859-1编码是一个8位的容器。
Latin1是ISO-8859-1的别名，有些环境下写作Latin-1。
**三、GB2312、GB13000、GB18030、GBK和BIG**
**1.GB2312**
GB2312码是中华人民共和国国家汉字信息交换用编码，全称《信息交换用汉字编码字符集——基本集》，由国家标准总局发布，1981年5月1日实施，通行于大陆。新加坡等地也使用此编码。基本集共收入汉字6763个和非汉字图形字符682个，通行于中国大陆。新加坡等地也使用此编码。
**2.GBK**
GBK即汉字内码扩展规范，K为扩展的汉语拼音中“扩”字的声母。英文全称Chinese Internal Code Specification。GBK编码标准兼容GB2312，共收录汉字21003个、符号883个，并提供1894个造字码位，简、繁体字融于一库。
**3.GB13000**
GB13000 全称：国家标准GB13000.1:1993《信息技术 通用多八位编码字符集（UCS）第一部分：体系结构与基本多文种平面》，此标准等同采用国际标准ISO/IEC 10646.1:1993《信息技术 通用多八位编码字符集（UCS）第一部分：体系结构与基本多文种平面》。
**4.GB18030**
国家标准GB18030-2000《信息交换用汉字编码字符集基本集的扩充》是我国继GB2312-1980和GB13000-1993之后最重要的汉字编码标准，是我国计算机系统必须遵循的基础性标准之一。 目前，GB18030有两个版本：GB18030-2000和GB18030-2005。GB18030-2000是GBK的取代版本，它的主要特点是在GBK基础上增加了CJK统一汉字扩充A的汉字。GB18030-2005的主要特点是在GB18030-2000基础上增加了CJK统一汉字扩充B的汉字。
**5.BIG**
BIG-5码是通行于台湾、香港地区的一个繁体字编码方案，俗称“大五码”。地区标准号为：CNS11643，这就是人们讲的BIG-5码。
二、消除网页乱码？
网页乱码是浏览器对HTML网页解释时形成的，如果网页制作时编码为繁体big5，浏览器却以编码gb2312显示该网页，就会出现乱码，因此只要你在浏览器中也以繁体big5显示该网页，就会消除乱码。打个比方有些像字典，繁体字得用繁体字典来查看，简体字得用简体字典来查看，不然你看不懂。
【解决办法】：在浏览器中选择“编码”菜单，事先为浏览器安装多语言支持包（例如在安装IE时要安装多语言支持包），这样当浏览网页出现乱码时，即可手工更改查看此网页的编码方式，在浏览器中选择菜单栏下的“查看”/“编码”/“自动选择”/简体中文（GB2312），如为繁体中文则选择“查看”/“编码”/“自动选择”/繁体中文（BIG5），其他语言依此类推，便可消除网页乱码现象。 分析：因为繁体big5编码后的文件，每个文字对应一个二进制流（假设是1212对应繁这个字），当我们以编码gb2312显示该网页时，gb2312编码会到表里去找1212（二进制流不会变的）对应谁，肯定不再是繁这个字了，当然显示的就不再是那个繁字了，也就会出现乱码了。这样理解简单些，其实中间还要转换成同一字符对应的系统内部使用的
 Unicode 编码，然后通过系统底层unicode编码还原成相应字符显示出来。
