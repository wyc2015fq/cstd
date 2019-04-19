# 获取与设置windows系统下音频设备音量 - yixianfeng41的专栏 - CSDN博客
2016年07月22日 01:09:10[第2梦](https://me.csdn.net/yixianfeng41)阅读数：6046
所属专栏：[C++编程](https://blog.csdn.net/column/details/14514.html)
## 一、获取设备音量
不同系统获取音量的方法是有差别的，比如vista和win7获取音量的方式就是不同的，因此，我们应该首先获取系统的信息，然后根据系统信息，用其对应的方法获取音量。
### 1、OSVERSIONINFO
系统信息结构体
以下结构体均用于检索或设置系统信息。
```cpp
ANIMATIONINFO
HW_PROFILE_INFO
MINIMIZEDMETRICS
NONCLIENTMETRICS
OSVERSIONINFO
SYSTEM_INFO
```
使用说明
       在OSVERSIONINFO数据结构中包含操作系统版本信息。这些信息包括主要和次要版本号，一个内部版本号，一个平台标识符，以及操作系统的说明性文本。GetVersionEx函数将使用这个结构体。
结构体原型：
```cpp
typedef struct _OSVERSIONINFO
{
    DWORD dwOSVersionInfoSize;
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD dwBuildNumber;
    DWORD dwPlatformId;
    TCHAR szCSDVersion[ 128 ];
} OSVERSIONINFO;
```
成员说明：
1）dwOSVersionInfoSize
       以字节为单位，指定这个数据结构的大小。须在调用GetVersionEx（OSVERSIONINFO）函数之前，将这个成员设置为sizeof(OSVERSIONINFO)。
2）dwMajorVersion
       标识操作系统的主版本号。例如，对于Windows NT 3.51版，其主版本号为3；对于Windows NT 4.0版，其主版本号为4。
3）dwMinorVersion
       标识操作系统的次要版本号。例如，对于Windows NT 3.51版，其次要版本号为51；对于Windows NT 4.0版本，其次要版本号为0。
4）dwBuildNumber
       Windows NT：标识操作系统的内部版本号。
Windows 95：低位字标识操作系统数的内部版本号。高位字包含了主要和次要版本号。
5）dwPlatformId
       标识操作系统平台。这个成员可以是下列值之一：
6）szCSDVersion
       Windows NT：包含一个以NULL结尾的字符串，如“Service Pack 3”，就表明系统上安装了最新的Service Pack。如果没有安装Service Pack，该字符串为空。
     Windows 95：包含一个以NULL结尾的字符串，提供了对操作系统的任意补充信息。
应用：
```cpp
OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx( &osVer );
	if( osVer.dwMajorVersion==6 )
	{
        GetAudioOuputVolum_Vita();
    }
    else
    {
        GetAudioOuputVolum_Win7();
    }
```
### 2、vista下获取设备音量
```cpp
int GetAudioOutputVolumn_Vista(const std::string &deviceName)
{
    IMMDeviceEnumerator* pEnumerator;
    IMMDeviceCollection* pCollection = NULL;
    IMMDevice *pDevice = NULL;
    IPropertyStore *pProperties=NULL;  
    IAudioEndpointVolume *pVolumeAPI=NULL;  
    UINT deviceCount = 0;
    float fVolume = -1;
    CoInitializeEx( NULL , COINIT_MULTITHREADED );
    HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
    if (hr != S_OK)  
    {  
        printf("CoCreateInstance Failed!\n");  
        return 0;  
    }  
    //hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
    hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATE_ACTIVE , &pCollection );
    if (hr != S_OK)  
    {  
        printf("EnumAudioEndpoints Failed!\n");  
        goto releasepEnumerator;  
    }  
    hr = pCollection->GetCount(&deviceCount);  
    if (hr != S_OK)  
    {  
        printf("GetCount Failed!\n");  
        goto releasepCollection;  
    }  
    for (UINT dev=0;dev<deviceCount;dev++)  
    {  
        pDevice = NULL;  
        hr = pCollection->Item(dev,&pDevice);  
        if (hr == S_OK)  
        {  
            hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);  
            if (hr == S_OK)  
            {  
                PROPVARIANT varName;  
                PropVariantInit(&varName);  
                hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName); 
                if( SUCCEEDED(hr) )
                {
                    if (varName.vt != VT_LPWSTR || 
                        (wstr2str(varName.pwszVal).find(deviceName) != 0)) //传入的值可能不完整，需要前部分匹配
                    {
                        continue;
                    }
                    hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));  
                    if (hr==S_OK)  
                    {  
                        hr = pVolumeAPI->GetMasterVolumeLevelScalar( &fVolume );
                        if (S_OK == hr) break;
                    } 
                }
                SAFE_RELEASE(pProperties);  
            }  
            SAFE_RELEASE(pDevice);  
        }  
    }  
releasepCollection:  
    SAFE_RELEASE(pCollection);  
releasepEnumerator:  
    SAFE_RELEASE(pEnumerator);  
    return  fVolume*100;
}
```
### 3、win7下获取设备音量
windows下主要有这几个API:
> 
mixerOpen和mixerClose函数用来打开和关闭混音器设备 
mixerGetNumDevs可以确定系统中有多少混音器设备 
mixerGetDevCaps函数可以确定混音器设备的能力 
mixerGetLineInfo可以检索指定音频线路的信息 
mixerGetLineControls用于检索一个或者多个与音频线路相关的控制的通用信息 
mixerGetControlDetails用于检索与某个音频线路相关的一个控制的属性 
mixerSetControlDetails用于设置制定控制的属性。
```cpp
int GetAudioOutputVolumn_Win7(const std::string &deviceName)
{
    MMRESULT          rc;     // 多媒体函数返回结果变量
    HMIXER            hMixer; // 混合器设备句柄
    MIXERLINE         mxl;    // 音频线路标准状态信息结构体
    MIXERLINECONTROLS mxlc;   // 音频线路控制器集合信息结构体
    MIXERCONTROL      mxc;    // 单个音频线路控制器信息结构体
    // 打开混合器设备
    rc = mixerOpen(&hMixer, // 返回的设备句柄
        0,       // 单声卡的设备ID为零
        0,       // 不使用回调机制
        0,       // 回调函数参数
        0);      // MIXER_OBJECTF_MIXER宏的值，表示任选有效设备ID
    // 打开混合器设备无错的话，则
    if (MMSYSERR_NOERROR == rc)
    {
        // MIXERLINE 结构体变量清零
        ZeroMemory(&mxl, sizeof(MIXERLINE));
        mxl.cbStruct = sizeof(MIXERLINE); // 微软用此办法判断版本
        // 指出需要获取的通道，声卡的音频输出用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
        mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
        // 取得混合器设备的指定线路信息
        rc = mixerGetLineInfo((HMIXEROBJ)hMixer,
            &mxl,
            // 取得MIXERLINE::dwComponentType指定类型的第一个音频线路信息
            MIXER_GETLINEINFOF_COMPONENTTYPE);
        // 取得混合器设备的指定线路信息成功的话，则
        if (MMSYSERR_NOERROR == rc)
        {
            // MIXERCONTROL 结构体变量清零
            ZeroMemory(&mxc, sizeof(MIXERCONTROL));
            mxc.cbStruct = sizeof(mxc); // 微软用此办法判断版本
            // MIXERLINECONTROLS 结构体变量清零
            ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));
            mxlc.cbStruct = sizeof(mxlc); // 微软用此办法判断版本
            mxlc.dwLineID = mxl.dwLineID; // 上面取得的声卡音频输出线路标识
            // 控制类型为控制音量
            mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
            mxlc.cControls = 1;          // 使用 MIXERCONTROL 结构体变量个数
            mxlc.pamxctrl = &mxc;        // MIXERCONTROL 结构体变量指针
            mxlc.cbmxctrl = sizeof(mxc); // MIXERCONTROL 结构体变量字节大小
            // 取得控制器信息
            rc = mixerGetLineControls((HMIXEROBJ)hMixer,
                &mxlc,
                MIXER_GETLINECONTROLSF_ONEBYTYPE);
            // 取得控制器信息成功的话，则
            if (MMSYSERR_NOERROR == rc)
            {
                // 获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum.
                MIXERCONTROLDETAILS        mxcd;      // 控制器的状态信息
                MIXERCONTROLDETAILS_SIGNED volStruct; // 音量结构体变量(就一个成员量)
                // MIXERCONTROLDETAILS 结构体变量清零
                ZeroMemory(&mxcd, sizeof(mxcd));
                mxcd.cbStruct = sizeof(mxcd);       // 微软用此办法判断版本
                mxcd.dwControlID = mxc.dwControlID; // 上面取得的控制器标识
                mxcd.paDetails = &volStruct;        // 音量结构体变量指针
                mxcd.cbDetails = sizeof(volStruct); // 音量结构体变量字节大小
                mxcd.cChannels = 1;                 // 取得或设置全部通道
                //  获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum. 
                // 获得音量值
                rc = mixerGetControlDetails((HMIXEROBJ)hMixer,
                    &mxcd,
                    MIXER_GETCONTROLDETAILSF_VALUE);
                if (MMSYSERR_NOERROR == rc)
                {
                    long step = ( mxc.Bounds.lMaximum - mxc.Bounds.lMinimum ) /100;
                    int index = ( volStruct.lValue - mxc.Bounds.lMinimum) /step ;
                    printf("音量：%X", volStruct.lValue);
                    return index;
                }
            }
        }
    }
}
```
## 二、设置设备音量
设置音量和获取音量大部分代码都是相同的，就几句不一样。
### 1、Vista下设置设备音量
```cpp
int SetAudioOutputVolum_Vista(int volumnIndex)
{
    IMMDeviceEnumerator* pEnumerator;
    IMMDeviceCollection* pCollection = NULL;
    IMMDevice *pDevice = NULL;
    IPropertyStore *pProperties=NULL;  
    IAudioEndpointVolume *pVolumeAPI=NULL;  
    UINT deviceCount = 0;
    float fVolume = -1;
    CoInitializeEx( NULL , COINIT_MULTITHREADED );
    HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
    if (hr != S_OK)  
    {  
        printf("CoCreateInstance Failed!\n");  
        return 0;  
    }  
    //hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
    hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATE_ACTIVE , &pCollection );
    if (hr != S_OK)  
    {  
        printf("EnumAudioEndpoints Failed!\n");  
        goto releasepEnumerator;  
    }  
    hr = pCollection->GetCount(&deviceCount);  
    if (hr != S_OK)  
    {  
        printf("GetCount Failed!\n");  
        goto releasepCollection;  
    }  
    for (UINT dev=0;dev<deviceCount;dev++)  
    {  
        pDevice = NULL;  
        hr = pCollection->Item(dev,&pDevice);  
        if (hr == S_OK)  
        {  
            hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);  
            if (hr == S_OK)  
            {  
                PROPVARIANT varName;  
                PropVariantInit(&varName);  
                hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName); 
                if( SUCCEEDED(hr) )
                {
                    hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));  
                    if (hr==S_OK)  
                    {  
                        float volumnLevel = (float)volumnIndex /100;
                        hr = <span style="color:#ff6666;">pVolumeAPI->SetMasterVolumeLevelScalar( volumnLevel, NULL );</span>
                    } 
                }
                SAFE_RELEASE(pProperties);  
            }  
            SAFE_RELEASE(pDevice);  
        }  
    }  
releasepCollection:  
    SAFE_RELEASE(pCollection);  
releasepEnumerator:  
    SAFE_RELEASE(pEnumerator);  
    return  fVolume*100;
}
```
### 2、Win7下设置设备音量
```cpp
int SetAudioOutputVolum_Win7(int volumnIndex)
{
    MMRESULT          rc;     // 多媒体函数返回结果变量
    HMIXER            hMixer; // 混合器设备句柄
    MIXERLINE         mxl;    // 音频线路标准状态信息结构体
    MIXERLINECONTROLS mxlc;   // 音频线路控制器集合信息结构体
    MIXERCONTROL      mxc;    // 单个音频线路控制器信息结构体
    // 打开混合器设备
    rc = mixerOpen(&hMixer, // 返回的设备句柄
        0,       // 单声卡的设备ID为零
        0,       // 不使用回调机制
        0,       // 回调函数参数
        0);      // MIXER_OBJECTF_MIXER宏的值，表示任选有效设备ID
    // 打开混合器设备无错的话，则
    if (MMSYSERR_NOERROR == rc)
    {
        // MIXERLINE 结构体变量清零
        ZeroMemory(&mxl, sizeof(MIXERLINE));
        mxl.cbStruct = sizeof(MIXERLINE); // 微软用此办法判断版本
        // 指出需要获取的通道，声卡的音频输出用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
        mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
        // 取得混合器设备的指定线路信息
        rc = mixerGetLineInfo((HMIXEROBJ)hMixer,
            &mxl,
            // 取得MIXERLINE::dwComponentType指定类型的第一个音频线路信息
            MIXER_GETLINEINFOF_COMPONENTTYPE);
        // 取得混合器设备的指定线路信息成功的话，则
        if (MMSYSERR_NOERROR == rc)
        {
            // MIXERCONTROL 结构体变量清零
            ZeroMemory(&mxc, sizeof(MIXERCONTROL));
            mxc.cbStruct = sizeof(mxc); // 微软用此办法判断版本
            // MIXERLINECONTROLS 结构体变量清零
            ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));
            mxlc.cbStruct = sizeof(mxlc); // 微软用此办法判断版本
            mxlc.dwLineID = mxl.dwLineID; // 上面取得的声卡音频输出线路标识
            // 控制类型为控制音量
            mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
            mxlc.cControls = 1;          // 使用 MIXERCONTROL 结构体变量个数
            mxlc.pamxctrl = &mxc;        // MIXERCONTROL 结构体变量指针
            mxlc.cbmxctrl = sizeof(mxc); // MIXERCONTROL 结构体变量字节大小
            // 取得控制器信息
            rc = mixerGetLineControls((HMIXEROBJ)hMixer,
                &mxlc,
                MIXER_GETLINECONTROLSF_ONEBYTYPE);
            // 取得控制器信息成功的话，则
            if (MMSYSERR_NOERROR == rc)
            {
                // 获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum.
                MIXERCONTROLDETAILS        mxcd;      // 控制器的状态信息
                MIXERCONTROLDETAILS_SIGNED volStruct; // 音量结构体变量(就一个成员量)
                // MIXERCONTROLDETAILS 结构体变量清零
                ZeroMemory(&mxcd, sizeof(mxcd));
                mxcd.cbStruct = sizeof(mxcd);       // 微软用此办法判断版本
                mxcd.dwControlID = mxc.dwControlID; // 上面取得的控制器标识
                mxcd.paDetails = &volStruct;        // 音量结构体变量指针
                mxcd.cbDetails = sizeof(volStruct); // 音量结构体变量字节大小
                mxcd.cChannels = 1;                 // 取得或设置全部通道
                //  获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum. 
                // 获得音量值
                rc = mixerGetControlDetails((HMIXEROBJ)hMixer,
                    &mxcd,
                    MIXER_GETCONTROLDETAILSF_VALUE);
                if (MMSYSERR_NOERROR == rc)
                {
                    long step = ( mxc.Bounds.lMaximum - mxc.Bounds.lMinimum ) /100;
                   <span style="color:#ff6666;"> volStruct.lValue = volumnIndex*step;	 // 想要设置的值  
                    rc = mixerSetControlDetails((HMIXEROBJ)hMixer,  
                        &mxcd,  
                        MIXER_SETCONTROLDETAILSF_VALUE);  
</span>
                    if (MMSYSERR_NOERROR == rc)  
                    {  
                        return 1;
                    }  
                }
            }
        }
    }
}
```

