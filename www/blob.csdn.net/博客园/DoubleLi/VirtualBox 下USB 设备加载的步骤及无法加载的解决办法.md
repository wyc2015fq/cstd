# VirtualBox 下USB 设备加载的步骤及无法加载的解决办法 - DoubleLi - 博客园






1. 将u盘插入电脑，在windows能够识别后，进入VirtualBox的选定要加载的虚拟机，选择设置——》选择usb设备项



2. 选择右边的第一或者是第二个图标，增加一个新的筛选器，第一个图标是增加一个空的筛选器，能够加载任何的usb设备。第二图标是加载host中usb列表的设备。

3. 增加筛选器后，通过windows的安全删除硬件方式卸载usb。

4. 启动 guest，进入ubuntu，此时应该会提示windows发现了virtual box usb驱动，指定驱动为virtual box安装目录C:/Program Files/Oracle/VirtualBox/drivers/USB/device下，然后进行安装。

5. 在启动完毕后，在保证guest系统选中的情况下，插入u盘，如果是第一次使用，在虚拟机的菜单的设备——》分配usb设备中，选择要加载的usb，然后虚拟机的右下角的usb图标会闪动，如果设置正确，ubuntu上就出现你的u盘的图标，然后就可以访问了。



注意：

1. 如果在设置后，然后选择了指定的usb设备后，无法在ubuntu中加载成功，并且再次选中是会弹出提示框，提示：USB device '<unknown>' with UUID {82ab2689-6d62-4878-be58-b97207388811} is busy with a previous request. Please try again later.


返回代码:
E_INVALIDARG (0x80070057)
组件:
HostUSBDevice
界面:
IHostUSBDevice {173b4b44-d268-4334-a00d-b6521c9a740a}
被召者:
IConsole {d5a1cbda-f5d7-4824-9afe-d640c94c7dcf}

这个问题我折腾了很久，一直无法解决，后来在oracle的论坛上看到一个回复，说他那也是同样的问题，后来把AMD usb filterdriver去除了就ok了，按他的方法去除了这个驱动，就能够正常加载了。

在另外一台电脑上也同样出现相同的问题，经过检查，上面安装了moto的手机驱动程序，只要把该驱动去除，就能够正常的加载usb设备了。

如果在设置那增加了过滤器后，没有提示发现virtualbox usb设备的话，很可能与这有关系，或者是你安装了其他的驱动，屏蔽了usb驱动，让virtualbox无法捕捉到usb的东西，所以就无法正确让guest使用usb。







Linux 下 VirtualBox 虚拟机 USB 设备的使用方法

[http://www.zzzke.org.cn/H/wd/10024109/02e74f10e0327ad868d138f2b4fdd6f0.html](http://www.zzzke.org.cn/H/wd/10024109/02e74f10e0327ad868d138f2b4fdd6f0.html)



一个关于虚拟机的很好的网站：

[http://www.vmbest.com/school/2-virtualbox-school](http://www.vmbest.com/school/2-virtualbox-school)



以上内容摘抄至：http://blog.csdn.net/lynxzong/article/details/6078089


以下内容摘抄至：http://hi.baidu.com/scolar/item/a232058c3ffdec874514cf5d


## 手工卸载AMD USB Filter Driver驱动


win7x64里Catalyst Install Manager竟然说我什么驱动都没装。

于是看AMD的文档，找到手工卸载的方法。

Installing and Uninstalling the Driver

These instructions explain how to install/uninstall the USB filter driver:

Note: For driver installation, you must log in as administrator or have administrator rights for your domain login.

The driver can be installed either by running the CIM installer or by right clicking the inf file and selecting install.
1. Install/Uninstall Using INF File



To install

1. Right-click on the usbfilter.inf file, and then select Install.

2. When installation is complete, reboot the computer.
Note: Using the above installation method will cause the Device Manger to show Usbfilter.sys to be unsigned. This is Device Manger issue and the driver is WHQL signed. If the driver is not WHQL signed, Windows will give a warning while installing the driver.

To uninstall

1. Open the registry and scroll down to:

HKLM\System\CCS\Control\Class\{36FC9E60-C465-11CF-8056-444553540000}

2. Remove usbfilter from UpperFilter.

3. Open the HKLM\System\CCS\Services registry and delete the usbfilter key.

4. Delete usbfilter.sys from:

\%system32%\drivers

5. Reboot the computer.
貌似只有AMD USB Filter Driver支持这种卸载方式，也只有这个有虚拟机问题。









