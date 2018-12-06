
#ifndef _ERROR_C_H_
#define _ERROR_C_H_

/* Fast search data structures  */
struct CvFeatureTree;
struct CvLSH;
struct CvLSHOperations;
#include <stdio.h>
#ifdef __GNUC__
#define CC_Error( code, err ) printf("%s:%d: error: (%d) %s in function %s\n", __FILE__, __LINE__, code, err, __func__)
#else
#define CC_Error( code, err ) printf("%s:%d: error: (%d) %s\n", __FILE__, __LINE__, code, err)
#endif
#define CC_Error_( code, args ) CC_Error(code, _format args)
#define CC_Assert( expr ) { if(!(expr)) CC_Error(CC_StsAssert, #expr); }
#define CC_StaticAssert(condition, reason)    {int _a[condition]={0}; const char* _s=reason;}
#ifdef _DEBUG
#define CC_DbgAssert(expr) CC_Assert(expr)
#else
#define CC_DbgAssert(expr)
#endif


#endif // _ERROR_C_H_
