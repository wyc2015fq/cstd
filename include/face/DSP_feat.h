#ifndef _DSP_FEAT_H_
#define _DSP_FEAT_H_
#define ID_LEN 32
//#define FEATURE_LEN   ((FEAT_SIZE2*sizeof( FEAT_T)+3)&~3)      //单个人脸特征大小
#define FEATURE_LEN   30000
#define FEATURE_LEN1 (FEAT_SIZE2+FEAT_SIZE3)*sizeof(FEAT_T)      //单个人脸特征大小
//#define FEATURE_LEN 60000*sizeof(FEAT_T)      //单个人脸特征大小
#define SPFEAT_LEN  1              //单个用户特征样本数目(15基本样本＋5更新样本)
//#define REGIST_NUM  1              //单个用户人脸注册时采集的样本数目

enum { FLAG_BASE = 0, FALG_UPDATE, FLAG_NULL = 255 };

//ID标识结构体
typedef struct {
  char m_ID[ ID_LEN ];
}CID;

//单个特征
typedef struct {
  FEAT_T m_Feature[ FEATURE_LEN ];  //特征数据
  uint arg[4];
  CID m_PID;                     //对应ID号
  int m_Flag;                 //更新或则基础,FLAG_BASE,FALG_UPDATE,FLAG_NULL 
  unsigned int hashid;
  time_t time;
}SPFeatInfo;

#define STDH 200
#define STDW 150
#define STDCN 3
#define STDSTEP (STDW*STDCN)
typedef struct {
  unsigned char m_data[STDH*STDW*STDCN];      //注册照片
  CID        m_PID;                     //对应ID号
  unsigned int m_Flag;
}SPFaceRegInfo;

// 识别记录
typedef struct {
  unsigned char m_data[STDH*STDW*STDCN]; //注册照片
  CID        m_PID;                      //对应ID号
  double score; // 得分
  time_t time;
}SPFaceRecInfo;

//单人特征集合
#define SPInfoID(SP, _I)  ((SP)->m_PFeatArry[_I].m_PID.m_ID)
typedef struct {
  SPFeatInfo m_PFeatArry[ SPFEAT_LEN ];
}SPInfo;
static int CID_set(char* m_ID, const char* name1) {
  if (name1) {
    int len1 = strlen(name1);
    if (len1>0 && len1<ID_LEN) {
      memset(m_ID, 0, ID_LEN);
      strcpy(m_ID, name1);
      return 1;
    }
  }
  return 0;
}
#endif // _DSP_FEAT_H_
