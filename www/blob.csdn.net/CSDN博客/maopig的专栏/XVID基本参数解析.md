# XVID基本参数解析 - maopig的专栏 - CSDN博客
2011年09月17日 11:35:54[maopig](https://me.csdn.net/maopig)阅读数：3113
                
            XVID，X264等是MPEG4、H264标准的开源编码器，其中X264只有编码部分，解码部分需要FFMPEG完成；XVID有编解码部分，其中解码亦可以利用FFMPEG中的MPEG4完成解码。视频压缩算法的计算复杂度，都是比较高的。其中具有最大计算复杂度有三部分：宏块搜素运动补偿部分、码率控制部分、滤波算法部分；这三部分占据了算法复杂度的绝大部分资源。
XVID参数解释：
参数的设置集中在FFMPEG中的libxvidff.C中的ff_xvid_encode_init（）函数里面，主要是对编码上下文进行初始化赋值操作，此函数中的宏定义在XVID.H之中：
ff_xvid_encode_init(AVCodecContext *avctx)  {
    int xerr, i;
    int xvid_flags = avctx->flags;
    xvid_context_t *x = avctx->priv_data;
    uint16_t *intra, *inter;
    int fd;
    xvid_plugin_single_t single;
    xvid_ff_pass1_t rc2pass1;
    xvid_plugin_2pass2_t rc2pass2;
    xvid_gbl_init_t xvid_gbl_init;
    xvid_enc_create_t xvid_enc_create;
    xvid_enc_plugin_t plugins[7];
    /* Bring in VOP flags from ffmpeg command-line */
    x->vop_flags = XVID_VOP_HALFPEL; /* Bare minimum quality 
*/ 半像素运动插值
    if( xvid_flags & CODEC_FLAG_4MV )
        x->vop_flags |= XVID_VOP_INTER4V; /* Level 3 
*/每个宏块分配四个运动矢量
    if( xvid_flags & CODEC_FLAG_TRELLIS_QUANT)
        x->vop_flags |= XVID_VOP_TRELLISQUANT; /* Level 5 */
    if( xvid_flags & CODEC_FLAG_AC_PRED )
        x->vop_flags |= XVID_VOP_HQACPRED; /* Level 6 */高质量的AC预测
    if( xvid_flags & CODEC_FLAG_GRAY )
        x->vop_flags |= XVID_VOP_GREYSCALE;
    /* Decide which ME quality setting to use */
    x->me_flags = 0;
    switch( avctx->me_method ) {//宏块搜索区域，搜索算法
       case ME_FULL:   /* Quality 6 */
           x->me_flags |=  XVID_ME_EXTSEARCH16/* extend PMV by more searches */
                       |   XVID_ME_EXTSEARCH8;/* use diamond/square for extended 8x8 search */
       case ME_EPZS:   /* Quality 4 */
           x->me_flags |=  XVID_ME_ADVANCEDDIAMOND8/* use advdiamond for XVID_ME_EXTSEARCH8 */
                       |   XVID_ME_HALFPELREFINE8
                       |   XVID_ME_CHROMA_PVOP/* also use chroma for P_VOP/S_VOP ME */
                       |   XVID_ME_CHROMA_BVOP;/* also use chroma for B_VOP ME */
       case ME_LOG:    /* Quality 2 */
       case ME_PHODS:
       case ME_X1:
           x->me_flags |=  XVID_ME_ADVANCEDDIAMOND16/* use advdiamonds instead of diamonds as search pattern */
                       |   XVID_ME_HALFPELREFINE16;
       case ME_ZERO:   /* Quality 0 */
       default:
           break;
    }
    /* Decide how we should decide blocks */
    switch( avctx->mb_decision ) {//是否选择码率控制方式 avctx->mb_decision=1时，编码一帧时间迅速增加
       case 2:
           x->vop_flags |= XVID_VOP_MODEDECISION_RD;
           x->me_flags |=  XVID_ME_HALFPELREFINE8_RD
                       |   XVID_ME_QUARTERPELREFINE8_RD
                       |   XVID_ME_EXTSEARCH_RD
                       |   XVID_ME_CHECKPREDICTION_RD;
       case 1:
           if( !(x->vop_flags & XVID_VOP_MODEDECISION_RD) )
               x->vop_flags |= XVID_VOP_FAST_MODEDECISION_RD;
           x->me_flags |=  XVID_ME_HALFPELREFINE16_RD
                       |   XVID_ME_QUARTERPELREFINE16_RD;
       default:
           break;
    }
    /* Bring in VOL flags from ffmpeg command-line */
    x->vol_flags = 0;
    if( xvid_flags & CODEC_FLAG_GMC ) {
        x->vol_flags |= XVID_VOL_GMC;
        x->me_flags |= XVID_ME_GME_REFINE;
    }
    if( xvid_flags & CODEC_FLAG_QPEL ) {//是否允许1/4像素
        x->vol_flags |= XVID_VOL_QUARTERPEL;
        x->me_flags |= XVID_ME_QUARTERPELREFINE16;
        if( x->vop_flags & XVID_VOP_INTER4V )
            x->me_flags |= XVID_ME_QUARTERPELREFINE8;
    }
    memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init));
    xvid_gbl_init.version = XVID_VERSION;
    xvid_gbl_init.debug = 0;
#ifdef ARCH_POWERPC
    /* XviD's PPC support is borked, use libavcodec to detect */
#if HAVE_ALTIVEC==1
    if( has_altivec() ) {
        xvid_gbl_init.cpu_flags = XVID_CPU_FORCE | XVID_CPU_ALTIVEC;
    } else
#endif
        xvid_gbl_init.cpu_flags = XVID_CPU_FORCE;
#else
    /* XviD can detect on x86 */
    xvid_gbl_init.cpu_flags = 0;
#endif
    /* Initialize */
    xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);
    /* Create the encoder reference */
    memset(&xvid_enc_create, 0, sizeof(xvid_enc_create));
    xvid_enc_create.version = XVID_VERSION;
    /* Store the desired frame size */
    xvid_enc_create.width = x->xsize = avctx->width;//图像宽度
    xvid_enc_create.height = x->ysize = avctx->height;//图像高度
    /* XviD can determine the proper profile to use */
     xvid_enc_create.profile = XVID_PROFILE_S_L3; //  
编码设定档次和级别 //压缩级别，MPEG4-ASP最高压缩级别
    /* We don't use zones or threads */
    xvid_enc_create.zones = NULL;
    xvid_enc_create.num_zones = 0;
    xvid_enc_create.num_threads = 0;
    xvid_enc_create.plugins = plugins;
    xvid_enc_create.num_plugins = 0;
    /* Initialize Buffers */
    x->twopassbuffer = NULL;
    x->old_twopassbuffer = NULL;
    x->twopassfile = NULL;
    if( xvid_flags & CODEC_FLAG_PASS1 ) {
        }
        x->twopassbuffer[0] = x->old_twopassbuffer[0] = 0;
        plugins[xvid_enc_create.num_plugins].func = xvid_ff_2pass;
        plugins[xvid_enc_create.num_plugins].param = &rc2pass1;
        xvid_enc_create.num_plugins++;
    } else if( xvid_flags & CODEC_FLAG_PASS2 ) {//第二次编码，需要第一次编码生成的文件才可以完成第二次编码，不适用实时编码
        memset(&rc2pass2, 0, sizeof(xvid_plugin_2pass2_t));
        rc2pass2.version = XVID_VERSION;
        rc2pass2.bitrate = avctx->bit_rate;
        close(fd);
        rc2pass2.filename = x->twopassfile;
        plugins[xvid_enc_create.num_plugins].func = xvid_plugin_2pass2;
        plugins[xvid_enc_create.num_plugins].param = &rc2pass2;
        xvid_enc_create.num_plugins++;
    } else if( !(xvid_flags & CODEC_FLAG_QSCALE) ) {
        /* Single Pass Bitrate Control! */
        memset(&single, 0, sizeof(xvid_plugin_single_t));
        single.version = XVID_VERSION;
        single.bitrate = avctx->bit_rate;
        plugins[xvid_enc_create.num_plugins].func = xvid_plugin_single;
        plugins[xvid_enc_create.num_plugins].param = &single;
        xvid_enc_create.num_plugins++;
    }
    /* Luminance Masking */
    if( 0.0 != avctx->lumi_masking ) {
        plugins[xvid_enc_create.num_plugins].func = xvid_plugin_lumimasking;
        plugins[xvid_enc_create.num_plugins].param = NULL;
        xvid_enc_create.num_plugins++;
    }
    /* Frame Rate and Key Frames */
    xvid_correct_framerate(avctx);
    xvid_enc_create.fincr = avctx->time_base.num;
    xvid_enc_create.fbase = avctx->time_base.den;
    if( avctx->gop_size > 0 )
        xvid_enc_create.max_key_interval = avctx->gop_size;//图像组的长度设定
    else
        xvid_enc_create.max_key_interval = 240; /* XviD's best default */
    /* Quants */
    if( xvid_flags & CODEC_FLAG_QSCALE ) x->qscale = 1; ///encodes[i]->codec.vcc->flags|=CODEC_FLAG_QSCALE;选择常量化/ //质量控制=量化参数，1~31，数值越小质量越高
    else x->qscale = 0; 
    xvid_enc_create.min_quant[0] = avctx->qmin;
    xvid_enc_create.min_quant[1] = avctx->qmin;
    xvid_enc_create.min_quant[2] = avctx->qmin;
    xvid_enc_create.max_quant[0] = avctx->qmax;
    xvid_enc_create.max_quant[1] = avctx->qmax;
    xvid_enc_create.max_quant[2] = avctx->qmax;
    /* Quant Matrices */
    x->intra_matrix = x->inter_matrix = NULL;
    if( avctx->mpeg_quant )//允许MPEG量化 量化矩阵
       x->vol_flags |= XVID_VOL_MPEGQUANT;
    if( (avctx->intra_matrix || avctx->inter_matrix) ) {//可以自己设定量化矩阵avctx->intra_matrix，avctx->inter_matrix
       x->vol_flags |= XVID_VOL_MPEGQUANT;
       if( avctx->intra_matrix ) {
           intra = avctx->intra_matrix;
           x->intra_matrix = av_malloc(sizeof(unsigned char) * 64);
       } else
           intra = NULL;
       if( avctx->inter_matrix ) {
           inter = avctx->inter_matrix;
           x->inter_matrix = av_malloc(sizeof(unsigned char) * 64);
       } else
           inter = NULL;
       for( i = 0; i < 64; i++ ) {
           if( intra )
               x->intra_matrix[i] = (unsigned char)intra[i];
           if( inter )
               x->inter_matrix[i] = (unsigned char)inter[i];
       }
    }
    /* Misc Settings */
    xvid_enc_create.frame_drop_ratio = 0;//丢帧率；0~100
    xvid_enc_create.global = 0;
    if( xvid_flags & CODEC_FLAG_CLOSED_GOP )
        xvid_enc_create.global |= XVID_GLOBAL_CLOSED_GOP;
    /* Determines which codec mode we are operating in */
    avctx->extradata = NULL;
    avctx->extradata_size = 0;
    if( xvid_flags & CODEC_FLAG_GLOBAL_HEADER ) {
        /* In this case, we are claiming to be MPEG4 */
        x->quicktime_format = 1;
        avctx->codec_id = CODEC_ID_MPEG4;
    } else {
        /* We are claiming to be XviD */
        x->quicktime_format = 0;
        if(!avctx->codec_tag)
            avctx->codec_tag = ff_get_fourcc("xvid");
    }
    /* Bframes */
    xvid_enc_create.max_bframes = avctx->max_b_frames;
    xvid_enc_create.bquant_offset = 100 * avctx->b_quant_offset;
    xvid_enc_create.bquant_ratio = 100 * avctx->b_quant_factor;
    if( avctx->max_b_frames > 0  && !x->quicktime_format ) xvid_enc_create.global |= XVID_GLOBAL_PACKED;
    /* Create encoder context */
    xerr = xvid_encore(NULL, XVID_ENC_CREATE, &xvid_enc_create, NULL);//建立编码上下文
    if( xerr ) {
        av_log(avctx, AV_LOG_ERROR, "XviD: Could not create encoder reference\n");
        return -1;
    }
    x->encoder_handle = xvid_enc_create.handle;
    avctx->coded_frame = &x->encoded_picture;
    return 0;
}
int ff_xvid_encode_frame(AVCodecContext *avctx,
                         unsigned char *frame, int buf_size, void *data) { 
开始编码一帧
 xerr = xvid_encore(x->encoder_handle, XVID_ENC_ENCODE,
        &xvid_enc_frame, &xvid_enc_stats);
//开始编码一帧
｝
XVID的量化可以有三种方式：常量化，用户自己设定量化矩阵，调用默认的量化矩阵
