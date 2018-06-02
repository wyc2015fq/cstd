
//#define Add(ARR, X)  ((ARR)[ARR##Len++]=(X))

// 旋转体
int SetObjColor(Object3d* pO, FRGBA clr)
{
  int i;

  for (i = 0; i < pO->vListLen; i++) {
    pO->vList[i].clr = clr;
  }

  return 0;
}
// 旋转体
int initRevolvingSolid(Object3d* pO, int nSlices, int nStacks, FRGBA clr)
{
  int i, j, k = 0;
  int nIndexFacet, nIndexVertex;

  for (i = 0; i < pO->vListLen; i++) {
    int j = (i) % nSlices;
    //经度递增量, 弧度表示
    float fLongitude = -4.712389f + (float)((j) * 2.0 * PIE / nSlices);
    float x = (float)(pO->vList[i].coord.x * cos(fLongitude));
    float y = (float)(pO->vList[i].coord.y * sin(fLongitude));
    float z = pO->vList[i].coord.z;
    //填写坐标分量
    V3SET(pO->vList[i].coord, x, y, z), ++k;
    pO->vList[i].clr = clr;
  }

  //i按纬度循环,  j按经度循环
  k = 0;

  //处理位于北极圈内的面:
  for (j = 0; j < nSlices; j++) {
    int j1 = j == (nSlices - 1) ? 0 : j + 1;
    A3SET(pO->sList[k].vIndex, 0, j + 1, j1 + 1), ++k;
  }

  //处理除两极以外的小面
  for (i = 0; i < (nStacks - 2); i++) {
    //小面位于两纬线之间,
    //上纬线(投影)与 y 轴交点序号
    int nIndexUp = i * nSlices + 1;
    //下纬线(投影)与 y 轴交点序号
    int nIndexDown = nIndexUp + nSlices;

    //按经区循环
    for (j = 0; j < nSlices; j++) {
      int j1 = j == (nSlices - 1) ? 0 : j + 1;
      //DefineTopology(&pO->sList[nIndexDown + j - 1], anVertices,  4);
      A3SET(pO->sList[k].vIndex, nIndexUp + j, nIndexDown + j, nIndexDown + j1), ++k;
      A3SET(pO->sList[k].vIndex, nIndexUp + j, nIndexDown + j1, nIndexUp + j1), ++k;
    }//end for j
  }//end for i;

  //处理南极圈内的小面
  //小面序号基数
  nIndexFacet = (nStacks - 1) * nSlices;
  //顶点序号基数
  nIndexVertex = nSlices * (nStacks - 2) + 1;

  for (j = 0; j < nSlices; j++) {
    int j1 = j == (nSlices - 1) ? 0 : j + 1;
    A3SET(pO->sList[k].vIndex, nIndexVertex + j, nIndexFacet  + 1, nIndexVertex + j1), ++k;
  }

  assert(k == pO->sListLen);

  return 0;
}
//float fRadius; 球的半径
//int nSlices;  球的经度区域个数,简称经区数, (经线条数与它相等)
//int nStacks;  球的纬度带状区域条数,简称纬带数, (纬线条数比它少 1)
//初始化顶点的几何(坐标和法线)
//初始化小面几何(坐标)
int initSphere(Object3d* pO, int n, float fRadius, int nSlices, int nStacks, FRGBA clr)
{
  int n0 = n;
  int i, j, k = 0;
  float x, y, z;
  fRadius = ABS(fRadius);
  //最少不得小于8, 否则不象个球
  nSlices = MAX(nSlices, 4);
  nStacks = MAX(nStacks, 4);


  for (; n--; pO++) {
    k = 0;
    // 纬度自北极向南极递增, 并不象地理划分那样分北纬和南纬
    // 经度从y轴正向开始, 按逆时针方向增加, 不区分东经和西经.
    // 处理北极点
    //初始化动态数组的大小.
    initObject3d(pO, (nSlices * (nStacks - 1) + 2), (2 * nSlices * (nStacks - 1)), 0);
    V3SET(pO->vList[k].coord, 0.0f, 0.0f, fRadius), ++k;

    //法线与缺省值相同
    //按纬度循环
    for (i = 0; i < (nStacks - 1); i++) {
      //纬度
      float fLatitude = (float)((i + 1) * PIE / nStacks);
      //坐标 z 分量
      z = fRadius * (float)cos(fLatitude);
      x = fRadius * (float)sin(fLatitude);
      y = x;

      //按经度循环
      for (j = 0; j < nSlices; j++) {
        V3SET(pO->vList[k].coord, x, y, z), ++k;
      }
    }

    // 处理南极点
    V3SET(pO->vList[k].coord, 0.0f, 0.0f, -fRadius), ++k;
    assert(k == pO->vListLen);
    initRevolvingSolid(pO, nSlices, nStacks, clr);
  }

  return n0;
}
// 圆椎体、圆柱体
int initCylinder(Object3d* pO, int n, float fRadius1, float fRadius2, float fH, int nSlices, int nStacks, FRGBA clr)
{
  int n0 = n;
  int i, j, k = 0;
  float x, y, z;
  float zoff = 0.f;
  fRadius1 = ABS(fRadius1);
  fRadius2 = ABS(fRadius2);
  nSlices = MAX(nSlices, 3);
  nStacks = MAX(nStacks, 3);

  for (; n--; pO++) {
    //初始化动态数组的大小.
    initObject3d(pO, (nSlices * (nStacks - 1) + 2), (2 * nSlices * (nStacks - 1)), 0);
    k = 0;
    V3SET(pO->vList[k].coord, 0.0f, 0.0f, fH + zoff), ++k;

    for (i = 0; i < (nStacks - 1); i++) {
      float fLatitude = (float)((i + 1) * PIE / nStacks);
      z = fH * (nStacks - 2 - i) / (nStacks - 2) + zoff;
      x = (fRadius1 * (nStacks - 2 - i) + fRadius2 * i) / (nStacks - 2);
      y = x;

      for (j = 0; j < nSlices; j++) {
        V3SET(pO->vList[k].coord, x, y, z), ++k;
      }
    }

    V3SET(pO->vList[k].coord, 0.0f, 0.0f, 0.f + zoff), ++k;
    assert(k == pO->vListLen);
    initRevolvingSolid(pO, nSlices, nStacks, clr);
  }

  return n0;
}

// 读取off文件
int loadObject_OFF(Object3d* pO, int n, LPTSTR lpszFileName)
{
  int i, iTempB = 0, len, iAugmentNum, n0 = n, sListLen, vListLen;
  char* buf = (char*)loadfile(lpszFileName, &len);

  if (0 != strncmp("OFF", buf, 3)) {
    free(buf);
    return 0;
  }

  for (; n--; pO++) {
    float x, y, z;
    char* szTemp = buf;
    NEXTLINE(szTemp);
    sscanf(szTemp, "%d %d %d\n", &vListLen, &sListLen, &iTempB);
    NEXTLINE(szTemp);
    pO->maxRadius = 0;

    initObject3d(pO, vListLen, sListLen * 4 / 3 + 3, 0);

    for (i = 0 ; i < pO->vListLen ; i++) {
      float max_vertex;
      sscanf(szTemp, "%f %f %f\n", &x, &y, &z);
      V3SET(pO->vList[i].coord, x, y, z);
      max_vertex = MAX(ABS(x), MAX(ABS(y), ABS(z)));

      if (pO->maxRadius < max_vertex) {
        pO->maxRadius = max_vertex;
      }

      NEXTLINE(szTemp);
    }

    iAugmentNum = 0;

    for (i = 0 ; i < sListLen ; i++) {
      int iTemp = 0;
      int iT1 = 0 , iT2 = 0 , iT3 = 0 , iT4 = 0;
      sscanf(szTemp, "%d %d %d %d\n", &iTemp, &iT1, &iT2, &iT3, &iT4);

      if (iTemp == 3) {
        //三角片的数目增加1个
        A3SET(pO->sList[iAugmentNum].vIndex, iT1, iT2, iT3);
        iAugmentNum++;
      }
      else if (iTemp == 4) {
        //把4个点组成的面片分成两个3个点组成的面片,并调整整个模型的面片数量
        //三角片的数目增加2个
        A3SET(pO->sList[iAugmentNum].vIndex, iT1, iT2, iT3);
        iAugmentNum++;
        A3SET(pO->sList[iAugmentNum].vIndex, iT1, iT3, iT4);
        iAugmentNum++;
      }
      else {
        //把n个点组成的面片分成两个3个点组成的面片
        //并调整整个模型的面片数量
        //("多个点组成的面片，现在还不能处理！！！");
        assert(0);
        return FALSE;
      }

      NEXTLINE(szTemp);
    }

    //调整整个模型的面片数量
    //整理面片索引的存储
    pO->sListLen = iAugmentNum;
  }

  free(buf);
  return n0;
}


// 读取off文件
int loadObject_TM(Object3d* pO, int n, LPTSTR lpszFileName)
{
  int i, len, iAugmentNum, n0 = n;
  char* buf = (char*)loadfile(lpszFileName, &len);

  for (; n--; pO++) {
    float x, y, z;
    char* szTemp = buf;
    sscanf(szTemp, "%d %d\n", &pO->vListLen, &pO->sListLen);
    NEXTLINE(szTemp);
    pO->maxRadius = 0;

    initObject3d(pO, pO->vListLen, pO->sListLen, 0);

    for (i = 0 ; i < pO->vListLen ; i++) {
      float max_vertex;
      sscanf(szTemp, "%f %f %f\n", &x, &y, &z);
      V3SET(pO->vList[i].coord, x, y, z);
      max_vertex = MAX(ABS(x), MAX(ABS(y), ABS(z)));

      if (pO->maxRadius < max_vertex) {
        pO->maxRadius = max_vertex;
      }

      NEXTLINE(szTemp);
    }

    iAugmentNum = 0;

    for (i = 0 ; i < pO->sListLen ; i++) {
      int iT1 = 0 , iT2 = 0 , iT3 = 0 , iT4 = 0;
      sscanf(szTemp, "%d %d %d\n", &iT1, &iT2, &iT3, &iT4);
      //三角片的数目增加1个
      A3SET(pO->sList[iAugmentNum].vIndex, iT1, iT2, iT3);
      iAugmentNum++;
      NEXTLINE(szTemp);
    }
  }

  free(buf);
  return n0;
}

