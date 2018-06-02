
#include "cstd.h"
//#include "cap.h"
#include "RSFaceSDK.h"
#include "img/imgio.inl"
#include "cv/drawing.inl"
//#include "facelib/facedetect/facedetect.inl"

//#pragma comment(lib,"libReadFace.lib")
#pragma comment(lib,"libReadFace-win64.lib")

// #define USE_DETECT

const int UNKNOWN_PERSON_ID = -111;

#define COLOR_BLUE   _RGBA(255, 102, 0, 255)   ///< Definition of the blue color
#define COLOR_ORANGE _RGBA(51, 153, 255, 255)  ///< Definition of the green color
#define COLOR_GREEN  _RGBA(204, 204, 0, 255)   ///< Definition of the red color
#define COLOR_RED    _RGBA(153, 102, 255, 255) ///< Definition of the red color

COLOR colors[] = { COLOR_BLUE, COLOR_ORANGE, COLOR_GREEN, COLOR_RED };

const int five_points[] = { 7, 10, 12, 16, 18 };

const char* age_tag[] = {
    "region1_5",
    "region6_10",
    "region11_15",
    "region16_20",
    "region21_25",
    "region26_30",
    "region31_35",
    "region36_40",
    "region41_45",
    "region46_50",
    "region51_55",
    "region56_60",
    "region61_65",
    "region66_70",
    "region71_75",
    "region76_80",
};
const char* gender_tag[] = {
  "Female",
  "Male",
};
int str_to_label(const char* str, const char** strs, int len, int delims) {
  int i;
  for (i=0; i<len; ++i) {
    const char* p = str;
    const char const * s = strs[i];
    int l = strlen(s);
    for (;*p;++p) {
      if (delims==*p && delims==p[l+1] && 0==_memicmp(p+1, s, l)) {
        return i;
      }
    }
  }
    printf("error : %s unkown label\n", str);
  if (1) {
    int asdf=0;
  }
  return -1;
}
double fsqr(double x) {
  return x*x;
}
int test_ReadFace2(int argc, char **argv)
{
	int h = 720, w = 1280;

	//RSHandle lm;
	RSHandle hFaceDetect;
	RSHandle hFaceTrack;
	//RSHandle hFaceRecognition;
	//RSHandle hLivenessDetect;
	RSHandle hLicense = NULL;
	RSHandle hAttributeDetect;
    rs_face_attr pAttrVal;

    const char* appId_face = "291e0e83138767cfbeadac20c2148ee4";//yourAppidFromReadSense
	const char* appSecrect = "718b6abf62113aaf80bbcfdfc417dd8225ecf6c2";

	int result = rsInitLicenseManager(&hLicense,appId_face, appSecrect);//,
		rs_face *pFaceArray = NULL;
		int faceCount = 0;

	if (hLicense == NULL) {
		printf("license error\n");
		return -1;
	}

	rsInitFaceDetect(&hFaceDetect, hLicense);
	if (!hFaceDetect) {
		printf("init error\n");
		return -1;
	}
#if 0
	rsInitFaceTrack(&hFaceTrack, hLicense);
	if (!hFaceTrack) {
		printf("init error");
		return -1;
	}
#endif
	rsInitAttributeDetect(&hAttributeDetect, hLicense);
	if (!hAttributeDetect) {
		printf("init hAttributeDetect error\n");
		return -1;
	}

  if (1) {
    const char* list_file = "list.txt";
    sys_chdir("D:/code/cnn/gender");
    vstr_t sv[1] = {0};
    int i;
    img_t im[1] = {0};
    DataSize size;
    int delims = '\\';
    vstr_load(list_file, sv);
    if (0) {
      for (i=0; i<sv->n; ++i) {
        int gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
        //int age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
      }
    }
	// A 11302 / 15636 / 15828  G: 72.3 FDR: 98.8 25.4 EM: 6.2 EAM: 15.5 EV: 302.8 1.4
    size = dataSize(sv->n, 3, 64, 64);
	int acc = 0, detcnt = 0;
	int accage = 0, cnt=0;
    float age_err_sum=0;
	float age_err_abssum = 0;
	float age_err_sqsum = 0;
	buf_t bf[1] = { 0 };
	double tage = 0;
	bfsetsize(bf, 100 * (1 << 20));
    //fwrite(&size, sizeof(size), 1, pout);
    for (i= sv->n*0.9; i<sv->n; ++i) {
      int gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
      int age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
      float age = age_id*5. + 3;
	  ++cnt;
      if (gender_id>=0 && imread(sv->v[i].s, size.c, 1, im)) {
        img_t* frame = im;
#if 0
        if (0) {
          struct FaceSet* faceset = NULL;
          face_cnt = 0;
          faceset = facedetect(bf->data, bf->len, im->h, im->w, im->data, im->s, im->c, 30);
          //detcnt += (faceset->nfaces>0);
          detcnt += (face_cnt>0);
          printf("acc %d / %d / %d  det_rate: %3.1f  acc_rate: %3.1f\n", acc, detcnt, i+1, detcnt*100./(i+1), acc*100./detcnt);
        }
#endif
        
        if (1) {
          int result = rsRunFaceDetect(hFaceDetect, frame->data,
            PIX_FORMAT_BGR888,
            frame->cols,
            frame->rows,
            frame->step,
            RS_IMG_CLOCKWISE_ROTATE_0,
            &pFaceArray,
            &faceCount);
          
          if (result == 0 && faceCount > 0) {
            /*printf("Face " << faceCount << endl;*/
            //faceCount = 1;
            //for ( int j = 0; j < faceCount;j++)
            {
              rs_face mFace = pFaceArray[0];
              cvRectangle2(frame, cRect(mFace.rect.left, mFace.rect.top, mFace.rect.width, mFace.rect.height), cScalar(0, 0, 255), 1, 8, 0);
              /*printf(mFace.rect.left << " " << mFace.rect.top<< " "<<mFace.rect.width<< " "<<mFace.rect.height<< " " << endl;*/
              
              /*printf(mFace.landmarks21[0].x << " " << mFace.landmarks21[0].y << " " << mFace.landmarks21[1].x << " " << mFace.landmarks21[1].y << endl;*/

			  {utime_start(_start_time);
              rsRunAttributeDetect(hAttributeDetect,frame->data,PIX_FORMAT_BGR888,frame->cols,frame->rows,frame->step,&mFace,&pAttrVal);
			  tage += utime_elapsed(_start_time); }
              acc += gender_id==pAttrVal.gender;
			  double age1 = pAttrVal.age + 6.2;
			  accage += fabs(age - age1)<10;
              age_err_sum += (age - age1);
              age_err_abssum += fabs(age - age1);
              age_err_sqsum += fsqr(age - age1);
              ++detcnt;
              if (0) {
                CvFont font[1];
                cvInitFont(font, CC_FONT_HERSHEY_DUPLEX, 0.75, 0.75);
                char buf[256];
                _snprintf(buf, 256, "Gender:%d", pAttrVal.gender);
                cvPutText(frame, buf,cPoint(mFace.rect.left, mFace.rect.top - 25), font, cScalar(0, 255, 255, 255));
                _snprintf(buf, 256, "Age:%d", pAttrVal.age);
                cvPutText(frame, buf, cPoint(mFace.rect.left, mFace.rect.top), font, cScalar(0, 255, 255, 255));
              }
            }
          }
          //printf("acc %d / %d / %d  det_rate: %3.1f  acc_rate: %3.1f\n", acc, detcnt, i+1, detcnt*100./(i+1), acc*100./detcnt);
          //printf("acc %d / %d / %d  det_rate: %3.1f  %3.1f err_mean: %3.1f err_absmean: %3.1f err_var: %3.1f\n", acc, detcnt, i+1, detcnt*100./(i+1), accage*1./ detcnt, age_err_sum/detcnt, age_err_abssum/detcnt, age_err_sqsum/detcnt);
		  printf("A %d / %d / %d  G: %3.1f FDR: %3.1f %3.1f EM: %3.1f EAM: %3.1f EV: %3.1f %3.1f\n",
			  acc, detcnt, cnt, acc*100. / detcnt, detcnt*100. / (cnt), accage*100. / detcnt,
			  age_err_sum / detcnt, age_err_abssum / detcnt, age_err_sqsum / detcnt, tage*1000. / detcnt);
        }
      }
    }
    imfree(im);
    vstr_free(sv);
  }
#if 0
  capdev cap[1] = { 0 };
  if (capdshow_open(cap, 0, w, h) <= 0) {
	  return 0;
  }

	img_t im[10] = {0};
	int ch=0, fram=0;
	for (; 'q' != ch && 'Q' != ch; ++fram) {
		img_t* frame = im;
		cap_getframe(cap, frame, CAP_RGB);
	/*	frame = imread("D:\\test_pic\\test_1127\\image1.jpeg");*/

	int result = rsRunFaceDetect(hFaceDetect, frame->data,
			PIX_FORMAT_BGR888,
			frame->cols,
			frame->rows,
			frame->step,
			RS_IMG_CLOCKWISE_ROTATE_0,
			&pFaceArray,
			&faceCount);

	//printf("---------------------------------------------------------" << endl;

		if (result == 0 && faceCount > 0) {
		
			/*printf("Face " << faceCount << endl;*/
			
			for ( int i = 0; i < faceCount;i++)
			{
				rs_face mFace = pFaceArray[i];
				cvRectangle2(frame, cRect(mFace.rect.left, mFace.rect.top, mFace.rect.width, mFace.rect.height), cScalar(0, 0, 255), 1, 8, 0);
				/*printf(mFace.rect.left << " " << mFace.rect.top<< " "<<mFace.rect.width<< " "<<mFace.rect.height<< " " << endl;*/
			
				/*printf(mFace.landmarks21[0].x << " " << mFace.landmarks21[0].y << " " << mFace.landmarks21[1].x << " " << mFace.landmarks21[1].y << endl;*/
				rsRunAttributeDetect(hAttributeDetect,frame->data,PIX_FORMAT_BGR888,frame->cols,frame->rows,frame->step,&mFace,&pAttrVal);
				CvFont font[1];
				cvInitFont(font, CC_FONT_HERSHEY_DUPLEX, 0.75, 0.75);
				char buf[256];
				_snprintf(buf, 256, "Gender:%d", pAttrVal.gender);
				cvPutText(frame, buf,cPoint(mFace.rect.left, mFace.rect.top - 25), font, cScalar(0, 255, 255, 255));
				_snprintf(buf, 256, "Age:%d", pAttrVal.age);
				cvPutText(frame, buf, cPoint(mFace.rect.left, mFace.rect.top), font, cScalar(0, 255, 255, 255));
			}
		}
	

		imshow_(frame);
		int key = WaitKey(20);
		if (27 == key) { break; }
	}
#endif


	////////////////////////////
	// release resources
	////////////////////////////

#ifdef USE_DETECT
	rsUnInitFaceDetect(&hFaceDetect);
#else
	rsUnInitFaceTrack(&hFaceTrack);
#endif
	rsUnInitAttributeDetect(&hAttributeDetect);
	//rsUnInitLivenessDetect(&hLivenessDetect);
	//rsUnInitFaceRecognition(&hFaceRecognition);
	rsUnInitLicenseManager(&hLicense);
	return 0;
}

