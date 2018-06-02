// EMF图像读写模块实现文件
// 读取功能：  可读取增强型元文件（EMF）
// 保存功能：  不支持保存功能


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}



static int _verify_file(ISFILE* pfile)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned int  filesize;
  ENHMETAHEADER emh;

  __try {
    __try {
      /* 首先验证文件大小，不应该小于emf文件头的大小 */
      if (isio_seek(pfile, 0, SEEK_END) == -1
          || (filesize = isio_tell(pfile)) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (filesize < sizeof(emh)) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* 读取emf文件头 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 确定是文件头记录,并验证文件长度 */
      if (emh.iType != EMR_HEADER
          || emh.dSignature != ENHMETA_SIGNATURE
          || emh.nBytes > filesize) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      /* 验证版本号 */
      if (emh.nVersion != 0x10000) {
        b_status = ER_NONIMAGE;
        __leave;
      }
    }
    __finally {

    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



static int _get_info(ISFILE* pfile, LPINFOSTR pinfo_str)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  HDC       hdc = NULL;

  assert(pfile);
  assert(pinfo_str);

  __try {
    __try {
      hdc = GetDC(GetDesktopWindow());

      if (hdc == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      /* 设置图像宽、高度 */
      {
        int width, height, t;
        ENHMETAHEADER emh;

        if (isio_seek(pfile, 0, SEEK_SET) == -1
            || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
          b_status = ER_FILERWERR;
          __leave;
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
      pinfo_str->imgtype     = IMT_VECTORSTATIC;
      pinfo_str->imgformat   = IMF_EMF;
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
    __finally {
      if (hdc != NULL) {
        ReleaseDC(GetDesktopWindow(), hdc);
      }
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}

static int _load_enh_metafile(ISFILE* pfile, HENHMETAFILE* phout)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  ENHMETAHEADER emh;
  unsigned char*  embits = NULL;

  assert(pfile);

  __try {
    __try {
      /* 读取emf文件头，获得文件大小 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(&emh, sizeof(emh), 1, pfile) != 1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读取文件内容，创建emf handle */
      embits = (unsigned char*)isirw_malloc(emh.nBytes);

      if (embits == NULL) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      if (isio_seek(pfile, 0, SEEK_SET) == -1
          || isio_read(embits, 1, emh.nBytes, pfile) != emh.nBytes) {
        b_status = ER_FILERWERR;
        __leave;
      }

      *phout = SetEnhMetaFileBits(emh.nBytes, embits);

      if (*phout == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

    }
    __finally {
      if (embits != NULL) {
        isirw_free(embits);
      }

      if (b_status != ER_SUCCESS || AbnormalTermination()) {
        *phout = NULL;
      }

    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}



static int _enh_meta_to_raster(HENHMETAFILE hemf, LPINFOSTR pinfo_str)
{
  enum EXERESULT  b_status = ER_SUCCESS;

  unsigned char* dib_bits = NULL;
  HDC       hdc = NULL, hComDC = NULL;
  HBITMAP     bmp = NULL, oldbmp = NULL;
  BITMAPINFO    bmi;
  RECT      rect;
  int       linesize;
  unsigned    i;
  unsigned char** ppt;

  assert(hemf);
  assert(pinfo_str);

  __try {
    __try {
      /* 创建与桌面兼容的DC */
      if ((hdc = GetDC(GetDesktopWindow())) == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      memset(&bmi, 0, sizeof(bmi));
      bmi.bmiHeader.biSize          = sizeof(bmi.bmiHeader);
      bmi.bmiHeader.biWidth         = pinfo_str->width;
      bmi.bmiHeader.biHeight        = pinfo_str->height;
      bmi.bmiHeader.biPlanes        = 1;
      bmi.bmiHeader.biBitCount      = (WORD)pinfo_str->bitcount;
      bmi.bmiHeader.biCompression   = BI_RGB;
      bmi.bmiHeader.biXPelsPerMeter = GetDeviceCaps(hdc, HORZRES) * 1000 / GetDeviceCaps(hdc, HORZSIZE);
      bmi.bmiHeader.biYPelsPerMeter = GetDeviceCaps(hdc, VERTRES) * 1000 / GetDeviceCaps(hdc, VERTSIZE);

      /* 创建位图句柄 */
      bmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &dib_bits, NULL, 0);

      if (bmp == NULL) {
        b_status = ER_SYSERR;
        __leave;
      }

      /* 创建兼容DC */
      if ((hComDC = CreateCompatibleDC(hdc)) == NULL) {
        b_status = ER_SYSERR;
        __leave;
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

      if ((pinfo_str->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4)) == NULL
          || (pinfo_str->pp_line_addr = isirw_malloc(sizeof(unsigned char*)*pinfo_str->height)) == NULL) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 将象素数据拷贝至数据包 */
      memcpy(pinfo_str->p_bit_data, dib_bits, linesize * pinfo_str->height);

      /* 填写行首地址数组 */
      ppt = (unsigned char**)pinfo_str->pp_line_addr;

      for (i = 0; i < pinfo_str->height; i++) {
        ppt[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
      }

      /* 填写数据包的其他域 */
      pinfo_str->pal_count  = 0;
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg    = NULL;
    }
    __finally {
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
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}




/* 读取图像位数据 */
EMF_API int CALLAGREEMENT emf_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
# ifdef WIN32
  ISFILE*      pfile = (ISFILE*)0;

  enum EXERESULT  b_status = ER_SUCCESS;

  HENHMETAFILE hemf;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&emf_load_img_critical);

      /* 调用系统回调函数 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 打开流 */
      if ((pfile = isio_open((const char*)psct, "rb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 读文件头结构 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 验证emf文件的有效性 */
      if ((b_status = _verify_file(pfile)) != ER_SUCCESS) {
        __leave;
      }

      /* 如果是空的数据包，首先获取图像概要信息，成功后再读取图像 */
      if (pinfo_str->data_state == 0) {
        b_status = _get_info(pfile, pinfo_str);

        if (b_status != ER_SUCCESS) {
          __leave;
        }

        pinfo_str->data_state = 1;
      }

      assert(pinfo_str->data_state == 1);

      /* 读取emf数据 */
      if ((b_status = _load_enh_metafile(pfile, &hemf)) != ER_SUCCESS) {
        __leave;
      }

      assert(hemf);

      /* 转换至数据包 */
      if ((b_status = _enh_meta_to_raster(hemf, pinfo_str)) != ER_SUCCESS) {
        __leave;
      }

      /* 回调通知 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 一次行进度调用 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      /* 主图象进度结束 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
      case  0:
        break;

      case  1:
        b_status = ER_USERBREAK;
        __leave;
        break;

      case  2:
        b_status = ER_SYSERR;
        __leave;
        break;

      case  3:
        b_status = ER_MEMORYERR;
        __leave;
        break;

      default:
        assert(0);
        b_status = ER_SYSERR;
        __leave;
        break;
      }

      pinfo_str->data_state = 2;
    }
    __finally {
      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        if (pinfo_str->p_bit_data) {
          isirw_free(pinfo_str->p_bit_data);
          pinfo_str->p_bit_data = (unsigned char*)0;
        }

        if (pinfo_str->pp_line_addr) {
          isirw_free(pinfo_str->pp_line_addr);
          pinfo_str->pp_line_addr = (void**)0;
        }

        if (pinfo_str->data_state == 2) {
          pinfo_str->data_state = 1;  /* 自动降级 */
        }

        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
      }
      else {
        switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD)) {
        case  0:
          break;

        case  1:
          b_status = ER_USERBREAK;
          break;

        case  2:
          b_status = ER_SYSERR;
          break;

        case  3:
          b_status = ER_MEMORYERR;
          break;

        default:
          assert(0);
          b_status = ER_SYSERR;
          break;
        }

        if (b_status != ER_SUCCESS) {
          if (pinfo_str->p_bit_data) {
            isirw_free(pinfo_str->p_bit_data);
            pinfo_str->p_bit_data = (unsigned char*)0;
          }

          if (pinfo_str->pp_line_addr) {
            isirw_free(pinfo_str->pp_line_addr);
            pinfo_str->pp_line_addr = (void**)0;
          }

          if (pinfo_str->data_state == 2) {
            pinfo_str->data_state = 1;  /* 自动降级 */
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&emf_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

# else

  b_status = ER_NOTSUPPORT;

# endif /* WIN32 */

  return (int)b_status;
}

