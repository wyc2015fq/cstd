# 【virtualbox】 your system is running in low graphics mode - 三少GG - CSDN博客
2013年06月10日 16:17:44[三少GG](https://me.csdn.net/scut1135)阅读数：1736
问题：
The system is running in low-graphics mode  Your screen, graphics card and input device settings could not be detected correctly. You will need to configure these yourself.
解决办法：[http://askubuntu.com/questions/141606/how-to-fix-the-system-is-running-in-low-graphics-mode-error](http://askubuntu.com/questions/141606/how-to-fix-the-system-is-running-in-low-graphics-mode-error)
This happens when a driver has a problem installing correctly (Most cases). For this do the following:
- 
Boot PC leaving SHIFT pressed to make the GRUB Menu show.
- 
Select **Recovery Mode** which will continue booting correctly until the Recovery Menu appeares.
- 
Select from the recovery menu **failsafeX**.
- 
In some cases failsafeX will load fine (You lucky dog), **for others (Me) it will give  an error along the lines of "The system is running in low-graphics mode" and will stay there forever. When this happens, pressCTRL+ALT+F1 to go to the terminal. Type in your Username and Password.**
- 
Reinstall the drivers depending on your case:
**NVIDIA**
`**sudo apt-get install nvidia-current**` - More stable/tested version**`sudo apt-get install nvidia-current-updates`**- More up-to-date
 version
For other cases see [this answer](http://askubuntu.com/questions/86977/how-to-correctly-enable-desktop-cube-in-unity-3d?lq=1) for details and follow the links there to help you along the way.
**ATI**
The simple way is to `sudo apt-get install fglrx`. If this does not work keep reading.
Go to [http://support.amd.com/us/gpudownload/Pages/index.aspx](http://support.amd.com/us/gpudownload/Pages/index.aspx), download the one you need, put it in some folder (rename it to "ati.run" to simplify name). Go to the folder where you downloaded
 the file and type `chmod +x ati.run` to give it Executable Permission. Now just simply`./sh ati.run` and follow the onscreen steps.
After rebooting all problems should be solved. If you test 'Additional Drivers' with a problem like this it will finish downloading the package but then it will give an error. It also gives the same error if you use 'Software Center' and 'Synaptic'. The
 only way was to go to the `failsafeX` option and do the workaround about changing to the`tty1` terminal and doing it via command line.
I also need to add that I do not recommend downloading the Drivers from the Nvidia site since they:
- Might create additional problems with Ubuntu
- Are not updated automatically
- Are not tested thoroughly in Ubuntu
Always use the `nvidia-current` package or the `nvidia-current-updates` one. This are tested and approved already for the Ubuntu version you are using and will give less errors and incompatibility bugs.
##        Intel
          For Intel it is recommended to do the following after doing all the steps mentioned above but before installing anything (When you are in the Terminal). You can choose  from the Xorg-Edgers which is an Unstable PPA or the X-Swat which is more stable:
Warning: This PPA is very unstable for some things. So do it with that in mind.
`sudo add-apt-repository ppa:xorg-edgers/ppa -y`
or X-Swat:
`sudo add-apt-repository ppa:ubuntu-x-swat/x-updates -y`
After that `sudo apt-get update` and you should receive several updates. X-Swat currently does not have Intel drivers in the latest versions of Ubuntu.
**UPDATE**: Added this extensive answer to solve many of the problems that might end with the error mentioned here:[Which
 driver should I install when using an Nvidia, Ati or Intel video card](http://askubuntu.com/questions/61396/which-driver-should-i-install-when-using-an-nvidia-ati-or-intel-video-card/61433#61433)
Like always please test and give feedback so I can enhance my answer since others will be also reading it. The better it is, the more people it will help.
