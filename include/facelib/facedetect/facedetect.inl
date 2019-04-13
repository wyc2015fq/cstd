
#ifdef TEST
//#include "cv/imgproc/drawing.inl"
#endif

#define tic
#define toc

#include "facedetect.h"
#include "dnn/cnn/mtcnn.h"
#include "dnn/cnn/agegender.h"

//面积计算函数
static double tri_area(float x1, float y1, float x2, float y2, float x3, float y3)
{
	double tarea;
	tarea = 0.5*(x1*y2 + x2*y3 + x3*y1 - x1*y3 - x2*y1 - x3*y2);//此处可以将上述的marray[i].mx/my直接带入，但是为了简洁，不带入
	return fabs(tarea);//取绝对值
}

double ry_ptdist(FPOINT a, FPOINT b) {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrt(dx*dx+dy+dy);
}
// 二维空间点到直线的垂足  
static double GetFootOfPerpendicular(FPOINT pt,     // 直线外一点  
	FPOINT begin,  // 直线开始点  
	FPOINT end)   // 直线结束点  
{
	FPOINT retVal;
	double u;
	double dx = begin.x - end.x;
	double dy = begin.y - end.y;
	if (fabs(dx) < 0.00000001 && fabs(dy) < 0.00000001)
	{
		retVal = begin;
	}
	else {
		u = (pt.x - begin.x)*(begin.x - end.x) + (pt.y - begin.y)*(begin.y - end.y);
		u = u / ((dx*dx) + (dy*dy));

		retVal.x = begin.x + u*dx;
		retVal.y = begin.y + u*dy;
	}
	 double ret = ry_ptdist(retVal, pt);
	 return ret;
}
float get_face_side1(const FPOINT* p) {
	double a1 = tri_area(p[0].x, p[0].y, p[2].x, p[2].y, p[3].x, p[3].x);
	double a2 = tri_area(p[1].x, p[1].y, p[2].x, p[2].y, p[4].x, p[4].x);
	a1 = GetFootOfPerpendicular(p[2], p[0], p[3]);
	a2 = GetFootOfPerpendicular(p[2], p[1], p[4]);
	double k = MIN(a1, a2)/MAX(a1, a2);
	//printf("%f %f\n", a1, a2);
	return k;
}
float get_face_side(const Face* fr) {
  FPOINT p0 = fr->leye_center;
  FPOINT p1 = fr->reye_center;
  FPOINT p2 = fr->nose_tip;
  FPOINT p3 = fr->mouth_l_corner;
  FPOINT p4 = fr->mouth_r_corner;
	double a1 = tri_area(p0.x, p0.y, p2.x, p2.y, p3.x, p3.x);
	double a2 = tri_area(p1.x, p1.y, p2.x, p2.y, p4.x, p4.x);
	a1 = GetFootOfPerpendicular(p2, p0, p3);
	a2 = GetFootOfPerpendicular(p2, p1, p4);
	double k = MIN(a1, a2)/MAX(a1, a2);
	//printf("%f %f\n", a1, a2);
	return k;
}
static struct FaceSet* bf_facedetect(buf_t* bf, const img_t* image, int minsize) {
  struct FaceSet* faceset = NULL;
  Bbox box[400];
  int i, box_size = countof(box);
  utime_start(_start_time);
  BF_CALLOC(faceset, 1);
  if (NULL==faceset) {
    return NULL;
  }
  faceset->nfaces = mtcnn_findFace(image, box, box_size, minsize);
  faceset->time_used = 1000*utime_elapsed(_start_time);
  BF_CALLOC(faceset->faces, faceset->nfaces);
  if (NULL==faceset->faces) {
    return NULL;
  }
  for (i=0; i<faceset->nfaces; ++i) {
    struct Face* face = faceset->faces + i;
    face->rect.x = box[i].y1;
    face->rect.y = box[i].x1;
    face->rect.width = box[i].y2-box[i].y1;
    face->rect.height = box[i].x2-box[i].x1;
    {
      float eav[3] = {0};
      EstimateHeadPose(box[i].ppoint, eav);
      face->headpose.pitch_angle = eav[0];
      face->headpose.yaw_angle   = eav[1];
      face->headpose.roll_angle  = eav[2];
    }
    face->gender = -1;
    face->age = -1;
    {
      face->gender = face_gender_face(image, face);
      face->age = face_age_face(image, face);
    }
    {
      float* x = box[i].ppoint;
      float* y = box[i].ppoint+5;
#define SETLANDMARK(I, PT)  face-> PT .x = x[I], face-> PT.y = y[I]
      SETLANDMARK(0, leye_center);
      SETLANDMARK(1, reye_center);
      SETLANDMARK(2, nose_tip);
      SETLANDMARK(3, mouth_l_corner);
      SETLANDMARK(4, mouth_r_corner);
#undef SETLANDMARK
    }
    face->sidek = get_face_side(face);
  }
  return faceset;
}

struct FaceSet* facedetect(void* buf, int buflen, int h, int w, const unsigned char* rgb_data, int data_step, int channels, int minsize) {
  buf_t bf[1] = {0};
  img_t im[1] = {0};
  bfinit(bf, buf, buflen);
  IMINIT(im, h, w, rgb_data, data_step, channels, 1);
  return bf_facedetect(bf, im, minsize);
}

static struct FaceSet* facedetect_static(int h, int w, const unsigned char* rgb_data, int data_step, int channels, int minsize) {
  static buf_t bf[1] = {0};
  img_t im[1] = {0};
  if (NULL==bf->data) {
    bfsetsize(bf, (20<<20));
  }
  IMINIT(im, h, w, rgb_data, data_step, channels, 1);
  return bf_facedetect(bf, im, minsize);
}
#ifdef TEST
#include "cv/imgproc/drawing.inl"

static int drawFace(img_t* im, const struct Face* face) {
  //printf("(%3d %3d %3d %3d)\n", (int)face->rect.left, (int)face->rect.top, (int)face->rect.width, (int)face->rect.height);
  char buf[256];
  _snprintf(buf, 256, "%d", (int)face->age);
  CScalar color = face->gender ? cScalar(255, 0, 0, 0) : cScalar(0, 0, 255, 0);
  putText(im, buf, -1, iPOINT(face->rect.x, face->rect.y), CC_FONT_HERSHEY_SIMPLEX, 1, color);
  cvRectangle(im, cPoint(face->rect.x, face->rect.y),
    cPoint(face->rect.x+face->rect.width, face->rect.y+face->rect.height), color, 2, 8, 0);
  
  cvCircle(im, cPoint(face->leye_center.x, face->leye_center.y), 3, cScalar(0, 255, 255, 0), -1, 8, 0);
  cvCircle(im, cPoint(face->reye_center.x, face->reye_center.y), 3, cScalar(0, 255, 255, 0), -1, 8, 0);
  cvCircle(im, cPoint(face->nose_tip.x, face->nose_tip.y), 3, cScalar(0, 255, 255, 0), -1, 8, 0);
  cvCircle(im, cPoint(face->mouth_l_corner.x, face->mouth_l_corner.y), 3, cScalar(0, 255, 255, 0), -1, 8, 0);
  cvCircle(im, cPoint(face->mouth_r_corner.x, face->mouth_r_corner.y), 3, cScalar(0, 255, 255, 0), -1, 8, 0);
  return 0;
}

//#include "pic/pic.inl"
//#include "img/color.inl" innerProduct
#include "img/imgio.inl"
#include "cap.h"
int test_facedetect() {
  buf_t bf[1] = {0};
  img_t im[1] = {0};
  int i, j;
  struct FaceSet* faceset = NULL;
  bfsetsize(bf, 100*(1<<20));
  if (1) {
#ifdef _PIC_INL_
    imsetsize(im, 128, 128, 1, 1);
    memcpy(im->tt.data, lena_128x128, 128*128);
    im2rgb(im, im, 3);
#else
    const char* filename;
    filename = "F:/dataset/facedetect/group/00ce619c165effe5e64a02e975101b68.jpg";
    filename = "F:/dataset/facedetect/hztest.jpg";
    imread(filename, 3, 1, im);
#endif // _PIC_INL_
    
    //faceset = bf_facedetect(bf, im, 60);
    faceset = facedetect(bf->data, bf->len, im->h, im->w, im->data, im->s, im->c, 60);
    printf("time_used = %f ms\n", faceset->time_used);
    if (1) {
      for (i = 0; i < faceset->nfaces; ++i) {
        drawFace(im, faceset->faces + i);
      }
    }
    imshow_(im);WaitKey(-1);
  }
  if (0) {
    dirlist_t dl[1] = {0};
    char ch=0;
    dirlist(dl, "F:/dataset/facedetect/group", ".jpg", FF_FULLNAME);
    for (j=73; ch !='q' && j<dl->n; ++j) {
      if (imread(dl->v[j].name, 4, 1, im)) {
        //printf("%d %s ", j, dl->v[j].name);
        faceset = facedetect(bf->data, bf->len, im->h, im->w, im->data, im->s, im->c, 60);
        printf("%d %d %f ms\n", j, faceset->nfaces, faceset->time_used);
        if (1) {
          for (i = 0; i < faceset->nfaces; ++i) {
            drawFace(im, faceset->faces + i);
          }
          imshow_(im);
          ch = WaitKey(-1);
          ch = tolower(ch);
        }
      }
    }
  }
  if (1) {
    capdev cap[1] = { 0 };
    int h = 720, w = 1280;
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
      int i;
      for (; 'q' != ch && 'Q' != ch; ++fram) {
        cap_getframe(cap, im, CAP_RGB);
        faceset = facedetect(bf->data, bf->len, im->h, im->w, im->data, im->s, im->c, 60);
        printf("%d %d %f ms\n", fram, faceset->nfaces, faceset->time_used);
        if (1) {
          for (i = 0; i < faceset->nfaces; ++i) {
            drawFace(im, faceset->faces + i);
          }
        }
        imshow_(im);
        ch = WaitKey(-1);
        ch = tolower(ch);
      }
    }
  }
  
  bffree(bf);
  imfree(im);
  return 0;
}

#endif // TEST