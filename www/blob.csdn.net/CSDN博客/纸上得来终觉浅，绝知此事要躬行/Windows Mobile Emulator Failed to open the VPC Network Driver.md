# Windows Mobile Emulator:Failed to open the VPC Network Driver. - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年07月15日 13:11:51[boonya](https://me.csdn.net/boonya)阅读数：1901








Failed to open the VPC Network Driver. Verify that the driver is installed, or install the driver from the
 Web download location at [http://go.microsoft.com/fwlink/?linkid=46859](http://go.microsoft.com/fwlink/?linkid=46859)

#### [方法一](http://go.microsoft.com/fwlink/?linkid=46859)

![](https://img-blog.csdn.net/20150715131938787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


[Windows 7](http://www.microsoft.com/windows/windows-7/) (as of RC1) now supports apretty spiffy new feature known as*Virtual Windows XP* or*Windows XPMode* based on what article you read.  At its core, it is a new versionof the[Virtual PC](http://www.microsoft.com/windows/virtual-pc/default.aspx) product, now named WindowsVirtual PC, installed closer to the
 OS, combined with a Windows XP hard diskimage which allow you to run legacy applications under Windows XP on yourWindows 7 machine in a pretty seamless way.

If you install theWindows Virtual PC add-on to Windows 7, you must first uninstall Virtual PC2007.  By doing that, you remove the special network driver that is alsorequired to use the Microsoft Device Emulator on the network.  The deviceemulator is typically
 used for Smart Device development.

So the puzzle: The Device Emulator doesn’t work with the new Windows Virtual PC driver, andyou can’t install the old version of Virtual PC 2007 and expect the new WindowsVirtual PC to keep working.  What to do?

The answer is toextract the Virtual PC 2007 network service drivers out of the MSI package andmanually install them in Windows 7.  So far this has provided the expectedresult for me:  Windows Virtual PC runs happily, and the Device Emulatornow connects to
 the network.  So how do we do this?  Thanks forasking.  Here is a step-by-step guide to make this go.  Note thatthis is very much a “works on my machine” experience.  If it burns yourhouse down, don’t hold me responsible.  Note that you should have a validand
 complete backup before attempting this on your machine just in case…
- Download the [Virtual PC 2007 SP1](http://www.microsoft.com/downloads/details.aspx?familyid=28C97D22-6EB8-4A09-A7F7-F6C7A1F000B5&displaylang=en) installer executable 
- Rename the installer from **setup.exe** to something else, such as**vpc.exe**
- Open a **cmd** prompt, navigate to the setup executable, and run (assuming you renamed the file to vpc.exe):
- **vpc.exe /c**

- This will extract the executable to an MSI file at located in the **%temp%** directory
- Change to the temp directory by running:
- **cd %temp%**

- In this directory, extract the MSI by running:
- **msiexec /a Virtual_PC_2007_Install.msi /qb TARGETDIR=c:\vpc**
- Of course, feel free to replace that directory with one of your choosing 

- This will extract the MSI file to **c:\vpc** (or the one you entered)
- Open the **Network and Sharing Center** and select **Change adapter settings**
- **![](https://img-blog.csdn.net/20150715131645147?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
- Right-click the network adapter you have connected to the internet and select**Properties**
- Click the **Install…** button 

- ![](https://img-blog.csdn.net/20150715131823158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

- Make sure **Service **is selected and click **Add…**
- **![](https://img-blog.csdn.net/20150715131844086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
- Click **Have Disk…**
- ![](https://img-blog.csdn.net/20150715131907665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://brianpeek.com/post/image.axd?picture=VirtualWindowsXPVirtualPC2007SP1NetworkD_48F3/disk.png)
- For x86 machines, navigate to:
- **C:\vpc\Program Files\Microsoft Virtual PC\Utility\VMNetSrv\**

- For x64 machines, navigate to:
- **C:\vpc\Program Files\Microsoft Virtual PC\Utility\VMNetSrv\x64**

- Select **Virtual Machine Network Services** and click **OK**
- **![](https://img-blog.csdn.net/20150715131928109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

At this point the driverwill be installed and attached to that network adapter.  If you have theneed to attach this to multiple adapters, simply repeat the steps above toinstall the service on each adapter required.

Once the driver isinstalled, you can then link the Device Emulator to the chosen adapter andconnect to the network as always, as well as connect to the network in the newversion of Virtual PC.

Let me know how itworks for you!


#### 方法二

下载：[http://download.csdn.net/detail/zhangjikuan/7895791](http://download.csdn.net/detail/zhangjikuan/7895791)

下载完成后解压文件，配置如方法一中图片所示一致。

#### 设置网络访问

这里不再详述过程，需要记住一点网络选择与本机的网络设置一致，并且模拟器上面设置为“单位设置”。最后看下效果：

![](https://img-blog.csdn.net/20150715132920411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20150715132933860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




