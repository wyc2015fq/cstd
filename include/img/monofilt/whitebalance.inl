//#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <vector>
using namespace std;
//frame number of histogram
const int FRAMENUM=32;
//threshold of discard pixels
const double DISCARDTHRESHOLD=0.0010;
//threshold of discard pixels number
const long PIXELNUMBERTHRESHOLD=100;

using namespace std;

#define _frame_
//#define _line_
//#define _PERSENTAGETHRESHOLD_
#define _NUMBERTHRESHOLD_
//discard pixels and recalculate new value

void Discardinfrequentpixel(long* curr, int currlen,CvSize size,double threshold)
{
	double numbercount=0;
	//discard pixels
	for(int ix=0;ix!=currlen;ix++)
	{
#ifdef _PERSENTAGETHRESHOLD_
		numbercount+=(double)curr[ix];
		if((numbercount/(double)(size.height*size.width))<=threshold)
			curr[ix]=0;
		else
			break;
#endif
#ifdef _NUMBERTHRESHOLD_
		numbercount+=(double)curr[ix];
		if((numbercount/(double)(size.height*size.width))<=threshold)
			curr[ix]=0;
		else if(ix+1<=255&&(double)curr[ix+1]<=PIXELNUMBERTHRESHOLD)
			curr[ix]=0;
		else
			break;
#endif
	}
	numbercount=0;
	for(int ie=currlen-1;ie>=0;ie--)
	{
#ifdef _PERSENTAGETHRESHOLD_		
		numbercount+=(double)curr[ie];
		if((numbercount/(double)(size.height*size.width))<=threshold)
			curr[ie]=255;
		else
			break;
#endif
#ifdef _NUMBERTHRESHOLD_
		numbercount+=(double)curr[ie];
		if((numbercount/(double)(size.height*size.width))<=threshold)
			curr[ie]=255;
		else if(ie-1>=0&&(double)curr[ie-1]<=PIXELNUMBERTHRESHOLD)
			curr[ie]=255;
		else
			break;
#endif
	}
#ifdef _frame_
	//ie-ix
	//in the original image there are unitsnumberperframeoforiginal units in a frame
	int unitsnumberperframeoforiginal=(ie-ix+1)/FRAMENUM;
	//check residue
	int residue=(ie-ix+1)%FRAMENUM;
	//if(residue!=(float)0.0)
	//	unitsnumberperframeoforiginal++;
	//in the result image there are unitsnumberperframeoforiginal units in a frame
	int unitsnumberperframeofresult=256/FRAMENUM;
	//recalculate new value
	for(int i=0;i<FRAMENUM&&ix<=ie;i++)
	{
		//the first unit will be at the place of 0
		if((int)(FRAMENUM-i)>residue)
		{
			for(int j=0;j<unitsnumberperframeoforiginal&&ix<=ie;j++)
			{
				curr[ix]=j+i*unitsnumberperframeofresult;
				ix++;
			}
		}
		//compensation
		if((int)(FRAMENUM-i)==residue)
			unitsnumberperframeoforiginal++;
		//the last unit will be at the place of 255
		if((int)(FRAMENUM-i)<=residue)
		{
			for(int j=(unitsnumberperframeofresult-unitsnumberperframeoforiginal);j<unitsnumberperframeofresult&&ix<=ie;j++)
			{
				curr[ix]=j+i*unitsnumberperframeofresult;
				ix++;
			}
		}
	}
#endif
#ifdef _line_
	int distance=ie-ix;
	for(int i=ix;i<=ie;i++)
	{
		curr[i]=(i-ix)*255/distance;
	}
#endif
}
//look up table then return the new value
uchar Lookuptable(long* curr, int currlen,uchar index)
{
	//0 is the new value
	if(curr[index]==(long)0)
		return (uchar)0;
	else if(curr[index]==(long)255)
		return (uchar)255;
	else
		return (uchar)curr[index];
}

void WBPhotoshop(IplImage* original, IplImage* result)
{
  long saturation[256]={0};
	if(original==NULL)
		return;
	cvSetZero(result);
	cvCvtColor(original,original,CC_BGR2HSV);
	result=cvCloneImage(original);
	//saturation
	//histogram calculate
	for(int i=0;i<original->height;i++)
	{
		for(int j=0;j<original->width;j++)
		{
			//saturation
			uchar sindex=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+1];
			saturation[sindex]++;
		}
	}
	//discard
	Discardinfrequentpixel(saturation, 256,cvGetSize(original),DISCARDTHRESHOLD);
	//calculate result image
	for(i=0;i<original->height;i++)
	{
		for(int j=0;j<original->width;j++)
		{
			//saturation
			uchar sindex=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+1];
			uchar saturationnew=Lookuptable(saturation, 256,sindex);
			//saturation
			result->imageData[i*result->widthStep+j*result->nChannels+1]=saturationnew;
		}
	}
	cvCvtColor(result,result,CC_HSV2BGR);
}

CvScalar CalculateColortemprature(IplImage* original)
{
	return cvScalar(0,0,0,0);
}


void GWM(IplImage* original, IplImage* result)
{
	//B G R
	CvScalar mean;
	mean=cvAvg(original);
	//grey value
	uchar bluemean=mean.val[0];
	uchar greenmean=mean.val[1];
	uchar redmean=mean.val[2];
	uchar greyvalue=(bluemean*11+greenmean*59+redmean*30)/100;
	uchar bluetd=greyvalue-mean.val[0];
	uchar greentd=greyvalue-mean.val[1];
	uchar redtd=greyvalue-mean.val[2];
	//calculate new value
	for(int i=0;i<original->height;i++)
	{
		for(int j=0;j<original->width;j++)
		{
			uchar bluenewvalue=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+0];
			bluenewvalue+=bluetd;
			if(bluenewvalue<0)
				bluenewvalue-=bluetd;
			else if(bluenewvalue>255)
				bluenewvalue-=bluetd;
			uchar greennewvalue=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+1];
			greennewvalue+=greentd;
			if(greennewvalue<0)
				greennewvalue-=greentd;
			else if(greennewvalue>255)
				greennewvalue-=greentd;
			uchar rednewvalue=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+2];
			rednewvalue+=redtd;
			if(rednewvalue<0)
				rednewvalue-=redtd;
			else if(rednewvalue>255)
				rednewvalue-=redtd;
			result->imageData[i*result->widthStep+j*result->nChannels+0]=(uchar)bluenewvalue;
			result->imageData[i*result->widthStep+j*result->nChannels+1]=(uchar)greennewvalue;
			result->imageData[i*result->widthStep+j*result->nChannels+2]=(uchar)rednewvalue;
		}
	}
}

void WBWithHis(IplImage* original, IplImage* result)
{
  long R[256]={0};
	long G[256]={0};
	long B[256]={0};
	if(original==NULL)
		return;
	cvSetZero(result);
	//B G R
	//histogram calculate
	for(int i=0;i<original->height;i++)
	{
		for(int j=0;j<original->width;j++)
		{
			//B
			uchar bindex=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+0];
			//G
			uchar gindex=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+1];
			//R
			uchar rindex=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+2];
			B[bindex]++;
			G[gindex]++;
			R[rindex]++;
		}
	}
	//discard
	Discardinfrequentpixel(B, 256,cvGetSize(original),DISCARDTHRESHOLD);
	Discardinfrequentpixel(G, 256,cvGetSize(original),DISCARDTHRESHOLD);
	Discardinfrequentpixel(R, 256,cvGetSize(original),DISCARDTHRESHOLD);
	//calculate result image
	for(i=0;i<original->height;i++)
	{
		for(int j=0;j<original->width;j++)
		{
			//B
			uchar bindex=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+0];
			//G
			uchar gindex=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+1];
			//R
			uchar rindex=(uchar)original->imageData[i*original->widthStep+j*original->nChannels+2];
			uchar bnew=Lookuptable(B, 256,bindex);
			uchar gnew=Lookuptable(G, 256,gindex);
			uchar rnew=Lookuptable(R, 256,rindex);
			//evaluate new value
			//B
			result->imageData[i*result->widthStep+j*result->nChannels+0]=bnew;
			//G
			result->imageData[i*result->widthStep+j*result->nChannels+1]=gnew;
			//R
			result->imageData[i*result->widthStep+j*result->nChannels+2]=rnew;
		}
	}
}