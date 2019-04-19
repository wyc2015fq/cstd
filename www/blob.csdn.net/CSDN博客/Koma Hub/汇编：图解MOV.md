# 汇编：图解MOV - Koma Hub - CSDN博客
2019年03月21日 22:14:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：58
《Art Of Intel x86 Assembly》
**Table of Contents**
[MOV AL, DS:[8088h] 、MOV DS:[1234h], DL](#MOV%20AL%2C%20DS%3A%5B8088h%5D%20%E3%80%81MOV%20DS%3A%5B1234h%5D%2C%20DL)
[MOV AX, DS:[1234h]](#MOV%20AX%2C%20DS%3A%5B1234h%5D)
[MOV AL, [BX]](#MOV%20AL%2C%20%5BBX%5D)
[MOV AL, [BP]](#MOV%20AL%2C%20%5BBP%5D)
[MOV AL, [BX+disp]、MOV AL, [BP+disp]](#MOV%20AL%2C%20%5BBX%2Bdisp%5D%E3%80%81MOV%20AL%2C%20%5BBP%2Bdisp%5D)
[MOV AL, [BX+SI]、MOV AL, [BP+SI]](#MOV%20AL%2C%20%5BBX%2BSI%5D%E3%80%81MOV%20AL%2C%20%5BBP%2BSI%5D)
[MOV AL, [BX+SI+disp]、MOV AL, [BP+SI+disp]](#MOV%20AL%2C%20%5BBX%2BSI%2Bdisp%5D%E3%80%81MOV%20AL%2C%20%5BBP%2BSI%2Bdisp%5D)
[DISP](#DISP)
# MOV AL, DS:[8088h] 、MOV DS:[1234h], DL
![](https://img-blog.csdnimg.cn/20190321220554332.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# MOV AX, DS:[1234h]
![](https://img-blog.csdnimg.cn/20190321220709704.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# MOV AL, [BX]
![](https://img-blog.csdnimg.cn/20190321220735886.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# MOV AL, [BP]
![](https://img-blog.csdnimg.cn/2019032122081834.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# MOV AL, [BX+disp]、MOV AL, [BP+disp]
![](https://img-blog.csdnimg.cn/2019032122092712.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# MOV AL, [BX+SI]、MOV AL, [BP+SI]
![](https://img-blog.csdnimg.cn/20190321221035982.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# MOV AL, [BX+SI+disp]、MOV AL, [BP+SI+disp]
![](https://img-blog.csdnimg.cn/20190321221101430.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# DISP
![](https://img-blog.csdnimg.cn/2019032122114424.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
