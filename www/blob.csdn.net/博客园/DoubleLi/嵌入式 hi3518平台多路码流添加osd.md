# 嵌入式 hi3518平台多路码流添加osd - DoubleLi - 博客园







**[html]**[view plain](http://blog.csdn.net/skdkjzz/article/details/40429587)[copy](http://blog.csdn.net/skdkjzz/article/details/40429587)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <span style="font-family:Courier New;">  
- 
- /******************************************************************************  
- function : overlay process  
- 1) create some overlay regions  
- 2) display overlay regions ( One Region -- Multi-VencGroup )  
- 3) change all vencGroups Regions' positio  
- 4) change all vencGroups Regions' Layern  
- 5) change all vencGroups Regions' color  
- 6) load bmp form bmp-file to Region-0  
- 7) change all vencGroups Regions' front alpha  
- 8) change all vencGroups Regions' backgroud alpha  
- 9) update bitmap(not support now)  
- 10) show or hide overlay regions  
- 11) Detach overlay regions from chn  
- 12) Detroy overlay regions  
-  ******************************************************************************/  
- HI_S32 JOSEPH_SAMPLE_RGN_OverlayProcess(void)  
- {  
-     HI_S32 s32Ret = HI_FAILURE;  
-     RGN_HANDLE RgnHandle;  
-     RGN_ATTR_S stRgnAttr;  
-     MPP_CHN_S stChn;  
-     VENC_GRP VencGrp;  
-     RGN_CHN_ATTR_S stChnAttr;  
-     HI_U32 Joseph_u32Layer = 7;  
-     HI_U32 Joseph_bind_DevId = 0;   
-     int last_sec = 0;  
- 
-     /****************************************  
-       step 1: create overlay regions  
-      ****************************************/  
- stRgnAttr.enType = OVERLAY_RGN;  
- stRgnAttr.unAttr.stOverlay.enPixelFmt = PIXEL_FORMAT_RGB_1555;//PIXEL_FORMAT_RGB_565   
- stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 288;  
- stRgnAttr.unAttr.stOverlay.stSize.u32Height = 128;  
- stRgnAttr.unAttr.stOverlay.u32BgColor = 0xffffff;  
- 
- RgnHandle = 0;  
- 
- s32Ret = HI_MPI_RGN_Create(RgnHandle, &stRgnAttr);  
-     if(HI_SUCCESS != s32Ret)  
-     {  
-         SAMPLE_PRT("HI_MPI_RGN_Create (%d) failed with %#x!\n", \  
-             RgnHandle, s32Ret);  
-         return HI_FAILURE;  
-     }  
-     SAMPLE_PRT("the handle:%d,creat success!\n",RgnHandle);  
- 
- 
-     /*********************************************  
-       step 2: display overlay regions to venc groups  
-      *********************************************/  
- 
-     for(Joseph_bind_DevId = 0; Joseph_bind_DevId < joseph_ipnc_param.joseph_s32ChnNum; Joseph_bind_DevId++)  
-     {  
- 
- VencGrp = 0;  
- stChn.enModId = HI_ID_GROUP;  
- stChn.s32DevId = Joseph_bind_DevId;  
- stChn.s32ChnId = VencGrp;  
- RgnHandle = 0;  
- 
-         printf("%s:[%d] stChn.s32ChnId is %d ,\n",__FUNCTION__,__LINE__,stChn.s32ChnId);  
- 
-         memset(&stChnAttr,0,sizeof(stChnAttr));  
- stChnAttr.bShow = HI_TRUE;  
- stChnAttr.enType = OVERLAY_RGN;  
- 
- stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 16;  
- stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 32;  
- stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;  
- stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 0;  
- stChnAttr.unChnAttr.stOverlayChn.u32Layer = Joseph_u32Layer--;  
- 
- stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;  
- stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;  
- 
- stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 32;  
- stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width = 16;  
- stChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 64;  
- stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn = HI_TRUE;  
- stChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod = LESSTHAN_LUM_THRESH;  
- 
- s32Ret = HI_MPI_RGN_AttachToChn(RgnHandle, &stChn, &stChnAttr);  
-         if(HI_SUCCESS != s32Ret)  
-         {  
-             SAMPLE_PRT("HI_MPI_RGN_AttachToChn (%d) failed with %#x!\n",\  
-                 RgnHandle, s32Ret);  
-             return HI_FAILURE;  
-         }  
-         printf("display region to s32DevId %d chn success!\n",stChn.s32DevId);  
-     }  
- 
- 
-     /*********************************************  
-       step 3: show bitmap  
-      *********************************************/  
-     struct tm *tmnow;  
-     struct timeval tv;  
-     BITMAP_S Joseph_Osd_Bmp;  
-     gettimeofday(&tv,NULL);  
- 
-     setlocale(LC_ALL,"zh_CN.UTF-8");  
-     char osd_content[256] = {0};  
-     char week_chinese[16] = {0};  
- 
- tmnow = localtime(&tv.tv_sec);  
- 
-     TTF_Font *font;   
-     /* Initialize the TTF library */    
-     if ( TTF_Init() < 0 ) {    
-         return -1;    
-     }  
- 
- font = TTF_OpenFont(JOSEPH_FONT_FILE, JOSEPH_FONT_SIZE);    
-     if ( font == NULL ) {    
-         printf("%s %d The fonts is NULL !\n",__FUNCTION__,__LINE__);  
-         return -1;  
-     }  
- 
-     /*TTF_SetFontStyle(font,  TTF_STYLE_ITALIC);*/   
- 
-     while(1)  
-     {  
-         memset(osd_content,0,sizeof(osd_content));  
-         memset(week_chinese,0,sizeof(week_chinese));  
- 
-         while(1)  
-         {  
-             gettimeofday(&tv,NULL);  
-             if(tv.tv_sec != last_sec)  
-             {  
- last_sec = tv.tv_sec;  
-                 break;  
-             }  
-             usleep(20000);  
-         }  
- 
- tmnow = localtime(&tv.tv_sec);  
- 
-         #if 0  
- 
-         sprintf(osd_content,"%04d-%02d-%02d %02d:%02d:%02d %s ",\  
-             tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday,tmnow->tm_hour,\  
-             tmnow->tm_min, tmnow->tm_sec,Joseph_utf_weeky_show(tmnow->tm_wday,week_chinese));  
-         #else  
- 
-         sprintf(osd_content,"%04d-%02d-%02d %s %02d:%02d:%02d",\  
-             tmnow->tm_year+1900, tmnow->tm_mon+1, tmnow->tm_mday, Joseph_utf_weeky_show(tmnow->tm_wday,week_chinese),\  
-             tmnow->tm_hour,tmnow->tm_min, tmnow->tm_sec);  
- #if 0  
-         printf("%s:[%d] The time stamp is %s ... \n",__FUNCTION__,__LINE__,osd_content);  
- #endif  
-         joseph_get_luminance_from_sensor_test();  
-         #endif  
- 
-         #if 0  
- 
-         joseph_creat_bmp_data(font,osd_content);  
-         #else  
- 
-         joseph_creat_bmp_data_show(font,osd_content,&Joseph_Osd_Bmp);  
-         #endif  
-         for(RgnHandle = 0;RgnHandle < 1;RgnHandle++)  
-         {  
-             #if 0  
- s32Ret = HI_MPI_RGN_SetBitMap(RgnHandle,&stBitmap);  
-             #else  
- s32Ret = HI_MPI_RGN_SetBitMap(RgnHandle,&Joseph_Osd_Bmp);  
-             #endif  
-             if(s32Ret != HI_SUCCESS)  
-             {  
-                 SAMPLE_PRT("HI_MPI_RGN_SetBitMap failed with %#x!\n", s32Ret);  
-                 exit(0);  
-                 return HI_FAILURE;  
-             }  
- 
-             free(Joseph_Osd_Bmp.pData);               
- Joseph_Osd_Bmp.pData = NULL;  
- 
-         }  
- 
-     }  
- 
-     TTF_CloseFont(font);  
- font = NULL;  
-     TTF_Quit();   
- 
- #if 1  
-     /*********************************************  
-       step 4: destory region  
-      *********************************************/  
- 
-     for(Joseph_bind_DevId = 0; Joseph_bind_DevId < joseph_ipnc_param.joseph_s32ChnNum; Joseph_bind_DevId++)  
-     {  
- RgnHandle = 0;  
- VencGrp = 0;  
- stChn.enModId = HI_ID_GROUP;  
- stChn.s32DevId = Joseph_bind_DevId;  
- stChn.s32ChnId = VencGrp;  
- RgnHandle = 0;  
- 
- 
- s32Ret = HI_MPI_RGN_DetachFrmChn(RgnHandle, &stChn);  
-         if(HI_SUCCESS != s32Ret)  
-         {  
-             SAMPLE_PRT("HI_MPI_RGN_DetachFrmChn (%d) failed with %#x!\n",\  
-                    RgnHandle, s32Ret);  
-             return HI_FAILURE;  
-         }  
-     }  
- 
- RgnHandle = 0;  
- s32Ret = HI_MPI_RGN_Destroy(RgnHandle);  
-     if (HI_SUCCESS != s32Ret)  
-     {  
-         SAMPLE_PRT("HI_MPI_RGN_Destroy [%d] failed with %#x\n",\  
-             RgnHandle, s32Ret);  
-     }  
-     SAMPLE_PRT("destory all region success!\n");  
- #endif  
- 
-     return HI_SUCCESS;  
- }  
- </span>  



具体Demo代码可以下载：[http://download.csdn.net/detail/skdkjxy/8838309](http://download.csdn.net/detail/skdkjxy/8838309) 仅供参考，谢谢~






from:http://blog.csdn.net/skdkjzz/article/details/40429587










