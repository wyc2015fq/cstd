// Layer2.java -- MPEG-1/2/2.5 Audio Layer II 解码

// ISO/IEC 11172-3 Table 3-B.2a
// ISO/IEC 11172-3 Table 3-B.2b
// ISO/IEC 11172-3 Table 3-B.2c
// ISO/IEC 11172-3 Table 3-B.2d
// ISO/IEC 13818-3 Table B.1
static const byte nbal0[] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2 };
static const byte nbal1[] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2 };
static const byte nbal2[] = { 4, 4, 3, 3, 3, 3, 3, 3 };
static const byte nbal3[] = { 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };
static const byte nbal4[] = { 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
static const byte* nbal[8] = {nbal0,nbal1,nbal2,nbal3,nbal4};
static const byte aidx_table[3][2][16] = {
  {{0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 1, 1, 1, 1, 1, 0}, {0, 2, 2, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }},
  {{0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
  {{0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, 0}, {0, 3, 3, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }}
};
// cq_xxx: Layer II classes of quantization, ISO/IEC 11172-3 Table 3-B.4
static const int cq_steps[17] = { 3, 5, 7, 9, 15, 31, 63, 127, 255, 511, 1023,2047, 4095, 8191, 16383, 32767, 65535};
static const float cq_C[17] = { 1.3333333f, 1.6f, 1.1428571f, 1.77777778f, 1.0666667f, 1.0322581f, 1.015873f, 1.007874f, 1.0039216f,1.0019569f, 1.0009775f, 1.0004885f, 1.0002442f, 1.000122f,1.000061f, 1.0000305f, 1.00001525902f};
static const float cq_D[17] = { 0.5f, 0.5f, 0.25f, 0.5f, 0.125f, 0.0625f,0.03125f, 0.015625f, 0.0078125f, 0.00390625f, 0.001953125f,0.0009765625f, 0.00048828125f, 0.00024414063f, 0.00012207031f,0.00006103516f, 0.00003051758f};
static const byte cq_bits[17] = {5, 7, 3, 10, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

// ISO/IEC 11172-3 Table 3-B.2a
static const byte sbquant_offset0[] = {7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0};
// ISO/IEC 11172-3 Table 3-B.2b
static const byte sbquant_offset1[] = {7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0};
// ISO/IEC 11172-3 Table 3-B.2c
static const byte sbquant_offset2[] = {5, 5, 2, 2, 2, 2, 2, 2};
// ISO/IEC 11172-3 Table 3-B.2d
static const byte sbquant_offset3[] = {5, 5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
// ISO/IEC 13818-3 Table B.1
static const byte sbquant_offset4[] = {4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
static const byte* sbquant_offset[8] = {sbquant_offset0,sbquant_offset1,sbquant_offset2,sbquant_offset3,sbquant_offset4};
static const byte bitalloc_offset[8] =  { 0, 3, 3, 1, 2, 3, 4, 5 };
static const byte offset_table[6][15] = { { 0, 1, 16 }, { 0, 1, 2, 3, 4, 5, 16 },
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 },
{ 0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 16 },
{ 0, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }
};
const byte group[17] = { 2, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

struct Layer2 : public AbstractLayer {
  Header header;
  BitStream bs;
  int channels, aidx, sblimit;
  //byte aidx_table[3][2][16];  //[3][2][16]
  byte allocation[2][32];  //[2][32]
  //byte* nbal[5];      //[5][]
  const byte const * sbquant_offset[5];//[5][]
  byte scfsi[2][32];     //[2][32]
  byte scalefactor[2][32][3]; //[2][32][3]
  //int cq_steps[17];     //[17]
  //float cq_C[17];     //[17]
  //float cq_D[17];     //[17]
  //byte cq_bits[17];     //[17]
  //byte bitalloc_offset[8]; //[8]
  //byte offset_table[6][15];  //[6][15]
  //byte group[17];     //[17]
  int samplecode[3];   //[3]
  float syin[2][3][32];   //[2][3][32]
  /**
   * 创建一个指定头信息和音频输出的LayerⅡ帧解码器。
   *
   * @param h
   *            已经解码的帧头信息。
   * @param audio
   *            音频输出对象。
   */
  Layer2(Header h, IAudio* audio) : bs(4096, 512) {
    set(h, audio);
    header = h;
    channels = header.getChannels();
    //aidx,sblimit...
    if (header.getVersion() == MPEG2) {
      aidx = 4;
      sblimit = 30;
    }
    else {
      aidx = aidx_table[header.getSamplingFrequency()][2 - channels][header.getBitrateIndex()];
      if (aidx == 0) {
        sblimit = 27;
      }
      else if (aidx == 1) {
        sblimit = 30;
      }
      else if (aidx == 2) {
        sblimit = 8;
      }
      else {
        sblimit = 12;
      }
    }
  }
  void requantization(int index, int gr, int ch, int sb) {
    int nb, s, c;
    int nlevels = cq_steps[index];
    if ((nb = group[index]) != 0) {   // degrouping
      c = bs.getBits17(cq_bits[index]);
      for (s = 0; s < 3; s++) {
        samplecode[s] = c % nlevels;
        c /= nlevels;
      }
      nlevels = (1 << nb) - 1;  //用于计算fractional
    }
    else {
      nb = cq_bits[index];
      for (s = 0; s < 3; s++) {
        samplecode[s] = bs.getBits17(nb);
      }
    }
    for (s = 0; s < 3; s++) {
      float fractional = 2.0f * samplecode[s] / (nlevels + 1) - 1.0f;
      // s'' = C * (s''' + D)
      syin[ch][s][sb] = cq_C[index] * (fractional + cq_D[index]);
      // s' = factor * s''
      syin[ch][s][sb] *= factor[scalefactor[ch][sb][gr >> 2]];
    }
  }
  void stereo(int index, int gr, int sb) {
    int nb, s, c;
    int nlevels = cq_steps[index];
    if ((nb = group[index]) != 0) {
      c = bs.getBits17(cq_bits[index]);
      for (s = 0; s < 3; s++) {
        samplecode[s] = c % nlevels;
        c /= nlevels;
      }
      nlevels = (1 << nb) - 1;
    }
    else {
      nb = cq_bits[index];
      for (s = 0; s < 3; s++) {
        samplecode[s] = bs.getBits17(nb);
      }
    }
    for (s = 0; s < 3; s++) {
      float fractional = 2.0f * samplecode[s] / (nlevels + 1) - 1.0f;
      // s'' = C * (s''' + D)
      syin[0][s][sb] = syin[1][s][sb] = cq_C[index] * (fractional + cq_D[index]);
      // s' = factor * s''
      syin[0][s][sb] *= factor[scalefactor[0][sb][gr >> 2]];
      syin[1][s][sb] *= factor[scalefactor[1][sb][gr >> 2]];
    }
  }
  int decodeAudioData(byte* b, int off) {
    int maindata_begin , bound, sb, ch;
    int intMainDataBytes = header.getMainDataSize();
    if (bs.append(b, off, intMainDataBytes) < intMainDataBytes) {
      return off + intMainDataBytes;  // skip
    }
    off += intMainDataBytes;
    maindata_begin = bs.getBytePos();
    bound = (header.getMode() == 1) ? ((header.getModeExtension() + 1) << 2) : 32;
    if (bound > sblimit) {
      bound = sblimit;
    }
    /*
     * 1. Bit allocation decoding
     */
    for (sb = 0; sb < bound; sb++) {
      for (ch = 0; ch < channels; ch++) {
        allocation[ch][sb] = (byte)bs.getBits9(nbal[aidx][sb]);  // 2..4 bits
      }
    }
    for (sb = bound; sb < sblimit; sb++) {
      allocation[1][sb] = allocation[0][sb] = (byte)bs.getBits9(nbal[aidx][sb]);
    }
    /*
     * 2. Scalefactor selection information decoding
     */
    for (sb = 0; sb < sblimit; sb++) {
      for (ch = 0; ch < channels; ch++) {
        if (allocation[ch][sb] != 0) {
          scfsi[ch][sb] = (byte)bs.getBits9(2);
        }
        else {
          scfsi[ch][sb] = 0;
        }
      }
    }
    /*
     * 3. Scalefactor decoding
     */
    for (sb = 0; sb < sblimit; ++sb) {
      for (ch = 0; ch < channels; ++ch) {
        if (allocation[ch][sb] != 0) {
          scalefactor[ch][sb][0] = (byte)bs.getBits9(6);
          switch (scfsi[ch][sb]) {
          case 2:
            scalefactor[ch][sb][2] = scalefactor[ch][sb][1] = scalefactor[ch][sb][0];
            break;
          case 0:
            scalefactor[ch][sb][1] = (byte)bs.getBits9(6);
          case 1:
          case 3:
            scalefactor[ch][sb][2] = (byte)bs.getBits9(6);
          }
          if ((scfsi[ch][sb] & 1) == 1) {
            scalefactor[ch][sb][1] = scalefactor[ch][sb][scfsi[ch][sb] - 1];
          }
        }
      }
    }
    int gr, index, s;
    for (gr = 0; gr < 12; gr++) {
       // 4. Requantization of subband samples
      for (sb = 0; sb < bound; sb++) {
        for (ch = 0; ch < channels; ch++) {
          if ((index = allocation[ch][sb]) != 0) {
            index = offset_table[bitalloc_offset[sbquant_offset[aidx][sb]]][index - 1];
            requantization(index, gr, ch, sb);
          }
          else {
            syin[ch][0][sb] = syin[ch][1][sb] = syin[ch][2][sb] = 0;
          }
        }
      }
      //mode=1(Joint Stereo)
      for (sb = bound; sb < sblimit; sb++) {
        if ((index = allocation[0][sb]) != 0) {
          index = offset_table[bitalloc_offset[sbquant_offset[aidx][sb]]][index - 1];
          stereo(index, gr, sb);
        }
        else {
          for (ch = 0; ch < channels; ch++) {
            syin[ch][0][sb] = syin[ch][1][sb] = syin[ch][2][sb] = 0;
          }
        }
      }
      for (ch = 0; ch < channels; ch++) {
        for (s = 0; s < 3; s++) {
          for (sb = sblimit; sb < 32; sb++) {
            syin[ch][s][sb] = 0;
          }
        }
      }
      /*
       * 5. Synthesis subband filter
       */
      for (ch = 0; ch < channels; ch++) {
        for (s = 0; s < 3; s++) {
          synthesisSubBand(syin[ch][s], ch);
        }
      }
    } // for(gr...)
    /*
     * 6. Ancillary bits
     */
    int discard = intMainDataBytes + maindata_begin - bs.getBytePos();
    bs.skipBytes(discard);
    /*
     * 7. output
     */
    outputPCM();
    return off;
  }
};

