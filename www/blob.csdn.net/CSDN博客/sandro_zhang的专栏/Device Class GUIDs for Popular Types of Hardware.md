# Device Class GUIDs for Popular Types of Hardware - sandro_zhang的专栏 - CSDN博客
2011年09月19日 10:22:49[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：972
今天发现驱动的类的GUID都是唯一的，不能随意更改，找到了一个列表，记录下来。
Other than programming for a device [driver](http://xpdrivers.com/tag/driver/), the Globally Unique Identifier (GUID) for a
 device class is useful when tracking down [driver](http://xpdrivers.com/tag/driver/) information in the Windows Registry.
For example, the solution to several [Device Manager errors](http://xpdrivers.com/troubleshooting/device-manager-error-codes/) include removing specific registry entries after a device’s GUID.
**Note:** This is not a complete list of device class GUIDs. Devices can generate unique classes based on various variables making it virtually impossible to list them all.
|Device|Class|Class GUID|Notes|
|----|----|----|----|
|Battery Devices|Battery|{72631e54-78a4-11d0-bcf7-00aa00b7b32a}|This class includes battery devices and UPS devices.|
|Biometric Device|Biometric|{53D29EF7-377C-4D14-864B-EB3A85769359}|Starting with Windows Server 2003, this class includes all biometric-based personal identification devices.|
|Bluetooth Devices|Bluetooth|{e0cbf06c-cd8b-4647-bb8a-263b43f0f974}|Starting with Windows XP SP1, this class includes all Bluetooth devices.|
|CD-ROM Drives|CDROM|{4d36e965-e325-11ce-bfc1-08002be10318}|This class includes CD-ROM drives, including SCSI CD-ROM drives. By default, the system’s CD-ROM class installer also installs a system-supplied CD audio driver and CD-ROM changer driver as Plug and Play filters.|
|Disk Drives|DiskDrive|{4d36e967-e325-11ce-bfc1-08002be10318}|This class includes hard disk drives. See also the HDC and SCSIAdapter classes.|
|Display Adapters|Display|{4d36e968-e325-11ce-bfc1-08002be10318}|This class includes video adapters. Drivers for this class include display drivers and video miniport drivers.|
|Floppy Disk Controllers|FDC|{4d36e969-e325-11ce-bfc1-08002be10318}|This class includes floppy disk drive controllers.|
|Floppy Disk Drives|FloppyDisk|{4d36e980-e325-11ce-bfc1-08002be10318}|This class includes floppy disk drives.|
|Hard Disk Controllers|HDC|{4d36e96a-e325-11ce-bfc1-08002be10318}|This class includes hard disk controllers, including ATA/ATAPI controllers but not SCSI and RAID disk controllers.|
|Human Interface Devices (HID)|HIDClass|{745a17a0-74d3-11d0-b6fe-00a0c90f57da}|This class includes interactive input devices that are operated by the system-supplied [HID class driver](http://msdn.microsoft.com/en-us/library/ms790197.aspx). This includes USB devices that comply with the [USB HID Standard](http://msdn.microsoft.com/en-us/library/ms789907.aspx) and non-USB devices that use a HID minidriver. For more information, see [HIDClass Device Setup Class](http://msdn.microsoft.com/en-us/library/ms790192.aspx).. (See also the Keyboard or Mouse classes later in this list.)|
|IEEE 1284.4 Devices|Dot4|{48721b56-6795-11d2-b1a8-0080c72e74a2}|This class includes devices that control the operation of multifunction IEEE 1284.4 peripheral devices.|
|IEEE 1284.4 Print Functions|Dot4Print|{49ce6ac8-6f86-11d2-b1e5-0080c72e74a2}|This class includes Dot4 print functions. A Dot4 print function is a function on a Dot4 device and has a single child device, which is a member of the Printer device setup class.|
|IEEE 1394 Devices That Support the 61883 Protocol|61883|{7ebefbc0-3200-11d2-b4c2-00a0C9697d07}|This class includes IEEE 1394 devices that support the IEC-61883 protocol device class.The 61883 component includes the *61883.sys*protocol driver that transmits various audio and video data streams over the 1394 bus. These currently include standard/high/low quality DV, MPEG2, DSS, and Audio. These data streams are defined by the IEC-61883 specifications.|
|IEEE 1394 Devices That Support the AVC Protocol|AVC|{c06ff265-ae09-48f0-812c-16753d7cba83}|This class includes IEEE 1394 devices that support the AVC protocol device class.|
|IEEE 1394 Devices That Support the SBP2 Protocol|SBP2|{d48179be-ec20-11d1-b6b8-00c04fa372a7}|This class includes IEEE 1394 devices that support the SBP2 protocol device class.|
|IEEE 1394 Host Bus Controller|1394|{6bdd1fc1-810f-11d0-bec7-08002be2092f}|This class includes 1394 host controllers connected on a PCI bus, but not 1394 peripherals. Drivers for this class are system-supplied.|
|Imaging Device|Image|{6bdd1fc6-810f-11d0-bec7-08002be2092f}|This class includes still-image capture devices, digital cameras, and scanners.|
|IrDA Devices|Infrared|{6bdd1fc5-810f-11d0-bec7-08002be2092f}|This class includes infrared devices. Drivers for this class include Serial-IR and Fast-IR NDIS miniports, but see also the Network Adapter class for other NDIS network adapter miniports.|
|Keyboard|Keyboard|{4d36e96b-e325-11ce-bfc1-08002be10318}|This class includes all keyboards. That is, it must also be specified in the (secondary) INF for an enumerated child HID keyboard device.|
|Media Changers|MediumChanger|{ce5939ae-ebde-11d0-b181-0000f8753ec4}|This class includes SCSI media changer devices.|
|Memory Technology Driver|MTD|{4d36e970-e325-11ce-bfc1-08002be10318}|This class includes memory devices, such as flash memory cards.|
|Modem|Modem|{4d36e96d-e325-11ce-bfc1-08002be10318}|This class includes [modem devices](http://msdn.microsoft.com/en-us/library/ms795937.aspx). An INF file for a device of this class specifies the features and configuration of the device and stores this information in the registry. An INF file for a device of this class can also be used to install device drivers for a [*controllerless modem*](http://msdn.microsoft.com/en-us/library/ms789538.aspx) or a [software modem](http://msdn.microsoft.com/en-us/library/ms789546.aspx). These devices split the functionality between the modem device and the device driver. For more information about modem INF files and Microsoft Windows Driver Model (WDM) modem devices, see [Overview of Modem INF Files](http://msdn.microsoft.com/en-us/library/ms795219.aspx)and [Adding WDM Modem Support](http://msdn.microsoft.com/en-us/library/ms795880.aspx).|
|Monitor|Monitor|{4d36e96e-e325-11ce-bfc1-08002be10318}|This class includes display monitors. An INF for a device of this class installs no device driver(s), but instead specifies the features of a particular monitor to be stored in the registry for use by drivers of video adapters. (Monitors are enumerated as the child devices of display adapters.)|
|Mouse|Mouse|{4d36e96f-e325-11ce-bfc1-08002be10318}|This class includes all mouse devices and other kinds of pointing devices, such as trackballs. That is, this class must also be specified in the (secondary) INF for an enumerated child HID mouse device.|
|Multifunction Devices|Multifunction|{4d36e971-e325-11ce-bfc1-08002be10318}|This class includes combo cards, such as a PCMCIA modem and netcard adapter. The driver for such a Plug and Play multifunction device is installed under this class and enumerates the modem and netcard separately as its child devices.|
|Multimedia|Media|{4d36e96c-e325-11ce-bfc1-08002be10318}|This class includes Audio and DVD multimedia devices, joystick ports, and full-motion video capture devices.|
|Multiport Serial Adapters|MultiportSerial|{50906cb8-ba12-11d1-bf5d-0000f805f530}|This class includes intelligent multiport serial cards, but not peripheral devices that connect to its ports. It does not include unintelligent (16550-type) multiport serial controllers or single-port serial controllers (see the Ports class).|
|Network Adapter|Net|{4d36e972-e325-11ce-bfc1-08002be10318}|This class includes NDIS miniport drivers excluding Fast-IR miniport drivers, NDIS intermediate drivers (of virtual adapters), and CoNDIS MCM miniport drivers.|
|Network Client|NetClient|{4d36e973-e325-11ce-bfc1-08002be10318}|This class includes network and/or print providers.|
|Network Service|NetService|{4d36e974-e325-11ce-bfc1-08002be10318}|This class includes network services, such as redirectors and servers.|
|Network Transport|NetTrans|{4d36e975-e325-11ce-bfc1-08002be10318}|This class includes NDIS protocols, CoNDIS stand-alone call managers, and CoNDIS clients, in addition to higher level drivers in transport stacks.|
|PCI SSL Accelerator|Security Accelerator|{268c95a1-edfe-11d3-95c3-0010dc4050a5}|This class includes devices that accelerate secure socket layer (SSL) cryptographic processing.|
|PCMCIA Adapters|PCMCIA|{4d36e977-e325-11ce-bfc1-08002be10318}|This class includes PCMCIA and CardBus host controllers, but not PCMCIA or CardBus peripherals. Drivers for this class are system-supplied.|
|Ports (COM & LPT ports)|Ports|{4d36e978-e325-11ce-bfc1-08002be10318}|This class includes serial and parallel port devices. See also the MultiportSerial class.|
|Printers|Printer|{4d36e979-e325-11ce-bfc1-08002be10318}|This class includes printers.|
|Printers, Bus-specific class drivers|PNPPrinters|{4658ee7e-f050-11d1-b6bd-00c04fa372a7}|This class includes SCSI/1394-enumerated printers. Drivers for this class provide printer communication for a specific bus.|
|Processors|Processor|{50127dc3-0f36-415e-a6cc-4cb3be910b65}|This class includes processor types.|
|SCSI and RAID Controllers|SCSIAdapter|{4d36e97b-e325-11ce-bfc1-08002be10318}|This class includes SCSI HBAs (Host Bus Adapters) and disk-array controllers.|
|Smart Card Readers|SmartCardReader|{50dd5230-ba8a-11d1-bf5d-0000f805f530}|This class includes smart card readers.|
|Storage Volumes|Volume|{71a27cdd-812a-11d0-bec7-08002be2092f}|This class includes storage volumes as defined by the system-supplied logical volume manager and class drivers that create device objects to represent storage volumes, such as the system disk class driver.|
|System Devices|System|{4d36e97d-e325-11ce-bfc1-08002be10318}|This class includes HALs, system buses, system bridges, the system ACPI driver, and the system volume manager driver.|
|Tape Drives|TapeDrive|{6d807884-7d21-11cf-801c-08002be10318}|This class includes tape drives, including all tape miniclass drivers.|
|USB|USB|{36fc9e60-c465-11cf-8056-444553540000}|This class includes USB host controllers and USB hubs, but not USB peripherals. Drivers for this class are system-supplied.|
|Windows CE USB ActiveSync Devices|WCEUSBS|{25dbce51-6c8f-4a72-8a6d-b54c2b4fc835}|This class includes Windows CE ActiveSync devices.The WCEUSBS setup class supports communication between a personal computer and a device that is compatible with the Windows CE ActiveSync driver (generally, PocketPC devices) over USB.|
|Windows SideShow|SideShow|{997b5d8d-c442-4f2e-baf3-9c8e671e9e21}|This class includes all devices that are compatible with Windows SideShow. This class is supported in Windows Vista and later versions of Windows.|
### System-Defined Device Setup Classes Reserved for System Use
|Device|Class|Class GUID|Notes|
|----|----|----|----|
|Adapter|Adapter|{4d36e964-e325-11ce-bfc1-08002be10318}|This class is obsolete.|
|APM|APMSupport|{d45b1c18-c8fa-11d1-9f77-0000f805f530}|This class is reserved for system use.|
|Computer|Computer|{4d36e966-e325-11ce-bfc1-08002be10318}|This class is reserved for system use.|
|Decoders|Decoder|{6bdd1fc2-810f-11d0-bec7-08002be2092f}|This class is reserved for future use.|
|Global Positioning System|GPS|{6bdd1fc3-810f-11d0-bec7-08002be2092f}|This class is reserved for future use.|
|Host-side IEEE 1394 Kernel Debugger Support|1394Debug|{66f250d6-7801-4a64-b139-eea80a450b24}|This class is reserved for system use.|
|IEEE 1394 IP Network Enumerator|Enum1394|{c459df55-db08-11d1-b009-00a0c9081ff6}|This class is reserved for system use.|
|No driver|NoDriver|{4d36e976-e325-11ce-bfc1-08002be10318}|This class is obsolete.|
|Non-Plug and Play Drivers|LegacyDriver|{8ecc055d-047f-11d1-a537-0000f8753ed1}|This class is reserved for system use.|
|Other Devices|Unknown|{4d36e97e-e325-11ce-bfc1-08002be10318}|This class is reserved for system use. Enumerated devices for which the system cannot determine the type are installed under this class. Do not use this class if you are unsure in which class your device belongs. Either determine the correct device setup class or create a new class.|
|Printer Upgrade|Printer Upgrade|{4d36e97a-e325-11ce-bfc1-08002be10318}|This class is reserved for system use.|
|Sound|Sound|{4d36e97c-e325-11ce-bfc1-08002be10318}|This class is obsolete.|
|Storage Volume Snapshots|VolumeSnapshot|{533c5b84-ec70-11d2-9505-00c04F79deaf}|This class is reserved for system use.|
原始链接：[http://xpdrivers.com/troubleshooting/device-class-guids-for-popular-types-of-hardware/](http://xpdrivers.com/troubleshooting/device-class-guids-for-popular-types-of-hardware/)
