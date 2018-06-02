
#ifndef _FACETRACK_INL_
#define _FACETRACK_INL_
//#include "faceDetect.inl"
//#include "asmfit.inl"

int iRectArea(IRECT r) {
  int area = (r.r-r.l)*(r.b-r.t);
  return abs(area);
}

double iRectCoverage(IRECT r1, IRECT r2) {
  int b = iRectArea(r1) + iRectArea(r2);
  double area=0;
  if (b>0) {
    IRECT r = iRectInter(r1, r2);
    int a = iRectArea(r);
    area = 2.0*a/b;
  }
  return area;
}

IRECT iRectXRECT(CRectf r) {
  IRECT ri = iRECT(r.x, r.y, r.x+r.w, r.y+r.h);
  return ri;
}
DRECT dRectXRECT(CRectf r) {
  DRECT ri = dRECT(r.x, r.y, r.x+r.w, r.y+r.h);
  return ri;
}
double xRectCoverage(CRectf xr1, CRectf xr2) {
  IRECT r1 = iRectXRECT(xr1);
  IRECT r2 = iRectXRECT(xr2);
  r1 = iRectInflate1(r1, xr1.w*1.5);
  r2 = iRectInflate1(r2, xr2.w*1.5);
  return iRectCoverage(r1, r2);
}

#include "facelib/facedetect/facedetect.h"

int facetrack_find(CRectf rc, const faceobj_t* fo, int nfo) {
  double area = 0.3;
  int i, j=-1;
  for (i=0; i<nfo; ++i) {
    double a = xRectCoverage(rc, fo[i].face.rect);
    if (a>area) {
      area = a;
      j = i;
    }
  }
  return j;
}

int contours_ending_points[20];
int contours_ending_points_len=0;
int shape_result_len=0;

FPOINT face_part_center(const FPOINT* pt, int i)  {
  int i0 = i>0 ? contours_ending_points[i-1]:0;
  int i1 = contours_ending_points[i];
  FPOINT cpt = {0, 0};
  if (i0<i1) {
    int i, n=i1-i0;
    for (i=i0; i<i1; ++i) {
      cpt.x += pt[i].x;
      cpt.y += pt[i].y;
    }
    cpt.x /=n;
    cpt.y /=n;
  }
  return cpt;
}
IPOINT face_part_center_i(const FPOINT* pt, int i)  {
  FPOINT fpt = face_part_center(pt, i);
  return iPOINT(fpt.x, fpt.y);
}
DRECT face_part_rect(const FPOINT* pt, int i)  {
  int i0 = i>0 ? contours_ending_points[i-1]:0;
  int i1 = contours_ending_points[i];
  DRECT rc = {0, 0, 0, 0};
  int n = i1-i0;
  pt += i0;
  rc = dRECT(pt[0].x, pt[0].y, pt[0].x, pt[0].y);
  for (i=1; i<n; ++i) {
    rc.l = MIN(rc.l, pt[i].x);
    rc.t = MIN(rc.t, pt[i].y);
    rc.r = MAX(rc.r, pt[i].x);
    rc.b = MAX(rc.b, pt[i].y);
  }
  return rc;
}
XRECT face_part_xrect(const FPOINT* pt, int i)  {
  DRECT rc = face_part_rect(pt, i);
  XRECT xrc;
  xrc.x = rc.l;
  xrc.w = rc.r-rc.l;
  xrc.y = rc.t;
  xrc.h = rc.b-rc.t;
  return xrc;
}

int faceobj_clear(faceobj_t* fo, int nfo) {
  int i=0, j=0;
  for (i=0; i<nfo; ++i) {
    if (fo[i].event>=0) {
      if (j!=i) {
        bffree(fo[j].image);
        fo[j] = fo[i];
        fo[i].image->data = 0;
        fo[i].image->len = 0;
      }
      j++;
    }
  }
  return j;
}

XRECT xRECT(int x, int y, int w, int h) {
  XRECT r = {0};
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  return r;
}

static void faceobj_init(faceobj_t* f, int* face_code) {
  memset(f, 0, sizeof(*f));
  f->face_code = (*face_code)++;
  f->enter_time = -1;
  f->left_time = -1;
  f->id = -1;
  f->sideface = -1;
  int i;
  for (i=0; i<MAX_COUNT_AGEGENDER; ++i) {
    f->ages[i] = -1;
    f->genders[i] = -1;
  }
  f->glass = -1;
  f->smile = -1;
  f->image->len = 0;
}

img_t imsubrefex(const img_t* im, XRECT face, int d)
{
	IRECT rc = { face.x, face.y, face.x + face.w, face.y + face.h };
	img_t im1[1] = { 0 };
	rc = iRectInflate1(rc, d);
	imsubref(im, rc, im1);
	return *im1;
}

#if 0
//#include "facelib/MTCNN/test_MTCNN.inl"
#include "E:\code\cstd\include/facelib/MTCNN/test_MTCNN.inl"

#include "E:\code\cstd\include/facelib/libfacedetect/facedetect-dll.h"
#pragma comment(lib,"E:/code/cstd/include/facelib/libfacedetect/libfacedetect.lib")

int ysq_facedetect(Mat img, XRECT* objects)
{
	int *pResults;
	static unsigned char *pBuffer = NULL;
	Mat imgGray;
	if (img.channels() == 3) {
		cv::cvtColor(img, imgGray, CV_BGR2GRAY);
	}
	else {
		imgGray = img;
	}
	if (NULL == pBuffer) {
		pBuffer = (unsigned char *)malloc(0xC004);
	}
	double scale = 1.1;
	int min_neighbors = 3;
	int min_object_width = 60;
	//int max_object_width = 2;
	int8_t type = 3;
	switch (type)
	{
	case 0:
		pResults = facedetect_frontal(pBuffer, (unsigned char*)(imgGray.ptr(0)), imgGray.cols, imgGray.rows, (int)imgGray.step, scale, min_neighbors, min_object_width);
		break;
	case 1:
		pResults = facedetect_frontal_surveillance(pBuffer, (unsigned char*)(imgGray.ptr(0)), imgGray.cols, imgGray.rows, (int)imgGray.step, scale, min_neighbors, min_object_width);
		break;
	case 2:
		pResults = facedetect_multiview(pBuffer, (unsigned char*)(imgGray.ptr(0)), imgGray.cols, imgGray.rows, (int)imgGray.step, scale, min_neighbors, min_object_width);
		break;
	case 3:
		pResults = facedetect_multiview_reinforce(pBuffer, (unsigned char*)(imgGray.ptr(0)), imgGray.cols, imgGray.rows, (int)imgGray.step, scale, min_neighbors, min_object_width);
		break;
	default:
		return -1;
	}
	int j = 0;
	for (uint8_t i = 0; i < (pResults ? *pResults : 0); i++)
	{
		short * p = ((short*)(pResults + 1)) + 6 * i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		if (w < 0)
			w = 1;
		if (h < 0)
			h = 1;
		if (x + w > imgGray.cols)
			w = imgGray.cols - x;
		if (y + h > imgGray.rows)
			h = imgGray.rows - y;
		Rect ret = Rect(x, y, w, h);
		objects[j].x = x;
		objects[j].y = y;
		objects[j].w = w;
		objects[j].h = h;
		++j;
	}
	return j;
}
#endif

#if 0
#include "D:/code/facelib/ArcSoft/ARCSOFT_FSDK_FACE_DETECTION_1.0.0.68_VS2013_WIN32_09202017/arcsoft_fsdk_face_detection.inl"
#endif
//#include "facelib/ReadFace/RSFaceSDK.inl"
#include "facelib/facedetect/facedetect.inl"
//#include "dnn/ncnn/mtcnn.inl"
//#include "dnn/ncnn/test_mtcnn.inl"

static int add_agegender(int* arr, int x) {
  int i=0;
  for (i=0; i<MAX_COUNT_AGEGENDER && arr[i]>=0; ++i);
  if (i<MAX_COUNT_AGEGENDER) {
    arr[i] = x;
  }
#define INTGREATER(A, B)  ((A)>(B))
  QSORT(arr, arr+i, INTGREATER, int);
#undef INTGREATER
  return arr[i/2];
}

static int facetrack(const img_t* im, faceobj_t* fo, int nfo, int maxfo, int* face_code) {
  int i, j, ret=0;
  static int lastid = 1;
  //asmfit_param_t ap[1] = {0};
  IM_BEGIN(im1, 10);
  img_t* gray = im1++;
  struct Face faces[20];
  //float* ptx=0, *pty=0;
  if (im->c==3) {
    imcolorcvt( im, gray, T_BGR, T_GRAY );
  } else if (im->c==1) {
    imclone2(im, gray);
  }
#if 0
  ap->height = im->h, ap->width = im->w, ap->data = gray->tt.data, ap->datastep = gray->s;
  //ap->leye = leye, ap->reye = reye;
  ap->asm_mode = asmTrainData_all;
  if (NULL==ap->buf) {
    asmfit_memsize(&ap);
    //ap->shape_result = MALLOC(CvPoint2D32f, ap->shape_result_len);
    ap->buf = MALLOC(char, ap->buflen);
  }
#endif
  //cvShowImage( "sadf", stdh, stdw, stdimg, stdstep, 3 );
  //ret = asmfit_process2( &ap );
  {
    //asmfit_param_t* p = ap;
    int face_cnt;
    int height = im->h, width = im->w;
    const unsigned char* gry = gray->tt.data;
    //imwrite("AAAA.bmp",height, width, gry, width, 1);
#if 0
    enum { BUFLEN = 100 };
    XRECT out[ BUFLEN ];
	if (0) {
		double min_width = 60;
		double ss = 20. / min_width;
		face_cnt = CPM_FaceDetect(vis_nesting_face0701, height, width, gry, width,
			0, 0, ss, 1.f, 15.f, 1.1f, 0, 0.5f, 4, out, BUFLEN);
#if 0
		if (0 && face_cnt > 0) {
			int opencv_facedetect(int h, int w, const unsigned char* img, int step, XRECT* out);
			int j = 0;
			for (i = 0; i < face_cnt; ++i) {
				int d = out[i].h / 4;
				img_t im2[1] = { 0 };
				//IMINIT(im1, height, width, gry, width, 1, 1);
				*im2 = imsubrefex(im, out[i], d);
				int ret = opencv_facedetect(im2->h, im2->w, im2->tt.data, im2->s, NULL);
				if (ret) {
					out[j++] = out[i];
				}
			}
			face_cnt = j;
		}
#endif
	}
#endif

	face_cnt = 0;
#if 1
	if (1) {
		struct FaceSet* faceset = facedetect_static(im->h, im->w, im->tt.data, im->s, im->c, 60);
		j = 0;
		for (j=0; j<faceset->nfaces; ++j) {
      faces[j] = faceset->faces[j];
      //rectangle(image, Point((*it).y1, (*it).x1), Point((*it).y2, (*it).x2), Scalar(0, 0, 255), 2, 8, 0);
		}
		face_cnt = faceset->nfaces;
	}
#endif

#if 0
	if (1) {
		int opencv_facedetect(int h, int w, const unsigned char* img, int step, XRECT* out);
		face_cnt = opencv_facedetect(im->h, im->w, im->tt.data, im->s, out);
	}
	if (1) {
		face_cnt = RSFaceSDK_face_detection(im, out);
	}
	if (1) {
		face_cnt = arcsoft_fsdk_face_detection(im, out);
	}
	if (1) {
		Mat image = Mat(im->h, im->w, CV_8UC3, im->tt.data, im->s);
		face_cnt = ysq_facedetect(image, out);
	}
#endif
    for (i=0; i<nfo; ++i) {
      fo[i].nodetectcnt++;
      fo[i].event = 0;
    }
    j = nfo;
    //printf("face_cnt = %d\n", face_cnt);
#define RECTCENTER(_RT, _PT) ((_PT).x = (_RT).x + (_RT).w / 2, (_PT).y = (_RT).y + (_RT).h / 2)
    for (i=0; i<face_cnt && j<maxfo; ++i) {
      struct Face *face = faces + i;
      XRECT out;
      out.x = face->rect.x;
      out.y = face->rect.y;
      out.w = face->rect.width;
      out.h = face->rect.height;

      int i1 = facetrack_find(faces[i].rect, fo, nfo);
      //XRECT * face = out+i;
#if 0
      XRECT *eye = out + face_cnt;
      int len = BUFLEN - face_cnt;
      const void * eyecas = VisAllEye_nesting;
      const void * alleye = 0;
      const void * eyewg = 0; //CPM_GetCascade(NIR_EYE_WITHOUT_GLASS_CASCADE);
      const void * deyecas = 0;  //CPM_GetCascade(NIR_DEYE_CASCADE);
      int ret1 = 3 == CPM_EyesDetect( deyecas, eyecas, eyecas, alleye, alleye, eyewg, eyewg, gry, width, 1, face, eye, len );
#endif
      if (1) {
        //RECTCENTER( eye[ 1 ], p->leye );
        //RECTCENTER( eye[ 2 ], p->reye );
        if (i1<0) {
          i1 = j++;
          faceobj_init(fo+i1, face_code);
          fo[i1].event = 1;
          fo[i1].inited = 0;
          fo[i1].id = lastid++;
        }
        fo[i1].nodetectcnt = 0;
        fo[i1].face = faces[ i ];
        //fo[i1].leye = eye[ 1 ];
        //fo[i1].reye = eye[ 2 ];
      }
#if 0
      else {
        if (i1>=0) {
          //FPOINT fpt;
          fo[i1].nodetectcnt = 0;
          fo[i1].face = out[i];
          ap->shape_result = fo[i1].landmark;
          fo[i1].leye = face_part_xrect(fo[i1].landmark, 1);
          fo[i1].reye = face_part_xrect(fo[i1].landmark, 3);
          //p->leye = face_part_center_i(fo[j].landmark, 1);
          RECTCENTER( fo[i1].leye, p->leye );
          RECTCENTER( fo[i1].reye, p->reye );
        } else if (1) {
          i1 = j++;
          faceobj_init(fo+i1, face_code);
          fo[i1].event = 1;
          fo[i1].inited = 0;
          fo[i1].id = lastid++;
          fo[i1].nodetectcnt = 0;
          fo[i1].leye = xRECT(face->x+face->w/4-10, face->y+face->h/4-5, 20, 10);
          fo[i1].reye = xRECT(face->x+face->w*3/4-10, face->y+face->h/4-5, 20, 10);
        }
      }
      
      if (i1>=0) {
        fo[i1].face = out[ i ];
        ap->shape_result = fo[i1].landmark;
        asmfit_process(p);
        shape_result_len = p->shape_result_len;
        contours_ending_points_len = p->contours_ending_points_len;
        MEMCPY(contours_ending_points, p->contours_ending_points, p->contours_ending_points_len);
        fo[i1].leye = face_part_xrect(fo[i1].landmark, 1);
        fo[i1].reye = face_part_xrect(fo[i1].landmark, 3);
      }
#endif
    }
    nfo = j;
    {
      for (i=0; i<nfo; ++i) {
        if (fo[i].nodetectcnt==0) {
          int gender = face_gender_face(im, &fo[i].face);
          //imshow_(im2);
          int age = face_age_face(im, &fo[i].face);
          fo[i].gender = add_agegender(fo[i].genders, gender);
          fo[i].age = add_agegender(fo[i].ages, age);
        }
      }
    }
    for (i=0; i<nfo; ++i) {
      if (fo[i].nodetectcnt>5) {
        fo[i].event = -1;
      }
    }
#undef BUFLEN
  }
  //FREE(ap->buf);
  IM_END(im1);
  return nfo;
}

#endif // _FACETRACK_INL_
