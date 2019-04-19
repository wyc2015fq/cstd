# 如何打Android的补丁包------how to patch android - xqhrs232的专栏 - CSDN博客
2012年02月03日 22:22:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1885
How to fast apply R10.3.2 patches
Here I take frameworks/base.git
as an example, you can follow the same method
for other gits.
 Assume you had unzipped R10.3.2 release package to
/opt/imx-android-r10.3.2/.
//===========================how to patch many pacths one time==============
$ cd /opt/imx-android-r10.3.2/code/r10.3.2/platform/frameworks/base.git
$ ls *.patch
> series
$ cd ~/myandroid/frameworks/base
$ git quiltimport --patches
/opt/imx-android-r10.3.2/code/r10.3.2/platform/frameworks/base.git
If error happened when applying some patch, please use git apply to patch it separately, to
do that follow the next example,
using frameworks/base
//===========================how to patch  a  pacth one time==============
$cd ~/myandroid/frameworks/base
$git apply /opt/imx-android-r10.3.2/code/r10.3.2/platform/frameworks/base.git/0001-ENGR00133115-Support-MMS-streaming-playback-in-OMX-G.patch
Repeat the last command with each patch under the folder.
