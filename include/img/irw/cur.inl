/********************************************************************
 cur.c
 本文件用途： CUR图像读写模块实现文件
 读取功能：1、4、8、16、24、32位光标图象
 保存功能：不支持
 本文件编写人：
 YZ yzfree##yeah.net
 本文件版本： 30401
 最后修改于： 2003-4-1
 ----------------------------------------------------------------
 修正历史：
 2003-4 支持图象读写引擎提供的象素缓冲区功能
 2002-8 插件接口升级至2.2、引入内存防护概念。
 2002-3 发布新版模块。去除了所有与Windows系统有关
 的API调用，使代码更容易移植。
 2000-8 第一个发布版。
********************************************************************/
#ifndef WIN32
#if defined(_WIN32)||defined(_WINDOWS)
#define WIN32
#endif
#endif /* WIN32 */
/* 光标类型定义 */
#define CURTYPE 2
/* 主、子图象类型定义 */
enum CUR_DATA_TYPE {
  CUR_PRI_IMAGE,
  CUR_SUB_IMAGE
};
#pragma pack(1)
/* 光标的图象块信息头结构 */
typedef struct {
  uchar width; /* 图象宽度 */
  uchar height; /* 图象高度 */
  uchar color_count; /* 颜色个数(如果位深度大于8时为0) */
  uchar reserved;
  ushort x_hotspot; /* 热点X坐标 */
  ushort y_hotspot; /* 热点Y坐标 */
  ulong bytes_in_res; /* 图象数据在资源中所占的字节数 */
  ulong image_offset; /* 图象数据的偏移 */
} CURDIRENTRY, *LPCURDIRENTRY;
/* 光标信息头结构 */
typedef struct {
  ushort reserved; /* 保留 */
  ushort type; /* 资源类型(CURTYPE为光标) */
  ushort count; /* 图象个数 */
  CURDIRENTRY entries[1]; /* 每一个图象的入口 */
} CURDIR, *LPCURDIR;
#pragma pack()
//IRWP_INFO cur_irwp_info; /* 插件信息表 */
/* 内部助手函数 */
/*****************************************************************************/
/* 内部辅助函数 */
/* 获取CUR头结构（包括图像个数、图像尺寸等），并进行简单的鉴别 */
EXERESULT _read_cur_header(ISFILE* file, CURDIR** cur)
{
  CURDIR curdir;
  int infosize;
  EXERESULT result = ER_SUCCESS;
  LPCURDIR lpir = 0;
  assert((file != 0) && (cur != 0));
  do {
    /* 定位于文件首部 */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = ER_FILERWERR;
      break; /* 文件读写失败 */
    }
    /* 读取光标文件信息头结构 */
    if (isio_read((void*)&curdir, sizeof(CURDIR), 1, file) == 0) {
      result = ER_NONIMAGE;
      break; /* 不是一个光标文件 */
    }
    /* 判断文件标志 */
    if ((curdir.reserved != 0) || (curdir.type != CURTYPE)) {
      result = ER_NONIMAGE;
      break; /* 不是一个光标文件 */
    }
    /* 至少会存在一个图象 */
    if (curdir.count < 1) {
      result = ER_BADIMAGE;
      break; /* 光标文件受损 */
    }
    /* 计算光标信息块的尺寸 */
    infosize = sizeof(CURDIR) + (curdir.count - 1) * sizeof(CURDIRENTRY);
    /* 分配用于存放光标信息的缓冲区内存块 */
    if ((lpir = (LPCURDIR)isirw_malloc(infosize)) == 0) {
      result = ER_MEMORYERR;
      break; /* 内存不足 */
    }
    /* 读取光标信息 */
    if (isio_seek(file, 0, SEEK_SET) == -1) {
      result = ER_FILERWERR;
      break;
    }
    if (isio_read((void*)lpir, infosize, 1, file) == 0) {
      result = ER_BADIMAGE;
      break; /* 光标文件受损（也有可能是文件读写错，但图象受损的可能性更大） */
    }
  }
  while (0);
  if (result != ER_SUCCESS) {
    if (lpir) {
      isirw_free(lpir);
      lpir = 0;
    }
  }
  *cur = lpir; /* 当发生错误或有异常时，*cur将被置 0 */
  return result; /* 返回 0 成功，非 0 值失败。*cur指向的内存块在不需要时应释放。 */
}
/* 读取一个光标的象素数据块（包含BITMAPINFOHEADER结构、XOR数据和AND数据）*/
EXERESULT _read_image_block(ISFILE* file, ulong offset, ulong len, uchar** image)
{
  uchar* tmp = 0;
  EXERESULT result = ER_SUCCESS;
  assert((file != 0) && (image != 0));
  /* 数据合法性检查 */
  if ((offset == 0) || (len == 0)) {
    return ER_BADIMAGE; /* 光标文件受损（数据不合法）*/
  }
  do {
    /* 定位读写位置 */
    if (isio_seek(file, offset, SEEK_SET) == -1) {
      result = ER_FILERWERR;
      break; /* 文件读写失败 */
    }
    if ((tmp = (uchar*)isirw_malloc(len)) == 0) {
      result = ER_MEMORYERR;
      break; /* 内存不足 */
    }
    /* 读取图象数据 */
    if (isio_read((void*)tmp, len, 1, file) == 0) {
      result = ER_BADIMAGE;
      break; /* 光标文件受损 */
    }
    if (((wBITMAPINFOHEADER*)tmp)->biSize != sizeof(wBITMAPINFOHEADER)) {
      result = ER_BADIMAGE;
      break; /* 光标文件受损 */
    }
  }
  while (0);
  if (result != 0) {
    if (tmp) {
      isirw_free(tmp);
      tmp = 0;
    }
  }
  *image = tmp;
  return result; /* 返回 0 成功，非 0 值失败。*tmp指向的内存块在不需要时应释放。 */
}
/* 合成一个扫描行的XOR图和AND图，并进行ISee位流格式转换，结果保存在XOR图中 */
int _compose_img(int bitcount, int width, uchar* pxor, uchar* pand)
{
  uchar buf[32], tmp, *p = pxor, isc = 0;
  int i, j, k, l, bytesize;
  assert(bitcount > 0);
  assert(pxor && pand);
  /* 一次处理 8 个象素 */
  for (i = 0, k = 0; i < width; i += 8, k++) {
    /* 取得 8 个象素的AND数据（共 1 个字节） */
    tmp = *(pand + k);
    /* 将AND数据转为XOR象素格式 */
    switch (bitcount) {
    case 1:
      buf[0] = tmp;
      bytesize = 1;
      break;
    case 4:
      for (l = 0; l < 8; l += 2) {
        buf[l / 2] = ((tmp >> (7 - l)) & 0x1) ? 0xf0 : 0x0;
        buf[l / 2] |= ((tmp >> (7 - l - 1)) & 0x1) ? 0xf : 0x0;
      }
      bytesize = 4;
      break;
    case 8:
      for (l = 0; l < 8; l++) {
        buf[l] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
      }
      bytesize = ((width - i) < 8) ? (width - i) : 8;
      break;
    case 16:
      for (l = 0; l < 8; l++) {
        buf[l * 2] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 2 + 1] = buf[l * 2] & 0x7f; /* 555格式 */
      }
      bytesize = ((width - i) < 8) ? (width - i) * 2 : 16;
      break;
    case 24:
      for (l = 0; l < 8; l++) {
        buf[l * 3] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 3 + 1] = buf[l * 3];
        buf[l * 3 + 2] = buf[l * 3];
      }
      bytesize = ((width - i) < 8) ? (width - i) * 3 : 24;
      break;
    case 32:
      for (l = 0; l < 8; l++) {
        buf[l * 4] = ((tmp >> (7 - l)) & 0x1) ? 0xff : 0x0;
        buf[l * 4 + 1] = buf[l * 4];
        buf[l * 4 + 2] = buf[l * 4];
        buf[l * 4 + 3] = 0; /* 888格式 */
      }
      bytesize = ((width - i) < 8) ? (width - i) * 4 : 32;
      break;
    default:
      return -1; /* 不支持的位深度 */
      break;
    }
    /* 合成图象（一次合成 8 个象素）*/
    for (j = 0; j < bytesize; j++) {
      *p++ ^= buf[j];
    }
  }
  return 0; /* 返回 0 表示成功，返回非 0 值表示失败 */
}
/* 将光标数据块转为数据包块 */
EXERESULT _conv_image_block(wBITMAPINFOHEADER* lpbmi, void* lpdest, enum CUR_DATA_TYPE mark)
{
  EXERESULT b_status = ER_SUCCESS;
  INFOSTR* pinfo = (INFOSTR*)0;
  SUBIMGBLOCK* psubinfo = (SUBIMGBLOCK*)0;
  wBITMAPINFO* pbi = (wBITMAPINFO*)lpbmi;
  uchar* p, *pxor, *pand;
  ulong pal_num = 0;
  int i, linesize, linesize2;
  assert((lpbmi) && (lpdest));
  assert((mark == CUR_PRI_IMAGE) || (mark == CUR_SUB_IMAGE));
  if (mark == CUR_PRI_IMAGE) {
    pinfo = (INFOSTR*)lpdest;
    /* 设置调色板数据 */
    if (lpbmi->biBitCount <= 8) {
      pinfo->pal_count = 1UL << pinfo->bitcount;
      pal_num = (lpbmi->biClrUsed == 0) ? pinfo->pal_count : lpbmi->biClrUsed;
      if (pal_num > pinfo->pal_count) {
        return ER_BADIMAGE; /* 图象受损 */
      }
      memmove((void*)(pinfo->palette), (const void*)(pbi->bmiColors), sizeof(wRGBQUAD)*pal_num);
    }
    else {
      pinfo->pal_count = 0;
    }
    /* XOR象素扫描行尺寸 */
    linesize = _calcu_scanline_size(pinfo->width, pinfo->bitcount);
    /* AND象素扫描行尺寸 */
    linesize2 = _calcu_scanline_size(pinfo->width, 1);
    assert(pinfo->p_bit_data == (uchar*)0);
    /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
    pinfo->p_bit_data = (uchar*)isirw_malloc(linesize * pinfo->height + 4);
    if (!pinfo->p_bit_data) {
      return ER_MEMORYERR; /* 内存不足 */
    }
    assert(pinfo->pp_line_addr == NULL);
    /* 分配行首地址数组 */
    pinfo->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * pinfo->height);
    if (!pinfo->pp_line_addr) {
      isirw_free(pinfo->p_bit_data);
      pinfo->p_bit_data = 0;
      return ER_MEMORYERR; /* 内存不足 */
    }
    /* 初始化行首地址数组（倒向） */
    for (i = 0; i < (int)(pinfo->height); i++) {
      pinfo->pp_line_addr[i] = (pinfo->p_bit_data + ((pinfo->height - i - 1) * linesize));
    }
    /* 计算XOR象素数据首地址 */
    p = (uchar*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(wRGBQUAD);
    /* 复制XOR象素数据 */
    memmove((void*)pinfo->p_bit_data, (const void*)p, (linesize * pinfo->height));
    /* 取得 XOR 象素首地址 */
    pxor = (uchar*)pinfo->p_bit_data;
    /* 计算 AND 象素首地址 */
    pand = p + linesize * pinfo->height;
    /* 合成图象 */
    for (i = 0; i < (int)pinfo->height; i++) {
      if (_compose_img(pinfo->bitcount, pinfo->width, pxor, pand) != 0) {
        isirw_free(pinfo->p_bit_data);
        pinfo->p_bit_data = 0;
        isirw_free(pinfo->pp_line_addr);
        pinfo->pp_line_addr = 0;
        return ER_BADIMAGE; /* 受损的图象 */
      }
      pxor += linesize;
      pand += linesize2;
    }
  }
  else {
    psubinfo = (SUBIMGBLOCK*)lpdest;
    /* 填写图像信息 */
    psubinfo->width = (int)lpbmi->biWidth;
    psubinfo->height = (int)lpbmi->biHeight / 2; /* XOR和AND 图总高度 */
    psubinfo->order = 1; /* 总是倒向图 */
    psubinfo->bitcount = (int)lpbmi->biBitCount;
    if(ER_SUCCESS!=(b_status = _get_mask(psubinfo->bitcount,(&psubinfo->b_mask),(&psubinfo->g_mask),
      &psubinfo->r_mask,&psubinfo->a_mask))) {
      return b_status;
    }
    psubinfo->left = psubinfo->top = 0;
    psubinfo->dowith = psubinfo->userinput = psubinfo->time = 0;
    psubinfo->colorkey = -1; /* 定为无关键色 */
    /* 设置调色板数据 */
    if (lpbmi->biBitCount <= 8) {
      psubinfo->pal_count = 1UL << psubinfo->bitcount;
      pal_num = (lpbmi->biClrUsed == 0) ? psubinfo->pal_count : lpbmi->biClrUsed;
      if (pal_num > (ulong)psubinfo->pal_count) {
        return ER_BADIMAGE; /* 图象受损 */
      }
      memmove((void*)(psubinfo->palette), (const void*)(pbi->bmiColors), sizeof(wRGBQUAD)*pal_num);
    }
    else {
      psubinfo->pal_count = 0;
    }
    /* 取得XOR象素扫描行尺寸 */
    linesize = _calcu_scanline_size(psubinfo->width, psubinfo->bitcount);
    /* 计算AND象素扫描行尺寸 */
    linesize2 = _calcu_scanline_size(psubinfo->width, 1);
    assert(psubinfo->p_bit_data == (uchar*)0);
    /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
    psubinfo->p_bit_data = (uchar*)isirw_malloc(linesize * psubinfo->height + 4);
    if (!psubinfo->p_bit_data) {
      return ER_MEMORYERR; /* 内存不足 */
    }
    assert(psubinfo->pp_line_addr == NULL);
    /* 分配行首地址数组 */
    psubinfo->pp_line_addr = (uchar**)isirw_malloc(sizeof(uchar*) * psubinfo->height);
    if (!psubinfo->pp_line_addr) {
      isirw_free(psubinfo->p_bit_data);
      psubinfo->p_bit_data = 0;
      return ER_MEMORYERR; /* 内存不足 */
    }
    /* 初始化行首地址数组（倒向） */
    for (i = 0; i < (int)(psubinfo->height); i++) {
      psubinfo->pp_line_addr[i] = (psubinfo->p_bit_data + ((psubinfo->height - i - 1) * linesize));
    }
    /* 计算象素数据首地址 */
    p = (uchar*)lpbmi;
    p += lpbmi->biSize;
    p += pal_num * sizeof(wRGBQUAD);
    /* 复制象素数据 */
    memmove((void*)psubinfo->p_bit_data, (const void*)p, (linesize * psubinfo->height));
    /* 计算 XOR 象素首地址 */
    pxor = (uchar*)psubinfo->p_bit_data;
    /* 计算 AND 象素首地址 */
    pand = p + linesize * psubinfo->height;
    /* 合成图象（使用XOR数据与AND数据）*/
    for (i = 0; i < psubinfo->height; i++) {
      if (_compose_img(psubinfo->bitcount, psubinfo->width, pxor, pand) != 0) {
        isirw_free(psubinfo->p_bit_data);
        psubinfo->p_bit_data = 0;
        isirw_free(psubinfo->pp_line_addr);
        psubinfo->pp_line_addr = 0;
        return ER_BADIMAGE; /* 受损的图象 */
      }
      pxor += linesize;
      pand += linesize2;
    }
  }
  return ER_SUCCESS; /* 返回 0 成功，非 0 失败 */
}
/* 获取图像信息 */
EXERESULT cur_get_image_info(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LPCURDIR lpir = 0;
  wBITMAPINFOHEADER* lpbmi = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 如果数据包中已有了图像位数据，则不能再改变包中的图像信息 */
  do {
    /* 读取文件头信息 */
    IF_LEAVE(_read_cur_header(pfile, (CURDIR**)&lpir));
    pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
    pinfo_str->imgformat = IMF_CUR; /* 图像文件格式（后缀名） */
    pinfo_str->compression = ICS_RGB; /* 图像的压缩方式 */
    /* 读取主图象数据块 */
    IF_LEAVE(_read_image_block(pfile, (ulong)lpir->entries[0].image_offset, (ulong)lpir->entries[0].bytes_in_res, (uchar**)&lpbmi));
    /* 填写图像信息 */
    pinfo_str->width = (ulong)lpbmi->biWidth;
    pinfo_str->height = (ulong)lpbmi->biHeight / 2; /* XOR和AND 图总高度 */
    pinfo_str->order = 1; /* 总是倒向图 */
    pinfo_str->bitcount = (ulong)lpbmi->biBitCount;
    IF_LEAVE(_get_mask(pinfo_str->bitcount, &pinfo_str->b_mask, &pinfo_str->g_mask, &pinfo_str->r_mask, &pinfo_str->a_mask));
    /* 设定数据包状态 */
    pinfo_str->data_state = 1;
  }
  while (0);
  if (lpbmi) {
    isirw_free(lpbmi);
  }
  if (lpir) {
    isirw_free(lpir);
  }
  return b_status;
}
/* 读取图像位数据 */
EXERESULT cur_load_image(ISFILE* pfile, INFOSTR* pinfo_str)
{
  LPCURDIR lpir = 0;
  uchar* lpimg = 0;
  int i;
  wBITMAPINFOHEADER* lpbmi;
  SUBIMGBLOCK* lpsub = 0;
  enum EXERESULT b_status = ER_SUCCESS;
  assert(pfile && pinfo_str);
  assert(pinfo_str->data_state < 2); /* 数据包中不能存在图像位数据 */
  do {
    /* 读取文件头信息 */
    IF_LEAVE(_read_cur_header(pfile, (CURDIR**)&lpir));
    /* 读取主图象数据块 */
    IF_LEAVE(_read_image_block(pfile, (ulong)lpir->entries[0].image_offset, (ulong)lpir->entries[0].bytes_in_res, &lpimg));
    lpbmi = (wBITMAPINFOHEADER*)lpimg;
    assert(lpbmi);
    assert(lpir->count > 0);
    /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
    if (pinfo_str->data_state == 0) {
      pinfo_str->imgtype = IMT_RESSTATIC; /* 图像文件类型 */
      pinfo_str->imgformat = IMF_CUR; /* 图像文件格式（后缀名） */
      pinfo_str->compression = ICS_RGB; /* 图像的压缩方式 */
      /* 填写图像信息 */
      pinfo_str->width = (ulong)lpbmi->biWidth;
      pinfo_str->height = (ulong)lpbmi->biHeight / 2; /* XOR和AND 图总高度 */
      pinfo_str->order = 1; /* 总是倒向图 */
      pinfo_str->bitcount = (ulong)lpbmi->biBitCount;
      IF_LEAVE(_get_mask(pinfo_str->bitcount,
          (&pinfo_str->b_mask),
          (&pinfo_str->g_mask),
          (&pinfo_str->r_mask),
          (&pinfo_str->a_mask)));
      /* 设定数据包状态 */
      pinfo_str->data_state = 1;
    }
    /* 设置图像个数 */
    pinfo_str->imgnumbers = (ulong)lpir->count;
    assert(pinfo_str->psubimg == 0);
    pinfo_str->psubimg = (SUBIMGBLOCK*)0;
    /* 读首帧图象到数据包中 */
    IF_LEAVE(_conv_image_block(lpbmi, (void*)pinfo_str, CUR_PRI_IMAGE));
    /* 读取的首帧图象数据已无用，释放 */
    isirw_free(lpimg);
    lpimg = NULL;
    lpbmi = NULL;
    /* 读入子图象 */
    for (i = 0; i < (int)(pinfo_str->imgnumbers - 1); i++) { /*主图象不算*/
      if ((lpsub = _alloc_SUBIMGBLOCK(pinfo_str)) == 0) {
        b_status = ER_MEMORYERR;
        break;
      }
      lpsub->number = i + 1; /* 子图像序号由 1 开始 */

      /* 读取子图象数据块 */
      IF_LEAVE(_read_image_block(pfile, (ulong)lpir->entries[i + 1].image_offset, (ulong)lpir->entries[i + 1].bytes_in_res, &lpimg));
      lpbmi = (wBITMAPINFOHEADER*)lpimg;
      /* 转换子图象到数据包中 */
      IF_LEAVE(_conv_image_block(lpbmi, (void*)lpsub, CUR_SUB_IMAGE))
      isirw_free(lpimg);
      lpimg = NULL;
      lpbmi = NULL;
    }
    pinfo_str->data_state = 2;
  }
  while (0);
  
  if (lpimg) {
    isirw_free(lpimg);
  }
  if (lpir) {
    isirw_free(lpir);
  }
  if ((b_status != ER_SUCCESS)) {
    INFOSTR_free(pinfo_str);
  }
  return b_status;
}


