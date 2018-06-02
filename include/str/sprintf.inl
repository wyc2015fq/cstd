// 自己动手写C语言格式化输出函数
// 分类： C/C++ 2011-12-22 13:32 3331人阅读 评论(6) 收藏 举报
// 语言c编译器bufferwindows数据分析
//
// printf系列函数，包括fprintf、sprintf函数等，其功能是将C语言的所有基本数据类型按用户要求进行格式化输出。
//
// printf函数几乎是所有学习C语言的人接触到的第一个函数，是C语言标准中使用频率最高的函数。
//
// printf函数是C语言标准函数中最著名的可变参数函数，看见printf这个函数名，就想起了C语言的说法一点也不过分，因此，可以说是C语言标准函数中的最具标志性的函数。
//
// printf系列函数。在DOS环境下，这一系列输出函数涵盖了PC机所能用到的所有输出设备，所以printf系列函数也是C语言中最复杂的函数。
//
// 当然，随着DOS时代的结束，不仅printf系列函数的作用减弱了，就连C语言本身也被压缩到了最小的应用领域。
//
// 本文写的sprintfA函数，也是应一个小友要求写的几个函数之一，包括我昨天发布的《自己动手写C语言浮点数转换字符串函数》中的FloatToStr函数，是用来学习用的。之所以取名为sprintfA，不仅是区别系统本身的sprintf函数，同时也因为在Windows下，A表示的是传统的ANSI函数。因为在Windows下，printf系列函数也“与时俱进”了，如wprintf等就是在宽字符环境下的输出函数。由于我在sprintfA函数中使用了Windows的宽字符转换函数，因此该函数只适用于Windows环境。
//
// 由于sprintfA函数代码比较长，将分为多篇文章发布，《自己动手写C语言浮点数转换字符串函数》一文中的代码也应算作一篇：
//
// 一、数据定义：
typedef struct {
  INT type; // 数据长度类型
  INT width; // 数据最小宽度
  INT precision; // 数据精度
  BOOL left; // 是否居左
  BOOL zero; // 是否前导零
  INT decimals; // 浮点数: 1强制小数位; 16进制: -1: 0x, 1: 0X
  INT negative; // 符号：-1: '-'; 1: '+'
  LPSTR param; // 参数指针
} FormatRec;
typedef __int64 LLONG, *PLLONG;
typedef unsigned __int64 ULLONG, *PULLONG;
#define TYPE_CHAR 0
#define TYPE_SHORT 1
#define TYPE_GENERAL 2
#define TYPE_LONG 3
#define TYPE_LLONG 4
#define PTR_SIZE sizeof(VOID*)
#define TypeSize(size) (((size + PTR_SIZE - 1) / PTR_SIZE) * PTR_SIZE)
#define TS_PTR PTR_SIZE
#define TS_CHAR TypeSize(sizeof(CHAR))
#define TS_WCHAR TypeSize(sizeof(WCHAR))
#define TS_SHORT TypeSize(sizeof(SHORT))
#define TS_INT TypeSize(sizeof(INT))
#define TS_LONG TypeSize(sizeof(LONG))
#define TS_LLONG TypeSize(sizeof(LLONG))
#define TS_FLOAT TypeSize(sizeof(FLOAT))
#define TS_DOUBLE TypeSize(sizeof(double))
#define TS_EXTENDED TypeSize(sizeof(EXTENDED))
#define CHAR_SPACE ' '
#define CHAR_ZERO '0'
#define CHAR_POS '+'
#define CHAR_NEG '-'
#define HEX_PREFIX_U "0X"
#define HEX_PREFIX_L "0x"
#define MAX_DIGITS_SIZE 40
//
// FormatRec是一个数据格式化结构，它包含了sprintfA格式化各种数据所需的基本信息。
//
// TYPE_XXXX是数据类型标记，对应于FormatRec.type字段。
//
// TS_XXXX是各种数据类型在sprintfA可变参数传递时所占的栈字节长度。除指针类型和INT类型长度直接用sizeof关键字确定栈字节长度外，其它数据类型所占栈长度则用TypeSize宏配合计算取得，这样就使得这些数据所占栈字节长度在各种环境下都是正确的，比如字符型长度为1字节，TypeSizesizeof(CHAR))，在32位编译环境时等于4，在64位编译环境时则等于8。
//
// 对于带任意类型可变参数的函数来说，实参数据类型的栈字节长度正确与否，完全取决于程序员。比如在sprintfA的格式参数中定义了%Ld，应该是个64位整数类型，而在对应的可变参数部分却给了一个int类型，在32位编译器环境下，就存在2个错误，一是数据类型不正确，二是栈字节长度不匹配，64位整数长度为8字节，而INT的长度却只有4字节，其结果就是这个数据以及其后的所有数据都会出现错误的显示结果，甚至还有可能造成程序崩溃。这也是一些C语言初学者在使用printf系列函数时最容易犯的错误，他们混淆了一般函数与带可变参数函数调用的区别， 对于一般的C函数，形参的数据类型是固定的，在调用时，如果实参与形参数据类型不匹配，编译器视情况会作出错误、警告或者转换等处理，而对于不同精度的相同数据类型，编译器大都会自动进行扩展或截断；而调用带可变参数函数时，由于函数原型的形参说明部分为“...”，编译器就没法将int扩展为_int64了。
//
// 另外，还有有关浮点数部分的数据定义在《自己动手写C语言浮点数转换字符串函数》。
//
// 二、函数主体。
// 获取字符串中的数字。参数：字符串，数字指针。返回字符串中最后一个数字位置
static LPSTR GetControlNum(LPCSTR s, INT* value)
{
  register LPCSTR p = s;
  register INT v;
  for (v = 0; *p >= '0' && *p <= '9'; p ++) {
    v = v * 10 + (*p - '0');
  }
  *value = v;
  return (LPSTR)(p - 1);
}
// sprintfA函数的主体部分就是一个简单的解释器，通过一个主循环，对字符串参数format逐字符的作如下解析：
//
// 1）如果不是数据格式前缀字符'%'，直接拷贝到输出缓冲区buffer；
//
// 2）如果'%'后接着一个'%'字符，则表示要输出后面这个'%'；
//
// 3）紧接着'%'后面的，应该是数据格式前导字符。共有4个前导字符：
//
// 1、'0'：前导零标志。如果数据被格式化后的长度小于规定的格式化宽度，则在被格式化后的数据前补0；
//
// 2、'-'：左对齐标记。
//
// 3、'+'：正数符号输出标记。正数在正常格式输出时，其符号是省略了的，'+'则表示要输出这个符号；
//
// 4、'#'：对浮点数，这是强制小数点（'.'）输出标记。无论这个数有没有小数部分，都必须输出这个小数位符号；对整数的十六进制输出，则是十六进制前缀（0x或者0X）输出标记。
//
// 前导字符不是必须的，也可有多个前导符同时出现在'%'后面，但'0'必须排在最后一个，其余顺序可任意。
//
// 4）解析数据输出宽度和精度。宽度是指数据输出时必须达到的字节数，如果格式化后的数据长度小于宽度，应用空格或者零补齐；精度则是数据要求格式化的长度，视数据类型不同而有所区别，如浮点数是指小数部分的长度，而其它数据则是指全部数据格式化长度，大于精度的数据是保留还是截断，小于精度是忽略还是补齐（零或空格），后面涉及具体数据类型时再说明。
//
// 宽度和精度一般以'.'为分隔符，左边是宽度，右边是精度，如果只有宽度则'.'可忽略。宽度和精度可用固定数字表示，如“10.6”，也可用可变形式“*.*”表示。可变形式的宽度和精度必须在sprintf的可变参数部分有其对应的整数实参。
//
// 宽度和精度部分也不是必须的。
//
// 5）分析数据类型精度字符。在C语言中，相同类型的基本数据可能有不同的精度，如整数有长短之分，浮点数有精度之分，而字符有ANSI和UNICODE之分等等。在sprintfA中，是靠分析类型精度字符来取得的。字符'l'和'h'分别表示长数据和短数据，在16位编译器环境下，一个'l'或'h'就够了，而32位及以上编译器中，随着数据精度的提高，必须靠多个类型精度字符才能表示完整，为此，也可用字符'L'和'H'分别表示数据类型的最大精度和最小精度。sprintfA的数据类型精度分析有较高的容错处理，你可以输入任意多个类型精度字符。
//
// 类型精度字符也不是必须的，缺省情况下，按一般类型精度处理。
//
// 6）解析数据类型字符。数据类型字符的作用有2个，一是确定将要输出的数据类型，如x是整型数，e是浮点数等；二是确定要输出的形式，x是以小写十六进制输出整型数，e则是以指数形式输出浮点数。
//
// 数据类型字符是必须的。数据类型字符解析完毕，各种信息写入FormatRec结构，接着就是具体的各种数据的格式化过程了，其代码将在后面给出。
//
// 7）错误处理。如果在'%'字符后，出现上述各种字符以外的字符，或者上述各种字符排列顺序错误，就需要进行错误处理。printf系列函数的错误处理在不同的编译器中的处理方式是不一样的，主要有2种处理方式：一是忽略本次数据分析，format指针退回到'%'之后，继续循环（'%'后的字符作一般字符处理）；二是不再作分析，直接将'%'后的所有字符输出到buffer后退出函数。本文sprintfA函数采用了后一种处理方式，前一种处理方式在函数主体中也能找到，就是被注释了的语句。
//
// 如果没有错误，则回到1），继续下一数据分析。
//
// 三、格式化字符及字符串。
// 宽字符串转换ANSI字符串。参数：ANSI字符串，宽字符串，转换字符数(0不转换)。
// 返回实际转换字符个数
static INT WStrToStr(LPSTR dst, LPCWSTR src, INT count)
{
  return WideCharToMultiByte(CP_THREAD_ACP, 0, src, -1,
      dst, count > 0 ? count + 1 : 0, NULL, NULL) - 1;
}
// 格式化字符。参数：缓冲区，格式记录。返回缓冲区尾偏移
static LPSTR FormatCharA(LPSTR buffer, FormatRec* rec)
{
  INT len, spaces;
  LPSTR p = buffer;
  if (rec->type == TYPE_LONG) {
    len = WStrToStr(NULL, (LPCWSTR)rec->param, 0);
    if (len == 0) {
      len = sizeof(CHAR);
    }
  }
  else {
    len = sizeof(CHAR);
  }
  spaces = rec->width - len;
  if (rec->left == FALSE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  if (rec->type == TYPE_LONG) {
    WStrToStr(p, (LPCWSTR)rec->param, len);
    p += len;
  }
  else {
    *p ++ = *(LPCSTR)rec->param;
  }
  if (rec->left == TRUE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  rec->param += rec->type == TYPE_LONG ? TS_WCHAR : TS_CHAR;
  return p;
}
// 格式化字符串。参数：缓冲区，格式记录。返回缓冲区尾偏移
static LPSTR FormatStrA(LPSTR buffer, FormatRec* rec)
{
  INT len, spaces;
  LPSTR p = buffer;
  if (rec->type == TYPE_LONG) {
    len = WStrToStr(NULL, *(LPCWSTR*)rec->param, 0);
  }
  else {
    len = lstrlenA(*(LPCSTR*)rec->param);
  }
  if (rec->precision >= 0 && len > rec->precision) {
    len = rec->precision;
  }
  spaces = rec->width - len;
  if (rec->left == FALSE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  if (rec->type == TYPE_LONG) {
    WStrToStr(p, *(LPCWSTR*)rec->param, len);
  }
  else {
    memcpy(p, *(LPCSTR*)rec->param, len);
  }
  p += len;
  if (rec->left == TRUE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  rec->param += TS_PTR;
  return p;
}
//
// 如果不涉及宽字符，格式化字符和字符串是很简单的。
//
// 对于字符和字符串，"%lc"和"%ls"表示宽字符和宽字符串，其它类型精度全部视为默认值，即ANSI字符和ANSI字符串。
//
// 宽字符的转换是由WStrToStr函数来完成的，而WStrToStr又是调用的Windows API函数WideCharToMultiByte，
//
// 在格式化字符0时，C语言的printf和sprintf有所不同，前者是用空格替代的。例如：printf("%s%c456", "123", 0)，显示出来是“123 456"，而sprintf(s, "%s%c456", "123", 0)后，s="123"，因此，sprintfA也就是s="123"。
//
// 四、格式化整型数。
// 格式化数字串。参数：缓冲区，格式记录，数字串，数字串长度。返回缓冲区尾偏移
static LPSTR FormatDigitsA(LPSTR buffer, FormatRec* rec, LPCSTR digits, INT len)
{
  LPSTR p = buffer;
  INT spaces;
  if (rec->precision >= 0) {
    rec->zero = FALSE;
  }
  rec->precision -= len;
  if (rec->precision < 0) {
    rec->precision = 0;
  }
  spaces = rec->width - len - rec->precision;
  if (rec->negative) {
    spaces --;
    if (rec->left || rec->zero) {
      *p ++ = (rec->negative == -1 ? CHAR_NEG : CHAR_POS);
    }
  }
  if (rec->left == FALSE) {
    if (spaces > 0) {
      memset(p, rec->zero ? CHAR_ZERO : CHAR_SPACE, spaces);
      p += spaces;
    }
    if (rec->negative && !rec->zero && !rec->decimals) {
      *p ++ = (rec->negative == -1 ? CHAR_NEG : CHAR_POS);
    }
  }
  if (rec->precision != 0) {
    memset(p, CHAR_ZERO, rec->precision);
    p += rec->precision;
  }
  memcpy(p, digits, len);
  p += len;
  if (rec->left == TRUE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  return p;
}
// 整型数转换为数字串。参数：数字串，整型数，是否无符号整数
static INT IntToDigits(LPSTR digits, LONG src, BOOL* isUnsigned)
{
  ULONG v;
  LPSTR p = digits + MAX_DIGITS_SIZE;
  if (*isUnsigned == FALSE && src < 0) {
    src = -src;
  }
  else {
    *isUnsigned = TRUE;
  }
  v = (ULONG)src;
  do {
    *(-- p) = (CHAR)(v % 10 + '0');
    v /= 10;
  }
  while (v);
  return (INT)(MAX_DIGITS_SIZE - (p - digits));
}
static INT LLongToDigits(LPSTR digits, LLONG src, BOOL* isUnsigned)
{
  ULLONG v;
  LPSTR p = digits + MAX_DIGITS_SIZE;
  if (*isUnsigned == FALSE && src < 0) {
    src = -src;
  }
  else {
    *isUnsigned = TRUE;
  }
  v = (ULLONG)src;
  do {
    *(-- p) = (CHAR)(v % 10 + '0');
    v /= 10;
  }
  while (v);
  return (INT)(MAX_DIGITS_SIZE - (p - digits));
}
static INT numSizes[] = {sizeof(CHAR), sizeof(SHORT), sizeof(INT), sizeof(LONG), sizeof(LLONG)};
// 格式化整型数。参数：缓冲区，格式记录，是否无符号整数。返回缓冲区尾偏移
static LPSTR FormatIntA(LPSTR buffer, FormatRec* rec, BOOL isUnsigned)
{
  ULONG value;
  INT len;
  CHAR digits[MAX_DIGITS_SIZE];
  if (isUnsigned) {
    rec->negative = 0;
  }
  if (numSizes[rec->type] <= TS_PTR) {
    value = *(PULONG)rec->param;
    if (isUnsigned) {
      value &= ((ULONG)(-1) >> ((TS_PTR - numSizes[rec->type]) << 3));
    }
    len = IntToDigits(digits, value, &isUnsigned);
  }
  else {
    len = LLongToDigits(digits, *(PLLONG)rec->param, &isUnsigned);
  }
  if (!isUnsigned) {
    rec->negative = -1;
  }
  rec->param += TypeSize(numSizes[rec->type]);
  rec->decimals = 0;
  return FormatDigitsA(buffer, rec, &digits[MAX_DIGITS_SIZE - len], len);
}
//
// 在C的基本数据中，整型数的表达范围是最“与时俱进”的。16位编译器时，int是2字节，long为4字节；而32编译器下，int和long都变成了4字节，另外多了个8字节的_int64类型；64位编译器下，int仍然是4字节，long成了8字节，是否会有个16字节的_int128？我没用过64位编译器，不知道。代码中定义了一个LLONG类型，并写了2个整型数转换字符串函数，凡是小于或等于指针长度范围的整型数，使用IntToDigits函数，否则使用LLongToDigits函数。从表面看，这2个函数除数据类型?煌猓锞涫且谎模嘁牒螅罢叩乃俣纫臁Ｈ绻切瓷逃玫暮ㄒ榛故鞘褂貌迦牖惚嘟凶唬蛭惚嘀蛔饕桓龀ǎ涂傻牡缴毯陀嗍呒队镅孕枳?个除法。
//
// 有些C语言格式化输出函数在整型数转换时，是忽略hh（或者H）精度的，也就是说整型数转换的最小精度为sizeof(SHORT)，而sprintfA的整型数的最小精度为sizeof(CHAR)。比如"%hhu", -123，前者输出是65413，而后者却是133。如果把代码中numSizes数组的第一个元素改为sizeof(SHORT)，sprintfA也会忽略hh（或者H）精度。
//
// 五、整型数格式化为十六进制和八进制数字串。
static CHAR hexDigitsU[] = "0123456789ABCDEF";
static CHAR hexDigitsL[] = "0123456789abcdef";
// 整型数转换为十六进制串。参数：十六进制串，整型数，字节长度，转换精度，是否大写
static INT NumberToHexA(LPSTR hex, LPCVOID lpNumber, INT bytes, INT precision, BOOL upper)
{
  LPSTR ph = hex;
  LPBYTE pn = (LPBYTE)lpNumber;
  LPSTR hexDigits;
  INT len;
  for (bytes --; bytes > 0 && pn[bytes] == 0; bytes --);
  pn += bytes;
  bytes ++;
  len = bytes * 2;
  if ((*pn & 0xf0) == 0) {
    len --;
  }
  if (hex == NULL) {
    return precision > len ? precision : len;
  }
  for (precision -= len; precision > 0; *ph ++ = '0', precision --);
  hexDigits = upper ? hexDigitsU : hexDigitsL;
  if ((*pn & 0xf0) == 0) {
    *ph ++ = hexDigits[*pn -- & 0x0f];
    bytes --;
  }
  for (; bytes > 0; bytes --, pn --) {
    *ph ++ = hexDigits[*pn >> 4];
    *ph ++ = hexDigits[*pn & 0x0f];
  }
  return (INT)(ph - hex);
}
// 按十六进制格式化整型数。参数：缓冲区，格式记录，类型字符(x or X)
static LPSTR FormatHexA(LPSTR buffer, FormatRec* rec, CHAR hexChar)
{
  LPSTR p = buffer;
  INT spaces, len, pix;
  BOOL upper = hexChar == 'X';
  if (rec->precision >= 0) {
    rec->zero = FALSE;
  }
  pix = rec->decimals ? 2 : 0;
  rec->precision -= pix;
  len = NumberToHexA(NULL, rec->param, numSizes[rec->type], rec->precision, upper);
  spaces = rec->width - len - pix;
  if (rec->decimals && (rec->left || rec->zero)) {
    memcpy(p, rec->decimals > 0 ? HEX_PREFIX_U : HEX_PREFIX_L, 2);
    p += 2;
  }
  if (rec->left == FALSE) {
    if (spaces > 0) {
      memset(p, rec->zero ? CHAR_ZERO : CHAR_SPACE, spaces);
      p += spaces;
    }
    if (rec->decimals && !rec->zero) {
      memcpy(p, rec->decimals > 0 ? HEX_PREFIX_U : HEX_PREFIX_L, 2);
      p += 2;
    }
  }
  p += NumberToHexA(p, rec->param, numSizes[rec->type], rec->precision, upper);
  if (rec->left == TRUE && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  rec->param += TypeSize(numSizes[rec->type]);
  return p;
}
// 整型数转换为八进制串。参数：八进制串，整型数，字节长度
static INT NumberToOtcalA(LPSTR otcal, LPCVOID lpNumber, INT bytes)
{
  LPSTR p = otcal + MAX_DIGITS_SIZE;
  ULLONG v = 0;
  memcpy(&v, lpNumber, bytes);
  do {
    *(-- p) = (CHAR)((v & 7) + '0');
    v >>= 3;
  }
  while (v);
  return (INT)(MAX_DIGITS_SIZE - (p - otcal));
}
// 按八进制格式化整型数。参数：缓冲区，格式记录
static LPSTR FormatOctalA(LPSTR buffer, FormatRec* rec)
{
  CHAR otcal[MAX_DIGITS_SIZE];
  INT len = NumberToOtcalA(otcal, rec->param, numSizes[rec->type]);
  rec->param += TypeSize(numSizes[rec->type]);
  rec->negative = 0;
  return FormatDigitsA(buffer, rec, &otcal[MAX_DIGITS_SIZE - len], len);
}
//
// 整型数转换为十六进制或者八进制数字串，除了进制不同，其它与前面整型数转换为10进制数是一样的。
//
// 六、格式化指针。
// 按十六进制格式化指针。参数：缓冲区，格式记录
static LPSTR FormatPointerA(LPSTR buffer, FormatRec* rec)
{
  INT prec = PTR_SIZE << 1;
  CHAR tmp[PTR_SIZE * 2];
  NumberToHexA(tmp, rec->param, TS_PTR, prec, TRUE);
  rec->precision = -1; // 忽略精度
  return FormatDigitsA(buffer, rec, tmp, prec);
}
//
// 因为指针地址同样也是个整型数，所以指针的格式化和整型数转换为十六进制数字串是一样的，只不过精度是固定的，32位编译器下为8位十六进制数，64位编译器下则为16位十六进制数。
//
// 七、获取缓冲区当前位置字节数。
// 获取缓冲区当前位置字节数。参数：缓冲区，缓冲区首地址，格式记录
static VOID GetPosSizeA(LPSTR buffer, LPSTR buffer0, FormatRec* rec)
{
  LLONG size = buffer - buffer0;
  memcpy((LPVOID) * (PLONG*)rec->param, &size, numSizes[rec->type]);
  rec->param += TS_PTR;
}
//
// 这是格式化输?龊凶钐厥獾氖涑觯皇前涯掣霾问闹凳涑龅交撼迩前咽涑龌撼迩鼻拔恢玫某ざ仁涑龅侥掣霾问飧霾问匦胧侵刚胄问降摹?
//
// 同整型数转换为数字串一样，sprintfA确认的最小数据精度为sizeof(CHAR)，也可以改变为sizeof(SHORT)。
//
// 八、格式化浮点数（有关浮点数的数据定义和底层的数据转换函数见《自己动手写C语言浮点?蛔址芬晃模?
// 转换浮点数信息到浮点数记录fRec。参数：格式记录，格式方式标记，浮点数记录
static void GetFloatRec(FormatRec* rec, INT flag, FloatRec* fRec)
{
  EXTENDED value;
  if (rec->precision < 0) {
    rec->precision = F_DEFDECIMALS;
  }
  else if (rec->precision > F_MAXDECIMALS) {
    rec->precision = F_MAXDECIMALS;
  }
  if (rec->type == TYPE_LLONG) {
    value = *(PEXTENDED)rec->param;
    rec->param += TS_EXTENDED;
  }
  else {
    value = *(double*)rec->param;
    rec->param += TS_DOUBLE;
  }
  switch (flag) {
  case 0: // %f
    FloatResolve(&value, F_MAXPRECISION, rec->precision, fRec);
    break;
  case 1: // %e or %E
    FloatResolve(&value, rec->precision + 1, 9999, fRec);
    break;
  case 2: // %g or %G
    FloatResolve(&value, rec->precision, 9999, fRec);
  }
  if (fRec->negative) {
    rec->negative = -1;
  }
}
// 格式化小数字串。参数：缓冲区，格式记录，数字串，数字串长度。返回缓冲区尾偏移
static LPSTR FormatDecimalA(LPSTR buffer, FormatRec* rec, LPCSTR str, INT strLen)
{
  LPSTR p = buffer;
  INT spaces = rec->width - strLen;
  if (rec->negative) {
    spaces --;
    if (rec->left || rec->zero) {
      *p ++ = (rec->negative == -1 ? CHAR_NEG : CHAR_POS);
    }
  }
  if (rec->left == FALSE) {
    if (spaces > 0) {
      memset(p, rec->zero ? CHAR_ZERO : CHAR_SPACE, spaces);
      p += spaces;
    }
    if (rec->negative && !rec->zero) {
      *p ++ = (rec->negative == -1 ? CHAR_NEG : CHAR_POS);
    }
  }
  memcpy(p, str, strLen);
  p += strLen;
  if (rec->left && spaces > 0) {
    memset(p, CHAR_SPACE, spaces);
    p += spaces;
  }
  return p;
}
#define F_MAXEXPONENT 45
#define F_MINEXPONENT -45
// 输出指数字符串到buffer，返回指数字符串长度
INT PutExponent1(LPSTR buffer, CONST FloatRec* rec)
{
  LPSTR p = buffer;
  INT e, exp = rec->digits[0] ? rec->exponent - 1 : 0;
  *p ++ = rec->negative & 0x80 ? 'E' : 'e';
  if (exp < 0) {
    exp = -exp;
    *p ++ = '-';
  }
  else {
    *p ++ = '+';
  }
  if ((e = (exp / 1000)) != 0) {
    *p ++ = e + 0x30;
    exp %= 1000;
  }
  *p ++ = exp / 100 + 0x30;
  exp %= 100;
  *(PUSHORT)p = (((exp % 10) << 8) | (exp / 10)) + 0x3030;
  return (INT)(p - buffer + 2);
}
// 按浮点数记录信息转换为指数格式数字串。
// 参数：缓冲区，浮点数记录，转换精度，是否强制小数位
static INT FloatExponentA(LPSTR buffer, CONST FloatRec* rec, INT precision, BOOL decPoint)
{
  LPSTR p = buffer;
  LPCSTR digits = rec->digits;
  if (*digits) {
    *p ++ = *digits ++;
  }
  else {
    *p ++ = '0';
  }
  if (precision > 0 || decPoint) {
    for (*p ++ = '.'; precision > 0 && *digits; *p ++ = *digits ++, precision --);
    for (; precision > 0; *p ++ = '0', precision --);
  }
  p += PutExponent(p, rec);
  return (INT)(p - buffer);
}
// 按浮点数记录信息转换为小数格式数字串。
// 参数：缓冲区，浮点数记录，转换精度，是否强制小数位
static INT FloatDecimalA(LPSTR buffer, CONST FloatRec* rec, INT precision, BOOL decPoint)
{
  LPSTR p;
  LPCSTR digits;
  INT exp = rec->exponent;
  if (exp > F_MAXEXPONENT || exp < F_MINEXPONENT) {
    return FloatExponentA(buffer, rec, precision, decPoint);
  }
  p = buffer;
  digits = rec->digits;
  if (exp > 0) {
    for (; exp > 0 && *digits; *p ++ = *digits ++, exp --);
    for (; exp > 0; *p ++ = '0', exp --);
    if (decPoint || precision > 0) {
      *p ++ = '.';
    }
  }
  else {
    exp = -exp;
    precision -= exp;
    if (precision < 0) {
      exp += precision;
      precision = 0;
    }
    *p ++ = '0';
    if (exp > 0 || decPoint || precision > 0) {
      *p ++ = '.';
      for (; exp > 0; *p ++ = '0', exp --);
    }
  }
  for (; precision > 0 && *digits; *p ++ = *digits ++, precision --);
  for (; precision > 0; *p ++ = '0', precision --);
  return (INT)(p - buffer);
}
// 浮点数格式化为小数串。参数：缓冲区，格式记录。返回缓冲区尾偏移
static LPSTR FormatFloatFA(LPSTR buffer, FormatRec* rec)
{
  FloatRec fRec;
  INT len;
  CHAR tmp[F_MAXDECIMALS + 48];
  GetFloatRec(rec, 0, &fRec);
  if (fRec.digits[0] > '9') { // nan or inf
    return FormatDecimalA(buffer, rec, fRec.digits, 3);
  }
  len = FloatDecimalA(tmp, &fRec, rec->precision, rec->decimals);
  return FormatDecimalA(buffer, rec, tmp, len);
}
// 浮点数格式化为指数串。参数：缓冲区，格式记录。返回缓冲区尾偏移
static LPSTR FormatFloatEA(LPSTR buffer, FormatRec* rec, CHAR expChar)
{
  FloatRec fRec;
  INT len;
  CHAR tmp[F_MAXDECIMALS + 8];
  GetFloatRec(rec, 1, &fRec);
  if (fRec.digits[0] > '9') { // nan or inf
    return FormatDecimalA(buffer, rec, fRec.digits, 3);
  }
  if (expChar == 'E') {
    fRec.negative |= 0x80; // 高位置1，大写
  }
  len = FloatExponentA(tmp, &fRec, rec->precision, rec->decimals);
  return FormatDecimalA(buffer, rec, tmp, len);
}
// 浮点数格式化为小数串或者指数串。参数：缓冲区，格式记录。返回缓冲区尾偏移
static LPSTR FormatFloatGA(LPSTR buffer, FormatRec* rec, CHAR expChar)
{
  FloatRec fRec;
  INT len, precision;
  CHAR tmp[F_MAXDECIMALS + 48];
  GetFloatRec(rec, 2, &fRec);
  if (fRec.digits[0] > '9') { // nan or inf
    return FormatDecimalA(buffer, rec, fRec.digits, 3);
  }
  if (expChar == 'G') {
    fRec.negative |= 0x80; // 高位置1，大写
  }
  if (fRec.exponent > rec->precision || fRec.exponent < -3) {
    precision = rec->decimals ? rec->precision - 1 : lstrlenA(fRec.digits) - 1;
    len = FloatExponentA(tmp, &fRec, precision, rec->decimals);
  }
  else {
    precision = rec->decimals ? rec->precision - fRec.exponent : lstrlenA(fRec.digits) - fRec.exponent;
    if (precision < 0) {
      precision = 0;
    }
    len = FloatDecimalA(tmp, &fRec, precision, rec->decimals);
  }
  return FormatDecimalA(buffer, rec, tmp, len);
}
//
// 在sprintfA函数中，浮点数的格式化是最复杂的。浮点数有2种表现形式，即小数形式和指数形式，分别用"%f"和"%e"格式表示，另外还有个"%g"格式，这是个自动格式，即sprintfA通过分析后，自行决定采用哪种形式。
//
// 在以小数形式的格式化中，对数据的格式化有个极限长度，不然，在扩展精度浮点数下，有些浮点数长度可达到近5000位，即使是双精度浮点数，最高长度也达300多。在printf系列函数中，这个极限长度随编译器不同而不同，有的将这个值定为100，有的定为单精度浮点数的最大表现形式，即38等，我在这里把它定为了正负45位，当数据超过这个极限长度，就自动采用指数形式来格式化数据了。
//
// 在介绍sprintfA数据定义时就说过，由于sprintfA的可变参数部分没有参数原型供编译器对照，所以在输入浮点数参数时要注意与格式字符串中对应的浮点数精度匹配，32位编译器的浮点数缺省精度是64位双精度数，即使你给的参数变量是个单精度数，也会扩展为双精度数，如果参数变量是long double，而你使用的编译器支持80位扩展精度浮点数时，传递的是80位扩展精度数，否则也是双精度数，如果你给出一个整数，编译器是不会自动转换的。如果你在参数位置输入的是常数就更应该注意了，123，123L，123.0f，123.0，123.0L这几种常数形式是不同的（L也可是小写），分别是整数，长整数，单精度浮点数，双精度浮点数，扩展精度浮点数（如果编译器不支持，也是双精度数）。所以，在32位及以上编译器中格式%f和%lf是等同的，自然，在不支持扩展精度浮点数的编译器中，%llf（%Lf）也等同于%f。
//
// 本文格式化浮点数时用到的FloatResolve函数以及有关数据定义见《自己动手写C语言浮点数转换字符串函数》。
//
// 有关sprintfA函数的介绍就全部完毕。文章代码没进行严格的测试。
LPSTR _sprintfA(LPSTR buffer, LPCSTR format, ...)
{
  FormatRec rec;
  BOOL flag;
  CHAR c;
  LPCSTR psave; // ?
  register LPCSTR pf = format;
  register LPSTR pb = buffer;
  va_list paramList;
  va_start(paramList, format);
  rec.param = (LPSTR)paramList;
  while (TRUE) {
    while (*pf && *pf != '%') {
      *pb ++ = *pf ++;
    }
    if (*pf == 0) {
      break;
    }
    if (*(pf + 1) == '%') { // 处理%%
      *pb ++ = '%';
      pf += 2;
      continue;
    }
    psave = pf; // ?
    rec.width = rec.decimals = rec.negative = 0;
    rec.left = rec.zero = FALSE;
    rec.type = TYPE_GENERAL;
    rec.precision = -1;
    // 解析前导符号
    flag = TRUE;
    while (flag) {
      pf ++;
      switch (*pf) {
      case '0':
        rec.zero = TRUE;
        flag = FALSE;
        break;
      case '-':
        rec.left = TRUE;
        break;
      case '+':
        rec.negative = 1;
        break;
      case '#':
        rec.decimals = 1;
        break;
      default:
        pf --;
        flag = FALSE;
        break;
      }
    }
    // 解析输出宽度和精度
    flag = TRUE;
    while (flag) {
      pf ++;
      switch (*pf) {
      case '.': // 如小数点后为'*','0' - '9'继续处理精度和宽度
        rec.precision = 0;
        c = *(pf + 1);
        flag = (c == '*' || (c >= '0' && c <= '9'));
        break;
      case '*': // 处理'*'表示的宽度参数和精度参数
        if (*(pf - 1) == '.') {
          rec.precision = *(PINT)rec.param;
          flag = FALSE;
        }
        else {
          rec.width = *(PINT)rec.param;
          flag = *(pf + 1) == '.';
        }
        rec.param += TS_PTR;
        break;
      default: // 处理格式串中数字表示的宽度和精度
        if (*(pf - 1) == '.') {
          pf = GetControlNum(pf, &rec.precision);
          flag = FALSE;
        }
        else {
          pf = GetControlNum(pf, &rec.width);
          flag = *(pf + 1) == '.';
        }
      }
    }
    // 解析数据类型精度
    flag = TRUE;
    while (flag) {
      pf ++;
      switch (*pf) {
      case 'L':
        rec.type = TYPE_LLONG;
        break;
      case 'l':
        if (rec.type < TYPE_LLONG) {
          rec.type ++;
        }
        break;
      case 'H':
        rec.type = TYPE_CHAR;
        break;
      case 'h':
        if (rec.type > TYPE_CHAR) {
          rec.type --;
        }
        break;
      default:
        flag = FALSE;
      }
    }
    // 解析数据类型，并格式化
    c = *pf ++;
    switch (c) {
    case 's':
      pb = FormatStrA(pb, &rec);
      break;
    case 'c':
      pb = FormatCharA(pb, &rec);
      break;
    case 'd':
    case 'i':
    case 'u':
      pb = FormatIntA(pb, &rec, c == 'u');
      break;
    case 'f':
      pb = FormatFloatFA(pb, &rec);
      break;
    case 'e':
    case 'E':
      pb = FormatFloatEA(pb, &rec, c);
      break;
    case 'g':
    case 'G':
      pb = FormatFloatGA(pb, &rec, c);
      break;
    case 'x':
      if (rec.decimals) {
        rec.decimals = -1;
      }
    case 'X':
      pb = FormatHexA(pb, &rec, c);
      break;
    case 'o':
      pb = FormatOctalA(pb, &rec);
      break;
    case 'p':
      pb = FormatPointerA(pb, &rec);
      break;
    case 'n':
      GetPosSizeA(pb, buffer, &rec);
      break;
    default: // 错误：拷贝format剩余字符，返回
      // pf = psave + 1; // ? 也可处理为忽略后继续
      // break; // ?
      lstrcpyA(pb, psave);
      return buffer;
    }
  }
  va_end(paramList);
  *pb = 0;
  return buffer;
}

