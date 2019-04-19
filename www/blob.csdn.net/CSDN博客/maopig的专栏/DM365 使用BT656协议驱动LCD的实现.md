# DM365 使用BT656协议驱动LCD的实现 - maopig的专栏 - CSDN博客
2011年12月31日 16:19:03[maopig](https://me.csdn.net/maopig)阅读数：3588
   前两天已经调好了，主要是对davinci_platform.c的修改
   因为输入输出都为pal的制式，所以就在pal的函数中进行了修改。
  在PAL设置的函数中，修改如下：
/*
 * setting PAL mode
 */
static void davinci_enc_set_pal(struct vid_enc_mode_info *mode_info)
{
       enableDigitalOutput(0); //gjx orignal : 0
       if (cpu_is_davinci_dm355()) {
              dispc_reg_out(VENC_CLKCTL, 0x1);
              dispc_reg_out(VENC_VIDCTL, 0);
              /* DM350 Configure VDAC_CONFIG  */
              davinci_writel(0x0E21A6B6, DM3XX_VDAC_CONFIG);
       } else if (cpu_is_davinci_dm365()) {
              dispc_reg_out(VENC_VMOD,0x1043);//gjx  1:YCC8; 0: ;4:PAL; 3:Composite output enable.Video encoder enable.
              dispc_reg_out(VENC_CLKCTL, 0x11); //gjx enable digital lcd clock;orignal:0x1
              dispc_reg_out(VENC_VIDCTL, 0x6000); //gjx  vclk pin output enable & vclk polarity inverse(6000)
              dispc_reg_out(VENC_YCCCTL, 0x1); //gjx bt656
              /* Set OSD clock and OSD Sync Adavance registers */
              dispc_reg_out(VENC_OSDCLK0, 1); //gjx 
              dispc_reg_out(VENC_OSDCLK1, 2); //gjx
dispc_reg_out(VENC_VDPRO, 0x20);//xjx
              davinci_writel(0x081141CF, DM3XX_VDAC_CONFIG);
       } else {
              /* to set VENC CLK DIV to 1 - final clock is 54 MHz */
              dispc_reg_merge(VENC_VIDCTL, 0, 1 << 1);
              /* Set REC656 Mode */
              dispc_reg_out(VENC_YCCCTL, 0x1);
       }
       dispc_reg_out(VENC_SYNCCTL,0);//gjx
       if (cpu_is_davinci_dm355()) {
              davinci_writel(mode_info->left_margin,
                            (DM355_OSD_REG_BASE + OSD_BASEPX));
              davinci_writel(mode_info->upper_margin,
                            (DM355_OSD_REG_BASE + OSD_BASEPY));
       } else if (cpu_is_davinci_dm365()) {
              davinci_writel(mode_info->left_margin,
                            (DM365_OSD_REG_BASE + OSD_BASEPX));
              /* PAL display shows shakiness in the OSD0 when
               * this is set to upper margin. Need to bump it
               * by 2
               */
              davinci_writel((mode_info->upper_margin + 2),
                            (DM365_OSD_REG_BASE + OSD_BASEPY));
       } else {
              davinci_writel(mode_info->left_margin,
                            (DM644X_OSD_REG_BASE + OSD_BASEPX));
              davinci_writel(mode_info->upper_margin,
                            (DM644X_OSD_REG_BASE + OSD_BASEPY));
       }
       dispc_reg_merge(VENC_VMOD, VENC_VMOD_VENC, VENC_VMOD_VENC);
       dispc_reg_out(VENC_DACTST, 0x0);
    dispc_reg_out(VENC_CMPNT, 0);
}
环境变量
setenv bootargs mem=70M console=ttyS0,115200n8 root=/dev/nfs rw nfsroot=192.168.0.168:/home/gjx/workdir/filesys ip=192.168.0.3:192.168.0.168:192.168.0.1:255.255.255.0::eth0:off eth=00:40:01:2B:64:60 video=davincifb:vid0=OFF:vid1=OFF:osd0=720x576x16,4050K
 dm365_imp.oper_mode=0 davinci_capture.device_type=1 davinci_enc_mngr.ch0_mode=pal
这样重新编译内核就直接可以输出bt656的视频了，直接接支持BT656的LCD就显示成功了。
