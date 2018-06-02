
//
// BCGCBPro.cpp : Defines the initialization routines for the DLL.
//


#include "BCGCBPro.h"
#include "BCGPLocalResource.h"

static AFX_EXTENSION_MODULE BCGCBProDLL = { 0 };

///////////////////////////////////////////////////////////////////////
// CBCGLocalResource implementation:

CBCGPLocalResource::CBCGPLocalResource()
{
	m_hInstOld = AfxGetResourceHandle ();
	ASSERT (m_hInstOld != NULL);

	AfxSetResourceHandle (BCGCBProDLL.hResource);
}

CBCGPLocalResource::~CBCGPLocalResource()
{
	AfxSetResourceHandle (m_hInstOld);
}

BCGCBPRODLLEXPORT void BCGCBProSetResourceHandle (HINSTANCE hinstResDLL)
{
	BCGCBProDLL.hResource = (hinstResDLL == NULL) ? 
		BCGCBProDLL.hModule : hinstResDLL;
}

HINSTANCE BCGCBProGetResourceHandle()
{
	return BCGCBProDLL.hResource;
}

HINSTANCE BCGCBProGetInstanceHandle ()
{
	return BCGCBProDLL.hModule;
}

#ifdef _BCGCBPRO_IN_OTHER_DLL
	__declspec(dllexport) void BCGCBProDllInitialize ()
	{
		new CDynLinkLibrary(BCGCBProDLL);
	}
#endif	// _BCGCBPRO_IN_OTHER_DLL
