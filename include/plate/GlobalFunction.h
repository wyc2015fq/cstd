
#define GrayScale 255
#define S(image,x,y) ((uchar*)(image->tt.data + image->s*(y)))[(x)]   //S


#define S(image,x,y)   ((uchar*)(image->tt.data + image->s*(y)))[(x)]   //S
#define F(image,x,y,z) ((uchar*)(image->tt.data + image->s*(y)))[(x)*image->c+(z)]    //S
#define I(image,x,y)   ((float*)(image->tt.data + image->s*(y)))[(x)]   //S
//#define SS(image,x,y) ((short*)(image->tt.data + image->s*(y)))[(x)]    //S

#define C_NUM 6
#define MIN_I 9999999

#define GrayScale 255



#define MAXCOUNT  10000

typedef struct m_Rect {
  int x;
  int y;
  int w;
  int h;
  int count;
  int flag;
} m_Rect;

typedef struct runpass_t {
  m_Rect run[MAXCOUNT];
  m_Rect lastrun[MAXCOUNT];
  int runlen;
  int runcount;
  int lastruncount;
} runpass_t;

typedef struct plate_segment_t {
  short plateColor;
  CvRect rect[1];
  int L[7];
  int R[7];
  CvRect rect_all;
  int L_R_center[7];
  int min_ver_p;
  int lenth;
  float connected_t;
  float broken_t;
  float height_t;
  int flag;//取值 0 分割步骤失败；1 分割成功；2异常返回（颜色判别失败或精定位失败）
} plate_segment_t;


static struct svm_model* model;
static struct svm_node* x;
static int max_nr_attr = 288;
#define PLSTD_H  24  //图像归一化后的高度
#define PLSTD_W  12   //图像归一化后的宽度
static double predict_result = 0;
const char character[37] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //字符索引
const int prob[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33};
int rectdetect(int height, int width, const unsigned char* data, int imgwidth);


//融合的阈值
#define min_fusion_height  10
#define max_fusion_height  50
#define max_fusion_margin  6
#define min_row_width  60
#define PRE_NUM  5
#define MAX_VALUE(X,Y)   ((X)>(Y)?(X):(Y))
#define MIN_VALUE(X,Y)   ((X)<(Y)?(X):(Y))

#define  Dist(x,y)  ((x)-(y))




