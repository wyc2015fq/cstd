
#define USE_MYCV

#include "cstd.h"
#include "facelib/facedetect/facedetect.h"
#include "cv/imgproc/drawing.inl"

//#include "img/color.inl"
#include "mtcnn.h"

#include "agegender.h"
#include "cap.h"
#undef ACCESS_MASK

//#include "draw/imdraw.inl"
#include "img/imgio.inl"
//#include "face/face_recog.inl"

// < 	little-endian 	standard 	none
// > 	big-endian 	standard 	none
// x 	pad byte 	no value 	  	 
// c 	char 	bytes of length 1 	1 	 
// b 	signed char 	integer 	1 	(1),(3)
// B 	unsigned char 	integer 	1 	(3)
// ? 	_Bool 	bool 	1 	(1)
// h 	short 	integer 	2 	(3)
// H 	unsigned short 	integer 	2 	(3)
// i 	int 	integer 	4 	(3)
// I 	unsigned int 	integer 	4 	(3)
// l 	long 	integer 	4 	(3)
// L 	unsigned long 	integer 	4 	(3)
// q 	long long 	integer 	8 	(2), (3)
// Q 	unsigned long long 	integer 	8 	(2), (3)
// n 	ssize_t 	integer 	  	(4)
// N 	size_t 	integer 	  	(4)
// e 	(7) 	float 	2 	(5)
// f 	float 	float 	4 	(5)
// d 	double 	float 	8 	(5)
// s 	char[] 	bytes 	  	 
// p 	char[] 	bytes 	  	 
// P 	void * 	integer 	  	(6)

int test_mtcnn()
{
#ifdef _WIN32
  //CPCODE;
  //sys_chdir("E:/code/cstd/include/facelib/MTCNN"); cRange
#endif
  int minsize = 60;
  Bbox out[200];

#if defined _WIN32 && defined _DEBUG

  if (1) {
    img_t im[1] = {0};
    int i, num, nface = 0, times = 2;
    const char* filename;
    filename = "C:/code/testc/feat/0/0.jpg";
    filename = "Z:/yiwei/rywclm/aaa_20171025041558_2.jpg";
    filename = "Z:/yiwei/rywclm/aaa_20171026030548_0.jpg";
    filename = "Z:/yiwei/rywclm/aaa_20171026041207_0.jpg";
    filename = "D:/rywclm/aaa_20171031053339_9.jpg";
    filename = "F:/dataset/facedetect/hztest.jpg";
	filename = "D:/data/fddb/img_18.jpg";
    //filename = ("C:/code/testc/Release/aaa_20171020145235_0.jpg");
    imread(filename, 3, 1, im);
    {
      utime_start(_start_time);
      tictoc0;

      for (i = 0; i < times; ++i) {
        nface = mtcnn_findFace(im, out, countof(out), minsize);
      }

      tictoc;
      //ASSERT(nface==15);
      printf("avg time: %f\n", utime_elapsed(_start_time) / times);

      if (nface > 0) {
        float ev[3] = {0};
        EstimateHeadPose(out->ppoint, ev);
        printf("%f %f %f\n", ev[0], ev[1], ev[2]);
      }

    }

    if (0) {
      for (i = 0; i < nface; ++i) {
        Bbox* it = out + i;
        printf("(%3d %3d %3d %3d)\n", (int)(*it).y1, (int)(*it).x1, (int)((*it).y2 - (*it).y1), (int)((*it).x2 - (*it).x1));
        cvRectangle(im, cPoint((*it).y1, (*it).x1), cPoint((*it).y2, (*it).x2), cScalar(0, 0, 255, 0), 2, 8, 0);

        for (num = 0; num < 5; num++) {
          cvCircle(im, cPoint((int) * (it->ppoint + num), (int) * (it->ppoint + num + 5)), 3, cScalar(0, 255, 255, 0), -1, 8, 0);
        }

        //imdrawaa_rect(im, false, NULL, dRECT((*it).y1, (*it).x1, (*it).y2, (*it).x2), 0, _RGB(255, 0, 0), 3);
        //for(int num=0;num<5;num++) imdrawaa_circle(im, NULL, it->ppoint[num], it->ppoint[num+5], 3, _RGB(255, 255, 0), 0, 0);
      }

      printf("\n");
      imshow_(im);
      WaitKey(0);
    }

    imfree(im);
    return 0;
  }

#endif

  {
    capdev cap[1] = { 0 };
    int h = 720, w = 1280;
    h = 1080, w = 1920;
    
    //h = 480, w = 640, minsize=60;
    if (capdshow_open(cap, 0, w, h) > 0) {
      int fram = 0;
      char ch = 0;
      img_t im[2] = { 0 };
      img_t* im1 = im + 1;
      int nface = 0;
      double avg = 0;
      double avgs[32] = {0};
      double avg_10 = 0;
      double avg2 = 0;
      XRECT out1[500] = {0};
      img_t* gry = im + 1;
      int i, num;
      
      for (; 'q' != ch && 'Q' != ch; ++fram) {
        cap_getframe(cap, im, CAP_RGB);
        //image.create(h, w, CC_8UC3, im->tt.data, im->s);
        //imtrans(im, im1, 1, 1, 0);
    //imwrite("test_1920x1080.bmp", im);
        im1 = im;
#ifdef _FACE_RECOG_INL_
        if (0) {
          utime_start(_start_time);
          imcolorcvt(im, gry, T_BGR, T_GRAY);
          tictoc0;
          CPM_FaceDetect(vis_nesting_face20110713, gry->h, gry->w, gry->data, gry->s, NULL, 0, 1. / 3, 1, 100, 1.1, 1, 0.8, 4, out1, countof(out1));
          tictoc;
          double t = utime_elapsed(_start_time);
          avg2 += t;
          printf("%f %f ", t, avg2 / (fram + 1));
        }
#endif // _FACE_RECOG_INL_
        
        {
          double t;
          utime_start(_start_time);
          tictoc0;
          nface = mtcnn_findFace(im1, out, countof(out), minsize);
          tictoc;
          t = utime_elapsed(_start_time);
          avgs[fram % countof(avgs)] = t;
          avg = 0;
          
          for (i = 0; i < countof(avgs); ++i) {
            avg += avgs[i];
          }
          
          printf("%f %f ", t, (avg) / countof(avgs));
        }
        
        if (1) {
          int age[100] = {0};
          int gender[100] = {0};
          for (i = 0; i < nface; ++i) {
            Bbox* it = out + i;
            int d = ((*it).y2-(*it).y1)/3;
            IRECT rc = iRECT((int)(*it).y1, (int)(*it).x1, (int)(*it).y2, (int)(*it).x2);
            img_t im1[1] = {0};
            rc = iRectInflate1(rc, d);
            age[i] = face_age(imsubref(im, rc, im1));
            gender[i] = face_gender(imsubref(im, rc, im1));
          }
          for (i = 0; i < nface; ++i) {
            Bbox* it = out + i;
            printf("(%3d %3d %3d %3d)", (int)(*it).y1, (int)(*it).x1, (int)((*it).y2 - (*it).y1), (int)((*it).x2 - (*it).x1));
            CPoint pt;
            cvRectangle(im1, pt = cPoint((*it).y1, (*it).x1), cPoint((*it).y2, (*it).x2), cScalar(0, 0, 255, 0), 2, 8, 0);
            char buf[256] = {0};
            _snprintf(buf, 256, "%d %d", gender[i], age[i]);
            putText(im1, buf, pt, CC_FONT_HERSHEY_SIMPLEX, 1, cScalar(0, 0, 255));
            
            for (num = 0; num < 5; num++) {
              cvCircle(im1, cPoint((int) it->ppoint[num], (int) it->ppoint[num + 5]), 3, cScalar(0, 255, 255, 0), -1, 8, 0);
            }
            
            //imdrawaa_rect(im, false, NULL, dRECT((*it).y1, (*it).x1, (*it).y2, (*it).x2), 0, _RGB(255, 0, 0), 3);
            //for(int num=0;num<5;num++) imdrawaa_circle(im, NULL, it->ppoint[num], it->ppoint[num+5], 3, _RGB(255, 255, 0), 0, 0);
          }
        }
        
        printf("\n");
        imshow_(im1);
        ch = WaitKey(10);
      }
      
      cap_close(cap);
      imfrees2(im);
    }
  }

  return 0;
}

int test_MTCNN1()
{
  sys_chdir("E:/code/cstd/include/facelib/MTCNN");
#if 0

  if (1) {
    Mat image = imread("4.jpg");
    mtcnn find(image.rows, image.cols);
    clock_t start;
    start = clock();
    find.findFace(image);
    imshow("result", image);
    imwrite("result.jpg", image);
    start = clock() - start;
    cout << "time is  " << start / 10e3 << endl;
  }

  if (1) {
    Mat image;
    VideoCapture cap(0);
    int h = 720, w = 1280;
    cap.set(CAP_PROP_FRAME_WIDTH, w);
    cap.set(CAP_PROP_FRAME_HEIGHT, h);
    mtcnn find(h, w);

    if (!cap.isOpened()) {
      cout << "fail to open!" << endl;
    }

    for (;;) {
      cap >> image;
      {
        utime_start(_start_time);
        find.findFace(image);
        printf("%f\n", utime_elapsed(_start_time));
      }
      imshow("result", image);
      waitKey(10);
    }

    if (!image.data) {
      cout << "读取视频失败" << endl;
      return -1;
    }
  }

  // mtcnn find(image.rows, image.cols);
  // clock_t start;
  // int stop = 1200;
  // while(stop--){
  //     start = clock();
  //     cap>>image;
  //     find.findFace(image);
  //     imshow("result", image);
  //     if( waitKey(1)>=0 ) break;
  //     start = clock() -start;
  //     cout<<"time is  "<<start/10e3<<endl;
  // }

  waitKey(0);
#endif
  //image.release();
  return 0;
}
