# 【linux指令】dialog实现终端下的GUI-3 - Koma Hub - CSDN博客
2019年01月12日 15:16:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：59
个人分类：[GUI																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/7517387)
**目录**
[prgbox](#prgbox)
[progressbox、progressbox](#progressbox%E3%80%81progressbox)
[radiolist](#radiolist)
[rangebox](#rangebox)
[tailbox、tailboxbg](#tailbox%E3%80%81tailboxbg)
[textbox](#textbox)
[timebox](#timebox)
[treeview](#treeview)
[yesno](#yesno)
# prgbox
```bash
$ dialog --prgbox "prgbox" uname 10 50; clear
```
```
--prgbox text command height width
--prgbox command height width
A prgbox is very similar to a programbox.
This dialog box is used to display the output of a command that is specified
as an argument to prgbox.
After the command completes, the user can press the ENTER key so that dialog
will exit and the calling shell script can continue its operation.
If three parameters are given, it displays the text under the title, delin‐
eated from the scrolling file's contents.  If only two parameters are given,
this text is omitted.
```
![](https://img-blog.csdnimg.cn/20190112142040747.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# progressbox、progressbox
```bash
$ dialog --programbox "programbox" 10 50 ; clear
```
![](https://img-blog.csdnimg.cn/20190112142755950.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# radiolist
```bash
#!/bin/bash
tmp=`mktemp tmpXXX.out`
 # press space to select
dialog --radiolist "radiolist" 20 50 50 \
        1 "item1" on  \
        2 "item2" off \
        3 "item3" off \
        2>$tmp
path=`cat $tmp`
clear
echo $path
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190112143718240.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# rangebox
```bash
#!/bin/bash
# --rangebox     <text> <height> <width> <min-value> <max-value> <default-value>
tmp=`mktemp tmpXXXX.out`
dialog --rangebox "rangebox" 10 50 0 100 49 2>$tmp
clear
cat $tmp
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190112144546971.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# tailbox、tailboxbg
```bash
$ dialog --tailbox rangebox.sh 30 50; clear
```
![](https://img-blog.csdnimg.cn/20190112144835219.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# textbox
```bash
$ dialog --textbox tmp.out 20 50; clear
```
![](https://img-blog.csdnimg.cn/20190112145141435.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# timebox
```bash
#!/bin/sh
tmp=`mktemp tmpXXX.out`
#--timebox      <text> <height> <width> <hour> <minute> <second>
dialog --timebox "timebox" 5 30 23 59 59 2>$tmp
clear
cat $tmp
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190112145626166.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# treeview
```bash
#!/bin/bash
tmp=`mktemp tmpXXX.out`
#--treeview     <text> <height> <width> <list-height> <tag1> <item1> <status1> <depth1>...
dialog --treeview "treeview" 20 50 45 1 "item1" on 3 \
        2 "item2" "status2" 2 \
        2>$tmp
clear
cat $tmp
rm -f $tmp
```
![](https://img-blog.csdnimg.cn/20190112150338979.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
# yesno
```bash
$ dialog --yesno "Are you ok?" 5 20 ; clear
```
![](https://img-blog.csdnimg.cn/20190112151610627.PNG)
```
--yesno text height width
909-              A yes/no dialog box of size height rows by width columns will be  displayed.
910-              The  string  specified  by text is displayed inside the dialog box.  If this
911-              string is too long to fit in one line, it will be automatically divided into
912-              multiple  lines at appropriate places.  The text string can also contain the
913-              sub-string "\n" or newline characters `\n' to control line breaking explic‐
914-              itly.   This dialog box is useful for asking questions that require the user
915-              to answer either yes or no.  The dialog box has a Yes button and a No  but‐
916-              ton, in which the user can switch between by pressing the TAB key.
917-
918-              On  exit,  no  text is written to dialog's output.  In addition to the "Yes"
919-              and "No" exit codes (see DIAGNOSTICS) an ESC exit status may be returned.
920-
921-              The codes used for "Yes" and "No" match those used for  "OK"  and  "Cancel",
922-              internally no distinction is made.
```
