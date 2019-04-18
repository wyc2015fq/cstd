# Android杂谈:dumpsys audio了解音频系统 - weixin_33985507的博客 - CSDN博客
2017年06月07日 10:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：59
> 
dumsys是调试很有用的命令，可以用他看系统的一些信息。现在主要是看下audio的信息啦。
主要包括：
1.控制线程的id
2.AudioPolicyManager 信息，包括主输出，forceuse，可用输入输出设备及其配置文件
3.硬件module，包括module下的output，input，设备，路由
4.所有的output dump信息
5.音效
6.audiopatch
```
AudioPolicyManager: 0xe7de7340
Command Thread: 0xe7da2020                  //命令线程
Tones Thread: 0xe7da1fc0                         //按键音线程
AudioCommandThread 0xe7da2020 Dump
- Commands:
   Command Time        Wait pParam
  Last Command
   03      043273.404  0    0xe550f010
AudioCommandThread 0xe7da1fc0 Dump
- Commands:
   Command Time        Wait pParam
  Last Command
     none
AudioPolicyManager Dump: 0xe7de7340
 Primary Output: 13                                   //主输出
 Phone state: 0
 Force use for communications 0                //forceuse
 Force use for media 0
 Force use for record 0
 Force use for dock 8
 Force use for system 0
 Force use for hdmi system audio 0
 Force use for encoded surround output 0
 TTS output not available
 Master mono: off
- Available output devices:                       //可用的输出设置
  Device 1:
  - id:  1
  - tag name: Earpiece
  - type: AUDIO_DEVICE_OUT_EARPIECE                       
  - Profiles:                                                   //配置文件
      Profile 0:
          - format: AUDIO_FORMAT_PCM_16_BIT
          - sampling rates:44100, 48000
          - channel masks:0x0001
      Profile 1:
          - format: AUDIO_FORMAT_PCM_32_BIT
          - sampling rates:44100, 48000
          - channel masks:0x0001
  Device 2:
  - id:  2
  - tag name: Speaker
  - type: AUDIO_DEVICE_OUT_SPEAKER                        
  - Profiles:
      Profile 0:
          - format: AUDIO_FORMAT_PCM_16_BIT
          - sampling rates:44100, 48000
          - channel masks:0x0003
      Profile 1:
          - format: AUDIO_FORMAT_PCM_32_BIT
          - sampling rates:44100, 48000
          - channel masks:0x0003
- Available input devices:                                        //可用的输入设备
  Device 1:
  - id:  5
  - tag name: Built-In Mic
  - type: AUDIO_DEVICE_IN_BUILTIN_MIC                     
  - Profiles:
      Profile 0:
          - format: AUDIO_FORMAT_PCM_16_BIT
          - sampling rates:8000, 16000, 32000, 44100, 48000
          - channel masks:0x000c, 0x0010
      Profile 1:
          - format: AUDIO_FORMAT_PCM_16_BIT
          - sampling rates:8000, 16000, 32000, 44100, 48000
          - channel masks:0x000c, 0x0010, 0x4000, 0x8000, 0xc000
  Device 2:
  - id:  6
  - tag name: FM Tuner In
  - type: AUDIO_DEVICE_IN_FM_TUNER                        
  - Profiles:
      Profile 0:
          - format: AUDIO_FORMAT_PCM_16_BIT
          - sampling rates:8000, 16000, 32000, 44100, 48000
          - channel masks:0x000c, 0x0010
      Profile 1:
          - format: AUDIO_FORMAT_PCM_16_BIT
          - sampling rates:8000, 16000, 32000, 44100, 48000
          - channel masks:0x000c, 0x0010, 0x4000, 0x8000, 0xc000
  Device 3:
  - id:  7
  - tag name: Voice Call In
  - type: AUDIO_DEVICE_IN_TELEPHONY_RX                    
  - Profiles:
      Profile 0:
          - format: AUDIO_FORMAT_PCM_16_BIT
          - sampling rates:8000, 16000, 32000, 44100, 48000
          - channel masks:0x000c, 0x0010, 0x4000, 0x8000, 0xc000
  Device 4:
  - id:  8
  - tag name: Remote Submix In
  - type: AUDIO_DEVICE_IN_REMOTE_SUBMIX                   
  - address: 0                               
  - Profiles:
      Profile 0:
          - format: AUDIO_FORMAT_PCM_16_BIT
          - sampling rates:48000
          - channel masks:0x000c
HW Modules dump:                                //硬件module
- HW Module 1:
  - name: primary
  - handle: 10
  - version: 3.0
  - outputs:
    output 0:
    - name: primary output
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    - flags: 0x0002
    - Supported devices:
      Device 1:
      - id:  1
      - tag name: Earpiece
      - type: AUDIO_DEVICE_OUT_EARPIECE                       
      Device 2:
      - id:  2
      - tag name: Speaker
      - type: AUDIO_DEVICE_OUT_SPEAKER                        
      Device 3:
      - tag name: Wired Headset
      - type: AUDIO_DEVICE_OUT_WIRED_HEADSET                  
      Device 4:
      - tag name: Wired Headphones
      - type: AUDIO_DEVICE_OUT_WIRED_HEADPHONE                
      Device 5:
      - tag name: AUX Digital Out
      - type: AUDIO_DEVICE_OUT_AUX_DIGITAL                    
      Device 6:
      - tag name: BT SCO
      - type: AUDIO_DEVICE_OUT_BLUETOOTH_SCO                  
      Device 7:
      - tag name: BT SCO Headset
      - type: AUDIO_DEVICE_OUT_BLUETOOTH_SCO_HEADSET          
      Device 8:
      - tag name: BT SCO Car Kit
      - type: AUDIO_DEVICE_OUT_BLUETOOTH_SCO_CARKIT           
      Device 9:
      - tag name: Analog Dock Headset
      - type: AUDIO_DEVICE_OUT_ANLG_DOCK_HEADSET              
      Device 10:
      - tag name: Digital Dock Headset
      - type: AUDIO_DEVICE_OUT_DGTL_DOCK_HEADSET              
      Device 11:
      - tag name: FM Tuner Out
      - type: AUDIO_DEVICE_OUT_FM                             
    output 1:
    - name: fast
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    - flags: 0x0004
    - Supported devices:
      Device 1:
      - id:  1
      - tag name: Earpiece
      - type: AUDIO_DEVICE_OUT_EARPIECE                       
      Device 2:
      - id:  2
      - tag name: Speaker
      - type: AUDIO_DEVICE_OUT_SPEAKER                        
      Device 3:
      - tag name: Wired Headset
      - type: AUDIO_DEVICE_OUT_WIRED_HEADSET                  
      Device 4:
      - tag name: Wired Headphones
      - type: AUDIO_DEVICE_OUT_WIRED_HEADPHONE                
  - inputs:
    input 0:
    - name: primary input
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010, 0x4000, 0x8000, 0xc000
    - flags: 0x0000
    - Supported devices:
      Device 1:
      - id:  5
      - tag name: Built-In Mic
      - type: AUDIO_DEVICE_IN_BUILTIN_MIC                     
      Device 2:
      - tag name: Built-In Back Mic
      - type: AUDIO_DEVICE_IN_BACK_MIC                        
      Device 3:
      - tag name: Wired Headset Mic
      - type: AUDIO_DEVICE_IN_WIRED_HEADSET                   
      Device 4:
      - tag name: BT SCO Headset Mic
      - type: AUDIO_DEVICE_IN_BLUETOOTH_SCO_HEADSET           
      Device 5:
      - tag name: AUX Digital In
      - type: AUDIO_DEVICE_IN_AUX_DIGITAL                     
      Device 6:
      - id:  6
      - tag name: FM Tuner In
      - type: AUDIO_DEVICE_IN_FM_TUNER                        
      Device 7:
      - id:  7
      - tag name: Voice Call In
      - type: AUDIO_DEVICE_IN_TELEPHONY_RX                    
  - Declared devices:
    Device 1:
    - id:  1
    - tag name: Earpiece
    - type: AUDIO_DEVICE_OUT_EARPIECE                       
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0001
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0001
    Device 2:
    - id:  2
    - tag name: Speaker
    - type: AUDIO_DEVICE_OUT_SPEAKER                        
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    Device 3:
    - tag name: Wired Headset
    - type: AUDIO_DEVICE_OUT_WIRED_HEADSET                  
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    Device 4:
    - tag name: Wired Headphones
    - type: AUDIO_DEVICE_OUT_WIRED_HEADPHONE                
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    Device 5:
    - tag name: AUX Digital Out
    - type: AUDIO_DEVICE_OUT_AUX_DIGITAL                    
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    Device 6:
    - tag name: BT SCO
    - type: AUDIO_DEVICE_OUT_BLUETOOTH_SCO                  
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000
            - channel masks:0x0003
    Device 7:
    - tag name: BT SCO Headset
    - type: AUDIO_DEVICE_OUT_BLUETOOTH_SCO_HEADSET          
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000
            - channel masks:0x0003
    Device 8:
    - tag name: BT SCO Car Kit
    - type: AUDIO_DEVICE_OUT_BLUETOOTH_SCO_CARKIT           
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000
            - channel masks:0x0003
    Device 9:
    - tag name: Analog Dock Headset
    - type: AUDIO_DEVICE_OUT_ANLG_DOCK_HEADSET              
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    Device 10:
    - tag name: Digital Dock Headset
    - type: AUDIO_DEVICE_OUT_DGTL_DOCK_HEADSET              
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    Device 11:
    - tag name: FM Tuner Out
    - type: AUDIO_DEVICE_OUT_FM                             
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
        Profile 1:
            - format: AUDIO_FORMAT_PCM_32_BIT
            - sampling rates:44100, 48000
            - channel masks:0x0003
    Device 12:
    - id:  5
    - tag name: Built-In Mic
    - type: AUDIO_DEVICE_IN_BUILTIN_MIC                     
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010
        Profile 1:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010, 0x4000, 0x8000, 0xc000
    Device 13:
    - tag name: Built-In Back Mic
    - type: AUDIO_DEVICE_IN_BACK_MIC                        
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010
    Device 14:
    - tag name: Wired Headset Mic
    - type: AUDIO_DEVICE_IN_WIRED_HEADSET                   
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010
    Device 15:
    - tag name: BT SCO Headset Mic
    - type: AUDIO_DEVICE_IN_BLUETOOTH_SCO_HEADSET           
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000
            - channel masks:0x0003
    Device 16:
    - tag name: AUX Digital In
    - type: AUDIO_DEVICE_IN_AUX_DIGITAL                     
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010
    Device 17:
    - id:  6
    - tag name: FM Tuner In
    - type: AUDIO_DEVICE_IN_FM_TUNER                        
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010
        Profile 1:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010, 0x4000, 0x8000, 0xc000
    Device 18:
    - id:  7
    - tag name: Voice Call In
    - type: AUDIO_DEVICE_IN_TELEPHONY_RX                    
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:8000, 16000, 32000, 44100, 48000
            - channel masks:0x000c, 0x0010, 0x4000, 0x8000, 0xc000
  Audio Routes (12):                //音频路由
  - Route 1:
    - Type: Mix
    - Sink: Earpiece
    - Sources: 
        primary output 
        fast 
  - Route 2:
    - Type: Mix
    - Sink: Speaker
    - Sources: 
        primary output 
        fast 
        FM Tuner In 
  - Route 3:
    - Type: Mix
    - Sink: Wired Headset
    - Sources: 
        primary output 
        fast 
        FM Tuner In 
  - Route 4:
    - Type: Mix
    - Sink: Wired Headphones
    - Sources: 
        primary output 
        fast 
        FM Tuner In 
  - Route 5:
    - Type: Mix
    - Sink: AUX Digital Out
    - Sources: 
        primary output 
  - Route 6:
    - Type: Mix
    - Sink: BT SCO
    - Sources: 
        primary output 
  - Route 7:
    - Type: Mix
    - Sink: BT SCO Headset
    - Sources: 
        primary output 
  - Route 8:
    - Type: Mix
    - Sink: BT SCO Car Kit
    - Sources: 
        primary output 
  - Route 9:
    - Type: Mix
    - Sink: Analog Dock Headset
    - Sources: 
        primary output 
  - Route 10:
    - Type: Mix
    - Sink: Digital Dock Headset
    - Sources: 
        primary output 
  - Route 11:
    - Type: Mix
    - Sink: FM Tuner Out
    - Sources: 
        primary output 
  - Route 12:
    - Type: Mix
    - Sink: primary input
    - Sources: 
        Built-In Mic 
        Built-In Back Mic 
        Wired Headset Mic 
        BT SCO Headset Mic 
        AUX Digital In 
        FM Tuner In 
        Voice Call In 
- HW Module 2:                               //硬件module2
  - name: a2dp
  - handle: 18
  - version: 2.0
  - outputs:
    output 0:
    - name: a2dp output
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100
            - channel masks:0x0003
    - flags: 0x0000
    - Supported devices:
      Device 1:
      - tag name: BT A2DP Out
      - type: AUDIO_DEVICE_OUT_BLUETOOTH_A2DP                 
      Device 2:
      - tag name: BT A2DP Headphones
      - type: AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES      
      Device 3:
      - tag name: BT A2DP Speaker
      - type: AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER         
  - inputs:
    input 0:
    - name: a2dp input
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100
            - channel masks:0x000c
    - flags: 0x0000
    - Supported devices:
      Device 1:
      - tag name: BT A2DP In
      - type: AUDIO_DEVICE_IN_BLUETOOTH_A2DP                  
  - Declared devices:
    Device 1:
    - tag name: BT A2DP Out
    - type: AUDIO_DEVICE_OUT_BLUETOOTH_A2DP                 
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100
            - channel masks:0x0003
    Device 2:
    - tag name: BT A2DP Headphones
    - type: AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES      
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100
            - channel masks:0x0003
    Device 3:
    - tag name: BT A2DP Speaker
    - type: AUDIO_DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER         
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100
            - channel masks:0x0003
    Device 4:
    - tag name: BT A2DP In
    - type: AUDIO_DEVICE_IN_BLUETOOTH_A2DP                  
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100
            - channel masks:0x000c
  Audio Routes (4):
  - Route 1:
    - Type: Mix
    - Sink: BT A2DP Out
    - Sources: 
        a2dp output 
  - Route 2:
    - Type: Mix
    - Sink: BT A2DP Headphones
    - Sources: 
        a2dp output 
  - Route 3:
    - Type: Mix
    - Sink: BT A2DP Speaker
    - Sources: 
        a2dp output 
  - Route 4:
    - Type: Mix
    - Sink: a2dp input
    - Sources: 
        BT A2DP In 
- HW Module 3:                                    //硬件module3
  - name: usb
  - handle: 26
  - version: 2.0
  - outputs:
    output 0:
    - name: usb_accessory output
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100
            - channel masks:0x0003
    - flags: 0x0000
    - Supported devices:
      Device 1:
      - tag name: USB Host Out
      - type: AUDIO_DEVICE_OUT_USB_ACCESSORY                  
    output 1:
    - name: usb_device output
    - Profiles:
        Profile 0:[dynamic format][dynamic channels][dynamic rates]
    - flags: 0x0000
    - Supported devices:
      Device 1:
      - tag name: USB Device Out
      - type: AUDIO_DEVICE_OUT_USB_DEVICE                     
  - inputs:
    input 0:
    - name: usb_device input
    - Profiles:
        Profile 0:[dynamic format][dynamic channels][dynamic rates]
    - flags: 0x0000
    - Supported devices:
      Device 1:
      - tag name: USB Device In
      - type: AUDIO_DEVICE_IN_USB_DEVICE                      
  - Declared devices:
    Device 1:
    - tag name: USB Host Out
    - type: AUDIO_DEVICE_OUT_USB_ACCESSORY                  
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:44100
            - channel masks:0x0003
    Device 2:
    - tag name: USB Device Out
    - type: AUDIO_DEVICE_OUT_USB_DEVICE                     
    - Profiles:
        Profile 0:[dynamic format][dynamic channels][dynamic rates]
    Device 3:
    - tag name: USB Device In
    - type: AUDIO_DEVICE_IN_USB_DEVICE                      
    - Profiles:
        Profile 0:[dynamic format][dynamic channels][dynamic rates]
  Audio Routes (3):
  - Route 1:
    - Type: Mix
    - Sink: USB Host Out
    - Sources: 
        usb_accessory output 
  - Route 2:
    - Type: Mix
    - Sink: USB Device Out
    - Sources: 
        usb_device output 
  - Route 3:
    - Type: Mix
    - Sink: usb_device input
    - Sources: 
        USB Device In 
- HW Module 4:
  - name: r_submix
  - handle: 34
  - version: 2.0
  - outputs:
    output 0:
    - name: r_submix output
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:48000
            - channel masks:0x0003
    - flags: 0x0000
    - Supported devices:
      Device 1:
      - tag name: Remote Submix Out
      - type: AUDIO_DEVICE_OUT_REMOTE_SUBMIX                  
      - address: 0                               
  - inputs:
    input 0:
    - name: r_submix input
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:48000
            - channel masks:0x000c
    - flags: 0x0000
    - Supported devices:
      Device 1:
      - id:  8
      - tag name: Remote Submix In
      - type: AUDIO_DEVICE_IN_REMOTE_SUBMIX                   
      - address: 0                               
  - Declared devices:
    Device 1:
    - tag name: Remote Submix Out
    - type: AUDIO_DEVICE_OUT_REMOTE_SUBMIX                  
    - address: 0                               
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:48000
            - channel masks:0x0003
    Device 2:
    - id:  8
    - tag name: Remote Submix In
    - type: AUDIO_DEVICE_IN_REMOTE_SUBMIX                   
    - address: 0                               
    - Profiles:
        Profile 0:
            - format: AUDIO_FORMAT_PCM_16_BIT
            - sampling rates:48000
            - channel masks:0x000c
  Audio Routes (2):
  - Route 1:
    - Type: Mix
    - Sink: Remote Submix Out
    - Sources: 
        r_submix output 
  - Route 2:
    - Type: Mix
    - Sink: r_submix input
    - Sources: 
        Remote Submix In 
//输出output dump
Outputs dump:
- Output 13 dump:
 Latency: 128
 Flags 00000002
 ID: 3
 Sampling rate: 48000
 Format: 00000003
 Channels: 00000003
 Devices 00000002
 Stream volume refCount muteCount
 00     -758.000     00       00
 01     0.000     00       00
 02     -758.000     00       00
 03     0.000     00       00
 04     -758.000     00       00
 05     -758.000     00       00
 06     -1.000     00       00
 07     -758.000     00       00
 08     -758.000     00       00
 09     -758.000     00       01
 10     -758.000     00       00
 11     -758.000     00       00
 12     -1.000     00       00
- Output 21 dump:
 Latency: 42
 Flags 00000004
 ID: 4
 Sampling rate: 48000
 Format: 00000003
 Channels: 00000003
 Devices 00000002
 Stream volume refCount muteCount
 00     -758.000     00       00
 01     0.000     00       00
 02     -758.000     00       00
 03     -758.000     00       00
 04     -758.000     00       00
 05     -758.000     00       00
 06     -1.000     00       00
 07     -758.000     00       00
 08     0.000     00       00
 09     -758.000     00       01
 10     -758.000     00       00
 11     -758.000     00       00
 12     -1.000     00       00
Inputs dump:
Streams dump:
 Stream  Can be muted  Index Min  Index Max  Index Cur [device : index]...
 00      true          00         06         40000000 : 04, 
 01      true          00         16         0002 : 16, 40000000 : 14, 
 02      true          00         16         0002 : 16, 40000000 : 14, 
 03      true          00         16         0002 : 16, 40000000 : 09, 
 04      true          00         16         0002 : 16, 40000000 : 14, 
 05      true          00         16         0002 : 16, 40000000 : 14, 
 06      true          00         15         40000000 : 08, 
 07      true          00         16         0002 : 16, 40000000 : 14, 
 08      true          00         16         0002 : 16, 40000000 : 14, 
 09      true          00         16         0002 : 16, 40000000 : 09, 
 10      true          00         16         0002 : 16, 40000000 : 09, 
 11      true          00         01         40000000 : 00, 
 12      true          00         01         40000000 : 00, 
Volume Curves for Use Cases (aka Stream types) dump:
 AUDIO_STREAM_VOICE_CALL (00): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  0, -4200), ( 33, -2800), ( 66, -1400), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  0, -2400), ( 33, -1600), ( 66,  -800), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  0, -2400), ( 33, -1600), ( 66,  -800), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
 AUDIO_STREAM_SYSTEM (01): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  1, -3000), ( 33, -2600), ( 66, -2200), (100, -1800) }
   DEVICE_CATEGORY_SPEAKER : {(  1, -2400), ( 33, -1800), ( 66, -1200), (100,  -600) }
   DEVICE_CATEGORY_EARPIECE : {(  1, -2400), ( 33, -1800), ( 66, -1200), (100,  -600) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -2100), (100, -1000) }
 AUDIO_STREAM_RING (02): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  1, -4950), ( 33, -3350), ( 66, -1700), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  1, -2970), ( 33, -2010), ( 66, -1020), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  1, -4950), ( 33, -3350), ( 66, -1700), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -2100), (100, -1000) }
 AUDIO_STREAM_MUSIC (03): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
 AUDIO_STREAM_ALARM (04): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  1, -4950), ( 33, -3350), ( 66, -1700), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  1, -2970), ( 33, -2010), ( 66, -1020), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  1, -4950), ( 33, -3350), ( 66, -1700), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -2100), (100, -1000) }
 AUDIO_STREAM_NOTIFICATION (05): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  1, -4950), ( 33, -3350), ( 66, -1700), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  1, -2970), ( 33, -2010), ( 66, -1020), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  1, -4950), ( 33, -3350), ( 66, -1700), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -2100), (100, -1000) }
 AUDIO_STREAM_BLUETOOTH_SCO (06): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  0, -4200), ( 33, -2800), ( 66, -1400), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  0, -2400), ( 33, -1600), ( 66,  -800), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  0, -4200), ( 33, -2800), ( 66, -1400), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
 AUDIO_STREAM_ENFORCED_AUDIBLE (07): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  1, -3000), ( 33, -2600), ( 66, -2200), (100, -1800) }
   DEVICE_CATEGORY_SPEAKER : {(  1, -2400), ( 33, -1800), ( 66, -1200), (100,  -600) }
   DEVICE_CATEGORY_EARPIECE : {(  1, -2400), ( 33, -1800), ( 66, -1200), (100,  -600) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -2100), (100, -1000) }
 AUDIO_STREAM_DTMF (08): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  1, -3000), ( 33, -2600), ( 66, -2200), (100, -1800) }
   DEVICE_CATEGORY_SPEAKER : {(  1, -2400), ( 33, -1800), ( 66, -1200), (100,  -600) }
   DEVICE_CATEGORY_EARPIECE : {(  1, -2400), ( 33, -1800), ( 66, -1200), (100,  -600) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -2100), (100, -1000) }
 AUDIO_STREAM_TTS (09): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  0, -9600), (100, -9600) }
   DEVICE_CATEGORY_SPEAKER : {(  0,     0), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  0, -9600), (100, -9600) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  0, -9600), (100, -9600) }
 AUDIO_STREAM_ACCESSIBILITY (10): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  1, -5800), ( 20, -4000), ( 60, -1700), (100,     0) }
 AUDIO_STREAM_REROUTING (11): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  0,     0), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  0,     0), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  0,     0), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  0,     0), (100,     0) }
 AUDIO_STREAM_PATCH (12): Curve points for device category (index, attenuation in millibel)
   DEVICE_CATEGORY_HEADSET : {(  0,     0), (100,     0) }
   DEVICE_CATEGORY_SPEAKER : {(  0,     0), (100,     0) }
   DEVICE_CATEGORY_EARPIECE : {(  0,     0), (100,     0) }
   DEVICE_CATEGORY_EXT_MEDIA : {(  0,     0), (100,     0) }
Total Effects CPU: 0.000000 MIPS, Total Effects memory: 0 KB, Max memory used: 0 KB
Registered effects:                   //音效
- Effect 11 dump:
 I/O: 13
 Strategy: 0
 Session: 0
 Name: Dirac Shared Controller
 Disabled
- Effect 19 dump:
 I/O: 13
 Strategy: 0
 Session: 0
 Name: Dirac [music]
 Enabled
Audio Patches:                               //audiopatch
  Audio patch 1:
  - handle:  1
  - audio flinger handle: 12
  - owner uid: 1041
  - 1 sources:
    - Mix ID 3 I/O handle 13
  - 1 sinks:
    - Device ID 2 AUDIO_DEVICE_OUT_SPEAKER
  Audio patch 2:
  - handle:  2
  - audio flinger handle: 20
  - owner uid: 1041
  - 1 sources:
    - Mix ID 4 I/O handle 21
  - 1 sinks:
    - Device ID 2 AUDIO_DEVICE_OUT_SPEAKER
```
