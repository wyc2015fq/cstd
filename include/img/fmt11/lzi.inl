// LZI图像读写模块实现文件

/* 文件结构定义 */
#define CURLZIVER 1     /* 当前文件格式版本 */

#define BUFSIZE   4096    /* size of ring buffer */
#define UPLIMIT   18      /* upper limit for match_length */
#define THRESHOLD 2     /* encode string into position and length if match_length is greater than this */
#define NIL     BUFSIZE   /* index for root of binary search trees */

#define ERREXCEPT -1L     /* memory r/w except */

#define LZIMAGIC  0x46495A4C  /* LZI 文件标签 */


/* 文件头结构 */
typedef struct _tagLZIHEAD {
  unsigned long magic;    /* LZIMAGIC */
  unsigned long ver;    /* 格式版本 */

  unsigned long width;
  unsigned long height;
  unsigned long bitcnt;
  unsigned long imgsize;

  unsigned long order;    /* 0：正像图、1：倒像图 */
  unsigned long rev0;
} LZIFILEHEAD, *LPLZIFILEHEAD;

unsigned char   text_buf[BUFSIZE + UPLIMIT - 1];  /* ring buffer of size BUFSIZE, with extra UPLIMIT-1 bytes to facilitate string comparison */
int         match_position, match_length;   /* of longest match.  These are set by the InsertNode() procedure. */
int         lson[BUFSIZE + 1], rson[BUFSIZE + 257], dad[BUFSIZE + 1];  /* left & right children & parents -- These constitute binary search trees. */



/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
/* 内部辅助函数 */

/* 计算扫描行尺寸(四字节对齐) */
int CALLAGREEMENT _calcu_scanline_size(int w/* 宽度 */, int bit/* 位深 */)
{
  return DIBSCANLINE_WIDTHBYTES(w * bit);
}


/* 判断传入的图像是否可以被保存（如不支持保存功能，可将本函数去掉） */
int CALLAGREEMENT _lzi_is_valid_img(LPINFOSTR pinfo_str)
{
  /* ################################################################# */
  /* 位数减一，否则32位格式无法表示。（LONG为32位）*/
  if (!(lzi_irwp_info.irwp_save.bitcount & (1UL << (pinfo_str->bitcount - 1)))) {
    return -1;  /* 不支持的位深图像 */
  }

  /* ################################################################# */

  assert(pinfo_str->imgnumbers);

  if (lzi_irwp_info.irwp_save.img_num) {
    if (lzi_irwp_info.irwp_save.img_num == 1) {
      if (pinfo_str->imgnumbers != 1) {
        return -2;  /* 图像个数不正确 */
      }
    }
  }

  return 0;
}


/* initialize trees */
void InitTree(void)
{
  int  i;

  /* For i = 0 to BUFSIZE - 1, rson[i] and lson[i] will be the right and
     left children of node i.  These nodes need not be initialized.
     Also, dad[i] is the parent of node i.  These are initialized to
     NIL (= BUFSIZE), which stands for 'not used.'
     For i = 0 to 255, rson[BUFSIZE + i + 1] is the root of the tree
     for strings that begin with character i.  These are initialized
     to NIL.  Note there are 256 trees.
  */

  for (i = BUFSIZE + 1; i <= BUFSIZE + 256; i++) {
    rson[i] = NIL;
  }

  for (i = 0; i < BUFSIZE; i++) {
    dad[i] = NIL;
  }
}


/* Inserts string of length UPLIMIT, text_buf[r..r+UPLIMIT-1], into one of the
   trees (text_buf[r]'th tree) and returns the longest-match position
   and length via the global variables match_position and match_length.
   If match_length = UPLIMIT, then removes the old node in favor of the new
   one, because the old one will be deleted sooner.
   Note r plays double role, as tree node and position in buffer.
*/
void InsertNode(int r)
{
  int       i, p, cmp;
  unsigned char*  key;

  cmp = 1;
  key = &text_buf[r];
  p = BUFSIZE + 1 + key[0];
  rson[r] = lson[r] = NIL;
  match_length = 0;

  for (;;) {
    if (cmp >= 0) {
      if (rson[p] != NIL) {
        p = rson[p];
      }
      else {
        rson[p] = r;
        dad[r] = p;
        return;
      }
    }
    else {
      if (lson[p] != NIL) {
        p = lson[p];
      }
      else {
        lson[p] = r;
        dad[r] = p;
        return;
      }
    }

    for (i = 1; i < UPLIMIT; i++) {
      if ((cmp = key[i] - text_buf[p + i]) != 0) {
        break;
      }
    }

    if (i > match_length) {
      match_position = p;

      if ((match_length = i) >= UPLIMIT) {
        break;
      }
    }
  }

  dad[r] = dad[p];
  lson[r] = lson[p];
  rson[r] = rson[p];

  dad[lson[p]] = r;
  dad[rson[p]] = r;

  if (rson[dad[p]] == p) {
    rson[dad[p]] = r;
  }
  else {
    lson[dad[p]] = r;
  }

  dad[p] = NIL;   /* remove p */
}


/* deletes node p from tree */
void DeleteNode(int p)
{
  int  q;

  if (dad[p] == NIL) {
    return;  /* not in tree */
  }

  if (rson[p] == NIL) {
    q = lson[p];
  }
  else if (lson[p] == NIL) {
    q = rson[p];
  }
  else {
    q = lson[p];

    if (rson[q] != NIL) {
      do {
        q = rson[q];
      }
      while (rson[q] != NIL);

      rson[dad[q]] = lson[q];
      dad[lson[q]] = dad[q];
      lson[q] = lson[p];
      dad[lson[p]] = q;
    }

    rson[q] = rson[p];
    dad[rson[p]] = q;
  }

  dad[q] = dad[p];

  if (rson[dad[p]] == p) {
    rson[dad[p]] = q;
  }
  else {
    lson[dad[p]] = q;
  }

  dad[p] = NIL;
}


/* Encode data */
long LzssEncode(unsigned char* soudat, unsigned long sousize, unsigned char* desbuf)
{
  int       i, c, len, r, s, last_match_length, code_buf_ptr;
  long      result = 0L;
  unsigned long ulspos = 0UL, uldpos = 0UL;
  unsigned char* psou = soudat, *pdes = desbuf;
  unsigned char code_buf[17], mask;

  /* initialize trees */
  InitTree();

  /*
    code_buf[1..16] saves eight units of code, and
    code_buf[0] works as eight flags, "1" representing that the unit
    is an unencoded letter (1 byte), "0" a position-and-length pair
    (2 bytes).  Thus, eight units require at most 16 bytes of code.
  */
  code_buf[0] = 0;

  code_buf_ptr = mask = 1;

  s = 0;
  r = BUFSIZE - UPLIMIT;

  for (i = s; i < r; i++) {
    text_buf[i] = ' ';  /* Clear the buffer with any character that will appear often. */
  }

  if (sousize < UPLIMIT) {
    len = sousize;
  }
  else {
    len = UPLIMIT;
  }

  /* Read UPLIMIT bytes into the last UPLIMIT bytes of the buffer */
  memcpy((void*)(unsigned char*)&text_buf[r], (const void*)psou, len);

  psou   += len;
  ulspos += len;

  if (len == 0) {
    return 0;  /* text of size zero */
  }

  /*
   Insert the UPLIMIT strings, each of which begins with one or more 'space' characters.  Note
   the order in which these strings are inserted.  This way, degenerate trees will be less likely to occur.
  */
  for (i = 1; i <= UPLIMIT; i++) {
    InsertNode(r - i);
  }

  /* Finally, insert the whole string just read.  The global variables match_length and match_position are set. */
  InsertNode(r);

  __try {
    __try {
      do {
        if (match_length > len) {
          match_length = len;     /* match_length may be spuriously long near the end of text. */
        }

        if (match_length <= THRESHOLD) {
          match_length = 1;     /* Not long enough match.  Send one byte. */
          code_buf[0] |= mask;    /* 'send one byte' flag */
          code_buf[code_buf_ptr++] = text_buf[r];  /* Send uncoded. */
        }
        else {
          code_buf[code_buf_ptr++] = (unsigned char) match_position;

          /* Send position and length pair. Note match_length > THRESHOLD. */
          code_buf[code_buf_ptr++] = (unsigned char)(((match_position >> 4) & 0xf0) | (match_length - (THRESHOLD + 1)));
        }

        if ((mask <<= 1) == 0) {        /* Shift mask left one bit. */
          for (i = 0; i < code_buf_ptr; i++) { /* Send at most 8 units of code together */
            *pdes++ = code_buf[i];
          }

          uldpos += code_buf_ptr;
          code_buf[0]  = 0;
          code_buf_ptr = mask = 1;
        }

        last_match_length = match_length;

        for (i = 0; i < last_match_length && ulspos < sousize; i++) {
          DeleteNode(s);        /* Delete old strings and */
          c = *psou++;
          ulspos++;

          text_buf[s] = c;      /* read new bytes */

          /* If the position is near the end of buffer, extend the buffer to make string comparison easier. */
          if (s < UPLIMIT - 1) {
            text_buf[s + BUFSIZE] = c;
          }

          /* Since this is a ring buffer, increment the position modulo BUFSIZE. */
          s = (s + 1) & (BUFSIZE - 1);
          r = (r + 1) & (BUFSIZE - 1);

          InsertNode(r);        /* Register the string in text_buf[r..r+UPLIMIT-1] */
        }

        /* After the end of text, */
        while (i++ < last_match_length) {
          DeleteNode(s);          /* no need to read, but */

          s = (s + 1) & (BUFSIZE - 1);
          r = (r + 1) & (BUFSIZE - 1);

          if (--len) {
            InsertNode(r);  /* buffer may not be empty. */
          }
        }

      }
      while (len > 0);    /* until length of string to be processed is zero */

      if (code_buf_ptr > 1) {
        for (i = 0; i < code_buf_ptr; i++) {
          *pdes++ = code_buf[i];
        }

        uldpos += code_buf_ptr;
      }

      result = uldpos;
    }
    __finally {
      /* ... */
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = ERREXCEPT;
  }

  return result;
}


/* Just the reverse of Encode(). */
long LzssDecode(unsigned char* lzssdat, unsigned long lzssize, unsigned char* extbuf)
{
  int       i, j, k, r, c;
  long      result = 0L;
  unsigned int  flags;
  unsigned long ullzsspos = 0UL, ulextpos = 0UL;
  unsigned char* plzss = lzssdat, *pext = extbuf;

  for (i = 0; i < BUFSIZE - UPLIMIT; i++) {
    text_buf[i] = ' ';
  }

  r     = BUFSIZE - UPLIMIT;
  flags = 0;

  __try {
    __try {
      for (;;) {
        if (((flags >>= 1) & 256) == 0) {
          if (ullzsspos >= lzssize) {
            break;
          }
          else {
            c = (int) * plzss++;
            ullzsspos++;
          }

          flags = c | 0xff00;   /* uses higher byte cleverly to count eight */
        }

        if (flags & 1) {
          if (ullzsspos >= lzssize) {
            break;
          }
          else {
            c = (int) * plzss++;
            ullzsspos++;
          }

          *pext++ = (unsigned char)c;
          ulextpos++;

          text_buf[r++] = c;
          r &= (BUFSIZE - 1);
        }
        else {
          if (ullzsspos >= lzssize) {
            break;
          }
          else {
            i = (int) * plzss++;
            ullzsspos++;
          }

          if (ullzsspos >= lzssize) {
            break;
          }
          else {
            j = (int) * plzss++;
            ullzsspos++;
          }

          i |= ((j & 0xf0) << 4);
          j = (j & 0x0f) + THRESHOLD;

          for (k = 0; k <= j; k++) {
            c = text_buf[(i + k) & (BUFSIZE - 1)];

            *pext++ = (unsigned char)c;
            ulextpos++;

            text_buf[r++] = c;
            r &= (BUFSIZE - 1);
          }
        }
      }

      result = ulextpos;
    }
    __finally {
      /* Ha Ha..... GameOver!!!! */
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    result = ERREXCEPT;
  }

  return result;
}

/* 读取图像位数据 */
LZI_API int CALLAGREEMENT lzi_load_image(PISADDR psct, LPINFOSTR pinfo_str)
{
  ISFILE*      pfile = (ISFILE*)0;
  LZIFILEHEAD   head;
  unsigned long lzssdatsize;
  unsigned char* plzss = 0;
  int       linesize, i;
  long      result;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state < 2);  /* 数据包中不能存在图像位数据 */

  __try {
    __try {
      EnterCriticalSection(&lzi_load_img_critical);

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

      /* 读文件定位 */
      if (isio_seek(pfile, 0, SEEK_SET) == -1) {
        b_status = ER_FILERWERR;
        __leave;
      }

      if (isio_read((void*)&head, sizeof(LZIFILEHEAD), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 判断是否是有效的图象文件 */
      if ((head.magic != LZIMAGIC) || ((head.bitcnt != 24) && (head.bitcnt != 32))) {
        b_status = ER_NONIMAGE;
        __leave;
      }

      /* 如果该图像还未调用过提取信息函数，则填写图像信息 */
      if (pinfo_str->data_state == 0) {
        pinfo_str->imgtype    = IMT_RESSTATIC;  /* 图像文件类型 */
        pinfo_str->imgformat  = IMF_APM;      /* 图像文件格式（后缀名） */
        pinfo_str->compression  = ICS_RGB;

        pinfo_str->width  = head.width;
        pinfo_str->height = head.height;
        pinfo_str->order  = head.order;

        pinfo_str->bitcount = head.bitcnt;

        pinfo_str->b_mask = 0xff;
        pinfo_str->g_mask = 0xff00;
        pinfo_str->r_mask = 0xff0000;
        pinfo_str->a_mask = 0x0;

        /* 设定数据包状态 */
        pinfo_str->data_state = 1;
      }

      pinfo_str->pal_count = 0;

      /* 取得位数据长度 */
      lzssdatsize = isio_length(pfile) - sizeof(LZIFILEHEAD);
      /* 取得扫描行尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);

      /* 设置图像个数 */
      pinfo_str->imgnumbers = 1;
      pinfo_str->psubimg    = (LPSUBIMGBLOCK)0;

      assert(pinfo_str->p_bit_data == (unsigned char*)0);
      /* 分配目标图像内存块（+4 － 尾部附加4字节缓冲区） */
      pinfo_str->p_bit_data = (unsigned char*)isirw_orgpix_malloc(linesize * pinfo_str->height + 4);

      if (!pinfo_str->p_bit_data) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      assert(pinfo_str->pp_line_addr == (void**)0);
      /* 分配行首地址数组 */
      pinfo_str->pp_line_addr = (void**)isirw_malloc(sizeof(void*) * pinfo_str->height);

      if (!pinfo_str->pp_line_addr) {
        b_status = ER_MEMORYERR;
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

      if (pinfo_str->order == 0) {  /* 正向 */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + (i * linesize));
        }
      }
      else {            /* 倒向 */
        for (i = 0; i < (int)(pinfo_str->height); i++) {
          pinfo_str->pp_line_addr[i] = (void*)(pinfo_str->p_bit_data + ((pinfo_str->height - i - 1) * linesize));
        }
      }

      /* 申请LZS压缩数据缓冲区 */
      plzss = (unsigned char*) isirw_malloc(lzssdatsize);

      if (!plzss) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 一次性读入压缩数据 */
      if (isio_read((void*)plzss, lzssdatsize, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      /* 解码 */
      result = LzssDecode(plzss, lzssdatsize, pinfo_str->p_bit_data);

      if ((unsigned long)result != head.imgsize) {
        b_status = ER_BADIMAGE;
        __leave;
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
      if (plzss) {
        isirw_free(plzss);
      }

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
            pinfo_str->data_state = 1;
          }

          (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_LOAD);
        }
      }

      LeaveCriticalSection(&lzi_load_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


/* 保存图像 */
LZI_API int CALLAGREEMENT lzi_save_image(PISADDR psct, LPINFOSTR pinfo_str, LPSAVESTR lpsave)
{
  ISFILE*      pfile = (ISFILE*)0;
  LZIFILEHEAD   head;
  unsigned char* plzss = 0;
  int       linesize, imgsize;
  long      lzssdatsize;

  enum EXERESULT  b_status = ER_SUCCESS;

  assert(psct && lpsave && pinfo_str);
  assert(pinfo_str->sct_mark == INFOSTR_DBG_MARK);
  assert(pinfo_str->data_state == 2); /* 必须存在图像位数据 */
  assert(pinfo_str->p_bit_data);

  __try {
    __try {
      EnterCriticalSection(&lzi_save_img_critical);

      /* 调用系统回调函数 */
      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_INTO_PLUG, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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

      /* 判断是否是受支持的图像数据 */
      if (_lzi_is_valid_img(pinfo_str) != 0) {
        b_status = ER_NSIMGFOR;
        __leave;
      }

      /* 如果存在同名流，本函数将不进行确认提示 */
      if ((pfile = isio_open((const char*)psct, "wb")) == (ISFILE*)0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      head.magic  = LZIMAGIC;
      head.ver    = CURLZIVER;
      head.width  = pinfo_str->width;
      head.height = pinfo_str->height;
      head.bitcnt = pinfo_str->bitcount;

      /* 取得扫描行尺寸及图象尺寸 */
      linesize = _calcu_scanline_size(pinfo_str->width, pinfo_str->bitcount);
      imgsize  = linesize * pinfo_str->height;

      /* 申请压缩缓冲区 */
      plzss = (unsigned char*) isirw_malloc(imgsize);

      if (!plzss) {
        b_status = ER_MEMORYERR;
        __leave;
      }

      /* 压缩图象数据 */
      lzssdatsize = LzssEncode(pinfo_str->p_bit_data, imgsize, plzss);

      if ((lzssdatsize == ERREXCEPT) || (lzssdatsize == 0)) {
        b_status = ER_BADIMAGE;
        __leave;
      }

      head.imgsize = imgsize;
      head.rev0    = 0;
      head.order   = pinfo_str->order;

      /* 写入文件头结构 */
      if (isio_write((const void*)&head, sizeof(LZIFILEHEAD), 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BUILD_MAST_IMAGE, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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

      /* 写入压缩数据 */
      if (isio_write((const void*)plzss, lzssdatsize, 1, pfile) == 0) {
        b_status = ER_FILERWERR;
        __leave;
      }

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_PROCESS, 0, 0, pinfo_str->height, IRWE_CALLBACK_FUNID_SAVE)) {
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

      switch ((*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OVER_SIR, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE)) {
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
    }
    __finally {
      if (plzss) {
        isirw_free(plzss);
      }

      if (pfile) {
        isio_close(pfile);
      }

      if ((b_status != ER_SUCCESS) || (AbnormalTermination())) {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_BREAK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }
      else {
        (*pinfo_str->irwpfun)(pinfo_str->pater, PFC_OK, 0, 0, 0, IRWE_CALLBACK_FUNID_SAVE);
      }

      LeaveCriticalSection(&lzi_save_img_critical);
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    b_status = ER_SYSERR;
  }

  return (int)b_status;
}


