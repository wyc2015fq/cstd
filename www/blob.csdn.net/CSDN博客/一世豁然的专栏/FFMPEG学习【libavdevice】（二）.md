# FFMPEG学习【libavdevice】（二） - 一世豁然的专栏 - CSDN博客





2017年06月23日 11:17:32[一世豁然](https://me.csdn.net/Explorer_day)阅读数：772









|unsigned|[avdevice_version](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga6a111710c7b02c2a11d8b6c43e0dd63d) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|
返回LIBAVDEVICE_VERSION_INT常量。





|const char *|[avdevice_configuration](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga84490e9144a7c9311a3df5cc2a66e0c6) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libavdevice构建时配置。





|const char *|[avdevice_license](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga146d7576da658d0fb388aa89d563bcfa) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libavdevice许可证。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avdevice_register_all](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga7c90a3585267b55941ae2f7388c006b6) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

初始化libavdevice并注册所有输入和输出设备。





|[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *|[av_input_audio_device_next](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga521e86d5a7e1f96f894edbecda3d6970) ([AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *d)|
|----|----|

音频输入设备迭代器。

如果d为NULL，则返回第一个注册的输入音频/视频设备，如果d为非空，则在d之后返回下一个已注册的输入音频/视频设备，如果d为最后一个则返回NULL。






|[AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *|[av_input_video_device_next](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga4f4b2a42fa434a3557b96a02ba7cd36a) ([AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *d)|
|----|----|

视频输入设备迭代器。

如果d为NULL，则返回第一个注册的输入音频/视频设备，如果d为非空，则在d之后返回下一个已注册的输入音频/视频设备，如果d为最后一个则返回NULL。






|[AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *|[av_output_audio_device_next](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gaf79c3b9e53b5631f422f7b92768c5164) ([AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *d)|
|----|----|

音频输出设备迭代器。

如果d为NULL，则返回第一个注册的输出音频/视频设备，如果d为非NULL，则在d之后返回下一个已注册的输出音频/视频设备，如果d为最后一个则返回NULL。






|| |
|----|----|
|[AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *|[av_output_video_device_next](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga52f3b793ab7b9511521069cf6213ebd2) ([AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *d)|

视频输出设备迭代器。

如果d为NULL，则返回第一个注册的输出音频/视频设备，如果d为非NULL，则在d之后返回下一个已注册的输出音频/视频设备，如果d为最后一个则返回NULL。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avdevice_app_to_dev_control_message](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga62f5a22de09f0bc7fd2380d39ee1bc45) (struct [AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), enum [AVAppToDevMessageType](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga16d549a25f11693a06a211f00864fed8)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), size_t data_size)|
|----|----|

从应用程序发送控制消息到设备。

参数：s：设备上下文

    type：消息类型

    data：消息数据。 精确类型取决于消息类型。

data_size：消息数据的大小。

返回：> = 0成功，否定错误。 当设备没有实现消息的处理程序时，AVERROR（ENOSYS）。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avdevice_dev_to_app_control_message](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gaf426838f78fb5bcdc2095c4d3162cf6e) (struct [AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), enum [AVDevToAppMessageType](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gac379d3691ce091da6387a461a158a690)[type](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a7d05960f4f1c1b11f3177dc963a45d86), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[data](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), size_t data_size)|
|----|----|

发送控制消息从设备到应用程序。

参数：s：设备上下文

    type：消息类型

    data：消息数据。 可以为NULL。

data_size：消息数据的大小。

返回：> = 0成功，否定错误。
 当应用程序不实现消息的处理程序时，AVERROR（ENOSYS）。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avdevice_capabilities_create](http://ffmpeg.org/doxygen/trunk/group__lavd.html#gaf802f1388f3f209cbbc93d9ba983a341) ([AVDeviceCapabilitiesQuery](http://ffmpeg.org/doxygen/trunk/structAVDeviceCapabilitiesQuery.html) **caps, [AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **device_options)|
|----|----|

基于AVOption API初始化能力探测API。

当不再使用查询功能API时，必须调用avdevice_capabilities_free（）。


参数：caps：设备功能数据。指向NULL指针的指针必须传递。

    s：设备上下文

device_options：一个填充了设备私有选项的AVDictionary。
 返回时，此参数将被销毁，并替换为包含未找到的选项的dict。 可能为NULL。 稍后必须将相同的选项传递给输出设备的avformat_write_header（）或输入设备的avformat_open_input（），或影响设备私有选项的任何其他位置。

返回：> = 0成功，否则为否。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avdevice_capabilities_free](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga89c36d6e5698ad7dc2637226e1655dff) ([AVDeviceCapabilitiesQuery](http://ffmpeg.org/doxygen/trunk/structAVDeviceCapabilitiesQuery.html) **caps, [AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

释放由avdevice_capabilities_create（）创建的免费资源。

参数：caps：设备功能数据被释放。

    s：设备上下文





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avdevice_list_devices](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga4bf9cc38ae904b9104fda1e4def71474) (struct [AVFormatContext](http://ffmpeg.org/doxygen/trunk/structAVFormatContext.html) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [AVDeviceInfoList](http://ffmpeg.org/doxygen/trunk/structAVDeviceInfoList.html) **device_list)|
|----|----|

列出设备

返回可用的设备名称及其参数。


注意：某些设备可能会接受不能自动检测的依赖于系统的设备名称。 该函数返回的列表不能被假定为始终完成。

参数：s：设备上下文

device_list：自动检测设备列表。

返回：计数自动检测设备，否定错误。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avdevice_free_list_devices](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga52dcbb2d9ae0f33b7a89548b5a0c87bd) ([AVDeviceInfoList](http://ffmpeg.org/doxygen/trunk/structAVDeviceInfoList.html) **device_list)|
|----|----|

便捷函数可以释放avdevice_list_devices（）的结果。

参数：device_list：设备列表被释放。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avdevice_list_input_sources](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga202488c6f1ed7e87b659c7b8df079e50) (struct [AVInputFormat](http://ffmpeg.org/doxygen/trunk/structAVInputFormat.html) *device, const char *device_name, [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *device_options, [AVDeviceInfoList](http://ffmpeg.org/doxygen/trunk/structAVDeviceInfoList.html) **device_list)|
|----|----|

列表设备

返回可用的设备名称及其参数。 这些是avdevice list_devices（）的方便的包装器。 设备上下文在内部被分配和释放。


参数：device：设备格式。 如果设备名称被设置，可能为NULL。

device_name：设备名称。 如果设置格式被设置，可能为NULL。

device_options：一个填充了设备私有选项的AVDictionary。
 可能为NULL。 稍后必须将相同的选项传递给输出设备的avformat_write_header（）或输入设备的avformat_open_input（），或影响设备私有选项的任何其他位置。

device_list：自动检测设备列表

返回：计数自动检测设备，否定错误。

注意：当两个设置都设置参数时，设备参数优先于device_name。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avdevice_list_output_sinks](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga2227616998276fefe5e4221e478d667f) (struct [AVOutputFormat](http://ffmpeg.org/doxygen/trunk/structAVOutputFormat.html) *device, const char *device_name, [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *device_options, [AVDeviceInfoList](http://ffmpeg.org/doxygen/trunk/structAVDeviceInfoList.html) **device_list)|
|----|----|








二、变量


|const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html)|[av_device_capabilities](http://ffmpeg.org/doxygen/trunk/group__lavd.html#ga1362af8ea203781c69bc644179388259) []|
|----|----|

设备使用的AVOption表来实现设备功能API。

不应该被用户使用




