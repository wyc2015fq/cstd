
#include "cstd.h"

typedef struct dicom_t {
  int unused;
} dicom_t;

enum IMAGEFORMAT {
  IF_UNKNOWN,
  IF_BMP = 1,
  IF_GIF = 2,
  IF_JPG = 3,
  IF_PNG = 4,
  IF_ICO,
  IF_TIF,
  IF_TGA,
  IF_PCX,
  IF_WBMP,
  IF_WMF,
  IF_JP2,
  IF_JPC,
  IF_PGX,
  IF_PNM,
  IF_RAS,
  IF_JBG,
  IF_MNG,
  IF_SKA,
  IF_RAW,
  IF_DICOM
};
struct POSITIONINGLINEDATA {
  IPOINT pt1;
  IPOINT pt2;
  str_t strSerialUID; //序列UID
  str_t strSerialInstanceNum; //序列号
  str_t strInstanceNum; //影像号
  str_t strSOPInstanceUID;
};
struct Equation { //二维直线一般方程:ax+by+c=0;
  double a;
  double b;
  double c;
};

struct PLANE { // 平面的解析方程 a*x+b*y+c*z+d=0 为统一表示，约定 a >= 0
  double a;
  double b;
  double c;
  double d;
};

struct Point3D { //空间点
  double x;
  double y;
  double z;
};

#define DICOMINFO_STR_DEF_DEF(DICOMINFO_STR_DEF) \
  DICOMINFO_STR_DEF(Modality, 图像类型) \
  DICOMINFO_STR_DEF(PatientsName, 病人姓名) \
  DICOMINFO_STR_DEF(PatientsAge, 病人年龄) \
  DICOMINFO_STR_DEF(PatientID, 病人ID) \
  DICOMINFO_STR_DEF(PatientsBirthDate, 病人出生日期) \
  DICOMINFO_STR_DEF(Manufacturer, 制造商) \
  DICOMINFO_STR_DEF(InstitutionName, 机构名称) \
  DICOMINFO_STR_DEF(ManufacturersModelName, 型号) \
  DICOMINFO_STR_DEF(GantryDetectorTilt, 设备倾斜度) \
  DICOMINFO_STR_DEF(ContentTime, 图像获得时间) \
  DICOMINFO_STR_DEF(BodyPartExamined, 检查部位) \
  DICOMINFO_STR_DEF(StudyTime, 检查时间) \
  DICOMINFO_STR_DEF(StudyDate, 检查日期) \
  DICOMINFO_STR_DEF(KVP, 电压) \
  DICOMINFO_STR_DEF(XRayTubeCurrent, 电流) \
  DICOMINFO_STR_DEF(SliceThickness, 层厚 ) \
  DICOMINFO_STR_DEF(SliceLocation, 切片位置) \
  DICOMINFO_STR_DEF(StudyInstanceUID, 检查UID) \
  DICOMINFO_STR_DEF(StudyID, 检查ID) \
  DICOMINFO_STR_DEF(SeriesInstanceUID, 序列UDI) \
  DICOMINFO_STR_DEF(SeriesNumber, 序列编号) \
  DICOMINFO_STR_DEF(InstanceNumber, 图像编号) \
  DICOMINFO_STR_DEF(ExposureTime, 曝光时间) \
  DICOMINFO_STR_DEF(PerformingPhysiciansName, 检查医生) \
  DICOMINFO_STR_DEF(InstitutionalDepartmentName, 送检科室) \
  DICOMINFO_STR_DEF(SOPInstanceUID, SOPInstanceUID) \
  DICOMINFO_STR_DEF(FrameOfReferenceUID, FrameOfReferenceUID) \
  DICOMINFO_STR_DEF(AccessionNumber, 检查流水号) \
  DICOMINFO_STR_DEF(ImageType, 图片类型) \
   

struct DICOMINFO {
  //#define DICOMINFO_STR_DEF(name, nameinfo)  str_t str ## name[1];
#define DICOMINFO_STR_DEF(name, nameinfo)  char str ## name[32];
#define DICOMINFO_FLOAT_DEF(name, nameinfo)  double d ## name[32];
  DICOMINFO_STR_DEF_DEF(DICOMINFO_STR_DEF)
#undef DICOMINFO_STR_DEF

  //
  Point3D ImagePosition; //原点
  Point3D ImageOrientationX; //X方向余弦
  Point3D ImageOrientationY; //Y方向余弦
  double dPixelSpacingX; //X
  double dPixelSpacingY; //Y 两像素间的距离
  BOOL bIsPositioningImage;//是否是定位图
  int iAllFrame; //图像帧数
  char strFilePath[MAX_PATH]; //图像路径
  int WinWidth; //
  int WinCenter;
};
#define WND_BREAKE 2
#define ST_FIT 0 //适合窗口
#define ST_NORMAL 1 //实际大小
#define ST_ZOOM_OUT 2 //放大
#define ST_ZOOM_IN 3 //缩小