
#include "../../include/cstd.h"

#define CC_BGFG_MOG_UPDATE_RATE (0.2f)
#define CC_BGFG_MOG_UPDATE_THD  (10)

typedef int Real_t;

typedef struct CvAvgBGStatModelParams {
  int     update_thd;
  Real_t  update_rate;
} CvAvgBGStatModelParams;

typedef struct CvAvgBGModel {
  CC_BG_STAT_MODEL_FIELDS();
  CvAvgBGStatModelParams  params;
  int                     countFrames;
  char*                   buf;
  int                     buflen;
}
CvAvgBGModel;

static void CC_CDECL
icvReleaseAvgBGModel(CvAvgBGModel** _bg_model)
{
  CC_FUNCNAME("icvReleaseAvgBGModel");

  __BEGIN__;

  if (*_bg_model) {
    CvAvgBGModel* bg_model = *_bg_model;
    FREE(bg_model);
  }

  __END__;
}

static int icvUpdateAvgBGModel(unsigned char* curr_frame, int curr_frame_step, CvAvgBGModel* bg_model)
{
  int i, j, k;
  int region_count = 0;
  Real_t pixel[ 4 ], sum;
  Real_t thd = bg_model->params.update_thd;
  Real_t aa = bg_model->params.update_rate;
  Real_t bb = SHIFT1 - aa;

  bg_model->countFrames++;

  //cvShowImage( "asdf", bg_model->height, bg_model->width,
  //        bg_model->background, bg_model->width*3, 3 ); cvWaitKey(-1);

  for (i = 0; i < bg_model->height; i++) {
    for (j = 0; j < bg_model->width; j++) {
      const int nChannels = bg_model->nChannels;
      const int n = i * bg_model->width + j;
      const int p = n * bg_model->nChannels;
      sum = 0;

      for (k = 0; k < nChannels; k++) {
        pixel[k] = (bg_model->background[ p + k ] * bb + curr_frame[ p + k ] * aa) >> SHIFT;
        sum += ABSSUB(bg_model->background[ p + k ], pixel[k]);
        bg_model->background[ p + k ] = pixel[k];
      }

      bg_model->foreground[n] = sum > thd ? 255 : 0;
    }
  }

  //cvShowImage( "asdf", bg_model->height, bg_model->width,
  //  bg_model->background, bg_model->width*3, 3 ); cvWaitKey(-1);
  return region_count;
}

CvBGStatModel* cvCreateAvgBGModel_process(int height, int width, unsigned char* first_frame,
    int widthStep, int nChannels, void* parameters1,
    char* buf, int buflen)
{
  CvAvgBGModel* bg_model = 0;
  CvAvgBGStatModelParams* parameters = (CvAvgBGStatModelParams*)parameters1;
  CvAvgBGStatModelParams params;
  int i, j, n, m, p;

  BUFUSEBEGIN(buf, buflen);

  CC_FUNCNAME("cvCreateAvgBGModel");

  __BEGIN__;

  //init parameters
  if (parameters == NULL) {
    params.update_rate = (int)(SHIFT1 * (CC_BGFG_MOG_UPDATE_RATE) + 0.5);
    params.update_thd = CC_BGFG_MOG_UPDATE_THD;
  }
  else {
    params = *parameters;
  }

  if (!(first_frame)) {
    CC_ERROR(CC_StsBadArg, "Invalid or NULL first_frame parameter");
  }

  bg_model = BUFMALLOC(CvAvgBGModel, 1);
  bg_model->buf = buf;
  bg_model->buflen = buflen;
  memset(bg_model, 0, sizeof(*bg_model));
  bg_model->type = CC_BG_MODEL_MOG;
  bg_model->release = (CvReleaseBGStatModel) icvReleaseAvgBGModel;
  bg_model->update = (CvUpdateBGStatModel) icvUpdateAvgBGModel;

  bg_model->params = params;

  bg_model->background = BUFMALLOC(unsigned char, width * height * nChannels);
  bg_model->foreground = BUFMALLOC(unsigned char, width * height);

  //CC_CALL( bg_model->storage = cvCreateMemStorage());

  BUFBOUNDCHECK();
  bg_model->countFrames = 0;

  for (i = 0, p = 0, n = 0; i < height; i++) {
    for (j = 0; j < width; j++, n++) {
      //update_rate;
      for (m = 0; m < nChannels; m++) {
        bg_model->background[ p + m ] = (first_frame[ p + m ]);
      }

      p += nChannels;
    }
  }

  BUFUSEEND();

  __END__;

  {
    bg_model->height = height;
    bg_model->width = width;
    bg_model->nChannels = nChannels;
  }

  return (CvBGStatModel*) bg_model;
}

int cvCreateAvgBGModel_memsize(int width, int height, int nChannels);

CvBGStatModel*
cvCreateAvgBGModel(int height, int width, unsigned char* first_frame,
    int widthStep, int nChannels, void* parameters1)
{
  char* buf = 0;
  int buflen;
  CvBGStatModel* bg_model;
  CvAvgBGStatModelParams* parameters = (CvAvgBGStatModelParams*)parameters1;
  CvAvgBGStatModelParams params;

  //init parameters
  if (parameters == NULL) {
    params.update_rate = (int)(SHIFT1 * (CC_BGFG_MOG_UPDATE_RATE) + 0.5);
    params.update_thd = CC_BGFG_MOG_UPDATE_THD;
  }
  else {
    params = *parameters;
  }

  buflen = SIZEOFARR(CvAvgBGModel, 1) +
      SIZEOFARR(unsigned char, width * height * nChannels) +
      SIZEOFARR(unsigned char, width * height);
  buf = MALLOC(char, buflen);
  bg_model = cvCreateAvgBGModel_process(height, width, first_frame,
      widthStep, nChannels, &params, buf, buflen);

  if (NULL == bg_model) {
    FREE(buf);
  }

  return bg_model;
}

#if 0
int bgmodel_memsize(bgmodel* b)
{
  int width = b->width, height = b->height;
  CvGaussBG2StatModelParams* parameters = NULL;
  CvGaussBG2StatModelParams params;

  //init parameters
  if (parameters == NULL) {
    params.win_size = CC_BGFG_MOG_WINDOW_SIZE;
    params.bg_threshold = SHIFT_TOINT(CC_BGFG_MOG_BACKGROUND_THRESHOLD);
    params.std_threshold = SHIFT_TOINT(CC_BGFG_MOG_STD_THRESHOLD);
    params.weight_init = SHIFT_TOINT(CC_BGFG_MOG_WEIGHT_INIT);
    params.variance_init = SHIFT_TOINT(CC_BGFG_MOG_SIGMA_INIT * CC_BGFG_MOG_SIGMA_INIT);
    params.minArea = SHIFT_TOINT(CC_BGFG_MOG_MINAREA);
    params.n_gauss = CC_BGFG_MOG_NGAUSSIANS;
  }
  else {
    params = *(CvGaussBG2StatModelParams*)parameters;
  }

  b->buflen =
      SIZEOFARR(CvGaussBG2Model, 1) + SIZEOFARR(CvGaussBG2Point, ((width * height) + 256)) +
      SIZEOFARR(unsigned char, width * height * b->nChannels) + SIZEOFARR(unsigned char, width * height) +
      SIZEOFARR(CvGaussBG2Values, params.n_gauss * (width * height + 128));
  return b->buflen;
}

#define UPDATEBGMODEL icvUpdateGaussianBG2Model
#define CREATEBGMODEL cvCreateGaussianBG2Model_process
#else
int bgmodel_memsize(bgmodel* b)
{
  b->buflen = SIZEOFARR(CvAvgBGModel, 1) +
      SIZEOFARR(unsigned char, b->width * b->height * b->nChannels) +
      SIZEOFARR(unsigned char, b->width * b->height);
  return b->buflen;
}

#define UPDATEBGMODEL icvUpdateAvgBGModel
#define CREATEBGMODEL cvCreateAvgBGModel_process
#endif

int bgmodel_process(bgmodel* b)
{
  CvBGStatModel* bg_model;

  if (BGMODEL_UPDATE == b->mode) {
    //cvUpdateBGStatModel( img2, al2, pBGStatModel );
    bg_model = (CvBGStatModel*)(b->buf);
    UPDATEBGMODEL(b->frame, b->step, (CvAvgBGModel*)(b->buf));
    b->background = bg_model->background;
    b->foreground = bg_model->foreground;
  }
  else if (BGMODEL_INIT == b->mode) {
    CvAvgBGStatModelParams params;

    if (b->update_rate > 0 && b->update_thd > 0) {
      params.update_rate = b->update_rate;
      params.update_thd = b->update_thd;
    }
    else {
      params.update_rate = (int)(SHIFT1 * (CC_BGFG_MOG_UPDATE_RATE) + 0.5);
      params.update_thd = CC_BGFG_MOG_UPDATE_THD;
    }

    bg_model = CREATEBGMODEL(b->height, b->width, b->frame,
        b->step, b->nChannels, &params, b->buf, b->buflen);
    ASSERT((char*)bg_model == b->buf);
    b->background = bg_model->background;
    b->foreground = bg_model->foreground;
  }

  return b->buflen;
}

/* End of file. */
