# Ubuntu下 Android真机调试 - 三少GG - CSDN博客
2011年11月25日 14:41:19[三少GG](https://me.csdn.net/scut1135)阅读数：1364
首先非常不错到视频：
[http://www.tudou.com/programs/view/ldKvE3BW7Lw/](http://www.tudou.com/programs/view/ldKvE3BW7Lw/)
上面到研究透即可。
配合的指令：  adb kill-server    adb devices
sudo service udev restart
lsusb
相关的官网参考： [http://developer.android.com/guide/developing/device.html](http://developer.android.com/guide/developing/device.html)
- If you're developing on Ubuntu Linux, you need to add a [`udev`](http://www.kernel.org/pub/linux/utils/kernel/hotplug/udev.html) rules file that contains a USB configuration for each type of deviceyou want to use for development. In the rules file, each device manufactureris identified by a unique vendor ID, as specified by the`ATTR{idVendor}` property.
 For a list of vendor IDs, see [USB Vendor IDs](http://developer.android.com/guide/developing/device.html#VendorIds), below. To set up device detection onUbuntu Linux:
- Log in as root and create this file: `/etc/udev/rules.d/51-android.rules`.
Use this format to add each vendor to the file:
`SUBSYSTEM=="usb", ATTR{idVendor}=="0bb4", MODE="0666", GROUP="plugdev"`
In this example, the vendor ID is for HTC. The `MODE`assignment specifies read/write permissions, and`GROUP` defineswhich Unix group owns the device node.
**Note:** The rule syntaxmay vary slightly depending on your environment. Consult the`udev`documentation for your system as needed. For an overview of rule syntax, seethis guide to[writing
 udevrules](http://www.reactivated.net/writing_udev_rules.html).
- Now execute:
`chmod a+r /etc/udev/rules.d/51-android.rules`
- **以上a,b两个步骤完成后，拔出手机USB后重新插入即可。**
You can verify that your device is connected by executing `adbdevices` from your SDK`platform-tools/` directory. If connected,you'll see the device name listed as a "device."
If using Eclipse, run or debug your application as usual. You will bepresented with a**Device Chooser** dialog that lists the availableemulator(s) and connected device(s). Select the device upon which you want toinstall and run the application.
If using the [AndroidDebug Bridge](http://developer.android.com/guide/developing/tools/adb.html) (adb), you can issue commands with the `-d` flag totarget your connected device.
### USB Vendor IDs
This table provides a reference to the vendor IDs needed in order to add USBdevice support on Linux. The USB Vendor ID is the value given to the`ATTR{idVendor}` property in the rules file, as described above.
|Company|USB Vendor ID|
|----|----|
|Acer|`0502`|
|ASUS|`0B05`|
|Dell|`413C`|
|Foxconn|`0489`|
|Garmin-Asus|`091E`|
|Google|`18D1`|
|HTC|`0BB4`|
|Huawei|`12D1`|
|K-Touch|`24E3`|
|KT Tech|`2116`|
|Kyocera|`0482`|
|Lenevo|`17EF`|
|LG|`1004`|
|Motorola|`22B8`|
|NEC|`0409`|
|Nook|`2080`|
|Nvidia|`0955`|
|OTGV|`2257`|
|Pantech|`10A9`|
|Pegatron|`1D4D`|
|Philips|`0471`|
|PMC-Sierra|`04DA`|
|Qualcomm|`05C6`|
|SK Telesys|`1F53`|
|Samsung|`04E8`|
|Sharp|`04DD`|
|Sony Ericsson|`0FCE`|
|Toshiba|`0930`|
|ZTE|`19D2`|
