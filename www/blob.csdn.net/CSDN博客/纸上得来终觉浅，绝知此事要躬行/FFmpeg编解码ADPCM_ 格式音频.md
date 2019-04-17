# FFmpeg编解码ADPCM_*格式音频 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月11日 11:15:37[boonya](https://me.csdn.net/boonya)阅读数：1498








ADPCM格式音频编解码：[https://lists.ffmpeg.org/doxygen/3.2/adpcm_8c_source.html](https://lists.ffmpeg.org/doxygen/3.2/adpcm_8c_source.html)

以下是代码：



```cpp
1 /*
    2  * Copyright (c) 2001-2003 The FFmpeg project
    3  *
    4  * first version by Francois Revol (revol@free.fr)
    5  * fringe ADPCM codecs (e.g., DK3, DK4, Westwood)
    6  *   by Mike Melanson (melanson@pcisys.net)
    7  * CD-ROM XA ADPCM codec by BERO
    8  * EA ADPCM decoder by Robin Kay (komadori@myrealbox.com)
    9  * EA ADPCM R1/R2/R3 decoder by Peter Ross (pross@xvid.org)
   10  * EA IMA EACS decoder by Peter Ross (pross@xvid.org)
   11  * EA IMA SEAD decoder by Peter Ross (pross@xvid.org)
   12  * EA ADPCM XAS decoder by Peter Ross (pross@xvid.org)
   13  * MAXIS EA ADPCM decoder by Robert Marston (rmarston@gmail.com)
   14  * THP ADPCM decoder by Marco Gerards (mgerards@xs4all.nl)
   15  *
   16  * This file is part of FFmpeg.
   17  *
   18  * FFmpeg is free software; you can redistribute it and/or
   19  * modify it under the terms of the GNU Lesser General Public
   20  * License as published by the Free Software Foundation; either
   21  * version 2.1 of the License, or (at your option) any later version.
   22  *
   23  * FFmpeg is distributed in the hope that it will be useful,
   24  * but WITHOUT ANY WARRANTY; without even the implied warranty of
   25  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   26  * Lesser General Public License for more details.
   27  *
   28  * You should have received a copy of the GNU Lesser General Public
   29  * License along with FFmpeg; if not, write to the Free Software
   30  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
   31  */
   32 #include "avcodec.h"
   33 #include "get_bits.h"
   34 #include "bytestream.h"
   35 #include "adpcm.h"
   36 #include "adpcm_data.h"
   37 #include "internal.h"
   38 
   39 /**
   40  * @file
   41  * ADPCM decoders
   42  * Features and limitations:
   43  *
   44  * Reference documents:
   45  * http://wiki.multimedia.cx/index.php?title=Category:ADPCM_Audio_Codecs
   46  * http://www.pcisys.net/~melanson/codecs/simpleaudio.html [dead]
   47  * http://www.geocities.com/SiliconValley/8682/aud3.txt [dead]
   48  * http://openquicktime.sourceforge.net/
   49  * XAnim sources (xa_codec.c) http://xanim.polter.net/
   50  * http://www.cs.ucla.edu/~leec/mediabench/applications.html [dead]
   51  * SoX source code http://sox.sourceforge.net/
   52  *
   53  * CD-ROM XA:
   54  * http://ku-www.ss.titech.ac.jp/~yatsushi/xaadpcm.html [dead]
   55  * vagpack & depack http://homepages.compuserve.de/bITmASTER32/psx-index.html [dead]
   56  * readstr http://www.geocities.co.jp/Playtown/2004/
   57  */
   58 
   59 /* These are for CD-ROM XA ADPCM */
   60 static const int xa_adpcm_table[5][2] = {
   61     {   0,   0 },
   62     {  60,   0 },
   63     { 115, -52 },
   64     {  98, -55 },
   65     { 122, -60 }
   66 };
   67 
   68 static const int ea_adpcm_table[] = {
   69     0,  240,  460,  392,
   70     0,    0, -208, -220,
   71     0,    1,    3,    4,
   72     7,    8,   10,   11,
   73     0,   -1,   -3,   -4
   74 };
   75 
   76 // padded to zero where table size is less then 16
   77 static const int swf_index_tables[4][16] = {
   78     /*2*/ { -1, 2 },
   79     /*3*/ { -1, -1, 2, 4 },
   80     /*4*/ { -1, -1, -1, -1, 2, 4, 6, 8 },
   81     /*5*/ { -1, -1, -1, -1, -1, -1, -1, -1, 1, 2, 4, 6, 8, 10, 13, 16 }
   82 };
   83 
   84 /* end of tables */
   85 
   86 typedef struct ADPCMDecodeContext {
   87     ADPCMChannelStatus status[14];
   88     int vqa_version;                /**< VQA version. Used for ADPCM_IMA_WS */
   89     int has_status;
   90 } ADPCMDecodeContext;
   91 
   92 static av_cold int adpcm_decode_init(AVCodecContext * avctx)
   93 {
   94     ADPCMDecodeContext *c = avctx->priv_data;
   95     unsigned int min_channels = 1;
   96     unsigned int max_channels = 2;
   97 
   98     switch(avctx->codec->id) {
   99     case AV_CODEC_ID_ADPCM_DTK:
  100     case AV_CODEC_ID_ADPCM_EA:
  101         min_channels = 2;
  102         break;
  103     case AV_CODEC_ID_ADPCM_AFC:
  104     case AV_CODEC_ID_ADPCM_EA_R1:
  105     case AV_CODEC_ID_ADPCM_EA_R2:
  106     case AV_CODEC_ID_ADPCM_EA_R3:
  107     case AV_CODEC_ID_ADPCM_EA_XAS:
  108         max_channels = 6;
  109         break;
  110     case AV_CODEC_ID_ADPCM_MTAF:
  111         min_channels = 2;
  112         max_channels = 8;
  113         break;
  114     case AV_CODEC_ID_ADPCM_PSX:
  115         max_channels = 8;
  116         break;
  117     case AV_CODEC_ID_ADPCM_IMA_DAT4:
  118     case AV_CODEC_ID_ADPCM_THP:
  119     case AV_CODEC_ID_ADPCM_THP_LE:
  120         max_channels = 14;
  121         break;
  122     }
  123     if (avctx->channels < min_channels || avctx->channels > max_channels) {
  124         av_log(avctx, AV_LOG_ERROR, "Invalid number of channels\n");
  125         return AVERROR(EINVAL);
  126     }
  127 
  128     switch(avctx->codec->id) {
  129     case AV_CODEC_ID_ADPCM_CT:
  130         c->status[0].step = c->status[1].step = 511;
  131         break;
  132     case AV_CODEC_ID_ADPCM_IMA_WAV:
  133         if (avctx->bits_per_coded_sample < 2 || avctx->bits_per_coded_sample > 5)
  134             return AVERROR_INVALIDDATA;
  135         break;
  136     case AV_CODEC_ID_ADPCM_IMA_APC:
  137         if (avctx->extradata && avctx->extradata_size >= 8) {
  138             c->status[0].predictor = AV_RL32(avctx->extradata);
  139             c->status[1].predictor = AV_RL32(avctx->extradata + 4);
  140         }
  141         break;
  142     case AV_CODEC_ID_ADPCM_IMA_WS:
  143         if (avctx->extradata && avctx->extradata_size >= 2)
  144             c->vqa_version = AV_RL16(avctx->extradata);
  145         break;
  146     default:
  147         break;
  148     }
  149 
  150     switch(avctx->codec->id) {
  151         case AV_CODEC_ID_ADPCM_AICA:
  152         case AV_CODEC_ID_ADPCM_IMA_DAT4:
  153         case AV_CODEC_ID_ADPCM_IMA_QT:
  154         case AV_CODEC_ID_ADPCM_IMA_WAV:
  155         case AV_CODEC_ID_ADPCM_4XM:
  156         case AV_CODEC_ID_ADPCM_XA:
  157         case AV_CODEC_ID_ADPCM_EA_R1:
  158         case AV_CODEC_ID_ADPCM_EA_R2:
  159         case AV_CODEC_ID_ADPCM_EA_R3:
  160         case AV_CODEC_ID_ADPCM_EA_XAS:
  161         case AV_CODEC_ID_ADPCM_THP:
  162         case AV_CODEC_ID_ADPCM_THP_LE:
  163         case AV_CODEC_ID_ADPCM_AFC:
  164         case AV_CODEC_ID_ADPCM_DTK:
  165         case AV_CODEC_ID_ADPCM_PSX:
  166         case AV_CODEC_ID_ADPCM_MTAF:
  167             avctx->sample_fmt = AV_SAMPLE_FMT_S16P;
  168             break;
  169         case AV_CODEC_ID_ADPCM_IMA_WS:
  170             avctx->sample_fmt = c->vqa_version == 3 ? AV_SAMPLE_FMT_S16P :
  171                                                       AV_SAMPLE_FMT_S16;
  172             break;
  173         default:
  174             avctx->sample_fmt = AV_SAMPLE_FMT_S16;
  175     }
  176 
  177     return 0;
  178 }
  179 
  180 static inline int16_t adpcm_ima_expand_nibble(ADPCMChannelStatus *c, int8_t nibble, int shift)
  181 {
  182     int step_index;
  183     int predictor;
  184     int sign, delta, diff, step;
  185 
  186     step = ff_adpcm_step_table[c->step_index];
  187     step_index = c->step_index + ff_adpcm_index_table[(unsigned)nibble];
  188     step_index = av_clip(step_index, 0, 88);
  189 
  190     sign = nibble & 8;
  191     delta = nibble & 7;
  192     /* perform direct multiplication instead of series of jumps proposed by
  193      * the reference ADPCM implementation since modern CPUs can do the mults
  194      * quickly enough */
  195     diff = ((2 * delta + 1) * step) >> shift;
  196     predictor = c->predictor;
  197     if (sign) predictor -= diff;
  198     else predictor += diff;
  199 
  200     c->predictor = av_clip_int16(predictor);
  201     c->step_index = step_index;
  202 
  203     return (int16_t)c->predictor;
  204 }
  205 
  206 static inline int16_t adpcm_ima_wav_expand_nibble(ADPCMChannelStatus *c, GetBitContext *gb, int bps)
  207 {
  208     int nibble, step_index, predictor, sign, delta, diff, step, shift;
  209 
  210     shift = bps - 1;
  211     nibble = get_bits_le(gb, bps),
  212     step = ff_adpcm_step_table[c->step_index];
  213     step_index = c->step_index + ff_adpcm_index_tables[bps - 2][nibble];
  214     step_index = av_clip(step_index, 0, 88);
  215 
  216     sign = nibble & (1 << shift);
  217     delta = av_mod_uintp2(nibble, shift);
  218     diff = ((2 * delta + 1) * step) >> shift;
  219     predictor = c->predictor;
  220     if (sign) predictor -= diff;
  221     else predictor += diff;
  222 
  223     c->predictor = av_clip_int16(predictor);
  224     c->step_index = step_index;
  225 
  226     return (int16_t)c->predictor;
  227 }
  228 
  229 static inline int adpcm_ima_qt_expand_nibble(ADPCMChannelStatus *c, int nibble, int shift)
  230 {
  231     int step_index;
  232     int predictor;
  233     int diff, step;
  234 
  235     step = ff_adpcm_step_table[c->step_index];
  236     step_index = c->step_index + ff_adpcm_index_table[nibble];
  237     step_index = av_clip(step_index, 0, 88);
  238 
  239     diff = step >> 3;
  240     if (nibble & 4) diff += step;
  241     if (nibble & 2) diff += step >> 1;
  242     if (nibble & 1) diff += step >> 2;
  243 
  244     if (nibble & 8)
  245         predictor = c->predictor - diff;
  246     else
  247         predictor = c->predictor + diff;
  248 
  249     c->predictor = av_clip_int16(predictor);
  250     c->step_index = step_index;
  251 
  252     return c->predictor;
  253 }
  254 
  255 static inline int16_t adpcm_ms_expand_nibble(ADPCMChannelStatus *c, int nibble)
  256 {
  257     int predictor;
  258 
  259     predictor = (((c->sample1) * (c->coeff1)) + ((c->sample2) * (c->coeff2))) / 64;
  260     predictor += ((nibble & 0x08)?(nibble - 0x10):(nibble)) * c->idelta;
  261 
  262     c->sample2 = c->sample1;
  263     c->sample1 = av_clip_int16(predictor);
  264     c->idelta = (ff_adpcm_AdaptationTable[(int)nibble] * c->idelta) >> 8;
  265     if (c->idelta < 16) c->idelta = 16;
  266     if (c->idelta > INT_MAX/768) {
  267         av_log(NULL, AV_LOG_WARNING, "idelta overflow\n");
  268         c->idelta = INT_MAX/768;
  269     }
  270 
  271     return c->sample1;
  272 }
  273 
  274 static inline int16_t adpcm_ima_oki_expand_nibble(ADPCMChannelStatus *c, int nibble)
  275 {
  276     int step_index, predictor, sign, delta, diff, step;
  277 
  278     step = ff_adpcm_oki_step_table[c->step_index];
  279     step_index = c->step_index + ff_adpcm_index_table[(unsigned)nibble];
  280     step_index = av_clip(step_index, 0, 48);
  281 
  282     sign = nibble & 8;
  283     delta = nibble & 7;
  284     diff = ((2 * delta + 1) * step) >> 3;
  285     predictor = c->predictor;
  286     if (sign) predictor -= diff;
  287     else predictor += diff;
  288 
  289     c->predictor = av_clip_intp2(predictor, 11);
  290     c->step_index = step_index;
  291 
  292     return c->predictor << 4;
  293 }
  294 
  295 static inline int16_t adpcm_ct_expand_nibble(ADPCMChannelStatus *c, int8_t nibble)
  296 {
  297     int sign, delta, diff;
  298     int new_step;
  299 
  300     sign = nibble & 8;
  301     delta = nibble & 7;
  302     /* perform direct multiplication instead of series of jumps proposed by
  303      * the reference ADPCM implementation since modern CPUs can do the mults
  304      * quickly enough */
  305     diff = ((2 * delta + 1) * c->step) >> 3;
  306     /* predictor update is not so trivial: predictor is multiplied on 254/256 before updating */
  307     c->predictor = ((c->predictor * 254) >> 8) + (sign ? -diff : diff);
  308     c->predictor = av_clip_int16(c->predictor);
  309     /* calculate new step and clamp it to range 511..32767 */
  310     new_step = (ff_adpcm_AdaptationTable[nibble & 7] * c->step) >> 8;
  311     c->step = av_clip(new_step, 511, 32767);
  312 
  313     return (int16_t)c->predictor;
  314 }
  315 
  316 static inline int16_t adpcm_sbpro_expand_nibble(ADPCMChannelStatus *c, int8_t nibble, int size, int shift)
  317 {
  318     int sign, delta, diff;
  319 
  320     sign = nibble & (1<<(size-1));
  321     delta = nibble & ((1<<(size-1))-1);
  322     diff = delta << (7 + c->step + shift);
  323 
  324     /* clamp result */
  325     c->predictor = av_clip(c->predictor + (sign ? -diff : diff), -16384,16256);
  326 
  327     /* calculate new step */
  328     if (delta >= (2*size - 3) && c->step < 3)
  329         c->step++;
  330     else if (delta == 0 && c->step > 0)
  331         c->step--;
  332 
  333     return (int16_t) c->predictor;
  334 }
  335 
  336 static inline int16_t adpcm_yamaha_expand_nibble(ADPCMChannelStatus *c, uint8_t nibble)
  337 {
  338     if(!c->step) {
  339         c->predictor = 0;
  340         c->step = 127;
  341     }
  342 
  343     c->predictor += (c->step * ff_adpcm_yamaha_difflookup[nibble]) / 8;
  344     c->predictor = av_clip_int16(c->predictor);
  345     c->step = (c->step * ff_adpcm_yamaha_indexscale[nibble]) >> 8;
  346     c->step = av_clip(c->step, 127, 24567);
  347     return c->predictor;
  348 }
  349 
  350 static inline int16_t adpcm_mtaf_expand_nibble(ADPCMChannelStatus *c, uint8_t nibble)
  351 {
  352     c->predictor += ff_adpcm_mtaf_stepsize[c->step][nibble];
  353     c->predictor = av_clip_int16(c->predictor);
  354     c->step += ff_adpcm_index_table[nibble];
  355     c->step = av_clip_uintp2(c->step, 5);
  356     return c->predictor;
  357 }
  358 
  359 static int xa_decode(AVCodecContext *avctx, int16_t *out0, int16_t *out1,
  360                      const uint8_t *in, ADPCMChannelStatus *left,
  361                      ADPCMChannelStatus *right, int channels, int sample_offset)
  362 {
  363     int i, j;
  364     int shift,filter,f0,f1;
  365     int s_1,s_2;
  366     int d,s,t;
  367 
  368     out0 += sample_offset;
  369     if (channels == 1)
  370         out1 = out0 + 28;
  371     else
  372         out1 += sample_offset;
  373 
  374     for(i=0;i<4;i++) {
  375         shift  = 12 - (in[4+i*2] & 15);
  376         filter = in[4+i*2] >> 4;
  377         if (filter >= FF_ARRAY_ELEMS(xa_adpcm_table)) {
  378             avpriv_request_sample(avctx, "unknown XA-ADPCM filter %d", filter);
  379             filter=0;
  380         }
  381         f0 = xa_adpcm_table[filter][0];
  382         f1 = xa_adpcm_table[filter][1];
  383 
  384         s_1 = left->sample1;
  385         s_2 = left->sample2;
  386 
  387         for(j=0;j<28;j++) {
  388             d = in[16+i+j*4];
  389 
  390             t = sign_extend(d, 4);
  391             s = ( t<<shift ) + ((s_1*f0 + s_2*f1+32)>>6);
  392             s_2 = s_1;
  393             s_1 = av_clip_int16(s);
  394             out0[j] = s_1;
  395         }
  396 
  397         if (channels == 2) {
  398             left->sample1 = s_1;
  399             left->sample2 = s_2;
  400             s_1 = right->sample1;
  401             s_2 = right->sample2;
  402         }
  403 
  404         shift  = 12 - (in[5+i*2] & 15);
  405         filter = in[5+i*2] >> 4;
  406         if (filter >= FF_ARRAY_ELEMS(xa_adpcm_table)) {
  407             avpriv_request_sample(avctx, "unknown XA-ADPCM filter %d", filter);
  408             filter=0;
  409         }
  410 
  411         f0 = xa_adpcm_table[filter][0];
  412         f1 = xa_adpcm_table[filter][1];
  413 
  414         for(j=0;j<28;j++) {
  415             d = in[16+i+j*4];
  416 
  417             t = sign_extend(d >> 4, 4);
  418             s = ( t<<shift ) + ((s_1*f0 + s_2*f1+32)>>6);
  419             s_2 = s_1;
  420             s_1 = av_clip_int16(s);
  421             out1[j] = s_1;
  422         }
  423 
  424         if (channels == 2) {
  425             right->sample1 = s_1;
  426             right->sample2 = s_2;
  427         } else {
  428             left->sample1 = s_1;
  429             left->sample2 = s_2;
  430         }
  431 
  432         out0 += 28 * (3 - channels);
  433         out1 += 28 * (3 - channels);
  434     }
  435 
  436     return 0;
  437 }
  438 
  439 static void adpcm_swf_decode(AVCodecContext *avctx, const uint8_t *buf, int buf_size, int16_t *samples)
  440 {
  441     ADPCMDecodeContext *c = avctx->priv_data;
  442     GetBitContext gb;
  443     const int *table;
  444     int k0, signmask, nb_bits, count;
  445     int size = buf_size*8;
  446     int i;
  447 
  448     init_get_bits(&gb, buf, size);
  449 
  450     //read bits & initial values
  451     nb_bits = get_bits(&gb, 2)+2;
  452     table = swf_index_tables[nb_bits-2];
  453     k0 = 1 << (nb_bits-2);
  454     signmask = 1 << (nb_bits-1);
  455 
  456     while (get_bits_count(&gb) <= size - 22*avctx->channels) {
  457         for (i = 0; i < avctx->channels; i++) {
  458             *samples++ = c->status[i].predictor = get_sbits(&gb, 16);
  459             c->status[i].step_index = get_bits(&gb, 6);
  460         }
  461 
  462         for (count = 0; get_bits_count(&gb) <= size - nb_bits*avctx->channels && count < 4095; count++) {
  463             int i;
  464 
  465             for (i = 0; i < avctx->channels; i++) {
  466                 // similar to IMA adpcm
  467                 int delta = get_bits(&gb, nb_bits);
  468                 int step = ff_adpcm_step_table[c->status[i].step_index];
  469                 int vpdiff = 0; // vpdiff = (delta+0.5)*step/4
  470                 int k = k0;
  471 
  472                 do {
  473                     if (delta & k)
  474                         vpdiff += step;
  475                     step >>= 1;
  476                     k >>= 1;
  477                 } while(k);
  478                 vpdiff += step;
  479 
  480                 if (delta & signmask)
  481                     c->status[i].predictor -= vpdiff;
  482                 else
  483                     c->status[i].predictor += vpdiff;
  484 
  485                 c->status[i].step_index += table[delta & (~signmask)];
  486 
  487                 c->status[i].step_index = av_clip(c->status[i].step_index, 0, 88);
  488                 c->status[i].predictor = av_clip_int16(c->status[i].predictor);
  489 
  490                 *samples++ = c->status[i].predictor;
  491             }
  492         }
  493     }
  494 }
  495 
  496 /**
  497  * Get the number of samples that will be decoded from the packet.
  498  * In one case, this is actually the maximum number of samples possible to
  499  * decode with the given buf_size.
  500  *
  501  * @param[out] coded_samples set to the number of samples as coded in the
  502  *                           packet, or 0 if the codec does not encode the
  503  *                           number of samples in each frame.
  504  * @param[out] approx_nb_samples set to non-zero if the number of samples
  505  *                               returned is an approximation.
  506  */
  507 static int get_nb_samples(AVCodecContext *avctx, GetByteContext *gb,
  508                           int buf_size, int *coded_samples, int *approx_nb_samples)
  509 {
  510     ADPCMDecodeContext *s = avctx->priv_data;
  511     int nb_samples        = 0;
  512     int ch                = avctx->channels;
  513     int has_coded_samples = 0;
  514     int header_size;
  515 
  516     *coded_samples = 0;
  517     *approx_nb_samples = 0;
  518 
  519     if(ch <= 0)
  520         return 0;
  521 
  522     switch (avctx->codec->id) {
  523     /* constant, only check buf_size */
  524     case AV_CODEC_ID_ADPCM_EA_XAS:
  525         if (buf_size < 76 * ch)
  526             return 0;
  527         nb_samples = 128;
  528         break;
  529     case AV_CODEC_ID_ADPCM_IMA_QT:
  530         if (buf_size < 34 * ch)
  531             return 0;
  532         nb_samples = 64;
  533         break;
  534     /* simple 4-bit adpcm */
  535     case AV_CODEC_ID_ADPCM_CT:
  536     case AV_CODEC_ID_ADPCM_IMA_APC:
  537     case AV_CODEC_ID_ADPCM_IMA_EA_SEAD:
  538     case AV_CODEC_ID_ADPCM_IMA_OKI:
  539     case AV_CODEC_ID_ADPCM_IMA_WS:
  540     case AV_CODEC_ID_ADPCM_YAMAHA:
  541     case AV_CODEC_ID_ADPCM_AICA:
  542         nb_samples = buf_size * 2 / ch;
  543         break;
  544     }
  545     if (nb_samples)
  546         return nb_samples;
  547 
  548     /* simple 4-bit adpcm, with header */
  549     header_size = 0;
  550     switch (avctx->codec->id) {
  551         case AV_CODEC_ID_ADPCM_4XM:
  552         case AV_CODEC_ID_ADPCM_IMA_DAT4:
  553         case AV_CODEC_ID_ADPCM_IMA_ISS:     header_size = 4 * ch;      break;
  554         case AV_CODEC_ID_ADPCM_IMA_AMV:     header_size = 8;           break;
  555         case AV_CODEC_ID_ADPCM_IMA_SMJPEG:  header_size = 4 * ch;      break;
  556     }
  557     if (header_size > 0)
  558         return (buf_size - header_size) * 2 / ch;
  559 
  560     /* more complex formats */
  561     switch (avctx->codec->id) {
  562     case AV_CODEC_ID_ADPCM_EA:
  563         has_coded_samples = 1;
  564         *coded_samples  = bytestream2_get_le32(gb);
  565         *coded_samples -= *coded_samples % 28;
  566         nb_samples      = (buf_size - 12) / 30 * 28;
  567         break;
  568     case AV_CODEC_ID_ADPCM_IMA_EA_EACS:
  569         has_coded_samples = 1;
  570         *coded_samples = bytestream2_get_le32(gb);
  571         nb_samples     = (buf_size - (4 + 8 * ch)) * 2 / ch;
  572         break;
  573     case AV_CODEC_ID_ADPCM_EA_MAXIS_XA:
  574         nb_samples = (buf_size - ch) / ch * 2;
  575         break;
  576     case AV_CODEC_ID_ADPCM_EA_R1:
  577     case AV_CODEC_ID_ADPCM_EA_R2:
  578     case AV_CODEC_ID_ADPCM_EA_R3:
  579         /* maximum number of samples */
  580         /* has internal offsets and a per-frame switch to signal raw 16-bit */
  581         has_coded_samples = 1;
  582         switch (avctx->codec->id) {
  583         case AV_CODEC_ID_ADPCM_EA_R1:
  584             header_size    = 4 + 9 * ch;
  585             *coded_samples = bytestream2_get_le32(gb);
  586             break;
  587         case AV_CODEC_ID_ADPCM_EA_R2:
  588             header_size    = 4 + 5 * ch;
  589             *coded_samples = bytestream2_get_le32(gb);
  590             break;
  591         case AV_CODEC_ID_ADPCM_EA_R3:
  592             header_size    = 4 + 5 * ch;
  593             *coded_samples = bytestream2_get_be32(gb);
  594             break;
  595         }
  596         *coded_samples -= *coded_samples % 28;
  597         nb_samples      = (buf_size - header_size) * 2 / ch;
  598         nb_samples     -= nb_samples % 28;
  599         *approx_nb_samples = 1;
  600         break;
  601     case AV_CODEC_ID_ADPCM_IMA_DK3:
  602         if (avctx->block_align > 0)
  603             buf_size = FFMIN(buf_size, avctx->block_align);
  604         nb_samples = ((buf_size - 16) * 2 / 3 * 4) / ch;
  605         break;
  606     case AV_CODEC_ID_ADPCM_IMA_DK4:
  607         if (avctx->block_align > 0)
  608             buf_size = FFMIN(buf_size, avctx->block_align);
  609         if (buf_size < 4 * ch)
  610             return AVERROR_INVALIDDATA;
  611         nb_samples = 1 + (buf_size - 4 * ch) * 2 / ch;
  612         break;
  613     case AV_CODEC_ID_ADPCM_IMA_RAD:
  614         if (avctx->block_align > 0)
  615             buf_size = FFMIN(buf_size, avctx->block_align);
  616         nb_samples = (buf_size - 4 * ch) * 2 / ch;
  617         break;
  618     case AV_CODEC_ID_ADPCM_IMA_WAV:
  619     {
  620         int bsize = ff_adpcm_ima_block_sizes[avctx->bits_per_coded_sample - 2];
  621         int bsamples = ff_adpcm_ima_block_samples[avctx->bits_per_coded_sample - 2];
  622         if (avctx->block_align > 0)
  623             buf_size = FFMIN(buf_size, avctx->block_align);
  624         if (buf_size < 4 * ch)
  625             return AVERROR_INVALIDDATA;
  626         nb_samples = 1 + (buf_size - 4 * ch) / (bsize * ch) * bsamples;
  627         break;
  628     }
  629     case AV_CODEC_ID_ADPCM_MS:
  630         if (avctx->block_align > 0)
  631             buf_size = FFMIN(buf_size, avctx->block_align);
  632         nb_samples = (buf_size - 6 * ch) * 2 / ch;
  633         break;
  634     case AV_CODEC_ID_ADPCM_MTAF:
  635         if (avctx->block_align > 0)
  636             buf_size = FFMIN(buf_size, avctx->block_align);
  637         nb_samples = (buf_size - 16 * (ch / 2)) * 2 / ch;
  638         break;
  639     case AV_CODEC_ID_ADPCM_SBPRO_2:
  640     case AV_CODEC_ID_ADPCM_SBPRO_3:
  641     case AV_CODEC_ID_ADPCM_SBPRO_4:
  642     {
  643         int samples_per_byte;
  644         switch (avctx->codec->id) {
  645         case AV_CODEC_ID_ADPCM_SBPRO_2: samples_per_byte = 4; break;
  646         case AV_CODEC_ID_ADPCM_SBPRO_3: samples_per_byte = 3; break;
  647         case AV_CODEC_ID_ADPCM_SBPRO_4: samples_per_byte = 2; break;
  648         }
  649         if (!s->status[0].step_index) {
  650             if (buf_size < ch)
  651                 return AVERROR_INVALIDDATA;
  652             nb_samples++;
  653             buf_size -= ch;
  654         }
  655         nb_samples += buf_size * samples_per_byte / ch;
  656         break;
  657     }
  658     case AV_CODEC_ID_ADPCM_SWF:
  659     {
  660         int buf_bits       = buf_size * 8 - 2;
  661         int nbits          = (bytestream2_get_byte(gb) >> 6) + 2;
  662         int block_hdr_size = 22 * ch;
  663         int block_size     = block_hdr_size + nbits * ch * 4095;
  664         int nblocks        = buf_bits / block_size;
  665         int bits_left      = buf_bits - nblocks * block_size;
  666         nb_samples         = nblocks * 4096;
  667         if (bits_left >= block_hdr_size)
  668             nb_samples += 1 + (bits_left - block_hdr_size) / (nbits * ch);
  669         break;
  670     }
  671     case AV_CODEC_ID_ADPCM_THP:
  672     case AV_CODEC_ID_ADPCM_THP_LE:
  673         if (avctx->extradata) {
  674             nb_samples = buf_size * 14 / (8 * ch);
  675             break;
  676         }
  677         has_coded_samples = 1;
  678         bytestream2_skip(gb, 4); // channel size
  679         *coded_samples  = (avctx->codec->id == AV_CODEC_ID_ADPCM_THP_LE) ?
  680                           bytestream2_get_le32(gb) :
  681                           bytestream2_get_be32(gb);
  682         buf_size       -= 8 + 36 * ch;
  683         buf_size       /= ch;
  684         nb_samples      = buf_size / 8 * 14;
  685         if (buf_size % 8 > 1)
  686             nb_samples     += (buf_size % 8 - 1) * 2;
  687         *approx_nb_samples = 1;
  688         break;
  689     case AV_CODEC_ID_ADPCM_AFC:
  690         nb_samples = buf_size / (9 * ch) * 16;
  691         break;
  692     case AV_CODEC_ID_ADPCM_XA:
  693         nb_samples = (buf_size / 128) * 224 / ch;
  694         break;
  695     case AV_CODEC_ID_ADPCM_DTK:
  696     case AV_CODEC_ID_ADPCM_PSX:
  697         nb_samples = buf_size / (16 * ch) * 28;
  698         break;
  699     }
  700 
  701     /* validate coded sample count */
  702     if (has_coded_samples && (*coded_samples <= 0 || *coded_samples > nb_samples))
  703         return AVERROR_INVALIDDATA;
  704 
  705     return nb_samples;
  706 }
  707 
  708 static int adpcm_decode_frame(AVCodecContext *avctx, void *data,
  709                               int *got_frame_ptr, AVPacket *avpkt)
  710 {
  711     AVFrame *frame     = data;
  712     const uint8_t *buf = avpkt->data;
  713     int buf_size = avpkt->size;
  714     ADPCMDecodeContext *c = avctx->priv_data;
  715     ADPCMChannelStatus *cs;
  716     int n, m, channel, i;
  717     int16_t *samples;
  718     int16_t **samples_p;
  719     int st; /* stereo */
  720     int count1, count2;
  721     int nb_samples, coded_samples, approx_nb_samples, ret;
  722     GetByteContext gb;
  723 
  724     bytestream2_init(&gb, buf, buf_size);
  725     nb_samples = get_nb_samples(avctx, &gb, buf_size, &coded_samples, &approx_nb_samples);
  726     if (nb_samples <= 0) {
  727         av_log(avctx, AV_LOG_ERROR, "invalid number of samples in packet\n");
  728         return AVERROR_INVALIDDATA;
  729     }
  730 
  731     /* get output buffer */
  732     frame->nb_samples = nb_samples;
  733     if ((ret = ff_get_buffer(avctx, frame, 0)) < 0)
  734         return ret;
  735     samples = (int16_t *)frame->data[0];
  736     samples_p = (int16_t **)frame->extended_data;
  737 
  738     /* use coded_samples when applicable */
  739     /* it is always <= nb_samples, so the output buffer will be large enough */
  740     if (coded_samples) {
  741         if (!approx_nb_samples && coded_samples != nb_samples)
  742             av_log(avctx, AV_LOG_WARNING, "mismatch in coded sample count\n");
  743         frame->nb_samples = nb_samples = coded_samples;
  744     }
  745 
  746     st = avctx->channels == 2 ? 1 : 0;
  747 
  748     switch(avctx->codec->id) {
  749     case AV_CODEC_ID_ADPCM_IMA_QT:
  750         /* In QuickTime, IMA is encoded by chunks of 34 bytes (=64 samples).
  751            Channel data is interleaved per-chunk. */
  752         for (channel = 0; channel < avctx->channels; channel++) {
  753             int predictor;
  754             int step_index;
  755             cs = &(c->status[channel]);
  756             /* (pppppp) (piiiiiii) */
  757 
  758             /* Bits 15-7 are the _top_ 9 bits of the 16-bit initial predictor value */
  759             predictor = sign_extend(bytestream2_get_be16u(&gb), 16);
  760             step_index = predictor & 0x7F;
  761             predictor &= ~0x7F;
  762 
  763             if (cs->step_index == step_index) {
  764                 int diff = predictor - cs->predictor;
  765                 if (diff < 0)
  766                     diff = - diff;
  767                 if (diff > 0x7f)
  768                     goto update;
  769             } else {
  770             update:
  771                 cs->step_index = step_index;
  772                 cs->predictor = predictor;
  773             }
  774 
  775             if (cs->step_index > 88u){
  776                 av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
  777                        channel, cs->step_index);
  778                 return AVERROR_INVALIDDATA;
  779             }
  780 
  781             samples = samples_p[channel];
  782 
  783             for (m = 0; m < 64; m += 2) {
  784                 int byte = bytestream2_get_byteu(&gb);
  785                 samples[m    ] = adpcm_ima_qt_expand_nibble(cs, byte & 0x0F, 3);
  786                 samples[m + 1] = adpcm_ima_qt_expand_nibble(cs, byte >> 4  , 3);
  787             }
  788         }
  789         break;
  790     case AV_CODEC_ID_ADPCM_IMA_WAV:
  791         for(i=0; i<avctx->channels; i++){
  792             cs = &(c->status[i]);
  793             cs->predictor = samples_p[i][0] = sign_extend(bytestream2_get_le16u(&gb), 16);
  794 
  795             cs->step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
  796             if (cs->step_index > 88u){
  797                 av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
  798                        i, cs->step_index);
  799                 return AVERROR_INVALIDDATA;
  800             }
  801         }
  802 
  803         if (avctx->bits_per_coded_sample != 4) {
  804             int samples_per_block = ff_adpcm_ima_block_samples[avctx->bits_per_coded_sample - 2];
  805             int block_size = ff_adpcm_ima_block_sizes[avctx->bits_per_coded_sample - 2];
  806             uint8_t temp[20 + AV_INPUT_BUFFER_PADDING_SIZE] = { 0 };
  807             GetBitContext g;
  808 
  809             for (n = 0; n < (nb_samples - 1) / samples_per_block; n++) {
  810                 for (i = 0; i < avctx->channels; i++) {
  811                     int j;
  812 
  813                     cs = &c->status[i];
  814                     samples = &samples_p[i][1 + n * samples_per_block];
  815                     for (j = 0; j < block_size; j++) {
  816                         temp[j] = buf[4 * avctx->channels + block_size * n * avctx->channels +
  817                                         (j % 4) + (j / 4) * (avctx->channels * 4) + i * 4];
  818                     }
  819                     ret = init_get_bits8(&g, (const uint8_t *)&temp, block_size);
  820                     if (ret < 0)
  821                         return ret;
  822                     for (m = 0; m < samples_per_block; m++) {
  823                         samples[m] = adpcm_ima_wav_expand_nibble(cs, &g,
  824                                           avctx->bits_per_coded_sample);
  825                     }
  826                 }
  827             }
  828             bytestream2_skip(&gb, avctx->block_align - avctx->channels * 4);
  829         } else {
  830         for (n = 0; n < (nb_samples - 1) / 8; n++) {
  831             for (i = 0; i < avctx->channels; i++) {
  832                 cs = &c->status[i];
  833                 samples = &samples_p[i][1 + n * 8];
  834                 for (m = 0; m < 8; m += 2) {
  835                     int v = bytestream2_get_byteu(&gb);
  836                     samples[m    ] = adpcm_ima_expand_nibble(cs, v & 0x0F, 3);
  837                     samples[m + 1] = adpcm_ima_expand_nibble(cs, v >> 4  , 3);
  838                 }
  839             }
  840         }
  841         }
  842         break;
  843     case AV_CODEC_ID_ADPCM_4XM:
  844         for (i = 0; i < avctx->channels; i++)
  845             c->status[i].predictor = sign_extend(bytestream2_get_le16u(&gb), 16);
  846 
  847         for (i = 0; i < avctx->channels; i++) {
  848             c->status[i].step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
  849             if (c->status[i].step_index > 88u) {
  850                 av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
  851                        i, c->status[i].step_index);
  852                 return AVERROR_INVALIDDATA;
  853             }
  854         }
  855 
  856         for (i = 0; i < avctx->channels; i++) {
  857             samples = (int16_t *)frame->data[i];
  858             cs = &c->status[i];
  859             for (n = nb_samples >> 1; n > 0; n--) {
  860                 int v = bytestream2_get_byteu(&gb);
  861                 *samples++ = adpcm_ima_expand_nibble(cs, v & 0x0F, 4);
  862                 *samples++ = adpcm_ima_expand_nibble(cs, v >> 4  , 4);
  863             }
  864         }
  865         break;
  866     case AV_CODEC_ID_ADPCM_MS:
  867     {
  868         int block_predictor;
  869 
  870         block_predictor = bytestream2_get_byteu(&gb);
  871         if (block_predictor > 6) {
  872             av_log(avctx, AV_LOG_ERROR, "ERROR: block_predictor[0] = %d\n",
  873                    block_predictor);
  874             return AVERROR_INVALIDDATA;
  875         }
  876         c->status[0].coeff1 = ff_adpcm_AdaptCoeff1[block_predictor];
  877         c->status[0].coeff2 = ff_adpcm_AdaptCoeff2[block_predictor];
  878         if (st) {
  879             block_predictor = bytestream2_get_byteu(&gb);
  880             if (block_predictor > 6) {
  881                 av_log(avctx, AV_LOG_ERROR, "ERROR: block_predictor[1] = %d\n",
  882                        block_predictor);
  883                 return AVERROR_INVALIDDATA;
  884             }
  885             c->status[1].coeff1 = ff_adpcm_AdaptCoeff1[block_predictor];
  886             c->status[1].coeff2 = ff_adpcm_AdaptCoeff2[block_predictor];
  887         }
  888         c->status[0].idelta = sign_extend(bytestream2_get_le16u(&gb), 16);
  889         if (st){
  890             c->status[1].idelta = sign_extend(bytestream2_get_le16u(&gb), 16);
  891         }
  892 
  893         c->status[0].sample1 = sign_extend(bytestream2_get_le16u(&gb), 16);
  894         if (st) c->status[1].sample1 = sign_extend(bytestream2_get_le16u(&gb), 16);
  895         c->status[0].sample2 = sign_extend(bytestream2_get_le16u(&gb), 16);
  896         if (st) c->status[1].sample2 = sign_extend(bytestream2_get_le16u(&gb), 16);
  897 
  898         *samples++ = c->status[0].sample2;
  899         if (st) *samples++ = c->status[1].sample2;
  900         *samples++ = c->status[0].sample1;
  901         if (st) *samples++ = c->status[1].sample1;
  902         for(n = (nb_samples - 2) >> (1 - st); n > 0; n--) {
  903             int byte = bytestream2_get_byteu(&gb);
  904             *samples++ = adpcm_ms_expand_nibble(&c->status[0 ], byte >> 4  );
  905             *samples++ = adpcm_ms_expand_nibble(&c->status[st], byte & 0x0F);
  906         }
  907         break;
  908     }
  909     case AV_CODEC_ID_ADPCM_MTAF:
  910         for (channel = 0; channel < avctx->channels; channel+=2) {
  911             bytestream2_skipu(&gb, 4);
  912             c->status[channel    ].step      = bytestream2_get_le16u(&gb) & 0x1f;
  913             c->status[channel + 1].step      = bytestream2_get_le16u(&gb) & 0x1f;
  914             c->status[channel    ].predictor = sign_extend(bytestream2_get_le16u(&gb), 16);
  915             bytestream2_skipu(&gb, 2);
  916             c->status[channel + 1].predictor = sign_extend(bytestream2_get_le16u(&gb), 16);
  917             bytestream2_skipu(&gb, 2);
  918             for (n = 0; n < nb_samples; n+=2) {
  919                 int v = bytestream2_get_byteu(&gb);
  920                 samples_p[channel][n    ] = adpcm_mtaf_expand_nibble(&c->status[channel], v & 0x0F);
  921                 samples_p[channel][n + 1] = adpcm_mtaf_expand_nibble(&c->status[channel], v >> 4  );
  922             }
  923             for (n = 0; n < nb_samples; n+=2) {
  924                 int v = bytestream2_get_byteu(&gb);
  925                 samples_p[channel + 1][n    ] = adpcm_mtaf_expand_nibble(&c->status[channel + 1], v & 0x0F);
  926                 samples_p[channel + 1][n + 1] = adpcm_mtaf_expand_nibble(&c->status[channel + 1], v >> 4  );
  927             }
  928         }
  929         break;
  930     case AV_CODEC_ID_ADPCM_IMA_DK4:
  931         for (channel = 0; channel < avctx->channels; channel++) {
  932             cs = &c->status[channel];
  933             cs->predictor  = *samples++ = sign_extend(bytestream2_get_le16u(&gb), 16);
  934             cs->step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
  935             if (cs->step_index > 88u){
  936                 av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
  937                        channel, cs->step_index);
  938                 return AVERROR_INVALIDDATA;
  939             }
  940         }
  941         for (n = (nb_samples - 1) >> (1 - st); n > 0; n--) {
  942             int v = bytestream2_get_byteu(&gb);
  943             *samples++ = adpcm_ima_expand_nibble(&c->status[0 ], v >> 4  , 3);
  944             *samples++ = adpcm_ima_expand_nibble(&c->status[st], v & 0x0F, 3);
  945         }
  946         break;
  947     case AV_CODEC_ID_ADPCM_IMA_DK3:
  948     {
  949         int last_byte = 0;
  950         int nibble;
  951         int decode_top_nibble_next = 0;
  952         int diff_channel;
  953         const int16_t *samples_end = samples + avctx->channels * nb_samples;
  954 
  955         bytestream2_skipu(&gb, 10);
  956         c->status[0].predictor  = sign_extend(bytestream2_get_le16u(&gb), 16);
  957         c->status[1].predictor  = sign_extend(bytestream2_get_le16u(&gb), 16);
  958         c->status[0].step_index = bytestream2_get_byteu(&gb);
  959         c->status[1].step_index = bytestream2_get_byteu(&gb);
  960         if (c->status[0].step_index > 88u || c->status[1].step_index > 88u){
  961             av_log(avctx, AV_LOG_ERROR, "ERROR: step_index = %i/%i\n",
  962                    c->status[0].step_index, c->status[1].step_index);
  963             return AVERROR_INVALIDDATA;
  964         }
  965         /* sign extend the predictors */
  966         diff_channel = c->status[1].predictor;
  967 
  968         /* DK3 ADPCM support macro */
  969 #define DK3_GET_NEXT_NIBBLE() \
  970     if (decode_top_nibble_next) { \
  971         nibble = last_byte >> 4; \
  972         decode_top_nibble_next = 0; \
  973     } else { \
  974         last_byte = bytestream2_get_byteu(&gb); \
  975         nibble = last_byte & 0x0F; \
  976         decode_top_nibble_next = 1; \
  977     }
  978 
  979         while (samples < samples_end) {
  980 
  981             /* for this algorithm, c->status[0] is the sum channel and
  982              * c->status[1] is the diff channel */
  983 
  984             /* process the first predictor of the sum channel */
  985             DK3_GET_NEXT_NIBBLE();
  986             adpcm_ima_expand_nibble(&c->status[0], nibble, 3);
  987 
  988             /* process the diff channel predictor */
  989             DK3_GET_NEXT_NIBBLE();
  990             adpcm_ima_expand_nibble(&c->status[1], nibble, 3);
  991 
  992             /* process the first pair of stereo PCM samples */
  993             diff_channel = (diff_channel + c->status[1].predictor) / 2;
  994             *samples++ = c->status[0].predictor + c->status[1].predictor;
  995             *samples++ = c->status[0].predictor - c->status[1].predictor;
  996 
  997             /* process the second predictor of the sum channel */
  998             DK3_GET_NEXT_NIBBLE();
  999             adpcm_ima_expand_nibble(&c->status[0], nibble, 3);
 1000 
 1001             /* process the second pair of stereo PCM samples */
 1002             diff_channel = (diff_channel + c->status[1].predictor) / 2;
 1003             *samples++ = c->status[0].predictor + c->status[1].predictor;
 1004             *samples++ = c->status[0].predictor - c->status[1].predictor;
 1005         }
 1006 
 1007         if ((bytestream2_tell(&gb) & 1))
 1008             bytestream2_skip(&gb, 1);
 1009         break;
 1010     }
 1011     case AV_CODEC_ID_ADPCM_IMA_ISS:
 1012         for (channel = 0; channel < avctx->channels; channel++) {
 1013             cs = &c->status[channel];
 1014             cs->predictor  = sign_extend(bytestream2_get_le16u(&gb), 16);
 1015             cs->step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
 1016             if (cs->step_index > 88u){
 1017                 av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
 1018                        channel, cs->step_index);
 1019                 return AVERROR_INVALIDDATA;
 1020             }
 1021         }
 1022 
 1023         for (n = nb_samples >> (1 - st); n > 0; n--) {
 1024             int v1, v2;
 1025             int v = bytestream2_get_byteu(&gb);
 1026             /* nibbles are swapped for mono */
 1027             if (st) {
 1028                 v1 = v >> 4;
 1029                 v2 = v & 0x0F;
 1030             } else {
 1031                 v2 = v >> 4;
 1032                 v1 = v & 0x0F;
 1033             }
 1034             *samples++ = adpcm_ima_expand_nibble(&c->status[0 ], v1, 3);
 1035             *samples++ = adpcm_ima_expand_nibble(&c->status[st], v2, 3);
 1036         }
 1037         break;
 1038     case AV_CODEC_ID_ADPCM_IMA_DAT4:
 1039         for (channel = 0; channel < avctx->channels; channel++) {
 1040             cs = &c->status[channel];
 1041             samples = samples_p[channel];
 1042             bytestream2_skip(&gb, 4);
 1043             for (n = 0; n < nb_samples; n += 2) {
 1044                 int v = bytestream2_get_byteu(&gb);
 1045                 *samples++ = adpcm_ima_expand_nibble(cs, v >> 4  , 3);
 1046                 *samples++ = adpcm_ima_expand_nibble(cs, v & 0x0F, 3);
 1047             }
 1048         }
 1049         break;
 1050     case AV_CODEC_ID_ADPCM_IMA_APC:
 1051         while (bytestream2_get_bytes_left(&gb) > 0) {
 1052             int v = bytestream2_get_byteu(&gb);
 1053             *samples++ = adpcm_ima_expand_nibble(&c->status[0],  v >> 4  , 3);
 1054             *samples++ = adpcm_ima_expand_nibble(&c->status[st], v & 0x0F, 3);
 1055         }
 1056         break;
 1057     case AV_CODEC_ID_ADPCM_IMA_OKI:
 1058         while (bytestream2_get_bytes_left(&gb) > 0) {
 1059             int v = bytestream2_get_byteu(&gb);
 1060             *samples++ = adpcm_ima_oki_expand_nibble(&c->status[0],  v >> 4  );
 1061             *samples++ = adpcm_ima_oki_expand_nibble(&c->status[st], v & 0x0F);
 1062         }
 1063         break;
 1064     case AV_CODEC_ID_ADPCM_IMA_RAD:
 1065         for (channel = 0; channel < avctx->channels; channel++) {
 1066             cs = &c->status[channel];
 1067             cs->step_index = sign_extend(bytestream2_get_le16u(&gb), 16);
 1068             cs->predictor  = sign_extend(bytestream2_get_le16u(&gb), 16);
 1069             if (cs->step_index > 88u){
 1070                 av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
 1071                        channel, cs->step_index);
 1072                 return AVERROR_INVALIDDATA;
 1073             }
 1074         }
 1075         for (n = 0; n < nb_samples / 2; n++) {
 1076             int byte[2];
 1077 
 1078             byte[0] = bytestream2_get_byteu(&gb);
 1079             if (st)
 1080                 byte[1] = bytestream2_get_byteu(&gb);
 1081             for(channel = 0; channel < avctx->channels; channel++) {
 1082                 *samples++ = adpcm_ima_expand_nibble(&c->status[channel], byte[channel] & 0x0F, 3);
 1083             }
 1084             for(channel = 0; channel < avctx->channels; channel++) {
 1085                 *samples++ = adpcm_ima_expand_nibble(&c->status[channel], byte[channel] >> 4  , 3);
 1086             }
 1087         }
 1088         break;
 1089     case AV_CODEC_ID_ADPCM_IMA_WS:
 1090         if (c->vqa_version == 3) {
 1091             for (channel = 0; channel < avctx->channels; channel++) {
 1092                 int16_t *smp = samples_p[channel];
 1093 
 1094                 for (n = nb_samples / 2; n > 0; n--) {
 1095                     int v = bytestream2_get_byteu(&gb);
 1096                     *smp++ = adpcm_ima_expand_nibble(&c->status[channel], v >> 4  , 3);
 1097                     *smp++ = adpcm_ima_expand_nibble(&c->status[channel], v & 0x0F, 3);
 1098                 }
 1099             }
 1100         } else {
 1101             for (n = nb_samples / 2; n > 0; n--) {
 1102                 for (channel = 0; channel < avctx->channels; channel++) {
 1103                     int v = bytestream2_get_byteu(&gb);
 1104                     *samples++  = adpcm_ima_expand_nibble(&c->status[channel], v >> 4  , 3);
 1105                     samples[st] = adpcm_ima_expand_nibble(&c->status[channel], v & 0x0F, 3);
 1106                 }
 1107                 samples += avctx->channels;
 1108             }
 1109         }
 1110         bytestream2_seek(&gb, 0, SEEK_END);
 1111         break;
 1112     case AV_CODEC_ID_ADPCM_XA:
 1113     {
 1114         int16_t *out0 = samples_p[0];
 1115         int16_t *out1 = samples_p[1];
 1116         int samples_per_block = 28 * (3 - avctx->channels) * 4;
 1117         int sample_offset = 0;
 1118         while (bytestream2_get_bytes_left(&gb) >= 128) {
 1119             if ((ret = xa_decode(avctx, out0, out1, buf + bytestream2_tell(&gb),
 1120                                  &c->status[0], &c->status[1],
 1121                                  avctx->channels, sample_offset)) < 0)
 1122                 return ret;
 1123             bytestream2_skipu(&gb, 128);
 1124             sample_offset += samples_per_block;
 1125         }
 1126         break;
 1127     }
 1128     case AV_CODEC_ID_ADPCM_IMA_EA_EACS:
 1129         for (i=0; i<=st; i++) {
 1130             c->status[i].step_index = bytestream2_get_le32u(&gb);
 1131             if (c->status[i].step_index > 88u) {
 1132                 av_log(avctx, AV_LOG_ERROR, "ERROR: step_index[%d] = %i\n",
 1133                        i, c->status[i].step_index);
 1134                 return AVERROR_INVALIDDATA;
 1135             }
 1136         }
 1137         for (i=0; i<=st; i++)
 1138             c->status[i].predictor  = bytestream2_get_le32u(&gb);
 1139 
 1140         for (n = nb_samples >> (1 - st); n > 0; n--) {
 1141             int byte   = bytestream2_get_byteu(&gb);
 1142             *samples++ = adpcm_ima_expand_nibble(&c->status[0],  byte >> 4,   3);
 1143             *samples++ = adpcm_ima_expand_nibble(&c->status[st], byte & 0x0F, 3);
 1144         }
 1145         break;
 1146     case AV_CODEC_ID_ADPCM_IMA_EA_SEAD:
 1147         for (n = nb_samples >> (1 - st); n > 0; n--) {
 1148             int byte = bytestream2_get_byteu(&gb);
 1149             *samples++ = adpcm_ima_expand_nibble(&c->status[0],  byte >> 4,   6);
 1150             *samples++ = adpcm_ima_expand_nibble(&c->status[st], byte & 0x0F, 6);
 1151         }
 1152         break;
 1153     case AV_CODEC_ID_ADPCM_EA:
 1154     {
 1155         int previous_left_sample, previous_right_sample;
 1156         int current_left_sample, current_right_sample;
 1157         int next_left_sample, next_right_sample;
 1158         int coeff1l, coeff2l, coeff1r, coeff2r;
 1159         int shift_left, shift_right;
 1160 
 1161         /* Each EA ADPCM frame has a 12-byte header followed by 30-byte pieces,
 1162            each coding 28 stereo samples. */
 1163 
 1164         if(avctx->channels != 2)
 1165             return AVERROR_INVALIDDATA;
 1166 
 1167         current_left_sample   = sign_extend(bytestream2_get_le16u(&gb), 16);
 1168         previous_left_sample  = sign_extend(bytestream2_get_le16u(&gb), 16);
 1169         current_right_sample  = sign_extend(bytestream2_get_le16u(&gb), 16);
 1170         previous_right_sample = sign_extend(bytestream2_get_le16u(&gb), 16);
 1171 
 1172         for (count1 = 0; count1 < nb_samples / 28; count1++) {
 1173             int byte = bytestream2_get_byteu(&gb);
 1174             coeff1l = ea_adpcm_table[ byte >> 4       ];
 1175             coeff2l = ea_adpcm_table[(byte >> 4  ) + 4];
 1176             coeff1r = ea_adpcm_table[ byte & 0x0F];
 1177             coeff2r = ea_adpcm_table[(byte & 0x0F) + 4];
 1178 
 1179             byte = bytestream2_get_byteu(&gb);
 1180             shift_left  = 20 - (byte >> 4);
 1181             shift_right = 20 - (byte & 0x0F);
 1182 
 1183             for (count2 = 0; count2 < 28; count2++) {
 1184                 byte = bytestream2_get_byteu(&gb);
 1185                 next_left_sample  = sign_extend(byte >> 4, 4) << shift_left;
 1186                 next_right_sample = sign_extend(byte,      4) << shift_right;
 1187 
 1188                 next_left_sample = (next_left_sample +
 1189                     (current_left_sample * coeff1l) +
 1190                     (previous_left_sample * coeff2l) + 0x80) >> 8;
 1191                 next_right_sample = (next_right_sample +
 1192                     (current_right_sample * coeff1r) +
 1193                     (previous_right_sample * coeff2r) + 0x80) >> 8;
 1194 
 1195                 previous_left_sample = current_left_sample;
 1196                 current_left_sample = av_clip_int16(next_left_sample);
 1197                 previous_right_sample = current_right_sample;
 1198                 current_right_sample = av_clip_int16(next_right_sample);
 1199                 *samples++ = current_left_sample;
 1200                 *samples++ = current_right_sample;
 1201             }
 1202         }
 1203 
 1204         bytestream2_skip(&gb, 2); // Skip terminating 0x0000
 1205 
 1206         break;
 1207     }
 1208     case AV_CODEC_ID_ADPCM_EA_MAXIS_XA:
 1209     {
 1210         int coeff[2][2], shift[2];
 1211 
 1212         for(channel = 0; channel < avctx->channels; channel++) {
 1213             int byte = bytestream2_get_byteu(&gb);
 1214             for (i=0; i<2; i++)
 1215                 coeff[channel][i] = ea_adpcm_table[(byte >> 4) + 4*i];
 1216             shift[channel] = 20 - (byte & 0x0F);
 1217         }
 1218         for (count1 = 0; count1 < nb_samples / 2; count1++) {
 1219             int byte[2];
 1220 
 1221             byte[0] = bytestream2_get_byteu(&gb);
 1222             if (st) byte[1] = bytestream2_get_byteu(&gb);
 1223             for(i = 4; i >= 0; i-=4) { /* Pairwise samples LL RR (st) or LL LL (mono) */
 1224                 for(channel = 0; channel < avctx->channels; channel++) {
 1225                     int sample = sign_extend(byte[channel] >> i, 4) << shift[channel];
 1226                     sample = (sample +
 1227                              c->status[channel].sample1 * coeff[channel][0] +
 1228                              c->status[channel].sample2 * coeff[channel][1] + 0x80) >> 8;
 1229                     c->status[channel].sample2 = c->status[channel].sample1;
 1230                     c->status[channel].sample1 = av_clip_int16(sample);
 1231                     *samples++ = c->status[channel].sample1;
 1232                 }
 1233             }
 1234         }
 1235         bytestream2_seek(&gb, 0, SEEK_END);
 1236         break;
 1237     }
 1238     case AV_CODEC_ID_ADPCM_EA_R1:
 1239     case AV_CODEC_ID_ADPCM_EA_R2:
 1240     case AV_CODEC_ID_ADPCM_EA_R3: {
 1241         /* channel numbering
 1242            2chan: 0=fl, 1=fr
 1243            4chan: 0=fl, 1=rl, 2=fr, 3=rr
 1244            6chan: 0=fl, 1=c,  2=fr, 3=rl,  4=rr, 5=sub */
 1245         const int big_endian = avctx->codec->id == AV_CODEC_ID_ADPCM_EA_R3;
 1246         int previous_sample, current_sample, next_sample;
 1247         int coeff1, coeff2;
 1248         int shift;
 1249         unsigned int channel;
 1250         uint16_t *samplesC;
 1251         int count = 0;
 1252         int offsets[6];
 1253 
 1254         for (channel=0; channel<avctx->channels; channel++)
 1255             offsets[channel] = (big_endian ? bytestream2_get_be32(&gb) :
 1256                                              bytestream2_get_le32(&gb)) +
 1257                                (avctx->channels + 1) * 4;
 1258 
 1259         for (channel=0; channel<avctx->channels; channel++) {
 1260             bytestream2_seek(&gb, offsets[channel], SEEK_SET);
 1261             samplesC = samples_p[channel];
 1262 
 1263             if (avctx->codec->id == AV_CODEC_ID_ADPCM_EA_R1) {
 1264                 current_sample  = sign_extend(bytestream2_get_le16(&gb), 16);
 1265                 previous_sample = sign_extend(bytestream2_get_le16(&gb), 16);
 1266             } else {
 1267                 current_sample  = c->status[channel].predictor;
 1268                 previous_sample = c->status[channel].prev_sample;
 1269             }
 1270 
 1271             for (count1 = 0; count1 < nb_samples / 28; count1++) {
 1272                 int byte = bytestream2_get_byte(&gb);
 1273                 if (byte == 0xEE) {  /* only seen in R2 and R3 */
 1274                     current_sample  = sign_extend(bytestream2_get_be16(&gb), 16);
 1275                     previous_sample = sign_extend(bytestream2_get_be16(&gb), 16);
 1276 
 1277                     for (count2=0; count2<28; count2++)
 1278                         *samplesC++ = sign_extend(bytestream2_get_be16(&gb), 16);
 1279                 } else {
 1280                     coeff1 = ea_adpcm_table[ byte >> 4     ];
 1281                     coeff2 = ea_adpcm_table[(byte >> 4) + 4];
 1282                     shift = 20 - (byte & 0x0F);
 1283 
 1284                     for (count2=0; count2<28; count2++) {
 1285                         if (count2 & 1)
 1286                             next_sample = sign_extend(byte,    4) << shift;
 1287                         else {
 1288                             byte = bytestream2_get_byte(&gb);
 1289                             next_sample = sign_extend(byte >> 4, 4) << shift;
 1290                         }
 1291 
 1292                         next_sample += (current_sample  * coeff1) +
 1293                                        (previous_sample * coeff2);
 1294                         next_sample = av_clip_int16(next_sample >> 8);
 1295 
 1296                         previous_sample = current_sample;
 1297                         current_sample  = next_sample;
 1298                         *samplesC++ = current_sample;
 1299                     }
 1300                 }
 1301             }
 1302             if (!count) {
 1303                 count = count1;
 1304             } else if (count != count1) {
 1305                 av_log(avctx, AV_LOG_WARNING, "per-channel sample count mismatch\n");
 1306                 count = FFMAX(count, count1);
 1307             }
 1308 
 1309             if (avctx->codec->id != AV_CODEC_ID_ADPCM_EA_R1) {
 1310                 c->status[channel].predictor   = current_sample;
 1311                 c->status[channel].prev_sample = previous_sample;
 1312             }
 1313         }
 1314 
 1315         frame->nb_samples = count * 28;
 1316         bytestream2_seek(&gb, 0, SEEK_END);
 1317         break;
 1318     }
 1319     case AV_CODEC_ID_ADPCM_EA_XAS:
 1320         for (channel=0; channel<avctx->channels; channel++) {
 1321             int coeff[2][4], shift[4];
 1322             int16_t *s = samples_p[channel];
 1323             for (n = 0; n < 4; n++, s += 32) {
 1324                 int val = sign_extend(bytestream2_get_le16u(&gb), 16);
 1325                 for (i=0; i<2; i++)
 1326                     coeff[i][n] = ea_adpcm_table[(val&0x0F)+4*i];
 1327                 s[0] = val & ~0x0F;
 1328 
 1329                 val = sign_extend(bytestream2_get_le16u(&gb), 16);
 1330                 shift[n] = 20 - (val & 0x0F);
 1331                 s[1] = val & ~0x0F;
 1332             }
 1333 
 1334             for (m=2; m<32; m+=2) {
 1335                 s = &samples_p[channel][m];
 1336                 for (n = 0; n < 4; n++, s += 32) {
 1337                     int level, pred;
 1338                     int byte = bytestream2_get_byteu(&gb);
 1339 
 1340                     level = sign_extend(byte >> 4, 4) << shift[n];
 1341                     pred  = s[-1] * coeff[0][n] + s[-2] * coeff[1][n];
 1342                     s[0]  = av_clip_int16((level + pred + 0x80) >> 8);
 1343 
 1344                     level = sign_extend(byte, 4) << shift[n];
 1345                     pred  = s[0] * coeff[0][n] + s[-1] * coeff[1][n];
 1346                     s[1]  = av_clip_int16((level + pred + 0x80) >> 8);
 1347                 }
 1348             }
 1349         }
 1350         break;
 1351     case AV_CODEC_ID_ADPCM_IMA_AMV:
 1352         c->status[0].predictor = sign_extend(bytestream2_get_le16u(&gb), 16);
 1353         c->status[0].step_index = bytestream2_get_byteu(&gb);
 1354         bytestream2_skipu(&gb, 5);
 1355         if (c->status[0].step_index > 88u) {
 1356             av_log(avctx, AV_LOG_ERROR, "ERROR: step_index = %i\n",
 1357                    c->status[0].step_index);
 1358             return AVERROR_INVALIDDATA;
 1359         }
 1360 
 1361         for (n = nb_samples >> (1 - st); n > 0; n--) {
 1362             int v = bytestream2_get_byteu(&gb);
 1363 
 1364             *samples++ = adpcm_ima_expand_nibble(&c->status[0], v >> 4, 3);
 1365             *samples++ = adpcm_ima_expand_nibble(&c->status[0], v & 0xf, 3);
 1366         }
 1367         break;
 1368     case AV_CODEC_ID_ADPCM_IMA_SMJPEG:
 1369         for (i = 0; i < avctx->channels; i++) {
 1370             c->status[i].predictor = sign_extend(bytestream2_get_be16u(&gb), 16);
 1371             c->status[i].step_index = bytestream2_get_byteu(&gb);
 1372             bytestream2_skipu(&gb, 1);
 1373             if (c->status[i].step_index > 88u) {
 1374                 av_log(avctx, AV_LOG_ERROR, "ERROR: step_index = %i\n",
 1375                        c->status[i].step_index);
 1376                 return AVERROR_INVALIDDATA;
 1377             }
 1378         }
 1379 
 1380         for (n = nb_samples >> (1 - st); n > 0; n--) {
 1381             int v = bytestream2_get_byteu(&gb);
 1382 
 1383             *samples++ = adpcm_ima_qt_expand_nibble(&c->status[0 ], v >> 4, 3);
 1384             *samples++ = adpcm_ima_qt_expand_nibble(&c->status[st], v & 0xf, 3);
 1385         }
 1386         break;
 1387     case AV_CODEC_ID_ADPCM_CT:
 1388         for (n = nb_samples >> (1 - st); n > 0; n--) {
 1389             int v = bytestream2_get_byteu(&gb);
 1390             *samples++ = adpcm_ct_expand_nibble(&c->status[0 ], v >> 4  );
 1391             *samples++ = adpcm_ct_expand_nibble(&c->status[st], v & 0x0F);
 1392         }
 1393         break;
 1394     case AV_CODEC_ID_ADPCM_SBPRO_4:
 1395     case AV_CODEC_ID_ADPCM_SBPRO_3:
 1396     case AV_CODEC_ID_ADPCM_SBPRO_2:
 1397         if (!c->status[0].step_index) {
 1398             /* the first byte is a raw sample */
 1399             *samples++ = 128 * (bytestream2_get_byteu(&gb) - 0x80);
 1400             if (st)
 1401                 *samples++ = 128 * (bytestream2_get_byteu(&gb) - 0x80);
 1402             c->status[0].step_index = 1;
 1403             nb_samples--;
 1404         }
 1405         if (avctx->codec->id == AV_CODEC_ID_ADPCM_SBPRO_4) {
 1406             for (n = nb_samples >> (1 - st); n > 0; n--) {
 1407                 int byte = bytestream2_get_byteu(&gb);
 1408                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
 1409                                                        byte >> 4,   4, 0);
 1410                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[st],
 1411                                                        byte & 0x0F, 4, 0);
 1412             }
 1413         } else if (avctx->codec->id == AV_CODEC_ID_ADPCM_SBPRO_3) {
 1414             for (n = (nb_samples<<st) / 3; n > 0; n--) {
 1415                 int byte = bytestream2_get_byteu(&gb);
 1416                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
 1417                                                         byte >> 5        , 3, 0);
 1418                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
 1419                                                        (byte >> 2) & 0x07, 3, 0);
 1420                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
 1421                                                         byte & 0x03,       2, 0);
 1422             }
 1423         } else {
 1424             for (n = nb_samples >> (2 - st); n > 0; n--) {
 1425                 int byte = bytestream2_get_byteu(&gb);
 1426                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
 1427                                                         byte >> 6        , 2, 2);
 1428                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[st],
 1429                                                        (byte >> 4) & 0x03, 2, 2);
 1430                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[0],
 1431                                                        (byte >> 2) & 0x03, 2, 2);
 1432                 *samples++ = adpcm_sbpro_expand_nibble(&c->status[st],
 1433                                                         byte & 0x03,       2, 2);
 1434             }
 1435         }
 1436         break;
 1437     case AV_CODEC_ID_ADPCM_SWF:
 1438         adpcm_swf_decode(avctx, buf, buf_size, samples);
 1439         bytestream2_seek(&gb, 0, SEEK_END);
 1440         break;
 1441     case AV_CODEC_ID_ADPCM_YAMAHA:
 1442         for (n = nb_samples >> (1 - st); n > 0; n--) {
 1443             int v = bytestream2_get_byteu(&gb);
 1444             *samples++ = adpcm_yamaha_expand_nibble(&c->status[0 ], v & 0x0F);
 1445             *samples++ = adpcm_yamaha_expand_nibble(&c->status[st], v >> 4  );
 1446         }
 1447         break;
 1448     case AV_CODEC_ID_ADPCM_AICA:
 1449         if (!c->has_status) {
 1450             for (channel = 0; channel < avctx->channels; channel++)
 1451                 c->status[channel].step = 0;
 1452             c->has_status = 1;
 1453         }
 1454         for (channel = 0; channel < avctx->channels; channel++) {
 1455             samples = samples_p[channel];
 1456             for (n = nb_samples >> 1; n > 0; n--) {
 1457                 int v = bytestream2_get_byteu(&gb);
 1458                 *samples++ = adpcm_yamaha_expand_nibble(&c->status[channel], v & 0x0F);
 1459                 *samples++ = adpcm_yamaha_expand_nibble(&c->status[channel], v >> 4  );
 1460             }
 1461         }
 1462         break;
 1463     case AV_CODEC_ID_ADPCM_AFC:
 1464     {
 1465         int samples_per_block;
 1466         int blocks;
 1467 
 1468         if (avctx->extradata && avctx->extradata_size == 1 && avctx->extradata[0]) {
 1469             samples_per_block = avctx->extradata[0] / 16;
 1470             blocks = nb_samples / avctx->extradata[0];
 1471         } else {
 1472             samples_per_block = nb_samples / 16;
 1473             blocks = 1;
 1474         }
 1475 
 1476         for (m = 0; m < blocks; m++) {
 1477         for (channel = 0; channel < avctx->channels; channel++) {
 1478             int prev1 = c->status[channel].sample1;
 1479             int prev2 = c->status[channel].sample2;
 1480 
 1481             samples = samples_p[channel] + m * 16;
 1482             /* Read in every sample for this channel.  */
 1483             for (i = 0; i < samples_per_block; i++) {
 1484                 int byte = bytestream2_get_byteu(&gb);
 1485                 int scale = 1 << (byte >> 4);
 1486                 int index = byte & 0xf;
 1487                 int factor1 = ff_adpcm_afc_coeffs[0][index];
 1488                 int factor2 = ff_adpcm_afc_coeffs[1][index];
 1489 
 1490                 /* Decode 16 samples.  */
 1491                 for (n = 0; n < 16; n++) {
 1492                     int32_t sampledat;
 1493 
 1494                     if (n & 1) {
 1495                         sampledat = sign_extend(byte, 4);
 1496                     } else {
 1497                         byte = bytestream2_get_byteu(&gb);
 1498                         sampledat = sign_extend(byte >> 4, 4);
 1499                     }
 1500 
 1501                     sampledat = ((prev1 * factor1 + prev2 * factor2) +
 1502                                  ((sampledat * scale) << 11)) >> 11;
 1503                     *samples = av_clip_int16(sampledat);
 1504                     prev2 = prev1;
 1505                     prev1 = *samples++;
 1506                 }
 1507             }
 1508 
 1509             c->status[channel].sample1 = prev1;
 1510             c->status[channel].sample2 = prev2;
 1511         }
 1512         }
 1513         bytestream2_seek(&gb, 0, SEEK_END);
 1514         break;
 1515     }
 1516     case AV_CODEC_ID_ADPCM_THP:
 1517     case AV_CODEC_ID_ADPCM_THP_LE:
 1518     {
 1519         int table[14][16];
 1520         int ch;
 1521 
 1522 #define THP_GET16(g) \
 1523     sign_extend( \
 1524         avctx->codec->id == AV_CODEC_ID_ADPCM_THP_LE ? \
 1525         bytestream2_get_le16u(&(g)) : \
 1526         bytestream2_get_be16u(&(g)), 16)
 1527 
 1528         if (avctx->extradata) {
 1529             GetByteContext tb;
 1530             if (avctx->extradata_size < 32 * avctx->channels) {
 1531                 av_log(avctx, AV_LOG_ERROR, "Missing coeff table\n");
 1532                 return AVERROR_INVALIDDATA;
 1533             }
 1534 
 1535             bytestream2_init(&tb, avctx->extradata, avctx->extradata_size);
 1536             for (i = 0; i < avctx->channels; i++)
 1537                 for (n = 0; n < 16; n++)
 1538                     table[i][n] = THP_GET16(tb);
 1539         } else {
 1540             for (i = 0; i < avctx->channels; i++)
 1541                 for (n = 0; n < 16; n++)
 1542                     table[i][n] = THP_GET16(gb);
 1543 
 1544             if (!c->has_status) {
 1545                 /* Initialize the previous sample.  */
 1546                 for (i = 0; i < avctx->channels; i++) {
 1547                     c->status[i].sample1 = THP_GET16(gb);
 1548                     c->status[i].sample2 = THP_GET16(gb);
 1549                 }
 1550                 c->has_status = 1;
 1551             } else {
 1552                 bytestream2_skip(&gb, avctx->channels * 4);
 1553             }
 1554         }
 1555 
 1556         for (ch = 0; ch < avctx->channels; ch++) {
 1557             samples = samples_p[ch];
 1558 
 1559             /* Read in every sample for this channel.  */
 1560             for (i = 0; i < (nb_samples + 13) / 14; i++) {
 1561                 int byte = bytestream2_get_byteu(&gb);
 1562                 int index = (byte >> 4) & 7;
 1563                 unsigned int exp = byte & 0x0F;
 1564                 int factor1 = table[ch][index * 2];
 1565                 int factor2 = table[ch][index * 2 + 1];
 1566 
 1567                 /* Decode 14 samples.  */
 1568                 for (n = 0; n < 14 && (i * 14 + n < nb_samples); n++) {
 1569                     int32_t sampledat;
 1570 
 1571                     if (n & 1) {
 1572                         sampledat = sign_extend(byte, 4);
 1573                     } else {
 1574                         byte = bytestream2_get_byteu(&gb);
 1575                         sampledat = sign_extend(byte >> 4, 4);
 1576                     }
 1577 
 1578                     sampledat = ((c->status[ch].sample1 * factor1
 1579                                 + c->status[ch].sample2 * factor2) >> 11) + (sampledat << exp);
 1580                     *samples = av_clip_int16(sampledat);
 1581                     c->status[ch].sample2 = c->status[ch].sample1;
 1582                     c->status[ch].sample1 = *samples++;
 1583                 }
 1584             }
 1585         }
 1586         break;
 1587     }
 1588     case AV_CODEC_ID_ADPCM_DTK:
 1589         for (channel = 0; channel < avctx->channels; channel++) {
 1590             samples = samples_p[channel];
 1591 
 1592             /* Read in every sample for this channel.  */
 1593             for (i = 0; i < nb_samples / 28; i++) {
 1594                 int byte, header;
 1595                 if (channel)
 1596                     bytestream2_skipu(&gb, 1);
 1597                 header = bytestream2_get_byteu(&gb);
 1598                 bytestream2_skipu(&gb, 3 - channel);
 1599 
 1600                 /* Decode 28 samples.  */
 1601                 for (n = 0; n < 28; n++) {
 1602                     int32_t sampledat, prev;
 1603 
 1604                     switch (header >> 4) {
 1605                     case 1:
 1606                         prev = (c->status[channel].sample1 * 0x3c);
 1607                         break;
 1608                     case 2:
 1609                         prev = (c->status[channel].sample1 * 0x73) - (c->status[channel].sample2 * 0x34);
 1610                         break;
 1611                     case 3:
 1612                         prev = (c->status[channel].sample1 * 0x62) - (c->status[channel].sample2 * 0x37);
 1613                         break;
 1614                     default:
 1615                         prev = 0;
 1616                     }
 1617 
 1618                     prev = av_clip_intp2((prev + 0x20) >> 6, 21);
 1619 
 1620                     byte = bytestream2_get_byteu(&gb);
 1621                     if (!channel)
 1622                         sampledat = sign_extend(byte, 4);
 1623                     else
 1624                         sampledat = sign_extend(byte >> 4, 4);
 1625 
 1626                     sampledat = (((sampledat << 12) >> (header & 0xf)) << 6) + prev;
 1627                     *samples++ = av_clip_int16(sampledat >> 6);
 1628                     c->status[channel].sample2 = c->status[channel].sample1;
 1629                     c->status[channel].sample1 = sampledat;
 1630                 }
 1631             }
 1632             if (!channel)
 1633                 bytestream2_seek(&gb, 0, SEEK_SET);
 1634         }
 1635         break;
 1636     case AV_CODEC_ID_ADPCM_PSX:
 1637         for (channel = 0; channel < avctx->channels; channel++) {
 1638             samples = samples_p[channel];
 1639 
 1640             /* Read in every sample for this channel.  */
 1641             for (i = 0; i < nb_samples / 28; i++) {
 1642                 int filter, shift, flag, byte;
 1643 
 1644                 filter = bytestream2_get_byteu(&gb);
 1645                 shift  = filter & 0xf;
 1646                 filter = filter >> 4;
 1647                 if (filter >= FF_ARRAY_ELEMS(xa_adpcm_table))
 1648                     return AVERROR_INVALIDDATA;
 1649                 flag   = bytestream2_get_byteu(&gb);
 1650 
 1651                 /* Decode 28 samples.  */
 1652                 for (n = 0; n < 28; n++) {
 1653                     int sample = 0, scale;
 1654 
 1655                     if (flag < 0x07) {
 1656                         if (n & 1) {
 1657                             scale = sign_extend(byte >> 4, 4);
 1658                         } else {
 1659                             byte  = bytestream2_get_byteu(&gb);
 1660                             scale = sign_extend(byte, 4);
 1661                         }
 1662 
 1663                         scale  = scale << 12;
 1664                         sample = (int)((scale >> shift) + (c->status[channel].sample1 * xa_adpcm_table[filter][0] + c->status[channel].sample2 * xa_adpcm_table[filter][1]) / 64);
 1665                     }
 1666                     *samples++ = av_clip_int16(sample);
 1667                     c->status[channel].sample2 = c->status[channel].sample1;
 1668                     c->status[channel].sample1 = sample;
 1669                 }
 1670             }
 1671         }
 1672         break;
 1673 
 1674     default:
 1675         return -1;
 1676     }
 1677 
 1678     if (avpkt->size && bytestream2_tell(&gb) == 0) {
 1679         av_log(avctx, AV_LOG_ERROR, "Nothing consumed\n");
 1680         return AVERROR_INVALIDDATA;
 1681     }
 1682 
 1683     *got_frame_ptr = 1;
 1684 
 1685     if (avpkt->size < bytestream2_tell(&gb)) {
 1686         av_log(avctx, AV_LOG_ERROR, "Overread of %d < %d\n", avpkt->size, bytestream2_tell(&gb));
 1687         return avpkt->size;
 1688     }
 1689 
 1690     return bytestream2_tell(&gb);
 1691 }
 1692 
 1693 static void adpcm_flush(AVCodecContext *avctx)
 1694 {
 1695     ADPCMDecodeContext *c = avctx->priv_data;
 1696     c->has_status = 0;
 1697 }
 1698 
 1699 
 1700 static const enum AVSampleFormat sample_fmts_s16[]  = { AV_SAMPLE_FMT_S16,
 1701                                                         AV_SAMPLE_FMT_NONE };
 1702 static const enum AVSampleFormat sample_fmts_s16p[] = { AV_SAMPLE_FMT_S16P,
 1703                                                         AV_SAMPLE_FMT_NONE };
 1704 static const enum AVSampleFormat sample_fmts_both[] = { AV_SAMPLE_FMT_S16,
 1705                                                         AV_SAMPLE_FMT_S16P,
 1706                                                         AV_SAMPLE_FMT_NONE };
 1707 
 1708 #define ADPCM_DECODER(id_, sample_fmts_, name_, long_name_) \
 1709 AVCodec ff_ ## name_ ## _decoder = {                        \
 1710     .name           = #name_,                               \
 1711     .long_name      = NULL_IF_CONFIG_SMALL(long_name_),     \
 1712     .type           = AVMEDIA_TYPE_AUDIO,                   \
 1713     .id             = id_,                                  \
 1714     .priv_data_size = sizeof(ADPCMDecodeContext),           \
 1715     .init           = adpcm_decode_init,                    \
 1716     .decode         = adpcm_decode_frame,                   \
 1717     .flush          = adpcm_flush,                          \
 1718     .capabilities   = AV_CODEC_CAP_DR1,                     \
 1719     .sample_fmts    = sample_fmts_,                         \
 1720 }
 1721 
 1722 /* Note: Do not forget to add new entries to the Makefile as well. */
 1723 ADPCM_DECODER(AV_CODEC_ID_ADPCM_4XM,         sample_fmts_s16p, adpcm_4xm,         "ADPCM 4X Movie");
 1724 ADPCM_DECODER(AV_CODEC_ID_ADPCM_AFC,         sample_fmts_s16p, adpcm_afc,         "ADPCM Nintendo Gamecube AFC");
 1725 ADPCM_DECODER(AV_CODEC_ID_ADPCM_AICA,        sample_fmts_s16p, adpcm_aica,        "ADPCM Yamaha AICA");
 1726 ADPCM_DECODER(AV_CODEC_ID_ADPCM_CT,          sample_fmts_s16,  adpcm_ct,          "ADPCM Creative Technology");
 1727 ADPCM_DECODER(AV_CODEC_ID_ADPCM_DTK,         sample_fmts_s16p, adpcm_dtk,         "ADPCM Nintendo Gamecube DTK");
 1728 ADPCM_DECODER(AV_CODEC_ID_ADPCM_EA,          sample_fmts_s16,  adpcm_ea,          "ADPCM Electronic Arts");
 1729 ADPCM_DECODER(AV_CODEC_ID_ADPCM_EA_MAXIS_XA, sample_fmts_s16,  adpcm_ea_maxis_xa, "ADPCM Electronic Arts Maxis CDROM XA");
 1730 ADPCM_DECODER(AV_CODEC_ID_ADPCM_EA_R1,       sample_fmts_s16p, adpcm_ea_r1,       "ADPCM Electronic Arts R1");
 1731 ADPCM_DECODER(AV_CODEC_ID_ADPCM_EA_R2,       sample_fmts_s16p, adpcm_ea_r2,       "ADPCM Electronic Arts R2");
 1732 ADPCM_DECODER(AV_CODEC_ID_ADPCM_EA_R3,       sample_fmts_s16p, adpcm_ea_r3,       "ADPCM Electronic Arts R3");
 1733 ADPCM_DECODER(AV_CODEC_ID_ADPCM_EA_XAS,      sample_fmts_s16p, adpcm_ea_xas,      "ADPCM Electronic Arts XAS");
 1734 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_AMV,     sample_fmts_s16,  adpcm_ima_amv,     "ADPCM IMA AMV");
 1735 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_APC,     sample_fmts_s16,  adpcm_ima_apc,     "ADPCM IMA CRYO APC");
 1736 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_DAT4,    sample_fmts_s16,  adpcm_ima_dat4,    "ADPCM IMA Eurocom DAT4");
 1737 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_DK3,     sample_fmts_s16,  adpcm_ima_dk3,     "ADPCM IMA Duck DK3");
 1738 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_DK4,     sample_fmts_s16,  adpcm_ima_dk4,     "ADPCM IMA Duck DK4");
 1739 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_EA_EACS, sample_fmts_s16,  adpcm_ima_ea_eacs, "ADPCM IMA Electronic Arts EACS");
 1740 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_EA_SEAD, sample_fmts_s16,  adpcm_ima_ea_sead, "ADPCM IMA Electronic Arts SEAD");
 1741 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_ISS,     sample_fmts_s16,  adpcm_ima_iss,     "ADPCM IMA Funcom ISS");
 1742 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_OKI,     sample_fmts_s16,  adpcm_ima_oki,     "ADPCM IMA Dialogic OKI");
 1743 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_QT,      sample_fmts_s16p, adpcm_ima_qt,      "ADPCM IMA QuickTime");
 1744 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_RAD,     sample_fmts_s16,  adpcm_ima_rad,     "ADPCM IMA Radical");
 1745 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_SMJPEG,  sample_fmts_s16,  adpcm_ima_smjpeg,  "ADPCM IMA Loki SDL MJPEG");
 1746 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_WAV,     sample_fmts_s16p, adpcm_ima_wav,     "ADPCM IMA WAV");
 1747 ADPCM_DECODER(AV_CODEC_ID_ADPCM_IMA_WS,      sample_fmts_both, adpcm_ima_ws,      "ADPCM IMA Westwood");
 1748 ADPCM_DECODER(AV_CODEC_ID_ADPCM_MS,          sample_fmts_s16,  adpcm_ms,          "ADPCM Microsoft");
 1749 ADPCM_DECODER(AV_CODEC_ID_ADPCM_MTAF,        sample_fmts_s16p, adpcm_mtaf,        "ADPCM MTAF");
 1750 ADPCM_DECODER(AV_CODEC_ID_ADPCM_PSX,         sample_fmts_s16p, adpcm_psx,         "ADPCM Playstation");
 1751 ADPCM_DECODER(AV_CODEC_ID_ADPCM_SBPRO_2,     sample_fmts_s16,  adpcm_sbpro_2,     "ADPCM Sound Blaster Pro 2-bit");
 1752 ADPCM_DECODER(AV_CODEC_ID_ADPCM_SBPRO_3,     sample_fmts_s16,  adpcm_sbpro_3,     "ADPCM Sound Blaster Pro 2.6-bit");
 1753 ADPCM_DECODER(AV_CODEC_ID_ADPCM_SBPRO_4,     sample_fmts_s16,  adpcm_sbpro_4,     "ADPCM Sound Blaster Pro 4-bit");
 1754 ADPCM_DECODER(AV_CODEC_ID_ADPCM_SWF,         sample_fmts_s16,  adpcm_swf,         "ADPCM Shockwave Flash");
 1755 ADPCM_DECODER(AV_CODEC_ID_ADPCM_THP_LE,      sample_fmts_s16p, adpcm_thp_le,      "ADPCM Nintendo THP (little-endian)");
 1756 ADPCM_DECODER(AV_CODEC_ID_ADPCM_THP,         sample_fmts_s16p, adpcm_thp,         "ADPCM Nintendo THP");
 1757 ADPCM_DECODER(AV_CODEC_ID_ADPCM_XA,          sample_fmts_s16p, adpcm_xa,          "ADPCM CDROM XA");
 1758 ADPCM_DECODER(AV_CODEC_ID_ADPCM_YAMAHA,      sample_fmts_s16,  adpcm_yamaha,      "ADPCM Yamaha");
```





