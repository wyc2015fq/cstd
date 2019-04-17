# the panorama mosaix(拼接) - 家家的专栏 - CSDN博客





2010年11月17日 19:06:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：955








The steps required to complete the panorama are listed below:
||Step|EXE|
|----|----|----|
|1.|Take pictures on a tripod (or handheld)||
|2.|Warp to spherical coordinates|(Panorama.exe)|
|3.|Extract features|(Features.exe)|
|4.|Match features|(Features.exe)|
|5.|Align neighboring pairs using RANSAC|(Panorama.exe)|
|6.|Write out list of neighboring translations|(Panorama.exe)|
|7.|Correct for drift|(Panorama.exe)|
|8.|Read in warped images and blend them|(Panorama.exe)|
|9.|Crop the result and import into a viewer| |



