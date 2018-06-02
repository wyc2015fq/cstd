/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//            Intel License Agreement
//        For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "_cxcore.h"

#if defined WIN32 || defined WIN64
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef struct
{
    const char* file;
    int         line;
}
CvStackRecord;

typedef struct CvContext
{
    int  err_code;
    int  err_mode;
    CvErrorCallback error_callback;
    void*  userdata;
    char  err_msg[4096];
    CvStackRecord  err_ctx;
} CvContext;

#if defined WIN32 || defined WIN64
#define CC_DEFAULT_ERROR_CALLBACK  cvGuiBoxReport
#else
#define CC_DEFAULT_ERROR_CALLBACK  cvStdErrReport
#endif

static CvContext*
icvCreateContext(void)
{
    CvContext* context = (CvContext*)malloc(sizeof(*context));

    context->err_mode = CC_ErrModeLeaf;
    context->err_code = CC_StsOk;

    context->error_callback = CC_DEFAULT_ERROR_CALLBACK;
    context->userdata = 0;

    return context;
}

static void
icvDestroyContext(CvContext* context)
{
    free(context);
}

#if defined WIN32 || defined WIN64
    static DWORD g_TlsIndex = TLS_OUT_OF_INDEXES;
#else
    static pthread_key_t g_TlsIndex;
#endif

static CvContext*
icvGetContext(void)
{
#ifdef CC_DLL
#if defined WIN32 || defined WIN64
    CvContext* context;

    //assert(g_TlsIndex != TLS_OUT_OF_INDEXES);
    if(g_TlsIndex == TLS_OUT_OF_INDEXES)
    {
        g_TlsIndex = TlsAlloc();
        if(g_TlsIndex == TLS_OUT_OF_INDEXES)
            FatalAppExit(0, "Only set CC_DLL for DLL usage");
    }

    context = (CvContext*)TlsGetValue(g_TlsIndex);
    if(!context)
    {
        context = icvCreateContext();
        if(!context)
            FatalAppExit(0, "OpenCV. Problem to allocate memory for TLS OpenCV context.");

        TlsSetValue(g_TlsIndex, context);
    }
    return context;
#else
    CvContext* context = (CvContext*)pthread_getspecific(g_TlsIndex);
    if(!context)
    {
    context = icvCreateContext();
    if(!context)
    {
            fprintf(stderr,"OpenCV. Problem to allocate memory for OpenCV context.");
        exit(1);
    }
    pthread_setspecific(g_TlsIndex, context);
    }
    return context;
#endif
#else /* static single-thread library case */
    static CvContext* context = 0;

    if(!context)
        context = icvCreateContext();

    return context;
#endif
}


CC_IMPL int
cvStdErrReport(int code, const char *func_name, const char *err_msg,
                const char *file, int line, void*)
{
    if(code == CC_StsBackTrace || code == CC_StsAutoTrace)
        fprintf(stderr, "\tcalled from ");
    else
        fprintf(stderr, "OpenCV ERROR: %s (%s)\n\tin function ",
                 cvErrorStr(code), err_msg ? err_msg : "no description");

    fprintf(stderr, "%s, %s(%d)\n", func_name ? func_name : "<unknown>",
             file != NULL ? file : "", line);

    if(cvGetErrMode() == CC_ErrModeLeaf)
    {
        fprintf(stderr, "Terminating the application...\n");
        return 1;
    }
    else
        return 0;
}


CC_IMPL int
cvGuiBoxReport(int code, const char *func_name, const char *err_msg,
                const char *file, int line, void*)
{
#if !defined WIN32 && !defined WIN64
    return cvStdErrReport(code, func_name, err_msg, file, line, 0);
#else
    if(code != CC_StsBackTrace && code != CC_StsAutoTrace)
    {
        size_t msg_len = strlen(err_msg ? err_msg : "") + 1024;
        char* message = (char*)alloca(msg_len);
        char title[100];

        wsprintf(message, "%s (%s)\nin function %s, %s(%d)\n\n"
                  "Press \"Abort\" to terminate application.\n"
                  "Press \"Retry\" to debug (if the app is running under debugger).\n"
                  "Press \"Ignore\" to continue (this is not safe).\n",
                  cvErrorStr(code), err_msg ? err_msg : "no description",
                  func_name, file, line);

        wsprintf(title, "OpenCV GUI Error Handler");

        int answer = MessageBox(NULL, message, title, MB_ICONERROR|MB_ABORTRETRYIGNORE|MB_SYSTEMMODAL);

        if(answer == IDRETRY)
        {
            CC_DBG_BREAK();
        }
        return answer != IDIGNORE;
    }
    return 0;
#endif
}


CC_IMPL int cvNulDevReport(int /*code*/, const char* /*func_name*/,
    const char* /*err_msg*/, const char* /*file*/, int /*line*/, void*)
{
    return cvGetErrMode() == CC_ErrModeLeaf;
}


CC_IMPL CvErrorCallback
cvRedirectError(CvErrorCallback func, void* userdata, void** prev_userdata)
{
    CvContext* context = icvGetContext();

    CvErrorCallback old = context->error_callback;
    if(prev_userdata)
        *prev_userdata = context->userdata;
    if(func)
    {
        context->error_callback = func;
        context->userdata = userdata;
    }
    else
    {
        context->error_callback = CC_DEFAULT_ERROR_CALLBACK;
        context->userdata = 0;
    }

    return old;
}


CC_IMPL int cvGetErrInfo(const char** errorcode_desc, const char** description,
                          const char** filename, int* line)
{
    int code = cvGetErrStatus();

    if(errorcode_desc)
        *errorcode_desc = cvErrorStr(code);

    if(code >= 0)
    {
        if(description)
            *description = 0;
        if(filename)
            *filename = 0;
        if(line)
            *line = 0;
    }
    else
    {
        CvContext* ctx = icvGetContext();

        if(description)
            *description = ctx->err_msg;
        if(filename)
            *filename = ctx->err_ctx.file;
        if(line)
            *line = ctx->err_ctx.line;
    }

    return code;
}


CC_IMPL const char* cvErrorStr(int status)
{
    static char buf[256];

    switch (status)
    {
    case CC_StsOk :        return "No Error";
    case CC_StsBackTrace : return "Backtrace";
    case CC_StsError :     return "Unspecified error";
    case CC_StsInternal :  return "Internal error";
    case CC_StsNoMem :     return "Insufficient memory";
    case CC_StsBadArg :    return "Bad argument";
    case CC_StsNoConv :    return "Iterations do not converge";
    case CC_StsAutoTrace : return "Autotrace call";
    case CC_StsBadSize :   return "Incorrect size of input array";
    case CC_StsNullPtr :   return "Null pointer";
    case CC_StsDivByZero : return "Divizion by zero occured";
    case CC_BadStep :      return "Image step is wrong";
    case CC_StsInplaceNotSupported : return "Inplace operation is not supported";
    case CC_StsObjectNotFound :      return "Requested object was not found";
    case CC_BadDepth :     return "Input image depth is not supported by function";
    case CC_StsUnmatchedFormats : return "Formats of input arguments do not match";
    case CC_StsUnmatchedSizes :  return "Sizes of input arguments do not match";
    case CC_StsOutOfRange : return "One of arguments\' values is out of range";
    case CC_StsUnsupportedFormat : return "Unsupported format or combination of formats";
    case CC_BadCOI :      return "Input COI is not supported";
    case CC_BadNumChannels : return "Bad number of channels";
    case CC_StsBadFlag :   return "Bad flag (parameter or structure field)";
    case CC_StsBadPoint :  return "Bad parameter of type CPoint";
    case CC_StsBadMask : return "Bad type of mask argument";
    case CC_StsParseError : return "Parsing error";
    case CC_StsNotImplemented : return "The function/feature is not implemented";
    case CC_StsBadMemBlock :  return "Memory block has been corrupted";
    };

    sprintf(buf, "Unknown %s code %d", status >= 0 ? "status":"error", status);
    return buf;
}

CC_IMPL int cvGetErrMode(void)
{
    return icvGetContext()->err_mode;
}

CC_IMPL int cvSetErrMode(int mode)
{
    CvContext* context = icvGetContext();
    int prev_mode = context->err_mode;
    context->err_mode = mode;
    return prev_mode;
}

CC_IMPL int cvGetErrStatus()
{
    return icvGetContext()->err_code;
}

CC_IMPL void cvSetErrStatus(int code)
{
    icvGetContext()->err_code = code;
}


CC_IMPL void cvError(int code, const char* func_name,
                      const char* err_msg,
                      const char* file_name, int line)
{
    if(code == CC_StsOk)
        cvSetErrStatus(code);
    else
    {
        CvContext* context = icvGetContext();

        if(code != CC_StsBackTrace && code != CC_StsAutoTrace)
        {
            char* message = context->err_msg;
            context->err_code = code;

            strcpy(message, err_msg);
            context->err_ctx.file = file_name;
            context->err_ctx.line = line;
        }

        if(context->err_mode != CC_ErrModeSilent)
        {
            int terminate = context->error_callback(code, func_name, err_msg,
                                                    file_name, line, context->userdata);
            if(terminate)
            {
#if !defined WIN32 && !defined WIN64
                assert(0); // for post-mortem analysis with GDB
#endif
                exit(-abs(terminate));
            }
        }
    }
}


/******************** End of implementation of profiling stuff *********************/


/**********************DllMain********************************/

#if defined WIN32 || defined WIN64
BOOL WINAPI DllMain(HINSTANCE, DWORD  fdwReason, LPVOID)
{
    CvContext *pContext;

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        g_TlsIndex = TlsAlloc();
        if(g_TlsIndex == TLS_OUT_OF_INDEXES) return FALSE;
        //break;

    case DLL_THREAD_ATTACH:
        pContext = icvCreateContext();
        if(pContext == NULL)
            return FALSE;
        TlsSetValue(g_TlsIndex, (LPVOID)pContext);
        break;

    case DLL_THREAD_DETACH:
        if(g_TlsIndex != TLS_OUT_OF_INDEXES)
        {
            pContext = (CvContext*)TlsGetValue(g_TlsIndex);
            if(pContext != NULL)
                icvDestroyContext(pContext);
        }
        break;

    case DLL_PROCESS_DETACH:
        if(g_TlsIndex != TLS_OUT_OF_INDEXES)
        {
            pContext = (CvContext*)TlsGetValue(g_TlsIndex);
            if(pContext != NULL)
                icvDestroyContext(pContext);
        }
        TlsFree(g_TlsIndex);
        break;
    default:
        ;
    }
    return TRUE;
}
#else
/* POSIX pthread */

/* function - destructor of thread */
void icvPthreadDestructor(void* key_val)
{
    CvContext* context = (CvContext*) key_val;
    icvDestroyContext(context);
}

int pthrerr = pthread_key_create(&g_TlsIndex, icvPthreadDestructor);

#endif

/* function, which converts int to int */
CC_IMPL int
cvErrorFromIppStatus(int status)
{
    switch (status)
    {
    case CC_BADSIZE_ERR: return CC_StsBadSize;
    case CC_BADMEMBLOCK_ERR: return CC_StsBadMemBlock;
    case CC_NULLPTR_ERR: return CC_StsNullPtr;
    case CC_DIV_BY_ZERO_ERR: return CC_StsDivByZero;
    case CC_BADSTEP_ERR: return CC_BadStep ;
    case CC_OUTOFMEM_ERR: return CC_StsNoMem;
    case CC_BADARG_ERR: return CC_StsBadArg;
    case CC_NOTDEFINED_ERR: return CC_StsError;
    case CC_INPLACE_NOT_SUPPORTED_ERR: return CC_StsInplaceNotSupported;
    case CC_NOTFOUND_ERR: return CC_StsObjectNotFound;
    case CC_BADCONVERGENCE_ERR: return CC_StsNoConv;
    case CC_BADDEPTH_ERR: return CC_BadDepth;
    case CC_UNMATCHED_FORMATS_ERR: return CC_StsUnmatchedFormats;
    case CC_UNSUPPORTED_COI_ERR: return CC_BadCOI;
    case CC_UNSUPPORTED_CHANNELS_ERR: return CC_BadNumChannels;
    case CC_BADFLAG_ERR: return CC_StsBadFlag;
    case CC_BADRANGE_ERR: return CC_StsBadArg;
    case CC_BADCOEF_ERR: return CC_StsBadArg;
    case CC_BADFACTOR_ERR: return CC_StsBadArg;
    case CC_BADPOINT_ERR: return CC_StsBadPoint;

    default: return CC_StsError;
    }
}
/* End of file */


