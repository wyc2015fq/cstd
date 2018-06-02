// DibMerger.cpp: implementation of the CDibMerger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Texture.h"
#include "Dib.h"
#include "DibMerger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDibMerger::CDibMerger()
{

}

CDibMerger::~CDibMerger()
{

}

/*
BOOL CDibMerger::SetComparDIB(CDib *bmp1, CDib *bmp2)
{
   
	//最大面积
    m_iMAXarea=0;
    //从第二幅图象的最左上角开始，从第一幅图象的最右上角开始，比点比列比块
	for(int rightbmp_h=0;rightbmp_h<bmp2->GetHeight()/3;rightbmp_h++)
	    for(int leftbmp_w=bmp1->GetWidth();leftbmp_w>=bmp1->GetWidth()/2;leftbmp_w--)
		    for(unsigned int leftbmp_h=0;leftbmp_h<bmp1->GetHeight();leftbmp_h++)
		{
			bmp2->Pix_X=0;                   
			bmp2->Pix_Y=rightbmp_h;
			bmp1->Pix_X=leftbmp_w;
			bmp1->Pix_Y=leftbmp_h;
			if(IsSamePix(bmp1,bmp2,bmp1->Pix_X,bmp1->Pix_Y,bmp2->Pix_X,bmp2->Pix_Y))
				if(IsSameCol(bmp1,bmp2,bmp1->Pix_X,bmp1->Pix_Y,bmp2->Pix_X,bmp2->Pix_Y))
				if(IsSameRect(bmp1,bmp2))
						break;
					else 
						continue;
		}

	//如果相同块的面积不为零则拼接
     if(m_iMAXarea!=0)
	 {
		 MergeDIB(bmp1,bmp2);
		 return TRUE;
	 }
	 else
	 {
		 MessageBox("没有找到合并点，不给予合并");
		 return FALSE;
	 }
}

long CDibMerger::GetPixOffset(unsigned int X, unsigned int Y, CDib *bmp)
{

   //计算点在图象中的位置
	if((X>=0&&X<bmp->GetWidth())&&(Y>=0&&Y<bmp->GetHeight()))
		return (Y*bmp->GetDibSaveDim().cx+X*(bmp->GetBiBitCount()/8));//要在CDib中定义bmp_line_w和bmp_biBitCount
	return 0;
}


BOOL CDibMerger::IsSamePix(CDib *bmp1, CDib *bmp2, int x1, int y1, int x2, int y2)
{


	BYTE *p,*q;
	p=bmp1->m_pDibBits+GetPixOffset(x1,y1,bmp1);
	q=bmp2->m_pDibBits+GetPixOffset(x2,y2,bmp2);
	for(int i=0;i<bmp1->GetBiBitCount()/8;i++)
		if(abs(p[i]-q[i])>50)

			return FALSE;
		//OR if(abs(p[i]-q[i])!=0)return FALSE;  //m_pDibBits
		else 
			return TRUE;
		return TRUE;
}


BOOL CDibMerger::IsSameCol(CDib *bmp1, CDib *bmp2, int x1, int y1, int x2, int y2)
{

    //计算当前象素点到图象1底部的高度
	unsigned int below_h1=bmp1->GetHeight()-y1;
	//计算图象2当前象素点到底部的高度
	unsigned int below_h2=bmp2->GetHeight()-y2;
	unsigned int h=(below_h1>below_h2)?below_h2:below_h1;

	for(unsigned int i=0;i<h;i++)
	{
		//判断是否是相同的象素点，如果是，则在同一列中往下比较
		if(!IsSamePix(bmp1,bmp2,x1,y1,x2,y2))
			return FALSE ;
		y1++;
		y2++;
	}
	return TRUE;
}

BOOL CDibMerger::IsSameRect(CDib *bmp1, CDib *bmp2)
{


	unsigned int x1,y1,x2,y2;
	unsigned int w1=bmp1->GetWidth()-bmp1->Pix_X;
	unsigned int w2=bmp2->GetWidth()-bmp2->Pix_X;
	unsigned int w=(w1>w2)?w2:w1;
	
	x1=bmp1->Pix_X;y1=bmp1->Pix_Y;
	x2=bmp2->Pix_X;y2=bmp2->Pix_Y;
	for(unsigned int i=0;i<w;i++)
	{     
		 //判断是否是相同的列，如果是则向左移动继续比较是否是相同的块
		if(!IsSameCol(bmp1,bmp2,x1,y1,x2,y2))
			return FALSE;
		x1++;
		x2++;
	}

	unsigned int h=(bmp1->GetHeight()-bmp1->Pix_Y)<(bmp2->GetHeight()-bmp2->Pix_Y)?
		(bmp1->GetHeight()-bmp1->Pix_Y):(bmp2->GetHeight()-bmp2->Pix_Y);
    //如果块的面积比先前的最大面积大，则取代之
	if(i*h>m_iMAXarea)
	{
		m_iMAXarea=i*h;

		left_starX=bmp1->Pix_X;
	    left_starY=bmp1->Pix_Y;
		left_endX=i+bmp1->Pix_X;
		left_endY=h+bmp1->Pix_Y;
		right_starX=bmp2->Pix_X;
		right_starY=bmp2->Pix_Y;
		right_endX=i+bmp2->Pix_X;
		right_endY=h+bmp2->Pix_Y;
	}
	return TRUE;
}

CDib * CDibMerger::MergeDIB(CDib *bmp1, CDib *bmp2)
{
    CDib *m_pDibResult;
	//
	BYTE* unitbuff;
	unitbuff=NULL;


	unsigned int w1=bmp1->GetHeight();
	unsigned int w2=bmp2->GetHeight();


	//新图象的宽
	unsigned int unit_w=bmp2->GetWidth()+left_starX;
	//重叠部分的上部高度
	unsigned int unit_up=(left_starY>right_starY)?left_starY:right_starY;
	//重叠部分的下部高度
	unsigned int unit_down=((w1-left_endY)>(w2-right_endY))?(w1-left_endY):(w2-right_endY);
	//新图象的高
	unsigned int unit_h=unit_up+(left_endY-left_starY) +unit_down;


	//得到每行实际占的字节数
	int byte_w=unit_w*(bmp1->GetBiBitCount()/8);
	if(byte_w%4)
		byte_w=byte_w+(4-byte_w%4);

	//得到新图象的数据大小
	int imagedata=byte_w*unit_h;


		// 将此图象用CDib类封装
	m_pDibResult = new CDib(CSize(unit_w,unit_h), 8);

	// 计算结果图象的存储大小尺寸
	CSize sizeSaveResult;
	sizeSaveResult = m_pDibResult->GetDibSaveDim();

	// 拷贝调色板
	memcpy(m_pDibResult->m_lpvColorTable, m_pDibInit->m_lpvColorTable, m_pDibResult->m_nColorTableEntries*sizeof(RGBQUAD));

	// 应用调色板
	m_pDibResult->MakePalette();
	
	
	// 分配内存给合并后的图象
	LPBYTE lpImgResult;
	lpImgResult = (LPBYTE)new unsigned char[sizeSaveResult.cx * sizeSaveResult.cy];

	BYTE *p1,*p2,*p3;
	p1=bmp1->m_pDibBits+left_starY*bmp1->GetDibSaveDim().cx;  //数据1起始地址
	p2=bmp2->m_pDibBits+right_starY*bmp2->GetDibSaveDim().cx; //数据2起始地址
	p3=lpImgResult;

	//保存图象的数据，先把第一幅图象的第一行存进去，再存第二幅图象的第一行，然后再存
	//第一幅图象的第二行，再存第二幅图象的第二行，以次类推
	for(unsigned int i=0;i<unit_h;i++)
	{
		
		memcpy(p3,p1,left_starX*(bmp1->GetBiBitCount()/8));
		p3+=left_starX*(bmp1->GetBiBitCount()/8);
		p1+=bmp1->GetDibSaveDim().cx;
		memcpy(p3,p2,bmp2->GetWidth()*(bmp2->GetBiBitCount()/8));
		p3+=bmp2->GetWidth()*(bmp2->GetBiBitCount()/8);
		p2+=bmp2->GetDibSaveDim().cx;
        //如果不是4的倍数则填满为4的倍数
		if((unsigned int)(p3-unitbuff)%4!=0)
			p3+=4-(unsigned int)((p3-unitbuff)%4);
	}
	// 将指针赋值给CDib类的数据
	m_pDibResult->m_pDibBits = lpImgResult;

    //创建显示拼接后图象的对话框
	CDlgMosaicRes* pDlg;
	pDlg = new CDlgMosaicRes(NULL, 	m_pDibResult);
	pDlg->DoModal();
	
	// 删除对象
	delete pDlg;


	return ;
	

}

*/
