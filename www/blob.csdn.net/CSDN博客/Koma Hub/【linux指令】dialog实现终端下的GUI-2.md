# 【linux指令】dialog实现终端下的GUI-2 - Koma Hub - CSDN博客
2019年01月10日 22:46:37[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：47
个人分类：[GUI																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/7517387)
**目录**
[fselect](#fselect)
[buildlist](#buildlist)
[checklist](#checklist)
[dselect](#dselect)
[editbox](#editbox)
[form](#form)
[gauge](#gauge)
[infobox](#infobox)
[inputbox](#inputbox)
[inputmenu](#inputmenu)
[mixedform](#mixedform)
[mixedgauge-未完待续](#mixedgauge-%E6%9C%AA%E5%AE%8C%E5%BE%85%E7%BB%AD)
[pause](#pause)
# fselect
```bash
#!/bin/bash
tmp=`mktemp tmpXXX.out`
dialog --fselect $PWD 20 50 2>$tmp
clear
path=`cat $tmp | awk '{print $1}'`
echo $path
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190110210646610.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# buildlist
```bash
#!/bin/bash
tmp=`mktemp tmpXXX.out`
dialog --buildlist "buildlist" 20 50 50 \
        1 "item1" "status1" \
        2 "item2" "status2" \
        3 "item3" "status3" \
        2>$tmp
path=`cat $tmp`
clear
echo $path
```
![](https://img-blog.csdnimg.cn/2019011021144983.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# checklist
```bash
#!/bin/bash
tmp=`mktemp tmpXXX.out`
dialog --checklist "buildlist" 20 50 50 \
        1 "item1" "status1" \
        2 "item2" "status2" \
        3 "item3" "status3" \
        2>$tmp
path=`cat $tmp`
clear
echo $path
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/2019011021190844.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# dselect
```bash
#!/bin/bash
tmp=`mktemp tmpXXX.out`
dialog --title "dir select" --dselect $PWD 20 50 2>$tmp
path=`cat $tmp`
clear
echo $path
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190110212248458.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# editbox
```bash
#!/bin/bash
if [ $# -lt 1 ]; then
        echo "Usage: $0 <filename>"
        exit
fi
dialog --title "edit text" --editbox $1 20 50 2> `mktemp tmpXXXX`
clear
```
![](https://img-blog.csdnimg.cn/20190110213323797.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# form
```bash
#!/bin/bash
tmp=$(mktemp tmpXXXX)
# --form text height width formheight [ label y x item y x flen ilen ] ...
dialog --form "LTE-long term evolution" 25 50 20 \
        "    ip:" 1  1  "127.0.0.1"  1  10  15  20 \
        "  user:" 2  1  "rongtao  "  2  10  15  20 \
        "   grp:" 3  1  "sylincom "  3  10  15  10 \
        2>$tmp
string=`cat $tmp`
clear
echo $string
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190110215939178.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# gauge
```bash
#!/bin/bash
for ((i=0;i<=100;i+=4))
do
        echo $i | dialog --gauge "percent" 6 30 0
        sleep 0.5
done
clear
```
![](https://img-blog.csdnimg.cn/20190110221051270.PNG)
# infobox
```bash
dialog --title "title" --infobox "information" 10 30
```
![](https://img-blog.csdnimg.cn/20190110221237685.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# inputbox
```bash
dialog --inputbox "text" 10 20 "rongtao" 2>inputbox.txt
```
![](https://img-blog.csdnimg.cn/20190110221539285.PNG)
# inputmenu
```bash
#!/bin/bash
tmp=`mktemp tmpXXXXX`
dialog --inputmenu "Input Menu" 20 30 25 \
        1 "item1" \
        2 "item2" \
        3 "item3" \
        5 "item5" \
        2>$tmp
clear
echo `cat $tmp`
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190110222437820.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# mixedform
```bash
#!/bin/bash
tmp=$(mktemp tmpXXXX)
# --mixedform text height width formheight [ label y x item y x flen ilen itype ] ...
# 1    hidden, e.g., a password field.
# 2    readonly, e.g., a label
dialog --mixedform "LTE-long term evolution" 25 50 20 \
        "    ip:" 1  1  "127.0.0.1"  1  10  15  20  2\
        "  user:" 2  1  "rongtao  "  2  10  15  20  1\
        "   grp:" 3  1  "sylincom "  3  10  15  10  0\
        2>$tmp
string=`cat $tmp`
clear
echo $string
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/2019011022281070.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# mixedgauge-未完待续
```bash
#!/bin/bash
for ((i=0;i<=100;i+=4))
do
        dialog --mixedgauge "percent" 20 30 $i \
                                                40 20 \
                                                60 40 \
                                                80 60 \
                                                90 70 \
                                                91 71 \
                                                94 74 \
                                                "99" 78 \
                                                2>> tmp.txt
        sleep 0.5
done
clear
```
![](https://img-blog.csdnimg.cn/20190110224215225.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# pause
```bash
#--pause text height width seconds
 dialog --pause "wait a second" 9 30 8
```
![](https://img-blog.csdnimg.cn/20190110224516697.PNG)
