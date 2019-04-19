# openh264  api 使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年11月27日 14:58:05[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：121
IS_PARAMETER_SET_NAL：是不是参数集nal
头文件codec_api.h codec_app_def.h codec_def.h codec_ver.h
SEncParamExt.iMultipleThreadIdc： lager than 1: count number of threads;
typedef enum {
CAMERA_VIDEO_REAL_TIME, ///< camera video for real-time communication
SCREEN_CONTENT_REAL_TIME, ///< screen content signal
CAMERA_VIDEO_NON_REAL_TIME,
SCREEN_CONTENT_NON_REAL_TIME,
INPUT_CONTENT_TYPE_ALL,
} EUsageType;
typedef enum {
SM_SINGLE_SLICE = 0, ///< | SliceNum==1
SM_FIXEDSLCNUM_SLICE = 1, ///< | according to SliceNum | enabled dynamic slicing for multi-thread
SM_RASTER_SLICE = 2, ///< | according to SlicesAssign | need input of MB numbers each slice. In addition, if other constraint in SSliceArgument is presented, need to follow the constraints. Typically if MB num and slice size are both constrained, re-encoding may be involved.
SM_SIZELIMITED_SLICE = 3, ///< | according to SliceSize | slicing according to size, the slicing will be dynamic(have no idea about slice_nums until encoding current frame)
SM_RESERVED = 4
} SliceModeEnum;
Step1:setup encoder
ISVCEncoder* encoder_;
int rv = WelsCreateSVCEncoder (&encoder_);
assert (rv == 0);
assert (encoder_ != NULL);
step2:initilize with basic parameter
SEncParamBase param;
memset (¶m, 0, sizeof (SEncParamBase));
param.iUsageType = usageType; //from EUsageType enum
param.fMaxFrameRate = frameRate;
param.iPicWidth = width;
param.iPicHeight = height;
param.iTargetBitrate = 5000000;
encoder_->Initialize (¶m);
或
Step 2:initialize with extension parameter
SEncParamExt param;
encoder_->GetDefaultParams (¶m);
param.iUsageType = usageType;
param.fMaxFrameRate = frameRate;
param.iPicWidth = width;
param.iPicHeight = height;
param.iTargetBitrate = 5000000;
param.bEnableDenoise = denoise;
param.iSpatialLayerNum = layers;
//SM_DYN_SLICE don't support multi-thread now
if (sliceMode != SM_SINGLE_SLICE && sliceMode != SM_DYN_SLICE)
param.iMultipleThreadIdc = 2;
for (int i = 0; i < param.iSpatialLayerNum; i++) {
param.sSpatialLayers[i].iVideoWidth = width >> (param.iSpatialLayerNum - 1 - i);
param.sSpatialLayers[i].iVideoHeight = height >> (param.iSpatialLayerNum - 1 - i);
param.sSpatialLayers[i].fFrameRate = frameRate;
param.sSpatialLayers[i].iSpatialBitrate = param.iTargetBitrate;
param.sSpatialLayers[i].sSliceCfg.uiSliceMode = sliceMode;
if (sliceMode == SM_DYN_SLICE) {
param.sSpatialLayers[i].sSliceCfg.sSliceArgument.uiSliceSizeConstraint = 600;
param.uiMaxNalSize = 1500;
}
}
param.iTargetBitrate *= param.iSpatialLayerNum;
encoder_->InitializeExt (¶m);
int videoFormat = videoFormatI420;
encoder_->SetOption (ENCODER_OPTION_DATAFORMAT, &videoFormat);
Step3:set option, set option during encoding process
encoder_->SetOption (ENCODER_OPTION_TRACE_LEVEL, &g_LevelSetting);
int videoFormat = videoFormatI420;
encoder_->SetOption (ENCODER_OPTION_DATAFORMAT, &videoFormat);
//changing encoding param SEncParamBase param; //fill in instant param即时参数content encoder_->SetOption (ENCODER_OPTION_SVC_ENCODE_PARAM_BASE, ¶m);
Step4: encode and store ouput bistream
int frameSize = width * height * 3 / 2;
BufferedData buf;
buf.SetLength (frameSize);
assert (buf.Length() == (size_t)frameSize);
SFrameBSInfo info;
memset (&info, 0, sizeof (SFrameBSInfo));
SSourcePicture pic;
memset (&pic, 0, sizeof (SsourcePicture));
pic.iPicWidth = width;
pic.iPicHeight = height;
pic.iColorFormat = videoFormatI420;
pic.iStride[0] = pic.iPicWidth;
pic.iStride[1] = pic.iStride[2] = pic.iPicWidth >> 1;
pic.pData[0] = buf.data();
pic.pData[1] = pic.pData[0] + width * height;
pic.pData[2] = pic.pData[1] + (width * height >> 2);
for(int num = 0;num<total_num;num++) {
//prepare input data
rv = encoder_->EncodeFrame (&pic, &info);
assert (rv == cmResultSuccess);
if (info.eFrameType != videoFrameTypeSkip) {
//output bitstream handling
}
}
Step5:teardown encoder
if (encoder_) {
encoder_->Uninitialize();
WelsDestroySVCEncoder (encoder_);
}
by [https://github.com/starrtc/android-demo](https://github.com/starrtc/android-demo)
