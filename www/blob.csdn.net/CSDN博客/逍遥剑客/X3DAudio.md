# X3DAudio - 逍遥剑客 - CSDN博客
2008年11月26日 15:49:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：3326标签：[vector																[distance																[null																[float																[matrix																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=matrix&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=distance&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[视频音频](https://blog.csdn.net/xoyojank/article/category/484788)
**概述**
X3DAudio是与XAuido2和XACT在3D空间中协作的API. 你可以用它创建出一个声音从一个点飞到摄像机处的效果. 音频不需要有3D位置, 这些都是通过X3DAudio来完成的
**收听者和发射器**
- X3DAudio使用收听者(listener)和发射器(emitter)来管理3D空间中的声音. 收听者和发射器别表示收听声音和音源的位置. 
一个收听者定义为空间中的一个带朝向和收听位置的点. 一般来说位置和朝向是跟摄像机一样的, 不管你是第一人称还是第三人称视角. 收听者的位置用世界坐标来表示, 不过要注意收听者跟发射器的相对位置才决定最终计算出的扬声器音量. 
- 一个发射器定义为空间中的一个(或多个)带有音源的点. 发射器的位置可以在3D空间中的任何地方. 跟收听者一样, 发射器的位置也用世界坐标表示. 
除了位置外, 收听者跟发射器还可以包含速度. 跟3D渲染引擎不一样, X3DAudio只使用速度来计算多普勒效应(不需要计算位置). 
**整合X3DAudio和XACT**
XACT可以利用X3DAudio来支持3D声音输出. 下面我们来看一下怎么用X3DAudio和相关的XACT辅助函数来加入3D的声音.
**简介**
X3DAudio库是一个3D声音几何库, 它提供了收听者和发射器的声音位置的计算功能. XACT使用X3DAudio把3D位置信息应用到Cue和它们的相关的播放, 这样用户才能在单体声或5.1声道下听到真实的定位效果. 
XACT同样也使用3D信息来设置cue的变量值, 因此声音设计师可以创建基于声音位置的效果, 例如基于距离的音量衰减等. 
X3DAudio是一个严格的3D数学库, 并不包含信号处理功能. 为了把声音位置信息方便地整合X3DAudio库到XACT的cue,  XACT提供了两个辅助函数:
XACT3DCalculate, 计算收听者和发射器之间的位置信息
XACT3DApply, 应用上面计算的结果到一个cue, 这样播放起来才有位置效果
要使用X3DAudio把3D声音应用到XACT的cue, 按下面的步骤来做:/
- 初始化X3DAudio结构
- 设置发射器/收听者
- 调用XACT3DCalculate和XACT3DApply
**初始化X3DAudio结构**
要计算3D位置信息, 必须提供给X3DAudio相关的初始化数据: 声音发射器的声道数和声音收听者的声道数. 这两个值保存在X3DAUDIO_DSP_SETTINGS结构体中的SrcChannelCount和DstChannelCount中, 它们决定了计算结果的输出格式(立体声还是5.1声道). 
在数量设置后必须分配一个矩阵系数. 矩阵要足够大以保存从源声道到目标声道的映射信息. 这个矩阵是一个一维的, 并且必须包含SrcChannelCount x DstChannelCount数量的元素. 你必须手动的调用XACT3DCalculate来填充矩阵的值. 
下面的代码配置了一个从单声道或立体声来源映射到立体声或5.1声道的X3DAUDIO_DSP_SETTINGS结构体: 
UINT32 srcChannelCount;  // number of points on the emitter. (DSPSettings.SrcChannelCount) 
UINT32 speakerconfig;   // either SPEAKER_STEREO or SPEAKER_5POINT1 (Should match the speaker configuration of the user's system) 
// Set up DSP settings based on your desired speaker configuration and emitter channel count 
X3DAUDIO_DSP_SETTINGS DSPSettings;
// Set source channel and matrix coeeficients 
DSPSettings.SrcChannelCount = srcChannelCount;
                                    
switch( speakerconfig )
{
 case SPEAKER_STEREO: 
  //Set destination 
  //Destination channels must be 2 for stereo 
  DSPSettings.DstChannelCount = 2;    // must match number of channels of SpeakerChannelMask sent to XACT3DCalculate 
  DSPSettings.pMatrixCoefficients = new FLOAT32[DSPSettings.SrcChannelCount * DSPSettings.DstChannelCount];
  break;
 case SPEAKER_5POINT1: 
  //Set destination 
  //Destination channels must be 6 for 5.1 sound 
  DSPSettings.DstChannelCount = 6;  // must match number of channels of SpeakerChannelMask sent to XACT3DCalculate 
  DSPSettings.pMatrixCoefficients = new FLOAT32[DSPSettings.SrcChannelCount * DSPSettings.DstChannelCount];
  
 default:
  // HANDLE FAILURE 
  break;
} 
注意这些代码只处理1或2声道的来源. 更复杂的情况就是, 源音频可能是5.1格式, 所以源声道数为6, 增加了矩阵的大小. 
**设置发射器/收听者**
X3DAudio的功能是计算收听者跟发射器间的位置信息. X3DAudio的XACT实现在每次调用XACT3DCalculate时使用了一个发射器和收听者. 
首先你要提供填充发射器和接收者的位置信息, 下面的代码设置了一个简单的发射器和接收者:
// generic listener that's not moving 
X3DAUDIO_LISTENER listener = {
        Z_AXIS_VECTOR,      // z axis {0,0,1}, orient front 
        Y_AXIS_VECTOR,      // y axis {0,1,0}, orient top 
        POSITION_VECTOR,    // user defined vector for position 
        ZERO_VECTOR };      // {0,0,0}, listener not moving 
// Audio cone.  Ignored in multi-point emitters (channel count > 1) 
X3DAUDIO_CONE cone = {  X3DAUDIO_PI/2.0f,   // InnerAngle 
                            X3DAUDIO_PI,        // OuterAngle 
                            2.0f,               // InnerVolume 
                            1.0f,               // OuterVolume 
                            0.0f,               // InnerLPF 
                            0.0f,               // OuterLPF 
                            0.0f,               // InnerReverb 
                            0.0f };             // OuterReverb 
    
    
//set up emitter 
X3DAUDIO_EMITTER emitter;
    
emitter.pCone = &cone;
// the following need to be orthonormal 
emitter.OrientFront = Z_AXIS_VECTOR;
emitter.OrientTop = Y_AXIS_VECTOR;
emitter.Position = ZERO_VECTOR;
emitter.Velocity = ZERO_VECTOR; // needs to not be zero if you want to hear Doppler effect 
emitter.ChannelCount = pDSPSettings->SrcChannelCount;   // emitter ChannelCount and DSP Setting's SrcChannelCount must match 
emitter.ChannelRadius = 0.0f;   // this will be set by XACT3DCalculate if ChannelCount > 1. 
// will be set by XACT3DCalculate 
emitter.pChannelAzimuths = NULL;
// will be set by XACT3DCalculate 
emitter.pVolumeCurve = emitter.pLFECurve
        = emitter.pLPFDirectCurve
        = emitter.pLPFReverbCurve
        = emitter.pReverbCurve
        = NULL;
    
emitter.CurveDistanceScaler = FLT_MIN;
emitter.DopplerScaler = 1.0f; 
注意一些发射器的一些字段设为NULL来使用默认值, 还有一些由XACT3DCalculation来提供. 
上面用到的X3DAUDIO_CONE结构体是可选的, 在一个单声道的发射器上, 设置pCone的值为NULL的话表示它是一个全方向的发射器. 
**调用XACT3DCalculate和XACT3DApply**最后的步骤就是调用XACT3DCalculation来计算最终的声道映射, 然后调用XACT3DApply把这个映射应用到cue, 还有复制3D信息(如距离)到cue的固有变量中. 
下面的代码假设XACT已经初始化, 并且cue存在:
// call calculate and apply.  It doesn't matter if the cue is already playing or just prepared. 
hr = XACT3DCalculate(speakerconfig, &listener, &emitter, &DSPSettings, g_pEngine);
if(S_OK == hr)
{
 hr = XACT3DApply( &DSPSettings, pCue);
} 
固有变量设置
当XACT3DApply调用成功后, XACT会更新目标cue的一些固有变量:
|变量|意义|
|----|----|
|Distance|发射器和收听者之间的距离. 相当于XACT3DCalculate返回的X3DAUDIO_DSP_SETTINGS.EmitterToListenerDistance .|
|OrientationAngle|从发射器到收听者的内角, 表示了发射器在前方多少度. 相当XACT3DCalculate返回的X3DAUDIO_DSP_SETTINGS.EmitterToListenerAngle, 转换成角度.|
|DopplerPitchScalar|发射器和收听者之间的多普勒变换因数. 相当XACT3DCalculate返回的X3DAUDIO_DSP_SETTINGS.DopplerFactor .|
如果XACT的3D功能在实际产品中应用的话, 音效设计师可以使用RPCs(Runtime Parameter Controls)来改变相关的固有变量的值, 如音量和音调.             
