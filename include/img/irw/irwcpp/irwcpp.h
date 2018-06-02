/********************************************************************

	irwcpp.h

	
	本文件用途：	图象读写引擎C++语言版接口定义文件

	本文件编写人：	
					YZ			yzfree##sina.com

	本文件版本：	30831
	最后修改于：	2003-08-31

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
	    地址收集软件。
  	----------------------------------------------------------------
	修正历史：

		2003-8		增加Clear()接口函数
		2003-8		增加重载操作符‘=’函数
		2003-8		GetInfo()函数增加CHAVIMG信息类型
		2003-7		第一个测试版发布

********************************************************************/

#ifndef __SEEIMAGE_INC__
#define __SEEIMAGE_INC__

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <vfw.h>
#include <stdarg.h>
#include <assert.h>

#include "iseemem.h"
#include "iseeio.h"
#include "iseeimn.h"
#include "iseeirw.h"

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CIrwcppApp

class CIrwcppApp : public CWinApp
{
public:
	CIrwcppApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIrwcppApp)
	public:
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIrwcppApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// 常量及公共参数定义

#define DEFAULTEXT						("bmp")

#define ISD_COMBOBOXLIST_HEIGHT			100
#define ISD_CTLCNT						(ISD_ITEM_NUM+1)

// 新控件ID定义
#define IDC_SAVEASCNT_STC				2012
#define IDC_SAVEASCNT_CON				(IDC_SAVEASCNT_STC+ISD_ITEM_NUM+1)

// 有保存功能的插件信息结构
typedef struct _tagSavePluginInfo
{
	int				index;								// 插件索引
	int				piid;								// 插件ID
	char			str[64];							// 显示串
	char			ext[32];							// 缺省扩展名串
	IRW_SAVE_DESC	cont;								// 原始的保存功能描述
} SAVEPLUGININFO, *LPSAVEPLUGININFO;


/////////////////////////////////////////////////////////////////////////////
// 读介质类型
typedef enum
{
	CR_FILE		= ISEEIO_CONFER_LOC_FILE,				// 本地文件类型
	CR_FILEFAST	= ISEEIO_CONFER_LOC_FILE_SEQUENTIAL,	// 本地文件类型（快速版）
	CR_MEMORY	= ISEEIO_CONFER_LOC_MEMORY,				// 本地内存类型
	CR_SEEIMAGE	= ISEEIO_CONFER_MAX+1,					// 另一个CSeeImage对象
	CR_NIHILITY,										// 虚无（即创建）
	CR_CLIP,											// 剪贴板
	CR_WINDOW,											// 窗口
	CR_RESOURCE_ID,										// 资源（以ID方式给出）
	CR_RESOURCE_NAME,									// 资源（以资源名称串方式给出）
	CR_HBITMAP,											// HBITMAP句柄
	CR_HDIB												// HDIB句柄
} CSITYPER, *LPCSITYPER;

// 写介质类型
typedef enum
{
	CW_FILE		= ISEEIO_CONFER_LOC_FILE,				// 本地文件类型
	CW_FILEFAST	= ISEEIO_CONFER_LOC_FILE_SEQUENTIAL,	// 本地文件类型（快速版）
	CW_MEMORY	= ISEEIO_CONFER_LOC_MEMORY,				// 本地内存类型
	CW_CLIP		= ISEEIO_CONFER_MAX+1,					// 剪贴板
	CW_HBITMAP,											// HBITMAP句柄
	CW_HDIB												// HDIB句柄
} CSITYPEW, *LPCSITYPEW;

// 绘制动作
typedef enum
{
	CD_NON	= 0x1,										// 原尺寸绘制
	CD_STL	= 0x2,										// 拉伸
	CD_CEN	= 0x100,									// 居中
	CD_TAN	= 0x200,									// 透明绘制
} CSIDRAW, *LPCSIDRAW;

// 转换标识
typedef enum
{
	COVER    = IRWE_CNVMARK_AUTO,						/* 自动转为最匹配的标准格式（也是变长参数的结尾标志） */
	CNVAUTO  = COVER,
	CNV1     = IRWE_CNVMARK_1,							/* 强制转为 1 位调色板图 */           
	CNV4     = IRWE_CNVMARK_4,							/* 强制转为 4 位调色板图 */           
	CNV8     = IRWE_CNVMARK_8,							/* 强制转为 8 位调色板图 */           
	CNV16555 = IRWE_CNVMARK_16_555,						/* 强制转为 16 位(555)格式位图 */     
	CNV16565 = IRWE_CNVMARK_16_565,						/* 强制转为 16 位(565)格式位图 */     
	CNV24    = IRWE_CNVMARK_24,							/* 强制转为 24 位位图 */              
	CNV32    = IRWE_CNVMARK_32							/* 强制转为 32 位位图(带Alpha掩码) */ 
} CNVTYPE, *LPCNVTYPE;

// 保存位深度类型
typedef enum
{
	CSAVEAUTO	= IRWE_CNVMARK_AUTO,
	CSAVE1		= IRWE_CNVMARK_1,
	CSAVE4		= IRWE_CNVMARK_4,
	CSAVE8		= IRWE_CNVMARK_8,
	CSAVE16555	= IRWE_CNVMARK_16_555,
	CSAVE16565	= IRWE_CNVMARK_16_565,
	CSAVE24		= IRWE_CNVMARK_24,
	CSAVE32		= IRWE_CNVMARK_32
} CSAVETYPE, *LPCSAVETYPE;

// 类状态类型
typedef enum
{
	CSISTATUS_NULL,										// 空类
	CSISTATUS_IRW,										// 读写状态
	CSISTATUS_IP										// 图象处理状态
} CSISTATUS, *LPCSISTATUS;

// 入口参数检查类型
typedef enum
{
	CSIPARMCHK_LOAD,									// 读函数
	CSIPARMCHK_DRAW,									// 绘制函数
	CSIPARMCHK_SAVE										// 保存函数
} CSIPARMCHK, *LPCSIPARMCHK;

// 预定义HDIB类型（实际上就是指向没有文件头结构的BMP数据）
typedef char	*HDIB;

// CSeeImage的类标签
#define CSEEIMAGE_MAGIC		0x4d495343					// "CSIM"

// 特定断言定义
#ifdef _DEBUG
#define Assert(expr, str)	{ if (!(expr)){ assert(!(str)); } }
#else
#define Assert(expr, str)	((void*)0)
#endif

// IP模式位深度
#define IP_BITCNT			32

// 调色板项的最大个数
#define	MAX_PALETTE_CNT		256

// DIB信息结构
typedef struct _tagDIBINFO
{
	wBITMAPINFOHEADER	bmi;
	wRGBQUAD				pal[MAX_PALETTE_CNT];
} DIBINFO, *LPDIBINFO;

// 逻辑调色板结构
typedef struct _tagLOGPAL
{
	LOGPALETTE			pal;
	PALETTEENTRY		entry[MAX_PALETTE_CNT];
} LOGPAL, *LPLOGPAL;

// 内部图象读写缓冲区开启标志
const bool CSIRW_BUFFER = false;

// 易挥发型数据来源类特有的文件名
#define CSINIHFILENAME		"..bmp"


// 获取信息函数所用到的标志
typedef enum
{
	CWIDTH		= 0x1,									// 获取图象宽度
	CHEIGHT		= 0x2,									// 获取图象高度
	CBITCNT		= 0x4,									// 位深度
	CORDER		= 0x8,									// 图象的放置方式

	CCURIMGNUM	= 0x10,									// 当前图象的序号（0基）
	CPALCNT		= 0x20,									// 获取调色板项的个数
	CSTATUS		= 0x40,									// 获取类的状态（读写或IP状态）
	CIMGCNT		= 0x80,									// 获取图象页数

	CENGSTATUS	= 0x100,								// 引擎状态（是否开启）
	CHAVIMG		= 0x200,								// 类中是否存在图象

	CDIRECT		= 0x10000,								// 直接与间接获取参数的分割值

	CPALETTE	= 0x10000,								// 获取调色板数据
	CPACKPTR	= 0x20000,								// 获取类中专业包的地址
	CBITS		= 0x40000,								// 获取象素缓冲区首地址
	COPENSTR	= 0x80000								// 获取读写引擎的OPEN串

} IMGINFOTYPE, *LPIMGINFOTYPE;

// 图象信息结构
typedef struct _tagImageInfoStr
{
	ulong	flag;

	ulong	width;								/* 图像宽度 */
	ulong	height;								/* 图像高度 */
	ulong	bitcount;							/* 每个像素所占的位数（位深度） */
	ulong	order;								/* 放置方式（正向或倒向）*/
	
	unsigned int	pal_count;							/* 调色板有效项的个数 */
	ulong	palette[MAX_PALETTE_COUNT];			/* 调色板数据 */
	char			*pbits;								/* 象素数据首地址 */

	int				engstatus;							/* 引擎状态(true-已开启、flase-未开启) */
	CSISTATUS		status;								/* 类状态 */
	int				havimg;								/* 类中是否存在图象(1-存在、2-不存在) */
	int				imgcnt;								/* 图象页数 */
	int				currimgnum;							/* 当前图象（0基序号）*/

	LPSPEC_DATAPACK	pack;								/* 专业包地址 */

	char			*openstr;							/* 引擎open串 */

	int				rev[12];							/* 保留 */
} IMAGEINFOSTR, *LPIMAGEINFOSTR;


/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx - ‘另存为’对话框类

class CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)
		
public:
	CFileDialogEx(LPSPEC_DATAPACK);
	
	static EXERESULT	DispLastErrMess(int err);
	static IRWE_CNVMARK GetStdIDFromBitCnt(int bitcnt);


protected:
	LPSPEC_DATAPACK	m_pPack;							// 待保存的图象包地址
	
	int				m_iRunMark;							// 初始化成功标记（0－成功）
	int				m_ctrlcnt;							// 当前所选插件的新增列表个数
	SAVESTR			m_save;								// 保存参数结构
	CString			m_ext;								// 当前缺省扩展名
	
	CStatic			*m_pStatic[ISD_CTLCNT];				// 标签类数组
	CComboBox		*m_pComboBox[ISD_CTLCNT];			// 列表类数组
	
	int				m_saveinfocnt;						// 具有保存功能的插件个数
	SAVEPLUGININFO	m_saveinfo[ISEEIRW_MAX_PLUGIN_CNT];	// 有保存功能插件的信息结构数组
	
	
	int  _GetPluginIndex(LPCTSTR sou);
	int  _GetPluginIndexFromFilterIndex(int idx);
	int  _SetFileType(int index);
	
	BOOL _OK(void);
	
protected:
	//{{AFX_MSG(CFileDialogEx)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////////////
// CSeeImage类定义

#ifdef	_AFXDLL
#define CLASSEXTTYPE	
#else
#define CLASSEXTTYPE	__declspec(dllimport)
#endif

class CLASSEXTTYPE CSeeImage
{
public:
	CSeeImage();
	~CSeeImage();


/////////////////////////////////////////////////////////////////////////////////////
// 系统接口
public:
	static int		OpenSeeImage(char *path);
	static int		CloseSeeImage(void);
	static void *	ImAlloc(size_t size);
	static void *	ImFree(void *p);

	CSeeImage& operator=(CSeeImage&sou);
	void			Clear(void);
	
// 读写、绘制接口
public:

	int				Load(CSITYPER type, ...);
	int				GetInfo(int flag, LPIMAGEINFOSTR pinfo = NULL);
	int				Draw(CSIDRAW type, ...);
	int				Save(CSITYPEW type, ...);

// 图象处理接口
public:
	int				OpenIPMode();
	char*			IP_GetScanLineAddr(int y);
	ulong	IP_GetPixel(int x, int y);
	ulong	IP_SetPixel(int x, int y, ulong newpix);
	int				CloseIPMode();



/////////////////////////////////////////////////////////////////////////////////////	
// 内部函数
protected:
	int				_typesafe_check(CSIPARMCHK pct, int type, va_list ap);
	void			_cnvtype_check(int cnk);

	char*			_LoadDIBFromPE(char *fname, int id);
	char*			_LoadDIBFromResource(char *lpResourceName);
	int				_AllocSpecPack(LPSPEC_DATAPACK *ppPack, wBITMAPINFO* pbmi, int cnvmark, int sn, char *pbits, int bitmark);
	int				_SaveSpecPack(ISEEIO_CONFER_TYPE type, char *name, ulong par1, ulong par2, CSAVETYPE cnvmark, SAVESTR* par);
	SUBIMGBLOCK*	_CalSubImageAddr(LPINFOSTR pinfo, int num);
	void			_UpdateDispInfo(void);

	static char*	_NormalizePath(char *path, char *buff);



/////////////////////////////////////////////////////////////////////////////////////
// 内部助手
#ifdef	_AFXDLL
	friend	class CIrwcppApp;
#endif

#ifdef _DEBUG
#define TYPESAFE_CHECK(pct) _typesafe_check(pct, type, reinterpret_cast<va_list>(&type)+_INTSIZEOF(type))
#define CNVTYPE_CHECK(cnv)	_cnvtype_check(cnv)
#else
#define TYPESAFE_CHECK(pct)	((void*)0)
#define CNVTYPE_CHECK(cnv)	((void*)0)
#endif



/////////////////////////////////////////////////////////////////////////////////////
// 内部数据
private:
	ulong		m_uMagic;						// 类标签-CSEEIMAGE_MAGIC

	LPSPEC_DATAPACK		m_pPack;						// 专业数据包指针
	DIBINFO				m_cDispInfo;					// 显示用信息
	CSISTATUS			m_eStatus;						// 类状态标记
	bool				m_bIsChange;					// 是否打开过图象IP模式

	HDRAWDIB			m_hDrawDib;						// DrawDib库句柄
	
	static bool			m_bMark;						// 引擎初始化成功标记
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif  // __SEEIMAGE_INC__
