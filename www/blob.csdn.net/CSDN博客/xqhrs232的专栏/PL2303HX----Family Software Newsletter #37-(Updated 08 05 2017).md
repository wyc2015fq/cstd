# PL2303HX----Family Software Newsletter #37 - (Updated 08/05/2017) - xqhrs232的专栏 - CSDN博客
2017年08月12日 20:56:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：291
原文地址::[http://www.ifamilysoftware.com/news37.html](http://www.ifamilysoftware.com/news37.html)
**Family Software Newsletter #37 - (Updated 08/05/2017)****Drag Racing Computers and Software**
**Copyright 2014, Family Software. All rights reserved.**
Option 1 $10.00 USD
Option  2 $15.00 USDOption 3 $20.00 USD
Option  4 $25.00 USDOption 5 $30.00 USD![](https://www.paypalobjects.com/en_US/i/btn/x-click-butcc-donate.gif)
**Prolific USB To Serial Driver "Code 10" Fix!**
**Windows 32 and 64-bit Operating Systems - Prolific PL-2303 Driver Fix (VID_067B&PID_2303)**
**For Windows XP, Windows 7, Windows 8, Windows 8.1 and Windows 10!**
"This Device cannot start (Code 10)"
"No driver installed for this device"
"Device driver was not successfully installed"
The Internet is full of web pages discussing this problem, and hardly anyone really seems to understand why it's happening. Even the companies selling these USB to Serial adapters and cables
 appear to be either dumbfounded or have selective amnesia. After all, it's absolutely great for business. Unfortunately there are hundreds of legacy devices that can no longer be used on newer computers running Windows 7, 8, 8.1, or 10. There are many "backyard"
 fixes out there, but none of them are done properly and if they work it's just a temporary "Band-Aid" to the problem. It will come back to haunt you.
What has happened is that there have been counterfeit "Prolific" chips coming from China. The counterfeit chips use the same Vendor ID (VID_067B) and Product ID (PID_2303) as the authentic
 Prolific chips. So, Prolific made a new chip and dropped all support for the old one thereby rendering the counterfeit chips unusable. Unfortunately, it renders hundreds of legacy devices using the earlier chip inoperative. Getting a working driver installed
 by the average user is almost impossible.
If one of these Prolific drivers gets installed to your Windows 64-bit computer then your legacy device will no longer work and will issue the generic "Code 10" error. Driver Versions:
3.3.5.122, 3.3.11.152, 3.3.17.203, 3.4.25.218, 3.4.31.231, 3.4.36.247 3.4.48.272, 3.4.62.293
3.4.67.325, 3.4.67.333, 3.6.78.350, 3.6.81.357, 3.8.3.0*, 3.81.10.0*, 3.8.12.0* (*New additions in this version)
Or, you may get no error at all, but your device will not work. If your adapter was working prior to going to Windows Update, you may be able to "roll back" to the previous installed driver
 and all will be well for awhile. However, if you didn't have a previous driver installed that worked, you'll have to go through the process of removing any PL-2303 driver installation programs,the actual driver files, and the information (.INF) files BEFORE
 you are able to successfully install the correct driver. What aggravates the issue is that there are many "Prolific Driver Removal Tools" that do not work properly as well! And Windows 8, 8.1, and Window 10 are set by default to automatically update your drivers
 without your permission or even notifying you of the update. So, no matter how many times you remove the driver files and reboot, the next time you insert the USB-To-Serial adapter, Windows installs the newest non-working version again. Yes, it's frustrating.
The best 64-bit driver I have found that works with all the "Prolific" adapters is old Version 3.3.2.102. Our "PL2303 Code 10 Fix" program will remove all incompatible drivers, make a change
 in the registry so that Windows can never update the driver without your permission, and install the Version 3.3.2.102 compatible drivers.
**Windows 64-bit Fix for:**
- All Windows 64-bit operating systems including Windows 10 
.Prolific USB to Serial Adapter OR other device. 
.Device using PL-2303 H, HX, HXA, HXD, X, XA, EA, RA, SA, TA, TD version chips
.Driver Version: 3.3.2.102
.Driver Date: 09/29/08
.Supported device ID and product strings: . VID_067B&PID_2303 for "Prolific USB-to-Serial Comm Port"
1. Download and Save the "PL2303_64bit_ Installer.exe" at the link below: 
[http://www.ifamilysoftware.com/Drivers/PL2303_64bit_Installer.exe](http://www.ifamilysoftware.com/Drivers/PL2303_64bit_Installer.exe)
You can just Save it to your Desktop to make it easier. Norton's 360 won't like it, so to save all the grief of dealing with that beast you might want to disable your anti-virus before running the installer.
2. Unplug all USB-To-Serial adapters and Double click on the installer "PL2303_64bit_Installer.exe". This file can be deleted later.
3. The "PL2303 Code 10 Fix" program will be installed along with a shortcut on your Windows Desktop.
4. Double click on the "PL2303 Code 10 Fix Shortcut" 
5. Plug in one (1) of your USB-To-Serial adapters and click "Continue".
6. When it finishes, Reboot your computer.
7. Unplug the adapter and plug back in again. That's it! Donations are appreciated.
**(Our program is Free for individual use, but cannot be distributed! Theft of intelligent property is a crime.)**
**Trouble Shooting:** You must follow ever step in the process precisely. If you still receive an error after running the "PL2303 Code 10 Fix" and your device is plugged in, go
 to the Windows Device Manager. Scroll down to Ports (Com & LPT) and Double-Click on "Prolific USB-to-Serial Comm Port (COM#)". In the Properties Window, Click on "Driver". The "Driver Version" must say "3.3.2.102" dated 09/24/08. If not, then the correct driver
 is not installed. Unplug the USB-To-Serial adapter and run the "PL2303 Code 10 Fix" again, following the directions precisely until you get it right. 
**Removal: **To remove the "PL2303 Code 10 Fix" from you computer, go to your Control Panel and un-install like any Windows program. Removing the program will not delete the drivers. The drivers can be deleted in the Device Manager.
**Windows 32-bit Fix for:**- All Windows 32-bit operating systems from XP up
.Prolific USB to Serial Adapter . 
.Device using PL-2303 H/HXA/HX/X version chips 
.Driver Version: 2.0.2.8
.Driver Date: 11/20/07
.Supported device ID and product strings: . VID_067B&PID_2303 for "Prolific USB-to-Serial Comm Port"
1. Download and Save the "PL-2303_Driver_ Installer.exe" program at the link below: 
[http://www.ifamilysoftware.com/Drivers/PL-2303_Driver_Installer.exe](http://www.ifamilysoftware.com/Drivers/PL-2303_Driver_Installer.exe)
You can just Save it to your Desktop to make things easy.
2. Run the installer program. If it offers a choice to remove the driver, then select to remove the current "bad" driver. Then run the installer again to install the correct driver. 
**Trouble Shooting:** If you still receive an error after running the PL-2303_Driver_Installer.exe and your device is plugged in, go to the Windows Device Manager. Scroll down to Ports (Com & LPT) and Double-click on "Prolific USB-to-Serial Comm
 Port (COM#)". In the Properties Window, Click on "Driver". The "Driver Version" must say "2.0.2.8" dated 11/20/07. If not, then the correct driver is not installed. Unplug the USB-To-Serial adapter and run the "PL2303_Driver_Installer.exe" again, following
 the directions precisely until the correct driver appears in the Device Manager.
**Removal:** The Prolific 32-bit PL-2303_Driver_Installer is a "Program" file and therefore installed to your computer and must stay. If you want to delete it, use the Windows
 "Un-install a Program" or "Add or Remove Program" feature, however this will also un-install the driver itself.
**Windows 32-bit Files Direct Install:**For XP, Vista, and Windows 7 users that cannot get the driver files installed using the PL-2303_Driver_Installer above, you can also try having Windows install the files directly. Download and run the file "PL2303Win32.exe" installer program from the
 link below:
[http://www.ifamilysoftware.com/Drivers/PL-2303_Win32.exe](http://www.ifamilysoftware.com/Drivers/PL2303_Win32.exe)
This will install the driver files in a directory on your hard drive named, "C:\PL2303Win32". Plug in your Prolific USB to Serial adapter, go into Device Manger, and then Scroll down to Ports
 (Com & LPT) and Double-click on "Prolific USB-to-Serial Comm Port (COM#)", or in some cases, "Other Device". Click on "Update Driver' and then choose to locate the driver on your computer. Click on "Browse" and direct Windows to the folder "C:\PL2303Win32"
 where the files are located and click OK. Windows will copy the files directly into the Driver Store for the system to use. Next, unplug your Prolific adapter and plug it in again and the driver will be installed from the Driver Store to Windows/System32/Drivers
 and your device should be working properly.
Key words: Sabrent, SBT-USC1M, Prolific, PL-2303, PL2303, USB to Serial, VID_067B, PID_2303, Code 10 
