# xlslib生成excel文件 - 深之JohnChen的专栏 - CSDN博客

2018年10月29日 14:47:04[byxdaz](https://me.csdn.net/byxdaz)阅读数：689


一、xlslib介绍

C++用来操作Excel的方法很多，但是涉及到跨平台，同时又要对Excel的读写操作兼顾，而且免费的库，那应该是要用xlslib和libxls了。xlslib库只能写Excel文件，libxls库只能读Excel文件。另外libxl库功能强大，但是收费的。

xlslib

官网:[http://xlslib.sourceforge.net](http://xlslib.sourceforge.net/)

下载:[http://sourceforge.net/projects/xlslib](http://sourceforge.net/projects/xlslib)

libxls

下载:[http://sourceforge.net/projects/libxls/](http://sourceforge.net/projects/libxls/)

libxl

官网:[http://www.libxl.com/](http://www.libxl.com/)

 二、工程加载

 1、加载xlslib_lib和xlslib_dll工程时，提示下面错误

 xlslib-package-2.5.0\xlslib\xlslib\build\msvc2012\xlslib_dll.vcxproj : error  : 筛选器“Source Files”下已存在项目“..\..\src\xlslib\formula.cpp”。

 解决方法：

 用记事本打开编辑xlslib_lib.vcxproj和xlslib_dll.vcxproj

 删除formula.cpp一行，因为有两行只保留一行就行。

 重新打开xlslib.sln工程没有错误。

 三、工程编译

 createDLL工程编译，此工程用于将xlslib.lib文件转换成xlslib.dll文件。

 编译createDLL，提示下面错误

 error C2338: <hash_map> is deprecated and will be REMOVED. Please use <unordered_map>. You can define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS to acknowledge that you have received this warning.

 解决方法：在工程属性中增加宏定义_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

 xlslib_lib和xlslib_dll工程编译

 1 “sheet_notes”前的类型(构造函数有返回类型或是当前类型名称的非法重定义?)

解决

src/xlslib/note.h

44-47行

struct sheet_notes

{

 uint16_t sheet_idx

uint16_t sheet_notes;

}

成员sheet_notes和结构名一样冲突了

修改成员名unit16_t sheet_notes为uint16_t sheet_notes__只要不冲突就行。

后续用到这个变量的地方也修改，这个编译的时候会提示的。

note.cpp(342) last_sheet.sheet_notes__

note.cpp(355) sn.sheet_notes__

note.cpp(358) sn.sheet_notes__

2 formula.cpp(917) error C2059 语法错误"{"

解决

将function_propertyp = (function_property) {CELLOP_AS_VALUE, A_UNKNOWN};

修改为

function_property p ={CELLOP_AS_VALUE, A_UNKNOWN};

去掉 (function_property)

3测试的例子也有错误

targets\test\md5.c(464): error C2143: 语法错误 : 缺少“;”(在“类型”的前面)

3>..\..\targets\test\md5.c(467): error C2065: “bload”: 未声明的标识符

这个是.c语法 后面申明了 int bload

将Int bload提到该函数的最前面一行

再次编译 提示错误

char const * __cdecl check_file(char const *,char const *)

这个是由于c文件导出函数接口不同

在Target/test/md5.h

将

#if defined(__cplusplus)

extern "C" 

{

#endif

#if defined(__cplusplus)

}

#endif

注释取消掉

编译xlslib_dll工程时，提示error LNK1104: 无法打开文件“gdi32.lib”。

此类错误解决方法参考这篇文章https://www.cnblogs.com/AkazaAkari/p/6145144.html

右键项目->属性->配置属性->VC++目录 要修改两个值：

包含目录

库目录

点击右侧的倒三角->点击编辑->双击上方的空白编辑区，

包含目录就追加    C:\Program Files\Microsoft SDKs\Windows\v7.1A;$(IncludePath)

库目录追加     C:\Program Files\Microsoft SDKs\Windows\v7.1A\Lib;$(LibraryPath)

（注意，因为版本不同，文件夹不一定是v7.1A，主要看这个目录下的lib目录当中有没有我们报错中提到的库）比如win10系统下lib和include路径

包含目录

C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\um

C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\shared

C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\winrt

库目录

C:\Program Files (x86)\Windows Kits\10\Lib\10.0.14393.0\um\x86

四、使用实例

1、生成xls

```
#include <string.h>
#include <xlslib/xlslib.h>

using namespace xlslib_core;
using namespace std;

int main (int argc, char *argv[]) {
    workbook wb;
    xf_t* xf = wb.xformat();
    worksheet* ws;
    ws = wb.sheet("sheet1");
    string label = "Hello, World!";
    ws->label(1,2,label,xf);    // 从0开始数，第1行，第2列，即C3
    wb.Dump("workbook.xls");
    return 0;
}
```

2、设置颜色

```
#include <string.h>
#include <xlslib/xlslib.h>

using namespace xlslib_core;
using namespace std;

int main (int argc, char *argv[]) {
    workbook wb; 
    font_t * _font = wb.font("Calibri");
    _font->SetBoldStyle(BOLDNESS_BOLD);  // 设置粗字体
    xf_t* xf = wb.xformat();   
    xf->SetFont(_font);
    xf->SetFillBGColor(CLR_WHITE);
    xf->SetFillFGColor(CLR_RED);
    
    worksheet* ws;    
    ws = wb.sheet("sheet1");   
    cell_t * cell;    
    cell = ws->label(1,2,"hello",xf);    // 从0开始数，第1行，第2列，即C3   
    cell = ws->label(2,2,"world");    
    cell->fillfgcolor(CLR_RED);
    cell->fillbgcolor(CLR_WHITE);
    
    range * _range;    
    _range = ws->rangegroup(0,0,1500,100);   // 设置背景为白色
    _range->cellcolor(CLR_WHITE);    
    _range = ws->rangegroup(1,2,2,2);
    _range->cellcolor(CLR_GOLD);       
    wb.Dump("workbook.xls");
    return 0;
}
```

3、设置列宽

```
#include <string.h>
#include <xlslib/xlslib.h>

using namespace xlslib_core;
using namespace std;

int main (int argc, char *argv[]) {
    workbook wb;
    worksheet* ws;
    
    ws = wb.sheet("sheet1");
    ws->defaultColwidth(256*10);
    ws->colwidth(2,256*30);
    
    cell_t * cell; 
    cell = ws->label(1,2,"hello",xf);    // 从0开始数，第1行，第2列，即C3
    cell = ws->label(2,2,"world");   
    wb.Dump("workbook.xls");
    return 0;
}
```

4、输入中文

```
int main (int argc, char *argv[]) {
	 workbook wb;
   worksheet* sh = wb.sheet("sheet");
   sh->label(1,1,"test1");
   sh->label(2,1,L"test3中文");//这里中文使用unicode编码
   wb.Dump("workbook.xls");
   return 0;
}
```

五、注意事项

1、用createDLL生成dll文件时，需要用管理员权限打开vs编译。

2、输入字符串中包含有中文，请使用unicode编码，否则就会出现乱码。

3、低于2.5.0版本的xlslib库，写中文字符串时可以使用unicode编码方式写入中文字符串，也可以使用utf-8方式写入中文字符串。但utf-8方式写入中文字符串，需要修改源代码。

```
/*
* 在xlslib/src/sheetrec.h中增加如下代码
*/
cell_t* label(int code, unsigned16_t row, unsigned16_t col, const std::ustring& strlabel, xf_t* pxformat = NULL);
cell_t* label(int code, unsigned16_t row, unsigned16_t col, const char* strlabel, xf_t* pxformat = NULL);

/*
* 在xlslib/src/sheetrec.cpp中增加如下代码
*/
cell_t* worksheet::label(int code, unsigned16_t row, unsigned16_t col, 
                         const ustring& strlabel, xf_t* pxformat)
{
        enum { UTF8, GBK };
        u16string str16;
        label_t* lbl;
        u16string::const_iterator u16begin, u16end;
        ustring::const_iterator ubegin, uend;
        size_t len;
        
        if (code == UTF8) {
                len = strlabel.length();
                str16.reserve(len);
                ubegin = strlabel.begin();
                uend = strlabel.end();
        
                while(ubegin != uend) {
                        unichar_t c;
                        c = *ubegin++;
                        str16.push_back(c);        
                }
                lbl = new label_t(m_GlobalRecords, row, col, str16, pxformat);
                AddCell((cell_t*)lbl);

                return (cell_t*)lbl;
        } else {
                return NULL;
        }
}
cell_t* worksheet::label(int code, unsigned16_t row, unsigned16_t col, 
                         const char* strlabel, xf_t* pxformat)
{
        enum { UTF8, GBK };
        unsigned16_t u16;
        u16string str16;
        label_t* lbl;
        wstring::const_iterator wbegin, wend;
        size_t len;
        
        if (code == UTF8) {
                if (strlabel == NULL) {
                        return NULL;
                } else {
                        len = strlen(strlabel);
                        wchar_t wcs[len+1];
                        mbstowcs(wcs, strlabel, len+1);
                        len = wcslen(wcs);
                        for (int i = 0; i < len; i++) {
                                u16 = wcs[i];
                                str16.push_back(u16);
                        }
                }

                lbl = new label_t(m_GlobalRecords, row, col, str16, pxformat);
                AddCell((cell_t*)lbl);

                return (cell_t*)lbl;
        } else {
                return NULL;
        }
}
```

```
实例：
#include <locale.h>
#include "ocilib.h"
#include "xlslib.h"
using namespace xlslib_core;

#define RECORDCOUNT                65536

int main(int argc, char *argv[])
{
        setlocale(LC_ALL, "zh_CN.utf-8");

        enum { UTF8, GBK };
        workbook wb1, wb2;
        worksheet* wb1sh1 = wb1.sheet("Sheet1");
        worksheet* wb2sh1 = wb2.sheet("Sheet1");
        int row = 0, coloumn = 0;

        if (argc != 4) {
                printf("Example: ./demo SID USER PASSWORD/n");
                return 0;
        }

        OCI_Connection* cn;
    OCI_Statement* st;
    OCI_Resultset* rs;

    OCI_Initialize(NULL, NULL, OCI_ENV_DEFAULT);
    cn = OCI_ConnectionCreate(argv[1], argv[2], argv[3], OCI_SESSION_DEFAULT);
    st = OCI_StatementCreate(cn);
    OCI_ExecuteStmt(st, "select * from t_test_a");

    rs = OCI_GetResultset(st);
    while (OCI_FetchNext(rs))
    {
                if (row < RECORDCOUNT) {
                        wb1sh1->label(UTF8, row, coloumn,        OCI_GetString(rs, 1));
                        wb1sh1->label(UTF8, row, coloumn+1,        OCI_GetString(rs, 5));
                        wb1sh1->label(UTF8, row, coloumn+2,        OCI_GetString(rs, 6));
                } else {
                        wb2sh1->label(UTF8, row-RECORDCOUNT, coloumn,        OCI_GetString(rs, 1));
                        wb2sh1->label(UTF8, row-RECORDCOUNT, coloumn+1, OCI_GetString(rs, 5));
                        wb2sh1->label(UTF8, row-RECORDCOUNT, coloumn+2, OCI_GetString(rs, 6));
                }
                printf("/r%d", row+1);
                fflush(stdout);
                row++;
    }
        printf("/n");

        wb1.Dump("./demo1.xls");
        wb2.Dump("./demo2.xls");
    OCI_Cleanup();

    return EXIT_SUCCESS;
}
```

xlslib-2.5.0 vs2015版本下载地址[https://download.csdn.net/download/byxdaz/10750846](https://download.csdn.net/download/byxdaz/10750846)

