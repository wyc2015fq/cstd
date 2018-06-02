
//ColorMover.c
#include <stdlib.h>
#include <string.h>


static unsigned char _color_table[ 256 * 3 ];
const unsigned char* color_table = &_color_table[ 256 ];

static long border_color(long x)
{
  if (x < 0) {
    return 0;
  }
  else if (x > 255) {
    return 255;
  }
  else {
    return x;
  }
}

int _CAuto_inti_color_table()
{
  static int _Auto_inti_color_table = 0;

  if (0 == _Auto_inti_color_table) {
    int i;
    _Auto_inti_color_table = 1;

    for (i = 0; i < 256 * 3; ++i) {
      _color_table[ i ] = (unsigned char) border_color(i - 256);
    }
  }

  return 1;
}


static unsigned char _colorRound_table[(csMaxColorRoundValue + 1) * 2 ];
const unsigned char* colorRound_table = &_colorRound_table[(csMaxColorRoundValue + 1) ];

static long round_color(long x)
{
  double rd;
  long ri;

  //const double PI = 3.1415926535897932384626433832795;
  if (x < 0) {
    x = -x;
  }

  while (x > csMaxColorRoundValue) {
    x -= csMaxColorRoundValue;
  }

  rd = (sin(x * (2.0 * PI / csMaxColorRoundValue)) + 1 + 0.1) / (2.0 + 0.1);
  ri = (long)(rd * 255 + 0.5);

  //long ri=abs(x-csMaxColorRoundValue/2);
  if (ri < 0) {
    return 0;
  }
  else if (ri > 255) {
    return 255;
  }
  else {
    return ri;
  }
}

int _CAuto_inti_colorRound_table()
{
  static int _Auto_inti_colorRound_table = 0;

  if (0 == _Auto_inti_colorRound_table) {
    int i;

    for (i = 0; i < (csMaxColorRoundValue + 1) * 2; ++i) {
      _colorRound_table[ i ] = (unsigned char) round_color(i - (csMaxColorRoundValue + 1));
    }
  }

  return 1;
}

void CColorMoverInti(CColorMover* cm, double kMin, double kMax, double ColorVMin, double ColorVMax)
{
  _CAuto_inti_color_table();
  _CAuto_inti_colorRound_table();
  cm->m_kR = rand() * (1.0 / RAND_MAX) * (kMax - kMin) + kMin;
  cm->m_kG = rand() * (1.0 / RAND_MAX) * (kMax - kMin) + kMin;
  cm->m_kB = rand() * (1.0 / RAND_MAX) * (kMax - kMin) + kMin;

  cm->m_VR = rand() * (1.0 / RAND_MAX) * (ColorVMax - ColorVMin) + ColorVMin;
  cm->m_VG = rand() * (1.0 / RAND_MAX) * (ColorVMax - ColorVMin) + ColorVMin;
  cm->m_VB = rand() * (1.0 / RAND_MAX) * (ColorVMax - ColorVMin) + ColorVMin;

  if (rand() > RAND_MAX / 2) {
    cm->m_VR *= (-1);
  }

  if (rand() > RAND_MAX / 2) {
    cm->m_VG *= (-1);
  }

  if (rand() > RAND_MAX / 2) {
    cm->m_VB *= (-1);
  }

  cm->m_R0 = rand() * (1.0 / RAND_MAX) * csMaxColorRoundValue;
  cm->m_G0 = rand() * (1.0 / RAND_MAX) * csMaxColorRoundValue;
  cm->m_B0 = rand() * (1.0 / RAND_MAX) * csMaxColorRoundValue;
}

void CColorMoverUpdate(CColorMover* cm, unsigned long StepTime_ms)
{
  cm->m_R0 += cm->m_VR * StepTime_ms;
  cm->m_G0 += cm->m_VG * StepTime_ms;
  cm->m_B0 += cm->m_VB * StepTime_ms;

  if (cm->m_R0 < 0) {
    cm->m_R0 += csMaxColorRoundValue;
  }
  else if (cm->m_R0 >= csMaxColorRoundValue) {
    cm->m_R0 -= csMaxColorRoundValue;
  }

  if (cm->m_G0 < 0) {
    cm->m_G0 += csMaxColorRoundValue;
  }
  else if (cm->m_G0 >= csMaxColorRoundValue) {
    cm->m_G0 -= csMaxColorRoundValue;
  }

  if (cm->m_B0 < 0) {
    cm->m_B0 += csMaxColorRoundValue;
  }
  else if (cm->m_B0 >= csMaxColorRoundValue) {
    cm->m_B0 -= csMaxColorRoundValue;
  }
}

void CPointMoverInti(CPointMover* pm, double MinValue, double MaxValue, double MinV, double MaxV)
{
  pm->m_MinValue = MinValue;
  pm->m_MaxValue = MaxValue;
  pm->m_MinV = MinV;
  pm->m_MaxV = MaxV;
  pm->m_V = 0;
  CPointMoverGetRandomValue(pm);
  CPointMoverGetRandomV(pm);
}

void CPointMoverUpdate(CPointMover* pm, unsigned long StepTime_ms)
{
  pm->m_Value += pm->m_V * StepTime_ms;

  if (pm->m_Value < pm->m_MinValue) {
    pm->m_Value = pm->m_MinValue;
    CPointMoverGetRandomV(pm);
  }
  else if (pm->m_Value > pm->m_MaxValue) {
    pm->m_Value = pm->m_MaxValue;
    CPointMoverGetRandomV(pm);
  }
}

void CPointMoverGetNextCV(CPointMover* pm, double StepTime_ms)
{
  pm->m_Value += pm->m_V * StepTime_ms;

  if (pm->m_Value < pm->m_MinValue) {
    pm->m_Value = pm->m_MinValue;
    CPointMoverGetRandomV(pm);
  }
  else if (pm->m_Value > pm->m_MaxValue) {
    pm->m_Value = pm->m_MaxValue;
    CPointMoverGetRandomV(pm);
  }
}

void CScenePointFractalUpdateMover(void* pf0, unsigned long StepTime_ms)
{
  int i;
  CScenePointFractal* pf = (CScenePointFractal*) pf0;

  for (i = 0; i < pf->m_MoverListSize; ++i) {
    CPointMoverUpdate(&pf->m_MoverList[ i ], StepTime_ms);
  }

  for (i = 0; i < pf->m_ColorMoverListSize; ++i) {
    CColorMoverUpdate(&pf->m_ColorMoverList[ i ], StepTime_ms);
  }
}

void darkle(int height, int width, unsigned char* pdata, int byte_width, int cn, long del_color)
{
  int x, y;

  for (y = 0; y < height; ++y) {
    unsigned char* pdst = pdata;

    for (x = 0; x < width; ++x, pdst += cn) {
      if (pdst[ 0 ] != 0 && pdst[ 1 ] != 0 && pdst[ 2 ] != 0) {
        if (pdst[ 0 ] > del_color) {
          pdst[ 0 ] = color_table[ pdst[ 0 ] - del_color ];
        }

        if (pdst[ 1 ] > del_color) {
          pdst[ 1 ] = color_table[ pdst[ 1 ] - del_color ];
        }

        if (pdst[ 2 ] > del_color) {
          pdst[ 2 ] = color_table[ pdst[ 2 ] - del_color ];
        }
      }
    }

    pdata += byte_width;
  }
}

#define FRACTAL_NEW(_STR_NAME, _CLASS_NAME) \
  if (0==stricmp(_STR_NAME, name)) { \
    return _CLASS_NAME##Inti(); \
  }

void* fractal_new(const char* name)
{
  FRACTAL_NEW("Julia", CSceneJulia);
  FRACTAL_NEW("SpotFormat", CSceneSpotFormat);
  FRACTAL_NEW("King", CSceneKing);
  FRACTAL_NEW("King2", CSceneKing2);
  FRACTAL_NEW("Mira", CSceneMira);
  FRACTAL_NEW("Newton", CSceneNewton);
  FRACTAL_NEW("Water", CSceneWater);
  FRACTAL_NEW("Fire", CSceneFire);
  FRACTAL_NEW("Plasma", CScenePlasma);
  FRACTAL_NEW("Pool", CScenePool);
  return 0;
}

void fractal_update(void* sf, unsigned long StepTime_ms)
{
  ((scene_fractal_base*)(sf)) ->update(sf, StepTime_ms);
}

void fractal_draw(void* sf, int dst_height, int dst_width, unsigned char* dst, int dst_step, int cn)
{
  ((scene_fractal_base*)(sf)) ->draw(sf, dst_height, dst_width, dst, dst_step, cn);
}

void fractal_clear(void* sf)
{
  ((scene_fractal_base*)(sf)) ->clear(sf);
}
