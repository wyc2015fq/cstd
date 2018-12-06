
extern char error_string[90];
void exitmessage(char* message)
{
  printf("%s\n", message);
  exit(0);
}
// JPEG decoder module
// Copyright 1999 Cristi Cuturicu
char error_string[90];
#define exit_func(err) { strcpy(error_string, err); return 0;}
static const uint8 zigzag[64] = { //
  0, 1, 5, 6, 14, 15, 27, 28,
  2, 4, 7, 13, 16, 26, 29, 42,
  3, 8, 12, 17, 25, 30, 41, 43,
  9, 11, 18, 24, 31, 40, 44, 53,
  10, 19, 23, 32, 39, 45, 52, 54,
  20, 22, 33, 38, 46, 51, 55, 60,
  21, 34, 37, 47, 50, 56, 59, 61,
  35, 36, 48, 49, 57, 58, 62, 63
};
typedef struct {
  uint8 Length[17];  // k =1-16 ; L[k] indicates the number of Huffman codes of length k
  uint16 minor_code[17];  // indicates the value of the smallest Huffman code of length k
  uint16 major_code[17];  // similar, but the highest code
  uint8 V[65536];  // V[k][j] = Value associated to the j-th Huffman code of length k
  // High nibble = nr of previous 0 coefficients
  // Low nibble = size (in bits) of the coefficient which will be taken from the data stream
} Huffman_table;
#define BYTE_p(i) reader->bp=reader->file_buf[(i)++]
#define WORD_p(i) reader->wp=(((uint16)(reader->file_buf[(i)]))<<8) + reader->file_buf[(i)+1]; (i)+=2
typedef struct jpg_reader_t {
  uint8* file_buf; // the buffer we use for storing the entire JPG file
  uint8 bp; //current byte
  uint16 wp; //current word
  uint32 byte_pos; // current byte position
  // uint16 X_image_size,Y_image_size; // X,Y sizes of the image
  uint16 X_round, Y_round; // The dimensions rounded to multiple of Hmax*8 (X_round)
  // and Ymax*8 (Y_round)
  uint8* im_buffer; // RGBA image buffer
  uint32 X_image_bytes; // size in bytes of 1 line of the image = reader->X_round * 4
  uint32 y_inc_value ; // 32*reader->X_round; // used by decode_MCU_1x2,2x1,2x2
  uint8 YH, YV, CbH, CbV, CrH, CrV; // sampling factors (horizontal and vertical) for Y,Cb,Cr
  uint16 Hmax, Vmax;
  float QT[4][64]; // quantization tables, no more than 4 quantization tables (QT[0..3])
  //Huffman_table HTDC[4]; //DC huffman tables , no more than 4 (0..3)
  //Huffman_table HTAC[4]; //AC huffman tables                  (0..3)
  uint8 YQ_nr, CbQ_nr, CrQ_nr; // quantization table number for Y, Cb, Cr
  uint8 YDC_nr, CbDC_nr, CrDC_nr; // DC Huffman table number for Y,Cb, Cr
  uint8 YAC_nr, CbAC_nr, CrAC_nr; // AC Huffman table number for Y,Cb, Cr
  uint8 Restart_markers; // if 1 => Restart markers on , 0 => no restart markers
  uint16 MCU_restart; //Restart markers appears every MCU_restart MCU blocks
  int16 DCY, DCCb, DCCr; // Coeficientii DC pentru Y,Cb,Cr
  int16 DCT_coeff[64]; // Current DCT_coefficients
  //uint8 Y[64], Cb[64], Cr[64]; // Y, Cb, Cr of the current 8x8 block for the 1x1 case
  //uint8 Y_1[64], Y_2[64], Y_3[64], Y_4[64];
  uint8 tab_1[64], tab_2[64], tab_3[64], tab_4[64]; // tabelele de supraesantionare pt cele 4 blocuri
  int16 Cr_tab[256], Cb_tab[256]; // Precalculated Cr, Cb tables
  //int16 Cr_Cb_green_tab[65536];
  // Initial conditions:
  // reader->byte_pos = start position in the Huffman coded segment
  // WORD_get(w1); WORD_get(w2);wordval=w1;
  uint8 d_k; // Bit displacement in memory, relative to the offset of w1
  // it's always <16
  uint16 w1, w2; // w1 = First word in memory; w2 = Second word
  uint32 wordval ; // the actual used value in Huffman decoding.
  //uint32 mask[17];
  int shift_temp;
  Huffman_table HTDC[4]; //DC huffman tables , no more than 4 (0..3)
  Huffman_table HTAC[4]; //AC huffman tables                  (0..3)
} jpg_reader_t;
static int16 g_Cr_Cb_green_tab[65536];
// Initial conditions:
// reader->byte_pos = start position in the Huffman coded segment
// WORD_get(w1); WORD_get(w2);wordval=w1;
static const int16 neg_pow2[17] = {0, -1, -3, -7, -15, -31, -63, -127, -255, -511, -1023, -2047, -4095, -8191, -16383, -32767};
//static uint32 start_neg_pow2 = (uint32)neg_pow2;
#define JPG_RIGHT_SHIFT(x,shft)  \
  ((reader->shift_temp = (x)) < 0 ? \
      (reader->shift_temp >> (shft)) | ((~(0L)) << (32-(shft))) : \
      (reader->shift_temp >> (shft)))
#define JPGDESCALE(x,n)  JPG_RIGHT_SHIFT((x) + (1L << ((n)-1)), n)
#define JPG_RANGE_MASK 1023L
static uint8 _rlimit_table[5 * 256 + 128];
uchar* rlimit_table = _rlimit_table;


#if 1
//int16 get_svalue(jpg_reader_t* reader, uint8 k) {
//  int aa = ((reader->wordval) << k) >> 16;
//  if (!(aa & (1 << (k - 1)))) {
//    aa += neg_pow2[k];
//  }
//  return aa;
//}
//uint16 lookKbits(jpg_reader_t* reader, uint8 k) { return reader->wordval >> (16 - k); }
#define lookKbits(reader, k)   (reader->wordval >> (16 - (uint8)(k)))
#define WORD_hi_lo(byte_high, byte_low)   (uint16)((uint8)((byte_high) << 8) | (uint8)(byte_low))
// uint16 WORD_hi_lo(uint8 byte_high, uint8 byte_low) { return (byte_high << 8) | (byte_low); }
#else
#ifdef _MSC_VER
uint16 lookKbits1(uint8 k)
{
  _asm {
    mov dl, k
    mov cl, 16
    sub cl, dl
    mov eax, [reader->wordval]
    shr eax, cl
  }
}
uint16 WORD_hi_lo1(uint8 byte_high, uint8 byte_low)
{
  _asm {
    mov ah, byte_high
    mov al, byte_low
  }
}
// k>0 always
// Takes k bits out of the BIT stream (wordval), and makes them a signed value
int16 get_svalue(uint8 k)
{
  _asm {
    xor ecx, ecx
    mov cl, k
    mov eax, [reader->wordval]
    shl eax, cl
    shr eax, 16
    dec cl
    bt eax, ecx
    jc end_macro
    signed_value:inc cl
    mov ebx, [start_neg_pow2]
    add ax, word ptr [ebx+ecx*2]
    end_macro:
  }
}
#endif
#ifdef __WATCOMC__
uint16 lookKbits(uint8 k);
#pragma aux lookKbits=\
            "mov eax,[wordval]"\
            "mov cl, 16"\
            "sub cl, dl"\
            "shr eax, cl"\
             parm [dl] \
             value [ax] \
             modify [eax cl];
uint16 WORD_hi_lo(uint8 byte_high, uint8 BYTE_low);
#pragma aux WORD_hi_lo=\
         parm [ah] [al]\
         value [ax] \
         modify [ax];
// k>0 always
// Takes k bits out of the BIT stream (wordval), and makes them a signed value
#pragma aux get_svalue=\
      "xor ecx, ecx"\
      "mov cl, al"\
      "mov eax,[wordval]"\
      "shl eax, cl"\
      "shr eax, 16"\
      "dec cl"\
      "bt eax,ecx"\
      "jc end_macro"\
      "signed_value:inc cl"\
      "mov ebx,[start_neg_pow2]"\
      "add ax,word ptr [ebx+ecx*2]"\
      "end_macro:"\
      parm [al]\
      modify [eax ebx ecx]\
      value [ax];
#endif
#endif
void skipKbits(jpg_reader_t* reader, uint8 k)
{
  uint8 b_high, b_low;
  reader->d_k += k;

  if (reader->d_k >= 16) {
    reader->d_k -= 16;
    reader->w1 = reader->w2;
    // Get the next word in w2
    BYTE_p(reader->byte_pos);

    if (reader->bp != 0xFF) {
      b_high = reader->bp;
    }
    else {
      if (reader->file_buf[reader->byte_pos] == 0) {
        reader->byte_pos++;  //skip 00
      }
      else {
        reader->byte_pos--;  // stop reader->byte_pos pe restart marker
      }

      b_high = 0xFF;
    }

    BYTE_p(reader->byte_pos);

    if (reader->bp != 0xFF) {
      b_low = reader->bp;
    }
    else {
      if (reader->file_buf[reader->byte_pos] == 0) {
        reader->byte_pos++;  //skip 00
      }
      else {
        reader->byte_pos--;  // stop reader->byte_pos pe restart marker
      }

      b_low = 0xFF;
    }

    reader->w2 = WORD_hi_lo(b_high, b_low);
  }

  reader->wordval = ((uint32)(reader->w1) << 16) + reader->w2;
  reader->wordval <<= reader->d_k;
  reader->wordval >>= 16;
}
int16 getKbits(jpg_reader_t* reader, uint8 k)
{
  int16 signed_wordvalue;
  //signed_wordvalue = get_svalue(reader, k);
  signed_wordvalue = ((reader->wordval) << k) >> 16;

  if (!(signed_wordvalue & (1 << (k - 1)))) {
    signed_wordvalue += neg_pow2[k];
  }

  skipKbits(reader, k);
  return signed_wordvalue;
}
void process_Huffman_data_unit(jpg_reader_t* reader, uint8 DC_nr, uint8 AC_nr, int16* previous_DC)
{
  // Process one data unit. A data unit = 64 DCT coefficients
  // Data is decompressed by Huffman decoding, then the array is dezigzag-ed
  // The result is a 64 DCT coefficients array: DCT_coeff
  uint8 nr, k, j, EOB_found;
  register uint16 tmp_Hcode;
  uint8 size_val, count_0;
  uint16* min_code, *maj_code; // min_code[k]=minimum code of length k, maj_code[k]=similar but maximum
  uint16* max_val, *min_val;
  uint8* huff_values;
  int16 DCT_tcoeff[64];
  uint8 byte_temp;
  // Start Huffman decoding
  // First the DC coefficient decoding
  min_code = reader->HTDC[DC_nr].minor_code;
  maj_code = reader->HTDC[DC_nr].major_code;
  huff_values = reader->HTDC[DC_nr].V;

  for (nr = 0; nr < 64 ; nr++) {
    DCT_tcoeff[nr] = 0;  //Initialize DCT_tcoeff
  }

  nr = 0; // DC coefficient
  min_val = &min_code[1];
  max_val = &maj_code[1];

  for (k = 1; k <= 16; k++) {
    tmp_Hcode = lookKbits(reader, k);

    //     max_val = &maj_code[k]; min_val = &min_code[k];
    if ((tmp_Hcode <= *max_val) && (tmp_Hcode >= *min_val)) { //Found a valid Huffman code
      skipKbits(reader, k);
      size_val = huff_values[WORD_hi_lo(k, (uint8)(tmp_Hcode - *min_val))];

      if (size_val == 0) {
        DCT_tcoeff[0] = *previous_DC;
      }
      else {
        DCT_tcoeff[0] = *previous_DC + getKbits(reader, size_val);
        *previous_DC = DCT_tcoeff[0];
      }

      break;
    }

    min_val++;
    max_val++;
  }

  // Second, AC coefficient decoding
  min_code = reader->HTAC[AC_nr].minor_code;
  maj_code = reader->HTAC[AC_nr].major_code;
  huff_values = reader->HTAC[AC_nr].V;
  nr = 1; // AC coefficient
  EOB_found = 0;

  while ((nr <= 63) && (!EOB_found)) {
    max_val = &maj_code[1];
    min_val = &min_code[1];

    for (k = 1; k <= 16; k++) {
      tmp_Hcode = lookKbits(reader, k);

      // max_val = &maj_code[k]; &min_val = min_code[k];
      if ((tmp_Hcode <= *max_val) && (tmp_Hcode >= *min_val)) {
        skipKbits(reader, k);
        byte_temp = huff_values[WORD_hi_lo(k, (uint8)(tmp_Hcode - *min_val))];
        size_val = byte_temp & 0xF;
        count_0 = byte_temp >> 4;

        if (size_val == 0) {
          if (count_0 == 0) {
            EOB_found = 1;
          }
          else if (count_0 == 0xF) {
            nr += 16;  //skip 16 zeroes
          }
        }
        else {
          nr += count_0; //skip count_0 zeroes
          DCT_tcoeff[nr++] = getKbits(reader, size_val);
        }

        break;
      }

      min_val++;
      max_val++;
    }

    if (k > 16) {
      nr++;  // This should not occur
    }
  }

  for (j = 0; j <= 63; j++) {
    reader->DCT_coeff[j] = DCT_tcoeff[zigzag[j]];  // Et, voila ... DCT_coeff
  }

  return ;
}
// Fast float IDCT transform
void IDCT_transform(jpg_reader_t* reader, const int16* incoeff, uint8* outcoeff, uint8 Q_nr)
{
  uint8 x;
  int8 y;
  const int16* inptr;
  uint8* outptr;
  float workspace[64];
  float* wsptr;//Workspace pointer
  float* quantptr; // Quantization table pointer
  float dcval;
  float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
  float tmp10, tmp11, tmp12, tmp13;
  float z5, z10, z11, z12, z13;
  const uint8* range_limit = rlimit_table + 128;
  // Pass 1: process COLUMNS from input and store into work array.
  wsptr = workspace;
  inptr = incoeff;
  quantptr = reader->QT[Q_nr];

  for (y = 0; y <= 7; y++) {
    if ((inptr[8] | inptr[16] | inptr[24] | inptr[32] | inptr[40] | inptr[48] | inptr[56]) == 0) {
      // AC terms all zero (in a column)
      dcval = inptr[0] * quantptr[0];
      wsptr[0]  = dcval;
      wsptr[8]  = dcval;
      wsptr[16] = dcval;
      wsptr[24] = dcval;
      wsptr[32] = dcval;
      wsptr[40] = dcval;
      wsptr[48] = dcval;
      wsptr[56] = dcval;
      inptr++;
      quantptr++;
      wsptr++;//advance pointers to next column
      continue ;
    }

    //Even part
    tmp0 = inptr[0] * quantptr[0];
    tmp1 = inptr[16] * quantptr[16];
    tmp2 = inptr[32] * quantptr[32];
    tmp3 = inptr[48] * quantptr[48];
    tmp10 = tmp0 + tmp2;// phase 3
    tmp11 = tmp0 - tmp2;
    tmp13 = tmp1 + tmp3;// phases 5-3
    tmp12 = (tmp1 - tmp3) * 1.414213562f - tmp13; // 2*c4
    tmp0 = tmp10 + tmp13;// phase 2
    tmp3 = tmp10 - tmp13;
    tmp1 = tmp11 + tmp12;
    tmp2 = tmp11 - tmp12;
    // Odd part
    tmp4 = inptr[8] * quantptr[8];
    tmp5 = inptr[24] * quantptr[24];
    tmp6 = inptr[40] * quantptr[40];
    tmp7 = inptr[56] * quantptr[56];
    z13 = tmp6 + tmp5;// phase 6
    z10 = tmp6 - tmp5;
    z11 = tmp4 + tmp7;
    z12 = tmp4 - tmp7;
    tmp7 = z11 + z13;// phase 5
    tmp11 = (z11 - z13) * 1.414213562f; // 2*c4
    z5 = (z10 + z12) * 1.847759065f; // 2*c2
    tmp10 = 1.082392200f * z12 - z5; // 2*(c2-c6)
    tmp12 = -2.613125930f * z10 + z5;// -2*(c2+c6)
    tmp6 = tmp12 - tmp7;// phase 2
    tmp5 = tmp11 - tmp6;
    tmp4 = tmp10 + tmp5;
    wsptr[0]  = tmp0 + tmp7;
    wsptr[56] = tmp0 - tmp7;
    wsptr[8]  = tmp1 + tmp6;
    wsptr[48] = tmp1 - tmp6;
    wsptr[16] = tmp2 + tmp5;
    wsptr[40] = tmp2 - tmp5;
    wsptr[32] = tmp3 + tmp4;
    wsptr[24] = tmp3 - tmp4;
    inptr++;
    quantptr++;
    wsptr++;//advance pointers to the next column
  }

  //  Pass 2: process ROWS from work array, store into writer array.
  // Note that we must descale the results by a factor of 8 = 2^3
  outptr = outcoeff;
  wsptr = workspace;

  for (x = 0; x <= 7; x++) {
    // Even part
    tmp10 = wsptr[0] + wsptr[4];
    tmp11 = wsptr[0] - wsptr[4];
    tmp13 = wsptr[2] + wsptr[6];
    tmp12 = (wsptr[2] - wsptr[6]) * 1.414213562f - tmp13;
    tmp0 = tmp10 + tmp13;
    tmp3 = tmp10 - tmp13;
    tmp1 = tmp11 + tmp12;
    tmp2 = tmp11 - tmp12;
    // Odd part
    z13 = wsptr[5] + wsptr[3];
    z10 = wsptr[5] - wsptr[3];
    z11 = wsptr[1] + wsptr[7];
    z12 = wsptr[1] - wsptr[7];
    tmp7 = z11 + z13;
    tmp11 = (z11 - z13) * 1.414213562f;
    z5 = (z10 + z12) * 1.847759065f; // 2*c2
    tmp10 = 1.082392200f * z12 - z5;  // 2*(c2-c6)
    tmp12 = -2.613125930f * z10 + z5; // -2*(c2+c6)
    tmp6 = tmp12 - tmp7;
    tmp5 = tmp11 - tmp6;
    tmp4 = tmp10 + tmp5;
    // Final writer stage: scale down by a factor of 8
    outptr[0] = range_limit[(JPGDESCALE((int)(tmp0 + tmp7), 3)) & 1023L];
    outptr[7] = range_limit[(JPGDESCALE((int)(tmp0 - tmp7), 3)) & 1023L];
    outptr[1] = range_limit[(JPGDESCALE((int)(tmp1 + tmp6), 3)) & 1023L];
    outptr[6] = range_limit[(JPGDESCALE((int)(tmp1 - tmp6), 3)) & 1023L];
    outptr[2] = range_limit[(JPGDESCALE((int)(tmp2 + tmp5), 3)) & 1023L];
    outptr[5] = range_limit[(JPGDESCALE((int)(tmp2 - tmp5), 3)) & 1023L];
    outptr[4] = range_limit[(JPGDESCALE((int)(tmp3 + tmp4), 3)) & 1023L];
    outptr[3] = range_limit[(JPGDESCALE((int)(tmp3 - tmp4), 3)) & 1023L];
    wsptr += 8; //advance pointer to the next row
    outptr += 8;
  }
}
// Functia (ca optimizare) poate fi apelata si fara parametrii Y,Cb,Cr
// Stim ca va fi apelata doar in cazul 1x1
void convert_8x8_YCbCr_to_RGB(jpg_reader_t* reader, const uint8* Y, const uint8* Cb, const uint8* Cr, uint32 im_loc)
{
  uint32 x, y;
  uint8 im_nr;
  const uint8* Y_val = Y, *Cb_val = Cb, *Cr_val = Cr;
  uint8* ibuffer = reader->im_buffer + im_loc;

  for (y = 0; y < 8; y++) {
    im_nr = 0;

    for (x = 0; x < 8; x++) {
      ibuffer[im_nr++] = rlimit_table[*Y_val + reader->Cb_tab[*Cb_val]]; //B
      ibuffer[im_nr++] = rlimit_table[*Y_val + g_Cr_Cb_green_tab[WORD_hi_lo(*Cr_val, *Cb_val)]]; //G
      ibuffer[im_nr++] = rlimit_table[*Y_val + reader->Cr_tab[*Cr_val]]; // R
      /*
      // Monochrome display
           im_buffer[im_nr++] = *Y_val;
           im_buffer[im_nr++] = *Y_val;
           im_buffer[im_nr++] = *Y_val;
      */
      Y_val++;
      Cb_val++;
      Cr_val++;
      im_nr++;
    }

    ibuffer += reader->X_image_bytes;
  }
}
void convert_8x8_YCbCr_to_RGB_tab(jpg_reader_t* reader, const uint8* Y, const uint8* Cb, const uint8* Cr, const uint8* tab, uint32 im_loc)
// Functia (ca optimizare) poate fi apelata si fara parametrii Cb,Cr
{
  uint32 x, y;
  uint8 nr, im_nr;
  uint8 Y_val, Cb_val, Cr_val;
  uint8* ibuffer = reader->im_buffer + im_loc;
  nr = 0;

  for (y = 0; y < 8; y++) {
    im_nr = 0;

    for (x = 0; x < 8; x++) {
      Y_val = Y[nr];
      Cb_val = Cb[tab[nr]];
      Cr_val = Cr[tab[nr]]; // reindexare folosind tabelul
      // de supraesantionare precalculat
      ibuffer[im_nr++] = rlimit_table[Y_val + reader->Cb_tab[Cb_val]]; //B
      ibuffer[im_nr++] = rlimit_table[Y_val + g_Cr_Cb_green_tab[WORD_hi_lo(Cr_val, Cb_val)]]; //G
      ibuffer[im_nr++] = rlimit_table[Y_val + reader->Cr_tab[Cr_val]]; // R
      nr++;
      im_nr++;
    }

    ibuffer += reader->X_image_bytes;
  }
}
void resync(jpg_reader_t* reader)
// reader->byte_pos  = pozitionat pe restart marker
{
  reader->byte_pos += 2;
  BYTE_p(reader->byte_pos);

  if (reader->bp == 0xFF) {
    reader->byte_pos++;  // skip 00
  }

  reader->w1 = WORD_hi_lo(reader->bp, 0);
  BYTE_p(reader->byte_pos);

  if (reader->bp == 0xFF) {
    reader->byte_pos++;  // skip 00
  }

  reader->w1 += reader->bp;
  BYTE_p(reader->byte_pos);

  if (reader->bp == 0xFF) {
    reader->byte_pos++;  // skip 00
  }

  reader->w2 = WORD_hi_lo(reader->bp, 0);
  BYTE_p(reader->byte_pos);

  if (reader->bp == 0xFF) {
    reader->byte_pos++;  // skip 00
  }

  reader->w2 += reader->bp;
  reader->wordval = reader->w1;
  reader->d_k = 0; // Reinit bitstream decoding
  reader->DCY = 0;
  reader->DCCb = 0;
  reader->DCCr = 0; // Init DC coefficients
}
void decode_MCU_1x1(jpg_reader_t* reader, uint32 im_loc)
{
  uint8 Y[64], Cb[64], Cr[64]; // Y, Cb, Cr of the current 8x8 block for the 1x1 case
  // Y
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y, reader->YQ_nr);
  // Cb
  process_Huffman_data_unit(reader, reader->CbDC_nr, reader->CbAC_nr, &reader->DCCb);
  IDCT_transform(reader, reader->DCT_coeff, Cb, reader->CbQ_nr);
  // Cr
  process_Huffman_data_unit(reader, reader->CrDC_nr, reader->CrAC_nr, &reader->DCCr);
  IDCT_transform(reader, reader->DCT_coeff, Cr, reader->CrQ_nr);
  convert_8x8_YCbCr_to_RGB(reader, Y, Cb, Cr, im_loc);
}
void decode_MCU_2x1(jpg_reader_t* reader, uint32 im_loc)
{
  uint8 Y_1[64], Y_2[64], Cb[64], Cr[64];
  // Y
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y_1, reader->YQ_nr);
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y_2, reader->YQ_nr);
  // Cb
  process_Huffman_data_unit(reader, reader->CbDC_nr, reader->CbAC_nr, &reader->DCCb);
  IDCT_transform(reader, reader->DCT_coeff, Cb, reader->CbQ_nr);
  // Cr
  process_Huffman_data_unit(reader, reader->CrDC_nr, reader->CrAC_nr, &reader->DCCr);
  IDCT_transform(reader, reader->DCT_coeff, Cr, reader->CrQ_nr);
  convert_8x8_YCbCr_to_RGB_tab(reader, Y_1, Cb, Cr, reader->tab_1, im_loc);
  convert_8x8_YCbCr_to_RGB_tab(reader, Y_2, Cb, Cr, reader->tab_2, im_loc + 32);
}
void decode_MCU_2x2(jpg_reader_t* reader, uint32 im_loc)
{
  uint8 Y_1[64], Y_2[64], Y_3[64], Y_4[64], Cb[64], Cr[64];
  // Y
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y_1, reader->YQ_nr);
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y_2, reader->YQ_nr);
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y_3, reader->YQ_nr);
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y_4, reader->YQ_nr);
  // Cb
  process_Huffman_data_unit(reader, reader->CbDC_nr, reader->CbAC_nr, &reader->DCCb);
  IDCT_transform(reader, reader->DCT_coeff, Cb, reader->CbQ_nr);
  // Cr
  process_Huffman_data_unit(reader, reader->CrDC_nr, reader->CrAC_nr, &reader->DCCr);
  IDCT_transform(reader, reader->DCT_coeff, Cr, reader->CrQ_nr);

  convert_8x8_YCbCr_to_RGB_tab(reader, Y_1, Cb, Cr, reader->tab_1, im_loc);
  convert_8x8_YCbCr_to_RGB_tab(reader, Y_2, Cb, Cr, reader->tab_2, im_loc + 32);
  convert_8x8_YCbCr_to_RGB_tab(reader, Y_3, Cb, Cr, reader->tab_3, im_loc + reader->y_inc_value);
  convert_8x8_YCbCr_to_RGB_tab(reader, Y_4, Cb, Cr, reader->tab_4, im_loc + reader->y_inc_value + 32);
}
void decode_MCU_1x2(jpg_reader_t* reader, uint32 im_loc)
{
  uint8 Y_1[64], Y_2[64], Cb[64], Cr[64];
  // Y
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y_1, reader->YQ_nr);
  process_Huffman_data_unit(reader, reader->YDC_nr, reader->YAC_nr, &reader->DCY);
  IDCT_transform(reader, reader->DCT_coeff, Y_2, reader->YQ_nr);
  // Cb
  process_Huffman_data_unit(reader, reader->CbDC_nr, reader->CbAC_nr, &reader->DCCb);
  IDCT_transform(reader, reader->DCT_coeff, Cb, reader->CbQ_nr);
  // Cr
  process_Huffman_data_unit(reader, reader->CrDC_nr, reader->CrAC_nr, &reader->DCCr);
  IDCT_transform(reader, reader->DCT_coeff, Cr, reader->CrQ_nr);
  convert_8x8_YCbCr_to_RGB_tab(reader, Y_1, Cb, Cr, reader->tab_1, im_loc);
  convert_8x8_YCbCr_to_RGB_tab(reader, Y_2, Cb, Cr, reader->tab_3, im_loc + reader->y_inc_value);
}
int jpg_load(stream_t* fp, int cn_req, img_t* im)
{
  uint32 X_image, Y_image;
  uint32 length_of_file;
  uint8 vers, units;
  uint16 Xdensity, Ydensity, Xthumbnail, Ythumbnail;
  uint16 length;
  float* qtable;
  uint32 old_byte_pos;
  Huffman_table* htable;
  int i, j, k;
  uint8 precision, comp_id, nr_components;
  uint8 QT_info, HT_info;
  uint8 SOS_found, SOF_found;
  uint32 mask[17];
  jpg_reader_t* reader;
  // Used markers:
  enum {
    SOI = 0xD8,
    EOI = 0xD9,
    APP0 = 0xE0,
    SOF = 0xC0,
    DQT = 0xDB,
    DHT = 0xC4,
    SOS = 0xDA,
    DRI = 0xDD,
    COM = 0xFE,
  };

  if (fp == NULL) {
    exitmessage("File not found ?");
  }

  //int load_JPEG_header(jpg_reader_t* reader, stream_t* fp, uint32* X_image, uint32* Y_image)
  length_of_file = stream_filesize(fp);

  reader = pmalloc(sizeof(jpg_reader_t));
  memset(reader, 0, sizeof(jpg_reader_t));
  reader->file_buf = (uint8*)pmalloc(length_of_file + 4);

  if (reader->file_buf == NULL) {
    exit_func("Not enough memory for loading file");
  }

  stream_read(fp, reader->file_buf, length_of_file);

  if ((reader->file_buf[0] != 0xFF) || (reader->file_buf[1] != SOI)) {
    exit_func("Not a JPG file ?\n");
  }

  if ((reader->file_buf[2] != 0xFF) || (reader->file_buf[3] != APP0)) {
    exit_func("Invalid JPG file.");
  }

  if ((reader->file_buf[6] != 'J') || (reader->file_buf[7] != 'F') || (reader->file_buf[8] != 'I') || (reader->file_buf[9] != 'F') ||
      (reader->file_buf[10] != 0)) {
    exit_func("Invalid JPG file.");
  }

  //init
  reader->byte_pos = 0;

  //init QT
  for (i = 0; i <= 3; i++) {
    //reader->QT[i] = (float*)mal1loc(sizeof(float) * 64);
  }

  // calculate_mask
  for (k = 0; k <= 16; k++) {
    uint32 tmpdv = 0x10000;  //precalculated bit mask
    mask[k] = (tmpdv >> k) - 1;
  }

  //void prepare_range_limit_table()
  // Allocate and fill in the sample_range_limit table
  {
    //rlimit_table = (uint8*)mal1loc(5 * 256L + 128) ;
    rlimit_table = _rlimit_table;
    /* First segment of "simple" table: limit[x] = 0 for x < 0 */
    memset((void*)rlimit_table, 0, 256);
    rlimit_table += 256;  /* allow negative subscripts of simple table */

    /* Main part of "simple" table: limit[x] = x */
    for (j = 0; j < 256; j++) {
      rlimit_table[j] = j;
    }

    /* End of simple table, rest of first half of post-IDCT table */
    for (j = 256; j < 640; j++) {
      rlimit_table[j] = 255;
    }

    /* Second half of post-IDCT table */
    memset((void*)(rlimit_table + 640), 0, 384);

    for (j = 0; j < 128 ; j++) {
      rlimit_table[j + 1024] = j;
    }
  }
  // precalculate_Cr_Cb_tables()
  {
    uint16 Cr_v, Cb_v;

    for (k = 0; k <= 255; k++) {
      reader->Cr_tab[k] = (int16)((k - 128.0) * 1.402);
    }

    for (k = 0; k <= 255; k++) {
      reader->Cb_tab[k] = (int16)((k - 128.0) * 1.772);
    }

    for (Cr_v = 0; Cr_v <= 255; Cr_v++) {
      for (Cb_v = 0; Cb_v <= 255; Cb_v++) {
        g_Cr_Cb_green_tab[((uint16)(Cr_v) << 8) + Cb_v] = (int)(-0.34414 * (Cb_v - 128.0) - 0.71414 * (Cr_v - 128.0));
      }
    }
  }
  //for future error check
  reader->byte_pos = 11;
  BYTE_p(reader->byte_pos);
  vers = reader->bp;

  if (vers != 1) {
    exit_func("JFIF version not supported");
  }

  BYTE_p(reader->byte_pos); // vers_lo=reader->bp;
  BYTE_p(reader->byte_pos);
  units = reader->bp;

  if (units != 0) //exit_func("JPG format not supported");
    ;// printf("units = %d\n", units);

  WORD_p(reader->byte_pos);
  Xdensity = reader->wp;
  WORD_p(reader->byte_pos);
  Ydensity = reader->wp;

  if ((Xdensity != 1) || (Ydensity != 1)) //exit_func("JPG format not supported");
    ;  //{printf("X density = %d\n",Xdensity); printf("Y density = %d\n",Ydensity);}

  BYTE_p(reader->byte_pos);
  Xthumbnail = reader->bp;
  BYTE_p(reader->byte_pos);
  Ythumbnail = reader->bp;

  if ((Xthumbnail != 0) || (Ythumbnail != 0)) {
    exit_func(" Cannot process JFIF thumbnailed files\n");
  }

  // Start decoding process
  SOS_found = 0;
  SOF_found = 0;
  reader->Restart_markers = 0;

  while ((reader->byte_pos < length_of_file) && !SOS_found) {
    BYTE_p(reader->byte_pos);

    if (reader->bp != 0xFF) {
      continue;
    }

    // A marker was found
    BYTE_p(reader->byte_pos);

    switch (reader->bp) {
    case DQT:
      WORD_p(reader->byte_pos);
      length = reader->wp; // length of the DQT marker

      for (j = 0; j < reader->wp - 2;) {
        old_byte_pos = reader->byte_pos;
        BYTE_p(reader->byte_pos);
        QT_info = reader->bp;

        if ((QT_info >> 4) != 0) {
          exit_func("16 bit quantization table not supported");
        }

        qtable = reader->QT[QT_info & 0xF];
        //load_quant_table(reader, qtable);
        //void load_quant_table(jpg_reader_t* reader, float* quant_table)
        {
          static const float scalefactor[8] = {1.0f, 1.387039845f, 1.306562965f, 1.175875602f, 1.0f, 0.785694958f, 0.541196100f, 0.275899379f };
          uint8 k, row, col;
          float* quant_table = qtable;

          // Load quantization coefficients from JPG file, scale them for DCT and reorder
          // from zig-zag order
          for (k = 0; k <= 63; k++) {
            quant_table[k] = reader->file_buf[reader->byte_pos + zigzag[k]];
          }

          k = 0;

          for (row = 0; row <= 7; row++) {
            for (col = 0; col <= 7; col++) {
              quant_table[k] *= scalefactor[row] * scalefactor[col];
              k++;
            }
          }

          reader->byte_pos += 64;
        }
        j += reader->byte_pos - old_byte_pos;
      }

      break;

    case DHT:
      WORD_p(reader->byte_pos);
      length = reader->wp;

      for (j = 0; j < reader->wp - 2;) {
        old_byte_pos = reader->byte_pos;
        BYTE_p(reader->byte_pos);
        HT_info = reader->bp;

        if ((HT_info & 0x10) != 0) {
          htable = &reader->HTAC[HT_info & 0xF];
        }
        else {
          htable = &reader->HTDC[HT_info & 0xF];
        }

        //load_Huffman_table(reader, htable);
        //void load_Huffman_table(jpg_reader_t* reader, Huffman_table* HT)
        {
          uint8 k, j;
          uint32 code;
          Huffman_table* HT = htable;

          for (j = 1; j <= 16; j++) {
            BYTE_p(reader->byte_pos);
            HT->Length[j] = reader->bp;
          }

          for (k = 1; k <= 16; k++) {
            for (j = 0; j < HT->Length[k]; j++) {
              BYTE_p(reader->byte_pos);
              HT->V[WORD_hi_lo(k, j)] = reader->bp;
            }
          }

          code = 0;

          for (k = 1; k <= 16; k++) {
            HT->minor_code[k] = (uint16)code;

            for (j = 1; j <= HT->Length[k]; j++) {
              code++;
            }

            HT->major_code[k] = (uint16)(code - 1);
            code *= 2;

            if (HT->Length[k] == 0) {
              HT->minor_code[k] = 0xFFFF;
              HT->major_code[k] = 0;
            }
          }
        }
        j += reader->byte_pos - old_byte_pos;
      }

      break;

    case COM:
      WORD_p(reader->byte_pos);
      length = reader->wp;
      reader->byte_pos += reader->wp - 2;
      break;

    case DRI:
      reader->Restart_markers = 1;
      WORD_p(reader->byte_pos);
      length = reader->wp; //should be = 4
      WORD_p(reader->byte_pos);
      reader->MCU_restart = reader->wp;

      if (reader->MCU_restart == 0) {
        reader->Restart_markers = 0;
      }

      break;

    case SOF:
      WORD_p(reader->byte_pos);
      length = reader->wp; //should be = 8+3*3=17
      BYTE_p(reader->byte_pos);
      precision = reader->bp;

      if (precision != 8) {
        exit_func("Only 8 bit precision supported");
      }

      WORD_p(reader->byte_pos);
      Y_image = reader->wp;
      WORD_p(reader->byte_pos);
      X_image = reader->wp;
      BYTE_p(reader->byte_pos);
      nr_components = reader->bp;

      if (nr_components != 3) {
        exit_func("Only truecolor JPGS supported");
      }

      for (j = 1; j <= 3; j++) {
        BYTE_p(reader->byte_pos);
        comp_id = reader->bp;

        if ((comp_id == 0) || (comp_id > 3)) {
          exit_func("Only YCbCr format supported");
        }

        switch (comp_id) {
        case 1: // Y
          BYTE_p(reader->byte_pos);
          reader->YH = reader->bp >> 4;
          reader->YV = reader->bp & 0xF;
          BYTE_p(reader->byte_pos);
          reader->YQ_nr = reader->bp;
          break;

        case 2: // Cb
          BYTE_p(reader->byte_pos);
          reader->CbH = reader->bp >> 4;
          reader->CbV = reader->bp & 0xF;
          BYTE_p(reader->byte_pos);
          reader->CbQ_nr = reader->bp;
          break;

        case 3: // Cr
          BYTE_p(reader->byte_pos);
          reader->CrH = reader->bp >> 4;
          reader->CrV = reader->bp & 0xF;
          BYTE_p(reader->byte_pos);
          reader->CrQ_nr = reader->bp;
          break;
        }
      }

      SOF_found = 1;
      break;

    case SOS:
      WORD_p(reader->byte_pos);
      length = reader->wp; //should be = 6+3*2=12
      BYTE_p(reader->byte_pos);
      nr_components = reader->bp;

      if (nr_components != 3) {
        exit_func("Invalid SOS marker");
      }

      for (j = 1; j <= 3; j++) {
        BYTE_p(reader->byte_pos);
        comp_id = reader->bp;

        if ((comp_id == 0) || (comp_id > 3)) {
          exit_func("Only YCbCr format supported");
        }

        switch (comp_id) {
        case 1: // Y
          BYTE_p(reader->byte_pos);
          reader->YDC_nr = reader->bp >> 4;
          reader->YAC_nr = reader->bp & 0xF;
          break;

        case 2: // Cb
          BYTE_p(reader->byte_pos);
          reader->CbDC_nr = reader->bp >> 4;
          reader->CbAC_nr = reader->bp & 0xF;
          break;

        case 3: // Cr
          BYTE_p(reader->byte_pos);
          reader->CrDC_nr = reader->bp >> 4;
          reader->CrAC_nr = reader->bp & 0xF;
          break;
        }
      }

      BYTE_p(reader->byte_pos);
      BYTE_p(reader->byte_pos);
      BYTE_p(reader->byte_pos); // Skip 3 bytes
      SOS_found = 1;
      break;

    case 0xFF:
      break; // do nothing for 0xFFFF, sequence of consecutive 0xFF are for

      // filling purposes and should be ignored
    default:
      WORD_p(reader->byte_pos);
      length = reader->wp;
      reader->byte_pos += reader->wp - 2; //skip unknown marker
      break;
    }
  }

  if (!SOS_found) {
    exit_func("Invalid JPG file. No SOS marker found.");
  }

  if (!SOF_found) {
    exit_func("Progressive JPEGs not supported");
  }

  if ((reader->CbH > reader->YH) || (reader->CrH > reader->YH)) {
    exit_func("Vertical sampling factor for Y should be >= sampling factor for Cb,Cr");
  }

  if ((reader->CbV > reader->YV) || (reader->CrV > reader->YV)) {
    exit_func("Horizontal sampling factor for Y should be >= sampling factor for Cb,Cr");
  }

  if ((reader->CbH >= 2) || (reader->CbV >= 2)) {
    exit_func("Cb sampling factors should be = 1");
  }

  if ((reader->CrV >= 2) || (reader->CrV >= 2)) {
    exit_func("Cr sampling factors should be = 1");
  }

  // Restricting sampling factors for Y,Cb,Cr should give us 4 possible cases for sampling factors
  // YHxYV,CbHxCbV,CrHxCrV: 2x2,1x1,1x1;  1x2,1x1,1x1; 2x1,1x1,1x1;
  // and 1x1,1x1,1x1 = no upsampling needed
  reader->Hmax = reader->YH, reader->Vmax = reader->YV;

  if (X_image % (reader->Hmax * 8) == 0) {
    reader->X_round = X_image;  // reader->X_round = Multiple of Hmax*8
  }
  else {
    reader->X_round = (X_image / (reader->Hmax * 8) + 1) * (reader->Hmax * 8);
  }

  if (Y_image % (reader->Vmax * 8) == 0) {
    reader->Y_round = Y_image;  // reader->Y_round = Multiple of Vmax*8
  }
  else {
    reader->Y_round = (Y_image / (reader->Vmax * 8) + 1) * (reader->Vmax * 8);
  }

  reader->im_buffer = (uint8*)pmalloc(reader->X_round * reader->Y_round * 4);

  if (reader->im_buffer == NULL) {
    exit_func("Not enough memory for storing the JPEG image");
  }

  // main decoder
  //decode_JPEG_image(reader);
  //void decode_JPEG_image(jpg_reader_t* reader)
  {
    typedef void (*decode_MCU_func)(jpg_reader_t * reader, uint32);
    decode_MCU_func decode_MCU;
    uint16 x_mcu_cnt, y_mcu_cnt;
    uint32 nr_mcu;
    uint16 X_MCU_nr, Y_MCU_nr; // Nr de MCU-uri
    uint32 MCU_dim_x; //dimensiunea in bufferul imagine a unui MCU pe axa x
    uint32 im_loc_inc; // = 7 * reader->X_round * 4 sau 15*reader->X_round*4;
    uint32 im_loc; //locatia in bufferul imagine
    uint8 tab_temp[256];
    uint8 x, y;
    reader->byte_pos -= 2;
    resync(reader);
    reader->y_inc_value = 32 * reader->X_round;

    // Calcul tabele de supraesantionare, tinand cont de YH si YV
    // Tabelul de supraesantionare 16x16
    for (y = 0; y < 16; y++) {
      for (x = 0; x < 16; x++) {
        tab_temp[y * 16 + x] = (y / reader->YV) * 8 + x / reader->YH;
      }
    }

    // Din el derivam tabelele corespunzatoare celor 4 blocuri de 8x8 pixeli
    for (y = 0; y < 8; y++) {
      for (x = 0; x < 8; x++) {
        reader->tab_1[y * 8 + x] = tab_temp[y * 16 + x];
      }

      for (x = 8; x < 16; x++) {
        reader->tab_2[y * 8 + (x - 8)] = tab_temp[y * 16 + x];
      }
    }

    for (y = 8; y < 16; y++) {
      for (x = 0; x < 8; x++) {
        reader->tab_3[(y - 8) * 8 + x] = tab_temp[y * 16 + x];
      }

      for (x = 8; x < 16; x++) {
        reader->tab_4[(y - 8) * 8 + (x - 8)] = tab_temp[y * 16 + x];
      }
    }

    if ((reader->YH == 1) && (reader->YV == 1)) {
      decode_MCU = decode_MCU_1x1;
    }
    else {
      if (reader->YH == 2) {
        if (reader->YV == 2) {
          decode_MCU = decode_MCU_2x2;
        }
        else {
          decode_MCU = decode_MCU_2x1;
        }
      }
      else {
        decode_MCU = decode_MCU_1x2;
      }
    }

    MCU_dim_x = reader->Hmax * 8 * 4;
    Y_MCU_nr = reader->Y_round / (reader->Vmax * 8); // nr of MCUs on Y axis
    X_MCU_nr = reader->X_round / (reader->Hmax * 8); // nr of MCUs on X axis
    reader->X_image_bytes = reader->X_round * 4;
    im_loc_inc = (reader->Vmax * 8 - 1) * reader->X_image_bytes;
    nr_mcu = 0;
    im_loc = 0; // memory location of the current MCU

    {
      utime_start(_start_time);
      int cnt = 0;

      for (y_mcu_cnt = 0; y_mcu_cnt < Y_MCU_nr; y_mcu_cnt++) {
        for (x_mcu_cnt = 0; x_mcu_cnt < X_MCU_nr; x_mcu_cnt++) {
          //{utime_start(_start_time);
          decode_MCU(reader, im_loc);
          ++cnt;
          //printf("%f\n", utime_elapsed(_start_time));}

          if ((reader->Restart_markers) && ((nr_mcu + 1) % reader->MCU_restart == 0)) {
            resync(reader);
          }

          nr_mcu++;
          im_loc += MCU_dim_x;
        }

        im_loc += im_loc_inc;
      }

      printf("%f\n", utime_elapsed(_start_time));
    }
  }
  //int get_JPEG_buffer(uint16 X_image, uint16 Y_image, uint8** address_dest_buffer)
  {
    uint16 y;
    uint32 dest_loc = 0;
    uint8* src_buffer = reader->im_buffer;
    uint8* dest_buffer;
    uint32 src_bytes_per_line = reader->X_round * 4;
    uint32 dest_bytes_per_line;
    cn_req = cn_req < 1 ? 4 : cn_req;
    imsetsize(im, Y_image, X_image, cn_req, 1);
    dest_bytes_per_line = im->s;
    dest_buffer = im->tt.data;

    if (4 == cn_req) {
      for (y = 0; y < Y_image; y++) {
        memcpy(dest_buffer, src_buffer, dest_bytes_per_line);
        src_buffer += src_bytes_per_line;
        dest_buffer += dest_bytes_per_line;
      }
    }
    else {
      for (y = 0; y < Y_image; y++) {
        rgb_fillrow(X_image, dest_buffer, cn_req, src_buffer, 32);
        src_buffer += src_bytes_per_line;
        dest_buffer += dest_bytes_per_line;
      }
    }
  }

  pfree(reader->im_buffer);
  // release the buffer which contains the JPG file
  pfree(reader->file_buf);
  pfree(reader);
  return 1;
}
#define jpg_load_filename(fname, cn_req, im) jpg_load_file_close(fopen(fname, "rb"), cn_req, im)
int jpg_load_file_close(FILE* pf, int cn_req, img_t* im)
{
  int ret = 0;

  if (pf) {
    stream_t s[1] = {0};
    fstream_init(s, pf);
    ret = jpg_load(s, cn_req, im);
    fclose(pf);
  }

  return ret;
}
#undef BYTE_p
#undef WORD_p
#undef MARKER_SOI
#undef MARKER_APP0
#undef MARKER_DQT
#undef MARKER_SOF0
#undef MARKER_DHT
#undef MARKER_SOS
#undef MARKER_EOI
#undef DCT_SIZE
#undef JPG_RIGHT_SHIFT
#undef JPG_RANGE_MASK
#undef JPGDESCALE
