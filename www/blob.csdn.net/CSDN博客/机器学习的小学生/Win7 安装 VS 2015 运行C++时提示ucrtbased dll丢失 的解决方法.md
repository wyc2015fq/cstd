
# Win7 安装 VS 2015 运行C++时提示ucrtbased.dll丢失 的解决方法 - 机器学习的小学生 - CSDN博客


2017年09月06日 09:44:19[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：4962


This problem is from VS 2015 silently failing to copy ucrtbased.dll (debug) and ucrtbase.dll (release) into the appropriate system folders during the installation of Visual Studio. (Or you did not select “Common Tools for Visual C++ 2015” during installation.) This is why reinstalling may help. However, reinstalling is an extreme measure… this can be fixed without a complete reinstall.
First, if you don’t really care about the underlying problem and just want to get this one project working quickly, then here is a fast solution: just copy ucrtbased.dll from C:\Program Files (x86)\Windows Kits\10\bin\x86\ucrt\ucrtbased.dll (for 32bit debug) into your application’s \debug directory alongside the executable. Then it WILL be found and the error will go away. But, this will only work for this one project.
A more permanent solution is to get ucrtbased.dll and ucrtbase.dll into the correct system folders. Now we could start copying these files into \Windows\System32 and \SysWOW64, and it might fix the problem. However, this isn’t the best solution. There was a reason this failed in the first place, and forcing the use of specific .dll’s this way could cause problems.
The best solution is to open up the control panel –> Programs and Features –> Microsoft Visual Studio 2015 –> Modify. Then uncheck and re-check “Visual C++ –> Common Tools for Visual C++ 2015”. Click Next, then and click Update, and after a few minutes, it should be working.
If it still doesn’t work, run the modify tool again, uncheck the “Common Tools for Visual C++ 2015”, and apply to uninstall that component. Then run again, check it, and apply to reinstall. Make sure anti-virus is disabled, no other tasks are open, etc. and it should work. This is the best way to ensure that these files are copied exactly where they should be.
Note that if the modify tool gives an error code at this point, then the problem is almost certainly specific to your system. Research the error code to find what is going wrong and hopefully, how to fix it.
参考文献：
[https://stackoverflow.com/questions/33743493/why-visual-studio-2015-cant-run-exe-file-ucrtbased-dll](https://stackoverflow.com/questions/33743493/why-visual-studio-2015-cant-run-exe-file-ucrtbased-dll)

