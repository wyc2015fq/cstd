#ifndef _ADABOOST_CONFIG_H_
#define _ADABOOST_CONFIG_H_
//#include "console.h"
#define _PRINT_PRO          // ´òÓ¡½ø¶È
#define LUT_BIN (32)
#define X_ESP   (0.00001f)
#define DELIMT    " "
#define _D(_F)   "%"#_F DELIMT
#define _DF()    FLOATFMT DELIMT
#if 0
//typedef float real;
//typedef double real;
#define _F()   "%f" DELIMT
#else
//typedef double real;
#define _F()   "%lf" DELIMT
#endif
typedef unsigned int size_t;


#define INIFILENAME ".\\adaboost.ini"
#define ADABOOSTSECT  "adaboost"

#endif // _ADABOOST_CONFIG_H_
