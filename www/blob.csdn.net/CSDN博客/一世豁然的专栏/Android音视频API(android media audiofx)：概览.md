# Android音视频API(android.media.audiofx)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月06日 10:25:01[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2052








提供管理媒体框架中实现的音频效果的类。




一、接口

[AudioEffect.OnControlStatusChangeListener](https://developer.android.google.cn/reference/android/media/audiofx/AudioEffect.OnControlStatusChangeListener.html)


OnControlStatusChangeListener接口定义了一个由效果引擎的控制获得或丢失的AudioEffect调用的方法





[AudioEffect.OnEnableStatusChangeListener](https://developer.android.google.cn/reference/android/media/audiofx/AudioEffect.OnEnableStatusChangeListener.html)


OnEnableStatusChangeListener接口定义了当控件应用程序更改效果引擎的启用状态时由AudioEffect调用的方法。





[BassBoost.OnParameterChangeListener](https://developer.android.google.cn/reference/android/media/audiofx/BassBoost.OnParameterChangeListener.html)


OnParameterChangeListener接口定义了一个参数值改变时由BassBoost调用的方法。





[EnvironmentalReverb.OnParameterChangeListener](https://developer.android.google.cn/reference/android/media/audiofx/EnvironmentalReverb.OnParameterChangeListener.html)


OnParameterChangeListener接口定义了一个参数值更改时由EnvironmentReverb调用的方法。





[Equalizer.OnParameterChangeListener](https://developer.android.google.cn/reference/android/media/audiofx/Equalizer.OnParameterChangeListener.html)


OnParameterChangeListener接口定义了当参数值发生变化时均衡器调用的方法。





[PresetReverb.OnParameterChangeListener](https://developer.android.google.cn/reference/android/media/audiofx/PresetReverb.OnParameterChangeListener.html)


OnParameterChangeListener接口定义了当参数值发生变化时PresetReverb调用的方法。





[Virtualizer.OnParameterChangeListener](https://developer.android.google.cn/reference/android/media/audiofx/Virtualizer.OnParameterChangeListener.html)


当参数值已更改时，OnParameterChangeListener接口定义了由Virtualizer调用的方法。





[Visualizer.OnDataCaptureListener](https://developer.android.google.cn/reference/android/media/audiofx/Visualizer.OnDataCaptureListener.html)


OnDataCaptureListener接口定义了Visualizer调用的方法来定期更新音频可视化捕获。








二、类

[AcousticEchoCanceler](https://developer.android.google.cn/reference/android/media/audiofx/AcousticEchoCanceler.html)


声学回波消除器（AEC）。





[AudioEffect](https://developer.android.google.cn/reference/android/media/audiofx/AudioEffect.html)


AudioEffect是用于控制由android音频框架提供的音频效果的基础。





[AudioEffect.Descriptor](https://developer.android.google.cn/reference/android/media/audiofx/AudioEffect.Descriptor.html)


效果描述符包含有关在音频框架中实现的特定效果的信息：


type：UUID标识效果类型。





[AutomaticGainControl](https://developer.android.google.cn/reference/android/media/audiofx/AutomaticGainControl.html)


自动增益控制（AGC）。





[BassBoost](https://developer.android.google.cn/reference/android/media/audiofx/BassBoost.html)


低音提升是提升或放大低频声音的音效。





[BassBoost.Settings](https://developer.android.google.cn/reference/android/media/audiofx/BassBoost.Settings.html)


Settings类重新分组所有的低音boost参数。





[EnvironmentalReverb](https://developer.android.google.cn/reference/android/media/audiofx/EnvironmentalReverb.html)


在房间内产生的声音在许多方向行进。





[EnvironmentalReverb.Settings](https://developer.android.google.cn/reference/android/media/audiofx/EnvironmentalReverb.Settings.html)


“设置”类重新分组所有环境混响参数。





[Equalizer](https://developer.android.google.cn/reference/android/media/audiofx/Equalizer.html)


均衡器用于改变特定音乐源或主输出组合的频率响应。





[Equalizer.Settings](https://developer.android.google.cn/reference/android/media/audiofx/Equalizer.Settings.html)


Settings类重新分组所有均衡器参数。





[LoudnessEnhancer](https://developer.android.google.cn/reference/android/media/audiofx/LoudnessEnhancer.html)


LoudnessEnhancer是增加音频响度的音频效果。





[NoiseSuppressor](https://developer.android.google.cn/reference/android/media/audiofx/NoiseSuppressor.html)


噪音抑制器（NS）。





[PresetReverb](https://developer.android.google.cn/reference/android/media/audiofx/PresetReverb.html)


在房间内产生的声音在许多方向行进。





[PresetReverb.Settings](https://developer.android.google.cn/reference/android/media/audiofx/PresetReverb.Settings.html)


“设置”类重新分组所有预设的混响参数。





[Virtualizer](https://developer.android.google.cn/reference/android/media/audiofx/Virtualizer.html)


音频虚拟器是影响音频通道空间化的通用名称。





[Virtualizer.Settings](https://developer.android.google.cn/reference/android/media/audiofx/Virtualizer.Settings.html)


Settings类重新分组所有虚拟机参数。





[Visualizer](https://developer.android.google.cn/reference/android/media/audiofx/Visualizer.html)


Visualizer类允许应用程序检索当前正在播放的音频的一部分用于可视化目的。





[Visualizer.MeasurementPeakRms](https://developer.android.google.cn/reference/android/media/audiofx/Visualizer.MeasurementPeakRms.html)


存储峰值和RMS值的类。




