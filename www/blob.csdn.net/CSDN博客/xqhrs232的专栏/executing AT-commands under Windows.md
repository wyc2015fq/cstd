# executing AT-commands under Windows - xqhrs232的专栏 - CSDN博客
2011年12月25日 08:41:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：940标签：[terminal																[windows																[application																[manager																[mobile																[command](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=mobile&t=blog)](https://so.csdn.net/so/search/s.do?q=manager&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=terminal&t=blog)
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://3g-modem.wetpaint.com/page/executing+AT-commands+under+Windows](http://3g-modem.wetpaint.com/page/executing+AT-commands+under+Windows)
how to execute AT-commands under Windows
In order to execute AT-commands, you first have to install drivers for your 3G device. During driver installation several specific virtual interfaces will be created, which will appear under Windows as serial COM ports or as a modem.
After drivers have been installed, you need to find out the required COM port in Windows device manager, as described below:
- Open the Windows device manager
- Windows XP:
- Click **Start**, click **Control Panel**, click **Performance and Maintenance**, and then click **System**. 
- On the **Hardware** tab, click **Device Manager**. Double-click the type of device you want to change.
- Windows Vista:
- Click **Start**, click **Control Panel**, click **System and Maintenance**, and then click **Device Manager**.‌ If you are prompted for an administrator password or confirmation, type the password or provide confirmation.
- In Windows device manager you will find the required port depending on your modem's manufacturer:
- Huawei devices: 
- expand the "Modems" section 
- right-click the item "HUAWEI Mobile Connect - 3G modem" (or similar) and select "properties"
- click the "modem" tab 
- There you should see the "port", which is the required port.
- Sierra Wireless devices:
- expand the "Ports (COM & LPT)" section 
- There you should see an entry called "Sierra Wireless AT Commnd Port (UMTS) (COM ?)", where ? is the required port.
- ZTE devices:
- expand the "Modems" section 
- right-click the item "ZTE Proprietary USB Modem" (or similar) and select "properties"
- click the "modem" tab 
- There you should see the "port", which is the required port.
- Now that you know the port, where to address your AT-commands to, you just need a terminal application by which you can send those commands to your device. The best choice is perhaps[PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html),
 a universal terminal program also capable of Telnet and SSH, on which the following instructions will base. So first download putty by right-clicking[this
 link](http://the.earth.li/~sgtatham/putty/latest/x86/putty.exe) and saving it to some folder on your harddisk, where you will find it later again (perhaps just on the desktop).
- After having downloaded putty.exe, make sure all other applications (especially dashboards like Huawei's Mobile Partner, Vodafone Mobile Connect, MWconn or MDMA), that may access your device are completely closed, because otherwise such applications will
 occupy the required COM port. 
- If you are sure, that no other application, that may access your device, is running, start PuTTY by doubleclicking putty.exe
- Not the PuTTY Configuration appears:
- select "Serial" as connection type 
- under "Serial line" enter the COM port, which you have looked up earlier (e.g. COM6)
- click on "Open" to establish the terminal connection to your device
![configure putty for executing AT-commands on a serial port](http://image.wetpaint.com/image/2/UTzPk6Es8z1JpwANCZ9k-w18293/GW456H436)
- Now you will see the following empty black window:
![PuTTY terminal window](http://image.wetpaint.com/image/2/g4_X153QRsGHPuTTPfiX7Q8210/GW666H413)
- To check if everything is working type **ATZ** (= resetting the device)and press [enter] and your modem should answer with "**OK**"
 (depending on if command echo is enabled or not, you might not see what you are typing, so don't wonder if the screen remains unchanged while typing).
Afterwards try the command **ATI**[enter], which will output information on your device:
![PuTTY output after AT-command ATI](http://image.wetpaint.com/image/2/emfkAa3D9s-GKEpjnMJL9g12534/GW666H413)
- If you have come so far, you are ready for entering just any AT-command you desire to.
