#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>


#include "highgui.h"
#include "cvaux.h"

IplImage* praw;
float VideoBlurDetect(IplImage* data)
{
	int width=data->widthStep;
	int height=data->height;
	ushort* sobelTable = new ushort[width*height];
	memset(sobelTable, 0, width*height*sizeof(ushort));

	int i, j, mul;

	uchar* udata = (uchar*)data->imageData;
	for(i = 1, mul = i*width; i < height - 1; i++, mul += width)
		for(j = 1; j < width - 1; j++)

			sobelTable[mul+j]=abs(udata[mul+j-width-1] + 2*udata[mul+j-1] + udata[mul+j-1+width] -\
			udata[mul+j+1-width] - 2*udata[mul+j+1] - udata[mul+j+width+1]);

	for(i = 1, mul = i*width; i < height - 1; i++, mul += width)
		for(j = 1; j < width - 1; j++)
			if(sobelTable[mul+j] < 50 || sobelTable[mul+j] <= sobelTable[mul+j-1] ||\
				sobelTable[mul+j] <= sobelTable[mul+j+1]) sobelTable[mul+j] = 0;

	int totLen = 0;
	int totCount = 1;

	uchar suddenThre = 50;
	uchar sameThre = 3;

	for(i = 1, mul = i*width; i < height - 1; i++, mul += width)
	{
		for(j = 1; j < width - 1; j++)
		{
			if(sobelTable[mul+j])
			{
				int   count = 0;
				uchar tmpThre = 5;
				uchar max = udata[mul+j] > udata[mul+j-1] ? 0 : 1;

				for(int t = j; t > 0; t--)
				{
					count++;
					if(abs(udata[mul+t] - udata[mul+t-1]) > suddenThre)
						break;

					if(max && udata[mul+t] > udata[mul+t-1])
						break;

					if(!max && udata[mul+t] < udata[mul+t-1])
						break;

					int tmp = 0;
					for(int s = t; s > 0; s--)
					{ 
						if(abs(udata[mul+t] - udata[mul+s]) < sameThre)
						{
							tmp++;
							if(tmp > tmpThre) break;
						}
						else break;
					}

					if(tmp > tmpThre) break;
				}

				max = udata[mul+j] > udata[mul+j+1] ? 0 : 1;

				for(int t = j; t < width; t++)
				{
					count++;
					if(abs(udata[mul+t] - udata[mul+t+1]) > suddenThre)
						break;

					if(max && udata[mul+t] > udata[mul+t+1])
						break;

					if(!max && udata[mul+t] < udata[mul+t+1])
						break;

					int tmp = 0;
					for(int s = t; s < width; s++)
					{
						if(abs(udata[mul+t] - udata[mul+s]) < sameThre)
						{
							tmp++;
							if(tmp > tmpThre) break;
						}
						else break;
					}

					if(tmp > tmpThre) break;
				}
				count--;

				totCount++;
				totLen += count;
			}
		}
	}

	float result = (float)totLen/totCount;
	delete[] sobelTable;
	return result;
}

int main()
{	
	//CvCapture* cvCreateCameraCapture(-1);
		
	float re;
	IplImage* praw=cvLoadImage("nongfu.png",CV_LOAD_IMAGE_GRAYSCALE);
	 re=VideoBlurDetect(praw);


	std::cout<<re;
	cvNamedWindow("Example",CV_WINDOW_AUTOSIZE);
	cvShowImage("Example",praw);
	cvWaitKey(0);
	cvReleaseImage(&praw);
	cvDestroyWindow("Example");
	return 0;
}


