
#include "cap.h"

int edit_text(char* text, int text_size, char ch) {
  int l = strlen(text);
  if (isalphnmb(ch)) {
    if (l<text_size-1) {
      text[l++] = ch;
      text[l++] = 0;
    }
  } else if (8==ch) {
    if (l>0) {
      text[--l] = 0;
    }
  }
  return 0;
}

#include "facelib/facedetect/facedetect.inl"

struct userinfo {
  char name[32];
  float feat[2048];
};

int userinfo_find(buf_t* bf, const char* name) {
  int len = bf->len/sizeof(userinfo);
  userinfo* info = (userinfo*)(bf->data);
  int i;
  for (i=0; i<len; ++i) {
    if (0==stricmp(name, info[i].name)) {return i;}
  }
  return -1;
}
int userinfo_add(buf_t* bf, userinfo* ui) {
  int len = bf->len/sizeof(userinfo);
  bfsetsize(bf, (len+1)*sizeof(userinfo));
  userinfo* info = (userinfo*)(bf->data);
  info[len] = *ui;
  return 0;
}

float CalcSimilarity(const float* fc1, const float* fc2, long dim);

int userinfo_recog(buf_t* bf, const float* feat, float* pmaxprod) {
  int len = bf->len/sizeof(userinfo);
  userinfo* info = (userinfo*)(bf->data);
  int i, j=-1;
  float maxprod = 0;
  for (i=0; i<len; ++i) {
    float prod = CalcSimilarity(info[i].feat, feat, 2048);
    if (prod>maxprod) {
      maxprod = prod;
      j = i;
    }
  }
  if (pmaxprod) {
    *pmaxprod = maxprod;
  }
  return j;
}

int test_face_demo() {
const char* WINDOW_NAME = "CVUI Hello World!";
	int count = 0;
  img_t im[10] = {0};
  img_t* frame = im+1;
  capdev cap[1] = {0};
  if (capdshow_open(cap, 0, 640, 480) <= 0) {
    return 0;
  }
  cvSetMat(frame, 480, 640+200, CC_8U, 3);

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui_init(WINDOW_NAME);
  bool is_reg = false;
  const char* fn = "face.dat";
  buf_t facelib[1] = {0};
  buf_load(fn, facelib);
  userinfo cur[1] = {0};
  char errmsg[256] = {0};

int ExtractFaceFeature(int cols, int rows, unsigned char* data, int c, const float* points, float* gallery_fea);

	while (true) {
    cap_getframe(cap, im, CAP_RGB);
		// Fill the frame with a nice color
		cvSet(frame, cScalar(49, 52, 49));
    cvui_image(frame, 0, 0, im);
    int i;
    struct FaceSet* faceset = facedetect_static(im->h, im->w, im->data, im->s, im->c, 80);
    if (faceset && faceset->nfaces>0) {
      for (i=0; i<faceset->nfaces; ++i) {
        struct Face* face = faceset->faces + i;
        cvui_rect(frame, face->rect.x, face->rect.y, face->rect.w, face->rect.h, 0xff0000);
        //cvui_printf(frame, 640+10, 90+i*40, 0.4, 0xff0000, "%d age: %d", i, face->age);
        //cvui_printf(frame, 640+10, 90+i*40, 0.4, 0xff0000, "%d gender: %s", i, gender_tag[!!face->gender]);
        float points[10] = {
            face->leye_center.x,
            face->reye_center.x,
            face->nose_tip.x,
            face->mouth_l_corner.x,
            face->mouth_r_corner.x,

			face->leye_center.y,
			face->reye_center.y,
			face->nose_tip.y,
			face->mouth_l_corner.y,
			face->mouth_r_corner.y,
        };
        ExtractFaceFeature(im->w, im->h, im->data, im->c, points, cur->feat);
        if (is_reg) {
          if (*cur->name) {
            if (userinfo_find(facelib, cur->name)<0) {
              userinfo_add(facelib, cur);
              buf_save(fn, facelib);
            } else {
              snprintf(errmsg, 256, "err: name is NULL!!");
            }
            is_reg = false;
          }
        } else if (facelib->data) {
          float maxprod = 0;
          int id = userinfo_recog(facelib, cur->feat, &maxprod);
          if (id>=0) {
            userinfo* info = (userinfo*)(facelib->data);
            cvui_printf(frame, face->rect.x, face->rect.y, 1, maxprod<0.8 ? 0xff0000 : 0x00ff00, "%s(%.3f)", info[id].name, maxprod);
          }
        }
      }
    }
		// Buttons will return true if they were clicked, which makes
		// handling clicks a breeze.
		if (cvui_button(frame, 640+10, 20, "reg")) {
			// The button was clicked, so let's increment our counter.
			count++;
      is_reg = true;
		}

		// Sometimes you want to show text that is not that simple, e.g. strings + numbers.
		// You can use cvui_printf for that. It accepts a variable number of parameter, pretty
		// much like printf does.
		// Let's show how many times the button has been clicked.
		//cvui_printf(frame, 640+10, 50, 0.4, 0xff0000, "Button click count: %d", count);
		cvui_printf(frame, 640+10, 50, 0.4, 0xffffff, "name: %s", cur->name);
		cvui_printf(frame, 640+10, 90, 0.4, 0xff0000, "%s", errmsg);
		//cvui_text(frame, 640+10, 50, input, 0.5, 0xff0000);

		// Update cvui stuff and show everything on the screen
		cvui_imshow(WINDOW_NAME, frame);

		// Check if ESC key was pressed
    char ch = waitKey(20);
    edit_text(cur->name, 32, ch);
		if (ch == 27) {
			break;
		}
	}
  imfrees2(im);
  return 0;
}
