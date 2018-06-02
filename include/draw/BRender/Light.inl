typedef struct Material {
  //材质对环境光的反射率(reflectivity)
  FRGBA refAmb;
  //材质对漫反射光的反射率
  FRGBA refDif;
  //材质对镜面高光的反射率
  FRGBA refSpe;
  //材质的自体辐射光色(Emission)
  FRGBA clrEmi;
  //材质亮度（高光指数）
  float fShininess;
  //面的类型,前面,后面,还是前后两面(双面)
  DWORD dwFaceType;
} Material;
int initMaterial(Material* pM, int n)
{
  for (; n-- > 0; ++pM) {
    //缺省材质
    FRGBASET(pM->refAmb, 0.2f, 0.2f, 0.2f, 1.0f);
    FRGBASET(pM->refDif, 0.8f, 0.8f, 0.8f, 1.0f);
    FRGBASET(pM->refSpe, 0.0f, 0.0f, 0.0f, 1.0f);
    FRGBASET(pM->clrEmi, 0.0f, 0.0f, 0.0f, 1.0f);
    pM->fShininess = 128.0f;
    pM->dwFaceType = G3D_FRONT;
  }

  return 0;
}
//设置光亮度
//dwFace-------光照面, 取枚举值
//dwProperty---属性, 取 G3D_SHININESS 为有效
//fParam-------具体参数值
void Materialf(Material* pM, DWORD dwFace,  DWORD dwProperty,  float fParam)
{
  if (dwProperty == G3D_SHININESS) {
    pM->dwFaceType = dwFace;
    pM->fShininess = (fParam < 0.0f) ? 0.0f : ((fParam > 128.0f) ? 128.0f : fParam);
  }
}
//设置向量参数
//dwFace-------光照面, 取枚举值
//dwProperty---属性,
//pfParams-----具体参数值
void Materialfv(Material* pM, DWORD dwFace,  DWORD dwProperty,  const float* pfParams)
{
  ASSERT(pfParams);
  pM->dwFaceType = dwFace;

  switch (dwProperty) {
    //环境光反射系数
  case G3D_AMBIENT:
    FRGBASETv(pM->refAmb, pfParams);
    break;

    //漫反射光反射系数
  case G3D_DIFFUSE:
    FRGBASETv(pM->refDif, pfParams);
    break;

    //镜面高光反射系数
  case G3D_SPECULAR:
    FRGBASETv(pM->refSpe, pfParams);
    break;

    //环境和漫反射系数相同
  case G3D_AMBIENT_AND_DIFFUSE:
    FRGBASETv(pM->refAmb, pfParams);
    FRGBASETv(pM->refDif, pfParams);
    break;

    //自体辐射光色
  case G3D_EMISSION:
    FRGBASETv(pM->clrEmi, pfParams);
    break;
  }
}
typedef struct _LightObj {
  //光的环境颜色
  FRGBA clrAmb;
  //光的漫反射颜色
  FRGBA clrDif;
  //光的镜面高光颜色
  FRGBA clrSpe;
  //光源位置
  VECTOR3D hcPosition;
  //聚光照射方向:
  VECTOR3D  spotDirection;
  //聚光指数
  float fSpotExp;
  //聚光发散半角
  float fSpotCutoff;
  //常数衰减系数
  float fAttCoef0;
  //线性衰减系数
  float fAttCoef1;
  //二次衰减系数
  float fAttCoef2;
  //光是否开启
  BOOL bOnOff;
} LightObj;
int initLightObj(LightObj* pLO, int n)
{
  for (; n > 0; --n, ++pLO) {
    //缺省光源为一全向白色点光源
    FRGBASET(pLO->clrAmb, 0.0f, 0.0f, 0.0f, 1.0f);
    FRGBASET(pLO->clrDif, 1.0f, 1.0f, 1.0f, 1.0f);
    FRGBASET(pLO->clrSpe, 1.0f, 1.0f, 1.0f, 1.0f);
    pLO->hcPosition.x = 0.0f;
    pLO->hcPosition.y = 0.0f;
    pLO->hcPosition.z = 8.0f;
    pLO->hcPosition.w = 1.0f;
    pLO->spotDirection.x = 0.0f;
    pLO->spotDirection.y = 0.0f;
    pLO->spotDirection.z = -1.0f;
    pLO->fSpotExp = 0.0f;
    pLO->fSpotCutoff = 180.0f;
    pLO->fAttCoef0 = 1.0f;
    pLO->fAttCoef1 = 0.0f;
    pLO->fAttCoef2 = 0.0f;
    pLO->bOnOff = TRUE;
  }

  return 0;
}
//光是否为开启状态
//设置光源是否可以开启
//形式上统一, 第一个参数为G3D_LIGHT_ON_OFF有效
void Lightb(LightObj* pLO, DWORD dwProperty,  BOOL bOnOff)
{
  if (dwProperty == G3D_LIGHT_ON_OFF) {
    pLO->bOnOff = bOnOff;
  }
}
//设置标量参数
//dwProperty----属性
//fParam------参数值, 标量
void Lightf(LightObj* pLO, DWORD dwProperty,  float fParam)
{
  switch (dwProperty) {
    //聚光指数
  case G3D_SPOT_EXPONENT:
    //发散指数在[0, 128]之间
    pLO->fSpotExp = (fParam < 0.0f) ? 0.0f : ((fParam > 128.0f) ? 128.0f : fParam);
    break;

    //发散半角
  case G3D_SPOT_CUTOFF:

    //发散半角取 [0, 90] 或 180
    if (ABS(fParam - 180.0f) < 0.001f) {
      pLO->fSpotCutoff = 180.0f;
    }
    else {
      pLO->fSpotCutoff = (fParam < 0.0f) ? 0.0f : ((fParam > 90.0f) ? 90.0f : fParam);
    }

    break;

    //常数衰减因子
  case G3D_CONSTANT_ATTENUATION:
    pLO->fAttCoef0 = ABS(fParam);
    break;

    //线性衰减因子
  case G3D_LINEAR_ATTENUATION:
    pLO->fAttCoef1 = ABS(fParam);
    break;

    //二次衰减因子
  case G3D_QUADRATIC_ATTENUATION:
    pLO->fAttCoef2 = ABS(fParam);
    break;
  }
}
//设置向量参数
//参数设置, 向量值(一维数组)
//dwProperty----属性
//pfParams------参数值, 按 RGBA 顺序或 XYZW 顺序组织数据
#define Lightfv(pLO, dwProperty, pfParams)  Lightff(pLO, dwProperty, pfParams[0], pfParams[1], pfParams[2], pfParams[3])
int Lightff(LightObj* pLO, DWORD dwProperty, float param0, float param1, float param2, float param3)
{
  switch (dwProperty) {
    //环境光
  case G3D_AMBIENT:
    FRGBASET(pLO->clrAmb, param0, param1, param2, param3);
    break;

    //漫反射光
  case G3D_DIFFUSE:
    FRGBASET(pLO->clrDif, param0, param1, param2, param3);
    break;

    //镜面高光
  case G3D_SPECULAR:
    FRGBASET(pLO->clrSpe, param0, param1, param2, param3);
    break;

    //光的位置
  case G3D_POSITION:
    V4SET(pLO->hcPosition, param0, param1, param2, param3);
    break;

    //聚光照射方向(非齐次坐标)
  case G3D_SPOT_DIRECTION: {
    //直接单位化
    float fMag = (float)sqrt(param0 * param0 + param1 * param1 + param2 * param2);

    //对于退化向量, 将照射方向移至 z 轴
    if (fMag < 0.0001f) {
      V3SET(pLO->spotDirection, 0.0f, 0.0f, -1.0f);
    }
    else {
      V3SET(pLO->spotDirection, param0 / fMag, param1 / fMag, param2 / fMag);
    }
  }
  break;
  }

  return 0;
}

typedef struct _RenderDC {
  TDrawDC hDC[1]; // 颜色缓冲
  HMC hZB; // 深度缓冲
  Material material[1]; // 物体标准材质
  MATRIX3D m[1]; // 模型变换矩阵
  VECTOR3D viewer;//视点，用于光照计算，被定义于世界坐标系
  VIEWFINDER pVF[1]; // 取景器
  LightObj lights[4]; // 多光源
  int nNumLight; // 光源个数
  FRGBA clrGlobalAmb; //全局环境光
  BOOL bLocalViewer; // 光局部视点 0:无穷远视点模型
  BOOL bTwoSides; // 是否双面光照 0:只照射前面
  DWORD dwRop; // 着色模式
  Image* tex;
} RenderDC;
int initRenderDC(RenderDC* rDC, HDDC hDC)
{
  Material defMaterial = {{1.f, 1.f, 1.f, 1.0f}, {1.f, 1.f, 1.f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.f, 1.f, 1.f, 1.0f}, 50.0f, G3D_FRONT};
  VIEWFINDER ViewFinder = {100.0f, 16.0f, 16.0f, {hDC->w / 2, hDC->h / 2}};
  memset(rDC, 0, sizeof(RenderDC));
  initLightObj(rDC->lights, 4);
  initMaterial(rDC->material, 1);
  rDC->bLocalViewer = 1;
  M3IDENTITY(*rDC->m);
  FRGBASET(rDC->clrGlobalAmb, 0.2f, 0.2f, 0.2f, 1.0f);
  V3SET(rDC->viewer, 0.0f, 0.0f, 8.0f);
  *rDC->pVF = ViewFinder;
  //深度缓冲区, 与颜色缓冲区大小保持一致
  rDC->hZB = newZBuffer(hDC->w, hDC->h);
  *rDC->hDC = *hDC;
  //*rDC->material = defMaterial;
  initAllDepth(rDC->hZB, 500.0f);
  return 0;
}
int delRenderDC(RenderDC* rDC)
{
  free(rDC->hZB);
  return 0;
}
int LightingVertex(Vertex3d* pVertex, RenderDC* rDC)
{
  int j; // 按光源个数进行循环
  Material* material = rDC->material;
  VIEWFINDER* pVF = rDC->pVF;
  const LightObj* lights = rDC->lights;
  int nNumLight = rDC->nNumLight;
  BOOL bLocalViewer = rDC->bLocalViewer;
  BOOL bTwoSides = rDC->bTwoSides;

  //注:   该光照计算在世界坐标系中完成
  //显式表示视点, 它被定义于世界坐标系
  VECTOR3D Viewer = rDC->viewer;
  //定义一个无穷远视点观察向量,
  //如果光照计算模型采用无穷远视点, 则该向量将起作用, 否则不起作用
  //在此情况下, 所有顶点指向视点的向量都是相同的.
  VECTOR3D vLocalView = rDC->viewer;
  //如果计算高光时采用无穷远视点, 则现在就单位化, 这将减少大量的计算
  //为了通用, 无论其顶点是可见的, 还是不可见的, 这里对所有顶点都进行计算
  //由于该光照系统在世界坐标系中进行, 顶点的可见性信息还不知道.
  //对于复杂的场景, 基于平面公式法进行消隐已经不能完成任务.完整的边和小面的可见性很模糊.
  //但, 另一方面, 边、小面的颜色又依赖于顶点的颜色
  //获取物体的第 i 个顶点:
  VECTOR3D Vertex = pVertex->coord;
  FRGBA ff, fLast;

  //第四步, 用观察坐标系中的 z 坐标来代替屏幕坐标系下的 z 坐标
  //这是一个几何跨越，利用齐次坐标获取实际深度,
  //注意, CViewFinder_PerspectiveH()函数作了与下后面这个循环相同的除法运算;
  //在计算上有些重复, 但观察坐标在更加复杂的系统中还有其它的应用.
  //因而, 我们并不通过CViewFinder类的Perspective()函数来把观察坐标变换为等价的三维坐标
  if (ABS(pVertex->scrn.w) < 0.00001f) {
    pVertex->scrn.z = 100000.0f;
  }
  else {
    pVertex->scrn.z /= pVertex->scrn.w;
  }

  if (!bLocalViewer) {
    Unitize(&vLocalView);
  }

  //将齐次坐标转化为真实坐标
  //无穷远点, 近似地用 0.0001 来代替(扩大齐次坐标10000倍的点来代替无穷远点的颜色)
  //不必考虑 wVertex 趋近于 0 时的符号
  if (ABS(Vertex.w) < 0.0001f) {
    Vertex.w = 0.0001f;
  }

  V3OPTv(Vertex, /= Vertex.w);
  //顶点最后的颜色
  //第一项, 物体辐射颜色, 这是直接贡献项
  fLast = material->clrEmi;
  //第二项, 全局环境光的贡献
  //全局环境光 * 材质对环境光的反射系数 -----
  //分量对应相乘后, 累积至最后的颜色之中.
  FRGBOPTvvv(fLast, += , rDC->clrGlobalAmb, *, material->refAmb);

  //来自于光源的贡献，
  for (j = 0; j < nNumLight; j++) {
    //如果光源是开启的
    if (lights[j].bOnOff) {
      //其一, 计算衰减系数
      //衰减系数.等于1.0则不衰减
      float fAttenuCoef = 1.0f;
      float fCos, fSpotFactor;
      float fInsideCone;
      //获取第 j 个光源位置
      VECTOR3D Light = lights[j].hcPosition;
      VECTOR3D vVertexToLight;

      //只有点光源我们才认为它是可衰减的, 此时, w = 1.0f.
      //如果光源是平行光源, 即定向光源, 则不衰减, 时时, w = 0.0f
      //定位光源
      if (ABS(Light.w) > 0.0001f) {
        float fc1, fc2;
        //真实位置, 保持齐次坐标分量不变
        V3OPTv(Light, /= Light.w);
        //常数衰减系数
        fAttenuCoef = lights[j].fAttCoef0;
        //一次衰减系数与二次衰减系数
        fc1 = lights[j].fAttCoef1;
        fc2 = lights[j].fAttCoef2;

        if ((fc1 > 0.0001f) || (fc2 > 0.0001f)) {
          //求顶点至光源的距离
          float fDist = V3DIST(Light, Vertex);
          //=(float)sqrt((Light.x - Vertex.x) * (Light.x - Vertex.x) + (Light.y - Vertex.y) * (Light.y - Vertex.y) + (Light.z - Vertex.z) * (Light.z - Vertex.z));
          //加入一次和二次因子
          fAttenuCoef += (fc1 * fDist + fc2 * fDist * fDist);
        }

        if (fAttenuCoef < 0.0001f) {
          fAttenuCoef = 0.0001f;
        }

        fAttenuCoef = 1.0f / fAttenuCoef;
        //衰减系数不得大于1.0
        fAttenuCoef = MIN(1.0f, fAttenuCoef);
      }//end if ---- for calculating the coefficient of attenuation

      //其二, 计算聚光因子
      //聚光因子, 一般点光源的聚光因子为 1.0f, (发散半角为180度)
      fSpotFactor = 1.0f;

      //聚光产生的条件:第一, 光为定位光源; 第二, 光的发散半角小于或等于90度
      if ((ABS(Light.w) > 0.0001f) && (lights[j].fSpotCutoff < 90.0001f)) {
        //光源的真实位置已经为(Light.x, Light.y, Light.z),
        //定向光源不产生聚光效果
        //向量: 聚光位置指向照射顶点
        VECTOR3D vLightToVertex;// = {(Vertex.x - Light.x), (Vertex.y - Light.y), (Vertex.z - Light.z)};
        V3OPTvvv(vLightToVertex, = , Vertex, -, Light);
        //单位化
        Unitize(&vLightToVertex);
        //聚光照射方向(已经是一个单位向量) 与 向量 vLightToVertex 夹角的余弦
        fInsideCone = V3DOT(vLightToVertex, lights[j].spotDirection);

        //如果顶点位于光锥之外, 则不会有聚光光线照射到物体上
        if (fInsideCone < (float)cos((lights[j].fSpotCutoff * PIE) / 180.0)) {
          fSpotFactor = 0.0f;
        }
        else {
          //利用聚光指数进行计算
          fSpotFactor = (float)pow(fInsideCone, lights[j].fSpotExp);
        }
      }//enf if ---- for calculating spot factor

      //其三, 计算来自光源的贡献(现在已经有足够的条件了)
      //首先, 加入环境反射部分:
      FRGBOPTvvv(ff, = , material->refAmb, *, lights[j].clrAmb);
      //其次, 计算漫反射部分
      //顶点指向光源的向量
      V3SET(vVertexToLight, (Light.x - Vertex.x), (Light.y - Vertex.y), (Light.z - Vertex.z));

      //如果光源为平行光源(定位光源)
      if ((ABS(Light.w) <= 0.0001f)) {
        //光源的位置就是照射方向, 因而顶点至光源的向量就是光源位置向量的相反向量
        V3OPTvv(vVertexToLight, = -, Light);
      }

      //单位化
      Unitize(&vVertexToLight);
      //顶点法线向量与 vVertexToLight 向量的夹角的余弦
      //顶点法线应是单位向量, 这在建模时已经或必须保证的
      fCos = V3DOT(vVertexToLight, pVertex->normal);
      fCos = (fCos < 0.0f) ? 0.0f : fCos;
      //加入漫反射部分的贡献
      FRGBOPTvvv(ff, += fCos*, material->refDif, *, lights[j].clrDif);

      //最后, 计算高光部分
      //如果顶点法线与顶点至光源的向量夹角大于90度, 则没有高光产生
      //否则就产生
      if (fCos > 0.0f) {
        //计算顶点指向视点的单位向量:
        VECTOR3D vVertexToViewer;
        float fShine, fShineFactor;

        //无穷远视点模型
        if (!bLocalViewer) {
          vVertexToViewer = vLocalView;
        }
        //局部视点模型
        else {
          V3OPTvvv(vVertexToViewer, = , Viewer, -, Vertex);
          //单位化:
          Unitize(&vVertexToViewer);
        }

        //加上顶点指向光源的单位向量:
        //vVertexToViewer += vVertexToLight;
        V3ADD(vVertexToViewer, vVertexToViewer, vVertexToLight);
        //再次单位化:
        Unitize(&vVertexToViewer);
        //光度因子基数:与顶点法线作点积
        fShine = V3DOT(vVertexToViewer, pVertex->normal);
        fShine = (fShine < 0.0f) ? 0.0f : fShine;
        fShineFactor = (float)pow(fShine, material->fShininess);
        //加入高光部分的贡献
        FRGBOPTvvv(ff, += fShineFactor*, material->refSpe, *, lights[j].clrSpe);
      }// end if(fCos > 0.0f)

      //最后乘以衰减和聚光因子
      //这就是第 j 个光对物体的第个顶点的照射:
      FRGBOPTv(ff, *= fAttenuCoef * fSpotFactor);
      //累加至最后颜色:
      FRGBOPTvv(fLast, += , ff);
    }// end if(lights[0].bOnOff)
  }//end j (number of lights)

  //作颜色归一化处理:
  FRGBROUND01(fLast);
  //颜色赋值
  FRGBOPTvv(pVertex->clr, *= , fLast);
  //对于alpha, 我们这里简单地用材质漫反射属性来代替
  //这只是简化处理, 对于透明效果, 还应该增加物理模型.
  pVertex->clr.a = material->refDif.a;
  return 0;
}
