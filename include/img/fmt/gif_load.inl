
static BOOL gif_decode_extension(gif_load_t* fmt, stream_t* m_strm)
{
  BOOL bContinue;
  unsigned char count;
  unsigned char fc;

  bContinue = (1 == Read(&fc, sizeof(fc), 1));

  if (bContinue) {
    /* AD - for transparency */
    if (fc == 0xF9) {
      bContinue = (1 == Read(&count, sizeof(count), 1));

      if (bContinue) {
        assert(sizeof(fmt->gifgce) == 4);
        bContinue = (count == Read(&fmt->gifgce, 1, sizeof(fmt->gifgce)));
        fmt->gifgce.delaytime = bit_ntohs(fmt->gifgce.delaytime); // Avoid Byte order problem with Mac <AMSN>

        if (bContinue) {
          fmt->nBkgndIndex = (fmt->gifgce.flags & 0x1) ? fmt->gifgce.transpcolindex : -1;
          fmt->dwFrameDelay = fmt->gifgce.delaytime;
          fmt->dispmeth = ((fmt->gifgce.flags >> 2) & 0x7);
        }
      }
    }

    if (fc == 0xFE) { //<DP> Comment block
      bContinue = (1 == Read(&count, sizeof(count), 1));

      if (bContinue) {
        bContinue = (1 == Read(fmt->m_comment, count, 1));
        fmt->m_comment[count] = '\0';
      }
    }

    if (fc == 0xFF) { //<DP> Application Extension block
      bContinue = (1 == Read(&count, sizeof(count), 1));

      if (bContinue) {
        bContinue = (count == 11);

        if (bContinue) {
          char AppID[11];
          bContinue = (1 == Read(AppID, count, 1));

          if (bContinue) {
            bContinue = (1 == Read(&count, sizeof(count), 1));

            if (bContinue) {
              uint8* dati = (uint8*)pmalloc(count);
              bContinue = (dati != NULL);

              if (bContinue) {
                bContinue = (1 == Read(dati, count, 1));

                if (count > 2) {
                  fmt->m_loops = dati[1] + 256 * dati[2];
                }
              }

              pfree(dati);
            }
          }
        }
      }
    }

    while (bContinue && Read(&count, sizeof(count), 1) && count) {
      //log << "Skipping " << count << " bytes" << endl;
      Seek(count, SEEK_CUR);
    }
  }

  return bContinue;

}

#if 1
// - This external (machine specific) function is expected to return
// either the next uint8 from the GIF file, or a negative error number.
static int gif_get_byte1(gif_load_t* fmt, stream_t* m_strm)
{
  if (fmt->ibf >= GIFBUFTAM) {
    // FW 06/02/98 >>>
    fmt->ibfmax = (int)Read(fmt->buf , 1 , GIFBUFTAM);

    if (fmt->ibfmax < GIFBUFTAM) {
      fmt->buf[ fmt->ibfmax ] = 255;
    }

    // FW 06/02/98 <<<
    fmt->ibf = 0;
  }

  if (fmt->ibf >= fmt->ibfmax) {
    return -1; //<DP> avoid overflows
  }

  return fmt->buf[fmt->ibf++];
}

#define gif_get_byte(fmt, m_strm, out) { \
    if (fmt->ibf>=GIFBUFTAM){ \
      fmt->ibfmax = (int)Read(fmt->buf , 1 , GIFBUFTAM); \
      if( fmt->ibfmax < GIFBUFTAM ) fmt->buf[ fmt->ibfmax ] = 255; \
      fmt->ibf = 0; \
    } \
    if (fmt->ibf>=fmt->ibfmax) return -1; \
    out=(short)fmt->buf[fmt->ibf++]; }

#endif

#define gif_get_byte2(fmt, m_strm, out)  out = (short)stream_get8(m_strm)

#define fmtgif_free(x) (0)

// GIF decoder
// DECODE.C - An LZW decoder for GIF
// Copyright (C) 1987, by Steven A. Bennett
// Copyright (C) 1994, C++ version by Alejandro Aguilar Sierra
//
// Permission is given by the author to freely redistribute and include
// this code in any program as long as this credit is given where due.
//
// In accordance with the above, I want to credit Steve Wilhite who wrote
// the code which this is heavily inspired by...
//
// GIF and 'Graphics Interchange Format' are trademarks (tm) of
// Compuserve, Incorporated, an H&R Block Company.
//
// Release Notes: This file contains a decoder routine for GIF images
// which is similar, structurally, to the original routine by Steve Wilhite.
// It is, however, somewhat noticably faster in most cases.
static short LZW_init_exp(gif_load_t* fmt, short size)
{
  fmt->curr_size = (short)(size + 1);
  fmt->top_slot = (short)(1 << fmt->curr_size);
  fmt->clear = (short)(1 << size);
  fmt->ending = (short)(fmt->clear + 1);
  fmt->slot = fmt->newcodes = (short)(fmt->ending + 1);
  fmt->navail_bytes = fmt->nbits_left = 0;

  memset(fmt->stack, 0, MAX_CODES + 1);
  memset(fmt->prefix, 0, MAX_CODES + 1);
  memset(fmt->suffix, 0, MAX_CODES + 1);
  return(0);
}


/* LZW_get_next_code()
* - gets the next code from the GIF file. Returns the code, or else
* a negative number in case of file errors...
*/
static short LZW_get_next_code(gif_load_t* fmt, stream_t* m_strm)
{
  short i, x;
  uint32 ret;

  if (fmt->nbits_left == 0) {
    if (fmt->navail_bytes <= 0) {
      /* Out of bytes in current block, so read next block */
      fmt->pbytes = fmt->byte_buff;
      gif_get_byte(fmt, m_strm, fmt->navail_bytes);

      if (fmt->navail_bytes < 0) {
        return(fmt->navail_bytes);
      }
      else if (fmt->navail_bytes) {
        for (i = 0; i < fmt->navail_bytes; ++i) {
          gif_get_byte(fmt, m_strm, x);

          if (x < 0) {
            return(x);
          }

          fmt->byte_buff[i] = (uint8)x;
        }
      }
    }

    fmt->b1 = *fmt->pbytes++;
    fmt->nbits_left = 8;
    --fmt->navail_bytes;
  }

  if (fmt->navail_bytes < 0) {
    return fmt->ending; // prevent deadlocks (thanks to Mike Melnikov)
  }

  ret = fmt->b1 >> (8 - fmt->nbits_left);

  while (fmt->curr_size > fmt->nbits_left) {
    if (fmt->navail_bytes <= 0) {
      /* Out of bytes in current block, so read next block*/
      fmt->pbytes = fmt->byte_buff;
      gif_get_byte(fmt, m_strm, fmt->navail_bytes);

      if (fmt->navail_bytes < 0) {
        return(fmt->navail_bytes);
      }
      else if (fmt->navail_bytes) {
        for (i = 0; i < fmt->navail_bytes; ++i) {
          gif_get_byte(fmt, m_strm, x);

          if (x < 0) {
            return(x);
          }

          fmt->byte_buff[i] = (uint8)x;
        }
      }
    }

    fmt->b1 = *fmt->pbytes++;
    ret |= fmt->b1 << fmt->nbits_left;
    fmt->nbits_left += 8;
    --fmt->navail_bytes;
  }

  fmt->nbits_left = (short)(fmt->nbits_left - fmt->curr_size);
  ret &= code_mask[fmt->curr_size];
  return((short)(ret));
}

static int LZW_out_line(gif_load_t* fmt, image_iterator* iter, unsigned char* pixels, int linelen)
{
  long x;

  if (iter == NULL || pixels == NULL) {
    return -1;
  }

  //<DP> for 1 & 4 bpp images, the pixels are compressed
  if (fmt->biBitCount < 8) {
    for (x = 0; x < iter->w; x++) {
      uint8 pos;
      uint8* iDst = pixels + (x * fmt->biBitCount >> 3);

      if (fmt->biBitCount == 4) {
        pos = (uint8)(4 * (1 - x % 2));
        *iDst &= ~(0x0F << pos);
        *iDst |= ((pixels[x] & 0x0F) << pos);
      }
      else if (fmt->biBitCount == 1) {
        pos = (uint8)(7 - x % 8);
        *iDst &= ~(0x01 << pos);
        *iDst |= ((pixels[x] & 0x01) << pos);
      }
    }
  }

  /* AD - for interlace */
  if (fmt->interlaced) {
    iter->Ity = fmt->iheight - fmt->iypos - 1;
    memcpy(iter->IterImage + iter->Ity * iter->s, pixels, linelen);

    if ((fmt->iypos += fmt->istep) >= fmt->iheight) {
      do {
        if (fmt->ipass++ > 0) {
          fmt->istep /= 2;
        }

        fmt->iypos = fmt->istep / 2;
      }
      while (fmt->iypos > fmt->iheight);
    }

    return 0;
  }
  else {
    if (0 <= iter->Ity && iter->Ity < iter->h && 0 <= iter->Itx && iter->Itx < iter->w) {
      memcpy(iter->IterImage + iter->Ity * iter->s, pixels, linelen);
      --iter->Ity;
      return 0;
    }
    else {
      // puts("chafeo");
      return -1;
    }
  }

  return 0;
}

#define bad_code_count (*pbad_code_count)
static short LZW_decode(gif_load_t* fmt, stream_t* file, image_iterator* iter, short linewidth, int* pbad_code_count)
{
  register uint8* sp, *bufptr;
  uint8* buf;
  register short code, fc, oc, bufcnt;
  short c, size, ret;

  // Initialize for decoding a new image...
  bad_code_count = 0;
  gif_get_byte(fmt, file, size);

  if (size < 0) {
    return(size);
  }

  if (size < 2 || 9 < size) {
    return(BAD_CODE_SIZE);
  }

  // LZW_out_line = outline;
  LZW_init_exp(fmt, size);
  //printf("L %d %x\n",linewidth,size);

  /* Initialize in case they forgot to put in a fmt->clear code.
  * (This shouldn't happen, but we'll try and decode it anyway...)
  */
  oc = fc = 0;

  /* Allocate space for the decode buffer */
  if ((buf = MALLOC(uint8, linewidth + 1)) == NULL) {
    return(OUT_OF_MEMORY);
  }

  /* Set up the fmt->stack pointer and decode buffer pointer */
  sp = fmt->stack;
  bufptr = buf;
  bufcnt = linewidth;

  /* This is the main loop. For each code we get we pass through the
  * linked list of fmt->prefix codes, pushing the corresponding "character" for
  * each code onto the fmt->stack. When the list reaches a single "character"
  * we push that on the fmt->stack too, and then start unstacking each
  * character for output in the correct order. Special handling is
  * included for the fmt->clear code, and the whole thing ends when we get
  * an fmt->ending code.
  */
  while ((c = LZW_get_next_code(fmt, file)) != fmt->ending) {
    /* If we had a file error, return without completing the decode*/
    if (c < 0) {
      pfree(buf);
      return(0);
    }

    /* If the code is a fmt->clear code, reinitialize all necessary items.*/
    if (c == fmt->clear) {
      fmt->curr_size = (short)(size + 1);
      fmt->slot = fmt->newcodes;
      fmt->top_slot = (short)(1 << fmt->curr_size);

      /* Continue reading codes until we get a non-fmt->clear code
      * (Another unlikely, but possible case...)
      */
      while ((c = LZW_get_next_code(fmt, file)) == fmt->clear);

      /* If we get an fmt->ending code immediately after a fmt->clear code
      * (Yet another unlikely case), then break out of the loop.
      */
      if (c == fmt->ending) {
        break;
      }

      /* Finally, if the code is beyond the range of already set codes,
      * (This one had better NOT happen... I have no idea what will
      * result from this, but I doubt it will look good...) then set it
      * to color zero.
      */
      if (c >= fmt->slot) {
        c = 0;
      }

      oc = fc = c;

      /* And let us not forget to put the char into the buffer... And
      * if, on the off chance, we were exactly one pixel from the end
      * of the line, we have to send the buffer to the LZW_out_line()
      * routine...
      */
      *bufptr++ = (uint8)c;

      if (--bufcnt == 0) {
        if (iter) {
          if ((ret = (short)LZW_out_line(fmt, iter, buf, linewidth)) < 0) {
            pfree(buf);
            return(ret);
          }
        }

        bufptr = buf;
        bufcnt = linewidth;
      }
    }
    else {
      /* In this case, it's not a fmt->clear code or an fmt->ending code, so
      * it must be a code code... So we can now decode the code into
      * a fmt->stack of character codes. (Clear as mud, right?)
      */
      code = c;

      /* Here we go again with one of those off chances... If, on the
      * off chance, the code we got is beyond the range of those already
      * set up (Another thing which had better NOT happen...) we trick
      * the decoder into thinking it actually got the last code read.
      * (Hmmn... I'm not sure why this works... But it does...)
      */
      if (code >= fmt->slot && sp < (fmt->stack + MAX_CODES - 1)) {
        if (code > fmt->slot) {
          ++bad_code_count;
        }

        code = oc;
        *sp++ = (uint8)fc;
      }

      /* Here we scan back along the linked list of prefixes, pushing
      * helpless characters (ie. suffixes) onto the fmt->stack as we do so.
      */
      while (code >= fmt->newcodes && sp < (fmt->stack + MAX_CODES - 1)) {
        *sp++ = fmt->suffix[code];
        code = fmt->prefix[code];
      }

      /* Push the last character on the fmt->stack, and set up the new
      * fmt->prefix and fmt->suffix, and if the required fmt->slot number is greater
      * than that allowed by the current bit size, increase the bit
      * size. (NOTE - If we are all full, we *don't* save the new
      * fmt->suffix and fmt->prefix... I'm not certain if this is correct...
      * it might be more proper to overwrite the last code...
      */
      *sp++ = (uint8)code;

      if (fmt->slot < fmt->top_slot) {
        fmt->suffix[fmt->slot] = (uint8)(fc = (uint8)code);
        fmt->prefix[fmt->slot++] = oc;
        oc = c;
      }

      if (fmt->slot >= fmt->top_slot) {
        if (fmt->curr_size < 12) {
          fmt->top_slot <<= 1;
          ++fmt->curr_size;
        }
      }

      /* Now that we've pushed the decoded string (in reverse order)
      * onto the fmt->stack, lets pop it off and put it into our decode
      * buffer... And when the decode buffer is full, write another
      * line...
      */
      while (sp > fmt->stack) {
        *bufptr++ = *(--sp);

        if (--bufcnt == 0) {
          if (iter) {
            if ((ret = (short)LZW_out_line(fmt, iter, buf, linewidth)) < 0) {
              pfree(buf);
              return(ret);
            }
          }

          bufptr = buf;
          bufcnt = linewidth;
        }
      }
    }
  }

  ret = 0;

  if (bufcnt != linewidth && iter) {
    ret = (short)LZW_out_line(fmt, iter, buf, (linewidth - bufcnt));
  }

  pfree(buf);
  return(ret);
}
#undef bad_code_count

static long gif_seek_next_image(const gif_load_t* fmt, long position)
{
  char ch1, ch2;
  Seek(position, SEEK_SET);
  ch1 = ch2 = 0;

  while (Read(&ch2, sizeof(char), 1) > 0) {
    if (ch1 == 0 && ch2 == ',') {
      Seek(-1, SEEK_CUR);
      return Tell();
    }
    else {
      ch1 = ch2;
    }
  }

  return -1;
}

static int gif_get_num_frames(gif_load_t* fmt, stream_t* m_strm, struct_TabCol* TabColSrc, struct_dscgif* dscgif)
{
  struct_image image;
  long pos = Tell();
  int nframes = 0;
  int badcode;
  char ch;
  BOOL bPreviousWasNull = TRUE;
  BOOL bContinue = TRUE;
  struct_TabCol TempTabCol;
  memcpy(&TempTabCol, TabColSrc, sizeof(struct_TabCol));

  for (bContinue = TRUE; bContinue;) {
    if (Read(&ch, sizeof(ch), 1) != 1) {
      break;
    }

    if (bPreviousWasNull || ch == 0) {
      long pos_start;

      switch (ch) {
      case '!': { // extension
        gif_decode_extension(fmt, m_strm);
        break;
      }

      case ',': { // image
        assert(sizeof(image) == 9);
        //log << "Image header" << endl;
        Read(&image, sizeof(image), 1);

        //avoid byte order problems with Solaris <candan> <AMSN>
        image.l = bit_ntohs(image.l);
        image.t = bit_ntohs(image.t);
        image.w = bit_ntohs(image.w);
        image.h = bit_ntohs(image.h);

        // in case of images with empty screen descriptor, give a last chance
        if (dscgif->scrwidth == 0 && dscgif->scrheight == 0) {
          dscgif->scrwidth = image.w;
          dscgif->scrheight = image.h;
        }

        if (((image.l + image.w) > dscgif->scrwidth) || ((image.t + image.h) > dscgif->scrheight)) {
          break;
        }

        nframes++;

        // Local colour map?
        if (image.pf & 0x80) {
          TempTabCol.sogct = (short)(1 << ((image.pf & 0x07) + 1));
          assert(3 == sizeof(rgb_color));
          Read(TempTabCol.paleta, sizeof(struct rgb_color)*TempTabCol.sogct, 1);
          //log << "Local colour map" << endl;
        }

        badcode = 0;
        fmt->ibf = GIFBUFTAM + 1;

        fmt->interlaced = image.pf & 0x40;
        fmt->iheight = image.h;
        fmt->istep = 8;
        fmt->iypos = 0;
        fmt->ipass = 0;

        pos_start = Tell();

        //if (fmt->interlaced) log << "Interlaced" << endl;
        LZW_decode(fmt, m_strm, 0, image.w, &badcode);

        if (badcode) {
          gif_seek_next_image(fmt, pos_start);
        }
        else {
          Seek(-(fmt->ibfmax - fmt->ibf - 1), SEEK_CUR);
        }

        break;
      }

      case ';': //terminator
        bContinue = FALSE;
        break;

      default:
        bPreviousWasNull = (ch == 0);
        break;
      }
    }
  }

  Seek(pos, SEEK_SET);
  return nframes;
}

static int gif_load_impl(gif_load_t* fmt, stream_t* m_strm, int req_comp, img_t* im)
{
  struct_image image;
  //long first_transparent_index;
  int iImage;
  char ch;
  BOOL bPreviousWasNull = TRUE;
  int prevdispmeth = 0;
  BOOL bContinue;
  fmt->m_strm = m_strm;
  // read header

  stream_seek(m_strm, 0, SEEK_SET);
  Read(&fmt->dscgif,/*sizeof(dscgif)*/13, 1);

  //if (strncmp(dscgif.header,"GIF8",3)!=0)
  if (strncmp(fmt->dscgif.header, "GIF8", 4) != 0) {
    return FALSE;
  }

  // Avoid Byte order problem with Mac <AMSN>
  fmt->dscgif.scrheight = bit_ntohs(fmt->dscgif.scrheight);
  fmt->dscgif.scrwidth = bit_ntohs(fmt->dscgif.scrwidth);

  /* AD - for interlace */
  fmt->TabCol.sogct = (short)(1 << ((fmt->dscgif.pflds & 0x07) + 1));
  fmt->TabCol.colres = (short)(((fmt->dscgif.pflds & 0x70) >> 4) + 1);

  // assume that the image is a truecolor-gif if
  // 1) no global color map found
  // 2) (image.w, image.h) of the 1st image != (dscgif.scrwidth, dscgif.scrheight)
  fmt->bTrueColor = 0;

  // Global colour map?
  if (fmt->dscgif.pflds & 0x80) {
    Read(fmt->TabCol.paleta, sizeof(struct rgb_color)*fmt->TabCol.sogct, 1);
  }
  else {
    fmt->bTrueColor++; //first chance for a truecolor gif
  }

  fmt->first_transparent_index = 0;

  fmt->nFrame = gif_get_num_frames(fmt, m_strm, &fmt->TabCol, &fmt->dscgif);

  //im->tt.data=(unsigned char*)pmalloc(im->h*im->s*im->f);
  //memset(im->tt.data, 0, im->h*im->s*im->f);
  if (fmt->nFrame <= 0) {
    return FALSE;
  }

  imsetsize(im, fmt->dscgif.scrheight, fmt->dscgif.scrwidth, req_comp, fmt->nFrame);
  // read body

  iImage = 0;

  //it cannot be a TRUE color GIF with only one frame
  if (im->f == 1) {
    fmt->bTrueColor = 0;
  }

  for (bContinue = TRUE; bContinue;) {
    rgb_color locpal[256]; //Local Palette
    rgb_color* pcurpal;
    short palcount;

    if (Read(&ch, sizeof(ch), 1) != 1) {
      break;
    }

    if (bPreviousWasNull || ch == 0) {
      int badcode;
      image_iterator iter[1] = {0};

      switch (ch) {
      case '!': { // extension
        bContinue = gif_decode_extension(fmt, m_strm);
        break;
      }

      case ',': { // image
        unsigned char* IterImage;
        long pos_start;
        unsigned char* curImage = (unsigned char*)(im->tt.data + iImage * im->h * im->s);
        assert(sizeof(image) == 9);
        Read(&image, sizeof(image), 1);
        //avoid byte order problems with Solaris <candan> <AMSN>
        image.l = bit_ntohs(image.l);
        image.t = bit_ntohs(image.t);
        image.w = bit_ntohs(image.w);
        image.h = bit_ntohs(image.h);

        if (((image.l + image.w) > im->w) || ((image.t + image.h) > im->h)) {
          break;
        }

        // check if it could be a truecolor gif
        if ((iImage == 0) && (image.w != im->w) && (image.h != im->h)) {
          fmt->bTrueColor++;
        }

        pcurpal = fmt->TabCol.paleta; //Current Palette
        palcount = fmt->TabCol.sogct; //Current Palette color count

        // Local colour map?
        if (image.pf & 0x80) {
          palcount = (short)(1 << ((image.pf & 0x07) + 1));
          assert(3 == sizeof(struct rgb_color));
          Read(locpal, sizeof(struct rgb_color)*palcount, 1);
          pcurpal = locpal;
        }
        
#if 0
      int bpp; //<DP> select the correct bit per pixel value
        if (palcount <= 2) {
          bpp = 1;
        }
        else if (palcount <= 16) {
          bpp = 4;
        }
        else {
          bpp = 8;
        }

        backimage.CopyInfo(*this);

        if (iImage == 0) {
          //first frame: build image background
          backimage.Create(dscgif.scrwidth, dscgif.scrheight, bpp, CXIMAGE_FORMAT_GIF);
          first_transparent_index = fmt->nBkgndIndex;
          backimage.Clear((uint8)fmt->gifgce.transpcolindex);
          previousFrame = new CxImage(backimage);
          previousFrame->SetRetreiveAllFrames(FALSE);
        }
        else {
          if (prevdispmeth == 2) {
            backimage.Copy(*this, FALSE, FALSE, FALSE);
            backimage.Clear((uint8)first_transparent_index);
          }
          else if (prevdispmeth == 3) {
            backimage.Copy(*this, FALSE, FALSE, FALSE);
            backimage.Create(previousFrame->GetWidth(),
                previousFrame->GetHeight(),
                previousFrame->GetBpp(), CXIMAGE_FORMAT_GIF);
            memcpy(backimage.GetDIB(), previousFrame->GetDIB(),
                backimage.GetSize());
            backimage.AlphaSet(*previousFrame);
          }
          else {
            backimage.Copy(*this);
          }
        }

        //active frame
        Create(image.w, image.h, bpp, CXIMAGE_FORMAT_GIF);
#endif


        //generic frame: handle disposal method from previous one
        //Values :
        // 0 - No disposal specified. The decoder is not required to take any action.
        // 1 - Do not dispose. The graphic is to be left in place.
        // 2 - Restore to background color. The area used by the graphic must be restored to the background color.
        // 3 - Restore to previous. The decoder is required to restore the area overwritten by the graphic with
        // what was there prior to rendering the graphic.
        if (iImage == 0) {
          int ttt = fmt->gifgce.transpcolindex;
          //first_transparent_index = fmt->nBkgndIndex;
          //first frame: build image background
          //fmt->bGetAllFrames = FALSE;
        }

        //printf("%d\n", prevdispmeth);
        if (iImage > 0) {
          unsigned char* preImage = (unsigned char*)(im->tt.data + (iImage - 1) * im->h * im->s);

          if (prevdispmeth == 2) {
            memset(curImage, 255, im->h * im->s);
            //memset(curImage, 255, im->h * im->s);
            //backimage.Clear((uint8)first_transparent_index);
          }
          else if (prevdispmeth == 3) {
            memcpy(curImage, preImage, im->h * im->s);
            //memcpy(backimage.GetDIB(),previousFrame->GetDIB(), backimage.GetSize());
            //backimage.AlphaSet(*previousFrame);
          }
          else if (prevdispmeth == 1) {
            memcpy(curImage, preImage, im->h * im->s);
            //backimage.Copy(*this);
          }
          else if (prevdispmeth == 0) {
            memcpy(curImage, preImage, im->h * im->s);
          }
        }

        // 有局部调色板
        if ((image.pf & 0x80) || (fmt->dscgif.pflds & 0x80)) {
        }

        //IterImage=(unsigned char*)(im->tt.data+im->f*im->h*im->s-im->h*im->w);
        IterImage = (unsigned char*)pmalloc(im->h * im->w);
        memset(IterImage, fmt->nBkgndIndex, im->h * im->w);

        iter->h = im->h;
        iter->w = im->w;
        iter->Itx = 0;
        iter->Ity = image.h - 1;
        iter->s = im->w;
        iter->IterImage = IterImage;
        badcode = 0;
        fmt->ibf = GIFBUFTAM + 1;

        fmt->interlaced = image.pf & 0x40;
        fmt->iheight = image.h;
        fmt->istep = 8;
        fmt->iypos = 0;
        fmt->ipass = 0;

        pos_start = Tell();
        //if (fmt->interlaced) log << "Interlaced" << endl;
        LZW_decode(fmt, m_strm, iter, image.w, &badcode);

        prevdispmeth = (fmt->gifgce.flags >> 2) & 0x7;

        {
          int x, y;
          long ymin = MAX(0, (long)(im->h - image.t - image.h));
          long ymax = im->h - image.t;
          long xmin = image.l;
          long xmax = MIN(im->w, (image.l + image.w));

          //char buf[256]; _snprintf(buf, 256, ".\\out\\aaa_%03d.txt", iImage);
          //FPRINT2D(buf, "%3d, ", "\n", ymax, xmax, IterImage, im->w, 1);
          for (y = ymin; y < ymax; y++) {
            unsigned char* curImage_line = curImage + (im->h - 1 - y) * im->s;
            unsigned char* IterImage_line = IterImage + (y - ymin) * im->w;

            if (im->c < 3) {
              for (x = xmin; x < xmax; x++) {
                int x1 = x - xmin;
                int t = IterImage_line[x1];

                if (t != fmt->nBkgndIndex) {
                  curImage_line[x] = GREYSCALIZE(pcurpal[t].r, pcurpal[t].g, pcurpal[t].b);
                }
              }
            }
            else {
              for (x = xmin; x < xmax; x++) {
                int x1 = x - xmin;
                int t = IterImage_line[x1];

                if (t != fmt->nBkgndIndex) {
                  curImage_line[im->c * x + 0] = pcurpal[t].b;
                  curImage_line[im->c * x + 1] = pcurpal[t].g;
                  curImage_line[im->c * x + 2] = pcurpal[t].r;
                }
              }
            }
          }
        }

        pfree(IterImage);

        if (fmt->bTrueColor < 2) { //standard GIF: mix frame with background
          int asdf = 0;
          //backimage.GifMix(*this,image);
          //backimage.SetTransIndex(first_transparent_index);
          //backimage.SetPalette(im->pal);
          //Transfer(backimage,FALSE);
        }
        else { //it's a truecolor gif!
          //force full image decoding
          fmt->nFrame = im->f - 1;
          //build the RGB image
#if 0

          if (imaRGB == NULL) {
            imaRGB = new CxImage(dscgif.scrwidth, dscgif.scrheight, 24, CXIMAGE_FORMAT_GIF);
          }

          //copy the partial image into the full RGB image
          for (long y = 0; y < image.h; y++) {
            for (long x = 0; x < image.w; x++) {
              imaRGB->SetPixelColor(x + image.l, dscgif.scrheight - 1 - image.t - y, GetPixelColor(x, image.h - y - 1));
            }
          }

#endif
        }

        //restore the correct position in the file for the next image
        if (badcode) {
          gif_seek_next_image(fmt, pos_start);
        }
        else {
          Seek(-(fmt->ibfmax - fmt->ibf - 1), SEEK_CUR);
        }

#if 0

        if (fmt->bGetAllFrames && imaRGB == NULL) {
          if (iImage == 0) {
            DestroyFrames();
            ppFrames = new CxImage*[im->f];

            for (int frameIdx = 0; frameIdx < im->f; frameIdx++) {
              ppFrames[frameIdx] = NULL;
            }
          }

          ppFrames[iImage] = new CxImage(*this);
          ppFrames[iImage]->SetRetreiveAllFrames(FALSE);
        }

        if (prevdispmeth <= 1) {
          delete previousFrame;
          previousFrame = new CxImage(*this);
          previousFrame->SetRetreiveAllFrames(FALSE);
        }

#endif

        fmt->nFrame = im->f - 1;

        if (fmt->nFrame == iImage) {
          bContinue = FALSE;
        }
        else {
          iImage++;
        }

        break;
      }

      case ';': //terminator
        bContinue = FALSE;
        break;

      default:
        bPreviousWasNull = (ch == 0);
        break;
      }
    }
  }

#if 0

  if (bTrueColor >= 2 && imaRGB) {
    if (fmt->gifgce.flags & 0x1) {
      imaRGB->SetTransColor(GetPaletteColor((uint8)fmt->nBkgndIndex));
      imaRGB->SetTransIndex(0);
    }

    Transfer(*imaRGB);
  }

#endif
  return TRUE;
}
