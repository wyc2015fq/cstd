

#ifndef _FACEDETECT_H_
#define _FACEDETECT_H_

#ifndef __CRECTF_DEFINED__
#define __CRECTF_DEFINED__
typedef struct CRectf {
  float x;
  float y;
  union {float w; float width;};
  union {float h; float height;};
} CRectf;
#endif

#ifndef __FPOINT_DEFINED__
#define __FPOINT_DEFINED__
typedef struct {
  float x, y;
} FPOINT, FPOINT2;
#endif // __FPOINT_DEFINED__

// doc: https://console.faceplusplus.com.cn/documents/4888373
struct Face {
  CRectf rect;
  struct {
    float sadness,
      neutral,
      disgust,
      anger,
      surprise,
      fear,
      happiness;
  } emotion;
  int gender;
  int age;
  float sidek;
  // Landmark
  FPOINT nose_tip,
    leye_center,
    reye_center,
    mouth_l_corner,
    mouth_r_corner;
  struct {
    float yaw_angle,
      pitch_angle,
      roll_angle;
  } headpose;
#if 0
  struct {
    struct {
      float normal_glass_eye_open,
        no_glass_eye_close,
        occlusion,
        no_glass_eye_open,
        normal_glass_eye_close,
        dark_glasses;
    } left, right;
  } eyestatus;
  float glass;
  struct {
    struct {
      float threshold,
        value;
    } blurness,
      motionblur,
      gaussianblur;
  } blur;
  struct {
    float threshold,
      value;
  } smile,
    facequality;
  struct {
    float white;
  } ethnicity;
#endif
};

struct FaceSet {
  float time_used;
  int nfaces;
  struct Face *faces;
};

struct FaceSet* facedetect(void* buf, int buflen, int h, int w, const unsigned char* rgb_data, int data_step, int channels, int minsize);

#define MAX_FACE_LANDMARK  108
#define MAX_COUNT_AGEGENDER  20
typedef struct faceobj_t {
  unsigned int id;
  //CRectf rect;
  struct Face face;// , leye, reye;
  //IPOINT leye, reye;
  int inited;
  int genders[MAX_COUNT_AGEGENDER];
  int ages[MAX_COUNT_AGEGENDER];
  int gender;
  int age;
  int glass;
  int facequality;
  int blur;
  
  int face_code;
  int enter_time;
  int left_time;
  //int face_no;
  int sideface;
  int smile;
  
  //FPOINT landmark[MAX_FACE_LANDMARK];
  int nodetectcnt;
  int event; // 0 表示没有事件  1表示增加 2表示获取了年龄等信息 -1表示消失
  buf_t image[1];
} faceobj_t;

#endif // _FACEDETECT_H_
