
#ifndef _EXPRESSION_INL_
#define _EXPRESSION_INL_
#include <stdio.h>
//#include <tchar.h>
#include "macro.h"
#include "cstd.h"
#include "img/filters.h"
#include "macro.h"
#include "inifile.h"


#include "imresize.h"
#include "expression.h"
#include "img/Subdiv.h"
#include "fmtimg/fmtimg.inl"
#include "draw/imdraw.inl"
#include "asmfit.inl"
#include "wrap.inl"
#include "img\imeffect.inl"

// "tchar.h"
#undef _TT
#undef TTCHAR
#undef IniGetString
#define _TT(x)      x
#define TTCHAR char
#define TEFFECT EFFECTA
#define new_expressionT new_expressionA
#define del_expressionT del_expressionA
#define config_expressionT config_expressionA
#define add_expressionT add_expressionA
#define _sntprintfT  _snprintf
#define new_readerT  new_readerA
#define TIniGetInt(filename, lpAppName, lpKeyName, nDefault) \
  GetPrivateProfileIntA(lpAppName, lpKeyName, nDefault, filename)
#define IniGetString(filename, lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize) \
  GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, filename) \

#include "expression_char.h"

#undef _TT
#undef TTCHAR
#undef TEFFECT
#undef new_expressionT
#undef del_expressionT
#undef config_expressionT
#undef add_expressionT
#undef _sntprintfT
#undef TIniGetInt
#undef IniGetString
#undef new_readerT

#define _TT(x)      L##x
#define TTCHAR wchar_t
#define TEFFECT EFFECTW
#define new_expressionT new_expressionW
#define del_expressionT del_expressionW
#define config_expressionT config_expressionW
#define add_expressionT add_expressionW
#define _sntprintfT  _snwprintf
#define new_readerT  new_readerW
#define TIniGetInt(filename, lpAppName, lpKeyName, nDefault) \
  GetPrivateProfileIntW(lpAppName, lpKeyName, nDefault, filename)

#define IniGetString(filename, lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize) \
  GetPrivateProfileStringW(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, filename) \

#include "expression_char.h"

#undef _TT
#undef TCHAR
#undef TEFFECT
#undef new_expressionT
#undef del_expressionT
#undef config_expressionT
#undef add_expressionT
#undef _sntprintfT
#undef TIniGetInt
#undef IniGetString
#undef new_readerT



#endif // _EXPRESSION_INL_
