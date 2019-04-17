# Unicode 和 ANSI - DoubleLi - 博客园






Project Properties -> General-> Character set,里面显示了是不是unicode. Unicode处理String的方式不一样，一定要注意！！





### 使你的C/C++代码支持Unicode的第一步
- 定义宏 _UNICODE， 如果定义了宏 _MBCS 则取消它的定义(undefine)。
- 在字符串前添加 L 标记或者用 _T宏修饰字符串。
- 使用 Wide 或者 TCHAR 版本的字符串处理函数。
- 确定API中的字符串长度是按字节计数还是按字符个数计数。因为基于字符的显示和打印(与此不同的是，GUI是基于像素的)使用 列数，而不是字节数或者字符个数。
- 在字符串指针相关的计算中使用GetNext格式，因为一个字符可能包含多于一个Unicode字符单元。
- 注意缓冲区的大小以及防止缓冲区溢出。
改变编码方式可能需要增大缓冲区或者限制字符串的最大长度。假设单个字符的大小从1个字节变为4个字节，并且字 符串本来20个字符占用20字节，那么你需要将字符串缓冲区扩大为80字节或者将字符串长度限制为5个字符(字符串缓冲区仍为20字节)。注意缓冲区的扩 大可能被限制到一个最大值(比如65KB)。减少字符串长度到一个固定值可能破坏现有的程序，限制字符串长度到固定值可能是危险的。比如，限制到20字 节，将字符串转化为大写形式就可能导致字符串变长并且超过限制。
- 将接受或者返回单字符参数的函数替换为使用字符串的版本。 (在一些语言中) 对于单个字符的操作可能导致返回多个代码点。例如，upper('ß')将 返回"SS"。
- 使用 wmain 代替 main。环境变量也由_environ变 为_wenviron 。
wmain(int argc, wchar_t *argv[], wchar_t *envp[])
- MFC Unicode 程序使用  wWinMain 作为程序入口点(VC++ 6.0)。 
project->settings->Link，选择output选项卡
在Entry point symbol一行加上 wWinMainCRTStartup 。
- 字体的因素，分清使用的字体用于渲染各种语言的字符还是用于脚本中。

[返回页面顶端](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#top)

### 文件 I/O, 数据库, 传输协议等因素
- 考虑是否需要读写文件、数据库中的 UTF-8 或者 UTF-16 字符，以及是否进行数据交换。
- 考虑 UTF-16 格式文件中的字节序。 
读写网络传输的数据总是使用 Big-Endian ，如果你没有产生 BOM 也使用 Big-Endian。
文件的字节序依赖于文件格式以及/或者源/目标机器的体系结构。
读取 UTF-16 或者 UTF-32编码的文件时，考虑是否需要将字符按字节逆序。 
对于 [streams](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#streams) 和传输协议也需要做上述的考虑。
- 传输协议和用于数据交换的文件要使用正确的编码方式。例如 HTTP，HTML，XML 必须设置为 UTF-8 或者 UTF-16。
- 考虑Unicode字节序标记( BOM ,Byte Order Marker) 以及是否需要将它同数据一同写入。读取数据时记得去掉BOM。
- 考虑遗留数据和文件的编码惯例，考虑导入和导出以及传输协议。([MultiByteToWideChar](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/unicode_17si.asp), [WideCharToMultiByte](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/intl/unicode_2bj9.asp), [mbtowc](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclib/html/_crt_mbtowc.asp), [wctomb](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclib/html/_crt_wctomb.asp), [wctombs](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclib/html/_crt_wcstombs.asp), [mbstowcs](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclib/html/_crt_mbstowcs.asp) )
- 考虑复制文本到剪贴板
使用 CF_TEXT 格式并且写入本地编码的文本(ANSI格式)
使用 CF_UNICODETEXT 格式并写入Unicode文本
- 数据库程序需要考虑数据类型 (NCHAR, NVARCHAR) 和模式(schema)的变更，触发器，存储过程和查询，数据的增长，索引以及性能等因素。
注意：Unicode模式的改变对于不同厂商的数据库产生不同的影响。如果需要数据库具有较好的可移植性，就要考虑到每种数据库的特性和行为。 
*(我知道针对这个问题说得不够多，以后有机会再补充吧)*

[返回页面顶端](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#top)

### 流式 I/O

如果你使用Microsoft C++编译器，你可能遇到与流式 I/O相关的3类问题：
- **不支持Unicode文件名。**
解决方式是使用 FILE * _wfopen 函数，之后使用FILE句柄初始化流式 I/O。


std::ifstream stm(_wfopen(pFilename, L"r"));

- **在 读/写 的时候，流式 I/O 会把数据 从本地代码页(ANSI格式)转换到Unicode格式/从Unicode格式转换到ANSI格式，而非UTF-8 或者 UTF-16。**但是可以修改表示流的类使之支持读写 UTF-8 格式字符。你可以自己实现一个读写时把数据在 Unicode 和 UTF-8 之间转换的I/O stream类。


codecvt >

- 如果要用流式 I/O读写 UTF-16 字符，应该用二进制模式打开并且在二进制模式下输入输出。可以用如下方法设置 I/O 为二进制模式：


_setmode( _fileno( stdin ), _O_BINARY );

也可以参考 Microsoft 运行时库参考： "**Unicode Stream I/O in Text and Binary Modes**".

**注意：**针对cout/wcout, cin/wcin等并没有相应的 TCHAR 版本。如果你需要在ANSI/Unicode两种模式下编译 代码，你可能需要自己定义一个名字类似"tout"的宏。

[返回页面顶端](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#top)

### 国际化，Unicode高级技术，平台和其它因素
- 考虑使用基于地区的办法和更进一步的国际化。
- 对于 Windows 95，98 和 Windiws ME，考虑使用 [Microsoft MSLU (Microsoft Layer for Unicode)](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#mslu)
- 考虑字符串比较和排序，[Unicode Collation Algorithm](http://www.unicode.org/reports/tr10/)
- 考虑 [Unicode Normalization](http://www.unicode.org/reports/tr15/)
- 考虑 [Character Folding](http://www.unicode.org/reports/tr30/)
- ![XenCraft provides Unicode-enabling services](http://www.i18nguy.com/images/xencraftlogo.gif)重新 考虑是否要自己处理这些事情。借助于一家[Unicode咨询公司](http://www.xencraft.com/)，然后让你的团队集中精力做他们擅长的事情。(嗨， 我们也要谋生啊...)


[返回页面顶端](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#top)

### Unicode字节序标记(BOM) 值
|编码方式|BOM值|
|----|----|
| | |
|UTF-8|EF BB BF|
|UTF-16 (big-endian)|FE FF|
|UTF-16 (little-endian)|FF FE|
|UTF-16BE, UTF-32BE (big-endian)|No BOM!|
|UTF-16LE, UTF-32LE (little-endian)|No BOM!|
|UTF-32 (big-endian)|00 00 FE FF|
|UTF-32 (little-endian)|FF FE 00 00|
|SCSU (compression)|0E FE FF|

Unicode字节序标记(BOM，Byte Order Marker) 是Unicode字符 U+FEFF。(它也能表示一个被称作 Zero Width No-break Space 的字符)。U+FFFE 这个代码点在Unicode中是非法的，它永远不应该出现在一个Unicode字符流中。所以BOM可以作为放置于文件(或者一个字符串)的起始作为字节 序的指示器。对UTF-16编码而言，如果第一个字符的值是FE FF 那么文本和读取文本的机器有相同的字节序。如果是 FF FE，那么有相反的字节序并且需要对每 个16-bit字按字节逆序。同样的，BOM指示了UTF-32编码的文本的字节序。

注意不是所有的文件都以Unicode字节序标记开始。事实上，Unicode标准称若不以Unicode字节序标记 (BOM)开始(数据)就必须被表示成big-endian形式。

字符 U+FEFF 同样作为不同Unicode编码方式的标记。左边的表格说明了 U+FEFF 在每一种Unicode编码方式中的值。注意：按照定义，标记为UTF-16BE, UTF-32BE, UTF-32LE or UTF-16LE 的文本不应该有BOM，字节序已经由标记本身指出了。

对于使用 [SCSU](http://www.unicode.org/reports/tr6/) (Standard Compression Scheme for Unicode) 算法压缩过的文本，也有一个推荐的签名。

[返回页面顶端](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#top)


### 常量和全局变量
|ANSI版本|宽字符版本|宏定义版本(TCHAR)|
|----|----|----|
|EOF|WEOF|_TEOF|
|_environ|_wenviron|_tenviron|
|_pgmptr|_wpgmptr|_tpgmptr|



### 数据类型
|ANSI版本|宽字符版本|宏定义版本(TCHAR)|
|----|----|----|
|char|wchar_t|_TCHAR|
|_finddata_t|_wfinddata_t|_tfinddata_t|
|__finddata64_t|__wfinddata64_t|_tfinddata64_t|
|_finddatai64_t|_wfinddatai64_t|_tfinddatai64_t|
|int|wint_t|_TINT|
|signed char|wchar_t|_TSCHAR|
|unsigned char|wchar_t|_TUCHAR|
|char|wchar_t|_TXCHAR|
||L|_T 或者 _TEXT|
|LPSTR(char *)|LPWSTR(wchar_t *)|LPTSTR(_TCHAR *)|
|LPCSTR(const char *)|LPCWSTR(const wchar_t *)|LPCTSTR(const _TCHAR *)|
|LPOLESTR(For OLE)|LPWSTR|LPTSTR|


[返回页面顶端](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#top)


### Platform SDK字符串处理API

有很多Windows API函数会根据宏 UNICODE 是 否被定义而编译成不同形式。那些需要同时操作ANSI字符和宽字符的模块需要了解这一点。否则，应该使用宏定义版本的名字，这样的话就只需要定义宏 UNICODE 并且重新编译程序。

下列列表并没有列举所有的有ANSI和宽字符两个版本的API，只列举了与字符和字符串处理相关的一些。如果需要查看与代码页和地区相 关 的API请查看WinNLS.h头文件。
|ANSI版本|宽字符版本|宏定义版本(TCHAR)|
|----|----|----|
|CharLowerA|CharLowerW|CharLower|
|CharLowerBuffA|CharLowerBuffW|CharLowerBuff|
|CharNextA|CharNextW|CharNext|
|CharNextExA|CharNextExW|CharNextEx|
|CharPrevA|CharPrevW|CharPrev|
|CharPrevExA|CharPrevExW|CharPrevEx|
|CharToOemA|CharToOemW|CharToOem|
|CharToOemBuffA|CharToOemBuffW|CharToOemBuff|
|CharUpperA|CharUpperW|CharUpper|
|CharUpperBuffA|CharUpperBuffW|CharUpperBuff|
|CompareStringA|CompareStringW|CompareString|
|FoldStringA|FoldStringW|FoldString|
|GetStringTypeA|GetStringTypeW|GetStringType|
|GetStringTypeExA|GetStringTypeExW|GetStringTypeEx|
|IsCharAlphaA|IsCharAlphaW|IsCharAlpha|
|IsCharAlphaNumericA|IsCharAlphaNumericW|IsCharAlphaNumeric|
|IsCharLowerA|IsCharLowerW|IsCharLower|
|IsCharUpperA|IsCharUpperW|IsCharUpper|
|LoadStringA|LoadStringW|LoadString|
|lstrcatA|lstrcatW|lstrcat|
|lstrcmpA|lstrcmpW|lstrcmp|
|lstrcmpiA|lstrcmpiW|lstrcmpi|
|lstrcpyA|lstrcpyW|lstrcpy|
|lstrcpynA|lstrcpynW|lstrcpyn|
|lstrlenA|lstrlenW|lstrlen|
|OemToCharA|OemToCharW|OemToChar|
|OemToCharBuffA|OemToCharBuffW|OemToCharBuff|
|wsprintfA|wsprintfW|wsprintf|
|wvsprintfA|wvsprintfW|wvsprintf|


[返回页面顶端](http://www.i18nguy.com/unicode/c-unicode.zh-CN.html#top)


### CRT字符串处理API

函数按照ANSI版本的ASCII字母顺序排序，方便转换到相应的Unicode版本。
|ANSI版本|宽字符版本|宏定义版本(TCHAR)|
|----|----|----|
|_access|_waccess|_taccess|
|_atoi64|_wtoi64|_tstoi64|
|_atoi64|_wtoi64|_ttoi64|
|_cgets|_cgetws|cgetts|
|_chdir|_wchdir|_tchdir|
|_chmod|_wchmod|_tchmod|
|_cprintf|_cwprintf|_tcprintf|
|_cputs|_cputws|_cputts|
|_creat|_wcreat|_tcreat|
|_cscanf|_cwscanf|_tcscanf|
|_ctime64|_wctime64|_tctime64|
|_execl|_wexecl|_texecl|
|_execle|_wexecle|_texecle|
|_execlp|_wexeclp|_texeclp|
|_execlpe|_wexeclpe|_texeclpe|
|_execv|_wexecv|_texecv|
|_execve|_wexecve|_texecve|
|_execvp|_wexecvp|_texecvp|
|_execvpe|_wexecvpe|_texecvpe|
|_fdopen|_wfdopen|_tfdopen|
|_fgetchar|_fgetwchar|_fgettchar|
|_findfirst|_wfindfirst|_tfindfirst|
|_findnext64|_wfindnext64|_tfindnext64|
|_findnext|_wfindnext|_tfindnext|
|_findnexti64|_wfindnexti64|_tfindnexti64|
|_fputchar|_fputwchar|_fputtchar|
|_fsopen|_wfsopen|_tfsopen|
|_fullpath|_wfullpath|_tfullpath|
|_getch|_getwch|_gettch|
|_getche|_getwche|_gettche|
|_getcwd|_wgetcwd|_tgetcwd|
|_getdcwd|_wgetdcwd|_tgetdcwd|
|_ltoa|_ltow|_ltot|
|_makepath|_wmakepath|_tmakepath|
|_mkdir|_wmkdir|_tmkdir|
|_mktemp|_wmktemp|_tmktemp|
|_open|_wopen|_topen|
|_popen|_wpopen|_tpopen|
|_putch|_putwch|_puttch|
|_putenv|_wputenv|_tputenv|
|_rmdir|_wrmdir|_trmdir|
|_scprintf|_scwprintf|_sctprintf|
|_searchenv|_wsearchenv|_tsearchenv|
|_snprintf|_snwprintf|_sntprintf|
|_snscanf|_snwscanf|_sntscanf|
|_sopen|_wsopen|_tsopen|
|_spawnl|_wspawnl|_tspawnl|
|_spawnle|_wspawnle|_tspawnle|
|_spawnlp|_wspawnlp|_tspawnlp|
|_spawnlpe|_wspawnlpe|_tspawnlpe|
|_spawnv|_wspawnv|_tspawnv|
|_spawnve|_wspawnve|_tspawnve|
|_spawnvp|_wspawnvp|_tspawnvp|
|_spawnvpe|_wspawnvpe|_tspawnvpe|
|_splitpath|_wsplitpath|_tsplitpath|
|_stat64|_wstat64|_tstat64|
|_stat|_wstat|_tstat|
|_stati64|_wstati64|_tstati64|
|_strdate|_wstrdate|_tstrdate|
|_strdec|_wcsdec|_tcsdec|
|_strdup|_wcsdup|_tcsdup|
|_stricmp|_wcsicmp|_tcsicmp|
|_stricoll|_wcsicoll|_tcsicoll|
|_strinc|_wcsinc|_tcsinc|
|_strlwr|_wcslwr|_tcslwr|
|_strncnt|_wcsncnt|_tcsnbcnt|
|_strncnt|_wcsncnt|_tcsnccnt|
|_strncnt|_wcsncnt|_tcsnccnt|
|_strncoll|_wcsncoll|_tcsnccoll|
|_strnextc|_wcsnextc|_tcsnextc|
|_strnicmp|_wcsnicmp|_tcsncicmp|
|_strnicmp|_wcsnicmp|_tcsnicmp|
|_strnicoll|_wcsnicoll|_tcsncicoll|
|_strnicoll|_wcsnicoll|_tcsnicoll|
|_strninc|_wcsninc|_tcsninc|
|_strnset|_wcsnset|_tcsncset|
|_strnset|_wcsnset|_tcsnset|
|_strrev|_wcsrev|_tcsrev|
|_strset|_wcsset|_tcsset|
|_strspnp|_wcsspnp|_tcsspnp|
|_strtime|_wstrtime|_tstrtime|
|_strtoi64|_wcstoi64|_tcstoi64|
|_strtoui64|_wcstoui64|_tcstoui64|
|_strupr|_wcsupr|_tcsupr|
|_tempnam|_wtempnam|_ttempnam|
|_ui64toa|_ui64tow|_ui64tot|
|_ultoa|_ultow|_ultot|
|_ungetch|_ungetwch|_ungettch|
|_unlink|_wunlink|_tunlink|
|_utime64|_wutime64|_tutime64|
|_utime|_wutime|_tutime|
|_vscprintf|_vscwprintf|_vsctprintf|
|_vsnprintf|_vsnwprintf|_vsntprintf|
|asctime|_wasctime|_tasctime|
|atof|_wtof|_tstof|
|atoi|_wtoi|_tstoi|
|atoi|_wtoi|_ttoi|
|atol|_wtol|_tstol|
|atol|_wtol|_ttol|
|字符比较|映射为宏或者inline 函数|_tccmp|
|字符拷贝|映射为宏或者inline 函数|_tccpy|
|字符长度|映射为宏或者inline 函数|_tclen|
|ctime|_wctime|_tctime|
|fgetc|fgetwc|_fgettc|
|fgets|fgetws|_fgetts|
|fopen|_wfopen|_tfopen|
|fprintf|fwprintf|_ftprintf|
|fputc|fputwc|_fputtc|
|fputs|fputws|_fputts|
|freopen|_wfreopen|_tfreopen|
|fscanf|fwscanf|_ftscanf|
|getc|getwc|_gettc|
|getchar|getwchar|_gettchar|
|getenv|_wgetenv|_tgetenv|
|gets|getws|_getts|
|isalnum|iswalnum|_istalnum|
|isalpha|iswalpha|_istalpha|
|isascii|iswascii|_istascii|
|iscntrl|iswcntrl|_istcntrl|
|isdigit|iswdigit|_istdigit|
|isgraph|iswgraph|_istgraph|
|islead (总是返回FALSE)|(总是返回FALSE)|_istlead|
|isleadbyte (总是返回FALSE)|isleadbyte (总是返回FALSE)|_istleadbyte|
|islegal (总是返回TRUE)|(总是返回TRUE)|_istlegal|
|islower|iswlower|_istlower|
|isprint|iswprint|_istprint|
|ispunct|iswpunct|_istpunct|
|isspace|iswspace|_istspace|
|isupper|iswupper|_istupper|
|isxdigit|iswxdigit|_istxdigit|
|main|wmain|_tmain|
|perror|_wperror|_tperror|
|printf|wprintf|_tprintf|
|putc|putwc|_puttc|
|putchar|putwchar|_puttchar|
|puts|_putws|_putts|
|remove|_wremove|_tremove|
|rename|_wrename|_trename|
|scanf|wscanf|_tscanf|
|setlocale|_wsetlocale|_tsetlocale|
|sprintf|swprintf|_stprintf|
|sscanf|swscanf|_stscanf|
|strcat|wcscat|_tcscat|
|strchr|wcschr|_tcschr|
|strcmp|wcscmp|_tcscmp|
|strcoll|wcscoll|_tcscoll|
|strcpy|wcscpy|_tcscpy|
|strcspn|wcscspn|_tcscspn|
|strerror|_wcserror|_tcserror|
|strftime|wcsftime|_tcsftime|
|strlen|wcslen|_tcsclen|
|strlen|wcslen|_tcslen|
|strncat|wcsncat|_tcsncat|
|strncat|wcsncat|_tcsnccat|
|strncmp|wcsncmp|_tcsnccmp|
|strncmp|wcsncmp|_tcsncmp|
|strncpy|wcsncpy|_tcsnccpy|
|strncpy|wcsncpy|_tcsncpy|
|strpbrk|wcspbrk|_tcspbrk|
|strrchr|wcsrchr|_tcsrchr|
|strspn|wcsspn|_tcsspn|
|strstr|wcsstr|_tcsstr|
|strtod|wcstod|_tcstod|
|strtok|wcstok|_tcstok|
|strtol|wcstol|_tcstol|
|strtoul|wcstoul|_tcstoul|
|strxfrm|wcsxfrm|_tcsxfrm|
|system|_wsystem|_tsystem|
|tmpnam|_wtmpnam|_ttmpnam|
|tolower|towlower|_totlower|
|toupper|towupper|_totupper|
|ungetc|ungetwc|_ungettc|
|vfprintf|vfwprintf|_vftprintf|
|vprintf|vwprintf|_vtprintf|
|vsprintf|vswprintf|_vstprintf|
|WinMain|wWinMain|_tWinMain|











