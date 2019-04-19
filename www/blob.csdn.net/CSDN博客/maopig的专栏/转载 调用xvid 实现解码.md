# 转载 调用xvid 实现解码 - maopig的专栏 - CSDN博客
2011年09月16日 18:43:52[maopig](https://me.csdn.net/maopig)阅读数：1718
2011-06-01 00:26:14)
![](http://simg.sinajs.cn/blog7style/images/common/sg_trans.gif)转载
view plaincopy to clipboardprint?
///////////////////  
intinit_decoder()   
{  
    intret;  
   xvid_gbl_init_t  xvid_gbl_init;  
   xvid_dec_create_txvid_dec_create;  
   memset(&xvid_gbl_init, 0,sizeof(xvid_gbl_init_t));  
   memset(&xvid_dec_create, 0,sizeof(xvid_dec_create_t));  
    XDIM = 0;   
    YDIM = 0;   
    FORMAT = 0;   
    CSP =XVID_CSP_BGR;  
    BPP =3;  
    dec_handle =NULL ;  
    mp4_buffer =NULL ;   
    out_buffer =NULL ;   
    mp4_ptr =NULL ;   
   xvid_gbl_init.version =XVID_VERSION;  
   xvid_gbl_init.cpu_flags = 0;  // use assemblyoptimized   
   xvid_gbl_init.debug = 0;    // set debug level 0, no debugoutput   
   xvid_global(NULL, 0, &xvid_gbl_init, NULL);//XVID初始化  
   xvid_dec_create.version =XVID_VERSION;  
   xvid_dec_create.width = 0;//解码后图像的宽度 如果事先已经知道那么这里就填写 否则设置0  
   xvid_dec_create.height = 0;//解码后图像的高度 如果事先已经知道那么这里就填写否则设置0  
    ret =xvid_decore(NULL, XVID_DEC_CREATE,&xvid_dec_create, NULL);//创建xvid解码实例  
    dec_handle =xvid_dec_create.handle;////将创建的句柄 赋值 给dec_handle  
   return(ret);  
}  
//////////////////////////////////////////////////////////////////  
int decode_one_frame(unsigned char *istream, unsigned char*ostream,  
                                     int istream_size, xvid_dec_stats_t*xvid_dec_stats)  
{  
    intret;  
   xvid_dec_frame_txvid_dec_frame;  
   memset(&xvid_dec_frame, 0,sizeof(xvid_dec_frame_t));  
   memset(xvid_dec_stats, 0,sizeof(xvid_dec_stats_t));  
