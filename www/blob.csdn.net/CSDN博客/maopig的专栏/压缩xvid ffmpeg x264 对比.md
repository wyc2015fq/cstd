# 压缩xvid ffmpeg x264 对比 - maopig的专栏 - CSDN博客
2011年07月12日 10:55:20[maopig](https://me.csdn.net/maopig)阅读数：2733
### 压缩xvid ffmpeg x264 对比 
Xvid是基于MPEG4协议的编解码器，x264是基于H.264协议的编码器，ffmpeg集合了各种音频，视频编解码协议，通过设置参数可以完成基于MPEG4,H.264等协议的编解码，现在将其对比结果罗列如下：
1.软件版本：
       Xvid:1.2.1
       x264:无
       ffmpeg:0.5
2. 视频样本 foreman.cif 300帧，大小352x288
3. 所选用编解码器：  xvid, x264, ffmpeg
4. 视频流比特率设定 : 1000kbps.
5. 测试结果：
 编码器  编码时间(s) 视频编码后大小(B) 比特率(kbps) 输入视频格式
    Xvid           0.98        1779213        1.18        yuyv
    X264           2.2         1788824        1.19        yv12
 ffmpeg(mpeg4 协议) 0.76        1754847        1.17        yv12
  ffmpeg(h.264协议) 4.38        1741733        1.16        yv12
    解码器      视频所用的编码器    解码时间(ｓ)    输出视频格式
    Xvid               Xvid             0.47        yuyv
    Xvid               ffmpeg           0.50        yuyv
    ffmpeg(mpeg4)      Xvid             0.26        yv12
    ffmpeg(mpeg4)       ffmpeg(mpeg4)    0.31        yv12
    ffmpeg(h.264)        ffmpeg(h.264)    0.45        yv12
    ffmpeg(h.264)        X264             0.78        yuyv
6.结论
    从以上可以看出：
    1).    对于基于MPEG4协议的编解码，ffmpeg的效率都要高于Xvid,主要原因应该是Xvid是以保存视频效果为首要出发点的，其对视频质量的保存应该是做的最好的，这就在一定程度上增加了编解码的复杂性，影响了速度的提升。
    2).    对于基于H.264协议的编解码,由于ffmpeg没有本身的h.264编码器，其H.264协议编码是调用x264的编码库来完成的，所以其性能本质上还是x264的，只不过ffmpeg对其进行了封装，但从数据对比可以看出，在本次实验中，ffmpeg调用x264库编码用了4.38秒，远远高于 x264的2.2秒，这主要应该是ffmpeg调用x264库时对其进行了不同的参数设置，当然对x264进行接口的封装也会影响一部分性能，但应该不是主要的，主要的应该还是调用x264时，参数有些不同。
    所以如果做基于MPEG4的编解码，如果对速度要求高的话，ffmpeg是很好用的工具。
    对于H.264协议来说，ffmpeg能很好的解码，而编码的话最好还是要调用x264，因为ffmpeg还没有实现H.264的编码。
附：各编解码器参数设定：
    Xvid:         
    xvid_enc_create_obj.version = XVID_VERSION;
        xvid_enc_create_obj.max_key_interval = 100;  //must mark it !!
        xvid_enc_create_obj.profile = 0xf5;//XVID_PROFILE_S_L0;
        xvid_enc_create_obj.fincr = 1;
        xvid_enc_create_obj.fbase = 25;        //must set for bitrate kxw
        xvid_enc_create_obj.num_threads = 0;
        xvid_enc_create_obj.global = 0;
        xvid_enc_create_obj.plugins = plugins;
        xvid_enc_create_obj.num_plugins = 1;
                memset(&plugin_single, 0, sizeof(xvid_plugin_single_t));
                plugin_single.version = XVID_VERSION;
                plugin_single.bitrate = bitrate * 1024; // 1000*1024
                plugin_single.reaction_delay_factor = 16;
                plugin_single.averaging_period = 100;
                plugin_single.buffer = 100;
                plugins[0].func = xvid_plugin_single;
                plugins[0].param = &plugin_single;
    X264:
 param->cpu = X264_CPU_MMXEXT|X264_CPU_SSE|X264_CPU_SSE2;//0x177a;//x264_cpu_detect();
    param->i_threads = 1;
    param->b_deterministic = 1;
    /* Video properties */
    param->i_csp           = X264_CSP_I420;
    param->i_width         = 0;
    param->i_height        = 0;
    param->vui.i_sar_width = 0;
    param->vui.i_sar_height= 0;
    param->vui.i_overscan  = 0;  /* undef */
    param->vui.i_vidformat = 5;  /* undef */
    param->vui.b_fullrange = 0;  /* off */
    param->vui.i_colorprim = 2;  /* undef */
    param->vui.i_transfer  = 2;  /* undef */
    param->vui.i_colmatrix = 2;  /* undef */
    param->vui.i_chroma_loc= 0;  /* left center */
    param->i_fps_num       = 25;
    param->i_fps_den       = 1;
    param->i_level_idc     = -1;
    /* Encoder parameters */
    param->i_frame_reference = 1;
    param->i_keyint_max = 250;
    param->i_keyint_min = 25;
    param->i_bframe = 0;                //ｎｏ　ｂ　ｆｒａｍｅ.
    param->i_scenecut_threshold = 40;
    param->i_bframe_adaptive = X264_B_ADAPT_FAST;
    param->i_bframe_bias = 0;
    param->b_bframe_pyramid = 0;
    param->b_deblocking_filter = 1;
    param->i_deblocking_filter_alphac0 = 0;
    param->i_deblocking_filter_beta = 0;
    param->b_cabac = 1;
    param->i_cabac_init_idc = 0;
    param->rc.i_rc_method = X264_RC_ABR;   //kxw notice!
    param->rc.i_bitrate = 0;
    param->rc.f_rate_tolerance = 1.0;
    param->rc.i_vbv_max_bitrate = 0;
    param->rc.i_vbv_buffer_size = 0;
    param->rc.f_vbv_buffer_init = 0.9;
    param->rc.i_qp_constant = 26;    //            default QP = 26...........kxw mod
    param->rc.f_rf_constant = 0;
    param->rc.i_qp_min = 10;
    param->rc.i_qp_max = 51;
    param->rc.i_qp_step = 4;
    param->rc.f_ip_factor = 1.4;
    param->rc.f_pb_factor = 1.3;
    param->rc.i_aq_mode = X264_AQ_VARIANCE;
    param->rc.f_aq_strength = 1.0;
    param->rc.b_stat_write = 0;
    //param->rc.psz_stat_out = "x264_2pass.log";
    param->rc.b_stat_read = 0;
    //param->rc.psz_stat_in = "x264_2pass.log";
    param->rc.f_qcompress = 0.6;
    param->rc.f_qblur = 0.5;
    param->rc.f_complexity_blur = 20;
    param->rc.i_zones = 0;
    /* Log */
    param->pf_log = NULL;//x264_log_default;//x264_log_default;
    param->p_log_private = NULL;
    param->i_log_level = X264_LOG_INFO;
    #if 0                                                //consume time .kxw mod
    param->analyse.intra = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8;
    param->analyse.inter = X264_ANALYSE_I4x4 | X264_ANALYSE_I8x8
                         | X264_ANALYSE_PSUB16x16 | X264_ANALYSE_BSUB16x16;
#endif
    param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_SPATIAL;
    param->analyse.i_me_method = X264_ME_HEX;
    param->analyse.f_psy_rd = 1.0;
    param->analyse.f_psy_trellis = 0;
    param->analyse.i_me_range = 16;
#if 0
    param->analyse.i_subpel_refine = 100;  //consume time so much!! kxw mod
#endif
    param->analyse.b_chroma_me = 1;
    param->analyse.i_mv_range_thread = -1;
    param->analyse.i_mv_range = -1; // set from level_idc
    param->analyse.i_chroma_qp_offset = 0;
    param->analyse.b_fast_pskip = 1;
    param->analyse.b_dct_decimate = 1;
    param->analyse.i_luma_deadzone[0] = 21;
    param->analyse.i_luma_deadzone[1] = 11;
    param->analyse.b_psnr = 1;
    param->analyse.b_ssim = 1;
    param->i_cqm_preset = X264_CQM_FLAT;
     x264_param.rc.i_bitrate = br;   //target bitrate. 1000
        x264_param.i_width = width;
        x264_param.i_height = height;
ffmpeg:
    c->bit_rate = 1000000;
    /* resolution must be a multiple of two */
    c->width = FRAMEE_WIDTH;
    c->height = FRAME_HEIGHT;
    /* frames per second */
    c->time_base= (AVRational){1,25};
    c->gop_size = 250; /* emit one intra frame every ten frames */
    c->max_b_frames=0;
    c->pix_fmt = PIX_FMT_YUV420P; 
**X264电影压缩率画质全对比**：http://www.mov8.com/dvd/freetalk_show.asp?id=29778
