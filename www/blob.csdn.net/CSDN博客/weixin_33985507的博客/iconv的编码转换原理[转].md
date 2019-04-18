# iconv的编码转换原理[转] - weixin_33985507的博客 - CSDN博客
2011年07月21日 15:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
(2008-04-01 15:43:20)
转载标签： 
iconv	分类： PHP专题 
更广义的字符集转换：iconv
 在先前的章节中，我们已见到了两组 mbs 与 wcs 互转的函式，第一组对于有「状态改变」的编码系统 mbs 无法做到字符串的状态控制，故不适合做该 mbs 的转换工作；而第二组则可以直接做字符串的状态控制，故使用范围就更广了。然而，这两组字符串转换函式在某些使用场合下都有很大的限制，广义而言，它们都属于「字符集转换函式」，然而它们都直接与 I18N、locale 机制绑在一起，也就是说在使用它们之前，程序都必须设好正确的 locale 才行。故在以下的情况使用它们就会很不方便，甚至根本行不通：
 "        如果程序中需要做 A 字集编码与 B 字集编码的转换时，不幸的是这两种字集 编码都不是目前程序所处的 locale 所采用的。若使用前述的 wcs 与 mbs 转换函式，唯一的办法只有先呼叫 setlocale()，将程序的 LC_CTYPE locale 先切换到使用字集编码 A 的 locale，把字符串 A 转成 wcs 字符串后，再呼叫一次 setlocale()，将程序的 locale 切换到使用字集编码 B 的 locale，最后才把 wcs 字符串转成字符串 B。万一 都找不到任何 locale 采用字集 A 或字集 B 时，则这一招就没有用了。
 "        如果程序中需要同时做多种字集编码的转换时，则这些 wcs 与 mbs 的转换函式就无法办到了。原因是一旦 setlocale() 设定好程序的 locale 之后，其影响是遍及整个程序每个部分，我们不可能做到在转换字集编码 A 时设好 locale A，又 ``同时'' 设好 locale B 来转字集编码 B。
 因此，我们需要更广义的字集转换系统，一个可以与 locale 完全无关的转换系统，才能方便地达到上述的要求。故在 XPG2 标准中另外定义了一组全新的函式接口： iconv。事实上，在 glibc 中，表面上看来那些 wcs 与 mbs 转换函式与 iconv 不太一样，但它们底层的 wcs 与 mbs 转换工作却是由 iconv 来达成的。故 iconv 可以说是字集转换系统中最基底的函式接口。
 iconv 字集转换系统只有三个函式，在很多系统中是宣告在 iconv.h 里头，使用上与一般在做档案读写的概念一样，先 ``开启''、之后 ``操作''、完毕后要 ``关闭''，这些函式包括：
 "        iconv_t iconv_open(const char *TOENC, const char *FROMENC)
 "        size_t iconv (iconv_t CD, const char **INBUF, size_t *INBYTESLEFT, char **OUTBUF, size_t *OUTBYTESLEFT)
 "        int iconv_close(iconv_t CD)
 首先 iconv_open() 函式就是做 ``开启'' 动作，也就是当我们要将编码系统 A 转换到编码系统 B 时，必须先呼叫此函式，将 FROMENC 设成编码系统 A 的名字，同时将 TOENC 设成编码系统 B 的名字，这时此函式就会做类似档案开启的动作，传回一个代表此转换管道的数据结构 iconv_t 供后续使用。事实上，在系统的实作中真的是将 iconv_open() 当作 ``开启档案'' 来处理，故它会受到目前系统或同一行程中可开启档案数所限，如果系统或程序的其它部分已开启了太多的档案以至于逼近系统上限，则有可能这边的 iconv_open() 会失败。
 如果 iconv_open() 开启失败时，它会传回 (iconv_t)-1 的值，同时设定 errno 全域变量，用以指出开启失败的原因。而开启失败的原因，不外乎就是已开启的档案数已超过上限、或系统内存不足、或系统本身无法做到编码系统 A 与 B 之间的转换。有兴趣的读者可以直接去阅读 info libc， * Character Set Handling:: 一节了解各 errno 的值与其所代表的意义。
 第二个 iconv() 函式就是用来做实际编码系统转换工作的，它必须先呼叫过 iconv_open() 并取得 iconv_t 结构后才能工作。只要 iconv_open() 可以开启成功，则理论上它就可以进行转换工作，而且不论其来源编码与目标编码是 mbs 字符串或 wcs 字符串、或二者的混合互转，都没有关系。但要注意的是，如果其中有 wcs 字符串时，就算用来存放 wcs 字符串是使用 wchar_t * 形别，在传入此函式时仍然统一用 char * 来处理。
 若要做字符串 A 转换成字符串 B 时，字符串 A 是经由 *INBUF 传入，而 *INBYTESLEFT 则传入数组 A 的长度，一律以字节数来计算。而转换的结果则由 *OUTBUF 传回，同样的 *OUTBYTESLEFT 则为 *OUTBUF 的长度。如果转换成功了，则 *INBUF 最后会设在存放字符串 A 的数组末尾，而 *INBYTESLEFT 会设为目前此数组还剩多少字节可以用。而 *OUTBUF 与 *OUTBYTESLEFT 也是一样。因此我们可以在相同的 A、B 数组中重复呼叫此函式。举个例子来说，如果转换过程中在 A 数组里头遇到了不合法的字符而无法进行转换时，则 *INBUF 与 *OUTBUF 就会停在无法转换的位置上，并将已转换成功的结果传回，则我们可以自行决定看是要跳过那个不合法的字符，继续转换剩余的部分，或做其它的特殊处理 ...等等。
 在很多情况下有可能造成 iconv() 的转换失败，例如前面提到的在 A 数组中遇到不合法的字符，或者 A 中某个字符在 B 中找不到可对应的字。其中第二个情况是最有可能发生的，而目前 glibc-2.1.x 系统中遇到这种情况时就当做是转换失败，而不做进一步的处理。当然这并不是最好的处理方式，故未来可能会有所改变。而根据 info libc 的说明，如果转换失败了， iconv() 最后的传回值是 (size_t)-1，并设定 errno 的值说明失败的原因；如果转换成功，则传回值会是已成功转换的字符个数。但我们实际的测试结果，在 glibc-2.1.3 的系统下，如果转换成功后其传回值却永远是 0，与 info libc 上的描述不一样，这点有点奇怪，不晓得是不是目前 glibc 的 bug？
 若遇到含「状态改变」的编码系统时，iconv() 也能正确工作，它能在转换的过程中随时记录、更新字符串目前的状态 (应该就是记录在 iconv_t CD 结构中)，故就算是采「分期付款」方式将同一字符串切成数分一段段来转换，也不会出问题。但要注意的是，在第一次使用 iconv() 之前，必须先初始化一下 A、B 两字符串的状态，就好像我们在先前所提的 mbsrtowcs() 等函式一样，在使用前也必须先做好状能初始化，如此才能让后续可以正常工作。而对于 iconv() 而言，初始化的方式就是呼叫它时，*INBUF 与 *OUTBUF 都设为 NULL 即可。
 最后一个 iconv_close() 函式，就是当整个转换结束后，用来做「关闭档案」用的。
 底下我们就写了一个范例程序，用来说明 iconv 函式界面的使用方式：
 #include <stdio.h>
 #include <string.h>
 #include <iconv.h>
int main(int argc, char **argv)
 {
   FILE *fin, *fout;
   char *encFrom, *encTo;
   char bufin[1024], bufout[1024], *sin, *sout;
   int mode, lenin, lenout, ret, nline;
   iconv_t c_pt;
  if (argc != 5) {
     printf("Usage: a.out <encFrom> <encTo> <fin> <fout>\n");
     return 0;
   }
   encFrom = argv[1];
   encTo   = argv[2];
   if ((fin = fopen(argv[3], "rt")) == NULL) {
     printf("Cannot open file: %s\n", argv[3]);
     return -1;
   }
   if ((fout = fopen(argv[4], "wt")) == NULL) {
     printf("Cannot open file: %s\n", argv[4]);
     return -1;
   }
  if ((c_pt = iconv_open(encTo, encFrom)) == (iconv_t)-1) {
     printf("iconv_open false: %s ==> %s\n", encFrom, encTo);
     return -1;
   }
   iconv(c_pt, NULL, NULL, NULL, NULL);
  nline = 0;
   while (fgets(bufin, 1024, fin) != NULL) {
     nline ++;
     lenin  = strlen(bufin) + 1;
     lenout = 1024;
     sin    = bufin;
     sout   = bufout;
     ret = iconv(c_pt, &sin, &lenin, &sout, &lenout);
     printf("%s -> %s: %d: ret=%d, len_in=%d, len_out=%d\n",
            encFrom, encTo, nline, ret, lenin, lenout);
     if (ret == -1) {
       printf("stop at: %s\n", sin);
       break;
     }
     fprintf(fout, "%s", bufout);
   }
   iconv_close(c_pt);
   fclose(fin);
   fclose(fout);
   return 0;
 }
 这个程序可以从命令列输入来源档案与目的档案的编码系统名称，将来源档案的内容转换成目的档案。各位读者可以注意到，在程序中我们完全没有做 locale 设定等相关动作，原因正是不需要，iconv 函式接口是与 locale 完全无关的。同时，我们的程序只适合拿来做两个 mbs 编码系统间的转换，不适合拿来做其中一个有 wcs 的转换，原因是程序中我们没有特别为 wcs 的情况使用 wchar_t * 数组，同时记得我们以前也提过，wcs 字符串是不能拿来做档案输出的，而我们的程序目前正是直接做档案编码转换的工作。
 这样的转码程序倒底「耐不耐用」呢？有兴趣的读者可以跟着我们一起来测试。由于先前我们已介绍过了如何在您的 GNU/Linux 系统中安装 zh_TW.Big5 与 zh_CN.GB2312 locale 环境，如果您都已安装无误的话，理论上您的系统的 Big5 与 GB2312 的 gconv 系统都有不成问题了 (在 glibc 中，gconv 系统可以说是 iconv 的心脏，我们会在下一小节详细说明)，故我们就直接以 Big5 与 GB2312 两个编码系统互转为例子，测试一下我们的程序。首先，请先准备一个叫 f-big5 的档案，内容用 Big5 编码打入这样一行的内容：
 我是研究生
 然后，将我们的范例程序编译好，假设程序执行文件名就是 a.out，接着执行：
 a.out BIG5 GB2312 f-big5 output
 在 glibc-2.1.x 的系统下，应该可以正确转换，这时您应该会见到如下的程序输出：
 BIG5 -> GB2312: 1: ret=0, len_in=0, len_out=1012
 其中 ret=0 就表示转换成功，然后，您可以再开一个可以看 GB2312 编码的 terminal 来看 output 档的内容，您就会见到原来 f-big5 档的内容已转换成 GB2312 码了。
 看起来似乎很完美，我们再进一步测试。现在请在 f-big5 档中再加一些内容，像这样：
 我是研究生
 目前在正在做研究测试
 按照上头的指令再执行一次，我们得到如下的结果：
 BIG5 -> GB2312: 1: ret=0, len_in=0, len_out=1012
 BIG5 -> GB2312: 2: ret=-1, len_in=6, len_out=1008
 stop at: 测试
 第一行没有问题，但第二行失败了，程序指出第二行最后两个字 ``测试'' 无法做转换，为什么会这样呢？
 要探讨其原因，就要更深入 iconv 的编码系统转换原理了。我们在之前已多次提过，在做 mbs 与 wcs 之间转换时，其实就是将 mbs 转换成系统的基底字集，而 iconv 的转换原理也是这般。事实上在 glibc 系统中，它在做编码系统 A 与 B 之间的转换时，有两种途径可选择：一是如果 iconv 内部已内建了 A 与 B 的对应表格时，就采用此对应表格来转换，而这也是最可信赖的转换方式。万一它找不到适当的对应表格时，它则会先将 A 转换成基底字集，再由基底字集转换成 B。也就是系统的基底字集在这里扮演了中间媒介的角色。
 要注意的是，在其它的 UNIX 系统中，它们可能只提供第一种途径供转换，而 glibc 提供第二种途径的理由是，这样可以确保任意两个字集编码都有机会进行转换，只要这两个字集编码都同时被系统所支持。而且，由于 glibc 所选择的「基底字集」理论上已包含了目前世界上所有正在使用的计算机字集，故使用基底字集做为中间媒介是合理而且可行的。
 但各种字集编码的问题是很复杂的玩意儿，有时候这一套也有出槌的时候。就像前面见到的 Big5 的 ``测试'' 无法转成 GB2312 一样，我们再做以下的实验，就会知道原因了。因为 glibc 是以 UCS4 做为其系统的基底字集，而 UCS4 的 mbs 形式为 UTF8 编码系统，故我们现在准备两个档案，一个是 f-big5，内含 Big5 码的 ``测试'' 两字，另一个是 f-gb，内含 GB2312 码的 ``测试'' 两字。现在我们用上头那个范例程序将这两个档案都转成 UTF8 编码：
 a.out BIG5 UTF8 f-big5 output1
 a.out GB2312 UTF8 f-gb output2
 这两个都可以转换成功。然后，我们用 diff 程序比较一下 output1 与 output2 的内容，发现它们的内容不一样！
 这就是无法转换成功的原因！因为在中文里头意义一样的两个字，在 Big5 与 GB2312 里头分别被对应到不同的 UCS4 编码里头去了。为什么会如此？这牵涉到更深刻的 UCS4 编码规则与其它编码系统的对应问题，在这里我们就先略过了，以后若有机会的话再回来讨论。现在的问题是，在 glibc 中，如果 A 与 B 的某些字分别被对应到基底字集中不同的编码中，就有可能会无法转换成功。而这往往是很有可能发生的，特别是 glibc 直接采用 UCS4 做为其基底字集时。
 而且，在回顾一下我们前面谈的，在不同的 UNIX 系统下，其 iconv 底层的运作模式可能都不太一样，这就造成了更严重的问题：就算系统同时支持 A 与 B 两种编码，我们仍不能保证可以将 A 转成 B。或者，该系统中可以将 A 转成 C，也可以将 C 转成 B，但也不能保证可以将 A 直接 (或间接) 转成 B。
 这的确是另人气馁的问题，现在也许读者就会问了，那 iconv 有什么用？我们的认为是，它在小字集与大字集编码间的转换是有用的。例如 Big5 与 GB2312 分别是 UTF8 的子字集，则我们可以很放心地使用 iconv 将 Big5 或 GB2312 转成 UTF8，或反方向地转换回来 (当然，前提是原来的 UTF8 字符串中没有包含 Big5 或 GB2312 所没有的字)。又或者 Big5 是 GBK 的一个子字集，我们一样也可以利用它来做转换。
 但如果要直接用 iconv 来做 Big5 与 GB2312 的转换的话，唯一个可靠方法就是在 iconv 系统中加入一个 Big5 与 GB2312 直接互转的表格，并且希望此表格可以成系统标准的一部分 (至少要是 glibc 标准的一部分)，如此我们的程序这样写才会有用 (至少在 glibc 的系统下)。因此，现在我们就进入下一小节，来看看 glibc 的 iconv 的核心部分，做为本章的一个结束。
