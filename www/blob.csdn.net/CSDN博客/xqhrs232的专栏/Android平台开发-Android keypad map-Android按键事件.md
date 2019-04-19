# Android平台开发-Android keypad map-Android按键事件 - xqhrs232的专栏 - CSDN博客
2011年09月07日 10:02:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：853标签：[android																[平台																[keyboard																[frameworks																[linux																[character](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=frameworks&t=blog)](https://so.csdn.net/so/search/s.do?q=keyboard&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://hi.baidu.com/i_know_all/blog/item/0a1dd8ecdde99acb2f2e2159.html](http://hi.baidu.com/i_know_all/blog/item/0a1dd8ecdde99acb2f2e2159.html)
一、Android底层按键事件处理过程
   在系统启动后，在文件。。。中，android 会通过
    static const char *device_path = "/dev/input"; 
    bool EventHub::penPlatformInput(void) 
    res = scan_dir(device_path);
    通过下面的函数打开设备。
    int EventHub::pen_device(const char *deviceName) 
    { 
      ... 
      fd = open(deviceName, O_RDWR); 
      ...  
      mFDs[mFDCount].fd = fd; 
      mFDs[mFDCount].events = POLLIN; 
      ... 
      ioctl(mFDs[mFDCount].fd, EVIOCGNAME(sizeof(devname)-1), devname); 
      ... 
      const char* root = getenv("ANDROID_ROOT"); 
      snprintf(keylayoutFilename, sizeof(keylayoutFilename), 
                 "%s/usr/keylayout/%s.kl", root, tmpfn); 
      ... 
      device->layoutMap->load(keylayoutFilename); 
      ... 
  }
  打开设备的时候，如果 device->classes&CLASS_KEYBOARD 不等于 0 表明是键盘。 
  常用输入设备的定义有： 
  enum { 
        CLASS_KEYBOARD      = 0x00000001, //键盘 
        CLASS_ALPHAKEY      = 0x00000002, // 
        CLASS_TOUCHSCREEN   = 0x00000004, //触摸屏 
        CLASS_TRACKBALL     = 0x00000008  //轨迹球 
  };
  打开键盘设备的时候通过上面的 ioctl 获得设备名称，命令字 EVIOCGNAME 的定义在文件：  
  kernel/include/linux/input.h 中。
  对于按键事件，调用mDevices->layoutMap->map进行映射，调用的是文件 KeyLayoutMap.cpp
  (frameworks\base\libs\ui)中的函数：
  status_t KeyLayoutMap::load(const char* filename)通过解析 <Driver name>.kl 把按键的
  映射关系保存在 ：KeyedVector<int32_t,Key> m_keys; 中。
  当获得按键事件以后调用： status_t KeyLayoutMap::map(int32_t scancode, int32_t
  *keycode, uint32_t *flags)
  由映射关系 KeyedVector<int32_t,Key> m_keys 把扫描码转换成andorid上层可以识别的按键。
二、按键映射
  Key layout maps的路径是 /system/usr/keylayout，第一个查找的名字是按键驱动的名字，例如
  mxckpd.kl。如果没有的话，默认为qwerty.kl。
  Key character maps的路径是 /system/usr/keychars，第一个查找的名字是按键驱动的名字，例如
  mxckpd.kcm。如果没有的话，默认为qwerty.kl。
  qwerty.kl是 UTF-8类型的，格式为：key SCANCODE KEYCODE [FLAGS...]。
  SCANCODE表示按键扫描码；
  KEYCODE表示键值，例如HOME,BACK,1,2,3...
  FLAGS有如下定义：
    SHIFT: While pressed, the shift key modifier is set 
    ALT: While pressed, the alt key modifier is set 
    CAPS: While pressed, the caps lock key modifier is set 
    WAKE: When this key is pressed while the device is asleep, the device will
                  wake up and the key event gets sent to the app. 
    WAKE_DROPPED: When this key is pressed while the device is asleep, the device
                  will wake up and the key event does not get sent to the app
  qwerty.kcm文件为了节省空间，在编译过程中会用工具makekcharmap转化为二进制文件qwerty.bin。
三、按键分发
  1、输入事件分发线程
    在frameworks/base/services/java/com/android/server/WindowManagerService.java里创
    建了一个输入事件分发线程，它负责把事件分发到相应的窗口上去。
    在WindowManagerService类的构造函数WindowManagerService()中：
        mQueue = new KeyQ(); //读取按键
        mInputThread = new InputDispatcherThread();  //创建分发线程     
        ...     
        mInputThread.start();
    在启动的线程InputDispatcherThread中：
        run() 
        process(); 
        QueuedEvent ev = mQueue.getEvent(...) 
      在process() 方法中进行处理事件：
        switch (ev.classType) 
          case RawInputEvent.CLASS_KEYBOARD: 
             ... 
             dispatchKey((KeyEvent)ev.event, 0, 0); 
             mQueue.recycleEvent(ev); 
             break; 
          case RawInputEvent.CLASS_TOUCHSCREEN: 
             //Log.i(TAG, "Read next event " + ev); 
             dispatchPointer(ev, (MotionEvent)ev.event, 0, 0); 
             break; 
 　       case RawInputEvent.CLASS_TRACKBALL:
             dispatchTrackball(ev, (MotionEvent)ev.event, 0, 0);
             break;
  2、上层读取按键的流程
    WindowManagerService()  //(frameworks\base\services\java\com\android\server
                                                    \WindowManagerService.java)
      |
    KeyQ()  //KeyQ 是抽象类 KeyInputQueue 的实现
      |
    InputDeviceReader //在 KeyInputQueue 类中创建的线程 
     |
    readEvent()  //
     |
    android_server_KeyInputQueue_readEvent() //frameworks\base\services\jni\
                                              com_android_server_KeyInputQueue.cpp
     |
    hub->getEvent()
     |
    EventHub::getEvent() //frameworks\base\libs\ui\EventHub.cpp
     |
    res = read(mFDs.fd, &iev, sizeof(iev)); //
     Android的应用不仅仅是平板电脑，MID，phone,还可以放到STB机顶盒，智能家庭终端上面去，所以按键的映射是一定要自定义的，不管按键是固定在设备上，还是通过无线设备还是蓝牙遥控，都需要键的映射。
       Android也是基于Linux的核心，大部分时候都是操作系统在调度任务，执行任务。相应的，Android输入系统也是遵循LINUX的input 输入输出子系统，关于这部分的分析可以Google，有许多原理方面的分析。Android使用标准的Linux输入事件设备（/dev /event0)，驱动描述可以查看内核树头文件include/linux/input.h。如果想深入学习Linux input subsystem，可以访问：http://git.kernel.org/?p=linux/kernel/git/stable/linux-
 2.6.24.y.git;a=blob;f=Documentation/input/input.txt
注：event0是您的keypad/gpio-key注册到内核的节点号，如果有其他的输入设备注册进内核，也可以是event1。
功能性
   Android输入事件设备，用的是中断（硬件触发）或者轮询结构（软件模拟),捕获设备具体的扫描码，通过input_event()转化成标准的内核可接受的事件。
  键映射驱动的其他主要驱动是建立一个probe函数，用于注册中断或者您的软件模拟的轮询功能函数，硬件初始化，用input_register_device()注册驱动/设备到输入输出系统。
  注：关于probe属于linux设备驱动模型相关知识，可以阅读LDD3或者LINUX设备驱动原理与实践，有很详细的描述。 
下面表描述了从键盘输入最终转成相应应用行为的转化步骤
步骤                            行为                                                解释 
1                    窗口管理器从Linux键盘驱动获取键盘事件                             按键消息不指定任何逻辑事件，它只与其硬件位置有关, 也就是说，按键的键盘码没有任何软件含义,映射键盘码
2                    窗口管理器映射扫描码为键码。                                    当窗口管理器从驱动读到一个键，它利用那个键布局映射文件将扫描码映射为键值。特别的，这个键值就是屏幕显示的条码。例如
                                                                            KEYCODE_DPAD_CENTER是导航五位键的中间的键，即使ALT＋G产生一个"?"字符，事实上KEYCODE_G就是这个键值。
3                           窗口管理器发送扫描码和键码到应用程序`                            扫描码和键码被当前焦点所在界面处理，具体"翻译"要看具体的应用场合。   
键布局映射
如何选择一个键布局映射文件
键布局映射文件通常放在/system/usr/keylayout和/data/usr/keylayout
对于每一个键盘设备xxx,设置系统属性android.keylayout.xxx,如果没有为自己的硬件设备创建一个特定的设备，Android将去读/system/usr/keylayout/qwerty.kl这个配置文件。
注：如果设置系统属性，请查看ttp://www.kandroid.org/android_pdk/build_new_device.html
文件格式:
键映射文件通常以UTF8文本文件格式存储于设备，通常有如下特性：
注释：用＃表示，以＃开头的内容都将被忽略。
空白：所有的空行被忽略
键定义：键定义遵循如下格式key SCANCODE KEYCODE [FLAGS...],当扫描码是一个数字，键码定义在你描述的布局文件android.keylayout.xxx，另外可以设置相关的FLAGS:
SHIFT: 当按下，自动加上SHIFT键值
ALT:当按下，自动加上ALT
CAPS:当按下，自动带上CAPS大写
WAKE:当按下，当设备进入睡眠的时候，按下这个键将唤醒，而且发送消息给应用层。
WAKE_DROPPED:当按下，且设备正处于睡眠，设备被唤醒，但是不发送消息给应用层。
键盘映射文件示例：
android/src/device/product/generic/tuttle2.kl
# Copyright 2007 The Android Open Source Project
key 2     1
key 3     2
key 4     3
key 5     4
key 6     5
key 7     6
key 8     7
key 9     8
key 10    9
key 11    0
key 158   BACK              WAKE_DROPPED
key 230   SOFT_RIGHT        WAKE
key 60    SOFT_RIGHT        WAKE
key 107   ENDCALL           WAKE_DROPPED
key 62    ENDCALL           WAKE_DROPPED
key 229   MENU         WAKE_DROPPED
key 59    MENU         WAKE_DROPPED
key 228   POUND
key 227   STAR
key 231   CALL              WAKE_DROPPED
key 61    CALL              WAKE_DROPPED
key 232   DPAD_CENTER       WAKE_DROPPED
key 108   DPAD_DOWN         WAKE_DROPPED
key 103   DPAD_UP           WAKE_DROPPED
key 102   HOME              WAKE
key 105   DPAD_LEFT         WAKE_DROPPED
key 106   DPAD_RIGHT        WAKE_DROPPED
key 115   VOLUME_UP
key 114   VOLUME_DOWN
key 116   POWER             WAKE
key 212   SLASH
key 16    Q
key 17    W
key 18    E
key 19    R
key 20    T
key 21    Y
key 22    U
key 23    I
key 24    O
key 25    P
key 30    A
key 31    S
key 32    D
key 33    F
key 34    G
key 35    H
key 36    J
key 37    K
key 38    L
key 14    DEL
key 44    Z
key 45    X
key 46    C
key 47    V
key 48    B
key 49    N
key 50    M
key 51    COMMA
key 52    PERIOD
key 28    ENTER 
key 56    ALT_LEFT
key 42    SHIFT_LEFT
key 215   AT
key 57    SPACE
key 53    SLASH
key 127   SYM
key 100   ALT_LEFT
key 399   GRAVE
键字符映射：
键字符映射位于：/system/usr/keychars和/data/usr/keychars!
比如对于一个特定的设备xxx，设置android.keychar.xxx系统属性，用全路径表示去描述所需的键字符映射。如果你没有描述任何一个键字符映射，系统将默认使用/system/usr/keychar/qwerty.kl！
注：这一点可以在开发板接USB KEYBOARD的时候，将logcat打开看调试信息，会看到default to qwerty.kl类似的信息。
文件格式：
键字符映射文件以二进制减少加载时间的形式存储于设备中，键字符映射文件有如下特征：
注释：以＃开始为注释
空行：所有的空行被忽略
列定义：当一个事件来临的时候按下组合键。这个事通常是MODIFIER_SHIFT,MODIFIER_CTRL,MODIFIER_ALT的组合。
O                     no modifiers
S                     MODIFIER_SHIFT
C                     MODIFIER_CONTROL
L                     MODIFIER_CAPS_LOCK
A                     MODIFIER_ALT
键值定义：键值定义遵循如下规则：
键 扫描码 字符[....]
扫描码和字符通常是一个十进制的值或者是UTF8字符，可以通过strtol的解析。
键字符文件的示例：
下面这个文件来自于android/src/device/product/generic/tuttle2.kcm，代表了一个完整的键字符文件。
以type开始的语句描述了你所要描述键盘的类型，大体分为三种
1：NUMERIC,12键的数字键盘
2：Q14：键盘包括所有的字符，但是可以一键多个字符。
3：QWERTY键盘包括了所有可能的字符和数字，类似于全键盘。
下面是一个QWERTY全键盘的定义示例，因为android主要用于手机，手机一般是全键。
# Copyright 2007 The Android Open Source Project
[type=QWERTY]
# keycode   base    caps    fn      caps_fn number  display_label
A           'a'     'A'     '%'     0x00    '%'     'A'
B           'b'     'B'     '='     0x00    '='     'B'
C           'c'     'C'     '8'     0x00E7  '8'     'C'
D           'd'     'D'     '5'     0x00    '5'     'D'
E           'e'     'E'     '2'     0x0301  '2'     'E'
F           'f'     'F'     '6'     0x00A5  '6'     'F'
G           'g'     'G'     '-'     '_'     '-'     'G'
H           'h'     'H'     '['     '{'     '['     'H'
I           'i'     'I'     '$'     0x0302  '$'     'I'
J           'j'     'J'     ']'     '}'     ']'     'J'
K           'k'     'K'     '"'     '~'     '"'     'K'
L           'l'     'L'     '''     '`'     '''     'L'
M           'm'     'M'     '>'     0x00    '>'     'M'
N           'n'     'N'     '<'     0x0303  '<'     'N'
O           'o'     'O'     '('     0x00    '('     'O'
P           'p'     'P'     ')'     0x00    ')'     'P'
Q           'q'     'Q'     '*'     0x0300  '*'     'Q'
R           'r'     'R'     '3'     0x20AC  '3'     'R'
S           's'     'S'     '4'     0x00DF  '4'     'S'
T           't'     'T'     '+'     0x00A3  '+'     'T'
U           'u'     'U'     '&'     0x0308  '&'     'U'
V           'v'     'V'     '9'     '^'     '9'     'V'
W           'w'     'W'     '1'     0x00    '1'     'W'
X           'x'     'X'     '7'     0xEF00  '7'     'X'
Y           'y'     'Y'     '!'     0x00A1  '!'     'Y'
Z           'z'     'Z'     '#'     0x00    '#'     'Z'
COMMA       ','     ';'     ';'     '|'     ','     ','
PERIOD      '.'     ':'     ':'     0x2026  '.'     '.'
AT          '@'     '0'     '0'     0x2022  '0'     '@'
SLASH       '/'     '?'     '?'     '/'     '/'     '/'
SPACE       0x20    0x20    0x9     0x9     0x20    0x20
NEWLINE     0xa     0xa     0xa     0xa     0xa     0xa
# on pc keyboards
TAB         0x9     0x9     0x9     0x9     0x9     0x9
0           '0'     ')'     ')'     ')'     '0'     '0'
1           '1'     '!'     '!'     '!'     '1'     '1'
2           '2'     '@'     '@'     '@'     '2'     '2'
3           '3'     '#'     '#'     '#'     '3'     '3'
4           '4'     '$'     '$'     '$'     '4'     '4'
5           '5'     '%'     '%'     '%'     '5'     '5'
6           '6'     '^'     '^'     '^'     '6'     '6'
7           '7'     '&'     '&'     '&'     '7'     '7'
8           '8'     '*'     '*'     '*'     '8'     '8'
9           '9'     '('     '('     '('     '9'     '9'
GRAVE         '`'     '~'     '`'     '~'     '`'     '`'
MINUS         '-'     '_'     '-'     '_'     '-'     '-'
EQUALS        '='     '+'     '='     '+'     '='     '='
LEFT_BRACKET  '['     '{'     '['     '{'     '['     '['
RIGHT_BRACKET ']'     '}'     ']'     '}'     ']'     ']'
BACKSLASH     '/'     '|'     '/'     '|'     '/'     '/'
SEMICOLON     ';'     ':'     ';'     ':'     ';'     ';'
APOSTROPHE    '''     '"'     '''     '"'     '''     '''
STAR          '*'     '*'     '*'     '*'     '*'     '*'
POUND         '#'     '#'     '#'     '#'     '#'     '#'
PLUS          '+'     '+'     '+'     '+'     '+'     '+'
资源二进制格式
上面所描述的一段通过makekcharmap工具转换成下面的格式，用户可以通过mmap这个文件，用于进程之间共享大概4K数据，可以节省加载时间。
Offset              Size (bytes)              Description
0x00-0x0b                             The ascii value "keycharmap1" including the null character
0x0c-0x0f                             padding
0x10-0x13                             The number of entries in the modifiers table (COLS)
0x14-0x17                             The number of entries in the characters table (ROWS)
0x18-0x1f                             padding
                4*COLS                 Modifiers table. The modifier mask values that each of the columns in the characters table correspond to.
                                    padding to the next 16 byte boundary
                4*COLS*ROWS         Characters table. The modifier mask values that each of the columns correspond to.
完善你自己的键盘事件驱动（略）

