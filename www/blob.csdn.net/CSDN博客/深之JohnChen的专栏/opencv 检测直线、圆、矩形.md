# opencv 检测直线、圆、矩形 - 深之JohnChen的专栏 - CSDN博客

2009年12月01日 09:26:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：34756


检测直线：cvHoughLines，cvHoughLines2

检测圆：cvHoughCircles

检测矩形：opencv中没有对应的函数，下面有段代码可以检测矩形，是通过先找直线，然后找到直线平行与垂直的四根线。

检测直线代码：

/* This is a standalone program. Pass an image name as a first parameter of the program.

Switch between standard and probabilistic Hough transform by changing "#if 1" to "#if 0" and back */

#include <cv.h>

#include <highgui.h>

#include <math.h>

int main(int argc, char** argv)

{

const char* filename = argc >= 2 ? argv[1] : "pic1.png";

IplImage* src = cvLoadImage( filename, 0 );

IplImage* dst;

IplImage* color_dst;

CvMemStorage* storage = cvCreateMemStorage(0);

CvSeq* lines = 0;

int i;

if( !src )

return -1;

dst = cvCreateImage( cvGetSize(src), 8, 1 );

color_dst = cvCreateImage( cvGetSize(src), 8, 3 );

cvCanny( src, dst, 50, 200, 3 );

cvCvtColor( dst, color_dst, CV_GRAY2BGR );

#if 0

lines = cvHoughLines2( dst, storage, CV_HOUGH_STANDARD, 1, CV_PI/180, 100, 0, 0 );

for( i = 0; i < MIN(lines->total,100); i++ )

{

float* line = (float*)cvGetSeqElem(lines,i);

float rho = line[0];

float theta = line[1];

CvPoint pt1, pt2;

double a = cos(theta), b = sin(theta);

double x0 = a*rho, y0 = b*rho;

pt1.x = cvRound(x0 + 1000*(-b));

pt1.y = cvRound(y0 + 1000*(a));

pt2.x = cvRound(x0 - 1000*(-b));

pt2.y = cvRound(y0 - 1000*(a));

cvLine( color_dst, pt1, pt2, CV_RGB(255,0,0), 3, CV_AA, 0 );

}

#else

lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );

for( i = 0; i < lines->total; i++ )

{

CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);

cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );

}

#endif

cvNamedWindow( "Source", 1 );

cvShowImage( "Source", src );

cvNamedWindow( "Hough", 1 );

cvShowImage( "Hough", color_dst );

cvWaitKey(0);

return 0;

}

检测圆代码：

#include <cv.h>

#include <highgui.h>

#include <math.h>

int main(int argc, char** argv)

{

IplImage* img;

if( argc == 2 && (img=cvLoadImage(argv[1], 1))!= 0)

{

IplImage* gray = cvCreateImage( cvGetSize(img), 8, 1 );

CvMemStorage* storage = cvCreateMemStorage(0);

cvCvtColor( img, gray, CV_BGR2GRAY );

cvSmooth( gray, gray, CV_GAUSSIAN, 9, 9 ); // smooth it, otherwise a lot of false circles may be detected

CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2, gray->height/4, 200, 100 );

int i;

for( i = 0; i < circles->total; i++ )

{

float* p = (float*)cvGetSeqElem( circles, i );

cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), 3, CV_RGB(0,255,0), -1, 8, 0 );

cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );

}

cvNamedWindow( "circles", 1 );

cvShowImage( "circles", img );

}

return 0;

}

检测矩形代码：
/*在程序里找寻矩形*/#ifdef _CH_#pragma package <opencv>#endif#ifndef _EiC#include "cv.h"#include "highgui.h"#include <stdio.h>#include <math.h>#include <string.h>#endifint thresh = 50;IplImage* img = 0;IplImage* img0 = 0;CvMemStorage* storage = 0;CvPoint pt[4];const char* wndname = "Square Detection Demo";// helper function:// finds a cosine of angle between vectors// from pt0->pt1 and from pt0->pt2 double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 ){double dx1 = pt1->x - pt0->x;double dy1 = pt1->y - pt0->y;double dx2 = pt2->x - pt0->x;double dy2 = pt2->y - pt0->y;return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);}// returns sequence of squares detected on the image.// the sequence is stored in the specified memory storageCvSeq* findSquares4( IplImage* img, CvMemStorage* storage ){CvSeq* contours;int i, c, l, N = 11;CvSize sz = cvSize( img->width & -2, img->height & -2 );IplImage* timg = cvCloneImage( img ); // make a copy of input imageIplImage* gray = cvCreateImage( sz, 8, 1 ); IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );IplImage* tgray;CvSeq* result;double s, t;// create empty sequence that will contain points -// 4 points per square (the square's vertices)CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );// select the maximum ROI in the image// with the width and height divisible by 2cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));// down-scale and upscale the image to filter out the noisecvPyrDown( timg, pyr, 7 );cvPyrUp( pyr, timg, 7 );tgray = cvCreateImage( sz, 8, 1 );// find squares in every color plane of the imagefor( c = 0; c < 3; c++ ){// extract the c-th color planecvSetImageCOI( timg, c+1 );cvCopy( timg, tgray, 0 );// try several threshold levelsfor( l = 0; l < N; l++ ){// hack: use Canny instead of zero threshold level.// Canny helps to catch squares with gradient shadingif( l == 0 ){// apply Canny. Take the upper threshold from slider// and set the lower to 0 (which forces edges merging) cvCanny( tgray, gray, 0, thresh, 5 );// dilate canny output to remove potential// holes between edge segments cvDilate( gray, gray, 0, 1 );}else{// apply threshold if l!=0://tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );}// find contours and store them all as a listcvFindContours( gray, storage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );// test each contourwhile( contours ){// approximate contour with accuracy proportional// to the contour perimeterresult = cvApproxPoly( contours, sizeof(CvContour), storage,CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );// square contours should have 4 vertices after approximation// relatively large area (to filter out noisy contours)// and be convex.// Note: absolute value of an area is used because// area may be positive or negative - in accordance with the// contour orientationif( result->total == 4 &&fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&cvCheckContourConvexity(result) ){s = 0;for( i = 0; i < 5; i++ ){// find minimum angle between joint// edges (maximum of cosine)if( i >= 2 ){t = fabs(angle((CvPoint*)cvGetSeqElem( result, i ),(CvPoint*)cvGetSeqElem( result, i-2 ),(CvPoint*)cvGetSeqElem( result, i-1 )));s = s > t ? s : t;}}// if cosines of all angles are small// (all angles are ~90 degree) then write quandrange// vertices to resultant sequence if( s < 0.3 )for( i = 0; i < 4; i++ )cvSeqPush( squares,(CvPoint*)cvGetSeqElem( result, i ));}// take the next contourcontours = contours->h_next;}}}// release all the temporary imagescvReleaseImage( &gray );cvReleaseImage( &pyr );cvReleaseImage( &tgray );cvReleaseImage( &timg );return squares;}// the function draws all the squares in the imagevoid drawSquares( IplImage* img, CvSeq* squares ){CvSeqReader reader;IplImage* cpy = cvCloneImage( img );int i;// initialize reader of the sequencecvStartReadSeq( squares, &reader, 0 );// read 4 sequence elements at a time (all vertices of a square)for( i = 0; i < squares->total; i += 4 ){CvPoint* rect = pt;int count = 4;// read 4 verticesmemcpy( pt, reader.ptr, squares->elem_size );CV_NEXT_SEQ_ELEM( squares->elem_size, reader );memcpy( pt + 1, reader.ptr, squares->elem_size );CV_NEXT_SEQ_ELEM( squares->elem_size, reader );memcpy( pt + 2, reader.ptr, squares->elem_size );CV_NEXT_SEQ_ELEM( squares->elem_size, reader );memcpy( pt + 3, reader.ptr, squares->elem_size );CV_NEXT_SEQ_ELEM( squares->elem_size, reader );// draw the square as a closed polyline cvPolyLine( cpy, &rect, &count, 1, 1, CV_RGB(0,255,0), 3, CV_AA, 0 );}// show the resultant imagecvShowImage( wndname, cpy );cvReleaseImage( &cpy );}void on_trackbar( int a ){if( img )drawSquares( img, findSquares4( img, storage ) );}char* names[] = { "pic1.png", "pic2.png", "pic3.png","pic4.png", "pic5.png", "pic6.png", 0 };int main(int argc, char** argv){int i, c;// create memory storage that will contain all the dynamic datastorage = cvCreateMemStorage(0);for( i = 0; names[i] != 0; i++ ){// load i-th imageimg0 = cvLoadImage( names[i], 1 );if( !img0 ){printf("Couldn't load %s/n", names[i] );continue;}img = cvCloneImage( img0 );// create window and a trackbar (slider) with parent "image" and set callback// (the slider regulates upper threshold, passed to Canny edge detector) cvNamedWindow( wndname, 1 );cvCreateTrackbar( "canny thresh", wndname, &thresh, 1000, on_trackbar );// force the image processingon_trackbar(0);// wait for key.// Also the function cvWaitKey takes care of event processingc = cvWaitKey(0);// release both imagescvReleaseImage( &img );cvReleaseImage( &img0 );// clear memory storage - reset free space positioncvClearMemStorage( storage );if( c == 27 )break;}cvDestroyWindow( wndname );return 0;}#ifdef _EiCmain(1,"squares.c");#endif

