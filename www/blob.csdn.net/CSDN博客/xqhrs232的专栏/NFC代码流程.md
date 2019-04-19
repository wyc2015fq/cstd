# NFC代码流程 - xqhrs232的专栏 - CSDN博客
2013年11月28日 11:06:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1709
原文地址::[http://blog.csdn.net/anskya520/article/details/8531670](http://blog.csdn.net/anskya520/article/details/8531670)
相关网帖
1、[android2.3.3系统NFC调试](http://blog.csdn.net/comdoc/article/details/6545746)----[http://blog.csdn.net/comdoc/article/details/6545746](http://blog.csdn.net/comdoc/article/details/6545746)
2、Android中NFC功能流程图解析及代码演示----[http://pan.baidu.com/share/link?shareid=353340&uk=2535495845&fid=3575362669](http://pan.baidu.com/share/link?shareid=353340&uk=2535495845&fid=3575362669)
NFC是基于RFID发展而来。RFID即电子标签，跟条码的作用相同，但RFID由于通过RF进行识别，无须露在产品外部。NFC所支持的标准14443和15693也都是RFID的标准。
14443为Proximity Card标准，其radio范围为几厘米
15693为Vicinity Card标准，范围较长，可达1~1.5米
NFC为一个新标准，其目的为让智能手机间可以通过短距离radio进行数据传输。其作用于蓝牙相似，但传输距离较蓝牙更短，但更容易建立连接，并且更省电。
从android文档来看，android支持的NFC标准有：
NfcANFC-A (ISO 14443-3A)
NfcBNFC-B (ISO 14443-3B)
NfcFNFC-F (JIS 6319-4)
NfcVNFC-V (ISO 15693)
IsoDepISO-DEP (ISO 14443-4)
MifareClassic
MifareUltralight
android NFC代码位置
frameworks\base\core\java\android\nfc - frameworks中java代码
frameworks\base\core\jni - frameworks中jni代码，会跟libnfc交互
packages\apps\Nfc - app，会跟libnfc和frameworks交互
device\samsung\tuna\nfc - 设备厂家提供的hardware lib
external\libnfc-nxp - libnfc库，nfc数据的读取和解析
初始化NFC
nfc_jni_initialize(), packages/apps/Nfc/jni/com_android_nfc_NativeNfcManager.cpp
nfc_jni_configure_driver，link type通过ro.nfc.port获取，为COM1/COM2...
phLibNfc_Mgt_ConfigureDriver， src/phLibNfc.c
phDal4Nfc_Config，根据link type设置dal回调函数，init然后open设备
pthread_create(&(nat->thread), NULL, nfc_jni_client_thread, nat)创建线程，处理协议栈读写消息
phLibNfc_Mgt_Initialize(), libnfc-nxp/src/phLibNfc.c
phHal4Nfc_Open(), libnfc-nxp/src/phHal4Nfc.c
phHal4Nfc_Configure_Layers(), assign phDal4Nfc_Register to pointer layer_registry
phHciNfc_Initialise()
phHciNfc_Initialise()
phLlcNfc_Register(), phLlcNfc.c, called through func pointer layer_registry in phHal4Nfc.c
phLlcNfc_Interface_Register(), libnfc-nxp/src/phLlcNfc_Interface.c
phDal4Nfc_Register(), libnfc-nxp/Linux_x86/phDal4Nfc.c, called through layer_registry pointer
plower_if->init()，即phLlcNfc_Init
phLlcNfc_H_CreateUFramePayload
phLlcNfc_StartTimers，frame类型为init_u_rset_frame
因此, 通过指针，phHal4Nfc中设置的两层的注册函数phLlcNfc_Register和phDal4Nfc_Register依次被调用。
phLlcNfc_Interface_Register将phLlcNfc_WrResp_Cb传入到Dal，这个函数会在发送消息完成后被调用。
phLlcNfc_Init会导致有数据通过uart发出。发送成功后，会调用LLC层的phLlcNfc_WrResp_Cb。phLlcNfc_WrResp_Cb会尝试从uart上读取回应。（我这里发现读取回应的长度为0，导致crash）
phDal4Nfc_Register
向nfc注册了phDal4Nfc_Init和phDal4Nfc_Read。
phDal4Nfc_Init会调用到phDal4Nfc_Config。在phDal4Nfc_Config中，获取NFC_HARDWARE_MODULE_ID，得到hardware动态库的信息，打开和配置串口。最后，会调用phDal4Nfc_StartThreads来启动一个读写线程。
phDal4Nfc_Read会post信号量nfc_read_sem。phDal4Nfc_StartThreads获取信号量nfc_read_sem后，会从串口中读取数据。
扫描和读取tag
com_android_nfc_NfcManager_enableDiscovery(), Nfc/jni/com_android_nfc_NativeNfcManager.cpp
nfc_jni_start_discovery_locked()
phLibNfc_RemoteDev_NtfRegister(nfc_jni_Discovery_notification_callback), 传入回调函数
发现tag后，
nfc_jni_Discovery_notification_callback()
e->CallVoidMethod(nat->manager, cached_NfcManager_notifyNdefMessageListeners, tag);
notifyNdefMessageListeners(), NativeNfcManager.java
sendMessage(NfcService.MSG_NDEF_TAG, tag);
NfcService.java收到MSG_NDEF_TAG，
findAndReadNdef(), Nfc/src/com/android/nfc/nxp/NativeNfcTag.java
readNdef();
doRead(), Nfc/jni/com_android_nfc_NativeNfcTag.java
phLibNfc_Ndef_Read();
new NdefMessage(buff), core/java/android/nfc/NdefMessage.java
parseNdefMessage(data),
android_nfc_NdefMessage_parseNdefMessage()
phFriNfc_NdefRecord_GetRecords(), 进入libnfc
