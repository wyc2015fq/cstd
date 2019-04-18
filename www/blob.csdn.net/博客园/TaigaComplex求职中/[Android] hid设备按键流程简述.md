# [Android] hid设备按键流程简述 - TaigaComplex求职中 - 博客园







# [[Android] hid设备按键流程简述](https://www.cnblogs.com/TaigaCon/p/4813742.html)





hexdump /dev/hidraw0就能看到usbhid设备传输过来的裸流

如：按下Input键
003ae60 0000 0096 8000 006b 0000 0000 0000 0000
*
003aea0 0000 0086 8000 0000 0000 0000 0000 0000
*


前面的估计是码流头（003ae60），还有按下（0096），松开（0086）等信息，识别按键有效信息是（006b）

006b其实是索引值，会在下表中寻找键值发送给Android，如：6b对应186
//drivers/hid/hid-input.c
static const unsigned char hid_keyboard[256] = {
      0,  0,  0,  0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38,
     50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44,  2,  3,
      4,  5,  6,  7,  8,  9, 10, 11, 28,  1, 14, 15, 57, 12, 13, 26,
     27, 43, 43, 39, 40, 41, 51, 52, 53, 58, 59, 60, 61, 62, 63, 64,
     65, 66, 67, 68, 87, 88, 99, 70,119,110,102,104,111,107,109,106,
    105,108,103, 69, 98, 55, 74, 78, 96, 79, 80, 81, 75, 76, 77, 71,
     72, 73, 82, 83, 86,127,116,117,183,184,185,186,187,188,189,190,
    191,192,193,194,134,138,130,132,128,129,131,137,133,135,136,113,
    115,114,unk,unk,unk,121,unk, 89, 93,124, 92, 94, 95,unk,unk,unk,
    122,123, 90, 91, 85,unk,unk,unk,unk,unk,unk,unk,111,unk,unk,unk,
    unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,
    unk,unk,unk,unk,unk,unk,179,180,unk,unk,unk,unk,unk,unk,unk,unk,
    unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,unk,
    unk,unk,unk,unk,unk,unk,unk,unk,111,unk,unk,unk,unk,unk,unk,unk,
     29, 42, 56,125, 97, 54,100,126,164,166,165,163,161,115,114,113,
    150,158,159,128,136,177,178,176,142,152,173,140,unk,unk,unk,unk
};




在Android层接收的消息我们可以通过getevent获得
/dev/input/event2: 0004 0004 0007006b
/dev/input/event2: 0001 00ba 00000001
/dev/input/event2: 0000 0000 00000000
/dev/input/event2: 0004 0004 0007006b
/dev/input/event2: 0001 00ba 00000000
/dev/input/event2: 0000 0000 00000000


可以看到006b这一项就是hid设备发送过来的码值，ba就是186，在linux层获取到的键值

Android通过event2获得到键值后，接下来就是标准的Android按键映射流程了





首先就是查看/dev/input/event2这个设备文件对应的设备信息及映射表
dumpsys input

    2:   Haier Smart TV RF Receiver
      Classes: 0x80000023
      Path: /dev/input/event2
      Descriptor: 4c674c2e6733e157d6ab300ee6f2c3dc1245e649
      Location: usb-rtk-3/input4
      ControllerNumber: 0
      UniqueId: 
      Identifier: bus=0x0003, vendor=0x1a1d, product=0x00e0, version=0x0100
      KeyLayoutFile: /system/usr/keylayout/Vendor_1a1d_Product_00e0.kl
      KeyCharacterMapFile: /system/usr/keychars/Generic.kcm
      ConfigurationFile: 
      HaveKeyboardLayoutOverlay: false



设备信息是：vendor=0x1a1d, product=0x00e0

那么根据我们之前按键映射流程的分析，会先查找是否有Vendor_[vendor]_Product_[product].kl

这里得到/system/usr/keylayout/Vendor_1a1d_Product_00e0.kl，源码目录在frameworks/base/data/keyboards/Vendor_1a1d_Product_00e0.kl

查看映射表文件，发现
key 186         TV_INPUT
即最后会向APK发送KEYCODE_TV_INPUT





最后需要注意的一点是，如果TV_INPUT在KeycodeLabel中没定义，是解析Vendor_1a1d_Product_00e0.kl这个映射表文件的时候会出错，从而导致event2用Generic.kl

因此KeycodeLabel中必须有TV_INPUT项：
    { "TV_INPUT", 178 },











