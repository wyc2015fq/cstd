
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the VIDEO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// VIDEO_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifndef VIDEO_MODE
#define VIDEO_MODE
#ifdef __cplusplus
#define VIDEO_API extern "C" __declspec(dllexport)
#else
#define VIDEO_API __declspec(dllexport)
#endif

typedef BOOL (WINAPI* PFCALLBACK)(DWORD Width, DWORD Height, BYTE* buffer, LPVOID param) ;
//显示视频的回调函数，Width和Height分别为视频的宽度和高度，buffer为RGB格式的视频数据

BOOL WINAPI CBFunc(DWORD Width, DWORD Height, BYTE* buffer, LPVOID param);
//视频显示的回调函数

VIDEO_API BOOL SetVideoModel(BYTE VideoModel);//0:640*480,1:320*240
//设置视频显示模式，VideoModel=0为640*480的分辨率显示，VideoModel=1为320*240的分辨率显示，其它为640*480分辨率显示

VIDEO_API BOOL StartVideo(PFCALLBACK Func, LPVOID param);
//开启视频，Func为定义的回调函数，param为回调函数参数

VIDEO_API void ShowVideo(HWND hWnd, DWORD Width, DWORD Height);
//显示视频，hwnd为窗口句柄，可以为NULL，Width和Height分别为视频的宽和高

VIDEO_API BOOL SetSensorAEWorAEB(BYTE light);
//设置传感器视频亮度，范围是0-255，默认打开为63

VIDEO_API BOOL SetSensorAGC(BYTE Contrast);
//设置传感器视频对比度，范围是0-255，默认打开是传感器的自动设置，手动默认为38

VIDEO_API BOOL SetSensorAEC(int Saturation);
//设置视频曝光度，范围是0-65535，默认打开为传感器自动设置，手动默认为140

VIDEO_API BOOL SetManuallight();
//手动控制亮度的开关，由于传感器的原因，亮度没有自动设置一项，要手动设置亮度必须将对比度和曝光度设置为自动

VIDEO_API BOOL SetManualcontrast(BOOL state);
//手动自动设置对比度的开关，state为TRUE是手动，FALSE是自动

VIDEO_API BOOL SetManualsaturation(BOOL state);
//手动自动设置曝光度的开关，state为TRUE是手动，FALSE是自动

VIDEO_API BOOL SetFrameRate(int rate);
//设置帧率，rate有60、30、15三个值，其它将设置为60

VIDEO_API BOOL GetBMPPhoto(TCHAR* filename);
//扫描图像，filename为包含文件路径的扫描图像保存文件名

VIDEO_API BOOL GetPhoto();
//获得硬件拍照按钮是否按下，此函数需要在一个计时器函数中实时监控
//如果硬件拍照按钮按下返回TRUE，否则返回FALSE，可针对不同的返回值作相应的处理

VIDEO_API int GetSaturation();
//获得手动设置的曝光值，此项在自动设置时无效

VIDEO_API int GetContrast();
//获得手动设置的对比度，此项在自动设置时无效

VIDEO_API int GetLight();
//获得手动设置的亮度

VIDEO_API void ExitVideo();
//关闭视频设备，退出，清理数据

VIDEO_API void SethandLight(int light);
//设置上位视频亮度，不能设置手动自动，只能手动

VIDEO_API void SethandContrast(int contrast);
//设置上位视频对比度，不能设置手动自动，只能手动

VIDEO_API void SetThresholdValue(int value);
//设置上位对比度阀值

VIDEO_API int GethandLight();
//获得上位视频亮度

VIDEO_API int GethandContrast();
//获得上位视频对比度

VIDEO_API int GetThresholdValue();
//获得上位对比度阀值

VIDEO_API BOOL CheckSensor();
//检查设备是否连接就需

VIDEO_API BOOL SendMsg(BYTE msg);
//向设备发送消息
#endif