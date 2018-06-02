
typedef struct huff_writer_t {
  int m_bit_idx;
  ulong m_pad_val;
  ulong m_val;
  uchar m_start[1024];
  uchar* m_current;
  uchar* m_end;
  stream_t* m_user;
} huff_writer_t;

static int huff_writeblock(huff_writer_t* s)
{
  uchar* ptr = s->m_start;

  if (!bsIsBigEndian()) {
    bsBSwapBlock(s->m_start, s->m_current);
  }

  while (ptr < s->m_current) {
    int val = *ptr++;
    stream_put8(s->m_user, val);

    if (val == 0xff) {
      stream_put8(s->m_user, 0);
    }
  }

  s->m_current = s->m_start;
  return 0;
}

static int huff_init(huff_writer_t* s, stream_t* f)
{
  s->m_val = 0;
  s->m_bit_idx = 32;
  s->m_current = s->m_start;
  s->m_user = f;
  s->m_end = s->m_start + sizeof(s->m_start);
  return 0;
}

static void huff_Put(huff_writer_t* s, int val, int bits)
{
  int bit_idx = s->m_bit_idx - bits;
  ulong curval = s->m_val;

  assert(0 <= bits && bits < 32);

  val &= bs_bit_mask[bits];

  if (bit_idx >= 0) {
    curval |= val << bit_idx;
  }
  else {
    *((ulong*)s->m_current) = curval | ((unsigned)val >> -bit_idx);
    s->m_current += sizeof(ulong);

    if (s->m_current >= s->m_end) {
      huff_writeblock(s);
    }

    bit_idx += 32;
    curval = val << bit_idx;
  }

  s->m_val = curval;
  s->m_bit_idx = bit_idx;
}

static int huff_PutHuff(huff_writer_t* s, int val, const ulong* table)
{
  int min_val = (int)table[0];
  ulong code;
  val -= min_val;

  assert((unsigned)val < table[1]);

  code = table[val + 2];
  assert(code != 0);

  huff_Put(s, code >> 8, code & 255);
  return 0;
}

static void huff_close(huff_writer_t* s)
{
  huff_Put(s, -1, s->m_bit_idx & 31);
  *((ulong*)s->m_current) = s->m_val;
  s->m_current += sizeof(ulong);
  huff_writeblock(s);
  //ResetBuffer();
}

static BOOL jpg_save2(stream_t* f, const img_t* im, int QValue)
{
  const uchar* data = im->tt.data;
  huff_writer_t huff[1] = {0};
  int step = im->s, width = im->w, height = im->h, _channels = im->c;

  // encode the header and tables
  // for each mcu:
  // convert rgb to yuv with downsampling (if color).
  // for every block:
  // calc dct and quantize
  // encode block.
  int x, y;
  int i, j;
  //WMByteStream& lowstrm = m_strm.m_low_strm;
  int fdct_qtab[2][64];
  ulong huff_dc_tab[2][16];
  ulong huff_ac_tab[2][256];
  int channels = _channels > 1 ? 3 : 1;
  int x_scale = channels > 1 ? 2 : 1, y_scale = x_scale;
  int dc_pred[] = { 0, 0, 0 };
  int x_step = x_scale * 8;
  int y_step = y_scale * 8;
  int block[6][64];
  int buffer[1024];
  int luma_count = x_scale * y_scale;
  int block_count = luma_count + channels - 1;
  int Y_step = x_scale * 8;
  const int UV_step = 16;
  double inv_quality;
  const int max_quality = 12;
  double quality = QValue * max_quality / 100.;


  huff_init(huff, f);
  assert(data && width > 0 && height > 0);


  quality = BOUND(quality, 3, max_quality);

  inv_quality = 10. / quality;

  // Encode header
  stream_write(f, jpegHeader, sizeof(jpegHeader) - 1);

  // Encode quantization tables
  for (i = 0; i < (channels > 1 ? 2 : 1); i++) {
    const uchar* qtable = i == 0 ? jpegTableK1_T : jpegTableK2_T;
    int chroma_scale = i > 0 ? luma_count : 1;

    stream_put16be(f, 0xffdb); // DQT marker
    stream_put16be(f, 2 + 65 * 1); // put single qtable
    stream_put8(f, 0 * 16 + i); // 8-bit table

    // put coefficients
    for (j = 0; j < 64; j++) {
      int idx = zigzag[j];
      int qval = ROUND(qtable[idx] * inv_quality);

      if (qval < 1) {
        qval = 1;
      }

      if (qval > 255) {
        qval = 255;
      }

      fdct_qtab[i][idx] = ROUND((1 << (postshift + 9)) /
          (qval * chroma_scale * idct_prescale[idx]));
      stream_put8(f, qval);
    }
  }

  // Encode huffman tables
  for (i = 0; i < (channels > 1 ? 4 : 2); i++) {
    const uchar* htable = i == 0 ? jpegTableK3 : i == 1 ? jpegTableK5 :
        i == 2 ? jpegTableK4 : jpegTableK6;
    int is_ac_tab = i & 1;
    int idx = i >= 2;
    int tableSize = 16 + (is_ac_tab ? 162 : 12);

    stream_put16be(f, 0xFFC4); // DHT marker
    stream_put16be(f, 3 + tableSize); // define one huffman table
    stream_put8(f, is_ac_tab * 16 + idx); // put DC/AC flag and table index
    stream_write(f, htable, tableSize); // put table

    bsCreateEncodeHuffmanTable(bsCreateSourceHuffmanTable(htable, buffer, 16, 9),
        is_ac_tab ? huff_ac_tab[idx] : huff_dc_tab[idx], is_ac_tab ? 256 : 16);
  }

  // put frame header
  stream_put16be(f, 0xFFC0); // SOF0 marker
  stream_put16be(f, 8 + 3 * channels); // length of frame header
  stream_put8(f, 8);  // sample precision
  stream_put16be(f, height);
  stream_put16be(f, width);
  stream_put8(f, channels); // number of components

  for (i = 0; i < channels; i++) {
    stream_put8(f, i + 1); // (i+1)-th component id (Y,U or V)

    if (i == 0) {
      stream_put8(f, x_scale * 16 + y_scale); // chroma scale factors
    }
    else {
      stream_put8(f, 1 * 16 + 1);
    }

    stream_put8(f, i > 0); // quantization table idx
  }

  // put scan header
  stream_put16be(f, 0xFFDA); // SOS marker
  stream_put16be(f, 6 + 2 * channels); // length of scan header
  stream_put8(f, channels); // number of components in the scan

  for (i = 0; i < channels; i++) {
    stream_put8(f, i + 1); // component id
    stream_put8(f, (i > 0) * 16 + (i > 0)); // selection of DC & AC tables
  }

  stream_put16be(f, 0 * 256 + 63); // start and end of spectral selection - for
  // sequental DCT start is 0 and end is 63

  stream_put8(f, 0); // successive approximation bit position
  // high & low - (0,0) for sequental DCT

  // encode data
  for (y = 0; y < height; y += y_step, data += y_step * step) {
    for (x = 0; x < width; x += x_step) {
      int x_limit = x_step;
      int y_limit = y_step;
      const uchar* rgb_data = data + x * _channels;
      int* Y_data = block[0];

      if (x + x_limit > width) {
        x_limit = width - x;
      }

      if (y + y_limit > height) {
        y_limit = height - y;
      }

      memset(block, 0, block_count * 64 * sizeof(block[0][0]));

      if (channels > 1) {
        int* UV_data = block[luma_count];

        for (i = 0; i < y_limit; i++, rgb_data += step, Y_data += Y_step) {
          for (j = 0; j < x_limit; j++, rgb_data += _channels) {
            int r = rgb_data[2];
            int g = rgb_data[1];
            int b = rgb_data[0];

            int Y = descale(r * y_r + g * y_g + b * y_b, fixc - 2) - 128 * 4;
            int U = descale(r * cb_r + g * cb_g + b * cb_b, fixc - 2);
            int V = descale(r * cr_r + g * cr_g + b * cr_b, fixc - 2);
            int j2 = j >> (x_scale - 1);

            Y_data[j] = Y;
            UV_data[j2] += U;
            UV_data[j2 + 8] += V;
          }

          rgb_data -= x_limit * _channels;

          if (((i + 1) & (y_scale - 1)) == 0) {
            UV_data += UV_step;
          }
        }
      }
      else {
        for (i = 0; i < y_limit; i++, rgb_data += step, Y_data += Y_step) {
          for (j = 0; j < x_limit; j++) {
            Y_data[j] = rgb_data[j] * 4 - 128 * 4;
          }
        }
      }

      for (i = 0; i < block_count; i++) {
        int is_chroma = i >= luma_count;
        int src_step = x_scale * 8;
        int run = 0, val;
        int* src_ptr = block[i & -2] + (i & 1) * 8;
        const ulong* htable = huff_ac_tab[is_chroma];

        aan_fdct8x8(src_ptr, buffer, src_step, fdct_qtab[is_chroma]);

        j = is_chroma + (i > luma_count);
        val = buffer[0] - dc_pred[j];
        dc_pred[j] = buffer[0];

        {
          int cat;
          suf32_t a;
          a.f = (float)val;
          cat = (((a.i) >> 23) & 255) - (126 & (val ? -1 : 0));

          assert(cat <= 11);
          huff_PutHuff(huff, cat, huff_dc_tab[is_chroma]);
          huff_Put(huff, val - (val < 0 ? 1 : 0), cat);
        }

        for (j = 1; j < 64; j++) {
          val = buffer[zigzag[j]];

          if (val == 0) {
            run++;
          }
          else {
            while (run >= 16) {
              huff_PutHuff(huff, 0xF0, htable); // encode 16 zeros
              run -= 16;
            }

            {
              int cat;
              suf32_t a;
              a.f = (float)val;
              cat = (((a.i) >> 23) & 255) - (126 & (val ? -1 : 0));

              assert(cat <= 10);
              huff_PutHuff(huff, cat + run * 16, htable);
              huff_Put(huff, val - (val < 0 ? 1 : 0), cat);
            }

            run = 0;
          }
        }

        if (run) {
          huff_PutHuff(huff, 0x00, htable); // encode EOB
        }
      }
    }
  }

  // Flush
  //m_strm.Flush();
  huff_close(huff);

  stream_put16be(f, 0xFFD9); // EOI marker
  return TRUE;
}

#define jpg_save2_filename(fname, im, QValue) jpg_save2_file_close(fopen(fname, "wb"), im, QValue)
static int jpg_save2_file_close(FILE* pf, const img_t* im, int QValue)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = jpg_save2(s, im, QValue);
    fclose(pf);
  }

  return ret;
}

#undef fix
#undef fix1
#undef fixmul

#undef C0_707
#undef C0_924
#undef C0_541
#undef C0_382
#undef C1_306

#undef C1_082
#undef C1_414
#undef C1_847
#undef C2_613

#undef fixc
#undef b_cb
#undef g_cb
#undef g_cr
#undef r_cr

#undef y_r
#undef y_g
#undef y_b

#undef cb_r
#undef cb_g
#undef cb_b

#undef cr_r
#undef cr_g
#undef cr_b

