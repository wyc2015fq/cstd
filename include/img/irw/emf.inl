/********************************************************************
 emf.c
 本文件用途： EMF图像读写模块实现文件
 读取功能： 可读取增强型元文件（EMF）
 保存功能： 不支持保存功能
 本文件编写人： YZ yzfree##yeah.net
 swstudio swstudio##sohu.com
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2。引入内存防护概念。
 2002-6 第二版插件发布（新版）
 2001-1 修正部分注释信息
 2000-7 修正多处BUG，并加强了模块的容错性
 2000-6 第一个版本发布
********************************************************************/

/* 内部助手函数 */
static int _verify_file(ISFILE* pfile);
static int _get_info(ISFILE* pfile, INFOSTR* pinfo_str);
static int _load_enh_metafile(ISFILE* pfile, HENHMETAFILE* phout);
static int _enh_meta_to_raster(HENHMETAFILE hemf, INFOSTR* pinfo_str);

/* 获取图像信息 */
EXERESULT emf_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
# ifdef WIN32
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */

    do {
      /* 读取文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
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
  
# else
  b_status = ER_NOTSUPPORT;
# endif /* WIN32 */
  return b_status;
}
/* 读取图像位数据 */
EXERESULT emf_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
# ifdef WIN32
  enum EXERESULT b_status = ER_SUCCESS;
  HENHMETAFILE hemf;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  do {
    do {
      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 验证emf文件的有效性 */
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
      /* 读取emf数据 */
      if ((b_status = _load_enh_metafile(pfile, &hemf)) != ER_SUCCESS) {
        break;
      }
      assert(hemf);
      /* 转换至数据包 */
      if ((b_status = _enh_meta_to_raster(hemf, pinfo_str)) != ER_SUCCESS) {
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
      //LeaveCriticalSection(&emf_load_img_critical);
    }
  }
# else
  b_status = ER_NOTSUPPORT;
# endif /* WIN32 */
  return b_status;
}
/* 保存图像 */
EXERESULT emf_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  return ER_NOTSUPPORT;
}
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */
/* 计算扫描行尺寸(四字节对齐) */
int _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}
static int _verify_file(ISFILE* pfile)
{
  enum EXERESULT b_status = ER_SUCCESS;
  unsigned int filesize;
  ENHMETAHEADER emh;
  do {
    do {
      /* 首先验证文件大小，不应该小于emf文件头的大小 */
      if (isio_seek(pfile, 0, SEEK_END) == -1
      || (filesize = isio_tell(pfile)) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      if (filesize < sizeof(emh)) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 读取emf文件头 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
      || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 确定是文件头记录,并验证文件长度 */
      if (emh.iType != EMR_HEADER
          || emh.dSignature != ENHMETA_SIGNATURE
      || emh.nBytes > filesize) {
        b_status = ER_BADIMAGE;
        break;
      }
      /* 验证版本号 */
      if (emh.nVersion != 0x10000) {
        b_status = ER_NONIMAGE;
        break;
      }
    }
    while (0);
    {
    }
  }
  return b_status;
}
static int _get_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  enum EXERESULT b_status = ER_SUCCESS;
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
      /* 设置图像宽、高度 */
      {
        int width, height, t;
        ENHMETAHEADER emh;
        if (isio_seek(pfile, 0, SEEK_SET) == -1
        || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          break;
        }
        /* 获得以 0.01 mm 为单位的图像尺寸 */
        width = emh.rclFrame.right - emh.rclFrame.left;
        height = emh.rclFrame.bottom - emh.rclFrame.top;
        /* 转换为以像素为单位的尺寸 */
        t = GetDeviceCaps(hdc, HORZSIZE) * 100;
        width = (width * GetDeviceCaps(hdc, HORZRES) + t / 2) / t;
        t = GetDeviceCaps(hdc, VERTSIZE) * 100;
        height = (height * GetDeviceCaps(hdc, VERTRES) + t / 2) / t;
        pinfo_str->width = width;
        pinfo_str->height = height;
      }
      /* 设置图像类型、存储格式和数据压缩方式 */
      pinfo_str->imgtype = IMT_VECTORSTATIC;
      pinfo_str->imgformat = IMF_EMF;
      pinfo_str->compression = ICS_GDIRECORD;
      /* 设置图像为倒向放置 */
      pinfo_str->order = 1;
      /* 设置位深度，直接设定为24bit */
      pinfo_str->bitcount = 24;
      /* 设置各颜色分量掩码 */
      pinfo_str->r_mask = 0xff0000;
      pinfo_str->g_mask = 0xff00;
      pinfo_str->b_mask = 0xff;
      pinfo_str->a_mask = 0x0;
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
static int _load_enh_metafile(ISFILE* pfile, HENHMETAFILE* phout)
{
  enum EXERESULT b_status = ER_SUCCESS;
  ENHMETAHEADER emh;
  uchar* embits = NULL;
  assert(pfile);
  do {
    do {
      /* 读取emf文件头，获得文件大小 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
      || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 读取文件内容，创建emf handle */
      embits = (uchar*)isirw_malloc(emh.nBytes);
      if (embits == NULL) {
        b_status = ER_MEMORYERR;
        break;
      }
      if (isio_seek(pfile, 0, SEEK_SET) == -1
      || isio_read(embits, 1, emh.nBytes, pfile) != emh.nBytes) {
        b_status = ER_FILERWERR;
        break;
      }
      *phout = SetEnhMetaFileBits(emh.nBytes, embits);
      if (*phout == NULL) {
        b_status = ER_SYSERR;
        break;
      }
    }
    while (0);
    {
      if (embits != NULL) {
        isirw_free(embits);
      }
      if (b_status != ER_SUCCESS || 1) {
        *phout = NULL;
      }
    }
  }
  return b_status;
}
static int _enh_meta_to_raster(HENHMETAFILE hemf, INFOSTR* pinfo_str)
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
  assert(hemf);
  assert(pinfo_str);
  do {
    do {
      /* 创建与桌面兼容的DC */
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
      /* 向兼容DC播放emf数据 */
      PlayEnhMetaFile(hComDC, hemf, &rect);
      /* 利用得到的位图数据填充数据包相关域 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
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

