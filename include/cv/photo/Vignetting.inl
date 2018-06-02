
// 暗角特效 
// 有的app叫晕影效果，有的app叫暗角效果，其实就是以图像为中心，提高图像四角的亮度或者降低亮度。
// 为图像四角提高亮度的不多，更多的还是给图像增加暗角效果，以突出图像主体。 
// 尤其是很多lomo滤镜，基本都会加一个暗角。那么暗角是怎么实现的呢，看似很简单，
// 但真做到ps般色调过渡自然、平滑，看着舒服，还是需要一番计算与调试的。算法总体上没有复杂原理与计算，看代码基本就明白了。
// 下面是相关示例代码（亮角: strength范围为[0.0f, 1.0f], 暗角: strength范围为[-1.0f, 0.0f]）：

#define CLAMP0255(x)  BOUND(x, 0, 255)
#define SCREEN_XY(a, b)  ((a*b)>>8)
void Vignetting(int h, int w, const uchar* src, int srcstep, uchar* dst, int dststep, int cn, float strength)  
{    
  float radio = 0.0;  
  float maxLen = (float)MAX(w, h);  
  float cx = maxLen * 0.5f;  
  float cy = maxLen * 0.5f;  
  float maxDist = cx*cx + cy*cy;
  float t0, t1, t2;
  int i, j;
  
  if (strength > 0.0f)  
  {
    // 亮角  
    for (i = 0; i < h; i++) {
      const uchar* src1 = src + i*srcstep;
      uchar* dst1 = dst + i*dststep;
      for (j = 0; j < w; j++, src1 += cn, dst1 += cn) {
        float nx = (float)maxLen * j / w;
        float ny = (float)maxLen * i / h;
        float curDist = (nx - cx)*(nx - cx) + (ny - cy)*(ny - cy);  
        radio = curDist / maxDist;  
        // radio *= radio;
        radio *= strength;
        t0 = src1[0]*(1.0f - radio) + SCREEN_XY(src1[0], 230)*radio;
        t1 = src1[1]*(1.0f - radio) + SCREEN_XY(src1[1], 230)*radio;
        t2 = src1[2]*(1.0f - radio) + SCREEN_XY(src1[2], 230)*radio;
        dst1[0] = (uchar)CLAMP0255(t0);
        dst1[1] = (uchar)CLAMP0255(t1);
        dst1[2] = (uchar)CLAMP0255(t2);
      }  
    }  
  }
  else  
  {
    // 暗角  
    for (i = 0; i < h; i++) {
      const uchar* src1 = src + i*srcstep;
      uchar* dst1 = dst + i*dststep;
      for (j = 0; j < w; j++, src1 += cn, dst1 += cn) {
        float nx = (float)maxLen * j / w;  
        float ny = (float)maxLen * i / h;  
        float curDist = (nx - cx)*(nx - cx) + (ny - cy)*(ny - cy);  
        radio = curDist / maxDist;  
        // radio *= radio;  
        radio *= strength;  
        radio = 1.0f + radio;
        t0 = src1[0]*radio;
        t1 = src1[1]*radio;
        t2 = src1[2]*radio;
        dst1[0] = (uchar)CLAMP0255(t0);  
        dst1[1] = (uchar)CLAMP0255(t1);  
        dst1[2] = (uchar)CLAMP0255(t2);  
      }  
    }  
  }  
}  

