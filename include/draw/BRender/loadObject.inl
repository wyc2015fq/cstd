
#define NEXTLINE(szText)  {szText=strchr(szText, '\n'); if (NULL==szText++) return 0;}
int loadObject_PLG(Object3d* pO, int n, LPTSTR lpszFileName)
{
  int i, vertex, len, n0 = n;
  char* buf = (char*)loadfile(lpszFileName, &len);

  for (; n--; ++pO) {
    char* szText = buf;
    // Read header information
    i = _stscanf(szText, _T("%s %d %d"), pO->szName, &pO->vListLen, &pO->sListLen);
    NEXTLINE(szText);
    //pO->world_pos = pos;
    // Read vertex information
    initObject3d(pO, pO->vListLen, pO->sListLen, 0);
    pO->maxRadius = 0;

    for (i = 0; i < pO->vListLen; i++) {
      float max_vertex;
      _stscanf(szText, _T("%f %f %f"), &pO->vList[i].coord.x, &pO->vList[i].coord.y, &pO->vList[i].coord.z);
      pO->vList[i].coord.w = 1.0f;
      max_vertex = MAX(ABS(pO->vList[i].coord.x), MAX(ABS(pO->vList[i].coord.y), ABS(pO->vList[i].coord.z)));
      NEXTLINE(szText);

      if (pO->maxRadius < max_vertex) {
        pO->maxRadius = max_vertex;
      }
    }

    for (i = 0; i < pO->sListLen; i++) {
      _TCHAR szTemp[16];
      int number_vertices;
      int poly_surface_descr = 0;
      BYTE shade;
      _stscanf(szText, _T("%s %d %d %d %d"), szTemp, &number_vertices, &pO->sList[i].vIndex[0], &pO->sList[i].vIndex[1], &pO->sList[i].vIndex[2]);
      NEXTLINE(szText);

      if ((szTemp[0] == '0') && (_totupper(szTemp[1]) == 'X')) {
        _stscanf(szTemp, _T("%X"), &poly_surface_descr);
      }
      else {
        poly_surface_descr = _ttoi(szTemp);
      }

      if (poly_surface_descr & 0x8000) {
        BYTE red = (poly_surface_descr & 0x0F00) >> 8;
        BYTE green = (poly_surface_descr & 0x00F0) >> 4;
        BYTE blue = poly_surface_descr & 0x000F;
        FRGBSET(pO->sList[i].clr, red << 4, green << 4, blue << 4);
      }
      else {
        DWORD clr = poly_surface_descr & 0x00FF;
        FRGBSET(pO->sList[i].clr, GetRV(clr), GetGV(clr), GetBV(clr));
      }

      shade = (poly_surface_descr & 0x6000) >> 13;
    }

    for (vertex = 0; vertex < pO->vListLen; vertex++) {
      pO->vList[vertex].coord.x /= pO->maxRadius * 2;
      pO->vList[vertex].coord.y /= pO->maxRadius * 2;
      pO->vList[vertex].coord.z /= pO->maxRadius * 2;
    }
  }

  free(buf);
  return n0;
}

typedef struct __MD2HEADER {
  int identifier;
  int version;
  int skin_width;
  int skin_height;
  int framesize;
  int num_skins;
  int num_verts;
  int num_textcoords;
  int num_polys;
  int num_openGLcmds;
  int num_frames;
  int offset_skins;
  int offset_textcoords;
  int offset_polys;
  int offset_frames;
  int offset_openGlcmds;
  int offset_end;
} _MD2HEADER;

typedef struct __MD2POLY {
  unsigned short vindex[3];
  unsigned short tindex[3];
} _MD2POLY;

typedef struct __MD2TEXTCOORD {
  short u;
  short v;
} _MD2TEXTCOORD;

typedef struct __MD2POINT {
  unsigned char v[3];
  unsigned char normal_index;
} _MD2POINT;

typedef struct __MD2FRAME {
  float scale[3];
  float translate[3];
  char name[16];
  _MD2POINT vlist[1];
} _MD2FRAME;

typedef enum __TEXTURE_FILTERING {
  _TEXTURE_FILTERING_NONE = 0x0000,
  _TEXTURE_FILTERING_BILINEAR = 0x0001
} _TEXTURE_FILTERING;

#define MD2_MAGIC_NUM ('I' + ('D' << 8) + ('P' << 16) + ('2' << 24))  // MD2 file magic number
#define MD2_VERSION   8

int extractFrame_MD2(Object3d* pO, int n, _MD2HEADER* md2_header, int md2_curr_frame)
{
  int vindex, pindex, tindex, n0 = n;
  uchar* lpData = (uchar*)md2_header;
  int frame = md2_curr_frame % md2_header->num_frames;
  _MD2FRAME* md2_frame = (_MD2FRAME*)(lpData + md2_header->offset_frames + md2_header->framesize * frame);
  _MD2POLY* md2_poly = (_MD2POLY*)(lpData + md2_header->offset_polys);
  float sx = md2_frame->scale[0];
  float sy = md2_frame->scale[1];
  float sz = md2_frame->scale[2];
  float tx = md2_frame->translate[0];
  float ty = md2_frame->translate[1];
  float tz = md2_frame->translate[2];

  if ((md2_header->identifier != MD2_MAGIC_NUM) || (md2_header->version != MD2_VERSION)) {
    return 0;
  }

  for (; n--; ++pO) {
    pO->maxRadius = 0;
    initObject3d(pO, md2_header->num_verts, md2_header->num_polys, md2_header->num_textcoords);

    for (tindex = 0; tindex < pO->tListLen; tindex++) {
      pO->tList[tindex].x = ((_MD2TEXTCOORD*)(lpData + md2_header->offset_textcoords))[tindex].u;
      pO->tList[tindex].y = ((_MD2TEXTCOORD*)(lpData + md2_header->offset_textcoords))[tindex].v;
    }

    for (vindex = 0; vindex < pO->vListLen; vindex++) {
      VECTOR3D v;
      float max_vertex, temp;
      v.x = (float)(md2_frame->vlist[vindex].v[0]) * sx + tx;
      v.y = (float)(md2_frame->vlist[vindex].v[1]) * sy + ty;
      v.z = (float)(md2_frame->vlist[vindex].v[2]) * sz + tz;
      v.w = 1.0f;

      SWAP(v.y, v.z, temp);

      pO->vList[vindex].coord = v;

      max_vertex = MAX(ABS(pO->vList[vindex].coord.x), MAX(ABS(pO->vList[vindex].coord.y), ABS(pO->vList[vindex].coord.z)));

      if (pO->maxRadius < max_vertex) {
        pO->maxRadius = max_vertex;
      }
    }

    for (pindex = 0; pindex < pO->sListLen; pindex++) {
      pO->sList[pindex].vIndex[0] = md2_poly[pindex].vindex[0];
      pO->sList[pindex].vIndex[1] = md2_poly[pindex].vindex[1];
      pO->sList[pindex].vIndex[2] = md2_poly[pindex].vindex[2];

      pO->sList[pindex].tIndex[0] = md2_poly[pindex].tindex[0];
      pO->sList[pindex].tIndex[1] = md2_poly[pindex].tindex[1];
      pO->sList[pindex].tIndex[2] = md2_poly[pindex].tindex[2];
    }
  }

  return n0;
}

//----------------------------------------------------
//Function Name : Read3DSTextureChunk
//Desc          : 从当前位置开始读取纹理块
//Input         ：file       : 用于定位的fstream文件对象的引用
//              : dwLength  : 块大小
//              : lpData    : 用于填充数据的对象
//Output        : 成功返回true，否则返回false
//Notice        : 当前文件指针必须指向纹理块的数据部分（不包括块头）
//                file，lpData的完备性由使用者保证
//----------------------------------------------------
int Read3DSTextureChunk(tMatInfo* pMaterial, tChunk* pPreChunk, char* buf)
{
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk;             /**< 读取过程中当前块 */

  /** 继续读入这些块 */
  while (buf < ebuf) {
    /** 读入下一块 */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** 判断读入的是什么块 */
    switch (m_CurrentChunk->ID) {
    case CHUNK_MAPFILENAME:            /**< 材质文件的名称 */
      /** 读入材质的文件名称 */
      memcpy(pMaterial->strTexture, buf, m_CurrentChunk->length - sizeof(tChunk));
      break;

    case CHUNK_OFFSETU:     //以下为局部纹理映射参数
      memcpy((char*) & (pMaterial->fOffsetU), buf, sizeof(float));
      break;

    case CHUNK_OFFSETV:
      memcpy((char*) & (pMaterial->fOffsetV), buf, sizeof(float));
      pMaterial->fOffsetV = -(pMaterial->fOffsetV);
      break;

    case CHUNK_TILINGU:
      memcpy((char*) & (pMaterial->fTilingU), buf, sizeof(float));
      break;

    case CHUNK_TILINGV:
      memcpy((char*) & (pMaterial->fTilingV), buf, sizeof(float));
      break;

    case CHUNK_ROTATEW:
      memcpy((char*) & (pMaterial->fRotationW), buf, sizeof(float));
      pMaterial->fRotationW = -(pMaterial->fRotationW);

    default:
      /** 跳过不需要读入的块 */
      printf("id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}

//----------------------------------------------------
//Function Name : Read3DSColorChunk
//Desc          : 从当前位置开始读取颜色块
//Input         ：file       : 用于定位的fstream文件对象的引用
//              : wID       :
//Output        : 成功返回true，否则返回false
//              : r         : 红色分量（float）
//              : g         : 绿………
//              : b         : 蓝………
//Notice        : 当前文件指针必须指向颜色的数据部分（不包括块头）
//                file的完备性由使用者保证
//----------------------------------------------------
int Read3DSColorChunk(float* rgb, char* buf)
{
  tChunk*       m_CurrentChunk;             /**< 读取过程中当前块 */
  m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

  if (m_CurrentChunk->ID == CHUNK_RGB1) {         //3 floats of rgb
    memcpy(rgb, buf, 3 * sizeof(float));
  }
  else if (m_CurrentChunk->ID == CHUNK_RGB2) {
    //3 bytes of rgb
    BYTE byRGB[4];
    memcpy(byRGB, buf, 3);
    rgb[0] = (float)byRGB[0] / 255.0f;
    rgb[1] = (float)byRGB[1] / 255.0f;
    rgb[2] = (float)byRGB[2] / 255.0f;
  }
  else {
    return 0;
  }

  return 1;
}
/** 处理所有的材质信息 */
int ReadNextMatChunk(tMatInfo* pMaterial, tChunk* pPreChunk, char* buf)
{
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk;             /**< 读取过程中当前块 */

  /** 继续读入这些块 */
  while (buf < ebuf) {
    /** 读入下一块 */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** 判断读入的是什么块 */
    switch (m_CurrentChunk->ID) {
    case CHUNK_MATNAME:             /**< 材质的名称 */
      /** 读入材质的名称 */
      memcpy(pMaterial->strName, buf, m_CurrentChunk->length - sizeof(tChunk));
      break;

    case CHUNK_AMBIENT:     //材质参数，下同
      //每个参量都是由RGB构成的
      Read3DSColorChunk(pMaterial->pAmbient, buf);
      break;

    case CHUNK_DIFFUSE:            /**< 对象的R G B颜色 */
      Read3DSColorChunk(pMaterial->pDiffuse, buf);
      break;

    case CHUNK_SPECULAR:
      Read3DSColorChunk(pMaterial->pSpecular, buf);
      break;

    case CHUNK_TEXTURE:              /**< 纹理信息的头部 */
      /** 下一个材质块信息 */
      Read3DSTextureChunk(pMaterial, m_CurrentChunk, buf);
      break;

    default:
      /** 跳过不需要读入的块 */
      //printf("id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}

/** 处理所有的文件中对象的信息 */
int ReadNextTrimesh(t3DObject* pObject, tChunk* pPreChunk, char* buf)
{
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk = (tChunk*)buf;           /**< 读取过程中当前块 */

  /** 继续读入块的内容直至本子块结束 */
  while (buf < ebuf) {
    /** 读入下一个块 */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** 区别读入是哪种块 */
    switch (m_CurrentChunk->ID) {
    case CHUNK_VERTLIST: {      /**< 读入是对象顶点 */
      /** 读入顶点的数目 */
      pObject->numOfVerts = *(unsigned short*)(buf);
      /** 读入顶点序列 */
      pObject->pVerts = (Vector3*)(buf + 2);
    }

    break;

    case CHUNK_FACELIST: {       /**< 读入的是对象的面 */
      /** 读入该对象中面的数目 */
      pObject->numOfFaces = *(unsigned short*)(buf);
      /** 分配所有面的存储空间，并初始化结构 */
      pObject->pFaces = (tFace*)(buf + 2);
    }

    break;

    case CHUNK_MAPLIST: {          /**< 读入对象的UV纹理坐标 */
      /** 读入UV坐标的数量 */
      pObject->numTexVertex = *(unsigned short*)(buf);
      /** 读入纹理坐标 */
      pObject->pTexVerts = (Vector2*)(buf + 2);
    }
    break;


    case CHUNK_FACEMAT: {      /**< 读入的是对象的材质名称 */
      char* strMaterial;      /**< 用来保存对象的材质名称 */
      /** 读入赋予当前对象的材质名称 */
      strMaterial = buf;
    }

    break;

    case CHUNK_TRMATRIX:
      //取得物体的中心坐标（目前没有用处）
      ASSERT(54 == m_CurrentChunk->length);
      memcpy(&pObject->vtxCenter, buf + 9 * sizeof(float), 3 * sizeof(float));
      break;

    default:
      /** 略过不需要读入的块 */
      //ASSERT(0);
      //printf("ReadNextObjChunk id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}
/** 处理所有的文件中对象的信息 */
int ReadNextObjChunk(t3DObject* pObject, tChunk* pPreChunk, char* buf)
{
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk = (tChunk*)buf;           /**< 读取过程中当前块 */

  /** 获得并保存对象的名称，然后增加读入的字节数 */
  strcpy(pObject->strName, buf);
  buf += strlen(buf) + 1;

  /** 继续读入块的内容直至本子块结束 */
  while (buf < ebuf) {
    /** 读入下一个块 */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** 区别读入是哪种块 */
    switch (m_CurrentChunk->ID) {
    case CHUNK_TRIMESH:          /**< 正读入的是一个新块 */
      /** 使用递归函数调用，处理该新块 */
      ReadNextTrimesh(pObject, m_CurrentChunk, buf);
      break;

    default:
      /** 略过不需要读入的块 */
      //ASSERT(0);
      //printf("ReadNextObjChunk id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}
/** 读出3ds文件的主要部分 */
int ReadNextChunk(t3DModel* pModel, tChunk* pPreChunk, char* buf)
{
  unsigned int version = 0;         /**< 保存文件版本 */
  char* ebuf = buf + pPreChunk->length - sizeof(tChunk);
  tChunk*       m_CurrentChunk;             /**< 读取过程中当前块 */

  /** 继续读入子块 */
  while (buf < ebuf) {
    /** 读入下一个块 */
    m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

    /** 判断块的ID号 */
    switch (m_CurrentChunk->ID) {
    case 0x3d3e:
      /** 读入下一个块 */
      /** 获得网格的版本号 */
      memcpy(&version, buf, m_CurrentChunk->length - sizeof(tChunk));
      break;

      /** 文件版本号 */
      /** 材质信息 */
    case CHUNK_MATERIAL:
      /** 进入材质装入函数 */
      ASSERT(pModel->numOfMaterials < MAX_MATERIALS);
      ReadNextMatChunk(pModel->pMaterials + pModel->numOfMaterials, m_CurrentChunk, buf);
      pModel->numOfMaterials++;
      break;

      /** 对象名称 */
    case CHUNK_OBJBLOCK:
      ASSERT(pModel->numOfMaterials < MAX_OBJECTS);
      /** 进入其余的对象信息的读入 */
      ReadNextObjChunk(pModel->pObject + pModel->numOfObjects, m_CurrentChunk, buf);
      pModel->numOfObjects++;
      break;

      /** 关键帧 */
    case CHUNK_KEYFRAMER:
      break;

    default:
      //ASSERT(0);
      //printf("ReadNextChunk id = %4x\n", m_CurrentChunk->ID);
      break;
    }

    buf += m_CurrentChunk->length - sizeof(tChunk);
  }

  return 0;
}
int load3DSTex(t3DModel* pModel, char* path)
{
  int i;
  char buf[256];

  for (i = 0; i < pModel->numOfMaterials; ++i) {
    if (path) {
      _snprintf(buf, 256, "%s/%s", path, pModel->pMaterials[i].strTexture);
      loadbmpfile(buf, 32, 1, pModel->pMaterials[i].tex);
    }
    else {
      if (pModel->pMaterials[i].tex) {
        free(pModel->pMaterials[i].tex);
      }
    }
  }

  return 0;
}
/** 载入3DS文件到模型结构中 */
int load3DS(t3DModel* pModel, char* buf)
{
  int n0 = 0;
  tChunk* m_CurrentChunk;
  memset(pModel, 0, sizeof(t3DModel));

  /**< 检查文件指针 */
  if (!buf) {
    return 0;
  }

  /** 将文件的第一块读出并判断是否是3ds文件 */
  m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

  /** 确保是3ds文件 */
  if (m_CurrentChunk->ID != CHUNK_MAIN) {
    MessageBox(NULL, "不能加载主块!", "Error", MB_OK);
    return 0;
  }

  m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

  if (CHUNK_VERSION == m_CurrentChunk->ID) {
    /** 读入文件的版本号 */
    unsigned int version = *(unsigned int*)(buf);
    memcpy(&version, buf, m_CurrentChunk->length - sizeof(tChunk));

    /** 如果文件版本号大于3，给出一个警告信息 */
    if (version > 0x03) {
      MessageBox(NULL, " 该3DS文件版本大于3.0,可能不能正确读取", "警告", MB_OK);
    }
  }

  buf += m_CurrentChunk->length - sizeof(tChunk);
  m_CurrentChunk = (tChunk*)buf, buf += sizeof(tChunk);

  if (CHUNK_OBJMESH == m_CurrentChunk->ID) {
    ReadNextChunk(pModel, m_CurrentChunk, buf);
  }

  return n0;
}
