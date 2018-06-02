// Layer1.java -- MPEG-1 Audio Layer I 解码

// Layer1也用到factor[]
// ISO/IEC 11172-3 Table 3-B.1
// scalefactor值为'0000 00'..'1111 11'(0..63),应该有64个值.在末尾补一个数0.0f
static float factor[] = {
  2.00000000000000f, 1.58740105196820f, 1.25992104989487f, 1.00000000000000f,
    0.79370052598410f, 0.62996052494744f, 0.50000000000000f, 0.39685026299205f,
    0.31498026247372f, 0.25000000000000f, 0.19842513149602f, 0.15749013123686f,
    0.12500000000000f, 0.09921256574801f, 0.07874506561843f, 0.06250000000000f,
    0.04960628287401f, 0.03937253280921f, 0.03125000000000f, 0.02480314143700f,
    0.01968626640461f, 0.01562500000000f, 0.01240157071850f, 0.00984313320230f,
    0.00781250000000f, 0.00620078535925f, 0.00492156660115f, 0.00390625000000f,
    0.00310039267963f, 0.00246078330058f, 0.00195312500000f, 0.00155019633981f,
    0.00123039165029f, 0.00097656250000f, 0.00077509816991f, 0.00061519582514f,
    0.00048828125000f, 0.00038754908495f, 0.00030759791257f, 0.00024414062500f,
    0.00019377454248f, 0.00015379895629f, 0.00012207031250f, 0.00009688727124f,
    0.00007689947814f, 0.00006103515625f, 0.00004844363562f, 0.00003844973907f,
    0.00003051757813f, 0.00002422181781f, 0.00001922486954f, 0.00001525878906f,
    0.00001211090890f, 0.00000961243477f, 0.00000762939453f, 0.00000605545445f,
    0.00000480621738f, 0.00000381469727f, 0.00000302772723f, 0.00000240310869f,
    0.00000190734863f, 0.00000151386361f, 0.00000120155435f, 0.0f
  };

struct Layer1 : public AbstractLayer {
  Header header;
  BitStream bs;
  byte allocation[2][32];  //[2][32]
  byte scalefactor[2][32]; //[2][32]
  float syin[2][32];     //[2][32]
  /**
   * 创建一个指定头信息和音频输出的LayerⅠ帧解码器。
   *
   * @param h
   *            已经解码的帧头信息。
   * @param audio
   *            音频输出对象。
   */
  Layer1(Header h, IAudio* audio) : bs(4096, 512) {
    set(h, audio);
    header = h;
  }
  /*
   * 逆量化公式:
   * s'' = (2^nb / (2^nb - 1)) * (s''' + 2^(-nb + 1))
   * s' = factor * s''
   */
  float requantization(int ch, int sb, int nb) {
    int samplecode = bs.getBits17(nb);
    int nlevels = (1 << nb);
    float requ = 2.0f * samplecode / nlevels - 1.0f;  //s'''
    requ += (float)pow(2, 1 - nb);
    requ *= nlevels / (nlevels - 1);    //s''
    requ *= factor[scalefactor[ch][sb]];  //s'
    return requ;
  }
  int decodeAudioData(byte* b, int off) {
    int sb, gr, ch, nb;
    int nch = header.getChannels();
    int bound = (header.getMode() == 1) ? ((header.getModeExtension() + 1) * 4) : 32;
    int intMainDataBytes = header.getMainDataSize();
    if (bs.append(b, off, intMainDataBytes) < intMainDataBytes) {
      return -1;
    }
    off += intMainDataBytes;
    int maindata_begin = bs.getBytePos();
    //1. Bit allocation decoding
    for (sb = 0; sb < bound; sb++)
      for (ch = 0; ch < nch; ++ch) {
        nb = bs.getBits9(4);
        if (nb == 15) {
          return -2;
        }
        allocation[ch][sb] = (byte)((nb != 0) ? (nb + 1) : 0);
      }
    for (sb = bound; sb < 32; sb++) {
      nb = bs.getBits9(4);
      if (nb == 15) {
        return -2;
      }
      allocation[0][sb] = (byte)((nb != 0) ? (nb + 1) : 0);
    }
    //2. Scalefactor decoding
    for (sb = 0; sb < 32; sb++)
      for (ch = 0; ch < nch; ch++)
        if (allocation[ch][sb] != 0) {
          scalefactor[ch][sb] = (byte)bs.getBits9(6);
        }
    for (gr = 0; gr < 12; gr++) {
      //3. Requantization of subband samples
      for (sb = 0; sb < bound; sb++) {
        for (ch = 0; ch < nch; ch++) {
          nb = allocation[ch][sb];
          if (nb == 0) {
            syin[ch][sb] = 0;
          }
          else {
            syin[ch][sb] = requantization(ch, sb, nb);
          }
        }
      }
      //mode=1(Joint Stereo)
      for (sb = bound; sb < 32; sb++)
        if ((nb = allocation[0][sb]) != 0)
          for (ch = 0; ch < nch; ch++) {
            syin[ch][sb] = requantization(ch, sb, nb);
          }
        else
          for (ch = 0; ch < nch; ch++) {
            syin[ch][sb] = 0;
          }
      //4. Synthesis subband filter
      for (ch = 0; ch < nch; ch++) {
        synthesisSubBand(syin[ch], ch);
      }
    }
    //5. Ancillary bits
    int discard = intMainDataBytes + maindata_begin - bs.getBytePos();
    bs.skipBytes(discard);
    //6. output
    outputPCM();
    return off;
  }
};

