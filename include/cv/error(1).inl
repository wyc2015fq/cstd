
#ifndef _CXCORE_ERROR_H_
#define _CXCORE_ERROR_H_

/************Below is declaration of error handling stuff in PLSuite manner**/

typedef int CVStatus;

/* this part of CVStatus is compatible with IPLStatus 
  Some of below symbols are not [yet] used in OpenCV
*/
#define CC_StsOk                    0  /* everithing is ok                */
#define CC_StsBackTrace            -1  /* pseudo error for back trace     */
#define CC_StsError                -2  /* unknown /unspecified error      */
#define CC_StsInternal             -3  /* internal error (bad state)      */
#define CC_StsNoMem                -4  /* insufficient memory             */
#define CC_StsBadArg               -5  /* function arg/param is bad       */
#define CC_StsBadFunc              -6  /* unsupported function            */
#define CC_StsNoConv               -7  /* iter. didn't converge           */
#define CC_StsAutoTrace            -8  /* tracing                         */

#define CC_HeaderIsNull            -9  /* image header is NULL            */
#define CC_BadImageSize            -10 /* image size is invalid           */
#define CC_BadOffset               -11 /* offset is invalid               */
#define CC_BadDataPtr              -12 /**/
#define CC_BadStep                 -13 /**/
#define CC_BadModelOrChSeq         -14 /**/
#define CC_BadNumChannels          -15 /**/
#define CC_BadNumChannel1U         -16 /**/
#define CC_BadDepth                -17 /**/
#define CC_BadAlphaChannel         -18 /**/
#define CC_BadOrder                -19 /**/
#define CC_BadOrigin               -20 /**/
#define CC_BadAlign                -21 /**/
#define CC_BadCallBack             -22 /**/
#define CC_BadTileSize             -23 /**/
#define CC_BadCOI                  -24 /**/
#define CC_BadROISize              -25 /**/

#define CC_MaskIsTiled             -26 /**/

#define CC_StsNullPtr                -27 /* null pointer */
#define CC_StsVecLengthErr           -28 /* incorrect vector length */
#define CC_StsFilterStructContentErr -29 /* incorr. filter structure content */
#define CC_StsKernelStructContentErr -30 /* incorr. transform kernel content */
#define CC_StsFilterOffsetErr        -31 /* incorrect filter ofset value */

/*extra for CV */
#define CC_StsBadSize                -201 /* the input/output structure size is incorrect  */
#define CC_StsDivByZero              -202 /* division by zero */
#define CC_StsInplaceNotSupported    -203 /* in-place operation is not supported */
#define CC_StsObjectNotFound         -204 /* request can't be completed */
#define CC_StsUnmatchedFormats       -205 /* formats of input/output arrays differ */
#define CC_StsBadFlag                -206 /* flag is wrong or not supported */  
#define CC_StsBadPoint               -207 /* bad CPoint */ 
#define CC_StsBadMask                -208 /* bad format of mask (neither 8uC1 nor 8sC1)*/
#define CC_StsUnmatchedSizes         -209 /* sizes of input/output structures do not match */
#define CC_StsUnsupportedFormat      -210 /* the data format/type is not supported by the function*/
#define CC_StsOutOfRange             -211 /* some of parameters are out of range */
#define CC_StsParseError             -212 /* invalid syntax/structure of the parsed file */
#define CC_StsNotImplemented         -213 /* the requested function/feature is not implemented */
#define CC_StsBadMemBlock            -214 /* an allocated block has been corrupted */

/********************************* Error handling Macros ********************************/

#define OPENCC_ERROR(status,func,context)                           \
                cvError((status),(func),(context),__FILE__,__LINE__)

#define OPENCC_ERRCHK(func,context)                                 \
                {if (cvGetErrStatus() >= 0)                         \
                 {OPENCC_ERROR(CC_StsBackTrace,(func),(context));}}

#define OPENCC_ASSERT(expr,func,context)                            \
                {if (! (expr))                                      \
                 {OPENCC_ERROR(CC_StsInternal,(func),(context));}}

#define OPENCC_RSTERR() (cvSetErrStatus(CC_StsOk))

#define OPENCC_CALL(Func)                                         \
{                                                                   \
    Func;                                                           \
} 


/**************************** OpenCV-style error handling *******************************/

/* CC_FUNCNAME macro defines icvFuncName constant which is used by CC_ERROR macro */
#ifdef CC_NO_FUNC_NAMES
    #define CC_FUNCNAME(Name)
    #define cvFuncName ""
#else    
    #define CC_FUNCNAME(Name)  \
    static char cvFuncName[] = Name
#endif


/*
  CC_ERROR macro unconditionally raises error with passed code and message.
  After raising error, control will be transferred to the exit label.
*/
#define CC_ERROR(Code, Msg)                                       \
{                                                                   \
     cvError((Code), cvFuncName, Msg, __FILE__, __LINE__);        \
     EXIT;                                                          \
}

/* Simplified form of CC_ERROR */
#define CC_ERROR_FROM_CODE(code)   \
    CC_ERROR(code, "")

/*
 CC_CHECK macro checks error status after CV (or IPL)
 function call. If error detected, control will be transferred to the exit
 label.
*/
#define CC_CHECK()                                                  \
{                                                                   \
    if(cvGetErrStatus() < 0)                                      \
        CC_ERROR(CC_StsBackTrace, "Inner function failed.");      \
}


/*
 CC_CALL macro calls CV (or IPL) function, checks error status and
 signals a error if the function failed. Useful in "parent node"
 error procesing mode
*/
#define CC_CALL(Func)                                             \
{                                                                   \
    Func;                                                           \
    CC_CHECK();                                                     \
}


/* Runtime assertion macro */
#define CC_ASSERT(Condition)                                          \
{                                                                       \
    if(!(Condition))                                                  \
        CC_ERROR(CC_StsInternal, "Assertion: " #Condition " failed"); \
}

#define __BEGIN__       {
#define __END__         goto exit; exit: ; }
#define __CLEANUP__
#define EXIT            goto exit

#endif /* _CXCORE_ERROR_H_ */


