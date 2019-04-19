# struct x264_t  维护着CODEC的诸多重要信息 - maopig的专栏 - CSDN博客
2011年08月26日 20:08:25[maopig](https://me.csdn.net/maopig)阅读数：2821
                
//x264_t结构体维护着CODEC的诸多重要信息
struct x264_t
{
    /* encoder parameters ( 编码器参数 )*/
    x264_param_t    param;
    x264_t *thread[X264_SLICE_MAX];
    /* bitstream output ( 字节流输出 ) */
    struct
    {
        int         i_nal;
        x264_nal_t  nal[X264_NAL_MAX];
        int         i_bitstream;    /* size of p_bitstream */
        uint8_t     *p_bitstream;   /* will hold data for all nal */
        bs_t        bs;
    } out;
    /* frame number/poc ( 帧序号 )*/
    int             i_frame;
    int             i_frame_offset; /* decoding only */
    int             i_frame_num;    /* decoding only */
    int             i_poc_msb;      /* decoding only */
    int             i_poc_lsb;      /* decoding only */
    int             i_poc;          /* decoding only */
    int             i_thread_num;   /* threads only */
    int             i_nal_type;     /* threads only */
    int             i_nal_ref_idc;  /* threads only */
    /* We use only one SPS(序列参数集) and one PPS(图像参数集) */
    x264_sps_t      sps_array[1];
    x264_sps_t      *sps;
    x264_pps_t      pps_array[1];
    x264_pps_t      *pps;
    int             i_idr_pic_id;
    int             (*dequant4_mf[4])[4][4]; /* [4][6][4][4] */
    int             (*dequant8_mf[2])[8][8]; /* [2][6][8][8] */
    int             (*quant4_mf[4])[4][4];   /* [4][6][4][4] */
    int             (*quant8_mf[2])[8][8];   /* [2][6][8][8] */
    int             (*unquant4_mf[4])[16];   /* [4][52][16] */
    int             (*unquant8_mf[2])[64];   /* [2][52][64] */
    uint32_t        nr_residual_sum[2][64];
    uint32_t        nr_offset[2][64];
    uint32_t        nr_count[2];
    /* Slice header (片头部) */
    x264_slice_header_t sh;
    /* cabac(适应性二元算术编码) context */
    x264_cabac_t    cabac;
    struct
    {
        /* Frames to be encoded (whose types have been decided(明确的) ) */
        x264_frame_t *current[X264_BFRAME_MAX+3];//current是已经准备就绪可以编码的帧，其类型已经确定
        /* Temporary(临时的) buffer (frames types not yet decided) */
        x264_frame_t *next[X264_BFRAME_MAX+3];//next是尚未确定类型的帧
        /* 未使用的帧Unused frames */
        x264_frame_t *unused[X264_BFRAME_MAX+3];//unused用于回收不使用的frame结构体以备今后再次使用
        /* For adaptive(适应的) B decision(决策) */
        x264_frame_t *last_nonb;
        /* frames used for reference +1 for decoding + sentinels (发射器,标记) */
        x264_frame_t *reference[16+2+1+2];
        int i_last_idr; /* Frame number of the last IDR (立即刷新图像) */
        int i_input;    //frames结构体中i_input指示当前输入的帧的（播放顺序）序号。/* Number of input frames already accepted */
        int i_max_dpb;  /* Number of frames allocated (分配) in the decoded picture buffer */
        int i_max_ref0;
        int i_max_ref1;
        int i_delay;    //i_delay设置为由B帧个数（线程个数）确定的帧缓冲延迟，在多线程情况下为i_delay = i_bframe + i_threads - 1。而判断B帧缓冲填充是否足够则通过条件判断：h->frames.i_input <= h->frames.i_delay + 1 - h->param.i_threads。 /* Number of frames buffered for B reordering (重新排序) */
        int b_have_lowres;  /* Whether 1/2 resolution (分辨率) luma(亮度) planes(平面) are being used */
    } frames;//指示和控制帧编码过程的结构
    /* current frame being encoded */
    x264_frame_t    *fenc;
    /* frame being reconstructed(重建的) */
    x264_frame_t    *fdec;
    /* references(参考) lists */
    int             i_ref0;
    x264_frame_t    *fref0[16+3];     /* ref list 0 */
    int             i_ref1;
    x264_frame_t    *fref1[16+3];     /* ref list 1 */
    int             b_ref_reorder[2];
    /* Current MB DCT coeffs(估计系数) */
    struct
    {
        DECLARE_ALIGNED( int, luma16x16_dc[16], 16 );
        DECLARE_ALIGNED( int, chroma_dc[2][4], 16 );
        // FIXME merge(合并) with union(结合,并集)
        DECLARE_ALIGNED( int, luma8x8[4][64], 16 );
        union
        {
            DECLARE_ALIGNED( int, residual_ac[15], 16 );
            DECLARE_ALIGNED( int, luma4x4[16], 16 );
        } block[16+8];
    } dct;
    /* MB table and cache(高速缓冲存储器) for current frame/mb */
    struct
    {
        int     i_mb_count;                 /* number of mbs in a frame */ /* 在一帧中的宏块中的序号 */
        /* Strides (跨,越) */
        int     i_mb_stride;
        int     i_b8_stride;
        int     i_b4_stride;
        /* Current index */
        int     i_mb_x;
        int     i_mb_y;
        int     i_mb_xy;
        int     i_b8_xy;
        int     i_b4_xy;
        /* Search parameters (搜索参数) */
        int     i_me_method;
        int     i_subpel_refine;
        int     b_chroma_me;
        int     b_trellis;
        int     b_noise_reduction;
        /* Allowed qpel(四分之一映像点) MV range to stay (继续,停留) within the picture + emulated(模拟) edge (边) pixels */
        int     mv_min[2];
        int     mv_max[2];
        /* Subpel MV range for motion search.
         * same mv_min/max but includes levels' i_mv_range. */
        int     mv_min_spel[2];
        int     mv_max_spel[2];
        /* Fullpel MV range for motion search */
        int     mv_min_fpel[2];
        int     mv_max_fpel[2];
        /* neighboring MBs */
        unsigned int i_neighbour;
        unsigned int i_neighbour8[4];       /* neighbours of each 8x8 or 4x4 block that are available */
        unsigned int i_neighbour4[16];      /* at the time the block is coded */
        int     i_mb_type_top; 
        int     i_mb_type_left; 
        int     i_mb_type_topleft; 
        int     i_mb_type_topright; 
        /* mb table */
        int8_t  *type;                      /* mb type */
        int8_t  *qp;                        /* mb qp */
        int16_t *cbp;                       /* mb cbp: 0x0?: luma, 0x?0: chroma, 0x100: luma dc, 0x0200 and 0x0400: chroma dc  (all set for PCM)*/
        int8_t  (*intra4x4_pred_mode)[7];   /* intra4x4 pred mode. for non I4x4 set to I_PRED_4x4_DC(2) */
        uint8_t (*non_zero_count)[16+4+4];  /* nzc. for I_PCM set to 16 */
        int8_t  *chroma_pred_mode;          /* chroma_pred_mode. cabac only. for non intra I_PRED_CHROMA_DC(0) */
        int16_t (*mv[2])[2];                /* mb mv. set to 0 for intra mb */
        int16_t (*mvd[2])[2];               /* mb mv difference with predict. set to 0 if intra. cabac only */
        int8_t   *ref[2];                   /* mb ref. set to -1 if non used (intra or Lx only) */
        int16_t (*mvr[2][16])[2];           /* 16x16 mv for each possible ref */
        int8_t  *skipbp;                    /* block pattern for SKIP or DIRECT (sub)mbs. B-frames + cabac only */
        int8_t  *mb_transform_size;         /* transform_size_8x8_flag of each mb */
        /* current value */
        int     i_type;
        int     i_partition;
        int     i_sub_partition[4];
        int     b_transform_8x8;
        int     i_cbp_luma;
        int     i_cbp_chroma;
        int     i_intra16x16_pred_mode;
        int     i_chroma_pred_mode;
        struct
        {
            /* space for p_fenc and p_fdec */
#define FENC_STRIDE 16
#define FDEC_STRIDE 32
            DECLARE_ALIGNED( uint8_t, fenc_buf[24*FENC_STRIDE], 16 );
            DECLARE_ALIGNED( uint8_t, fdec_buf[27*FDEC_STRIDE], 16 );
            /* pointer over mb of the frame to be compressed */
            uint8_t *p_fenc[3];
            /* pointer over mb of the frame to be reconstrucated  */
            uint8_t *p_fdec[3];
            /* pointer over mb of the references */
            uint8_t *p_fref[2][16][4+2]; /* last: lN, lH, lV, lHV, cU, cV */
            uint16_t *p_integral[2][16];
            /* fref stride */
            int     i_stride[3];
        } pic;
        /* cache */
        struct
        {
            /* real intra4x4_pred_mode if I_4X4 or I_8X8, I_PRED_4x4_DC if mb available, -1 if not */
            int     intra4x4_pred_mode[X264_SCAN8_SIZE];
            /* i_non_zero_count if availble else 0x80 */
            int     non_zero_count[X264_SCAN8_SIZE];
            /* -1 if unused, -2 if unavaible */
            int8_t  ref[2][X264_SCAN8_SIZE];
            /* 0 if non avaible */
            int16_t mv[2][X264_SCAN8_SIZE][2];
            int16_t mvd[2][X264_SCAN8_SIZE][2];
            /* 1 if SKIP or DIRECT. set only for B-frames + CABAC */
            int8_t  skip[X264_SCAN8_SIZE];
            int16_t direct_mv[2][X264_SCAN8_SIZE][2];
            int8_t  direct_ref[2][X264_SCAN8_SIZE];
            /* number of neighbors (top and left) that used 8x8 dct */
            int     i_neighbour_transform_size;
            int     b_transform_8x8_allowed;
        } cache;
        /* */
        int     i_qp;       /* current qp */
        int     i_last_qp;  /* last qp */
        int     i_last_dqp; /* last delta qp */
        int     b_variable_qp; /* whether qp is allowed to vary per macroblock */
        int     b_lossless;
        int     b_direct_auto_read; /* take stats for --direct auto from the 2pass log */
        int     b_direct_auto_write; /* analyse direct modes, to use and/or save */
        /* B_direct and weighted prediction */
        int     dist_scale_factor[16][16];
        int     bipred_weight[16][16];
        /* maps fref1[0]'s ref indices into the current list0 */
        int     map_col_to_list0_buf[2]; // for negative indices
        int     map_col_to_list0[16];
    } mb;
    /* rate control encoding only */
    x264_ratecontrol_t *rc;//struct x264_ratecontrol_t,ratecontrol.c中定义
    /* stats */
    struct
    {
        /* Current frame stats */
        struct
        {
            /* Headers bits (MV+Ref+MB Block Type */
            int i_hdr_bits;
            /* Texture bits (Intra/Predicted) */
            int i_itex_bits;
            int i_ptex_bits;
            /* ? */
            int i_misc_bits;
            /* MB type counts */
            int i_mb_count[19];
            int i_mb_count_i;
            int i_mb_count_p;
            int i_mb_count_skip;
            int i_mb_count_8x8dct[2];//宏块数_8x8DCT
            int i_mb_count_size[7];
            int i_mb_count_ref[16];
            /* Estimated (SATD) cost as Intra/Predicted frame */
            /* XXX: both omit the cost of MBs coded as P_SKIP */
            int i_intra_cost;
            int i_inter_cost;
            /* Adaptive direct mv pred */
            int i_direct_score[2];
        } frame;
        /* Cummulated stats */
        /* per slice info */
        int     i_slice_count[5];
        int64_t i_slice_size[5];
        int     i_slice_qp[5];
        /* */
        int64_t i_sqe_global[5];
        float   f_psnr_average[5];
        float   f_psnr_mean_y[5];
        float   f_psnr_mean_u[5];
        float   f_psnr_mean_v[5];
        /* */
        int64_t i_mb_count[5][19];
        int64_t i_mb_count_8x8dct[2];
        int64_t i_mb_count_size[2][7];
        int64_t i_mb_count_ref[2][16];
        /* */
        int     i_direct_score[2];
        int     i_direct_frames[2];
    } stat;
    /* CPU functions dependants */
    x264_predict_t      predict_16x16[4+3];//predict:预测
    x264_predict_t      predict_8x8c[4+3];
    x264_predict8x8_t   predict_8x8[9+3];
    x264_predict_t      predict_4x4[9+3];
    x264_pixel_function_t pixf;
    x264_mc_functions_t   mc;
    x264_dct_function_t   dctf;
    x264_csp_function_t   csp;//x264_csp_function_t是一个结构体，在common/csp.h中定义
    x264_quant_function_t quantf;//在common/quant.h中定义
    x264_deblock_function_t loopf;//在common/frame.h中定义
    /* vlc table for decoding purpose only */
    x264_vlc_table_t *x264_coeff_token_lookup[5];
    x264_vlc_table_t *x264_level_prefix_lookup;
    x264_vlc_table_t *x264_total_zeros_lookup[15];
    x264_vlc_table_t *x264_total_zeros_dc_lookup[3];
    x264_vlc_table_t *x264_run_before_lookup[7];
#if VISUALIZE
    struct visualize_t *visualize;
#endif
};
