# response.setHeader()设置下载中文文件名乱码问题 - 零度的博客专栏 - CSDN博客
2016年07月04日 15:58:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：17106
个人分类：[编码/解码																[Http/HttpURLConnection](https://blog.csdn.net/zmx729618/article/category/6191435)](https://blog.csdn.net/zmx729618/article/category/6225091)
1. HTTP消息头
（1）通用信息头
即能用于请求消息中,也能用于响应信息中,但与被传输的实体内容没有关系的信息头,如Data,Pragma
主要: Cache-Control , Connection , Data , Pragma , Trailer , Transfer-Encoding , Upgrade
（2）请求头
用于在请求消息中向服务器传递附加信息,主要包括客户机可以接受的数据类型,压缩方法,语言,以及客户计算机上保留的信息和发出该请求的超链接源地址等.
主要: Accept , Accept-Encoding , Accept-Language , Host ,
（3）响应头
用于在响应消息中向客户端传递附加信息,包括服务程序的名称,要求客户端进行认证的方式,请求的资源已移动到新地址等.
主要: Location , Server , WWW-Authenticate(认证头)
（4）实体头
用做实体内容的元信息,描述了实体内容的属性,包括实体信息的类型,长度,压缩方法,最后一次修改的时间和数据的有效期等.
主要: Content-Encoding , Content-Language , Content-Length , Content-Location , Content-Type
（4）扩展头
主要：Refresh, Content-Disposition
2. 几个主要头的作用
（1）Content-Type的作用
该实体头的作用是让服务器告诉浏览器它发送的数据属于什么文件类型。
例如：当Content-Type 的值设置为text/html和text/plain时,前者会让浏览器把接收到的实体内容以HTML格式解析,后者会让浏览器以普通文本解析.
（2）Content-Disposition 的作用
当Content-Type 的类型为要下载的类型时 , 这个信息头会告诉浏览器这个文件的名字和类型。
在讲解这个内容时,张老师同时讲出了解决中文文件名乱码的解决方法,平常想的是使用getBytes() , 实际上应使用email的附件名编码方法对文件名进行编码,但IE不支持这种作法(其它浏览器支持) , 使用javax.mail.internet.*包的MimeUtility.encodeWord("中文.txt")的方法进行编码。
Content-Disposition扩展头的例子：
<%@ page pageEncoding="GBK" contentType="text/html;charset=utf-8" import="java.util.*,java.text.*" %>
<%=DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.SHORT, Locale.CHINA).format(new Date())
%>
<%
               response.setHeader("Content-Type","video/x-msvideo");
               response.setHeader("Content-Disposition", "attachment;filename=aaa.doc");
%>
Content-Disposition中指定的类型是文件的扩展名，并且弹出的下载对话框中的文件类型图片是按照文件的扩展名显示的，点保存后，文件以filename的值命名，保存类型以Content中设置的为准。
注意：在设置Content-Disposition头字段之前，一定要设置Content-Type头字段。
（3）Authorization头的作用
Authorization的作用是当客户端访问受口令保护时，服务器端会发送401状态码和WWW-Authenticate响应头，要求客户机使用Authorization来应答。
例如：
<%@ page pageEncoding="GBK" contentType="text/html;charset=utf-8" import="java.util.*,java.text.*" %>
<%=DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.SHORT, Locale.CHINA).format(new Date())
%>
<%
response.setStatus(401);
response.setHeader("WWW-Authenticate", "Basic realm=/"Tomcat Manager Application/"");
%>
3．如何实现文件下载
要实现文件下载，我们只需要设置两个特殊的相应头，它们是什么头？如果文件名带中文，该如何解决？
两个特殊的相应头：
----Content-Type:       application/octet-stream
----Content-Disposition: attachment;filename=aaa.zip
例如：
response.setContentType("image/jpeg");response.setHeader("Content- Disposition","attachment;filename=Bluehills.jpg");
如果文件中filename参数中有中文，则就会出现乱码。
解决办法：
（1）MimeUtility.encodeWord("中文.txt");//现在版本的IE还不行
（2）new String("中文".getBytes("GB2312"),"ISO8859- 1");//实际上这个是错误的
4. 测试并分析文件名乱码问题
response.setHeader()下载中文文件名乱码问题
response.setHeader("Content-Disposition", "attachment; filename=" + java.net.URLEncoder.encode(fileName, "UTF-8"));
下载的程序里有了上面一句，一般在IE6的下载提示框上将正确显示文件的名字，无论是简体中文，还是日文。不过当时确实没有仔细测试文件名为很长的中文文件名的情况。现如今经过仔细测试，发现文字只要超过17个字，就不能下载了。分析如下：
一. 通过原来的方式，也就是先用URLEncoder编码，当中文文字超过17个时，IE6 无法下载文件。这是IE的bug，参见微软的知识库文章 KB816868 。原因可能是IE在处理 Response Header 的时候，对header的长度限制在150字节左右。而一个汉字编码成UTF-8是9个字节，那么17个字便是153个字节，所以会报错。而且不跟后缀也不对.
二. 解决方案：将文件名编码成ISO8859-1是有效的解决方案，代码如下：
response.setHeader( "Content-Disposition", "attachment;filename=" + new String( fileName.getBytes("gb2312"), "ISO8859-1" ) );
在确保附件文件名都是简体中文字的情况下，那么这个办法确实是最有效的，不用让客户逐个的升级IE。如果台湾同胞用，把gb2312改成big5就行。但现在的系统通常都加入了 国际化的支持，普遍使用UTF-8。如果文件名中又有简体中文字，又有繁体中文，还有日文。那么乱码便产生了。另外，在上Firefox (v1.0-en)下载也是乱码。
三. 参看邮件中的中文附件名的形式，用outlook新建一个带有中文附件的邮件，然后看这个邮件的源代码，找到：
Content-Disposition: attachment;
filename="=?gb2312?B?0MK9qCDOxLG+zsS1tS50eHQ=?="
用这个filename原理上就可以显示中文名附件，但是现在IE并不支持，Firefox是支持的。尝试使用 javamail 的MimeUtility.encode()方法来编码文件名，也就是编码成 =?gb2312?B?xxxxxxxx?= 这样的形式，并从 RFC1522 中找到对应的标准支持。
折中考虑，结合了一、二的方式，代码片断如下：
String fileName = URLEncoder.encode(atta.getFileName(), "UTF-8");
/*
* see http://support.microsoft.com/default.aspx?kbid=816868
*/
if (fileName.length() > 150) {
String guessCharset = xxxx
//根据request的locale 得出可能的编码，中文操作系统通常是gb2312
fileName = new String(atta.getFileName().getBytes(guessCharset), "ISO8859-1");
}
response.setHeader("Content-Disposition", "attachment; filename=" + fileName);
编码转换的原理：
        首先在源程序中将编码设置成GB2312字符编码,然后将源程序按Unicode编码转换成字节码加载到内存中（java加载到内存中的字节码都是Unicode编码），然后按GB2312编码获得中文字符串的字节数组，然后生成按ISO8859-1编码形式的Unicode字符串（这时的4个字节就变成了8个字节，高位字节补零）,当在网络中传输时，因为setHeader方法中的字符只能按ISO8859-1传输，所以这时候就又把Unicode字符转换成了ISO8859-1的编码传到浏览器（就是把刚才高位补的零全去掉），这时浏览器接收到的ISO8859-1码的字符因为符合GB2312编码，所以就可以显示中文了。
5. jsp翻译成class时的编码问题
记事本中代码块1：
<%=
       "a中文".length()
%>
代码块2：
<%@ page pageEncoding="gbk"%>
<%=
       "a中文".length()
%>
为什么上面的输出值为5，改成下面的则输出3？
因为上面的代码没有添加该文件的编码说明 , WEB应用程序在将jsp翻译成class文件时 , 把该字符串的内容按默认的保存方式指定的编码ASCII码来算的，在UTF-8中，原ASCII字符占一个字节，汉字占两个字节，对应两个字符，长度就变成了5 , 而下面的是GBK编码, 一个汉字和一个英文都对应一个字符,得到结果就为3.
]
response.setHeader(...)文件名中有空格的时候
String fileName = StringUtils.trim(file.getName());
String formatFileName = encodingFileName(name);//在后面定义方法encodingFileName(String fileName);
response.setHeader("Content-Disposition", "attachment; filename=" + formatFileName );
//处理文件名中出现的空格   
//其中%20是空格在UTF-8下的编码
public static String encodingFileName(String fileName) {
        String returnFileName = "";
        try {
            returnFileName = URLEncoder.encode(fileName, "UTF-8");
            returnFileName = StringUtils.replace(returnFileName, "+", "%20");
            if (returnFileName.length() > 150) {
                returnFileName = new String(fileName.getBytes("GB2312"), "ISO8859-1");
                returnFileName = StringUtils.replace(returnFileName, " ", "%20");
            }
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            if (log.isWarnEnabled()) {
                log.info("Don't support this encoding ...");
            }
        }
        return returnFileName;
    }
常见编码总结：
## 1. ISO 8859-1
     最早的编码就是ISO 8859-1，属于单字节编码，应用于英文系列。最多能表示的字符范围是0-255（编码范围是0x00-0xFF），其中0x00-0x7F之间完全和ASCII一致（ASCII是7位编码，能个表示128个字符），因此向下兼容ASCII。除ASCII收录的字符外，ISO-8859-1收录的字符还包括西欧语言、希腊语、泰语、阿拉伯语、希伯来语对应的文字符号。欧元符号等出现的比较晚，没有被收录在ISO 8859-1当中。
很明显，ISO 8859-1编码表示的字符范围很窄，例如无法表示中文字符。但是由于ISO-8859-1编码范围使用了单字节内的所有空间，在支持ISO 8859-1的系统中传输和存储其他任何编码的字节流都不会被抛弃。换言之，把其他任何编码的字节流当作ISO-8859-1编码看待都没有问题。这是个很重要的特性，所以很多情况下（如很多协议传输数据时）都使用ISO 8859-1编码。我们可以这么说，ASCII编码是一个7位的容器，ISO 8859-1编码是一个8位的容器。
比如，虽然“中文”两个字符就不存在ISO 8859-1编码，但可以用iso8859-1编码来“表示”。通过查询下文将要介绍的GB2312编码表，“中文”应该是"d6d0 cec4"两个字符，使用ISO 8859-1编码来“表示”的时候则将它拆开为4个字节来表示，即"d6 d0  ce c4"（事实上，在进行存储的时候，也是以字节为单位处理的）。如果使用Unicode编码，则表示为"4e2d 6587"；使用UTF编码，则是6个字节"e4 b8 ad
  e6 96 87"。很明显，这种使用ISO 8869-1对汉字进行表示的方法还需要以另一种编码为基础。
有些环境下，将ISO 8859-1写作Latin-1。
## 2. GB2312/BIG5/GBK
      GB2312是汉子的国标码，由中华人民共和国政府制定的，简体汉字编码规范。其表示汉字时是双字节编码，而英文字母和ISO 8859-1一致（兼容ISO 8859-1单字节编码，因此是**变长**编码系统）。与此对应的还有BIG5，是中华民国政府制定的，繁体汉字的编码规范，一般用于海外计算机的繁体中文显示。所谓的繁体中文Windows，简体中文Windows，指的就是采用BIG5和GB2312编码格式的操作系统。这两种编码方式不兼容，如果使用一种编码的文本阅读器来读另一种编码的文本，就会出现乱码。比如在简体中文Windows上读BIG5编码的文件，就是乱码，反之亦然。使用简体浏览器浏览的时候，到了繁体中文网站，如果不改变码制，也是乱码。
GBK，又称GBK大字符集，简而言之就是将所有亚洲文字的双字节字符，包括简体中文，繁体中文，日语，韩语等，都使用一种格式编码，这样就能够做到在所有的语言平台上面兼容。而且，目前GB2312，BIG5所包含的汉字数量也不足，比如朱总理的名字中间那个字一般就打不出。而GBK大字符集包含的汉字数量比GB2312和BIG5多的多了，足够使用。简而言之，GBK编码能够用来同时表示繁体字和简体字，而GB2312只能表示简体字，因此GBK又是兼容GB2312编码的。
## 3. Unicode
Unicode是Unicode.org制定的编码标准，目前得到了绝大部分操作系统和编程语言的支持。Unicode.org官方对Unicode的定义是：Unicode provides a unique number for every character。可见，Unicode所做的是为每个字符定义了一个相应的数字表示。比如，"a"的Unicode值是0x0061,“一”的Unicde值是0x4E00，这是最简单的情况，每个字符用2个字节表示。
Unicode是最统一的编码，可以用来表示所有语言的字符，而且是**定长**双字节（如果考虑辅助平面，也有四字节的）编码，包括英文字母在内，都以双字节表示，所以它是不兼容ISO 8859-1编码的。不过，相对于ISO 8859-1中所编码的字符来说，Unicode编码只是在前面增加了一个全0字节，例如字母a的Unicode编码为"00 61"。和GB2312/GBK等非定长编码相比，定长编码便于计算机处理，而Unicode又可以用来表示所有字符，所以在很多软件内部是使用Unicode编码来处理的，比如java。
Unicode的编码空间从U+0000到U+10FFFF，共有1,112,064个码位（code point）可用来映射字符. Unicode的编码空间可以划分为17个平面（plane），每个平面包含216（65,536）个码位。17个平面的码位可表示为从U+xx0000到U+xxFFFF， 其中xx表示十六进制值从00(16) 到10(16)，共计17个平面。第一个平面称为**基本多语言平面**（Basic
 Multilingual Plane, **BMP**），或称第零平面（Plane 0），码位从U+0000至U+FFFF，包含了最常用的字符。其他平面称为**辅助平面**(Supplementary Planes)。
对于在Unicode基本多文种平面定义的字符（无论是拉丁字母、汉字或其他文字或符号），一律使用2字节储存，但是从U+D800到U+DFFF之间的码位区段是永久保留不映射到任何Unicode字符的。而在辅助平面定义的字符，即从U+10000到U+10FFFF的码位，则（UTF-16的做法是）以代理对（surrogate pair）的形式，将其拆分成两个2字节（位于0xD800-0xDFFF区段）共4字节的值来储存。进行代理对映射的方法本文就不深入讨论了，有兴趣的可以自行搜索。
## 4. UTF
考虑到Unicode编码不兼容ISO 8859-1编码，而且容易占用更多的空间：因为对于英文字母，Unicode也需要两个字节来表示，所以Unicode不便于传输和存储。因此而产生了UTF编码。
UTF 是 Unicode Translation Format，即把Unicode转做某种格式的意思。事实上可以这么认为，**Unicode是一种编码方式，和ACSII是同一个概念，而UTF是一种存储方式（格式）**。
那么，UTF是如何做这种格式转换的呢？
**UTF-32**
Unicode.org定义了百万个以上的字符，如果将所有的字符用统一的格式表示，需要的是4个字节。"a"的Unicode表示就会变成0x00000061，而“一”的Unicode值是0x00004E00。实际上，这就是UTF-32，也是Linux操作系统上所使用的Unicode方案，也是一种定长编码。其缺点很显然是造成了空间的巨大浪费，从而非常没有效率，因此没有UTF-8和UTF-16使用的频繁。
**UTF-16**
但是，上文已经提到，Unicode基本多语言平面的字符只使用2个字节就可以表示了，真正需要扩展到4个字节来表示的字符少之又少。所以使用2个字节来表示Unicode代码是一种很自然的选择，例如英文的Unicode范围是0x0000-0x007F，中文的Unicode范围是0x4E00-0x9F**。对于那些扩展平面中需要4个字节才能表示的字符，UTF-16使用一种代理的手法来扩展（使用了基本多语言平面保留的0xD800-0xDFFF区段，表示这是一个代理，从而用2个16位码元组成一个字符）。这样的好处是大量的节约了存取空间，也提高了处理的速度。这种Unicode表示方法就是UTF-16，显然，UTF-16需要1个或者2个16位长的码元来表示，因此这是一个变长表示。一般在Windows平台上，提到Unicode，那就是指UTF-16了。
UTF-16有一个著名的Endian的问题，即UTF16-LE和UTF16-BE，LE指Little Endian，而BE指Big Endian。关于这方面的信息，网上有很多相关的帖子。这与计算机的CPU架构有一定关系，我们一般的X86系统都是Little Endian的，可以认为UTF16就是UTF16-LE。
另外，UTF有一个BOM（Byte Order Mark）的问题。在Unicode编码中有一个叫做"zero-width no-break space (ZWNBSP)"的字符，它的编码是0xFEFF。而0xFEFF在是一个实际中不存在的字符，所以**不应该出现在实际传输中**。UCSUCS (Unicode Character Set) 规范建议我们在传输字节流前，先传输字符"ZWNBSP"。这样如果接收者收到FEFF，就表明这个字节流是Big-Endian的；如果收到FFFE，就表明这个字节流是Little-
 Endian的。因此字符"ZWNBSP"又被称作BOM。
**UTF-8**
UTF16的最大好处在于大部分字符都以固定长度的字节（2字节）储存，但UTF-16却无法兼容于ASCII编码。由于对于欧洲和北美，实际上使用的编码范围在0x0000-0x00FF之间，只需要一个字符就可以表示所有的字符。即使是使用UTF16来作为内存的存取方式，还是会带来巨大的空间浪费，因此就有了UTF8的编码方式。
UTF-8编码是最灵活的UTF编码形式，即兼容ISO 8859-1的编码，同时也可以用来表示所有语言的字符。显然，UTF-8编码是不定长编码，每一个字符的长度从1-6个字节不等。另外，UTF编码自带简单的校验功能。
UTF-8编码中，对于只需要1个字节的字符，就使用一个字节；对于中日韩等Unicode中需要两个字节才能表示的字符，则通过一个 UTF16 – UTF8 的算法实现相互之间的转换（转换后的UTF-8一般需要3个字节），而对于Unicode中需要4个字节才能表示的字符，UTF-8根据需要可以扩展到6个字节来表示一个字符。UTF8使用的算法很有意思，大致映射关系如下：
UTF-32                                                         UTF8
0x00000000 - 0x0000007F         0xxxxxxx
0x00000080 - 0x000007FF          110xxxxx 10xxxxxx
0x00000800 - 0x0000FFFF          1110xxxx 10xxxxxx 10xxxxxx
0x00010000 - 0x001FFFFF           11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
0x00200000 - 0x03FFFFFF           111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
0x04000000 - 0x7FFFFFFF           1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
可以发现这和IP的分址算法很是相像。
由于UTF-8可以方便的转换为UTF16和UTF32（不需要码表，执行一个转换算法即可，在Unicode.org上提供了[C代码](http://ftp//www.unicode.org/Public/PROGRAMS/CVTUTF/)）。而且UTF-8在每个操作系统平台上的实现都是一样的，也不存在跨平台的问题，所以UTF-8成为跨平台的Unicode很好的解决方案。当然，对于中文来说，由于每个字符需要3个字节才能表示，还是有点浪费的。
注意，虽然说UTF-8是为了使用更少的空间而使用的，但那只是相对于Unicode编码来说，如果已经知道是汉字，则使用GB2312/GBK无疑是最节省的。不过另一方面，值得说明的是，对于中文网页，虽然UTF-8编码对汉字使用3个字节，UTF8编码也会比UTF-16编码节省，因为网页HTML中包含了更多的英文字符。
UTF-8 是不需要BOM来表明字节顺序，但可以用BOM来表明编码方式。字符"ZWNBSP"即“0xFEFF”的UTF-8编码是EF BB BF（根据上表转换关系）。所以如果接收者收到以EF BB BF开头的字节流，就知道这是通知其收到的是UTF-8编码了。
Windows系统就是用BOM来标记文本文件的编码方式的。用UltraEdit的16进制编辑模式查看UTF-8编码的文件，都是EF BB BF开头的，说明都是带BOM的。参照下面的GB2312/GBK的编码，可以解释为什么在出现编码问题时，经常看到这三个汉字“锘匡豢”：
![](https://oixbwg.dm1.livefilestore.com/y2pb2VhtE2ItV0gLcSWaZs7gXtoG55zCaD2q4k0E1dxe121fFQ35bEwHOvBlNmbU42id5-2ErVyIOi-MbPL39K5516XozStmqL9BMNEUYXfpkE/utf8-efbbbf.png)
