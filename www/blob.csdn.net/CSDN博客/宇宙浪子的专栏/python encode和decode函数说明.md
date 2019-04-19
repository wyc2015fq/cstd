# python encode和decode函数说明 - 宇宙浪子的专栏 - CSDN博客
2015年01月21日 15:34:23[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：343标签：[python																[encode																[decode](https://so.csdn.net/so/search/s.do?q=decode&t=blog)](https://so.csdn.net/so/search/s.do?q=encode&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python](https://blog.csdn.net/bluehawksky/article/category/2542577)
字符串编码常用类型：utf-8,gb2312,cp936,gbk等。
python中，我们使用decode()和encode()来进行解码和编码
在python中，使用unicode类型作为编码的基础类型。即
     decode              encode
str ---------> unicode --------->str
```
u = u'中文' #显示指定unicode类型对象u
str = u.encode('gb2312') #以gb2312编码对unicode对像进行编码
str1 = u.encode('gbk') #以gbk编码对unicode对像进行编码
str2 = u.encode('utf-8') #以utf-8编码对unicode对像进行编码
u1 = str.decode('gb2312')#以gb2312编码对字符串str进行解码，以获取unicode
u2 = str.decode('utf-8')#如果以utf-8的编码对str进行解码得到的结果，将无法还原原来的unicode类型
```
如上面代码，str\str1\str2均为字符串类型（str）,给字符串操作带来较大的复杂性。
好消息来了，对，那就是python3，在新版本的python3中，取消了unicode类型，代替它的是使用unicode字符的字符串类型(str),字符串类型（str）成为基础类型如下所示，而编码后的变为了字节类型(bytes)但是两个函数的使用方法不变：
     decode              encode
bytes ------> str(unicode)------>bytes
```
u = '中文' #指定字符串类型对象u
str = u.encode('gb2312') #以gb2312编码对u进行编码，获得bytes类型对象str
u1 = str.decode('gb2312')#以gb2312编码对字符串str进行解码，获得字符串类型对象u1
u2 = str.decode('utf-8')#如果以utf-8的编码对str进行解码得到的结果，将无法还原原来的字符串内容
```
避免不了的是，文件读取问题：
假如我们读取一个文件，文件保存时，使用的编码格式，决定了我们从文件读取的内容的编码格式，例如，我们从记事本新建一个文本文件test.txt, 编辑内容，保存的时候注意，编码格式是可以选择的，例如我们可以选择gb2312,那么使用python读取文件内容，方式如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
f = open('test.txt','r')
s = f.read() #读取文件内容,如果是不识别的encoding格式（识别的encoding类型跟使用的系统有关），这里将读取失败
'''假设文件保存时以gb2312编码保存'''
u = s.decode('gb2312') #以文件保存格式对内容进行解码，获得unicode字符串
'''下面我们就可以对内容进行各种编码的转换了'''
str = u.encode('utf-8')#转换为utf-8编码的字符串str
str1 = u.encode('gbk')#转换为gbk编码的字符串str1
str1 = u.encode('utf-16')#转换为utf-16编码的字符串str1
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
python给我们提供了一个包codecs进行文件的读取，这个包中的open()函数可以指定编码的类型：
```
import codecs
f = codecs.open('text.text','r+',encoding='utf-8')#必须事先知道文件的编码格式，这里文件编码是使用的utf-8
content = f.read()#如果open时使用的encoding和文件本身的encoding不一致的话，那么这里将将会产生错误
f.write('你想要写入的信息')
f.close()
```
知识点：编码格式，下面放一下，文件编码的格式介绍（转来的）：
# [文件编码格式](http://www.cnblogs.com/zhoug2020/archive/2012/01/13/2321259.html)
从文件编码的方式来看，文件可分为ASCII码文件和二进制码文件两种。
ASCII文件也称为文本文件，这种文件在磁盘中存放时每个字符对应一个字节，用于存放对应的ASCII码。例如，数5678的存储形式为：
ASC码： 　00110101 00110110 00110111 00111000
　　　　　↓ 　　　　↓　　　　↓ 　　　↓
十进制码： 5　　　　　6　　　　7　　　　8 共占用4个字节。ASCII码文件可在屏幕上按字符显示， 例如源程序文件就是ASCII文件，用DOS命令TYPE可显示文件的内容。 由于是按字符显示，因此能读懂文件内容。
二进制文件是按二进制的编码方式来存放文件的。 例如， 数5678的存储形式为： 00010110 00101110只占二个字节。二进制文件虽然也可在屏幕上显示，但其内容无法读懂。C系统在处理这些文件时，并不区分类型，都看成是字符流，按字节进行 处理。输入输出字符流的开始和结束只由程序控制而不受物理符号(如回车符)的控制。 因此也把这种文件称作“流式文件”。
|UCS-2编码(16进制)|UTF-8 字节流(二进制)|
|----|----|
|0000 - 007F|0xxxxxxx|
|0080 - 07FF|110xxxxx 10xxxxxx|
|0800 - FFFF|1110xxxx 10xxxxxx 10xxxxxx|
- 问题一：
使用Windows记事本的“另存为”，可以在GBK、Unicode、Unicode big endian和UTF-8这几种编码方式间相互转换。同样是txt文件，Windows是怎样识别编码方式的呢？
我很早前就发现Unicode、Unicode big endian和UTF-8编码的txt文件的开头会多出几个字节，分别是FF、FE（Unicode）,FE、FF（Unicode big endian）,EF、BB、BF（UTF-8）。但这些标记是基于什么标准呢？
- 问题二：
最近在网上看到一个ConvertUTF.c，实现了UTF- 32、UTF-16和UTF-8这三种编码方式的相互转换。对于Unicode(UCS2)、 GBK、UTF-8这些编码方式，我原来就了解。但这个程序让我有些糊涂，想不起来UTF-16和UCS2有什么关系。
查了查相关资料，总算将这些问题弄清楚了，顺带也了解了一些Unicode的细节。写成一篇文章，送给有过类似疑问的朋友。本文在写作时尽量做到通俗易懂，但要求读者知道什么是字节，什么是十六进制。
### 0、big endian和little endian
big endian和little endian是CPU处理多字节数的不同方式。例如“汉”字的Unicode编码是6C49。那么写到文件里时，究竟是将6C写在前面，还是将49写在前 面？如果将6C写在前面，就是big endian。还是将49写在前面，就是little endian。
“endian”这个词出自《格列佛游记》。小人国的内战就源于吃鸡蛋时是究竟从大头(Big-Endian)敲开还是从小头(Little-Endian)敲开，由此曾发生过六次叛乱，其中一个皇帝送了命，另一个丢了王位。
我们一般将endian翻译成“字节序”，将big endian和little endian称作“大尾”和“小尾”。
### 1、字符编码、内码，顺带介绍汉字编码
字符必须编码后才能被计算机处理。计算机使用的缺省编码方式就是计算机的内码。早期的计算机使用7位的ASCII编码，为了处理汉字，程序员设计了用于简体中文的GB2312和用于繁体中文的big5。
GB2312(1980年)一共收录了7445个字符，包括6763个汉字和682个其它符号。汉字区的内码范围高字节从B0-F7，低字节从A1-FE，占用的码位是72*94=6768。其中有5个空位是D7FA-D7FE。
GB2312 支持的汉字太少。1995年的汉字扩展规范GBK1.0收录了21886个符号，它分为汉字区和图形符号区。汉字区包括21003个字符。2000年的 GB18030是取代GBK1.0的正式国家标准。该标准收录了27484个汉字，同时还收录了藏文、蒙文、维吾尔文等主要的少数民族文字。现在的PC平 台必须支持GB18030，对嵌入式产品暂不作要求。所以手机、MP3一般只支持GB2312。
从ASCII、 GB2312、GBK到GB18030，这些编码方法是向下兼容的，即同一个字符在这些方案中总是有相同的编码，后面的标准支持更多的字符。在这些编码 中，英文和中文可以统一地处理。区分中文编码的方法是高字节的最高位不为0。按照程序员的称呼，GB2312、GBK到GB18030都属于双字节字符集 (DBCS)。
有的中文Windows的缺省内码还是GBK，可以通过GB18030升级包升级到GB18030。不过GB18030相对GBK增加的字符，普通人是很难用到的，通常我们还是用GBK指代中文Windows内码。
这里还有一些细节：
- 
GB2312的原文还是区位码，从区位码到内码，需要在高字节和低字节上分别加上A0。
- 
在DBCS中，GB内码的存储格式始终是big endian，即高位在前。
- 
GB2312 的两个字节的最高位都是1。但符合这个条件的码位只有128*128=16384个。所以GBK和GB18030的低字节最高位都可能不是1。不过这不影 响DBCS字符流的解析：在读取DBCS字符流时，只要遇到高位为1的字节，就可以将下两个字节作为一个双字节编码，而不用管低字节的高位是什么。
### 2、Unicode、UCS和UTF
前面提到从ASCII、GB2312、GBK到GB18030的编码方法是向下兼容的。而Unicode只与ASCII兼容（更准确地说，是与ISO-8859-1兼容），与GB码不兼容。例如“汉”字的Unicode编码是6C49，而GB码是BABA。
Unicode 也是一种字符编码方法，不过它是由国际组织设计，可以容纳全世界所有语言文字的编码方案。Unicode的学名是"Universal Multiple-Octet Coded Character Set"，简称为UCS。UCS可以看作是"Unicode Character Set"的缩写。
根据维基百科全书(http: //zh.wikipedia.org/wiki/)的记载：历史上存在两个试图独立设计Unicode的组织，即国际标准化组织（ISO）和一个软件制 造商的协会（unicode.org）。ISO开发了ISO 10646项目，Unicode协会开发了Unicode项目。
在1991年前后，双方都认识到世界不需要两个不兼容的字符集。于是它们开始合并双方的工作成果，并为创立一个单一编码表而协同工作。从Unicode2.0开始，Unicode项目采用了与ISO 10646-1相同的字库和字码。
目前两个项目仍都存在，并独立地公布各自的标准。Unicode协会现在的最新版本是2005年的Unicode 4.1.0。ISO的最新标准是10646-3:2003。
UCS规定了怎么用多个字节表示各种文字。怎样传输这些编码，是由UTF(UCS Transformation Format)规范规定的，常见的UTF规范包括UTF-8、UTF-7、UTF-16。
IETF 的RFC2781和RFC3629以RFC的一贯风格，清晰、明快又不失严谨地描述了UTF-16和UTF-8的编码方法。我总是记不得IETF是 Internet Engineering Task Force的缩写。但IETF负责维护的RFC是Internet上一切规范的基础。
### 3、UCS-2、UCS-4、BMP
UCS有两种格式：UCS-2和UCS-4。顾名思义，UCS-2就是用两个字节编码，UCS-4就是用4个字节（实际上只用了31位，最高位必须为0）编码。下面让我们做一些简单的数学游戏：
UCS-2有2^16=65536个码位，UCS-4有2^31=2147483648个码位。
UCS -4根据最高位为0的最高字节分成2^7=128个group。每个group再根据次高字节分为256个plane。每个plane根据第3个字节分为 256行 (rows)，每行包含256个cells。当然同一行的cells只是最后一个字节不同，其余都相同。
group 0的plane 0被称作Basic Multilingual Plane, 即BMP。或者说UCS-4中，高两个字节为0的码位被称作BMP。
将UCS-4的BMP去掉前面的两个零字节就得到了UCS-2。在UCS-2的两个字节前加上两个零字节，就得到了UCS-4的BMP。而目前的UCS-4规范中还没有任何字符被分配在BMP之外。
### 4、UTF编码
UTF-8就是以8位为单元对UCS进行编码。从UCS-2到UTF-8的编码方式如下：
例如“汉”字的Unicode编码是6C49。6C49在0800-FFFF之间，所以肯定要用3字节模板了：1110xxxx 10xxxxxx 10xxxxxx。将6C49写成二进制是：0110
 110001 001001， 用这个比特流依次代替模板中的x，得到：11100110 10110001 10001001，即E6
 B1 89。
读者可以用记事本测试一下我们的编码是否正确。
UTF -16以16位为单元对UCS进行编码。对于小于0x10000的UCS码，UTF-16编码就等于UCS码对应的16位无符号整数。对于不小于 0x10000的UCS码，定义了一个算法。不过由于实际使用的UCS2，或者UCS4的BMP必然小于0x10000，所以就目前而言，可以认为UTF -16和UCS-2基本相同。但UCS-2只是一个编码方案，UTF-16却要用于实际的传输，所以就不得不考虑字节序的问题。
### 5、UTF的字节序和BOM
UTF -8以字节为编码单元，没有字节序的问题。UTF-16以两个字节为编码单元，在解释一个UTF-16文本前，首先要弄清楚每个编码单元的字节序。例如收 到一个“奎”的Unicode编码是594E，“乙”的Unicode编码是4E59。如果我们收到UTF-16字节流“594E”，那么这是“奎”还是 “乙”？
Unicode规范中推荐的标记字节顺序的方法是BOM。BOM不是“Bill Of Material”的BOM表，而是Byte Order Mark。BOM是一个有点小聪明的想法：
在UCS 编码中有一个叫做"ZERO WIDTH NO-BREAK SPACE"的字符，它的编码是FEFF。而FFFE在UCS中是不存在的字符，所以不应该出现在实际传输中。UCS规范建议我们在传输字节流前，先传输 字符"ZERO WIDTH NO-BREAK SPACE"。
这样如果接收者收到FEFF，就表明这个字节流是Big-Endian的；如果收到FFFE，就表明这个字节流是Little-Endian的。因此字符"ZERO WIDTH NO-BREAK SPACE"又被称作BOM。
UTF -8不需要BOM来表明字节顺序，但可以用BOM来表明编码方式。字符"ZERO WIDTH NO-BREAK SPACE"的UTF-8编码是EF BB BF（读者可以用我们前面介绍的编码方法验证一下）。所以如果接收者收到以EF BB BF开头的字节流，就知道这是UTF-8编码了。
Windows就是使用BOM来标记文本文件的编码方式的。
### 6、进一步的参考资料
本文主要参考的资料是 "Short overview of ISO-IEC 10646 and Unicode" (http://www.nada.kth.se/i18n/ucs/unicode-iso10646-oview.html)。
我还找了两篇看上去不错的资料，不过因为我开始的疑问都找到了答案，所以就没有看：
- "Understanding Unicode A general introduction to the Unicode Standard" (http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&item_id=IWS-Chapter04a)
- "Character set encoding basics Understanding character set encodings and legacy encodings" (http://scripts.sil.org/cms/scripts/page.php?site_id=nrsi&item_id=IWS-Chapter03)
