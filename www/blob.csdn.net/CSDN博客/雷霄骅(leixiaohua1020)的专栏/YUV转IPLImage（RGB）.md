# YUV转IPLImage（RGB） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 17:07:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：42









一个小的程序，在网上找了很久没有发现

自己搞了一个大家看看

第一个是很笨的办法：

yuv三个分量分别写在3个矩阵下，然后合并之后转换为rgb分量的图片格式就可以了；

代码如下：





**[html]**[view plain](http://blog.csdn.net/dreamd1987/article/details/7259479)[copy](http://blog.csdn.net/dreamd1987/article/details/7259479)



- IplImage*image,*rgbimg,*yimg,*uimg,*vimg,*uuimg,*vvimg;
- 
- rgbimg=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
- image=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
- 
- yimg=cvCreateImageHeader(cvSize(nWidth,nHeight),IPL_DEPTH_8U,1);
- uimg=cvCreateImageHeader(cvSize(nWidth/2,nHeight/2),IPL_DEPTH_8U,1);
- vimg=cvCreateImageHeader(cvSize(nWidth/2,nHeight/2),IPL_DEPTH_8U,1);
- 
- uuimg=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,1);
- vvimg=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,1);
- 
- cvSetData(yimg,pBuf,nWidth);
- cvSetData(uimg,pBuf+nWidth*nHeight,nWidth/2);
- cvSetData(vimg,pBuf+long(nWidth*nHeight*1.25),nWidth/2);
- cvResize(uimg,uuimg,CV_INTER_LINEAR);
- cvResize(vimg,vvimg,CV_INTER_LINEAR);
- 
- cvMerge(yimg,uuimg,vvimg,NULL,image);
- cvCvtColor(image,rgbimg,CV_YCrCb2BGR);




**[html]**[view plain](http://blog.csdn.net/woxincd/article/details/7405584)[copy](http://blog.csdn.net/woxincd/article/details/7405584)



- IplImage*image,*rgbimg,*yimg,*uimg,*vimg,*uuimg,*vvimg;
- 
- rgbimg=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
- image=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
- 
- yimg=cvCreateImageHeader(cvSize(nWidth,nHeight),IPL_DEPTH_8U,1);
- uimg=cvCreateImageHeader(cvSize(nWidth/2,nHeight/2),IPL_DEPTH_8U,1);
- vimg=cvCreateImageHeader(cvSize(nWidth/2,nHeight/2),IPL_DEPTH_8U,1);
- 
- uuimg=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,1);
- vvimg=cvCreateImage(cvSize(nWidth,nHeight),IPL_DEPTH_8U,1);
- 
- cvSetData(yimg,pBuf,nWidth);
- cvSetData(uimg,pBuf+nWidth*nHeight,nWidth/2);
- cvSetData(vimg,pBuf+long(nWidth*nHeight*1.25),nWidth/2);
- cvResize(uimg,uuimg,CV_INTER_LINEAR);
- cvResize(vimg,vvimg,CV_INTER_LINEAR);
- 
- cvMerge(yimg,uuimg,vvimg,NULL,image);
- cvCvtColor(image,rgbimg,CV_YCrCb2BGR);



还有一个方法就比较复杂







首先自己根据原理转换为rgb格式

然后利用cvSetData（）函数写入数据生成IplImage格式的图片

首先定义转换的公式：





**[html]**[view plain](http://blog.csdn.net/dreamd1987/article/details/7259479)[copy](http://blog.csdn.net/dreamd1987/article/details/7259479)



- #defineMR(Y,U,V)(Y+(1.403)*(V-128))
- #defineMG(Y,U,V)(Y-(0.344)*(U-128)-(0.714)*(V-128))
- #defineMB(Y,U,V)(Y+((1.773)*(U-128)))




**[html]**[view plain](http://blog.csdn.net/woxincd/article/details/7405584)[copy](http://blog.csdn.net/woxincd/article/details/7405584)



- #defineMR(Y,U,V)(Y+(1.403)*(V-128))
- #defineMG(Y,U,V)(Y-(0.344)*(U-128)-(0.714)*(V-128))
- #defineMB(Y,U,V)(Y+((1.773)*(U-128)))



yuv转rgb的函数：











**[html]**[view plain](http://blog.csdn.net/dreamd1987/article/details/7259479)[copy](http://blog.csdn.net/dreamd1987/article/details/7259479)



- voidYUV420_C_RGB(char*pYUV,unsignedchar*pRGB,intheight,intwidth)
- {
- char*pY=pYUV;
- char*pU=pYUV+height*width;
- char*pV=pU+(height*width/4);
- 
- 
- unsignedchar*pBGR=NULL;
- unsignedcharR=0;
- unsignedcharG=0;
- unsignedcharB=0;
- charY=0;
- charU=0;
- charV=0;
- doubletmp=0;
- for(inti=0;i<height;++i)
- {
- for(intj=0;j<width;++j)
- {
- pBGR=pRGB+i*width*3+j*3;
- 
- Y=*(pY+i*width+j);
- U=*pU;
- V=*pV;
- 
- //B
- tmp=MB(Y,U,V);
- //B=(tmp>255)?255:(char)tmp;
- //B=(B<0)?0:B;
- B=(unsignedchar)tmp;
- //G
- tmp=MG(Y,U,V);
- //G=(tmp>255)?255:(char)tmp;
- //G=(G<0)?0:G;
- G=(unsignedchar)tmp;
- //R
- tmp=MR(Y,U,V);
- //R=(tmp>255)?255:(char)tmp;
- //R=(R<0)?0:R;
- R=(unsignedchar)tmp;
- 
- 
- *pBGR=R;
- *(pBGR+1)=G;
- *(pBGR+2)=B;
- 
- 
- if(i%2==0&&j%2==0)
- {
- *pU++;
- //*pV++;
- }
- else
- {
- if(j%2==0)
- {
- *pV++;
- }
- }
- }
- 
- }
- }




**[html]**[view plain](http://blog.csdn.net/woxincd/article/details/7405584)[copy](http://blog.csdn.net/woxincd/article/details/7405584)



- voidYUV420_C_RGB(char*pYUV,unsignedchar*pRGB,intheight,intwidth)
- {
- char*pY=pYUV;
- char*pU=pYUV+height*width;
- char*pV=pU+(height*width/4);
- 
- 
- unsignedchar*pBGR=NULL;
- unsignedcharR=0;
- unsignedcharG=0;
- unsignedcharB=0;
- charY=0;
- charU=0;
- charV=0;
- doubletmp=0;
- for(inti=0;i<height;++i)
- {
- for(intj=0;j<width;++j)
- {
- pBGR=pRGB+i*width*3+j*3;
- 
- Y=*(pY+i*width+j);
- U=*pU;
- V=*pV;
- 
- //B
- tmp=MB(Y,U,V);
- //B=(tmp>255)?255:(char)tmp;
- //B=(B<0)?0:B;
- B=(unsignedchar)tmp;
- //G
- tmp=MG(Y,U,V);
- //G=(tmp>255)?255:(char)tmp;
- //G=(G<0)?0:G;
- G=(unsignedchar)tmp;
- //R
- tmp=MR(Y,U,V);
- //R=(tmp>255)?255:(char)tmp;
- //R=(R<0)?0:R;
- R=(unsignedchar)tmp;
- 
- 
- *pBGR=R;
- *(pBGR+1)=G;
- *(pBGR+2)=B;
- 
- 
- if(i%2==0&&j%2==0)
- {
- *pU++;
- //*pV++;
- }
- else
- {
- if(j%2==0)
- {
- *pV++;
- }
- }
- }
- 
- }
- }



最后是写入IplImage的代码：











**[html]**[view plain](http://blog.csdn.net/dreamd1987/article/details/7259479)[copy](http://blog.csdn.net/dreamd1987/article/details/7259479)



- unsignedchar*pRGB=NULL;
- 
- pRGB=(unsignedchar*)malloc(nSize*sizeof(unsignedchar*)*2);
- 
- YUV420_C_RGB(pBuf,pRGB,nWidth,nHeight);
- 
- IplImage*image;
- image=cvCreateImageHeader(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
- cvSetData(image,pRGB,nWidth*3);




**[html]**[view plain](http://blog.csdn.net/woxincd/article/details/7405584)[copy](http://blog.csdn.net/woxincd/article/details/7405584)



- unsignedchar*pRGB=NULL;
- 
- pRGB=(unsignedchar*)malloc(nSize*sizeof(unsignedchar*)*2);
- 
- YUV420_C_RGB(pBuf,pRGB,nWidth,nHeight);
- 
- IplImage*image;
- image=cvCreateImageHeader(cvSize(nWidth,nHeight),IPL_DEPTH_8U,3);
- cvSetData(image,pRGB,nWidth*3);


程序都运行过







编译环境为vs2008

opencv2.0版本



原帖地址：[http://blog.csdn.net/dreamd1987/article/details/7259479](http://blog.csdn.net/dreamd1987/article/details/7259479)




