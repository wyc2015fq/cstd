
#include "face/face_recog_mojin.inl"
#include "cap.h"

// Ä§¾µ
int test_face_recog_mojin() {
  int high = 0;
  capdev cap[1] = {0};

#ifdef _WIN32
  //sys_chdir("E:/code/cstd/include/facelib/MTCNN");
#endif
  if (capdshow_open(cap, 0, 640, 480) > 0) {
    int fram = 0;
    char ch = 0;
    img_t im[maxfeatnum+1] = {0};
    int runing = 0;
    int i, j;
    int featnum = 10;
    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      //printf("%d w=%d h=%d\n", fram, im->w, im->h);
      if (runing>0) {
        printf("%d\n", runing);
        imclone2(im, im+runing);
        --runing;
        if (0==runing) {
          int ret=0, id=-1, isnew=0;
          tic;
          ret = face_recog_mojin(im+1, featnum, &id, &isnew);
          toc_;
          const char* retstr = face_recog_falg_to_str(ret);
          printf("%s id = %d, isnew=%d\n", retstr, id, isnew);
          if (ret==FACE_RECOG_FALG_OK) {
            char buf[256];
            _snprintf(buf, 256, "feat/%d", id);
            sys_mkdir(buf);
            for (i=0; i<featnum; ++i) {
              _snprintf(buf, 256, "feat/%d/%d.jpg", id, i);
              imwrite(buf, im+i+1);
            }
          }
        }
      }
      imshow_(im);
      ch = WaitKey(200);
      ch = tolower(ch);
      if ('r'==ch) {
        runing=featnum;
      }
    }
    
    cap_close(cap);
    imfrees2(im);
  }

  return 0;
}
