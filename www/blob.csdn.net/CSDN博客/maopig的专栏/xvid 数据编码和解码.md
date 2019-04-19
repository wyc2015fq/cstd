# xvid 数据编码和解码 - maopig的专栏 - CSDN博客
2011年09月13日 14:36:05[maopig](https://me.csdn.net/maopig)阅读数：2099
由于视频开发的需求，封装xvid c调用接口，使之优雅易用
我已经对pc camera视频流(RGB)进行尝试，编码之后传输到远程主机进行解码回放，效果不错
具体的xvid的参数可以进一步优化，这里只是提供简单的范例
1. xvid 解码
 1
 2/*
 3    encode.cpp
 4    xvid 编码YUV数据，为进一步传输提供准备
 5
 6*/
 7#ifndef _XVID_ENCODE_H
 8#define _XVID_ENCODE_H
 9
10#include <xvid.h>
11
12struct DecodeInfo{
13    unsigned short    width;
14    unsigned short  height;        
15void (*after_decode)(void* data,unsigned int size,int width,int height,void* user);
16void* user;
17};
18
19
20class Xvid_Decoder{
21
22public:
23    Xvid_Decoder(){ 
24    }
25bool            Open();
26void            Close();
27    DecodeInfo &    GetDecodeInfo(){    return _dec_info;}
28void            decode(void*data,unsigned int size);
29staticvoid    xvid_global_init();
30    xvid_dec_create_t&    GetHandle(){    return _xvid_dec_create;}
31private:
32int                xvid_dec(unsigned char*bitstream,int bs_size,unsigned char*image);
33private:
34    DecodeInfo    _dec_info;
35void*        _dec_handle;
36char         _image[1024*768*3];
37    xvid_dec_create_t _xvid_dec_create;    
38};
39
40#endif
41
 1
 2/*
 3    encode.cpp
 4    xvid 编码YUV数据，为进一步传输提供准备
 5
 6*/
 7#include "xvid_dec.h"
 8#include <windows.h>
 9
10//////////////////////////////////////////////////////////////////////////
11
12
13void Xvid_Decoder::Close(){
14int xerr;    
15/* Destroy the encoder instance */
16    xerr = xvid_decore(_dec_handle, XVID_ENC_DESTROY, NULL, NULL);    
17}
18
19void Xvid_Decoder::xvid_global_init(){
20/*------------------------------------------------------------------------
21     * XviD core initialization
22     *----------------------------------------------------------------------*/
23    xvid_gbl_init_t xvid_gbl_init;
24    memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init));
25    xvid_gbl_init.version = XVID_VERSION;
26    xvid_gbl_init.cpu_flags = XVID_CPU_FORCE;
27/* Initialize XviD core -- Should be done once per __process__ */
28    xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);
29}
30
31bool Xvid_Decoder::Open(){
32int ret;    
33
34/*------------------------------------------------------------------------
35     * XviD encoder initialization
36     *----------------------------------------------------------------------*/
37int width,height;
38    width = _xvid_dec_create.width ;
39    height = _xvid_dec_create.height;
40    memset(&_xvid_dec_create, 0, sizeof(xvid_dec_create_t));
41    _xvid_dec_create.version = XVID_VERSION;
42
43/* Width and Height of input frames */
44    _xvid_dec_create.width = width;
45    _xvid_dec_create.height = height;
46
47    ret = xvid_decore(NULL, XVID_DEC_CREATE, &_xvid_dec_create, NULL);
48    _dec_handle = _xvid_dec_create.handle;
49returntrue;
50}
51
52void Xvid_Decoder::decode(void* data,unsigned int size){
53int ret;
54    ret = xvid_dec((unsigned char*)data,(int)size,(unsigned char*)_image);    
55if (ret >0)
56        _dec_info.after_decode(_image,(unsigned int)ret,(int)_xvid_dec_create.width,(int)_xvid_dec_create.height,_dec_info.user);
57}
58
59/*
60raw xvid_encode procedure
61*/
62int    Xvid_Decoder::xvid_dec(unsigned char*bitstream,int bs_size,unsigned char*image)
63{
64int ret;
65    xvid_dec_frame_t xvid_dec_frame;
66
67/* Reset all structures */
68    memset(&xvid_dec_frame, 0, sizeof(xvid_dec_frame_t));
69/* Set version */
70    xvid_dec_frame.version = XVID_VERSION;
71//    xvid_dec_stats->version = XVID_VERSION;
72
73/* No general flags to set */
74    xvid_dec_frame.general          =0;
75
76/* Input stream */
77    xvid_dec_frame.bitstream        = bitstream;
78    xvid_dec_frame.length           = bs_size;
79
80/* Output frame structure */
81    xvid_dec_frame.output.plane[0]  = image;
82    xvid_dec_frame.output.stride[0] = _xvid_dec_create.width  *3;
83    xvid_dec_frame.output.csp = XVID_CSP_BGR;
84
85    ret = xvid_decore(_dec_handle, XVID_DEC_DECODE, &xvid_dec_frame, NULL);
86return(ret);    
87}
88
89
90
2. xvid 编码
 1
 2/*
 3    encode.cpp
 4    xvid 编码YUV数据，为进一步传输提供准备
 5
 6*/
 7#ifndef _XVID_ENCODE_H
 8#define _XVID_ENCODE_H
 9
10#include <xvid.h>
11#include <nv.h>
12
13
14
15struct EncodeInfo{
16    unsigned short    width;
17    unsigned short  height;        
18void (*after_encode)(void* data,unsigned int size,void* user);
19void* user;
20};
21
22class Xvid_Encoder:public NVObject{
23public:
24    Xvid_Encoder(){ _closed =true;
25    }
26bool            Open();
27void            Close();
28    EncodeInfo &    GetEncodeInfo(){    return _enc_info;}
29void            encode(void*data,unsigned int size);
30staticvoid    xvid_global_init();
31int                xvid_enc(unsigned char*image,unsigned char*bitstream);
32private:
33    EncodeInfo    _enc_info;
34void*        _enc_handle;
35char         _bitstream[1024*100];
36bool        _closed;
37
38
39};
40
41#endif
42
  1
  2/*
  3    encode.cpp
  4    xvid 编码YUV数据，为进一步传输提供准备
  5
  6*/
  7#include "encode.h"
  8#include <windows.h>
  9
 10
 11staticconstint motion_presets[] = {
 12/* quality 0 */
 130,
 14
 15/* quality 1 */
 16        XVID_ME_ADVANCEDDIAMOND16,
 17
 18/* quality 2 */
 19        XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16,
 20
 21/* quality 3 */
 22        XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
 23        XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8,
 24
 25/* quality 4 */
 26        XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
 27        XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 |
 28        XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
 29
 30/* quality 5 */
 31        XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 |
 32        XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 |
 33        XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
 34
 35/* quality 6 */
 36        XVID_ME_ADVANCEDDIAMOND16 | XVID_ME_HALFPELREFINE16 | XVID_ME_EXTSEARCH16 |
 37        XVID_ME_ADVANCEDDIAMOND8 | XVID_ME_HALFPELREFINE8 | XVID_ME_EXTSEARCH8 |
 38        XVID_ME_CHROMA_PVOP | XVID_ME_CHROMA_BVOP,
 39
 40};
 41#define ME_ELEMENTS (sizeof(motion_presets)/sizeof(motion_presets[0]))
 42
 43staticconstint vop_presets[] = {
 44/* quality 0 */
 450,
 46
 47/* quality 1 */
 480,
 49
 50/* quality 2 */
 51    XVID_VOP_HALFPEL,
 52
 53/* quality 3 */
 54    XVID_VOP_HALFPEL | XVID_VOP_INTER4V,
 55
 56/* quality 4 */
 57    XVID_VOP_HALFPEL | XVID_VOP_INTER4V,
 58
 59/* quality 5 */
 60    XVID_VOP_HALFPEL | XVID_VOP_INTER4V |
 61    XVID_VOP_TRELLISQUANT,
 62
 63/* quality 6 */
 64    XVID_VOP_HALFPEL | XVID_VOP_INTER4V |
 65    XVID_VOP_TRELLISQUANT | XVID_VOP_HQACPRED,
 66
 67};
 68#define VOP_ELEMENTS (sizeof(vop_presets)/sizeof(vop_presets[0]))
 69
 70//////////////////////////////////////////////////////////////////////////
 71#define MAX_ZONES   64
 72
 73/* Maximum number of frames to encode */
 74#define ABS_MAXFRAMENR 9999
 75
 76staticint ARG_STATS =0;
 77staticint ARG_DUMP =0;
 78staticint ARG_LUMIMASKING =0;
 79staticint ARG_BITRATE =0;
 80staticint ARG_SINGLE =0;
 81staticchar*ARG_PASS1 =0;
 82staticchar*ARG_PASS2 =0;
 83staticint ARG_QUALITY = ME_ELEMENTS -1;
 84staticfloat ARG_FRAMERATE =25.00f;
 85staticint ARG_MAXFRAMENR = ABS_MAXFRAMENR;
 86staticint ARG_MAXKEYINTERVAL =0;
 87staticchar*ARG_INPUTFILE = NULL;
 88staticint ARG_INPUTTYPE =0;
 89staticint ARG_SAVEMPEGSTREAM =0;
 90staticint ARG_SAVEINDIVIDUAL =0;
 91staticchar*ARG_OUTPUTFILE = NULL;
 92
 93staticint ARG_BQRATIO =150;
 94staticint ARG_BQOFFSET =100;
 95staticint ARG_MAXBFRAMES =0;
 96staticint ARG_PACKED =0;
 97
 98staticint ARG_VOPDEBUG =0;
 99staticint ARG_GMC =0;
100staticint ARG_INTERLACING =0;
101staticint ARG_QPEL =0;
102staticint ARG_CLOSED_GOP =0;
103
104#ifndef READ_PNM
105#define IMAGE_SIZE(x,y) ((x)*(y)*3/2)
106#else
107#define IMAGE_SIZE(x,y) ((x)*(y)*3)
108#endif
109
110#define MAX(A,B) ( ((A)>(B)) ? (A) : (B) )
111#define SMALL_EPS (1e-10)
112
113#define SWAP(a) ( (((a)&0x000000ff)<<24) | (((a)&0x0000ff00)<<8) | \
114(((a)&0x00ff0000)>>8)  | (((a)&0xff000000)>>24) )
115
116
117//////////////////////////////////////////////////////////////////////////
118
119
120void Xvid_Encoder::Close(){
121int xerr;    
122    NVMutexLock lock(_lock);
123
124    _closed =true;
125/* Destroy the encoder instance */
126    xerr = xvid_encore(_enc_handle, XVID_ENC_DESTROY, NULL, NULL);    
127
128}
129
130void Xvid_Encoder::xvid_global_init(){
131/*------------------------------------------------------------------------
132     * XviD core initialization
133     *----------------------------------------------------------------------*/
134    xvid_gbl_init_t xvid_gbl_init;
135    memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init));
136    xvid_gbl_init.version = XVID_VERSION;
137    xvid_gbl_init.cpu_flags = XVID_CPU_FORCE;
138/* Initialize XviD core -- Should be done once per __process__ */
139    xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);
140}
141
142bool Xvid_Encoder::Open(){
143int xerr;        
144    xvid_enc_create_t xvid_enc_create;
145    _closed =false;
146/*------------------------------------------------------------------------
147     * XviD encoder initialization
148     *----------------------------------------------------------------------*/
149    memset(&xvid_enc_create, 0, sizeof(xvid_enc_create));
150    xvid_enc_create.version = XVID_VERSION;
151
152/* Width and Height of input frames */
153    xvid_enc_create.width = GetEncodeInfo().width;
154    xvid_enc_create.height = GetEncodeInfo().height;
155    xvid_enc_create.profile = XVID_PROFILE_AS_L4;
156
157/* init plugins  */
158/*
159    xvid_enc_create.zones = ZONES;
160    xvid_enc_create.num_zones = NUM_ZONES;
161
162    xvid_enc_create.plugins = plugins;
163    xvid_enc_create.num_plugins = 0;
164*/
165
166
167/* No fancy thread tests */
168    xvid_enc_create.num_threads =0;
169
170/* Frame rate - Do some quick float fps = fincr/fbase hack */
171    xvid_enc_create.fincr =1;
172    xvid_enc_create.fbase = (int) 10;
173/* Maximum key frame interval */
174    xvid_enc_create.max_key_interval = (int)-1;    //--default 10s
175/* Bframes settings */
176    xvid_enc_create.max_bframes = ARG_MAXBFRAMES;
177    xvid_enc_create.bquant_ratio = ARG_BQRATIO;
178    xvid_enc_create.bquant_offset = ARG_BQOFFSET;
179
180/* Dropping ratio frame -- we don't need that */
181    xvid_enc_create.frame_drop_ratio =0;
182/* Global encoder options */
183    xvid_enc_create.global =0;
184
185if (ARG_PACKED)
186        xvid_enc_create.global |= XVID_GLOBAL_PACKED;
187
188if (ARG_CLOSED_GOP)
189        xvid_enc_create.global |= XVID_GLOBAL_CLOSED_GOP;
190
191if (ARG_STATS)
192        xvid_enc_create.global |= XVID_GLOBAL_EXTRASTATS_ENABLE;
193
194/* I use a small value here, since will not encode whole movies, but short clips */
195    xerr = xvid_encore(NULL, XVID_ENC_CREATE, &xvid_enc_create, NULL);
196    _enc_handle = xvid_enc_create.handle;
197returntrue;
198}
199
200void Xvid_Encoder::encode(void* data,unsigned int size){
201int ret;    
202    _lock.Lock();
203if( _closed ){
204        _lock.Unlock();
205return;
206    }
207    ret = xvid_enc((unsigned char*)data,(unsigned char*)_bitstream);        
208    _lock.Unlock();
209if (ret >0)
210        _enc_info.after_encode(_bitstream,(unsigned int)ret,_enc_info.user);
211
212}
213
214/*
215raw xvid_encode procedure
216*/
217int Xvid_Encoder::xvid_enc(unsigned char*image,unsigned char*bitstream)
218{
219int ret;    
220    xvid_enc_frame_t xvid_enc_frame;
221    xvid_enc_stats_t xvid_enc_stats;
222
223/* Version for the frame and the stats */
224    memset(&xvid_enc_frame, 0, sizeof(xvid_enc_frame));
225    xvid_enc_frame.version = XVID_VERSION;
226
227    memset(&xvid_enc_stats, 0, sizeof(xvid_enc_stats));
228    xvid_enc_stats.version = XVID_VERSION;
229
230/* Bind output buffer */
231    xvid_enc_frame.bitstream = bitstream;
232    xvid_enc_frame.length =-1;
233
234/* Initialize input image fields */
235    xvid_enc_frame.input.plane[0] = image;
236    xvid_enc_frame.input.csp = XVID_CSP_BGR;
237    xvid_enc_frame.input.stride[0] = _enc_info.width*3;
238
239
240/* Set up core's general features */
241    xvid_enc_frame.vol_flags =0;
242
243/* Set up core's general features */
244    xvid_enc_frame.vop_flags = vop_presets[ARG_QUALITY-2];
245
246/* Frame type -- let core decide for us */
247    xvid_enc_frame.type = XVID_TYPE_AUTO;
248
249/* Force the right quantizer -- It is internally managed by RC plugins */
250    xvid_enc_frame.quant =0;
251
252/* Set up motion estimation flags */
253    xvid_enc_frame.motion = motion_presets[ARG_QUALITY-2];
254
255/* We don't use special matrices */
256    xvid_enc_frame.quant_intra_matrix = NULL;
257    xvid_enc_frame.quant_inter_matrix = NULL;
258
259/* Encode the frame */
260    ret = xvid_encore(_enc_handle, XVID_ENC_ENCODE, &xvid_enc_frame,NULL);
261//    &xvid_enc_stats);
262//--判别是否是关键帧
263//*key = (xvid_enc_frame.out_flags & XVID_KEYFRAME);
264//*stats_type = xvid_enc_stats.type;
265//*stats_quant = xvid_enc_stats.quant;
266//*stats_length = xvid_enc_stats.length;
267//sse[0] = xvid_enc_stats.sse_y;
268//sse[1] = xvid_enc_stats.sse_u;
269//sse[2] = xvid_enc_stats.sse_v;
270
271return (ret);
272}
273
274
275
