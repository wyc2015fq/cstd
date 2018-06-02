/********************************************************************

	irwcpp.cpp

	
	本文件用途：	图象读写引擎C++语言版接口实现文件

	本文件编写人：	
					YZ			yzfree##sina.com

	本文件版本：	30903
	最后修改于：	2003-9-3

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
	    地址收集软件。
  	----------------------------------------------------------------
	修正历史：

		2003-9		修正保存函数中的一处BUG（缺少对保存文件名有效性判断）
		2003-8		增加Clear()接口函数
		2003-8		修正Load()接口函数资源ID类型中枚举功能的一处BUG
		2003-8		增加重载操作符‘=’函数
		2003-8		GetInfo()函数增加CHAVIMG信息类型，并修正一处BUG
		2003-7		第一个测试版发布

********************************************************************/

#include "stdafx.h"

#include <dlgs.h>
#include "irwcpp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment (lib, "vfw32.lib")
#ifdef _DEBUG
#pragma comment (lib, "iseemem.lib")
#endif
#pragma comment (lib, "iseeio.lib")
#pragma comment (lib, "iseeirw.lib")

bool CSeeImage::m_bMark = false;


/////////////////////////////////////////////////////////////////////////////
// CIrwcppApp

BEGIN_MESSAGE_MAP(CIrwcppApp, CWinApp)
	//{{AFX_MSG_MAP(CIrwcppApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIrwcppApp construction

CIrwcppApp::CIrwcppApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIrwcppApp object

CIrwcppApp theApp;


/////////////////////////////////////////////////////////////////////////////
// C++接口实现

// 引擎接口初始化
int CSeeImage::OpenSeeImage(char *path)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	Assert(!m_bMark, "OpenSeeImage()函数－引擎不能重复初始化。");

	char	buff[MAX_PATH];
	int		result;

	// 正规化调用者提供的路径串（转换为绝对路径）
	_NormalizePath(path, (char*)buff);

	// 初始化ISee图象读写引擎
	if ((result=isirw_init((char*)buff, 0)) != 0)
	{
		/*
			-1 :内存不足
			-2 :无可用插件
			-3 :文件读写错误
			-4 :INI文件内容非法
			-5 :系统异常
			-6 :消息泵线程创建失败
		*/	
		return result;
	}

	// 根据设定开启引擎的内部象素缓冲区
	if (CSIRW_BUFFER)
	{
		isirw_enbuffer(IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV);
	}

	// 设置引擎开启标志
	m_bMark = true;

	// 锁定引擎
	LPIRWMF_HEADER	phead = isirw_lock();

	if (phead == NULL)
	{
		CloseSeeImage();
		return -5;										// 系统异常
	}

	UPDATAINFO	udp;
	
	// 刷新插件信息表
	switch (result=isirw_update(phead, IRWE_UPDTYPE_EMPLOY, &udp))
	{
	case	0:			// 0: 刷新成功
		break;
	case	1:			// 1: 无可用插件
		result = -2;
		break;
	case	2:			// 2: 读写错误
		result = -3;
		break;
	case	3:			// 3: 内存不足
		result = -1;
		break;
	case	4:			// 4: 系统异常
	default:
		result = -5;
		break;
	}
	
	// 引擎解锁
	isirw_unlock(phead);

	if (result != 0)
	{
		CloseSeeImage();
	}

	return result;
}


// 关闭引擎
int CSeeImage::CloseSeeImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_bMark)
	{
		return -1;										// 引擎并未打开过
	}
	
	// 设置引擎开启标志
	m_bMark = false;

	// 关闭引擎的象素缓冲区
	if (CSIRW_BUFFER)
	{
		isirw_dibuffer(IRWE_BUFTYPE_ORG|IRWE_BUFTYPE_CNV);
	}
	
	// 关闭ISee图象读写引擎
	isirw_close();

	return 0;
}



CSeeImage::CSeeImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_eStatus   = CSISTATUS_NULL;
	m_pPack     = NULL;
	m_bIsChange	= false;
	m_hDrawDib	= DrawDibOpen();

	memset((void*)&m_cDispInfo, 0, sizeof(DIBINFO));

	m_uMagic	= CSEEIMAGE_MAGIC;
}


CSeeImage::~CSeeImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	Assert(m_uMagic == CSEEIMAGE_MAGIC, "CSeeImage析构函数－类内存遭到破坏！");

	if (m_hDrawDib)
	{
		DrawDibClose(m_hDrawDib);
	}
	
	if (m_pPack != NULL)
	{
		isirw_free_SPEC_DATAPACK(m_pPack);
	}
}


// 从指定的位置读取图象
int CSeeImage::Load(CSITYPER type, ...)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	LPSPEC_DATAPACK		psOldPack;
	enum EXERESULT		eer;
	va_list				argp;
	int					cnvmark, i;

	// 引擎必须已经开启
	Assert(m_bMark, "Load()函数－引擎还未开启！");

	if (!m_bMark)
	{
		return 0;
	}

	// 入口参数类型检查
	TYPESAFE_CHECK(CSIPARMCHK_LOAD);

	va_start(argp, type);
	
	switch (type)
	{
	case	CR_FILE:									// 本地文件类型
	case	CR_FILEFAST:								// 本地文件类型（快速版）
		{
			char	*fname;
			char	buff[MAX_PATH];

			fname   = va_arg(argp, char*);
			cnvmark	= va_arg(argp, int);

			va_end(argp);

			Assert(cnvmark != IRWE_CNVMARK_NOT, "Load()函数－FILE类型－第二个参数不能为-2。");

			// 保存原有的专业包
			psOldPack = m_pPack;

			// 检查是否需要打开Open对话框
			if (fname == NULL)
			{
				CFileDialog		fdl(TRUE, NULL, NULL, 0, (LPCTSTR)isirw_get_openstr(), ::AfxGetMainWnd());

				if (fdl.DoModal() == IDOK)
				{
					CString	str = fdl.GetPathName();
					lstrcpy((LPSTR)buff, (LPCSTR)str);
					fname = (char*)buff;
				}
				else
				{
					return ER_NONIMAGE;
				}
			}

			// 向引擎发送读取命令
			if (isirw_s_read_img_c((ISEEIO_CONFER_TYPE)type, fname, 0, 0, (int)type, (IRWE_CNVMARK)cnvmark, NULL, &m_pPack) == ISIRW_LASTERR_SUCCESS)
			{
				assert(m_pPack != NULL);

				// 等待读动作完成
				eer = isirw_s_wait(m_pPack, 0);

				if (eer != ER_SUCCESS)					// 读取错误
				{
					assert(m_pPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(m_pPack);
				}
				else									// 读取成功
				{
					;
				}
			}
			else
			{
				eer = ER_MEMORYERR;						// 内存不足（系统不稳定的可能性较小）
			}

			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}

				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}

			return eer;
		}
		break;
	case	CR_MEMORY:									// 本地内存类型
		{
			char			*mname;
			char			*addr;
			ulong	len;
			
			mname   = va_arg(argp, char*);
			addr	= va_arg(argp, char*);
			len		= va_arg(argp, ulong);
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);
			
			// 保存原有的专业包
			psOldPack = m_pPack;
			
			// 向引擎发送读取命令
			if (isirw_s_read_img_c(ISEEIO_CONFER_LOC_MEMORY, mname, (ulong)addr, len, (int)type, (IRWE_CNVMARK)cnvmark, NULL, &m_pPack) == ISIRW_LASTERR_SUCCESS)
			{
				assert(m_pPack != NULL);
				
				// 等待读动作完成
				eer = isirw_s_wait(m_pPack, 0);
				
				if (eer != ER_SUCCESS)					// 读取错误
				{
					assert(m_pPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(m_pPack);
				}
				else									// 读取成功
				{
					;
				}
			}
			else
			{
				eer = ER_MEMORYERR;						// 内存不足（系统不稳定的可能性较小）
			}
			
			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}
			
			return eer;
		}
		break;
	case	CR_SEEIMAGE:								// 另一个CSeeImage对象
		{
			CSeeImage	*pSou;
			
			pSou    = va_arg(argp, CSeeImage*);
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);

			assert(pSou&&(pSou->GetInfo(CSTATUS)!=(int)CSISTATUS_NULL));

			// 保存原有的专业包
			psOldPack = m_pPack;

			DIBINFO			bmi;
			IMAGEINFOSTR	iis;
			
			// 取得源图象的宽、高等数据
			iis.flag = CWIDTH|CHEIGHT|CBITCNT|CORDER|CPALCNT|CPALETTE|CBITS;
			pSou->GetInfo(0, &iis);

			// 填写源图象信息结构
			bmi.bmi.biSize     = sizeof(wBITMAPINFOHEADER);
			bmi.bmi.biWidth	   = iis.width;
			bmi.bmi.biHeight   = (iis.order == 0) ? -(long)(iis.height) : iis.height;
			bmi.bmi.biBitCount = (ushort)iis.bitcount;
			bmi.bmi.biPlanes   = 1;
			
			bmi.bmi.biCompression   = BI_RGB;
			bmi.bmi.biSizeImage     = 0;
			bmi.bmi.biXPelsPerMeter = 0;
			bmi.bmi.biYPelsPerMeter = 0;
			bmi.bmi.biClrImportant  = 0;
			bmi.bmi.biClrUsed       = 0;
			
			memcpy((void*)bmi.pal, (const void *)iis.palette, sizeof(wRGBQUAD)*iis.pal_count);
			
			// 申请并填写专业包
			eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)&bmi, cnvmark, (int)type, iis.pbits, 1);
			
			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}
			
			return eer;
		}
		break;
	case	CR_NIHILITY:								// 虚无（即创建）
		{
			int		width;
			int		height;
			int		bitcount;
			
			width   = va_arg(argp, int);
			height	= va_arg(argp, int);
			bitcount= va_arg(argp, int);
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);
			
			// 保存原有的专业包
			psOldPack = m_pPack;

			DIBINFO	bmi;

			// 填写源图象信息结构
			bmi.bmi.biSize     = sizeof(wBITMAPINFOHEADER);
			bmi.bmi.biWidth	   = width;
			bmi.bmi.biHeight   = height;
			bmi.bmi.biBitCount = bitcount;
			bmi.bmi.biPlanes   = 1;

			bmi.bmi.biCompression   = BI_RGB;
			bmi.bmi.biSizeImage     = 0;
			bmi.bmi.biXPelsPerMeter = 0;
			bmi.bmi.biYPelsPerMeter = 0;
			bmi.bmi.biClrImportant  = 0;
			bmi.bmi.biClrUsed       = 0;

			// 合成灰度调色板
			switch (bitcount)
			{
			case	1:
				*(ulong *)&(bmi.pal[0]) = 0;
				*(ulong *)&(bmi.pal[1]) = 0xffffff;
				break;
			case	4:
				for (i=0; i<16; i++)
				{
					*(ulong *)&(bmi.pal[i]) = ((i*17)<<16)|((i*17)<<8)|(i*17);
				}
				break;
			case	8:
				for (i=0; i<256; i++)
				{
					*(ulong *)&(bmi.pal[i]) = (i<<16)|(i<<8)|(i);
				}
				break;
			case	16:
			case	24:
			case	32:
				break;
			default:
				assert(0);
				return ER_NOTSUPPORT;
			}
			
			// 申请并填写专业包
			eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)&bmi, cnvmark, (int)type, NULL, 1);

			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}
			
			return eer;
		}
		break;
	case	CR_CLIP:									// 剪贴板
		{
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);
			
			// 保存原有的专业包
			psOldPack = m_pPack;

			// 判断剪贴板中是否存在图象
			if (IsClipboardFormatAvailable(CF_DIB) == FALSE)
			{
				return ER_NONIMAGE;
			}

			// 打开剪贴板
			if (::OpenClipboard(NULL))
			{
				// 获取剪贴板中的DIB数据
				HANDLE h = ::GetClipboardData(CF_DIB);
				
				if (h == 0)
				{
					CloseClipboard();
					return ER_BADIMAGE;
				}

				// 锁定剪贴板资源，获取地址
				LPSTR lpDIB = (LPSTR)::GlobalLock((HGLOBAL)h);
				wBITMAPINFOHEADER*	pbmi;
				char				*pbits;

				if (!lpDIB)
				{
					CloseClipboard();					// 读入的剪贴板数据句柄不需要释放，系统会自动释放
					return ER_BADIMAGE;
				}
				
				// 检查DIB信息块的有效性
				if (*((DWORD*)lpDIB) != sizeof(wBITMAPINFOHEADER))
				{
					::GlobalUnlock((HGLOBAL)h);
					CloseClipboard();
					return ER_BADIMAGE;
				}
				
				pbmi  = (wBITMAPINFOHEADER*)lpDIB;
				pbits = (char*)(pbmi+1);

				// 申请并填写专业包
				eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)pbmi, cnvmark, (int)type, pbits, 0);

				::GlobalUnlock((HGLOBAL)h);
				::CloseClipboard();
				
				if (eer == ER_SUCCESS)
				{
					// 如果存在旧专业包，则先释放它
					if (psOldPack != NULL)
					{
						assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
						isirw_free_SPEC_DATAPACK(psOldPack);
					}
					
					// 类置为读写状态
					m_eStatus = CSISTATUS_IRW;
					// 设当前图象为主图象
					m_pPack->lab1 = 0;
					// 设置显示用信息
					_UpdateDispInfo();
				}
				else
				{
					// 如果读取失败，则恢复旧的数据
					m_pPack = psOldPack;
				}
				
				return eer;
			}
		}
		break;
	case	CR_WINDOW:									// 窗口
		{
			HWND	hWnd;

			hWnd    = va_arg(argp, HWND);
			cnvmark	= va_arg(argp, int);
			
			va_end(argp);
			
			// 保存原有的专业包
			psOldPack = m_pPack;
			
			// 申请新的专业数据包
			if ((m_pPack=isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, CSINIHFILENAME, 0, 0, (IRWE_CNVMARK)cnvmark)) != NULL)
			{
				assert(m_pPack->org_img != NULL);

				CWnd		*pWnd = CWnd::FromHandle(hWnd);
				CBitmap		bitmap;
				CWindowDC	dc(pWnd);
				CDC			memDC;
				CRect		rect;
				
				memDC.CreateCompatibleDC(&dc);
				
				pWnd->GetWindowRect(rect);

				//用窗口大小定义一个兼容位图
				bitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());
				
				CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
				memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc, 0, 0, SRCCOPY);
				
				// 创建调色板
				CPalette	pal;
				LOGPAL		logp;

				if (dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
				{
					logp.pal.palVersion    = 0x300;
					logp.pal.palNumEntries = GetSystemPaletteEntries(dc, 0, MAX_PALETTE_CNT, logp.pal.palPalEntry);
					pal.CreatePalette(&logp.pal);
				}
				
				memDC.SelectObject(pOldBitmap);
				memDC.DeleteDC();

				BITMAP		btm;
				
				// 获得位图信息（位深度）
				bitmap.GetBitmap(&btm);
				
				int				width    = btm.bmWidth;
				int				height   = abs(btm.bmHeight);
				int				bitcount = btm.bmBitsPixel;
				LPINFOSTR		pinfo    = m_pPack->org_img;
				ulong	linesize = DIBSCANLINE_DWORDALIGN(width*bitcount);
				ulong	palcnt;
				
				// 申请行首地址数组
				if ((pinfo->pp_line_addr=(void**)isirw_malloc(sizeof(char*)*height)) == NULL)
				{
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_MEMORYERR;
				}
				
				// 申请象素缓冲区
				if ((pinfo->p_bit_data=(uchar *)isirw_malloc((int)linesize*height)) == NULL)
				{
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_MEMORYERR;
				}

				DIBINFO			bih;

				bih.bmi.biSize			= sizeof(wBITMAPINFOHEADER);
				bih.bmi.biWidth			= btm.bmWidth;
				bih.bmi.biHeight		= btm.bmHeight;
				bih.bmi.biPlanes		= 1;
				bih.bmi.biBitCount		= btm.bmBitsPixel;
				bih.bmi.biSizeImage		= 0;
				bih.bmi.biXPelsPerMeter	= 0;
				bih.bmi.biYPelsPerMeter	= 0;
				bih.bmi.biClrImportant	= 0;
				bih.bmi.biClrUsed		= 0;
				bih.bmi.biCompression	= BI_RGB;
				
				::GetDIBits(dc, bitmap, 0, height, pinfo->p_bit_data, (BITMAPINFO*)&bih, DIB_RGB_COLORS);

				switch (bitcount)
				{
				case	1:
					SETMASK_8(pinfo);
					palcnt = 2;
					pal.GetPaletteEntries(0, palcnt, (LPPALETTEENTRY)pinfo->palette);

					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = ((pinfo->palette[i]&0xff)<<16)|((pinfo->palette[i]&0xff0000)>>16)|((pinfo->palette[i]&0xff00));
					}
					break;
				case	4:
					SETMASK_8(pinfo);
					palcnt = 16;
					pal.GetPaletteEntries(0, palcnt, (LPPALETTEENTRY)pinfo->palette);

					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = ((pinfo->palette[i]&0xff)<<16)|((pinfo->palette[i]&0xff0000)>>16)|((pinfo->palette[i]&0xff00));
					}
					break;
				case	8:
					SETMASK_8(pinfo);
					palcnt = 256;
					pal.GetPaletteEntries(0, palcnt, (LPPALETTEENTRY)pinfo->palette);

					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = ((pinfo->palette[i]&0xff)<<16)|((pinfo->palette[i]&0xff0000)>>16)|((pinfo->palette[i]&0xff00));
					}
					break;
				case	16:
					if (bih.bmi.biCompression == BI_RGB)
					{
						pinfo->r_mask = 0x7c00;
						pinfo->g_mask = 0x3e0;
						pinfo->b_mask = 0x1f;
						pinfo->a_mask = 0x0;
					}
					else
					{
						pinfo->r_mask = 0xf800;
						pinfo->g_mask = 0x7e0;
						pinfo->b_mask = 0x1f;
						pinfo->a_mask = 0x0;
					}
					palcnt = 0;
					break;
				case	24:
					SETMASK_24(pinfo);
					palcnt = 0;
					break;
				case	32:
					SETMASK_32(pinfo);
					palcnt = 0;
					break;
				default:
					assert(0);
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_NOTSUPPORT;
					break;
				}
				
				pinfo->imgtype     = IMT_RESSTATIC;		/* 图像文件类型 */
				pinfo->imgformat   = IMF_BMP;			/* 图像文件格式（插件标识） */
				pinfo->compression = ICS_RGB;			/* 图像文件的压缩方式 */

				pinfo->width    = (ulong)width;
				pinfo->height   = (ulong)height;
				pinfo->bitcount = (ulong)bitcount;
				pinfo->order    = (bih.bmi.biHeight > 0) ? 1 : 0;
				
				// 填写行首地址数组
				if (pinfo->order == 0)		/* 正向 */
				{
					for (i=0;i<(int)(pinfo->height);i++)
					{
						pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+(i*linesize));
					}
				}
				else							/* 倒向 */
				{
					for (i=0;i<(int)(pinfo->height);i++)
					{
						pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+((pinfo->height-i-1)*linesize));
					}
				}
				
				pinfo->pal_count = palcnt;				/* 调色板有效项的个数 */
				
				pinfo->play_number = 0;					/* 播放顺序表尺寸（以元素为单位） */
				pinfo->play_order  = NULL;				/* 播放顺序表 */
				pinfo->time        = 0;					/* 首帧停留时间 */
				pinfo->colorkey    = -1;				/* 主图象透明色（-1表示无透明色） */
				
				pinfo->imgnumbers  = 1;					/* 该文件中图像的个数 */
				pinfo->psubimg     = NULL;				/* 子图像数据链地址 */
				pinfo->data_state  = 2;					/* 数据包当前的数据状态 */
				
				m_pPack->command   = IRWE_SDPCOMM_LOAD;	/* 模拟读命令 */
				m_pPack->data_state= SDP_DATA_STATE_ORG_PIX|SDP_DATA_STATE_ORG_INFO|SDP_DATA_STATE_PATH;
				m_pPack->exec_state= IRWE_SDPSTATE_IDLE;
				m_pPack->sn        = (int)type;
				
				if (cnvmark != IRWE_CNVMARK_NOT)
				{
					if (isirw_cnv(m_pPack, (IRWE_CNVMARK)cnvmark, 0) < 0)
					{
						isirw_free_SPEC_DATAPACK(m_pPack);
						m_pPack = psOldPack;
						return ER_MEMORYERR;
					}
				}
				
				eer = ER_SUCCESS;
			}
			else
			{
				eer = ER_MEMORYERR;
			}
			
			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}
			
			return eer;
		}
		break;
	case	CR_RESOURCE_ID:								// 资源（以ID方式给出）
		{
			char	*filename;							// 资源文件全路径（EXE、DLL）
			int		id;
			char	*pdib;
			wBITMAPINFO*	pmi;

			filename = va_arg(argp, char*);
			id       = va_arg(argp, int);
			cnvmark	 = va_arg(argp, int);
			
			va_end(argp);
			
			// 保存原有的专业包
			psOldPack = m_pPack;

			// 判断是否是读取当前进程中的资源
			if (filename == NULL)
			{
				assert(id >= 0);						// 读当前进程中的资源时没有枚举特性
				pdib = _LoadDIBFromResource((char*)MAKEINTRESOURCE(id));
			}
			else
			{
				pdib = _LoadDIBFromPE(filename, id);	// 从指定的PE格式文件中读取资源
			}

			if (pdib == NULL)
			{
				return ER_NONIMAGE;						// 没有图象或数据非法
			}

			pmi = (wBITMAPINFO*)pdib;

			if ((pmi->bmiHeader.biSize != sizeof(wBITMAPINFOHEADER))||
				(pmi->bmiHeader.biWidth == 0)||(pmi->bmiHeader.biWidth > 32767)||
				(pmi->bmiHeader.biHeight == 0))
			{
				if (filename != NULL)
				{
					delete []pdib;
				}
				return ER_NONIMAGE;
			}

			if (pmi->bmiHeader.biCompression == BI_RGB)
			{
				// 申请并填写专业包
				eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)pdib, cnvmark, (int)type, (char*)(pdib+sizeof(wBITMAPINFOHEADER)), 0);
			}
			else
			{
				int		imgsize = 0;
				int		bmpoff  = 0;

				// 计算图象文件的真实尺寸
				imgsize = sizeof(BITMAPFILEHEADER);
				imgsize += sizeof(wBITMAPINFOHEADER);

				// 累加调色板尺寸
				if (pmi->bmiHeader.biBitCount <= 8)
				{
					if (pmi->bmiHeader.biClrUsed == 0)
					{
						switch (pmi->bmiHeader.biBitCount)
						{
						case	1:
							imgsize += 2*sizeof(wRGBQUAD);
							break;
						case	2:
							imgsize += 4*sizeof(wRGBQUAD);
							break;
						case	4:
							imgsize += 16*sizeof(wRGBQUAD);
							break;
						case	8:
							imgsize += (unsigned int)256*sizeof(wRGBQUAD);
							break;
						default:
							if (filename != NULL)
							{
								delete []pdib;
							}
							return ER_NONIMAGE;
						}
					}
					else
					{
						imgsize += pmi->bmiHeader.biClrUsed*sizeof(wRGBQUAD);
					}
				}
				else
				{
					if (pmi->bmiHeader.biCompression == BI_BITFIELDS)
					{
						imgsize += 3*sizeof(ulong);
					}
				}

				bmpoff = imgsize;

				// 累加位图数据区尺寸
				if (pmi->bmiHeader.biSizeImage == 0)
				{
					int lns = DIBSCANLINE_DWORDALIGN(pmi->bmiHeader.biWidth*pmi->bmiHeader.biBitCount);

					imgsize += lns*abs(pmi->bmiHeader.biHeight);
				}
				else
				{
					imgsize += pmi->bmiHeader.biSizeImage;
				}

				// 申请内存文件存放区
				char	*pbmp = (char*)isirw_malloc(imgsize);

				if (pbmp == NULL)
				{
					if (filename != NULL)
					{
						delete []pdib;
					}
					return ER_MEMORYERR;				// 内存不足
				}

				LPBITMAPFILEHEADER	pfi = (LPBITMAPFILEHEADER)pbmp;

				// 制作内存BMP文件
				pfi->bfType = ((ushort)('M'<<8)|'B');
				pfi->bfSize = imgsize;
				pfi->bfOffBits   = bmpoff;
				pfi->bfReserved1 = 0;
				pfi->bfReserved2 = 0;

				memcpy((void*)(pfi+1), (const void *)pmi, imgsize-sizeof(BITMAPFILEHEADER));

				// 向引擎发送读取命令
				if (isirw_s_read_img_c(ISEEIO_CONFER_LOC_MEMORY, "a.bmp", (ulong)pbmp, imgsize, (int)type, (IRWE_CNVMARK)cnvmark, NULL, &m_pPack) == ISIRW_LASTERR_SUCCESS)
				{
					assert(m_pPack != NULL);
					
					// 等待读动作完成
					eer = isirw_s_wait(m_pPack, 0);
					
					if (eer != ER_SUCCESS)					// 读取错误
					{
						assert(m_pPack->exec_state == IRWE_SDPSTATE_IDLE);
						isirw_free_SPEC_DATAPACK(m_pPack);
					}
				}
				else
				{
					eer = ER_MEMORYERR;						// 内存不足（系统不稳定的可能性较小）
				}

				isirw_free(pbmp);
			}

			if (filename != NULL)						// 必须释放从PE文件中读入的缓冲区
			{
				delete []pdib;
			}
			
			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}

			return eer;
		}
	case	CR_RESOURCE_NAME:							// 资源（以资源名称串方式给出）
		{
			char	*resname;							// 资源名
			char	*pdib;

			resname  = va_arg(argp, char*);
			cnvmark	 = va_arg(argp, int);
			
			va_end(argp);
			
			// 保存原有的专业包
			psOldPack = m_pPack;

			// 读取当前进程中的资源
			pdib = _LoadDIBFromResource(resname);

			if (pdib == NULL)
			{
				return ER_NONIMAGE;						// 没有图象或数据非法
			}

			// 申请并填写专业包
			eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)pdib, cnvmark, (int)type, (char*)(pdib+sizeof(wBITMAPINFOHEADER)), 0);
			
			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}

			return eer;
		}
	case	CR_HBITMAP:									// HBITMAP句柄
		{
			HBITMAP	hBmp;								// 源位图句柄
			
			hBmp     = va_arg(argp, HBITMAP);
			cnvmark	 = va_arg(argp, int);
			
			va_end(argp);
			
			// 保存原有的专业包
			psOldPack = m_pPack;
			
			HWND	hWnd = ::GetDesktopWindow();
			HDC		hDC;
			BITMAP	bmp;
			DIBINFO	binfo;

			assert(hWnd != NULL);
			
			// 获取给定位图的宽度、高度及颜色格式信息
			if (::GetObject(hBmp, sizeof(BITMAP), (LPVOID)&bmp) != sizeof(BITMAP))
			{
				return ER_BADIMAGE;
			}
			
			// 设置DIB信息块内容
			binfo.bmi.biSize   = sizeof(wBITMAPINFOHEADER);
			binfo.bmi.biWidth  = bmp.bmWidth;
			binfo.bmi.biHeight = bmp.bmHeight;
			binfo.bmi.biPlanes = 1;
			// 在16色系统中，平面数是4，而位数是1，但这在DIB中是不允许的
			binfo.bmi.biBitCount    = max(bmp.bmPlanes, bmp.bmBitsPixel);
			binfo.bmi.biCompression = BI_RGB;
			
			// 计算设备分辩率
			hDC = ::GetDC(hWnd);
			binfo.bmi.biXPelsPerMeter = (GetDeviceCaps(hDC,HORZRES)*1000)/GetDeviceCaps(hDC,HORZSIZE);
			binfo.bmi.biYPelsPerMeter = (GetDeviceCaps(hDC,VERTRES)*1000)/GetDeviceCaps(hDC,VERTSIZE);
			::ReleaseDC(hWnd, hDC);
			
			binfo.bmi.biClrUsed      = 0;
			binfo.bmi.biClrImportant = 0;
			binfo.bmi.biSizeImage    = 0;
			
			// 申请新的专业数据包
			if ((m_pPack=isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, CSINIHFILENAME, 0, 0, (IRWE_CNVMARK)cnvmark)) != NULL)
			{
				assert(m_pPack->org_img != NULL);

				int				width    = binfo.bmi.biWidth;
				int				height   = abs(binfo.bmi.biHeight);
				int				bitcount = binfo.bmi.biBitCount;
				LPINFOSTR		pinfo    = m_pPack->org_img;
				char			*pbits   = 0;
				ulong	linesize = DIBSCANLINE_DWORDALIGN(width*bitcount);
				ulong	palcnt;

				// 申请行首地址数组
				if ((pinfo->pp_line_addr=(void**)isirw_malloc(sizeof(char*)*height)) == NULL)
				{
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_MEMORYERR;
				}
				
				// 申请象素缓冲区
				if ((pinfo->p_bit_data=(uchar *)isirw_malloc((int)linesize*height)) == NULL)
				{
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_MEMORYERR;
				}

				// 获取象素数据及调色板数据
				hDC = ::GetDC(hWnd);
				if (!::GetDIBits(hDC, hBmp, 0, height, (LPVOID)pinfo->p_bit_data, (wBITMAPINFO*)&binfo, DIB_RGB_COLORS))
				{
					::ReleaseDC(hWnd, hDC);
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_BADIMAGE;
				}
				::ReleaseDC(hWnd, hDC);

				switch (bitcount)
				{
				case	1:
					SETMASK_8(pinfo);
					palcnt = 2;
					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = *(ulong *)&(binfo.pal[i]);
					}
					break;
				case	4:
					SETMASK_8(pinfo);
					palcnt = 16;
					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = *(ulong *)&(binfo.pal[i]);
					}
					break;
				case	8:
					SETMASK_8(pinfo);
					palcnt = 256;
					for (i=0; i<(int)palcnt; i++)
					{
						pinfo->palette[i] = *(ulong *)&(binfo.pal[i]);
					}
					break;
				case	16:
					if (binfo.bmi.biCompression == BI_RGB)
					{
						pinfo->r_mask = 0x7c00;
						pinfo->g_mask = 0x3e0;
						pinfo->b_mask = 0x1f;
						pinfo->a_mask = 0x0;
					}
					else
					{
						pinfo->r_mask = 0xf800;
						pinfo->g_mask = 0x7e0;
						pinfo->b_mask = 0x1f;
						pinfo->a_mask = 0x0;
					}
					palcnt = 0;
					break;
				case	24:
					SETMASK_24(pinfo);
					palcnt = 0;
					break;
				case	32:
					SETMASK_32(pinfo);
					palcnt = 0;
					break;
				default:
					assert(0);
					isirw_free_SPEC_DATAPACK(m_pPack);
					m_pPack = psOldPack;
					return ER_NOTSUPPORT;
					break;
				}
				
				pinfo->imgtype     = IMT_RESSTATIC;		/* 图像文件类型 */
				pinfo->imgformat   = IMF_BMP;			/* 图像文件格式（插件标识） */
				pinfo->compression = ICS_RGB;			/* 图像文件的压缩方式 */

				pinfo->width    = (ulong)width;
				pinfo->height   = (ulong)height;
				pinfo->bitcount = (ulong)bitcount;
				pinfo->order    = (binfo.bmi.biHeight > 0) ? 1 : 0;
				
				// 填写行首地址数组
				if (pinfo->order == 0)		/* 正向 */
				{
					for (i=0;i<(int)(pinfo->height);i++)
					{
						pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+(i*linesize));
					}
				}
				else							/* 倒向 */
				{
					for (i=0;i<(int)(pinfo->height);i++)
					{
						pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+((pinfo->height-i-1)*linesize));
					}
				}
				
				pinfo->pal_count = palcnt;				/* 调色板有效项的个数 */
				
				pinfo->play_number = 0;					/* 播放顺序表尺寸（以元素为单位） */
				pinfo->play_order  = NULL;				/* 播放顺序表 */
				pinfo->time        = 0;					/* 首帧停留时间 */
				pinfo->colorkey    = -1;				/* 主图象透明色（-1表示无透明色） */
				
				pinfo->imgnumbers  = 1;					/* 该文件中图像的个数 */
				pinfo->psubimg     = NULL;				/* 子图像数据链地址 */
				pinfo->data_state  = 2;					/* 数据包当前的数据状态 */
				
				m_pPack->command   = IRWE_SDPCOMM_LOAD;	/* 模拟读命令 */
				m_pPack->data_state= SDP_DATA_STATE_ORG_PIX|SDP_DATA_STATE_ORG_INFO|SDP_DATA_STATE_PATH;
				m_pPack->exec_state= IRWE_SDPSTATE_IDLE;
				m_pPack->sn        = (int)type;
				
				if (cnvmark != IRWE_CNVMARK_NOT)
				{
					if (isirw_cnv(m_pPack, (IRWE_CNVMARK)cnvmark, 0) < 0)
					{
						isirw_free_SPEC_DATAPACK(m_pPack);
						m_pPack = psOldPack;
						return ER_MEMORYERR;
					}
				}
				
				eer = ER_SUCCESS;
			}
			else
			{
				eer = ER_MEMORYERR;
			}

			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}

			return eer;
		}
		break;											
	case	CR_HDIB:									// HDIB句柄
		{
			HDIB			hDib;
			wBITMAPINFO*	pbmi;
			char			*pbits = 0;
			
			hDib     = va_arg(argp, HDIB);
			cnvmark	 = va_arg(argp, int);
			
			va_end(argp);
			
			// 保存原有的专业包
			psOldPack = m_pPack;
			
			pbmi  = (wBITMAPINFO*)(char*)hDib;
			pbits = (char*)(((char*)pbmi)+sizeof(wBITMAPINFOHEADER));

			// 判断源数据是否有效
			if (pbmi->bmiHeader.biSize != sizeof(wBITMAPINFOHEADER))
			{
				return	ER_BADIMAGE;
			}

			// 申请并填写专业包
			eer = (enum EXERESULT)_AllocSpecPack(&m_pPack, (wBITMAPINFO*)pbmi, cnvmark, (int)type, pbits, 0);

			if (eer == ER_SUCCESS)
			{
				// 如果存在旧专业包，则先释放它
				if (psOldPack != NULL)
				{
					assert(psOldPack->exec_state == IRWE_SDPSTATE_IDLE);
					isirw_free_SPEC_DATAPACK(psOldPack);
				}
				
				// 类置为读写状态
				m_eStatus = CSISTATUS_IRW;
				// 设当前图象为主图象
				m_pPack->lab1 = 0;
				// 设置显示用信息
				_UpdateDispInfo();
			}
			else
			{
				// 如果读取失败，则恢复旧的数据
				m_pPack = psOldPack;
			}

			return eer;
		}
		break;
	default:
		va_end(argp);
		assert(0);
		break;
	}

	return 0;
}


// 获取已读入图象的各种信息
int CSeeImage::GetInfo(int flag, LPIMAGEINFOSTR pimginfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 获取类的状态（读写或IP状态）
	// 因为该功能可能会在没有图象数据的情况下调用，为了避免触发断言，所以先对它进行判断
	if (pimginfo == NULL)
	{
		if (flag == CSTATUS)
		{
			return (int)m_eStatus;						
		}
		else if (flag == CENGSTATUS)
		{
			return m_bMark;
		}
		else if (flag == CHAVIMG)
		{
			return (m_eStatus == CSISTATUS_NULL) ? 0 : 1;
		}
	}
	else
	{
		if (pimginfo->flag == CSTATUS)
		{
			pimginfo->status = m_eStatus;
			return 0;
		}
		else if (flag == COPENSTR)
		{
			pimginfo->openstr = isirw_get_openstr();
			return 0;
		}
		else if (flag == CENGSTATUS)
		{
			pimginfo->engstatus = m_bMark;
			return 0;
		}
		else if (flag == CHAVIMG)
		{
			pimginfo->havimg = (m_eStatus == CSISTATUS_NULL) ? 0 : 1;
			return 0;
		}
	}

	// 此时引擎必须已经开启
	Assert(m_bMark, "GetInfo()函数－引擎还未开启！");

	if (!m_bMark)
	{
		return 0;
	}
	
	Assert(m_eStatus != CSISTATUS_NULL, "GetInfo()函数－类中必须存在图象数据。");
	Assert(m_pPack&&m_pPack->org_img, "GetInfo()函数－类中必须存在图象数据。");

	LPINFOSTR	pinfo = (m_pPack->cnv_img) ? m_pPack->cnv_img : m_pPack->org_img;

	// 判断是否是获取单个参数
	if (pimginfo == NULL)
	{
		int	result = 0;

		if (flag == CCURIMGNUM)							// 当前图象的序号（0基）
		{
			return (int)m_pPack->lab1;
		}
		else if (flag == CIMGCNT)						// 获取图象的页数
		{
			return (int)m_pPack->org_img->imgnumbers;
		}
		else
		{
			// 如果是获取主图象信息
			if (m_pPack->lab1 == 0)
			{
				switch (flag)
				{
				case	CWIDTH:							// 获取图象宽度
					result = (int)pinfo->width;
					break;
				case	CHEIGHT:						// 获取图象高度
					result = (int)pinfo->height;
					break;
				case	CBITCNT:						// 位深度
					result = (int)pinfo->bitcount;
					break;
				case	CORDER:							// 放置方式（正向或倒向）
					result = (int)pinfo->order;
					break;
				case	CPALCNT:						// 获取调色板项的个数
					result = (int)pinfo->pal_count;
					break;
				default:
					Assert(0, "GetInfo()函数－快捷方式－一次只能获取一个参数。");
					return 0;
					break;
				}
			}
			else
			{
				// 获取当前图象结构的地址
				SUBIMGBLOCK* psub = _CalSubImageAddr(pinfo, m_pPack->lab1);

				assert(psub);

				switch (flag)
				{
				case	CWIDTH:							// 获取图象宽度
					result = (int)psub->width;
					break;
				case	CHEIGHT:						// 获取图象高度
					result = (int)psub->height;
					break;
				case	CBITCNT:						// 位深度
					result = (int)psub->bitcount;
					break;
				case	CORDER:							// 放置方式（正向或倒向）
					result = (int)psub->order;
					break;
				case	CPALCNT:						// 获取调色板项的个数
					result = (int)psub->pal_count;
					break;
				default:
					Assert(0, "GetInfo()函数－快捷方式－一次只能获取一个参数。");
					return 0;
					break;
				}
			}

			return result;
		}
	}
	else
	{
		flag = pimginfo->flag;

		// 初始化结构中各变量的值（flag成员变量除外）
		memset((void*)(((char*)pimginfo)+sizeof(ulong)), 0, sizeof(IMAGEINFOSTR)-sizeof(ulong));

		if (flag & CCURIMGNUM)							// 当前图象的序号（0基）
		{
			pimginfo->currimgnum = (int)m_pPack->lab1;
		}

		if (flag & CSTATUS)								// 获取类的状态（读写或IP状态）
		{
			pimginfo->status = m_eStatus;
		}

		if (flag & CIMGCNT)								// 获取图象的页数
		{
			pimginfo->imgcnt = (int)m_pPack->org_img->imgnumbers;
		}

		if (flag & CPACKPTR)							// 获取专业包地址
		{
			pimginfo->pack = m_pPack;
		}

		if (flag & COPENSTR)							// 获取引擎open串
		{
			pimginfo->openstr = isirw_get_openstr();
		}

		if (flag == CENGSTATUS)							// 获取引擎状态
		{
			pimginfo->engstatus = m_bMark;
		}
		
		// 如果是获取主图象信息
		if (m_pPack->lab1 == 0)
		{
			if (flag & CWIDTH)							// 获取图象宽度
			{
				pimginfo->width = pinfo->width;
			}

			if (flag & CHEIGHT)							// 获取图象高度
			{
				pimginfo->height = pinfo->height;
			}

			if (flag & CBITCNT)							// 位深度
			{
				pimginfo->bitcount = pinfo->bitcount;
			}

			if (flag & CORDER)							// 放置方式
			{
				pimginfo->order = pinfo->order;
			}

			if (flag & CPALCNT)							// 获取调色板项的个数
			{
				pimginfo->pal_count = pinfo->pal_count;
			}

			if (flag & CPALETTE)						// 获取调色板数据
			{
				memcpy((void*)pimginfo->palette, (const void *)pinfo->palette, sizeof(ulong)*pinfo->pal_count);
			}

			if (flag & CBITS)							// 获取象素缓冲区首地址
			{
				pimginfo->pbits = (char*)pinfo->p_bit_data;
			}
		}
		else
		{
			// 获取当前图象结构的地址
			SUBIMGBLOCK* psub = _CalSubImageAddr(pinfo, m_pPack->lab1);
			
			assert(psub);

			if (flag & CWIDTH)							// 获取图象宽度
			{
				pimginfo->width = psub->width;
			}
			
			if (flag & CHEIGHT)							// 获取图象高度
			{
				pimginfo->height = psub->height;
			}
			
			if (flag & CBITCNT)							// 位深度
			{
				pimginfo->bitcount = psub->bitcount;
			}
			
			if (flag & CORDER)							// 放置方式
			{
				pimginfo->order = psub->order;
			}

			if (flag & CPALCNT)							// 获取调色板项的个数
			{
				pimginfo->pal_count = psub->pal_count;
			}
			
			if (flag & CPALETTE)						// 获取调色板数据
			{
				memcpy((void*)pimginfo->palette, (const void *)psub->palette, sizeof(ulong)*psub->pal_count);
			}

			if (flag & CBITS)							// 获取象素缓冲区首地址
			{
				pimginfo->pbits = (char*)psub->p_bit_data;
			}
		}
	}

	return 0;
}


// 保存类中的图象到指定位置
int CSeeImage::Save(CSITYPEW type, ...)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 引擎必须已经开启
	Assert(m_bMark, "Save()函数－引擎还未开启！");

	if (!m_bMark)
	{
		return 0;
	}
	
	// 类中必须已经有图象数据
	Assert((m_eStatus!=CSISTATUS_NULL)&&m_pPack&&m_pPack->org_img, "Save函数－类中必须存在图象数据。");

	va_list			argp;
	LPINFOSTR		pOldInfo = NULL;
	int				result   = (int)ER_SUCCESS;
	
	va_start(argp, type);

	// 入口参数类型检查
	TYPESAFE_CHECK(CSIPARMCHK_SAVE);

	// 如果类打开过IP模式，则保存IP缓冲区的图象，用户可能对它进行过修改。
	if (m_bIsChange)
	{
		assert(m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));
		
		// 用转换包（IP缓冲区）顶替原始图象包
		pOldInfo = m_pPack->org_img;
		m_pPack->org_img = m_pPack->cnv_img;
		m_pPack->cnv_img = NULL;
		
		m_pPack->data_state &= ~SDP_DATA_STATE_CNV;
	}
	
	switch (type)
	{
	case	CW_FILE:									// 本地文件类型
	case	CW_FILEFAST:								// 本地文件类型（快速版）
		{
			char			*fname;
			CSAVETYPE		bitcount;
			int				par1;
			int				par2;
			
			fname    = va_arg(argp, char*);
			bitcount = va_arg(argp, CSAVETYPE);
			par1     = va_arg(argp, int);
			par2     = va_arg(argp, int);
			
			va_end(argp);

			// 如果没有指定目标文件名，则使用对话框
			if (fname == NULL)
			{
				CFileDialogEx	dsa(m_pPack);

				// 使用定制的‘另存为’对话框保存图象
				result = dsa.DoModal();

				if (result != IDOK)
				{
					result = ER_USERBREAK;
				}
				else
				{
					result = ER_SUCCESS;
				}
			}
			else
			{
				SAVESTR			savestr;

				// 设置保存参数
				savestr.para_value[0] = par1;
				savestr.para_value[1] = par2;
				
				// 保存图象
				result = _SaveSpecPack((ISEEIO_CONFER_TYPE)type, fname, 0, 0, bitcount, &savestr);
			}
		}
		break;
	case	CW_MEMORY:									// 本地内存类型
		{
			char			*pname;
			char			**ppaddr;
			int				*pimgsize;
			CSAVETYPE		bitcount;
			int				par1;
			int				par2;
			
			pname    = va_arg(argp, char*);
			ppaddr   = va_arg(argp, char**);
			pimgsize = va_arg(argp, int*);
			bitcount = va_arg(argp, CSAVETYPE);
			par1     = va_arg(argp, int);
			par2     = va_arg(argp, int);
			
			va_end(argp);
			
			SAVESTR			savestr;
			
			// 设置保存参数
			savestr.para_value[0] = par1;
			savestr.para_value[1] = par2;
			
			// 保存图象
			result = _SaveSpecPack((ISEEIO_CONFER_TYPE)type, pname, (ulong)0, (ulong)0, bitcount, &savestr);

			if (result == ER_SUCCESS)
			{
				// 回写内存块的首地址及图象尺寸（该内存块必须用ImFree()接口函数释放）
				*ppaddr   = (char*)m_pPack->despath->param1;
				*pimgsize = (int)m_pPack->despath->param2;
				assert((*ppaddr)&&(*pimgsize));
			}
			else
			{
				*ppaddr   = NULL;
				*pimgsize = 0;
			}
		}
		break;
	case	CW_CLIP:									// 剪贴板
		{
			LPRECT	prc;

			prc = va_arg(argp, LPRECT);

			va_end(argp);

			// 转换包优先
			LPINFOSTR	pinfo = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;
			CRect		rc;

			if (prc != NULL)
			{
				// 对于1、4位深的图象，不支持局部粘贴
				assert(pinfo->bitcount >= 8);
				rc.SetRect(prc->left, prc->top, prc->right, prc->bottom);
			}
			else
			{
				rc.SetRect(0, 0, pinfo->width, pinfo->height);
			}

			rc.NormalizeRect();

			// 矩形必须合法
			assert(pinfo);
			Assert((rc.left<(int)pinfo->width)&&(rc.top<(int)pinfo->height), "Save函数－CLIP类型－传入的矩形坐标必须合法。");
			Assert((rc.Width()>0)&&(rc.Height()>0), "Save函数－CLIP类型－传入的矩形坐标必须合法。");

			// 剪裁矩形
			rc.right  = min(rc.right, (int)pinfo->width);
			rc.bottom = min(rc.bottom, (int)pinfo->height);

			int			linesize = DIBSCANLINE_DWORDALIGN(rc.Width()*pinfo->bitcount);
			int			imgsize  = linesize*rc.Height();
			int			headsize = sizeof(wBITMAPINFOHEADER);
			int			colssize = 0;
			int			bpp      = 0;

			// 确定调色板尺寸
			switch (pinfo->bitcount)
			{
			case	1:
			case	4:
			case	8:
				colssize = (1<<pinfo->bitcount)*sizeof(wRGBQUAD);
				bpp      = 1;
				break;
			case	16:
				colssize = 0;
				bpp      = 2;
				break;
			case	24:
				colssize = 0;
				bpp      = 3;
				break;
			case	32:
				colssize = 0;
				bpp      = 4;
				break;
			default:
				assert(0);
				break;
			}

			HGLOBAL			hData = ::GlobalAlloc(GHND, headsize+colssize+imgsize);
			if (hData == NULL)
			{
				result = ER_MEMORYERR;
				break;
			}

			char			*pda = (char*)::GlobalLock(hData);
			if (pda == NULL)
			{
				::GlobalFree(hData);
				result = ER_MEMORYERR;
				break;
			}

			wBITMAPINFO*	pbmi = (wBITMAPINFO*)pda;
			char			*pbit= pda+headsize+colssize;
			char			*pcpy;
			int				y;

			// 制作DIB信息头结构
			pbmi->bmiHeader.biSize          = sizeof(wBITMAPINFOHEADER);
			pbmi->bmiHeader.biWidth         = rc.Width();
			pbmi->bmiHeader.biHeight        = rc.Height();				// 固定保存为倒向图
			pbmi->bmiHeader.biPlanes        = 1;
			pbmi->bmiHeader.biBitCount      = (WORD)pinfo->bitcount;
			pbmi->bmiHeader.biCompression   = BI_RGB;
			pbmi->bmiHeader.biSizeImage     = 0;
			pbmi->bmiHeader.biXPelsPerMeter = 0;
			pbmi->bmiHeader.biYPelsPerMeter = 0;
			pbmi->bmiHeader.biClrUsed       = 0;
			pbmi->bmiHeader.biClrImportant  = 0;
			
			// 拷贝调色板数据
			if (pinfo->pal_count > 0)
			{
				memcpy((void*)pbmi->bmiColors, (const void *)pinfo->palette, sizeof(wRGBQUAD)*pinfo->pal_count);
			}
			
			// 复制象素数据
			for (y=rc.top; y<rc.bottom; y++)
			{
				pcpy = (char*)pinfo->pp_line_addr[y] + rc.left*bpp;
				memcpy((void*)(pbit+(rc.bottom-1-y)*linesize), (const void *)pcpy, rc.Width()*bpp);
			}

			// 内存块解锁（准备帖往剪贴板）
			::GlobalUnlock(hData);

			// 打开剪贴板
			if (::OpenClipboard(NULL))
			{
				// 清除现有数据
				if (::EmptyClipboard())
				{
					// 将图象发往剪贴板
					if (::SetClipboardData(CF_DIB, hData) == NULL)
					{
						result = ER_SYSERR;
					}
					else
					{
						result = ER_SUCCESS;			// 发送成功
					}
				}
				else
				{
					result = ER_SYSERR;
				}

				::CloseClipboard();
			}
			else
			{
				result = ER_SYSERR;
			}

			// 如果发送失败，则释放以前申请的内存
			if (result != ER_SUCCESS)
			{
				::GlobalFree(hData);
			}
		}
		break;
	case	CW_HBITMAP:									// HBITMAP句柄
		{
			HBITMAP		*phb;

			phb = va_arg(argp, HBITMAP *);
			
			va_end(argp);
			
			HWND	hWnd = ::GetDesktopWindow();
			HDC		hDC  = ::GetDC(hWnd);
			wBITMAPINFOHEADER	bih;
			DIBINFO				binfo;
			HBITMAP				bmp;

			if ((hWnd == NULL)||(hDC == NULL))
			{
				result = ER_SYSERR;
				*phb   = NULL;
				break;
			}

			LPINFOSTR	pinfo = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;

			// 制作目标位图信息头结构
			bih.biSize          = sizeof(wBITMAPINFOHEADER);
			bih.biWidth         = pinfo->width;
			bih.biHeight        = (pinfo->order == 0) ? -(long)pinfo->height : pinfo->height;
			bih.biPlanes        = 1;
			bih.biBitCount      = (WORD)pinfo->bitcount;
			bih.biCompression   = BI_RGB;
			bih.biSizeImage     = 0;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed       = 0;
			bih.biClrImportant  = 0;
			
			memcpy((void*)&binfo.bmi, (const void *)&bih, sizeof(wBITMAPINFOHEADER));

			// 拷贝调色板数据
			if (pinfo->pal_count > 0)
			{
				memcpy((void*)binfo.pal, (const void *)pinfo->palette, sizeof(wRGBQUAD)*pinfo->pal_count);
			}
			
			// 创建DDB位图
			bmp = ::CreateDIBitmap(hDC, &bih, CBM_INIT, (const void *)pinfo->p_bit_data, (CONST BITMAPINFO *)&binfo, DIB_RGB_COLORS);

			::ReleaseDC(hWnd, hDC);

			if (bmp != NULL)
			{
				*phb = bmp;
				result = ER_SUCCESS;
			}
			else
			{
				*phb = NULL;
				result = ER_SYSERR;
			}
		}
		break;
	case	CW_HDIB:									// HDIB句柄
		{
			HDIB	*phdib;

			phdib = va_arg(argp, HDIB *);

			va_end(argp);

			// 转换包优先
			LPINFOSTR	pinfo = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;

			assert(pinfo);

			int			linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
			int			imgsize  = linesize*pinfo->height;
			int			headsize = sizeof(wBITMAPINFOHEADER);
			int			colssize = 0;

			// 确定调色板尺寸
			if ((pinfo->bitcount==1)||(pinfo->bitcount==4)||(pinfo->bitcount==8))
			{
				colssize = (1<<pinfo->bitcount)*sizeof(wRGBQUAD);
			}
			else
			{
				colssize = 0;
			}

			char	*pimg = (char*)ImAlloc(headsize+colssize+imgsize);

			if (pimg == NULL)
			{
				result = ER_MEMORYERR;
				*phdib = NULL;
				break;
			}

			wBITMAPINFO*	pbmi = (wBITMAPINFO*)pimg;
			char			*pbit= pimg+headsize+colssize;
			char			*pcpy;
			int				y;

			// 制作DIB信息头结构
			pbmi->bmiHeader.biSize          = sizeof(wBITMAPINFOHEADER);
			pbmi->bmiHeader.biWidth         = pinfo->width;
			pbmi->bmiHeader.biHeight        = pinfo->height;	// 固定保存为倒向图
			pbmi->bmiHeader.biPlanes        = 1;
			pbmi->bmiHeader.biBitCount      = (WORD)pinfo->bitcount;
			pbmi->bmiHeader.biCompression   = BI_RGB;
			pbmi->bmiHeader.biSizeImage     = 0;
			pbmi->bmiHeader.biXPelsPerMeter = 0;
			pbmi->bmiHeader.biYPelsPerMeter = 0;
			pbmi->bmiHeader.biClrUsed       = 0;
			pbmi->bmiHeader.biClrImportant  = 0;
			
			// 拷贝调色板数据
			if (pinfo->pal_count > 0)
			{
				memcpy((void*)pbmi->bmiColors, (const void *)pinfo->palette, sizeof(wRGBQUAD)*pinfo->pal_count);
			}
			
			// 复制象素数据
			for (y=0; y<(int)pinfo->height; y++)
			{
				pcpy = (char*)pinfo->pp_line_addr[y];
				memcpy((void*)(pbit+(pinfo->height-1-y)*linesize), (const void *)pcpy, linesize);
			}

			// 传回DIB数据块
			*phdib = pimg;

			result = ER_SUCCESS;
		}
		break;
	default:
		assert(0);
		break;
	}

	if (m_bIsChange)
	{
		m_pPack->cnv_img = m_pPack->org_img;
		m_pPack->org_img = pOldInfo;
		
		m_pPack->data_state |= SDP_DATA_STATE_CNV;
	}
	
	return result;
}


// 将类中的图象以指定的方式及坐标绘制在指定的窗口中
int CSeeImage::Draw(CSIDRAW type, ...)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 引擎必须已经开启
	Assert(m_bMark, "Draw()函数－引擎还未开启！");

	if (!m_bMark)
	{
		return 0;
	}
	
	// 如果类中没有图象，则直接返回
	if (m_eStatus == CSISTATUS_NULL)
	{
		return 0;
	}

	assert(m_pPack&&m_pPack->org_img);
	
	va_list			argp;
	LPINFOSTR		pinfo  = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;
	
	va_start(argp, type);
	
	// 入口参数类型检查
	
	switch (type)
	{
	case	CD_NON:										// 正常绘制
		{
			HDC	hdc;
			int	x, y;

			hdc = va_arg(argp, HDC);
			x   = va_arg(argp, int);
			y   = va_arg(argp, int);
			
			va_end(argp);

			if (m_hDrawDib&&(pinfo->order==1))
			{
				DrawDibDraw(m_hDrawDib, hdc, 
					x, y, pinfo->width, pinfo->height,
					(wBITMAPINFOHEADER*)&m_cDispInfo, 
					(LPVOID)pinfo->p_bit_data, 
					0, 0, pinfo->width, pinfo->height, 
					DDF_HALFTONE);
			}
			else
			{	
				StretchDIBits(hdc, 
					x, y, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
					0, 0, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
					pinfo->p_bit_data, (wBITMAPINFO*)&m_cDispInfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}
		break;
	case	CD_STL:										// 拉伸绘制
		{
			HDC	hdc;
			int	x, y, w, h;
			
			hdc = va_arg(argp, HDC);
			x   = va_arg(argp, int);
			y   = va_arg(argp, int);
			w   = va_arg(argp, int);
			h   = va_arg(argp, int);
			
			va_end(argp);
			
			if (m_hDrawDib&&(pinfo->order==1))
			{
				DrawDibDraw(m_hDrawDib, hdc, 
					x, y, w, h,
					(wBITMAPINFOHEADER*)&m_cDispInfo, 
					(LPVOID)pinfo->p_bit_data, 
					0, 0, pinfo->width, pinfo->height, 
					DDF_HALFTONE);
			}
			else
			{	
				StretchDIBits(hdc, 
				x, y, w, h, 
				0, 0, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
				pinfo->p_bit_data, (wBITMAPINFO*)&m_cDispInfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}
		break;
	case	CD_CEN:										// 居中、正常绘制
		{
			HDC	hdc;
			int	x, y, w, h;
			
			hdc = va_arg(argp, HDC);
			x   = va_arg(argp, int);
			y   = va_arg(argp, int);
			w   = va_arg(argp, int);
			h   = va_arg(argp, int);
			
			va_end(argp);
			
			x = x+w/2-m_cDispInfo.bmi.biWidth/2;
			y = y+h/2-abs(m_cDispInfo.bmi.biHeight)/2;
			
			StretchDIBits(hdc, 
				x, y, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
				0, 0, m_cDispInfo.bmi.biWidth, abs(m_cDispInfo.bmi.biHeight), 
				pinfo->p_bit_data, (wBITMAPINFO*)&m_cDispInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		break;
	case	CD_TAN:										// 透明、正常绘制
		break;
	default:
		break;
	}

	return 0;
}


// 重定义赋值操作符
CSeeImage& CSeeImage::operator=(CSeeImage&sou)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// 引擎必须已经开启
	Assert(m_bMark, "OpenIPMode()函数－引擎还未开启！");
	
	if (!m_bMark)
	{
		return *this;
	}

	// 检查类中是否有图象
	if (m_eStatus != CSISTATUS_NULL)
	{
		if (m_eStatus == CSISTATUS_IP)
		{
			CloseIPMode();
		}

		assert(m_pPack != NULL);
		
		// 如果有则是否原有图象
		isirw_free_SPEC_DATAPACK(m_pPack);

		m_eStatus   = CSISTATUS_NULL;
		m_pPack     = NULL;
		m_bIsChange	= false;
		
		memset((void*)&m_cDispInfo, 0, sizeof(DIBINFO));
	}

	assert(m_eStatus == CSISTATUS_NULL);

	// 复制数据
	m_pPack     = sou.m_pPack;							// 专业数据包指针
	m_eStatus   = sou.m_eStatus;						// 类状态标记
	m_bIsChange = sou.m_bIsChange;						// 是否打开过图象IP模式

	// 显示用信息
	memcpy((void*)&m_cDispInfo, (const void *)&sou.m_cDispInfo, sizeof(DIBINFO));
	
	// 源类置空
	sou.m_pPack     = NULL;
	sou.m_eStatus   = CSISTATUS_NULL;
	sou.m_bIsChange = false;
	memset((void*)&sou.m_cDispInfo, 0, sizeof(DIBINFO));

	return *this;
}


// 清除类中的图象（如果有的话）
void CSeeImage::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 引擎必须已经开启
	Assert(m_bMark, "OpenIPMode()函数－引擎还未开启！");
	
	if (!m_bMark)
	{
		return;
	}

	Assert(m_uMagic == CSEEIMAGE_MAGIC, "Clear函数－类内存遭到破坏！");
	
	if (m_pPack != NULL)
	{
		if (m_eStatus == CSISTATUS_IP)
		{
			CloseIPMode();
		}

		// 释放专业数据包
		isirw_free_SPEC_DATAPACK(m_pPack);

		m_pPack = NULL;
	}

	// 类置空
	m_eStatus   = CSISTATUS_NULL;
	m_bIsChange = false;
	memset((void*)&m_cDispInfo, 0, sizeof(DIBINFO));

	return;
}


int CSeeImage::OpenIPMode()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 引擎必须已经开启
	Assert(m_bMark, "OpenIPMode()函数－引擎还未开启！");

	if (!m_bMark)
	{
		return 0;
	}
	
	// 检查类中是否有图象
	if (m_eStatus == CSISTATUS_NULL)
	{
		assert(m_pPack == NULL);

		return 1;										// 类中还没有图象数据
	}

	assert((m_pPack != NULL)&&(m_pPack->org_img != NULL));

	// 检查是否已处于IP(Image Process)模式
	if (m_eStatus == CSISTATUS_IP)
	{
		assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));
		assert(m_bIsChange == true);

		return 2;										// 已处于IP模式
	}

	m_eStatus = CSISTATUS_IP;

	LPINFOSTR		poldcnv = NULL;
	IRWE_CNVMARK	oldcnvmark;

	if (m_pPack->cnv_img != NULL)
	{
		// 如果存在32位的转换包，则直接使用它
		if (m_pPack->cnv_img->bitcount == IP_BITCNT)
		{
			m_bIsChange = true;
			return 0;
		}
		else
		{
			// 如果是非32位转换包，则先保存它
			poldcnv          = m_pPack->cnv_img;
			m_pPack->cnv_img = NULL;
			oldcnvmark       = m_pPack->cnvmark;
		}
	}

	// 申请IP缓冲区（32位转换包）
	switch (isirw_cnv(m_pPack, IRWE_CNVMARK_32, 1))
	{
	case	-1:											// 操作系统异常，无法转入IP模式
		if (poldcnv != NULL)
		{
			m_pPack->cnv_img = poldcnv;
			m_pPack->cnvmark = oldcnvmark;
		}
		m_eStatus = CSISTATUS_IRW;
		return 3;
	case	-2:											// 内存不足		
		if (poldcnv != NULL)
		{
			m_pPack->cnv_img = poldcnv;
			m_pPack->cnvmark = oldcnvmark;
		}
		m_eStatus = CSISTATUS_IRW;
		return 4;
	case	0:											// 成功
		if (poldcnv != NULL)
		{
			isirw_free_INFOSTR(poldcnv);
		}

		_UpdateDispInfo();
		break;
	default:
		assert(0);										// 不可能再出现别的返回值
		break;
	}

	m_bIsChange = true;

	return 0;
}


// 获得指定象素行的起始地址
char* CSeeImage::IP_GetScanLineAddr(int y)
{
	// 引擎必须已经开启，并且必须已处于IP模式
	Assert(m_bMark, "IP_GetScanLineAddr()函数－引擎还未开启！");

	if (!m_bMark)
	{
		return 0;
	}
	
	Assert(m_eStatus == CSISTATUS_IP, "IP_GetScanLineAddr()函数－类并未进入IP模式。");
	assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));

	// y参数的值不能超过图象的高度
	Assert((y>=0)&&(y<(int)m_pPack->cnv_img->height), "IP_GetScanLineAddr()函数－给出的宽度或高度值非法。");

	return (char*)(m_pPack->cnv_img->pp_line_addr[y]);
}


// 获取指定位置的象素值
ulong CSeeImage::IP_GetPixel(int x, int y)
{
	// 引擎必须已经开启，并且必须已处于IP模式
	Assert(m_bMark, "IP_GetPixel()函数－引擎还未开启！");

	if (!m_bMark)
	{
		return 0;
	}
	
	Assert(m_eStatus == CSISTATUS_IP, "IP_GetPixel()函数－类并未进入IP模式。");
	assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));
	
	// x、y的参数必须合法
	Assert((y>=0)&&(y<(int)m_pPack->cnv_img->height), "IP_GetPixel()函数－指定的y值非法！");
	Assert((x>=0)&&(x<(int)m_pPack->cnv_img->width), "IP_GetPixel()函数－指定的x值非法！");

	return ((ulong *)(m_pPack->cnv_img->pp_line_addr[y]))[x];
}


// 设置指定位置的象素值
ulong CSeeImage::IP_SetPixel(int x, int y, ulong newpix)
{
	// 引擎必须已经开启，并且必须已处于IP模式
	Assert(m_bMark, "IP_SetPixel()函数－引擎还未开启！");

	if (!m_bMark)
	{
		return 0;
	}
	
	Assert(m_eStatus == CSISTATUS_IP, "IP_SetPixel()函数－类并未进入IP模式。");
	assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));
	
	// x、y的参数必须合法
	Assert((y>=0)&&(y<(int)m_pPack->cnv_img->height), "IP_SetPixel()函数－指定的y值非法！");
	Assert((x>=0)&&(x<(int)m_pPack->cnv_img->width), "IP_SetPixel()函数－指定的x值非法！");
	
	ulong	*pl = (ulong *)(m_pPack->cnv_img->pp_line_addr[y]);
	ulong	old = pl[x];

	pl[x] = newpix;

	// 返回旧的象素数据
	return old;
}


// 关闭IP模式（转换到读写模式）
int CSeeImage::CloseIPMode()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	// 引擎必须已经开启
	Assert(m_bMark, "CloseIPMode()函数－引擎还未开启！");
	
	if (!m_bMark)
	{
		return 0;
	}

	// 检查是否处于IP模式
	if (m_eStatus == CSISTATUS_IP)
	{
		assert(m_pPack&&m_pPack->cnv_img&&(m_pPack->cnv_img->bitcount==IP_BITCNT));

		// 只是简单的设置标识为读写状态
		m_eStatus = CSISTATUS_IRW;
		
		return 0;
	}
	
	// 类并未处于IP模式
	return 1;
}


// ISee内存防护系统内存申请接口
void *CSeeImage::ImAlloc(size_t size)
{
	Assert(size > 0, "ImAlloc函数－待分配内存的尺寸不能小于等于0。");
	return isirw_malloc(size);
}


// ISee内存防护系统内存释放接口
void *CSeeImage::ImFree(void *p)
{
	if (p == NULL)
	{
		return p;
	}

	isirw_free(p);

	return NULL;
}





////////////////////////////////////////////////////////////////////////////////////
// 内用接口函数

// 参数类型的安全检测（只适用于调试版）
//
// 注：虽然可变长参数机制提供了强大的灵活度，但也丧失了C++语言的参数类型检查，所以
//     加入此函数用于弥补这方面的不足。该函数只在调试版状态下有效，不会对最终的发布
//     版造成任何性能上的损失。
//
int CSeeImage::_typesafe_check(CSIPARMCHK pct, int type, va_list ap)
{
	va_list			argp = ap;
	int				cnvmark;
	
	assert(argp);

	switch (pct)
	{
	case	CSIPARMCHK_LOAD:							// 读函数
		switch (type)
		{
		case	CR_FILE:								// 本地文件类型
		case	CR_FILEFAST:							// 本地文件类型（快速版）
			{
				char	*fname;
				
				fname   = va_arg(argp, char*);
				cnvmark	= va_arg(argp, int);

				if (fname != NULL)
				{
					Assert(lstrlen((LPCSTR)fname) < MAX_PATH, "Load()函数－FILE类型－第一个参数必须是有效的文件路径。");
					Assert(fname[1] == ':', "Load()函数－FILE类型－第一个参数必须是文件全路径。");
				}

				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_MEMORY:								// 本地内存类型
			{
				char			*mname;
				char			*addr;
				ulong	len;
				LPFGUARDSTRUCT	pfdt;
				
				mname   = va_arg(argp, char*);
				addr	= va_arg(argp, char*);
				len		= va_arg(argp, ulong);
				cnvmark	= va_arg(argp, int);

				// 内存图象必须起一个有效的名字
				Assert((mname != NULL)&&(lstrlen((LPCSTR)mname)<MAX_PATH), "Load()函数－MEMORY类型－第一个参数必须指向一个有效的内存图象名。");
				// 必须给出内存图象的首地址
				Assert(addr != NULL, "Load()函数－MEMORY类型－第二个参数必须给出存放图象数据的内存块首地址。");

				pfdt = (LPFGUARDSTRUCT)(addr-sizeof(FGUARDSTRUCT));
				// 内存必须是由CSeeImage::ImAlloc()函数分配的
				Assert(pfdt->magic == IM_GUARD_MAGIC, "Load()函数－MEMORY类型－第二个参数指定的内存块必须是由CSeeImage::ImAlloc()函数分配的。");

				// 图象长度不能为0
				Assert(len > 0, "Load()函数－MEMORY类型－第三个参数的值不能为0。");
				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_SEEIMAGE:							// 另一个CSeeImage对象
			{
				CSeeImage	*pSou;
				
				pSou    = va_arg(argp, CSeeImage*);
				cnvmark	= va_arg(argp, int);

				// 必须给出一个有效的源CSeeImage对象
				Assert((pSou != NULL)&&(pSou->m_uMagic == CSEEIMAGE_MAGIC), "Load()函数－SEEIMAGE类型－第一个参数必须指向一个有效的CSeeImage对象。");
				// 源对象中必须存在图象数据
				Assert(pSou->m_eStatus != CSISTATUS_NULL, "Load()函数－SEEIMAGE类型－第一个参数指向的CSeeImage对象中必须存在图象数据。");

				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_NIHILITY:							// 虚无（即创建）
			{
				int		width;
				int		height;
				int		bitcount;
				
				width   = va_arg(argp, int);
				height	= va_arg(argp, int);
				bitcount= va_arg(argp, int);
				cnvmark	= va_arg(argp, int);

				// 待创建图象的宽度、高度不能为0
				Assert(width&&height, "Load()函数－NIHILITY类型－不能创建0宽度或0高度的图象。");
				
				// 检验位深度合法性
				switch (bitcount)
				{
				case	1:
				case	4:
				case	8:
				case	16:
				case	24:
				case	32:
					break;
				default:
					Assert(0, "Load()函数－NIHILITY类型－第三个参数的位深度值非法（只能是1、4、8、16、24、32其中之一）。");
					break;
				}

				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_CLIP:								// 剪贴板
			{
				cnvmark	= va_arg(argp, int);
				
				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_WINDOW:								// 窗口
			{
				HWND	hWnd;
				
				hWnd    = va_arg(argp, HWND);
				cnvmark	= va_arg(argp, int);

				// 检验句柄有效性
				Assert(::IsWindow(hWnd), "Load()函数－WINDOW类型－第一个参数必须是一个有效的窗口句柄。");

				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_RESOURCE_ID:							// 资源（以ID方式给出）
			{
				char	*filename;						// 资源文件全路径（EXE、DLL）
				int		id;
				
				filename = va_arg(argp, char*);
				id       = va_arg(argp, int);
				cnvmark	 = va_arg(argp, int);

				// 检测文件路径参数
				if (filename != NULL)
				{
					Assert((lstrlen((LPCSTR)filename)<MAX_PATH)&&(filename[1]==':'), "Load()函数－RESOURCE_ID类型－第一个参数必须是有效的资源文件全路径。");
				}

				// 检测id的合法性
				Assert((id&0x7fffffff)<0xffff, "Load()函数－RESOURCE_ID类型－第二个参数必须是有效的ID或枚举值。");

				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_RESOURCE_NAME:						// 资源（以资源名称串方式给出）
			{
				char	*resname;						// 资源名
				
				resname  = va_arg(argp, char*);
				cnvmark	 = va_arg(argp, int);

				// 资源名称不能为空
				Assert(resname!=NULL, "Load()函数－RESOURCE_NAME类型－第一个参数不能为NULL。");

				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_HBITMAP:								// HBITMAP句柄
			{
				HBITMAP	hBmp;							// 源位图句柄
				
				hBmp     = va_arg(argp, HBITMAP);
				cnvmark	 = va_arg(argp, int);

				// 不能指定空位图句柄
				Assert(hBmp!=NULL, "Load()函数－HBITMAP类型－第一个参数的位图句柄不能为NULL。");

				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		case	CR_HDIB:								// HDIB句柄
			{
				HDIB			hDib;
				
				hDib     = va_arg(argp, HDIB);
				cnvmark	 = va_arg(argp, int);
				
				// 不能指定空HDIB句柄
				Assert(hDib!=NULL, "Load()函数－HDIB类型－第一个参数的HDIB句柄不能为NULL。");
				
				// 检测结尾参数的合法性
				CNVTYPE_CHECK(cnvmark);
			}
			break;
		default:
			Assert(0, "Load()函数－指定了非法的操作类型！");
			break;
		}
		break;
	case	CSIPARMCHK_DRAW:							// 绘制函数
		switch (type)
		{
		case	CD_NON:									// 原尺寸绘制
			{
				HDC	hdc;
				int	x, y;
				
				hdc = va_arg(argp, HDC);
				x   = va_arg(argp, int);
				y   = va_arg(argp, int);

				// 不能指定空HDC
				Assert(hdc!=NULL, "Draw()函数－NON类型－第一个参数的HDC句柄不能为NULL。");
			}
			break;
		case	CD_TAN:									// 透明绘制
			{
			}
			break;
		case	CD_STL:									// 拉伸
			{
				HDC	hdc;
				int	x, y, w, h;
				
				hdc = va_arg(argp, HDC);
				x   = va_arg(argp, int);
				y   = va_arg(argp, int);
				w   = va_arg(argp, int);
				h   = va_arg(argp, int);

				Assert(hdc!=NULL, "Draw()函数－STL类型－第一个参数的HDC句柄不能为NULL。");
			}
			break;
		case	CD_CEN:									// 居中
			{
				HDC	hdc;
				int	x, y, w, h;
				
				hdc = va_arg(argp, HDC);
				x   = va_arg(argp, int);
				y   = va_arg(argp, int);
				w   = va_arg(argp, int);
				h   = va_arg(argp, int);
				
				Assert(hdc!=NULL, "Draw()函数－CEN类型－第一个参数的HDC句柄不能为NULL。");
			}
			break;
		default:
			Assert(0, "Draw()函数－指定了非法的操作类型！");
			break;
		}
		break;
	case	CSIPARMCHK_SAVE:							// 保存函数
		switch (type)
		{
		case	CW_FILE:								// 本地文件类型
		case	CW_FILEFAST:							// 本地文件类型（快速版）
			{
				char			*fname;
				CSAVETYPE		bitcount;
				int				par1;
				int				par2;
				
				fname    = va_arg(argp, char*);
				bitcount = va_arg(argp, CSAVETYPE);
				par1     = va_arg(argp, int);
				par2     = va_arg(argp, int);

				if (fname != NULL)
				{
					// 检验文件串的合法性
					Assert(lstrlen((LPCSTR)fname)<MAX_PATH, "Save()函数－FILE类型－第一个参数必须指向一个有效的文件路径串。");
					Assert(fname[1]==':', "Save()函数－FILE类型－第一个参数必须是一个文件的全路径串。");

					// 校验位深度标志的合法性
					switch (bitcount)
					{
					case	CSAVEAUTO:
					case	CSAVE1:
					case	CSAVE4:
					case	CSAVE8:
					case	CSAVE16555:
					case	CSAVE16565:
					case	CSAVE24:
					case	CSAVE32:
						break;
					default:
						Assert(0, "Save()函数－FILE类型－第二个参数值非法！");
						break;
					}
				}
			}
			break;
		case	CW_MEMORY:								// 本地内存类型
			{
				char			*pname;
				char			**ppaddr;
				int				*pimgsize;
				CSAVETYPE		bitcount;
				int				par1;
				int				par2;
				
				pname    = va_arg(argp, char*);
				ppaddr   = va_arg(argp, char**);
				pimgsize = va_arg(argp, int*);
				bitcount = va_arg(argp, CSAVETYPE);
				par1     = va_arg(argp, int);
				par2     = va_arg(argp, int);

				// 检验虚拟内存图象名称是否存在
				Assert(pname!=NULL, "Save()函数－MEMORY类型－第一个参数必须指向一个内存图象名称。");
				Assert(lstrlen((LPCSTR)pname)<MAX_PATH, "Save()函数－MEMORY类型－第一个参数必须指向一个‘正规’的图象名称。");

				// 接收内存地址及长度的变量不能为NULL
				Assert(ppaddr&&pimgsize, "Save()函数－MEMORY类型－第二、三个参数不能为NULL。");

				// 校验位深度标志的合法性
				switch (bitcount)
				{
				case	CSAVEAUTO:
				case	CSAVE1:
				case	CSAVE4:
				case	CSAVE8:
				case	CSAVE16555:
				case	CSAVE16565:
				case	CSAVE24:
				case	CSAVE32:
					break;
				default:
					Assert(0, "Save()函数－MEMORY类型－第四个参数值非法！");
					break;
				}
			}
			break;
		case	CW_CLIP:								// 剪贴板
			{
				LPRECT	prc;
				
				prc = va_arg(argp, LPRECT);

				// 空
			}
			break;
		case	CW_HBITMAP:								// HBITMAP句柄
			{
				HBITMAP		*phb;
				
				phb = va_arg(argp, HBITMAP *);

				// 接收句柄的指针不能为NULL
				Assert(phb!=NULL, "Save()函数－HBITMAP类型－第一个参数必须是有效的HBITMAP指针。");
			}
			break;
		case	CW_HDIB:								// HDIB句柄
			{
				HDIB	*phdib;
				
				phdib = va_arg(argp, HDIB *);

				// 接收句柄的指针不能为NULL
				Assert(phdib!=NULL, "Save()函数－HDIB类型－第一个参数必须是有效的HDIB指针。");
			}
			break;
		default:
			Assert(0, "Save()函数－指定了非法的操作类型！");
			break;
		}
		break;
	default:
		Assert(0, "##编译器错误！！！");
	}

	return 0;
}


// 转换标志合法性检测
void CSeeImage::_cnvtype_check(int cnk)
{
	switch (cnk)
	{
	case	IRWE_CNVMARK_AUTO:							/* 自动转为最匹配的标准格式 */
	case	IRWE_CNVMARK_1:								/* 强制转为 1 位调色板图 */           
	case	IRWE_CNVMARK_4:								/* 强制转为 4 位调色板图 */           
	case	IRWE_CNVMARK_8:								/* 强制转为 8 位调色板图 */           
	case	IRWE_CNVMARK_16_555:						/* 强制转为 16 位(555)格式位图 */     
	case	IRWE_CNVMARK_16_565:						/* 强制转为 16 位(565)格式位图 */     
	case	IRWE_CNVMARK_24:							/* 强制转为 24 位位图 */              
	case	IRWE_CNVMARK_32:							/* 强制转为 32 位位图(带Alpha掩码) */ 
		break;
	default:
		Assert(0, "转换类型不合法！");					/* 入口参数cnk不是一个合法的转换标志 */
		break;
	}
}


// 从PE格式的文件中（EXE、DLL）读入DIB资源（ID形式的）
char* CSeeImage::_LoadDIBFromPE(char *fname, int id)
{
	CFile	ff;
	int		i;
	DWORD	subRva;										// RAV与物理地址的偏移
	short	nSection   = 0;								// 节数
	DWORD	ptResource = 0;								// 资源节地址
	WORD	nResource  = 0;								// 资源项个数
	char	*buff      = NULL;							// 待返回的DIB缓冲区
	bool	enumid     = (id<0);						// 标识类型

	IMAGE_DOS_HEADER					stDOSHeader;	// DOS EXE文件头
	IMAGE_SECTION_HEADER			stSectionHeader;	// 节表
	IMAGE_NT_HEADERS32					 stPEHeader;	// NT文件头
	IMAGE_RESOURCE_DIRECTORY		  stResourceDir;	// 资源目录
	IMAGE_RESOURCE_DIRECTORY_ENTRY	stResourceEntry;	// 资源目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY	 stResDirEntry1;	// 第一层目录入口（数组）
	IMAGE_RESOURCE_DATA_ENTRY        stResDataEntry;	// 资源入口


	// 打开调用者指定的PE文件
	if(!ff.Open(fname, CFile::typeBinary|CFile::shareDenyNone))
	{
		return NULL;									// 文件不存在
	}

	try
	{
		// 取DOS文件头
		ff.Read(&stDOSHeader, sizeof(IMAGE_DOS_HEADER));

		// 判断文件是否是微软格式
		if (stDOSHeader.e_magic != IMAGE_DOS_SIGNATURE)
		{
			throw 0;
		}

		// 尝试着定位到PE文件头
		ff.Seek(stDOSHeader.e_lfanew, CFile::begin);

		// 读PE文件头
		ff.Read(&stPEHeader, sizeof(IMAGE_NT_HEADERS32));

		// 判断文件是否是PE格式
		if(stPEHeader.Signature != IMAGE_NT_SIGNATURE)
		{
			throw 0;
		}
	}
	catch (...)
	{
		ff.Close();
		return NULL;									// 非PE格式
	}

	stResDirEntry1 = NULL;

	try
	{
		// 取总节数
		nSection = stPEHeader.FileHeader.NumberOfSections;

		// 读取节表
		for (i=0; i<nSection; i++)
		{
			ff.Read(&stSectionHeader, sizeof(IMAGE_SECTION_HEADER));

			// 判断是否有资源目录（以节名称判断，注：此方法并未被微软认可）
			if (!strcmp((LPCSTR)stSectionHeader.Name, ".rsrc\0\0"))
			{
				ptResource = stSectionHeader.PointerToRawData;
				subRva     = stSectionHeader.VirtualAddress - stSectionHeader.PointerToRawData;
				break;
			}
		}

		//如果有资源节则读取它
		if (ptResource != 0)
		{
			// 读资源根目录
			ff.Seek(ptResource, CFile::begin);
			ff.Read(&stResourceDir, sizeof(stResourceDir));

			// ID类型的个数
			nResource = stResourceDir.NumberOfIdEntries;

			// 忽略命名目录项（只识别ID形式的资源类型）
			ff.Seek(sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY)*stResourceDir.NumberOfNamedEntries, CFile::current);

			// 搜索BMP资源（资源类型）
			for(i=0; i<nResource; i++)
			{
				ff.Read((void*)&stResourceEntry, sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));

				if ((stResourceEntry.DataIsDirectory)&&(stResourceEntry.Id == 2))
				{
					break;
				}
			}

			// 如果没有找到BMP资源，则直接返回
			if (i >= nResource)
			{
				throw 0;
			}

			ff.Seek(stResourceEntry.OffsetToDirectory+ptResource,CFile::begin);
			ff.Read((void*)&stResourceDir, sizeof(IMAGE_RESOURCE_DIRECTORY));

			nResource = (enumid) ? stResourceDir.NumberOfIdEntries+stResourceDir.NumberOfNamedEntries : stResourceDir.NumberOfIdEntries;

			// 如果没有BMP图象，则返回
			if (nResource == 0)
			{
				throw 0;
			}

			// 如果不是枚举形式，则忽略命名项（只识别ID形式）
			if (!enumid)
			{
				ff.Seek(sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY)*stResourceDir.NumberOfNamedEntries, CFile::current);
			}
			
			stResDirEntry1 = new IMAGE_RESOURCE_DIRECTORY_ENTRY[nResource];
			
			// 读所有的BMP资源目录项（资源名称）
			for (i=0; i<nResource; i++)
			{
				ff.Read(&stResDirEntry1[i], sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
			}

			if (!enumid)								// 正常ID
			{
				for (i=0; i<nResource; i++)
				{
					if (stResDirEntry1[i].Id == (ushort)id)
					{
						break;
					}
				}
			}
			else										// 枚举ID
			{
				id &= 0xffff;
				i = id;
			}

			// 读DIB
			if ((ushort)i < nResource)
			{
				ff.Seek(stResDirEntry1[i].OffsetToDirectory+ptResource, CFile::begin);
				ff.Read(&stResourceDir, sizeof(IMAGE_RESOURCE_DIRECTORY));
				
				// 资源语言（因BMP资源没有语言差别，所以只读一个目录项）
				ff.Read(&stResourceEntry, sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
				
				// 读数据偏移及尺寸
				ff.Seek(stResourceEntry.OffsetToDirectory+ptResource, CFile::begin);
				ff.Read(&stResDataEntry, sizeof(IMAGE_RESOURCE_DATA_ENTRY));
				
				// 分配DIB缓冲区
				buff = new char[stResDataEntry.Size];
				
				// 读DIB数据
				ff.Seek(stResDataEntry.OffsetToData-subRva, CFile::begin);
				ff.Read(buff, stResDataEntry.Size);
			}

			delete stResDirEntry1;
			stResDirEntry1 = NULL;
		}

		ff.Close(); 
	}
	catch (...)
	{
		if (stResDirEntry1 != NULL)
		{
			delete []stResDirEntry1;
		}

		if (buff != NULL)
		{
			delete []buff;
		}

		ff.Close();

		return NULL;
	}

	return buff;
}


// 从当前进程中读入DIB资源
char* CSeeImage::_LoadDIBFromResource(char *lpResourceName)
{
	HMODULE		hInst = ::GetModuleHandle(NULL); 
	char		*pbuf = NULL;
	
	ASSERT(hInst);
	
	// 找指定的BMP资源
	HRSRC hSrc = ::FindResource(hInst, (LPCSTR)lpResourceName, RT_BITMAP);

	if (hSrc)
	{
		HGLOBAL hResData = ::LoadResource(hInst, hSrc);
	
		if (hResData)
		{
			LPVOID lpResData = ::LockResource(hResData);

			if (lpResData)
			{
				// 如果是有效的DIB资源，则资源的首位应该是BITMAPINFOHEADER结构
				if (*((DWORD*)lpResData) == sizeof(wBITMAPINFOHEADER))
				{
					pbuf = (char*)lpResData;
				}
			}
		}
		// 由LoadResource()函数读入的资源目标不需要显示的删除它，Windows系统会自动删除。
		// LockResource()锁定的资源目标也不需要显示的解锁
	}
	
	return pbuf;
}


// 申请新的专业数据包并填写初始化数据
//
// 入口参数：
//	ppPack	- 指向接收专业包地址的指针（指针的指针）
//	pbmi	- 指向BITMAPINFO结构的指针，该结构中应包含源图象的信息（包括调色板数据）
//	cnvmark	- 转换标志
//	sn		- 调用者的标识
//	pbits	- 指向BITMAPINFOHEADER结构后面的指针，或是指向纯象素数据的指针
//	bitmark	- 0 表示pbits指向BITMAPINFOHEADER结构后面、1 表示pbits指向纯象素数据
//	
// 返回值：参见iseeirw.h文件中的enum EXERESULT值
//	
int CSeeImage::_AllocSpecPack(LPSPEC_DATAPACK *ppPack, wBITMAPINFO* pbmi, int cnvmark, int sn, char *pbits, int bitmark)
{
	assert((ppPack != NULL)&&(pbmi != NULL));

	int				i;
	int				width    = pbmi->bmiHeader.biWidth;
	int				height   = abs(pbmi->bmiHeader.biHeight);
	int				bitcount = pbmi->bmiHeader.biBitCount;
	int				result   = ER_SUCCESS;
	LPINFOSTR		pinfo    = NULL;
	ulong	linesize = DIBSCANLINE_DWORDALIGN(width*bitcount);
	LPSPEC_DATAPACK	pPack    = NULL;
	ulong	palcnt;

	assert(cnvmark != IRWE_CNVMARK_NOT);

	// 检验图象信息是否合法
	if ((width == 0)||(height == 0)||(bitcount == 0))
	{
		return ER_BADIMAGE;
	}

	// 申请专业包
	if ((pPack=isirw_alloc_SPEC_DATAPACK(ISEEIO_CONFER_LOC_FILE, CSINIHFILENAME, 0, 0, (IRWE_CNVMARK)cnvmark)) != NULL)
	{
		assert(pPack->org_img != NULL);

		pinfo = pPack->org_img;

		// 申请行首地址数组
		if ((pinfo->pp_line_addr=(void**)isirw_malloc(sizeof(char*)*height)) == NULL)
		{
			isirw_free_SPEC_DATAPACK(pPack);
			return ER_MEMORYERR;
		}
		
		// 申请象素缓冲区
		if ((pinfo->p_bit_data=(uchar *)isirw_malloc((int)linesize*height)) == NULL)
		{
			isirw_free_SPEC_DATAPACK(pPack);
			return ER_MEMORYERR;
		}

		switch (bitcount)
		{
		case	1:
			SETMASK_8(pinfo);
			palcnt = 2;
			if (bitmark == 0)
			{
				pbits += palcnt*sizeof(wRGBQUAD);
			}
			for (i=0; i<(int)palcnt; i++)
			{
				pinfo->palette[i] = *(ulong *)&(pbmi->bmiColors[i]);
			}
			break;
		case	4:
			SETMASK_8(pinfo);
			palcnt = 16;
			if (bitmark == 0)
			{
				pbits += palcnt*sizeof(wRGBQUAD);
			}
			for (i=0; i<(int)palcnt; i++)
			{
				pinfo->palette[i] = *(ulong *)&(pbmi->bmiColors[i]);
			}
			break;
		case	8:
			SETMASK_8(pinfo);
			palcnt = 256;
			if (bitmark == 0)
			{
				pbits += palcnt*sizeof(wRGBQUAD);
			}
			for (i=0; i<(int)palcnt; i++)
			{
				pinfo->palette[i] = *(ulong *)&(pbmi->bmiColors[i]);
			}
			break;
		case	16:
			if (pbmi->bmiHeader.biCompression == BI_RGB)
			{
				pinfo->r_mask = 0x7c00;
				pinfo->g_mask = 0x3e0;
				pinfo->b_mask = 0x1f;
				pinfo->a_mask = 0x0;
			}
			else
			{
				pinfo->r_mask = 0xf800;
				pinfo->g_mask = 0x7e0;
				pinfo->b_mask = 0x1f;
				pinfo->a_mask = 0x0;
				if (bitmark == 0)
				{
					pbits += 3*sizeof(wRGBQUAD);
				}
			}
			palcnt = 0;
			break;
		case	24:
			SETMASK_24(pinfo);
			palcnt = 0;
			break;
		case	32:
			SETMASK_32(pinfo);
			palcnt = 0;
			if (pbmi->bmiHeader.biCompression == BI_BITFIELDS)
			{
				if (bitmark == 0)
				{
					pbits += 3*sizeof(wRGBQUAD);
				}
			}
			break;
		default:
			assert(0);
			isirw_free_SPEC_DATAPACK(pPack);
			return ER_NOTSUPPORT;
		}
		
		// 复制象素数据
		if (pbits != NULL)
		{
			memcpy((void*)pinfo->p_bit_data, (const void *)pbits, linesize*height);
		}
		else
		{
			memset((void*)pinfo->p_bit_data, 0xff, linesize*height);
		}

		pinfo->imgtype     = IMT_RESSTATIC;		/* 图像文件类型 */
		pinfo->imgformat   = IMF_BMP;			/* 图像文件格式（插件标识） */
		pinfo->compression = ICS_RGB;			/* 图像文件的压缩方式 */

		pinfo->width    = (ulong)width;
		pinfo->height   = (ulong)height;
		pinfo->bitcount = (ulong)bitcount;
		pinfo->order    = (pbmi->bmiHeader.biHeight > 0) ? 1 : 0;
		
		// 填写行首地址数组
		if (pinfo->order == 0)					/* 正向 */
		{
			for (i=0;i<(int)(pinfo->height);i++)
			{
				pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+(i*linesize));
			}
		}
		else									/* 倒向 */
		{
			for (i=0;i<(int)(pinfo->height);i++)
			{
				pinfo->pp_line_addr[i] = (void *)(pinfo->p_bit_data+((pinfo->height-i-1)*linesize));
			}
		}
		
		pinfo->pal_count = palcnt;				/* 调色板有效项的个数 */
		
		pinfo->play_number = 0;					/* 播放顺序表尺寸（以元素为单位） */
		pinfo->play_order  = NULL;				/* 播放顺序表 */
		pinfo->time        = 0;					/* 首帧停留时间 */
		pinfo->colorkey    = -1;				/* 主图象透明色（-1表示无透明色） */
		
		pinfo->imgnumbers  = 1;					/* 该文件中图像的个数 */
		pinfo->psubimg     = NULL;				/* 子图像数据链地址 */
		pinfo->data_state  = 2;					/* 数据包当前的数据状态 */
		
		pPack->command     = IRWE_SDPCOMM_LOAD;	/* 模拟读命令 */
		pPack->data_state  = SDP_DATA_STATE_ORG_PIX|SDP_DATA_STATE_ORG_INFO|SDP_DATA_STATE_PATH;
		pPack->exec_state  = IRWE_SDPSTATE_IDLE;
		pPack->sn          = sn;
		
		if (cnvmark != IRWE_CNVMARK_NOT)
		{
			if (isirw_cnv(pPack, (IRWE_CNVMARK)cnvmark, 0) < 0)
			{
				isirw_free_SPEC_DATAPACK(pPack);
				return ER_MEMORYERR;
			}
		}

		*ppPack = pPack;
		
		result = ER_SUCCESS;
	}
	else
	{
		*ppPack = NULL;

		result = ER_MEMORYERR;
	}

	return result;
}


// 将类中图象保存在指定目标介质中
//
// 入口参数：
//	type	- 目标介质类型
//	name	- 目标名称
//	par1/2	- ISeeIO所需的两个附加参数
//	cnvmark	- 保存位数ID
//	par		- 保存参数结构
//
// 出口参数：
//	ER_SUCCESS	- 成功
//	其它值失败，具体原因参见对EXERESULT枚举变量的定义
//
int CSeeImage::_SaveSpecPack(ISEEIO_CONFER_TYPE type, char *name, ulong par1, ulong par2, CSAVETYPE cnvmark, SAVESTR* par)
{
	assert(name&&par);

	// 保存原转换包数据
	IRWE_CNVMARK	cnvmode    = m_pPack->cnvmark;
	LPINFOSTR		oldcnvpack = m_pPack->cnv_img;
	EXERESULT		result     = ER_SUCCESS;

	m_pPack->cnv_img = 0;
	m_pPack->cnvmark = IRWE_CNVMARK_NOT;

	if (oldcnvpack)
	{
		m_pPack->data_state &= ~SDP_DATA_STATE_CNV;
	}

	IRWE_CNVMARK	cnvid = (IRWE_CNVMARK)cnvmark;

	assert((cnvid>=CNVAUTO)&&(cnvid<=CNV32));

	// 将图象转换为用户指定的位深
	if (isirw_cnv(m_pPack, cnvid, 0) >= 0)
	{
		// 保存图象
		if (isirw_s_write_img((ISEEIO_CONFER_TYPE)type, (const char *)name, par1, par2, m_pPack, par, 0) == 0)
		{
			// 等待操作完成
			AfxGetApp()->DoWaitCursor(1);
			result = isirw_s_wait(m_pPack, 0);
			AfxGetApp()->DoWaitCursor(0);
			
			switch ((int)result)
			{
			case	ER_SUCCESS:
				break;
			case	ER_USERBREAK:
				CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_USER);
				break;
			case	ER_MEMORYERR:
				CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_MEM);
				break;
			case	ER_FILERWERR:
				CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_RW);
				break;
			case	ER_SYSERR:
				CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_OSERR);
				break;
			case	ER_NONIMAGE:						// 指定的文件名不合法
				break;
			case	ER_NSIMGFOR:
				ASSERT(0);								// 插件设计问题
				break;
			default:
				ASSERT(0);								// 不可能是其它值
				break;
			}
		}
		else
		{
			result = CFileDialogEx::DispLastErrMess(isirw_s_get_last_err());
		}
	}
	else
	{
		result = CFileDialogEx::DispLastErrMess(ISIRW_LASTERR_MEM);
	}

	// 释放转换包
	if (m_pPack->cnv_img)
	{
		isirw_free_INFOSTR(m_pPack->cnv_img);
	}

	if (oldcnvpack)
	{
		m_pPack->data_state |= SDP_DATA_STATE_CNV;
	}

	// 恢复原状态
	m_pPack->cnv_img = oldcnvpack;
	m_pPack->cnvmark = cnvmode;

	return (int)result;
}


// 正规化调用者发来的路径串
//
// 入口参数：
//	path	- 调用者发来的路径串
//	buff	- 由调用者提供的、用于存放正规化后路径串的缓冲区（尺寸必须大于MAX_PATH个字节）
//
// 返回值：
//	同入口参数buff
//
// 注释：
//	1、如果path为NULL，则函数返回调用者程序所处的绝对路径
//	2、如果path为一相对路径，则函数根据调用者程序将其转换为绝对路径
//	3、本函数返回的串尾部为 '\' 字符
//
char * CSeeImage::_NormalizePath(char *path, char *buff)
{
	assert(buff != NULL);

	int		iLen;
	
	// 如果调用者没有指定路径，则获取调用程序所处路径
	if (path == NULL)
	{
		// 取得应用程序的全路径
		::GetModuleFileName(NULL, (LPTSTR)buff, MAX_PATH);
		
		iLen = lstrlen((char*)buff);
		
		// 去除尾部的应用程序名
		while ((buff[--iLen] != '\\')&&(iLen > 0))
		{
			buff[iLen] = '\0';
		}
	}
	else
	{
		// 判断调用者指定的路径是相对路径还是绝对路径
		if ((lstrlen(path) == 1)||(path[1] != ':'))
		{
			// 取得应用程序的全路径
			::GetModuleFileName(NULL, (LPTSTR)buff, MAX_PATH);
			
			iLen = lstrlen((char*)buff);
			
			// 去除尾部的应用程序名
			while ((buff[--iLen] != '\\')&&(iLen > 0))
			{
				buff[iLen] = '\0';
			}
			
			// 合成绝对路径
			lstrcat((LPSTR)buff, (LPCSTR)path);
			
			// 保证串尾部为 '\' 字符
			if (buff[lstrlen((LPCSTR)buff)-1] != '\\')
			{
				lstrcat((LPSTR)buff, (LPCSTR)"\\");
			}
		}
		else
		{
			assert(path[1] == ':');
			// 复制绝对路径
			lstrcpy((LPSTR)buff, (LPCSTR)path);

			if (buff[lstrlen((LPCSTR)buff)-1] != '\\')
			{
				lstrcat((LPSTR)buff, (LPCSTR)"\\");
			}
		}
	}

	return buff;
}


// 计算指定序号的子图象结构地址
//
// 入口参数：
//	pinfo	- 主图象结构地址[in]
//	num		- 待提取地址的子图象序号[in]
//
// 返回值：
//	子图象结构地址，失败返回NULL
//
SUBIMGBLOCK* CSeeImage::_CalSubImageAddr(LPINFOSTR pinfo, int num)
{
	// 图象序号不能为0，0是主图象
	assert(pinfo&&num);

	SUBIMGBLOCK*	psub = pinfo->psubimg;

	while (psub != NULL)
	{
		if (psub->number == num)
		{
			break;
		}
		else
		{
			psub = psub->next;
		}
	}

	return psub;
}


// 刷新显示信息
void CSeeImage::_UpdateDispInfo()
{
	// 引擎必须已经开启，并且必须含有图象
	assert(m_pPack&&m_pPack->org_img);

	LPINFOSTR	pinfo = (m_pPack->cnv_img != NULL) ? m_pPack->cnv_img : m_pPack->org_img;

	// 填写新图象的尺寸、显示等信息
	m_cDispInfo.bmi.biSize          = sizeof(wBITMAPINFOHEADER);
	m_cDispInfo.bmi.biWidth         = pinfo->width;
	m_cDispInfo.bmi.biHeight        = (pinfo->order == 0) ? -(long)pinfo->height : pinfo->height;
	m_cDispInfo.bmi.biPlanes        = 1;
	m_cDispInfo.bmi.biBitCount      = (WORD)pinfo->bitcount;
	m_cDispInfo.bmi.biCompression   = BI_RGB;
	m_cDispInfo.bmi.biSizeImage     = 0;
	m_cDispInfo.bmi.biXPelsPerMeter = 0;
	m_cDispInfo.bmi.biYPelsPerMeter = 0;
	m_cDispInfo.bmi.biClrUsed       = 0;
	m_cDispInfo.bmi.biClrImportant  = 0;
	
	memcpy((void*)m_cDispInfo.pal, (const void *)pinfo->palette, sizeof(wRGBQUAD)*pinfo->pal_count);
}


/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(LPSPEC_DATAPACK pack) : CFileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, ::AfxGetMainWnd())
{
	LPIRWMF_HEADER		pib;
	LPIRWMF_INFOBLOCK	pinfo;
	int					i;

	ASSERT(pack);

	// 等待包操作完成
	isirw_s_wait(pack, 0);
	
	m_pPack       = pack;
	m_saveinfocnt = 0;
	m_ctrlcnt     = 0;

	m_ext.Empty();

	memset((void*)m_saveinfo, 0, sizeof(SAVEPLUGININFO)*ISEEIRW_MAX_PLUGIN_CNT);
	memset((void*)m_pStatic, 0, sizeof(CStatic*)*ISD_CTLCNT);
	memset((void*)m_pComboBox, 0, sizeof(CComboBox*)*ISD_CTLCNT);
	memset((void*)m_save.para_value, 0, sizeof(SAVESTR));
	
	// 引擎不能处于锁定状态
	ASSERT(isirw_is_busy() != 3);
	
	// 等待引擎空闲下来
	while (isirw_is_busy())
	{
		::Sleep(0);
	}
	
	// 锁定图象读写引擎
	pib = isirw_lock();
	
	pinfo = pib->puh;
	
	// 将具有保存功能的插件信息提取出来，存放在信息数组中
	while (pinfo)
	{
		if (pinfo->ifo.irwp_function & IRWP_WRITE_SUPP)
		{
			m_saveinfo[m_saveinfocnt].index = m_saveinfocnt;
			m_saveinfo[m_saveinfocnt].piid  = pinfo->ifo.plug_id;
			strcpy((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strupr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)" 格式 (*.");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)")|");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)"*.");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)"|");
			memcpy((void*)&(m_saveinfo[m_saveinfocnt].cont), (const void *)&(pinfo->ifo.irwp_save), sizeof(IRW_SAVE_DESC));
			strcpy((char*)m_saveinfo[m_saveinfocnt].ext, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			m_saveinfocnt++;
		}
		pinfo = pinfo->next;
	}
	
	isirw_unlock(pib);
	
	m_ofn.lpstrDefExt    = "";
	m_ofn.Flags			&= ~OFN_ENABLESIZING;
	m_ofn.Flags			&= ~OFN_OVERWRITEPROMPT;
	
	strcpy((char*)m_szFileName, "未命名");
	
	// 将MFC风格的过滤串转换为系统风格
	if (m_saveinfocnt)
	{
		for (i=0, m_strFilter.Empty(); i<m_saveinfocnt; i++)
		{
			m_strFilter += (LPCTSTR)m_saveinfo[i].str;
		}

		LPTSTR pch = m_strFilter.GetBuffer(0);

		// 用 '\0' 替换'|'符号
		while ((pch = _tcschr(pch, '|')) != NULL)
		{
			*pch++ = '\0';
		}

		m_ofn.lpstrFilter = m_strFilter;
	}

	m_iRunMark = 0;
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
	//{{AFX_MSG_MAP(CFileDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFileDialogEx::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	CWnd		*wndDlg = GetParent();
	CRect		Rect;
	int			i;

	// 此时包中必须含有图象的象素数据
	if (!(m_pPack->data_state&SDP_DATA_STATE_ORG_PIX))
	{
		m_iRunMark++;
	}

	for (i=0, Rect.SetRectEmpty(); i<ISD_CTLCNT; i++)
	{
		m_pStatic[i]   = new CStatic;
		m_pComboBox[i] = new CComboBox;
		
		// 创建新增控件窗口并设置字体
		if (m_pStatic[i])
		{
			m_pStatic[i]->Create((LPCTSTR)"", WS_CHILD|WS_TABSTOP|SS_LEFT|SS_LEFTNOWORDWRAP, Rect, wndDlg, IDC_SAVEASCNT_STC+i);
			m_pStatic[i]->SetFont(wndDlg->GetFont());
		}
		else
		{
			m_iRunMark++;
		}

		if (m_pComboBox[i])
		{
			m_pComboBox[i]->Create(WS_CHILD|CBS_DROPDOWN|WS_VSCROLL|WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, Rect, wndDlg, IDC_SAVEASCNT_CON+i);
			m_pComboBox[i]->SetFont(wndDlg->GetFont());
		}
		else
		{
			m_iRunMark++;
		}
	}

	_SetFileType(0);
		
	return TRUE;
}

void CFileDialogEx::OnDestroy() 
{
	CFileDialog::OnDestroy();
	
	for (int i=0; i<ISD_CTLCNT; i++)
	{
		if (m_pStatic[i])
		{
			delete m_pStatic[i];
		}
		if (m_pComboBox[i])
		{
			delete m_pComboBox[i];
		}
	}
}


// 根据给定的字符串查找插件索引
int CFileDialogEx::_GetPluginIndex(LPCTSTR sou)
{
	for (int i=0; i<m_saveinfocnt; i++)
	{
		if (!memcmp((const void *)sou, (const void *)m_saveinfo[i].str, 12))
		{
			return i;
		}
	}
	return -1;
}


// 将过滤串索引转换为插件信息数组对应的索引
int CFileDialogEx::_GetPluginIndexFromFilterIndex(int idx)
{
	char	*p = (char*)m_ofn.lpstrFilter;
	int		i;

	ASSERT(p);

	for (i=1; i<idx; i++)
	{
		p += strlen((const char *)p)+1;					// 绕过过滤串对（2次）
		p += strlen((const char *)p)+1;
	}

	i = _GetPluginIndex((LPCTSTR)p);

	return i;
}


void CFileDialogEx::OnSize(UINT nType, int cx, int cy) 
{
	CFileDialog::OnSize(nType, cx, cy);
}


BOOL CFileDialogEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	OFNOTIFY			*pNotify = (OFNOTIFY*)lParam;
	
	switch(pNotify->hdr.code)
	{
	case CDN_TYPECHANGE:
		_SetFileType(pNotify->lpOFN->nFilterIndex);
		return TRUE;
	case CDN_FILEOK:
		*pResult = _OK();
		return TRUE;
	}

	return CFileDialog::OnNotify(wParam, lParam, pResult);
}


int CFileDialogEx::_SetFileType(int index)
{
	CWnd		*wndDlg = GetParent();
	int			i, j, k;
	DWORD		savebits;
	CString		stmp;

	// 如果初始化未成功，不执行下面的操作
	if (m_iRunMark)
	{
		return -1;
	}

	UpdateData(TRUE);

	static CRect	rcwnd(0,0,0,0);
	
	// 修正对话框尺寸
	if (rcwnd.IsRectEmpty())
	{
		wndDlg->GetWindowRect(rcwnd);
	}
	
	// 取得当前插件索引
	i = _GetPluginIndexFromFilterIndex(index);

	ASSERT(i >= 0);

	const UINT nControls = 5;	
	
	// 获取原始对话框相关控件窗口的尺寸数据
	UINT  Controls[nControls] = {stc3, stc2, edt1, cmb1, lst1};
	CRect rc[nControls];							
	
	// 修正原始窗口及控件矩形的坐标
	for (j=0; j<nControls; j++)
	{
		CWnd *wndCtrl = wndDlg->GetDlgItem(Controls[j]);
		wndCtrl->GetWindowRect(&rc[j]);
		wndDlg->ScreenToClient(&rc[j]);					// 转换为客户区坐标
	}

	// 新增一个控件窗口所需的高度
	int iExtraSize = rc[1].Height()+(rc[1].top-rc[0].bottom);
	
	// 设置位深度选择控制窗口标题
	m_pStatic[0]->SetWindowText("位深度选择:");
	// 清除以前的位数选项
	m_pComboBox[0]->ResetContent();

	// 当前插件支持的保存深度数据
	savebits = m_saveinfo[i].cont.bitcount;
	
	// 填充位深度项组合框
	for (j=0; j<ISEEIRW_MAX_BITCOUNT; j++)
	{
		if ((savebits>>j)&1)
		{
			stmp.Format("%d 位", j+1);
			k = m_pComboBox[0]->AddString((LPCTSTR)stmp);
			m_pComboBox[0]->SetItemData(k, j+1);
		}
	}

	// 缺省选择为第一项
	m_pComboBox[0]->SetCurSel(0);

	// 位深度控件窗口总是可见的
	m_pStatic[0]->SetWindowPos(NULL, rc[1].left, rc[1].top+iExtraSize, rc[1].Width(), rc[1].Height(), 0);
	m_pStatic[0]->ShowWindow(SW_SHOW);
	m_pComboBox[0]->SetWindowPos(NULL, rc[3].left, rc[3].top+iExtraSize, rc[3].Width(), rc[3].Height()+ISD_COMBOBOXLIST_HEIGHT, 0);
	m_pComboBox[0]->ShowWindow(SW_SHOW);

	// 先隐藏前次的控件窗口
	for (j=1; j<m_ctrlcnt+1; j++)
	{
		m_pStatic[j]->ShowWindow(SW_HIDE);
		m_pComboBox[j]->ShowWindow(SW_HIDE);
	}

	// 待新增的列表个数
	m_ctrlcnt = m_saveinfo[i].cont.count;

	// 生成新的控件窗口
	for(j=1; j<(m_ctrlcnt+1); j++)
	{
		CString strTxt;
		CRect   cr = rc[1];

		cr.OffsetRect(0, iExtraSize*(j+1));

		if(m_pStatic[j]->GetSafeHwnd())
		{
			m_pStatic[j]->SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), 0);
			m_pStatic[j]->ShowWindow(SW_SHOW);
			strTxt = m_saveinfo[i].cont.para[j-1].desc;
			m_pStatic[j]->SetWindowText(strTxt+":");
		}
		else
		{
			strTxt = m_saveinfo[i].cont.para[j-1].desc;
			m_pStatic[j]->Create(strTxt+":", WS_CHILD|WS_VISIBLE, cr, wndDlg, IDC_SAVEASCNT_STC+j);
			m_pStatic[j]->SetFont(wndDlg->GetFont());
		}

		// 生成或者更新下拉控件
		cr = rc[3];

		cr.OffsetRect(0, iExtraSize*(j+1));
		cr.bottom += ISD_COMBOBOXLIST_HEIGHT;

		if(m_pComboBox[j]->GetSafeHwnd())
		{
			m_pComboBox[j]->SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), 0);
			m_pComboBox[j]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pComboBox[j]->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, cr, wndDlg, IDC_SAVEASCNT_CON+j);
			m_pComboBox[j]->SetFont(wndDlg->GetFont());
		}

		//添加下拉控件的内容
		m_pComboBox[j]->ResetContent();

		// 加入选择项字串
		for (k=0; k<m_saveinfo[i].cont.para[j-1].count; k++)
		{
			stmp.Format("%s", (LPCTSTR)m_saveinfo[i].cont.para[j-1].value_desc[k]);
			m_pComboBox[j]->AddString((LPCTSTR)stmp);
		}

		m_pComboBox[j]->SetCurSel(0);
	}

	// 隐藏其余控件
	for(k=m_ctrlcnt+1; k<ISD_CTLCNT; k++)
	{
		if(m_pStatic[k]->GetSafeHwnd())
		{
			m_pStatic[k]->ShowWindow(SW_HIDE);
		}

		if(m_pComboBox[k]->GetSafeHwnd())
		{
			m_pComboBox[k]->ShowWindow(SW_HIDE);
		}
	}

	// 设置缺省扩展名
	m_ext = (LPCSTR)m_saveinfo[i].ext;

	// 修正对话框尺寸
	wndDlg->SetWindowPos(NULL, 0, 0, rcwnd.Width(), rcwnd.Height()+iExtraSize*(m_ctrlcnt), SWP_NOMOVE);
	
	UpdateData(FALSE);

	return 0;
}


// 保存文件
BOOL CFileDialogEx::_OK()
{
	// 此时包中必须含有图象的象素数据
	if (!(m_pPack->data_state&SDP_DATA_STATE_ORG_PIX))
	{
		m_iRunMark++;
	}
	
	UpdateData(TRUE);

	// 获取目标文件路径
	CString		strExt      = GetFileExt();
	CString		strFileName = GetFileName();
	CString		strPathName = GetPathName();
	CFileFind	fileFind;
	CString		strMsg;

	// 修正文件扩展名
	if (strExt.IsEmpty())
	{
		strPathName += ".";
		strPathName += m_ext;
		strFileName += ".";
		strFileName += m_ext;
	}
	else
	{
		if (_stricmp((const char *)strExt, (const char *)m_ext))
		{
			strPathName += ".";
			strPathName += m_ext;
			strFileName += ".";
			strFileName += m_ext;
		}
	}

	// 判断文件是否存在
	if(fileFind.FindFile(strPathName))
	{
		strMsg.Format("文件 %s 已经存在，要替换该文件吗？", strFileName);

		if(MessageBox(strMsg, "另存为", MB_ICONQUESTION|MB_YESNO) == IDNO)
		{
			UpdateData(FALSE);
			return TRUE;
		}
	}

	// 收集用户选择信息
	for(int i=0; i<m_ctrlcnt; i++)
	{
		m_save.para_value[i] = m_pComboBox[i+1]->GetCurSel();
	}

	// 获取目标图象位深度信息
	int	bitcount = (int)m_pComboBox[0]->GetItemData(m_pComboBox[0]->GetCurSel());

	// 保存原转换包数据
	IRWE_CNVMARK	cnvmode    = m_pPack->cnvmark;
	LPINFOSTR		oldcnvpack = m_pPack->cnv_img;

	m_pPack->cnv_img = 0;
	m_pPack->cnvmark = IRWE_CNVMARK_NOT;

	if (oldcnvpack)
	{
		m_pPack->data_state &= ~SDP_DATA_STATE_CNV;
	}

	// 将图象转换为用户指定的位深
	if (isirw_cnv(m_pPack, GetStdIDFromBitCnt(bitcount), 0) >= 0)
	{
		// 保存图象
		if (isirw_s_write_img(ISEEIO_CONFER_LOC_FILE, (const char *)strPathName, 0, 0, m_pPack, &m_save, 0) == 0)
		{
			// 等待操作完成
			AfxGetApp()->DoWaitCursor(1);
			int retwait = isirw_s_wait(m_pPack, 0);
			AfxGetApp()->DoWaitCursor(0);
			
			switch (retwait)
			{
			case	ER_SUCCESS:
				break;
			case	ER_USERBREAK:
				DispLastErrMess(ISIRW_LASTERR_USER);
				break;
			case	ER_MEMORYERR:
				DispLastErrMess(ISIRW_LASTERR_MEM);
				break;
			case	ER_FILERWERR:
				DispLastErrMess(ISIRW_LASTERR_RW);
				break;
			case	ER_SYSERR:
				DispLastErrMess(ISIRW_LASTERR_OSERR);
				break;
			case	ER_NSIMGFOR:
				ASSERT(0);								// 插件设计问题
				break;
			default:
				ASSERT(0);								// 不可能是其它值
				break;
			}
		}
		else
		{
			DispLastErrMess(isirw_s_get_last_err());
		}
	}
	else
	{
		DispLastErrMess(ISIRW_LASTERR_MEM);
	}

	// 释放转换包
	if (m_pPack->cnv_img)
	{
		isirw_free_INFOSTR(m_pPack->cnv_img);
	}

	if (oldcnvpack)
	{
		m_pPack->data_state |= SDP_DATA_STATE_CNV;
	}
	
	// 恢复原状态
	m_pPack->cnv_img = oldcnvpack;
	m_pPack->cnvmark = cnvmode;

	UpdateData(FALSE);

	return FALSE;
}


// 将位深值转换为标准格式ID
IRWE_CNVMARK CFileDialogEx::GetStdIDFromBitCnt(int bitcnt)
{
	IRWE_CNVMARK ret = IRWE_CNVMARK_32;

	switch (bitcnt)
	{
	case	1:
		ret = IRWE_CNVMARK_1;
		break;
	case	4:
		ret = IRWE_CNVMARK_4;
		break;
	case	8:
		ret = IRWE_CNVMARK_8;
		break;
	case	16:
		ret = IRWE_CNVMARK_16_555;						// 保存功能只支持555格式
		break;
	case	24:
		ret = IRWE_CNVMARK_24;
		break;
	case	32:
		ret = IRWE_CNVMARK_32;
		break;
	default:
		ASSERT(0);										// 插件保存功能提供的位深不可能超出标准格式之外
		break;
	}
	
	return ret;
}


// 显示错误信息
EXERESULT CFileDialogEx::DispLastErrMess(int err)
{
	EXERESULT	ret;
	CString		str;

	str.Empty();

	switch (err)
	{
	case	ISIRW_LASTERR_SUCCESS:				// 没有错误
		ret = ER_SUCCESS;
		break;
	case	ISIRW_LASTERR_SYSCLOSE:				// 引擎系统未开启
		str = "ISee图象读写引擎系统未开启，无法保存图象！";
		ret = ER_SUCCESS;
		break;
	case	ISIRW_LASTERR_MEM:					// 内存不足
		str = "内存不足，保存操作无法完成！";
		ret = ER_MEMORYERR;
		break;
	case	ISIRW_LASTERR_RW:					// 介质读写错误
		str = "发生磁盘读写错误，保存操作无法完成！";
		ret = ER_FILERWERR;
		break;
	case	ISIRW_LASTERR_USER:					// 操作被用户中断
		str = "用户中断保存操作，该操作尚未完成。";
		ret = ER_USERBREAK;
		break;
	case	ISIRW_LASTERR_IMGERR:				// 图象错误(受损或非图象)
		str = "图象错误(受损或非图象)。";
		ret = ER_BADIMAGE;
		break;
	case	ISIRW_LASTERR_OSERR:				// 操作系统错误
		str = "操作系统错误！保存操作无法完成！";
		ret = ER_SYSERR;
		break;
	default:
		ASSERT(0);
		str = "软件异常，建议退出程序！";
		ret = ER_SYSERR;
		break;
	}

	if (!str.IsEmpty())
	{
		AfxMessageBox((LPCTSTR)str);
	}

	return ret;
}



