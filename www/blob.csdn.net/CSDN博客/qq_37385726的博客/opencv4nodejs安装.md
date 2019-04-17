# opencv4nodejs安装 - qq_37385726的博客 - CSDN博客





2018年05月25日 11:22:57[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：4930








**1.根据网站[如何安装opencv4nodejs](https://github.com/justadudewhohacks/opencv4nodejs#how-to-install)可以看到我们需要如下几个步骤**

![](https://img-blog.csdn.net/20180525111052599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**2.按照步骤**

**·安装cmake并配置环境变量**

**·npm install --global windows-build-tools**

**·npm install --save opencv4nodejs**



**出现了第一个问题**



> 
opencv-build@0.0.11 install C:\WINDOWS\system32\node_modules\opencv-build
node ./install.js


info install if you want to use an own OpenCV installation set OPENCV4NODEJS_DISABLE_AUTOBUILD
info install running install script...
ERR! Error: Command failed: git --version
'git' is not recognized as an internal or external command,
operable program or batch file.

npm WARN enoent ENOENT: no such file or directory, open 'C:\WINDOWS\system32\package.json'
npm WARN system32 No description
npm WARN system32 No repository field.
npm WARN system32 No README data
npm WARN system32 No license field.

npm ERR! code ELIFECYCLE
npm ERR! errno 1
npm ERR! opencv-build@0.0.11 install: `node ./install.js`
npm ERR! Exit status 1
npm ERR!
npm ERR! Failed at the opencv-build@0.0.11 install script.
npm ERR! This is probably not a problem with npm. There is likely additional logging output above.

npm ERR! A complete log of this run can be found in:
npm ERR! C:\Users\jerem\AppData\Roaming\npm-cache_logs\2018-04-04T12_10_53_353Z-debug.log


**##解决方式##**

**![](https://img-blog.csdn.net/20180525112115432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**安装git并配置环境变量**







**3.安装好git后再次**npm install --save opencv4nodejs****



**出现了第二个问题**


> opencv-build@0.0.12 install C:\Users\Administrator\node_modules\opencv-build
> node ./install.js


info install if you want to use an own OpenCV installation set OPENCV4NODEJS_DIS
ABLE_AUTOBUILD
info install running install script...
info install using msbuild: { path: 'C:\\Program Files (x86)\\Microsoft Visual S
tudio\\2017\\Community\\MSBuild\\15.0\\Bin\\MSBuild.exe',
info install   version: 15 }
Cloning into 'opencv_contrib'...
remote: Counting objects: 25853, done.
remote: Compressing objects: 100% (30/30), done.
error: RPC failed; curl 18 transfer closed with outstanding read data remaining
fatal: The remote end hung up unexpectedly
fatal: early EOF
fatal: index-pack failed
ERR! child process exited with code 128
npm WARN Administrator@1.0.0 No description
npm WARN Administrator@1.0.0 No repository field.


npm ERR! code ELIFECYCLE
npm ERR! errno 1
npm ERR! opencv-build@0.0.12 install: `node ./install.js`
npm ERR! Exit status 1
npm ERR!
npm ERR! Failed at the opencv-build@0.0.12 install script.
npm ERR! This is probably not a problem with npm. There is likely additional log
ging output above.


npm ERR! A complete log of this run can be found in:
npm ERR!     C:\Users\Administrator\AppData\Roaming\npm-cache\_logs\2018-05-25T0


2_48_37_479Z-debug.log







**#解决方式#**

![](https://img-blog.csdn.net/20180525211322476?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**是因为网络问题**



**综上：**

**安装opencv4nodejs仅需**



**·安装cmake并配置环境变量**

**·安装git并配置环境变量**

**·npm install --global windows-build-tools**

**·npm install --save opencv4nodejs**




