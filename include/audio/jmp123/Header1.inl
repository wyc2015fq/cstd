// MPEG-1/2/2.5 Audio Layer I/II/III 帧同步和帧头信息解码
/**
 * 帧同步及帧头信息解码。<p>源代码中解析帧头32位用到的变量及含义：
 * <table border="2" bordercolor="#000000" cellpadding="8" style="border-collapse:collapse">
 * <tr><th>偏移量</th><th>长度</th><th>变量名</th><th>含义</th></tr>
 * <tr><td>0</td><td>11</td><td>帧同步时直接解析</td><td>11位全设置为'1'的帧同步字</td></tr>
 * <tr><td>11</td><td>2</td><td>verID</td><td>MPEG版本</td></tr>
 * <tr><td>13</td><td>2</td><td>layer</td><td>MPEG压缩层</td></tr>
 * <tr><td>15</td><td>1</td><td>protection_bit</td><td>是否CRC</td></tr>
 * <tr><td>16</td><td>4</td><td>bitrate_index</td><td>位率索引</td></tr>
 * <tr><td>20</td><td>2</td><td>sampling_frequency</td><td>采样率索引</td></tr>
 * <tr><td>22</td><td>1</td><td>padding</td><td>当前帧是否附加填充一槽数据</td></tr>
 * <tr><td>23</td><td>1</td><td>未解析</td><td>告知是否私有</td></tr>
 * <tr><td>24</td><td>2</td><td>mode</td><td>声道模式</td></tr>
 * <tr><td>26</td><td>2</td><td>mode_extension</td><td>声道扩展模式</td></tr>
 * <tr><td>28</td><td>1</td><td>未解析</td><td>告知是否有版权</td></tr>
 * <tr><td>29</td><td>1</td><td>未解析</td><td>告知是否为原版</td></tr>
 * <tr><td>30</td><td>2</td><td>不常用，未解析</td><td>预加重</td></tr>
 * </table>
 * <p>
 * @version 0.400
 */
/**
 * MPEG版本MPEG-1。
 */
static const int MPEG1 = 3;
/**
 * MPEG版本MPEG-2。
 */
static const int MPEG2 = 2;
/**
 * MPEG版本MPEG-2.5（非官方版本）。
 */
static const int MPEG25 = 0;
//static int MAX_FRAMESIZE = 1732;
/*
 * 用lsf,layer,bitrate_index索引访问: bitrate[lsf][layer-1][bitrate_index]
*/
static const int bitrate[2][3][15] = {
  {
    //MPEG-1
    //Layer I
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},
    //Layer II
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384},
    //Layer III
    {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320}
  },
  {
    //MPEG-2/2.5
    //Layer I
    {0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256},
    //Layer II
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160},
    //Layer III
    {0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160}
  }
};
/*
 * samplingRate[verID][sampling_frequency]
 */
static const int samplingRate[4][4] = {
  {11025 , 12000 , 8000, 0}, //MPEG-2.5
  {0, 0, 0, 0,},      //reserved
  {22050, 24000, 16000 , 0}, //MPEG-2 (ISO/IEC 13818-3)
  {44100, 48000, 32000, 0}  //MPEG-1 (ISO/IEC 11172-3)
};
typedef struct Header {
  /*
   * verID: 2-bit
   * '00'  MPEG-2.5 (unofficial extension of MPEG-2);
   * '01'  reserved;
   * '10'  MPEG-2 (ISO/IEC 13818-3);
   * '11'  MPEG-1 (ISO/IEC 11172-3).
   */
  int verID;
  /*
   * layer: 2-bit
   * '11'  Layer I
   * '10'  Layer II
   * '01'  Layer III
   * '00'  reserved
   *
   * 已换算layer=4-layer: 1--Layer I; 2--Layer II; 3--Layer III; 4--reserved
   */
  int layer;
  /*
   * protection_bit: 1-bit
   * '1'  no CRC;
   * '0'  by 16-bit CRC following header.
   */
  int protection_bit;
  /*
   * bitrate_index: 4-bit
   */
  int bitrate_index;
  /*
   * sampling_frequency: 2-bit
   * '00'  44.1kHz
   * '01'  48kHz
   * '10'  32kHz
   * '11'  reserved
   */
  int sampling_frequency;
  int padding;
  /*
   * mode: 2-bit
   * '00'  Stereo;
   * '01'  Joint Stereo (Stereo);
   * '10'  Dual channel (Two mono channels);
   * '11'  Single channel (Mono).
   */
  int mode;
  /*
   * mode_extension: 2-bit
   *     intensity_stereo MS_stereo
   * '00'  off        off
   * '01'  on         off
   * '10'  off        on
   * '11'  on         on
   */
  int mode_extension;
  int framesize;
  int maindatasize;
  int sideinfosize;
  int lsf;
  BOOL isMS, isIntensityStereo;
} Header;
/**
 * 帧头解码。
 * @param h 帧头，大头在上的4字节(32位)整数。
 */
void Header_decode(int h, Header* s)
{
  s->verID = (h >> 19) & 3;
  s->layer = 4 - ((h >> 17) & 3);
  s->protection_bit = (h >> 16) & 0x1;
  s->bitrate_index = (h >> 12) & 0xF;
  s->sampling_frequency = (h >> 10) & 3;
  s->padding = (h >> 9) & 0x1;
  s->mode = (h >> 6) & 3;
  s->mode_extension = (h >> 4) & 3;
  s->isMS = s->mode == 1 && (s->mode_extension & 2) != 0;
  s->isIntensityStereo = s->mode == 1 && (s->mode_extension & 0x1) != 0;
  s->lsf = (s->verID == MPEG1) ? 0 : 1;
  switch (s->layer) {
  case 1:
    s->framesize = bitrate[s->lsf][0][s->bitrate_index] * 12000;
    s->framesize /= samplingRate[s->verID][s->sampling_frequency];
    s->framesize += s->padding;
    s->framesize <<= 2; // 1-slot = 4-byte
    break;
  case 2:
    s->framesize  = bitrate[s->lsf][1][s->bitrate_index] * 144000;
    s->framesize /= samplingRate[s->verID][s->sampling_frequency];
    s->framesize += s->padding;
    break;
  case 3:
    s->framesize = bitrate[s->lsf][2][s->bitrate_index] * 144000;
    s->framesize /= samplingRate[s->verID][s->sampling_frequency] << s->lsf;
    s->framesize += s->padding;
    //计算帧边信息长度
    if (s->verID == MPEG1) {
      s->sideinfosize = (s->mode == 3) ? 17 : 32;
    }
    else {
      s->sideinfosize = (s->mode == 3) ? 9 : 17;
    }
    break;
  }
  //计算主数据长度
  s->maindatasize = s->framesize - 4 - s->sideinfosize;
  if (s->protection_bit == 0) {
    s->maindatasize -= 2;  //CRC-word
  }
}
/**
 * 是否有循环冗余校验码。
 * @return 返回true表示有循环冗余校验码，帧头之后邻接有2字节的数据用于CRC。
 */
#define Header_isProtected(h) (h->protection_bit == 0)
/**
 * 获取声道模式是否为中/侧立体声（Mid/Side stereo）模式。
 *
 * isMS true表示是中/侧立体声模式。
*/
/**
 * 获取声道模式是否为强度立体声（Intensity Stereo）模式。
 *
 * isIntensityStereo true表示是强度立体声模式。
 */
/**
 * 获取当前帧的位率。
 *
 * @return 当前帧的位率，单位为“千位每秒（Kbps）”。
 */
#define Header_getBitrate(h, j) bitrate[h->lsf][h->layer - 1][h->bitrate_index];
/**
 * 获取当前帧的位率的索引值。
 *
 * @return 当前帧的位率的索引值，位率的索引值范围是1至14的某一整数。
 */
#define Header_getBitrateIndex(h)  h->bitrate_index
/**
 * 获取声道数。
 *
 * @return 声道数：1或2。
 */
#define Header_getChannels(h)  ((h->mode == 3) ? 1 : 2)
/**
 * 获取声道模式。
 *
 * @return 声道模式，其值表示的含义：
 * <table border="1" bordercolor="#000000" cellpadding="8" style="border-collapse:collapse">
 * <tr><th>返回值</th><th>声道模式</th></tr>
 * <tr><td>0</td><td>立体声（stereo）</td></tr>
 * <tr><td>1</td><td>联合立体声（joint stereo）</td></tr>
 * <tr><td>2</td><td>双声道（dual channel）</td></tr>
 * <tr><td>3</td><td>单声道（mono channel）</td></tr>
 * </table>
 * @see #getModeExtension()
 */
/**
 * 获取声道扩展模式。
 *
 * @return 声道扩展模式，该值表示当前声道使用的立体声编码方式：
 * <table border="1" bordercolor="#000000" cellpadding="8" style="border-collapse:collapse">
 * <tr><th>返回值</th><th>强度立体声</th><th>中/侧立体声</th></tr>
 * <tr><td>0</td><td>off</td><td>off</td></tr>
 * <tr><td>1</td><td>on</td><td>off</td></tr>
 * <tr><td>2</td><td>off</td><td>on</td></tr>
 * <tr><td>3</td><td>on</td><td>on</td></tr>
 * </table>
 * @see #getMode()
 */
#define Header_getModeExtension(h) h->mode_extension
/**
 * 获取MPEG版本。
 *
 * @return MPEG版本：{@link #MPEG1}、 {@link #MPEG2} 或 {@link #MPEG25} 。
 */
#define Header_getVersion(h)  h->verID
/**
 * 获取MPEG编码层。
 *
 * @return MPEG编码层：返回值1表示LayerⅠ，2表示LayerⅡ，3表示LayerⅢ。
 */
/**
 * 获取PCM样本采样率的索引值。
 *
 * @return PCM样本采样率的索引值。
 */
#define Header_getSamplingFrequency(h)  h->sampling_frequency
/**
 * 获取PCM样本采样率。
 *
 * @return 获取PCM样本采样率，单位“赫兹（Hz）”
 */
#define Header_getSamplingRate(h)   samplingRate[h->verID][h->sampling_frequency]
/**
 * 获取主数据长度。
 *
 * @return 当前帧的主数据长度，单位“字节”。
 */
#define Header_getMainDataSize(h)   h->maindatasize
/**
 * 获取边信息长度。
 *
 * @return 当前帧边信息长度，单位“字节”。
 */
#define Header_getSideInfoSize(h)   h->sideinfosize
/**
 * 获取帧长度。<p>帧的长度 = 4字节帧头 + CRC（如果有的话，2字节） + 音乐数据长度。
 * <br>其中音乐数据长度 = 边信息长度 + 主数据长度。
 * <p>无论是可变位率（VBR）编码的文件还是固定位率（CBR）编码的文件，每帧的长度不一定同。
 *
 * @return 当前帧的长度，单位“字节”。
 */
#define Header_getFrameSize(h) h->framesize
/**
 * 获取当前帧解码后得到的PCM样本长度。通常情况下同一文件每一帧解码后得到的PCM样本长度是相同的。
 *
 * @return 当前帧解码后得到的PCM样本长度，单位“字节”。
 */
int Header_getPcmSize(Header* h)
{
  int pcmsize = (h->verID == MPEG1) ? 4608 : 2304;
  if (h->mode == 3) { // if channels == 1
    pcmsize >>= 1;
  }
  return pcmsize;
}
/**
 * 获取当前文件一帧的播放时间长度。
 *
 * @return 当前文件一帧的播放时间长度，单位“秒”。
 */
#define Header_getFrameDuration(h) (1152.f / (Header_getSamplingRate(h) << h->lsf))
/**
 * 获取帧头的简短信息。
 * @return 帧头的简短信息。
 */
int Header_toString(const Header* s, char* sbuf, int len)
{
  int l2 = 0;
  if (len <= 0 || NULL == sbuf) {
    return 0;
  }
  sbuf[0] = 0;
  if (s->verID == MPEG25) {
    l2 += cstr_cat1(sbuf, len, "MPEG-2.5");
  }
  else if (s->verID == MPEG2) {
    l2 += cstr_cat1(sbuf, len, "MPEG-2");
  }
  else if (s->verID == MPEG1) {
    l2 += cstr_cat1(sbuf, len, "MPEG-1");
  }
  else {
    cstr_cpy(sbuf, len, "Let me tell you gently\nThe header is unavailable", -1);
    return 0;
  }
  l2 += cstr_cat1(sbuf, len, ", Layer ");
  cstr_cat_int32(sbuf, len, s->layer);
  l2 += cstr_cat1(sbuf, len, ", ");
  cstr_cat_int32(sbuf, len, Header_getSamplingRate(s));
  l2 += cstr_cat1(sbuf, len, "Hz, ");
  if (s->mode == 0) {
    l2 += cstr_cat1(sbuf, len, "Stereo");
  }
  else if (s->mode == 1) {
    l2 += cstr_cat1(sbuf, len, "Joint Stereo");
  }
  else if (s->mode == 2) {
    l2 += cstr_cat1(sbuf, len, "Dual channel");
  }
  else if (s->mode == 3) {
    l2 += cstr_cat1(sbuf, len, "Mono");
  }
  if (s->mode_extension == 1) {
    l2 += cstr_cat1(sbuf, len, "(I/S)");
  }
  else if (s->mode_extension == 2) {
    l2 += cstr_cat1(sbuf, len, "(M/S)");
  }
  else if (s->mode_extension == 3) {
    l2 += cstr_cat1(sbuf, len, "(I/S & M/S)");
  }
  return 0;
}

