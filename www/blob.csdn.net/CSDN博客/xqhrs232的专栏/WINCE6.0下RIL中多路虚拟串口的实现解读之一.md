# WINCE6.0下RIL中多路虚拟串口的实现解读之一 - xqhrs232的专栏 - CSDN博客
2010年02月04日 11:03:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2344
710MUX多路复用驱动程序Mux07_10.dll把一路实际的物理串口虚拟成两路串口。
1。一路虚拟串口是COM7-----用于一般的AT 命令发送
2。一路虚拟串口是COM9-----用于ppp connection over CSD / GPRS
看对应的注册表项就可以清楚，下面的内容来自C:/WINCE600/PLATFORM/DEVICEEMULATOR/FILES/platform.reg文件
 [HKEY_LOCAL_MACHINE/Drivers/BuiltIn/RIL]
            "Dll"="rilgsm.dll"
            ;DEVFLAGS_LOADLIBRARY    0x00000002      // Use LoadLibrary instead of LoadDriver
            "Flags"=dword:2
            "Index"=dword:1
            "Order"=dword:3
            "Prefix"="RIL"                
            "ComPort"="COM7:"
            "RebootOKEvName"="Mux0710_RebootOK"    ; name of the event used to signal reboot OK
            "RebootKOEvName"="Mux0710_RebootKO"    ; name of the event used to signal reboot failure
        [HKEY_LOCAL_MACHINE/Drivers/BuiltIn/GSM0710_1]
            "Prefix"="COM"
            "Dll"="Mux07_10.dll"
            "DeviceArrayIndex"=dword:1
            "Index"=dword:7
            "Order"=dword:2
            ; power manager enabled device driver
            "IClass"="{A32942B7-920C-486b-B0E6-92A702A99B35}"
            ; GSM 07.10 specific
            "GSM_0710_DLCI"=dword:1   ; Rem: Dlci also define priority (lowest is low prio)
            "GSM_0710_TYPE"=dword:1   ; boolean: 1 (true) is for AT cmd, 0 (false) is for Data
        ; COM9 for ppp connection over CSD / GPRS                    
        [HKEY_LOCAL_MACHINE/Drivers/BuiltIn/GSM0710_2]
            "Prefix"="COM"
            "Dll"="Mux07_10.dll"
            "DeviceArrayIndex"=dword:1
            "Index"=dword:9
            "Order"=dword:3
            ; power manager enabled device driver
            "IClass"="{A32942B7-920C-486b-B0E6-92A702A99B35}"
            ; GSM 07.10 specific
            "GSM_0710_DLCI"=dword:2    ; Rem: Dlci also define priority (lowest is low prio)
            "GSM_0710_TYPE"=dword:1    ; boolean: 1 (true) is for AT cmd, 0 (false) is for Data
        ; COM1 for underlying GSM 07.10
        [HKEY_LOCAL_MACHINE/Software/Microsoft/GSM07_10]
            "ComPort"="COM1:"
            "BaudRate"=dword:1C200    ; 115200
            "FrameOption"=dword:1     ; Advanced
再看Mux07_10.dll驱动程序对应的注册表项，下面的内容来自C:/WINCE600/PUBLIC/CELLCORE/710MUX/mux07_10.reg
; COM7 for RIL AT command
; ----------------------------------------
[HKEY_LOCAL_MACHINE/Drivers/BuiltIn/RIL]
    "ComPort"="COM7:"
 "RebootOKEvName"="Mux0710_RebootOK"  ; name of the event used to signal reboot OK
 "RebootKOEvName"="Mux0710_RebootKO"  ; name of the event used to signal reboot failure
 ;Init string: $VSELECT=1|%DATA=2,"UART",0,2,"SER","UART",0,1| => handset for voice,redirect PPP data from DLCI 1 to DLCI 2
 "ComInitString"=hex:/
 24,00,56,00,53,00,45,00,4c,00,45,00,43,00,54,00,3d,00,31,00,7c,00,/
 25,00,44,00,41,00,54,00,41,00,3d,00,32,00,2c,00,22,00,55,00,41,00,52,00,54,00,22,/
 00,2c,00,30,00,2c,00,32,00,2c,00,22,00,53,00,45,00,52,00,22,00,2c,00,22,00,55,00,/
    41,00,52,00,54,00,22,00,2c,00,30,00,2c,00,31,00,7c,00,00,00
[HKEY_LOCAL_MACHINE/Drivers/BuiltIn/GSM0710_1]
    "Prefix"="COM"
    "Dll"="Mux07_10.dll"
    "DeviceArrayIndex"=dword:1
    "Index"=dword:7
    "Order"=dword:2
 ; power manager enabled device driver
 ;-------------------------------------
 "IClass"="{A32942B7-920C-486b-B0E6-92A702A99B35}" 
 ; GSM 07.10 specific
 ;---------------------
 "GSM_0710_DLCI"=dword:1  ; Rem: Dlci also define priority (lowest => low prio)
 "GSM_0710_TYPE"=dword:1  ; boolean: 1 (true) => AT cmd, 0 (false) => Data 
 ; monitor channel activity
 ;---------------------------
 "GSM_0710_ACTIVITY_TIMEOUT"=dword:2710  ; time-out on channel activity in ms
            ;
            ; remark:
            ; the Mux driver will notify the device
            ; when this time-out expire 
            ; if there is no activity
; COM9 for ppp connection over CSD / GPRS
; ----------------------------------------
[HKEY_LOCAL_MACHINE/Drivers/BuiltIn/GSM0710_2]
    "Prefix"="COM"
    "Dll"="Mux07_10.dll"
    "DeviceArrayIndex"=dword:1
    "Index"=dword:9
    "Order"=dword:3
 ; power manager enabled device driver
 ;-------------------------------------
 "IClass"="{A32942B7-920C-486b-B0E6-92A702A99B35}" 
 ; GSM 07.10 specific
 ;---------------------
 "GSM_0710_DLCI"=dword:2  ; Rem: Dlci also define priority (lowest => low prio)
 "GSM_0710_TYPE"=dword:1  ; boolean: 1 (true) => AT cmd, 0 (false) => Data
 ; monitor channel activity
 ;---------------------------
 "GSM_0710_ACTIVITY_TIMEOUT"=dword:4E20  ; time-out on channel activity in ms
            ;
            ; remark:
            ; the Mux driver will notify the device
            ; when this time-out expire 
            ; if there is no activity
; COM2 for underlying GSM 07.10 
[HKEY_LOCAL_MACHINE/Software/Microsoft/GSM07_10]
    "ComPort"="COM2:"
    "BaudRate"=dword:1C200    ; 115200
    "FrameOption"=dword:1               ; 1 - Advanced; 0 -Basic
    "FrameSize"=dword:00000040          ; max data size 
//##########################################################################################################
从上面的内容可以看出Mux07_10.dll被加载2次，虚拟出两个虚拟串口，COM7+COM9，COM7用于一般的AT命令，COM9用于ppp connection over CSD / GPRS。
但上面的两个文件的对应用到的物理串口不一样，这个是要特别注意的地方。
 ; COM1 for underlying GSM 07.10
        [HKEY_LOCAL_MACHINE/Software/Microsoft/GSM07_10]------platform.reg文件
            "ComPort"="COM1:"
            "BaudRate"=dword:1C200    ; 115200
            "FrameOption"=dword:1     ; Advanced
//----------------------------------------------------------------------------
; COM2 for underlying GSM 07.10 
[HKEY_LOCAL_MACHINE/Software/Microsoft/GSM07_10]-------------mux07_10.reg
    "ComPort"="COM2:"
    "BaudRate"=dword:1C200    ; 115200
    "FrameOption"=dword:1               ; 1 - Advanced; 0 -Basic
    "FrameSize"=dword:00000040          ; max data size 
注册表的统一，估计要用到编译的时候，相同的注册表项会覆盖的功能！！！-----注意底层实际的物理串口只能有一个。
看到物理串口要虚拟出两个物理串口，所以Mux07_10.dll驱动必须先于rilgsm.dll被加载，因为rilgsm.dll要用虚拟出的COM7来进行AT命令发送。
看系统编译后的reginit.ini文件就应该可以清楚最后的注册表情况，对于[HKEY_LOCAL_MACHINE/Software/Microsoft/GSM07_10]注册表项是谁覆盖了谁。
