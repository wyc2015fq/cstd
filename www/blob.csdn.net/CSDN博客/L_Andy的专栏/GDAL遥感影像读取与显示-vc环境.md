# GDAL遥感影像读取与显示-vc环境 - L_Andy的专栏 - CSDN博客

2014年03月24日 10:58:26[卡哥](https://me.csdn.net/L_Andy)阅读数：1589



GDAL是一个读写栅格和矢量空间数据的库，在GIS行业中有着巨大的应用，遥感影像是我们的空间数据的一种重要的类型，下面就给出遥感数据读取与显示的具体代码：

**[cpp]**[view
 plain](http://blog.csdn.net/zhouxuguang236/article/details/8070090#)[copy](http://blog.csdn.net/zhouxuguang236/article/details/8070090#)

- char* szFileName = "K:\\各省市地图\\新建文件夹 (2)\\fuzhoucity.tif";  
-     GDALDataset *poDataset;   //GDAL数据集
-     GDALAllRegister();  
- 
-     poDataset = (GDALDataset*)GDALOpen(szFileName,GA_ReadOnly);  
- if( poDataset == NULL )  
-     {  
-         AfxMessageBox(_T("文件打开失败!!!"));  
- return;  
-     }   
- 
-     GDALRasterBand *poBand1;   //遥感的一个波段
-     GDALRasterBand *poBand2;  
-     GDALRasterBand *poBand3;  
- int nBandCount = poDataset->GetRasterCount();  
-     poBand1 = poDataset->GetRasterBand(1);  
-     poBand2 = poDataset->GetRasterBand(2);  
-     poBand3 = poDataset->GetRasterBand(3);  
- 
- 
- 
- //获得图像显示窗口的尺寸
-     GetClientRect(&m_ViewRect);  
- 
- int nImgSizeX = poDataset->GetRasterXSize();  
- int nImgSizeY = poDataset->GetRasterYSize();  
- 
- //获得仿射变换的系数
- double adfGeoTransform[6];  
-     poDataset->GetGeoTransform( adfGeoTransform );  
- 
- double right  = adfGeoTransform[0] + nImgSizeX*adfGeoTransform[1];  
- double bottom  = adfGeoTransform[3] + nImgSizeY*adfGeoTransform[5];  
- 
- int nBufferSizeX,nBufferSizeY;  
- 
-     nBufferSizeX = nImgSizeX;  
-     nBufferSizeY = nImgSizeY;  
- 
- int nScrrenWidth = m_ViewRect.Width();  
- int nScrrenHeight= m_ViewRect.Height();  
- 
- //为数据缓冲区分配内存
- BYTE *pafScanblock1,*pafScanblock2,*pafScanblock3,*TempLock1,*TempLock2,*TempLock3;  
-     pafScanblock1 = (BYTE *) CPLMalloc((nScrrenWidth)*(nScrrenHeight));  
-     pafScanblock2 = (BYTE *) CPLMalloc((nScrrenWidth)*(nScrrenHeight));  
-     pafScanblock3 = (BYTE *) CPLMalloc((nScrrenWidth)*(nScrrenHeight));  
-     TempLock1 = pafScanblock1;  
-     TempLock2 = pafScanblock2;  
-     TempLock3 = pafScanblock3;  
- 
- //读取数据
-     poBand1->RasterIO( GF_Read, 0, 0,nBufferSizeX,nBufferSizeY,   
-         pafScanblock1,nScrrenWidth,nScrrenHeight, GDT_Byte,0, 0 );  
-     poBand2->RasterIO( GF_Read, 0, 0,nBufferSizeX,nBufferSizeY,   
-         pafScanblock2,nScrrenWidth,nScrrenHeight, GDT_Byte,0, 0 );  
-     poBand3->RasterIO( GF_Read, 0, 0,nBufferSizeX,nBufferSizeY,   
-         pafScanblock3,nScrrenWidth,nScrrenHeight, GDT_Byte,0, 0 );  
- 
- 
- 
- //在View逐点显示图像
- DWORD dwBytes = (nScrrenWidth * 24) / 8;  
- while(((DWORD) dwBytes) % 4)   
-     {  
-         dwBytes++;  
-     }  
- 
- BYTE *szBuffer = newBYTE[nScrrenHeight*dwBytes];  
-     memset(szBuffer,0,nScrrenHeight*dwBytes);  
- BYTE *pTemp = szBuffer;  
-     CClientDC dc(this);  
- int nIndex = 0;  
- for (int i=0;i<nScrrenHeight;i++)  
-     {  
- for (int j=0;j<nScrrenWidth;j++)  
-         {  
- 
- BYTE dn1 = *pafScanblock1;  
- BYTE dn2 = *pafScanblock2;  
- BYTE dn3 = *pafScanblock3;  
- 
- //先绘制到内存DC中，而后再绘制到设备DC中
-             dcMem.SetPixel(j,i,RGB(dn1,dn2,dn3)); //显示像素值
- 
-             pafScanblock1 ++;  
-             pafScanblock2 ++;  
-             pafScanblock3 ++;  
- 
-         }  
- 
-         szBuffer = pTemp+dwBytes*i;  
- 
-     }  
- 
-     pDC->BitBlt(0,0,m_ViewRect.Width(),m_ViewRect.Height(),&dcMem,0,0,  
-         SRCCOPY);  
- 
-     CPLFree(TempLock1);  
-     CPLFree(TempLock2);  
-     CPLFree(TempLock3);  

下面是遥感影像显示的效果：

![](https://img-my.csdn.net/uploads/201210/14/1350205471_8918.jpg)

这样显示效果就和ArcGIS打开的一样了。

