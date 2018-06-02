// Eagleeye2_ViewerDoc.cpp : implementation of the CEagleeye2_ViewerDoc class
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "Eagleeye2_ViewerDoc.h"
#include <winsock.h>
#include "SelectFileDlg.h"
#include "GeneralIODlg.h"
#include "process.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//socket ports
#define COMMANDPORT		35000				//向相机发送命令的网络线程端口
#define TRGIMGPORT		45000				//触发模式下与相机进行数据交换的网络线程端口
#define FCONIMGPORT		55000				//全分辨率连续模式下与相机进行数据交换的网络线程端口
#define HFRIMGPORT		65000				//高祯率模式下与相机进行数据交换的网络线程端口
#define HFRBINIMGPORT	60000				//高祯率高灵敏度模式下与相机进行数据交换的网络线程端口
#define IMGINFLEN		sizeof(EagleEye2ImgInfStruct)//抓拍信息长度
//Recv image data thread
void	RecvTrgImagedata(LPVOID pParam);	//触发模式下与相机进行数据交换的网络线程函数声明
void	RecvConImagedata(LPVOID pParam);	//全分辨率连续模式下与相机进行数据交换的网络线程函数声明
void	RecvHfconImagedata(LPVOID pParam);	//高祯率模式下与相机进行数据交换的网络线程函数声明
void	RecvHfBinconImagedata(LPVOID pParam);//高祯率模式下与相机进行数据交换的网络线程函数声明
//global variables
UINT			uiJpgDataLen;				//接受到的jpg图像数据的长度 
BUFFNODE		listJpgBuff[NUM_BUFF];		//装载JPG图像数据的环形缓冲数组
LONG			PrevBuff;					//环形缓冲的前一次使用索引
LONG			CurrBuff;					//环形缓冲的当前使用索引
volatile BYTE	TargetIPAdd1,TargetIPAdd2;	//
volatile BYTE	TargetIPAdd3,TargetIPAdd4;	//维护当前目标相机IP地址的4个变量
char			cRcvBuf[4096];				//与目标相机进行命令交互的接受缓冲区
char			cmdbuf[1024*5];				//与目标相机进行命令交互的发送缓冲区
volatile unsigned char	pImgBuf8[IMGLENGTH*3];
EagleEye2ImgInfStruct	EagleEye2_ImgInf =
{
	{'\0'},			//ui8LocusInf[101]
	{0},			//ui8TimeInf[7]
	0,				//ui8WayNum
	0,				//ui8TrgMode
	0.0				//fSpeed
};;
/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc

IMPLEMENT_DYNCREATE(CEagleeye2_ViewerDoc, CDocument)

BEGIN_MESSAGE_MAP(CEagleeye2_ViewerDoc, CDocument)
	//{{AFX_MSG_MAP(CEagleeye2_ViewerDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_CONFIG_TARGETIP, OnConfigTargetip)
	ON_COMMAND(ID_OMC_TRIGGER, OnOmcTrigger)
	ON_UPDATE_COMMAND_UI(ID_OMC_TRIGGER, OnUpdateOmcTrigger)
	ON_COMMAND(ID_TRGMODE_CONFIG, OnTrgmodeConfig)
	ON_UPDATE_COMMAND_UI(ID_TRGMODE_CONFIG, OnUpdateTrgmodeConfig)
	ON_COMMAND(ID_OPE_GRAB, OnOpeGrab)
	ON_UPDATE_COMMAND_UI(ID_OPE_GRAB, OnUpdateOpeGrab)
	ON_COMMAND(ID_OMC_MJPEG, OnOmcMjpeg)
	ON_UPDATE_COMMAND_UI(ID_OMC_MJPEG, OnUpdateOmcMjpeg)
	ON_COMMAND(ID_MJPEGMODE_CONFIG, OnMjpegmodeConfig)
	ON_UPDATE_COMMAND_UI(ID_MJPEGMODE_CONFIG, OnUpdateMjpegmodeConfig)
	ON_COMMAND(ID_OMC_HFR, OnOmcHfr)
	ON_UPDATE_COMMAND_UI(ID_OMC_HFR, OnUpdateOmcHfr)
	ON_COMMAND(ID_HFRMODE_CONFIG, OnHfrmodeConfig)
	ON_UPDATE_COMMAND_UI(ID_HFRMODE_CONFIG, OnUpdateHfrmodeConfig)
	ON_COMMAND(ID_CONFIG_NEWIP, OnConfigNewip)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_NEWIP, OnUpdateConfigNewip)
	ON_COMMAND(ID_CONFIG_NEWMAC, OnConfigNewmac)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_NEWMAC, OnUpdateConfigNewmac)
	ON_COMMAND(ID_CONFIG_AFEGAINCTRL, OnConfigAfegainctrl)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_AFEGAINCTRL, OnUpdateConfigAfegainctrl)
	ON_COMMAND(ID_CONFIG_WHITEBALANCE, OnConfigWhitebalance)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_WHITEBALANCE, OnUpdateConfigWhitebalance)
	ON_COMMAND(ID_CONFIG_GIOCONFIG, OnConfigGioconfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_GIOCONFIG, OnUpdateConfigGioconfig)
	ON_COMMAND(ID_CONFIG_RTCCONFIG, OnConfigRtcconfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_RTCCONFIG, OnUpdateConfigRtcconfig)
	ON_COMMAND(ID_CONFIG_WDTCONFIG, OnConfigWdtconfig)
	ON_UPDATE_COMMAND_UI(ID_CONFIG_WDTCONFIG, OnUpdateConfigWdtconfig)
	ON_COMMAND(ID_OPE_AUTOWB, OnOpeAutowb)
	ON_UPDATE_COMMAND_UI(ID_OPE_AUTOWB, OnUpdateOpeAutowb)
	ON_COMMAND(ID_OPE_AUTOTRG_ENABLE, OnOpeAutotrgEnable)
	ON_UPDATE_COMMAND_UI(ID_OPE_AUTOTRG_ENABLE, OnUpdateOpeAutotrgEnable)
	ON_COMMAND(ID_OPE_AUTOTRG_DISABLE, OnOpeAutotrgDisable)
	ON_UPDATE_COMMAND_UI(ID_OPE_AUTOTRG_DISABLE, OnUpdateOpeAutotrgDisable)
	ON_COMMAND(ID_OPE_LUT_ENABLE, OnOpeLutEnable)
	ON_UPDATE_COMMAND_UI(ID_OPE_LUT_ENABLE, OnUpdateOpeLutEnable)
	ON_COMMAND(ID_OPE_LUT_DISABLE, OnOpeLutDisable)
	ON_UPDATE_COMMAND_UI(ID_OPE_LUT_DISABLE, OnUpdateOpeLutDisable)
	ON_COMMAND(ID_OPE_RELAY_OPEN, OnOpeRelayOpen)
	ON_UPDATE_COMMAND_UI(ID_OPE_RELAY_OPEN, OnUpdateOpeRelayOpen)
	ON_COMMAND(ID_OPE_RELAY_CLOSE, OnOpeRelayClose)
	ON_UPDATE_COMMAND_UI(ID_OPE_RELAY_CLOSE, OnUpdateOpeRelayClose)
	ON_COMMAND(ID_OPE_RESETSYS, OnOpeResetsys)
	ON_UPDATE_COMMAND_UI(ID_OPE_RESETSYS, OnUpdateOpeResetsys)
	ON_COMMAND(ID_OPE_REMOTEUPDATEDSPPRROM, OnOpeRemoteupdatedspprrom)
	ON_UPDATE_COMMAND_UI(ID_OPE_REMOTEUPDATEDSPPRROM, OnUpdateOpeRemoteupdatedspprrom)
	ON_COMMAND(ID_OPE_IMGFORMAT_RGB, OnOpeImgformatRgb)
	ON_UPDATE_COMMAND_UI(ID_OPE_IMGFORMAT_RGB, OnUpdateOpeImgformatRgb)
	ON_COMMAND(ID_OPE_IMGFORMAT_YUV, OnOpeImgformatYuv)
	ON_UPDATE_COMMAND_UI(ID_OPE_IMGFORMAT_YUV, OnUpdateOpeImgformatYuv)
	ON_COMMAND(ID_OPE_IMGFORMAT_BAYER, OnOpeImgformatBayer)
	ON_UPDATE_COMMAND_UI(ID_OPE_IMGFORMAT_BAYER, OnUpdateOpeImgformatBayer)
	ON_COMMAND(ID_OPE_TRGSHUTTERCTRL_MANUAL, OnOpeTrgshutterctrlManual)
	ON_UPDATE_COMMAND_UI(ID_OPE_TRGSHUTTERCTRL_MANUAL, OnUpdateOpeTrgshutterctrlManual)
	ON_COMMAND(ID_OPE_TRGSHUTTERCTRL_AUTO, OnOpeTrgshutterctrlAuto)
	ON_UPDATE_COMMAND_UI(ID_OPE_TRGSHUTTERCTRL_AUTO, OnUpdateOpeTrgshutterctrlAuto)
	ON_COMMAND(ID_OMC_HFRB, OnOmcHfrb)
	ON_UPDATE_COMMAND_UI(ID_OMC_HFRB, OnUpdateOmcHfrb)
	ON_COMMAND(ID_HFRBINMODE_CONFIG, OnHfrbinmodeConfig)
	ON_UPDATE_COMMAND_UI(ID_HFRBINMODE_CONFIG, OnUpdateHfrbinmodeConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc construction/destruction

CEagleeye2_ViewerDoc::CEagleeye2_ViewerDoc()
{
	//一些通用控制变量的初始化
	m_bIsTrgMode	= FALSE;		//是否触发模式
	m_bIsFconMode	= FALSE;		//是否全分辨率连续模式
	m_bIsHFRMode	= FALSE;		//是否高祯率模式
	m_bIsHFRBinMode = FALSE;		//是否高祯率高灵敏度模式
	m_bIsEnsureTgtIP = FALSE;		//是否确认目标相机的IP地址
	m_bRelayIsOpen = TRUE;			//相机继电器是否断开
	m_bLUTEnable = TRUE;			//相机是否使能图像的LUT映射变换
	m_bAutoTrgEnable = FALSE;		//切换到触发模式后是否允许自动触发抓取一祯图像
	m_bFirstShow = TRUE ;			//浏览程序是否为初次显示，用来维护视图的刷新
	m_bIsDataFromCamera = FALSE;	//标志当前视图显示的图像是否来自相机，本浏览程序也可打开硬盘中的静态图像
	m_bIsJpeg = FALSE;				//标志当前视图显示的图像是否是JPG格式
	m_bIsHighFR = FALSE;			//标志当前需要显示的图像是否来自高祯率模式，以此区别于其它两种模式，用来在OnDraw函数中进行显示控制
	m_bIsRGBArray = TRUE;			//相机采集的图像格式是否为RGB格式
	m_bIsYUVArray = FALSE;			//相机采集的图像格式是否为YUV格式
	m_bIsBAYERArray = FALSE;		//相机采集的图像格式是否为BAYER格式
	m_bTrgShutterIsManual = TRUE;	//触发模式快门速度是否是手动模式
	//初始化环形缓冲区
	for (int nBuff=0; nBuff<NUM_BUFF; nBuff++) {
		listJpgBuff[nBuff].sizeBuff = 0;
		listJpgBuff[nBuff].bIsLocked = FALSE;
	}
	CurrBuff = 0; PrevBuff = -1;
}

CEagleeye2_ViewerDoc::~CEagleeye2_ViewerDoc()
{
}

BOOL CEagleeye2_ViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	POSITION firstViewPos;
	CView *pView;
	//获得视图类的窗口对象
	firstViewPos=GetFirstViewPosition();
	pView=GetNextView(firstViewPos);
	m_hViewWnd=pView->GetSafeHwnd();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc serialization
void CEagleeye2_ViewerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc diagnostics
#ifdef _DEBUG
void CEagleeye2_ViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEagleeye2_ViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerDoc commands
void CEagleeye2_ViewerDoc::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	int			i;
	CString		Filter;
	CString		strFileName;

	Filter.LoadString(IDS_PICFORMAT);
	
	CFileDialog dlgFile(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, Filter, NULL);
	if(dlgFile.DoModal() == IDCANCEL){
		return;
	}
	
	CWaitCursor		waitCursor;
	strFileName = dlgFile.GetPathName();
	i = strFileName.GetLength();
	if( i<4 || strFileName.GetAt(i-4)!='.'){
		strFileName += ".jpg";
	}
	if (!m_fPic.Save (strFileName))
	{
		::MessageBox (NULL, _T("保存文件失败! 请检查是否指定扩展名"), _T("错误"), MB_OK | MB_ICONINFORMATION) ;
		return ;
	}

}

void CEagleeye2_ViewerDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CString		Filter;
	Filter.LoadString(IDS_PICFORMAT);

	CSelectFileDlg		dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, Filter, NULL);
	if(dlgFile.DoModal() == IDCANCEL){
		return;
	}

	DeleteContents();
	CWaitCursor		waitCursor;
	if (m_fPic.Load(dlgFile.GetPathName()) == IMAGE_UNKNOW){
		::MessageBox(NULL, _T("Failed to read file"), _T("Error"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	this->SetPathName(dlgFile.GetPathName()); //pass the parameter to OnSaveDocument

	m_DDB.Attach(m_fPic.GetDDB_Handle()); //Get DDB
	m_fPic.SetUndoEnable(true); //set undo-flag
	m_bFirstShow = TRUE;
	m_bIsDataFromCamera = FALSE;
	this->UpdateAllViews (NULL);	
}

///////////////////////////////////////////////////////////////////////////////
//							Sub Threads
///////////////////////////////////////////////////////////////////////////////
void RecvTrgImagedata(LPVOID pParam)
{
	//监听DSP图像数据的线程
	CEagleeye2_ViewerDoc* pDoc=(CEagleeye2_ViewerDoc*) pParam;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockTrgClient;
	struct sockaddr_in	addr;
	int					i,cnt;
	BOOL				flag = TRUE;
	UINT				recvdatalen;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	UINT				tmp;
	CString				strFileName;
	static CString		strFileNamebak="\0";
	static int			iIndex = 0;

	//获取目标相机的IP地址
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("无法初始化全分辨率连续模式套接口对象");
		return;
	}
	sockTrgClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockTrgClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(TRGIMGPORT);
	//连接目标相机
	if ( connect( sockTrgClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
  //pDoc->OnOpeGrab();
	//开始接受相机上传图像数据
	while(1){
		//接受JPG码流长度
		i = 0;
		while( i < 4){
			cnt = recv( sockTrgClient, (char*)&uiJpgDataLen+i, 4-i, 0 );
			if( cnt <=0){
				goto leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = uiJpgDataLen;
			}
			//如果相机发送回0xffff则表示结束本次通讯
			if( recvdatalen==0x00 ){
				goto leave;
			}
		}
		//接受抓拍信息
		i = 0;
		while( i < IMGINFLEN ){
			cnt = recv( sockTrgClient, (char*)&EagleEye2_ImgInf+i, IMGINFLEN-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		//接受图像数据
		i = 0;
		listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
		pDoc->m_bIsJpeg = TRUE;
		while( i < recvdatalen ){
			cnt = recv( sockTrgClient, listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		CurrBuff++;
		if(CurrBuff==5){
			CurrBuff = 0;
		}
		PrevBuff++;
		if(PrevBuff==5){
			PrevBuff = 0;
		}
		pDoc->m_bIsDataFromCamera = TRUE;
		//通知视图进行显示
		PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
		//保存抓拍图像
		CString	strFileName,strDirectory;
		char tmpStr[20];
		CString	strFolderName;
		CFileFind   f;
		//地点和时间
		if( strlen(EagleEye2_ImgInf.cLocusInf) >0 ){
			strFileName += EagleEye2_ImgInf.cLocusInf;
			if( EagleEye2_ImgInf.byTimeInf[0]>9 ){
				strFileName += "_20";
				strFolderName += "20";
			}
			else{
				strFileName += "_200";
				strFolderName += "200";
			}
		}
		else{
			if( EagleEye2_ImgInf.byTimeInf[0]>9 ){
				strFileName += "20";
				strFolderName += "20";
			}
			else{
				strFileName += "200";
				strFolderName += "200";
			}
		}
		strFileName += itoa(EagleEye2_ImgInf.byTimeInf[0],tmpStr,10);
		strFolderName += itoa(EagleEye2_ImgInf.byTimeInf[0],tmpStr,10);
		if( EagleEye2_ImgInf.byTimeInf[1] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[1],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[1],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFolderName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[1],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[1],tmpStr,10);
		}
		if( EagleEye2_ImgInf.byTimeInf[2] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[2],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[2],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFolderName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[2],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[2],tmpStr,10);
		}
		if( EagleEye2_ImgInf.byTimeInf[4] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[4],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[4],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFolderName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[4],tmpStr,10);
			strFolderName+=itoa(EagleEye2_ImgInf.byTimeInf[4],tmpStr,10);
		}
		if( EagleEye2_ImgInf.byTimeInf[5] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[5],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[5],tmpStr,10);
		}
		if( EagleEye2_ImgInf.byTimeInf[6] >9 ){
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[6],tmpStr,10);
		}
		else{
			strFileName += "0";
			strFileName+=itoa(EagleEye2_ImgInf.byTimeInf[6],tmpStr,10);
		}
		//车道号
		strFileName += "_";
		strFileName += itoa(EagleEye2_ImgInf.byWayNum,tmpStr,10);
		strFileName += "车道";
		//祯索引
		if( !strcmp(strFileName,strFileNamebak) ){
			//备份文件名
			strFileNamebak = strFileName;
			//增加索引项
			strFileName += "_";
			strFileName += itoa(++iIndex,tmpStr,10);
		}
		else{
			//备份文件名
			strFileNamebak = strFileName;				
			iIndex = 0;
		}
		//触发类型
		switch( EagleEye2_ImgInf.byTrgMode ){
		case 0://软触发
			strFileName += "_软触发";
			break;
		case 1://单线圈
			strFileName += "_单线圈触发";
			break;
		case 2://双线圈
			strFileName += "_双线圈触发车速为";
			if( EagleEye2_ImgInf.fSpeed - (int)EagleEye2_ImgInf.fSpeed < 0.0001 ){
				strFileName += itoa((int)EagleEye2_ImgInf.fSpeed,tmpStr,10);
			}
			else{
				strFileName += _gcvt( EagleEye2_ImgInf.fSpeed, 5, tmpStr );
				int tttt;
				tttt = strlen(strFileName);
				if( strFileName.GetAt(strlen(strFileName)-1)=='.' ){
					strFileName += "00";
				}
			}
			strFileName += "公里";
			break;
		case 3://专用硬触发
			strFileName += "_专用硬触发";
			break;
		default:
			break;
		}
		//Creat Directory
		strDirectory.Format("D:\\Picture\\");
		if( !f.FindFile( strDirectory ) ){
			CreateDirectory(strDirectory,NULL);
		}
		strDirectory+=strFolderName;
		strDirectory+="\\";
		if( !f.FindFile( strDirectory ) ){
			CreateDirectory(strDirectory,NULL);
		}
		//end Creat Directory
		strDirectory += strFileName;
		strDirectory += ".jpg";
		pDoc->m_fPic1.LoadJpeg((BYTE*)listJpgBuff[PrevBuff].pBuff, listJpgBuff[PrevBuff].sizeBuff);
		pDoc->m_fPic1.Save (strDirectory);
	}

leave:
	//清理环境，退出线程
	closesocket(sockTrgClient);
	WSACleanup();
}

void RecvConImagedata(LPVOID pParam)
{
	//监听DSP图像数据的线程
	CEagleeye2_ViewerDoc* pDoc=(CEagleeye2_ViewerDoc*) pParam;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockFconClient;
	struct sockaddr_in	addr;	
	int					i,cnt;
	BOOL				flag = TRUE;
	UINT				recvdatalen;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	UINT				tmp;
	BYTE				replyBuf[2];

	//获取目标相机的IP地址
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("无法初始化全分辨率连续模式套接口对象");
		return;
	}
	sockFconClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockFconClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(FCONIMGPORT);
	//连接目标相机
	if ( connect( sockFconClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
	//开始接受相机上传图像数据
	while(1){
		i = 0;
		while( i < 4){
			cnt = recv( sockFconClient, (char*)&uiJpgDataLen+i, 4-i, 0 );
			if( cnt <=0){
				goto leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = uiJpgDataLen;
			}
		}
		i = 0;
		listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
		pDoc->m_bIsJpeg = TRUE;
		while( i < recvdatalen ){
			cnt = recv( sockFconClient, listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		CurrBuff++;
		if(CurrBuff==5){
			CurrBuff = 0;
		}
		PrevBuff++;
		if(PrevBuff==5){
			PrevBuff = 0;
		}
		pDoc->m_bIsDataFromCamera = TRUE;
		//在全分辨率模式下回复相机是否结束当前连接，若回复0xffff则结束，否则接续接受数据
		if( !pDoc->m_bIsFconMode ){
			replyBuf[0] = 0xff;
			replyBuf[1] = 0xff;
			if( send( sockFconClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
			goto leave;
		}
		else{
			replyBuf[0] = 0x0;
			replyBuf[1] = 0x0;
			if( send( sockFconClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
		}
		//通知视图进行显示
		PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
	}

leave:
	//清理环境，退出线程
	closesocket(sockFconClient);
	WSACleanup();
}

void RecvHfconImagedata(LPVOID pParam)
{
	//监听DSP图像数据的线程
	CEagleeye2_ViewerDoc* pDoc=(CEagleeye2_ViewerDoc*) pParam;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockHfrClient;
	struct sockaddr_in	addr;
	int					i,cnt;
	BOOL				flag = TRUE;
	UINT				recvdatalen;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	UINT				tmp;
	BYTE				replyBuf[2];

	//获取目标相机的IP地址
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("无法初始化全分辨率连续模式套接口对象");
		return;
	}
	sockHfrClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockHfrClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(HFRIMGPORT);
	//连接目标相机
	if ( connect( sockHfrClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
	//开始接受相机上传图像数据
	while(1){
		i = 0;
		while( i < 4){
			cnt = recv( sockHfrClient, (char*)&uiJpgDataLen+i, 4-i, 0 );
			if( cnt <=0){
				goto leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = uiJpgDataLen;
			}
		}
		i = 0;
		listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
		pDoc->m_bIsJpeg = TRUE;
		while( i < recvdatalen ){
			cnt = recv( sockHfrClient, listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		CurrBuff++;
		if(CurrBuff==5){
			CurrBuff = 0;
		}
		PrevBuff++;
		if(PrevBuff==5){
			PrevBuff = 0;
		}
		pDoc->m_bIsDataFromCamera = TRUE;
		//在高祯率模式下回复相机是否结束当前连接，若回复0xffff则结束，否则接续接受数据
		if( !pDoc->m_bIsHFRMode ){
			replyBuf[0] = 0xff;
			replyBuf[1] = 0xff;
			if( send( sockHfrClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
			goto leave;
		}
		else{
			replyBuf[0] = 0x0;
			replyBuf[1] = 0x0;
			if( send( sockHfrClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
		}
		//通知视图进行显示
		PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
	}

leave:
	//清理环境，退出线程
	closesocket(sockHfrClient);
	WSACleanup();
}

void RecvHfBinconImagedata(LPVOID pParam)
{
	//监听DSP图像数据的线程
	CEagleeye2_ViewerDoc* pDoc=(CEagleeye2_ViewerDoc*) pParam;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockHfrBinClient;
	struct sockaddr_in	addr;
	int					i,cnt;
	BOOL				flag = TRUE;
	UINT				recvdatalen;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	UINT				tmp;
	BYTE				replyBuf[2];

	//获取目标相机的IP地址
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("无法初始化全分辨率连续模式套接口对象");
		return;
	}
	sockHfrBinClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockHfrBinClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(HFRBINIMGPORT);
	//连接目标相机
	if ( connect( sockHfrBinClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
	//开始接受相机上传图像数据
	while(1){
		i = 0;
		while( i < 4){
			cnt = recv( sockHfrBinClient, (char*)&uiJpgDataLen+i, 4-i, 0 );
			if( cnt <=0){
				goto leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = uiJpgDataLen;
			}
		}
		i = 0;
		listJpgBuff[CurrBuff].sizeBuff = recvdatalen + 256;
		pDoc->m_bIsJpeg = TRUE;
		while( i < recvdatalen ){
			cnt = recv( sockHfrBinClient, listJpgBuff[CurrBuff].pBuff+i, recvdatalen-i, 0 );
			if( cnt <= 0 ){
				goto leave;
			}
			i += cnt;
		}
		CurrBuff++;
		if(CurrBuff==5){
			CurrBuff = 0;
		}
		PrevBuff++;
		if(PrevBuff==5){
			PrevBuff = 0;
		}
		pDoc->m_bIsDataFromCamera = TRUE;
		//在高祯率模式下回复相机是否结束当前连接，若回复0xffff则结束，否则接续接受数据
		if( !pDoc->m_bIsHFRBinMode ){
			replyBuf[0] = 0xff;
			replyBuf[1] = 0xff;
			if( send( sockHfrBinClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
			goto leave;
		}
		else{
			replyBuf[0] = 0x0;
			replyBuf[1] = 0x0;
			if( send( sockHfrBinClient, (char*)replyBuf, 2, 0 ) < 0 ){
				goto leave;
			}
		}
		//通知视图进行显示
		PostMessage(pDoc->m_hViewWnd, WM_RECVTRGIMAGEOK, 1, 0);
	}

leave:
	//清理环境，退出线程
	closesocket(sockHfrBinClient);
	WSACleanup();
}
//////////////////////////////////////////////////////////////////////////
//向目标相机发送命令
BOOL CEagleeye2_ViewerDoc::SendCMDtoDSP( char* cmdbuf)
{
	char				*buf = cmdbuf;
	WORD				wVersionRequested;
	WSADATA				wsaData;
	SOCKET				sockCMDClient;
	struct sockaddr_in	addr;
	UINT				i,cnt,tmp;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	u_long				bNonblock = 1;
	UINT				datalen,recvdatalen;

	//建立使用TCP协议的向目标相机发送指令的套接口对象
	tmp1 = TargetIPAdd1; tmp2 = TargetIPAdd2;
	tmp3 = TargetIPAdd3; tmp4 = TargetIPAdd4;
	tmp =	tmp1;
	tmp |=	tmp2 << 8;
	tmp |=	tmp3 << 16;
	tmp |=	tmp4 << 24;
	wVersionRequested = MAKEWORD(1, 1);
	i = WSAStartup(wVersionRequested, &wsaData);
	if (i != 0){
		AfxMessageBox("无法初始化发送命令套接口对象");
		return FALSE;
	}
	sockCMDClient = socket( AF_INET, SOCK_STREAM, 0 );
	if( sockCMDClient < 0){
		goto leave;
	}
	addr.sin_family		= AF_INET;
	addr.sin_addr.s_addr	= tmp;
	addr.sin_port		= htons(COMMANDPORT);
	//连接目标相机
	if ( connect( sockCMDClient, (const struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		goto leave;
	}
	//发送命令ID和参数的长度(字节数)
	if( send( sockCMDClient, buf, 6, 0 ) < 0 ){
		goto leave;
	}
	//发送命令参数
	if( send( sockCMDClient, buf+6, *(UINT*)(buf+2), 0 ) < 0 ){
		goto leave;
	}
	//若需相机应答，接受相机回应
	if( buf[1] ){
		i = 0;
		while( i < 4){
			cnt = recv( sockCMDClient, (char*)&datalen+i, 4-i, 0 );
			if( cnt <=0){
				goto	leave;
			}
			i += cnt;
			if( i == 4){
				recvdatalen = datalen;
			}
		}
		i = 0;
		while( i < recvdatalen){
			cnt = recv( sockCMDClient, cRcvBuf+i, recvdatalen-i, 0 );
			if( cnt <=0){
				goto	leave;
			}
			i += cnt;
		}
	}
	//关闭套接口对象并返回真值
	closesocket(sockCMDClient);
	WSACleanup();
	return TRUE;

//遇到错误，关闭套接口对象并返回
leave:
	closesocket(sockCMDClient);
	WSACleanup();
	m_bIsEnsureTgtIP = FALSE;
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//保存目标相机的IP地址，同时发送命令使目标相机启动全分辨率连续模式下的数据上传
void CEagleeye2_ViewerDoc::OnConfigTargetip() 
{
	CMainFrame	*pMain = NULL;
	UINT		tmp1 = 0, tmp2 = 0;

	if(m_TargetIPDlg.DoModal()==IDOK){
		TargetIPAdd1 = m_TargetIPDlg.m_byIPAdd1;
		TargetIPAdd2 = m_TargetIPDlg.m_byIPAdd2;
		TargetIPAdd3 = m_TargetIPDlg.m_byIPAdd3;
		TargetIPAdd4 = m_TargetIPDlg.m_byIPAdd4;
		//复位三种模式的标志和确认目标IP标志
		m_bIsTrgMode	= FALSE;
		m_bIsFconMode	= FALSE;
		m_bIsHFRMode	= FALSE;
		m_bIsHFRBinMode = FALSE;
		m_bIsEnsureTgtIP = FALSE;
		//检测触发模式服务器是否还没有退出连接，若没有则断开
		cmdbuf[0] = 0x23;
		cmdbuf[1] = 0x00;//需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x00;
		if(!SendCMDtoDSP(cmdbuf)){
			AfxMessageBox("连接相机失败!");
			return;
		}
		//查询目标相机的工作模式，若连接成功启动相应的客户端线程
		cmdbuf[0] = 0x03;
		cmdbuf[1] = 0x01;//需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x00;
		if(SendCMDtoDSP(cmdbuf)){
			m_bIsEnsureTgtIP = TRUE;
			//获取相关参数配置界面
			//配置可编程I/O的口方向和输出值以更新界面
			tmp1 = *(UINT*)(cRcvBuf+16);
			tmp2 = *(UINT*)(cRcvBuf+20);
			//port1方向
			if( (tmp1&0x001) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort1IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio1 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort1IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio1 = 1;
				if( (tmp2&0x001) == 0 ){
					m_GeneralIODlg.m_bPort1OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort1OUTValue = TRUE;
				}
			}
			//port2方向
			if( (tmp1&0x002) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort2IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio2 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort2IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio2 = 1;
				if( (tmp2&0x002) == 0 ){
					m_GeneralIODlg.m_bPort2OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort2OUTValue = TRUE;
				}
			}
			//port3方向
			if( (tmp1&0x004) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort3IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio3 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort3IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio3 = 1;
				if( (tmp2&0x004) == 0 ){
					m_GeneralIODlg.m_bPort3OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort3OUTValue = TRUE;
				}
			}
			//port4方向
			if( (tmp1&0x008) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort4IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio4 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort4IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio4 = 1;
				if( (tmp2&0x008) == 0 ){
					m_GeneralIODlg.m_bPort4OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort4OUTValue = TRUE;
				}
			}
			//port5方向
			if( (tmp1&0x010) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort5IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio5 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort5IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio5 = 1;
				if( (tmp2&0x010) == 0 ){
					m_GeneralIODlg.m_bPort5OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort5OUTValue = TRUE;
				}
			}
			//port6方向
			if( (tmp1&0x020) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort6IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio6 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort6IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio6 = 1;
				if( (tmp2&0x020) == 0 ){
					m_GeneralIODlg.m_bPort6OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort6OUTValue = TRUE;
				}
			}
			//port7方向
			if( (tmp1&0x040) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort7IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio7 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort7IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio7 = 1;
				if( (tmp2&0x040) == 0 ){
					m_GeneralIODlg.m_bPort7OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort7OUTValue = TRUE;
				}
			}
			//port8方向
			if( (tmp1&0x080) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort8IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio8 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort8IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio8 = 1;
				if( (tmp2&0x080) == 0 ){
					m_GeneralIODlg.m_bPort8OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort8OUTValue = TRUE;
				}
			}
			//port9方向
			if( (tmp1&0x100) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort9IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio9 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort9IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio9 = 1;
				if( (tmp2&0x100) == 0 ){
					m_GeneralIODlg.m_bPort9OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort9OUTValue = TRUE;
				}
			}
			//port10方向
			if( (tmp1&0x200) == 0 ){
				//输入
				m_GeneralIODlg.m_bIsPort10IN = TRUE;
				m_GeneralIODlg.m_pioIndicator.pio10 = 0;
			}
			else{
				//输出
				m_GeneralIODlg.m_bIsPort10IN = FALSE;
				m_GeneralIODlg.m_pioIndicator.pio10 = 1;
				if( (tmp2&0x200) == 0 ){
					m_GeneralIODlg.m_bPort10OUTValue = FALSE;
				}
				else{
					m_GeneralIODlg.m_bPort10OUTValue = TRUE;
				}
			}
			//配置图像采集格式
			switch( cRcvBuf[15] ){
				case RGB_ARRAY:
					m_bIsRGBArray = TRUE;
					m_bIsYUVArray = FALSE;
					m_bIsBAYERArray = FALSE;
					break;
				case YUV_ARRAY:
					m_bIsRGBArray = FALSE;
					m_bIsYUVArray = TRUE;
					m_bIsBAYERArray = FALSE;
					break;
				case BAYER_ARRAY:
					m_bIsRGBArray = FALSE;
					m_bIsYUVArray = FALSE;
					m_bIsBAYERArray = TRUE;
					break;
				default:
					break;
			}
			//配置运行模式
			switch( cRcvBuf[14] ) {
				case TrgMode:
					m_bIsTrgMode = TRUE;
					_beginthread(RecvTrgImagedata,0,(LPVOID)this);
					break;
				case FconMode:
					m_bIsFconMode = TRUE;
					_beginthread(RecvConImagedata,0,(LPVOID)this);
					break;
				case HfrMode:
					m_bIsHFRMode = TRUE;
					_beginthread(RecvHfconImagedata,0,(LPVOID)this);
					break;
				case HfrBinMode:
					m_bIsHFRBinMode = TRUE;
					_beginthread(RecvHfBinconImagedata,0,(LPVOID)this);
					break;
				default:
					break;
			}
			//配置白平衡环境选项的界面显示
			switch( cRcvBuf[13] ) {
				case DAYLIGHT:
					m_WhiteBalanceDlg.m_uiWBType = 0;
					break;
				case FLUORESCENCE:
					m_WhiteBalanceDlg.m_uiWBType = 1;
					break;
				case WITHOUTWB:
					m_WhiteBalanceDlg.m_uiWBType = 2;
					break;
				default:
					break;
				}
			//配置闪光灯同步输出状态
			if( cRcvBuf[8] ){
				m_GeneralIODlg.m_bIsFlashSynEnable = TRUE;
			}
			else{
				m_GeneralIODlg.m_bIsFlashSynEnable = FALSE;
			}
			//配置是否允许模式切换到触发模式后是否允许自动触发抓取一祯图像
			if( cRcvBuf[9] ){
				m_bAutoTrgEnable = TRUE;
			}
			else{
				m_bAutoTrgEnable = FALSE;
			}
			//配置是否使能图像LUT映射变换
			if( cRcvBuf[10] ){
				m_bLUTEnable = TRUE;
			}
			else{
				m_bLUTEnable = FALSE;
			}
			//配置继电器是否闭合
			if( cRcvBuf[11] ){
				m_bRelayIsOpen = FALSE;
			}
			else{
				m_bRelayIsOpen = TRUE;
			}
			//配置是否使能看门狗和设置看门狗定时器时间界面显示值
			if( cRcvBuf[12] ){
				m_WDTDlg.m_bIsWDTEnable = TRUE;
			}
			else{
				m_WDTDlg.m_bIsWDTEnable = FALSE;
			}
			m_WDTDlg.m_iWDT = *(unsigned int*)(cRcvBuf+4);
			//设置传感器输出信号增益界面显示值
			m_AfeConfigDlg.m_fAfeVGAGain = *(float*)(cRcvBuf);
			//////////////////////////////////////////////////////////////////////////
			//获得主框架类的窗口对象,通知主框架类更新状态条
			pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
			m_hMainFrameWnd = pMain->GetSafeHwnd();
			PostMessage(m_hMainFrameWnd, WM_SWICHNEWCAMERA, 0, 0);
		}
		else{
			m_bIsEnsureTgtIP = FALSE;
			AfxMessageBox("连接目标相机失败，请核对目标相机的IP地址");
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//						相机触发模式的相关控制
//////////////////////////////////////////////////////////////////////////
//切换相机到触发模式
void CEagleeye2_ViewerDoc::OnOmcTrigger() 
{
	m_bIsTrgMode	= TRUE;
	m_bIsFconMode	= FALSE;
	m_bIsHFRMode	= FALSE;
	m_bIsHFRBinMode = FALSE;
	//发送命令切换相机到触发模式
	cmdbuf[0] = 0x01;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	cmdbuf[6] = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
	else{
		_beginthread( RecvTrgImagedata, 0, (LPVOID)this);
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOmcTrigger(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
	pCmdUI->SetCheck(m_bIsTrgMode);
}
//配置相机在触发模式下的工作状态
void CEagleeye2_ViewerDoc::OnTrgmodeConfig() 
{
	if(m_TriggerModeCfgDlg.DoModal()==IDOK){
		int cmdlen = 0;
		//触发模式下的相机配置
		cmdbuf[0] = 0x04;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		//抓拍地点
		cmdlen += strlen(m_TriggerModeCfgDlg.m_strLocation);
		if( cmdlen>0 ){
			memcpy( cmdbuf+6, m_TriggerModeCfgDlg.m_strLocation, cmdlen);
		}
		memset( cmdbuf+6+cmdlen, '\0', 101-cmdlen );
		cmdlen = 101;
		//车道号
		cmdbuf[cmdlen+6] = m_TriggerModeCfgDlg.m_iWayIndex;
		cmdlen += 1;
		//触发方式
		if( m_TriggerModeCfgDlg.m_bIsSingalCoin ){
			cmdbuf[cmdlen+6] = 1;
		}
		else{
			cmdbuf[cmdlen+6] = 0;
		}
		cmdlen += 1;
		//若为双线圈触发，线圈间距
		*(float*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_fInterval;
		cmdlen += 4;
		//快门增益控制
		if( m_TriggerModeCfgDlg.m_bIsManualCtrl ){
			cmdbuf[cmdlen+6] = 0;
		}
		else{
			cmdbuf[cmdlen+6] = 1;
		}
		cmdlen += 1;
		//手动模式下的快门设置
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_uiTrgShutterTime;
		cmdlen += 4;
		//手动模式下的增益设置
		*(float*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_fTrgPga;
		cmdlen += 4;
		//自动模式下的快门上限设置
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iShutLimit;
		cmdlen += 4;
		//自动模式下的增益上限设置
		*(float*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_fPGALimit;
		cmdlen += 4;
		//自动模式下的当前快门设置
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iCurShut;
		cmdlen += 4;
		//自动模式下的当前增益设置
		*(float*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_fCurPga;
		cmdlen += 4;
		//自动模式下的测光区域起始行
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iSRow;
		cmdlen += 4;
		//自动模式下的测光区域起始列
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iSCol;
		cmdlen += 4;
		//自动模式下的测光区域终止行
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iERow;
		cmdlen += 4;
		//自动模式下的测光区域终止列
		*(UINT*)(cmdbuf+cmdlen+6) = m_TriggerModeCfgDlg.m_iECol;
		cmdlen += 4;
		//自动模式下的测光区域期望平均亮度
		cmdbuf[cmdlen+6] = m_TriggerModeCfgDlg.m_byYMean;
		cmdlen += 1;
		//自动模式下是否允许自测光触发
		if(m_TriggerModeCfgDlg.m_bEnableADLI){
			cmdbuf[cmdlen+6] = 1;
		}
		else{
			cmdbuf[cmdlen+6] = 0;
		}
		cmdlen += 1;
		//自动模式下是自测光触发周期
		cmdbuf[cmdlen+6] = m_TriggerModeCfgDlg.m_iADLIP;
		cmdlen += 1;
		//是否保存参数
		if(m_TriggerModeCfgDlg.m_bSaveParam){
			cmdbuf[cmdlen+6] = 1;
		}
		else{
			cmdbuf[cmdlen+6] = 0;
		}
		cmdlen += 1;
		//发送命令配置相机在触发模式下的曝光时间
		cmdbuf[0] = 0x04;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = cmdlen;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateTrgmodeConfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsTrgMode&&m_bIsEnsureTgtIP);
}
//软件触发抓取一祯图像
void CEagleeye2_ViewerDoc::OnOpeGrab() 
{
	//发送命令软触发抓取一祯图像
	cmdbuf[0] = 0x02;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeGrab(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsTrgMode&&m_bIsEnsureTgtIP);
}
//////////////////////////////////////////////////////////////////////////
//					相机全分辨连续模式的相关控制
//////////////////////////////////////////////////////////////////////////
//切换相机到全分辨率连续模式
void CEagleeye2_ViewerDoc::OnOmcMjpeg() 
{
	BOOL	bStopTrgSvr = FALSE;

	if( m_bIsTrgMode ){
		bStopTrgSvr = TRUE;
	}
	m_bIsTrgMode	= FALSE;
	m_bIsFconMode	= TRUE;
	m_bIsHFRMode	= FALSE;
	m_bIsHFRBinMode = FALSE;
	//是否停止相机端触发模式的服务
	if( bStopTrgSvr ){
		//发送停止触发模式下服务的标志
		cmdbuf[0] = 0x21;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x00;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
			return;
		}
	}
	Sleep( 200 );
	//切换相机到全分辨率连续模式
	cmdbuf[0] = 0x01;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 1;
	cmdbuf[6] = 0x01;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
	else{
		_beginthread(RecvConImagedata,0,(LPVOID)this);
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOmcMjpeg(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
	pCmdUI->SetCheck(m_bIsFconMode);
}
//配置相机在全分辨连续模式下的工作状态
void CEagleeye2_ViewerDoc::OnMjpegmodeConfig() 
{
	if(m_MJPEGModeCfgDlg.DoModal()==IDOK){
		if( m_MJPEGModeCfgDlg.m_bIsManual){
			//设置相机在全分辨连续模式下为手动运行模式
			cmdbuf[0] = 0x0f;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 0;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在全分辨连续模式的手动运行方式下的曝光时间
			cmdbuf[0] = 0x05;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_MJPEGModeCfgDlg.m_uiMJPEGShutterTime;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
		}
		else{
			//设置相机在全分辨连续模式下为自动运行模式
			cmdbuf[0] = 0x0f;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 1;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在全分辨连续模式下的信号增益自动调节的区间上限
			cmdbuf[0] = 0x13;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x04;
			*(float*)(cmdbuf+6) = m_MJPEGModeCfgDlg.m_fPGAUpLimit;
			m_HFRModeCfgDlg.m_fPGAUpLimit = m_MJPEGModeCfgDlg.m_fPGAUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在全分辨连续模式下的曝光时间自动调节的区间上限
			cmdbuf[0] = 0x10;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_MJPEGModeCfgDlg.m_uiAutoShutterUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在全分辨连续模式下的图像平均亮度阈值
			cmdbuf[0] = 0x14;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = m_MJPEGModeCfgDlg.m_uiBrightnessTH;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateMjpegmodeConfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsFconMode&&m_bIsEnsureTgtIP);
}
//////////////////////////////////////////////////////////////////////////
//					相机高祯率模式的相关控制
//////////////////////////////////////////////////////////////////////////
//切换相机到高祯率模式
void CEagleeye2_ViewerDoc::OnOmcHfr() 
{
	BOOL	bStopTrgSvr = FALSE;

	if( m_bIsTrgMode ){
		bStopTrgSvr = TRUE;
	}
	m_bIsTrgMode	= FALSE;
	m_bIsFconMode	= FALSE;
	m_bIsHFRMode	= TRUE;
	m_bIsHFRBinMode = FALSE;
	//是否停止相机端触发模式的服务
	if( bStopTrgSvr ){
		//发送停止触发模式下服务的标志
		cmdbuf[0] = 0x21;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x00;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
			return;
		}
	}
	//切换相机到高祯率模式
	cmdbuf[0] = 0x01;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 1;
	cmdbuf[6] = 0x02;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
	else{
		_beginthread(RecvHfconImagedata,0,(LPVOID)this);
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOmcHfr(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
	pCmdUI->SetCheck(m_bIsHFRMode);
}
//配置相机在高祯率模式下的工作状态
void CEagleeye2_ViewerDoc::OnHfrmodeConfig() 
{
	if(m_HFRModeCfgDlg.DoModal()==IDOK){
		if( m_HFRModeCfgDlg.m_bIsManual){
			//设置相机在高祯率模式下为手动运行模式
			cmdbuf[0] = 0x11;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 0;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在高祯率模式的手动运行方式下的曝光时间
			cmdbuf[0] = 0x06;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_HFRModeCfgDlg.m_uiHFRShutterTime;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
		}
		else{
			//设置相机在高祯率模式下为自动运行模式
			cmdbuf[0] = 0x11;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 1;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在高祯率模式下的信号增益自动调节的区间上限
			cmdbuf[0] = 0x13;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x04;
			*(float*)(cmdbuf+6) = m_HFRModeCfgDlg.m_fPGAUpLimit;
			m_MJPEGModeCfgDlg.m_fPGAUpLimit = m_HFRModeCfgDlg.m_fPGAUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在高祯率模式下的曝光时间自动调节的区间上限
			cmdbuf[0] = 0x12;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_HFRModeCfgDlg.m_uiAutoShutterUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在高祯率模式下的图像平均亮度阈值
			cmdbuf[0] = 0x15;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = m_HFRModeCfgDlg.m_uiBrightnessTH;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateHfrmodeConfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsHFRMode&&m_bIsEnsureTgtIP);
}
//////////////////////////////////////////////////////////////////////////
//					相机高祯率高灵敏度模式的相关控制
//////////////////////////////////////////////////////////////////////////
//切换相机到高祯率高灵敏度模式
void CEagleeye2_ViewerDoc::OnOmcHfrb() 
{
	BOOL	bStopTrgSvr = FALSE;

	if( m_bIsTrgMode ){
		bStopTrgSvr = TRUE;
	}
	m_bIsTrgMode	= FALSE;
	m_bIsFconMode	= FALSE;
	m_bIsHFRMode	= FALSE;
	m_bIsHFRBinMode = TRUE;
	//是否停止相机端触发模式的服务
	if( bStopTrgSvr ){
		//发送停止触发模式下服务的标志
		cmdbuf[0] = 0x21;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x00;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
			return;
		}
	}
	//切换相机到高祯率模式
	cmdbuf[0] = 0x01;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 1;
	cmdbuf[6] = 0x03;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
	else{
		_beginthread(RecvHfBinconImagedata,0,(LPVOID)this);
	}
}

void CEagleeye2_ViewerDoc::OnUpdateOmcHfrb(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
	pCmdUI->SetCheck(m_bIsHFRBinMode);
}

void CEagleeye2_ViewerDoc::OnHfrbinmodeConfig() 
{
	if(m_HFRBINModeCfgDlg.DoModal()==IDOK){
		if( m_HFRBINModeCfgDlg.m_bIsManual){
			//设置相机在高祯率模式下为手动运行模式
			cmdbuf[0] = 0x25;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 0;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在高祯率模式的手动运行方式下的曝光时间
			cmdbuf[0] = 0x24;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_HFRBINModeCfgDlg.m_uiHFRBINShutterTime;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
		}
		else{
			//设置相机在高祯率模式下为自动运行模式
			cmdbuf[0] = 0x25;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = 1;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在高祯率模式下的信号增益自动调节的区间上限
			cmdbuf[0] = 0x13;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x04;
			*(float*)(cmdbuf+6) = m_HFRBINModeCfgDlg.m_fPGAUpLimit;
			m_MJPEGModeCfgDlg.m_fPGAUpLimit = m_HFRBINModeCfgDlg.m_fPGAUpLimit;
			m_HFRModeCfgDlg.m_fPGAUpLimit = m_HFRBINModeCfgDlg.m_fPGAUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在高祯率模式下的曝光时间自动调节的区间上限
			cmdbuf[0] = 0x26;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x03;
			*(UINT*)(cmdbuf+6) = m_HFRBINModeCfgDlg.m_uiAutoShutterUpLimit;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置相机在高祯率模式下的图像平均亮度阈值
			cmdbuf[0] = 0x27;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			*(UINT*)(cmdbuf+6) = m_HFRBINModeCfgDlg.m_uiBrightnessTH;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
		}
	}
}

void CEagleeye2_ViewerDoc::OnUpdateHfrbinmodeConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bIsHFRBinMode&&m_bIsEnsureTgtIP);
}

//////////////////////////////////////////////////////////////////////////
//					相机的其它控制
//////////////////////////////////////////////////////////////////////////
//重新配置目标相机的IP地址，配置完毕相机重启后以新的IP地址工作
void CEagleeye2_ViewerDoc::OnConfigNewip() 
{
	int					IPAddress;
	BYTE				tmp1,tmp2,tmp3,tmp4;
	CString				macadd;

	if(m_NewIPMACDlg.DoModal()==IDOK){
		tmp1 = m_NewIPMACDlg.m_byIPAdd1; tmp2 = m_NewIPMACDlg.m_byIPAdd2; tmp3 = m_NewIPMACDlg.m_byIPAdd3; tmp4 = m_NewIPMACDlg.m_byIPAdd4;
		IPAddress =	tmp1;
		IPAddress |=	tmp2 << 8;
		IPAddress |=	tmp3 << 16;
		IPAddress |=	tmp4 << 24;
		//设置目标相机的新IP地址
		cmdbuf[0] = 0x1a;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x04;
		*(UINT*)(cmdbuf+6) = IPAddress;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigNewip(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//重新配置目标相机的MAC地址，配置完毕相机重启后以新的MAC地址工作
void CEagleeye2_ViewerDoc::OnConfigNewmac() 
{
	CString				macadd;
	int					i,j;

	if(m_MACCfgDlg.DoModal()==IDOK){
		//Set new MAC Address
		macadd = m_MACCfgDlg.m_strMacAdd4 + m_MACCfgDlg.m_strMacAdd5 + m_MACCfgDlg.m_strMacAdd6;
		i = macadd.GetLength();
		if( i!=6 ){
			if( i==0 ){
				return;
			}
			AfxMessageBox("MAC address is invaliable!");
			return;
		}
		for(i=0; i<3; i++){
			unsigned char tmp[2];
			for(j=0;j<2;j++){
				if( (macadd[i*2+j]>=48&&macadd[i*2+j]<=57) || (macadd[i*2+j]>=65&&macadd[i*2+j]<=70) || (macadd[i*2+j]>=97&&macadd[i*2+j]<=102)){
					if(macadd[i*2+j]>=48&&macadd[i*2+j]<=57){
						tmp[j] = macadd[i*2+j] - 48;
					}
					if(macadd[i*2+j]>=65&&macadd[i*2+j]<=70){
						tmp[j] = macadd[i*2+j] - 55;
					}
					if(macadd[i*2+j]>=97&&macadd[i*2+j]<=102){
						tmp[j] = macadd[i*2+j] - 87;
					}
				}
				else{
					AfxMessageBox("MAC address is invaliable!");
					return;
				}
			}
			cmdbuf[i+9] = (tmp[0]*16 + tmp[1]);
		}
		//发送命令修改目标相机的MAC地址
		cmdbuf[0] = 0x1b;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x06;
		cmdbuf[6] = 0x08; cmdbuf[7] = 0x00; cmdbuf[8] = 0x28;	//MAC code for TI
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigNewmac(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//CCD信号的增益控制(0 - 35.5dB 可调)
void CEagleeye2_ViewerDoc::OnConfigAfegainctrl() 
{
	if(m_AfeConfigDlg.DoModal()==IDOK){
		//设置传感器输出信号的增益
		cmdbuf[0] = 0x07;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x04;
		*(float*)(cmdbuf+6) = m_AfeConfigDlg.m_fAfeVGAGain;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigAfegainctrl(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//白平衡控制选项
void CEagleeye2_ViewerDoc::OnConfigWhitebalance() 
{
	if(m_WhiteBalanceDlg.DoModal()==IDOK){
		//设置目标相机白平衡校正的光照条件
		cmdbuf[0] = 0x08;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x01;
		*(UINT*)(cmdbuf+6) = *(UINT*)(&m_WhiteBalanceDlg.m_uiWBType);
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigWhitebalance(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//可编程IO口和闪光灯同步输出使能控制
void CEagleeye2_ViewerDoc::OnConfigGioconfig() 
{
	if(m_GeneralIODlg.DoModal()==IDOK){
		//设置目标相机十个可编程I/O的方向
		cmdbuf[0] = 0x0d;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x02;
		*(UINT*)(cmdbuf+6) = *(UINT*)(&m_GeneralIODlg.m_pioIndicator)&0x3ff;
		m_GeneralIODlg.m_bIsPortDirAdj = FALSE;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
		//设置目标相机十个可编程I/O的输出电平，仅对输出口有效
		cmdbuf[0] = 0x0e;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x02;
		*(UINT*)(cmdbuf+6) = (*(UINT*)(&m_GeneralIODlg.m_pioIndicator)&0xffc00)>>10;
		m_GeneralIODlg.m_bIsPortOutAdj = FALSE;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
		//设置目标相机的闪光灯同步输出是否使能
		cmdbuf[0] = 0x0c;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x01;
		if( m_GeneralIODlg.m_bIsFlashSynEnable){
			*(UINT*)(cmdbuf+6) = 1;//使能
		}
		else{
			*(UINT*)(cmdbuf+6) = 0;//禁止
		}
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigGioconfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//相机内部的实时时钟配置
void CEagleeye2_ViewerDoc::OnConfigRtcconfig() 
{
	CString strRTC;

	if(m_RTCConfigDlg.DoModal()==IDOK){
		//设置目标相机的RTC时间
		cmdbuf[0] = 0x18;
		cmdbuf[1] = 0x00;//不需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0x07;
		cmdbuf[6] = m_RTCConfigDlg.m_iYear%100;
		cmdbuf[7] = m_RTCConfigDlg.m_iMonth;
		cmdbuf[8] = m_RTCConfigDlg.m_iDay;
		cmdbuf[9] = m_RTCConfigDlg.m_iWeek;
		cmdbuf[10] = m_RTCConfigDlg.m_iHour;
		cmdbuf[11] = m_RTCConfigDlg.m_iMinute;
		cmdbuf[12] = m_RTCConfigDlg.m_iSecond;
		if( !SendCMDtoDSP(cmdbuf) ){
			AfxMessageBox("向相机发送命令失败");
		}
		//读取目标相机的RTC时间
		cmdbuf[0] = 0x19;
		cmdbuf[1] = 0x01;//需要相机回传数据
		*(UINT*)(cmdbuf+2) = 0;
		if( !SendCMDtoDSP( cmdbuf ) ){
			AfxMessageBox("向相机发送命令失败");
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigRtcconfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//相机的看门狗设置
void CEagleeye2_ViewerDoc::OnConfigWdtconfig() 
{
	if(m_WDTDlg.DoModal()==IDOK){
		
		if( m_WDTDlg.m_bIsWDTEnable ){
			//使能目标相机的看门狗
			cmdbuf[0] = 0x16;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			cmdbuf[6] = 0x01;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
			//设置目标相机的看门狗定时器(1~256秒)
			cmdbuf[0] = 0x17;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			cmdbuf[6] = m_WDTDlg.m_iWDT;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
		}
		else{
			//禁止目标相机的看门狗
			cmdbuf[0] = 0x16;
			cmdbuf[1] = 0x00;//不需要相机回传数据
			*(UINT*)(cmdbuf+2) = 0x01;
			cmdbuf[6] = 0x00;
			if( !SendCMDtoDSP(cmdbuf) ){
				AfxMessageBox("向相机发送命令失败");
			}
		}
	}
}
void CEagleeye2_ViewerDoc::OnUpdateConfigWdtconfig(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//控制相机抓取一祯当前图像进行自适应白平衡校正
void CEagleeye2_ViewerDoc::OnOpeAutowb() 
{
	//命令相机抓取一祯当前图像进行自适应白平衡校正
	cmdbuf[0] = 0x09;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeAutowb(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//控制相机在其它两种模式切换到触发模式后是否自动触发抓取一祯图像
void CEagleeye2_ViewerDoc::OnOpeAutotrgEnable() 
{
	//使能切换后自动触发功能
	m_bAutoTrgEnable = TRUE;
	cmdbuf[0] = 0x1f;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0x01;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeAutotrgEnable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bAutoTrgEnable);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
void CEagleeye2_ViewerDoc::OnOpeAutotrgDisable() 
{
	//禁止切换后自动触发功能
	m_bAutoTrgEnable = FALSE;
	cmdbuf[0] = 0x1f;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeAutotrgDisable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bAutoTrgEnable);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//是否使能图像LUT变换功能
void CEagleeye2_ViewerDoc::OnOpeLutEnable() 
{
	//使能LUT变换功能
	m_bLUTEnable = TRUE;
	cmdbuf[0] = 0x0a;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0x01;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeLutEnable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bLUTEnable);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
void CEagleeye2_ViewerDoc::OnOpeLutDisable() 
{
	//禁止LUT变换功能
	m_bLUTEnable = FALSE;
	cmdbuf[0] = 0x0a;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x02;
	cmdbuf[6] = 0x00;
	cmdbuf[7] = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeLutDisable(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bLUTEnable);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//继电器控制
void CEagleeye2_ViewerDoc::OnOpeRelayOpen() 
{
	//断开继电器
	m_bRelayIsOpen = TRUE;
	cmdbuf[0] = 0x1d;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeRelayOpen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bRelayIsOpen);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
void CEagleeye2_ViewerDoc::OnOpeRelayClose() 
{
	//闭合继电器
	m_bRelayIsOpen = FALSE;
	cmdbuf[0] = 0x1d;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 1;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeRelayClose(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bRelayIsOpen);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//设置相机图像采集格式为RGB阵列
void CEagleeye2_ViewerDoc::OnOpeImgformatRgb() 
{
	//设置相机图像采集格式为RGB阵列
	m_bIsRGBArray = TRUE;
	m_bIsYUVArray = FALSE;
	m_bIsBAYERArray = FALSE;
	cmdbuf[0] = 0x20;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeImgformatRgb(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bIsRGBArray);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//设置相机图像采集格式为YUV阵列
void CEagleeye2_ViewerDoc::OnOpeImgformatYuv() 
{
	//设置相机图像采集格式为YUV阵列
	m_bIsRGBArray = FALSE;
	m_bIsYUVArray = TRUE;
	m_bIsBAYERArray = FALSE;
	cmdbuf[0] = 0x20;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 1;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeImgformatYuv(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bIsYUVArray);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//设置相机图像采集格式为BAYER阵列
void CEagleeye2_ViewerDoc::OnOpeImgformatBayer() 
{
	//设置相机图像采集格式为YUV阵列
	m_bIsRGBArray = FALSE;
	m_bIsYUVArray = FALSE;
	m_bIsBAYERArray = TRUE;
	cmdbuf[0] = 0x20;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 2;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeImgformatBayer(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bIsBAYERArray);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//设置触发模式的快门速度的手动模式
void CEagleeye2_ViewerDoc::OnOpeTrgshutterctrlManual() 
{
	m_bTrgShutterIsManual = TRUE;
	cmdbuf[0] = 0x22;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 0;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeTrgshutterctrlManual(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bTrgShutterIsManual);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//设置触发模式的快门速度的自动模式
void CEagleeye2_ViewerDoc::OnOpeTrgshutterctrlAuto() 
{
	m_bTrgShutterIsManual = FALSE;
	cmdbuf[0] = 0x22;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x01;
	*(UINT*)(cmdbuf+6) = 1;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeTrgshutterctrlAuto(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bTrgShutterIsManual);
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}

//复位相机
void CEagleeye2_ViewerDoc::OnOpeResetsys() 
{
	//远程重启目标相机
	cmdbuf[0] = 0x00;
	cmdbuf[1] = 0x00;//不需要相机回传数据
	*(UINT*)(cmdbuf+2) = 0x00;
	if( !SendCMDtoDSP(cmdbuf) ){
		AfxMessageBox("向相机发送命令失败");
	}
}
void CEagleeye2_ViewerDoc::OnUpdateOpeResetsys(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
//远程更新用户程序
void CEagleeye2_ViewerDoc::OnOpeRemoteupdatedspprrom() 
{
	char CmdLine[]="EagleeyeDSPRemoteUpdate";
	UINT RES = WinExec(CmdLine, SW_SHOW);
}
void CEagleeye2_ViewerDoc::OnUpdateOpeRemoteupdatedspprrom(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsEnsureTgtIP);
}
