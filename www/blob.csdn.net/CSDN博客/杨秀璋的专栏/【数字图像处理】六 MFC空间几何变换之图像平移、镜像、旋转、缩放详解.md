
# 【数字图像处理】六.MFC空间几何变换之图像平移、镜像、旋转、缩放详解 - 杨秀璋的专栏 - CSDN博客

2015年06月04日 17:28:35[Eastmount](https://me.csdn.net/Eastmount)阅读数：18615标签：[MFC																](https://so.csdn.net/so/search/s.do?q=MFC&t=blog)[图像处理																](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)[几何变换																](https://so.csdn.net/so/search/s.do?q=几何变换&t=blog)[图像平移旋转																](https://so.csdn.net/so/search/s.do?q=图像平移旋转&t=blog)[图像镜像																](https://so.csdn.net/so/search/s.do?q=图像镜像&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=图像平移旋转&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=几何变换&t=blog)个人分类：[数字图像处理																](https://blog.csdn.net/Eastmount/article/category/1858075)[MFC图像知识																](https://blog.csdn.net/Eastmount/article/category/1858073)[
							](https://blog.csdn.net/Eastmount/article/category/1858075)
所属专栏：[MFC应用及图像处理](https://blog.csdn.net/column/details/eastmount-mfc.html)
[
																	](https://so.csdn.net/so/search/s.do?q=几何变换&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=MFC&t=blog)

本文主要讲述基于VC++6.0 MFC图像处理的应用知识，主要结合自己大三所学课程《数字图像处理》及课件进行讲解，主要通过MFC单文档视图实现显示BMP图片空间几何变换，包括图像平移、图形旋转、图像反转倒置镜像和图像缩放的知识。同时文章比较详细基础，没有采用GDI+获取矩阵，而是通过读取BMP图片信息头和矩阵像素实现变换，希望该篇文章对你有所帮助，尤其是初学者和学习图像处理的学生。
[【数字图像处理】一.MFC详解显示BMP格式图片](http://blog.csdn.net/eastmount/article/details/18238863)
[【数字图像处理】二.MFC单文档分割窗口显示图片](http://blog.csdn.net/eastmount/article/details/18987539)
[【数字图像处理】三.MFC实现图像灰度、采样和量化功能详解](http://blog.csdn.net/eastmount/article/details/46010637)
[【数字图像处理】四.MFC对话框绘制灰度直方图](http://blog.csdn.net/eastmount/article/details/46237463)
[【数字图像处理】五.MFC图像点运算之灰度线性变化、灰度非线性变化、阈值化和均衡化处理详解](http://blog.csdn.net/eastmount/article/details/46312145)
免费资源下载地址：
[http://download.csdn.net/detail/eastmount/8772951](http://download.csdn.net/detail/eastmount/8772951)

## 一. 图像平移
前一篇文章讲述了图像点运算(基于像素的图像变换)，这篇文章讲述的是图像几何变换：在不改变图像内容的情况下对图像像素进行空间几何变换的处理方式。
点运算对单幅图像做处理，不改变像素的空间位置；代数运算对多幅图像做处理，也不改变像素的空间位置；几何运算对单幅图像做处理，改变像素的空间位置，几何运算包括两个独立的算法：空间变换算法和灰度级插值算法。
空间变换操作包括简单空间变换、多项式卷绕和几何校正、控制栅格插值和图像卷绕，这里主要讲述简单的空间变换，如图像平移、镜像、缩放和旋转。主要是通过线性代数中的齐次坐标变换。
图像平移坐标变换如下：
![](https://img-blog.csdn.net/20150603155520636)
运行效果如下图所示，其中BMP图片(0,0)像素点为左下角。
![](https://img-blog.csdn.net/20150603172206441)
其代码核心算法：
1.在对话框中输入平移坐标(x,y) m_xPY=x，m_yPY=y
2.定义Place=dlg.m_yPY*m_nWidth*3 表示当前m_yPY行需要填充为黑色
3.新建一个像素矩阵 ImageSize=new unsigned char[m_nImage]
4.循环整个像素矩阵处理
for(int i=0 ; i<m_nImage ; i++ ){
if(i<Place) {ImageSize[i]=black; continue;}//黑色填充底部 从小往上绘图
else if(i>=Place && countWidth<dlg.m_xPY*3) {//黑色填充左部分
ImageSize[i]=black; countWidth++;  continue;
}
else if(i>=Place && countWidth>=dlg.m_xPY*3) {//图像像素平移区域
ImageSize[i]=m_pImage[m_pImagePlace];//原(0,0)像素赋值过去
m_pImagePlace++; countWidth++;
if(countWidth==m_nWidth*3) {//一行填满m_pImagePlace走到(0,1)
number++; m_pImagePlace=number*m_nWidth*3;
}
}
}
5.写文件绘图fwrite(ImageSize,m_nImage,1,fpw)
第一步：在ResourceView资源视图中，添加Menu子菜单如下：(注意ID号)
![](https://img-blog.csdn.net/20150603172625794)
第二步：设置平移对话框。将试图切换到ResourceView界面--选中Dialog,右键鼠标新建一个Dialog，并新建一个名为IDD_DIALOG_PY。编辑框(X)IDC_EDIT_PYX
 和 (Y)IDC_EDIT_PYY，确定为默认按钮。设置成下图对话框：
![](https://img-blog.csdn.net/20150603172702220)
第三步：在对话框资源模板空白区域双击鼠标—Create a new class创建一个新类--命名为CImagePYDlg。会自动生成它的.h和.cpp文件。打开类向导(Ctrl
 W)，选择类名:CImagePYDlg添加成员变量如下图所示，同时在Message Maps中生成ID_JHBH_PY实现函数。
![](https://img-blog.csdn.net/20150603172750190)![](https://img-blog.csdn.net/20150603172817740)
第四步：在CImageProcessingView.cpp中添加头文件\#include "ImagePYDlg.h"，并实现平移。
```python
/********************************************************/
/* 图像空间几何变换：图像平移 ID_JHBH_PY(几何变换-平移) 
/* 使用平移对话框：CImagePYDlg dlg                     
/* 算法：f(x,y)=f(x+x0,y+y0)图像所有点平移，空的补黑'0' 
/* 注意该图像平移方法只是从左上角(0,0)处开始平移        
/* 其他方向原理相同 自己去实现                           
/********************************************************/
void CImageProcessingView::OnJhbhPy() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能空间平移!",MB_OK,0);
		return;
	}
	//定义采样对话框也是用来空间变换平移的坐标
	CImagePYDlg dlg;     
	if( dlg.DoModal()==IDOK ) //显示对话框
	{
		//采样坐标最初为图片的自身像素
		if( dlg.m_xPY>m_nWidth || dlg.m_yPY>m_nHeight ) {
			AfxMessageBox("图片平移不能为超过原图长宽!",MB_OK,0);
			return;
		}
		AfxMessageBox("图片空间变换-平移!",MB_OK,0);
		//打开临时的图片 读写文件
		FILE *fpo = fopen(BmpName,"rb");
		FILE *fpw = fopen(BmpNameLin,"wb+");
		fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
		fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
		fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
		fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
		fread(m_pImage,m_nImage,1,fpo);
		/************************************************************/
		/* 图片空间变换-平移                                        
		/* 坐标(dlg.m_xPY,dlg.m_yPY)表示图像平移的坐标        
		/* 先用Plave计算出平移后的起始坐标，其他的坐标赋值为'0'黑色 
		/* 然后依次平移坐标，空的赋为黑色，否则填充                 
		/************************************************************/
		
		/******************************************************************/
		/* 严重错误1：数组变量赋值相等                                    
		/* 在View.h中定义变量 BYTE *m_pImage 读入图片数据后的指针         
		/* 建立临时变量数组,让它平移变换 unsigned char *ImageSize         
		/* ImageSize=m_pImage(错误)                                       
		/* 会导致ImageSize赋值变换时m_pImage也产生了变换,所以输出全为黑色 
		/*     因为它俩指向了相同的数组地址                               
		/* 解决方法：使用下面C++的new方法动态分配或for循环i=m_nImage赋值  
		/******************************************************************/
		/*临时变量存储的像素与m_pImage相同，便于处理图像*/
		unsigned char *ImageSize;      
	    ImageSize=new unsigned char[m_nImage];  //new和delete有效的进行动态内存的分配和释放
		int Place;                    //建立临时坐标 记录起始坐标(0,0)平移过来的位置
		int m_pImagePlace;            //原始图像平移为(0,0) 图像把它平移到Place位置
		unsigned char black;          //填充黑色='0' 
		/************************************************************/
		/* for(int i=0 ; i<m_nHeight ; i++ )                        
		/* for(int j=0 ; j<m_nWidth ; j++ )                         
		/* 不能使用的上面的因为可能图像的最后一行没有完整的一行像素 
		/* 这样会出现exe报错,使用m_nImage读写所有像素比较正确       
		/************************************************************/
		Place=dlg.m_yPY*m_nWidth*3;   //前m_yPY行都要填充为黑色          
		black=0;                       //颜色为黑色
		m_pImagePlace=0;               //图像处事位置为(0,0),把该点像素平移过去 
		int countWidth=0;              //记录每行的像素个数,满行时变回0
		int number=0;                  //数字记录使用的像素行数,平移时使用
		for(int i=0 ; i<m_nImage ; i++ )
		{
			/*如果每行的像素填满时清为0*/
			if(countWidth==m_nWidth*3) {
				countWidth=0;
			}
			
			/*第一部分：到平移后像素位置前面的所有像素点赋值为黑色*/
			if(i<Place) {
				ImageSize[i]=black;     //赋值为黑色
				continue;
			}
			
			/*第二部分：平移区域的左边部分赋值为黑色*/
			else if(i>=Place && countWidth<dlg.m_xPY*3) { //RGB乘3
				ImageSize[i]=black;     //赋值为黑色
				countWidth++;
				continue;
			}
	
			/****************************/
			/* 各部分如图所示:          
			/* 000000000000000000000000 
			/* 000000000000000000000000 
			/* 0000000................. 
			/* 0000000.................
			/* 0000000................. 
			/* 0000000.................
			/* 点表示像素部分,0为黑色   
			/****************************/
			/* 重点错误提示：由于bmp图像显示是从左下角开始存储(0,0)点所以输出图像为 */
			/* bmp图像是从左下角到右上角排列的 */
			/****************************/
			/* 各部分如图所示:          
			/* 0000000................. 
			/* 0000000................. 
			/* 0000000.................
			/* 0000000................. 
			/* 000000000000000000000000 
			/* 000000000000000000000000 
			/* 点表示像素部分,0为黑色   
			/****************************/
		
			/*第三部分：图像像素平移区域*/
			else if(i>=Place && countWidth>=dlg.m_xPY*3)
			{
				ImageSize[i]=m_pImage[m_pImagePlace];     
				m_pImagePlace++;
				countWidth++;
				if(countWidth==m_nWidth*3)
				{
					number++;
					m_pImagePlace=number*m_nWidth*3;
				}
			}
		}
		
		fwrite(ImageSize,m_nImage,1,fpw);  
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level=200;        //200表示几何变换
		Invalidate();
	}	
}
```
同时在ShowBitmap中添加level标记重新绘制图片，代码如下：
```python
else        //图像几何变换
if(level=200)
{
	m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
		LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
}
```
运行时需要注意一点：BMP图像在处理过程中可能会出现一些斜线，而平移(40,60)位移量时可能出现如下。他是因为BMP格式有个非常重要的规定，要求每一扫描的字节数据必须能被4整除，也就是Dword对齐(长度4字节)，如果图像的一行字节数不能被4整除，就需要在每行末尾不起0达到标准。
例如一行像素为97字节，我们就需要补3个字节吗，数值可以是0，但是我们在BMP格式的信息头里说明了其宽度，所以补齐后对我们没有影响，所以后面补若干个字节的0即可直到被4整除。
![](https://img-blog.csdn.net/20150603181703369)![](https://img-blog.csdn.net/20150603181551824)
通过后面的图像缩放后，我从学做了一遍这个补齐的缩放。代码如下，能够实现完美平移。nice啊~
```python
void CImageProcessingView::OnJhbhPy() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能空间平移!",MB_OK,0);
		return;
	}
	//定义采样对话框也是用来空间变换平移的坐标
	CImagePYDlg dlg;     
	if( dlg.DoModal()==IDOK ) //显示对话框
	{
		//采样坐标最初为图片的自身像素
		if( dlg.m_xPY>m_nWidth || dlg.m_yPY>m_nHeight ) {
			AfxMessageBox("图片平移不能为超过原图长宽!",MB_OK,0);
			return;
		}
		AfxMessageBox("图片空间变换-平移!",MB_OK,0);
		//打开临时的图片 读写文件
		FILE *fpo = fopen(BmpName,"rb");
		FILE *fpw = fopen(BmpNameLin,"wb+");
		fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
		fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
		int num;            //记录每行多余的图像素数个数
		int sfSize;         //补齐后的图像大小
		//重点:图像的每行像素都必须是4的倍数：1*1的图像为 r g b 00H 
		if(m_nWidth*3%4!=0)
		{
			num=(4-m_nWidth*3%4);
			sfSize=(m_nWidth*3+num)*m_nHeight; //每行多number个
		}
		else
		{
			num=0;
			sfSize=m_nWidth*m_nHeight*3;
		}
		//注意:假如最后一行像素不足,我默认处理为完整的一行,不足补00H
		//总之处理后的图像总是m*n且为4倍数,每行都完整存在
		/*更改文件头信息 定义临时文件头结构变量*/
		BITMAPFILEHEADER bfhsf;
		BITMAPINFOHEADER bihsf;       
		bfhsf=bfh;
		bihsf=bih;
		bfhsf.bfSize=sfSize+54;		
		fwrite(&bfhsf,sizeof(BITMAPFILEHEADER),1,fpw);
		fwrite(&bihsf,sizeof(BITMAPINFOHEADER),1,fpw);
		fread(m_pImage,m_nImage,1,fpo);
		CString str;
		str.Format("补齐=%d",num);
		AfxMessageBox(str);
		/*临时变量存储的像素与sfSize相同 new和delete有效的进行动态内存的分配和释放*/
		unsigned char *ImageSize;      
	    ImageSize=new unsigned char[sfSize];  
		int Place;                    //建立临时坐标 记录起始坐标(0,0)平移过来的位置  
        int m_pImagePlace;            //原始图像平移为(0,0) 图像把它平移到Place位置  
        unsigned char black=0;        //填充黑色='0'  
		unsigned char other=0;        //补码00H='\0'
		Place=dlg.m_yPY*(m_nWidth*3+num); //前m_yPY行都要填充为黑色          
		m_pImagePlace=0;                  //图像处事位置为(0,0),把该点像素平移过去 
		int countWidth=0;                 //记录每行的像素个数,满行时变回0
		int number=0;                     //数字记录使用的像素行数,平移时使用
		for(int i=0 ; i<sfSize ; i++ )  
        {  
            /*第一部分：到平移后像素位置前面的所有像素点赋值为黑色*/  
            if(i<Place) 
			{  
                ImageSize[i]=black;     //赋值为黑色  
                continue;  
            }  
              
            /*第二部分：平移区域的左边部分赋值为黑色*/  
            else if(i>=Place && countWidth<dlg.m_xPY*3)  //RGB乘3
			{   
                ImageSize[i]=black;     //赋值为黑色  
                countWidth++;  
                continue;  
            }  
          
            /*第三部分：图像像素平移区域*/  
            else if(i>=Place && countWidth>=dlg.m_xPY*3)  
            {  
                ImageSize[i]=m_pImage[m_pImagePlace];       
                m_pImagePlace++;  
                countWidth++;  
                if(countWidth==m_nWidth*3)  
                {  
					if(num==0)
					{
						countWidth=0;
						number++;  
						m_pImagePlace=number*m_nWidth*3; 
					}
					else //num为补0
					{
						for(int j=0;j<num;j++)
						{
							i++;
							ImageSize[i]=other;
						}
						countWidth=0;
						number++;  
						m_pImagePlace=number*(m_nWidth*3+num); //重点：添加Num
					}
                }  
            }  
        }  
          
        fwrite(ImageSize,sfSize,1,fpw);    
        fclose(fpo);  
        fclose(fpw);  
        numPicture = 2;  
        level=200;        //200表示几何变换  
        Invalidate();  
    }     
}
```
运行效果如下图所示，完美平移，其他算法遇到斜线问题类似补齐即可。
![](https://img-blog.csdn.net/20150604184518265)







## 二. 图像镜像
**1.水平镜像翻转**
其变换矩阵如下：
X=width-X0-1   (width为图像宽度)
Y=Y0
打开类向导，在CImageProcessingView中添加IDs为ID_JHBH_FZ，生成函数，代码如下：

```python
/* 几何变换 图像翻转：自己对这个功能比较感兴趣,做个图像反转 */
void CImageProcessingView::OnJhbhFz() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能空间反转!",MB_OK,0);
		return;
	}
	AfxMessageBox("图片空间变换-反转图像!",MB_OK,0);
	//打开临时的图片
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	fread(m_pImage,m_nImage,1,fpo);
	
	/*new和delete有效的进行动态内存的分配和释放*/
	unsigned char *ImageSize;      
	ImageSize=new unsigned char[m_nImage];    
	int countWidth=0;        //记录每行的像素个数,满行时变回0
	int Place;               //记录图像每行的位置,便于图像反转
	int number=0;            //数字记录使用的像素行数
	Place=m_nWidth*3-1;
	//翻转矩阵: y=y0 x=width-x0-1
	for(int i=0 ; i<m_nImage ; i++ )
	{
		if(countWidth==m_nWidth*3)
		{
			countWidth=0;
		}
		ImageSize[i]=m_pImage[Place]; //(0,0)赋值(0,width*3-1)像素
		Place--;
		countWidth++;
		if(countWidth==m_nWidth*3)
		{
			number++;
			Place=number*m_nWidth*3-1;
		}
	}
		
	fwrite(ImageSize,m_nImage,1,fpw);  
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=200;                      
	Invalidate();
}
```
运行效果如下图所示，其中还是存在一些小BUG，如前面的BMP图补0凑齐4整数倍宽度或颜色失帧。
![](https://img-blog.csdn.net/20150604140246807)
![](https://img-blog.csdn.net/20150604140436436)

**2.垂直镜像倒转**
其中变换矩阵如下：
X=X0
Y=height-Y0-1   (height为图像高度)
它相当于把原图的像素矩阵的最后一行像素值赋值给第一行，首先找到(0,0)对应的(height-1,0)像素值，然后依次赋值该行的像素数据；最后当前行赋值结束，依次下一行。重点是找到每行的第一个像素点即可。
代码中引用两个变量：Place=(m_nWidth*3)*(m_nHeight-1-1)即是(height-1,0)最后一行的第一个像素点；然后是循环中Place=(m_nWidth*3)*(m_nHeight-number-1)找到每行的第一个像素点。
![](https://img-blog.csdn.net/20150604142928563)
同样通过类向导生成函数void CImageProcessingView::OnJhbhDz()，代码如下：
```python
/* 几何变换 图像倒转 */
void CImageProcessingView::OnJhbhDz() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能空间反转!",MB_OK,0);
		return;
	}
	AfxMessageBox("图片空间变换-反转图像!",MB_OK,0);
	//打开临时的图片
	FILE *fpo = fopen(BmpName,"rb");
	FILE *fpw = fopen(BmpNameLin,"wb+");
	fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
	fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
	fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
	fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
	fread(m_pImage,m_nImage,1,fpo);
	
	/*new和delete有效的进行动态内存的分配和释放*/
	unsigned char *ImageSize;      
	ImageSize=new unsigned char[m_nImage];    
	int countWidth=0;                   //记录每行像素个数,满行时变回0
	int Place;                          //每列位置
	int number=0;                       //像素行数
	Place=(m_nWidth*3)*(m_nHeight-1-1); //0行存储
	//翻转矩阵: x=x0 y=height-y0-1 
	for(int i=0 ; i<m_nImage ; i++ )
	{
		ImageSize[i]=m_pImage[Place]; //(0,0)赋值(0,0)像素
		Place++;
		countWidth++;
		if(countWidth==m_nWidth*3)
		{
			countWidth=0;
			number++;
			Place=(m_nWidth*3)*(m_nHeight-number-1);
		}
	}
		
	fwrite(ImageSize,m_nImage,1,fpw);  
	fclose(fpo);
	fclose(fpw);
	numPicture = 2;
	level=200;                      
	Invalidate();
}
```
运行结果如下图所示，第二张图颜色没有失帧或变灰，这完全可以怀疑在翻转过程中RGB像素编程BGR后导致的结果，最终实现了翻转图像，但灰度存在一定；所以如果改为RBG顺序不变化即可原图颜色显示。
![](https://img-blog.csdn.net/20150604143322213)
![](https://img-blog.csdn.net/20150604143342103)



## 三. 图像旋转
图像饶原点旋转顺时针theta角矩阵变换如下：注意BMP图像(0,0)左下角
![](https://img-blog.csdn.net/20150604150806524)
写到这里真心觉得写底层的代码非常困难啊！尤其是以为像素转换二维像素，同时也觉得当时的自己算法部分还是很强大的，也感觉到如果采用GDI+操作像素矩阵Matrix或ColorMatrix是多么的方便，因为它定义好了X和Y向量，这就是为什么Android前面写的图像处理要容易得多。但是效率高~
好像利用GDI+旋转通过几句代码即可：
matrix.Rotate(15); //矩阵旋转15度
graph.SetTransform(&matrix);
graph.DrawImage(&image,points,3);
下面这部分代码是实现Android旋转的：参考[我的博客](http://blog.csdn.net/eastmount/article/details/40457273)
```python
//旋转图片  
private void TurnPicture() {  
    Matrix matrix = new Matrix();  
    turnRotate=turnRotate+15;  
    //选择角度 饶(0,0)点选择 正数顺时针 负数逆时针 中心旋转  
    matrix.setRotate(turnRotate,bmp.getWidth()/2,bmp.getHeight()/2);   
    Bitmap createBmp = Bitmap.createBitmap(bmp.getWidth(), bmp.getHeight(), bmp.getConfig());  
    Canvas canvas = new Canvas(createBmp);   
    Paint paint = new Paint();   
    canvas.drawBitmap(bmp, matrix, paint);  
    imageCreate.setBackgroundColor(Color.RED);  
    imageCreate.setImageBitmap(createBmp);  
    textview2.setVisibility(View.VISIBLE);  
}
```
实现效果如下图所示：
![](https://img-blog.csdn.net/20150604151526188)
言归正传，新建Dialog如下图所示，设置ID_DIALOG_XZ和变量：
![](https://img-blog.csdn.net/20150604152311045)
再点击空白处创建CImageXZDlg类(旋转)，它会自动生成.h和.cpp文件。打开类向导生成CImageXZDlg类的成员变量m_xzds(旋转度数)，并设置其为int型(最大值360 最小值0)。
在类向导(Ctrl+W)选择类CImageProcessingView，为ID_JHBH_TXXZ(图像旋转)添加函数，同时添加头文件\#include "ImageXZDlg.h"
```python
/**********************************************************/
/* 几何变换：图片旋转                                 
/* 先添加对话框:IDD_JHBH_TXXZ(图像旋转),创建新类CImageXZDlg  
/* 创建输入度数的:m_xzds Member variables 为int 0-360间 
/**********************************************************/
void CImageProcessingView::OnJhbhTxxz() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能空间旋转!",MB_OK,0);
		return;
	}
	
	//定义对话框并调用对话框
	CImageXZDlg dlg;    
	if( dlg.DoModal()==IDOK ) //显示对话框
	{
		AfxMessageBox("图片空间变换-旋转图像!",MB_OK,0);
		//读写文件
		FILE *fpo = fopen(BmpName,"rb");
		FILE *fpw = fopen(BmpNameLin,"wb+");
		fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
		fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
		fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,fpw);
		fwrite(&bih,sizeof(BITMAPINFOHEADER),1,fpw);
		fread(m_pImage,m_nImage,1,fpo);
		
		/*new和delete有效的进行动态内存的分配和释放*/
		unsigned char *ImageSize;      
		ImageSize=new unsigned char[m_nImage];    
		int Place;          //记录图像每行的位置,便于图像旋转
		/*定义PA=3.14时使用的方法是arcsin(1.0/2)*6即为π*/
		double PA;
		PA=asin(0.5)*6;
		
		/*把输入的0-360的正整数度数转换为角度,30度=π/6*/
		double degree; 
		degree=PA*dlg.m_xzds/180;   //调用dlg.m_xzds(旋转度数)
		
		//对应的二维矩阵 注意图像矩阵从左下角开始处理 它最终要转换成一维存储
		int X,Y;               //图像变换前通过一维矩阵转换为二维
		int XPlace,YPlace;
		//输出转换为的角度
		CString str;
		str.Format("转换后的角度=%f",degree);
		AfxMessageBox(str);
		//图像旋转处理
		for(int i=0 ; i<m_nImage ; i++ )
		{
			//原图：一维矩阵转换为二维矩阵
			X=(i/3)%m_nWidth;
			Y=(i/3)/m_nWidth;
			//注意错误：X=i/m_nHeight Y=i%m_nWidth; 只输出最后1/3
			//图像旋转为：a(x,y)=x*cos-y*sin b(x,y)=x*sin+y*cos
			XPlace=(int)(X*cos(degree)-Y*sin(degree));
			YPlace=(int)(X*sin(degree)+Y*cos(degree));
			
			//在转换为一维图想输出
			if( (XPlace>=0 && XPlace<=m_nWidth) && (YPlace>=0 && YPlace<=m_nHeight) )
			{
				Place=YPlace*m_nWidth*3+XPlace*3;
				//在图像范围内赋值为该像素
				if(Place+2<m_nImage) 
				{
					ImageSize[i]=m_pImage[Place];
					i++;
					ImageSize[i]=m_pImage[Place+1];
					i++;
					ImageSize[i]=m_pImage[Place+2];
				}
				//否则赋值为黑色
				else 
				{
					ImageSize[i]=0; 
					i++;
					ImageSize[i]=0;
					i++;
					ImageSize[i]=0;
				}
			}
			//否则赋值为黑色
			else
			{
				ImageSize[i]=0;
				i++;
				ImageSize[i]=0;
				i++;
				ImageSize[i]=0;
			}
		}
	
		fwrite(ImageSize,m_nImage,1,fpw);  
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level=200;        //几何变换              
		Invalidate();
	}
}
```
运行效果如下图所示，中心旋转太难了！找到中心那个位置就不太容易，我做不下去了，fuck~同时旋转过程中，由于是饶左下角(0,0)实现，故有的角度会到界面外显示全黑。下图分别旋转15度和355度。
![](https://img-blog.csdn.net/20150604153403181)
![](https://img-blog.csdn.net/20150604153608933)






## 四. 图像缩放
图像缩放主要有两种方法：
1.最近邻插值：向后映射时，输出图像的灰度等于离它所映射位置最近的输入图像的灰度值。其中向前映射和向后映射如下：
![](https://img-blog.csdn.net/20150604154936195)![](https://img-blog.csdn.net/20150604154937186)
对于向前映射每个输出图像的灰度要经过多次运算，对于向后映射，每个输出图像的灰度只经过一次运算。在实际应用中，更多的是采用向后映射法，其中根据四个相邻像素灰度值计算某个位置的像素灰度值即为灰度级插值。
2.双线性插值：四点确定一个平面函数，属于过约束问题。即单位正方形顶点已知，求正方形内任一点的f(x,y)值。
![](https://img-blog.csdn.net/20150604155900113)
换个通熟的说法，如下图所示。采用最近邻插值法就是P(x,y)像素值采用四舍五入等于离它最近的输入图像像素值。分别计算它到四个顶点之间的距离，但是这样会造成图像的马赛克、锯齿等现象。而采用双线性插值法，主要通过该坐标周围的四个像素值，按照比例混合计算器近似值。比例混合的依据是离哪个像素近，哪个像素的比例越大。
![](https://img-blog.csdn.net/20150604160440372)
下面是采用最近邻插值法的过程，注意BMP图缩放还需修改头文件信息。
第一步：在资源视图中添加“图像缩放”Dialog
![](https://img-blog.csdn.net/20150604161722333)
第二步：点击空白处创建对话框的类CImageSFDlg，同时打开类向导为其添加成员变量m_sfbs(缩放倍数)，其为int型在0-200之间。
![](https://img-blog.csdn.net/20150604162018979)
![](https://img-blog.csdn.net/20150604161916494)
第三步：打开类向导为其添加成员函数void CImageProcessingView::OnJhbhSf() 并实现缩放。同时添加头文件\#include "ImageSFDlg.h"。
```python
/*******************************************************************/
/* ID_JHBH_SF: 几何运算-缩放-最近邻插值算法               
/* 算法思想:输出图像的灰度等于离它所映射位置最近的输入图像的灰度值 
/* 先计算出放大缩小后的长宽，根据它计算找原图中的点灰度，四舍五入  
/*******************************************************************/
void CImageProcessingView::OnJhbhSf() 
{
	if(numPicture==0) {
		AfxMessageBox("载入图片后才能几何缩放图像!",MB_OK,0);
		return;
	}
	CImageSFDlg dlg;           //定义缩放对话框
	if( dlg.DoModal()==IDOK )
	{
		//采样坐标最初为图片的自身像素  m_sfbs(缩放倍数)
		if( dlg.m_sfbs==0 ) {
			AfxMessageBox("输入图片缩放倍数不能为0!",MB_OK,0);
			return;
		}
		
		FILE *fpo = fopen(BmpName,"rb");
		FILE *fpw = fopen(BmpNameLin,"wb+");
		fread(&bfh,sizeof(BITMAPFILEHEADER),1,fpo);
		fread(&bih,sizeof(BITMAPINFOHEADER),1,fpo);
		
		/*先求缩放后的长宽*/
		int sfWidth,sfHeight;                            //缩放后的长宽
		int sfSize;                                      //缩放后的图像大小
		sfWidth=(int)(m_nWidth*(dlg.m_sfbs*1.0)/100);    //24位图像RGB必须是3倍数 循环读取时为RGB
		sfHeight=(int)(m_nHeight*(dlg.m_sfbs*1.0)/100);
		int number;                                      //记录每行多余的图像素数个数
		//重点:图像的每行像素都必须是4的倍数：1*1的图像为 r g b 00H 
		if(sfWidth*3%4!=0) {
			number=(4-sfWidth*3%4);
			sfSize=(sfWidth*3+(4-sfWidth*3%4))*sfHeight;
		}
		else {
			number=0;
			sfSize=sfWidth*sfHeight*3;
		}
		//注意:假如最后一行像素不足,我默认处理为完整的一行,不足补00H
		//总之处理后的图像总是m*n且为4倍数,每行都完整存在
	
		/*更改文件头信息 定义临时文件头结构变量*/
		BITMAPFILEHEADER bfhsf;
		BITMAPINFOHEADER bihsf;                //缩放(sf)
		bfhsf=bfh;
		bihsf=bih;
		bfhsf.bfSize=sfSize+54;		
		bihsf.biWidth=sfWidth;
		bihsf.biHeight=sfHeight;
		//显示部分m_nDrawWidth<650显示原图,否则显示
		flagSF=1;                         //图像缩放为1标识变量
		m_nDrawWidthSF=sfWidth;
		m_nDrawHeightSF=sfHeight;
		fwrite(&bfhsf,sizeof(BITMAPFILEHEADER),1,fpw);
		fwrite(&bihsf,sizeof(BITMAPINFOHEADER),1,fpw);
		fread(m_pImage,m_nImage,1,fpo);	
		
		unsigned char red,green,blue;
		unsigned char other=0;                       //补码00H='\0'
		int placeX;                                  //记录在原图中的第几行的位置
		int placeY;									 //记录在原图中的位置(x,y)
		int placeBH;                                 //记录变换后在变换图中的位置
		/*new和delete有效的进行动态内存的分配和释放*/
		unsigned char *ImageSize;      
		ImageSize=new unsigned char[sfSize]; 
		/*读取文件像素信息 缩放注意:1.找最近灰度 2.四舍五入法(算法+0.5)*/
		for(int i=0; i<sfHeight ; i++ )                  //行
		{
			placeX=(int)(i/(dlg.m_sfbs*1.0/100)+0.5)*bih.biWidth*3;
			for(int j=0; j<sfWidth ; j++ )               //列
			{
				red=green=blue=0;
				//放大倍数为(dlg.m_sfbs*1.0/100)
				placeY=placeX+(int)(j/(dlg.m_sfbs*1.0/100)+0.5)*3;    
				//重点是:number*i补充00H,如果是numer图像会被切成2块
				placeBH=(i*sfWidth*3+number*i)+j*3;                  
				if(placeY+2<m_nImage)
				{
					ImageSize[placeBH]=m_pImage[placeY];
					ImageSize[placeBH+1]=m_pImage[placeY+1];
					ImageSize[placeBH+2]=m_pImage[placeY+2];
				}
				else
				{
					ImageSize[placeBH]=0;
					ImageSize[placeBH+1]=0;
					ImageSize[placeBH+2]=0;
				}
			}
		}
		
		fwrite(ImageSize,sfSize,1,fpw);
		fclose(fpo);
		fclose(fpw);
		numPicture = 2;
		level=200;
		Invalidate();
	}
}
```
第四步：因为图像缩放修改BMP图片头信息，所以需要修改ShowBitmap中的显示第二张图片时的部分代码。如下所示：添加变量flagSF、m_nDrawWidthSF和m_nDrawHeightSF。
```python
/*定义显示图像缩放时的长宽与标记*/
int flagSF=0;          //图像几何变换缩放变换
int m_nDrawWidthSF=0;  //图像显示宽度缩放后
int	m_nDrawHeightSF=0; //图像显示高度缩放后
//****************显示BMP格式图片****************//
void CImageProcessingView::ShowBitmap(CDC *pDC, CString BmpName)
{
        ......
		else        //图像几何变换
		if(level=200)
		{
			m_hBitmapChange = (HBITMAP) LoadImage(NULL,BmpNameLin,IMAGE_BITMAP,0,0,
				LR_LOADFROMFILE|LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
		}

		if( m_bitmap.m_hObject ) {
			m_bitmap.Detach();            //m_bitmap为创建的位图对象
		}
		m_bitmap.Attach(m_hBitmapChange);
		//定义并创建一个内存设备环境
		CDC dcBmp;
		if( !dcBmp.CreateCompatibleDC(pDC) )   //创建兼容性的DC
			return;
		BITMAP m_bmp;                          //临时bmp图片变量
		m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中
		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境
		//图片显示调用函数StretchBlt 
		if(flagSF==1)
		{
			CString str;
			str.Format("缩放长=%d 宽%d 原图长=%d 宽=%d",m_nDrawWidthSF,
                        m_nDrawHeightSF,m_nWidth,m_nHeight);
			AfxMessageBox(str);
			flagSF=0;
			//m_nDrawWidthSF缩放此存见函数最近邻插值法中赋值
			if(m_nDrawWidthSF<650 && m_nDrawHeightSF<650)   
				pDC->StretchBlt(m_nWindowWidth-m_nDrawWidthSF,0,
					m_nDrawWidthSF,m_nDrawHeightSF,&dcBmp,0,0,m_bmp.bmWidth,m_bmp.bmHeight,SRCCOPY);
			else
				pDC->StretchBlt(m_nWindowWidth-640,0,640,640,&dcBmp,0,0,
					m_bmp.bmWidth,m_bmp.bmHeight,SRCCOPY);  //显示大小为640*640
		}
		else {
			//如果图片太大显示大小为固定640*640 否则显示原图大小
			if(m_nDrawWidth<650 && m_nDrawHeight<650)
				pDC->StretchBlt(m_nWindowWidth-m_nDrawWidth,0,
					m_nDrawWidth,m_nDrawHeight,&dcBmp,0,0,m_bmp.bmWidth,m_bmp.bmHeight,SRCCOPY);
			else
				pDC->StretchBlt(m_nWindowWidth-640,0,640,640,&dcBmp,0,0,
					m_bmp.bmWidth,m_bmp.bmHeight,SRCCOPY); 
		}
		//恢复临时DC的位图
		dcBmp.SelectObject(pbmpOld);    
}
```
运行效果如下图所示，采用最近邻插值法缩放大了会出现失帧。

![](https://img-blog.csdn.net/20150604172340815)

![](https://img-blog.csdn.net/20150604171939530)
但是同时当图片缩小是总是报错，图片缩放确实有点难，因为像素需要补齐4整数倍，同时需要修改消息头，同时像素矩阵的变换都非常复杂。
![](https://img-blog.csdn.net/20150604172402711)
最后还是希望文章对你有所帮助，如果文章有不足或错误之处，请海涵。自己给自己点个赞，挺不容易的，但还会继续写完~
（By:Eastmount 2015-06-04 下午5点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）






