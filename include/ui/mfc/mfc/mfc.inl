//#define _AFX_NO_OLE_SUPPORT
//#define _AFX_NO_DAO_SUPPORT
//#define _AFX_NO_RICHEDIT_SUPPORT
//#define _AFX_NO_DHTML_SUPPORT
//#define _AFX_NO_OCC_SUPPORT
//#define _AFX_NO_DOCOBJECT_SUPPORT
//#define __STDCALL __stdcall
//#define __CDECL __cdecl
//#define _AFX_ENABLE_INLINES
//#define AfxEnableControlContainer()
#include "stdafx.h"
#include "AFXABORT.inl"
#include "AFXASERT.inl"
#include "AFXCRIT.inl"
#include "AFXDBCS.inl"
#include "AFXINL1.inl"
#include "AFXINL2.inl"
#include "AFXINL3.inl"
#include "AFXMEM.inl"
#include "AFXSTATE.inl"
#include "AFXTLS.inl"
#include "AFXTRACE.inl"
#include "APP3D.inl"
#include "APP3DS.inl"
#include "APPCORE.inl"
#include "APPDLG.inl"
#include "APPGRAY.inl"
#include "APPHELP.inl"
#include "APPHELPX.inl"
#include "APPINIT.inl"
#include "APPMODUL.inl"
#include "APPPRNT.inl"
#include "APPTERM.inl"
#include "APPUI.inl"
#include "APPUI1.inl"
#include "APPUI2.inl"
#include "APPUI3.inl"
#include "ARCCORE.inl"
#include "ARCEX.inl"
#include "ARCOBJ.inl"
#include "ARCSTRM.inl"
#include "ARRAY_B.inl"
#include "ARRAY_D.inl"
#include "ARRAY_O.inl"
#include "ARRAY_P.inl"
#include "ARRAY_S.inl"
#include "ARRAY_U.inl"
#include "ARRAY_W.inl"
#include "AUXDATA.inl"
#include "BARCOOL.inl"
#include "BARCORE.inl"
#include "BARDLG.inl"
#include "BARDOCK.inl"
#include "BARSTAT.inl"
#include "BARTOOL.inl"
#include "CCDATA.inl"
#include "CMDTARG.inl"
#include "CTLCACHE.inl"
#include "CTLCONN.inl"
#include "CTLCORE.inl"
#include "CTLDATA.inl"
#include "CTLEVENT.inl"
#include "CTLFONT.inl"
#include "CTLFRAME.inl"
#include "CTLINL.inl"
#include "CTLINPLC.inl"
#include "CTLLIC.inl"
#include "CTLMODUL.inl"
#include "CTLNOWND.inl"
#include "CTLOBJ.inl"
#include "CTLPBAG.inl"
#include "CTLPICT.inl"
#include "CTLPPG.inl"
#include "CTLPROP.inl"
#include "CTLPROPX.inl"
#include "CTLPSET.inl"
#include "CTLPSTG.inl"
#include "CTLPSTM.inl"
#include "CTLQUICK.inl"
#include "CTLREFL.inl"
#include "CTLREG.inl"
#include "CTLTRACK.inl"
#include "CTLVIEW.inl"
#include "DBCORE.inl"
#include "DBFLT.inl"
#include "DBLONG.inl"
#include "DBRFX.inl"
#include "DBVAR.inl"
#include "DBVIEW.inl"
#include "DCMETA.inl"
#include "DCPREV.inl"
#include "DLGCLR.inl"
#include "DLGCOMM.inl"
#include "DLGCORE.inl"
#include "DLGDATA.inl"
#include "DLGFILE.inl"
#include "DLGFLOAT.inl"
#include "DLGFNT.inl"
#include "DLGFR.inl"
#include "DLGPRNT.inl"
#include "DLGPROP.inl"
#include "DLGTEMPL.inl"
#include "DOCCORE.inl"
#include "DOCKCONT.inl"
#include "DOCKSTAT.inl"
#include "DOCMAPI.inl"
#include "DOCMGR.inl"
#include "DOCMULTI.inl"
#include "DOCSINGL.inl"
#include "DOCTEMPL.inl"
#include "DUMPCONT.inl"
#include "DUMPFLT.inl"
#include "DUMPINIT.inl"
#include "DUMPOUT.inl"
#include "DUMPSTAK.inl"
#include "EXCEPT.inl"
#include "FILECORE.inl"
#include "FILEFIND.inl"
#include "FILELIST.inl"
#include "FILEMEM.inl"
#include "FILESHRD.inl"
#include "FILEST.inl"
#include "FILETXT.inl"
#include "FILEX.inl"
#include "FIXALLOC.inl"
//#include "INET.inl"
//#include "INETCALL.inl"
//#include <afxisapi.h>
/////////////////////////////////////////////////////////////////////////////
// Turn off warnings for /W4
// To resume any of these warning: #pragma warning(default: 4xxx)
// which should be placed after the AFX include files
#ifndef ALL_WARNINGS
// warnings generated with common MFC/Windows code
#pragma warning(disable: 4127) // constant expression for TRACE/ASSERT
#pragma warning(disable: 4134) // message map member fxn casts
#pragma warning(disable: 4201) // nameless unions are part of C++
#pragma warning(disable: 4511) // private copy constructors are good to have
#pragma warning(disable: 4512) // private operator= are good to have
#pragma warning(disable: 4514) // unreferenced inlines are common
#pragma warning(disable: 4710) // private constructors are disallowed
#pragma warning(disable: 4705) // statement has no effect in optimized code
#pragma warning(disable: 4191) // pointer-to-function casting
// warnings caused by normal optimizations
#ifndef _DEBUG
#pragma warning(disable: 4701) // local variable *may* be used without init
#pragma warning(disable: 4702) // unreachable code caused by optimizations
#pragma warning(disable: 4791) // loss of debugging info in retail version
#pragma warning(disable: 4189) // initialized but unused variable
#pragma warning(disable: 4390) // empty controlled statement
#endif
// warnings specific to _AFXDLL version
#ifdef _AFXDLL
#pragma warning(disable: 4204) // non-constant aggregate initializer
#endif
#ifdef _AFXDLL
#pragma warning(disable: 4275) // deriving exported class from non-exported
#pragma warning(disable: 4251) // using non-exported as public in exported
#endif
#endif //!ALL_WARNINGS
#define STRICT 1
#ifndef _DEBUG
#ifndef _AFX_ENABLE_INLINES
#define _AFX_ENABLE_INLINES
#endif
#endif
#ifndef UNUSED
#ifdef _DEBUG
#define UNUSED(x)
#else
#define UNUSED(x) x
#endif
#endif
#define AFXISAPI __stdcall
#define AFXIS_DATADEF
#define AFXISAPI_CDECL __cdecl
#ifndef AFX_INLINE
#define AFX_INLINE __forceinline
#endif
#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif
//#include "ISAPIMIX.inl"
#include "LIST_O.inl"
#include "LIST_P.inl"
#include "LIST_S.inl"
#include "MAP_PP.inl"
#include "MAP_PW.inl"
#include "MAP_SO.inl"
#include "MAP_SP.inl"
#include "MAP_SS.inl"
#include "MAP_WO.inl"
#include "MAP_WP.inl"
#include "MTCORE.inl"
#include "MTEX.inl"
#include "NOLIB.inl"
#include "OBJCORE.inl"
#include "OCCCONT.inl"
#include "OCCDDX.inl"
#include "OCCDDXF.inl"
#include "OCCDLG.inl"
#include "OCCEVENT.inl"
#include "OCCLOCK.inl"
#include "OCCMGR.inl"
#include "OCCSITE.inl"
#include "OLEASMON.inl"
#include "OLEBAR.inl"
#include "OLECALL.inl"
#include "OLECLI1.inl"
#include "OLECLI2.inl"
#include "OLECLI3.inl"
#include "OLECNVRT.inl"
#include "OLECONN.inl"
#include "OLEDATA.inl"
#include "OLEDISP1.inl"
#include "OLEDISP2.inl"
#include "OLEDLGS1.inl"
#include "OLEDLGS2.inl"
#include "OLEDLGS3.inl"
#include "OLEDLL.inl"
#include "OLEDOBJ1.inl"
#include "OLEDOBJ2.inl"
#include "OLEDOC1.inl"
#include "OLEDOC2.inl"
#include "OLEDOCCL.inl"
#include "OLEDOCIP.inl"
#include "OLEDOCOB.inl"
#include "OLEDOCTG.inl"
#include "OLEDOCVW.inl"
#include "OLEDROP1.inl"
#include "OLEDROP2.inl"
#include "OLEENUM.inl"
#include "OLEEXP.inl"
#include "OLEFACT.inl"
#include "OLEINIT.inl"
#include "OLEIPFRM.inl"
#include "OLELINK.inl"
#include "OLELOCK.inl"
#include "OLEMISC.inl"
#include "OLEMON.inl"
#include "OLEMSGF.inl"
#include "OLEPSET.inl"
#include "OLEREG.inl"
#include "OLESTRM.inl"
#include "OLESVR1.inl"
#include "OLESVR2.inl"
#include "OLETSVR.inl"
#include "OLETYPLB.inl"
#include "OLEUI1.inl"
#include "OLEUI2.inl"
#include "OLEUNK.inl"
#include "OLEVAR.inl"
#include "OLEVAR1.inl"
#include "OLEVERB.inl"
#include "PLEX.inl"
#include "PPGCOLOR.inl"
#include "PPGFONT.inl"
#include "PPGPICT.inl"
#include "PPGSTOCK.inl"
#include "SOCKCORE.inl"
#include "STDAFX.inl"
#include "STRCORE.inl"
#include "STREX.inl"
#include "THRDCORE.inl"
#include "TIMECORE.inl"
#include "TOOLTIP.inl"
#include "TRCKRECT.inl"
#include "VALIDADD.inl"
#include "VIEWCMN.inl"
#include "VIEWCORE.inl"
#include "VIEWEDIT.inl"
#include "VIEWFORM.inl"
#include "VIEWHTML.inl"
#include "VIEWPREV.inl"
#include "VIEWPRNT.inl"
#include "VIEWRICH.inl"
#include "VIEWSCRL.inl"
#include "WINBTN.inl"
#include "WINCORE.inl"
#include "WINCTRL1.inl"
#include "WINCTRL2.inl"
#include "WINCTRL3.inl"
#include "WINCTRL4.inl"
#include "WINCTRL5.inl"
#include "WINCTRL6.inl"
#include "WINCTRL7.inl"
#include "WINFRM.inl"
#include "WINFRM2.inl"
#include "WINFRMX.inl"
#include "WINGDI.inl"
#include "WINGDIX.inl"
#include "WINHAND.inl"
#include "WINMAIN.inl"
#include "WINMDI.inl"
#include "WINMENU.inl"
#include "WINMINI.inl"
#include "WINOCC.inl"
#include "WINSPLIT.inl"
#include "WINSTR.inl"
#include "WINUTIL.inl"
#ifndef _AFX_NO_DAO_SUPPORT
#include "DAOCORE.inl"
#include "DAODFX.inl"
#include "DAOVIEW.inl"
#include "VIEWOLED.inl"
#endif
#if 0
#include "ISAPI.inl"
#include "DLLDB.inl"
#include "DLLINIT.inl"
#include "DLLMODUL.inl"
#include "DLLNET.inl"
#include "DLLOLE.inl"
#endif

#undef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
const GUID DECLSPEC_SELECTANY name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#include "dbdaoid.h"
#undef DEFINE_GUID
