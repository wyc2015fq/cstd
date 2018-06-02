
#include "RegPath.h"

CString BCGPGetRegPath (LPCTSTR lpszPostFix, LPCTSTR lpszProfileName)
{
	ASSERT (lpszPostFix != NULL);

	CString strReg;

	if (lpszProfileName != NULL &&
		lpszProfileName [0] != 0)
	{
		strReg = lpszProfileName;
	}
	else
	{
		CWinApp* pApp = AfxGetApp ();
		ASSERT_VALID (pApp);

		if (pApp->m_pszRegistryKey == NULL ||
			pApp->m_pszProfileName == NULL)
		{
			TRACE (_T("BCGPGetRegPath error: please call SetRegistryKey in your application InitInstance\n"));
			return strReg;
		}

		strReg = _T("SOFTWARE\\");

		CString strRegKey = pApp->m_pszRegistryKey;
		if (!strRegKey.IsEmpty ())
		{
			strReg += strRegKey;
			strReg += _T("\\");
		}

		strReg += pApp->m_pszProfileName;
		strReg += _T("\\");
		strReg += lpszPostFix ;
		strReg += _T("\\");
	}
    
    return strReg;
}
