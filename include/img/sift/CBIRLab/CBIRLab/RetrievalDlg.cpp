// RetrievalDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CBIRLab.h"
#include "RetrievalDlg.h"
#include "MatchSIFT.h"
#include "MySIFT.h"
#include "RetrievalResult.h"

// CRetrievalDlg 对话框

IMPLEMENT_DYNAMIC(CRetrievalDlg, CDialog)

CRetrievalDlg::CRetrievalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRetrievalDlg::IDD, pParent)
	, m_editResult(_T(""))
	, m_showFileName(_T(""))
	, m_editIndex(_T(""))
{
	showIndex = 0;
	m_currentImage = NULL;
	m_imageShow = NULL;
	res = 0;
	n_pos = 0;
}

CRetrievalDlg::~CRetrievalDlg()
{
}

void CRetrievalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_editResult);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_showFileName);
	DDX_Text(pDX, IDC_EDIT_INDEX, m_editIndex);
}


BEGIN_MESSAGE_MAP(CRetrievalDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_NEXT, CRetrievalDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PRIOR, CRetrievalDlg::OnBnClickedButtonPrior)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CRetrievalDlg 消息处理程序

BOOL CRetrievalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString str;
	
	//得到图片空间的尺寸
	// 获得显示控件的 DC
	CDC* pDC = GetDlgItem( IDC_PICTURE_RES_CTL ) ->GetDC();
	// 获取 HDC(设备句柄) 来进行绘图操作		
	HDC hDC = pDC ->GetSafeHdc();				
	CRect rect;
	GetDlgItem(IDC_PICTURE_RES_CTL) ->GetClientRect( &rect );
	// 求出图片控件的宽和高
	picWidth = rect.right - rect.left;			
	picHeight = rect.bottom - rect.top;

	ReleaseDC( pDC );

	//初始化m_imageShow
	CvSize ImgSize;
	ImgSize.height = picHeight;
	ImgSize.width = picWidth;
	m_imageShow = cvCreateImage( ImgSize, IMG_DEPTH_8U, IMAGE_CHANNELS );
	cvSet(m_imageShow, cvScalarAll(255));

	//初始化待检索图像的SIFT特征
	//imageSift.initialize(m_imagePath);

	if (res == 0)
	{
		str.Format("文件夹中没有格式正确的图片.\r\n");
		m_editResult += str;
	}
	else if (res == -1)
	{
		str.Format("无匹配结果.\r\n");
		m_editResult += str;
	}
	else if(m_currentImage)
	{
		ResizeImage( m_currentImage, m_imageShow );
		ShowImage( m_imageShow, IDC_PICTURE_RES_CTL);

		m_showFileName = (vcRetrievalResultSet[showIndex]).imagePath;
		m_editIndex.Format("当前第%d/%d张", showIndex+1, vcRetrievalResultSet.size());
	}

	str.Format("\r\n本次检索消耗时间：%d秒.\r\n", m_time/1000);
	m_editResult += str;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CRetrievalDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	// 重绘对话框
	CDialog::OnPaint();
	// 更新windows窗口，如果无这步调用，图片显示还会出现问题
	CDialog::UpdateWindow();
	// 重绘图片函数
	ShowImage( m_imageShow, IDC_PICTURE_RES_CTL);
}

void CRetrievalDlg::ShowImage( IplImage* img, UINT ID)
{
	if (img == NULL)
	{
		return;
	}
	// 获得显示控件的 DC
	CDC* pDC = GetDlgItem( ID ) ->GetDC();
	// 获取 HDC(设备句柄) 来进行绘图操作		
	HDC hDC = pDC ->GetSafeHdc();				

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
	// 求出图片控件的宽和高
	int rw = rect.right - rect.left;			
	int rh = rect.bottom - rect.top;
	// 读取图片的宽和高
	int iw = img->width;	
	int ih = img->height;
	// 使图片的显示位置正好在控件的正中
	int tx = (int)(rw - iw)/2;	
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );
	// 复制图片
	CvvImage cimg;
	cimg.CopyOf( img );
	// 将图片绘制到显示控件的指定区域内	
	cimg.DrawToHDC( hDC, &rect );	

	ReleaseDC( pDC );
}

//把图片src缩放到dest的大小，大的缩放，小的则不变
void CRetrievalDlg::ResizeImage(IplImage* src, IplImage* dest)
{
	if (!src || !dest)
	{
		return;
	}
	// 读取图片的宽和高
	int w = src->width;
	int h = src->height;
	float scale1 = (float)w/(float)picWidth;
	float scale2 = (float)h/(float)picHeight;

	// 找出宽和高中缩放比例的较大值者
	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = (scale1 > scale2) ? scale1: scale2;

	// 缩放后图片的宽和高
	int nw, nh;
	if(scale > 1.0f)
	{
		nw = (int)( w/scale );
		nh = (int)( h/scale );
	}
	else
	{
		nw = w;
		nh = h;
	}

	// 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
	int tlx = (nw > picWidth)? 0: (int)(picWidth-nw)/2;
	int tly = (picHeight > nh)? (int)(picHeight-nh)/2: 0;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI( dest, cvRect( tlx, tly, nw, nh) );

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize( src, dest );

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI( dest );
}

void CRetrievalDlg::computeResults()
{
	CFileFind tempFind;
	BOOL bFound; //判断是否成功找到文件
	bFound = tempFind.FindFile(m_folderPath + "*.*");   //修改" "内内容给限定查找文件类型
	int index = 0;
	CString strTmp, str;

	//遍历所有文件
	while(bFound)
	{
		//第一次执行FindNextFile是选择到第一个文件，以后执行为选择到下一个文件
		bFound = tempFind.FindNextFile();

		//找到的不是文件夹，也不是返回上层的目录
		if(!tempFind.IsDots() && !tempFind.IsDirectory())
		{
			//保存文件名，包括后缀名
			strTmp = tempFind.GetFilePath();
			//将此文件添加到向量中
			CString strExtention = PathFindExtensionA(strTmp);
			strExtention.MakeLower();
			if (strExtention.Compare(".png") == 0 || strExtention.Compare(".jpg") == 0
				|| strExtention.Compare(".jpeg") == 0 || strExtention.Compare(".bmp") == 0 
				|| strExtention.Compare(".gif") == 0)
			{
				vcImageDataSet.push_back(strTmp);
			}
		}
	}
	tempFind.Close();

	if (vcImageDataSet.size() == 0)
	{
		//文件夹为空
		res = 0;
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//计算SIFT特征和匹配结果
	CMatchSIFT matchImage;
	index = 1;
	for(vector<CString>::iterator it = vcImageDataSet.begin(); it != vcImageDataSet.end(); ++it, ++index)
	{
		CMySIFT tmpSift1, tmpSift2;
		if (!tmpSift1.initialize(m_imagePath) || !tmpSift2.initialize(*it, true))
		{
			AfxMessageBox("读入图像"+(*it)+"有误！");
			continue;
		}
		CRetrievalResult tmpResult;
		matchImage.initialize(&tmpSift1, &tmpSift2);
		tmpResult.initialize(*it);
		if (matchImage.matchTwoImage(tmpResult))
		{
			//添加一个检索结果，包括图像的路径名和结果区域框框
			vcRetrievalResultSet.push_back(tmpResult);
		}
		n_pos = (int)((index*100)/vcImageDataSet.size());
		m_parentDlg->m_progressCtrl.SetPos(n_pos);
		m_parentDlg->UpdateData(FALSE);
	}

	m_editResult.Format("在%d张图片中共检索出了%d个结果区域。\r\n\r\n", 
		vcImageDataSet.size(), vcRetrievalResultSet.size());

	if (vcRetrievalResultSet.size() == 0)
	{
		//无匹配结果
		res = -1;
		return;
	}

	index = 1;
  {for (vector<CRetrievalResult>::iterator it = vcRetrievalResultSet.begin(); 
		it != vcRetrievalResultSet.end(); ++it, ++index)
	{
		if ((*it).vpRegions.size() >= 0)
		{
			str.Format("%d,  图片%s,  矩形区域:  (%d, %d), (%d, %d).\r\n",
				index, PathFindFileName((*it).imagePath), (((*it).vpRegions[0]).rectTopLeft).x, (((*it).vpRegions[0]).rectTopLeft).y, 
				(((*it).vpRegions[0]).rectBottomRight).x, (((*it).vpRegions[0]).rectBottomRight).y);
			m_editResult += str;
		}
  }}

	// 从数据结果中读取一个合法的图像数据文件
	while(showIndex < vcRetrievalResultSet.size())
	{
		m_currentImage = cvLoadImage(vcRetrievalResultSet[showIndex].imagePath, 1);
		if(m_currentImage)
		{
			//将当前待显示图像的所有检索结果区域标识出来
			for (int i = 0; i < vcRetrievalResultSet[showIndex].regionCount; i++)
			{
				cvRectangle(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectTopLeft, 
					((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectBottomRight, CC_RGB(255,0,255), 2, 8, 0);

				/*标注出四边形区域
				cvLine(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[0], ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[1], cvScalarAll(0), 2, 8, 0);
				cvLine(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[1], ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[2], cvScalarAll(0), 2, 8, 0);
				cvLine(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[2], ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[3], cvScalarAll(0), 1, 8, 0);
				cvLine(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[3], ((vcRetrievalResultSet[showIndex]).vpRegions[i]).points[0], cvScalarAll(0), 1, 8, 0);
				*/
			}
			break;
		}
		else
			showIndex++;
	}

	res = 1;
	return;
}
void CRetrievalDlg::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	showIndex++;
	if (showIndex >= vcRetrievalResultSet.size())
	{
		AfxMessageBox("已经到达最后一幅图像！");
		showIndex--;
		return;
	}
	m_currentImage = cvLoadImage(vcRetrievalResultSet[showIndex].imagePath, 1);
	if(m_currentImage)
	{
		//将当前待显示图像的所有检索结果区域标识出来
		for (int i = 0; i < vcRetrievalResultSet[showIndex].regionCount; i++)
		{
			cvRectangle(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectTopLeft, 
				((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectBottomRight, CC_RGB(255,0,255), 2, 8, 0);
		}
		// 对上一幅显示的图片数据清零
		if( m_imageShow )
			cvSet(m_imageShow, cvScalarAll(255));
		ResizeImage( m_currentImage, m_imageShow );
		ShowImage( m_imageShow, IDC_PICTURE_RES_CTL);
		m_showFileName = vcRetrievalResultSet[showIndex].imagePath;
		m_editIndex.Format("当前第%d/%d张", showIndex+1, vcRetrievalResultSet.size());
	}

	UpdateData(FALSE);
}

void CRetrievalDlg::OnBnClickedButtonPrior()
{
	// TODO: 在此添加控件通知处理程序代码
	showIndex--;
	if (showIndex < 0)
	{
		AfxMessageBox("已经到达第一幅一幅图像！");
		showIndex++;
		return;
	}
	m_currentImage = cvLoadImage(vcRetrievalResultSet[showIndex].imagePath, 1);
	if(m_currentImage)
	{
		//将当前待显示图像的所有检索结果区域标识出来
		for (int i = 0; i < vcRetrievalResultSet[showIndex].regionCount; i++)
		{
			cvRectangle(m_currentImage, ((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectTopLeft, 
				((vcRetrievalResultSet[showIndex]).vpRegions[i]).rectBottomRight, CC_RGB(255,0,255), 2, 8, 0);
		}
		// 对上一幅显示的图片数据清零
		if( m_imageShow )
			cvSet(m_imageShow, cvScalarAll(255));
		ResizeImage( m_currentImage, m_imageShow );
		ShowImage( m_imageShow, IDC_PICTURE_RES_CTL);
		m_showFileName = vcRetrievalResultSet[showIndex].imagePath;
		m_editIndex.Format("当前第%d/%d张", showIndex+1, vcRetrievalResultSet.size());
	}

	UpdateData(FALSE);
}

void CRetrievalDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_parentDlg->m_progressCtrl.SetPos(0);
	CDialog::OnClose();
}
