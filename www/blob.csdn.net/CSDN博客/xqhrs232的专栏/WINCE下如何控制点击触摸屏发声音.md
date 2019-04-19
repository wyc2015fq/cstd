# WINCE下如何控制点击触摸屏发声音 - xqhrs232的专栏 - CSDN博客
2010年06月19日 11:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1699
相关网帖
1、[WinCE系统声音定制](http://blog.csdn.net/nanjianhui/article/details/5200066)----[http://blog.csdn.net/nanjianhui/article/details/5200066](http://blog.csdn.net/nanjianhui/article/details/5200066)
1》修改platform.reg注册表里面的对应项
; Sound CPL registry values
[HKEY_CURRENT_USER/ControlPanel/Volume]
 "Key"=dword:0  ; Keyclicks: 1=soft, 0x10002=loud
 "Screen"=dword:0 ; ScreenTaps: 1=soft, 0x10002=loud
 "Mute"=dword:7   ; 7=Enable all sounds (1=Notifications, 2=Apps, 4=Events.)
 "Volume"=dword:99999999 ; 0=off, 0xFFFFFFFF=maximum
2》修改WINCE系统里面的控制面板下的声音/音量设置项
     跟上面的注册表有一一对应的选项
3》修改C:/WINCE600/PUBLIC/COMMON/OAK/DRIVERS/WAVEUI文件夹下的声音文件可以修改声音
    1111----keyloud.wav+keysoft.wav----按键的两种声音文件
    2222----tchloud.wav+tchsoft.wav----触摸屏的两种声音文件
4》C:/WINCE600/PUBLIC/COMMON/OAK/DRIVERS/WAVEUI/waveui.rc文件的内容
// -----------------------------------------------------------------------------
//  Click wave files.
// -----------------------------------------------------------------------------
ID_WAVE_KEYLOUD WAVE keyloud.wav
ID_WAVE_KEYSOFT WAVE keysoft.wav
ID_WAVE_TCHLOUD WAVE tchloud.wav
ID_WAVE_TCHSOFT WAVE tchsoft.wav
