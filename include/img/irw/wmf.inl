/********************************************************************
 wmf.c
 本文件用途： WMF图像读写模块实现文件
 读取功能： 可读取Windows标准元文件、Aldus可放置元文件。
 保存功能： 不支持保存功能。
 本文件编写人： YZ （第一版）
 swstudio swstudio##sohu.com（第二版）
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2。引入内存防护概念。
 2002-6 修正新版插件的两处缺陷
 2002-5 第二版插件发布（新版）
 2001-1 修正部分注释信息
 2000-7 修正多处BUG，并加强了模块的容错性
 2000-6 第一个版本发布
********************************************************************/



/* 文件结构定义 */
#define ALDUSKEY        0x9AC6CDD7
#define ALDUSMETAHEADERSIZE   22

typedef struct {
  unsigned int  key;        /* 必需等于ALDUSKEY */
  ushort  hmf;
  short     left;
  short     top;
  short     right;
  short     bottom;
  ushort  inch;
  unsigned int  reserved;
  ushort  checksum;
} ALDUSMETAHEADER;

/* 内部助手函数 */
int _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */);
static int _verify_file(ISFILE* pfile);
static int _get_info(ISFILE* pfile, INFOSTR* pinfo_str);
static int _load_metafile(ISFILE* pfile, HMETAFILE* phout);
static int _meta_to_raster(HMETAFILE hwmf, INFOSTR* pinfo_str);
static int CALLBACK _EnumMetaFileProc(HDC hDC, HANDLETABLE* lpHTable, METARECORD* lpMFR, int nObj, LPARAM lpClientData);
#ifdef WIN32
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call) {
  case DLL_PROCESS_ATTACH:
    /* 初始化插件信息表 */
    _init_irwp_info(&wmf_irwp_info);
    /* 初始化访问关键段 */
    InitializeCriticalSection(&wmf_get_info_critical);
    InitializeCriticalSection(&wmf_load_img_critical);
    InitializeCriticalSection(&wmf_save_img_critical);
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    /* 销毁访问关键段 */
    DeleteCriticalSection(&wmf_get_info_critical);
    DeleteCriticalSection(&wmf_load_img_critical);
    DeleteCriticalSection(&wmf_save_img_critical);
    break;
  }
  return TRUE;
}
#endif /* WIN32 */
#ifdef WIN32
WMF_API LP { //IRWP_INFO is_irw_plugin()
  return (LPIRWP_INFO)& wmf_irwp_info;
}
#else
WMF_API LP { //IRWP_INFO wmf_get_plugin_info()
  _init_irwp_info(&wmf_irwp_info);
  return (LPIRWP_INFO)& wmf_irwp_info;
}
WMF_API void wmf_init_plugin()
{
  /* 初始化多线程同步对象 */
}
WMF_API void wmf_detach_plugin()
{
  /* 销毁多线程同步对象 */
}
#endif /* WIN32 */
/* 初始化插件信息结构 */
void _init_irwp_info(LP { //IRWP_INFO lpirwp_info)
  assert(lpirwp_info);
  /* 初始化结构变量 */
  memset((void*)lpirwp_info, 0, sizeof(IRWP_INFO));
  /* 版本号。（十进制值，十位为主版本号，个位为副版本，*/
  lpirwp_info->irwp_version = MODULE_BUILDID;
  /* 插件名称 */
  strcpy((char*)(lpirwp_info->irwp_name), MODULE_NAME);
  /* 本模块函数前缀 */
  strcpy((char*)(lpirwp_info->irwp_func_prefix), MODULE_FUNC_PREFIX);
  /* 填写插件标识 */
  lpirwp_info->plug_id = IMF_WMF;
  /* 插件的发布类型。0－调试版插件，1－发布版插件 */
#ifdef _DEBUG
  lpirwp_info->irwp_build_set = 0;
#else
  lpirwp_info->irwp_build_set = 1;
#endif
  /* 功能标识 （##需手动修正） */
  lpirwp_info->irwp_function = IRWP_READ_SUPP;
  /* 设置模块支持的保存位深 */
  lpirwp_info->irwp_save.bitcount = 0;
  lpirwp_info->irwp_save.img_num = 0;
  lpirwp_info->irwp_save.count = 0;
  /* 开发者人数（即开发者信息中有效项的个数）（##需手动修正）*/
  lpirwp_info->irwp_author_count = 2;
  /* 开发者信息（##需手动修正） */
  /* ---------------------------------[0] － 第一组 -------------- */
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_name),
  (const char*)"YZ");
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_email),
  (const char*)"");
  strcpy((char*)(lpirwp_info->irwp_author[0].ai_message),
  (const char*)"很高兴能在自由软件开发组作矢量图的工作，这使我学到了不少元文件的知识^_^");
  /* ---------------------------------[1] － 第二组 -------------- */
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_name),
  (const char*)"swstudio");
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_email),
  (const char*)"swstudio##sohu.com");
  strcpy((char*)(lpirwp_info->irwp_author[1].ai_message),
  (const char*)"GDI的坐标系统真faint:)");
  /* ---------------------------------[2] － 第三组 -------------- */
  /* 后续开发者信息可加在此处。
  strcpy((char*)(lpirwp_info->irwp_author[2].ai_name),
  (const char *)"");
  strcpy((char*)(lpirwp_info->irwp_author[2].ai_email),
  (const char *)"@");
  strcpy((char*)(lpirwp_info->irwp_author[2].ai_message),
  (const char *)":)");
  */
  /* ------------------------------------------------------------- */
  /* 插件描述信息（扩展名信息）*/
  strcpy((char*)(lpirwp_info->irwp_desc_info.idi_currency_name),
  (const char*)MODULE_FILE_POSTFIX);
  lpirwp_info->irwp_desc_info.idi_rev = 0;
  /* 别名个数（##需手动修正） */
  lpirwp_info->irwp_desc_info.idi_synonym_count = 1;
  strcpy((char*)(lpirwp_info->irwp_desc_info.idi_synonym[0]),
  (const char*)"APM");
  /* 设置初始化完毕标志 */
  lpirwp_info->init_tag = 1;
  return;
}
/* 获取图像信息 */
EXERESULT wmf_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
# ifdef WIN32
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
  do {
    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 验证wmf的正确性 */
      if ((b_status = _verify_file(pfile)) != ER_SUCCESS) {
        break;
      }
      if ((b_status = _get_info(pfile, pinfo_str)) != ER_SUCCESS) {
        break;
      }
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
    {
      //LeaveCriticalSection(&wmf_get_info_critical);
    }
  }
# else
  b_status = ER_NOTSUPPORT;
# endif /* WIN32 */
  return b_status;
}
/* 读取图像位数据 */
EXERESULT wmf_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
# ifdef WIN32
  enum EXERESULT b_status = ER_SUCCESS;
  HMETAFILE hwmf = NULL;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  do {
      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 验证wmf文件的有效性 */
      if ((b_status = _verify_file(pfile)) != ER_SUCCESS) {
        break;
      }
      /* 如果是空的数据包，首先获取图像概要信息，成功后再读取图像 */
      if (pinfo_str->data_state == 0) {
        b_status = _get_info(pfile, pinfo_str);
        if (b_status != ER_SUCCESS) {
          break;
        }
        pinfo_str->data_state = 1;
      }
      assert(pinfo_str->data_state == 1);
      /* 读入wmf数据 */
      if ((b_status = _load_metafile(pfile, &hwmf)) != ER_SUCCESS) {
        break;
      }
      assert(hwmf);
      /* 转至数据包 */
      if ((b_status = _meta_to_raster(hwmf, pinfo_str)) != ER_SUCCESS) {
        break;
      }
      /* 回调通知 */
       
      /* 一次行进度调用 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) 
      /* 主图象进度结束 */
       
      pinfo_str->data_state = 2;
    }
    while (0);
    {
      if (hwmf) {
        DeleteMetaFile(hwmf);
      }
      if (b_status != ER_SUCCESS) {
        if (pinfo_str->p_bit_data) {
          isirw_free(pinfo_str->p_bit_data);
          pinfo_str->p_bit_data = (uchar*)0;
        }
        if (pinfo_str->pp_line_addr) {
          isirw_free(pinfo_str->pp_line_addr);
          pinfo_str->pp_line_addr = (uchar**)0;
        }
        if (pinfo_str->data_state == 2) {
          pinfo_str->data_state = 1; /* 自动降级 */
        }
        
      }
      else {
        if (b_status != ER_SUCCESS) {
          if (pinfo_str->p_bit_data) {
            isirw_free(pinfo_str->p_bit_data);
            pinfo_str->p_bit_data = (uchar*)0;
          }
          if (pinfo_str->pp_line_addr) {
            isirw_free(pinfo_str->pp_line_addr);
            pinfo_str->pp_line_addr = (uchar**)0;
          }
          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1; /* 自动降级 */
          }
          
        }
      }
      //LeaveCriticalSection(&wmf_load_img_critical);
    }
  }
# else
  b_status = ER_NOTSUPPORT;
# endif /* WIN32 */
  return b_status;
}
/* 保存图像 */
EXERESULT wmf_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  return ER_NOTSUPPORT;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 判断指定图象是否是WMF */
static int _verify_file(ISFILE* pfile)
{
  enum EXERESULT b_status = ER_SUCCESS;
  unsigned int is_aldus, filesize, wmf_offset = 0;
  METAHEADER meta_hdr;

    do {
      /* 首先验证文件长度，不应该小于METAHEADER结构，外加一个6字节的尾记录长度 */
      if (isio_seek(pfile, 0, SEEK_END) == -1
      || (filesize = isio_tell(pfile)) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (filesize < (sizeof(METAHEADER) + 6)) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 读取文件的头四字节，判断是不是placeable元文件 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
      || isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (is_aldus == ALDUSKEY) {
        ALDUSMETAHEADER aldus_hdr;
        WORD* pw;
        WORD checksum = 0;
        /* 如果是，再次验证文件长度 */
        if (filesize < (ALDUSMETAHEADERSIZE + sizeof(METAHEADER) + 6)) {
          b_status = ER_BADIMAGE;
          break;
        }
        /* 读取placeable元文件头，并作代码和检查。 */
        if (isio_seek(pfile, 0, SEEK_SET) == -1
        || isio_read(&aldus_hdr, ALDUSMETAHEADERSIZE, 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          break;
        }
        for (pw = (WORD*)&aldus_hdr; pw < (WORD*)&aldus_hdr.checksum; pw++) {
          checksum ^= *pw;
        }
        /* 如失败，表示文件已损坏 */
        if (checksum != aldus_hdr.checksum) {
          b_status = ER_BADIMAGE;
          break;
        }
        wmf_offset = ALDUSMETAHEADERSIZE;
      }
      /* 读取windows标准元文件头，其中记录有元文件大小（不包括placeable元文件
      头在内，以WORD为单位），元文件长度不应该小于此 */
      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
      || isio_read(&meta_hdr, sizeof(METAHEADER), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (filesize < (meta_hdr.mtSize * 2 - wmf_offset)) {
        b_status = ER_BADIMAGE;
        break;
      }
    }
    while (0);
  return b_status;
}
/* 获取图象信息 */
static int _get_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
  unsigned int is_aldus;
  HDC hdc = NULL;
  assert(pfile);
  assert(pinfo_str);
  do {
    do {
      hdc = GetDC(GetDesktopWindow());
      if (hdc == NULL) {
        b_status = ER_SYSERR;
        break;
      }
      /* 设置图像宽、高度，标准wmf文件未记录相关信息，默认为屏幕宽、高度 */
      pinfo_str->width = GetDeviceCaps(hdc, HORZRES);
      pinfo_str->height = GetDeviceCaps(hdc, VERTRES);
      /* 设置图像为倒向放置 */
      pinfo_str->order = 1;
      /* 设置位深度,由于wmf文件并未记录相关信息，直接设定为24bit */
      pinfo_str->bitcount = 24;
      /* 设置各颜色分量掩码 */
      pinfo_str->r_mask = 0xff0000;
      pinfo_str->g_mask = 0xff00;
      pinfo_str->b_mask = 0xff;
      pinfo_str->a_mask = 0x0;
      /* 如果是可放置元文件，从文件头中获得宽度、高度 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (is_aldus == ALDUSKEY) {
        ALDUSMETAHEADER hdr;
        float fx, fy;
        if (isio_seek(pfile, 0, SEEK_SET) == -1) {
          b_status = ER_FILERWERR;
          break;
        }
        if (isio_read(&hdr, ALDUSMETAHEADERSIZE, 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 首先计算图像的单位尺寸 */
        fx = (float)(hdr.right - hdr.left);
        fy = (float)(hdr.bottom - hdr.top);
        /* hdr.inch表示一英寸相当于多少单位，由此计算出以英寸表示的图像尺寸，
        再乘以屏幕的DPI值，从而得到图像以象素为单位的尺寸*/
        fx = (fx / (float)hdr.inch) * (float)GetDeviceCaps(hdc, LOGPIXELSX) + 0.5f;
        fy = (fy / (float)hdr.inch) * (float)GetDeviceCaps(hdc, LOGPIXELSY) + 0.5f;
        pinfo_str->width = (ulong)fx;
        pinfo_str->height = (ulong)fy;
      }
      /* 设置图像类型、存储格式和数据压缩方式 */
      pinfo_str->imgtype = IMT_VECTORSTATIC;
      pinfo_str->imgformat = is_aldus == ALDUSKEY ? IMF_APM : IMF_WMF;
      pinfo_str->compression = ICS_GDIRECORD;
    }
    while (0);
    {
      if (hdc != NULL) {
        ReleaseDC(GetDesktopWindow(), hdc);
      }
    }
  }
  return b_status;
}
/* 读WMF数据，并创建HMETAFILE句柄 */
static int _load_metafile(ISFILE* pfile, HMETAFILE* phout)
{
  enum EXERESULT b_status = ER_SUCCESS;
  METAHEADER meta_hdr;
  ulong is_aldus;
  uchar* meta_bits = NULL;
  int wmf_offset = 0;
  assert(pfile);
  do {
    do {
      /* 读取文件?繁晔?*/
      if (isio_seek(pfile, 0, SEEK_SET) == -1
      || isio_read(&is_aldus, sizeof(is_aldus), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 如果是可放置元文件（APM），跳过APM文件头 */
      if (is_aldus == ALDUSKEY) {
        wmf_offset = ALDUSMETAHEADERSIZE;
      }
      /* 读入元文件数据，创建元文件handle */
      memset(&meta_hdr, 0, sizeof(meta_hdr));
      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
      || isio_read(&meta_hdr, sizeof(meta_hdr), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      meta_bits = (uchar*)isirw_malloc(2 * meta_hdr.mtSize);
      if (meta_bits == NULL) {
        b_status = ER_MEMORYERR;
        break;
      }
      if (isio_seek(pfile, wmf_offset, SEEK_SET) == -1
      || isio_read(meta_bits, 2, meta_hdr.mtSize, pfile) != meta_hdr.mtSize) {
        b_status = ER_FILERWERR;
        break;
      }
      *phout = SetMetaFileBitsEx(2 * meta_hdr.mtSize, meta_bits);
      if (*phout == NULL) {
        b_status = ER_SYSERR;
        break;
      }
    }
    while (0);
    {
      if (meta_bits != NULL) {
        isirw_free(meta_bits);
      }
      if (b_status != ER_SUCCESS || 1) {
        *phout = NULL;
      }
    }
  }
  return b_status;
}
/* 将WMF数据转至数据包（矢量至光栅）*/
static int _meta_to_raster(HMETAFILE hwmf, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
  uchar* dib_bits = NULL;
  HDC hdc = NULL, hComDC = NULL;
  HBITMAP bmp = NULL, oldbmp = NULL;
  BITMAPINFO bmi;
  RECT rect;
  int linesize;
  unsigned i;
  uchar** ppt;
  assert(hwmf);
  assert(pinfo_str);
  do {
    do {
      /* 创建与桌面兼容的DC，将元文件播放到该DC */
      if ((hdc = GetDC(GetDesktopWindow())) == NULL) {
        b_status = ER_SYSERR;
        break;
      }
      memset(&bmi, 0, sizeof(bmi));
      bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
      bmi.bmiHeader.biWidth = pinfo_str->width;
      bmi.bmiHeader.biHeight = pinfo_str->height;
      bmi.bmiHeader.biPlanes = 1;
      bmi.bmiHeader.biBitCount = (WORD)pinfo_str->bitcount;
      bmi.bmiHeader.biCompression = BI_RGB;
      bmi.bmiHeader.biXPelsPerMeter = GetDeviceCaps(hdc, HORZRES) * 1000 / GetDeviceCaps(hdc, HORZSIZE);
      bmi.bmiHeader.biYPelsPerMeter = GetDeviceCaps(hdc, VERTRES) * 1000 / GetDeviceCaps(hdc, VERTSIZE);
      /* 创建位图句柄 */
      bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &dib_bits, NULL, 0);
      if (bmp == NULL) {
        b_status = ER_SYSERR;
        break;
      }
      /* 创建兼容DC */
      if ((hComDC = CreateCompatibleDC(hdc)) == NULL) {
        b_status = ER_SYSERR;
        break;
      }
      oldbmp = SelectObject(hComDC, bmp);
      /* 将图象背景填充为白色 */
      rect.left = rect.top = 0;
      rect.right = pinfo_str->width;
      rect.bottom = pinfo_str->height;
      FillRect(hComDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
      /* 向兼容DC播放WMF数据 */
      SetMapMode(hComDC, MM_ANISOTROPIC);
      EnumMetaFile(hComDC, hwmf, _EnumMetaFileProc, (LPARAM)pinfo_str);
      SelectObject(hComDC, oldbmp);
      /* 利用得到的位图数据填充数据包相关域 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* 申请象素缓冲区 */
      if ((pinfo_str->p_bit_data = (uchar*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4)) == NULL
      || (pinfo_str->pp_line_addr = isirw_malloc(sizeof(uchar*)*pinfo_str->height)) == NULL) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 将象素数据拷贝至数据包 */
      memcpy(pinfo_str->p_bit_data, dib_bits, linesize * pinfo_str->height);
      /* 填写行首地址数组 */
      ppt = (uchar**)pinfo_str->pp_line_addr;
      for (i = 0; i < pinfo_str->height; i++) {
        ppt[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }
      /* 填写数据包的其他域 */
      pinfo_str->pal_count = 0;
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg = NULL;
    }
    while (0);
    {
      if (hdc != NULL) {
        ReleaseDC(GetDesktopWindow(), hdc);
      }
      if (hComDC != NULL) {
        DeleteDC(hComDC);
      }
      if (bmp != NULL) {
        DeleteObject(bmp);
      }
    }
  }
  return b_status;
}
/* WMF播放过程回调函数 */
static int CALLBACK _EnumMetaFileProc(HDC hDC, HANDLETABLE* lpHTable, METARECORD* lpMFR, int nObj, LPARAM lpClientData)
{
  static int viewport_ext_set = 0;
  INFOSTR* pinfo_str = (INFOSTR*)lpClientData;
  PlayMetaFileRecord(hDC, lpHTable, lpMFR, nObj);
  switch (lpMFR->rdFunction) {
  case 0x20e: /* SetViewportExt() */
    viewport_ext_set = !0;
    break;
  case 0x20c: /* SetWindowExt() */
    /* 如果元文件在设定窗口坐标(逻辑坐标）范围时，此前没有修改
    视坐标（设备坐标）范围，我们就自行设定。否则什么也不做，
    以防修改文件的自身设置 */
    if (!viewport_ext_set) {
      HDC hdc = GetDC(GetDesktopWindow());
      unsigned int cx, cy;
      float ratio;
      switch (pinfo_str->imgformat) {
      case IMF_APM:
        cx = pinfo_str->width;
        cy = pinfo_str->height;
        break;
        /* 如果是标准元文件，调整视坐标范围，使设备坐标纵横比
        与逻辑坐标一致，并且不超出图象的宽、高尺寸 */
      case IMF_WMF:
        cx = pinfo_str->width;
        cy = pinfo_str->height;
        /* ! 注意这里计算纵横比（y/x)的方法: 在元文件记录中，参数是
        与以函数形参表相反的顺序储存的，并且不储存hdc，因此对于
        SetWindowExt(),rdParm[0]储存Y范围，rdParm[1]储存X范围。*/
        ratio = (float)lpMFR->rdParm[0] / (float)lpMFR->rdParm[1];
        cy = (unsigned int)((float)cx * ratio);
        if (cy > pinfo_str->height) {
          cy = pinfo_str->height;
          cx = (unsigned int)((float)cy / ratio);
        }
        break;
      default:
        assert(0);
      }
      SetViewportExtEx(hDC, cx, cy, NULL);
      ReleaseDC(GetDesktopWindow(), hdc);
    }
    break;
  default:
    break;
  }
  return !0;
}

