/********************************************************************
 jbig.c
 引用代码编写人及版权信息：
 ----------------------------------------------------------------
 代码类别： JBIG编解代码库(jbig kit)
 相关文件： libjbig.h、libjbig.c、libjbigtab.c、libjbig.txt
 使用许可： GPL
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2、引入内存防护概念。
 2002-5 第一个发布版（新版）
********************************************************************/

#include "libjbig.inl"
#include "libjbigtab.inl"
#define BUFFER_SIZE (32*1024) /* 读数据缓冲区尺寸（32KB）*/

/* 获取图像信息 */
EXERESULT jbig_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  JBG_DEC_STATE jds;
  uchar* buf = 0, *p;
  int len, result;
  size_t cnt;
  ulong xmax = 4294967295UL;
  ulong ymax = 4294967295UL;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */

    do {
      jbg_dec_init(&jds);
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 申请读数据缓冲区 */
      if ((buf = (uchar*)isirw_malloc(BUFFER_SIZE)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 设定图象最大限长 */
      jbg_dec_maxsize(&jds, xmax, ymax);
      result = JBG_EAGAIN;
      do {
        /* 分段读取压缩数据 */
        if ((len = isio_read((void*)buf, 1, BUFFER_SIZE, pfile)) == 0) {
          if ((result == JBG_EOK_INTR) || (result == JBG_EOK)) {
            break;
          }
          else {
            b_status = ER_FILERWERR;
            break;
          }
        }
        cnt = 0;
        p = buf;
        while (len > 0 && (result == JBG_EAGAIN || result == JBG_EOK)) {
          /* 将数据倒入解码器 */
          result = jbg_dec_in(&jds, p, len, &cnt);
          p += cnt;
          len -= cnt;
        }
      }
      while (result == JBG_EAGAIN || result == JBG_EOK);
      /* 判断指定图象是否是JBIG图象 */
      switch (result) {
      case JBG_EOK:
      case JBG_EOK_INTR:
        break;
      case JBG_ENOMEM:
        b_status = ER_MEMORYERR;
        break;
        break;
      case JBG_EAGAIN:
      case JBG_EABORT:
      case JBG_EMARKER:
      case JBG_ENOCONT:
      case JBG_EINVAL:
      case JBG_EIMPL:
        b_status = ER_NONIMAGE;
        break;
        break;
      case JBG_ESYSERR:
        b_status = ER_SYSERR;
        break;
        break;
      }
      /* 只支持单色图象 */
      if (jbg_dec_getplanes(&jds) != 1) {
        b_status = ER_NONIMAGE;
        break;
      }
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_JBIG; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_JBIG; /* 压缩方式 */
      pinfo_str->width = (ulong)jbg_dec_getwidth(&jds);
      pinfo_str->height = (ulong)jbg_dec_getheight(&jds);
      pinfo_str->order = 0; /* 都为正向图 */
      pinfo_str->bitcount = (ulong)jbg_dec_getplanes(&jds);
      SETMASK_8(pinfo_str); /* 设置掩码为全0 */
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    while (0);
    
      if (buf) {
        isirw_free(buf);
      }
      jbg_dec_free(&jds);
  return b_status;
}
/* 读取图像位数据 */
EXERESULT jbig_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  JBG_DEC_STATE jds;
  uchar* buf = 0, *p, *bits;
  int len, srclinesize, result, i, linesize, step;
  size_t cnt;
  ulong xmax = 4294967295UL;
  ulong ymax = 4294967295UL;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */

    do {
      jbg_dec_init(&jds);
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        break;
      }
      /* 申请读数据缓冲区 */
      if ((buf = (uchar*)isirw_malloc(BUFFER_SIZE)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 设定图象最大限长 */
      jbg_dec_maxsize(&jds, xmax, ymax);
      result = JBG_EAGAIN;
      do {
        /* 分段读取压缩数据 */
        if ((len = isio_read((void*)buf, 1, BUFFER_SIZE, pfile)) == 0) {
          if ((result == JBG_EOK_INTR) || (result == JBG_EOK)) {
            break;
          }
          else {
            b_status = ER_FILERWERR;
            break;
          }
        }
        cnt = 0;
        p = buf;
        while (len > 0 && (result == JBG_EAGAIN || result == JBG_EOK)) {
          /* 将数据倒入解码器 */
          result = jbg_dec_in(&jds, p, len, &cnt);
          p += cnt;
          len -= cnt;
        }
      }
      while (result == JBG_EAGAIN || result == JBG_EOK);
      /* 判断指定图象是否是JBIG图象 */
      switch (result) {
      case JBG_EOK:
      case JBG_EOK_INTR:
        break;
      case JBG_ENOMEM:
        b_status = ER_MEMORYERR;
        break;
        break;
      case JBG_EAGAIN:
      case JBG_EABORT:
      case JBG_EMARKER:
      case JBG_ENOCONT:
      case JBG_EINVAL:
      case JBG_EIMPL:
        b_status = ER_NONIMAGE;
        break;
        break;
      case JBG_ESYSERR:
        b_status = ER_SYSERR;
        break;
        break;
      }
      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        if (jbg_dec_getplanes(&jds) != 1) {
          b_status = ER_NONIMAGE;
          break;
        }
        pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
        pinfo_str->imgformat = IMF_JBIG; /* 图像文件格式（后缀名） */
        pinfo_str->compression = ICS_JBIG; /* 压缩方式 */
        pinfo_str->width = (ulong)jbg_dec_getwidth(&jds);
        pinfo_str->height = (ulong)jbg_dec_getheight(&jds);
        pinfo_str->order = 0; /* 都为正向图 */
        pinfo_str->bitcount = (ulong)jbg_dec_getplanes(&jds);
        SETMASK_8(pinfo_str); /* 设置掩码为全0 */
        pinfo_str->data_state = 1;
      }
      /* 设置调色板数据 */
      assert(pinfo_str->bitcount == 1);
      pinfo_str->pal_count = 1UL << pinfo_str->bitcount;
      switch (pinfo_str->bitcount) {
      case 1:
        pinfo_str->palette[0] = 0xffffff;
        pinfo_str->palette[1] = 0x0;
        break;
      case 4:
      case 8:
        assert(0); /* 不应该执行到此处，下面的代码只是为将来扩展之用 */
        step = 256 / (int)pinfo_str->pal_count;
        for (i = 0; i < (int)pinfo_str->pal_count; i++) {
          pinfo_str->palette[i] = step * (pinfo_str->pal_count - i) - 1;
        }
        break;
      default:
        assert(0);
        b_status = ER_SYSERR;
        break;
        break;
      }
      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      /* JBIG解压后的扫描行长度（字节边界） */
      srclinesize = (int)((pinfo_str->bitcount * pinfo_str->width + 7) / 8);
      /* 设置图像个数 */
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg = (SUBIMGBLOCK*)0;
      assert(pinfo_str->p_bit_data == (uchar*)0);
      /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
      pinfo_str->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo_str->height + 4);
      if (!pinfo_str->p_bit_data) {
        b_status = ER_MEMORYERR;
        break;
      }
      assert(pinfo_str->pp_line_addr == NULL);
      /* 分配行首地址数组 */
      pinfo_str->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo_str->height);
      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
        break;
      }
      /* 填写行首地址数组（正向图）*/
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        pinfo_str->pp_line_addr[i] = (pinfo_str->p_bit_data + (i * linesize));
      }
      /* 获取解压后的图象数据地址 */
      bits = jbg_dec_getimage(&jds, 0);
      /* 逐行读入 */
      for (i = 0; i < (int)(pinfo_str->height); i++) {
        p = (uchar*)pinfo_str->pp_line_addr[i];
        memcpy((void*)p, (const void*)bits, srclinesize);
        bits += srclinesize;
      }
      pinfo_str->data_state = 2;
    }
    while (0);
      jbg_dec_free(&jds);
      if (buf) {
        isirw_free(buf);
      }
      if ((b_status != ER_SUCCESS) || (1)) {
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
            pinfo_str->data_state = 1;
          }
        }
      }
  return b_status;
}
/* 保存图像 */
EXERESULT jbig_save_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  /* ################ 请在下面编写保存图像的代码 ################ */
  /* 暂不支持JBIG的保存功能。这种图片一般都是由扫描仪、传真机产生的，所以只支持读功能即可。－YZ */
  /* ############################################################ */
  return ER_NOTSUPPORT;
}
