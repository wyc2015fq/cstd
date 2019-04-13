
# CreateFont函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月08日 10:44:25[seven-soft](https://me.csdn.net/softn)阅读数：579


文字的出现历史可以追溯到甲骨文的使用，直到今天使用的宋体文字。在软件开发里，经常遇到是跨国语言的使用，由于世界在变平，全球在变小，交通运输非常发达，由我所在的深圳，向东坐飞机12个小时就可以到达伦敦，向西坐飞机12小时就可以到达美国，可算得上朝发夕至。比如像炒外汇的人，一天可以不用睡觉都在工作着，比如早上炒东京的汇市，下午就可以炒伦敦的，晚上就可以炒纽约的了。软件的开发，也在全球化，比如昨晚在美国开发，早上就可以变成中国开发同样的软件了，一天24小时开发，这样加速软件的开发。全球的市场已经变得同步化了，开发的软件可以适应任何有人类的地方。经常开发的软件，就需要有中英双语化。这样就需要使用到不同的字体，才能适应国际化的需要，下面就来学习怎么样创建字体，并且使用它。
函数CreateFont声明如下：
WINGDIAPI HFONT   WINAPI CreateFontA( __in int cHeight, __in int cWidth, __in int cEscapement, __in int cOrientation, __in int cWeight, __in DWORD bItalic,
__in DWORD bUnderline, __in DWORD bStrikeOut, __in DWORD iCharSet, __in DWORD iOutPrecision, __in DWORD iClipPrecision,
__in DWORD iQuality, __in DWORD iPitchAndFamily, __in_opt LPCSTR pszFaceName);
WINGDIAPI HFONT   WINAPI CreateFontW( __in int cHeight, __in int cWidth, __in int cEscapement, __in int cOrientation, __in int cWeight, __in DWORD bItalic,
__in DWORD bUnderline, __in DWORD bStrikeOut, __in DWORD iCharSet, __in DWORD iOutPrecision, __in DWORD iClipPrecision,
__in DWORD iQuality, __in DWORD iPitchAndFamily, __in_opt LPCWSTR pszFaceName);
\#ifdef UNICODE
\#define CreateFont CreateFontW
\#else
\#define CreateFont CreateFontA
\#endif // !UNICODE
**cHeight**是字体的高度。
**cWidth**是字体的宽度。
**cEscapement**是字体的倾斜角。
**cOrientation**是字体的倾斜角。
**cWeight**是字体的粗细。
**bItalic**是字体是否斜体。
**bUnderline**是字体是否有下划线。
**bStrikeOut**是字体是否有删除线。
**iCharSet**是字体使用的字符集。
**iOutPrecision**是指定如何选择合适的字体。
**iClipPrecision**是用来确定裁剪的精度。
**iQuality**是怎么样跟选择的字体相符合。
**iPitchAndFamily**是间距标志和属性标志。
**pszFaceName**是字体的名称。

调用这个函数的例子如下：
\#001 //创建字体.
\#002 //
\#003 //
\#004 //
\#005 HFONT CCaiWinMsg::GetFont(void)
\#006 {
\#007  LOGFONT lf; //字符的结构
\#008
\#009  //获取当前系统的字体.
\#010  GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT),
\#011         &lf);
\#012
\#013  //设置字体的属性.
\#014  lf.lfWeight = FW_BOLD;
\#015  lf.lfItalic = true;
\#016  lf.lfHeight = 26;
\#017
\#018  //设置为宋体.
\#019  wsprintf(lf.lfFaceName,_T("%s"),_T("宋体"));
\#020
\#021  //创建字体并返回
\#022  return CreateFont(lf.lfHeight, lf.lfWidth,
\#023         lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
\#024         lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
\#025         lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
\#026         lf.lfPitchAndFamily, lf.lfFaceName);
\#027
\#028 }


