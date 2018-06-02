
#undef IPCVAPI_EX
#define IPCVAPI_EX(type,func_name,names,modules,arg) \
    { (void**)&func_name##_p, (void*)(size_t)-1, names, modules, 0 },

static CvPluginFuncInfo cv_ipp_tab[] =
{
#undef _CC_IPP_H_
#include "_cvipp.h"
#undef _CC_IPP_H_
    {0, 0, 0, 0, 0}
};

static CvModuleInfo cv_info = { 0, "cv", CC_VERSION, cv_ipp_tab };
CvModule cv_module(&cv_info);


